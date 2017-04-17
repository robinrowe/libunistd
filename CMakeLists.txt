# libunistd/CMakeList.txt

cmake_minimum_required (VERSION 3.0.0)
project(libunistd)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_GENERATOR_PLATFORM x64)

if(WIN32)
add_subdirectory(vcpp)
endif()

add_subdirectory(portable)
add_subdirectory(test)
add_subdirectory(xxHash)

#add_subdirectory(tabstop)

