# libunistd/portable/CMakeLists.txt

set(NAME libportable)
message("--- Building ${NAME} ---")

set(HEADERS
	Astring.h
	AtomicCounter.h
	AtomicLock.h
	AtomicLock0.h
	AtomicMutex.h
	Breakpoint.h
	BsdMulticast.h
	BSdPacketServer.h
	BsdSocket.h
	BsdSocketClient.h
	BsdSocketPool.h
	BsdSocketServer.h
	BsdSocketStartup.h
	Buffer.h
	Cfile.h
	CommandLine.h
	Counter.h
	CppTypes.h
	endian.h
	Folder.h
	homedir.h
	HtmlPage.h
	Http.h
	HttpDecoder.h
	Logger.h
	MsgBuffer.h
	Network.h
	Packet.h
	PacketBuffer.h
	PacketMarker.h
	PacketQueue.h
	PacketReader.h
	PacketSizer.h
	PacketWriter.h
	QtHelpers.h
	Pump.h
	Random.h
	SoftLock.h
	StdBlob.h
	StdCopy.h
	StdDevice.h
	StdFile.h
	StdPipe.h
	strcpy.h
	stub.h
	SystemCall.h
	thread_semaphore.h
	Timecode.h
	TimerPump.h
	TimeSpan.h
	TimeStamp.h
	UnrealLogger.h
	VariableClock.h
	Vec3d.h
	VerboseCounter.h
	WallClock.h
	Watchdog.h
	WormFile.h
)

set(SOURCE
	ipc/BsdPacketServer.cpp
	ipc/BsdSocket.cpp
	ipc/BsdSocketClient.cpp
	ipc/BsdSocketPool.cpp
	ipc/BsdSocketServer.cpp
	CommandLine.cpp
	http/Http.cpp
	Network.cpp
	ipc/PacketReader.cpp
	pump/Pump.cpp
	time/VariableClock.cpp
	WormFile.cpp
)

if(WIN32)
include_directories(../vcpp)
endif()

add_library(${NAME}	${HEADERS} ${SOURCE})

target_include_directories (${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../unistd")