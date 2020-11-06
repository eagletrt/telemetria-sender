import * as path from 'path';
import { runTests } from "../../utils/misc"

export default async function() {
    describe('mongo_services', function() {
        runTests(
            path.join(__dirname, 'mongo_start_session.test.out'),
            [{
                testDescription: 'Testone',
                args: [ 'pilotone', 'circuitone', 'localhost', '27017', 'eagle_test', 'chimera' ],

            }],
            async (prop) => {
                //console.log(prop.result);
            }
        );
    });
}