cmake_minimum_required(VERSION 3.15)
project(made_2019_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(01)

add_executable(algo_1_1_3
        algo/01/task_1_3/main.cpp)

add_executable(algo_1_2_4
        algo/01/task_2_4/main.cpp)

add_executable(cpp_02_test
        02/allocator.cpp
        02/allocator.hpp
        02/test.cpp)

add_executable(cpp_03_test
        03/tokenizer.cpp
        03/tokenizer.hpp
        03/test.cpp)
