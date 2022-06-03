virtual const char* name() const;
virtual const char* mimeName() const;
virtual int mibEnum() const;
virtual QTextDecoder* makeDecoder() const;
virtual QTextEncoder* makeEncoder() const;
virtual QString toUnicode(const char* chars, int len) const;
virtual QCString fromUnicode(const QString& uc, int& lenInOut) const;
QCString fromUnicode(const QString& uc) const;
QString toUnicode(QByteArray* a, int len) const;
QString toUnicode(QByteArray* a) const;
QString toUnicode(const QCString& a, int len) const;
QString toUnicode(const QCString& a) const;
QString toUnicode(const char* chars) const;
virtual bool canEncode(QChar ch) const;
virtual bool canEncode(const QString& s) const;
virtual int heuristicContentMatch(const char* chars, int len) const;
virtual int heuristicNameMatch(const char* hint) const;
