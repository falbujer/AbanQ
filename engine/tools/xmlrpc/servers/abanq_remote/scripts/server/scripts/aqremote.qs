/***************************************************************************
 aqremote.qs
 -------------------
 begin                : 28/01/2011
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

/** @file */

////////////////////////////////////////////////////////////////////////////
//// DECLARACION ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/** @class_declaration interna */
//////////////////////////////////////////////////////////////////
//// INTERNA /////////////////////////////////////////////////////
class interna
{
  var ctx;

  function interna(context)
  {
    this.ctx = context;
  }
}
//// INTERNA /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
class oficial extends interna
{
  const MAX_LOGIN_ATTEMPTS = 10;

  var qsac_;

  function oficial(context)
  {
    interna(context);
  }
  function serveXmlRpc(funName, args, callInfo)
  {
    return this.ctx.oficial_serveXmlRpc(funName, args, callInfo);
  }
  function printArguments(args)
  {
    return this.ctx.oficial_printArguments(args);
  }
  function authUser(args, callInfo)
  {
    return this.ctx.oficial_authUser(args, callInfo);
  }
  function xmlChangesModulesTrunk(mods)
  {
    return this.ctx.oficial_xmlChangesModulesTrunk(mods);
  }
  function xmlChangesProjectTrunk(args)
  {
    return this.ctx.oficial_xmlChangesProjectTrunk(args);
  }
  function xmlCompProject(args)
  {
    return this.ctx.oficial_xmlCompProject(args);
  }
  function xmlUpdatesProject(args)
  {
    return this.ctx.oficial_xmlUpdatesProject(args);
  }
  function xmlChangesUpdate(args)
  {
    return this.ctx.oficial_xmlChangesUpdate(args);
  }
  function projectHasValidContract(args)
  {
    return this.ctx.oficial_projectHasValidContract(args);
  }
  function getPackageDirect(args)
  {
    return this.ctx.oficial_getPackageDirect(args);
  }
  function getPackageContract(args)
  {
    return this.ctx.oficial_getPackageContract(args);
  }
  function getPackage(args, direct)
  {
    return this.ctx.oficial_getPackage(args, direct);
  }
  function compileQsa(args)
  {
    return this.ctx.oficial_compileQsa(args);
  }
  function userHasProject(nick, project)
  {
    return this.ctx.oficial_userHasProject(nick, project);
  }
  function userProjects(nick)
  {
    return this.ctx.oficial_userProjects(nick);
  }
  function clientDiagUi()
  {
    return this.ctx.oficial_clientDiagUi();
  }
  function shaOfFile(args)
  {
    return this.ctx.oficial_shaOfFile(args);
  }
}
//// OFICIAL /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////
class head extends oficial
{
  function head(context)
  {
    oficial(context);
  }
}
//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_declaration ifaceCtx */
/////////////////////////////////////////////////////////////////
//// INTERFACE  /////////////////////////////////////////////////
class ifaceCtx extends head
{
  function ifaceCtx(context)
  {
    head(context);
  }
  function pub_serveXmlRpc(funName, args, callInfo)
  {
    return this.serveXmlRpc(funName, args, callInfo);
  }
}

const iface = new ifaceCtx(this);
//// INTERFACE  /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//// DEFINICION ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/** @class_definition interna */
//////////////////////////////////////////////////////////////////
//// INTERNA /////////////////////////////////////////////////////
//// INTERNA /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
function oficial_serveXmlRpc(funName, args, callInfo)
{
  //debug("Function     : " + funName);
  //debug("Arguments    : " + args.left(300));
  //debug("Call Info    : " + callInfo);

  if (!callInfo || callInfo == undefined)
    return false;

  var _i = this.iface;

  switch (funName) {
    case "testArgs":
      return _i.printArguments(eval(args));

    case "authUser":
      return _i.authUser(eval(args), callInfo);

    case "xmlChangesModulesTrunk":
      return _i.xmlChangesModulesTrunk(eval(args));

    case "xmlChangesProjectTrunk": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.xmlChangesProjectTrunk(arrArgs);
    }

    case "xmlCompProject": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.xmlCompProject(arrArgs);
    }

    case "xmlUpdatesProject": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.xmlUpdatesProject(arrArgs);
    }

    case "xmlChangesUpdate": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.xmlChangesUpdate(arrArgs);
    }

    case "projectHasValidContract": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.projectHasValidContract(arrArgs);
    }

    case "getPackageDirect": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.getPackageDirect(arrArgs);
    }

    case "getPackageContract": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.getPackageContract(arrArgs);
    }

    case "compileQsa": {
      var arrArgs = eval(args);
      if (!_i.authUser(arrArgs, callInfo))
        return false;
      return _i.compileQsa(arrArgs);
    }

    case "clientDiagUi":
      return _i.clientDiagUi();

    case "shaOfFile":
      return _i.shaOfFile(eval(args));
  }

  return false;
}

