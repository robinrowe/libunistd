
TEMPLATE = lib

TARGET = libsqlite
VERSION = 1.0

QT -= gui
CONFIG += static c++11 console

PWD_UP = $$dirname(PWD)
INCLUDEPATH += $$PWD_UP
HEADERS = $$PWD_UP/*.h
SOURCES = $$PWD_UP/*.c $$PWD_UP/*.cpp

DESTDIR = $$PWD_UP/lib

