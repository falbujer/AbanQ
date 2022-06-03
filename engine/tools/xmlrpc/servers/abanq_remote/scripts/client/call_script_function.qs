class Http
{
  var host_;
  var port_;
  var diag_;
  var lblSt_;
  var prBar_;
  var http_;
  var data_;
  var buffer_;
  var log_;
  var error_;

  function Http(host, port)
  {
    var _i = this;

    _i.host_ = host;
    _i.port_ = (port == undefined ? 80 : port);
    _i.diag_ = new QDialog;
    _i.diag_.caption = "AbanQ Http";
    _i.diag_.name = "abanHttp";
    _i.diag_.modal = true;

    var lay = new QVBoxLayout(_i.diag_);

    _i.lblSt_ = new QLabel(_i.diag_);
    _i.lblSt_.alignment = AQS.AlignHCenter | AQS.AlignVCenter;
    lay.addWidget(_i.lblSt_);

    _i.prBar_ = new QProgressBar(_i.diag_);
    _i.prBar_.setCenterIndicator(true);
    _i.prBar_.setTotalSteps(100);
    _i.prBar_.setProgress(100);
    lay.addWidget(_i.prBar_);

    _i.diag_.resize(100, 50);

    _i.http_ = new QHttp(host, port);
    _i.log_ = "";
    _i.error_ = false;

    connect(_i.http_, "stateChanged(int)",          _i, "changeState()");
    connect(_i.http_, "dataSendProgress(int, int)", _i, "progressSend()");
    connect(_i.http_, "dataReadProgress(int, int)", _i, "progressRead()");
    connect(_i.http_, "requestStarted(int)",        _i, "startRequest()");
    connect(_i.http_, "requestFinished(int, bool)", _i, "finishRequest()");
    connect(_i.http_, "done(bool)",                 _i, "allDone()");
  }

  function close()
  {
    var _i = this;

    _i.http_.closeConnection();
    _i.diag_.close();

    _i.http_ = undefined;
    _i.diag_ = undefined;
    _i.data_ = undefined;
    _i.buffer_ = undefined;
  }

  function appendLog(msg)
  {
    var _i = this;

    if (_i.diag_ == undefined || !_i.diag_.visible)
      return;

    _i.log_ += msg + "\n";
    _i.lblSt_.text = msg;
  }

  function setProgress(done, total)
  {
    var _i = this;

    if (total > 0)
      _i.prBar_.setTotalSteps(total);
    _i.prBar_.setProgress(done);
  }

  function changeState(state)
  {
    var _i = this;

    var msg = sys.translate("Estado: %1");
    switch (state) {
      case AQS.HttpUnconnected:
        msg = msg.argStr(sys.translate("Desconectado"));
        break;
      case AQS.HttpHostLookup:
        msg = msg.argStr(sys.translate("Buscando Host"));
        break;
      case AQS.HttpConnecting:
        msg = msg.argStr(sys.translate("Conectando"));
        break;
      case AQS.HttpSending:
        msg = msg.argStr(sys.translate("Enviando"));
        break;
      case AQS.HttpReading:
        msg = msg.argStr(sys.translate("Recibiendo"));
        break;
      case AQS.HttpConnected:
        msg = msg.argStr(sys.translate("Conectado"));
        break;
      case AQS.HttpClosing:
        msg = msg.argStr(sys.translate("Cerrando"));
        break;
      default:
        msg = msg.argStr(sys.translate("Desconocido"));
    }
    _i.appendLog(msg);
  }

  function progressSend(done, total)
  {
    var _i = this;

    var msg = sys.translate("Enviando %1 bytes de %2");
    _i.appendLog(msg.argInt(done).argInt(total));
    _i.setProgress(done, total);
  }

  function progressRead(done, total)
  {
    var _i = this;

    var msg = sys.translate("Recibiendo %1 bytes de %2");
    _i.appendLog(msg.argInt(done).argInt(total));
    _i.setProgress(done, total);
  }

  function startRequest(id)
  {
    var _i = this;

    var msg = sys.translate("Iniciando petición %1");
    _i.appendLog(msg.argInt(id));
  }

  function finishRequest(id, error)
  {
    var _i = this;

    var msg = sys.translate("Finalizando petición %1 : ");
    if (error)
      _i.appendLog(msg.argInt(id) + "ERROR");
    else
      _i.appendLog(msg.argInt(id) + "OK");
  }

  function allDone(error)
  {
    var _i = this;

    _i.error_ = error;
    var msg = sys.translate("Comunicación realizada; Resultado : ");
    if (!error)
      _i.appendLog(msg + "OK");
    else
      _i.appendLog(msg + "ERROR");

    sys.AQTimer.singleShot(0, function() {
      if (_i.diag_ != undefined && _i.diag_.visible)
        _i.diag_.close();
    });
  }

  function request(method, path, contentType, data)
  {
    var _i = this;

    _i.data_ = new QByteArray;
    _i.buffer_ = new QBuffer(_i.data_);
    _i.log_ = "";
    _i.error_ = false;

    var header = new QHttpRequestHeader(method, path);
    header.setValue("Cache-Control", "no-cache");
    header.setContentType(contentType);
    header.setContentLength(data.size);
    _i.http_.request(header, data, _i.buffer_);

    _i.diag_.exec();
  }

  function state()
  {
    var _i = this;

    return _i.http_.state();
  }
}

