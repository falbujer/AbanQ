@QToolBar(const QString& label, QMainWindow* mainWindow, QWidget* parent, bool newLine=FALSE, const char* name=0, WFlags f=0);
@QToolBar(QMainWindow* parent=0, const char* name=0);
void addSeparator();
QMainWindow* mainWindow() const;
virtual void setStretchableWidget(QWidget* w);
virtual void setLabel(const QString&);
virtual void clear();
