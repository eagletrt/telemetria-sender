import { ChildProcessWithoutNullStreams, exec, ExecException, ExecOptions, spawn } from "child_process";
import { wait } from "../../utils/misc";

export function execAsync(cmd: string, options: ExecOptions = {}): Promise<{ stdout: string, stderr: string }> {
    return new Promise((resolve, reject) => {
        exec(cmd, options, (error, stdout, stderr) => {
            if (error) {
                reject(error);
            } else {
                resolve({ stdout: stdout, stderr: stderr });
            }
        });
    });
}

export function killProcess(process: ChildProcessWithoutNullStreams | null) {
    if (process)
        process!.kill('SIGINT');
}

let canplayerProcess;
let teleProcess;

export async function startCanplayer(path: string) {
    console.log('Starting canplaer')
    await wait(2000);
    canplayerProcess = await execAsync(`canplayer -I ${path}`, { cwd: './general' });
    console.log('Started canplayer');
}
export function stopCanplayer() {
    killProcess(canplayerProcess);
}

export async function startTelemetry(configPath: string) {
    console.log('Starting tele')
    execAsync(`./sender.out ${configPath}`, { cwd: '../' });
    console.log('Started tele')
    await wait(3000);
    console.log('Enabling tele')
    execAsync(`npm run enable`, { cwd: '../' });
    console.log('Enabled tele')
    await wait(2000);
}
export function stopTelemetry() {
    killProcess(teleProcess);
}