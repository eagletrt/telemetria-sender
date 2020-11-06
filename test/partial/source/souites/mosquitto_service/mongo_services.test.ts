import * as path from 'path';
import { runTests } from "../../utils/misc"
import * as assert from 'assert';
import { expect } from 'chai';

const MQTT_CONFIG = {
    host: 'localhost',
    port: 1883,
    data_topic: 'telemetria',
    log_topic: 'telemetria_log'
}

export default async function () {
    describe('mosquitto_service', function () {

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

    });
}