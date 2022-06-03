exists(../../../../settings.pro) {
        include(../../../../settings.pro)
} else {
        error("settings.pro missing, unable to build")
}

exists(../../settings.pro) {
        include(../../settings.pro)
} else {
        error("settings.pro missing, unable to build")
}

TEMPLATE = app
TARGET = abanq_remote
DESTDIR = $$BUILDDIR/bin
CONFIG += exceptions
CONFIG += warn_off link_prl qt 
unix:CONFIG += x11
CONFIG -= dll

INCLUDEPATH += .
INCLUDEPATH += $$ROOT/src/qt/src/kernel
INCLUDEPATH += $$ROOT $$ROOT/src/flbase $$ROOT/src/flbase/aqsobjects
DEPENDPATH += $$ROOT/src/flbase

LIBS *= -L$$PREFIX/lib
enable_hoard:LIBS *= -lhoard
enable_hoard:!win32:LIBS *= -L$$PREFIX/lib -ldl
LIBS *= -L$$PREFIX/lib -lflbase -lqsa
mac:LIBS *= -ladvance -lqui -lflmail -lqwt


LIBS += $$BUILDDIR/lib/libxmlrpc_server_abyss++.a \
        $$BUILDDIR/lib/libxmlrpc_server++.a \
        $$BUILDDIR/lib/libxmlrpc_server_abyss.a \
        $$BUILDDIR/lib/libxmlrpc_server.a \
        $$BUILDDIR/lib/libxmlrpc_abyss.a \
        $$BUILDDIR/lib/libxmlrpc++.a \
        $$BUILDDIR/lib/libxmlrpc.a \
        $$BUILDDIR/lib/libxmlrpc_util.a \
        $$BUILDDIR/lib/libxmlrpc_xmlparse.a \
        $$BUILDDIR/lib/libxmlrpc_xmltok.a 

HEADERS += AQApplicationXmlRpc.h
SOURCES += AQApplicationXmlRpc.cpp main.cpp

