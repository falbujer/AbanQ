QString name() const;
QString code() const;
QObject* context() const;
void setCode(const QString& code);
void addCode(const QString& code);
bool addFunction(const QString& functionName, const QString& functionBody=QString::null);
QString fileName() const;
void setFileName(const QString& name);
QString baseFileName() const;
void setBaseFileName(const QString& name);
