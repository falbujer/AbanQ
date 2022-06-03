/***************************************************************************
                            FLScriptEditor.h
                          -------------------
 begin                : mie mar 3 2004
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

#ifndef FLSCRIPTEDITOR_H
#define FLSCRIPTEDITOR_H

#include <qdialog.h>
#include <qseditor.h>
#include <qtextedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>

#include "FLWidgetScriptEditor.h"

#include "AQGlobal.h"

#ifdef QSDEBUGGER
class QSReplaceText;
class QSPreferencesContainer;
class PreferencesBase;
#endif

/**
Editor para scripts.

@author InfoSiAL S.L.
*/
class AQ_EXPORT FLScriptEditor : public FLWidgetScriptEditor
{

  Q_OBJECT

public:

  /**
  constructor

  @param nFS Nombre del fichero del script (*.qs). Si no corresponde con ninguno de los
  ficheros de script cargados, se utilizar� su valor como contenido del editor, esto es
  �til para poder usar esta clase como editor general pasando en este par�metro el texto
  a editar.
  */
  FLScriptEditor(const QString &nFS, QWidget *parent = 0, const char *name = 0);

  /**
  destructor
  */
  ~FLScriptEditor();

  /**
  Establece el c�digo a editar
  */
  void setCode(const QString &c);

  /**
  Obtiene el c�digo en edici�n
  */
  QString code() const;

protected slots:

  /**
  Busca texto

  @param expr Texto a buscar
  */
  bool find(const QString &expr);

  /**
  Repite �ltima b�squeda de texto
  */
  bool reFind();

  /**
  Mueve el cursor directamente a una l�nea

  @param line N�mero de la linea a la que saltar
  */
  void gotoLine(const QString &line);

  /**
  Muestra di�logo de reemplazar texto
  */
  void showReplace();

  /**
  Reemplazar primera ocurrencia de texto encontrada
  */
  void editReplace();

  /**
  Reemplaza todas las ocurrencias de texto encontradas
  */
  void editReplaceAll();

  /**
  Muestra di�logo para editar las preferencias
  */
  void editPreferences();

  /**
  Aplicar y guarda las preferencias editadas
  */
  void savePreferences();

protected:

  /**
  Para eventos de pulsaci�n de teclas
  */
  void keyPressEvent(QKeyEvent *e);

private:

  /**
  Reemplaza ocurrencias de textos encontradas

  @param all Si es TRUE reemplaza todo lo encontrado, si es FALSE s�lo la
     primera ocurrencia
  */
  void editReplace(bool all);

  /**
  QSA Editor
  */
  QSEditor *ed;

  /**
  �ltima b�squeda realizada en el texto
  */
  QString lastSearch;

#ifdef QSDEBUGGER
  /**
  Cuadro de di�logo de reemplazar texto
  */
  QSReplaceText *replaceText;

  /**
  Editor de preferencias
  */
  PreferencesBase *qsaEditorSyntax;

  /**
  Di�logo contenedor para el editor de preferencias
  */
  QSPreferencesContainer *preferencesContainer;
#endif
};

#endif
