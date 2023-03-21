# libunistd/CMakeList.txt

cmake_minimum_required (VERSION 3.0.0)
project(libunistd)
message("<<< Parsing libunistd/CMakeList.txt")
message("--- Building ${PROJECT_NAME} ${CMAKE_SYSTEM_NAME}:${CMAKE_HOST_SYSTEM_PROCESSOR}:${CMAKE_GENERATOR_TOOLSET} ---")

option(UE4 "Disable _ITERATOR_DEBUG_LEVEL for UE4 compatibility" OFF)
option(WITH_TESTS "Enable this to build the tests / examples as well." OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_EXTENSIONS FALSE)
#set(CMAKE_GENERATOR_PLATFORM x64)

add_definitions(-DTRACE_MSG_FUNCTIONS)

if(UE4)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
endif(UE4)
# cmake -D_HAS_ITERATOR_DEBUGGING=0 -D_ITERATOR_DEBUG_LEVEL=0 -A x64 ..
# _HAS_ITERATOR_DEBUGGING=0;_ITERATOR_DEBUG_LEVEL=0;

include(libunistd-config.cmake)

if(WIN32)
	include_directories(unistd)
	add_subdirectory(unistd)
	link_directories(build/win64)
	link_libraries(libunistd Ws2_32)
	message("Linking libunistd")
endif(WIN32)

if(NOT WIN32 AND NOT APPLE)
	link_libraries(rt pthread dl)
	message("Linking rt pthread dl")
endif(NOT WIN32 AND NOT APPLE)

add_subdirectory(portable)
add_subdirectory(sqlite)
add_subdirectory(uuid)
add_subdirectory(regex)
add_subdirectory(xxhash)
if(WITH_TESTS)
add_subdirectory(test)
endif()
