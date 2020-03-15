# cclater/use-libunistd.cmake.txt
# Created by Robin Rowe 2019-07-21
# License: Copyright 2019 Robin.Rowe@HeroicRobots.com ***Proprietary***

message("<<< Parsing use-libunistd.cmake")
include(${CMAKE_CURRENT_SOURCE_DIR}/DownloadProject.cmake)

# cmake -DLIBUNISTD_DOWNLOAD=ON ../.. -A x64
option(LIBUNISTD_DOWNLOAD "Force download of libunistd" FALSE)
set(LIBUNISTD_PATH /Code/github/libunistd)

function(GetProperty property)
	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	foreach(prop ${dirs})
		message(" property = ${property}")
	endforeach()
#	math(EXPR RESULT "${VALUE} * 2")
#	message("${RESULT}")
endfunction()

function(SetOutputDirectory dir)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${dir} )
	message("  SetOutputDirectory(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})")
	foreach( outputconfig ${CMAKE_CONFIGURATION_TYPES} )
		string( TOUPPER ${outputconfig} OUTPUTCONFIG )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir}/${outputconfig} )
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir}/${outputconfig} )
#		message("  CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} = ${dir}/${outputconfig}" )
	endforeach( outputconfig ${CMAKE_CONFIGURATION_TYPES} )
endfunction()

message("--- Searching for libunistd ---")
if(NOT IS_DIRECTORY ${LIBUNISTD_PATH})
	message("Not found: ${LIBUNISTD_PATH}")
	if(NOT ${LIBUNISTD_DOWNLOAD})
		set(LIBUNISTD_DOWNLOAD TRUE)
		message("Setting LIBUNISTD_DOWNLOAD TRUE")
	endif(NOT ${LIBUNISTD_DOWNLOAD})
endif(NOT IS_DIRECTORY ${LIBUNISTD_PATH})

#message("---Setting output directories ---")
if(${LIBUNISTD_DOWNLOAD})
#	message("Downloading libunistd...")
	set(OUTPUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/unistd)
else(${LIBUNISTD_DOWNLOAD})
	set(OUTPUTDIR ${CMAKE_CURRENT_BINARY_DIR}/build/x64)
endif(${LIBUNISTD_DOWNLOAD})

#	message(LIBUNISTD_DOWNLOAD = ${LIBUNISTD_DOWNLOAD})

if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
	set(BITS 64)
else("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
	set(BITS 32)
endif()
message("CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message("CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME} ${BITS}bit")
#message("CMAKE_SIZEOF_VOID_P = ${CMAKE_SIZEOF_VOID_P}")

set(OUTPUTDIR ${CMAKE_CURRENT_BINARY_DIR})

SetOutputDirectory(${OUTPUTDIR})
#SetOutputDirectory(${libunistd_BINARY_DIR})

if(${LIBUNISTD_DOWNLOAD})
	message("Downloading libunistd... ${OUTPUTDIR}")
	set(LIBUNISTD_PATH ${CMAKE_CURRENT_BINARY_DIR}/libunistd)
	set(GIT_REPO https://github.com/robinrowe/libunistd.git)
	download_project(
		PROJ				libunistd
#		PREFIX              ${LIBUNISTD_PATH}
		PREFIX				${OUTPUTDIR}
		GIT_REPOSITORY      ${GIT_REPO}
#		GIT_TAG             ${GIT_TAG}
		UPDATE_DISCONNECTED ON)
	message("libunistd_SOURCE_DIR = ${libunistd_SOURCE_DIR}")
	message("libunistd_BINARY_DIR = ${libunistd_BINARY_DIR}")
	set(LIBUNISTD_PATH ${libunistd_SOURCE_DIR}) #${CMAKE_CURRENT_BINARY_DIR}/libunistd/libunistd-src)
#	add_subdirectory(${LIBUNISTD_PATH})
	add_subdirectory(${libunistd_SOURCE_DIR} ${libunistd_BINARY_DIR})
endif(${LIBUNISTD_DOWNLOAD})

find_package(LIBUNISTD REQUIRED
	NAMES libunistd
	PATHS ${LIBUNISTD_PATH} NO_DEFAULT_PATH )

if(WIN32)
	include_directories(${LIBUNISTD_INCLUDE_DIRS})
	link_directories(${LIBUNISTD_LIB_DIRS})
	link_libraries(${LIBUNISTD_LIBRARIES})
else(WIN32)
	link_directories(${LIBPORTABLE_LIB_DIRS})
endif(WIN32)

include_directories(${LIBPORTABLE_INCLUDE_DIRS})
link_libraries(${LIBPORTABLE_LIBRARIES})

