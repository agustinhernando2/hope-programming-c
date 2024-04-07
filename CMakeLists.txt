# Request the minimum version of CMake, in case of lower version throws error
# See: https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Define your project
# See: https://cmake.org/cmake/help/latest/command/project.html
project(
  monitoring-application
  VERSION 1.0.0
  DESCRIPTION "Development of Monitoring Application for Resistance for operating systems II"
  LANGUAGES C
)

# Enable testing and add the tests
include(CTest)

# Define the C standard, we are going to use std17
# See: https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD.html
set(CMAKE_C_STANDARD 17)

# Include the include directory, where the header files are located
# See: https://cmake.org/cmake/help/latest/command/include_directories.html
include_directories(include)

# Add the src directory, where the source files are located
# See: https://cmake.org/cmake/help/latest/command/file.html#glob
#file(GLOB_RECURSE SOURCES "src/*.c")

# To automatically download the dependencies we can use FetchContent
# See: https://cmake.org/cmake/help/latest/module/FetchContent.html
include(FetchContent)

# We are going to use cJSON to send messages between server and client
set(CJSON_GIT_URL "https://github.com/DaveGamble/cJSON.git")

# In this example we are going to use Unity to download and to test our project
set(UNITY_GIT_URL "https://github.com/ThrowTheSwitch/Unity.git")

# Enable debug
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  Unity
  GIT_REPOSITORY ${UNITY_GIT_URL}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/unity
  UPDATE_COMMAND ""
)

FetchContent_Declare(
  cJSON
  GIT_REPOSITORY ${CJSON_GIT_URL}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON
  UPDATE_COMMAND ""
)

# Ensures the named dependencies have been populated, either by an earlier call or by populating them now.
# FetchContent_MakeAvailable(Unity cJSON)

# Create the executable
# See https://cmake.org/cmake/help/latest/command/add_executable.html
add_executable(server src/server.c)
add_executable(client_monitor src/client_monitor.c)

# Compile the libraries
# See: https://cmake.org/cmake/help/latest/command/add_subdirectory.html
add_subdirectory(lib/libdyn)
add_subdirectory(lib/libsta)

add_library(cJSON STATIC external/cJSON/cJSON.c)
target_compile_options(cJSON PRIVATE)

# Include headers in lib
# See: https://cmake.org/cmake/help/latest/command/target_include_directories.html
target_include_directories(server PUBLIC lib/libdyn/include)
target_include_directories(server PUBLIC lib/libsta/include)
target_include_directories(server PUBLIC external/cJSON)

# Compilation flags
# set(COMMON_FLAGS "-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11")

# gdb
if(GDB EQUAL 1)
  set(CMAKE_C_FLAGS "-g ")
endif()

# Link the libraries
# See: https://cmake.org/cmake/help/latest/command/target_link_libraries.html
target_link_libraries(server StatiCLib DynamiCLib cJSON) 

# Add subdirectory for the tests
if(RUN_TESTS EQUAL 1 OR RUN_COVERAGE EQUAL 1)
  add_subdirectory(tests)
endif()



# Binaries directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

# Target clear
add_custom_target(clear
    COMMAND rm -f ${CMAKE_BINARY_DIR}/server
    COMMAND rm -f ${CMAKE_BINARY_DIR}/client_monitor
            ...
    COMMENT "clear"
)