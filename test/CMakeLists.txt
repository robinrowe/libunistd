# libunistd/test/CMakeList.txt

if(WIN32)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../unistd")
link_libraries(libunistd)
endif(WIN32)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

add_subdirectory(test_unit)
add_subdirectory(test_scandir)
add_subdirectory(test_mman)
add_subdirectory(test_semaphore)
