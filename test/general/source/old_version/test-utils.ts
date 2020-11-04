import { ChildProcessWithoutNullStreams, exec, ExecException, ExecOptions } from "child_process";

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


export async function delay(ms: number) {
    await new Promise((resolve) => { setTimeout(resolve, ms); });
}