class XmlRpcClient
{
  const MAX_COST_CACHE = 20;
  var uuid_;
  var cache_;
  var cacheCount_;
  var host_;
  var port_;
  var http_;


  function XmlRpcClient()
  {
    var _i = this;

    _i.uuid_ = AQS.createUuid();
    _i.cache_ = [];
    _i.cacheCount_ = 0;
    _i.host_ = "ns1.infosial.com";
    _i.port_ = 8081;
  }

  function close()
  {
    var _i = this;

    if (_i.http_ != undefined) {
      _i.http_.close();
      _i.http_ = undefined;
      _i.cache_ = [];
      _i.cacheCount_ = 0;
    }
  }

  function cacheInsert(key, ba)
  {
    var _i = this;

    _i.cacheCount_++;
    _i.cache_[key] = ba;
    if (_i.cacheCount_ > _i.MAX_COST_CACHE) {
      var c = _i.MAX_COST_CACHE / 3;
      for (var k in _i.cache_) {
        _i.cacheCount_--;
        _i.cache_[k] = undefined;
        if (--c <= 0)
          break;
      }
    }
  }

  function parseArgumentsWithType(args)
  {
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
        ret += parseArgumentsWithType(args[k]) + "#";
      if (ret.charAt(ret.length - 1) == "#")
        ret = ret.left(ret.length - 1);
      ret += "]";
      return ret;
    }

    ret += (typeof args) + ":" + args.toString();
    return ret;
  }

  function parseArguments(args)
  {
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
        ret += parseArguments(args[k]) + ",";
      if (ret.charAt(ret.length - 1) == ",")
        ret = ret.left(ret.length - 1);
      ret += "]";
      return ret;
    }

    if ((typeof args) == "string")
      ret += "\"" + args.toString() + "\"";
    else
      ret += args.toString();
    return ret;
  }

  function callScriptFunction(funName, args, nocache)
  {
    var _i = this;

    var baFunName = new QByteArray;
    baFunName.string = funName;
    baFunName = AQS.encryptInternal(baFunName);

    var baArgs = new QByteArray;
    baArgs.string = parseArguments(args);
    baArgs = AQS.encryptInternal(baArgs);

    var ret = this.callXmlRpc((nocache ? "_" : "#") +
                              _i.uuid_ + AQS.toBase64(baFunName),
                              AQS.toBase64(baArgs));
    var error = ret[0];

    if (error) {
      this.close();
      _i.port_ = 8082;
      ret = this.callXmlRpc((nocache ? "_" : "#") +
                            _i.uuid_ + AQS.toBase64(baFunName),
                            AQS.toBase64(baArgs));
      error = ret[0];
    }

    if (error) {
      this.close();
      _i.port_ = 8083;
      ret = this.callXmlRpc((nocache ? "_" : "#") +
                            _i.uuid_ + AQS.toBase64(baFunName),
                            AQS.toBase64(baArgs));
      error = ret[0];
    }

    if (error) {
      this.close();
      _i.port_ = 8081;
      ret = this.callXmlRpc((nocache ? "_" : "#") +
                            _i.uuid_ + AQS.toBase64(baFunName),
                            AQS.toBase64(baArgs));
      error = ret[0];
    }

    if (error) {
      sys.errorMsgBox("HTTP ERROR\n" + ret[1]);
      return "false";
    }

    var doc = new QDomDocument;
    if (!doc.setContent(ret[2])) {
      sys.errorMsgBox("Error cargando XML:\n" + ret[2].toVariant);
      return "false";
    }

    var fault = (doc.elementsByTagName("fault").count() > 0);
    if (fault) {
      sys.errorMsgBox("XML-RPC FAULT\n" + doc.toString(2));
      return "false";
    }

    var txt = doc.elementsByTagName("string").item(0).toElement().text();
    var pos = txt.indexOf('@');
    var serial = txt.left(pos);
    var retTxt = txt.mid(pos + 1);

    if (!retTxt || retTxt.isEmpty()) {
      if ((serial in _i.cache_) && _i.cache_[serial] != undefined)
        return _i.cache_[serial];
      return _i.callScriptFunction(funName, args, true);
    }

    var baRet = new QByteArray;
    baRet.string = retTxt;
    baRet = baRet.fromBase64();
    var baRetval = AQS.decryptInternal(baRet);
    _i.cacheInsert(serial, baRetval);
    return baRetval;
  }

  function callXmlRpc(funName, args)
  {
    var _i = this;

    var doc = new QDomDocument;
    var instr = doc.createProcessingInstruction(
                  "xml", "version=\"1.0\" encoding=\"utf-8\""
                );

    doc.appendChild(instr);

    var methodCall = doc.createElement("methodCall");
    doc.appendChild(methodCall);

    var methodName = doc.createElement("methodName");
    methodName.appendChild(doc.createTextNode("callScriptFunction"));
    methodCall.appendChild(methodName);

    var params = doc.createElement("params");
    methodCall.appendChild(params);

    var param = doc.createElement("param");
    param.appendChild(this.nodeValue(doc, "string", funName));
    params.appendChild(param);

    param = doc.createElement("param");
    param.appendChild(this.nodeValue(doc, "string", args.toString()));
    params.appendChild(param);

    var data = new QByteArray;
    var ts = new QTextStream(data, File.WriteOnly);

    ts.setEncoding(AQS.UnicodeUTF8);
    doc.save(ts, 2);

    if (_i.http_ != undefined) {
      var state = _i.http_.state();
      if (state == AQS.HttpUnconnected || state == AQS.HttpClosing)
        _i.close();
    }

    if (_i.http_ == undefined)
      _i.http_ = new Http(_i.host_, _i.port_);

    _i.http_.request("POST", "/RPC2", "text/xml", data);

    return [_i.http_.error_, _i.http_.log_, _i.http_.data_];
  }

  function nodeValue(doc, t, v)
  {
    var value = doc.createElement("value");
    var type = doc.createElement(t);
    type.appendChild(doc.createTextNode(v.toString()));
    value.appendChild(type);
    return value;
  }
}

