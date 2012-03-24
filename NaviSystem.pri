defineReplace(cleanPath) {
    win32:1 ~= s|\\\\|/|g
    contains(1, ^/.*):pfx = /
    else:pfx =
    segs = $$split(1, /)
    out =
    for(seg, segs) {
        equals(seg, ..):out = $$member(out, 0, -2)
        else:!equals(seg, .):out += $$seg
    }
    return($$join(out, /, $$pfx))
}

defineReplace(targetPath) {
    return($$replace(1, /, $$QMAKE_DIR_SEP))
}

UV_SOURCE_TREE = $$PWD
isEmpty(UV_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    UV_BUILD_TREE = $$cleanPath($$OUT_PWD)
    UV_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
    UV_BUILD_TREE = $$UV_BUILD_TREE/out
}

isEmpty(UV_LIBRARY_BASENAME) {
    UV_LIBRARY_BASENAME = lib
}

DEFINES += UV_LIBRARY_BASENAME=\\\"$$UV_LIBRARY_BASENAME\\\"

isEmpty(UV_DOCS_BASENAME) {
    UV_DOCS_BASENAME = docs
}

DEFINES += UV_DOCS_BASENAME=\\\"$$UV_DOCS_BASENAME\\\"

UV_APP_PATH = $$UV_BUILD_TREE/bin
win32 {
    UV_APP_TARGET   = Navigation
} else {
    UV_APP_WRAPPER  = Navigation
    UV_APP_TARGET   = Navigation.bin
}
UV_LIBRARY_PATH = $$UV_BUILD_TREE/$$UV_LIBRARY_BASENAME
UV_DOCS_PATH    = $$UV_BUILD_TREE/$$UV_DOCS_BASENAME

INCLUDEPATH += \
    $$UV_SOURCE_TREE/src/libs

DEPENDPATH += \
    $$UV_SOURCE_TREE/src/libs

LIBS += -L$$UV_LIBRARY_PATH

#For strings operation
#DEFINES += QT_NO_CAST_FROM_ASCII
DEFINES += QT_NO_CAST_TO_ASCII
!macx:DEFINES += QT_USE_FAST_OPERATOR_PLUS QT_USE_FAST_CONCATENATION

win32-msvc* { 
    #Don't warn about sprintf, fopen etc being 'unsafe'
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

OBJECTS_DIR = $$UV_BUILD_TREE/tmp
MOC_DIR = $$UV_BUILD_TREE/tmp
RCC_DIR = $$UV_BUILD_TREE/tmp
UI_DIR = $$UV_BUILD_TREE/tmp

#Show debug message or not
CONFIG += show_debug_msg
show_debug_msg {
#    CONFIG += console
} else {
#    CONFIG += console
    DEFINES += QT_NO_DEBUG_OUTPUT \
               QT_NO_DEBUG
}

shared {
    DEFINES *= QT_LIB_SHARED
} else {
    DEFINES *= QT_LIB_STATIC
}
