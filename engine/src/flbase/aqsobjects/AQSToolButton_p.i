@QToolButton(QWidget* parent, const char* name=0);
@QToolButton(QIconSet* iconSet, const QString& textLabel, const QString& grouptext, QObject* receiver, const char* slot, QToolBar* parent, const char* name=0);
@QToolButton(Qt::ArrowType type, QWidget* parent, const char* name=0);
virtual void setIconSet(QIconSet*);
QIconSet* iconSet() const;
void setPopup(QPopupMenu* popup);
QPopupMenu* popup() const;
void setPopupDelay(int delay);
void openPopup();
void setAutoRaise(bool enable);
