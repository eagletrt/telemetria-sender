import { deserialize } from 'bson';
import { AsyncClient as MqttClient, connectAsync } from 'async-mqtt';
import { CanSimulatorInstance, simulateCan, virtualizeCan } from '@eagletrt/telemetria-simulator';
import getConfiguration from '../../utils/config';
import { wait, paraCulo, randWithDeviation, randRange } from '../../utils/misc';
import { startTelemetry, TelemetryProcessInstance } from '../../utils/telemetry';
import { expect } from 'chai';


const config = getConfiguration();

export default async function () {
    describe('Test enable idle (and if gives a stallo)', function () {

        let
            canSimulatorInstance: CanSimulatorInstance,
            telemetryProcessInstance: TelemetryProcessInstance,
            mqttClient: MqttClient;

        beforeEach(async function () {
            // Set telemetry config
            config.set({
                'mongodb.db': 'telemetry_test',
                'mongodb.collection': 'chimera_test',
            });

            // Connect to mqtt
            mqttClient = await connectAsync(`mqtt://${config.data.mqtt.host}:${config.data.mqtt.port}`);
            await mqttClient.subscribe(config.data.mqtt.data_topic);

            // Virtualize can if not already virtualized
            await virtualizeCan(config.data.can_interface);

            // Set telemetry config
            config.set({
                'gps.plugged': 0,
                'gps.simulated': 0
            });

            // Start telemetry
            telemetryProcessInstance = await startTelemetry(config.path);
            canSimulatorInstance = await simulateCan();

            await wait(1500);

        });

        it('Should enable and idle the telemetry several times', async function () {
            const iterations = 5;
            this.timeout((iterations * 2 * 1700) + 3000);

            let lastSessionName = '';
            let sessionChanges = 0;
            mqttClient.on('message', (topic, message: Buffer) => {
                if (topic === config.data.mqtt.data_topic) {
                    const obj = deserialize(message)
                    if (obj.sessionName !== lastSessionName) {
                        sessionChanges++;
                        lastSessionName = obj.sessionName;
                    }
                }
            });

            for (let i = 0; i < iterations; i++) {
                telemetryProcessInstance.enable();
                await wait(randWithDeviation(1500, 100));
                telemetryProcessInstance.disable();
                await wait(randWithDeviation(1500, 100));
            }
            await wait(1000);

            expect(sessionChanges).to.equal(iterations);
        });

        it('Should stress test enabling and disabling the telemetry', async function () {
            const iterations = 100;
            this.timeout((iterations * 2 * 200) + 2000);

            let currSessionName = '';
            mqttClient.on('message', (topic, message: Buffer) => {
                if (topic === config.data.mqtt.data_topic) {
                    currSessionName = deserialize(message).sessionName;
                }
            });

            for (let i = 0; i < iterations; i++) {
                telemetryProcessInstance.enable();
                await wait(randRange(50, 150));
                telemetryProcessInstance.disable();
                await wait(randRange(50, 150));
            }
            await wait(1000);
            
            const oldSession = currSessionName;
            telemetryProcessInstance.enable();
            await wait(1500);
            const newSession = currSessionName;

            expect(oldSession).to.not.equal(newSession, 'New session not created');
        });

        afterEach(async function () {
            await paraCulo(async () => { await telemetryProcessInstance.stop() })
            await wait(500);
            await paraCulo(async () => { await canSimulatorInstance.stop() })
            await paraCulo(async () => { await mqttClient.end() })
            await wait(500);
        })

    });
}