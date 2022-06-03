@QTimeEdit(QWidget* parent=0, const char* name=0);
@QTimeEdit(const QTime& time, QWidget* parent=0, const char* name=0);
void setRange(const QTime& min, const QTime& max);
QString separator() const;
void setSeparator(const QString& s);
