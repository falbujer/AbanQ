@QSocketNotifier(int socket, QSocketNotifier::Type type, QObject* parent=0, const char* name=0);
int socket() const;
uint type() const;
bool isEnabled() const;
virtual void setEnabled(bool enable);
