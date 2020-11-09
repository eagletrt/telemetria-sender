import * as path from 'path';
import { runTests, runTestsWithDone } from "../../utils/misc"
import * as mqtt from 'async-mqtt';
import { expect } from 'chai';

const MQTT_CONFIG = {
    host: 'localhost',
    port: 1883,
    data_topic: 'telemetria',
    log_topic: 'telemetria_log'
}

export default async function () {
    describe('mosquitto_service', function () {

        let mqttClient: mqtt.AsyncClient | undefined;
        
        before(function(done) {
            this.timeout(10000);
            mqttClient = mqtt.connect(`mqtt://${MQTT_CONFIG.host}:${MQTT_CONFIG.port}`);
            mqttClient.on('connect', function() { done(); });
        });

        describe('mosquitto_setup', function () {
            runTests(
                path.join(__dirname, 'mosquitto_setup.test.out'),
                [{
                    it: 'should set up mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`]
                }],
                async (prop) => {
                }
            );
        });

        after(async function() {
            await mqttClient?.end();
        });

    });
}