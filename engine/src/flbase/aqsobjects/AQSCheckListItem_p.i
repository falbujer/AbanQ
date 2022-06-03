@QCheckListItem(QCheckListItem* parent, const QString& text, QCheckListItem::Type tt=RadioButtonController);
@QCheckListItem(QCheckListItem* parent, QListViewItem* after, const QString& text, QCheckListItem::Type tt=RadioButtonController);
@QCheckListItem(QListViewItem* parent, const QString& text, QCheckListItem::Type tt=RadioButtonController);
@QCheckListItem(QListViewItem* parent, QListViewItem* after, const QString& text, QCheckListItem::Type tt=RadioButtonController);
@QCheckListItem(QListView* parent, const QString& text, QCheckListItem::Type tt=RadioButtonController);
@QCheckListItem(QListView* parent, QListViewItem* after, const QString& text, QCheckListItem::Type tt=RadioButtonController);
@QCheckListItem(QListViewItem* parent, const QString& text, QPixmap* p);
@QCheckListItem(QListView* parent, const QString& text, QPixmap* p);
void setOn(bool b);
bool isOn() const;
uint type() const;
QString text() const;
void setTristate(bool b);
bool isTristate() const;
uint state() const;
void setState(QCheckListItem::ToggleState s);