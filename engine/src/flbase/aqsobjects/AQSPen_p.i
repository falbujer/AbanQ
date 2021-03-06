@QPen();
@QPen(Qt::PenStyle style);
@QPen(QColor* color, uint width=0, Qt::PenStyle style=SolidLine);
@QPen(QColor* cl, uint w, Qt::PenStyle s, Qt::PenCapStyle c, Qt::PenJoinStyle j);
@QPen(QPen* p);
uint style() const;
void setStyle(Qt::PenStyle s);
uint width() const;
void setWidth(uint w);
QColor* color() const;
void setColor(QColor* c);
uint capStyle() const;
void setCapStyle(Qt::PenCapStyle c);
uint joinStyle() const;
void setJoinStyle(Qt::PenJoinStyle j);
