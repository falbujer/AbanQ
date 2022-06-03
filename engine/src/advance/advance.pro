exists(../../settings.pro) {
	include(../../settings.pro)
}
else {
	error("settings.pro missing, unable to build")
}

TEMPLATE = lib

CONFIG += warn_off createprl linkprl qt
!mac:CONFIG += plugin
unix:CONFIG += x11

win32 {
        CONFIG += dll shared
        DLLDESTDIR = $$PREFIX/bin
}
shared {
	win32:DEFINES	+= AQADV_DLL
} else {
	win32:DEFINES += AQADV_NO_DLL
}
DESTDIR = $$PREFIX/lib

INCLUDEPATH += $$ROOT

LIBS += -L$$PREFIX/lib

TARGET = advance

VERSION = 0.8.0

SOURCES += vdatepopup.cpp vdatetable.cpp

HEADERS += aqadvglobal.h vdatepopup.h vdatetable.h

IMAGES += images/1leftarrow.png images/1rightarrow.png \ 
	images/2leftarrow.png images/2rightarrow.png
