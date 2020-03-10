# libunistd/unistd/CMakeLists.txt

enable_language(ASM_MASM)

if(_M_X64)
	set(SRC Int128x64.asm)
else(_M_X64)
	set(SRC int128/Int128x86.cpp)
endif(_M_X64)

add_library(libunistd
    dirent.h
    dirent.cpp
	unistd.h
	gettimeofday.cpp
	uni_signal.cpp
	syslog.cpp
	int128/Int128.h
	${SRC}
)

target_include_directories(libunistd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libunistd LINK_PUBLIC libportable)