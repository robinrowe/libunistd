TEMPLATE = lib
CONFIG += staticlib

include(../libunistd.pri)

HEADERS = \
	chrono.h \
	dirent.h \
	dlfcn.h \
	endian.h \
	gettimeofday.h \
	grp.h \
	ifaddrs.h \
	libgen.h \
	libintl.h \
	magic.h \
	mqueue.h \
	netdb.h \
	poll.h \
	pthread.h \
	pwd.h \
	semaphore.h \
	strings.h \
	termios.h \
	uni_signal.h \
	unistd.h \
	arpa/inet.h \
	linux/rtc.h \
	net/if.h \
	net/route.h \
	netinet/in.h \
	netinet/ip.h \
	netinet/ip_icmp.h \
	sys/epoll.h \
	sys/file.h \
	sys/inotify.h \
	sys/ioctl.h \
	sys/mman.h \
	sys/poll.h \
	sys/prctl.h \
	sys/resource.h \
	sys/select.h \
	sys/socket.h \
	sys/socketvar.h \
	sys/statvfs.h \
	sys/sys_types.h \
	sys/syscall.h \
	sys/time.h \
	sys/vfs.h \
	sys/wait.h

SOURCES = \
	dirent.cpp \
	gettimeofday.cpp \
	uni_signal.cpp
