@AQFormRecordDB(FLSqlCursor* cursor, const QString& actionName=QString::null, QWidget* parent=0, bool showAcceptContinue=true);
FLSqlCursor* cursor() const;
QWidget* mainWidget() const;
void setIdMDI(const QString& id);
QString idMDI() const;
virtual void setMainWidget(QWidget* w);
virtual void setMainWidget();
QImage* snapShot();
void saveSnapShot(const QString& pathFile);
