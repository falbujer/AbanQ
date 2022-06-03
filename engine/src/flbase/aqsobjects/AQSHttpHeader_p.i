QString value(const QString& key) const;
void setValue(const QString& key, const QString& value);
void removeValue(const QString& key);
QStringList keys() const;
bool hasKey(const QString& key) const;
bool hasContentLength() const;
uint contentLength() const;
void setContentLength(int len);
bool hasContentType() const;
QString contentType() const;
void setContentType(const QString& type);
virtual QString toString() const;
bool isValid() const;
