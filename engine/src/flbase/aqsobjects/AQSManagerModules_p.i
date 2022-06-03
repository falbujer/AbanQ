QString content(const QString& n);
QString byteCodeToStr(QByteArray* byteCode) const;
QString contentCode(const QString& n);
QString contentCached(const QString& n, QString* shaKey=0);
void setContent(const QString& n, const QString& idM, const QString& content);
QWidget* createUI(const QString& n, QObject* connector=0, QWidget* parent=0, const char* name=0);
QWidget* createForm(const AQActionMD* a, QObject* connector=0, QWidget* parent=0, const char* name=0);
QWidget* createFormRecord(const AQActionMD* a, QObject* connector=0, QWidget* parent=0, const char* name=0);
void setActiveIdModule(const QString& id);
QString activeIdArea() const;
QString activeIdModule() const;
QStringList listIdAreas();
QStringList listIdModules(const QString& idA);
QStringList listAllIdModules();
QString idAreaToDescription(const QString& idA);
QString idModuleToDescription(const QString& idM);
QPixmap* iconModule(const QString& idM);
QString versionModule(const QString& idM);
QString shaLocal();
QString shaGlobal();
void setShaLocalFromGlobal();
QString shaOfFile(const QString& n);
void loadKeyFiles();
void loadAllIdModules();
void loadIdAreas();
QString idModuleOfFile(const QString& n);
QString contentFS(const QString& pN);