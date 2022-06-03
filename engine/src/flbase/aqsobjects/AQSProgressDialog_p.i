@QProgressDialog(QWidget* creator=0, const char* name=0, bool modal=FALSE, WFlags f=0);
@QProgressDialog(const QString& labelText, const QString& cancelButtonText, int totalSteps, QWidget* creator=0, const char* name=0, bool modal=FALSE, WFlags f=0);
void setLabel(QLabel* label);
void setCancelButton(QPushButton* cancelButton);
void setBar(QProgressBar* bar);
virtual QSize sizeHint() const;
QString labelText() const;
void setAutoReset(bool b);
bool autoReset() const;
void setAutoClose(bool b);
bool autoClose() const;
int minimumDuration() const;