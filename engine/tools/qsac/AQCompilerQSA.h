/***************************************************************************
 AQCompilerQSA.h
 -------------------
 begin                : 06/03/2011
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

#ifndef AQCOMPILERQSA_H_
#define AQCOMPILERQSA_H_

#include <qstringlist.h>

#define AQQSAC_VERSION "AbanQ qsac 2.4.0"

class AQLexer;

class AQCompilerQSA
{
public:

  AQCompilerQSA(AQLexer *l = 0);
  ~AQCompilerQSA();

  QByteArray byteCode(const QString &code, int id = 0, int lineno = 0,
                      bool humanRead = false, bool toPseudoCode = true);

  int errorType() const;
  QValueList<int> errorLines() const;
  QStringList errorMessages() const;

  static QString code(const QByteArray &byteCode);

private:

  AQLexer *lx;
  AQLexer *lx_;
  int errType;
  QValueList<int> errLines;
  QStringList errMsgs;
};

#endif /*AQCOMPILERQSA_H_*/
