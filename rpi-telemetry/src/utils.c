/**********************************************************************
 * utils.c - general pourpose utilities                               *
 **********************************************************************/
#include "utils.h"
#include <time.h>
#include <string.h>
#include <libgen.h>

void print_buffer(FILE * stream, const uint8_t *buf, size_t buflen) {
  
  size_t i, c = 1;
  fprintf(stream, "%04d: ", 0);
  for (i = 0; i < buflen; i++) {
    fprintf(stream, "%02x", buf[i]);
    if ((i + 5) % 4 == 0) { 
      fprintf(stream, " ");
      if ((c++ % 8) == 0) fprintf(stream, "\n%04lu: ", i+1); 
    }
  }
  fprintf(stream, "\n");
}

static int load_config_sub(lua_State *lua, config_t *cfg, char const *config_file) {
  if (lua_getglobal(lua, "config_sub") != LUA_TTABLE) {
    fprintf(stderr, "LUA: missing config_sub table\n");
    return EXIT_FAILURE;
  }

  if (lua_getfield(lua, -1, "mongo_uri") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_sub.mongo_uri\n");
    return EXIT_FAILURE;
  }
  cfg->mongo_uri = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "mongo_db") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_sub.mongo_db\n");
    return EXIT_FAILURE;
  }
  cfg->mongo_db = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);
  
  if (lua_getfield(lua, -1, "mongo_collection") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_sub.mongo_collection\n");
    return EXIT_FAILURE;
  }
  cfg->mongo_collection = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "broker_host") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_sub.broker_host\n");
    return EXIT_FAILURE;
  }
  cfg->broker_host = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "broker_port") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_sub.broker_port\n");
    return EXIT_FAILURE;
  }
  cfg->broker_port = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "mqtt_topic") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_sub.mqtt_topic\n");
    return EXIT_FAILURE;
  }
  cfg->mqtt_topic = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  lua_pop(lua, 1); // config_table

  // lua_close(lua);
  return EXIT_SUCCESS;
}

static int load_config_pub(lua_State *lua, config_t *cfg, char const *config_file) {
  if (lua_getglobal(lua, "config_pub") != LUA_TTABLE) {
    fprintf(stderr, "LUA: missing config_pub table\n");
    return EXIT_FAILURE;
  }

  if (lua_getfield(lua, -1, "broker_host") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_pub.broker_host\n");
    return EXIT_FAILURE;
  }
  cfg->broker_host = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "broker_port") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_pub.broker_port\n");
    return EXIT_FAILURE;
  }
  cfg->broker_port = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "mqtt_topic") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_pub.mqtt_topic\n");
    return EXIT_FAILURE;
  }
  cfg->mqtt_topic = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "plugin_path") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_pub.plugin_path\n");
    return EXIT_FAILURE;
  }
  cfg->plugin_path = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "cache_path") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config_pub.cache_path\n");
    return EXIT_FAILURE;
  }
  cfg->cache_path = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);
  
  lua_pop(lua, 1); // config_table

  // lua_close(lua);
  return EXIT_SUCCESS;
}

static int load_config_send(lua_State *lua, config_send *cfg, char const *config_file) {
  if (lua_getglobal(lua, "config_send") != LUA_TTABLE) {
    fprintf(stderr, "LUA: missing config_send table\n");
    return EXIT_FAILURE;
  }

  if (lua_getfield(lua, -1, "timing") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.timing\n");
    return EXIT_FAILURE;
  }
  cfg->timing = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "mongo_dbheavy_package_turnover") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.heavy_package_turnover\n");
    return EXIT_FAILURE;
  }
  cfg->heavy_package_turnover = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);
  
  if (lua_getfield(lua, -1, "resolver_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.resolver_data\n");
    return EXIT_FAILURE;
  }
  cfg->resolver_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "front_wheels_encoder_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.front_wheels_encoder_data\n");
    return EXIT_FAILURE;
  }
  cfg->front_wheels_encoder_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "imu_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.imu_data\n");
    return EXIT_FAILURE;
  }
  cfg->imu_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "throttle_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.throttle_data\n");
    return EXIT_FAILURE;
  }
  cfg->throttle_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "brake_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.brake_data\n");
    return EXIT_FAILURE;
  }
  cfg->brake_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);  

  if (lua_getfield(lua, -1, "steering_wheel_encoder_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.steering_wheel_encoder_data\n");
    return EXIT_FAILURE;
  }
  cfg->steering_wheel_encoder_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);  

  if (lua_getfield(lua, -1, "gps_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.gps_data\n");
    return EXIT_FAILURE;
  }
  cfg->gps_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  //HEAVY PACKAGE
  
  if (lua_getfield(lua, -1, "bms_hv_temp_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.bms_hv_temp_data\n");
    return EXIT_FAILURE;
  }
  cfg->bms_hv_temp_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);  

  if (lua_getfield(lua, -1, "bms_hv_volt_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.bms_hv_volt_data\n");
    return EXIT_FAILURE;
  }
  cfg->bms_hv_volt_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);  

  if (lua_getfield(lua, -1, "inv_temp_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.inv_temp_data\n");
    return EXIT_FAILURE;
  }
  cfg->inv_temp_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);  

  if (lua_getfield(lua, -1, "inv_volt_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.inv_volt_data\n");
    return EXIT_FAILURE;
  }
  cfg->inv_volt_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);  

  if (lua_getfield(lua, -1, "inv_curr_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.inv_curr_data\n");
    return EXIT_FAILURE;
  }
  cfg->inv_curr_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);  

  if (lua_getfield(lua, -1, "bms_lv_temp_data") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config_send.bms_lv_temp_data\n");
    return EXIT_FAILURE;
  }
  cfg->bms_lv_temp_data = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);


  lua_pop(lua, 1); // config_table

  // lua_close(lua);
  return EXIT_SUCCESS;
}

