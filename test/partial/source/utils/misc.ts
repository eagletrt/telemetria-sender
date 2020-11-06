import * as child_process from 'child_process'

function runCTest(path: string, args: string[]): number {
    let res = child_process.execSync(`${path} ${args.join(' ')}`).toString();
    return +res;
}

export function runTests(
    cTestPath: string,
    testsProperties: { testDescription: string, args: string[], [key: string]: any }[],
    callback: (properities: { testDescription: string, args: string[], result: number, [key: string]: any }) => Promise<void>,
) {
    for (const p of testsProperties) {
        it(p.testDescription, async function() {
            p.result = runCTest(cTestPath, p.args);
            await callback(p as { testDescription: string, args: string[], result: number, [key: string]: any });
        });
    }
}