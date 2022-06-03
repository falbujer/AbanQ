/***************************************************************************
 AQOdsSpreadSheet.h
 -------------------
 begin                : 12/12/2011
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

#ifndef AQSODSSPREADSHEET_H
#define AQSODSSPREADSHEET_H

#include "odf-gen/spreadsheet.h"

#include "AQOdsGenerator.h"

class AQOdsSpreadSheet : public Spreadsheet
{
public:
  AQOdsSpreadSheet(AQOdsGenerator& gen)
    : Spreadsheet(gen.fileStream()) {}
};

#endif /* AQSODSSPREADSHEET_H */
