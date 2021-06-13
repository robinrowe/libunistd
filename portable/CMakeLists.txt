# libunistd/portable/CMakeLists.txt

project(libportable)
message("--- Building library ${PROJECT_NAME} ---")

set(HEADERS
#	Astring.h
	AtomicCounter.h
	AtomicLock.h
	AtomicLock0.h
	AtomicMutex.h
	Breakpoint.h
	ipc/BsdMulticast.h
	ipc/BsdPacketServer.h
	ipc/BsdSocket.h
	ipc/BsdSocketClient.h
	ipc/BsdSocketPool.h
	ipc/BsdSocketServer.h
	ipc/BsdSocketStartup.h
	Buffer.h
	Cfile.h
	CommandLine.h
	Counter.h
	CppTypes.h
	endian.h
	Finder.h
	Folder.h
	homedir.h
	http/HtmlPage.h
	http/Http.h
	http/HttpDecoder.h
	Logger.h
	MsgBuffer.h
	Network.h
	ipc/Packet.h
	ipc/PacketBuffer.h
	ipc/PacketMarker.h
	ipc/PacketQueue.h
	ipc/PacketReader.h
	ipc/PacketSizer.h
	ipc/PacketWriter.h
	QtHelpers.h
	pump/Pump.h
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
	time/Timecode.h
	pump/TimerPump.h
	time/Timespan.h
	time/Timestamp.h
	UnrealLogger.h
	time/VariableClock.h
	Vec3d.h
	VerboseCounter.h
	time/WallClock.h
	time/Watchdog.h
	WormFile.h
)

set(SOURCE
	ipc/BsdPacketServer.cpp
	ipc/BsdSocket.cpp
	ipc/BsdSocketClient.cpp
	ipc/BsdSocketPool.cpp
	ipc/BsdSocketServer.cpp
	ipc/BsdSocketStartup.cpp
	CommandLine.cpp
	http/Http.cpp
	Network.cpp
	ipc/PacketReader.cpp
	pump/Pump.cpp
	time/Timestamp.cpp
	time/VariableClock.cpp
	WormFile.cpp
)

add_library(${PROJECT_NAME}	STATIC ${HEADERS} ${SOURCE})
