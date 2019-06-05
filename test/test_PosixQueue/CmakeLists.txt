# libunistd/test/test_PosixQueue/CMakeList.txt

add_executable(test_mq_receive mq_receive/main.cpp)
target_link_libraries(test_mq_receive libportable)

add_executable(test_mq_send mq_send/main.cpp)
target_link_libraries(test_mq_send libportable)