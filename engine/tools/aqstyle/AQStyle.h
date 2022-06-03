/***************************************************************************
 AQStyle.h
 -------------------
 begin                : 03/02/2012
 copyright            : (C) 2003-2012 by InfoSiAL S.L.
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

#ifndef AQSTYLE_H_
#define AQSTYLE_H_

#include <qstringlist.h>

#define AQSTYLE_VERSION "AbanQ Style 2.4.0"

class AQStyle
{
public:
  AQStyle(const QString &defaultAsOps = QString::null);

  QString beautify(const QString &code, const QString &asOps = QString::null);
  QString defaultAsOps() const;
  QStringList errorMessages() const;

private:
  QString defaultAsOps_;
  QStringList errMsgs_;
};

#endif /* AQSTYLE_H_ */
