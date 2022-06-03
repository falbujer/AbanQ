exists(../../settings.pro):include(../../settings.pro)
else:error("settings.pro missing, unable to build")
TEMPLATE = app
TARGET=packager
DESTDIR = $$PREFIX/bin
INCLUDEPATH += . \
    $$ROOT/src/flbase
HEADERS += AQUnpacker.h \
    $$ROOT/src/flbase/FLSha1.h \
    AQPackager.h
SOURCES += AQUnpacker.cpp \
    $$ROOT/src/flbase/FLSha1.cpp \
    AQPackager.cpp \
    main.cpp
include (../qsac/qsac.pri)
include (../roller/roller.pri)
LIBS *= -L$$PREFIX/lib -lflbase