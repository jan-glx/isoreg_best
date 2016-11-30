cmake_minimum_required(VERSION 3.5)
project(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src2/test_Best.cpp src2/test_rih.cpp)
add_executable(isoregBestTest src2/test_Best.cpp)

add_executable(rihTest src2/test_rih.cpp)