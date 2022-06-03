@QDomImplementation();
@QDomImplementation(QDomImplementation* x);
virtual bool hasFeature(const QString& feature, const QString& version);
virtual QDomDocumentType* createDocumentType(const QString& qName, const QString& publicId, const QString& systemId);
virtual QDomDocument* createDocument(const QString& nsURI, const QString& qName, QDomDocumentType* doctype);
bool isNull();
