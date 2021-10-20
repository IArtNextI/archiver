cmake_minimum_required(VERSION 2.8)
project(archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

add_library(PRIORITY_QUEUE priority_queue.cpp)

# Setup testing
link_directories(/usr/local/lib)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_custom_target(tests_priority_queue COMMAND GTEST_COLOR=yes ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB TEST_FILES "tests/*.cpp")
add_executable("priority_queue_tests" EXCLUDE_FROM_ALL ${TEST_FILES})
target_link_libraries("priority_queue_tests" gtest pthread PRIORITY_QUEUE TRIE)
add_dependencies(tests_priority_queue "priority_queue_tests")
add_test("priority_queue_tests" "./priority_queue_tests")