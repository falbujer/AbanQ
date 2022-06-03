/***************************************************************************
                         FLParameterQuery.cpp
                         -------------------
begin                : dom jun 23 2002
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

#include "FLParameterQuery.h"

FLParameterQuery::FLParameterQuery( const QString & n, const QString & a, const int t ) :
name_( n ), alias_( a ), type_( t ) {}

FLParameterQuery::~FLParameterQuery() {}
