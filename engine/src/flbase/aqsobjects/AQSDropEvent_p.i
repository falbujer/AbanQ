@QDropEvent(const QPoint& pos, QEvent::Type typ=Drop);
QPoint pos() const;
bool isAccepted() const;
void accept(bool y=TRUE);
void ignore();
bool isActionAccepted() const;
void acceptAction(bool y=TRUE);
void setAction(QDropEvent::Action a);
uint action() const;
QWidget* source() const;
virtual const char* format(int n=0) const;
virtual QByteArray* encodedData(const char* format) const;
virtual bool provides(const char* mimeType) const;
void setPoint(const QPoint& np);
