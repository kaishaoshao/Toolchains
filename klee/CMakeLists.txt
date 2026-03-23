cmake_minimum_required(VERSION 3.15)
project(MiniKLEE)

set(CMAKE_CXX_STANDARD 17)

# 寻找 LLVM
find_package(LLVM REQUIRED CONFIG)

# 核心：确保包含目录被正确引入
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 寻找 Z3 库（确保你安装了 libz3-dev）
find_library(Z3_LIB NAMES z3)

add_executable(mini_klee main.cpp)

# 映射 LLVM 组件
llvm_map_components_to_libnames(llvm_libs core support irreader)

# 链接
target_link_libraries(mini_klee ${llvm_libs} ${Z3_LIB})
