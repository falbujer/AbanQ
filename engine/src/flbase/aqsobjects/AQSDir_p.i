@QDir();
@QDir(const QString& path, const QString& nameFilter=QString::null);
@QDir(QDir* d);
virtual void setPath(const QString& path);
virtual QString path() const;
virtual QString absPath() const;
virtual QString canonicalPath() const;
virtual QString dirName() const;
virtual QString filePath(const QString& fileName, bool acceptAbsPath=TRUE) const;
virtual QString absFilePath(const QString& fileName, bool acceptAbsPath=TRUE) const;
virtual bool cd(const QString& dirName, bool acceptAbsPath=TRUE);
virtual bool cdUp();
QString nameFilter() const;
virtual void setNameFilter(const QString& nameFilter);
uint filter() const;
virtual void setFilter(int filterSpec);
uint sorting() const;
virtual void setSorting(int sortSpec);
bool matchAllDirs() const;
virtual void setMatchAllDirs(bool enable);
uint count() const;
virtual QStringList entryList(int filterSpec=QDir::DefaultFilter, int sortSpec=QDir::DefaultSort) const;
virtual QStringList entryList(const QString& nameFilter, int filterSpec=QDir::DefaultFilter, int sortSpec=QDir::DefaultSort) const;
virtual bool mkdir(const QString& dirName, bool acceptAbsPath=TRUE) const;
virtual bool rmdir(const QString& dirName, bool acceptAbsPath=TRUE) const;
virtual bool mkpath(const QString& dirPath) const;
virtual bool rmpath(const QString& dirPath) const;
virtual bool isReadable() const;
virtual bool exists() const;
virtual bool isRoot() const;
virtual bool isRelative() const;
virtual void convertToAbs();
virtual bool remove(const QString& fileName, bool acceptAbsPath=TRUE);
virtual bool rename(const QString& oldName, const QString& newName, bool acceptAbsPaths=TRUE);
virtual bool exists(const QString& name, bool acceptAbsPath=TRUE);
void refresh() const;