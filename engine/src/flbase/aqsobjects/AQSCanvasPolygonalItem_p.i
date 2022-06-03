virtual void setPen(QPen* p);
virtual void setBrush(QBrush* b);
QPen* pen() const;
QBrush* brush() const;
virtual QPointArray* areaPoints() const;
virtual QPointArray* areaPointsAdvanced() const;
virtual QRect boundingRect() const;
virtual int rtti() const;
