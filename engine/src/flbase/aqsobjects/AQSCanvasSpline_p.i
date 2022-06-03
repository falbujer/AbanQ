@QCanvasSpline(QCanvas* canvas);
void setControlPoints(QPointArray* ctrl, bool close=TRUE);
QPointArray* controlPoints() const;
bool closed() const;
virtual int rtti() const;
