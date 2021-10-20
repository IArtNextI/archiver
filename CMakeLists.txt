cmake_minimum_required(VERSION 2.8)
project(archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -DCMAKE_BUILD_TYPE=Release -Ofast")

add_executable(MAIN main.cpp)

add_subdirectory(archiver/)
add_subdirectory(priority_queue/)
add_subdirectory(reader/)
add_subdirectory(writer/)
add_subdirectory(trie/)

target_link_libraries(MAIN ARCHIVER READER WRITER PRIORITY_QUEUE TRIE)
