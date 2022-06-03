/***************************************************************************
                          FLParameterQuery.h
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

#ifndef FLPARAMETERQUERY_H
#define FLPARAMETERQUERY_H

#include <qstring.h>
#include <qvariant.h>

#include "AQGlobal.h"

/**
Representa objetos que encapsulan par�metros de una consulta  o un filtro

@author InfoSiAL S.L.
*/
class AQ_EXPORT FLParameterQuery {

public:

  /**
  constructor.

  @param n Nombre del par�metro
  @param a Alias del par�metro
  @param t Tipo del par�metro
  */
  FLParameterQuery( const QString & n, const QString & a, const int t );

  /**
  destructor
  */
  ~FLParameterQuery();

  /**
  Obtiene el nombre del par�metro.

  @return Nombre del par�metro
  */
  const QString & name() const {
    return name_;
  }

  /**
  Obtiene el alias del par�metro.

  @return Alias del par�metro
  */
  const QString & alias() const {
    return alias_;
  }

  /**
  Obtiene el tipo del par�metro.

  @return El tipo del par�metro
  */
  const int type() const {
    return type_;
  }

  /**
  Obtiene el valor del par�metro.

  @return El valor del par�metro
  */
  const QVariant & value() const {
    return value_;
  }

  /**
  Establece el valor del par�metro.

  @param v Valor del par�metro
  */
  void setValue( const QVariant & v ) {
    value_ = v;
  }

private:

  /**
  Nombre del par�metro
  */
  QString name_;

  /**
  Alias del par�metro
  */
  QString alias_;

  /**
  Tipo de datos del par�metros
  */
  int type_;

  /**
  Valor del par�metro
  */
  QVariant value_;
};

#endif
