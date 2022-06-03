#!/bin/bash

rm -f install

export QTDIR=$PWD/abanq-build
export PATH=$QTDIR/bin:$PATH

make -s clean
make -s distclean

cd src
make -s clean
make -s distclean

cd qsa
qmake
cd configure2
qmake
cd ../src/qsa
make -s clean
make -s distclean
cd ../..
make -s clean
make -s distclean
cd ../..

cd src/qt
make -s clean
make -s distclean
rm -f include/*.h
rm -f include/private/*.h
rm -f $HOME/.qmake.cache
rm -vf .qmake.cache
rm -vfR ./lib/lib*
rm -vfR ./lib/*.dll
cd qmake
make -s clean
make -s distclean
cd ../bin
rm -vf *
cd ../../../

rm -f      src/qsa/tools/designer/editor/preferences.cpp
rm -f      src/qsa/tools/designer/editor/preferences.h
rm -f      src/qsa/src/ide/idewindow.h
rm -f      src/qsa/src/ide/outputcontainer.cpp
rm -f      src/qsa/src/ide/preferencescontainer.cpp
rm -f      src/qsa/src/ide/outputcontainer.h
rm -f      src/qsa/src/ide/preferencescontainer.h
rm -f      src/qsa/src/ide/projectcontainer.cpp
rm -f      src/qsa/src/ide/projectcontainer.h
rm -f      src/qsa/src/ide/gotoline.cpp
rm -f      src/qsa/src/ide/replacetext.cpp
rm -f      src/qsa/src/ide/gotoline.h
rm -f      src/qsa/src/ide/replacetext.h
rm -f      src/qsa/src/ide/findtext.cpp
rm -f      src/qsa/src/ide/idewindow.cpp
rm -f      src/qsa/src/ide/findtext.h
rm -f      src/qsa/src/qsa/qmake_image_collection.cpp
rm -f      src/qt/lib/qt-mt.pc
rm -f      src/qt/lib/qt.pc
rm -f      src/qt/config.status
rm -f      src/qsa/configure2/configure2
rm -f	     src/qt/src/tools/qconfig.cpp

rm -f      src/qt/config.tests/unix/largefile/largefiletest
rm -f      src/qt/config.tests/unix/endian/endiantest
rm -f      src/qt/config.tests/unix/ptrsize/ptrsizetest
rm -f	     src/qt/config.tests/unix/ipv6/ipv6test

rm -f      src/qt/config.tests/unix/largefile/largefiletest.exe
rm -f      src/qt/config.tests/unix/endian/endiantest.exe
rm -f      src/qt/config.tests/unix/ptrsize/ptrsizetest.exe
rm -f	     src/qt/config.tests/unix/ipv6/ipv6test.exe

rm -f	     src/qt/mkspecs/default

rm -f	     src/qt/tools/designer/app/hoard.pri

find . -name "abanq-build" -exec rm -vfR \{\} \;
find . -name "*~" -exec rm -vf \{\} \;
find . -name "*.bak" -exec rm -vf \{\} \;
find . -name "*.old" -exec rm -vf \{\} \;
for a in $(find . -name "*.pro" -print) 
do
	cat $a | sed "s/PREFIX\ =\ .*/PREFIX=/" | sed "s/ROOT\ =\ .*/ROOT=/" | sed "s/INCLUDE_PGSQL\ =\ .*/INCLUDE_PGSQL=/" | sed "s/LIB_PGSQL\ =\ .*/LIB_PGSQL=/" > ./temp
	mv -f temp $a
done
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

rm -vf ./AQConfig.*
rm -vf ./src/translations/translations.pro
rm -vf *.kdevses
rm -vf *.pcs
rm -vf settings.pro
rm -vfR ./bin ./lib ./share

if [ -d qconfig ]; then
find . -name "C:*" -exec rm -fR \{\} \;
find . -name "*_ld_script" -exec rm -fR \{\} \;
find . -name ".o" -type d -exec rm -fR \{\} \;

rm -vfR       src/qt/phrasebooks
rm -vfR       src/qt/templates
rm -vfR       src/qt/share
rm -vfR       src/qt/plugins/designer
rm -vfR       src/qt/plugins/codecs
rm -vfR       src/qt/plugins/styles
rm -vfR       src/qt/plugins/accessible
rm -vfR       src/qt/plugins/imageformats
rm -vfR       src/qt/plugins/sqldrivers
rm -vfR       src/qt/include/qconfig.h.n
rm -vfR       src/qt/include/qmodules.h.n
rm -vfR       src/qt/include/qmake
rm -vfR       src/qt/src/Makefile.main
rm -vfR       src/qt/src/tools/qconfig.cpp.n
rm -vfR       src/qt/mkspecs/linux-g++/linux-g++
rm -vfR       src/qt/src/tmp
fi

rm -vfR       src/qt/plugins/accessible
rm -vfR       src/qt/include/qmake
rm -vfR       src/qt/config.tests/mac/tst_mac_version
rm -vfR       src/qt/config.tests/mac/mac_version.cpp

rm -f clean


