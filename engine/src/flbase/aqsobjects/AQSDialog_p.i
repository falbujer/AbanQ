@QDialog(QWidget* parent=0, const char* name=0, bool modal=FALSE, WFlags f=0);
int result() const;
void show();
void setOrientation(Qt::Orientation orientation);
@Qt::Orientation orientation() const;
void setExtension(QWidget* extension);
QWidget* extension() const;
