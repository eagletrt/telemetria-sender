/**********************************************************************
 * utils.c - general pourpose utilities                               *
 **********************************************************************/
#include "utils.h"
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

int can_data_to_bson(can_data_t *can_data, bson_t **bson, char const *plugin_path) {
  struct timespec wall;
  bson_t *accu, temp;
  int i;
  if( clock_gettime(CLOCK_REALTIME, &wall) == -1 ) {
    perror("clock gettime");
    exit(EXIT_FAILURE);
  }
  // Timestamp in BSON is expressen in milliseconds after epoch:
  // time.tv_sec * 1000 + time.tv_nsec / 1E6
  *bson = BCON_NEW(
    "wallclock", BCON_DATE_TIME(wall.tv_sec * 1000 + wall.tv_nsec / 1E6),
    "date", BCON_DATE_TIME(can_data->timestamp),
    "time", "[",
      BCON_INT64(wall.tv_sec),
      BCON_INT32(wall.tv_nsec),
    "]",
    "idx", BCON_INT32(can_data->id), 
    "plugin", BCON_UTF8(basename((char *)plugin_path)),
    "location", "{",
      "latitude", BCON_INT32(can_data->location.latitude),
      "longitude", BCON_INT32(can_data->location.longitude),
      "elevation", BCON_DOUBLE(can_data->location.elevation),
    "}",
    "speed", BCON_DOUBLE(can_data->speed),
    "odometry", BCON_DOUBLE(can_data->odometry),
    "steering_angle", BCON_DOUBLE(can_data->steering_angle),
    "throttle", BCON_DOUBLE(can_data->throttle),
    "brake", BCON_DOUBLE(can_data->brake),
    "acceleration", "{",
      "x", BCON_DOUBLE(can_data->acceleration[1].x),
      "y", BCON_DOUBLE(can_data->acceleration[1].y),
      "z", BCON_DOUBLE(can_data->acceleration[1].z),
    "}",
    "gyro", "{",
      "x", BCON_DOUBLE(can_data->gyro[1].x),
      "y", BCON_DOUBLE(can_data->gyro[1].y),
      "z", BCON_DOUBLE(can_data->gyro[1].z),
    "}",
    "magneto", "{",
      "x", BCON_DOUBLE(can_data->magneto[1].x),
      "y", BCON_DOUBLE(can_data->magneto[1].y),
      "z", BCON_DOUBLE(can_data->magneto[1].z),
    "}"
  );
  accu = BCON_NEW(
    "voltage", BCON_DOUBLE(can_data->accumulator.voltage),
    "current", "[",
      BCON_DOUBLE(can_data->accumulator.current[0]),
      BCON_DOUBLE(can_data->accumulator.current[1]),
      BCON_DOUBLE(can_data->accumulator.current[2]),
    "]"
  );
  BSON_APPEND_ARRAY_BEGIN(accu, "temperature", &temp);
  for(i = 0; i < ACCUMULATOR_MODULES; i++) {
    BSON_APPEND_DOUBLE(&temp, "", can_data->accumulator.temperature[i]);
  }
  bson_append_array_end(accu, &temp);
  BCON_APPEND(*bson, 
    "accumulator", BCON_DOCUMENT(accu)
  );
  bson_destroy(accu);
  bson_destroy(&temp);
  // TODO: dynamically add a subdocument made of an array of temps
  return EXIT_SUCCESS;
}