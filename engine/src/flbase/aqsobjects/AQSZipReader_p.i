@AQZipReader(const QString& fileName, uint mode=IO_ReadOnly);
@AQZipReader(QIODevice* device);
QIODevice* device() const;
bool isReadable() const;
bool exists() const;
#QValueList<FileInfo> fileInfoList() const;
int count() const;
#FileInfo entryInfoAt(int index) const;
QByteArray fileData(const QString& fileName) const;
bool extractAll(const QString& destinationDir) const;
uint status() const;
void close();