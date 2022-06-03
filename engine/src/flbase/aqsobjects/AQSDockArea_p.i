@QDockArea(Qt::Orientation o, QDockArea::HandlePosition h=QDockArea::Normal, QWidget* parent=0, const char* name=0);
void moveDockWindow(QDockWindow* w, const QPoint& p, const QRect& r, bool swap);
void removeDockWindow(QDockWindow* w, bool makeFloating, bool swap, bool fixNewLines=TRUE);
void moveDockWindow(QDockWindow* w, int index=-1);
bool hasDockWindow(QDockWindow* w, int* index=0);
bool isEmpty() const;
bool isDockWindowAccepted(QDockWindow* dw);
void setAcceptDockWindow(QDockWindow* dw, bool accept);
int findDockWindow(QDockWindow* w);
