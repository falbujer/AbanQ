@QWidgetStack(QWidget* parent=0, const char* name=0);
@QWidgetStack(QWidget* parent, const char* name, WFlags f);
int addWidget(QWidget* w, int id=-1);
void removeWidget(QWidget* w);
QWidget* widget(int id) const;
int id(QWidget* widget) const;
QWidget* visibleWidget() const;
