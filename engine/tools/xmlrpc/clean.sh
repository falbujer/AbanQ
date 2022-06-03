#!/bin/bash

BASEDIR=$PWD
BUILDDIR=$BASEDIR/build

cd stable
make distclean
rm -f src/test/srcdir
rm -r src/test/blddir
rm -f src/test/depend.mk
rm -vfR $BUILDDIR

cd $BASEDIR

cd servers
make distclean

find . -name "Makefile" -exec rm -vf \{\} \;
find . -name ".moc" -type d -exec rm -vfR \{\} \;
find . -name ".obj" -type d -exec rm -vfR \{\} \;
find . -name ".uic" -type d -exec rm -vfR \{\} \;
find . -name ".tmp" -type d -exec rm -vfR \{\} \;
find . -name "*.qm" -exec rm -vf \{\} \;
find . -name "*.o" -exec rm -vf \{\} \;
find . -name "*.moc" -exec rm -vf \{\} \;
find . -name ".*.cache" -exec rm -vf \{\} \;
find . -name "*.prl" -exec rm -vf \{\} \;
find . -name "*.*.orig" -exec rm -vf \{\} \;

cd $BASEDIR

rm -f settings.pro

