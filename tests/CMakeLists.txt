file(GLOB TESTING_SOURCE *_test.cc)
add_executable(cpputils_testing_bin ${TESTING_SOURCE})
target_link_libraries(cpputils_testing_bin gtest gtest_main)