class AbanQUpdaterDialog
{
  var rpc_;
  var w_;
  var layChangelog_;
  var layUpdates_;
  var lwModsChangelog_;
  var lwExtsChangelog_;
  var tbUpdates_;
  var splChangelog_;
  var leUsr_;
  var lePwd_;
  var lePro_;
  var lbUpt_;
  var pbCompile_;
  var lbCompile_;
  var prCompile_;
  var sigMap_;

  function AbanQUpdaterDialog()
  {
    var _i = this;

    _i.rpc_ =  new XmlRpcClient;

    if (!_i.downloadClientUi())
      return;

    var mng = aqApp.db().managerModules();
    _i.w_ = mng.createUI("client_diag.ui");
    if (_i.w_) {
      _i.setupUi();
      _i.connectToServer();
      _i.w_.exec();
    }

    _i.rpc_.close();
    _i.w_ = undefined;
    _i.rpc_ = undefined;
  }

  function setupUi()
  {
    var _i = this;
    var w = _i.w_;

    _i.layChangelog_ = w.child("frChangelog").layout();
    _i.layUpdates_ = w.child("frUpdates").layout();
    _i.leUsr_ = w.child("leUsr");
    _i.lePwd_ = w.child("lePwd");
    _i.lePro_ = w.child("lePro");
    _i.lbUpt_ = w.child("lblUpdates");
    _i.pbCompile_ = w.child("pbCompile");
    _i.lbCompile_ = w.child("lblCompile");
    _i.prCompile_ = w.child("pgbCompile");

    var dbProName = AQUtil.readDBSettingEntry("projectname");
    if (!dbProName)
      _i.lePro_.enabled = true;
    else
      _i.lePro_.text = dbProName;

    _i.readAccountInfo();

    connect(w.child("pbConectar"), "clicked()", _i, "connectToServer()");
    connect(w.child("pbCrearCuenta"), "clicked()", _i, "createAccount()");
    connect(w.child("pbOlvido"), "clicked()", _i, "restorePasswd()");
    connect(_i.pbCompile_, "clicked()", _i, "compileScripts()");

    _i.sigMap_ = new QSignalMapper(w);
    connect(_i.sigMap_, "mapped(const QString&)", _i, "doAction()");
  }

  function doAction(n)
  {
    var _i = this;

    var args = n.split(':');
    var act = args[0];

    switch (act) {
      case "verUpdate":
        _i.showChangesUpdate(args[1]);
        break;
      case "instalarUpdate":
        _i.installUpdate(args[1], args[2], false);
        break;
      case "instalarUpdateDirect":
        _i.installUpdate(args[1], args[2], true);
        break;
      case "showMsg":
        _i.showMsg(args[1]);
        break;

      default:
        debug("Action not hanled : " + n);
    }
  }

  function connectToServer()
  {
    var _i = this;

    if (_i.leUsr_.text.isEmpty() || _i.lePwd_.text.isEmpty()) {
      _i.lbUpt_.text = sys.translate("Cree una cuenta de actualizaciones o introduzca una válida para poder instalar las mejoras");
      _i.updateChangelogMods();
      return;
    }

    var auth = _i.rpc_.callScriptFunction(
                 "authUser", [_i.leUsr_.text, _i.lePwd_.text]
               ).toString();
    if (!auth || auth == "false" || auth == "invalid") {
      sys.errorMsgBox(sys.translate("Usuario o contraseña no válidos"));
      return;
    }

    _i.writeAccountInfo();
    _i.updateChangelogExts();
    _i.updateUpdates();
  }

  function createAccount()
  {
    sys.openUrl("http://abanq.org/productos/producto.php?ref=alta_rama");
  }

