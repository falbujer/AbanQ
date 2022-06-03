@QCanvasRectangle(QCanvas* canvas);
@QCanvasRectangle(const QRect& r, QCanvas* canvas);
@QCanvasRectangle(int x, int y, int width, int height, QCanvas* canvas);
int width() const;
int height() const;
void setSize(int width, int height);
QSize size() const;
QRect rect() const;
virtual int rtti() const;
