cmake_minimum_required(VERSION 3.20)

project("kdb" LANGUAGE CXX)

# 启动cmake内置测试系统
include(CTest)

add_subdirectory("src")
add_subdirectory("tools")

if(BUILD_TESTING)
    add_subdirectory("test")
endif()
