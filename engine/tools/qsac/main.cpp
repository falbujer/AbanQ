/***************************************************************************
 main.cpp
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

#include <AQLexer.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdatastream.h>

#include <errno.h>

#include "AQCompilerQSA.h"

static void printUsage()
{
  fprintf(stderr, "ECMA-Script(QSA/JavaScript) bytecode compiler for AbanQ v2"
          "\n(c) 2003-20012 InfoSiAL S.L.\n\n"
          "Usage:\n"
          "    qsac [options] script-src.qs [-qso script-bytecode.qso]\n"
          "Options:\n"
          "    -utf8\n"
          "        When is set this flag, reads the contents of the files in utf8 otherwise in iso8859\n"
          "    -noencode\n"
          "        Not encode the output file\n"
          "    -human\n"
          "        Generates a human-readable version. Implies -noencode\n"
          "    -nopseudo\n"
          "        When -human is set, not generates pseudo-code for keywords\n"
          "    -help\n"
          "        Display this information and exit\n"
          "    -version\n"
          "        Display the version of qsac and exit\n");
}

int main(int argc, char **argv)
{
  QString outputFile;
  int numFiles = 0;
  int i;
  bool utf8 = false;
  bool humanRead = false;
  bool toPseudoCode = true;
  bool noEncode = false;

  for (i = 1; i < argc; i++) {
    if (qstrcmp(argv[ i ], "-version") == 0) {
      fprintf(stderr, "%s\n", AQQSAC_VERSION);
      return 0;
    } else if (qstrcmp(argv[ i ], "-utf8") == 0) {
      utf8 = true;
    } else if (qstrcmp(argv[ i ], "-human") == 0) {
      humanRead = true;
    } else if (qstrcmp(argv[ i ], "-nopseudo") == 0) {
      toPseudoCode = false;
    } else if (qstrcmp(argv[ i ], "-noencode") == 0) {
      noEncode = true;
    } else if (qstrcmp(argv[ i ], "-qso") == 0) {
      if (i == argc - 1) {
        printUsage();
        return 1;
      } else {
        i++;
        outputFile = QString::fromLatin1(argv[ i ]);
        argv[ i ][ 0 ] = '-';
      }
    } else if (qstrcmp(argv[ i ], "-help") == 0) {
      printUsage();
      return 0;
    } else if (argv[ i ][ 0 ] == '-') {
      printUsage();
      return 1;
    } else {
      numFiles++;
    }
  }

  if (numFiles == 0) {
    printUsage();
    return 1;
  }

  for (i = 1; i < argc; i++) {
    if (argv[ i ][ 0 ] == '-')
      continue;

    QFile f(QString::fromLatin1(argv[ i ]));
    if (!f.open(IO_ReadOnly)) {
      fprintf(stderr,
              "qsac error: Cannot open file '%s': %s\n", argv[ i ],
              strerror(errno));
      return 1;
    }

    QTextStream t(&f);
    t.setEncoding(utf8 ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
    QString fullText(t.read());
    f.close();

    if (outputFile.isEmpty())
      outputFile = QString::fromLatin1(argv[ i ]) + ".qso";

    AQLexer lex;
    AQCompilerQSA qsac(&lex);
    QByteArray byteCode(qsac.byteCode(fullText, 0, 0, humanRead, toPseudoCode));

    if (qsac.errorType() != AQErrNoError) {
      for (uint i = 0; i < qsac.errorLines().size(); ++i)
        fprintf(stderr, "Line %d : %s\n",
                qsac.errorLines()[i], qsac.errorMessages()[i].latin1());
      return qsac.errorType();
    }

    QFile file(outputFile);
    if (!file.open(IO_WriteOnly)) {
#if defined(_MSC_VER) && _MSC_VER >= 1400
      char buf[ 100 ];
      strerror_s(buf, sizeof(buf), errno);
      fprintf(stderr,
              "qsac error: Cannot open output file '%s': %s\n", outputFile.latin1(),
              buf);
#else
      fprintf(stderr,
              "qsac error: Cannot open output file '%s': %s\n", outputFile.latin1(),
              strerror(errno));
#endif
      return 1;
    }

    if (humanRead || noEncode) {
      QByteArray baOut;
      {
        QDataStream out(baOut, IO_WriteOnly);
        out << byteCode;
      }
      QTextStream tdst(&file);
      tdst.setEncoding(QTextStream::Latin1);
      QString outText(qsac.code(baOut));
      tdst << outText;
    } else {
      QDataStream out(&file);
      out << byteCode;
      file.close();
    }

#if 0
    file.open(IO_ReadOnly);
    fprintf(stderr, "\n%s\n", AQCompilerQSA::code(file.readAll()).latin1());
#endif

    break;
  }

  fprintf(stderr, "Generated bytecode in %s\n", outputFile.latin1());

  return 0;
}
