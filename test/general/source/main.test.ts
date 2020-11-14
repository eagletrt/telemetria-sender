import canMessages from './suites/can_messages/can_messages.test';
import messagesIds from './suites/messages_ids/messages_ids.test';

describe('EagleTRT Telemetria General Tests', async function() {
    this.timeout(0);
    await canMessages();
    await messagesIds();
});