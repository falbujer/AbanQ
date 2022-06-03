/***************************************************************************
 qsac.cpp
 -------------------
 begin                : 17/09/2011
 copyright            : (C) 2003-2011 by InfoSiAL S.L.
 email                : mail@infosial.com
 ***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
 Este  programa es software libre. Puede redistribuirlo y/o modificarlo
 bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
 versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include <AQLexer.h>

#include <cassert>
#include <stdexcept>
#include <iostream>
#ifdef WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#include "AQCompilerQSA.h"

using namespace std;

#ifdef WIN32
#define SLEEP(seconds) SleepEx(seconds * 1000);
#else
#define SLEEP(seconds) sleep(seconds);
#endif

static inline QByteArray byteArrayToBase64(QByteArray *ba)
{
  const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
                          "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
  const char padchar = '=';
  int padlen = 0;
  QByteArray tmp((ba->size() * 4) / 3 + 3);
  int i = 0;
  char *out = tmp.data();
  const uchar *data = (const uchar *)ba->data();
  while (i < ba->size()) {
    int chunk = 0;
    chunk |= int(uchar(data[i++])) << 16;
    if (i == ba->size()) {
      padlen = 2;
    } else {
      chunk |= int(uchar(data[i++])) << 8;
      if (i == ba->size()) padlen = 1;
      else chunk |= int(uchar(data[i++]));
    }

    int j = (chunk & 0x00fc0000) >> 18;
    int k = (chunk & 0x0003f000) >> 12;
    int l = (chunk & 0x00000fc0) >> 6;
    int m = (chunk & 0x0000003f);
    *out++ = alphabet[j];
    *out++ = alphabet[k];
    if (padlen > 1) *out++ = padchar;
    else *out++ = alphabet[l];
    if (padlen > 0) *out++ = padchar;
    else *out++ = alphabet[m];
  }
  tmp.truncate(out - tmp.data());
  return tmp;
}

class qsacByteCode : public xmlrpc_c::method
{
public:
  qsacByteCode() {
    this->_signature = "s:s";
    this->_help = "qsac.byteCode";
  }
  void execute(xmlrpc_c::paramList const &paramList,
               xmlrpc_c::value *const retvalP) {

    std::string const code(paramList.getString(0));

    paramList.verifyEnd(1);

    AQLexer lex;
    AQCompilerQSA qsac(&lex);
    QByteArray byteCode(qsac.byteCode(code));

    if (qsac.errorType() != AQErrNoError) {
      QString errs("ERROR\n");
      for (uint i = 0; i < qsac.errorLines().size(); ++i) {
        errs += QString("Line %1 : %2\n")
                .arg(qsac.errorLines()[i])
                .arg(qsac.errorMessages()[i]);
      }
      *retvalP = xmlrpc_c::value_string(errs);
    } else {
      *retvalP = xmlrpc_c::value_string(QString(byteArrayToBase64(&byteCode)));
    }

    SLEEP(1);
  }
};


int main(int const, const char **const)
{

  try {
    xmlrpc_c::registry myRegistry;

    xmlrpc_c::methodPtr const qsacByteCodeP(new qsacByteCode);

    myRegistry.addMethod("qsac.byteCode", qsacByteCodeP);

    xmlrpc_c::serverAbyss myAbyssServer(
      xmlrpc_c::serverAbyss::constrOpt()
      .registryP(&myRegistry)
      .portNumber(8080));

    myAbyssServer.run();
    assert(false);
  } catch (exception const &e) {
    cerr << "Something failed.  " << e.what() << endl;
  }
  return 0;
}

