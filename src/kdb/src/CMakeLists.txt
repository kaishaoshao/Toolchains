# 创建一个名为 libkdb 的库目标
add_library(libkdb libkdb.cpp)
# 为 libkdb 创建一个带命名空间的别名目标 sdb::libkdb
# 作用：
# 通过 sdb:: 命名空间避免命名冲突
# 符合现代 CMake 的导入/导出规范
# 便于被其他项目通过 find_package() 查找使用
# 通过 sdb:: 前缀为库目标添加命名空间
# 避免不同项目间的目标名称冲突（例如：多个项目可能都有 libkdb，但 sdb::libkdb 是唯一的
add_library(kdb::libkdb ALIAS libkdb)

# 将默认的 libkdb 改为 kdb
# 效果示例：
# Linux: 生成 libkdb.so → 改为 libkdb.so
# Windows: 生成 kdb.lib/kdb.dll
set_target_properties(
    libkdb
    PROPERTIES OUTPUT_NAME kdb
)

target_compile_features(libkdb PRIVATE cxx_std_17)

target_include_directories(
    libkdb
    PUBLIC                        # 分离构建头文件和安装头文件
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

## 使用库和安装库信息
# GNUInstallDirs 包含指定通用安装目录的变量，如 /include 和 /lib
include(GNUInstallDirs) 
# 创建一个导出目标kdb-targets, 其中包含有关项目不同组件的安装位置信息
install(TARGETS libkdb
    EXPORT kdb-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 安装公共头文件到指定目录
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kdb
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 使libkdb可被其他cmake项目导入，我们需要将libkdb的导出信息保存到文件中
install(
    EXPORT kdb-targets
    FILE kdb-config.cmake
    NAMESPACE kdb::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kdb
)


