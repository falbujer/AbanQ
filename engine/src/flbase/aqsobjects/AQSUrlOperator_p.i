@QUrlOperator();
@QUrlOperator(const QString& url);
@QUrlOperator(const QUrlOperator& url);
@QUrlOperator(const QUrlOperator& url, const QString& relUrl, bool checkSlash=FALSE);
virtual const QNetworkOperation* listChildren();
virtual const QNetworkOperation* mkdir(const QString& dirname);
virtual const QNetworkOperation* remove(const QString& filename);
virtual const QNetworkOperation* rename(const QString& oldname, const QString& newname);
virtual const QNetworkOperation* get(const QString& location=QString::null);
virtual const QNetworkOperation* put(QByteArray* data, const QString& location=QString::null);
virtual void copy(const QStringList& files, const QString& dest, bool move=FALSE);
virtual bool isDir(bool* ok=0);
virtual void setNameFilter(const QString& nameFilter);
QString nameFilter() const;
virtual QUrlInfo* info(const QString& entry) const;
virtual void stop();
QString protocol() const;
virtual void setProtocol(const QString& protocol);
QString user() const;
virtual void setUser(const QString& user);
bool hasUser() const;
QString password() const;
virtual void setPassword(const QString& pass);
bool hasPassword() const;
QString host() const;
virtual void setHost(const QString& host);
bool hasHost() const;
int port() const;
virtual void setPort(int port);
bool hasPort() const;
QString path(bool correct=TRUE) const;
virtual void setPath(const QString& path);
bool hasPath() const;
virtual void setEncodedPathAndQuery(const QString& pathAndQuery);
QString encodedPathAndQuery();
virtual void setQuery(const QString& txt);
QString query() const;
QString ref() const;
virtual void setRef(const QString& txt);
bool hasRef() const;
bool isValid() const;
bool isLocalFile() const;
virtual void addPath(const QString& pa);
virtual void setFileName(const QString& name);
QString fileName() const;
QString dirPath() const;
@QUrl& operator= (const QUrl& url);
@QUrl& operator= (const QString& url);
@bool operator== (const QUrl& url) const;
@bool operator== (const QString& url) const;
virtual QString toString(bool encodedPath=FALSE, bool forcePrependProtocol=TRUE) const;
virtual bool cdUp();
void decode(QString& url);
void encode(QString& url);
bool isRelativeUrl(const QString& url);
