cmake_minimum_required(VERSION 3.25)
project(web_script)

# ENV - set path to emsdk see wasm32-emscripten
# EMSCRIPTEN_ROOT=/usr/share/emscripten

# cmake params
# -DCMAKE_C_COMPILER=/usr/bin/emcc -DCMAKE_CXX_COMPILER=/usr/bin/em++
# -DVCPKG_TARGET_TRIPLET=wasm32-emscripten -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

set(CMAKE_EXECUTABLE_SUFFIX .js)
set(CMAKE_CXX_STANDARD 17)

# find_package(Emscripten)
find_package(Angelscript CONFIG REQUIRED)

add_executable(web_script main.cpp)
target_link_libraries(web_script
    PRIVATE
        Angelscript::angelscript
        embind
)