config_t *new_config(char const *config_file, pubsub_t type)  {
  config_t *cfg = malloc(sizeof(config_t));
  lua_State *lua = cfg->lua;
  lua = luaL_newstate();
  luaL_openlibs(lua);
  if (luaL_dofile(lua, config_file) == 1) {
    fprintf(stderr, "Lua error running %s:\n%s\n", config_file,
            lua_tostring(lua, -1));
    return NULL;
  }
  memset(cfg, 0, sizeof(config_t));
  if (type == CFG_SUB) {
    if (load_config_sub(lua, cfg, config_file) != EXIT_SUCCESS) {
      return NULL;
    }
  }
  else if (type == CFG_PUB) {
    if (load_config_pub(lua, cfg, config_file) != EXIT_SUCCESS) {
      return NULL;
    }
  }
  else {
    fprintf(stderr, "Unsupported type\n");
    return NULL;
  }
  return cfg;
}

config_send *new_config_send(char const *config_file)  {
  config_send *cfg = malloc(sizeof(config_t));
  lua_State *lua = cfg->lua;
  lua = luaL_newstate();
  luaL_openlibs(lua);
  if (luaL_dofile(lua, config_file) == 1) {
    fprintf(stderr, "Lua error running %s:\n%s\n", config_file,
            lua_tostring(lua, -1));
    return NULL;
  }
  memset(cfg, 0, sizeof(config_send));
  if (load_config_send(lua, cfg, config_file) != EXIT_SUCCESS) {
    return NULL;
  }
  return cfg;
}


