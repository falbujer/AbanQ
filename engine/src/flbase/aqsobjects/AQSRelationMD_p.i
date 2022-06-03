@AQRelationMD(const QString& fT, const QString& fF, int rC, bool dC=false, bool uC=false, bool cI=true);
void setField(const QString& fN);
QString field() const;
QString foreignTable() const;
QString foreignField() const;
int cardinality() const;
bool deleteCascade() const;
bool updateCascade() const;
bool checkIn() const;
