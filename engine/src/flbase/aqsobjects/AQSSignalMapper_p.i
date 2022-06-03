@QSignalMapper(QObject* parent, const char* name=0);
void setMapping(const QObject* sender, int identifier);
void setMapping(const QObject* sender, const QString& identifier);
void removeMappings(const QObject* sender);
