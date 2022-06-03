@QMenuBar(QWidget* parent=0, const char* name=0);
int heightForWidth(int max_width) const;
int insertItem(const QString& text, int id=-1, int index=-1);
int insertItem(const QString& text, QPopupMenu* popup, int id=-1, int index=-1);
int insertItem(QPixmap* pixmap, int id=-1, int index=-1);
int insertItem(QPixmap* pixmap, QPopupMenu* popup, int id=-1, int index=-1);
int insertSeparator(int index=-1);
void removeItem(int id);
void removeItemAt(int index);
void clear();
uint count() const;
QKeySequence accel(int id) const;
void setAccel(const QKeySequence& key, int id);
QString text(int id) const;
_QPixmap* pixmap(int id) const;
void setWhatsThis(int id, const QString& text);
QString whatsThis(int id) const;
void changeItem(int id, const QString& text);
void changeItem(int id, QPixmap* pixmap);
bool isItemEnabled(int id) const;
void setItemEnabled(int id, bool enable);
bool isItemChecked(int id) const;
void setItemChecked(int id, bool check);
bool isItemVisible(int id) const;
void setItemVisible(int id, bool visible);
bool setItemParameter(int id, int param);
int itemParameter(int id) const;
void activateItemAt(int index);
