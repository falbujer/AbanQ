/***************************************************************************
 AQSObjectFactory.h
 -------------------
 begin                : 29/01/2011
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
 bajo  los  t?rminos  de  la  Licencia  P?blica General de GNU   en  su
 versi?n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef AQSOBJECTFACTORY_H_
#define AQSOBJECTFACTORY_H_

#include <qsobjectfactory.h>

class AQSObjectFactoryPrivate;

class AQSObjectFactory : public QSObjectFactory
{
public:
  AQSObjectFactory();
  ~AQSObjectFactory();

  QObject *create(const QString &className,
                  const QSArgumentList &arguments,
                  QObject *context);
  static QObject *staticCreate(const QString &className,
                               const QSArgumentList &arguments,
                               QObject *context,
                               QMap<int, QStringList> *candidateCtors = 0);
private:
  AQSObjectFactoryPrivate *d;
};

#endif /* AQSOBJECTFACTORY_H_ */
