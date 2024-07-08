#!/bin/bash

# Check if vcpkg is installed
if [ ! -d "./vcpkg" ]; then
    # Perform a shallow clone of the vcpkg repository
    git clone --depth 1 https://github.com/microsoft/vcpkg.git
    # Bootstrap vcpkg
    ./vcpkg/bootstrap-vcpkg.sh
    rm -rf ./vcpkg/.git
fi

# Install dependencies
./vcpkg/vcpkg install sdl2
./vcpkg/vcpkg install sdl2-image
./vcpkg/vcpkg install sdl2-ttf
./vcpkg/vcpkg install sdl2-mixer[mpg123]
./vcpkg/vcpkg install nlohmann-json
./vcpkg/vcpkg install gtest

# Configure and build project
mkdir -p build
cmake -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake -B build
cmake --build build