  function restorePasswd()
  {
    sys.openUrl("http://abanq.org/cuenta/olvide_contra.php");
  }

  function contractUpdates()
  {
    sys.openUrl("http://abanq.org/productos/producto.php?ref=seractmes");
  }

  function buyUpdate()
  {
    sys.openUrl("http://abanq.org/productos/producto.php?ref=seractpuntual");
  }

  function writeAccountInfo()
  {
    var _i = this;

    var settings = new AQSettings;
    var key = "Updater/";

    settings.writeEntry(key + "user", _i.leUsr_.text);
    var ba = new QByteArray;
    ba.string = _i.lePwd_.text;
    settings.writeEntry(key + "password",
                        AQS.toBase64(AQS.encryptInternal(ba)).toString());
    settings.writeEntry(key + "project", _i.lePro_.text);
  }


  function readAccountInfo()
  {
    var _i = this;

    var settings = new AQSettings;
    var key = "Updater/";

    if (_i.leUsr_.text.isEmpty())
      _i.leUsr_.text = settings.readEntry(key + "user");
    if (_i.lePwd_.text.isEmpty()) {
      var strPwd = settings.readEntry(key + "password");
      if (!strPwd.isEmpty()) {
        var ba = new QByteArray;
        ba.string = strPwd;
        _i.lePwd_.text = AQS.decryptInternal(AQS.fromBase64(ba)).toString();
      }
    }
    if (_i.lePro_.text.isEmpty())
      _i.lePro_.text = settings.readEntry(key + "project");
  }

  function updateRequest()
  {
    var _i = this;

    var diag = new QDialog;
    diag.caption = "Solicitar actualización";
    diag.modal = true;

    var msg = sys.translate("Actualmente no tiene un contrato de actualizaciones vigente ");
    msg += sys.translate("para este proyecto. Puede contratarlo ahora o si lo prefiere ");
    msg += sys.translate("puede comprar puntualmente una actualización. ");

    var layV = new QVBoxLayout(diag);
    layV.margin = 6;
    layV.spacing = 6;

    var lbl = new QLabel(diag);
    lbl.text = msg;
    lbl.alignment = AQS.AlignTop | AQS.WordBreak;

    layV.addWidget(lbl);

    var layV2 = new QVBoxLayout(layV);
    layV2.margin = 6;
    layV2.spacing = 6;

    var pbContract = new QPushButton(diag);
    pbContract.text = sys.translate("Contratar Servicio de Actualizaciones");
    var pbBuy = new QPushButton(diag);
    pbBuy.text = sys.translate("Comprar sólo esta actualización");
    var pbCancel = new QPushButton(diag);
    pbCancel.text = sys.translate("Cancelar");

    layV2.addWidget(pbContract);
    layV2.addWidget(pbBuy);
    layV2.addWidget(pbCancel);

    connect(pbContract, "clicked()", _i, "contractUpdates()");
    connect(pbBuy, "clicked()", _i, "buyUpdate()");
    connect(pbCancel, "clicked()", diag, "reject()");

    diag.exec();
  }

  function compileRequest()
  {
    var _i = this;

    var diag = new QDialog;
    diag.caption = "Solicitar compilación";
    diag.modal = true;

    var msg = sys.translate("Para poder compilar scripts debe tener un contrato de ");
    msg += sys.translate("actualizaciones vigente para este proyecto.");

    var layV = new QVBoxLayout(diag);
    layV.margin = 6;
    layV.spacing = 6;

    var lbl = new QLabel(diag);
    lbl.text = msg;
    lbl.alignment = AQS.AlignTop | AQS.WordBreak;

    layV.addWidget(lbl);

    var layV2 = new QVBoxLayout(layV);
    layV2.margin = 6;
    layV2.spacing = 6;

    var pbContract = new QPushButton(diag);
    pbContract.text = sys.translate("Contratar Servicio de Actualizaciones");
    var pbCancel = new QPushButton(diag);
    pbCancel.text = sys.translate("Cancelar");

    layV2.addWidget(pbContract);
    layV2.addWidget(pbCancel);

    connect(pbContract, "clicked()", _i, "contractUpdates()");
    connect(pbCancel, "clicked()", diag, "reject()");

    diag.exec();
  }

  function validContract()
  {
    var _i = this;

    var xml = _i.rpc_.callScriptFunction(
                "projectHasValidContract",
                [_i.leUsr_.text, _i.lePwd_.text, _i.lePro_.text]
              ).toString();

    if (!xml || xml == "false" || xml == "invalid")
      return false;
    return xml;
  }

  function reportFilesChanged(changes)
  {
    var ret = "";
    for (var key in changes) {
      if (key == "size")
        continue;
      var chg = changes[key].split('@');
      if (chg[1] != "del") {
        if (!ret.isEmpty())
          ret += ":";
        ret += chg[0];
      }
    }
    return ret;
  }

