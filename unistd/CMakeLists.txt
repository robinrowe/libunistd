# libunistd/vcpp/CMakeLists.txt

add_library (libunistd
    dirent.h
    dirent.cpp
	unistd.h
	gettimeofday.cpp
	uni_signal.cpp
)

target_include_directories (libunistd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (libunistd LINK_PUBLIC libportable)