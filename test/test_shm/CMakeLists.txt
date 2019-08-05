# libunistd/test/test_shm/CMakeList.txt

add_executable(test_shm_server shm_server.cpp)
target_link_libraries(test_shm_server libunistd)
add_executable(test_shm_client shm_server.cpp)
target_link_libraries(test_shm_client libunistd)