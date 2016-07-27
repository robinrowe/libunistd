
add_executable (tabstop main.cpp)
include_directories(${CMAKE_SOURCE_DIR}/..)
add_subdirectory("${CMAKE_SOURCE_DIR}/../portable" "${CMAKE_CURRENT_BINARY_DIR}/libwunistd/portable")
target_link_libraries(tabstop libportable)