/**********************************************************************
 * utils.c - general pourpose utilities                               *
 **********************************************************************/
#include "utils.h"
#include <string.h>

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
    fprintf(stderr, "LUA: missing config.mongo_uri\n");
    return EXIT_FAILURE;
  }
  cfg->mongo_uri = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "mongo_db") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config.mongo_db\n");
    return EXIT_FAILURE;
  }
  cfg->mongo_db = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);
  
  if (lua_getfield(lua, -1, "mongo_collection") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config.mongo_collection\n");
    return EXIT_FAILURE;
  }
  cfg->mongo_collection = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "broker_host") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config.broker_host\n");
    return EXIT_FAILURE;
  }
  cfg->broker_host = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "broker_port") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config.broker_port\n");
    return EXIT_FAILURE;
  }
  cfg->broker_port = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "mqtt_topic") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config.mqtt_topic\n");
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
    fprintf(stderr, "LUA: missing config.broker_host\n");
    return EXIT_FAILURE;
  }
  cfg->broker_host = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "broker_port") != LUA_TNUMBER) {
    fprintf(stderr, "LUA: missing config.broker_port\n");
    return EXIT_FAILURE;
  }
  cfg->broker_port = luaL_checkinteger(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "mqtt_topic") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config.mqtt_topic\n");
    return EXIT_FAILURE;
  }
  cfg->mqtt_topic = luaL_checkstring(lua, -1);
  lua_pop(lua, 1);

  if (lua_getfield(lua, -1, "plugin_path") != LUA_TSTRING) {
    fprintf(stderr, "LUA: missing config.plugin_path\n");
    return EXIT_FAILURE;
  }
  cfg->plugin_path = luaL_checkstring(lua, -1);
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