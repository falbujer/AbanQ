exists(../../settings.pro) {
        include(../../settings.pro)
} else {
        error("settings.pro missing, unable to build")
}

TEMPLATE = app
TARGET=qsac
DESTDIR = $$PREFIX/bin
INCLUDEPATH += . $$ROOT $$ROOT/src/flbase
HEADERS += AQCompilerQSA.h \
           AQGrammar.h \
           AQLexer.h
SOURCES += AQCompilerQSA.cpp \
	   AQGrammar.cpp \
	   AQLexer.cpp \
	   main.cpp
DEFINES *= AQLEXERCLASS="AQLexer"
