@QSpinBox(QWidget* parent=0, const char* name=0);
@QSpinBox(int minValue, int maxValue, int step=1, QWidget* parent=0, const char* name=0);
void setValidator(const QValidator* v);
const QValidator* validator() const;
QRect upRect() const;
QRect downRect() const;
