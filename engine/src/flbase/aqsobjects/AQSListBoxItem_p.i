QString text() const;
const _QPixmap* pixmap() const;
int height(const QListBox* lb) const;
int width(const QListBox* lb) const;
bool isSelected() const;
bool isCurrent() const;
QListBox* listBox() const;
void setSelectable(bool b);
bool isSelectable() const;
QListBoxItem* next() const;
QListBoxItem* prev() const;
