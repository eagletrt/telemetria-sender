import mongoServiceTests from './suites/mongo_services/mongo_services.test'
import mosquittoServiceTests from './suites/mosquitto_service/mosquitto_service.test'
import canServiceTests from './suites/can_service/can_service.test'
import configServiceTests from './suites/config_service/config_service.test'

describe('EagleTRT Telemetria Partial Tests', async function() {
    await mongoServiceTests();
    await mosquittoServiceTests();
    await mosquittoServiceTests();
    await canServiceTests();
    await configServiceTests();
});