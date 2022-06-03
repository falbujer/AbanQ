@QUrl();
@QUrl(const QString& url);
@QUrl(QUrl* url);
@QUrl(QUrl* url, const QString& relUrl, bool checkSlash=FALSE);
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
virtual QString toString(bool encodedPath=FALSE, bool forcePrependProtocol=TRUE) const;
virtual bool cdUp();