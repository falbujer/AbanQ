@QLabel(QWidget* parent, const char* name=0, WFlags f=0);
@QLabel(const QString& text, QWidget* parent, const char* name=0, WFlags f=0);
@QLabel(QWidget* buddy, const QString& text, QWidget* parent, const char* name=0, WFlags f=0);
_QPicture* picture() const;
void setBuddy(QWidget* buddy);
QWidget* buddy() const;
virtual void setPixmap(QPixmap* pixmap);
virtual void setPicture(QPicture* picture);
