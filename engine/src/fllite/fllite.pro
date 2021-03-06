exists(../../settings.pro) {
	include(../../settings.pro)
}
else {
	error("settings.pro missing, unable to build")
}

TEMPLATE = app 

CONFIG += warn_off link_prl qt 
unix:CONFIG += x11
CONFIG -= dll

win32:RC_FILE = fllite.rc
mac:RC_FILE = fl.icns

DESTDIR = $$PREFIX/bin

mac {
  TARGET = AbanQ
} else {
  TARGET = abanq 
}

INCLUDEPATH += $$ROOT/src/qt/src/kernel
INCLUDEPATH += $$ROOT $$ROOT/src/flbase
win32:INCLUDEPATH += $$ROOT/src/fllite
DEPENDPATH += $$ROOT/src/flbase

LIBS *= -L$$PREFIX/lib
enable_hoard:LIBS *= -lhoard
enable_hoard:!win32:LIBS *= -L$$PREFIX/lib -ldl
LIBS *= -L$$PREFIX/lib -lflbase
mac:LIBS *= -ladvance -lqui -lflmail -lqwt -lxslt -lexslt -lxsltproc
enable_digidoc:mac:LIBS *= -llibdigidoc -lcrypto -lssl -lxml2

load(qsa)

SOURCES += main.cpp
win32:SOURCES += backtrace.c
win32:HEADERS += execinfo.h

contains(DEFINES, FL_QUICK_CLIENT) {
	IMAGES += images/splashabanq.png
} else {
	IMAGES += images/splashabanq.png
}

win32:QMAKE_LFLAGS += -mwindows
