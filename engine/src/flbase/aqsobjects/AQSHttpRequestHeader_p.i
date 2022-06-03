@QHttpRequestHeader();
@QHttpRequestHeader(const QString& method, const QString& path, int majorVer=1, int minorVer=1);
@QHttpRequestHeader(QHttpRequestHeader* header);
@QHttpRequestHeader(const QString& str);
void setRequest(const QString& method, const QString& path, int majorVer=1, int minorVer=1);
QString method() const;
QString path() const;
virtual int majorVersion() const;
virtual int minorVersion() const;
