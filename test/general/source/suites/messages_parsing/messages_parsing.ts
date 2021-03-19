import * as dree from 'dree';
import * as fs from 'fs';
import { expect } from 'chai';
import { MongoClient } from 'mongodb';
import { deserialize } from 'bson';
import { AsyncClient as MqttClient, connectAsync } from 'async-mqtt';
import { clean } from 'mongo-cleaner';
import { join } from 'path';
import { CanSimulatorInstance, GpsSimulatorInstance, simulateCan, simulateGps, virtualizeCan } from '@eagletrt/eagletrt-telemetria-simulator';

import getConfiguration from '../../utils/config';
import { wait, paraCulo } from '../../utils/misc';
import { startTelemetry, TelemetryProcessInstance } from '../../utils/telemetry';
import { getDeepProperty, parseRecords, removeNegativeZeros } from '../../utils/data';
import { timerize } from '../../utils/timerize';

const config = getConfiguration();

export default async function () {
    describe('Test if can messages are correctly parsed, sent and saved', async function() {
        await testFolder('', join(__dirname, 'assets'), []);
    });
}

async function testFolder(name: string, path: string, keys: string[]): Promise<void> {
    describe(`Test ${name} messages group`, async function() {
        dree.scan(path, { depth: 1 }, () => {}, folder => {
            if (folder.path !== path) {
                if (/\.message$/.test(folder.name)) {
                    testMessageFolder(folder.name.split('.message')[0], folder.path, [...keys, folder.name.split('.message')[0]]);
                }
                else {
                    testFolder(folder.name, folder.path, [...keys, folder.name]);
                }
            }
        });
    
    });
}

function testMessageFolder(name: string, path: string, keys: string[]): void {
    const canLogName = `${name}.can.log`;
    const canLogPath = join(path, canLogName);
    const canLogExists = fs.existsSync(canLogPath);

    const gpsLogName = `${name}.gps.log`;
    const gpsLogPath = join(path, gpsLogName);
    const gpsLogExists = fs.existsSync(gpsLogPath);

    const expectedJsonName = `${name}.expected.json`;
    const expectedJsonPath = join(path, expectedJsonName);

    const settingsPath = join(path, 'settings.json');
    const settings = require(settingsPath);

    if (canLogExists) {
        timerize(canLogPath, settingsPath);
    }

    let 
        canSimulatorInstance: CanSimulatorInstance, 
        gpsSimulatorInstance: GpsSimulatorInstance, 
        telemetryProcessInstance: TelemetryProcessInstance,
        mongoConnection: MongoClient,
        mqttClient: MqttClient,
        mqttData: any;

    describe(`Test ${name} message`, function () {

        beforeEach(function (done) {
            const f = async function () {
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
                        const obj = deserialize(message)
                        mqttData.push(removeNegativeZeros(obj));
                    }
                });
                
                // Connect to mongodb
                const mongoUri = `mongodb://${config.data.mongodb.host}:${config.data.mongodb.port}`;
                await clean(mongoUri, undefined, { keep: database => database !== config.data.mongodb.db });
                mongoConnection = await MongoClient.connect(mongoUri, { useUnifiedTopology: true } );

                // Virtualize can if not already virtualized
                await virtualizeCan(config.data.can_interface);

                // Simulate gps and get its interface
                if (gpsLogExists) {
                    gpsSimulatorInstance = await simulateGps(gpsLogPath, { delay: 3000, iterations: 1, keepAlive: true });
                }
                else {
                    gpsSimulatorInstance = await simulateGps();
                }
                const gpsInterface = await gpsSimulatorInstance.getGpsInterface();
                
                // Set telemetry config
                config.set({
                    'gps.plugged': 1,
                    'gps.simulated': 1,
                    'gps.interface': gpsInterface
                });

                // Start telemetry
                telemetryProcessInstance = await startTelemetry(config.path);
                // Enable telemetry
                await wait(700);
                telemetryProcessInstance.enable();
                
                // Simulate can
                await wait(1500);
                if (canLogExists) {
                    canSimulatorInstance = await simulateCan(canLogPath, {
                        iterations: 1
                    });
                }
                else {
                    canSimulatorInstance = await simulateCan();
                }

                // Wait for all the can messages to be sent
                await wait(settings.time);
                await wait(500);

                // Disable the telemetry
                telemetryProcessInstance.disable();
                await wait(1000);

                // Stop the telemetry
                await telemetryProcessInstance.stop();
                await wait(1000);

                done();
            }
            f();
        });

        it(`Should parse the messages in either ${canLogName} or ${gpsLogName} and save them in mongodb as in ${expectedJsonName}`, async function () {
            const expectedDetails = JSON.parse(fs.readFileSync(expectedJsonPath, 'utf-8'));
            for (const expectedDetail of expectedDetails) {
                const message = expectedDetail.message;
                const expectedValues = expectedDetail.values;

                const collection = mongoConnection.db(config.data.mongodb.db).collection(config.data.mongodb.collection);
                const values = (await collection.aggregate([
                    { $sort: { id: 1 } },
                    { $match: { id: { $ne: undefined } } },
                    { $project: { [message]: 1 } },
                    { $unwind: { path: `$${message}`, preserveNullAndEmptyArrays: false } },
                    { $sort: { [`${message}.timestamp`]: 1 } },
                    { $project: { value: `$${message}.value` } } 
                ]).toArray()).map(el => removeNegativeZeros(el.value) );

                expect(values).to.deep.equal(expectedValues);
            }

        });

        it(`Should parse the messages in either ${canLogName} or ${gpsLogName} and send them on mqtt as in ${expectedJsonName}`, async function () {
            const expectedDetails = JSON.parse(fs.readFileSync(expectedJsonPath, 'utf-8'));

            const mqttParsedData = await parseRecords(mqttData);

            for (const expectedDetail of expectedDetails) {
                const message = expectedDetail.message;
                const expectedValues = expectedDetail.values;
                const values = getDeepProperty(mqttParsedData, message).map((el: any) => el.value);

                expect(values).to.deep.equal(expectedValues);
            }

        });

        afterEach(async function () {
            await paraCulo(gpsSimulatorInstance.stop)
            await paraCulo(canSimulatorInstance.stop)
            await paraCulo(mongoConnection.close)
            await paraCulo(mqttClient.end)
        });

    });
}