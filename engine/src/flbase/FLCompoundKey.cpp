/***************************************************************************
                          FLCompoundKey.cpp
                         -------------------
begin                : Fri Feb 1 2002
copyright            : (C) 2002-2005 by InfoSiAL S.L.
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

#include <qdict.h>
#include <qstringlist.h>

#include "FLTableMetaData.h"
#include "FLFieldMetaData.h"
#include "FLCompoundKey.h"

FLCompoundKey::FLCompoundKey()
{
  fieldList_ = new FLTableMetaData::FLFieldMetaDataList;
}

FLCompoundKey::FLCompoundKey(const FLCompoundKey *other)
{
  fieldList_ = new FLTableMetaData::FLFieldMetaDataList;
  copy(other);
}

FLCompoundKey::~FLCompoundKey()
{
  delete fieldList_;
}

void FLCompoundKey::addFieldMD(const FLFieldMetaData *f)
{
  fieldList_->insert(f->name().lower(), f);
}

bool FLCompoundKey::hasField(const QString &fN) const
{
  return fieldList_->find(fN.lower());
}

void FLCompoundKey::copy(const FLCompoundKey *other)
{
  if (other == this)
    return;
  *fieldList_ = *other->fieldList_;
}
