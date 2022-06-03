@QIconViewItem(QIconView* parent);
@QIconViewItem(QIconView* parent, QIconViewItem* after);
@QIconViewItem(QIconView* parent, const QString& text);
@QIconViewItem(QIconView* parent, QIconViewItem* after, const QString& text);
@QIconViewItem(QIconView* parent, const QString& text, QPixmap* icon);
@QIconViewItem(QIconView* parent, QIconViewItem* after, const QString& text, QPixmap* icon);
@QIconViewItem(QIconView* parent, const QString& text, QPicture* picture);
@QIconViewItem(QIconView* parent, QIconViewItem* after, const QString& text, QPicture* picture);
void setRenameEnabled(bool allow);
void setDragEnabled(bool allow);
void setDropEnabled(bool allow);
QString text() const;
_QPixmap* pixmap() const;
_QPicture* picture() const;
QString key() const;
bool renameEnabled() const;
bool dragEnabled() const;
bool dropEnabled() const;
QIconView* iconView() const;
QIconViewItem* prevItem() const;
QIconViewItem* nextItem() const;
int index() const;
void setSelected(bool s, bool cb);
void setSelected(bool s);
void setSelectable(bool s);
bool isSelected() const;
bool isSelectable() const;
void repaint();
bool move(int x, int y);
void moveBy(int dx, int dy);
bool move(const QPoint& pnt);
void moveBy(const QPoint& pnt);
QRect rect() const;
int x() const;
int y() const;
int width() const;
int height() const;
QSize size() const;
QPoint pos() const;
QRect textRect(bool relative=TRUE) const;
QRect pixmapRect(bool relative=TRUE) const;
bool contains(const QPoint& pnt) const;
bool intersects(const QRect& r) const;
int compare(QIconViewItem* i) const;
void setText(const QString& text);
void setPixmap(QPixmap* icon);
void setPicture(QPicture* icon);
void setText(const QString& text, bool recalc, bool redraw=TRUE);
void setPixmap(QPixmap* icon, bool recalc, bool redraw=TRUE);
void setKey(const QString& k);
