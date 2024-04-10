# Request the minimum version of CMake, in case of lower version throws error
# See: https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Define your project
# See: https://cmake.org/cmake/help/latest/command/project.html
project(
  monitoringApplication
  VERSION 1.0.0
  DESCRIPTION "Development of Monitoring Application for Resistance for operating systems II"
  LANGUAGES C
)

# Enable testing and add the tests
include(CTest)

# Define the C standard, we are going to use std17
# See: https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD.html
set(CMAKE_C_STANDARD 17)

# Set binary directory
# set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

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
add_library(cJSON STATIC external/cJSON/cJSON.c)
target_compile_options(cJSON PRIVATE)

# Compile the libraries
# See: https://cmake.org/cmake/help/latest/command/add_subdirectory.html
add_subdirectory(lib/libdyn)
add_subdirectory(lib/libsta)
add_subdirectory(client)
add_subdirectory(server)


# Include headers in lib
# See: https://cmake.org/cmake/help/latest/command/target_include_directories.html
# target_include_directories(server PUBLIC lib/libdyn/include)
# target_include_directories(server PUBLIC lib/libsta/include)
# target_include_directories(server PUBLIC external/cJSON)

# target_include_directories(client PUBLIC lib/libdyn/include)
# target_include_directories(client PUBLIC lib/libsta/include)
# target_include_directories(client PUBLIC external/cJSON)

# Compilation flags
# set(COMMON_FLAGS "-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11")

# Link the libraries
# See: https://cmake.org/cmake/help/latest/command/target_link_libraries.html
# target_link_libraries(server StatiCLib DynamiCLib cJSON) 
# target_link_libraries(client StatiCLib DynamiCLib cJSON) 

# Add subdirectory for the tests


# Target clear
add_custom_target(clear
    COMMAND rm -f ${CMAKE_BINARY_DIR}/server
    COMMAND rm -f ${CMAKE_BINARY_DIR}/client
            ...
    COMMENT "clear"
)
