# libunistd/unistd/CMakeLists.txt

if(_M_X64)
	enable_language(ASM_MASM)
	set(INT128_SRC Int128x64.asm)
else(_M_X64)
	set(INT128_SRC int128/Int128x86.cpp)
endif(_M_X64)

add_library(libunistd
	clock_gettime.h
	clock_gettime.cpp
    dirent.h
    dirent.cpp
	glob.h
	unistd.cpp
	unistd.h
	getopt.cpp
	pthread.cpp
	gettimeofday.cpp
	pthread.h
	pthread.cpp
	sigaction.cpp
	sigaction.h
	syslog.cpp
	fnmatch.h
	fnmatch.cpp
	fts.c
	fts.h
	netdb.h
	netdb.c
	unistd.h
	sys/sem.h
	sys/sem.cpp
	sys/random.h
	sys/random.cpp
	sys/file.h
	sys/file.cpp
	int128/Int128.h
	${INT128_SRC}
)

target_include_directories(libunistd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libunistd LINK_PUBLIC libportable)
