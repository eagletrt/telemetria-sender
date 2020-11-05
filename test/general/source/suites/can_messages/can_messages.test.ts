import * as dree from 'dree';
import * as fs from 'fs';
import { expect } from 'chai';
import { MongoClient } from 'mongodb';
import { clean } from 'mongo-cleaner';
import { join } from 'path';
import getConfiguration from '../../utils/config';

const config = getConfiguration();

export default function () {
    describe('Test if can messages are correctly parsed, sent and saved', function() {

        testFolder(join(__dirname, 'assets'), '', []);
    
    });
}

function testFolder(name: string, path: string, keys: string[]): void {
    describe(`Test ${name} messages group`, async function() {

        await dree.scanAsync(path, {}, () => {}, folder => {
            if (/\.message$/.test(folder.name)) {
                testMessageFolder(folder.name, folder.path, [...keys, name]);
            }
            else {
                testFolder(folder.name.split('.message')[0], folder.path, [...keys, name]);
            }
        });
    
    });
}

function testMessageFolder(name: string, path: string, keys: string[]): void {
    const canLogName = `${name}.can.log`;
    const canLogPath = join(path, canLogName);

    const expectedJsonName = `${name}.expected.json`;
    const expectedJsonPath = join(path, expectedJsonName);

    let stopSimulator: () => {}, mongoConnection: MongoClient;

    describe(`Test ${name} message`, function () {

        beforeEach(async function () {
            // config.set({
            //     'gps.plugged': 0
            // });

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
            mongoConnection = await MongoClient.connect(mongoUri);
        });

        it(`Should parse the messages in ${canLogName} and save them in mongodb as in ${expectedJsonName}`, async function () {
            return new Promise((resolve, _reject) => {
                setTimeout(async () => {
                    const collection = mongoConnection.db(config.data.mongodb.db).collection(config.data.mongodb.collection);
                    const property = keys.join('.');
                    const values = await collection.aggregate([
                        { $sort: { id: 1 } },
                        { $match: { $ne: undefined } },
                        { $project: { [property]: 1 } },
                        { $unwind: { path: [`$${property}`], preserveNullAndEmptyArrays: false } },
                        { $sort: { [`${property}.timestamp`]: 1 } },
                        { $project: { value: [`$${property}.value`] } } 
                    ]).toArray();
                    const expected = JSON.parse(fs.readFileSync(expectedJsonPath, 'utf-8'));

                    expect(values).to.deep.equal(expected);
                    resolve();
                }, 3000);
            });
        });

        afterEach(async function () {
            stopSimulator();
        });

    });
}