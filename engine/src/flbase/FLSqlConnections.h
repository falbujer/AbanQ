/***************************************************************************
                          FLSqlConnections.h
                          -------------------
 begin                : Mon Sep 18 2006
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

#ifndef FLSQLCONNECTIONS_H
#define FLSQLCONNECTIONS_H

#include <qdict.h>

#include "AQGlobal.h"

class FLSqlConnectionsPrivate;
class FLSqlDatabase;

/**
Clase para manejar distintas conexiones a bases de datos.

Cada conexi�n a una base de datos tendr� una cadena de caracteres
como nombre que la identifica. Se podr�n a�adir conexiones con FLSqlConnections::addDatabase,
eliminar con FLSqlConnections::removeDatabase y obtener con FLSqlConnections::database.

La conexi�n por defecto tendr� el nombre "default".

@author InfoSiAL S.L.
*/
class AQ_EXPORT FLSqlConnections
{
public:
  /**
  A�ade una base de datos a las conexiones disponibles.

  La base de datos ser� abierta. Si ya existiera una conexi�n con el mismo nombre
  la base datos correspondiente ser� cerrada y borrada, sustituy�ndola por la nueva.

  @param driverAlias Alias del driver ( PostgreSQL, MySQL, SQLite, ... ), ver FLSqlDatabase.
  @param nameDB  Nombre de la base de datos a la que conectar
  @param user  Usuario de la conexi�n
  @param password Contrase�a para el usuario
  @param host  Nombre o direcci�n del servidor de la base de datos
  @param port  Puerto TCP de conexion
  @param connectionName Nombre de la conexion
  @param connectOptions Contiene opciones auxiliares de conexi�n a la base de datos.
                        El formato de la cadena de opciones es una lista separada por punto y coma
                        de nombres de opci�n o la opci�n = valor. Las opciones dependen del uso del
                        driver de base de datos.
  @return TRUE si se pudo realizar la conexi�n, FALSE en caso contrario
  */
  static bool addDatabase(const QString &driverAlias, const QString &nameDB, const QString &user,
                          const QString &password, const QString &host, int port,
                          const QString &connectionName, const QString &connectOptions = QString::null);

  /**
  Sobrecargada por conveniencia

  Practicamente hace lo mismo que el m�todo anterior pero utilizando una base de datos ya construida

  @param db  Base datos a a�adir a las conexiones disponibles, ver FLSqlDatabase.
  @param connectionName Nombre de la conexion
  @return TRUE si se pudo realizar la conexi�n, FALSE en caso contrario
  */
  static bool addDatabase(FLSqlDatabase *db, const QString &connectionName = "default");

  /**
  Elimina una base de datos de las conexiones disponibles.

  Cierra la base de datos correspondiente y la elimina.

  @param connectionName Nombre de la conexion
  @return TRUE si se pudo eliminar la base de datos, FALSE en caso contrario
  */
  static bool removeDatabase(const QString &connectionName);

  /**
  Obtiene la base de datos de una conexion.

  @param connectionNmae Nombre de la conexion
  @return La base de datos correspondiente al nombre de conexion indicado
  */
  static FLSqlDatabase *database(const QString &connectionName = "default");

  /**
  Finalizar todas las conexiones
  */
  static void finish();

  /**
  @return Diccionario con las bases de datos abiertas
  */
  static QDict<FLSqlDatabase> *dictDatabases();

private:

  /**
  Privado
  */
  static FLSqlConnectionsPrivate *d;
};

#endif
