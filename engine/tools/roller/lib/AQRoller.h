/***************************************************************************
 AQRoller.h
 -------------------
 begin                : 21/01/2012
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

#ifndef AQROLLER_H_
#define AQROLLER_H_

#include <qstringlist.h>

#define AQROLLER_VERSION "AbanQ Roller 2.4.0"

class AQRoller
{
public:

  static QString roll(const QString &code,
                      const QString &fileName = QString::null);

  static QString rollCleaner(const QString &code,
                             const QString &fileName = QString::null);

  static QStringList logMessages() {
    return logMsgs_;
  }

  static QStringList logMsgs_;
};

#endif /* AQROLLER_H_ */
