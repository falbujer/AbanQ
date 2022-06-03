/***************************************************************************
                           FLAccessControl.h
                          -------------------
 begin                : Fri Oct 10 2005
 copyright            : (C) 2004-2005 by InfoSiAL S.L.
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

#ifndef FLACCESSCONTROL_H
#define FLACCESSCONTROL_H

class QObject;
class QString;
class QStringList;
class QDomElement;
class QDomDocument;

/**
Clase base para Controles de Acceso, tambi�n denominados Reglas de Control de Acceso.

Una regla de control de acceso se aplica a un usuario y a un objeto de alto nivel o contenedor
(ventanas principales,tablas, etc..), que a su vez contendr�n otros objetos (acciones, campos, etc..).
La regla est� definida por la siguiente informaci�n como cabecera de la misma, que la identifica
un�vocamente:

\code

         tipo           ;         nombre         ;      usuario           ;      permiso
--------------------------------------------------------------------------------------------------
FLAccessControl::type ; FLAccessControl::name ; FLAccessControl::user ; FLAccessControl::perm

\endcode

El tipo ser� el del objeto de alto nivel, el nombre ser� el del objeto, el usuario corresponder� al
nombre del usuario en la base de datos al que se le aplica la regla y permiso ser� un identificador
de texto que define el tipo de permiso que se atribuye al objeto para el usuario dado. Este permiso
es general o global y se aplicar� por defecto a todos los objetos hijos o que pertenezcan al objeto
de alto nivel.

Al mismo tiempo una regla podr� tener una lista de Objetos de Control de Acceso (denominados ACOs,
Access Control Objects), a los que se les quiere aplicar un permiso distinto al general. Los ACOs ser�n
objetos hijo o pertenecientes al objeto de alto nivel. Internamente la lista de ACOs est� compuesta por
tuplas de dos elementos; "nombre de objeto" y "permiso", el nombre de objeto ser� el que tiene asignado
dentro de la jerarqu�a de objetos pertenecientes al objeto de alto nivel y permiso ser� el permiso para ese
objeto y que sobreescribir� al permiso general.

Los valores de la regla se podr�n establecer a partir de un nodo DOM de un documento DOM/XML, mediante
FLAccessControl::set . De forma rec�proca se podr� obtener un nodo DOM con el contenido de la regla,
a insertar en un documento DOM/XML, mediante FLAccessControl::get . La estructura general en XML del nodo DOM
que representa una regla de control de acceso es la siguiente:

\code

 <[mainwindow,table,etc..] perm="XXX">
  <name>XXX</name>
  <user>XXX</user>
  <aco perm="XXX">XXX</aco>
  ....
  <aco perm="XXX">XXX</aco>
 </[mainwindow,table,etc..]>

\endcode

Por comodidad, tambi�n se proporciona el m�todo FLAccessControl::setAcos, que permite establecer la lista de
ACOs de una regla directamente a partir de una lista de cadenas de texto.

Esta clase no est� pensada para ser usada directamente, sino como base para clases derivadas que se
encargan espec�ficamente del procesamiento de objetos de alto nivel. Un ejemplo ser�a FLAccessControlMainWindow,
que se encarga de control de acceso para objetos de alto nivel de tipo "mainwindow", es decir ventanas principales,
como el selector de m�dulos, o cada una de las ventanas principales de los m�dulos.

@author InfoSiAL S.L.
*/
class FLAccessControl
{
public:

  /**
  Constructor
  */
  FLAccessControl();

  /**
  Destructor
  */
  virtual ~FLAccessControl();

  /**
  Obtiene el nombre del objeto de alto nivel.

  @return Cadena de texto con el nombre del objeto.
  */
  QString name() {
    return name_;
  }

  /**
  Obtiene el nombre del usuario de la base de datos.

  @return Cadena de texto con el nombre (login) del usuario.
  */
  QString user() {
    return user_;
  }

  /**
  Obtiene el permiso general.

  @return Cadena de texto que identifica el permiso a aplicar.
  */
  QString perm() {
    return perm_;
  }

