# lmdb/CMakeLists.txt

set(NAME test_lmdb)
set(INCDIR 
	${CMAKE_CURRENT_SOURCE_DIR}/../../lmdb/liblmdb
	${CMAKE_CURRENT_SOURCE_DIR}/../..
)
#message("INCDIR = " ${INCDIR})

add_executable(test_lmdb_c sample-mdb.c)
target_include_directories(test_lmdb_c PUBLIC ${INCDIR})
target_link_libraries(test_lmdb_c liblmdb)

add_executable(${NAME} test-lmdb.cpp ProductData.h)
target_include_directories(${NAME} PUBLIC ${INCDIR})
target_link_libraries(${NAME} liblmdb)