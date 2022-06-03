/***************************************************************************
 AQPackager.h
 -------------------
 begin                : 06/03/2011
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

#ifndef AQPACKAGER_H_
#define AQPACKAGER_H_

#include <AQUnpacker.h>

class AQPackager
{
public:
  AQPackager(const QString &output = QString::null);

  bool pack(const QString &input, bool cleanTmp = true,
            bool tgz = false, bool pseudoCode = false,
            bool roll = true, bool unifyActions = false);
  bool unpack(const QString &input);
  QString output() const;
  QString outputPackage() const;

  void setEncodeUtf8(bool b = true) {
    encodeUtf8_ = b;
  }

  QStringList errorMessages() const;
  QStringList logMessages() const;

  void setFilter(const QStringList &fileList) {
    filter_ = fileList;
  }
  QStringList filter() const {
    return filter_;
  }

private:
  bool createOutputDir();

  bool packModulesDef();
  bool packFilesDef(bool pseudoCode, bool roll, bool unifyActions);
  bool packTextFile(const QString &src, QString &fileName, QString &sha,
                    const QString &firstXml, const QStringList &restXml,
                    const QString &cntActionsDef, bool hasActionsDef,
                    bool pseudoCode, bool roll);
  bool packBinaryFile(const QString &src, QString &fileName, QString &sha);

  bool unpackModulesDef();
  bool unpackFilesDef();
  bool unpackFiles();
  bool unpackTextFile(const QString &fileName, const QString &sha);
  bool unpackBinaryFile(const QString &fileName, const QString &sha);

  bool packTgz(bool cleanTmp, bool pseudoCode, bool roll, bool unifyActions);
  bool createActionsDef(QString &firstXml, QStringList &restXml,
                        QString &content, bool onlyFileNames = false);

  QString input_;
  QString output_;

  QString outputPackage_;
  QDataStream *outPackage_;
  QDataStream *outCompressData_;

  AQUnpacker *unpacker_;

  bool encodeUtf8_;

  QStringList errMsgs_;
  QStringList logMsgs_;
  QStringList filter_;
};

#endif /* AQPACKAGER_H_ */
