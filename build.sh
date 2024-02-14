#!/bin/bash

# Check if vcpkg is installed
if [ ! -d "./vcpkg" ]
then
    # Clone vcpkg repository
    git clone https://github.com/microsoft/vcpkg.git
    # Bootstrap vcpkg
    ./vcpkg/bootstrap-vcpkg.sh
fi

# Install dependencies
./vcpkg/vcpkg install sdl2
./vcpkg/vcpkg install sdl2-image
./vcpkg/vcpkg install sdl2-ttf
./vcpkg/vcpkg install nlohmann-json

# Configure and build project
mkdir -p build
cmake -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake -B build
cmake --build build