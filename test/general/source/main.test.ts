import canMessages from './suites/can_messages/can_messages.test';

describe('EagleTRT Telemetria General Tests', async function() {
    this.timeout(0);
    await canMessages();
});