import { expect } from 'chai';
import { MongoClient } from 'mongodb';
import { deserialize } from 'bson';
import { AsyncClient as MqttClient, connectAsync } from 'async-mqtt';
import { clean } from 'mongo-cleaner';
import { CanSimulatorInstance, GpsSimulatorInstance, simulateCan, simulateGps, virtualizeCan } from '@eagletrt/eagletrt-telemetria-simulator';
import { wait } from '../../utils/misc';
import { startTelemetry, TelemetryProcessInstance } from '../../utils/telemetry';
import getConfiguration from '../../utils/config';

const config = getConfiguration();

let
    canSimulatorInstance: CanSimulatorInstance,
    gpsSimulatorInstance: GpsSimulatorInstance,
    telemetryProcessInstance: TelemetryProcessInstance,
    mongoConnection: MongoClient,
    mqttClient: MqttClient,
    mqttData: any;

export default async function () {
    describe('Test if timestamps are sent and saved correctly', async function () {
        this.timeout(0);

        beforeEach(async function () {
            // Set telemetry config
            config.set({
                'mongodb.db': 'telemetry_test',
                'mongodb.collection': 'chimera_test',
            });

            // Connect to mqtt
            mqttData = [];
            mqttClient = await connectAsync(`mqtt://${config.data.mqtt.host}:${config.data.mqtt.port}`);
            await mqttClient.subscribe(config.data.mqtt.data_topic);
            mqttClient.on('message', (topic, message: Buffer) => {
                if (topic === config.data.mqtt.data_topic) {
                    mqttData.push(deserialize(message));
                }
            });

            // Connect to mongodb
            const mongoUri = `mongodb://${config.data.mongodb.host}:${config.data.mongodb.port}`;
            await clean(mongoUri, undefined, { keep: database => database !== config.data.mongodb.db });
            mongoConnection = await MongoClient.connect(mongoUri, { useUnifiedTopology: true });

            // Virtualize can if not already virtualized
            await virtualizeCan(config.data.can_interface);
            // Simulate gps and get its interface
            gpsSimulatorInstance = await simulateGps();
            const gpsInterface = await gpsSimulatorInstance.getGpsInterface();


            // Set telemetry config
            config.set({
                'gps.plugged': 1,
                'gps.simulated': 1,
                'gps.interface': gpsInterface
            });

            // Start telemetry
            telemetryProcessInstance = await startTelemetry(config.path);

            // Simulate can
            await wait(1000);
            canSimulatorInstance = await simulateCan();
        })

        it('Should send all documents timestamps incrementals on mqtt', async function () {
            await wait(4 * config.data.sending_rate);
            await telemetryProcessInstance.stop();
            const timestamps: number[] = mqttData.map(el => el.timestamp);
            
            expect(timestamps).not.to.be.empty;

            let lastValue = -1;
            timestamps.forEach(timestamp => {
                expect(timestamp).to.be.a('number');
                expect(timestamp).to.be.greaterThan(lastValue);
                lastValue = timestamp;
            });
        });

        it('Should send all documents timestamps with attendible values on mqtt', async function () {
            await wait(4 * config.data.sending_rate);
            await telemetryProcessInstance.stop();
            const timestamps: number[] = mqttData.map(el => el.timestamp);
            
            expect(timestamps).not.to.be.empty;

            const mezzora = 30 * 60 * 1000;
            const from = Date.now() - mezzora;
            const to = Date.now() + mezzora;
            timestamps.forEach(timestamp => {
                expect(timestamp).to.be.a('number');
                expect(timestamp).to.be.within(from, to);
            });
        });

        it('Should send all messages timestamps with attendible values on mqtt', async function () {
            await wait(4 * config.data.sending_rate);
            await telemetryProcessInstance.stop();

            const mezzora = 30 * 60 * 1000;
            const from = Date.now() - mezzora;
            const to = Date.now() + mezzora;
            function analyzeObject(obj: any): void {
                for (const key in obj) {
                    if (Array.isArray(obj[key])) {
                        const messages = obj[key];
                        messages.forEach(message => {
                            expect(message.timestamp).to.be.a('number');
                            expect(message.timestamp).to.be.within(from, to);
                        });
                    }
                    else if (typeof obj[key] === 'object') {
                        analyzeObject(obj[key]);
                    }
                }
            }
            
            for (const document of mqttData) {
                analyzeObject(document);
            }
        });

        it('Should save all documents timestamps incrementals on mongodb', async function () {
            telemetryProcessInstance.enable();
            await wait(config.data.sending_rate);
            await wait(5 * config.data.sending_rate);
            await telemetryProcessInstance.stop();

            const collection = mongoConnection.db(config.data.mongodb.db).collection(config.data.mongodb.collection);
            const timestamps: number[] = (await collection.find({ id: { $gt: 1 } }).toArray()).map(el => el.timestamp);

            expect(timestamps).not.to.be.empty;

            let lastValue = -1;
            timestamps.forEach(timestamp => {
                expect(timestamp).to.be.a('number');
                expect(timestamp).to.be.greaterThan(lastValue);
                lastValue = timestamp;
            });
        });

        it('Should save all documents timestamps with attendible values on mongodb', async function () {
            telemetryProcessInstance.enable();
            await wait(config.data.sending_rate);
            await wait(5 * config.data.sending_rate);
            await telemetryProcessInstance.stop();
            
            const collection = mongoConnection.db(config.data.mongodb.db).collection(config.data.mongodb.collection);
            const timestamps: number[] = (await collection.find({ id: { $gt: 1 } }).toArray()).map(el => el.timestamp);
            
            expect(timestamps).not.to.be.empty;

            const mezzora = 30 * 60 * 1000;
            const from = Date.now() - mezzora;
            const to = Date.now() + mezzora;
            timestamps.forEach(timestamp => {
                expect(timestamp).to.be.a('number');
                expect(timestamp).to.be.within(from, to);
            });
        });

        it('Should save all messages timestamps with attendible values on mongodb', async function () {
            telemetryProcessInstance.enable();
            await wait(config.data.sending_rate);
            await wait(5 * config.data.sending_rate);
            await telemetryProcessInstance.stop();

            const mezzora = 30 * 60 * 1000;
            const from = Date.now() - mezzora;
            const to = Date.now() + mezzora;
            function analyzeObject(obj: any): void {
                for (const key in obj) {
                    if (Array.isArray(obj[key])) {
                        const messages = obj[key];
                        messages.forEach(message => {
                            expect(message.timestamp).to.be.a('number');
                            expect(message.timestamp).to.be.within(from, to);
                        });
                    }
                    else if (typeof obj[key] === 'object') {
                        analyzeObject(obj[key]);
                    }
                }
            }

            const collection = mongoConnection.db(config.data.mongodb.db).collection(config.data.mongodb.collection);
            const documents = (await collection.find({ id: { $gt: 1 } }).toArray()).map(el => el.timestamp);

            expect(documents).not.to.be.empty;
            
            for (const document of documents) {
                analyzeObject(document);
            }
        });

        afterEach(async function () {
            await gpsSimulatorInstance.stop();
            await canSimulatorInstance.stop();
            await mongoConnection.close();
            await mqttClient.end();
        });
    });
}