function oficial_printArguments(args)
{
  var _i = this.iface;

  if (args == undefined)
    return " undefined:";
  if (args == NaN)
    return " NaN:";
  if (args == Infinity)
    return " Infinity:";

  var ret = "";

  if ((typeof args) == "object") {
    ret += "[";
    for (var k in args)
      ret += _i.printArguments(args[k]) + "#";
    if (ret.charAt(ret.length - 1) == "#")
      ret = ret.left(ret.length - 1);
    ret += "]";
    return ret;
  }

  ret += (typeof args) + ":" + args.toString();
  return ret;
}

function oficial_authUser(args, callInfo)
{
  var _i = this.iface;

  var addrIp = "attempts:" + callInfo.split(':')[0];
  var aqVar = new AQVar;
  var attempts = parseInt(aqVar.get(addrIp));

  if (isNaN(attempts))
    attempts = 0;
  else if (attempts > _i.MAX_LOGIN_ATTEMPTS) {
    debug("authUser     : Max attempts " + args[0]);
    return false;
  }

  var usr = args[0];
  var pwd = args[1];

  var qry = new AQSqlQuery;
  qry.setSelect("nick");
  qry.setFrom("wi_usuarios");
  qry.setWhere("nick='" + usr +
               "' and upper(password)='" + AQUtil.sha1(pwd) + "'");

  var ret = qry.exec() && qry.next();
  if (!ret) {
    ++attempts;
    aqVar.set(addrIp, attempts.toString());
    debug("authUser     : Fail " + usr);
  } else if (attempts > 0)
    aqVar.del(addrIp);

  return ret;
}

function oficial_xmlChangesModulesTrunk(mods)
{
  if ((typeof mods) != "object" || mods.length == 0)
    return false;

  var minVer = "2.2";
  for (var i = 0; i < mods.length; ++i) {
    if (mods[i].endsWith("@")) {
      minVer = mods[i];
      minVer = minVer.left(minVer.length - 1);
      mods[i] = "''";
      continue;
    }
    mods[i] = "'" + mods[i] + "'";
  }

  var where = "mv_changelog.version>'" + minVer + "' and " +
              "mv_changelog.idmodulo in (" + mods.toString() + ")";

  var qry = new FLSqlQuery;
  qry.setSelect("idarea,mv_changelog.idmodulo,mv_changelog.fecha,mv_changelog.descripcion,cambios");
  qry.setFrom("mv_changelog inner join flmodules on mv_changelog.idmodulo = flmodules.idmodulo");
  qry.setWhere(where);
  qry.setOrderBy("idmodulo,fecha");

  var ret = false;
  if (qry.exec() && qry.next())
    ret = sys.toXmlReportData(qry);
  if (ret != undefined)
    ret = ret.toString();
  return ret;
}

