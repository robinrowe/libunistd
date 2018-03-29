# libunistd/CMakeList.txt

cmake_minimum_required (VERSION 3.0.0)
set(NAME libunistd)
project(${NAME})

if(WIN32)
set(CMAKE_GENERATOR_TOOLSET host=x64)
endif()
message("--- Building ${NAME} ${CMAKE_SYSTEM_NAME}:${CMAKE_HOST_SYSTEM_PROCESSOR}:${CMAKE_GENERATOR_TOOLSET} ---")

option(UE4 "Disable _ITERATOR_DEBUG_LEVEL for UE4 compatibility" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_GENERATOR_PLATFORM x64)

if(UE4)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
endif(UE4)
# cmake -D_HAS_ITERATOR_DEBUGGING=0 -D_ITERATOR_DEBUG_LEVEL=0 -A x64 ..
# _HAS_ITERATOR_DEBUGGING=0;_ITERATOR_DEBUG_LEVEL=0;

if(WIN32)
add_subdirectory(unistd)
endif()

add_subdirectory(portable)
add_subdirectory(test)
add_subdirectory(xxHash)

#add_subdirectory(tabstop)

