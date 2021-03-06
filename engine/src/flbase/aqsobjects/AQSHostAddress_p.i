@QHostAddress();
@QHostAddress(Q_UINT32 ip4Addr);
@QHostAddress(Q_UINT8* ip6Addr);
@QHostAddress(const Q_IPV6ADDR& ip6Addr);
@QHostAddress(QHostAddress* address);
void setAddress(Q_UINT32 ip4Addr);
void setAddress(Q_UINT8* ip6Addr);
bool setAddress(const QString& address);
bool isIPv4Address() const;
@Q_UINT32 toIPv4Address() const;
bool isIPv6Address() const;
@Q_IPV6ADDR toIPv6Address() const;
QString toString() const;
bool isNull() const;
