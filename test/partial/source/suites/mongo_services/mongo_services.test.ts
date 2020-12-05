import { clean } from 'mongo-cleaner';
import * as path from 'path';
import { runTests, getMongoUri, stringifyCJSON, parseCJSON } from "../../utils/misc"
import { MongoClient } from 'mongodb';
import { expect } from 'chai';

const MONGO_CONFIG = {
    host: 'localhost',
    port: 27017,
    db: 'telemetry_test'
};

export default async function () {
    describe('mongo_services', function () {

        beforeEach(async function () {
            await clean(getMongoUri(MONGO_CONFIG.host, MONGO_CONFIG.port), undefined, { keep: database => database !== MONGO_CONFIG.db });
        });

        describe('mongo_start_session', function () {
            runTests(
                path.join(__dirname, 'mongo_start_session.test.out'),
                [{
                    it: 'should insert start session record in chimera',
                    args: [MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'chimera', 'pilotone', 'tipo_prova', '0.0.0'],
                }, {
                    it: 'should insert start session record in random',
                    args: [MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'random', 'pilot_one', 'tipoProva', '1.0.0'],
                }],
                async (prop) => {
                    const mongoConnection = await MongoClient.connect(getMongoUri(prop.args[0], +prop.args[1]), { useUnifiedTopology: true });
                    const collection = mongoConnection.db(prop.args[2]).collection(prop.args[3]);
                    const coll = await collection.find().toArray();
                    await mongoConnection.close();

                    const expectedSessionNamePattern = `^\\d{8}_\\d{6}_${prop.args[4]}_${prop.args[5]}_v${prop.args[6]}$`;
                    expect(coll).to.have.length(1);
                    expect(coll[0].sessionName).to.match(new RegExp(expectedSessionNamePattern));
                    expect(coll[0].formatted_timestamp).to.match(/^\d{8}_\d{6}$/);
                    expect(coll[0].timestamp).within(Date.now() - 30 * 1000, Date.now() + 30 * 1000);
                    expect(coll[0].pilot).equals(prop.args[4]);
                    expect(coll[0].race).equals(prop.args[5]);
                    expect(coll[0].model_version).equals(prop.args[6]);
                }
            );
        });

        describe('mongo_insert', function () {

            runTests(
                path.join(__dirname, 'mongo_insert.test.out'),
                [
                    {
                        it: 'should insert a simple number json',
                        args: [MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'chimera', stringifyCJSON({ pippo: 1 })],
                    },
                    {
                        it: 'should insert a simple string json',
                        args: [MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'chimera', stringifyCJSON({ pippo: 'goofy' })],
                    },
                    //{
                    //    it: 'should insert a simple complex object',
                    //    args: [MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'chimera', stringifyCJSON({ obj: { with: "properties", inside: 3 } })],
                    //}
                ],
                async (prop) => {
                    const mongoConnection = await MongoClient.connect(getMongoUri(prop.args[0], +prop.args[1]), { useUnifiedTopology: true });
                    const collection = mongoConnection.db(prop.args[2]).collection(prop.args[3]);
                    let coll = await collection.find().toArray();
                    await mongoConnection.close();
                    expect(coll).to.have.length(1);
                    expect(coll[0]).contain(parseCJSON(prop.args[4]));
                }
            );
        });

        describe('mongo_quit', function () {
            runTests(
                path.join(__dirname, 'mongo_quit.test.out'),
                [
                    {
                        it: 'should correctly close the connection',
                        args: [MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'chimera' ],
                    }
                ],
                async (prop) => {
                }
            );
        });

    });
}