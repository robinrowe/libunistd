# scandir/CMakeLists.txt

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../build/unistd/Debug)

add_executable(scandir main.cpp)
target_link_libraries(scandir libunistd)