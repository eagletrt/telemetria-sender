import { clean } from 'mongo-cleaner';
import * as path from 'path';
import { runTests, getMongoUri } from "../../utils/misc"
import { MongoClient } from 'mongodb';
import * as assert from 'assert';

const MONGO_CONFIG = {
    host: 'localhost',
    port: 27017,
    db: 'eagle_test'
}

export default async function () {
    describe('mongo_services', function () {

        describe('mongo_start_session', function () {

            beforeEach(async function () {
                await clean(getMongoUri(MONGO_CONFIG.host, MONGO_CONFIG.port), undefined, { keep: database => database !== MONGO_CONFIG.db });
            });

            runTests(
                path.join(__dirname, 'mongo_start_session.test.out'),
                [{
                    it: 'should inset start session record in chimera',
                    args: [ MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'chimera', 'pilotone', 'circuitone'],

                }, {
                    it: 'should inset start session record in random',
                    args: [ MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'random', 'pilot_one', 'circuit_one' ],

                }],
                async (prop) => {
                    const mongoConnection = await MongoClient.connect(getMongoUri(prop.args[0], +prop.args[1]), { useUnifiedTopology: true });
                    const collection = mongoConnection.db(prop.args[2]).collection(prop.args[3]);
                    let coll = await collection.aggregate().toArray();
                    await mongoConnection.close();
                    assert(coll.length === 1);
                    assert(coll[0].pilot === prop.args[4]);
                    assert(coll[0].race === prop.args[5]);
                }
            );
        });





    });
}