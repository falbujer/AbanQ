@QBrush();
@QBrush(Qt::BrushStyle style);
@QBrush(QColor* color, Qt::BrushStyle style=SolidPattern);
@QBrush(QColor* color, QPixmap* pixmap);
@QBrush(QBrush* b);
uint style() const;
void setStyle(BrushStyle s);
QColor* color() const;
void setColor(QColor* c);
_QPixmap* pixmap() const;
void setPixmap(QPixmap* pixmap);
