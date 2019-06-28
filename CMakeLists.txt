# libunistd/CMakeList.txt

cmake_minimum_required (VERSION 3.0.0)
project(libunistd)

if(WIN32)
set(CMAKE_GENERATOR_TOOLSET host=x64)
endif()
message("--- Building ${PROJECT_NAME} ${CMAKE_SYSTEM_NAME}:${CMAKE_HOST_SYSTEM_PROCESSOR}:${CMAKE_GENERATOR_TOOLSET} ---")

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
	include_directories(unistd)
	add_subdirectory(unistd)
	link_directories(build/win64)
	link_libraries(libunistd)
endif(WIN32)
if(NOT WIN32 AND NOT APPLE)
	link_libraries(rt pthread dl)
endif(NOT WIN32 AND NOT APPLE)

add_subdirectory(libuuid)
add_subdirectory(lmdb)
add_subdirectory(portable)
add_subdirectory(regex)
add_subdirectory(sqlite)
add_subdirectory(test)
add_subdirectory(xxHash)

#add_subdirectory(tabstop)

