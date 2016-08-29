# libunistd/portable/CMakeLists.txt

add_library (libportable
	Network.h
	Network.cpp
	TimerPump.h
	TimerPump.cpp
	VariableClock.h
	VariableClock.cpp
	CommandLine.cpp
)

if (WIN32)
include_directories(../vcpp)
endif()
