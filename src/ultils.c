/**********************************************************************
 * utils.c - general pourpose utilities                               *
 **********************************************************************/
#include "utils.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>


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



int load_config(config_t *cfg, char const *config_file) {
  lua_State *lua;
  lua = luaL_newstate();
  luaL_openlibs(lua);
  if (luaL_dofile(lua, config_file) == 1) {
    fprintf(stderr, "Lua error running %s:\n%s\n", config_file,
            lua_tostring(lua, -1));
    return EXIT_FAILURE;
  }
  // TODO: fill cfg struct with fields in config_file
  lua_close(lua);
  return EXIT_SUCCESS;
}