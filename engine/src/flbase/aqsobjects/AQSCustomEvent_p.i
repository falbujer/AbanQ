@QCustomEvent(int type);
@QCustomEvent(QEvent::Type type, void* data);
void* data() const;
void setData(void* data);
