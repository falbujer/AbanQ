/***************************************************************************
                                aqcompile.qs
                            -------------------
   begin                : vie mar 18 2011
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
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

function main()
{
  var cur = new AQSqlCursor("flfiles");
  if (!cur.select("nombre like '%.qs'"))
    return;

  var qsac = new AQCompilerQSA;
  var p = 0;

  AQUtil.createProgressDialog("Compilando scripts", cur.size());

  while (cur.next()) {
    var fileName = cur.valueBuffer("nombre");
    var sha = cur.valueBuffer("sha");
    
    AQUtil.setLabelText(String("Compilando %1").arg(fileName));
    AQUtil.setProgress(++p);

    var key = String(fileName).left(30);
    var shaLast = AQUtil.readDBSettingEntry(key);
    if (!shaLast || shaLast != sha)
      AQUtil.writeDBSettingEntry(key, sha);
    else if (shaLast == sha)
      continue;
    
    cur.setModeAccess(AQSql.Edit);
    cur.refreshBuffer();

    var code = cur.valueBuffer("contenido");
    var byteCode = qsac.byteCode(sys.fromUnicode(code, "ISO8859-15"));

    if (qsac.errorType() != 0) {
      var diag = new QDialog;
      var lay = new QVBoxLayout(diag);
      var ed = new QSEditor(diag);

      lay.addWidget(ed);
      diag.resize(800, 600);
      ed.textEdit().readOnly = true;
      ed.setText(code);

      var errLines = qsac.errorLines();
      var errMgs = qsac.errorMessages();
      var msg = String("No se puede seguir compilando, hay errores en %1:\n\n").arg(fileName);

      for (var i = 0; i < errLines.length; ++i) {
        msg += String(" Line %1: %2\n").arg(errLines[i]).arg(errMgs[i]);
        ed.setErrorMark(errLines[i]);
        errorMsgBox(msg);
        diag.exec();
      }
      break;
    }

    cur.setValueBuffer("binario", byteCode);
    
    //if (code.left(3).endsWith("R")) {
    //  var byteCode2 = qsac.byteCode(sys.fromUnicode(code, "ISO8859-15"), 0, 0, true);
    //  var baOut = new QByteArray;
    //  var out = new QDataStream(baOut, File.WriteOnly);
    //  out.opInByteArray(byteCode2);
    //  var outText = sys.toUnicode(qsac.code(baOut), "ISO8859-15");
    //  cur.setValueBuffer("sha", AQUtil.sha1(outText));
    //  cur.setValueBuffer("contenido", outText);
    //}
    
    if (!cur.commitBuffer()) {
      errorMsgBox(String("Fallo al guardar el registro para %1").arg(fileName));
      break;
    }
  }

  AQUtil.destroyProgressDialog();
}

function errorMsgBox(msg)
{
  AQUtil.destroyProgressDialog();
  msg += "\n";
  MessageBox.critical(msg, MessageBox.Ok, MessageBox.NoButton,
                      MessageBox.NoButton, "AbanQ");
}

