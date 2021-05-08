import messagesParsing from './suites/messages_parsing/messages_parsing.test';
import messagesIds from './suites/messages_ids/messages_ids.test';
import messagesTimestamps from './suites/messages_timestamps/messages_timestamps.test';
import enableIdle from './suites/enable_idle/enable_idle.test';

describe('EagleTRT Telemetria General Tests', async function() {
    this.timeout(0);
    await messagesParsing();
    await messagesIds();
    await messagesTimestamps();
    await enableIdle();
});