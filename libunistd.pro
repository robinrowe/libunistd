TEMPLATE = subdirs

#CONFIG += create_prl

SUBDIRS = portable xxhash
include(libunistd.pri)

win32 
{	SUBDIRS += unistd
}

message(libunistd SUBDIRS = $$SUBDIRS)
message(libunistd INCLUDEPATH = $$INCLUDEPATH)
message(libunistd LIBS = $$LIBS)
message(libunistd _PRO_FILE_PWD_ = $$_PRO_FILE_PWD_)
message(libunistd OUT_PWD = $$OUT_PWD)
#C:/Code/github/libunistd/portable

#message(libunistd OUT_DIR = $$OUT_DIR)
#message(DESTDIR = $$DESTDIR)
#message(DESTDIR_TARGET = $$DESTDIR_TARGET)
#QMAKE_POST_LINK = copy $$OUT_PWD\debug\*.lib  ..\my-lib
