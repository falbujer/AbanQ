@QCanvasPixmapArray();
@QCanvasPixmapArray(const QString& datafilenamepattern, int fc=0);
@QCanvasPixmapArray(QValueList<QPixmap> list, QPointArray* hotspots=0);
bool readPixmaps(const QString& filenamepattern, int fc=0);
bool readCollisionMasks(const QString& filename);
bool isValid() const;
QCanvasPixmap* image(int i) const;
void setImage(int i, QCanvasPixmap* p);
uint count() const;
