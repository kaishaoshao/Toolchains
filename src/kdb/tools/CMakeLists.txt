add_executable(kdb kdb.cpp)
target_link_libraries(kdb PRIVATE kdb::libkdb PkgConfig::readline)

include(GNUInstallDirs)
install(
    TARGETS kdb
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
