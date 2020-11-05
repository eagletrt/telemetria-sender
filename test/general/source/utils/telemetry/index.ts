import { ChildProcess } from 'child_process';
import * as path from 'path';
import { exec } from 'shelljs';

export class TelemetryProcess {
    private static PROCESS_CWD =  path.join(process.cwd(), '..');
    private telemetryProcess: ChildProcess | null = null;

    public start() {
        this.telemetryProcess = exec(`./sender.out ${this.configPath}`, {
            async: true,
            cwd: TelemetryProcess.PROCESS_CWD
        });
        console.log('telemetry id', this.telemetryProcess.pid)
    }

    public enable() {
        exec('npm run enable', {
            async: true,
            cwd: TelemetryProcess.PROCESS_CWD,
            silent: true
        });
    }

    public disable() {
        exec('npm run idle', {
            async: true,
            cwd: TelemetryProcess.PROCESS_CWD
        });
    }

    public stop() {
        this.telemetryProcess?.kill('SIGABRT');
    }

    constructor(private configPath: string) { }
}