  function isFilesDefInstalled(filesdef)
  {
    var doc = new QDomDocument;
    if (!doc.setContent(filesdef))
      return false;
    var root = doc.firstChild();
    var shaSum = root.namedItem("shasum").toElement().text();
    return AQUtil.sqlSelect("flupdates", "id",
                            "filesdef like '%<shasum>" + shaSum +
                            "</shasum>%'");
  }

  function installUpdate(id, filesdef, direct)
  {
    var _i = this;

    if (!filesdef || filesdef.isEmpty()) {
      var contract = _i.validContract();
      if (!contract || contract == "false" || contract == "invalid") {
        _i.updateRequest();
      } else {
        _i.showMsg(sys.translate(
                    "Esta actualización no está disponible, por favor contacte con InfoSiAL para activarla."
                   ));
      }
      return;
    }

    if (!direct) {
      var contract = _i.validContract();
      if (!contract || contract == "false" || contract == "invalid") {
        _i.updateRequest();
        return;
      }
    }

    var docUpt = new QDomDocument;
    if (!docUpt.setContent(filesdef)) {
      sys.errorMsgBox(sys.translate(
                        "Error XML al intentar cargar la definición de los ficheros."
                      ));
      return;
    }
    var docBd = sys.xmlFilesDefBd();
    var changes = sys.diffXmlFilesDef(docBd, docUpt);
    var filesChanged = _i.reportFilesChanged(changes);

    if (filesChanged.isEmpty()) {
      _i.showMsg(sys.translate("Ya está actualizado."));
      return;
    }

    var xml = _i.rpc_.callScriptFunction(
                direct ? "getPackageDirect" : "getPackageContract",
                [_i.leUsr_.text, _i.lePwd_.text, _i.lePro_.text, id, filesChanged]
              );

    if (!xml || xml.toString() == "false" || xml.toString() == "invalid") {
      sys.errorMsgBox(sys.translate(
                        "No se ha podido obtener la actualización desde el servidor."
                      ));
      return;
    }

    var fileName = Dir.home + "/" +
                   _i.lePro_.text + "_" + id + ".abanq";
    var ba = new QByteArray(AQS.uncompress(AQS.fromBase64(xml)));
    var file = new QFile(fileName);

    if (!file.open(File.WriteOnly)) {
      sys.errorMsgBox(file.errorString());
      return;
    }

    file.writeBlock(ba);
    file.close();

    _i.showMsg(
      sys.translate(
        "Se ha obtenido correctamente el paquete de actualización en:\n\n%1\n\n" +
        "Ahora se va a proceder a hacer una copia de seguridad.\n"
      ).arg(fileName)
    );

    var dumper = new sys.AbanQDbDumper;
    sys.AQTimer.singleShot(0, function() {
      dumper.initDump();
    });
    dumper.fileName_ += "_" + _i.lePro_.text + "_" + id;
    dumper.init();

    sys.loadAbanQPackage(fileName, !dumper.state().ok);
    _i.w_.close();
  }

  function compileScripts()
  {
    var _i = this;

    if (_i.leUsr_.text.isEmpty() || _i.lePwd_.text.isEmpty() ||
        _i.lePro_.text.isEmpty()) {
      sys.errorMsgBox(sys.translate("Debe indicar usuario, contraseña y proyecto"));
      return;
    }

    var auth = _i.rpc_.callScriptFunction(
                 "authUser", [_i.leUsr_.text, _i.lePwd_.text]
               ).toString();
    if (!auth || auth == "false" || auth == "invalid") {
      sys.errorMsgBox(sys.translate("Usuario o contraseña no válidos"));
      return;
    }

    var contract = _i.validContract();
    if (!contract || contract == "false" || contract == "invalid") {
      _i.compileRequest();
      return;
    }

    var cur = new AQSqlCursor("flfiles");
    if (!cur.select("nombre like '%.qs'"))
      return;

    _i.pbCompile_.setDisabled(true);
    _i.prCompile_.reset();
    _i.prCompile_.setTotalSteps(cur.size());
    var p = 0;
    var qsaError = false;
    while (cur.next()) {
      var fileName = cur.valueBuffer("nombre");
      _i.lbCompile_.text = String("Compilando %1").arg(fileName);
      _i.prCompile_.setProgress(++p);
      qsaError = !_i.compileQsa(cur);
      if (qsaError)
        break;
    }
    _i.prCompile_.reset();
    _i.lbCompile_.text = qsaError
                         ? sys.translate("Compilación terminada con errores")
                         : sys.translate("Compilación terminada sin errores");
    _i.pbCompile_.setDisabled(false);
  }

