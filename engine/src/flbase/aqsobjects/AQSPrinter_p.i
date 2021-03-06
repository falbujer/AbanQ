@QPrinter(QPrinter::PrinterMode m=QPrinter::ScreenResolution);
QString printerName() const;
void setPrinterName(const QString& name);
bool outputToFile() const;
void setOutputToFile(bool enable);
QString outputFileName() const;
void setOutputFileName(const QString& fileName);
QString printProgram() const;
void setPrintProgram(const QString& printProg);
QString printerSelectionOption() const;
void setPrinterSelectionOption(const QString& option);
QString docName() const;
void setDocName(const QString& name);
QString creator() const;
void setCreator(const QString& creator);
uint orientation() const;
void setOrientation(QPrinter::Orientation orientation);
uint pageSize() const;
void setPageSize(QPrinter::PageSize newPageSize);
void setPageOrder(QPrinter::PageOrder newPageOrder);
uint pageOrder() const;
void setResolution(int dpi);
int resolution() const;
void setColorMode(QPrinter::ColorMode newColorMode);
uint colorMode() const;
void setFullPage(bool fp);
bool fullPage() const;
QSize margins() const;
void setMargins(uint top, uint left, uint bottom, uint right);
int fromPage() const;
int toPage() const;
void setFromTo(int fromPage, int toPage);
int minPage() const;
int maxPage() const;
void setMinMax(int minPage, int maxPage);
int numCopies() const;
void setNumCopies(int numCopies);
uint printRange() const;
void setPrintRange(QPrinter::PrintRange range);
bool newPage();
bool abort();
bool aborted() const;
bool setup(QWidget* parent=0);
uint paperSource() const;
void setPaperSource(QPrinter::PaperSource source);
void setOptionEnabled(QPrinter::PrinterOption option, bool enable);
bool isOptionEnabled(QPrinter::PrinterOption option);
