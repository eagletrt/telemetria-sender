import * as path from 'path';
import { parseCJSON, runTests, runTestsWithDone, stringifyCJSON } from '../../utils/misc'
import * as mqtt from 'mqtt';
import * as bson from 'bson';
import { assert, expect } from 'chai';
import { resolve } from 'path';

const MQTT_CONFIG = {
    host: 'localhost',
    port: 1883,
    data_topic: 'telemetria',
    log_topic: 'telemetria_log'
}

export default async function () {
    describe('mosquitto_service', function () {

        let mqttClient: mqtt.Client | undefined;
        let mqttMessageCallback = (topic: string, messaege: Buffer): void => { }

        before(function (done) {
            this.timeout(10000);
            mqttClient = mqtt.connect(`mqtt://127.0.0.1:1883`);
            mqttClient.on('connect', async function () {
                await new Promise((resolve, reject) => { mqttClient?.subscribe(MQTT_CONFIG.data_topic, resolve); });
                await new Promise((resolve, reject) => { mqttClient?.subscribe(MQTT_CONFIG.log_topic, resolve); });
                done();
            });
            mqttClient?.on('message', function (topic, messaege) {
                mqttMessageCallback(topic, messaege);
            });
        });

        describe('mosquitto_setup', function () {
            runTests(
                path.join(__dirname, 'mosquitto_setup.test.out'),
                [{
                    it: 'should set up mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`]
                }]
            );
        });

        describe('mosquitto_send', function () {
            runTestsWithDone(
                path.join(__dirname, 'mosquitto_send.test.out'),
                [{
                    it: 'should send simple object to mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`, MQTT_CONFIG.data_topic, stringifyCJSON({ pippo: 'goofy' })]
                }, {
                    it: 'should send complex object to mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`, MQTT_CONFIG.data_topic, stringifyCJSON({ pippo: 'goofy', pluto: true, paperino: 3 })]
                }, {
                    it: 'should send object with array to mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`, MQTT_CONFIG.data_topic, stringifyCJSON({ pippo: 'goofy', pluto: ['qui', 'quo', 'qua'], paperino: ['Donald', 20, true] })]
                }, {
                    it: 'should send object with nested object to mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`, MQTT_CONFIG.data_topic, stringifyCJSON({ pippo: 'goofy', paperino: { qui: 1, quo: 'Dewey', qua: false } })]
                }],
                (prop, done) => { },
                (prop, done) => {
                    mqttMessageCallback = (topic, message) => {
                        expect(topic).to.equal(MQTT_CONFIG.data_topic);
                        expect(bson.deserialize(message)).to.deep.equal(parseCJSON(prop.args[3]));
                        done();
                    }
                }
            );
        });

        describe('mosquitto_log', function () {
            runTestsWithDone(
                path.join(__dirname, 'mosquitto_log.test.out'),
                [{
                    it: 'should send simple string to mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`, MQTT_CONFIG.log_topic, 'shortlog']
                },{
                    it: 'should send simple longer to mosquitto',
                    args: [MQTT_CONFIG.host, `${MQTT_CONFIG.port}`, MQTT_CONFIG.log_topic, 'Loremipsumdoloramet,consecteturadipiscingelit.Pellentesquelectuslacus,dictumnecmaximusac,ullamcorpernecleo.Suspendissegravidanecorcieueuismod.Vivamusetenimauctor,malesuadaleoquis,auctordolor.Phasellusacpellentesquedolor,faucibuspretiumlectus.Quisqueetconguedui.Namplaceratliberononmetuseuismodsagittis.']
                }],
                (prop, done) => { },
                (prop, done) => {
                    mqttMessageCallback = (topic, message) => {
                        expect(topic).to.equal(MQTT_CONFIG.log_topic);
                        expect(message.toString()).to.equal(prop.args[3]);
                        done();
                    }
                }
            );
        });

        after(async function () {
            mqttClient?.end();
        });

    });
}