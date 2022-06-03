@QUriDrag(QWidget* dragSource=0, const char* name=0);
void setFileNames(const QStringList& fnames);
void setUnicodeUris(const QStringList& uuris);
QString uriToLocalFile(const char* uri);
QCString localFileToUri(const QString& filename);
QString uriToUnicodeUri(const char* uri);
QCString unicodeUriToUri(const QString& uuri);
bool canDecode(const QMimeSource* e);
