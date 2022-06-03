/***************************************************************************
                         FLRelationMetaData.h
                          -------------------
 begin                : Mon Jul 2 2001
 copyright            : (C) 2001-2005 by InfoSiAL S.L.
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

#ifndef FLRELATIONMETADATA_H
#define FLRELATIONMETADATA_H

#include <qstring.h>

#include "AQGlobal.h"

class FLRelationMetaDataPrivate;

/**
Mantiene la descripcion/metadatos de las relaciones de un campo.

Esta clase solo tiene sentido cuando es parte de un objeto FLFieldMetaData.

IMPORTANTE: Aunque las relaciones gr�ficamente s�lo se dibujan en un sentido
      , ya que el otro se obvia, cuando haya que definirlas con c�digo es preciso
      indicar la relaci�n en ambos sentidos.

Ejemplo:Si tenemos ALMACENES <-M----1> PAISES, tenemos que especificar
    que ALMACENES est� relacionado M1 con PAISES, y tambi�n que PAISES esta
    relacionado 1M con ALMACENES, en c�digo quedar�a as�:

    - Para la tabla ALMACENES
    \code
    ....
    FLTableMetaData *tmd = new FLTableMetaData (n, tr ("Almacenes"), "almacenes.ui");
    FLFieldMetaData *f;
    FLRelationMetaData *r;

    f = new FLFieldMetaData ("codpais", tr ("Pais"), true, false, QVariant::String, 3);
    r = new FLRelationMetaData ("flpaises", "codpais", FLRelationMetaData::RELATION_M1);
    f->addRelationMD (r);
    tmd->addFieldMD (f);
    ....
    \endcode

    - Para la tabla PAISES
    \code
    ....
    FLTableMetaData *tmd = new FLTableMetaData (n, tr ("Paises"), "paises.ui");
    FLFieldMetaData *f;
    FLRelationMetaData *r;

    f = new FLFieldMetaData ("codpais", tr ("C�digo"), false, true, QVariant::String, 20);
    r = new FLRelationMetaData ("flalmacenes", "codpais", FLRelationMetaData::RELATION_1M);
    f->addRelationMD (r);
    tmd->addFieldMD (f);
    ....
    \endcode

@author InfoSiAL S.L.
*/
class AQ_EXPORT FLRelationMetaData : public QShared
{

public:

  /** Constantes de tipos de cardinalidades de una relacion */
  enum Cardinality {
    /** Uno a muchos */
    RELATION_1M = 0,
    /** Muchos a uno */
    RELATION_M1 = 1
  };

#ifdef FL_DEBUG
  static long count_;
#endif

  /**
  constructor

  @param fT Tabla for�nea relacionada
  @param fF Campo for�neo relacionado
  @param rC Cardinalidad de la relacion
  @param dC Borrado en cascada, s�lo se tiene en cuenta en cardinalidades M1
  @param uC Actualizaciones en cascada, s�lo se tiene en cuenta en cardinalidades M1
  @param cI Chequeos de integridad sobre la relacion
  */
  FLRelationMetaData(const QString &fT, const QString &fF, int rC,
                     bool dC = false, bool uC = false, bool cI = true);
  FLRelationMetaData(const FLRelationMetaData *other);

  /**
  destructor
  */
  ~FLRelationMetaData();

  /**
  Establece el nombre del campo relacionado.

  @param fN Nombre del campo relacionado
  */
  void setField(const QString &fN);

  /**
  Obtiene en el nombre del campo de la relacion.

  @return Devuelve el nombre del campo relacionado
  */
  QString field() const;

  /**
  Obtiene el nombre de la tabla for�nea.

  @return Devuelve el nombre de la tabla de la base de datos con la que se est� relacionada
  */
  QString foreignTable() const;

  /**
  Obtiene el nombre de la campo for�neo.

  @return Devuelve el nombre del campo de la tabla for�nea con la que est� relacionada
  */
  QString foreignField() const;

  /**
  Obtiene la cardinalidad de la relacion.

  @return Devuelve la cardinalidad de la relacion, mirando desde la tabla donde se
      define este objeto hacia la for�nea
  */
  int cardinality() const;

  /**
  Obtiene si la relaci�n implica borrados en cascada, s�lo se tiene en cuenta en cardinalidades M1.

  @return Devuelve TRUE si la relacion implica borrados en cascada, FALSE en caso contrario
  */
  bool deleteCascade() const;

  /**
  Obtiene si la relaci�n implica modificaciones en cascada, s�lo se tiene en cuenta en cardinalidades M1.

  @return Devuelve TRUE si la relacion implica modificaciones en cascada, FALSE en caso contrario
  */
  bool updateCascade() const;

  /**
  Obtiene si se deben aplicar la reglas de integridad sobre la relaci�n
  */
  bool checkIn() const;

private:

  /**
  Privado
  */
  FLRelationMetaDataPrivate *d;

  void copy(const FLRelationMetaData *other);
};

class FLRelationMetaDataPrivate
{
public:

  FLRelationMetaDataPrivate(const QString &fT, const QString &fF, int rC,
                            bool dC, bool uC, bool cI);

  FLRelationMetaDataPrivate() {}

  /**
  Nombre del campo a relacionar
  */
  QString field_;

  /**
  Nombre de la tabla for�nea a relacionar
  */
  QString foreignTable_;

  /**
  Nombre del campo for�neo relacionado
  */
  QString foreignField_;

  /**
  Cardinalidad de la relaci�n
  */
  int cardinality_;

  /**
  Indica si los borrados ser�n en cascada, en relaciones M1
  */
  bool deleteCascade_;

  /**
  Indica si las modificaciones ser�n en cascada, en relaciones M1
  */
  bool updateCascade_;

  /**
  Indica si se deben aplicar la reglas de integridad en esta relaci�n
  */
  bool checkIn_;
};

inline void FLRelationMetaData::setField(const QString &fN)
{
  d->field_ = fN.lower();
}

inline QString FLRelationMetaData::field() const
{
  return d->field_;
}

inline QString FLRelationMetaData::foreignTable() const
{
  return d->foreignTable_;
}

inline QString FLRelationMetaData::foreignField() const
{
  return d->foreignField_;
}

inline int FLRelationMetaData::cardinality() const
{
  return d->cardinality_;
}

inline bool FLRelationMetaData::deleteCascade() const
{
  return (d->deleteCascade_ && d->cardinality_ == RELATION_M1);
}

inline bool FLRelationMetaData::updateCascade() const
{
  return (d->updateCascade_ && d->cardinality_ == RELATION_M1);
}

inline bool FLRelationMetaData::checkIn() const
{
  return d->checkIn_;
}

#endif
