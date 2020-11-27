import { virtualizeCan, VirtualizeCanResult } from '@eagletrt/eagletrt-telemetria-simulator';
import * as path from 'path';
import { runTests } from "../../utils/misc"
import { execSync, exec } from 'child_process'
import { assert } from 'chai';
import * as util from 'util'
const execAsync = util.promisify(exec);

const CAN_CONFIG = {
    can_interface: 'can0'
}

export default async function () {

    describe('can_service', async function () {
        beforeEach(async function () {
            const vRes = await virtualizeCan(CAN_CONFIG.can_interface);
            if (vRes === VirtualizeCanResult.VIRTUALIZED) {
                // console.log('Can virtualized')
            } else if (vRes === VirtualizeCanResult.ALREADY_VIRTUALIZED) {
                // console.log('Can already virtualized')
            }
        });

        describe('can_setup', function () {
            runTests(
                path.join(__dirname, 'can_setup.test.out'),
                [{
                    it: 'should setup the can interface',
                    args: [CAN_CONFIG.can_interface],
                }],
                async (prop) => {
                }
            );
        });

        describe('can_read', function () {
            runTests(
                path.join(__dirname, 'can_read.test.out'),
                [{
                    it: 'should read the can interface with right and left',
                    args: [CAN_CONFIG.can_interface],
                    canMessage: [10, 10, 10]
                }, {
                    it: 'should read the can interface with only left',
                    args: [CAN_CONFIG.can_interface],
                    canMessage: [10, 10]
                }, {
                    it: 'should read the can interface with right and left',
                    args: [CAN_CONFIG.can_interface],
                    canMessage: [200, 1921681002, 123456789]
                }],
                async (prop) => {
                    const res = prop.result.stdout.split(' ');
                    assert(prop.canMessage[0] === parseInt(res[0]), 'Id not valid');
                    assert(prop.canMessage[1] === parseInt(res[1]), 'Left number not valid');
                    if (prop.canMessage.length >= 3) {
                        assert(prop.canMessage[2] === parseInt(res[2]), 'Right number not valid');
                    } else {
                        assert(parseInt(res[2]) === 0)
                    }

                },
                async () => { },
                async (prop) => {
                    const p0 = (prop.canMessage[0] as number).toString(16).padStart(3, '0').toLocaleUpperCase();
                    let p1 = (prop.canMessage[1] as number).toString(16).padStart(8, '0').toLocaleUpperCase();
                    if (prop.canMessage.length >= 3) {
                        p1 += (prop.canMessage[2] as number).toString(16).padStart(8, '0').toLocaleUpperCase();
                    }
                    execSync(`cansend can0 ${p0}#${p1}`);
                }
            );
        });

        describe('can_answer_wheel', function () {
            runTests(
                path.join(__dirname, 'can_answer_wheel.test.out'),
                [{
                    it: 'should answer wheel enable',
                    args: [CAN_CONFIG.can_interface, '0', '0', '0'],
                    expected: ['171', '1694498816']
                },{
                    it: 'should answer wheel disable',
                    args: [CAN_CONFIG.can_interface, '0', '0', '1'],
                    expected: ['171', '1694564352']
                },{
                    it: 'should answer wheel enable with another pilot',
                    args: [CAN_CONFIG.can_interface, '1', '0', '0'],
                    expected: ['171', '1694499072']
                },{
                    it: 'should answer wheel disable with another race',
                    args: [CAN_CONFIG.can_interface, '0', '1', '1'],
                    expected: ['171', '1694564353']
                }],
                async (prop) => {
                    const res = ((await prop['readerPromise']).stdout as string).split(' ');
                    assert.equal(res[0], prop.expected[0]);
                    assert.equal(res[1], prop.expected[1]);
                },
                async (prop) => {
                    prop['readerPromise'] = execAsync(`${path.join(__dirname, 'can_read.test.out')} ${CAN_CONFIG.can_interface}`);

                }
            );
        });

    });
}