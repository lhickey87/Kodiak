
cmake_minimum_required(VERSION 3.2.0)
project(Kodiak VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_library(Kodiak src/Column.cpp)
find_path(include)

target_include_directories(Kodiak PRIVATE include)
