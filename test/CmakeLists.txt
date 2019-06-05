# libunistd/test/CMakeList.txt

cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../unistd")
endif(WIN32)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

add_subdirectory(test_PosixQueue)
add_subdirectory(test_unit)
add_subdirectory(test_wormfile)
add_subdirectory(test_scandir)
add_subdirectory(test_lmdb)
add_subdirectory(test_sqlite)
add_subdirectory(test_mman)
add_subdirectory(test_semaphore)
