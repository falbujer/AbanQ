/***************************************************************************
 main.cpp
 -------------------
 begin                : 03/02/2012
 copyright            : (C) 2003-2012 by InfoSiAL S.L.
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

#include <qfile.h>
#include <qtextstream.h>

#include <errno.h>

#include "AQStyle.h"

static void printUsage()
{
  fprintf(stderr, "AbanQ QSA/JavaScript beautifier"
          "\n(c) 2003-20012 InfoSiAL S.L.\n\n"
          "Usage:\n"
          "    aqstyle [options] script-src.qs [-out script-beauty.bty]\n"
          "Options:\n"
          "    -utf8\n"
          "        When is set this flag, reads the contents of the files in utf8 otherwise in iso8859\n"
          "    -help\n"
          "        Display this information and exit\n"
          "    -version\n"
          "        Display the version of roller and exit\n");
}

int main(int argc, char **argv)
{
  QString outputFile;
  int numFiles = 0;
  int i;
  bool utf8 = false;

  for (i = 1; i < argc; i++) {
    if (qstrcmp(argv[ i ], "-version") == 0) {
      fprintf(stderr, "%s\n", AQSTYLE_VERSION);
      return 0;
    } else if (qstrcmp(argv[ i ], "-utf8") == 0) {
      utf8 = true;
    } else if (qstrcmp(argv[ i ], "-out") == 0) {
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
              "roller error: Cannot open file '%s': %s\n", argv[ i ],
              strerror(errno));
      return 1;
    }

    QTextStream t(&f);
    t.setEncoding(utf8 ? QTextStream::UnicodeUTF8 : QTextStream::Latin1);
    QString fullText(t.read());
    f.close();

    if (outputFile.isEmpty())
      outputFile = QString::fromLatin1(argv[ i ]) + ".bty";

    QFile file(outputFile);
    if (!file.open(IO_WriteOnly)) {
#if defined(_MSC_VER) && _MSC_VER >= 1400
      char buf[ 100 ];
      strerror_s(buf, sizeof(buf), errno);
      fprintf(stderr,
              "roller error: Cannot open output file '%s': %s\n", outputFile.latin1(),
              buf);
#else
      fprintf(stderr,
              "roller error: Cannot open output file '%s': %s\n", outputFile.latin1(),
              strerror(errno));
#endif
      return 1;
    }

    AQStyle bty;
    QTextStream tdst(&file);
    tdst.setEncoding(QTextStream::Latin1);
    QString outText(bty.beautify(fullText));
    tdst << outText;

    break;
  }

  fprintf(stderr, "Generated beauty code in %s\n", outputFile.latin1());

  return 0;
}
