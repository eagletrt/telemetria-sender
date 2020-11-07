import * as dree from 'dree';
import * as fs from 'fs';
import { expect } from 'chai';
import { MongoClient } from 'mongodb';
import { clean } from 'mongo-cleaner';
import { join } from 'path';
import { CanSimulatorInstance, GpsSimulatorInstance, simulateCan, simulateGps, virtualizeCan } from '@eagletrt/eagletrt-telemetria-simulator';

import getConfiguration from '../../utils/config';
import { wait } from '../../utils/misc';
import { startTelemetry, TelemetryProcessInstance } from '../../utils/telemetry';

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

    const expectedJsonName = `${name}.expected.json`;
    const expectedJsonPath = join(path, expectedJsonName);

    const settingsPath = join(path, 'settings.json');
    const settings = require(settingsPath);

    let 
        canSimulatorInstance: CanSimulatorInstance, 
        gpsSimulatorInstance: GpsSimulatorInstance, 
        telemetryProcessInstane: TelemetryProcessInstance,
        mongoConnection: MongoClient;

    describe(`Test ${name} message`, function () {

        beforeEach(async function () {
            
            // Connect to mongodb
            const mongoUri = `mongodb://${config.data.mongodb.host}:${config.data.mongodb.port}`;
            await clean(mongoUri, undefined, { keep: database => database !== config.data.mongodb.db });
            mongoConnection = await MongoClient.connect(mongoUri, { useUnifiedTopology: true } );

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
            telemetryProcessInstane = await startTelemetry(config.path);
            // Enable telemetry
            await wait(500);
            telemetryProcessInstane.enable();
            
            // Simulate can
            await wait(1000);
            canSimulatorInstance = await simulateCan(canLogPath, {
                iterations: 1
            });

            // Wait for a certain amount of time
            await wait(settings.time);
            await telemetryProcessInstane.stop();
        });

        it(`Should parse the messages in ${canLogName} and save them in mongodb as in ${expectedJsonName}`, async function () {
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
                ]).toArray()).map(el => el.value);

                expect(values).to.deep.equal(expectedValues);
            }

        });

        afterEach(async function () {
            await gpsSimulatorInstance.stop();
            await canSimulatorInstance.stop();
            await mongoConnection.close();
        });

    });
}