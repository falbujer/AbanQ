@QActionGroup(QObject* parent, const char* name=0);
@QActionGroup(QObject* parent, const char* name, bool exclusive);
void setExclusive(bool);
bool isExclusive() const;
void add(QAction* action);
void addSeparator();
virtual bool addTo(QWidget* w);
void setUsesDropDown(bool enable);
