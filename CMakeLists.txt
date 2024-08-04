# Request the minimum version of CMake, in case of lower version throws error
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Define your project
project(
  monitoringApplication
  VERSION 1.0.0
  DESCRIPTION "Development of Monitoring Application for Resistance for operating systems II"
  LANGUAGES C
)

# Define the C standard, we are going to use std17
set(CMAKE_C_STANDARD 17)

# Flags de compilación
set(CMAKE_C_FLAGS "-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11")

# To automatically download the dependencies we can use FetchContent
include(FetchContent)

# We are going to use cJSON to send messages between server and client
set(CJSON_GIT_URL "https://github.com/DaveGamble/cJSON.git")

# Enable debug
set(FETCHCONTENT_QUIET OFF)

# FetchContent_Declare(
#   Unity_test
#   GIT_REPOSITORY ${UNITY_GIT_URL}
#   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/unity
#   UPDATE_COMMAND ""
# )
# UNITY in cJSON library
FetchContent_Declare(
  cJSON
  GIT_REPOSITORY ${CJSON_GIT_URL}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON
  UPDATE_COMMAND ""
)
FetchContent_MakeAvailable(cJSON)

# Compile the libraries
add_subdirectory(lib/libdyn)
add_subdirectory(lib/libsta)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(external)

# Add tests if enabled
if(RUN_TESTS EQUAL 1 OR RUN_COVERAGE EQUAL 1)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

# Compilation flags
if(FLAGS EQUAL 1)
  set(COMMON_FLAGS "-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11")
endif()
