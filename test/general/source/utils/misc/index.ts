export function wait(milliseconds: number): Promise<void> {
    return new Promise(resolve => { setTimeout(resolve, milliseconds); });
}
export async function paraCulo(callback: () => Promise<void>): Promise<void> {
    try {
        await callback();
    } catch(error) {}
}