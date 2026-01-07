cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-15")

project(Kodiak CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find TBB
find_package(TBB REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(KodiakLib STATIC ${SOURCES})

target_include_directories(KodiakLib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${TBB_INCLUDE_DIRS}  # Force the TBB headers into the search path
)

# This tells CMake to propagate TBB's requirements to KodiakLib
target_link_libraries(KodiakLib PUBLIC TBB::tbb)

# FORCED FIX: If clangd still struggles, this forces the macro
# that GCC uses to enable parallel algorithms.
target_compile_definitions(KodiakLib PUBLIC _GLIBCXX_HAS_GTHREADS=1)
