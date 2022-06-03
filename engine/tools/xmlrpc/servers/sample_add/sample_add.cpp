/***************************************************************************
 sample_add.cpp
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

using namespace std;

#ifdef WIN32
#define SLEEP(seconds) SleepEx(seconds * 1000);
#else
#define SLEEP(seconds) sleep(seconds);
#endif


class sampleAddMethod : public xmlrpc_c::method
{
public:
  sampleAddMethod() {
    // signature and help strings are documentation -- the client
    // can query this information with a system.methodSignature and
    // system.methodHelp RPC.
    this->_signature = "i:ii";
    // method's result and two arguments are integers
    this->_help = "This method adds two integers together";
  }
  void
  execute(xmlrpc_c::paramList const &paramList,
          xmlrpc_c::value    *const  retvalP) {

    int const addend(paramList.getInt(0));
    int const adder(paramList.getInt(1));

    paramList.verifyEnd(2);

    *retvalP = xmlrpc_c::value_int(addend + adder);

    // Sometimes, make it look hard (so client can see what it's like
    // to do an RPC that takes a while).
    if (adder == 1)
      SLEEP(2);
  }
};



int
main(int           const,
     const char **const)
{

  try {
    xmlrpc_c::registry myRegistry;

    xmlrpc_c::methodPtr const sampleAddMethodP(new sampleAddMethod);

    myRegistry.addMethod("sample.add", sampleAddMethodP);

    xmlrpc_c::serverAbyss myAbyssServer(
      xmlrpc_c::serverAbyss::constrOpt()
      .registryP(&myRegistry)
      .portNumber(8080));

    myAbyssServer.run();
    // xmlrpc_c::serverAbyss.run() never returns
    assert(false);
  } catch (exception const &e) {
    cerr << "Something failed.  " << e.what() << endl;
  }
  return 0;
}
