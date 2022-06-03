@QGroupBox(QWidget* parent=0, const char* name=0);
@QGroupBox(const QString& title, QWidget* parent=0, const char* name=0);
@QGroupBox(int strips, Qt::Orientation orientation, QWidget* parent=0, const char* name=0);
@QGroupBox(int strips, Qt::Orientation orientation, const QString& title, QWidget* parent=0, const char* name=0);
int insideMargin() const;
int insideSpacing() const;
void setInsideMargin(int m);
void setInsideSpacing(int s);
void addSpace(int size);
bool isChecked() const;