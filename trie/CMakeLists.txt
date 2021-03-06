cmake_minimum_required(VERSION 2.8)
project(archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

add_library(TRIE trie.cpp)

# Setup testing
link_directories(/usr/local/lib)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_custom_target(tests_trie COMMAND GTEST_COLOR=yes ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB TEST_FILES "tests/*.cpp")
add_executable("trie_tests" EXCLUDE_FROM_ALL ${TEST_FILES})
target_link_libraries("trie_tests" gtest pthread TRIE)
add_dependencies(tests_trie "trie_tests")
add_test("trie_tests" "./trie_tests")