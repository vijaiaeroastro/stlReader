cmake_minimum_required(VERSION 3.20)
project(stlReader)

set(CMAKE_CXX_STANDARD 14)
find_package(Boost 1.65 REQUIRED)

add_executable(stlReader main.cpp)
