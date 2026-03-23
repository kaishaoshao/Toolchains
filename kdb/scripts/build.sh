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

# 处理用户输入
# sudo bash -c 'echo 0 > /proc/sys/kernel/yama/ptrace_scope'
# $  while sleep 10; do echo "I'm alive!"; done&
# $  ./build/tools/kdb -p 1460373 
# $ setcap CAP_SYS_PTRACE+eip ./build/tools/kdb

./build/tools/kdb 
./build/test/tests
