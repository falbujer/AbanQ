/***************************************************************************
 AQStyle.cpp
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

#include "AQStyle.h"

static QStringList *debugoutput = 0;

#ifdef _WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif
extern "C"
{
  char *STDCALL  AStyleMain(const char *textIn,
                            const char *options,
                            void(STDCALL *errorHandler)(int, char *),
                            char * (STDCALL *memoryAlloc)(unsigned long));
}

static void  STDCALL aqAstyleErrorHandler(int errorNumber, char *errorMessage)
{
  QString msg(QString::fromLatin1("AStyle error ") +
              QString::number(errorNumber) +
              QString::fromLatin1("\n") +
              QString::fromLatin1(errorMessage));
  fprintf(stderr, "%s\n", msg.latin1());
  if (debugoutput)
    debugoutput->append(msg);
}

static char *STDCALL aqAstyleMemoryAlloc(unsigned long memoryNeeded)
{
  char *buffer = new(std::nothrow) char [memoryNeeded];
  return buffer;
}

AQStyle::AQStyle(const QString &defaultAsOps)
{
  if (defaultAsOps.isEmpty()) {
    QString asOps;
    QChar sep('\n');
    asOps = "mode=java";
    asOps += sep;
    asOps += "style=k&r";
    asOps += sep;
    asOps += "indent=spaces=2";
    asOps += sep;
    asOps += "brackets=linux";
    asOps += sep;
    asOps += "convert-tabs";
    asOps += sep;
    asOps += "indent-namespaces";
    asOps += sep;
    asOps += "indent-switches";
    asOps += sep;
    asOps += "indent-preprocessor";
    asOps += sep;
    asOps += "unpad-paren";
    asOps += sep;
    asOps += "pad-oper";
    asOps += sep;
    asOps += "pad-header";
    asOps += sep;
    asOps += "align-pointer=name";
    asOps += sep;
    asOps += "max-instatement-indent=70";
    asOps += sep;
    asOps += "min-conditional-indent=0";
    asOps += sep;
    asOps += "indent-col1-comments";
    asOps += sep;
    defaultAsOps_ = asOps;
  } else
    defaultAsOps_ = defaultAsOps;
}

QString AQStyle::beautify(const QString &code, const QString &asOps)
{
  errMsgs_.clear();

  if (code.isEmpty())
    return QString::null;

  debugoutput = &errMsgs_;
  char *asOut = AStyleMain(code,
                           asOps.isEmpty()
                           ? defaultAsOps_
                           : asOps,
                           aqAstyleErrorHandler,
                           aqAstyleMemoryAlloc);
  debugoutput = 0;

  if (!asOut)
    return code;

  QString retCode(asOut);
  delete [] asOut;
  return retCode;
}

QString AQStyle::defaultAsOps() const
{
  return defaultAsOps_;
}

QStringList AQStyle::errorMessages() const
{
  return errMsgs_;
}
