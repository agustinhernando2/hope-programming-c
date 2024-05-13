#!/bin/bash


# We only make check, not changes
# Use the find command with the variable
PROJECT_PATH=$(pwd)

echo $PROJECT_PATH

# ToDo: Add documentation about 'tr' command and describe better this command
SOURCE_FILES=$(find $PROJECT_PATH/server/src -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES+=$(find $PROJECT_PATH/server/include -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES=$(find $PROJECT_PATH/client/src -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES+=$(find $PROJECT_PATH/client/include -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES+=$(find $PROJECT_PATH/lib/libdyn/src -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES+=$(find $PROJECT_PATH/lib/libdyn/include -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES+=$(find $PROJECT_PATH/lib/libsta/src -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES+=$(find $PROJECT_PATH/lib/libsta/include -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")

clang-format --style=Microsoft -i $SOURCE_FILES

sleep 1
# Elimina el directorio build si existe
rm -rf build

# Crea el directorio build y entra en él
mkdir build && cd build

# Configura el proyecto usando CMake con el generador Ninja
if [ "$1" == "test" ]; then
    cmake -GNinja -DRUN_COVERAGE=1 -DRUN_TESTS=1 ..
    ninja
    ctest --test-dir tests -VV 
elif [ "$1" == "debug" ]; then
    cmake -GNinja -DCMAKE_BUILD_TYPE=Debug ..
    ninja
else
    cmake -GNinja ..
    ninja
fi
