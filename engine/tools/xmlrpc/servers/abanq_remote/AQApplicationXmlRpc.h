/***************************************************************************
 AQApplicationXmlRpc.h
 -------------------
 begin                : 23/09/2011
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
 bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
 versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef AQAPPLICATIONXMLRPC_H_
#define AQAPPLICATIONXMLRPC_H_

#include <AQApplication.h>

class AQApplicationXmlRpc : public AQApplication
{
  Q_OBJECT

public:
  AQApplicationXmlRpc(int &argc, char **argv);

  void setPort(uint port) {
    port_ = port;
  }

  uint port() const {
    return port_;
  }

public slots:
  void runXmlRpcServer();

private:
  uint port_;
};

#endif /* AQAPPLICATIONXMLRPC_H_ */
