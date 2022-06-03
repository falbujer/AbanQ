#!/bin/bash

VER="2.5"

OPT_PREFIX=""
OPT_QMAKESPEC=""
OPT_DEBUG=no
OPT_SQLLOG=no
OPT_HOARD=no
OPT_QWT=yes
OPT_DIGIDOC=yes
OPT_MULTICORE=yes
OPT_AQ_DEBUG=no
QT_DEBUG="-release -DQT_NO_CHECK"
QSADIR=qsa

if [ "$BUILD_NUMBER" == "" ]; then
  BUILD_NUMBER="0"
fi

VERSION="$VER Developer Build $BUILD_NUMBER"
BUILD_KEY="$VER-Build-$BUILD_NUMBER"

for a in "$@"; do
  case "$a" in
    -debug)
      OPT_DEBUG=yes
    ;;
    -aqdebug)
      OPT_AQ_DEBUG=yes
    ;;
    -qtdebug)
      QT_DEBUG="-debug"
    ;;
    -sqllog)
      OPT_SQLLOG=yes
    ;;
    -hoard)
      OPT_HOARD=yes
    ;;
    -no-hoard)
      OPT_HOARD=no
    ;;
    -qwt)
      OPT_QWT=yes
    ;;
    -digidoc)
      OPT_DIGIDOC=yes
    ;;
    -no-digidoc)
      OPT_DIGIDOC=no
    ;;
    -prefix|-platform)
      VAR=`echo $a | sed "s,^-\(.*\),\1,"`
      shift
      VAL="yes"
    ;;
    *)
    if [ "$VAL" == "yes" ];then
      VAL=$a
    fi
    ;;
  esac
  case "$VAR" in
    prefix)
    if  [ $VAL != "yes" ];then
      OPT_PREFIX=$VAL
    fi
    ;;
    platform)
    if  [ $VAL != "yes" ];then
      OPT_QMAKESPEC=$VAL
      export QMAKESPEC=$OPT_QMAKESPEC
    fi
    ;;
  esac
done

CMD_MAKE="make -s "
MAKE_INSTALL=""

if [ "$OPT_MULTICORE" == "yes" ]; then
  PROCESSORS=$(expr  $(cat /proc/cpuinfo | grep processor | tail -n 1 | sed "s/.*:\(.*\)/\1/") + 1)
  CMD_MAKE="make -k -j $PROCESSORS -s "
fi
QT_DEBUG="$QT_DEBUG -DQT_NO_COMPAT"

if [ "$OPT_QMAKESPEC" == "" ]; then
  case `uname -m` in
  amd64 | x86_64)
    OPT_QMAKESPEC="linux-g++-64"
    export QMAKESPEC=$OPT_QMAKESPEC
  ;;
  *)
    OPT_QMAKESPEC="linux-g++"
    export QMAKESPEC=$OPT_QMAKESPEC
  ;;
  esac
fi

echo -e "\nUtilidad de compilacion e instalacion de AbanQ $VERSION"
echo -e "(C) 2003-2013 InfoSiAL, S.L. http://infosial.com - http://abanq.org\n"

MAKE_INSTALL="install"

if [ "$OPT_PREFIX" == "" ]
then
  echo -e "AVISO : No se ha especificado directorio de instalacion"
  echo -e "Uso :  $0 directorio_de_instalacion\n"
  DIRINST=$PWD/abanq-build
  echo -e "Utilizando por defecto el directorio $DIRINST\n"
else
  DIRINST=$OPT_PREFIX
fi

mkdir -p $DIRINST

if [ ! -w $DIRINST ]
then
  echo -e "ERROR : Actualmente no tienes permisos de escritura en el directorio de instalacion ($DIRINST)."
  echo -e "Solucion : Cambia los permisos o ejecuta este script como un usuario que tenga permisos de escritura en ese directorio.\n"
  exit 1
fi

BASEDIR=$PWD
PREFIX=$DIRINST

echo -e "Directorio de instalacion : $PREFIX\n"

echo -e "Estableciendo configuracion...\n"

cd $BASEDIR/src/libdigidoc/openssl/crypto
rm -f opensslconf.h
if [ "$OPT_QMAKESPEC" == "linux-g++-64" ]; then
  ln -s opensslconf.h.64 opensslconf.h
else
  ln -s opensslconf.h.32 opensslconf.h
fi
cd $BASEDIR

rm -f $HOME/.qmake.cache

export QTDIR=$BASEDIR/src/qt

if [ ! -f $QTDIR/include/qglobal.h ]
then
  cd $QTDIR
  make -s -f Makefile.cvs
  cd $BASEDIR
