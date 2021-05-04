export function wait(milliseconds: number): Promise<void> {
    return new Promise(resolve => { setTimeout(resolve, milliseconds); });
}
export async function paraCulo(callback: () => Promise<void>): Promise<void> {
    try {
        await callback();
    } catch (error) { }
}

export function randRange(min: number, max: number): number {
    return min + Math.floor(Math.random() * (max - min));
}
export function randWithDeviation(base: number, deviation: number): number {
    return base + (Math.ceil(Math.random() * 2 * deviation) - deviation)
}