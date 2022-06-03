@AQCompilerQSA();
QByteArray byteCode(const QString& code, int id=0, int lineno=0, bool humanRead=false);
int errorType() const;
QStringList errorMessages() const;
QString code(QByteArray* byteCode);
