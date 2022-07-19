# libunistd/lmdb/CMakeLists.txt

# git clone git://git.openldap.org/openldap.git --branch OPENPROJECT_NAME_REL_ENG_2_4

project(liblmdb)
message("--- Building library ${PROJECT_NAME} ---")

message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("BINARY_DIR = ${CMAKE_BINARY_DIR}/${CMAKE_GENERATOR_PLATFORM}")

if(WIN32)
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../unistd")
	link_directories(${CMAKE_BINARY_DIR}/${CMAKE_GENERATOR_PLATFORM})
	link_libraries(libunistd)
endif(WIN32)

set(LIBLMDB
	${PROJECT_NAME}/lmdb.h
	${PROJECT_NAME}/midl.h
	${PROJECT_NAME}/mdb.c
	${PROJECT_NAME}/midl.c
)

set(SOURCES
	${LIBLMDB}
	Cursor.h
	Cursor.cpp
	Field.h
	Item.h
	LightningDb.h
	LightningDb.cpp
	Transaction.h
	Transaction.cpp
)

if(WIN32)
	message("Suppressing Windows warnings for lmdb")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif(WIN32)

#necessary for building mdb_dkey for mtest6:
add_definitions(-DMDB_DEBUG)

#For detecting double-free():
#add_definitions(-DPRINT_MALLOC)

set(INCDIR 
	${CMAKE_CURRENT_SOURCE_DIR}/../lmdb/liblmdb
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCDIR})

add_subdirectory(test)