  function compileQsa(cur)
  {
    var _i = this;

    cur.setModeAccess(AQSql.Edit);
    cur.refreshBuffer();

    var name = cur.valueBuffer("nombre");
    var sha = cur.valueBuffer("sha");
    var key = String(name).left(30);
    var shaLast = AQUtil.readDBSettingEntry(key);
    if (!shaLast || shaLast != sha)
      AQUtil.writeDBSettingEntry(key, sha);
    else if (shaLast == sha)
      return true;

    var code = new QByteArray;
    code.string = cur.valueBuffer("contenido").toString();
    var byteCode = _i.rpc_.callScriptFunction(
                     "compileQsa",
                     [_i.leUsr_.text, _i.lePwd_.text, _i.lePro_.text,
                      name, AQS.toBase64(code).toString()]
                   );

    if (!byteCode || byteCode.toString() == "false" || byteCode.toString() == "invalid")
      return false;

    if (byteCode.toString().startsWith("## ERROR ##")) {
      sys.errorMsgBox(byteCode.toString());
      return false;
    }

    cur.setValueBuffer("binario", AQS.fromBase64(byteCode));
    if (!cur.commitBuffer()) {
      sys.errorMsgBox(String("Fallo al guardar el registro para %1").arg(name));
      return false;
    }

    return true;
  }

  function showChangesUpdate(id)
  {
    var _i = this;

    var xml = _i.rpc_.callScriptFunction(
                "xmlChangesUpdate",
                [_i.leUsr_.text, _i.lePwd_.text, _i.lePro_.text, id]
              ).toString();

    if (!xml || xml == "false" || xml == "invalid")
      return;

    var diag = new QDialog;
    diag.caption = "AbanQ";
    diag.modal = true;
    var lay = new QHBoxLayout(diag);
    var spl = new QSplitter(AQS.Vertical, diag);
    buildListViewChangesModules(xml, spl);
    buildListViewChangesExtensions(xml, spl);
    lay.addWidget(spl);
    diag.resize(800, 600);

    diag.exec();
  }

  function showMsg(msg)
  {
    var _i = this;

    if (msg.isEmpty())
      return;
    MessageBox.information(_i.wordwrap(msg),
                           MessageBox.Ok, MessageBox.NoButton,
                           MessageBox.NoButton, "AbanQ");
  }

  function updateUpdates()
  {
    var _i = this;
    var w = _i.w_;

    var xml = _i.rpc_.callScriptFunction(
                "xmlUpdatesProject",
                [_i.leUsr_.text, _i.lePwd_.text, _i.lePro_.text]
              ).toString();

    if (_i.tbUpdates_ != undefined) {
      _i.tbUpdates_.close();
      _i.tbUpdates_.deleteLater();
      _i.tbUpdates_ = undefined;
    }

    if (!xml || xml == "false" || xml == "invalid")
      return;

    _i.lbUpt_.text = sys.translate("Actualizaciones Disponibles");
    _i.tbUpdates_ = _i.buildTableUpdates(
                      xml, w.child("frUpdates")
                    );
    _i.tbUpdates_.show();
    _i.layUpdates_.addWidget(_i.tbUpdates_);
  }

  function clearChangelogLayers()
  {
    var _i = this;

    if (_i.lwModsChangelog_ != undefined) {
      _i.lwModsChangelog_.close();
      _i.lwModsChangelog_.deleteLater();
      _i.lwModsChangelog_ = undefined;
    }

    if (_i.lwExtsChangelog_ != undefined) {
      _i.lwExtsChangelog_.close();
      _i.lwExtsChangelog_.deleteLater();
      _i.lwExtsChangelog_ = undefined;
    }

    if (_i.splChangelog_ != undefined) {
      _i.splChangelog_.close();
      _i.splChangelog_.deleteLater();
      _i.splChangelog_ = undefined;
    }
  }

  function updateChangelogMods()
  {
    var _i = this;
    var w = _i.w_;

    var mng = aqApp.db().managerModules();
    var mods = mng.listAllIdModules();
    var minVer = AQUtil.sqlSelect("flmodules", "min(version)", "version>'2.2'");
    if (!minVer)
      minVer = "2.2";
    mods.push(minVer + "@");
    var xml = _i.rpc_.callScriptFunction(
                "xmlChangesModulesTrunk", mods
              ).toString();

    _i.clearChangelogLayers();

    _i.lwModsChangelog_ = _i.buildListViewChangesModules(
                            xml, w.child("frChangelog")
                          );
    _i.lwModsChangelog_.show();
    _i.layChangelog_.addWidget(_i.lwModsChangelog_);
  }

  function updateChangelogExts()
  {
    var _i = this;
    var w = _i.w_;

    var minVer = AQUtil.sqlSelect("flmodules", "min(version)", "version>'1.0'");
    if (!minVer)
      minVer = "2.0";

    var xml = _i.rpc_.callScriptFunction(
                "xmlChangesProjectTrunk",
                [_i.leUsr_.text, _i.lePwd_.text, _i.lePro_.text, minVer]
              ).toString();

    if (!xml || xml == "false" || xml == "invalid") {
      _i.lbUpt_.text = sys.translate("Cree una cuenta de actualizaciones o introduzca una válida para poder instalar las mejoras");
      _i.updateChangelogMods();
      return;
    }

    _i.lbUpt_.text = sys.translate("No hay actualizaciones disponibles");
    _i.clearChangelogLayers();

    _i.splChangelog_ = new QSplitter(AQS.Horizontal, w.child("frChangelog"));
    _i.lwModsChangelog_ = _i.buildListViewChangesModules(
                            xml, _i.splChangelog_
                          );
    _i.lwExtsChangelog_ = _i.buildListViewChangesExtensions(
                            xml, _i.splChangelog_
                          );
    _i.splChangelog_.show();
    _i.layChangelog_.addWidget(_i.splChangelog_);
  }

