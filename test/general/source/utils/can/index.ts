import { ChildProcess } from 'child_process';
import * as path from 'path';
import { exec } from 'shelljs';

export class CanProcess {
    private static PROCESS_CWD =  path.join(process.cwd(), '..');
    private canplayerProcess: ChildProcess | null = null;

    public start() {
        this.canplayerProcess = exec(`canplayer -I ${this.canlogPath}`, {
            async: true,
            cwd: CanProcess.PROCESS_CWD
        });
    }

    public stop() {
        this.canplayerProcess?.kill('SIGINT');
    }

    constructor(private canlogPath: string) { }
}