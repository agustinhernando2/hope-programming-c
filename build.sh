#!/bin/bash

# Elimina el directorio build si existe
rm -rf build

# Crea el directorio build y entra en él
mkdir build && cd build

# Configura el proyecto usando CMake con el generador Ninja
cmake -GNinja -DRUN_TESTS=1 ..

# Construye el proyecto con Ninja
ninja
