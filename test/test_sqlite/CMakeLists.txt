# libunistd/test/test_sqlite/CMakeList.txt

include_directories("..")
add_executable(test_sqlite main.cpp)
link_directories(${LIBSQLITE_LIB_DIRS}/sqlite)
target_link_libraries(test_sqlite libsqlite)
