import { assert } from "chai";
import { spawn } from "child_process";
import Simulator from './simulator';
import { delay, execAsync, killProcess } from "./test-utils";
import * as fs from 'fs';

describe('General test', function () {
    this.timeout(20000)
    const simulator = new Simulator();

    before(async function () {
        await simulator.start();
    });

    after(async function () {
        await simulator.stop();
    })

    it('ciao', async function () {
        //TODO: set interface and update json
        let config = JSON.parse(fs.readFileSync('./config.json', 'utf-8'));
        let inter = simulator.getInterfacePath();
        if (inter && config.gps !== undefined && typeof config.gps === 'object') {
            config.gps.interface = simulator.getInterfacePath();
            fs.writeFileSync('./config.test.json', JSON.stringify(config));
            console.log(`Using gps interface ${inter}`)
        }

        const senderProcess = spawn('./sender.out', ['config.test.json']);
        senderProcess.stdout.on('data', (data) => {
            // console.log(`${data.toString()}`);
        })
        console.log(" --- START YOUR");
        await delay(2000);
        
        simulator.enable();
        console.log(" --- ENABLED");
        await delay(4000);
        
        simulator.idle();
        console.log(" --- DISABLED");
        await delay(2000);

        killProcess(senderProcess);
        await delay(2000);

        fs.rmSync('config.test.json');

        assert.isTrue(true);
    })
});

