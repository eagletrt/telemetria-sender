import messagesParsing from './suites/messages_parsing/messages_parsing';
import messagesIds from './suites/messages_ids/messages_ids.test';
import messagesTimestamps from './suites/messagess_timestamps/messagess_timestamps.test';
import enableIdle from './suites/enable_idle/enable_idle.test';

describe('EagleTRT Telemetria General Tests', async function() {
    this.timeout(0);
    await messagesParsing();
    await messagesIds();
    await messagesTimestamps();
    await enableIdle();
});