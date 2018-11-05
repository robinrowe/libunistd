# lmdb/CMakeLists.txt

set(NAME test_lmdb)

add_executable(${NAME} test-lmdb.cpp)
target_link_libraries(${NAME} liblmdb)