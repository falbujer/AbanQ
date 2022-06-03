@QCanvasPixmap(const QString& datafilename);
@QCanvasPixmap(QImage* image);
@QCanvasPixmap(QPixmap* pm, const QPoint& offset);
int offsetX() const;
int offsetY() const;
void setOffset(int x, int y);
