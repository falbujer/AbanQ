@QScrollView(QWidget* parent=0, const char* name=0, WFlags f=0);
void addChild(QWidget* child, int x=0, int y=0);
void moveChild(QWidget* child, int x, int y);
int childX(QWidget* child);
int childY(QWidget* child);
QWidget* cornerWidget() const;
void setCornerWidget(QWidget* corner);
QScrollBar* horizontalScrollBar() const;
QScrollBar* verticalScrollBar() const;
QWidget* viewport() const;
QWidget* clipper() const;
void updateContents(int x, int y, int w, int h);
void updateContents(const QRect& r);
void updateContents();
void repaintContents(int x, int y, int w, int h, bool erase=TRUE);
void repaintContents(const QRect& r, bool erase=TRUE);
void repaintContents(bool erase=TRUE);
void contentsToViewport(int x, int y, int& vx, int& vy) const;
void viewportToContents(int vx, int vy, int& x, int& y) const;
QPoint contentsToViewport(const QPoint& p) const;
QPoint viewportToContents(const QPoint& vp) const;
void enableClipper(bool y);
void setStaticBackground(bool y);
bool hasStaticBackground() const;
QSize viewportSize(int x, int y) const;
bool isHorizontalSliderPressed();
bool isVerticalSliderPressed();
uint vScrollBarMode() const;
virtual void setVScrollBarMode(QScrollView::ScrollBarMode);
uint hScrollBarMode() const;
virtual void setHScrollBarMode(QScrollView::ScrollBarMode);