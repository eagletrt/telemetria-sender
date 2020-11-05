import * as dree from 'dree';
import * as fs from 'fs';
import { expect } from 'chai';
import { MongoClient } from 'mongodb';
import { clean } from 'mongo-cleaner';
import { join } from 'path';
import getConfiguration from '../../utils/config';
import { startCanplayer, startTelemetry, stopCanplayer, stopTelemetry } from './to-be-removed';
import { wait } from '../../utils/misc';
import { TelemetryProcess } from '../../utils/telemetry';
import { CanProcess } from '../../utils/can';

const config = getConfiguration();
const telemetryProcess = new TelemetryProcess(config.path);

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

    let canlogProcess: CanProcess, mongoConnection: MongoClient;

    describe(`Test ${name} message`, function () {

        beforeEach(async function () {
            config.set({
                'gps.plugged': 0
            });

            // stopSimulator = simulateTelemetry({
            //     simulateCan: true,
            //     simulateGPS: false,
            //     can: {
            //         src: canLogPath,
            //         interface: config.data.can_interface,
            //         iterations: 1,
            //         simulateTime: true,
            //         delay: 1000
            //     }
            // });

            const mongoUri = `mongodb://${config.data.mongodb.host}:${config.data.mongodb.port}`;
            await clean(mongoUri, undefined, { keep: database => database !== config.data.mongodb.db });
            mongoConnection = await MongoClient.connect(mongoUri, { useUnifiedTopology: true } );

            canlogProcess = new CanProcess(canLogPath);

            telemetryProcess.start();
            await wait(1000);
            telemetryProcess.enable();
            await wait(1000);
            canlogProcess.start();
            await wait(10000);
        });

        it(`Should parse the messages in ${canLogName} and save them in mongodb as in ${expectedJsonName}`, async function () {
            telemetryProcess.stop();
            const collection = mongoConnection.db(config.data.mongodb.db).collection(config.data.mongodb.collection);
            const property = keys.join('.');
            const values = (await collection.aggregate([
                { $sort: { id: 1 } },
                { $match: { id: { $ne: undefined } } },
                { $project: { [property]: 1 } },
                { $unwind: { path: `$${property}`, preserveNullAndEmptyArrays: false } },
                { $sort: { [`${property}.timestamp`]: 1 } },
                { $project: { value: `$${property}.value` } } 
            ]).toArray()).map(el => el.value);
            const expected = JSON.parse(fs.readFileSync(expectedJsonPath, 'utf-8'));

            expect(values).to.deep.equal(expected);
        });

        afterEach(async function () {
            telemetryProcess.stop();
            canlogProcess.stop();
            await mongoConnection.close();
        });

    });
}