fi

version=$(cat $QTDIR/include/qglobal.h | grep "QT_VERSION_STR" | sed "s/.*\"\(.*\)\"/\1/g")
echo -e "Version de Qt... $version\n"
echo -e "Compilando qmake y moc...\n"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib
cd $QTDIR

if [ "$OPT_HOARD" = "yes" ]
then
  echo "CONFIG *= enable_hoard" >> tools/designer/app/hoard.pri
fi

cp -vf Makefile.qt Makefile
export ORIGIN=\\\$\$ORIGIN
./configure -platform $OPT_QMAKESPEC -prefix $PREFIX -R'$$(ORIGIN)/../lib' -L$PREFIX/lib -L/usr/lib/x86_64-linux-gnu $QT_DEBUG -thread -stl \
            -no-pch -no-exceptions -buildkey $BUILD_KEY -xinerama -disable-opengl -no-cups \
            -no-nas-sound -no-nis -qt-libjpeg -qt-gif -qt-libmng -qt-libpng -qt-imgfmt-png -qt-imgfmt-jpeg -qt-imgfmt-mng

make -s qmake-install || exit 1
make -s moc-install || exit 1

cd $BASEDIR

export PATH=$PREFIX/bin:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PREFIX/lib

echo -e "\nComprobando qmake...\n"
$QTDIR/bin/qmake -v > /dev/null
if [ $? = 154 ]
then
  echo -e "OK : qmake encontrado\n"
else
  echo -e "ERROR : No se encuentra qmake, esta utilidad se proporciona con las Qt."
  echo -e "        Comprueba que se encuentra en la ruta de busqueda (variable $PATH).\n"
  exit 1
fi

cd $BASEDIR

mkdir -p $PREFIX/share/abanq/forms
mkdir -p $PREFIX/share/abanq/tables
mkdir -p $PREFIX/share/abanq/translations
mkdir -p $PREFIX/share/abanq/scripts
mkdir -p $PREFIX/share/abanq/queries
mkdir -p $PREFIX/share/abanq/reports
mkdir -p $PREFIX/share/abanq/tmp
mkdir -p $PREFIX/share/abanq/doc
mkdir -p $PREFIX/share/abanq/packages
mkdir -p $PREFIX/lib
mkdir -p $PREFIX/bin

export CROSS=i586-mingw32msvc-
export AQ_CIN="C=(~C)^7"
export AQ_PACK_VER="(qstrlen(V) > 0 && qstrcmp(AQPACKAGER_VERSION, V) == 0);Q_UINT32 a; (*stream_) >> a;Q_UINT32 b; (*stream_) >> b;Q_UINT32 c; (*stream_) >> c;if (c && (time(0) - c) > 0x3F480) stream_->setByteOrder(!(a|b))"
export AQ_ENC_KEY="char enckey[32] =  { 0x22, 0xff, 0xe0, 0x12, 0x20, 0x40, 0xab, 0xc2,0x12, 0xaf, 0xe0, 0x22, 0x26, 0x40, 0xab, 0xf2, 0x32, 0xdf, 0xe3, 0x32, 0x27, 0x44, 0xfb, 0xe2, 0x52, 0xef, 0xe2, 0x42, 0x28, 0x45, 0xcb, 0xa2 }"
export AQ_ENC_KEYVI="char enckeyvi[32] =  { 0x32, 0xdf, 0xe3, 0x32, 0x27, 0x44, 0xfb, 0xe2, 0x82, 0xbf, 0xe0, 0x12, 0x20, 0x10, 0x1b, 0xc2, 0x52, 0xef, 0xe2, 0x42, 0x28, 0x45, 0xcb, 0x42, 0x32, 0x8f, 0xe3, 0x32, 0x27, 0x44, 0xfb, 0xd2 }"
export _JAVA_OPTIONS='-Dsun.java2d.pmoffscreen=false -Dsun.java2d.noddraw=true'

if [ "$AQ_CIN" == "" ]; then
  AQ_CIN="C = C"
fi

if [ "$AQ_PACK_VER" == "" ]; then
  AQ_PACK_VER="(qstrlen(V) > 0 && qstrcmp(AQPACKAGER_VERSION, V) == 0)"
fi

if [ "$AQ_ENC_KEY" == "" ]; then
  AQ_ENC_KEY="char enckey[32]"
fi

if [ "$AQ_ENC_KEYVI" == "" ]; then
  AQ_ENC_KEYVI="char enckeyvi[32]"
fi

