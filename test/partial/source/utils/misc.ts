import * as child_process from 'child_process'

function runCTest(path: string, args: string[]): number {
    let res = child_process.execSync(`${path} ${args.join(' ')}`).toString();
    return +res;
}

export function getMongoUri(host: string, port: number) {
    return `mongodb://${host}:${port}`
}

export function runTests(
    cTestPath: string,
    testsProperties: { it: string, args: string[], [key: string]: any }[],
    callback: (properities: { it: string, args: string[], result: number, [key: string]: any }) => Promise<void>,
) {
    for (const p of testsProperties) {
        it(p.it, async function() {
            p.result = runCTest(cTestPath, p.args);
            await callback(p as { it: string, args: string[], result: number, [key: string]: any });
        });
    }
}