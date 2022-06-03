@QPaintEvent(QRegion* paintRegion, bool erased=TRUE);
@QPaintEvent(const QRect& paintRect, bool erased=TRUE);
@QPaintEvent(QRegion* paintRegion, const QRect& paintRect, bool erased=TRUE);
const QRect& rect() const;
QRegion* region() const;
bool erased() const;
