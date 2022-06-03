/***************************************************************************
 AQApplicationXmlRpc.cpp
 -------------------
 begin                : 23/09/2011
 copyright            : (C) 2003-2011 by InfoSiAL S.L.
 email                : mail@infosial.com
 ***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
 Este  programa es software libre. Puede redistribuirlo y/o modificarlo
 bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
 versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#include <qcache.h>

#include "AQApplicationXmlRpc.h"
#include "AQS_p.h"

extern AQS *globalAQS;

struct tcpPortAddr {
  unsigned char  ipAddr[4];
  unsigned short portNumber;
};

static inline struct tcpPortAddr tcpAddrFromSockAddr(struct sockaddr const sockAddr) {
  const struct sockaddr_in *const sockAddrInP(
    static_cast<struct sockaddr_in *>((void *)&sockAddr));

  const unsigned char *const ipAddr(
    static_cast<const unsigned char *>(
      (const void *)&sockAddrInP->sin_addr.s_addr)
  ); // 4 byte array

  struct tcpPortAddr retval;

  if (sockAddrInP->sin_family == AF_INET) {
    retval.ipAddr[0] = ipAddr[0];
    retval.ipAddr[1] = ipAddr[1];
    retval.ipAddr[2] = ipAddr[2];
    retval.ipAddr[3] = ipAddr[3];
    retval.portNumber = ntohs(sockAddrInP->sin_port);
  } else {
    retval.ipAddr[0] = 0;
    retval.ipAddr[1] = 0;
    retval.ipAddr[2] = 0;
    retval.ipAddr[3] = 0;
    retval.portNumber = 0;
  }

  return retval;
}

static inline std::string rpcIpAddrMsg(xmlrpc_c::callInfo_serverAbyss const &callInfo)
{
  void *chanInfoPtr;
  SessionGetChannelInfo(callInfo.abyssSessionP, &chanInfoPtr);

  struct abyss_unix_chaninfo *const chanInfoP(
    static_cast<struct abyss_unix_chaninfo *>(chanInfoPtr));

  struct tcpPortAddr const tcpAddr(tcpAddrFromSockAddr(chanInfoP->peerAddr));

  char msg[128];

  sprintf(msg, "%u.%u.%u.%u:%hu",
          tcpAddr.ipAddr[0],
          tcpAddr.ipAddr[1],
          tcpAddr.ipAddr[2],
          tcpAddr.ipAddr[3],
          tcpAddr.portNumber);

  return std::string(msg);
}

static inline QString decodeByteArray(const QString &in)
{
#ifdef AQ_XMLRPC_RAW_RESPONSE
  return in;
#else
  QByteArray ba;
  {
    QTextStream ts(ba, IO_WriteOnly);
    ts << in;
  }
  ba = globalAQS->fromBase64(&ba);
  return globalAQS->decryptInternal(&ba);
#endif
}

static inline QString encodeByteArray(const QString &in)
{
#ifdef AQ_XMLRPC_RAW_RESPONSE
  return in;
#else
  QByteArray ba;
  {
    QTextStream ts(ba, IO_WriteOnly);
    ts << in;
  }
  ba = globalAQS->encryptInternal(&ba);
  return globalAQS->toBase64(&ba);
#endif
}

class callScriptFunction : public xmlrpc_c::method2
{
public:
  callScriptFunction() {
    this->_signature = "s:ss";
    this->_help = "callScriptFunction";
    serialCache_ = 0;
    cache_.setAutoDelete(true);
  }

  bool cacheInsert(const QString &key, const QString &str) {
    QString *s = new QString(str);
    if (!cache_.insert(key, s)) {
      delete s;
      return false;
    }
    return true;
  }

  void execute(xmlrpc_c::paramList const &paramList,
               const xmlrpc_c::callInfo *const callInfoPtr,
               xmlrpc_c::value *const retvalP) {
    const xmlrpc_c::callInfo_serverAbyss *const callInfoP(
      dynamic_cast<const xmlrpc_c::callInfo_serverAbyss *>(callInfoPtr));

    std::string const functionName(paramList.getString(0));
    std::string const arguments(paramList.getString(1));

    paramList.verifyEnd(2);

    QString fn(functionName);
    int pos = fn.find('}');
#ifdef AQ_XMLRPC_RAW_RESPONSE
    bool nocache = true;
#else
    bool nocache = fn.left(1) != "#";
#endif
    QString uuid(fn.mid(1, pos));
    fn = decodeByteArray(fn.mid(pos + 1));
    QString args(decodeByteArray(arguments));

    QString key(uuid + fn + args);
    if (!nocache) {
      QString *sc = cache_.find(key);
      if (sc) {
        QString retval(*sc);
        retval = retval.left(retval.find('@') + 1);
        *retvalP = xmlrpc_c::value_string(retval);

        qWarning(QDateTime::currentDateTime().toString(Qt::ISODate) + "  : " +
                 uuid + " : " + fn + " : " + args.left(100) +
                 "\nHit cache : " + retval.left(100) + "\n");

        sleep(1);
        return;
      }
    }

    QString callInfo;
    QSArgumentList arglist;
    arglist.append(QVariant(fn));
    arglist.append(QVariant(args));
    if (callInfoP != NULL) {
      callInfo = rpcIpAddrMsg(*callInfoP);
      arglist.append(QVariant(callInfo));
    }

    QSArgument ret = aqApp->call("aqremote.serveXmlRpc", arglist, 0);

    QString retStr;
    if (ret.type() == QSArgument::Variant) {
      retStr = ret.variant().toString();
    } else {
      retStr = "invalid";
    }
#ifdef AQ_XMLRPC_RAW_RESPONSE
    QString retval(encodeByteArray(retStr));
#else
    QString serial(QString::number(serialCache_++) + QString::fromLatin1("@"));
    QString retval(serial + encodeByteArray(retStr));
#endif
    *retvalP = xmlrpc_c::value_string(retval);

    qWarning(QDateTime::currentDateTime().toString(Qt::ISODate) + "  : " +
             uuid + " : " + callInfo + " : " + fn + " : " + args.left(100) +
             "\n" + retStr.left(100) + "\n");

    if (!nocache)
      cacheInsert(key, retval);

    sleep(1);
  }

private:
  Q_ULLONG serialCache_;
  QCache<QString> cache_;
};

AQApplicationXmlRpc::AQApplicationXmlRpc(int &argc, char **argv)
  : AQApplication(argc, argv)
{
}

void AQApplicationXmlRpc::runXmlRpcServer()
{
  if (aqApp->mainWidget())
    aqApp->mainWidget()->hide();

  xmlrpc_c::registry registry;
  xmlrpc_c::methodPtr const callScriptFunctionP(new callScriptFunction);

  registry.addMethod("callScriptFunction", callScriptFunctionP);

#ifdef AQ_XMLRPC_NOLOG
  xmlrpc_c::serverAbyss xmlRpcServer(
    xmlrpc_c::serverAbyss::constrOpt()
    .registryP(&registry)
    .portNumber(port_)
    .keepaliveTimeout(60)
    .keepaliveMaxConn(256)
    .allowOrigin("*")
  );
#else
  QString log("/var/log/abanq_updater/xmlrpc-%1.log");
  xmlrpc_c::serverAbyss xmlRpcServer(
    xmlrpc_c::serverAbyss::constrOpt()
    .registryP(&registry)
    .portNumber(port_)
    .logFileName(log.arg(port_))
    .keepaliveTimeout(60)
    .keepaliveMaxConn(256)
    .allowOrigin("*")
  );
#endif
  xmlRpcServer.run();
}
