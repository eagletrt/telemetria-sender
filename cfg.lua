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

config_pub = {
  broker_host = "localhost",
  broker_port = 1883,
  mqtt_topic = "test/log",
  plugin_path = "install/bin/dummy.plugin"
}

-- Derived entries
config_sub.mongo_uri = "mongodb://"..config_sub.mongo_host..":"..config_sub.mongo_port

