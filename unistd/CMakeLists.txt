# libunistd/unistd/CMakeLists.txt
# Copyright 2016 Robin.Rowe@cinepaint.org
# License MIT open source

if(_M_X64)
	enable_language(ASM_MASM)
	set(INT128_SRC Int128x64.asm)
else(_M_X64)
	set(INT128_SRC int128/Int128x86.cpp)
endif(_M_X64)

file(STRINGS sources.cmake SOURCES)
add_library(libunistd 
	${SOURCES}
	int128/Int128.h
	${INT128_SRC}
)

target_include_directories(libunistd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libunistd LINK_PUBLIC shlwapi)
target_link_libraries(libunistd LINK_PUBLIC libportable)
