# libunistd/portable/CMakeLists.txt

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
	BsdPacketServer.cpp
	BsdSocket.cpp
	BsdSocketClient.cpp
	BsdSocketPool.cpp
	BsdSocketServer.cpp
	CommandLine.cpp
	Http.cpp
	Network.cpp
	PacketReader.cpp
	Pump.cpp
	VariableClock.cpp
	WormFile.cpp
)

if(WIN32)
include_directories(../vcpp)
endif()

add_library( libportable
	${HEADERS} ${SOURCE}
)

target_include_directories (libportable PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../unistd")