@QComboBox(QWidget* parent=0, const char* name=0);
@QComboBox(bool rw, QWidget* parent=0, const char* name=0);
void insertStringList(const QStringList& list, int index=-1);
void insertItem(QPixmap* pixmap, int index=-1);
void insertItem(QPixmap* pixmap, const QString& text, int index=-1);
void removeItem(int index);
@int currentItem() const;
void setCurrentItem(int index);
@QString currentText() const;
void setCurrentText(const QString&);
QString text(int index) const;
const _QPixmap* pixmap(int index) const;
void changeItem(const QString& t, int index);
void changeItem(QPixmap* im, int index);
void changeItem(QPixmap* im, const QString& t, int index);
void setValidator(const QValidator* v);
const QValidator* validator() const;
void setListBox(QListBox* newListBox);
QListBox* listBox() const;
void setLineEdit(QLineEdit* edit);
QLineEdit* lineEdit() const;
void popup();
