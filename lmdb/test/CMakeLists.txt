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

set(LDAP ../liblmdb)

add_executable(mdb_copy ${LDAP}/mdb_copy.c)
target_link_libraries(mdb_copy liblmdb)
add_executable(mdb_dump ${LDAP}/mdb_dump.c)
target_link_libraries(mdb_dump liblmdb ${EXTRALIB})
add_executable(mdb_load ${LDAP}/mdb_load.c)
target_link_libraries(mdb_load liblmdb ${EXTRALIB})
add_executable(mdb_stat ${LDAP}/mdb_stat.c)
target_link_libraries(mdb_stat liblmdb ${EXTRALIB})
add_executable(mtest ${LDAP}/mtest.c)
target_link_libraries(mtest liblmdb)
add_executable(mtest2 ${LDAP}/mtest2.c)
target_link_libraries(mtest2 liblmdb)
add_executable(mtest3 ${LDAP}/mtest3.c)
target_link_libraries(mtest3 liblmdb)
add_executable(mtest4 ${LDAP}/mtest4.c)
target_link_libraries(mtest4 liblmdb)
add_executable(mtest5 ${LDAP}/mtest5.c)
target_link_libraries(mtest5 liblmdb)
add_executable(mtest6 ${LDAP}/mtest6.c)
target_link_libraries(mtest6 liblmdb)
