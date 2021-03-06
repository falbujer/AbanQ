/***************************************************************************
 AQSPopupMenu_p.h
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

#ifndef AQSPOPUPMENU_P_H_
#define AQSPOPUPMENU_P_H_

#include "AQSFrame_p.h"
#include "AQSPixmap_p.h"
#include <qpopupmenu.h>

class AQSPopupMenu : public AQSFrame
{
  Q_OBJECT

  AQ_DECLARE_AQS_OBJECT(PopupMenu, Frame);

  //@AQ_BEGIN_DEF_PUB_SLOTS@
public slots:
  void popup(const QPoint &, int = -1);
  void updateItem(int);
  void setCheckable(bool);
  bool isCheckable() const;
  int exec();
  int exec(const QPoint &, int = 0);
  void setActiveItem(int);
  int idAt(int) const;
  int idAt(const QPoint &) const;
  int insertTearOffHandle(int = -1, int = -1);
  int insertItem(const QString &, int = -1, int = -1);
  int insertItem(const QString &, QPopupMenu *, int = -1, int = -1);
  int insertItem(QPixmap *, int = -1, int = -1);
  int insertItem(AQSPixmap *, int = -1, int = -1);
  int insertItem(QPixmap *, QPopupMenu *, int = -1, int = -1);
  int insertItem(AQSPixmap *, QPopupMenu *, int = -1, int = -1);
  int insertSeparator(int = -1);
  void removeItem(int);
  void removeItemAt(int);
  void clear();
  uint count() const;
  QKeySequence accel(int) const;
  void setAccel(const QKeySequence &, int);
  QString text(int) const;
  QPixmap *pixmap(int) const;
  void setWhatsThis(int, const QString &);
  QString whatsThis(int) const;
  void changeItem(int, const QString &);
  void changeItem(int, QPixmap *);
  void changeItem(int, AQSPixmap *);
  bool isItemEnabled(int) const;
  void setItemEnabled(int, bool);
  bool isItemChecked(int) const;
  void setItemChecked(int, bool);
  bool isItemVisible(int) const;
  void setItemVisible(int, bool);
  bool setItemParameter(int, int);
  int itemParameter(int) const;
  void activateItemAt(int);

protected:
  static void *construct(const QSArgumentList &args) {
    QMap<int, QStringList> candidates;
    candidates[0].append(QString::null);
    candidates[1].append(QString::fromLatin1("QWidget*"));
    candidates[2].append(QString::fromLatin1("QWidget*,QString"));
    QString sgt(castArgsSignature(args, candidates));
    if (sgt.isEmpty())
      return new QPopupMenu;
    if (sgt == QString::fromLatin1("QWidget*"))
      return new QPopupMenu(argValue<QWidget *>(args[0]));
    if (sgt == QString::fromLatin1("QWidget*,QString"))
      return new QPopupMenu(argValue<QWidget *>(args[0]),
                            *(argValue<QString *>(args[1])));
    return 0;
  }

public:
  static QMap<int, QStringList> candidateConstructors() {
    QMap<int, QStringList> candidates;
    candidates[0].append(QString::null);
    candidates[1].append(QString::fromLatin1("QWidget*"));
    candidates[2].append(QString::fromLatin1("QWidget*,QString"));
    return candidates;
  }
  //@AQ_END_DEF_PUB_SLOTS@
};

//@AQ_BEGIN_IMP_PUB_SLOTS@
inline void AQSPopupMenu::popup(const QPoint &arg0,  int arg1)
{
  AQ_CALL_VOID(popup(arg0, arg1));
}
inline void AQSPopupMenu::updateItem(int arg0)
{
  AQ_CALL_VOID(updateItem(arg0));
}
inline void AQSPopupMenu::setCheckable(bool arg0)
{
  AQ_CALL_VOID(setCheckable(arg0));
}
inline bool AQSPopupMenu::isCheckable() const
{
  AQ_CALL_RET_V(isCheckable(), bool);
}
inline int AQSPopupMenu::exec()
{
  AQ_CALL_RET_V(exec(), int);
}
inline int AQSPopupMenu::exec(const QPoint &arg0,  int arg1)
{
  AQ_CALL_RET_V(exec(arg0, arg1), int);
}
inline void AQSPopupMenu::setActiveItem(int arg0)
{
  AQ_CALL_VOID(setActiveItem(arg0));
}
inline int AQSPopupMenu::idAt(int arg0) const
{
  AQ_CALL_RET_V(idAt(arg0), int);
}
inline int AQSPopupMenu::idAt(const QPoint &arg0) const
{
  AQ_CALL_RET_V(idAt(arg0), int);
}
inline int AQSPopupMenu::insertTearOffHandle(int arg0,  int arg1)
{
  AQ_CALL_RET_V(insertTearOffHandle(arg0, arg1), int);
}
inline int AQSPopupMenu::insertItem(const QString &arg0,  int arg1,  int arg2)
{
  AQ_CALL_RET_V(insertItem(arg0, arg1, arg2), int);
}
inline int AQSPopupMenu::insertItem(const QString &arg0,  QPopupMenu *arg1,  int arg2,  int arg3)
{
  AQ_CALL_RET_V(insertItem(arg0, arg1, arg2, arg3), int);
}
inline int AQSPopupMenu::insertItem(QPixmap *arg0,  int arg1,  int arg2)
{
  AQ_CALL_RET_V(insertItem(*arg0, arg1, arg2), int);
}
inline int AQSPopupMenu::insertItem(AQSPixmap *arg0,  int arg1,  int arg2)
{
  AQ_CALL_RET_V(insertItem(*arg0, arg1, arg2), int);
}
inline int AQSPopupMenu::insertItem(QPixmap *arg0,  QPopupMenu *arg1,  int arg2,  int arg3)
{
  AQ_CALL_RET_V(insertItem(*arg0, arg1, arg2, arg3), int);
}
inline int AQSPopupMenu::insertItem(AQSPixmap *arg0,  QPopupMenu *arg1,  int arg2,  int arg3)
{
  AQ_CALL_RET_V(insertItem(*arg0, arg1, arg2, arg3), int);
}
inline int AQSPopupMenu::insertSeparator(int arg0)
{
  AQ_CALL_RET_V(insertSeparator(arg0), int);
}
inline void AQSPopupMenu::removeItem(int arg0)
{
  AQ_CALL_VOID(removeItem(arg0));
}
inline void AQSPopupMenu::removeItemAt(int arg0)
{
  AQ_CALL_VOID(removeItemAt(arg0));
}
inline void AQSPopupMenu::clear()
{
  AQ_CALL_VOID(clear());
}
inline uint AQSPopupMenu::count() const
{
  AQ_CALL_RET_V(count(), uint);
}
inline QKeySequence AQSPopupMenu::accel(int arg0) const
{
  AQ_CALL_RET_V(accel(arg0), QKeySequence);
}
inline void AQSPopupMenu::setAccel(const QKeySequence &arg0,  int arg1)
{
  AQ_CALL_VOID(setAccel(arg0, arg1));
}
inline QString AQSPopupMenu::text(int arg0) const
{
  AQ_CALL_RET_V(text(arg0), QString);
}
inline QPixmap *AQSPopupMenu::pixmap(int arg0) const
{
  AQ_CALL_RET(pixmap(arg0));
}
inline void AQSPopupMenu::setWhatsThis(int arg0,  const QString &arg1)
{
  AQ_CALL_VOID(setWhatsThis(arg0, arg1));
}
inline QString AQSPopupMenu::whatsThis(int arg0) const
{
  AQ_CALL_RET_V(whatsThis(arg0), QString);
}
inline void AQSPopupMenu::changeItem(int arg0,  const QString &arg1)
{
  AQ_CALL_VOID(changeItem(arg0, arg1));
}
inline void AQSPopupMenu::changeItem(int arg0,  QPixmap *arg1)
{
  AQ_CALL_VOID(changeItem(arg0, *arg1));
}
inline void AQSPopupMenu::changeItem(int arg0, AQSPixmap *arg1)
{
  AQ_CALL_VOID(changeItem(arg0, *arg1));
}
inline bool AQSPopupMenu::isItemEnabled(int arg0) const
{
  AQ_CALL_RET_V(isItemEnabled(arg0), bool);
}
inline void AQSPopupMenu::setItemEnabled(int arg0,  bool arg1)
{
  AQ_CALL_VOID(setItemEnabled(arg0, arg1));
}
inline bool AQSPopupMenu::isItemChecked(int arg0) const
{
  AQ_CALL_RET_V(isItemChecked(arg0), bool);
}
inline void AQSPopupMenu::setItemChecked(int arg0,  bool arg1)
{
  AQ_CALL_VOID(setItemChecked(arg0, arg1));
}
inline bool AQSPopupMenu::isItemVisible(int arg0) const
{
  AQ_CALL_RET_V(isItemVisible(arg0), bool);
}
inline void AQSPopupMenu::setItemVisible(int arg0,  bool arg1)
{
  AQ_CALL_VOID(setItemVisible(arg0, arg1));
}
inline bool AQSPopupMenu::setItemParameter(int arg0,  int arg1)
{
  AQ_CALL_RET_V(setItemParameter(arg0, arg1), bool);
}
inline int AQSPopupMenu::itemParameter(int arg0) const
{
  AQ_CALL_RET_V(itemParameter(arg0), int);
}
inline void AQSPopupMenu::activateItemAt(int arg0)
{
  AQ_CALL_VOID(activateItemAt(arg0));
}
//@AQ_END_IMP_PUB_SLOTS@

#endif /* AQSPOPUPMENU_P_H_ */
// @AQSWIDGET@
