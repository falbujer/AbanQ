@AQFormDB(QWidget* parent=0, const char* name=0, WFlags f=0);
@AQFormDB(const QString& actionName, QWidget* parent=0, WFlags f=0);
@AQFormDB(FLSqlCursor* cursor, const QString& actionName=QString::null, QWidget* parent=0, WFlags f=0);
virtual void setCursor(FLSqlCursor* c);
FLSqlCursor* cursor() const;
QWidget* mainWidget() const;
void setIdMDI(const QString& id);
QString idMDI() const;
virtual void setMainWidget(QWidget* w);
virtual void setMainWidget();
virtual void setMainWidget(const QString& uiFileName);
QImage* snapShot();
void saveSnapShot(const QString& pathFile);
