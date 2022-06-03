/***************************************************************************
 AQCompilerQSA.cpp
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

#include <qregexp.h>

#include <AQLexer.h>

#include "AQCompilerQSA.h"
#include "AQConfig.h"

extern int aqyyparse();

QString qs_format_error(const QString &err)
{
  if (err.startsWith(QString::fromLatin1("parse error,"))) {
    QString copy = err.mid(13);
    copy.replace(QString::fromLatin1("ABSTRACT"), QString::fromLatin1("abstract"));
    copy.replace(QString::fromLatin1("BREAK"), QString::fromLatin1("break"));
    copy.replace(QString::fromLatin1("CASE"), QString::fromLatin1("case"));
    copy.replace(QString::fromLatin1("CATCH"), QString::fromLatin1("catch"));
    copy.replace(QString::fromLatin1("CLASS"), QString::fromLatin1("class"));
    copy.replace(QString::fromLatin1("CONSTRUCTOR"), QString::fromLatin1("constructor"));
    copy.replace(QString::fromLatin1("CONTINUE"), QString::fromLatin1("continue"));
    copy.replace(QString::fromLatin1("DEFAULT"), QString::fromLatin1("default"));
    copy.replace(QString::fromLatin1("DO"), QString::fromLatin1("do"));
    copy.replace(QString::fromLatin1("ELSE"), QString::fromLatin1("else"));
    copy.replace(QString::fromLatin1("EXTENDS"), QString::fromLatin1("extends"));
    copy.replace(QString::fromLatin1("FALSETOKEN"), QString::fromLatin1("false"));
    copy.replace(QString::fromLatin1("FINAL"), QString::fromLatin1("final"));
    copy.replace(QString::fromLatin1("FINALLY"), QString::fromLatin1("finally"));
    copy.replace(QString::fromLatin1("FOR"), QString::fromLatin1("'for"));
    copy.replace(QString::fromLatin1("FUNCTION"), QString::fromLatin1("function"));
    copy.replace(QString::fromLatin1("IDENT"), QString::fromLatin1("identifier"));
    copy.replace(QString::fromLatin1("IF"), QString::fromLatin1("if"));
    copy.replace(QString::fromLatin1("IMPORT"), QString::fromLatin1("import"));
    copy.replace(QString::fromLatin1("INSTANCEOF"), QString::fromLatin1("instanceof"));
    copy.replace(QString::fromLatin1("IS"), QString::fromLatin1("is"));
    copy.replace(QString::fromLatin1("NEW"), QString::fromLatin1("new"));
    copy.replace(QString::fromLatin1("NULLTOKEN"), QString::fromLatin1("null"));
    copy.replace(QString::fromLatin1("PACKAGE"), QString::fromLatin1("package"));
    copy.replace(QString::fromLatin1("PRIVATE"), QString::fromLatin1("private"));
    copy.replace(QString::fromLatin1("PUBLIC"), QString::fromLatin1("public"));
    copy.replace(QString::fromLatin1("AQ_CONST"), QString::fromLatin1("const"));
    copy.replace(QString::fromLatin1("AQ_DELETE"), QString::fromLatin1("delete"));
    copy.replace(QString::fromLatin1("AQ_IN"), QString::fromLatin1("in"));
    copy.replace(QString::fromLatin1("AQ_VOID"), QString::fromLatin1("void"));
    copy.replace(QString::fromLatin1("RETURN"), QString::fromLatin1("return"));
    copy.replace(QString::fromLatin1("STATIC"), QString::fromLatin1("static"));
    copy.replace(QString::fromLatin1("SWITCH"), QString::fromLatin1("switch"));
    copy.replace(QString::fromLatin1("THIS"), QString::fromLatin1("this"));
    copy.replace(QString::fromLatin1("THROW"), QString::fromLatin1("throw"));
    copy.replace(QString::fromLatin1("TRUETOKEN"), QString::fromLatin1("true"));
    copy.replace(QString::fromLatin1("TRY"), QString::fromLatin1("try"));
    copy.replace(QString::fromLatin1("TYPEOF"), QString::fromLatin1("typeof"));
    copy.replace(QString::fromLatin1("VAR"), QString::fromLatin1("var"));
    copy.replace(QString::fromLatin1("WITH"), QString::fromLatin1("with"));
    copy.replace(QString::fromLatin1("RESERVED"), QString::fromLatin1("reserved keyword"));
    return copy;
  }
  return err;
}

AQCompilerQSA::AQCompilerQSA(AQLexer *l) :
  lx(l), lx_(0)
{
  if (!lx)
    lx = lx_ = new AQLexer;
}

AQCompilerQSA::~AQCompilerQSA()
{
  delete lx_;
}

QByteArray AQCompilerQSA::byteCode(const QString &code, int id, int lineno,
                                   bool humanRead, bool toPseudoCode)
{
  if (code.isEmpty())
    return QByteArray();

  QString scode(code);
  QString prefix(scode.left(45).lower());
  if (!prefix.contains("var form") &&
      !prefix.contains("266k for")) {
    lineno = 1;
    scode.prepend("var form = this;\n");
  }

#if 1
  QRegExp rx;
  rx.setPattern("ctx\\s*:\\s*Object\\s*");
  scode.replace(rx, "ctx");
  rx.setPattern("\\)\\s*:\\s*(FL\\w+|Object\\w*|String\\w*|Date\\w*|Number\\w*|Boolean\\w*|Array\\w*)");
  scode.replace(rx, ")");
  rx.setPattern(":\\s*(FL\\w+|Object\\w*|String\\w*|Date\\w*|Number\\w*|Boolean\\w*|Array\\w*)\\s*\\=");
  scode.replace(rx, "=");
  rx.setPattern("[\n\r]{3,}");
  scode.replace(rx, "\n");
  //rx.setMinimal(true);
  //rx.setPattern("class\\s+(\\w+)\\s+extends\\s+\\1([\\s\n]*\\{.*\\}[\\s\n]*\\})");
  //scode.replace(rx, "/* ¡¡ ERROR !! : LA CLASE HEREDA DE ELLA MISMA."
  //               "\nCODIGO INHABILITADO AUTOMÁTICAMENTE POR AbanQ :\n\n"
  //               "class \\1 extends \\1 \\2\n\n ¡¡ FIN ERROR !! */");
