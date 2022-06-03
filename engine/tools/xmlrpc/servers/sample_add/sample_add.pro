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
TARGET=sample_add_server
DESTDIR = $$BUILDDIR/bin
CONFIG -= moc
CONFIG += exceptions

INCLUDEPATH += .
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
        
SOURCES += sample_add.cpp
