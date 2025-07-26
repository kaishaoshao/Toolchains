#!/bin/bash
scripts_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
mkdir -p build
mkdir -p third
third_dir=${scripts_dir}/../third

if [ ! -d ${third}/vcpkg ]; then
    git clone https://github.com/microsoft/vcpkg.git ${third_dir}/vcpkg
fi

cmake -S . -B build  \
-DCMAKE_TOOLCHAIN_FILE=${third_dir}/vcpkg/scripts/buildsystems/vcpkg.cmake && cmake --build build 
./build/tools/kdb -p 
./build/test/tests
