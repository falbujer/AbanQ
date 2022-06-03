/***************************************************************************
 AQRoller.cpp
 -------------------
 begin                : 21/01/2012
 copyright            : (C) 2003-2012 by InfoSiAL S.L.
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

#include <stdio.h>

#include <AQLexer.h>

#include "AQRoller.h"
#include "AQNodes_p.h"
#include "AQStyle.h"

extern int aqryyparse();

QStringList AQRoller::logMsgs_;
AQNodeList  AQRootNode::nodes_;
AQRootNode *AQRootNode::root_ = 0;

#define AQROLLER_LOG(M)  \
  fprintf(stderr, "%s", M.latin1()); \
  AQRoller::logMsgs_ << M

#define AQ_CUSTOM_CLASS_NAME "CustomChanges"

static inline AQClassNode *firstBaseClass(AQClassNode *c)
{
  if (!c || c->base_.isEmpty())
    return c;
  if (c->name_ == c->base_ || c->name_ == AQ_CUSTOM_CLASS_NAME)
    return c;
  AQClassNode *b = AQRootNode::root_->findClass(c->base_);
  if (!b)
    return c;
  return firstBaseClass(b);
}

static inline void markInternaCtx()
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList l;
  root->queryNodes(AQNode::Class, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQClassNode *c = CAST_N(Class, it);
    if (c->name_ != "interna")
      continue;
    AQVarNode *ctx = c->findVar("ctx");
    if (!ctx)
      continue;
    c->isInternaCtx_ = true;
    c->inheritsInternaCtx_ = true;
    break;
  }
}

static inline void markInternaCtxChain()
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList l;
  root->queryNodes(AQNode::Class, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQClassNode *c = CAST_N(Class, it);
    if (c->inheritsInternaCtx_)
      continue;
    AQClassNode *b = firstBaseClass(c);
    if (!b || !b->isInternaCtx_)
      continue;
    c->inheritsInternaCtx_ = true;
  }
}

static inline void joinToClass(AQClassNode *from, AQClassNode *to,
                               const QString &fileName)
{
  if (to->join_)
    return joinToClass(from, to->join_, fileName);
  from->join_ = to;
  for (AQNode *it = from->nodes().first(); it;) {
    if (it->type() == AQNode::Function &&
        CAST_N(Function, it)->name_ == from->name_) {
      it = from->nodes().next();
      continue;
    }
    AQNode *n = from->nodes().take();
    if (n->type() == AQNode::VarConst) {
      AQVarNode *v = CAST_N(Var, n);
      AQVarNode *otherV = to->findVar(v->name_);
      if (otherV) {
        AQROLLER_LOG(
          QString().sprintf(
            "Warn %s Lines [%d,%d] : La clase '%s' declara la VARIABLE MIEMBRO '%s' "
            "pero una de sus clases padre ya ha declarado una variable con el mismo "
            "nombre. * SE VA A ELIMINAR LA VARIABLE PADRE Y SÓLO SE TIENE EN CUENTA "
            "LA ÚLTIMA VARIABLE DECLARADA *\n",
            fileName.latin1(),
            otherV->begin_.first_line - 1,
            v->begin_.first_line - 1,
            from->name_.latin1(),
            v->name_.latin1())
        );
        to->nodes().removeRef(otherV);
      }
    }
    to->nodes().append(n);
    it = from->nodes().current();
  }
  from->nodes().clear();
}

static inline bool joinClasses(const QString &fileName)
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList l;
  root->queryNodes(AQNode::Class, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQClassNode *n = CAST_N(Class, it);
    if (n->base_.isEmpty())
      continue;

    if (n->name_ == n->base_) {
      AQROLLER_LOG(
        QString().sprintf(
          "Error %s Line %d : La clase '%s' hereda de ella misma\n",
          fileName.latin1(),
          n->begin_.first_line - 1,
          n->name_.latin1())
      );
      return false;
    }

    AQClassNode *b = root->findClass(n->base_);
    if (!b) {
      AQROLLER_LOG(
        QString().sprintf(
          "Error %s Line %d : La clase '%s' hereda de la clase '%s' que no existe\n",
          fileName.latin1(),
          n->begin_.first_line,
          n->name_.latin1(),
          n->base_.latin1())
      );
      return false;
    }

    if (n->inheritsInternaCtx_)
      joinToClass(n, root->findClass(n->base_), fileName);
  }
  return true;
}

static inline bool updateNews()
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList l;
  root->queryNodes(AQNode::New, l);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQNewNode *n = CAST_N(New, it);
    AQClassNode *c = root->findClass(n->name_);
    if (!c)
      continue;
    while (c->join_)
      c = c->join_;
    n->name_ = c->name_;
  }
  return true;
}

static inline void addCtxToThisNodes(AQNode *n)
{
  AQNodeList l;
  n->queryNodes(AQNode::This, l);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQThisNode *t = CAST_N(This, it);
    t->addCtx_ = true;
  }
}

static inline void updateCalls(const QString &oldFunName,
                               const QString &newFunName)
{
  AQNodeList l;
  AQRootNode::root_->queryNodes(AQNode::Call, l);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQCallNode *c = CAST_N(Call, it);
    if (c->name_ == oldFunName)
      c->name_ = newFunName;
  }
}

static inline bool replaceFunctions(const QString &fileName)
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList classList;
  root->queryNodes(AQNode::Class, classList, false);
  AQNodeList updatesFun;
  int serialFun = 0;

  for (AQNode *cl = classList.first(); cl; cl = classList.next()) {
    if (!CAST_N(Class, cl)->inheritsInternaCtx_)
      continue;

    AQNodeList funList;
    cl->queryNodes(AQNode::Function, funList, false);

    for (AQNode *fn = funList.first(); fn; fn = funList.next()) {
      AQNodeList callList;
      fn->queryNodes(AQNode::Call, callList);
      if (callList.count() != 1)
        continue;

      AQCallNode *call = CAST_N(Call, callList.getFirst());
      AQNodeList callNodes(call->nodesWithOutJumps());

      if (callNodes.count() < 3)
        continue;

      AQNode *n = callNodes.at(0);
      if (n->type() != AQNode::This)
        continue;

      n = callNodes.at(1);
      if (n->type() != AQNode::Accessor ||
          CAST_N(Accessor, n)->name_ != "ctx")
        continue;

      n = callNodes.at(2);
      if (n->type() != AQNode::Accessor ||
          CAST_N(Accessor, n)->name_ != call->name_)
        continue;

      AQNode *rootFun = root->findFunction(call->name_, true);
      if (!rootFun) {
        AQROLLER_LOG(
          QString().sprintf(
            "Warn %s Line %d : Hay una llamada a una funcion que no existe: '%s'\n",
            fileName.latin1(),
            call->begin_.first_line,
            call->name_.latin1())
        );
        //fn->skip_ = true;
        continue;
      }

      AQFunctionNode *rf = CAST_N(Function, rootFun);
      if (rf->name_ == rf->origName_) {
        AQFunctionNode *f = CAST_N(Function, fn);
        QString newFunName(QString::fromLatin1("F") + QString::number(serialFun++));
        call->name_ = newFunName;
        rf->name_ = newFunName;
      } else
        call->name_ = rf->name_;

      if (!updatesFun.containsRef(rf))
        updatesFun.append(rf);

      //      fn->nodes().clear();
      //      fn->nodes() = rootFun->nodes();
      //      CAST_N(Function, fn)->params_ = CAST_N(Function, rootFun)->params_;
      //      rootFun->nodes().setAutoDelete(false);
      //      rootFun->nodes().clear();
      //      rootFun->nodes().setAutoDelete(true);
      //      addCtxToThisNodes(fn);
    }
  }

  for (AQNode *it = updatesFun.first(); it; it = updatesFun.next()) {
    AQFunctionNode *f = CAST_N(Function, it);
    updateCalls(f->origName_, f->name_);
  }

  return true;
}

static inline void clearVoidClassesAndFunctions()
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList l;
  root->queryNodes(AQNode::Class, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    if (it->nodes().isEmpty())
      AQRootNode::nodes_.removeRef(it);
  }

  root->queryNodes(AQNode::Function, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    if (it->nodes().isEmpty())
      AQRootNode::nodes_.removeRef(it);
  }
}

static inline void checkDuplicateVarNames(AQNode *n, const QString &fileName)
{
  if (!n || !n->supportNodes())
    return;
  QStringList names;
  AQNodeList l;
  n->queryNodes(AQNode::VarConst, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQVarNode *i = CAST_N(Var, it);
    if (names.contains(i->name_)) {
      if (n->type() == AQNode::Class) {
        AQROLLER_LOG(
          QString().sprintf(
            "Warn %s Line %d : Hay definida mas de una VARIABLE con el mismo nombre: '%s', dentro de la clase '%s'\n",
            fileName.latin1(),
            i->begin_.first_line,
            i->name_.latin1(),
            CAST_N(Class, n)->name_.latin1())
        );
      } else {
        AQROLLER_LOG(
          QString().sprintf(
            "Warn %s Line %d : Hay definida mas de una VARIABLE con el mismo nombre: '%s'\n",
            fileName.latin1(),
            i->begin_.first_line,
            i->name_.latin1())
        );
      }
      continue;
    }
    names.append(i->name_);
  }
}

static inline void checkDuplicateFunNames(AQNode *n, const QString &fileName)
{
  if (!n || !n->supportNodes())
    return;
  QStringList names;
  AQNodeList l;
  n->queryNodes(AQNode::Function, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQFunctionNode *i = CAST_N(Function, it);
    if (names.contains(i->name_)) {
      if (n->type() == AQNode::Class) {
        AQROLLER_LOG(
          QString().sprintf(
            "Warn %s Line %d : Hay definida mas de una FUNCION con el mismo nombre: '%s', dentro de la clase '%s'\n",
            fileName.latin1(),
            i->begin_.first_line,
            i->name_.latin1(),
            CAST_N(Class, n)->name_.latin1())
        );
      } else {
        AQROLLER_LOG(
          QString().sprintf(
            "Warn %s Line %d : Hay definida mas de una FUNCION con el mismo nombre: '%s'\n",
            fileName.latin1(),
            i->begin_.first_line,
            i->name_.latin1())
        );
      }
      continue;
    }
    names.append(i->name_);
  }
}

static inline void checkDuplicateClassNames(AQNode *n, const QString &fileName)
{
  if (!n || !n->supportNodes())
    return;
  QStringList names;
  AQNodeList l;
  n->queryNodes(AQNode::Class, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    AQClassNode *i = CAST_N(Class, it);
    if (names.contains(i->name_)) {
      AQROLLER_LOG(
        QString().sprintf(
          "Warn %s Line %d : Hay definida mas de una CLASE con el mismo nombre: '%s'\n",
          fileName.latin1(),
          i->begin_.first_line,
          i->name_.latin1())
      );
      continue;
    }
    names.append(i->name_);
  }
}

static inline void checkDuplicateNames(const QString &fileName)
{
  AQRootNode *root = AQRootNode::root_;

  checkDuplicateVarNames(root, fileName);
  checkDuplicateFunNames(root, fileName);
  checkDuplicateClassNames(root, fileName);

  AQNodeList l;
  root->queryNodes(AQNode::Class, l, false);
  for (AQNode *it = l.first(); it; it = l.next()) {
    checkDuplicateVarNames(it, fileName);
    checkDuplicateFunNames(it, fileName);
  }
}

static inline QString firstBaseFunName(AQClassNode *c, const QString &funName)
{
  if (!c || c->base_.isEmpty() || funName.isEmpty())
    return QString::null;
  if (c->name_ == c->base_ || c->name_ == AQ_CUSTOM_CLASS_NAME)
    return QString::null;
  AQClassNode *b = AQRootNode::root_->findClass(c->base_);
  if (!b)
    return QString::null;
  AQFunctionNode *f = b->findFunction(funName, true);
  if (f)
    return b->name_;
  return firstBaseFunName(b, funName);
}

static inline bool hasFuntionCall(AQFunctionNode *f, const QString &callName)
{
  if (!f || callName.isEmpty())
    return false;

  AQNodeList l;
  f->queryNodes(AQNode::Accessor, l, true);
  for (AQNode *ac = l.first(); ac; ac = l.next()) {
    if (CAST_N(Accessor, ac)->name_ == callName)
      return true;
  }
  return false;
}

static inline bool setImplFunctions(const QString &fileName)
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList classList;
  root->queryNodes(AQNode::Class, classList, false);

  for (AQNode *cl = classList.first(); cl; cl = classList.next()) {
    if (!CAST_N(Class, cl)->inheritsInternaCtx_)
      continue;

    AQNodeList rootsFun;
    AQNodeList targetsFun;
    AQNodeList funList;
    cl->queryNodes(AQNode::Function, funList, false);

    for (AQNode *fn = funList.first(); fn; fn = funList.next()) {
      AQNodeList callList;
      fn->queryNodes(AQNode::Call, callList);
      if (callList.count() != 1)
        continue;

      AQCallNode *call = CAST_N(Call, callList.getFirst());
      AQNodeList callNodes(call->nodesWithOutJumps());

      if (callNodes.count() < 3)
        continue;

      AQNode *n = callNodes.at(0);
      if (n->type() != AQNode::This)
        continue;

      n = callNodes.at(1);
      if (n->type() != AQNode::Accessor ||
          CAST_N(Accessor, n)->name_ != "ctx")
        continue;

      n = callNodes.at(2);
      if (n->type() != AQNode::Accessor ||
          CAST_N(Accessor, n)->name_ != call->name_)
        continue;

      AQNode *rootFun = root->findFunction(call->name_, true);
      if (!rootFun) {
        //fn->skip_ = true;
        continue;
      }

      AQFunctionNode *rf = CAST_N(Function, rootFun);
      AQFunctionNode *targetFun = CAST_N(Function, fn);
      targetFun->implFunName_ = rf->name_;
      targetFun->baseClassImplName_ = firstBaseFunName(CAST_N(Class, cl), targetFun->name_);
      targetFun->callBaseFun_ = hasFuntionCall(rf, "__" + targetFun->name_);

      if (!rootsFun.containsRef(rf))
        rootsFun.append(rf);
      if (!targetsFun.containsRef(targetFun))
        targetsFun.append(targetFun);
    }

    for (AQNode *it = targetsFun.first(); it; it = targetsFun.next()) {
      AQFunctionNode *f = CAST_N(Function, it);
      if (f->callBaseFun_)
        continue;
      for (AQNode *it2 = rootsFun.first(); it2; it2 = rootsFun.next()) {
        AQFunctionNode *f2 = CAST_N(Function, it2);
        bool callBase = hasFuntionCall(f2, "__" + f->name_);
        if (callBase) {
          f->callBaseFun_ = true;
          break;
        }
      }
    }
  }

  return true;
}

static inline void skipClassFun(const QString &className, const QString &funName)
{
  if (className.isEmpty() || funName.isEmpty())
    return;
  AQClassNode *c = AQRootNode::root_->findClass(className);
  if (!c)
    return;
  AQFunctionNode *f = c->findFunction(funName, true);
  if (!f)
    return;
  f->skip_ = true;
  f->callBaseFun_ = false;
  if (!f->baseClassImplName_.isEmpty())
    skipClassFun(f->baseClassImplName_, funName);
  if (!f->implFunName_.isEmpty()) {
    f = AQRootNode::root_->findFunction(f->implFunName_);
    if (f)
      f->skip_ = true;
  }
}

static inline bool skipBaseFunctionsNotCalled(const QString &fileName)
{
  AQRootNode *root = AQRootNode::root_;

  AQNodeList classList;
  root->queryNodes(AQNode::Class, classList, false);

  for (AQNode *cl = classList.first(); cl; cl = classList.next()) {
    if (!CAST_N(Class, cl)->inheritsInternaCtx_)
      continue;

    AQNodeList funList;
    cl->queryNodes(AQNode::Function, funList, false);

    for (AQNode *fn = funList.first(); fn; fn = funList.next()) {
      AQFunctionNode *targetFun = CAST_N(Function, fn);
      if (!targetFun->baseClassImplName_.isEmpty() && !targetFun->callBaseFun_)
        skipClassFun(targetFun->baseClassImplName_, targetFun->name_);
    }
  }

  return true;
}

static inline bool hasSkippedCode()
{
  AQNodeList l;
  AQRootNode::root_->queryNodes(AQNode::Any, l, true);
  for (AQNode *it = l.first(); it; it = l.next()) {
    if (it->skip_)
      return true;
  }
  return false;
}

QString AQRoller::roll(const QString &code, const QString &fileName)
{
  if (code.isEmpty())
    return QString::null;

  logMsgs_.clear();

  QChar sep('\n');
  AQStyle bty;

  QString ops(bty.defaultAsOps());
  ops += sep;
  ops += "no-comments";

  QString copyCode(bty.beautify(code, ops));
  QString markVersion(QString("///// %1\n").arg(AQROLLER_VERSION));

  if (!copyCode.startsWith(markVersion.mid(1)))
    copyCode.prepend(markVersion);

  AQRLexer lex;
  lex.setCode(copyCode, 0, 0, 0, true);
  int parseError = aqryyparse();

  if (parseError || lex.lexerState() == AQRLexer::Bad) {
    AQROLLER_LOG(
      QString().sprintf(
        "Parse Error %s Line %d : %s\n",
        fileName.latin1(),
        lex.lineNo(),
        lex.errorMessage().latin1())
    );
    return code;
  }

  if (!AQRootNode::root_)
    return code;

  markInternaCtx();
  markInternaCtxChain();

  checkDuplicateNames(fileName);

  QString retCode;
  bool ok = setImplFunctions(fileName);

  if (ok)
    ok = skipBaseFunctionsNotCalled(fileName);

  if (ok)
    ok = joinClasses(fileName);

  if (ok)
    ok = updateNews();

  if (ok)
    ok = replaceFunctions(fileName);

  if (ok) {
    clearVoidClassesAndFunctions();
    QStringList srcCode(QStringList::split(sep, copyCode, true));
    AQRootNode::root_->dumpCode(srcCode, retCode);
  }

  //if (ok)
  //  AQRootNode::root_->dumpDebug(0);
  //printf("%s\n", retCode.latin1());

  if (!AQRootNode::root_->findClass(AQ_CUSTOM_CLASS_NAME)) {
    retCode += sep;
    retCode += "/***********************************************************************";
    retCode += sep;
    retCode += "**";
    retCode += sep;
    retCode += "** Para añadir cambios personalizados se debe heredar de la clase";
    retCode += sep;
    retCode += "** '";
    retCode += AQ_CUSTOM_CLASS_NAME;
    retCode += "'";
    retCode += " e implementar esos cambios en las clases derivadas.";
    retCode += sep;
    retCode += "**";
    retCode += sep;
    retCode += "** ¡¡AVISO!! Todos cambios realizados fuera de la clase ";
    retCode += AQ_CUSTOM_CLASS_NAME;
    retCode += sep;
    retCode += "** y que modifiquen la clase interna con la versión oficial,";
    retCode += sep;
    retCode += "** se perderán en posteriores actualizaciones ofrecidas por InfoSiAL.";
    retCode += sep;
    retCode += "**";
    retCode += sep;
    retCode += "************************************************************************/";
    retCode += sep;
    retCode += "class ";
    retCode += AQ_CUSTOM_CLASS_NAME;
    retCode += " extends interna { function ";
    retCode += AQ_CUSTOM_CLASS_NAME;
    retCode += "(context) { interna(context); } }";
    retCode += sep;
  }

  delete AQRootNode::root_;
  AQRootNode::nodes_.clear();

  ops = bty.defaultAsOps();
  ops += sep;
  ops += "delete-empty-lines";

  QStringList l(QStringList::split(sep, retCode));
  retCode = QString::null;
  for (QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
    QString line((*it).stripWhiteSpace());
    if (!line.isEmpty())
      retCode.append(line + sep);
  }

  return bty.beautify(retCode, ops);
}

