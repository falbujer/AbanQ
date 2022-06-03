exists(../../../settings.pro):include(../../../settings.pro)
else:error("settings.pro missing, unable to build")
ROLLER_LIB = $$ROOT/tools/roller/lib
TEMPLATE = app
TARGET = roller
DESTDIR = $$PREFIX/bin
INCLUDEPATH +=  $$ROLLER_LIB \
                $$ROOT \
                $$ROOT/tools/qsac
HEADERS +=  $$ROLLER_LIB/AQNodes_p.h \
            $$ROLLER_LIB/AQRoller.h \
            $$ROLLER_LIB/AQGrammar.h \
            $$ROOT/tools/qsac/AQLexer.h
SOURCES +=  $$ROLLER_LIB/AQRoller.cpp \
            $$ROLLER_LIB/AQGrammar.cpp \
            $$ROOT/tools/qsac/AQLexer.cpp \
            main.cpp
DEFINES *= AQLEXERCLASS="AQRLexer"
DEFINES *= AQROLLERLEXER
include (../../aqstyle/aqstyle.pri)   
LIBS *= -L$$PREFIX/lib -lflbase