  function downloadClientUi()
  {
    var _i = this;

    var remoteSha = _i.rpc_.callScriptFunction(
                      "shaOfFile", ["client_diag.ui"]
                    ).toString();
    if (!remoteSha || remoteSha == "false" || remoteSha == "invalid")
      return false;
    var mng = aqApp.db().managerModules();
    var localSha = mng.shaOfFile("client_diag.ui");
    if (localSha != remoteSha) {
      xmlUi = _i.rpc_.callScriptFunction("clientDiagUi").toString();
      if (!xmlUi || xmlUi == "false" || xmlUi == "invalid")
        return false;
      mng.setContent("client_diag.ui", "sys", xmlUi);
      mng.loadKeyFiles();
    }
    return true;
  }

  function buildTableUpdates(xmlString, parentWidget)
  {
    var _i = this;

    var tb = new QTable(parentWidget);
    tb.numCols = 7;
    tb.setColumnLabels(["",
                        sys.translate("Fecha"),
                        sys.translate("Hora"),
                        sys.translate("Descripción"),
                        sys.translate("Desde Versión"),
                        sys.translate("Hasta Versión"), ""]);
    tb.readOnly = true;
    tb.showGrid = false;
    tb.verticalHeader().hide();

    var doc = new QDomDocument;
    if (!doc.setContent(xmlString))
      return tb;

    var root = doc.firstChild();
    var rows = root.childNodes();
    var installed = false;

    tb.numRows = rows.length();

    for (var i = 0; i < rows.length(); ++i) {
      var it = rows.item(i).toElement();

      var row = {
        fecha:        it.attribute("fechaupdate"),
        desde:        it.attribute("versiondesde"),
        hasta:        it.attribute("versionhasta"),
        id:           it.attribute("idactualizacion"),
        hora:         it.attribute("horaupdate"),
        des:          it.attribute("desrevision"),
        esrev:        it.attribute("esrevision"),
        libre:        it.attribute("libre"),
        filesdef:     it.attribute("filesdef")
      }

      tb.setText(i, 1, row.fecha);
      tb.setText(i, 2, row.hora);
      tb.setText(i, 3, row.des);
      tb.setText(i, 4, row.desde);
      tb.setText(i, 5, row.hasta);

      if (!installed && !_i.isFilesDefInstalled(row.filesdef)) {
        var pbIns = new QPushButton("Instalar", tb);
        if (row.libre == "false")
          pbIns.name = "instalarUpdate:" + row.id + ":" + row.filesdef;
        else
          pbIns.name = "instalarUpdateDirect:" + row.id + ":" + row.filesdef;
        pbIns.flat = true;
        connect(pbIns, "clicked()", _i.sigMap_, "map()");
        _i.sigMap_.setMapping(pbIns, pbIns.name);
        tb.setCellWidget(i, 6, pbIns);
      } else {
        tb.setText(i, 6, sys.translate("Ya instalada"));
        installed = true;
      }

      var pbChg = new QPushButton("Ver Cambios", tb);
      if (row.esrev == "false")
        pbChg.name = "verUpdate:" + row.id;
      else
        pbChg.name = "showMsg:" + row.des;
      pbChg.flat = true;
      connect(pbChg, "clicked()", _i.sigMap_, "map()");
      _i.sigMap_.setMapping(pbChg, pbChg.name);
      tb.setCellWidget(i, 0, pbChg);
    }

    return tb;
  }

