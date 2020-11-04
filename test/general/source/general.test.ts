import { assert } from "chai";
import Simulator from './simulator';

describe('General test', function () {
    this.timeout(20000)
    const simulator = new Simulator();

    before(async function () {
        await simulator.start();
    });
    
    after(async function () {
        simulator.stop();
    })

    it('ciao', function () {
        assert.isTrue(true);
    })
});