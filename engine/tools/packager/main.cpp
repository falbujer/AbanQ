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

#include "AQPackager.h"

static void printUsage()
{
  fprintf(stderr, "Packager for AbanQ v2"
          "\n(c) 2003-20012 InfoSiAL S.L.\n\n"
          "Usage:\n"
          "    packager [options] input [-out output]\n"
          "    'output' is optional, the default value is './packager-out'\n"
          "Options:\n"
          "    -pack\n"
          "        Pack the input directory of AbanQ's modules into the output file\n"
          "    -unpack\n"
          "        Unpack the input file created with packager into the output directory\n"
          "    -utf8\n"
          "        When is set this flag, reads the contents of the files in utf8 otherwise in iso8859\n"
          "    -noclean\n"
          "        Not clean temporary files\n"
          "    -tgz\n"
          "        Creates a .tgz package instead of a .abanq package\n"
          "    -pseudo\n"
          "        Generates pseudo-code for keywords\n"
          "    -noroll\n"
          "        Not use the tool AQRoller to generate the final code\n"
          "    -unifyactions\n"
          "        Unify all the action definition files into one file\n"
          "    -help\n"
          "        Display this information and exit\n"
          "    -version\n"
          "        Display the version of packager and exit\n");
}

int main(int argc, char **argv)
{
  QString output;
  QString input;
  bool pack = false;
  bool unpack = false;
  bool utf8 = false;
  bool cleanTmp = true;
  bool tgz = false;
  bool pseudoCode = false;
  bool roll = true;
  bool unifyActions = false;
  int numPaths = 0;
  int i;

  for (i = 1; i < argc; i++) {
    if (qstrcmp(argv[ i ], "-version") == 0) {
      fprintf(stderr, "%s\n", AQPACKAGER_VERSION);
      return 0;
    } else if (qstrcmp(argv[ i ], "-pack") == 0) {
      pack = true;
    } else if (qstrcmp(argv[ i ], "-unpack") == 0) {
      unpack = true;
    } else if (qstrcmp(argv[ i ], "-utf8") == 0) {
      utf8 = true;
    } else if (qstrcmp(argv[ i ], "-noclean") == 0) {
      cleanTmp = false;
    } else if (qstrcmp(argv[ i ], "-tgz") == 0) {
      tgz = true;
    } else if (qstrcmp(argv[ i ], "-pseudo") == 0) {
      pseudoCode = true;
    } else if (qstrcmp(argv[ i ], "-noroll") == 0) {
      roll = false;
    } else if (qstrcmp(argv[ i ], "-unifyactions") == 0) {
      unifyActions = true;
    } else if (qstrcmp(argv[ i ], "-out") == 0) {
      if (i == argc - 1) {
        printUsage();
        return 1;
      } else {
        i++;
        output = QString::fromLatin1(argv[ i ]);
        argv[ i ][ 0 ] = '-';
      }
    } else if (qstrcmp(argv[ i ], "-help") == 0) {
      printUsage();
      return 0;
    } else if (argv[ i ][ 0 ] == '-') {
      printUsage();
      return 1;
    } else {
      numPaths++;
    }
  }

  if (numPaths == 0 || (!pack && !unpack) || (pack && unpack)) {
    printUsage();
    return 1;
  }

  for (i = 1; i < argc; i++) {
    if (argv[ i ][ 0 ] == '-')
      continue;

    input = QString::fromLatin1(argv[ i ]);
    if (output.isEmpty())
      output = QString::fromLatin1("./packager-out");

    AQPackager packager(output);
    packager.setEncodeUtf8(utf8);
    bool ok = (pack
               ? packager.pack(input, cleanTmp, tgz,
                               pseudoCode, roll, unifyActions)
               : packager.unpack(input));

    if (!ok) {
      QStringList l(packager.errorMessages());
      for (QStringList::const_iterator it = l.begin(); it != l.end(); ++it)
        fprintf(stderr, "packager error: %s\n", (*it).latin1());
      return 1;

    }

    if (pack) {
      fprintf(stderr, "Generated package: %s\n",
              packager.outputPackage().latin1());
    }

    break;
  }

  fprintf(stderr, "Directory output: %s\n", output.latin1());

  return 0;
}
