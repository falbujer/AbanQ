@QDomNode();
@QDomNode(QDomNode* n);
virtual QDomNode* insertBefore(QDomNode* newChild, QDomNode* refChild);
virtual QDomNode* insertAfter(QDomNode* newChild, QDomNode* refChild);
virtual QDomNode* replaceChild(QDomNode* newChild, QDomNode* oldChild);
virtual QDomNode* removeChild(QDomNode* oldChild);
virtual QDomNode* appendChild(QDomNode* newChild);
virtual bool hasChildNodes() const;
virtual QDomNode* cloneNode(bool deep=TRUE) const;
virtual void normalize();
virtual bool isSupported(const QString& feature, const QString& version) const;
virtual QString nodeName() const;
virtual uint nodeType() const;
virtual QDomNode* parentNode() const;
virtual QDomNodeList* childNodes() const;
virtual QDomNode* firstChild() const;
virtual QDomNode* lastChild() const;
virtual QDomNode* previousSibling() const;
virtual QDomNode* nextSibling() const;
virtual QDomNamedNodeMap* attributes() const;
virtual QDomDocument* ownerDocument() const;
virtual QString namespaceURI() const;
virtual QString localName() const;
virtual bool hasAttributes() const;
virtual QString nodeValue() const;
virtual void setNodeValue(const QString& v);
virtual QString prefix() const;
virtual void setPrefix(const QString& pre);
virtual bool isAttr() const;
virtual bool isCDATASection() const;
virtual bool isDocumentFragment() const;
virtual bool isDocument() const;
virtual bool isDocumentType() const;
virtual bool isElement() const;
virtual bool isEntityReference() const;
virtual bool isText() const;
virtual bool isEntity() const;
virtual bool isNotation() const;
virtual bool isProcessingInstruction() const;
virtual bool isCharacterData() const;
virtual bool isComment() const;
QDomNode* namedItem(const QString& name) const;
bool isNull() const;
void clear();
QDomAttr* toAttr();
QDomCDATASection* toCDATASection();
QDomDocumentFragment* toDocumentFragment();
QDomDocument* toDocument();
QDomDocumentType* toDocumentType();
QDomElement* toElement();
QDomEntityReference* toEntityReference();
QDomText* toText();
QDomEntity* toEntity();
QDomNotation* toNotation();
QDomProcessingInstruction* toProcessingInstruction();
QDomCharacterData* toCharacterData();
QDomComment* toComment();
void save(QTextStream* str, int indent) const;