  function buildListViewChangesModules(xmlString, parentWidget)
  {
    var _i = this;

    var lw = new QListView(parentWidget);
    lw.addColumn("");
    lw.addColumn("");
    lw.setSorting(-1);
    lw.rootIsDecorated = true;
    lw.setColumnWidth(1, 0);
    lw.setColumnWidthMode(1, 0);
    lw.hideColumn(1);
    lw.header().moving = false;
    lw.header().setResizeEnabled(false, 0);
    lw.header().setResizeEnabled(false, 1);
    lw.header().setClickEnabled(false, 0);
    lw.header().setClickEnabled(false, 1);
    lw.header().setLabel(0, "Módulos");
    lw.itemMargin = 4;

    var doc = new QDomDocument;
    if (!doc.setContent(xmlString))
      return lw;

    var mng = aqApp.db().managerModules();
    var visitedMods = [];
    var visitedAreas = [];
    var root = doc.firstChild();
    var rows = root.childNodes();

    for (var i = 0; i < rows.length(); ++i) {
      var it = rows.item(i).toElement();

      var row = {
        idarea:       it.attribute("idarea"),
        idmodulo:     it.attribute("mv_changelog.idmodulo"),
        fecha:        it.attribute("mv_changelog.fecha"),
        descripcion:  it.attribute("mv_changelog.descripcion"),
        cambios:      it.attribute("cambios")
      }

      if (row.idarea.isEmpty() || row.idmodulo.isEmpty())
        continue;

      var itemArea;
      if (!(row.idarea in visitedAreas)) {
        itemArea = new QListViewItem(lw);
        itemArea.setPixmap(0, AQS.Pixmap_fromMimeSource("folder.png"));
        itemArea.setText(0, mng.idAreaToDescription(row.idarea));
        itemArea.setText(1, row.idarea);
        visitedAreas[row.idarea] = itemArea;
      } else
        itemArea = visitedAreas[row.idarea];

      var itemMod;
      if (!(row.idmodulo in visitedMods)) {
        itemMod = new QListViewItem(itemArea);
        itemMod.setPixmap(0, _i.pixmap16x16(mng.iconModule(row.idmodulo)));
        itemMod.setText(0, mng.idModuleToDescription(row.idmodulo));
        itemMod.setText(1, row.idmodulo);
        visitedMods[row.idmodulo] = itemMod;
      } else
        itemMod = visitedMods[row.idmodulo];

      var itemDes = new QListViewItem(itemMod);
      itemDes.setText(0, row.fecha + " : " + row.descripcion);
      if (!row.cambios.isEmpty()) {
        var itemCam = new QListViewItem(itemDes);
        itemCam.setMultiLinesEnabled(true);
        itemCam.setText(0, _i.wordwrap(row.cambios));
      }
    }

    return lw;
  }

  function buildListViewChangesExtensions(xmlString, parentWidget)
  {
    var _i = this;

    var lw = new QListView(parentWidget);
    lw.addColumn("");
    lw.addColumn("");
    lw.setSorting(-1);
    lw.rootIsDecorated = true;
    lw.setColumnWidth(1, 0);
    lw.setColumnWidthMode(1, 0);
    lw.hideColumn(1);
    lw.header().moving = false;
    lw.header().setResizeEnabled(false, 0);
    lw.header().setResizeEnabled(false, 1);
    lw.header().setClickEnabled(false, 0);
    lw.header().setClickEnabled(false, 1);
    lw.header().setLabel(0, "Extensiones");
    lw.itemMargin = 4;

    var doc = new QDomDocument;
    if (!doc.setContent(xmlString))
      return lw;

    var visitedExts = [];
    var root = doc.firstChild();
    var rows = root.childNodes();

    for (var i = 0; i < rows.length(); ++i) {
      var it = rows.item(i).toElement();

      var row = {
        idext:        it.attribute("mv_changelog.codfuncional"),
        desccorta:    it.attribute("desccorta"),
        fecha:        it.attribute("mv_changelog.fecha"),
        descripcion:  it.attribute("mv_changelog.descripcion"),
        cambios:      it.attribute("cambios")
      }

      if (row.idext.isEmpty())
        continue;

      var itemExt;
      if (!(row.idext in visitedExts)) {
        itemExt = new QListViewItem(lw);
        itemExt.setPixmap(0, AQS.Pixmap_fromMimeSource("folder.png"));
        itemExt.setText(0, row.desccorta);
        itemExt.setText(1, row.idext);
        visitedExts[row.idext] = itemExt;
      } else
        itemExt = visitedExts[row.idext];

      var itemDes = new QListViewItem(itemExt);
      itemDes.setText(0, row.fecha + " : " + row.descripcion);
      if (!row.cambios.isEmpty()) {
        var itemCam = new QListViewItem(itemDes);
        itemCam.setMultiLinesEnabled(true);
        itemCam.setText(0, _i.wordwrap(row.cambios));
      }
    }

    return lw;
  }

  function wordwrap(str, len)
  {
    len = len || 50;

    while (str.endsWith('\n'))
      str = str.left(str.length - 1);

    if (!str.startsWith('\n') && str.indexOf('\n') != -1) {
      var breaks = str.split('\n');
      for (var i = 0; i < breaks.length; ++i)
        breaks[i] = wordwrap(breaks[i], len);
      return breaks.join('\n');
    }

    var stringWords = str.split(' ');
    var lineLen = 0;

    for (var i = 0; i < stringWords.length; ++i) {
      lineLen += stringWords[i].length;
      if (lineLen > len) {
        stringWords[i] = stringWords[i] + '\n';
        lineLen = 0;
      }
    }

    var ret = "";
    for (var i = 0; i < stringWords.length; ++i) {
      ret += stringWords[i];
      if (!stringWords[i].endsWith('\n') && i < stringWords.length - 1)
        ret += ' ';
    }
    while (ret.endsWith('\n'))
      ret = ret.left(str.length - 1);
    return ret;
  }

  function pixmap16x16(pix)
  {
    var pix = new QPixmap(pix);
    var img = pix.convertToImage();
    img = img.smoothScale(16, 16);
    return new QPixmap(img);
  }
}

function aqUpdaterMain24()
{
  var updater = new AbanQUpdaterDialog;
  updater = undefined;
}

