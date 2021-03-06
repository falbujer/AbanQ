@QContextMenuEvent(QContextMenuEvent::Reason reason, const QPoint& pos, const QPoint& globalPos, int state);
@QContextMenuEvent(QContextMenuEvent::Reason reason, const QPoint& pos, int state);
int x() const;
int y() const;
int globalX() const;
int globalY() const;
QPoint pos() const;
QPoint globalPos() const;
uint state() const;
bool isAccepted() const;
bool isConsumed() const;
void consume();
void accept();
void ignore();
uint reason() const;
