@QTable(QWidget* parent=0, const char* name=0);
@QTable(int numRows, int numCols, QWidget* parent=0, const char* name=0);
QHeader* horizontalHeader() const;
QHeader* verticalHeader() const;
void setItem(int row, int col, QTableItem* item);
void setText(int row, int col, const QString& text);
void setPixmap(int row, int col, QPixmap* pix);
QTableItem* item(int row, int col) const;
QString text(int row, int col) const;
QPixmap* pixmap(int row, int col) const;
void clearCell(int row, int col);
QRect cellGeometry(int row, int col) const;
int columnWidth(int col) const;
int rowHeight(int row) const;
int columnPos(int col) const;
int rowPos(int row) const;
int columnAt(int x) const;
int rowAt(int y) const;
void updateCell(int row, int col);
int currentRow() const;
int currentColumn() const;
void ensureCellVisible(int row, int col);
bool isSelected(int row, int col) const;
bool isRowSelected(int row, bool full=FALSE) const;
bool isColumnSelected(int col, bool full=FALSE) const;
void removeSelection(int num);
int currentSelection() const;
void selectCells(int start_row, int start_col, int end_row, int end_col);
void selectRow(int row);
void selectColumn(int col);
virtual void takeItem(QTableItem* i);
virtual void setCellWidget(int row, int col, QWidget* e);
QWidget* cellWidget(int row, int col) const;
void clearCellWidget(int row, int col);
QRect cellRect(int row, int col) const;
bool isRowReadOnly(int row) const;
bool isColumnReadOnly(int col) const;
void repaintSelections();
void updateHeaderStates();