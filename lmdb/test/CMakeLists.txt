# lmdb/test/CMakeLists.txt

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../unistd" 
	"${CMAKE_CURRENT_SOURCE_DIR}/../source")

#message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
		set(EXTRALIB libunistd)
else()
	set(EXTRALIB)
endif()

add_executable(mdb_copy mdb_copy.c)
target_link_libraries(mdb_copy liblmdb)
add_executable(mdb_dump mdb_dump.c)
target_link_libraries(mdb_dump liblmdb ${EXTRALIB})
add_executable(mdb_load mdb_load.c)
target_link_libraries(mdb_load liblmdb ${EXTRALIB})
add_executable(mdb_stat mdb_stat.c)
target_link_libraries(mdb_stat liblmdb ${EXTRALIB})
add_executable(mtest mtest.c)
target_link_libraries(mtest liblmdb)
add_executable(mtest2 mtest2.c)
target_link_libraries(mtest2 liblmdb)
add_executable(mtest3 mtest3.c)
target_link_libraries(mtest3 liblmdb)
add_executable(mtest4 mtest4.c)
target_link_libraries(mtest4 liblmdb)
add_executable(mtest5 mtest5.c)
target_link_libraries(mtest5 liblmdb)
add_executable(mtest6 mtest6.c)
target_link_libraries(mtest6 liblmdb)
