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

include(../../../qsac/qsac.pri)

TEMPLATE = app
TARGET = qsac_server
DESTDIR = $$BUILDDIR/bin
CONFIG -= moc
CONFIG += exceptions

INCLUDEPATH += . $$ROOT $$ROOT/src/flbase
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
        
SOURCES += qsac.cpp
