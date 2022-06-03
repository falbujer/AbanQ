QSAC_DIR = $$ROOT/tools/qsac

INCLUDEPATH += $$QSAC_DIR $$ROOT

HEADERS += $$QSAC_DIR/AQCompilerQSA.h \
           $$QSAC_DIR/AQGrammar.h \
           $$QSAC_DIR/AQLexer.h

SOURCES += $$QSAC_DIR/AQCompilerQSA.cpp \
	         $$QSAC_DIR/AQGrammar.cpp \
	         $$QSAC_DIR/AQLexer.cpp
DEFINES *= AQLEXERCLASS="AQLexer"
