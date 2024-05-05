#!/bin/bash

export EMSCRIPTEN_ROOT=/usr/share/emscripten

cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=/usr/bin/emcc -DCMAKE_CXX_COMPILER=/usr/bin/em++ \
  -DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
  -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake

cmake --build build
