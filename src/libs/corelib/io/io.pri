HEADERS += \
    io/uvserialport.h \
    io/uvserialportdef.h

SOURCES += \
    io/uvserialport.cpp

shared {
    LIBS    += -lqextserialport1
} else {
    LIBS    += -lqextserialport
}