cat > AQConfig.h <<EOF
// ** $(date +%d%m%Y):$PREFIX -> AQConfig.h
// ** AQConfig.h  Generated by $0
// ** WARNING!    All changes made in this file will be lost!

#ifndef AQCONFIG_H_
#define AQCONFIG_H_

#include "AQGlobal.h"

#define AQ_DIRAPP                   AQConfig::aqDirApp
#define AQ_PREFIX                   AQ_DIRAPP
#define AQ_QTDIR                    AQ_DIRAPP
#define AQ_KEYBASE                  AQConfig::aqKeyBase
#define AQ_DATA                     AQConfig::aqData
#define AQ_LIB                      AQConfig::aqLib
#define AQ_BIN                      AQConfig::aqBin
#define AQ_USRHOME                  AQConfig::aqUsrHome
#define AQ_VERSION                  "$VERSION"
#define AQ_CIN(C)                   $AQ_CIN
#define AQPACKAGER_VERSION_CHECK(V) $AQ_PACK_VER
#define AQ_ENC_KEY	            $AQ_ENC_KEY
#define AQ_ENC_KEYVI		    $AQ_ENC_KEYVI

class QApplication;
class FLApplication;

class AQ_EXPORT AQConfig
{
public:
  static QString aqDirApp;
  static QString aqKeyBase;
  static QString aqData;
  static QString aqLib;
  static QString aqBin;
  static QString aqUsrHome;
        
private:
  static void init(QApplication *);
  friend class FLApplication;
};

#endif /*AQCONFIG_H_*/
EOF

cat > AQConfig.cpp <<EOF
// ** $(date +%d%m%Y):$PREFIX -> AQConfig.cpp
// ** AQConfig.cpp  Generated by $0
// ** WARNING!    All changes made in this file will be lost!

#include <qapplication.h>
#include <qdir.h>

#include "AQConfig.h"

AQ_EXPORT QString AQConfig::aqDirApp;
AQ_EXPORT QString AQConfig::aqKeyBase;
AQ_EXPORT QString AQConfig::aqData;
AQ_EXPORT QString AQConfig::aqLib;
AQ_EXPORT QString AQConfig::aqBin;
AQ_EXPORT QString AQConfig::aqUsrHome;

void AQConfig::init(QApplication *aqApp)
{
#if defined(Q_OS_MACX)
  aqDirApp = QDir::cleanDirPath(aqApp->applicationDirPath() + QString::fromLatin1("/../../../.."));
#else
  aqDirApp = QDir::cleanDirPath(aqApp->applicationDirPath() + QString::fromLatin1("/.."));
#endif
  aqKeyBase = QString::fromLatin1("AbanQ/");
  aqData = aqDirApp + QString::fromLatin1("/share/abanq");
  aqLib = aqDirApp + QString::fromLatin1("/lib");
  aqBin = aqDirApp + QString::fromLatin1("/bin");
  aqUsrHome = QDir::cleanDirPath(QDir::home().absPath());
}
EOF

echo "include(./includes.pri)" > settings.pro
echo "PREFIX = $PREFIX" >> settings.pro
echo "ROOT = $BASEDIR" >> settings.pro
echo "INCLUDEPATH *= $PREFIX/include" >> settings.pro
echo "INCLUDEPATH *= $BASEDIR/src/qt/src/tmp" >> settings.pro
echo "CONFIG += warn_off" >> settings.pro
echo "DEFINES *= QSDEBUGGER" >> settings.pro
echo "AQSSLVERSION = 0.9.8" >> settings.pro
echo "DEFINES *= AQSTRSSLVERSION=\\\"098\\\"" >> settings.pro
#echo "DEFINES *= AQ_NO_PRINT_FUN" >> settings.pro
#echo "DEFINES *= AQ_NO_DEBUG_FUN" >> settings.pro

if [ "$OPT_DEBUG" = "yes" ]
then
  echo "CONFIG *= debug" >> settings.pro
  echo "DEFINES *= FL_DEBUG" >> settings.pro
fi

if [ "$OPT_AQ_DEBUG" = "yes" ]
then
  echo "DEFINES *= AQ_DEBUG" >> settings.pro
fi

if [ "$OPT_SQLLOG" = "yes" ]
then
  echo "DEFINES *= FL_SQL_LOG" >> settings.pro

fi

if [ "$OPT_QWT" = "yes" ]
then
  echo "CONFIG *= enable_qwt" >> settings.pro
  echo "DEFINES *= FL_QWT" >> settings.pro
fi

