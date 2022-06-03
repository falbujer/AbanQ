/***************************************************************************
 main.cpp
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

extern "C" {
#include <stdlib.h>
}

#include "FLSqlDatabase.h"
#include "FLSqlConnections.h"

#include "AQApplicationXmlRpc.h"
#include "AQConfig.h"

static uint portXmlRpc = 8080;

static inline bool silentConnect(const QString &conn)
{
  if (conn.isEmpty())
    return false;

  QString user, namedb, db, host, port, password;
  QStringList dat = QStringList::split(":", conn, true);
  int i = 0;
  for (QStringList::Iterator it = dat.begin(); it != dat.end(); ++it, ++i) {
    switch (i) {
      case 0:
        namedb = *it;
        break;
      case 1:
        user = *it;
        break;
      case 2:
        db = *it;
        break;
      case 3:
        host = *it;
        break;
      case 4:
        port = *it;
        break;
      case 5:
        password = *it;
        break;
    }
  }

  FLSqlDatabase *sqlDb = new FLSqlDatabase();

  if (!sqlDb->loadDriver(FLSqlDatabase::driverAliasToDriverName(db))) {
    delete sqlDb;
    return false;
  }

  if (!sqlDb->connectDB(namedb, user, password, host, port.toInt())) {
    delete sqlDb;
    return false;
  }

  FLSqlConnections::addDatabase(sqlDb);
  return true;
}

AQ_DECL_MAIN {
  QString formAlone, callFunction, arguments, strConn, silentConn;
  bool quitAfterCall = false, autoLogin_ = false, noMax = false;

  for (int i = 1; i < argc; i++) {
    if (qstrcmp(argv[i], "-f") == 0) {
      if (i != argc - 1) {
        i++;
        formAlone = QString::fromLatin1(argv[i]);
      }
    } else if (qstrcmp(argv[i], "-c") == 0) {
      if (i != argc - 1) {
        i++;
        callFunction = QString::fromLatin1(argv[i]);
      }
    } else if (qstrcmp(argv[i], "-a") == 0) {
      if (i != argc - 1) {
        i++;
        arguments = QString::fromLatin1(argv[i]);
      }
    } else if (qstrcmp(argv[i], "-sc") == 0) {
      if (i != argc - 1) {
        i++;
        strConn = QString::fromLatin1(argv[i]);
      }
    } else if (qstrcmp(argv[i], "-silentconn") == 0) {
      if (i != argc - 1) {
        i++;
        silentConn = QString::fromLatin1(argv[i]);
      }
    } else if (qstrcmp(argv[i], "-q") == 0) {
      quitAfterCall = true;
    } else if (qstrcmp(argv[i], "-auto") == 0) {
      autoLogin_ = true;
    } else if (qstrcmp(argv[i], "-nomax") == 0) {
      noMax = true;
    } else if (qstrcmp(argv[i], "-portXmlRpc") == 0) {
      if (i != argc - 1) {
        i++;
        portXmlRpc = QString::fromLatin1(argv[i]).toUInt();
      }
    }
  }

  FLApplication *AbanQ = aqApp;

  QStringList paths(AbanQ->libraryPaths());
  QString pathApp(AQ_PREFIX + "/plugins");
  if (!paths.contains(pathApp))
    paths << pathApp;
  if (!paths.contains(AQ_LIB))
    paths << AQ_LIB;
  AbanQ->setLibraryPaths(paths);

  AbanQ->installTranslator(AbanQ->createSysTranslator(QString(QTextCodec::locale()).left(2), true));
  AbanQ->installTranslator(AbanQ->createSysTranslator("multilang"));

  if (!silentConnect(silentConn)) {
    QTimer::singleShot(0, AbanQ, SLOT(quit()));
    return;
  }

  AbanQ->setNotExit(false);
  AbanQ->processEvents();
  AbanQ->init(formAlone, callFunction, arguments, quitAfterCall, noMax);
}

int main(int argc, char **argv)
{
  AQApplicationXmlRpc app(argc, argv);
  AQ_IMPL_MAIN(argc, argv);
  app.setPort(portXmlRpc);
  QTimer::singleShot(100, &app, SLOT(runXmlRpcServer()));
  return app.exec();
  exit(0);
}
