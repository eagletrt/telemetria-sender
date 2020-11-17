import * as path from 'path';
import { runTests } from '../../utils/misc';
import * as fs from 'fs';
import { assert } from 'chai';
const CAN_CONFIG = {
    can_interface: 'can0'
}

export default async function () {

    describe('config_service', async function () {

        describe('handle_config', function () {
            runTests(
                path.join(__dirname, 'handle_config.test.out'),
                [{
                    it: 'should setup the can interface',
                    args: [path.join(__dirname, '../../../source/suites/config_service/assets/config.json')],
                }],
                async (prop) => {
                    let config = JSON.parse(fs.readFileSync(prop.args[0]).toString());
                    prop.result.stdout.split('\n').filter(l => l.length > 0).forEach(l => {
                        let line = l.split(':');
                        let data = JSON.parse(line[1]);
                        let propObj = line[0].split('.');

                        let tempConfig = config;
                        for (const p of propObj) {
                            tempConfig = tempConfig[p]
                        }
                        assert.deepEqual(data, tempConfig)
                    });

                }
            );
        });

    });
}