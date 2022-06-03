@AQJasperEngine(QObject* parent=0);
bool setReportData(QDomNode* n);
bool setReportTemplate(const QString& t);
QDomDocument rptXmlData() const;
QDomDocument rptXmlTemplate() const;
