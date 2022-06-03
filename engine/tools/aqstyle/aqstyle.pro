exists(../../settings.pro):include(../../settings.pro)
else:error("settings.pro missing, unable to build")
TEMPLATE = app
TARGET = aqstyle
DESTDIR = $$PREFIX/bin
INCLUDEPATH += .
load(qsa)
LIBS *= -L$$PREFIX/lib -lflbase
HEADERS += AQStyle.h
SOURCES += AQStyle.cpp \
    main.cpp
