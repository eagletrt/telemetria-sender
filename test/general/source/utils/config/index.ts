import * as fs from 'fs';
import * as path from 'path';
import { TelemetryConfig } from '../../types';

class TelemetryConfiguration {

    public readonly path: string;
    public readonly data: TelemetryConfig;

    private changeObject(extendedKey: string, value: any): any {
        const keys = extendedKey.split('.');
        let reachedObject = this.data;
        for (let i = 0; i < keys.length - 1; i++) {
            reachedObject = reachedObject[keys[i]];
        }
        reachedObject[keys[keys.length - 1]] = value;
    }

    public set(changes: any): void {
        for (const key in changes) {
            this.changeObject(key, changes[key]);
        }
        fs.writeFileSync(this.path, JSON.stringify(this.data, null, 4));
    }

    constructor() {
        this.path = path.join(process.cwd(), 'general', 'config.json');
        const textContent = fs.readFileSync(this.path, 'utf-8');
        this.data = JSON.parse(textContent);
    }

}

let configuration: TelemetryConfiguration | null = null;

export default function (): TelemetryConfiguration {

    if (!configuration) {
        configuration = new TelemetryConfiguration();
    }
    return configuration;

}