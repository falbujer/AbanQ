@QSqlError(const QString& driverText=QString::null, const QString& databaseText=QString::null, int type=QSqlError::None, int number=-1);
@QSqlError(QSqlError* other);
QString driverText() const;
virtual void setDriverText(const QString& driverText);
QString databaseText() const;
virtual void setDatabaseText(const QString& databaseText);
int type() const;
virtual void setType(int type);
int number() const;
virtual void setNumber(int number);
QString text() const;
