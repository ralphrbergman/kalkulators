#!/bin/bash
set -e

NOT_IN_BUILD=false

if [ "$(basename "$PWD")" != "build" ]; then
    NOT_IN_BUILD=true

    mkdir -p "build"
    cd "build"
fi

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j$(nproc)

if [ "$NOT_IN_BUILD" = true ]; then
    cd ..
fi
