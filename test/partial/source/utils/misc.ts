import { execSync, exec } from 'child_process'
import * as util from 'util'
const execAsync = util.promisify(exec);

interface TestProperty {
    it: string;
    args: string[];
    [key: string]: any
}
interface TestPropertyResult extends TestProperty {
    result: { stdout: string, stderr: string }
}

export function getMongoUri(host: string, port: number) {
    return `mongodb://${host}:${port}`
}

export function runTests(
    cTestPath: string,
    testsProperties: TestProperty[],
    afterCallback: (properities: TestPropertyResult) => Promise<void> = (async () => {}),
    beforeCallback: (properities: TestProperty) => Promise<void> = (async () => {}),
    togetherCallback: (properities: TestProperty) => Promise<void> = (async () => {})
) {
    for (const p of testsProperties) {
        it(p.it, async function() {
            await beforeCallback(p);
            const testPromise = execAsync(`${cTestPath} ${p.args.join(' ')}`);
            await togetherCallback(p);
            p.result = (await testPromise);
            await afterCallback(p as TestPropertyResult);
        });
    }
}

export function runTestsWithDone(
    cTestPath: string,
    testsProperties: TestProperty[],
    afterCallback: (properities: TestPropertyResult, done: Mocha.Done) => void = (() => {}),
    beforeCallback: (properities: TestProperty, done: Mocha.Done) => void = (() => {})
) {
    for (const p of testsProperties) {
        it(p.it, function(done) {
            beforeCallback(p, done);
            p.result = {
                stdout: execSync(`${cTestPath} ${p.args.join(' ')}`).toString(),
                stderr: undefined
            }
            afterCallback(p as TestPropertyResult, done);
        });
    }
}


export function stringifyCJSON(obj: any) {
    return JSON.stringify(obj).replace(/\"/g, '\\"');
}
export function parseCJSON(s: string) {
    return JSON.parse(s.replace(/\\\"/g, '"'));
}