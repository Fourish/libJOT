#!/bin/bash

# set build variables
CMAKE_AR=D:/build-essential/msys64/ucrt64/bin/ar.exe
CMAKE_MAKE_PROGRAM=D:/build-essential/msys64/usr/bin/make.exe
CMAKE_PREFIX_PATH=D:/build-essential/msys64/ucrt64
CMAKE_C_COMPILER=D:/build-essential/msys64/ucrt64/bin/gcc.exe 
CMAKE_CXX_COMPILER=D:/build-essential/msys64/ucrt64/bin/g++.exe
CMAKE_BUILD_TYPE=Release
CMAKE_EXPORT_COMPILE_COMMANDS=ON

# set if build shared or static
BUILD_STATIC=ON
BUILD_SHARED=OFF

# check if build variables are set
if [ -z "$CMAKE_AR" ]; then
    echo "CMAKE_AR is not set"
    exit 1
fi
if [ -z "$CMAKE_MAKE_PROGRAM" ]; then
    echo "CMAKE_MAKE_PROGRAM is not set"
    exit 1
fi
if [ -z "$CMAKE_PREFIX_PATH" ]; then
    echo "CMAKE_PREFIX_PATH is not set"
    exit 1
fi
if [ -z "$CMAKE_C_COMPILER" ]; then
    echo "CMAKE_C_COMPILER is not set"
    exit 1
fi
if [ -z "$CMAKE_CXX_COMPILER" ]; then
    echo "CMAKE_CXX_COMPILER is not set"
    exit 1
fi
if [ -z "$CMAKE_BUILD_TYPE" ]; then
    echo "CMAKE_BUILD_TYPE is not set"
    exit 1
fi
if [ -z "$CMAKE_EXPORT_COMPILE_COMMANDS" ]; then
    echo "CMAKE_EXPORT_COMPILE_COMMANDS is not set"
    exit 1
fi

# make build dircetory
mkdir -p build && cd build

# run cmake
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE:STRING=$BUILD_TYPE -DCMAKE_AR:STRING=$CMAKE_AR -DCMAKE_MAKE_PROGRAM:STRING=$CMAKE_MAKE_PROGRAM -DCMAKE_PREFIX_PATH:STRING=$CMAKE_PREFIX_PATH  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=$CMAKE_EXPORT_COMPILE_COMMANDS -DCMAKE_C_COMPILER:STRING=$CMAKE_C_COMPILER -DCMAKE_CXX_COMPILER:STRING=$CMAKE_CXX_COMPILER -DBUILD_STATIC_LIBS:BOOL=$BUILD_STATIC -DBUILD_SHARED_LIBS:BOOL=$BUILD_SHARED -DCMAKE_BUILD_TYPE:STRING="$CMAKE_BUILD_TYPE" ..
cmake --build . --config $CMAKE_BUILD_TYPE
# run make
make -j$(nproc)