QString AQRoller::rollCleaner(const QString &code, const QString &fileName)
{
  if (code.isEmpty())
    return QString::null;

  logMsgs_.clear();

  QChar sep('\n');
  AQStyle bty;

  QString ops(bty.defaultAsOps());
  ops += sep;
  ops += "no-comments";

  QString copyCode(bty.beautify(code, ops));
  QString markVersion(QString("///// %1\n").arg(AQROLLER_VERSION));

  if (!copyCode.startsWith(markVersion.mid(1)))
    copyCode.prepend(markVersion);

  AQRLexer lex;
  lex.setCode(copyCode, 0, 0, 0, true);
  int parseError = aqryyparse();

  if (parseError || lex.lexerState() == AQRLexer::Bad) {
    AQROLLER_LOG(
      QString().sprintf(
        "Parse Error %s Line %d : %s\n",
        fileName.latin1(),
        lex.lineNo(),
        lex.errorMessage().latin1())
    );
    return code;
  }

  if (!AQRootNode::root_)
    return code;

  markInternaCtx();
  markInternaCtxChain();

  QString retCode;
  bool ok = setImplFunctions(fileName);

  if (ok)
    ok = skipBaseFunctionsNotCalled(fileName);

  if (ok && !hasSkippedCode()) {
    delete AQRootNode::root_;
    AQRootNode::nodes_.clear();
    return code;
  }

  if (ok) {
    clearVoidClassesAndFunctions();
    QStringList srcCode(QStringList::split(sep, copyCode, true));
    AQRootNode::root_->dumpCode(srcCode, retCode);
  }

  //if (ok)
  //  AQRootNode::root_->dumpDebug(0);
  //printf("%s\n", retCode.latin1());

  delete AQRootNode::root_;
  AQRootNode::nodes_.clear();

  ops = bty.defaultAsOps();
  ops += sep;
  ops += "delete-empty-lines";

  QStringList l(QStringList::split(sep, retCode));
  retCode = QString::null;
  for (QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
    QString line((*it).stripWhiteSpace());
    if (!line.isEmpty())
      retCode.append(line + sep);
  }

  return bty.beautify(retCode, ops);
}
