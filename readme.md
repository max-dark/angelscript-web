AngelScript interpreter for WASM

How to build

see `build.sh`

install vcpkg
```shell
git clone https://github.com/microsoft/vcpkg.git
```

install Emscripten
```shell
sudo apt install emscripten
```

configure project
```shell
export EMSCRIPTEN_ROOT=/usr/share/emscripten
cmake -S . -B build -DCMAKE_C_COMPILER=/usr/bin/emcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/em++ \
  -DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

build
```shell
cmake --build build
```