file(GLOB SOURCES *_test.cpp)
add_executable(cpputils_testing_bin ${SOURCES})
target_link_libraries(cpputils_testing_bin PRIVATE -Wl,--start-group cpputils GTest::gtest GTest::gtest_main fmt::fmt
                                                   -Wl,--end-group)
