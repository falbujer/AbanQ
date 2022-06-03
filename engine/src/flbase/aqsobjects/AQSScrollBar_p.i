@QScrollBar(QWidget* parent, const char* name=0);
@QScrollBar(Qt::Orientation orientation, QWidget* parent, const char* name=0);
@QScrollBar(int minValue, int maxValue, int lineStep, int pageStep, int value, Qt::Orientation orientation, QWidget* parent, const char* name=0);
int sliderStart() const;
QRect sliderRect() const;