int can_data_to_bson(can_data_t *can_data, bson_t **bson, char const *plugin_path) {
  struct timespec wall;
  int i;

  if( clock_gettime(CLOCK_REALTIME, &wall) == -1 ) {
    perror("clock gettime");
    exit(EXIT_FAILURE);
  }
    printf("bson ok\n");

  // Timestamp in BSON is expressen in milliseconds after epoch:
  // time.tv_sec * 1000 + time.tv_nsec / 1E6
  *bson = bson_new ();
  BSON_APPEND_DATE_TIME(*bson, "wallclock", (int64_t) (wall.tv_sec * 1000 + wall.tv_nsec / 1E6));
  BSON_APPEND_DATE_TIME(*bson, "car_wallclock", (int64_t) can_data->timestamp);
  BSON_APPEND_DATE_TIME(*bson, "date", time(NULL));

  BSON_APPEND_DOUBLE(*bson, "idx", can_data->id);
  BSON_APPEND_UTF8(*bson, "plugin", basename((char *)plugin_path));
  //append the resolver array
  const char *key;
  char buf[10];

  bson_t array_t;
  BSON_APPEND_ARRAY_BEGIN (*bson, "resolver", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOUBLE (&array_t, key, can_data->resolver[i]);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);

  //append the front wheels encoder array
  BSON_APPEND_ARRAY_BEGIN (*bson, "front_wheels_encoder", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOUBLE (&array_t, key, can_data->front_wheels_encoder[i]);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);
    printf("bson ok 1\n");

  //append the imu data
  BSON_APPEND_ARRAY_BEGIN (*bson, "imu_gyro", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_t elements;    
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOCUMENT_BEGIN (&array_t, key, &elements);
    printf("ok\n");
    printf("%f\n", can_data->imu_gyro[i].x);
    bson_append_double (&elements, "x",1, can_data->imu_gyro[i].x);
    bson_append_double (&elements, "y",1, can_data->imu_gyro[i].y);
    bson_append_double (&elements, "z",1, can_data->imu_gyro[i].z);
    bson_append_document_end (&array_t, &elements);
    bson_destroy(&elements);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);
    printf("bson ok 2\n");

  BSON_APPEND_ARRAY_BEGIN (*bson, "imu_axel", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_t elements;    
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOCUMENT_BEGIN (&array_t, key, &elements);
    bson_append_double (&elements, "x",1, can_data->imu_axel[i].x);
    bson_append_double (&elements, "y",1, can_data->imu_axel[i].y);
    bson_append_double (&elements, "z",1, can_data->imu_axel[i].z);
    bson_append_document_end (&array_t, &elements);
    bson_destroy(&elements);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);
    printf("bson ok 3\n");

  //append the throttle array
  BSON_APPEND_ARRAY_BEGIN (*bson, "throttle", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_INT32 (&array_t, key, can_data->throttle[i]);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);

  //append the brake array
  BSON_APPEND_ARRAY_BEGIN (*bson, "brake", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_INT32 (&array_t, key, can_data->brake[i]);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);

  //append the steering wheel encoder array
  BSON_APPEND_ARRAY_BEGIN (*bson, "steering_wheel_encoder", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOUBLE (&array_t, key, can_data->steering_wheel_encoder[i]);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);

  //append the gps data
  BSON_APPEND_ARRAY_BEGIN (*bson, "gps", &array_t);
  for (i = 0; i < 20; ++i) {
    bson_t elements;    
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOCUMENT_BEGIN (&array_t, key, &elements);
    BSON_APPEND_DOUBLE (&elements, "latitude", can_data->gps[i].latitude);
    BSON_APPEND_DOUBLE (&elements, "longitude", can_data->gps[i].longitude);
    BSON_APPEND_DOUBLE (&elements, "speed", can_data->gps[i].speed);
    BSON_APPEND_DOUBLE (&elements, "altitude", can_data->gps[i].altitude);
    BSON_APPEND_DOUBLE (&elements, "lat_o", can_data->gps[i].lat_o);
    BSON_APPEND_DOUBLE (&elements, "lon_o", can_data->gps[i].lon_o);
    bson_append_document_end (&array_t, &elements);
    bson_destroy(&elements);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);

  //append the marker
  BSON_APPEND_INT32 (*bson, "marker", can_data->marker);

  //append the hv bms data
  BSON_APPEND_ARRAY_BEGIN (*bson, "bms_hv", &array_t);
  for (i = 0; i < 1; ++i) {
    bson_t elements;    
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOCUMENT_BEGIN (&array_t, key, &elements);
    BSON_APPEND_DOUBLE (&elements, "temp", can_data->bms_hv[i].temp);
    BSON_APPEND_DOUBLE (&elements, "volt", can_data->bms_hv[i].volt);
    bson_append_document_end (&array_t, &elements);
    bson_destroy(&elements);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);

  //append the inverter data
  BSON_APPEND_ARRAY_BEGIN (*bson, "inverter", &array_t);
  for (i = 0; i < 1; ++i) {
    bson_t elements;    
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOCUMENT_BEGIN (&array_t, key, &elements);
    BSON_APPEND_DOUBLE (&elements, "temp", can_data->inv[i].temp);
    BSON_APPEND_DOUBLE (&elements, "curr", can_data->inv[i].curr);
    BSON_APPEND_DOUBLE (&elements, "volt", can_data->inv[i].volt);
    bson_append_document_end (&array_t, &elements);
    bson_destroy(&elements);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);
  //append the lv bms data
  
  BSON_APPEND_ARRAY_BEGIN (*bson, "bms_lv", &array_t);
  for (i = 0; i < 1; ++i) {
    bson_t elements;    
    bson_uint32_to_string (i, &key, buf, sizeof buf);
    BSON_APPEND_DOCUMENT_BEGIN (&array_t, key, &elements);
    BSON_APPEND_DOUBLE (&elements, "temp", can_data->bms_lv[i].temp);
    bson_append_document_end (&array_t, &elements);
    bson_destroy(&elements);
  }
  bson_append_array_end (*bson, &array_t);
  bson_destroy(&array_t);


  // TODO: dynamically add a subdocument made of an array of temps
  return EXIT_SUCCESS;
}