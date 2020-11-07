import * as path from 'path';
import { ChildProcess } from 'child_process';
import { exec } from 'shelljs';
import * as treeKill from 'tree-kill';

export class TelemetryProcessInstance {

    private _childprocess: ChildProcess;
    private _finished: boolean;
    private _cwd: string;

    public get childprocess(): ChildProcess {
        return this._childprocess;
    }
    public get finished(): boolean {
        return this._finished;
    }
    public get cwd(): string {
        return this._cwd;
    }

    public enable(): void {
        exec('npm run enable', {
            async: true,
            cwd: this.cwd,
            silent: true
        });
    }

    public disable(): void {
        exec('npm run idle', {
            async: true,
            cwd: this.cwd,
            silent: true
        });
    }

    public async stop(): Promise<void> {
        return new Promise((resolve, reject) => {
            if (this.finished) {
                resolve();
            }
            else {
                this.childprocess.removeAllListeners();
                this.childprocess.on('exit', (code, signal) => {
                    if (signal === 'SIGTERM') {
                        resolve();
                    }
                    else {
                        reject({ code, signal });
                    }
                    this._finished = true;
                });
                this.childprocess.on('error', (code, signal) => {
                    reject({ code, signal });
                });
                treeKill(this.childprocess.pid);
            }
        });
    }

    constructor(childprocess: ChildProcess, cwd: string) {
        this._childprocess = childprocess;
        this._finished = false;
        this._cwd = cwd;

        this.childprocess.on('exit', code => {
            this._finished = true;
        });
    }

}

export async function startTelemetry(configPath: string): Promise<TelemetryProcessInstance> {
    return new Promise<TelemetryProcessInstance>((resolve) => {
        const cwd = path.join(process.cwd(), '..')
        const childProcess = exec(`./sender.out ${configPath}`, {
            async: true,
            cwd: cwd,
            silent: true
        });
        const telemetryProcessInstance = new TelemetryProcessInstance(childProcess, cwd);
        resolve(telemetryProcessInstance);
    });
}