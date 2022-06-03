#!/bin/bash

BASEDIR=$PWD
BUILDDIR=$BASEDIR/build

mkdir -p $BUILDDIR

cd stable
./configure --prefix=$BUILDDIR --disable-wininet-client --disable-curl-client --disable-libwww-client \
            -disable-cgi-server
make
make install

cd $BASEDIR

echo "BUILDDIR = $BUILDDIR" >> settings.pro
echo "INCLUDEPATH *= $BUILDDIR/include" >> settings.pro
echo "LIBS *= -L$BUILDDIR/lib" >> settings.pro
echo "CONFIG += warn_off" >> settings.pro
#echo "DEFINES *= AQ_XMLRPC_RAW_RESPONSE" >> settings.pro
#echo "DEFINES *= AQ_XMLRPC_NOLOG" >> settings.pro

cd servers
cd abanq_remote
qmake
cd ..
qmake -nocache
make

cd $BUILDDIR
ln -s $QTDIR/share
ln -s $QTDIR/plugins

cd $BASEDIR

