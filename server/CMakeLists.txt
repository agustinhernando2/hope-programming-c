# Configure the client project
project(server)

# Add header
include_directories(include)

# Create the executable
add_executable(${PROJECT_NAME} src/server.c)

# Include directories for libraries and cJSON
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libdyn/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libsta/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/cJSON)

# Link libraries StatiCLib, DynamiCLib, and cJSON
target_link_libraries(${PROJECT_NAME} StatiCLib DynamiCLib cJSON)  