if [ "$OPT_DIGIDOC" = "yes" ]
then
  echo "CONFIG *= enable_digidoc" >> settings.pro
  echo "DEFINES *= FL_DIGIDOC" >> settings.pro
fi

if [ "$OPT_HOARD" = "yes" ]
then
  echo "CONFIG *= enable_hoard" >> settings.pro
  echo "DEFINES *= FL_HOARD" >> settings.pro
  echo -e "\nCompilando Hoard...\n"
  cd $BASEDIR/src/hoard
  $QTDIR/bin/qmake hoard.pro
  $CMD_MAKE || exit 1
  cd $BASEDIR
fi

echo -e "\nCompilando Qt ($QTDIR) ...\n"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib:$PREFIX/lib

cd $QTDIR
$CMD_MAKE -w $MAKE_INSTALL || exit 1

export QTDIR=$PREFIX

echo "Compilando QSA..."
cd $BASEDIR/src/$QSADIR
cp -f ../qt/.qmake.cache .qmake.cache
cp -f ../qt/.qmake.cache src/$QSADIR/
cp -f ../qt/.qmake.cache src/plugin/

./configure

$CMD_MAKE
make -s $MAKE_INSTALL
$CMD_MAKE
make -s $MAKE_INSTALL
$CMD_MAKE
make -s $MAKE_INSTALL

cd $BASEDIR

echo -e "Creando Makefiles con qmake...\n"

cp -f src/qt/.qmake.cache .qmake.cache
cp -f src/qt/.qmake.cache src/advance/
cp -f src/qt/.qmake.cache src/qwt/
cp -f src/qt/.qmake.cache src/qwt/designer
cp -f src/qt/.qmake.cache src/barcode/
cp -f src/qt/.qmake.cache src/flbase/
cp -f src/qt/.qmake.cache src/fllite/
cp -f src/qt/.qmake.cache src/flbase/
cp -f src/qt/.qmake.cache src/kugar/
cp -f src/qt/.qmake.cache src/sqlite/
cp -f src/qt/.qmake.cache src/libpq/
cp -f src/qt/.qmake.cache src/dbf/
cp -f src/qt/.qmake.cache src/plugins/designer/flfielddb/
cp -f src/qt/.qmake.cache src/plugins/designer/fltabledb/
cp -f src/qt/.qmake.cache src/plugins/sqldrivers/sqlite/
cp -f src/qt/.qmake.cache src/plugins/sqldrivers/psql/
cp -f src/qt/.qmake.cache src/plugins/styles/bluecurve/

$QTDIR/bin/qmake developer.pro

echo -e "Compilando...\n"
cd src/flbase
$QTDIR/bin/qmake flbase.pro

make -s uicables || exit 1

cd $BASEDIR
$CMD_MAKE
make -s $MAKE_INSTALL
$CMD_MAKE
make -s $MAKE_INSTALL
$CMD_MAKE || exit 1
make -s $MAKE_INSTALL

cd $BASEDIR/src/libdigidoc/openssl/crypto
rm -f opensslconf.h
ln -s opensslconf.h.32 opensslconf.h
cd $BASEDIR

if [ "$OPT_DEBUG" == "no" ]
then
  strip --strip-unneeded $PREFIX/bin/* 2> /dev/null
  strip --strip-unneeded $PREFIX/lib/* 2> /dev/null
  strip --strip-unneeded $PREFIX/plugins/designer/* 2> /dev/null
  strip --strip-unneeded $PREFIX/plugins/sqldrivers/* 2> /dev/null
  strip --strip-unneeded $PREFIX/plugins/styles/* 2> /dev/null
  cd src/translations
  ./update.sh 2> /dev/null
  cd ../..
fi

echo -e "\nTerminando compilacion...\n"
cp -f ./src/forms/*.ui $PREFIX/share/abanq/forms 2> /dev/null
cp -f ./src/tables/*.mtd $PREFIX/share/abanq/tables 2> /dev/null
cp -f ./src/translations/*.ts $PREFIX/share/abanq/translations 2> /dev/null
cp -f ./src/scripts/*.qs $PREFIX/share/abanq/scripts 2> /dev/null
cp -f ./src/docs/*.html $PREFIX/share/abanq/doc 2> /dev/null
cp -f ./src/*.xml $PREFIX/share/abanq 2> /dev/null
cp -f ./src/*.xpm $PREFIX/share/abanq 2> /dev/null

echo -e "\nAbanQ $VERSION\n(C) 2003-2013 InfoSiAL, S.L. http://infosial.com - http://abanq.org\n"
echo -e "Compilacion terminada.\n"

