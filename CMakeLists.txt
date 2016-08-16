# libunistd/CMakeList.txt

cmake_minimum_required (VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_EXTENSIONS FALSE)

include_directories(.)

if(WIN32)
add_subdirectory(vcpp)
endif()

add_subdirectory(portable)
add_subdirectory(tabstop)

