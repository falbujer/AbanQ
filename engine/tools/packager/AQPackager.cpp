/***************************************************************************
 AQPackager.cpp
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

#include <qdir.h>
#include <qregexp.h>
#include <qdom.h>
#include <qbuffer.h>
#include <qprocess.h>
#include <qapplication.h>
#include <errno.h>

#include <AQLexer.h>
#include <AQCompilerQSA.h>
#include <AQRoller.h>
#include <FLSha1.h>

#include "AQPackager.h"

#define AQ_NO_PACKAGE_EXPIRES

class AQBlockingProcess : public QProcess
{
  Q_OBJECT

public:
  AQBlockingProcess() {
    connect(this, SIGNAL(readyReadStdout()), this, SLOT(readOut()));
    connect(this, SIGNAL(readyReadStderr()), this, SLOT(readErr()));
    connect(this, SIGNAL(processExited()), this, SLOT(exited()));
    connect(this, SIGNAL(wroteToStdin()), this, SLOT(closeStdin()));
    outUsed = errUsed = 0;
    procApp = 0;
  }

  void waitExit() {
    if (!qApp) {
      int argc = 0;
      char **argv = 0;
      QApplication app(argc, argv);
      procApp = &app;
      app.exec();
    } else {
      procApp = 0;
      qApp->enter_loop();
    }
  }

public slots:
  void readOut() {
    QByteArray rout = readStdout();
    if (outUsed + rout.size() > out.size())
      out.resize(outUsed + rout.size());
    memcpy(out.data() + outUsed, rout, rout.size());
    outUsed += rout.size();
  }

  void readErr() {
    QByteArray rerr = readStderr();
    if (errUsed + rerr.size() > err.size())
      err.resize(errUsed + rerr.size());
    memcpy(err.data() + errUsed, rerr, rerr.size());
    errUsed += rerr.size();
  }

  void exited() {
    if (procApp)
      procApp->exit();
    else if (qApp)
      qApp->exit_loop();
    procApp = 0;
  }

public:
  QByteArray out;
  QByteArray err;
  int outUsed;
  int errUsed;
  QApplication *procApp;
};

static inline QStringList findFiles(const QStringList &paths,
                                    const QString &filter,
                                    bool includeFileName = false,
                                    bool breakOnFirstMatch = false,
                                    bool allowDuplicates = true)
{
  QStringList result, more;
  QStringList::Iterator it;
  for (uint i = 0; i < paths.size(); i++) {
    QDir dir(paths[i]);
    more = dir.entryList(filter, QDir::Files);

    for (it = more.begin(); it != more.end(); ++it) {
      QString pt(includeFileName
                 ? paths[i] + QString::fromLatin1("/") + *it
                 : paths[i]);
      if (breakOnFirstMatch)
        return pt;
      if (!allowDuplicates && result.contains(pt))
        continue;
      result.append(pt);
    }

    more = dir.entryList(QDir::Dirs).grep(QRegExp("[^.]"));
    for (it = more.begin(); it != more.end(); ++it)
      *it = paths[i] + QString::fromLatin1("/") + *it;
    more = findFiles(more, filter, includeFileName, breakOnFirstMatch);

    for (it = more.begin(); it != more.end(); ++it) {
      if (breakOnFirstMatch)
        return *it;
      if (!allowDuplicates && result.contains(*it))
        continue;
      result.append(*it);
    }
  }
  return result;
}

static inline QString sha1(const QString &str)
{
  QBuffer b;
  b.open(IO_WriteOnly);
  QTextStream t(&b);
  t << str;
  b.close();

  FLSha1 sha1;
  sha1.Update((unsigned char *) b.buffer().data(), b.buffer().size());
  sha1.Final();
  char strC[ 255 ];
  strC[ 0 ] = '\0';
  sha1.ReportHash(strC);

  return strC;
}

static inline QString usha1(uchar *data, uint len)
{
  FLSha1 sha1;
  sha1.Update(data, len);
  sha1.Final();
  char strC[ 255 ];
  strC[ 0 ] = '\0';
  sha1.ReportHash(strC);
  return strC;
}

static inline bool textPacking(const QString &ext)
{
  return ext.endsWith(".ui") ||
         ext.endsWith(".qry") ||
         ext.endsWith(".kut") ||
         ext.endsWith(".jrxml") ||
         ext.endsWith(".ar") ||
         ext.endsWith(".mtd") ||
         ext.endsWith(".ts") ||
         ext.endsWith(".qs") ||
         ext.endsWith(".xml") ||
         ext.endsWith(".xpm") ||
         ext.endsWith(".svg");
}

static inline bool binaryPacking(const QString &ext)
{
  return ext.endsWith(".qs");
}

static inline QString moduleId(const QString &path, bool encodeUtf8 = false)
{
  QFile f(path);
  f.open(IO_ReadOnly);
  QTextStream t(&f);
  t.setEncoding(encodeUtf8 ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
  QDomDocument doc;
  doc.setContent(t.read());
  QDomNode nodeModule(doc.namedItem("MODULE"));
  return nodeModule.namedItem("name").toElement().text();
}

AQPackager::AQPackager(const QString &output)
  : outPackage_(0), outCompressData_(0), unpacker_(0), encodeUtf8_(false)
{
  output_ = QDir::cleanDirPath(output);
  if (output_.isEmpty())
    output_ = QString::fromLatin1("./packager-out");
  if (!output.endsWith("/"))
    output_.append('/');
}

bool AQPackager::pack(const QString &input, bool cleanTmp, bool tgz,
                      bool pseudoCode, bool roll, bool unifyActions)
{
  errMsgs_.clear();
  logMsgs_.clear();

  input_ = QDir::cleanDirPath(input);

  QDir din(input_);
  if (!din.exists() || !din.isReadable()) {
    AQ_STRERROR(input_);
    return false;
  }

  if (tgz)
    return packTgz(cleanTmp, pseudoCode, roll, unifyActions);

  bool ok = createOutputDir();

  QDir dout(output_);
  dout.cdUp();
  outputPackage_ = dout.absPath() + '/' +
                   QDir(output_).dirName() +
                   QString::fromLatin1(".abanq");
  QFile filePack(outputPackage_);
  if (ok && !filePack.open(IO_WriteOnly)) {
    AQ_STRERROR(outputPackage_);
    ok = false;
  }

  QString compressData(outputPackage_ + QString::fromLatin1(".z"));
  QFile fileCompressData(compressData);
  if (ok && !fileCompressData.open(IO_WriteOnly)) {
    AQ_STRERROR(compressData);
    ok = false;
  }

  if (ok) {
    outPackage_ = new QDataStream(&filePack);
    (*outPackage_) << AQPACKAGER_VERSION;
    (*outPackage_) << outPackage_->byteOrder();
#ifdef AQ_NO_PACKAGE_EXPIRES
    (*outPackage_) << (Q_UINT32) 0;
    (*outPackage_) << (Q_UINT32) 0;
#else
    (*outPackage_) << time(0);
#endif
    outCompressData_ = new QDataStream(&fileCompressData);
  }

  if (ok)
    ok = packModulesDef();

  if (ok)
    ok = packFilesDef(pseudoCode, roll, unifyActions);

  delete outCompressData_;

  if (ok) {
    fileCompressData.close();
    if (ok && !fileCompressData.open(IO_ReadOnly)) {
      AQ_STRERROR(compressData);
      ok = false;
    }
    if (ok) {
      QDataStream dtc(&fileCompressData);
      while (!dtc.atEnd()) {
        QByteArray ba;
        dtc >> ba;
        (*outPackage_) << ba;
      }
    }
  }

  delete outPackage_;
  QFile::remove(compressData);

  if (cleanTmp) {
    dout.cd(output_);
    QStringList l(dout.entryList("*"));
    for (QStringList::const_iterator it = l.begin(); it != l.end(); ++it)
      dout.remove(*it);
    dout.cdUp();
    dout.rmdir(output_);
  }

  return ok;
}

bool AQPackager::unpack(const QString &input)
{
  bool ok = createOutputDir();

  unpacker_ = new AQUnpacker(input);
  if (!unpacker_->errorMessages().isEmpty()) {
    errMsgs_ += unpacker_->errorMessages();
    ok = false;
  }

  if (ok)
    ok = unpackModulesDef();

  if (ok)
    ok = unpackFilesDef();

  if (ok)
    ok = unpackFiles();

  delete unpacker_;

  return ok;
}

QString AQPackager::output() const
{
  return output_;
}

QString AQPackager::outputPackage() const
{
  return outputPackage_;
}

QStringList AQPackager::errorMessages() const
{
  return errMsgs_;
}

QStringList AQPackager::logMessages() const
{
  return logMsgs_;
}

bool AQPackager::createOutputDir()
{
  QDir d;
  if (!d.mkdir(output_)) {
    AQ_STRERROR(output_);
    return false;
  }
  return true;
}

bool AQPackager::packModulesDef()
{
  QDomDocument doc("modules_def");
  QDomElement root(doc.createElement("modules"));

  doc.appendChild(root);

  QString errMsg;
  int errLine, errColumn;
  bool ok = true;
  QStringList l(findFiles(input_, "*.mod", true));

  for (QStringList::const_iterator it = l.begin(); it != l.end(); ++it) {
    QFile fmod(*it);
    if (!fmod.open(IO_ReadOnly)) {
      AQ_STRERROR((*it));
      ok = false;
      continue;
    }

    QTextStream t(&fmod);
    t.setEncoding(encodeUtf8_ ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
    QDomDocument docTmp;
    if (!docTmp.setContent(t.read(), &errMsg, &errLine, &errColumn)) {
      errMsgs_ << QString().sprintf("'%s': XML error %s  line: %d  column: %d",
                                    (*it).latin1(), errMsg.latin1(),
                                    errLine, errColumn);
      ok = false;
      continue;
    }

    root.appendChild(docTmp.firstChild());
  }

  QString proFile(input_ + "/mvproject.xml");
  if (QFile::exists(proFile)) {
    QFile fpro(proFile);
    if (!fpro.open(IO_ReadOnly)) {
      AQ_STRERROR(proFile);
      ok = false;
    } else {
      QTextStream tpro(&fpro);
      tpro.setEncoding(encodeUtf8_ ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
      QDomDocument docPro;
      QString cnt(tpro.read());
      if (!docPro.setContent(cnt, &errMsg, &errLine, &errColumn)) {
        errMsgs_ << QString().sprintf("'%s': XML error %s  line: %d  column: %d",
                                      proFile.latin1(), errMsg.latin1(),
                                      errLine, errColumn);
        ok = false;
      } else {
        QString verPro(docPro.firstChild().toElement().attribute("version"));
        QString namPro(docPro.firstChild().toElement().attribute("name"));
        root.setAttribute("projectversion", verPro);
        root.setAttribute("projectname", namPro);
        doc.appendChild(doc.createComment(cnt));
      }
    }
  }

  QString outFile(output_ + QString::fromLatin1("modules.def"));
  QFile fout(outFile);

  if (!fout.open(IO_WriteOnly)) {
    AQ_STRERROR(outFile);
    ok = false;
  } else {
    QTextStream tout(&fout);
    doc.save(tout, 2);
    fout.close();
  }

  if (!fout.open(IO_ReadOnly)) {
    AQ_STRERROR(outFile);
    ok = false;
  } else {
    QTextStream ti(&fout);
    QByteArray ba;
    QTextOStream to(ba);
    to << ti.read();
    (*outPackage_) << qCompress(ba);
  }

  return ok;
}

bool AQPackager::packFilesDef(bool pseudoCode, bool roll, bool unifyActions)
{
  QDomDocument doc("files_def");
  QDomElement root(doc.createElement("files"));

  doc.appendChild(root);

  QString firstXml;
  QStringList restXml;
  QString cntActionsDef;
  bool hasActionsDef = unifyActions
                       ? createActionsDef(firstXml, restXml, cntActionsDef, true)
                       : false;

  bool ok = true;
  QString shaSum;
  QString shaSumTxt;
  QString shaSumBin;
  QStringList exts;
  exts << QString::fromLatin1("*.ui")
       << QString::fromLatin1("*.qry")
       << QString::fromLatin1("*.kut")
       << QString::fromLatin1("*.jrxml")
       << QString::fromLatin1("*.ar")
       << QString::fromLatin1("*.mtd")
       << QString::fromLatin1("*.ts")
       << QString::fromLatin1("*.qs")
       << QString::fromLatin1("*.xml")
       << QString::fromLatin1("*.xpm")
       << QString::fromLatin1("*.svg");

  QStringList m(findFiles(input_, "*.mod", true));
  for (QStringList::const_iterator itM = m.begin(); itM != m.end(); ++itM) {
    QString mId(moduleId(*itM, encodeUtf8_));
    QString mPath(*itM);
    mPath.truncate((*itM).findRev('/'));

    for (QStringList::const_iterator itE = exts.begin(); itE != exts.end(); ++itE) {
      QStringList l(findFiles(mPath, *itE, true));
      QDomElement ne;
      QDomText nt;
      QString fName;
      QString sha;

      for (QStringList::const_iterator it = l.begin(); it != l.end(); ++it) {
        QDomElement nf(doc.createElement("file"));
        root.appendChild(nf);

        ne = doc.createElement("module");
        nf.appendChild(ne);
        nt = doc.createTextNode(mId);
        ne.appendChild(nt);

        ne = doc.createElement("name");
        nf.appendChild(ne);
        fName = (*it).mid((*it).findRev('/') + 1);
        nt = doc.createTextNode(fName);
        ne.appendChild(nt);

        ne = doc.createElement("skip");
        nf.appendChild(ne);
        bool skip = (!filter_.isEmpty() && !filter_.contains(fName));
        nt = doc.createTextNode(skip ? "true" : "false");
        ne.appendChild(nt);

        if (textPacking(*itE)) {
          ne = doc.createElement("text");
          nf.appendChild(ne);
          if (!packTextFile(*it, fName, sha, firstXml,
                            restXml, cntActionsDef, hasActionsDef,
                            pseudoCode, roll)) {
            ok = false;
            continue;
          }
          nt = doc.createTextNode(fName);
          ne.appendChild(nt);

          ne = doc.createElement("shatext");
          nf.appendChild(ne);
          nt = doc.createTextNode(sha);
          ne.appendChild(nt);

          shaSum = sha1(shaSum + sha);
          shaSumTxt = sha1(shaSumTxt + sha);
        }

        if (binaryPacking(*itE)) {
          ne = doc.createElement("binary");
          nf.appendChild(ne);
          if (!packBinaryFile(*it, fName, sha)) {
            ok = false;
            continue;
          }
          nt = doc.createTextNode(fName);
          ne.appendChild(nt);

          ne = doc.createElement("shabinary");
          nf.appendChild(ne);
          nt = doc.createTextNode(sha);
          ne.appendChild(nt);

          shaSum = sha1(shaSum + sha);
          shaSumBin = sha1(shaSumBin + sha);
        }
      }
    }
  }

  QDomElement ns(doc.createElement("shasum"));
  ns.appendChild(doc.createTextNode(shaSum));
  root.appendChild(ns);

  ns = doc.createElement("shasumtxt");
  ns.appendChild(doc.createTextNode(shaSumTxt));
  root.appendChild(ns);

  ns = doc.createElement("shasumbin");
  ns.appendChild(doc.createTextNode(shaSumBin));
  root.appendChild(ns);

  QString outFile(output_ + QString::fromLatin1("files.def"));
  QFile fout(outFile);

  if (!fout.open(IO_WriteOnly)) {
    AQ_STRERROR(outFile);
    ok = false;
  } else {
    QTextStream tout(&fout);
    doc.save(tout, 2);
    fout.close();
  }

  if (!fout.open(IO_ReadOnly)) {
    AQ_STRERROR(outFile);
    ok = false;
  } else {
    QTextStream ti(&fout);
    QByteArray ba;
    QTextOStream to(ba);
    to << ti.read();
    (*outPackage_) << qCompress(ba);
  }

  return ok;
}

bool AQPackager::packTextFile(const QString &src, QString &fileName, QString &sha,
                              const QString &firstXml, const QStringList &restXml,
                              const QString &cntActionsDef, bool hasActionsDef,
                              bool pseudoCode, bool roll)
{
  QFile fsrc(src);
  if (!fsrc.open(IO_ReadOnly)) {
    AQ_STRERROR(src);
    return false;
  }
  QString dst(output_ + fileName);
  QFile fdst(dst);
  if (!fdst.open(IO_WriteOnly)) {
    AQ_STRERROR(dst);
    return false;
  }

  QTextStream tsrc(&fsrc);
  tsrc.setEncoding(encodeUtf8_ ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
  QString fullText(tsrc.read());
  QTextStream tdst(&fdst);

  if (fileName.endsWith(".qs")) {
    AQLexer lex;
    AQCompilerQSA qsac(&lex);
    QByteArray byteCode(qsac.byteCode(fullText, 0, 0, true, pseudoCode));

    if (qsac.errorType() != AQErrNoError) {
      for (uint i = 0; i < qsac.errorLines().size(); ++i) {
        errMsgs_ << QString().sprintf("QSAC Error %d   %s Line %d : %s\n",
                                      qsac.errorType(),
                                      fileName.latin1(),
                                      qsac.errorLines()[i],
                                      qsac.errorMessages()[i].latin1());
      }
      return false;
    }

    QByteArray baOut;
    {
      QDataStream dstout(baOut, IO_WriteOnly);
      dstout << byteCode;
    }
    if (roll) {
      fullText = AQRoller::roll(qsac.code(baOut), fileName);
      logMsgs_ += AQRoller::logMessages();
    } else
      fullText = qsac.code(baOut);
    sha = usha1((uchar *)(const char *)fullText, fullText.length());
  } else {
    if (hasActionsDef && fileName.endsWith(".xml")) {
      if (restXml.contains(fileName))
        fullText = QString::fromLatin1("<ACTIONS></ACTIONS>");
      else if (firstXml == fileName)
        fullText = cntActionsDef;
    }
    sha = sha1(fullText);
  }

  tdst.setEncoding(QTextStream::Latin1);
  tdst << fullText;

  bool skip = (!filter_.isEmpty() && !filter_.contains(fileName));
  if (!skip) {
    QByteArray ba;
    QTextOStream to(ba);
    to << fullText;
    (*outCompressData_) << qCompress(ba);
  }

  return true;
}

bool AQPackager::packBinaryFile(const QString &src,
                                QString &fileName, QString &sha)
{
  bool skip = (!filter_.isEmpty() && !filter_.contains(fileName));
  fileName.append(".qso");

  QFile fsrc(src);
  if (!fsrc.open(IO_ReadOnly)) {
    AQ_STRERROR(src);
    return false;
  }
  QString dst(output_ + fileName);
  QFile fdst(dst);
  if (!fdst.open(IO_WriteOnly)) {
    AQ_STRERROR(dst);
    return false;
  }

  QTextStream t(&fsrc);
  t.setEncoding(encodeUtf8_ ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
  QString fullText(t.read());
  fsrc.close();

  //fullText = AQRoller::rollCleaner(fullText, fileName);

  AQLexer lex;
  AQCompilerQSA qsac(&lex);
  QByteArray byteCode(qsac.byteCode(fullText));

  if (qsac.errorType() != AQErrNoError) {
    for (uint i = 0; i < qsac.errorLines().size(); ++i) {
      errMsgs_ << QString().sprintf("QSAC Error %d   %s Line %d : %s\n",
                                    qsac.errorType(),
                                    fileName.latin1(),
                                    qsac.errorLines()[i],
                                    qsac.errorMessages()[i].latin1());
    }
    return false;
  }

  QDataStream out(&fdst);
  out << byteCode;

  sha = usha1((uchar *)(const char *)byteCode, byteCode.size());

  if (!skip)
    (*outCompressData_) << qCompress(byteCode);

  return true;
}

bool AQPackager::unpackModulesDef()
{
  QString outFile(output_ + QString::fromLatin1("modules.def"));
  QFile fout(outFile);

  if (!fout.open(IO_WriteOnly)) {
    AQ_STRERROR(outFile);
    return false;
  }

  QTextStream to(&fout);
  to << unpacker_->getText();

  return true;
}

bool AQPackager::unpackFilesDef()
{
  QString outFile(output_ + QString::fromLatin1("files.def"));
  QFile fout(outFile);
  if (!fout.open(IO_WriteOnly)) {
    AQ_STRERROR(outFile);
    return false;
  }

  QTextStream to(&fout);
  to << unpacker_->getText();

  return true;
}

bool AQPackager::unpackFiles()
{
  QString inFile(output_ + QString::fromLatin1("files.def"));
  QFile fin(inFile);
  QString errMsg;
  int errLine, errColumn;
  bool ok = true;
  QDomDocument doc;

  if (!doc.setContent(&fin, &errMsg, &errLine, &errColumn)) {
    errMsgs_ << QString().sprintf("'%s': XML error %s  line: %d  column: %d",
                                  inFile.latin1(), errMsg.latin1(),
                                  errLine, errColumn);
    ok = false;
  }

  QDomNode root(doc.firstChild().toElement());
  QDomNodeList files(root.childNodes());

  for (uint i = 0; i < files.length(); ++i) {
    QDomNode it(files.item(i));

    if (it.namedItem("skip").toElement().text() == "true")
      continue;

    QString text(it.namedItem("text").toElement().text());
    if (!text.isEmpty())
      ok = unpackTextFile(text, it.namedItem("shatext").toElement().text());

    QString binary(it.namedItem("binary").toElement().text());
    if (!binary.isEmpty())
      ok = unpackBinaryFile(binary, it.namedItem("shabinary").toElement().text());
  }

  return ok;
}

bool AQPackager::unpackTextFile(const QString &fileName, const QString &sha)
{
  QString dst(output_ + fileName);
  QFile fdst(dst);
  if (!fdst.open(IO_WriteOnly)) {
    AQ_STRERROR(dst);
    return false;
  }

  QTextStream to(&fdst);
  if (fileName.endsWith(".qs"))
    to.setEncoding(QTextStream::Latin1);
  QString ct(unpacker_->getText());
  to << ct;

  if (sha != sha1(ct)) {
    errMsgs_ << QString().sprintf("'%s': SHA1 Checksum error", dst.latin1());
    return false;
  }

  return true;
}

bool AQPackager::unpackBinaryFile(const QString &fileName, const QString &sha)
{
  QString dst(output_ + fileName);
  QFile fdst(dst);
  if (!fdst.open(IO_WriteOnly)) {
    AQ_STRERROR(dst);
    return false;
  }

  QDataStream dt(&fdst);
  QByteArray ct(unpacker_->getBinary());
  dt << ct;

  if (sha != usha1((uchar *)(const char *)ct, ct.size())) {
    errMsgs_ << QString().sprintf("'%s': SHA1 Checksum error", dst.latin1());
    return false;
  }

  return true;
}

bool AQPackager::packTgz(bool cleanTmp, bool pseudoCode,
                         bool roll, bool unifyActions)
{
  bool ok = createOutputDir();

  QDir dout(output_);
  QDir din(input_);
  QString absInput(din.absPath());
  QString absOutput(dout.absPath() + '/');

  dout.cdUp();
  QString dirNameOut(QDir(output_).dirName());
  QString fileNameOut(dirNameOut + QString::fromLatin1(".tar.gz"));
  outputPackage_ = dout.absPath() + '/' + fileNameOut;

  QFile filePack(outputPackage_);
  if (ok && !filePack.open(IO_WriteOnly)) {
    AQ_STRERROR(outputPackage_);
    return false;
  } else
    filePack.remove();

  QString firstXml;
  QStringList restXml;
  QString cntActionsDef;
  bool hasActionsDef = unifyActions
                       ? createActionsDef(firstXml, restXml, cntActionsDef)
                       : false;

  QString filesFilter("COPYING *.mod *.xml *.xpm ");
  filesFilter += "*.ui *.qry *.kut *.jrxml ";
  filesFilter += "*.ar *.mtd *.ts *.qs *.svg";

  QStringList dirsSrc(findFiles(absInput, filesFilter, false, false, false));
  QStringList dirsDst;

  for (QStringList::const_iterator it = dirsSrc.begin(); it != dirsSrc.end(); ++it)
    dirsDst << (absOutput + (*it).mid(absInput.length() + 1));

  for (QStringList::const_iterator it = dirsDst.begin(); it != dirsDst.end(); ++it)
    dout.mkpath(*it);

  QStringList filesSrc(findFiles(absInput, filesFilter, true));
  for (QStringList::const_iterator it = filesSrc.begin(); it != filesSrc.end(); ++it) {
    QString fileNameSrc(*it);
    QString fileNameDst(absOutput + fileNameSrc.mid(absInput.length() + 1));

    bool skip = (!filter_.isEmpty() && !filter_.contains(fileNameSrc));
    if (skip)
      continue;

    QFile fsrc(fileNameSrc);
    if (!fsrc.open(IO_ReadOnly)) {
      AQ_STRERROR(fileNameSrc);
      ok = false;
      break;
    }
    QFile fdst(fileNameDst);
    if (!fdst.open(IO_WriteOnly)) {
      AQ_STRERROR(fileNameDst);
      ok = false;
      break;
    }

    QTextStream tsrc(&fsrc);
    tsrc.setEncoding(encodeUtf8_ ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
    QString fullText(tsrc.read());
    QTextStream tdst(&fdst);

    if (fileNameSrc.endsWith(".qs")) {
      AQLexer lex;
      AQCompilerQSA qsac(&lex);
      QByteArray byteCode(qsac.byteCode(fullText, 0, 0, true, pseudoCode));

      if (qsac.errorType() != AQErrNoError) {
        for (uint i = 0; i < qsac.errorLines().size(); ++i) {
          errMsgs_ << QString().sprintf("QSAC Error %d   %s Line %d : %s\n",
                                        qsac.errorType(),
                                        fileNameSrc.latin1(),
                                        qsac.errorLines()[i],
                                        qsac.errorMessages()[i].latin1());
        }
        ok = false;
        continue;
      }

      QByteArray baOut;
      {
        QDataStream dstout(baOut, IO_WriteOnly);
        dstout << byteCode;
      }
      if (roll) {
        fullText = AQRoller::roll(qsac.code(baOut), fileNameSrc);
        logMsgs_ += AQRoller::logMessages();
      } else
        fullText = qsac.code(baOut);
    } else if (hasActionsDef && fileNameSrc.endsWith(".xml")) {
      if (restXml.contains(fileNameSrc))
        fullText = QString::fromLatin1("<ACTIONS></ACTIONS>");
      else if (firstXml == fileNameSrc)
        fullText = cntActionsDef;
    }

    tdst.setEncoding(QTextStream::Latin1);
    tdst << fullText;
  }

  if (ok) {
    dout.cd(absOutput);
    dout.cdUp();

    AQBlockingProcess proc;
    QStringList command;
    command << "tar"
            << "czf"
            << fileNameOut
            << dirNameOut;
    proc.setWorkingDirectory(dout);
    proc.setArguments(command);
    if (!proc.start()) {
      AQ_STRERROR(QString::fromLatin1("Failed to run process: '%1'")
                  .arg(command.join(QString::fromLatin1(" "))));
      ok = false;
    }
    if (ok) {
      proc.waitExit();
      if (!proc.normalExit()) {
        AQ_STRERROR(QString::fromLatin1(proc.out, proc.outUsed));
        AQ_STRERROR(QString::fromLatin1(proc.err, proc.errUsed));
        ok = false;
      }
    }
  }

  if (cleanTmp) {
    for (QStringList::const_iterator it = dirsDst.begin(); it != dirsDst.end(); ++it) {
      dout.cd(*it);
      QStringList l(dout.entryList("*"));
      for (QStringList::const_iterator it2 = l.begin(); it2 != l.end(); ++it2)
        dout.remove(*it2);
      dout.cdUp();
      dout.rmpath(*it);
    }
    dout.cd(absOutput);
    dout.cdUp();
    dout.rmdir(absOutput);
  }

  return ok;
}

bool AQPackager::createActionsDef(QString &firstXml, QStringList &restXml,
                                  QString &content, bool onlyFileNames)
{
  QDomDocument doc;
  QDomElement root(doc.createElement("ACTIONS"));

  doc.appendChild(root);

  firstXml = QString::null;
  restXml.clear();
  QString errMsg;
  int errLine, errColumn;
  bool ok = true;

  QDir din(input_);
  QString absInput(din.absPath());

  QStringList l(findFiles(absInput, "*.xml", true));

  for (QStringList::const_iterator it = l.begin(); it != l.end(); ++it) {
    QFile fmod(*it);
    if (!fmod.open(IO_ReadOnly)) {
      AQ_STRERROR((*it));
      ok = false;
      continue;
    }

    QTextStream t(&fmod);
    t.setEncoding(encodeUtf8_ ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
    QDomDocument docTmp;
    if (!docTmp.setContent(t.read(), &errMsg, &errLine, &errColumn)) {
      errMsgs_ << QString().sprintf("'%s': XML error %s  line: %d  column: %d",
                                    (*it).latin1(), errMsg.latin1(),
                                    errLine, errColumn);
      ok = false;
      continue;
    }

    QDomElement rootTmp = docTmp.documentElement();
    if (rootTmp.tagName().upper() != QString::fromLatin1("ACTIONS"))
      continue;

    QDomNodeList acts(rootTmp.childNodes());
    for (uint i = 0; i < acts.length(); ++i) {
      if (i % 2)
        continue;
      QDomNode it = acts.item(i).cloneNode();
      root.appendChild(it);
    }
    for (uint i = 0; i < acts.length(); ++i) {
      if (!(i % 2))
        continue;
      QDomNode it = acts.item(i).cloneNode();
      root.appendChild(it);
    }

    if (firstXml.isEmpty())
      firstXml = (onlyFileNames ? QFileInfo(*it).fileName() : (*it));
    else
      restXml.append(onlyFileNames ? QFileInfo(*it).fileName() : (*it));
  }

  content = doc.toString(2);

  return ok;
}

#include "AQPackager.moc"
