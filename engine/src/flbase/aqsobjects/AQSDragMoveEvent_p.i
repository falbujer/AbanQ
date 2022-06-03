@QDragMoveEvent(const QPoint& pos, QEvent::Type type=DragMove);
QRect answerRect() const;
void accept(const QRect& r);
void ignore(const QRect& r);
