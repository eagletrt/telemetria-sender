import { ChildProcessWithoutNullStreams, exec, ExecOptions, spawn } from 'child_process';
import { pathToFileURL } from 'url';
import * as path from 'path';
import { resolve } from 'path';

export default class Simulator {
    private readonly REPO_URL = 'https://github.com/eagletrt/eagletrt-telemetry-simulator';
    private readonly WORKING_PATH = './test/general';
    private readonly SIMULATOR_FOLDER = 'eagletrt-telemetry-simulator';
    private readonly SIMULATOR_PATH = path.join(this.WORKING_PATH, this.SIMULATOR_FOLDER);
    private readonly COMPILE_SCRIPT_NAME = 'compile.sh';
    private readonly RUN_SCRIPT_NAME = 'run.sh';
    private readonly MOSQUITTO_PORT = 1883;
    private readonly ENABLE_COMMAND = 'npm run enalble';
    private readonly IDLE_COMMAND = 'npm run idle';

    private simulatorProcess: ChildProcessWithoutNullStreams | null = null;
    private mosquittoProcess: ChildProcessWithoutNullStreams | null = null;

    constructor() {
    }

    public async start() {
        await this.execAsync(`git clone ${this.REPO_URL}`, { cwd: this.WORKING_PATH });
        await this.execAsync(`./${this.COMPILE_SCRIPT_NAME}`, { cwd: this.SIMULATOR_PATH });

        this.simulatorProcess = spawn(`./${this.RUN_SCRIPT_NAME}`, { cwd: this.SIMULATOR_PATH });
        this.simulatorProcess.stdout.on('data', (data) => console.log(data.toString()));
        this.simulatorProcess.stderr.on('data', (data) => console.log(data.toString()));

        this.mosquittoProcess = spawn('mosquitto', ['-p', `${this.MOSQUITTO_PORT}`]);
        this.mosquittoProcess.stdout.on('data', (data) => console.log(data.toString()));
        this.mosquittoProcess.stderr.on('data', (data) => console.log(data.toString()));

        await this.delay(2000);
        console.log('SIMULATOR and MOSQUITTO started')
    }

    public async stop() {
        this.simulatorProcess!.kill('SIGINT');
        this.mosquittoProcess!.kill('SIGINT');
        await this.execAsync(`rm -r ${this.SIMULATOR_FOLDER}`, { cwd: this.WORKING_PATH });

        await this.delay(2000);
        console.log('SIMULATOR and MOSQUITTO stopped')
    }

    public enable() {
        exec(this.ENABLE_COMMAND);
    }

    public idle() {
        exec(this.IDLE_COMMAND);
    }

    private async delay(ms: number) {
        await new Promise((resolve) => { setTimeout(resolve, ms); });
    }

    private async execAsync(cmd: string, options: ExecOptions = {}) {
        return new Promise((resolve, reject) => {
            exec(cmd, options, (err, stdout, stderr) => {
                console.log(err);
                console.log(stdout);
                console.log(stderr);
                resolve();
            });
        });
    }
}