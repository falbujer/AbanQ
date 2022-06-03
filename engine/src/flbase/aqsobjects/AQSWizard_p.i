@QWizard(QWidget* parent=0, const char* name=0, bool modal=FALSE, WFlags f=0);
void addPage(QWidget* page, const QString& title);
void insertPage(QWidget* page, const QString& title, int index);
void removePage(QWidget* page);
QString title(QWidget* page) const;
void setTitle(QWidget* page, const QString& title);
void showPage(QWidget* page);
QWidget* currentPage() const;
QWidget* page(int index) const;
int pageCount() const;
int indexOf(QWidget* page) const;
bool appropriate(QWidget* page) const;
void setAppropriate(QWidget* page, bool appropriate);
QPushButton* backButton() const;
QPushButton* nextButton() const;
QPushButton* finishButton() const;
QPushButton* cancelButton() const;
QPushButton* helpButton() const;