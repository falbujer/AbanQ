/***************************************************************************
 AQSDomCDATASection_p.h
 -------------------
 begin                : 16/02/2011
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

#ifndef AQSDOMCDATASECTION_P_H_
#define AQSDOMCDATASECTION_P_H_

#include "AQSDomText_p.h"

// @AQ_PTR_INDIRECTION@

class AQSDomCDATASection : public AQSDomText
{
  Q_OBJECT

  AQ_DECLARE_AQS_VOID(DomCDATASection, DomText);

  //@AQ_BEGIN_DEF_PUB_SLOTS@
public slots:
  virtual uint nodeType() const;
  virtual bool isCDATASection() const;

protected:
  static void *construct(const QSArgumentList &args) {
    QMap<int, QStringList> candidates;
    candidates[0].append(QString::null);
    candidates[1].append(QString::fromLatin1("QDomCDATASection*"));
    QString sgt(castArgsSignature(args, candidates));
    if (sgt.isEmpty())
      return new QDomCDATASection;
    if (sgt == QString::fromLatin1("QDomCDATASection*"))
      return new QDomCDATASection(*argValue<QDomCDATASection *>(args[0]));
    return 0;
  }

public:
  static QMap<int, QStringList> candidateConstructors() {
    QMap<int, QStringList> candidates;
    candidates[0].append(QString::null);
    candidates[1].append(QString::fromLatin1("QDomCDATASection*"));
    return candidates;
  }
  //@AQ_END_DEF_PUB_SLOTS@
};

//@AQ_BEGIN_IMP_PUB_SLOTS@
inline uint AQSDomCDATASection::nodeType() const
{
  AQ_CALL_RET_V(nodeType(), uint);
}
inline bool AQSDomCDATASection::isCDATASection() const
{
  AQ_CALL_RET_V(isCDATASection(), bool);
}
//@AQ_END_IMP_PUB_SLOTS@

#endif /* AQSDOMCDATASECTION_P_H_ */
// @AQSVOIDPTR@
