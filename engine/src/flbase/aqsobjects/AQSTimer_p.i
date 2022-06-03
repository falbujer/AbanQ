@QTimer(QObject* parent=0, const char* name=0);
bool isActive() const;
int start(int msec, bool sshot=FALSE);
void changeInterval(int msec);
void stop();
int timerId() const;
