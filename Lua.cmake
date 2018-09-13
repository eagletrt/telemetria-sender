# Lua support
set(LUA_VERSION "5.3.5")
include(ExternalProject)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
  set(LUA_PLATFORM "macosx")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
  set(LUA_PLATFORM "linux")
else()
  message(FATAL_ERROR "Unsupported platform ${CMAKE_HOST_SYSTEM_NAME}")
endif()
ExternalProject_Add(project_lua
  URL https://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua-${LUA_VERSION}
  BUILD_IN_SOURCE TRUE
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make ${LUA_PLATFORM}
  INSTALL_COMMAND make install INSTALL_TOP=${CMAKE_CURRENT_BINARY_DIR}/lua-${LUA_VERSION}
)
ExternalProject_Get_Property(project_lua INSTALL_DIR)
message(STATUS "Installing lua in ${INSTALL_DIR}")
add_library(lua STATIC IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/liblua.a)
add_dependencies(lua project_lua)