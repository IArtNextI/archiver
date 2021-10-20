cmake_minimum_required(VERSION 2.8)
project(archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

add_library(WRITER writer.cpp)

# Setup testing
link_directories(/usr/local/lib)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_custom_target(tests_writer COMMAND GTEST_COLOR=yes ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB TEST_FILES "tests/*.cpp")
add_executable("writer_tests" EXCLUDE_FROM_ALL ${TEST_FILES})
target_link_libraries("writer_tests" gtest pthread WRITER)
add_dependencies(tests_writer "writer_tests")
add_test("writer_tests" "./writer_tests")