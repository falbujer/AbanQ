bool editorsModified() const;
bool scriptsModified() const;
QStringList scriptNames() const;
QSScript* script(const QString& name) const;
QSScript* script(QObject* context) const;
QObject* object(const QString& name) const;
QObjectList objects() const;
QSScript* createScript(QObject* context, const QString& code=QString::null);
QSScript* createScript(const QString& name, const QString& code=QString::null);
QSEditor* activeEditor() const;
QSEditor* editor(QSScript* script) const;
QSEditor* createEditor(QSScript* script, QWidget* parent=0, const char* name=0);
void addObject(QObject* object);
void removeObject(const QObject* object);
