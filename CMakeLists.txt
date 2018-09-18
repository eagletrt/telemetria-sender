cmake_minimum_required(VERSION 3.9.0)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

# PRELIMINARY AND COMMON SETUP
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

project(pub VERSION ${GIT_VERSION_TAG} LANGUAGES C)

option(CROSS "Do a cross-compile" off)
if(CROSS)
  message(STATUS "cross-compiling")
  set(CMAKE_TOOLCHAIN_FILE /root/Toolchain-rpi.cmake)
  # If using mongoc:
  set(CMAKE_PREFIX_PATH 
    /usr/arm-linux-gnueabihf/lib/cmake/libmongoc-1.0
    /usr/arm-linux-gnueabihf/lib/cmake/libmongoc-static-1.0
    /usr/arm-linux-gnueabihf/lib/cmake/libbson-1.0
    /usr/arm-linux-gnueabihf/lib/cmake/libbson-static-1.0
  )
  find_package(libmongoc-static-1.0 1.7 REQUIRED)
  message("--   mongoc found version \"${MONGOC_STATIC_VERSION}\"")
  message("--   mongoc include path \"${MONGOC_STATIC_INCLUDE_DIRS}\"")
  message("--   mongoc libraries \"${MONGOC_STATIC_LIBRARIES}\"")
  set(MONGO_LIBS ${MONGOC_STATIC_LIBRARIES})
  set(BASE_LIBS libmosquitto.a pthread dl m ssl crypto rt)
else()
  set(MONGO_LIBS mongoc-1.0 bson-1.0)
  set(BASE_LIBS libmosquitto.a pthread dl m ssl crypto)
  include_directories(include /usr/local/include/libbson-1.0)
  include_directories(include /usr/local/include/libmongoc-1.0)
endif()

# Common setup for all targets:
include_directories(include /usr/local/include) #search for headers in that dir
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
# target_include_directories(my_lib PRIVATE ${MONGOC_STATIC_INCLUDE_DIRS})
# target_link_libraries (my_lib libmosquitto.a ${MONGO_LIBS})
# target_compile_definitions (my_lib PRIVATE ${MONGOC_STATIC_DEFINITIONS})

# Target publisher (simple example)
add_executable(pub ${SOURCE_DIR}/app/pub.c)
target_include_directories (pub PRIVATE ${MONGOC_STATIC_INCLUDE_DIRS})
target_link_libraries (pub ${BASE_LIBS} ${MONGO_LIBS})
target_compile_definitions (pub PRIVATE ${MONGOC_STATIC_DEFINITIONS})

# Target publisher (simple example)
add_executable(sub ${SOURCE_DIR}/app/sub.c)
target_include_directories (sub PRIVATE ${MONGOC_STATIC_INCLUDE_DIRS})
target_link_libraries (sub ${BASE_LIBS} ${MONGO_LIBS})
target_compile_definitions (sub PRIVATE ${MONGOC_STATIC_DEFINITIONS})

# Install setup (directly copy to CMAKE_INSTALL_PREFIX when make install)
set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY}/install)
install(TARGETS pub sub
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)


# Install package generator (creates an installer script)
set(CPACK_GENERATOR "STGZ")
set(CPACK_PACKAGE_VERSION ${GIT_COMMIT_HASH})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME}-${CMAKE_BUILD_TYPE})
include(CPack)
