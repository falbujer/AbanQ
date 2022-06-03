@QIMEvent(QEvent::Type type, const QString& text, int cursorPosition);
const QString& text() const;
int cursorPos() const;
bool isAccepted() const;
void accept();
void ignore();
int selectionLength() const;
