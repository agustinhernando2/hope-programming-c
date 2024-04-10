# Configure the client project
project(client)

# Create the executable
add_executable(${PROJECT_NAME} src/client.c)

# Include directories for libraries and cJSON
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libdyn/include ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libsta/include external/cJSON)

# Link libraries StatiCLib, DynamiCLib, and cJSON
target_link_libraries(${PROJECT_NAME} StatiCLib DynamiCLib cJSON)  

# Add tests if enabled
if(RUN_TESTS EQUAL 1 OR RUN_COVERAGE EQUAL 1)
  add_subdirectory(tests)
endif()
