/***************************************************************************
 AQSCanvasPolygon_p.h
 -------------------
 begin                : 06/10/2011
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

#ifndef AQSCANVASPOLYGON_P_H_
#define AQSCANVASPOLYGON_P_H_

#include "AQSCanvasPolygonalItem_p.h"

class AQSCanvasPolygon : public AQSCanvasPolygonalItem
{
  Q_OBJECT

  AQ_DECLARE_AQS_VOID(CanvasPolygon, CanvasPolygonalItem);

  //@AQ_BEGIN_DEF_PUB_SLOTS@
public slots:
  void setPoints(QPointArray *);
  void setPoints(AQSPointArray *);
  QPointArray *points() const;
  virtual QPointArray *areaPoints() const;
  virtual int rtti() const;

protected:
  static void *construct(const QSArgumentList &args) {
    QMap<int, QStringList> candidates;
    candidates[1].append(QString::fromLatin1("QCanvas*"));
    QString sgt(castArgsSignature(args, candidates));
    if (sgt == QString::fromLatin1("QCanvas*"))
      return new QCanvasPolygon(argValue<QCanvas *>(args[0]));
    return 0;
  }

public:
  static QMap<int, QStringList> candidateConstructors() {
    QMap<int, QStringList> candidates;
    candidates[1].append(QString::fromLatin1("QCanvas*"));
    return candidates;
  }
  //@AQ_END_DEF_PUB_SLOTS@
};

//@AQ_BEGIN_IMP_PUB_SLOTS@
inline void AQSCanvasPolygon::setPoints(QPointArray *arg0)
{
  AQ_CALL_VOID(setPoints(*arg0));
}
inline void AQSCanvasPolygon::setPoints(AQSPointArray *arg0)
{
  AQ_CALL_VOID(setPoints(*arg0));
}
inline QPointArray *AQSCanvasPolygon::points() const
{
  AQ_CALL_RET_PTR(points(), QPointArray);
}
inline QPointArray *AQSCanvasPolygon::areaPoints() const
{
  AQ_CALL_RET_PTR(areaPoints(), QPointArray);
}
inline int AQSCanvasPolygon::rtti() const
{
  AQ_CALL_RET_V(rtti(), int);
}
//@AQ_END_IMP_PUB_SLOTS@

#endif /* AQSCANVASPOLYGON_P_H_ */
// @AQSVOIDPTR@
