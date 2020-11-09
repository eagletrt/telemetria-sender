import mongoServiceTests from './souites/mongo_services/mongo_services.test'
import mosquittoServiceTests from './souites/mosquitto_service/mosquitto_service.test'

describe('EagleTRT Telemetria Partial Tests', async function() {
    await mongoServiceTests();
    await mosquittoServiceTests();
});