function oficial_xmlChangesProjectTrunk(args)
{
  var _i = this.iface;

  var xmlComp = _i.xmlCompProject(args);
  if (!xmlComp)
    return false;

  var doc = new QDomDocument;
  if (!doc.setContent(xmlComp))
    return false;

  var mods = "";
  var exts = "";

  var list = doc.elementsByTagName("module");
  for (var i = 0; i < list.length(); ++i) {
    var it = list.item(i).toElement();
    if (i > 0)
      mods += ",";
    mods += "'" + it.attribute("name") + "'";
  }

  if (mods.isEmpty())
    return false;

  list = doc.elementsByTagName("extension");
  for (var i = 0; i < list.length(); ++i) {
    var it = list.item(i).toElement();
    if (i > 0)
      exts += ",";
    exts += "'" + it.attribute("name") + "'";
  }

  if (exts.isEmpty())
    return false;

  var minVer = args[3];
  var where = "mv_changelog.version>'" + minVer + "' and " +
              "(mv_changelog.idmodulo in (" + mods + ") or " +
              "mv_changelog.codfuncional in (" + exts + "))";

  var qry = new FLSqlQuery;
  qry.setSelect("idarea,mv_changelog.idmodulo,mv_changelog.codfuncional," +
                "mv_changelog.fecha,mv_changelog.descripcion,cambios,desccorta");
  qry.setFrom("mv_changelog left join flmodules on mv_changelog.idmodulo = flmodules.idmodulo " +
              "left join mv_funcional on mv_changelog.codfuncional = mv_funcional.codfuncional");
  qry.setWhere(where);
  qry.setOrderBy("idmodulo,codfuncional,fecha");

  var ret = false;
  if (qry.exec() && qry.next())
    ret = sys.toXmlReportData(qry);
  if (ret != undefined)
    ret = ret.toString();
  return ret;
}

function oficial_xmlCompProject(args)
{
  var _i = this.iface;

  var nick = args[0];
  var project = args[2];

  if (!_i.userHasProject(nick, project))
    return false;

  return AQUtil.sqlSelect("mv_funcional", "xmlcomp",
                          "codfuncional = '" + project + "'");
}

function oficial_xmlUpdatesProject(args)
{
  var _i = this.iface;

  var nick = args[0];
  var project = args[2];

  if (!_i.userHasProject(nick, project))
    return false;

  var qry = new FLSqlQuery;
  qry.setSelect("fecha,versiondesde,versionhasta,idactualizacion,esrevision," +
                "libre,desrevision,fechaupdate,horaupdate,modulesdef,filesdef");
  qry.setFrom("mv_actualizacionesfun");
  qry.setWhere("codfuncional = '" + project + "'");
  qry.setOrderBy("fechaupdate desc,horaupdate desc");

  var ret = false;
  if (qry.exec() && qry.next())
    ret = sys.toXmlReportData(qry);
  if (ret != undefined)
    ret = ret.toString();
  return ret;
}

function oficial_xmlChangesUpdate(args)
{
  var _i = this.iface;

  var nick = args[0];
  var project = args[2];

  if (!_i.userHasProject(nick, project))
    return false;

  var id = args[3];

  var qry = new AQSqlQuery;
  qry.setSelect("idcambio");
  qry.setFrom("mv_cambiosactualizacion");
  qry.setWhere("idactualizacion = " + id);

  if (!qry.exec())
    return false;

  var chgs = "";
  while (qry.next()) {
    if (!chgs.isEmpty())
      chgs += ",";
    chgs += "'" + qry.value(0).toString() + "'";
  }

  if (chgs.isEmpty())
    return false;

  var where = "mv_changelog.id in (" + chgs + ")";

  qry = new FLSqlQuery;
  qry.setSelect("idarea,mv_changelog.idmodulo,mv_changelog.codfuncional," +
                "mv_changelog.fecha,mv_changelog.descripcion,cambios,desccorta");
  qry.setFrom("mv_changelog left join flmodules on mv_changelog.idmodulo = flmodules.idmodulo " +
              "left join mv_funcional on mv_changelog.codfuncional = mv_funcional.codfuncional");
  qry.setWhere(where);
  qry.setOrderBy("idmodulo,codfuncional,fecha");

  var ret = false;
  if (qry.exec() && qry.next())
    ret = sys.toXmlReportData(qry);
  if (ret != undefined)
    ret = ret.toString();
  return ret;
}

function oficial_projectHasValidContract(args)
{
  var _i = this.iface;

  var nick = args[0];
  var project = args[2];

  if (!_i.userHasProject(nick, project))
    return false;

  return AQUtil.sqlSelect("contratos", "codigo",
                          "codfuncional = '" + project + "' " +
                          "and estado = 'Vigente'");
}

function oficial_getPackageDirect(args)
{
  var _i = this.iface;

  var nick = args[0];
  var project = args[2];

  if (!_i.userHasProject(nick, project))
    return false;

  return _i.getPackage(args, true);
}

