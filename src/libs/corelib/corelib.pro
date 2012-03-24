include(../../uvlibrary.pri)
include(global/global.pri)
include(io/io.pri)
include(communication/communication.pri)
include(tools/tools.pri)
include(ipc/ipc.pri)

TEMPLATE = lib
TARGET   = UvCore
!win32 {
    QT      -= gui
}

CONFIG += qt
CONFIG += warn_on
CONFIG += thread

#DEFINES += CORE_SHARED_LIBRARY
DEFINES += CORE_STATIC_LIBRARY

HEADERS += \
    corelib_global.h

win32::system(SubWCRev.exe $$PWD $$PWD/svn_ver_tpl $$PWD/svn_ver)
include(svn_ver)
VERSION = 1.0.0.$$SVN_VERSION
