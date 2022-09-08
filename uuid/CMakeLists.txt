# libunistd/libuuid/CMakeLists.txt

project(libuuid)
message("--- Building library ${PROJECT_NAME} ---")

if(WIN32)
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../unistd" 
)
endif(WIN32)

add_definitions(
	-DHAVE_UNISTD_H 
	-DHAVE_ERR
	-DHAVE_ERRX
	-DHAVE_WARN
	-DHAVE_WARNX
	-DHAVE_STDLIB_H
	-DHAVE_USLEEP
	-DHAVE_SYS_TIME_H
	)

set(HEADERS
	uuid.h
	all-io.h
	c.h
	randutils.h
	uuidd.h
	uuidP.h
)

set(SOURCES
	clear.cpp
	compare.cpp
	copy.cpp
	gen_uuid.cpp
	isnull.cpp
	pack.cpp
	parse.cpp
	unpack.cpp
	unparse.cpp
	uuid_time.cpp
	randutils.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
if(WITH_TESTS)
add_subdirectory(test)
endif()