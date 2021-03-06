int flags() const;
int mode() const;
int state() const;
bool isDirectAccess() const;
bool isSequentialAccess() const;
bool isCombinedAccess() const;
bool isBuffered() const;
bool isRaw() const;
bool isSynchronous() const;
bool isAsynchronous() const;
bool isTranslated() const;
bool isReadable() const;
bool isWritable() const;
bool isReadWrite() const;
bool isInactive() const;
bool isOpen() const;
int status() const;
void resetStatus();
virtual bool open(int mode);
virtual void close();
virtual void flush();
virtual uint size() const;
virtual uint at() const;
virtual bool at(uint pos);
virtual bool atEnd() const;
bool reset();
virtual long readBlock(char* data, ulong maxlen);
virtual long writeBlock(const char* data, ulong len);
virtual long readLine(char* data, ulong maxlen);
long writeBlock(QByteArray* data);
virtual QByteArray readAll();
virtual int getch();
virtual int putch(int ch);
virtual int ungetch(int ch);
