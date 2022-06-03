/***************************************************************************
                            posprinter.cpp
                         -------------------
begin                : Vie Mar 10 2006
copyright            : (C) 2006 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
   Este  programa es software libre. Puede redistribuirlo y/o modificarlo
   bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
   versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "posprinter.h"
#include "FLDiskCache.h"

AQ_EXPORT QString FLPosPrinter::printerName_;
AQ_EXPORT QString FLPosPrinter::server_;
AQ_EXPORT QString FLPosPrinter::queueName_;
#if defined(Q_OS_WIN32)
AQ_EXPORT bool FLPosPrinter::useLPT_ = false;
AQ_EXPORT QString FLPosPrinter::nameLPT_ = "LPT1";
#endif

static const int paperWidths[FLPosPrinter::NPaperWidth + 1] = { 57, 69, 76 };

FLPosPrinter::FLPosPrinter() :
  QPaintDevice(QInternal::Printer | QInternal::ExternalDevice),
  paperWidth_(P76MM), strBuffer(0), escBuffer(0), idxBuffer(0)
{
}

FLPosPrinter::~FLPosPrinter()
{
  cleanup();
}

void FLPosPrinter::setPaperWidth(PaperWidth newPaperWidth)
{
  paperWidth_ = newPaperWidth;
}

void FLPosPrinter::cleanup()
{
  if (strBuffer) {
    strBuffer->clear();
    delete strBuffer;
    strBuffer = 0;
  }

  if (escBuffer) {
    escBuffer->clear();
    delete escBuffer;
    escBuffer = 0;
  }

  idxBuffer = 0;
}

void FLPosPrinter::sendStr(const char c, const int col, const int row)
{
  if (!strBuffer)
    initStrBuffer();

  int idx = row * paperWidthToCols() + col;

  if (col < 0) {
    (*strBuffer)[idxBuffer] = c;
    idxBuffer++;
  } else {
    (*strBuffer)[idx] = c;
    idxBuffer = idx + 1;
  }
}

void FLPosPrinter::sendEsc(const QString &e, const int col, const int row)
{
  if (!escBuffer)
    initEscBuffer();

  int idx = row * paperWidthToCols() + col;

  int i = 4;
  QString hex, resHex;
  while (i < e.length()) {
    hex = e[i++];
    hex += e[i++];
    i++;
    resHex += QChar(hex.toInt(0, 16));
  }
  if (col < 0) {
    (*escBuffer)[idxBuffer] = resHex;
    idxBuffer++;
  } else {
    (*escBuffer)[idx] = resHex;
    idxBuffer = idx + 1;
  }
}

void FLPosPrinter::send(const QString &str, const int col, const int row)
{
  if (str.left(4) == "ESC:") {
    sendEsc(str, col, row);
  } else {
    if (col >= 0) {
      for (int i = 0; i < str.length(); i++)
        sendStr(str.ascii()[i], col + i, row);
    } else {
      for (int i = 0; i < str.length(); i++)
        sendStr(str.ascii()[i]);
    }
  }
}

bool FLPosPrinter::cmd(int c, QPainter *paint, QPDevCmdParam *p)
{
  if (c == PdcBegin) {
    cleanup();
    initStrBuffer();
    initEscBuffer();
    sendEsc("ESC:1B,40,1B,52,07,1B,74,10");
  }

  if (c == PdcEnd) {
    sendEsc("ESC:1B,64,07");
    flush();
  }

  switch (c) {

    case PdcDrawTextItem: {
      const QTextItem *ti = p[1].textItem;
      const QPoint point = *p[0].point;
      const QString str = ti->engine->string;
      int col = point.x() / 3;
      int row = point.y() / 10;

      send(str, col, row);

      return false;
    }

    case PdcDrawText2Formatted:
      return true;

    case PdcDrawText2:
      return true;

    default:
      break;
  }

  return true;
}

int FLPosPrinter::metric(int m) const
{
  int val;
  PaperWidth w = paperWidth();

  switch (m) {
    case QPaintDeviceMetrics::PdmWidth:
    case QPaintDeviceMetrics::PdmWidthMM:
      val = paperWidths[w];
      break;

    case QPaintDeviceMetrics::PdmHeight:
    case QPaintDeviceMetrics::PdmHeightMM:
      val = 9999;
      break;

    case QPaintDeviceMetrics::PdmDpiX:
      val = 72;
      break;

    case QPaintDeviceMetrics::PdmDpiY:
      val = 72;
      break;

    case QPaintDeviceMetrics::PdmPhysicalDpiX:
    case QPaintDeviceMetrics::PdmPhysicalDpiY:
      val = 72;
      break;

    case QPaintDeviceMetrics::PdmNumColors:
      val = 4;
      break;

    case QPaintDeviceMetrics::PdmDepth:
      val = 2;
      break;

    default:
      val = 0;
  }

  return val;
}

int FLPosPrinter::paperWidthToCols()
{
  switch (paperWidth_) {
    case P76MM:
      return 80;

    case P69_5MM:
      return 65;

    case P57_5MM:
      return 55;
  }
}

void FLPosPrinter::initFile()
{
  fileName_ = AQ_DISKCACHE_DIRPATH + "/outposprinter_" +
              QDateTime::currentDateTime().toString("ddMMyyyyhhmmsszzz") +
              ".tmp";
}

void FLPosPrinter::initStrBuffer()
{
  if (!strBuffer)
    strBuffer = new QMap< int, char >();
  else
    strBuffer->clear();
}

void FLPosPrinter::initEscBuffer()
{
  if (!escBuffer)
    escBuffer = new QMap< int, QString >();
  else
    escBuffer->clear();
}

void FLPosPrinter::parsePrinterName()
{
  int posdots = printerName_.find(":");
  server_ = printerName_.left(posdots);
  queueName_ = printerName_.right(printerName_.length() - posdots - 1);
}
