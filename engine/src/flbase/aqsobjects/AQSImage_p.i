@QImage();
@QImage(int w, int h, int depth, int numColors=0, QImage::Endian bitOrder=QImage::IgnoreEndian);
@QImage(const QSize& size, int depth, int numColors=0, QImage::Endian bitOrder=QImage::IgnoreEndian);
@QImage(const QString& fileName, const char* format=0);
@QImage(QByteArray* array);
@QImage(QImage* image);
QImage* copy() const;
QImage* copy(int x, int y, int w, int h, int conversion_flags=0) const;
QImage* copy(const QRect& r) const;
bool isNull() const;
int width() const;
int height() const;
QSize size() const;
QRect rect() const;
int depth() const;
int numColors() const;
uint bitOrder() const;
QRgb color(int i) const;
void setColor(int i, QRgb c);
void setNumColors(int numColors);
bool hasAlphaBuffer() const;
void setAlphaBuffer(bool enable);
bool allGray() const;
bool isGrayscale() const;
int numBytes() const;
int bytesPerLine() const;
bool create(int width, int height, int depth, int numColors=0, QImage::Endian bitOrder=QImage::IgnoreEndian);
bool create(const QSize&, int depth, int numColors=0, QImage::Endian bitOrder=QImage::IgnoreEndian);
void reset();
void fill(uint pixel);
void invertPixels(bool invertAlpha=TRUE);
QImage* convertDepth(int depth) const;
QImage* convertDepthWithPalette(int d, QRgb* palette, int palette_count, int conversion_flags=0) const;
QImage* convertDepth(int depth, int conversion_flags) const;
QImage* convertBitOrder(QImage::Endian bitOrder) const;
QImage* smoothScale(int w, int h, QImage::ScaleMode mode=QImage::ScaleFree) const;
QImage* smoothScale(const QSize& s, QImage::ScaleMode mode=QImage::ScaleFree) const;
QImage* scale(int w, int h, QImage::ScaleMode mode=QImage::ScaleFree) const;
QImage* scale(const QSize& s, QImage::ScaleMode mode=QImage::ScaleFree) const;
QImage* scaleWidth(int w) const;
QImage* scaleHeight(int h) const;
QImage* xForm(QWMatrix* matrix) const;
QImage* createAlphaMask(int conversion_flags=0) const;
QImage* createHeuristicMask(bool clipTight=TRUE) const;
QImage* mirror() const;
QImage* mirror(bool horizontal, bool vertical) const;
QImage* swapRGB() const;
bool loadFromData(QByteArray* buf, const char* format=0);
bool save(QIODevice* device, const char* format, int quality=-1) const;
bool load(const QString& fileName, const char* format=0);
bool save(const QString& fileName, const char* format, int quality=-1) const;
bool valid(int x, int y) const;
int pixelIndex(int x, int y) const;
QRgb pixel(int x, int y) const;
void setPixel(int x, int y, uint index_or_rgb);
int dotsPerMeterX() const;
int dotsPerMeterY() const;
void setDotsPerMeterX(int x);
void setDotsPerMeterY(int y);
QPoint offset() const;
void setOffset(const QPoint& p);
QStringList textLanguages() const;
QStringList textKeys() const;
QString text(const char* key, const char* lang=0) const;
void setText(const char* key, const char* lang, const QString& s);
