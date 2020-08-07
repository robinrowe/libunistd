# libunistd/test/CMakeList.txt

if(WIN32)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../unistd")
link_libraries(libunistd)
endif(WIN32)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

add_subdirectory(test_PosixQueue)
add_subdirectory(test_unit)
add_subdirectory(test_wormfile)
add_subdirectory(test_scandir)
add_subdirectory(test_lmdb)
add_subdirectory(test_sqlite)
add_subdirectory(test_mman)
add_subdirectory(test_semaphore)
add_subdirectory(test_logger)
add_subdirectory(test_Pump)
