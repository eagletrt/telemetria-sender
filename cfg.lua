-- CONFIG FILE
config_sub = {
  mongo_host = "localhost",
  mongo_port = 27017,
  mongo_db = "test",
  mongo_collection = "logs",
  broker_host = "localhost",
  broker_port = 1883,
  mqtt_topic = "test/+"
}

-- Derived entries
config.mongo_uri = "mongodb://"..config.mongo_host..":"..config.mongo_port

