/***************************************************************************
                                flmodules.qs
                            -------------------
   begin                : lun abr 26 2004
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
   bajo  los  t?rminos  de  la  Licencia  P?blica General de GNU   en  su
   versi?n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

var util = new FLUtil();

function init()
{
  var botonCargar = this.child("botonCargar");
  var botonExportar = this.child("botonExportar");
  connect(botonCargar, "clicked()", this, "botonCargar_clicked");
  connect(botonExportar, "clicked()", this, "botonExportar_clicked");
  var cursor = this.cursor();
  if (cursor.modeAccess() == cursor.Browse) {
    botonCargar.setEnabled(false);
    botonExportar.setEnabled(false);
  }
}

function cargarFicheroEnBD(nombre, contenido, log, directorio)
{
  if ((!util.isFLDefFile(contenido) &&
       !nombre.endsWith(".qs") && !nombre.endsWith(".ar") &&
       !nombre.endsWith(".svg")) ||
      nombre.endsWith("untranslated.ts"))
    return;

  var cursorFicheros = new FLSqlCursor("flfiles");
  var cursor = this.cursor();

  cursorFicheros.select("nombre = '" + nombre + "'");
  if (!cursorFicheros.first()) {

    if (nombre.endsWith(".ar")) {
      if (!cargarAr(nombre, contenido, log, directorio))
        return;
    }

    log.append(util.translate("scripts", "- Cargando :: ") + nombre);
    cursorFicheros.setModeAccess(cursorFicheros.Insert);
    cursorFicheros.refreshBuffer();
    cursorFicheros.setValueBuffer("nombre", nombre);
    cursorFicheros.setValueBuffer("idmodulo", cursor.valueBuffer("idmodulo"));
    cursorFicheros.setValueBuffer("sha", util.sha1(contenido));
    cursorFicheros.setValueBuffer("contenido", contenido);
    cursorFicheros.commitBuffer();
  } else {
    cursorFicheros.setModeAccess(cursorFicheros.Edit);
    cursorFicheros.refreshBuffer();

    if (cursorFicheros.valueBuffer("sha") != util.sha1(contenido)) {
      var contenidoCopia = cursorFicheros.valueBuffer("contenido");
      log.append(util.translate("scripts", "- Actualizando :: ") + nombre);
      cursorFicheros.setModeAccess(cursorFicheros.Insert);
      cursorFicheros.refreshBuffer();
      var d = new Date;
      cursorFicheros.setValueBuffer("nombre", nombre + d.toString());
      cursorFicheros.setValueBuffer("idmodulo", cursor.valueBuffer("idmodulo"));
      cursorFicheros.setValueBuffer("contenido", contenidoCopia);
      cursorFicheros.commitBuffer();
      log.append(util.translate("scripts", "- Backup :: ") + nombre + d.toString());
      cursorFicheros.select("nombre = '" + nombre + "'");
      cursorFicheros.first();
      cursorFicheros.setModeAccess(cursorFicheros.Edit);
      cursorFicheros.refreshBuffer();
      cursorFicheros.setValueBuffer("idmodulo", cursor.valueBuffer("idmodulo"));
      cursorFicheros.setValueBuffer("sha", util.sha1(contenido));
      cursorFicheros.setValueBuffer("contenido", contenido);
      cursorFicheros.commitBuffer();

      if (nombre.endsWith(".ar")) {
        if (!cargarAr(nombre, contenido, log, directorio))
          return;
      }
    }
  }
}


function cargarAr(nombre, contenido, log, directorio)
{
  if (!sys.isLoadedModule("flar2kut"))
    return false;

  if (util.readSettingEntry("scripts/sys/conversionAr") != "true")
    return false;

  log.append(util.translate("scripts", "Convirtiendo %0 a kut").arg(nombre));
  contenido = sys.toUnicode(contenido, "UTF-8");
  contenido = flar2kut.iface.pub_ar2kut(contenido);

  nombre = nombre.toString().left(nombre.length - 3) + ".kut";
  if (contenido) {

    localEnc = util.readSettingEntry("scripts/sys/conversionArENC");
    if (!localEnc)
      localEnc = "ISO-8859-15";

    contenido = sys.fromUnicode(contenido, localEnc);
    cargarFicheroEnBD(nombre, contenido, log, directorio);
    // Volcar a disco el kut
    log.append(util.translate("scripts", "Volcando a disco ") + nombre);
    File.write(Dir.cleanDirPath(directorio + "/" + nombre), contenido);
  } else {
    log.append(util.translate("scripts", "Error de conversi?n"));
    return false
  }

         return true;
}

function cargarFicheros(directorio, extension)
{
  var dir = new Dir(directorio);
  var ficheros = dir.entryList(extension, Dir.Files);
  var log = this.child("log");
  for (var i = 0; i < ficheros.length; ++i) {
    cargarFicheroEnBD(ficheros[i], File.read(Dir.cleanDirPath(directorio + "/" + ficheros[i])), log, directorio);
    sys.processEvents();
  }
}

function botonCargar_clicked()
{
  var directorio = FileDialog.getExistingDirectory("", util.translate("scripts", "Elegir Directorio"));
  cargarDeDisco(directorio, true);
}

function botonExportar_clicked()
{
  var directorio = FileDialog.getExistingDirectory("", util.translate("scripts", "Elegir Directorio"));
  exportarADisco(directorio);
}

function aceptarLicenciaDelModulo(directorio)
{
  var licencia = Dir.cleanDirPath(directorio + "/COPYING");
  if (!File.exists(licencia)) {
    MessageBox.critical(util.translate("scripts", "El fichero " + licencia + " con la licencia del m?dulo no existe.\nEste fichero debe existir para poder aceptar la licencia que contiene."), MessageBox.Ok);
    return false;
  }

  var licencia = File.read(licencia);
  var dialog = new Dialog;
  dialog.width = 600;
  dialog.caption = util.translate("scripts", "Acuerdo de Licencia.");
  dialog.newTab(util.translate("scripts", "Acuerdo de Licencia."));
  var texto = new TextEdit;
  texto.text = licencia;
  dialog.add(texto);
  dialog.okButtonText = util.translate("scripts", "S?, acepto este acuerdo de licencia.");
  dialog.cancelButtonText = util.translate("scripts", "No, no acepto este acuerdo de licencia.");
  if (dialog.exec()) return true;
  else return false;
}

function cargarDeDisco(directorio, comprobarLicencia)
{
  if (directorio) {
    if (comprobarLicencia) {
      if (!aceptarLicenciaDelModulo(directorio)) {
        MessageBox.critical(util.translate("scripts", "Imposible cargar el m?dulo.\nLicencia del m?dulo no aceptada."), MessageBox.Ok);
        return;
      }
    }
    sys.cleanupMetaData();
    if (this.cursor().commitBuffer()) {
      this.child("idMod").setDisabled(true);

      var log = this.child("log");
      log.text = "";
      sys.processEvents();
      this.setDisabled(true);
      cargarFicheros(directorio + "/", "*.xml");
      cargarFicheros(directorio + "/forms/", "*.ui");
      cargarFicheros(directorio + "/tables/", "*.mtd");
      cargarFicheros(directorio + "/scripts/", "*.qs");
      cargarFicheros(directorio + "/queries/", "*.qry");
      cargarFicheros(directorio + "/reports/", "*.kut");
      cargarFicheros(directorio + "/reports/", "*.ar");
      cargarFicheros(directorio + "/reports/", "*.jrxml");
      cargarFicheros(directorio + "/reports/", "*.svg");
      cargarFicheros(directorio + "/translations/", "*.ts");
      this.setDisabled(false);
      log.append(util.translate("scripts", "* Carga finalizada."));
      this.child("lineas").refresh();
    }
  }
}

function tipoDeFichero(nombre)
{
  var posPunto = nombre.lastIndexOf(".");
  return nombre.right(nombre.length - posPunto);
}

function exportarADisco(directorio)
{
  if (directorio) {
    var curFiles = this.child("lineas").cursor();
    if (curFiles.size() != 0) {
      var dir = new Dir();
      var idModulo = this.cursor().valueBuffer("idmodulo");
      var log = this.child("log");
      log.text = "";
      directorio = Dir.cleanDirPath(directorio + "/" + idModulo);
      if (!dir.fileExists(directorio)) dir.mkdir(directorio);
      if (!dir.fileExists(directorio + "/forms")) dir.mkdir(directorio + "/forms");
      if (!dir.fileExists(directorio + "/scripts")) dir.mkdir(directorio + "/scripts");
      if (!dir.fileExists(directorio + "/queries")) dir.mkdir(directorio + "/queries");
      if (!dir.fileExists(directorio + "/tables")) dir.mkdir(directorio + "/tables");
      if (!dir.fileExists(directorio + "/reports")) dir.mkdir(directorio + "/reports");
      if (!dir.fileExists(directorio + "/translations")) dir.mkdir(directorio + "/translations");
      curFiles.first();

      var file, tipo;
      var contenido: String;

      this.setDisabled(true);

      do {
        file = curFiles.valueBuffer("nombre");
        tipo = tipoDeFichero(file);
        contenido = curFiles.valueBuffer("contenido");

        if (!contenido.isEmpty()) {
          switch (tipo) {
            case ".ui":
              File.write(directorio + "/forms/" + file, contenido);
              log.append(util.translate("scripts", "* Exportando " + file + "."));
              break;
            case ".qs":
              File.write(directorio + "/scripts/" + file, contenido);
              log.append(util.translate("scripts", "* Exportando " + file + "."));
              break;
            case ".qry":
              File.write(directorio + "/queries/" + file, contenido);
              log.append(util.translate("scripts", "* Exportando " + file + "."));
              break;
            case ".mtd":
              File.write(directorio + "/tables/" + file, contenido);
              log.append(util.translate("scripts", "* Exportando " + file + "."));
              break;
            case ".kut":
            case ".ar":
            case ".jrxml":
            case ".svg":
              File.write(directorio + "/reports/" + file, contenido);
              log.append(util.translate("scripts", "* Exportando " + file + "."));
              break;
            case ".ts":
              File.write(directorio + "/translations/" + file, contenido);
              log.append(util.translate("scripts", "* Exportando " + file + "."));
              break;
            default:
              log.append(util.translate("scripts", "* Omitiendo " + file + "."));
          }
        }
        sys.processEvents();
      } while (curFiles . next());
      this.setDisabled(false);
      log.append(util.translate("scripts", "* Exportaci?n finalizada."));
    }
  }
}
