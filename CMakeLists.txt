cmake_minimum_required(VERSION 3.9.0)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

                                                        
#  _____                            _____     _           
# |     |___ _____ _____ ___ ___   |   __|___| |_ _ _ ___ 
# |   --| . |     |     | . |   |  |__   | -_|  _| | | . |
# |_____|___|_|_|_|_|_|_|___|_|_|  |_____|___|_| |___|  _|
#                                                    |_|  
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
  message(STATUS "Cross-compiling")
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
  message(STATUS "Host-only compilation, no support for CANbus")
  set(MONGO_LIBS mongoc-1.0 bson-1.0)
  if(APPLE)
    set(BASE_LIBS libmosquitto.a pthread dl m ssl crypto)
    include_directories(
      /usr/local/include/libbson-1.0
      /usr/local/include/libmongoc-1.0
      /usr/local/opt/openssl/include
    )
    link_directories(/usr/local/opt/openssl/lib)
  else()
    set(BASE_LIBS mosquitto pthread dl m ssl crypto)
    include_directories(/usr/include/libbson-1.0 /usr/local/include/libbson-1.0)
    include_directories(/usr/include/libmongoc-1.0 /usr/local/include/libmongoc-1.0)
    link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)
  endif()
endif()

# Common setup for all targets:
include_directories(/usr/local/include) #search for headers in that dir
set(SOURCE_DIR src)
file(GLOB LIB_SOURCES "${SOURCE_DIR}/*.c") #put into LIB_SOURCES the globbed list
file(GLOB LIB_HEADERS "${SOURCE_DIR}/*.h") # headers
if(UNIX AND NOT APPLE)
  set(LINUX_SOURCE_DIR src/linux)
  file(GLOB LINUX_LIB_SOURCES "${LINUX_SOURCE_DIR}/*.c")
  message(STATUS "Linux specific sources added to static lib: ${LINUX_LIB_SOURCES}")
endif()

# Header file containing the version
configure_file(
  ${CMAKE_HOME_DIRECTORY}/src/version.h.in
  ${CMAKE_HOME_DIRECTORY}/src/version.h
)

# If using Lua, uncomment the following:
if(NOT CROSS)
  include(Lua.cmake)
endif()
set(BASE_LIBS lua;${BASE_LIBS})

#  _____                 _       
# |_   _|___ ___ ___ ___| |_ ___ 
#   | | | .'|  _| . | -_|  _|_ -|
#   |_| |__,|_| |_  |___|_| |___|
#               |___|            

set(TARGETS_LIST pub sub dummy)

# Target library
add_library(erpi STATIC ${LIB_SOURCES} ${LINUX_LIB_SOURCES})
target_include_directories(erpi PUBLIC ${MONGOC_STATIC_INCLUDE_DIRS})
if(CROSS)
  target_link_libraries(erpi m ${BASE_LIBS} ${MONGO_LIBS})
else()
  target_link_libraries(erpi m ${BASE_LIBS} ${MONGO_LIBS})	
endif()
target_compile_definitions(erpi PUBLIC ${MONGOC_STATIC_DEFINITIONS})

# Target publisher (simple example)
add_executable(pub ${SOURCE_DIR}/app/pub.c)
target_include_directories(pub PRIVATE ${MONGOC_STATIC_INCLUDE_DIRS})
target_compile_definitions(pub PRIVATE ${MONGOC_STATIC_DEFINITIONS})
target_link_libraries(pub erpi)

# Target subscriber (simple example)
add_executable(sub ${SOURCE_DIR}/app/sub.c)
target_include_directories(sub PRIVATE ${MONGOC_STATIC_INCLUDE_DIRS})
target_compile_definitions(sub PRIVATE ${MONGOC_STATIC_DEFINITIONS})
target_link_libraries(sub erpi)

# Target plugin
add_library(dummy SHARED ${SOURCE_DIR}/plugin/dummy.c)
target_link_libraries(dummy erpi)
set_target_properties(dummy PROPERTIES PREFIX "" SUFFIX ".plugin")
if (CROSS)
  add_executable(canbus ${SOURCE_DIR}/plugin/canbus.c)
  target_link_libraries(canbus erpi)
  set_target_properties(canbus PROPERTIES PREFIX "" SUFFIX ".plugin")
  list(APPEND TARGETS_LIST canbus)
endif()

                           
#  _____         _       _ _ 
# |     |___ ___| |_ ___| | |
# |-   -|   |_ -|  _| .'| | |
# |_____|_|_|___|_| |__,|_|_|
                           
# Install setup (directly copy to CMAKE_INSTALL_PREFIX when make install)
set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY}/install)
install(TARGETS ${TARGETS_LIST}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION bin
  ARCHIVE DESTINATION lib
)
install(FILES ${LIB_HEADERS} DESTINATION include)

# Install package generator (creates an installer script)
set(CPACK_GENERATOR "STGZ")
set(CPACK_PACKAGE_VERSION ${GIT_COMMIT_HASH})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME}-${CMAKE_BUILD_TYPE})
include(CPack)
