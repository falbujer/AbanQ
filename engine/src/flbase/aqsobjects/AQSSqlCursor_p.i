@AQSqlCursor(const QString& n=QString::null, const QString& connName=QString::null);
FLTableMetaData* metadata() const;
int modeAccess() const;
void setModeAccess(int m);
void setAtomicValueBuffer(const QString& fN, const QString& functionName);
void setValueBuffer(const QString& fN, const QVariant& v);
QVariant valueBuffer(const QString& fN);
QVariant valueBufferCopy(const QString& fN) const;
QString mainFilter() const;
FLAction* action() const;
void setAction(FLAction* a);
void setMainFilter(const QString& f, bool doRefresh=true);
bool transaction(bool lock=false);
bool rollback();
bool commit(bool notify=true);
int size() const;
void setForwardOnly(bool forward);
bool isForwardOnly() const;
QSqlError* lastError() const;
QString lastQuery() const;
void openFormInMode(int m, bool cont=true);
void updateBufferCopy();
bool isModifiedBuffer();
void setEdition(bool b);
void setBrowse(bool b);
bool fieldDisabled(const QString& fN);
bool inTransaction();
void setAskForCancelChanges(bool a);
void setActivatedCheckIntegrity(bool a);
bool activatedCheckIntegrity() const;
void setActivatedCommitActions(bool a);
bool activatedCommitActions() const;
QString msgCheckIntegrity();
bool checkIntegrity(bool showError=true);
FLSqlCursor* cursorRelation() const;
FLRelationMetaData* relation() const;
void setUnLock(const QString& fN, bool v);
bool isLocked();
bool bufferIsNull(int i) const;
bool bufferIsNull(const QString& name) const;
void bufferSetNull(int i) const;
void bufferSetNull(const QString& name) const;
bool bufferCopyIsNull(int i) const;
bool bufferCopyIsNull(const QString& name) const;
void bufferCopySetNull(int i) const;
void bufferCopySetNull(const QString& name) const;
int atFrom();
int atFromBinarySearch(const QString& fN, const QString& v, bool orderAsc=true);
bool exec(const QString& query);
FLSqlDatabase* db() const;
int at() const;
QVariant value(int i) const;
QVariant value(const QString& name) const;
void setValue(int i, const QVariant& val);
void setValue(const QString& name, const QVariant& val);
QString baseFilter();
QString curFilter();
QString filter() const;
QString connectionName() const;
QString curName() const;
void changePos(const QString& name, int newPos=0);
void setOrder(const QStringList& order);