function oficial_getPackageContract(args)
{
  var _i = this.iface;

  if (!_i.projectHasValidContract(args))
    return false;

  return _i.getPackage(args, false);
}

function oficial_getPackage(args, direct)
{
  var _i = this.iface;

  var project = args[2];
  var id = args[3];

  var qry = new AQSqlQuery;
  qry.setSelect("nombreupdate");
  qry.setFrom("mv_actualizacionesfun");
  var where = "idactualizacion = " + id;
  if (direct)
    where += " and libre = true";
  qry.setWhere(where);

  if (!qry.exec() || !qry.next())
    return false;

  var dirBase = qry.value(0).toString();
  var fileRev = dirBase + "/REVISION";

  if (!File.exists(fileRev))
    return false;

  var revision = "";
  try {
    revision = File.read(fileRev);
  } catch (e) {
    return false;
  }

  var packageName = dirBase + "/" + project + "_" + revision;
  var files = args[4].split(':');
  var packager = new AQPackager(packageName);

  packager.setFilter(files);
  if (!packager.pack(dirBase + "/modulos"))
    return false;

  packageName = packageName + ".abanq";
  var file = new QFile(packageName);
  if (!file.open(File.ReadOnly))
    return false;
  return AQS.toBase64(AQS.compress(file.readAll()));
}

function oficial_compileQsa(args)
{
  var _i = this.iface;

  if (!_i.projectHasValidContract(args))
    return false;

  var fileName = args[3];
  var code = new QByteArray;
  code.string = args[4];

  if (_i.qsac_ == undefined)
    _i.qsac_ = new AQCompilerQSA;

  var qsac = _i.qsac_;
  var byteCode = qsac.byteCode(AQS.fromBase64(code));

  if (qsac.errorType() != 0) {
    var errLines = qsac.errorLines();
    var errMgs = qsac.errorMessages();
    var msg = String("## ERROR ##: No se puede seguir compilando, hay errores en %1:\n\n")
              .arg(fileName);
    for (var i = 0; i < errLines.length; ++i)
      msg += String(" Line %1: %2\n").arg(errLines[i]).arg(errMgs[i]);
    return msg;
  }

  return AQS.toBase64(byteCode);
}

function oficial_userHasProject(nick, project)
{
  var _i = this.iface;

  var uPros = _i.userProjects(nick);
  if ((typeof uPros) != "object" || uPros.length == 0) {
    debug("userHasProject: No " + project);
    return false;
  }
  for (var key in uPros) {
    if (uPros[key].cod == project)
      return true;
  }
  debug("userHasProject: No " + project);
  return false;
}

function oficial_userProjects(nick)
{
  var codCliente = AQUtil.sqlSelect("wi_usuarios", "codcliente",
                                    "nick = '" + nick + "'");
  var qry = new AQSqlQuery;
  if (!codCliente) {
    qry.setSelect("nombre,codfuncional");
    qry.setFrom("wi_ramasusuarios");
    qry.setWhere("nick = '" + nick + "'");
  } else {
    qry.setSelect("descripcion,codfuncional");
    qry.setFrom("se_proyectos");
    qry.setWhere("codcliente = '" + codCliente + "'");
  }

  if (!qry.exec())
    return false;

  var ret = [];

  while (qry.next()) {
    var nombre = qry.value(0).toString();
    var codFuncional = qry.value(1).toString();
    var codProyecto = AQUtil.sqlSelect("mv_funcional", "codfuncional",
                                       "proyecto and nombreproyecto='" + codFuncional + "'");
    if (!codProyecto)
      codProyecto = AQUtil.sqlSelect("mv_funcional", "codfuncional",
                                     "codfuncional='" + codFuncional + "'");
    ret.push( {
      cod: codProyecto,
      nom: nombre
    });
  }

  return ret;
}

function oficial_clientDiagUi()
{
  var mng = aqApp.db().managerModules();
  return mng.content("client_diag.ui");
}

function oficial_shaOfFile(args)
{
  var n = args[0];
  var mng = aqApp.db().managerModules();
  return mng.shaOfFile(n);
}
//// OFICIAL /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
