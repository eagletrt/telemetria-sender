import { ChildProcessWithoutNullStreams, exec, ExecOptions, spawn } from 'child_process';
import * as path from 'path';
import { delay, execAsync, killProcess } from './test-utils';

export default class Simulator {
    private readonly REPO_URL = 'https://github.com/eagletrt/eagletrt-telemetry-simulator';
    private readonly WORKING_PATH = './test/general';
    private readonly SIMULATOR_FOLDER = 'eagletrt-telemetry-simulator';
    private readonly SIMULATOR_PATH = path.join(this.WORKING_PATH, this.SIMULATOR_FOLDER);
    private readonly COMPILE_SCRIPT_NAME = 'compile.sh';
    private readonly RUN_SCRIPT_NAME = 'run.sh';
    private readonly MOSQUITTO_PORT = 1883;
    private readonly ENABLE_COMMAND = './enable_signal.sh';
    private readonly IDLE_COMMAND = './idle_signal.sh';
    private readonly INTERFACE_PATH = '/dev/pts';

    private simulatorProcess: ChildProcessWithoutNullStreams | null = null;
    private mosquittoProcess: ChildProcessWithoutNullStreams | null = null;
    private interfacePath = "/dev/pts/2";

    constructor(private log = false) {
    }

    public async start(): Promise<boolean> {
        let result = true;
        const originalInterface = await this.getInterfaceWIthLs();

        await execAsync(`git clone ${this.REPO_URL}`, { cwd: this.WORKING_PATH });
        await execAsync(`./${this.COMPILE_SCRIPT_NAME}`, { cwd: this.SIMULATOR_PATH });

        this.simulatorProcess = spawn(`./${this.RUN_SCRIPT_NAME}`, { cwd: this.SIMULATOR_PATH });
        this.simulatorProcess.stdout.on('data', (data) => this.logWrapper(data.toString()));
        this.simulatorProcess.stderr.on('data', (data) => this.logWrapper(data.toString()));

        this.mosquittoProcess = spawn('mosquitto', ['-p', `${this.MOSQUITTO_PORT}`]);
        this.mosquittoProcess.stdout.on('data', (data) => this.logWrapper(data.toString()));
        this.mosquittoProcess.stderr.on('data', (data) => this.logWrapper(data.toString()));

        await delay(2000);

        const newInterface = await this.getInterfaceWIthLs();
        let diff = newInterface.filter(x => !originalInterface.includes(x));
        if (diff.length > 0) {
            this.interfacePath = path.join(this.INTERFACE_PATH, `${diff[0]}`);
        } else {
            result = false;
        }
        if (result) {
            console.log('SIMULATOR and MOSQUITTO started')
        } else {
            console.log('Error starting SIMULATOR and MOSQUITTO')
        }
        return result;
    }

    public async stop() {
        killProcess(this.simulatorProcess);
        killProcess(this.mosquittoProcess);
        await execAsync(`rm -r ${this.SIMULATOR_FOLDER}`, { cwd: this.WORKING_PATH });

        await delay(2000);
        console.log('SIMULATOR and MOSQUITTO stopped')
    }

    public async enable() {
        exec(this.ENABLE_COMMAND);
    }

    public async idle() {
        exec(this.IDLE_COMMAND);
    }

    public getInterfacePath(): string {
        return this.interfacePath;
    }

    private logWrapper(data) {
        if (this.log) {
            console.log(data);
        }
    }

    private async getInterfaceWIthLs(): Promise<string[]> {
        return (await execAsync(`ls ${this.INTERFACE_PATH}`)).stdout.split('\n');

    }
}