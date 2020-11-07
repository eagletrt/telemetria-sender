import * as fs from 'fs';
import * as path from 'path';
import * as dree from 'dree';

interface Range {
    from: number;
    to: number;
}
type Rate = number | Range;

function fillZerosPre(n: number, totDigits: number): string {
    const l = `${n}`.length;
    const addedZeros = Array.from(Array(totDigits - l).keys())
        .map(() => 0)
        .join('');
    return addedZeros + n;
}

function getDelta(rate: Rate): number {
    return (typeof rate === 'number')
        ? rate
        : Math.floor((Math.random() * (rate.to - rate.from)) + rate.from);
}

function getTimestamp(milliseconds: number): string {
    const seconds = Math.floor(milliseconds / 1000);
    const microseconds = (milliseconds % 1000) * 1000;
    return fillZerosPre(seconds, 10) + '.' + fillZerosPre(microseconds, 6);
}

function parseLine(line: string, timestamp: string): string {
    return `(${timestamp})` + ((line[0] === '(')
        ? line.slice(19)
        : ' ' + line
    );
}

function parseFile(path: string, rate: Rate): string {
    const text = fs.readFileSync(path, 'utf-8');
    const lines = text
        .split('\n')
        .filter(line => !!line);
    
    let milliseconds = 0;
    return lines
        .map(line => {
            milliseconds += getDelta(rate);
            const timestamp = getTimestamp(milliseconds);
            return parseLine(line, timestamp);
        })
        .join('\n');
}

function main(): void {
    const args = process.argv.slice(2);
    const [dir] = args;

    dree.scan(dir, { matches: /\.message/, extensions: ['log']  }, file => {
        const dirname = file.path.replace(file.name, '');
        const settingsPath = path.join(dirname, 'settings.json');
        const settings = require(settingsPath);
        const parsedFile = parseFile(file.path, settings.rate);
        fs.writeFileSync(file.path, parsedFile);
    });
}
main();