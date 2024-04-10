add_library(Unity STATIC
    unity/src/unity.c
)
add_library(cJSON STATIC
    cJSON/cJSON.c
)

target_include_directories(Unity PUBLIC
    unity/src
)
target_include_directories(cJSON PUBLIC
cJSON
)
