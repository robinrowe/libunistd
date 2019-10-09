# libunistd/unistd/CMakeLists.txt

enable_language(ASM_MASM)
add_library(libunistd
    dirent.h
    dirent.cpp
	unistd.h
	gettimeofday.cpp
	uni_signal.cpp
	syslog.cpp
	Int128.h
	Int128.cpp
	Uint128.cpp
	Int128.asm
)

target_include_directories(libunistd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libunistd LINK_PUBLIC libportable)