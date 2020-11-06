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
                    args: ['pilotone', 'circuitone', MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'chimera'],

                }, {
                    it: 'should inset start session record in random',
                    args: ['pilot_one', 'circuit_one', MONGO_CONFIG.host, `${MONGO_CONFIG.port}`, MONGO_CONFIG.db, 'random'],

                }],
                async (prop) => {
                    const mongoConnection = await MongoClient.connect(getMongoUri(prop.args[2], +prop.args[3]), { useUnifiedTopology: true });
                    const collection = mongoConnection.db(prop.args[4]).collection(prop.args[5]);
                    let coll = await collection.aggregate().toArray();
                    mongoConnection.close();
                    assert(coll.length === 1);
                    assert(coll[0].pilot === prop.args[0]);
                    assert(coll[0].race === prop.args[1]);
                }
            );
        });





    });
}