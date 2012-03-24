include(../../uvapplication.pri)

TEMPLATE = app
TARGET = $$UV_APP_TARGET
#QT -= gui
QT += xml

SOURCES += \
    main.cpp \
    uvappconfighandler.cpp \
    uvappconfig.cpp \
    uvnaviserver.cpp \
    uvgdxmanager.cpp \
    uvnaviserver_global.cpp \
    uvgpsmanager.cpp

HEADERS += \
    uvappconfig.h \
    uvappconfighandler.h \
    uvnaviserver.h \
    uvgdxmanager.h \
    uvnaviserver_global.h \
    uvgpsmanager.h

shared {
    LIBS    += -lUvCore1
} else {
    LIBS    += -lUvCore
    LIBS    += -lqextserialport
}

win32::system(SubWCRev.exe $$PWD $$PWD/svn_ver_tpl $$PWD/svn_ver)
include(svn_ver)
VERSION = 1.0.0.$$SVN_VERSION