#endif

  errType = 0;
  errLines.clear();
  errMsgs.clear();

  QByteArray byteCodeBuffer;
  {
    QDataStream out(byteCodeBuffer, IO_WriteOnly);
    lx->setCode(scode, id, lineno, &out, humanRead, toPseudoCode);
    int parseError = aqyyparse();

    if (parseError || lx->lexerState() == AQLexer::Bad) {
      errType = AQErrParseError;
      errLines.append(lx->lineNo());
      errMsgs.append(QString("Parse Error: %1").arg(qs_format_error(lx->errorMessage())));
      return QByteArray();
    }
  }

  return byteCodeBuffer;
}

int AQCompilerQSA::errorType() const
{
  return errType;
}

QValueList<int> AQCompilerQSA::errorLines() const
{
  return errLines;
}

QStringList AQCompilerQSA::errorMessages() const
{
  return errMsgs;
}

QString AQCompilerQSA::code(const QByteArray &byteCode)
{
  QDataStream in(byteCode, IO_ReadOnly);
  Q_UINT32 size;
  in >> size;

  QString strOut;
  QTextStream out(&strOut, IO_WriteOnly);
  Q_UINT8 c1;
  Q_UINT8 c2;

  while (size > 0) {
    in >> c1;
    AQ_CIN(c1);
    --size;

    if (size <= 0) {
      if (c1)
        out << QChar((Q_UINT16)c1);
      break;
    }

    in >> c2;
    AQ_CIN(c2);
    --size;

    if (c2 & 0x80) {
      out << QChar(c2, c1);
    } else {
      if (c1)
        out << QChar((Q_UINT16)c1);
      if (c2)
        out << QChar((Q_UINT16)c2);
    }
  }

  return strOut;
}
