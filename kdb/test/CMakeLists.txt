add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE kdb::libkdb Catch2::Catch2WithMain)
