# libunistd/portable/CMakeLists.txt

add_library (libportable
	CommandLine.h
	CommandLine.cpp
	Network.h
	Network.cpp
	SystemCall.h
	SystemCall.cpp
	TimerPump.h
	TimerPump.cpp
	VariableClock.h
	VariableClock.cpp
)

