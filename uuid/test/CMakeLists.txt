# libuuid/test/CMakeLists.txt

if(WIN32)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../unistd")
endif(WIN32)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

#message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(test_uuid test_uuid.c)
target_link_libraries(test_uuid libuuid)
