@QPicture(int formatVersion=-1);
bool isNull() const;
uint size() const;
bool load(const QString& fileName, const char* format=0);
bool save(const QString& fileName, const char* format=0);
QRect boundingRect() const;
void setBoundingRect(const QRect& r);
