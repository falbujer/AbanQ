@QIconDrag(QWidget* dragSource, const char* name=0);
void append(QIconDragItem* i, const QRect& pr, const QRect& tr);
virtual QByteArray* encodedData(const char* mime) const;
bool canDecode(QMimeSource* e);
