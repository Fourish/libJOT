#!/bin/bash

# set build variables
CMAKE_AR=
CMAKE_MAKE_PROGRAM=
CMAKE_PREFIX_PATH=
CMAKE_C_COMPILER=
CMAKE_CXX_COMPILER=
CMAKE_BUILD_TYPE=Release
CMAKE_EXPORT_COMPILE_COMMANDS=ON

# set if build shared or static
LIBJOT_BUILD_STATIC=ON
LIBJOT_BUILD_SHARED=OFF
LIBJOT_BUILD_FULL=OFF

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
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE:STRING=$CMAKE_BUILD_TYPE -DCMAKE_AR:STRING=$CMAKE_AR -DCMAKE_MAKE_PROGRAM:STRING=$CMAKE_MAKE_PROGRAM -DCMAKE_PREFIX_PATH:STRING=$CMAKE_PREFIX_PATH -DCMAKE_C_COMPILER:STRING=$CMAKE_C_COMPILER -DCMAKE_CXX_COMPILER:STRING=$CMAKE_CXX_COMPILER -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=$CMAKE_EXPORT_COMPILE_COMMANDS -DBUILD_STATIC_LIBS:BOOL=$LIBJOT_BUILD_STATIC -DBUILD_SHARED_LIBS:BOOL=$LIBJOT_BUILD_SHARED -DLIBJOT_BUILD_FULL:BOOL=$LIBJOT_BUILD_FULL ..
cmake --build . --config $CMAKE_BUILD_TYPE
# run make
make -j$(nproc)