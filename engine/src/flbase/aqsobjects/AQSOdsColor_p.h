/***************************************************************************
 AQSOdsColor_p.h
 -------------------
 begin                : 16/12/2011
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

#ifndef AQSODSCOLOR_P_H_
#define AQSODSCOLOR_P_H_

#include "AQSVoidPtr_p.h"
#include "AQObjects.h"

// @AQ_PTR_INDIRECTION@

class AQSOdsColor : public AQSVoidPtr
{
  Q_OBJECT

  AQ_DECLARE_AQS_VOID_AQOBJECT(OdsColor, VoidPtr);

protected:
  void specializedInternalFinish() {
    if (!wrap_)
      delete o_;
  }

  //@AQ_BEGIN_DEF_PUB_SLOTS@
public slots:

protected:
  static void *construct(const QSArgumentList &args) {
    QMap<int, QStringList> candidates;
    candidates[0].append(QString::null);
    candidates[3].append(QString::fromLatin1("uint,uint,uint"));
    candidates[1].append(QString::fromLatin1("uint"));
    QString sgt(castArgsSignature(args, candidates));
    if (sgt.isEmpty())
      return new AQOdsColor;
    if (sgt == QString::fromLatin1("uint,uint,uint"))
      return new AQOdsColor(args[0].variant().toUInt(),
                            args[1].variant().toUInt(),
                            args[2].variant().toUInt());
    if (sgt == QString::fromLatin1("uint"))
      return new AQOdsColor(args[0].variant().toUInt());
    return 0;
  }

public:
  static QMap<int, QStringList> candidateConstructors() {
    QMap<int, QStringList> candidates;
    candidates[0].append(QString::null);
    candidates[3].append(QString::fromLatin1("uint,uint,uint"));
    candidates[1].append(QString::fromLatin1("uint"));
    return candidates;
  }
  //@AQ_END_DEF_PUB_SLOTS@
};

//@AQ_BEGIN_IMP_PUB_SLOTS@
//@AQ_END_IMP_PUB_SLOTS@

#endif /* AQSODSCOLOR_P_H_ */
// @AQOBJECT_VOID@
