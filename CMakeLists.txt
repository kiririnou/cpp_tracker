cmake_minimum_required(VERSION 3.13)
project (cpp_tracker)

set(cpp_tracker_VERSION_MAJOR 0)
set(cpp_tracker_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor/mingw-std-threads)
add_subdirectory(src)