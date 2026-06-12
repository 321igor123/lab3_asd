cmake_minimum_required(VERSION 3.14)
project(HashTableBenchmarks LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(hash_tests test.cpp HashTable.cpp catch_amalgamated.cpp)
target_include_directories(hash_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})