# libunistd/tabstop/CMakeLists.txt

add_executable (tabstop 
	main.cpp
	TabStop.cpp
)

target_link_libraries(tabstop libportable)