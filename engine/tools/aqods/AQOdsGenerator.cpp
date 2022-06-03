/***************************************************************************
 AQOdsGenerator.cpp
 -------------------
 begin                : 16/12/2011
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
 bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
 versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "AQOdsGenerator.h"
#include "AQS_p.h"
#include "AQZipWriter.h"
#include "FLDiskCache.h"

bool isXmlCharODS(const QChar &c)
{
  // Characters in this range must be accepted by XML parsers.
  // Consequently characters outside of this range need to be escaped.

  ushort uc = c.unicode();

  return uc == 0x9
         || uc == 0xA
         || uc == 0xD
         || 0x20 <= uc && uc <= 0xD7FF
         || 0xE000 <= uc && uc <= 0xFFFD;
}

// ### AbanQ
// to do compliant with the standar XML 1.0
// 2.11 End-of-Line Handling and 3.3.3 Attribute-Value Normalization
// see below encodeAttrODS
bool isEndOfLineCharODS(const QChar &c)
{
  ushort uc = c.unicode();

  return uc == 0x9
         || uc == 0xA
         || uc == 0xD;
}
// ### AbanQ

QString encodeAttrODS(const QString &str)
{
  QString tmp(str);
  uint len = tmp.length();
  uint i = 0;
  while (i < len) {
    if (tmp[(int)i] == '<') {
      tmp.replace(i, 1, "&lt;");
      len += 3;
      i += 4;
    } else if (tmp[(int)i] == '"') {
      tmp.replace(i, 1, "&quot;");
      len += 5;
      i += 6;
    } else if (tmp[(int)i] == '&') {
      tmp.replace(i, 1, "&amp;");
      len += 4;
      i += 5;
    } else if (tmp[(int)i] == '>' && i >= 2 && tmp[(int)i - 1] == ']' && tmp[(int)i - 2] == ']') {
      tmp.replace(i, 1, "&gt;");
      len += 3;
      i += 4;
    }
    // ### AbanQ
    // to do compliant with the standar XML 1.0
    // 2.11 End-of-Line Handling and 3.3.3 Attribute-Value Normalization
    else if (isEndOfLineCharODS(tmp[(int)i])) {
      QString repl = "&#x" + QString::number(tmp[(int)i].unicode(), 16) + ';';
      tmp.replace(i, 1, repl);
      len += repl.length() - 1;
      i += repl.length();
    }
    // ### AbanQ
    else if (!isXmlCharODS(tmp[(int)i])) {
      QString repl = "&#x" + QString::number(tmp[(int)i].unicode(), 16) + ';';
      qWarning("AQOdsRow: not saving invalid character %s, the document will not be well-formed", repl.latin1());
      repl = "?";
      tmp.replace(i, 1, repl);
      len += repl.length() - 1;
      i += repl.length();
    } else {
      ++i;
    }
  }

  return tmp;
}

// xslt/ods-main-content.xslt
static char data_main[] = {
  54, 68, 67, 48, 75, 50, 116, 74, 119, 69, 88, 74, 51, 106, 74, 72, 79, 51, 100, 90,
  111, 71, 81, 102, 49, 66, 97, 79, 108, 78, 88, 116, 55, 51, 49, 49, 108, 102, 53, 77,
  106, 80, 51, 52, 49, 74, 55, 90, 110, 108, 103, 78, 80, 74, 116, 90, 84, 108, 55, 89,
  83, 105, 50, 74, 76, 50, 116, 67, 80, 85, 65, 49, 48, 102, 108, 103, 108, 43, 101, 65,
  53, 71, 70, 65, 119, 101, 71, 74, 73, 120, 89, 101, 99, 120, 87, 119, 112, 76, 54, 66,
  76, 107, 119, 102, 109, 51, 105, 97, 101, 122, 102, 116, 90, 108, 83, 74, 73, 117, 49, 110,
  68, 108, 53, 65, 49, 53, 51, 57, 54, 81, 109, 68, 56, 97, 89, 51, 79, 105, 57, 84,
  114, 118, 120, 76, 90, 56, 57, 71, 81, 50, 65, 115, 74, 101, 66, 68, 66, 103, 104, 109,
  76, 67, 48, 67, 47, 106, 119, 82, 85, 114, 117, 73, 53, 85, 115, 114, 102, 50, 118, 103,
  103, 81, 80, 121, 117, 80, 75, 97, 47, 106, 72, 80, 122, 71, 103, 119, 47, 74, 85, 103,
  79, 87, 79, 119, 106, 67, 107, 97, 86, 88, 68, 111, 104, 79, 81, 88, 72, 74, 47, 108,
  76, 103, 56, 49, 106, 101, 55, 56, 84, 108, 56, 49, 76, 118, 48, 81, 101, 85, 69, 103,
  119, 52, 77, 77, 74, 74, 100, 72, 84, 51, 104, 68, 81, 97, 74, 102, 51, 99, 98, 55,
  106, 72, 65, 52, 121, 47, 71, 117, 82, 70, 49, 105, 43, 90, 49, 85, 57, 105, 76, 102,
  109, 52, 82, 55, 57, 69, 73, 49, 102, 83, 83, 47, 88, 51, 109, 117, 104, 69, 66, 52,
  43, 78, 49, 75, 104, 85, 106, 85, 52, 49, 69, 79, 77, 85, 73, 117, 77, 100, 100, 70,
  90, 76, 56, 86, 52, 75, 76, 47, 81, 112, 118, 108, 119, 105, 122, 79, 75, 66, 121, 101,
  87, 82, 76, 65, 69, 106, 106, 67, 56, 118, 120, 112, 121, 56, 97, 71, 120, 75, 67, 100,
  68, 69, 52, 122, 48, 52, 76, 114, 72, 47, 75, 113, 99, 118, 97, 80, 52, 100, 86, 79,
  109, 100, 121, 83, 99, 105, 97, 48, 73, 70, 53, 110, 75, 110, 98, 49, 50, 97, 113, 108,
  81, 75, 86, 52, 104, 76, 57, 43, 107, 76, 88, 72, 57, 54, 66, 84, 85, 48, 47, 97,
  103, 66, 118, 120, 111, 97, 70, 66, 109, 51, 52, 48, 47, 55, 74, 84, 105, 72, 88, 76,
  81, 79, 107, 84, 81, 111, 97, 102, 87, 70, 102, 85, 106, 49, 51, 77, 74, 86, 87, 102,
  105, 77, 88, 108, 47, 119, 99, 78, 72, 68, 98, 65, 66, 49, 101, 104, 113, 83, 56, 99,
  97, 87, 106, 85, 77, 106, 74, 98, 48, 70, 57, 74, 69, 68, 53, 72, 105, 117, 107, 70,
  85, 109, 114, 104, 107, 69, 83, 77, 113, 88, 57, 106, 85, 51, 104, 104, 87, 115, 81, 88,
  74, 121, 107, 110, 43, 116, 121, 72, 113, 86, 118, 70, 78, 103, 119, 98, 101, 113, 70, 72,
  113, 101, 83, 53, 112, 113, 90, 78, 54, 66, 122, 72, 84, 104, 83, 118, 50, 86, 50, 111,
  106, 69, 88, 53, 97, 79, 69, 79, 68, 75, 50, 55, 51, 76, 74, 47, 112, 50, 113, 77,
  69, 72, 100, 76, 67, 74, 52, 108, 49, 115, 76, 114, 52, 68, 77, 116, 111, 112, 53, 69,
  99, 118, 101, 122, 50, 87, 118, 78, 121, 106, 90, 90, 50, 112, 120, 82, 74, 54, 48, 85,
  48, 122, 66, 111, 52, 72, 110, 84, 50, 72, 120, 101, 119, 74, 73, 98, 90, 54, 121, 114,
  103, 87, 67, 54, 115, 105, 88, 80, 108, 81, 110, 73, 47, 68, 65, 116, 65, 88, 51, 50,
  109, 118, 118, 69, 115, 120, 48, 87, 57, 55, 99, 82, 77, 83, 122, 121, 67, 53, 72, 73,
  65, 115, 121, 57, 68, 78, 98, 101, 81, 111, 90, 109, 71, 52, 104, 100, 52, 78, 110, 52,
  83, 81, 51, 49, 114, 112, 55, 75, 43, 43, 111, 81, 107, 74, 117, 89, 106, 80, 55, 118,
  71, 104, 72, 105, 89, 74, 67, 99, 77, 71, 73, 68, 52, 101, 84, 113, 99, 49, 72, 74,
  76, 43, 48, 114, 105, 75, 115, 97, 53, 105, 84, 78, 109, 104, 47, 102, 47, 79, 109, 83,
  98, 78, 75, 56, 114, 52, 115, 105, 70, 121, 69, 84, 57, 105, 121, 122, 118, 83, 67, 53,
  101, 83, 110, 104, 114, 119, 87, 88, 78, 100, 52, 54, 109, 118, 85, 108, 77, 82, 89, 97,
  86, 65, 103, 70, 67, 72, 69, 85, 102, 116, 56, 78, 43, 57, 70, 101, 53, 43, 121, 68,
  77, 110, 52, 49, 49, 85, 77, 50, 54, 80, 48, 84, 120, 77, 89, 57, 113, 43, 47, 77,
  66, 120, 67, 82, 66, 110, 115, 73, 68, 114, 68, 108, 101, 114, 103, 122, 48, 53, 78, 43,
  107, 116, 49, 77, 103, 54, 104, 121, 69, 56, 78, 101, 86, 84, 74, 49, 101, 100, 119, 120,
  73, 79, 53, 108, 99, 54, 53, 88, 67, 108, 48, 51, 100, 88, 72, 100, 114, 47, 53, 108,
  68, 79, 57, 101, 72, 81, 89, 121, 113, 99, 103, 75, 115, 105, 72, 54, 122, 119, 99, 77,
  98, 115, 104, 53, 82, 106, 122, 116, 55, 48, 100, 108, 88, 106, 83, 110, 116, 56, 111, 75,
  65, 50, 121, 87, 43, 49, 107, 69, 69, 70, 56, 112, 66, 81, 100, 73, 81, 99, 102, 57,
  87, 111, 54, 85, 87, 119, 102, 118, 83, 113, 103, 66, 90, 102, 66, 47, 69, 99, 73, 102,
  118, 101, 53, 72, 78, 78, 119, 118, 97, 79, 107, 100, 51, 88, 122, 76, 109, 81, 90, 90,
  97, 82, 102, 87, 76, 78, 43, 99, 119, 102, 109, 47, 80, 107, 111, 68, 109, 118, 121, 65,
  121, 43, 120, 111, 76, 90, 98, 80, 87, 54, 75, 88, 80, 68, 118, 98, 77, 65, 119, 111,
  68, 77, 69, 121, 75, 114, 100, 83, 116, 97, 47, 113, 56, 53, 69, 51, 102, 90, 75, 79,
  121, 43, 49, 50, 118, 115, 117, 69, 65, 116, 73, 112, 84, 121, 113, 67, 78, 69, 78, 85,
  113, 108, 107, 86, 108, 119, 85, 56, 81, 66, 83, 122, 112, 48, 52, 111, 70, 75, 90, 82,
  106, 49, 106, 66, 99, 83, 57, 115, 112, 47, 74, 117, 80, 80, 99, 69, 86, 80, 71, 88,
  84, 49, 90, 119, 112, 101, 119, 87, 76, 116, 109, 90, 47, 82, 110, 89, 104, 53, 115, 86,
  113, 82, 49, 77, 121, 100, 76, 49, 50, 67, 65, 83, 71, 122, 109, 98, 115, 87, 65, 109,
  43, 43, 122, 115, 77, 48, 53, 104, 112, 88, 77, 76, 113, 87, 80, 80, 112, 47, 49, 101,
  65, 117, 53, 115, 86, 122, 116, 114, 90, 74, 105, 50, 55, 67, 72, 57, 103, 55, 101, 49,
  118, 51, 109, 97, 83, 74, 101, 55, 67, 107, 104, 106, 69, 117, 87, 75, 56, 83, 110, 100,
  108, 66, 55, 90, 103, 113, 48, 75, 88, 43, 83, 85, 116, 81, 77, 102, 76, 52, 77, 90,
  102, 97, 104, 102, 54, 68, 89, 79, 98, 97, 86, 52, 112, 101, 66, 80, 108, 71, 104, 51,
  81, 104, 112, 89, 75, 72, 77, 105, 83, 71, 78, 89, 88, 116, 54, 121, 75, 89, 110, 82,
  103, 56, 118, 87, 84, 114, 53, 79, 54, 112, 55, 87, 120, 87, 81, 47, 106, 87, 106, 116,
  111, 119, 75, 51, 85, 57, 82, 74, 48, 67, 80, 49, 89, 97, 120, 85, 106, 113, 78, 74,
  55, 49, 90, 66, 51, 73, 122, 75, 113, 116, 118, 52, 118, 88, 89, 121, 108, 52, 68, 121,
  73, 115, 111, 47, 119, 86, 100, 73, 118, 121, 86, 113, 118, 66, 75, 56, 121, 118, 48, 102,
  109, 86, 57, 70, 114, 52, 50, 120, 74, 53, 74, 49, 102, 54, 76, 118, 83, 75, 56, 53,
  111, 104, 106, 78, 50, 90, 52, 78, 57, 90, 105, 47, 47, 65, 83, 53, 109, 106, 99, 112,
  48, 87, 72, 100, 72, 47, 102, 105, 50, 66, 83, 87, 51, 104, 56, 65, 106, 68, 66, 118,
  102, 118, 66, 70, 49, 85, 52, 115, 56, 102, 56, 67, 89, 73, 48, 57, 67, 73, 112, 115,
  51, 118, 70, 103, 112, 97, 84, 71, 57, 103, 117, 88, 43, 53, 100, 67, 100, 101, 107, 57,
  86, 69, 116, 87, 89, 86, 49, 57, 114, 53, 54, 120, 120, 72, 72, 102, 85, 74, 76, 57,
  73, 107, 55, 68, 69, 121, 100, 100, 55, 120, 111, 116, 110, 88, 65, 100, 106, 106, 72, 75,
  90, 48, 75, 88, 102, 119, 101, 118, 72, 47, 108, 57, 120, 113, 87, 102, 78, 71, 56, 51,
  116, 66, 114, 106, 55, 80, 47, 77, 83, 79, 57, 71, 56, 77, 84, 51, 88, 104, 70, 110,
  53, 70, 120, 107, 78, 82, 121, 50, 76, 51, 53, 76, 79, 100, 116, 83, 54, 122, 119, 43,
  73, 100, 68, 66, 108, 119, 69, 114, 53, 90, 56, 98, 99, 88, 52, 83, 112, 116, 109, 84,
  57, 73, 97, 49, 85, 107, 110, 69, 55, 102, 71, 119, 85, 109, 80, 53, 109, 47, 57, 55,
  76, 109, 47, 120, 112, 71, 108, 113, 68, 112, 118, 78, 101, 76, 102, 68, 89, 54, 51, 109,
  43, 120, 84, 86, 101, 73, 71, 109, 68, 65, 84, 100, 47, 114, 106, 118, 87, 52, 84, 48,
  110, 120, 55, 68, 104, 110, 74, 88, 110, 117, 102, 81, 49, 111, 74, 82, 90, 118, 57, 115,
  67, 43, 89, 66, 105, 56, 108, 110, 110, 113, 66, 80, 74, 70, 50, 43, 72, 106, 71, 50,
  98, 114, 75, 71, 122, 84, 79, 43, 101, 121, 88, 76, 57, 54, 109, 114, 56, 86, 56, 84,
  107, 82, 48, 69, 115, 90, 107, 88, 101, 81, 98, 76, 107, 106, 43, 121, 103, 51, 82, 51,
  83, 86, 75, 51, 102, 77, 85, 84, 90, 56, 51, 87, 51, 48, 108, 97, 101, 68, 112, 78,
  117, 82, 121, 111, 73, 67, 99, 109, 104, 109, 78, 54, 105, 111, 54, 86, 55, 104, 49, 118,
  112, 80, 72, 118, 70, 57, 49, 98, 122, 116, 72, 71, 50, 69, 51, 48, 71, 101, 84, 106,
  115, 68, 81, 85, 51, 69, 99, 122, 84, 113, 81, 86, 115, 112, 80, 107, 103, 99, 70, 53,
  118, 122, 48, 106, 104, 69, 43, 69, 70, 69, 119, 80, 100, 99, 78, 78, 108, 50, 89, 78,
  66, 79, 53, 66, 50, 86, 90, 103, 79, 86, 83, 65, 90, 72, 111, 66, 57, 68, 72, 80,
  116, 88, 85, 89, 108, 55, 69, 79, 69, 115, 71, 87, 56, 119, 100, 55, 47, 117, 83, 71,
  70, 103, 105, 74, 74, 74, 109, 85, 83, 52, 79, 51, 107, 112, 82, 115, 100, 119, 75, 76,
  43, 101, 102, 109, 48, 65, 84, 103, 84, 85, 82, 102, 76, 98, 114, 77, 82, 99, 85, 118,
  54, 76, 101, 71, 56, 87, 98, 111, 52, 83, 105, 90, 54, 48, 88, 86, 117, 70, 97, 83,
  105, 51, 110, 122, 108, 68, 107, 90, 86, 108, 81, 68, 70, 65, 101, 65, 74, 51, 103, 105,
  117, 101, 101, 55, 98, 116, 70, 120, 75, 49, 49, 76, 72, 47, 52, 112, 87, 112, 117, 111,
  51, 56, 69, 54, 51, 100, 69, 104, 77, 43, 121, 76, 65, 48, 112, 72, 65, 55, 49, 122,
  84, 66, 108, 81, 78, 48, 118, 88, 71, 116, 86, 51, 122, 69, 90, 70, 112, 80, 89, 106,
  67, 106, 51, 57, 85, 66, 104, 122, 57, 114, 85, 77, 75, 54, 85, 103, 53, 86, 75, 79,
  87, 109, 56, 73, 84, 112, 122, 107, 69, 100, 108, 105, 68, 53, 85, 71, 122, 112, 53, 70,
  121, 80, 109, 50, 114, 98, 65, 107, 112, 85, 53, 122, 73, 80, 119, 49, 56, 105, 74, 90,
  65, 47, 107, 115, 110, 54, 113, 75, 90, 66, 74, 108, 56, 98, 107, 98, 115, 65, 97, 119,
  75, 43, 54, 53, 105, 84, 107, 86, 119, 103, 86, 84, 80, 83, 89, 98, 122, 108, 85, 87,
  85, 57, 78, 118, 47, 79, 116, 49, 86, 111, 108, 101, 84, 51, 88, 56, 97, 57, 120, 120,
  76, 119, 120, 65, 78, 105, 120, 57, 65, 99, 107, 50, 97, 89, 55, 72, 113, 53, 121, 104,
  98, 68, 56, 101, 71, 85, 65, 71, 66, 67, 120, 103, 67, 103, 102, 74, 52, 73, 79, 99,
  106, 72, 97, 107, 67, 57, 101, 68, 85, 103, 75, 76, 89, 105, 43, 67, 104, 103, 55, 116,
  97, 47, 73, 86, 109, 71, 43, 53, 73, 89, 112, 118, 109, 114, 90, 115, 97, 71, 47, 82,
  113, 100, 116, 69, 76, 67, 106, 106, 73, 107, 90, 47, 67, 100, 86, 53, 66, 49, 100, 109,
  69, 84, 50, 49, 100, 115, 43, 97, 120, 112, 66, 43, 69, 78, 112, 87, 99, 68, 118, 50,
  121, 77, 78, 57, 98, 117, 43, 118, 105, 106, 97, 77, 117, 53, 120, 66, 78, 56, 106, 76,
  118, 111, 74, 117, 52, 71, 48, 53, 68, 88, 87, 80, 104, 86, 86, 76, 80, 108, 115, 119,
  116, 54, 43, 107, 114, 117, 67, 97, 50, 108, 90, 70, 52, 51, 56, 56, 74, 100, 98, 89,
  101, 72, 65, 82, 109, 52, 109, 52, 100, 97, 43, 89, 85, 65, 88, 109, 74, 56, 53, 118,
  57, 108, 99, 118, 56, 116, 90, 49, 119, 87, 80, 88, 66, 43, 116, 104, 110, 87, 48, 120,
  88, 109, 51, 74, 100, 111, 112, 106, 78, 72, 106, 85, 109, 54, 119, 55, 97, 105, 72, 54,
  108, 106, 118, 90, 74, 120, 85, 122, 86, 109, 74, 110, 74, 110, 50, 122, 47, 90, 51, 77,
  102, 119, 75, 114, 80, 116, 118, 71, 82, 48, 114, 72, 106, 70, 57, 85, 87, 86, 99, 50,
  115, 81, 43, 83, 73, 48, 53, 121, 113, 118, 90, 121, 106, 75, 70, 99, 65, 98, 84, 68,
  80, 117, 49, 89, 79, 102, 55, 73, 77, 120, 49, 82, 108, 56, 108, 111, 113, 107, 52, 65,
  50, 90, 117, 103, 117, 68, 86, 75, 78, 122, 89, 104, 103, 110, 118, 97, 100, 85, 57, 71,
  79, 84, 97, 115, 53, 105, 106, 99, 113, 114, 55, 106, 87, 57, 51, 116, 51, 73, 73, 66,
  119, 88, 101, 79, 85, 43, 89, 49, 56, 81, 75, 101, 120, 78, 114, 87, 69, 119, 88, 56,
  48, 51, 75, 102, 113, 113, 47, 87, 110, 84, 122, 57, 69, 70, 103, 48, 101, 75, 47, 104,
  75, 118, 77, 115, 84, 82, 86, 116, 69, 79, 65, 73, 76, 83, 71, 79, 65, 103, 77, 67,
  113, 53, 114, 80, 53, 65, 98, 120, 67, 50, 82, 99, 80, 87, 79, 57, 89, 65, 103, 81,
  57, 113, 107, 43, 75, 51, 49, 78, 89, 83, 69, 121, 76, 84, 51, 70, 83, 65, 82, 97,
  90, 81, 76, 116, 100, 66, 106, 102, 75, 89, 101, 50, 108, 117, 51, 117, 73, 71, 115, 43,
  89, 55, 107, 112, 75, 84, 109, 50, 111, 115, 49, 85, 78, 75, 87, 113, 72, 69, 56, 50,
  53, 82, 65, 67, 54, 50, 67, 68, 78, 88, 110, 121, 48, 98, 52, 84, 89, 52, 118, 104,
  48, 54, 81, 100, 120, 77, 88, 113, 115, 51, 89, 102, 102, 89, 89, 80, 116, 54, 49, 67,
  81, 109, 57, 55, 103, 80, 113, 106, 78, 116, 101, 99, 85, 107, 109, 103, 120, 50, 49, 108,
  110, 78, 119, 55, 90, 49, 110, 81, 66, 55, 88, 55, 87, 68, 77, 117, 110, 78, 106, 121,
  70, 80, 48, 53, 116, 112, 82, 47, 52, 110, 50, 79, 69, 102, 69, 70, 119, 105, 106, 84,
  112, 122, 69, 69, 108, 71, 74, 65, 67, 65, 48, 61
};

// xslt/ods-extract-charts.xslt
static char data_chart[] = {
  111, 82, 87, 84, 77, 104, 85, 57, 52, 80, 111, 114, 116, 49, 52, 111, 86, 116, 54, 85,
  43, 52, 110, 117, 66, 102, 80, 56, 77, 111, 81, 74, 108, 119, 49, 80, 102, 110, 118, 89,
  88, 100, 122, 104, 66, 51, 79, 55, 117, 75, 116, 99, 75, 49, 109, 70, 57, 111, 75, 97,
  51, 110, 86, 57, 105, 101, 75, 55, 118, 70, 118, 110, 74, 90, 65, 112, 76, 55, 75, 82,
  98, 47, 105, 82, 111, 105, 87, 51, 48, 86, 52, 97, 97, 114, 80, 71, 73, 89, 110, 109,
  65, 48, 117, 69, 56, 112, 43, 83, 118, 47, 49, 50, 80, 43, 81, 115, 54, 66, 83, 115,
  84, 89, 74, 47, 66, 53, 104, 51, 76, 101, 83, 85, 105, 81, 98, 111, 53, 54, 78, 122,
  101, 82, 53, 69, 118, 66, 108, 99, 49, 114, 122, 65, 80, 117, 118, 56, 76, 88, 52, 101,
  104, 114, 84, 78, 57, 76, 115, 48, 88, 65, 55, 99, 43, 65, 85, 88, 78, 97, 85, 66,
  48, 54, 86, 56, 83, 57, 104, 111, 107, 76, 51, 43, 57, 70, 87, 51, 68, 83, 88, 122,
  90, 118, 73, 71, 43, 77, 68, 51, 97, 85, 98, 82, 56, 54, 52, 100, 120, 75, 80, 121,
  98, 68, 98, 50, 105, 82, 48, 86, 113, 47, 51, 119, 65, 88, 51, 68, 104, 114, 84, 87,
  84, 120, 98, 54, 82, 83, 121, 110, 70, 74, 83, 51, 48, 103, 52, 90, 52, 109, 81, 56,
  56, 100, 110, 104, 117, 74, 99, 120, 90, 51, 70, 79, 54, 111, 68, 65, 122, 54, 115, 103,
  74, 68, 69, 97, 115, 90, 122, 43, 81, 122, 53, 65, 111, 78, 74, 85, 116, 115, 99, 83,
  116, 76, 112, 119, 88, 114, 67, 99, 100, 82, 79, 100, 65, 51, 80, 51, 85, 73, 102, 56,
  50, 110, 53, 51, 82, 48, 54, 102, 118, 69, 101, 79, 57, 72, 118, 117, 103, 111, 78, 76,
  120, 50, 78, 48, 54, 71, 43, 108, 119, 111, 113, 98, 43, 67, 122, 69, 50, 54, 51, 53,
  88, 113, 86, 53, 71, 97, 84, 121, 103, 117, 90, 97, 65, 70, 84, 85, 115, 122, 47, 74,
  103, 56, 87, 109, 51, 52, 102, 52, 106, 121, 48, 43, 113, 51, 100, 101, 113, 78, 69, 99,
  65, 48, 77, 117, 107, 88, 87, 113, 67, 83, 90, 71, 84, 75, 102, 84, 84, 71, 102, 82,
  79, 53, 67, 84, 107, 73, 109, 103, 108, 85, 57, 121, 81, 51, 68, 109, 75, 84, 75, 82,
  65, 79, 107, 118, 57, 102, 116, 86, 106, 50, 75, 76, 82, 90, 119, 43, 99, 115, 85, 113,
  50, 113, 122, 57, 75, 105, 76, 79, 65, 98, 106, 103, 87, 79, 87, 76, 50, 117, 71, 71,
  107, 120, 116, 103, 121, 68, 87, 54, 54, 110, 100, 120, 112, 43, 115, 117, 90, 115, 87, 51,
  54, 76, 67, 67, 90, 117, 43, 69, 110, 97, 85, 100, 67, 86, 53, 55, 85, 65, 85, 86,
  111, 43, 119, 101, 43, 65, 79, 52, 88, 97, 106, 99, 120, 102, 98, 87, 89, 74, 118, 54,
  106, 52, 102, 85, 116, 48, 116, 67, 97, 82, 57, 98, 56, 110, 81, 47, 118, 54, 109, 70,
  115, 81, 48, 81, 97, 100, 111, 97, 68, 49, 74, 57, 87, 88, 77, 105, 97, 89, 69, 57,
  48, 121, 108, 71, 97, 47, 83, 109, 116, 73, 101, 110, 111, 86, 66, 102, 116, 111, 49, 85,
  52, 90, 81, 101, 78, 112, 75, 87, 51, 43, 50, 66, 71, 121, 84, 49, 114, 89, 74, 49,
  89, 47, 114, 84, 89, 52, 52, 105, 67, 107, 81, 70, 65, 65, 102, 75, 76, 55, 111, 67,
  118, 71, 113, 70, 122, 81, 103, 54, 83, 88, 118, 111, 53, 55, 110, 120, 121, 84, 53, 78,
  116, 65, 113, 104, 90, 109, 55, 86, 51, 90, 112, 65, 57, 122, 76, 122, 113, 52, 66, 110,
  108, 71, 84, 119, 112, 43, 99, 72, 47, 105, 113, 121, 103, 70, 66, 48, 68, 108, 57, 97,
  119, 55, 102, 71, 111, 68, 43, 75, 48, 119, 75, 109, 84, 116, 48, 113, 56, 99, 66, 52,
  114, 48, 83, 51, 114, 49, 100, 73, 51, 78, 50, 86, 106, 66, 54, 84, 56, 121, 49, 52,
  84, 120, 57, 103, 101, 109, 71, 78, 99, 85, 98, 116, 85, 98, 49, 79, 48, 117, 54, 122,
  122, 56, 66, 78, 116, 51, 72, 113, 103, 81, 101, 113, 81, 78, 67, 54, 80, 113, 51, 109,
  120, 101, 100, 84, 112, 81, 103, 101, 108, 66, 90, 79, 87, 67, 101, 115, 54, 88, 120, 104,
  103, 97, 111, 107, 74, 83, 108, 101, 67, 113, 116, 108, 108, 117, 111, 52, 55, 103, 57, 109,
  65, 98, 100, 103, 97, 69, 111, 121, 121, 106, 122, 67, 101, 98, 55, 119, 118, 105, 97, 52,
  109, 53, 119, 51, 90, 88, 78, 43, 110, 99, 104, 55, 70, 105, 115, 57, 115, 116, 113, 77,
  84, 99, 75, 47, 71, 115, 54, 55, 106, 119, 121, 108, 84, 78, 48, 68, 88, 57, 48, 119,
  74, 48, 70, 98, 84, 102, 83, 71, 116, 48, 104, 69, 109, 74, 97, 65, 72, 66, 55, 115,
  49, 118, 69, 115, 116, 80, 117, 49, 118, 67, 54, 112, 50, 121, 120, 69, 116, 65, 107, 117,
  73, 121, 122, 112, 70, 75, 73, 88, 51, 74, 85, 70, 119, 85, 81, 73, 66, 54, 49, 81,
  53, 54, 88, 116, 104, 55, 85, 120, 99, 108, 108, 100, 88, 110, 76, 81, 52, 100, 105, 115,
  80, 86, 79, 121, 52, 89, 87, 87, 97, 66, 89, 90, 120, 122, 65, 82, 119, 109, 71, 81,
  57, 68, 116, 117, 54, 108, 115, 84, 52, 102, 70, 119, 89, 84, 112, 67, 50, 119, 50, 50,
  67, 55, 106, 47, 101, 79, 113, 51, 118, 88, 82, 55, 114, 71, 117, 69, 109, 115, 57, 103,
  71, 121, 106, 101, 110, 88, 48, 106, 52, 66, 73, 71, 51, 68, 70, 43, 99, 72, 78, 48,
  113, 51, 86, 56, 81, 56, 70, 54, 110, 73, 80, 115, 112, 117, 55, 75, 67, 103, 106, 85,
  105, 87, 120, 81, 89, 80, 102, 50, 118, 113, 71, 84, 98, 122, 70, 49, 118, 66, 114, 115,
  47, 68, 118, 68, 112, 53, 102, 100, 52, 77, 54, 54, 118, 120, 52, 70, 57, 51, 51, 114,
  84, 86, 111, 98, 87, 101, 116, 117, 111, 86, 72, 113, 73, 70, 69, 102, 69, 83, 113, 49,
  86, 89, 51, 48, 74, 74, 120, 85, 90, 83, 116, 79, 57, 107, 73, 75, 121, 99, 107, 98,
  56, 56, 113, 70, 104, 104, 90, 81, 100, 51, 77, 90, 73, 52, 84, 99, 98, 75, 120, 53,
  56, 108, 107, 69, 109, 120, 75, 114, 56, 90, 90, 43, 54, 57, 88, 80, 75, 97, 107, 75,
  53, 107, 56, 76, 49, 57, 120, 53, 54, 79, 102, 54, 107, 115, 65, 117, 48, 110, 83, 54,
  56, 86, 80, 115, 57, 87, 75, 99, 88, 82, 65, 55, 102, 47, 105, 51, 55, 118, 73, 50,
  109, 105, 67, 105, 121, 116, 81, 112, 65, 117, 53, 54, 43, 114, 52, 112, 83, 73, 101, 52,
  101, 100, 89, 66, 66, 71, 81, 48, 75, 53, 89, 113, 118, 108, 79, 109, 84, 85, 113, 43,
  78, 117, 87, 114, 57, 47, 50, 105, 119, 51, 52, 73, 108, 114, 88, 88, 111, 71, 55, 75,
  108, 74, 119, 47, 116, 101, 83, 81, 47, 83, 69, 72, 89, 78, 90, 56, 56, 102, 118, 52,
  115, 110, 43, 115, 77, 83, 107, 77, 49, 70, 50, 112, 53, 67, 68, 112, 48, 84, 109, 74,
  47, 51, 115, 116, 89, 49, 102, 118, 50, 51, 89, 82, 70, 85, 57, 84, 66, 122, 74, 66,
  54, 81, 112, 83, 111, 77, 109, 51, 84, 89, 89, 74, 51, 69, 114, 52, 98, 53, 84, 78,
  100, 99, 49, 102, 68, 74, 80, 78, 113, 98, 52, 107, 99, 82, 67, 98, 78, 53, 43, 90,
  110, 77, 117, 97, 102, 69, 98, 100, 78, 51, 116, 115, 85, 83, 84, 106, 108, 57, 89, 66,
  100, 79, 108, 71, 73, 118, 79, 114, 53, 122, 117, 114, 48, 73, 85, 70, 109, 107, 48, 84,
  100, 56, 114, 102, 109, 81, 113, 108, 66, 74, 86, 107, 87, 66, 86, 102, 99, 51, 121, 82,
  113, 56, 82, 101, 101, 80, 55, 117, 87, 84, 49, 86, 67, 74, 110, 54, 102, 104, 109, 113,
  67, 103, 105, 55, 116, 43, 81, 90, 66, 88, 69, 52, 119, 65, 77, 54, 106, 118, 82, 49,
  109, 68, 83, 108, 115, 88, 101, 102, 47, 110, 74, 70, 100, 51, 72, 104, 57, 103, 104, 70,
  114, 47, 74, 69, 43, 50, 112, 113, 68, 56, 115, 97, 99, 84, 116, 79, 70, 117, 66, 121,
  121, 86, 116, 78, 121, 72, 57, 77, 56, 71, 73, 65, 116, 66, 105, 112, 55, 79, 70, 77,
  73, 82, 110, 49, 55, 67, 114, 109, 84, 109, 56, 119, 53, 74, 52, 100, 86, 108, 106, 73,
  122, 118, 114, 56, 69, 79, 86, 86, 75, 97, 110, 71, 101, 107, 117, 83, 72, 108, 77, 104,
  98, 110, 99, 88, 87, 43, 118, 54, 108, 122, 66, 43, 105, 49, 74, 71, 100, 66, 66, 49,
  52, 75, 119, 122, 90, 67, 52, 53, 115, 101, 69, 84, 102, 111, 121, 78, 81, 84, 79, 100,
  115, 117, 118, 99, 111, 86, 66, 81, 81, 80, 113, 82, 50, 108, 78, 77, 112, 43, 70, 119,
  99, 89, 112, 73, 112, 52, 115, 97, 43, 69, 108, 100, 83, 80, 47, 100, 78, 80, 84, 70,
  66, 50, 106, 55, 85, 89, 117, 53, 103, 80, 86, 112, 101, 72, 112, 89, 77, 104, 117, 98,
  83, 57, 55, 117, 114, 80, 106, 88, 109, 55, 105, 115, 105, 120, 56, 101, 111, 73, 71, 119,
  87, 73, 101, 56, 54, 85, 55, 113, 66, 87, 53, 71, 84, 101, 121, 98, 51, 80, 97, 114,
  69, 114, 87, 113, 103, 107, 104, 84, 72, 120, 57, 107, 87, 84, 77, 103, 73, 55, 122, 43,
  80, 81, 67, 104, 65, 51, 122, 75, 97, 97, 102, 53, 48, 47, 82, 79, 90, 101, 83, 79,
  70, 84, 104, 69, 50, 109, 105, 77, 43, 68, 48, 90, 80, 114, 56, 102, 83, 88, 82, 69,
  118, 70, 105, 122, 101, 110, 78, 86, 87, 52, 101, 56, 75, 121, 104, 71, 50, 81, 56, 53,
  121, 85, 103, 118, 81, 107, 99, 76, 108, 75, 70, 70, 49, 81, 119, 116, 85, 80, 104, 87,
  49, 47, 101, 107, 80, 84, 119, 90, 86, 116, 88, 56, 112, 84, 48, 51, 114, 101, 73, 87,
  115, 105, 117, 48, 90, 101, 79, 110, 51, 81, 65, 108, 102, 110, 119, 118, 53, 105, 75, 81,
  113, 107, 106, 120, 97, 49, 89, 116, 88, 51, 67, 117, 100, 74, 53, 47, 117, 108, 77, 52,
  87, 88, 76, 102, 52, 113, 47, 52, 122, 85, 49, 86, 66, 67, 90, 100, 97, 52, 84, 49,
  84, 120, 110, 116, 51, 78, 73, 102, 112, 121, 57, 108, 106, 120, 111, 76, 77, 54, 84, 121,
  85, 83, 78, 104, 66, 98, 113, 116, 118, 120, 66, 50, 106, 88, 104, 74, 109, 120, 69, 111,
  49, 107, 108, 79, 55, 98, 50, 121, 103, 104, 82, 107, 78, 83, 55, 56, 89, 97, 50, 53,
  102, 111, 106, 68, 98, 110, 107, 106, 120, 118, 107, 110, 122, 88, 79, 81, 66, 109, 113, 98,
  111, 99, 84, 88, 83, 87, 85, 109, 72, 83, 110, 115, 66, 48, 105, 112, 48, 52, 82, 99,
  54, 49, 70, 110, 111, 55, 69, 89, 117, 108, 103, 120, 52, 118, 49, 53, 121, 111, 81, 56,
  43, 73, 103, 78, 111, 68, 68, 110, 89, 107, 48, 99, 84, 54, 57, 104, 116, 76, 80, 102,
  68, 122, 76, 55, 74, 87, 87, 65, 56, 115, 47, 111, 122, 85, 120, 107, 102, 89, 98, 115,
  111, 99, 68, 70, 117, 102, 65, 101, 52, 122, 107, 75, 71, 104, 52, 84, 69, 101, 90, 117,
  66, 111, 83, 75, 109, 84, 97, 69, 82, 117, 71, 51, 66, 85, 67, 89, 68, 73, 117, 106,
  116, 105, 72, 75, 119, 82, 87, 71, 102, 55, 79, 118, 121, 121, 54, 112, 52, 112, 52, 82,
  121, 53, 74, 65, 73, 67, 114, 107, 55, 57, 48, 107, 98, 52, 114, 51, 97, 52, 78, 79,
  117, 50, 57, 107, 116, 120, 51, 103, 115, 104, 74, 69, 108, 57, 97, 77, 110, 49, 89, 86,
  55, 101, 83, 55, 82, 74, 89, 114, 55, 71, 72, 76, 115, 86, 49, 86, 80, 74, 103, 100,
  89, 105, 76, 99, 120, 114, 43, 67, 87, 70, 110, 98, 103, 99, 88, 79, 53, 107, 71, 88,
  82, 121, 79, 73, 97, 90, 89, 107, 83, 80, 53, 71, 80, 119, 115, 99, 78, 75, 65, 61
};

// xslt/ods-extract-charts.xslt
static char data_ext_charts[] = {
  83, 77, 49, 57, 76, 98, 75, 121, 83, 89, 111, 101, 89, 55, 72, 47, 74, 97, 114, 78,
  85, 88, 74, 43, 78, 102, 102, 102, 97, 111, 54, 116, 105, 110, 97, 89, 107, 88, 67, 112,
  106, 101, 53, 51, 69, 112, 82, 89, 84, 120, 68, 120, 70, 73, 113, 54, 100, 71, 89, 48,
  87, 68, 90, 50, 114, 88, 69, 54, 122, 65, 110, 107, 115, 121, 74, 107, 52, 77, 113, 75,
  100, 90, 52, 84, 57, 51, 106, 80, 107, 79, 103, 97, 112, 118, 87, 87, 113, 109, 78, 98,
  100, 67, 115, 52, 110, 70, 104, 52, 78, 87, 107, 66, 90, 87, 111, 73, 79, 79, 73, 77,
  77, 56, 100, 90, 56, 88, 83, 110, 113, 81, 104, 80, 121, 72, 54, 100, 69, 70, 86, 101,
  54, 113, 68, 56, 54, 112, 77, 71, 76, 48, 99, 122, 109, 49, 50, 87, 90, 120, 88, 97,
  114, 100, 109, 51, 105, 86, 114, 43, 102, 54, 117, 69, 113, 51, 109, 68, 71, 83, 51, 67,
  48, 70, 111, 83, 121, 74, 72, 52, 88, 55, 105, 74, 86, 65, 74, 49, 57, 51, 51, 99,
  115, 68, 54, 116, 119, 79, 78, 110, 119, 86, 49, 57, 48, 105, 122, 81, 79, 108, 66, 51,
  72, 77, 100, 105, 72, 85, 85, 75, 88, 84, 122, 76, 79, 113, 55, 99, 109, 102, 98, 43,
  54, 57, 86, 103, 115, 115, 68, 107, 48, 116, 112, 86, 69, 54, 72, 119, 74, 69, 82, 104,
  66, 98, 88, 52, 57, 108, 121, 113, 101, 109, 103, 70, 57, 72, 118, 74, 86, 57, 56, 49,
  73, 108, 68, 113, 118, 68, 90, 78, 57, 47, 101, 106, 78, 43, 107, 103, 103, 71, 104, 57,
  71, 119, 48, 56, 50, 66, 73, 112, 114, 56, 117, 116, 52, 65, 115, 101, 47, 108, 117, 70,
  116, 69, 113, 49, 52, 115, 53, 56, 69, 78, 117, 75, 111, 103, 109, 122, 115, 52, 103, 118,
  122, 54, 109, 73, 57, 53, 90, 73, 76, 101, 120, 54, 43, 69, 110, 77, 114, 90, 65, 121,
  116, 101, 111, 53, 80, 68, 122, 80, 109, 102, 120, 104, 105, 79, 56, 57, 57, 47, 67, 68,
  102, 53, 79, 77, 107, 53, 74, 56, 65, 85, 54, 103, 118, 87, 90, 54, 116, 103, 70, 67,
  88, 72, 53, 118, 119, 122, 70, 76, 115, 100, 120, 76, 103, 73, 113, 86, 121, 110, 117, 49,
  98, 113, 56, 108, 47, 107, 50, 121, 116, 47, 69, 118, 74, 97, 98, 87, 77, 119, 80, 76,
  83, 57, 102, 116, 113, 66, 86, 72
};

// xslt/ods-extract-images.xslt
static char data_ext_imgs[] = {
  47, 117, 65, 85, 102, 87, 47, 97, 84, 118, 121, 78, 102, 112, 76, 109, 113, 65, 52, 75,
  70, 52, 86, 71, 65, 115, 102, 104, 115, 71, 99, 54, 78, 56, 65, 122, 111, 105, 69, 68,
  109, 108, 51, 79, 121, 112, 82, 107, 80, 70, 88, 99, 49, 52, 100, 89, 56, 70, 108, 76,
  82, 82, 51, 72, 47, 106, 116, 97, 120, 65, 78, 66, 113, 70, 101, 115, 112, 115, 103, 49,
  119, 87, 70, 103, 111, 102, 82, 78, 78, 121, 49, 116, 80, 105, 87, 109, 114, 75, 106, 118,
  90, 54, 49, 87, 108, 108, 74, 49, 119, 66, 76, 83, 83, 106, 52, 106, 87, 89, 117, 48,
  119, 79, 99, 89, 75, 79, 85, 107, 117, 114, 112, 48, 112, 98, 120, 86, 78, 54, 49, 85,
  103, 120, 117, 118, 110, 82, 122, 74, 70, 57, 120, 52, 106, 121, 54, 80, 115, 78, 74, 55,
  52, 112, 114, 55, 107, 74, 68, 115, 84, 71, 72, 51, 82, 67, 101, 57, 99, 56, 79, 97,
  82, 71, 111, 104, 110, 77, 105, 84, 112, 113, 51, 48, 103, 55, 113, 70, 117, 112, 109, 57,
  86, 110, 101, 111, 109, 81, 50, 55, 47, 55, 100, 112, 109, 118, 79, 80, 104, 67, 110, 121,
  105, 67, 99, 122, 110, 54, 121, 104, 112, 115, 79, 66, 98, 70, 71, 48, 90, 69, 118, 109,
  70, 115, 113, 113, 102, 52, 80, 116, 78, 71, 72, 51, 53, 100, 98, 108, 56, 54, 49, 84,
  50, 122, 70, 80, 104, 56, 86, 73, 51, 114, 71, 57, 53, 115, 47, 76, 106, 43, 120, 70,
  76, 69, 50, 73, 78, 48, 115, 88, 87, 84, 68, 111, 54, 50, 120, 87, 118, 90, 108, 82,
  87, 68, 66, 98, 77, 104, 53, 102, 102, 53, 76, 100, 43, 52, 82, 100, 56, 52, 103, 120,
  107, 43, 87, 117, 55, 72, 106, 73, 121, 89, 103, 75, 57, 74, 57, 80, 83, 48, 116, 116,
  71, 83, 119, 57, 49, 75, 66, 80, 107, 114, 119, 70, 55, 76, 103, 116, 106, 74, 86, 88,
  97, 106, 50, 71, 71, 47, 67, 101, 109, 73, 88, 85, 122, 100, 98, 66, 106, 106, 101, 53,
  77, 56, 67, 90, 66, 89, 70, 98, 113, 86, 89, 117, 116, 107, 120, 57, 67, 74, 53, 119,
  71, 120, 104, 84, 54, 67, 79, 99, 89, 87, 47, 113, 57, 105, 67, 97, 48, 54, 100, 76,
  115, 101, 50, 85, 97, 79, 56, 109, 70, 109, 50, 51, 81, 50, 71, 113, 71, 77, 50, 102,
  117, 103, 77, 67, 86, 49, 112, 102
};

static inline QString chartName(const QString &chart)
{
  QString str("<chart name=\"");
  int start = chart.find(str);
  if (start < 0)
    return QString::null;
  int pos = start + str.length();
  int end = chart.find("\"", pos);
  return chart.mid(pos, end - pos);
}

static inline QString imageName(const QString &image)
{
  QString str("<image name=\"");
  int start = image.find(str);
  if (start < 0)
    return QString::null;
  int pos = start + str.length();
  int end = image.find("\"", pos);
  return image.mid(pos, end - pos);
}

static inline QString imageLink(const QString &image)
{
  QString str(" link=\"");
  int start = image.find(str);
  if (start < 0)
    return QString::null;
  int pos = start + str.length();
  int end = image.find("\"", pos);
  return image.mid(pos, end - pos);
}

AQOdsGenerator::AQOdsGenerator()
{
  clear();
}

AQOdsGenerator::~AQOdsGenerator()
{
  if (!fileName_.isEmpty()) {
    if (fileStream_.is_open()) {
      fileStream_.flush();
      fileStream_.close();
    }
    QFile::remove(fileName_);
  }
}

void AQOdsGenerator::clear()
{
  if (!fileName_.isEmpty()) {
    if (fileStream_.is_open()) {
      fileStream_.flush();
      fileStream_.close();
    }
    QFile::remove(fileName_);
  }
  strStream_.str("");
  fileName_ = AQ_DISKCACHE_DIRPATH + QString::fromLatin1("/report_") +
              QDateTime::currentDateTime().toString("ddMMyyyyhhmmsszzz") +
              QString::fromLatin1(".str");
  fileStream_.open(fileName_.ascii(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
  if (!fileStream_.is_open())
    qWarning("AQOdsGenerator::clear : Error opening " + fileName_);
}

QString AQOdsGenerator::str()
{
  if (fileStream_.is_open()) {
    fileStream_.flush();
    fileStream_.close();
    fileStream_.open(fileName_.ascii(), std::ios_base::in | std::ios_base::out | std::ios_base::app);
    strStream_ << fileStream_.rdbuf();
    fileStream_.seekp(0, std::ios_base::end);
  }
  return strStream_.str();
}

bool AQOdsGenerator::generateOds(const QString &fileNameOut)
{
  AQS *aqs = globalAQS;

  QByteArray baAux;

  QByteArray baMain;
  baAux.setRawData(data_main, sizeof(data_main));
  baMain = aqs->fromBase64(&baAux);
  baAux.resetRawData(data_main, sizeof(data_main));
  baMain = aqs->decryptInternal(&baMain);

  QByteArray baChart;
  baAux.setRawData(data_chart, sizeof(data_chart));
  baChart = aqs->fromBase64(&baAux);
  baAux.resetRawData(data_chart, sizeof(data_chart));
  baChart = aqs->decryptInternal(&baChart);

  QByteArray baExtCharts;
  baAux.setRawData(data_ext_charts, sizeof(data_ext_charts));
  baExtCharts = aqs->fromBase64(&baAux);
  baAux.resetRawData(data_ext_charts, sizeof(data_ext_charts));
  baExtCharts = aqs->decryptInternal(&baExtCharts);

  QByteArray baExtImgs;
  baAux.setRawData(data_ext_imgs, sizeof(data_ext_imgs));
  baExtImgs = aqs->fromBase64(&baAux);
  baAux.resetRawData(data_ext_imgs, sizeof(data_ext_imgs));
  baExtImgs = aqs->decryptInternal(&baExtImgs);

  QString strInput(str());
  QString fileNameContent(fileName_ + QString::fromLatin1(".cnt"));
  aqs->xsltproc(&baMain, fileName_, fileNameContent);

  QString errMsg;
  int errLine, errColumn;
  QFile filAux(fileNameContent);
  filAux.open(IO_ReadOnly);
  QDomDocument docTmp;
  if (!docTmp.setContent(&filAux, &errMsg, &errLine, &errColumn)) {
    qWarning("'%s': XML error %s  line: %d  column: %d",
             fileNameContent.latin1(), errMsg.latin1(),
             errLine, errColumn);
  } else {
    QString cntAux(docTmp.toString());
    cntAux.replace("__HREF1__", "<text:a xlink:href");
    cntAux.replace("__HREF2__", ">");
    cntAux.replace("__HREF3__", "</text:a>");
    filAux.close();

    if (!docTmp.setContent(cntAux, &errMsg, &errLine, &errColumn)) {
      qWarning("'%s cntAux': XML error %s  line: %d  column: %d",
               fileNameContent.latin1(), errMsg.latin1(),
               errLine, errColumn);
    } else {
      QFile::remove(fileNameContent);
      filAux.open(IO_WriteOnly);
      QTextStream tsAux(&filAux);
      docTmp.save(tsAux, 0);
      filAux.close();
    }
  }

  QStringList charts;
  if (strInput.contains("<chart name=\"")) {
    charts = QStringList::split("__SEP__",
                                aqs->xsltproc(&baExtCharts, fileName_));
  }

  QValueList<QPair<QString, QByteArray> > objects;
  for (QStringList::const_iterator it = charts.begin(); it != charts.end(); ++it) {
    QCString chart(*it);
    if (chart.isEmpty() || chart == "\n")
      continue;
    objects.append(
      qMakePair(chartName(chart),
                aqs->xsltproc(&baChart, &chart))
    );
  }

  QStringList images;
  if (strInput.contains("<image name=\"")) {
    images = QStringList::split("__SEP__",
                                aqs->xsltproc(&baExtImgs, fileName_));
  }

  QValueList<QPair<QString, QString> > imgs;
  for (QStringList::const_iterator it = images.begin(); it != images.end(); ++it) {
    QCString image(*it);
    if (image.isEmpty() || image == "\n")
      continue;
    imgs.append(
      qMakePair(imageLink(image),
                imageName(image))
    );
  }

  AQZipWriter zip(fileNameOut);

  QByteArray mimeType(QCString("application/vnd.oasis.opendocument.spreadsheet"));
  mimeType.truncate(mimeType.size() - 1);
  zip.addFile("mimetype", mimeType);
  QFile filCont(fileNameContent);
  zip.addFile("content.xml", &filCont);

  for (int i = 0; i < objects.size(); ++i) {
    QPair<QString, QByteArray> object = objects[i];
    QString path(object.first + "/content.xml");
    zip.addFile(path, object.second);
  }

  for (int i = 0; i < imgs.size(); ++i) {
    QPair<QString, QString> img = imgs[i];
    QString path("Pictures/" + img.second + ".png");
    QString link(img.first);
    QFile fil(link);
    if (!fil.open(IO_ReadOnly))
      continue;
    zip.addFile(path, fil.readAll());
  }

  QString manifest("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<manifest:manifest"
                   " xmlns:manifest=\"urn:oasis:names:tc:opendocument:xmlns:manifest:1.0\">\n"
                   " <manifest:file-entry"
                   " manifest:media-type=\"application/vnd.oasis.opendocument.spreadsheet\""
                   " manifest:version=\"1.2\""
                   " manifest:full-path=\"/\"/>\n"
                   " <manifest:file-entry"
                   " manifest:media-type=\"text/xml\""
                   " manifest:full-path=\"content.xml\"/>\n");

  for (int i = 0; i < objects.size(); ++i) {
    QPair<QString, QByteArray> object = objects[i];
    manifest += QString(" <manifest:file-entry"
                        " manifest:media-type=\"text/xml\""
                        " manifest:full-path=\"%1/content.xml\"/>\n")
                .arg(object.first);

    manifest += QString(" <manifest:file-entry"
                        " manifest:media-type=\"application/vnd.oasis.opendocument.chart\""
                        " manifest:full-path=\"%1/\"/>\n")
                .arg(object.first);
  }

  if (!imgs.isEmpty()) {
    for (int i = 0; i < imgs.size(); ++i) {
      QPair<QString, QString> img = imgs[i];
      manifest += QString(" <manifest:file-entry"
                          " manifest:media-type=\"image/png\""
                          " manifest:full-path=\"Pictures/%1.png\"/>\n")
                  .arg(img.second);
    }

    manifest += QString(" <manifest:file-entry"
                        " manifest:media-type=\"\""
                        " manifest:full-path=\"Pictures/\"/>\n");
  }

  manifest += "</manifest:manifest>";
  QByteArray mf(manifest.utf8());
  mf.truncate(mf.size() - 1);
  zip.addFile("META-INF/manifest.xml", mf);
  zip.close();

  QFile::remove(fileNameContent);

  return true;
}