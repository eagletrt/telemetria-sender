import * as fs from 'fs';
import * as path from 'path';
import * as dree from 'dree';
import { setgid } from 'process';

export interface Range {
    from: number;
    to: number;
}
export type Rate = number | Range;

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

function parseFile(path: string, rate: Rate, timerize: boolean, setTotalTime: boolean): number {
    const text = fs.readFileSync(path, 'utf-8');
    const lines = text
        .split('\n')
        .filter(line => !!line);

    let milliseconds = 0;
    
    if (timerize) {
        const parsedFile = lines
            .map(line => {
                milliseconds += getDelta(rate);
                const timestamp = getTimestamp(milliseconds);
                return parseLine(line, timestamp);
            })
            .join('\n');

        fs.writeFileSync(path, parsedFile);
    }
    else if (setTotalTime) {
        const firstLine = lines[0];
        const lastLine = lines[lines.length - 1];
        const handledLines = [firstLine, lastLine];
        const timestamps = handledLines
            .map(line => line.slice(1, 18))
            .map(timestamp => {
                const [seconds, microseconds] = timestamp.split('.');
                return (+seconds * 1000) + (Math.floor(+microseconds / 1000));
            });
        milliseconds = timestamps[1] - timestamps[0];
    }
    
    return milliseconds;
}

export function timerize(canLogPath: string, settingsPath: string) {
    const settings = require(settingsPath);

    const milliseconds = parseFile(canLogPath, settings.rate, settings.timerize, settings.setTotalTime);

    if (settings.setTotalTime) {
        settings.time = milliseconds + 500;
        fs.writeFileSync(settingsPath, JSON.stringify(settings, null, 2));
    }
}