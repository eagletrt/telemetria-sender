cmake_minimum_required(VERSION 3.9.0)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

# PRELIMINARY AND COMMON SETUP
# If using mongoc:
find_package(libmongoc-static-1.0 1.7 REQUIRED)
message("--   mongoc found version \"${MONGOC_STATIC_VERSION}\"")
message("--   mongoc include path \"${MONGOC_STATIC_INCLUDE_DIRS}\"")
message("--   mongoc libraries \"${MONGOC_STATIC_LIBRARIES}\"")

option(CROSS "Do a cross-compile" off)
if(CROSS)
  message(STATUS "cross-compiling")
  set(CMAKE_TOOLCHAIN_FILE /root/Toolchain-rpi.cmake)
endif()

# GIT versioning
execute_process(
  COMMAND git describe --long --dirty=§
  WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git describe --abbrev=0 --tags
  WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
  OUTPUT_VARIABLE GIT_VERSION_TAG
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

project(cross-template VERSION ${GIT_VERSION_TAG} LANGUAGES C)

# Common setup for all targets:
include_directories(include) #search for headers in that dir
set(SOURCE_DIR src)
file(GLOB LIB_SOURCES "${SOURCE_DIR}/*.c") #put into LIB_SOURCES the globbed list

# Header file containing the version
configure_file(
  ${CMAKE_HOME_DIRECTORY}/include.in/version.h.in
  ${CMAKE_HOME_DIRECTORY}/include/version.h
)

# If using Lua, uncomment the following:
# include(Lua.cmake)

# TARGETS
#########

# Target library
# add_library(my_lib STATIC ${LIB_SOURCES})
# target_include_directories(my_lib PRIVATE "${MONGOC_STATIC_INCLUDE_DIRS}")
# target_link_libraries (cross-template libmosquitto.a "${MONGOC_STATIC_LIBRARIES}")

# Target cross-template (simple example)
add_executable(cross-template ${SOURCE_DIR}/app/main.c)
target_include_directories (cross-template PRIVATE "${MONGOC_STATIC_INCLUDE_DIRS}")
target_link_libraries (cross-template libmosquitto.a "${MONGOC_STATIC_LIBRARIES}")
target_compile_definitions (cross-template PRIVATE "${MONGOC_STATIC_DEFINITIONS}")

# Install setup (directly copy to CMAKE_INSTALL_PREFIX when make install)
set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY}/install)
install(TARGETS cross-template
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)


# Install package generator (creates an installer script)
set(CPACK_GENERATOR "STGZ")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${GIT_VERSION_TAG})
include(CPack)
