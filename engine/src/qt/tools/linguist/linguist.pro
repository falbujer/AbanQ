TEMPLATE	= subdirs
SUBDIRS		= linguist \
		  lrelease \
		  lupdate \
		  qm2ts
CONFIG += ordered

REQUIRES=full-config #nocrosscompiler
