@QPainter();
@QPainter(const QPaintDevice* pd, bool unclipped=FALSE);
@QPainter(const QPaintDevice* pd, const QWidget* copyAttributes, bool unclipped=FALSE);
bool begin(const QPaintDevice* pd, bool unclipped=FALSE);
bool begin(const QPaintDevice* pd, const QWidget* copyAttributes, bool unclipped=FALSE);
bool end();
QPaintDevice* device() const;
bool isActive() const;
void flush(QRegion* region, QPainter::CoordinateMode cm=QPainter::CoordDevice);
void flush();
void save();
void restore();
QFontMetrics* fontMetrics() const;
const QFont& font() const;
void setFont(const QFont& font);
QPen* pen() const;
void setPen(QPen* pen);
void setPen(uint style);
void setPen(QColor* color);
QBrush* brush() const;
void setBrush(QBrush* brush);
void setBrush(BrushStyle style);
void setBrush(QColor* color);
QColor* backgroundColor() const;
void setBackgroundColor(QColor* c);
void setBackgroundMode(BGMode m);
void setRasterOp(RasterOp r);
QPoint brushOrigin() const;
void setBrushOrigin(int x, int y);
void setBrushOrigin(const QPoint& p);
bool hasViewXForm() const;
bool hasWorldXForm() const;
void setViewXForm(bool enable);
QRect window() const;
void setWindow(const QRect& r);
void setWindow(int x, int y, int w, int h);
QRect viewport() const;
void setViewport(const QRect& r);
void setViewport(int x, int y, int w, int h);
void setWorldXForm(bool enable);
QWMatrix* worldMatrix() const;
void setWorldMatrix(QWMatrix* m, bool combine=FALSE);
void scale(double sx, double sy);
void shear(double sh, double sv);
void rotate(double a);
void translate(double dx, double dy);
void resetXForm();
QPoint xForm(const QPoint& pv) const;
QRect xForm(const QRect& rv) const;
QPointArray* xForm(QPointArray* av) const;
QPointArray* xForm(QPointArray* av, int index, int npoints) const;
QPoint xFormDev(const QPoint& pd) const;
QRect xFormDev(const QRect& rd) const;
QPointArray* xFormDev(QPointArray* ad) const;
QPointArray* xFormDev(QPointArray* ad, int index, int npoints) const;
void setClipping(bool enable);
bool hasClipping() const;
QRegion* clipRegion(QPainter::CoordinateMode m=QPainter::CoordDevice) const;
void setClipRect(const QRect& r, QPainter::CoordinateMode m=QPainter::CoordDevice);
void setClipRect(int x, int y, int w, int h, QPainter::CoordinateMode m=QPainter::CoordDevice);
void setClipRegion(QRegion* rgn, QPainter::CoordinateMode m=QPainter::CoordDevice);
void drawPoint(int x, int y);
void drawPoint(const QPoint& p);
void drawPoints(QPointArray* a, int index=0, int npoints=-1);
void drawLine(int x1, int y1, int x2, int y2);
void drawLine(const QPoint& p1, const QPoint& p2);
void drawRect(int x, int y, int w, int h);
void drawRect(const QRect& r);
void drawWinFocusRect(int x, int y, int w, int h);
void drawWinFocusRect(int x, int y, int w, int h, QColor* bgColor);
void drawWinFocusRect(const QRect& r);
void drawWinFocusRect(const QRect& r, QColor* bgColor);
void drawRoundRect(int x, int y, int w, int h, int xRnd=25, int yRnd=25);
void drawRoundRect(const QRect& r, int xRnd=25, int yRnd=25);
void drawEllipse(int x, int y, int w, int h);
void drawEllipse(const QRect& r);
void drawArc(int x, int y, int w, int h, int a, int alen);
void drawArc(const QRect& r, int a, int alen);
void drawPie(int x, int y, int w, int h, int a, int alen);
void drawPie(const QRect& r, int a, int alen);
void drawChord(int x, int y, int w, int h, int a, int alen);
void drawChord(const QRect& r, int a, int alen);
void drawLineSegments(QPointArray* a, int index=0, int nlines=-1);
void drawPolyline(QPointArray* a, int index=0, int npoints=-1);
void drawPolygon(QPointArray* a, bool winding=FALSE, int index=0, int npoints=-1);
void drawConvexPolygon(QPointArray* pa, int index=0, int npoints=-1);
void drawCubicBezier(QPointArray* a, int index=0);
void drawPixmap(int x, int y, QPixmap* pixmap, int sx=0, int sy=0, int sw=-1, int sh=-1);
void drawPixmap(const QPoint& p, QPixmap* pm, const QRect& sr);
void drawPixmap(const QPoint& p, QPixmap* pm);
void drawPixmap(const QRect& r, QPixmap* pm);
void drawImage(int x, int y, QImage* image, int sx=0, int sy=0, int sw=-1, int sh=-1, int conversionFlags=0);
void drawImage(const QPoint&, QImage*, const QRect& sr, int conversionFlags=0);
void drawImage(const QPoint& p, QImage* i, int conversion_flags=0);
void drawImage(const QRect& r, QImage* i);
void drawTiledPixmap(int x, int y, int w, int h, QPixmap* pixmap, int sx=0, int sy=0);
void drawTiledPixmap(const QRect& r, QPixmap* pm, const QPoint& sp);
void drawTiledPixmap(const QRect& r, QPixmap* pm);
void drawPicture(int x, int y, QPicture* pic);
void drawPicture(const QPoint& p, QPicture* pic);
void fillRect(int x, int y, int w, int h, QBrush* brush);
void fillRect(const QRect& r, QBrush* brush);
void eraseRect(int x, int y, int w, int h);
void eraseRect(const QRect& r);
void drawText(int x, int y, const QString&, int len=-1, QPainter::TextDirection dir=QPainter::Auto);
void drawText(const QPoint&, const QString&, int len=-1, QPainter::TextDirection dir=QPainter::Auto);
void drawText(int x, int y, const QString&, int pos, int len, QPainter::TextDirection dir=QPainter::Auto);
void drawText(const QPoint& p, const QString&, int pos, int len, QPainter::TextDirection dir=QPainter::Auto);
void drawText(int x, int y, int w, int h, int flags, const QString&, int len=-1, QRect* br=0, QTextParag** internal=0);
void drawText(const QRect& r, int tf, const QString& str, int len=-1, QRect* brect=0, QTextParag** internal=0);
QRect boundingRect(int x, int y, int w, int h, int flags, const QString&, int len=-1, QTextParag** intern=0);
QRect boundingRect(const QRect& r, int flags, const QString& str, int len=-1, QTextParag** internal=0);
int tabStops() const;
void setTabStops(int ts);
int* tabArray() const;
void setTabArray(int* ta);
