import { assert } from "chai";
import { spawn } from "child_process";
import { send } from "process";
import Simulator from './simulator';
import { delay, killProcess } from "./test-utils";

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
        const senderProcess = spawn('./sender.out', ['config.json']);
        senderProcess.stdout.on('data', (data) => {
            console.log(` -- ${data.toString()}`);
        })
        console.log("START YOUR --- ")
        await delay(2000);
        
        simulator.enable();
        console.log("ENABLED --- ")
        await delay(4000);
        
        simulator.idle();
        console.log("DISABLED --- ")
        await delay(2000);

        killProcess(senderProcess);
        await delay(2000);

        assert.isTrue(true);
    })
});