  /**
  Establece el nombre del objeto de alto nivel.

  @param n Nombre del objeto.
  */
  void setName(const QString &n) {
    name_ = n;
  }

  /**
  Establece el nombre del usuario de la base de datos.

  @param u Nombre (login) del usuario.
  */
  void setUser(const QString &u) {
    user_ = u;
  }

  /**
  Establece el permiso general.

  @param p Cadena de texto con el identificador del permiso.
  */
  void setPerm(const QString &p) {
    perm_ = p;
  }

  /**
  Limpia la regla vaci�ndola y liberando todos los recursos
  */
  void clear();

  /**
  Devuelve una constante de texto que identifica el tipo.

  Esta funci�n deber� ser reimplementada en las clases derivadas que se
  encargan del procesamiento de un tipo de objeto concreto y devolver
  el identificador pertinente.

  @return Cadena de texto que identifica al tipo de objeto general de la regla, p.e.: "table".
  */
  virtual QString type();

  /**
  Define la regla de control de acceso a partir de la informaci�n de un nodo DOM de un documento DOM/XML dado.

  @param e Elemento correspondiente al nodo DOM que se utilizar� para definir la regla.
  */
  virtual void set(QDomElement *e);

  /**
  A partir del contenido de la regla de control de acceso crea un nodo DOM que se insertar� como
  hijo del primer nodo de un documento DOM/XML.

  @param d Documento DOM/XML donde se insertar� el nodo construido a partir de la regla de control de acceso.
  */
  virtual void get(QDomDocument *d);

  /**
  Establece la lista de ACOs a partir de una lista de cadenas de texto.

  Esta lista de textos deber� tener en sus componentes de orden par los nombres de los objetos,y en los
  componentes de orden impar el permiso a aplicar a ese objeto, p.e.: "pbAbrir", "r-", "lblTexto", "--", "tbBorrar", "rw",...

  @param acos Lista de cadenas de texto con los objetos y permisos.
  */
  virtual void setAcos(const QStringList &acos);

  /**
  Obtiene una lista de cadenas de texto correspondiente a la lista de ACOs establecida

  El formato de esta lista es igual al descrito en FLAccessControl::setAcos
  p.e.: "pbAbrir", "r-", "lblTexto", "--", "tbBorrar", "rw",...

  @return Lista de cadenas de texto con los objetos y permisos.
  */
  virtual QStringList getAcos();

  /**
  Procesa un objeto dado aplicando la regla de control de acceso.

  Este m�todo es virtual puro, y deber� definirse en cada una de las
  clases derivadas que se encargan de control de acceso de un tipo de
  objeto espec�fico. Por lo tanto, qu� se tiene que hacer y c�mo se debe
  hacer con la informaci�n de la regla de control de acceso con respecto
  a un tipo de objeto de alto nivel concreto queda en el �mbito de la clase
  derivada que se encarga de ese tipo de objeto.

  @param obj Puntero general al objeto a procesar. Debe ser o heredar de la clase QObject.
  */
  virtual void processObject(QObject *obj) = 0;

  /**
  Define la regla de control de acceso a partir de las propiedades de un objeto concreto.

  Las clases derivadas para cada uno de los tipos de objetos deber� implementar c�mo se define la
  regla de control a partir de las propiedades del objeto espec�fico.

  @param  obj Puntero general al objeto a procesar. Debe ser o heredar de la clase QObject.
  */
  virtual void setFromObject(QObject *obj) = 0;

protected:

  /**
  Almacena el nombre del objeto de alto nivel.
  */
  QString name_;

  /**
  Almacena el nombre del usuario de la base de datos.
  */
  QString user_;

  /**
  Almacena el permiso general de la regla de control de acceso.
  */
  QString perm_;

  /**
  Diccionario de permisos espec�ficos de los ACOs (Access Control Objects)
  hijos o pertenecientes al objeto de alto nivel. El diccionario almacena la
  correspondencia entre el nombre del ACO (utilizado como clave de b�squeda)
  y el permiso a aplicar.
  */
  QDict<QString> *acosPerms_;
};

#endif
