@QTextDrag(const QString& text, QWidget* dragSource=0, const char* name=0);
@QTextDrag(QWidget* dragSource=0, const char* name=0);
virtual void setText(const QString& text);
virtual void setSubtype(const QCString& st);
bool canDecode(const QMimeSource* e);
