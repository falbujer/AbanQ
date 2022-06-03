@QStatusBar(QWidget* parent=0, const char* name=0);
virtual void addWidget(QWidget* widget, int stretch=0, bool permanent=FALSE);
virtual void removeWidget(QWidget* widget);
void setSizeGripEnabled(bool);
bool isSizeGripEnabled() const;
