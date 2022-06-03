@QCanvasView(QWidget* parent=0, const char* name=0, WFlags f=0);
@QCanvasView(QCanvas* canvas, QWidget* parent=0, const char* name=0, WFlags f=0);
QCanvas* canvas() const;
void setCanvas(QCanvas* canvas);
QWMatrix* worldMatrix() const;
QWMatrix* inverseWorldMatrix() const;
bool setWorldMatrix(QWMatrix* wm);
