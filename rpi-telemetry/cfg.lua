-- CONFIG FILE
config_sub = {
  mongo_host = "localhost",
  mongo_port = 27017,
  mongo_db = "test",
  mongo_collection = "logs",
  broker_host = "localhost",
  broker_port = 1883,
  mqtt_topic = "test/log"
}

config_pub = {
  broker_host = "192.168.43.46",
  broker_port = 1883,
  mqtt_topic = "test/log",
  plugin_path = "install/bin/dummy.plugin",
  cache_path = "cache.dat"
}

-- Derived entries
config_sub.mongo_uri = "mongodb://"..config_sub.mongo_host..":"..config_sub.mongo_port

-- DATA SENDING CONFIGS
config_send = {
  -- milliseconds passing from a sending to another one.
  timing = 500, 
  -- number of light package sent between an heavy and another one. heavys contain less important data.
  heavy_package_turnover = 10;

  resolver_data = 20,
  front_wheels_encoder_data = 20,
  imu_data = 20,
  throttle_data = 20,
  brake_data = 10,
  steering_wheel_encoder_data = 20,
  gps_data = 20,

  --HEAVY
  bms_hv_temp_data = 1,
  bms_hv_volt_data = 1,
  inv_temp_data = 1,
  inv_volt_data = 1,
  inv_curr_data = 1,
  bms_lv_temp_data = 1,

}
