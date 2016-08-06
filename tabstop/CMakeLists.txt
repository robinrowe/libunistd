
add_executable (tabstop 
	main.cpp
	TabStop.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/.. ${CMAKE_SOURCE_DIR}/../vcpp)
add_subdirectory("${CMAKE_SOURCE_DIR}/../portable" "${CMAKE_CURRENT_BINARY_DIR}/libwunistd/portable")
target_link_libraries(tabstop libportable)