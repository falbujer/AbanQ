exists(../../../settings.pro):include(../../../settings.pro)
else:error("settings.pro missing, unable to build")
TEMPLATE = lib
TARGET = aqroller
DESTDIR = $$PREFIX/lib
INCLUDEPATH +=  . \
                $$ROOT \
                $$ROOT/tools/qsac
HEADERS +=  AQNodes_p.h \
            AQRoller.h \
            AQGrammar.h \
            $$ROOT/tools/qsac/AQLexer.h
SOURCES +=  AQRoller.cpp \
            AQGrammar.cpp \
            $$ROOT/tools/qsac/AQLexer.cpp
            
CONFIG += warn_off createprl linkprl
!mac:CONFIG += plugin
VERSION = 2.4.0
DEFINES *= AQLEXERCLASS="AQRLexer"
DEFINES *= AQROLLERLEXER
include (../../aqstyle/aqstyle.pri)
