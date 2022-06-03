@QDateEdit(QWidget* parent=0, const char* name=0);
@QDateEdit(const QDate& date, QWidget* parent=0, const char* name=0);
void setRange(const QDate& min, const QDate& max);
QString separator() const;
void setSeparator(const QString& s);
