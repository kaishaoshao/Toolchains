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
    PRIVATE include
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)



