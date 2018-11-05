# test_scandir/CMakeLists.txt

add_executable(test_scandir scandir.cpp)
target_link_libraries(test_scandir libunistd)

add_executable(test_opendir opendir.cpp)
target_link_libraries(test_opendir libunistd)

get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
