@QDockWindow(QDockWindow::Place p=QDockWindow::InDock, QWidget* parent=0, const char* name=0, WFlags f=0);
@QDockWindow(QWidget* parent, const char* name=0, WFlags f=0);
virtual void setWidget(QWidget* w);
QWidget* widget() const;
QDockArea* area() const;
uint place() const;
virtual void setCloseMode(int m);
bool isCloseEnabled() const;
virtual void setResizeEnabled(bool b);
virtual void setMovingEnabled(bool b);
bool isResizeEnabled() const;
bool isMovingEnabled() const;
virtual void setHorizontallyStretchable(bool b);
virtual void setVerticallyStretchable(bool b);
bool isHorizontallyStretchable() const;
bool isVerticallyStretchable() const;
bool isStretchable() const;
virtual void setOffset(int o);
virtual void setFixedExtentWidth(int w);
virtual void setFixedExtentHeight(int h);
QSize fixedExtent() const;
virtual void setNewLine(bool b);
bool newLine() const;
uint orientation() const;
QBoxLayout* boxLayout();
virtual void setOpaqueMoving(bool b);
