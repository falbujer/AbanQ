@QPushButton(QWidget* parent, const char* name=0);
@QPushButton(const QString& text, QWidget* parent, const char* name=0);
@QPushButton(QIconSet* icon, const QString& text, QWidget* parent, const char* name=0);
void setPopup(QPopupMenu* popup);
QPopupMenu* popup() const;
