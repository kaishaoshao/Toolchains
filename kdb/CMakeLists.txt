cmake_minimum_required(VERSION 3.20)

project("kdb" LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(readline REQUIRED IMPORTED_TARGET readline)

# 启动cmake内置测试系统
include(CTest)

add_subdirectory("src")
add_subdirectory("tools")

if(BUILD_TESTING)
find_package(Catch2 CONFIG REQUIRED)
    add_subdirectory("test")
endif()
