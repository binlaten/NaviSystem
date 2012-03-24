include(../../../uvlibrary.pri)

TEMPLATE = lib
TARGET = qextserialport

CONFIG += qt
CONFIG += warn_on
CONFIG += thread

QT -= gui

#DEFINES += QESP_SHARED_LIBRARY
DEFINES += QESP_STATIC_LIBRARY

HEADERS += \
    qextserialport.h \
    qextserialbase.h \
    qesp_global.h

SOURCES += \
    qextserialport.cpp \
    qextserialbase.cpp

unix:HEADERS += posix_qextserialport.h
unix:SOURCES += posix_qextserialport.cpp

win32 {
    wince* {
        HEADERS += wince_qextserialport.h
        SOURCES += wince_qextserialport.cpp
    } else {
        HEADERS += win_qextserialport.h
        SOURCES += win_qextserialport.cpp
        LIBS    += -lsetupapi
    }
}

win32::system(SubWCRev.exe $$PWD $$PWD/svn_ver_tpl $$PWD/svn_ver)
include(svn_ver)
VERSION = 1.3.1.$$SVN_VERSION
