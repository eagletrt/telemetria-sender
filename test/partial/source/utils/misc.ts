import * as child_process from 'child_process'

interface TestProperty {
    it: string;
    args: string[];
    [key: string]: any
}
interface TestPropertyResult extends TestProperty {
    result: number
}

function runCTest(path: string, args: string[]): number {
    let res = child_process.execSync(`${path} ${args.join(' ')}`).toString();
    return +res;
}

export function getMongoUri(host: string, port: number) {
    return `mongodb://${host}:${port}`
}

export function runTests(
    cTestPath: string,
    testsProperties: TestProperty[],
    callback: (properities: TestPropertyResult) => Promise<void> = (async (p) => {}),
    beforeC: (properities: TestProperty) => Promise<void> = (async (p) => {})
) {
    for (const p of testsProperties) {
        it(p.it, async function() {
            await beforeC(p);
            console.log
            p.result = runCTest(cTestPath, p.args);
            await callback(p as TestPropertyResult);
        });
    }
}

export function runTestsWithDone(
    cTestPath: string,
    testsProperties: TestProperty[],
    callback: (properities: TestPropertyResult, done: Mocha.Done) => void = ((p, d) => {}),
    beforeC: (properities: TestProperty, done: Mocha.Done) => void = ((p, d) => {})
) {
    for (const p of testsProperties) {
        it(p.it, function(done) {
            this.timeout(5000);
            beforeC(p, done);
            p.result = runCTest(cTestPath, p.args);
            callback(p as TestPropertyResult, done);
        });
    }
}

export function stringifyCJSON(obj: any) {
    return JSON.stringify(obj).replace(/\"/g, '\\"');
}
export function parseCJSON(s: string) {
    return JSON.parse(s.replace(/\\\"/g, '"'));
}