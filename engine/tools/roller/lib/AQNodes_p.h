/***************************************************************************
 AQNodes_p.h
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

#ifndef AQNODES_P_H_
#define AQNODES_P_H_

#include <qptrlist.h>
#include <qstringlist.h>

#include "AQGrammar.h"

#define ADD_N(N) AQRootNode::addNode(N)
#define CAST_N(N,P) static_cast<AQ##N##Node *>(P)

class AQNode;
class AQClassNode;
class AQFunctionNode;
class AQVarNode;
class AQJumpNode;

typedef QPtrList<AQNode> AQNodeList;

static inline QString extracText(const QStringList &lines,
                                 int firstLine, int firstCol,
                                 int lastLine, int lastCol)
{
  if (firstLine < 1 || lastLine < 1 ||
      firstCol < 0 || lastCol < 0 || firstLine > lastLine)
    return QString::null;

  if (firstLine == lastLine) {
    if (firstCol > lastCol)
      return QString::null;
    QString line(lines[firstLine - 1]);
    if (lastCol > line.length())
      return line.mid(firstCol - 1) + '\n';
    return line.mid(firstCol - 1, lastCol - firstCol + 1);
  }

  QString ret;
  for (int i = firstLine - 1; i < lastLine; ++i) {
    QString line(lines[i]);
    if (i < lastLine - 1) {
      if (i == firstLine - 1)
        ret += line.mid(firstCol - 1) + '\n';
      else
        ret += line + '\n';
      continue;
    }
    if (lastCol == 0)
      continue;
    if (lastCol > line.length())
      ret += line + '\n';
    else
      ret += line.mid(0, lastCol);
  }
  return ret;
}

class AQNode
{
public:
  enum NodeType {
    Void,
    Jump,
    Root,
    Class,
    Function,
    VarConst,
    This,
    Accessor,
    Return,
    Call,
    New,
    Any
  };

  AQNode(const YYLTYPE &b, const YYLTYPE &e)
    : begin_(b),
      end_(e),
      skip_(false) {
    this->nodes().setAutoDelete(true);
  }

  virtual ~AQNode() {}

  virtual AQNode::NodeType type() const = 0;
  virtual bool supportNodes() const = 0;
  virtual void dumpDebug(int indent = 0) = 0;
  virtual void bindNodes();
  virtual void insertJumps();
  virtual void queryNodes(AQNode::NodeType t, AQNodeList &dstList, bool recur = true);
  virtual void dumpCode(const QStringList &srcCode, QString &dstCode);
  virtual AQNodeList nodesWithOutJumps();

  virtual AQNodeList &nodes() {
    return nodes_;
  }

  virtual YYLTYPE innerBegin() const {
    return begin_;
  }
  virtual YYLTYPE innerEnd() const {
    return end_;
  }

  YYLTYPE begin_;
  YYLTYPE end_;
  AQNodeList nodes_;
  bool skip_;

private:
  AQNode(const AQNode &);
  AQNode &operator=(const AQNode &);
};

class AQJumpNode : public AQNode
{
public:
  AQJumpNode(const YYLTYPE &b)
    : AQNode(b, b) {
  }

  AQNode::NodeType type() const {
    return AQNode::Jump;
  }

  bool supportNodes() const {
    return false;
  }

  void dumpDebug(int indent = 0) {
    //if (skip_)
    //  return;
    //qWarning("%*s" "%s [%d,%d] [%d,%d]", indent, " ", "Jump",
    //         begin_.first_line, begin_.first_column,
    //         begin_.last_line, begin_.last_column);
  }
};

class AQRootNode : public AQNode
{
public:
  AQRootNode()
    : AQNode( {1, 1, 1, 1}, aqryylloc) {
    root_ = this;
    bindNodes();
    insertJumps();
  }

  ~AQRootNode() {
    if (root_ = this)
      root_ = 0;
  }

  AQNode::NodeType type() const {
    return AQNode::Root;
  }

  bool supportNodes() const {
    return true;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s [%d,%d]", indent, " ", "Root Begin",
             begin_.first_line, begin_.first_column);
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpDebug(indent + 2);
    qWarning("%*s" "%s [%d,%d]", indent, " ", "Root End",
             end_.last_line, end_.last_column);
  }

  void bindNodes() {
    AQNodeList l(nodes_);
    for (AQNode *it = l.first(); it; it = l.next())
      it->bindNodes();
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpCode(srcCode, dstCode);

    //    for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
    //      if (it->type() != AQNode::Function)
    //        it->dumpCode(srcCode, dstCode);
    //    }
    //    AQNodeList l;
    //    queryNodes(AQNode::Function, l, false);
    //    l.sort();
    //    for (AQNode *it = l.first(); it; it = l.next()) {
    //      it->dumpCode(srcCode, dstCode);
    //    }
  }

  AQNodeList &nodes() {
    return nodes_;
  }

  inline AQClassNode *findClass(const QString &name);
  inline AQFunctionNode *findFunction(const QString &name, bool checkOrigName = false);
  inline AQVarNode *findVar(const QString &name, bool checkOrigName = false);

  static void addNode(AQNode *node) {
    Q_ASSERT(node);
    nodes_.append(node);
  }

  static AQNodeList nodes_;
  static AQRootNode *root_;
};

class AQClassNode : public AQNode
{
public:
  AQClassNode(const YYLTYPE &b, const YYLTYPE &e,
              const QString &name, const QString &base = QString::null)
    : AQNode(b, e),
      name_(name),
      base_(base),
      join_(0),
      isInternaCtx_(false),
      inheritsInternaCtx_(false) {
  }

  AQNode::NodeType type() const {
    return AQNode::Class;
  }

  bool supportNodes() const {
    return true;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s %s%s [%d,%d] [%d,%d]", indent, " ",
             "Class", name_.latin1(),
             base_.isEmpty() ? "" : (QString::fromLatin1("->") + base_).latin1(),
             begin_.first_line, begin_.first_column,
             end_.first_line, end_.first_column);
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpDebug(indent + 2);
    qWarning("%*s" "%s [%d,%d] %d %d", indent, " ",
             "End", end_.last_line, end_.last_column,
             isInternaCtx_, inheritsInternaCtx_);
  }

  virtual YYLTYPE innerBegin() const {
    return {end_.first_line, end_.first_column + 1,
            end_.first_line, end_.first_column + 1
           };
  }
  virtual YYLTYPE innerEnd() const {
    return {end_.last_line, end_.last_column - 1,
            end_.last_line, end_.last_column - 1
           };
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    QString calcText;
    for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
      if (it->type() == AQNode::VarConst)
        it->dumpCode(srcCode, calcText);
    }
    for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
      if (it->type() != AQNode::VarConst)
        it->dumpCode(srcCode, calcText);
    }

    if (base_.isEmpty()) {
      QString skel("class %1 {%2}");
      calcText = skel.arg(name_).arg(calcText);
    } else {
      QString skel("class %1 extends %2 {%3}");
      calcText = skel.arg(name_).arg(base_).arg(calcText);
    }

    dstCode += calcText;
  }

  inline AQFunctionNode *findFunction(const QString &name, bool checkOrigName = false);
  inline AQVarNode *findVar(const QString &name, bool checkOrigName = false);

  QString name_;
  QString base_;
  AQClassNode *join_;
  bool isInternaCtx_;
  bool inheritsInternaCtx_;
};

class AQFunctionNode : public AQNode
{
public:
  AQFunctionNode(const YYLTYPE &b, const YYLTYPE &e,
                 const QString &name = QString::null,
                 const QString &params = QString::null)
    : AQNode(b, e),
      name_(name),
      origName_(name),
      params_(params),
      callBaseFun_(false) {
  }

  AQNode::NodeType type() const {
    return AQNode::Function;
  }

  bool supportNodes() const {
    return true;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s %s(%s) %s [%d,%d] [%d,%d] %s %d", indent, " ",
             "Function", name_.latin1(),
             params_.isEmpty() ? "" : params_.latin1(),
             (origName_.isEmpty() || origName_ == name_) ? "" : origName_.latin1(),
             begin_.first_line, begin_.first_column,
             end_.first_line, end_.first_column,
             baseClassImplName_.isEmpty() ? "" : baseClassImplName_.latin1(),
             callBaseFun_);
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpDebug(indent + 2);
    qWarning("%*s" "%s [%d,%d] %s", indent, " ",
             "End", end_.last_line, end_.last_column,
             implFunName_.isEmpty() ? "" : implFunName_.latin1());
  }

  virtual YYLTYPE innerBegin() const {
    return {end_.first_line, end_.first_column + 1,
            end_.first_line, end_.first_column + 1
           };
  }
  virtual YYLTYPE innerEnd() const {
    return {end_.last_line, end_.last_column - 1,
            end_.last_line, end_.last_column - 1
           };
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    QString calcText;
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpCode(srcCode, calcText);

    QString skel("function %1(%2) {%3}");
    calcText = skel.arg(name_).arg(params_).arg(calcText);

    dstCode += calcText;
  }

  inline AQVarNode *findVar(const QString &name, bool checkOrigName = false);

  QString name_;
  QString origName_;
  QString params_;
  QString implFunName_;
  QString baseClassImplName_;
  bool callBaseFun_;
};

class AQVarNode : public AQNode
{
public:
  AQVarNode(const YYLTYPE &b, const YYLTYPE &e,
            const QString &name, int kind)
    : AQNode(b, e),
      name_(name),
      origName_(name),
      kind_(kind) {
  }

  AQNode::NodeType type() const {
    return AQNode::VarConst;
  }

  bool supportNodes() const {
    return true;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s %s [%d,%d] [%d %d]", indent, " ",
             kind_ == 0 ? "Var" : "Const", name_.latin1(),
             begin_.first_line, begin_.first_column,
             begin_.last_line, begin_.last_column);
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpDebug(indent + 2);
    qWarning("%*s" "%s [%d,%d]", indent, " ",
             "End", end_.last_line, end_.last_column);
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    QString calcText;

    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpCode(srcCode, calcText);

    calcText.prepend(kind_ == 0 ? "var " : "const ");

    if (!origName_.isEmpty() && origName_ != name_)
      calcText.replace(calcText.find(origName_), origName_.length(), name_);

    dstCode += calcText;
  }

  QString name_;
  QString origName_;
  int kind_;
};

class AQThisNode : public AQNode
{
public:
  AQThisNode(const YYLTYPE &b, const YYLTYPE &e)
    : AQNode(b, e),
      addCtx_(false) {
  }

  AQNode::NodeType type() const {
    return AQNode::This;
  }

  bool supportNodes() const {
    return false;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s [%d,%d] [%d,%d]", indent, " ", "This",
             begin_.first_line, begin_.first_column,
             begin_.last_line, begin_.last_column);
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    QString calcText(extracText(srcCode,
                                begin_.first_line, begin_.first_column,
                                end_.last_line, end_.last_column));
    if (addCtx_)
      calcText.replace(calcText.find("this"), 4 , "this.ctx");
    dstCode += calcText;
  }

  bool addCtx_;
};

class AQAccessorNode : public AQNode
{
public:
  AQAccessorNode(const YYLTYPE &b, const YYLTYPE &e,
                 const QString &name)
    : AQNode(b, e),
      name_(name),
      origName_(name) {
  }

  AQNode::NodeType type() const {
    return AQNode::Accessor;
  }

  bool supportNodes() const {
    return false;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s %s [%d,%d] [%d,%d]", indent, " ",
             "Accessor", name_.latin1(),
             begin_.first_line, begin_.first_column,
             end_.last_line, end_.last_column);
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    QString calcText(extracText(srcCode,
                                begin_.first_line, begin_.first_column,
                                end_.last_line, end_.last_column));

    if (!origName_.isEmpty() && origName_ != name_)
      calcText.replace(calcText.find(origName_), origName_.length(), name_);

    dstCode += calcText;
  }

  QString name_;
  QString origName_;
};

class AQReturnNode : public AQNode
{
public:
  AQReturnNode(const YYLTYPE &b, const YYLTYPE &e)
    : AQNode(b, e) {
  }

  AQNode::NodeType type() const {
    return AQNode::Return;
  }

  bool supportNodes() const {
    return true;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s [%d,%d]", indent, " ", "Return",
             begin_.first_line, begin_.first_column);
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpDebug(indent + 2);
    qWarning("%*s" "%s [%d,%d]", indent, " ",
             "End", end_.last_line, end_.last_column);
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    if (nodes_.isEmpty()) {
      dstCode += "return;";
      return;
    }

    QString calcText;

    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpCode(srcCode, calcText);

    calcText.prepend("return ");

    dstCode += calcText;
  }
};

class AQCallNode : public AQNode
{
public:
  AQCallNode(const YYLTYPE &b, const YYLTYPE &e,
             const QString &name)
    : AQNode(b, e),
      name_(name),
      origName_(name) {
  }

  AQNode::NodeType type() const {
    return AQNode::Call;
  }

  bool supportNodes() const {
    return true;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s %s [%d,%d]", indent, " ",
             "Call", name_.latin1(),
             begin_.first_line, begin_.first_column);
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpDebug(indent + 2);
    qWarning("%*s" "%s [%d,%d]", indent, " ",
             "End", end_.last_line, end_.last_column);
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    if (nodes_.isEmpty()) {
      dstCode +=  extracText(srcCode,
                             begin_.first_line, begin_.first_column - 1,
                             end_.last_line, end_.last_column);
      return;
    }

    QString calcText;
    AQNode *it = nodes_.first();

    it->begin_.first_line = begin_.first_line;
    it->begin_.first_column = begin_.first_column;
    for (; it; it = nodes_.next())
      it->dumpCode(srcCode, calcText);

    if (!origName_.isEmpty() && origName_ != name_)
      calcText.replace(calcText.findRev(origName_), origName_.length(), name_);

    dstCode += calcText;
  }

  QString name_;
  QString origName_;
};

class AQNewNode : public AQNode
{
public:
  AQNewNode(const YYLTYPE &b, const YYLTYPE &e,
            const QString &name)
    : AQNode(b, e),
      name_(name),
      origName_(name) {
  }

  AQNode::NodeType type() const {
    return AQNode::New;
  }

  bool supportNodes() const {
    return true;
  }

  void dumpDebug(int indent = 0) {
    if (skip_)
      return;
    qWarning("%*s" "%s %s [%d,%d] [%d,%d]", indent, " ",
             "New", name_.latin1(),
             begin_.first_line, begin_.first_column,
             end_.last_line, end_.last_column);
    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpDebug(indent + 2);
    qWarning("%*s" "%s [%d,%d]", indent, " ",
             "End", end_.last_line, end_.last_column);
  }

  void dumpCode(const QStringList &srcCode, QString &dstCode) {
    if (skip_)
      return;
    QString calcText;

    for (AQNode *it = nodes_.first(); it; it = nodes_.next())
      it->dumpCode(srcCode, calcText);

    QString eText(extracText(srcCode,
                             begin_.first_line, begin_.first_column,
                             end_.last_line, end_.last_column));
    if (eText.startsWith("("))
      calcText.prepend("(new ");
    else
      calcText.prepend("new ");

    if (!origName_.isEmpty() && origName_ != name_)
      calcText.replace(calcText.find(origName_), origName_.length(), name_);

    dstCode += calcText;
  }

  QString name_;
  QString origName_;
};

inline AQClassNode *AQRootNode::findClass(const QString &name)
{
  for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
    if (it->type() != AQNode::Class)
      continue;
    AQClassNode *n = CAST_N(Class, it);
    if (n->name_ == name)
      return n;
  }
  return 0;
}

inline AQFunctionNode *AQRootNode::findFunction(const QString &name, bool checkOrigName)
{
  for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
    if (it->type() != AQNode::Function)
      continue;
    AQFunctionNode *n = CAST_N(Function, it);
    if (n->name_ == name || (checkOrigName && n->origName_ == name))
      return n;
  }
  return 0;
}

inline AQVarNode *AQRootNode::findVar(const QString &name, bool checkOrigName)
{
  for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
    if (it->type() != AQNode::VarConst)
      continue;
    AQVarNode *n = CAST_N(Var, it);
    if (n->name_ == name || (checkOrigName && n->origName_ == name))
      return n;
  }
  return 0;
}

inline void AQNode::bindNodes()
{
  if (!this->supportNodes() || AQRootNode::nodes_.isEmpty())
    return;

  int firstLine = begin_.first_line;
  int firstCol = begin_.first_column;
  int lastLine = end_.last_line;
  int lastCol = end_.last_column;

  for (AQNode *it = AQRootNode::nodes_.first(); it;) {
    if (it == this) {
      it = AQRootNode::nodes_.next();
      continue;
    }
    if ((it->begin_.first_line > firstLine ||
         (it->begin_.first_line == firstLine &&
          it->begin_.first_column >= firstCol))
        &&
        (it->end_.last_line < lastLine ||
         (it->end_.last_line == lastLine &&
          it->end_.last_column <= lastCol))) {
      this->nodes().append(AQRootNode::nodes_.take());
      it = AQRootNode::nodes_.current();
    } else
      it = AQRootNode::nodes_.next();
  }
}

inline void AQNode::insertJumps()
{
  if (!this->supportNodes())
    return;

  YYLTYPE iB = this->innerBegin();
  YYLTYPE iE = this->innerEnd();
  AQJumpNode *markBegin = new AQJumpNode( {iB.last_line, iB.last_column,
                                           iB.last_line, iB.last_column
                                          });
  AQJumpNode *markEnd = new AQJumpNode( {iE.last_line, iE.last_column,
                                         iE.last_line, iE.last_column
                                        });
  this->nodes().prepend(markBegin);
  this->nodes().append(markEnd);

  int lastLine;
  int lastCol;
  int idx = 0;

  AQNode *it = this->nodes().first();
  while (it) {
    if (it->type() == AQNode::Jump) {
      lastLine = it->begin_.last_line;
      lastCol = it->begin_.last_column + 1;
    } else {
      lastLine = it->end_.last_line;
      lastCol = it->end_.last_column + 1;
    }

    it = this->nodes().next();
    if (!it)
      break;
    ++idx;
    it->insertJumps();

    if (it->begin_.first_line > lastLine ||
        (it->begin_.first_line == lastLine &&
         it->begin_.first_column > lastCol)) {
      AQJumpNode *jump = new AQJumpNode( {lastLine, lastCol,
                                          it->begin_.first_line,
                                          it->begin_.first_column - 1
                                         });
      this->nodes().insert(idx, jump);
      it = this->nodes().next();
      ++idx;
    }
  }

  this->nodes().remove(markBegin);
  this->nodes().remove(markEnd);

  AQNode *last = this->nodes().getLast();
  if (!last)
    return;
  if (last->end_.last_line != end_.last_line ||
      last->end_.last_column != end_.last_column) {
    this->nodes().append(new AQJumpNode( {end_.last_line,
                                          end_.last_column,
                                          end_.last_line,
                                          end_.last_column
                                         }));
  }
}

inline void AQNode::queryNodes(AQNode::NodeType t, AQNodeList &dstList, bool recur)
{
  if (!this->supportNodes())
    return;
  for (AQNode *it = this->nodes().first(); it; it = this->nodes().next()) {
    if (t == AQNode::Any || it->type() == t)
      dstList.append(it);
    if (recur)
      it->queryNodes(t, dstList, recur);
  }
}

inline void AQNode::dumpCode(const QStringList &srcCode, QString &dstCode)
{
  if (skip_)
    return;
  dstCode += extracText(srcCode,
                        begin_.first_line, begin_.first_column,
                        end_.last_line, end_.last_column);
  if (!this->supportNodes())
    return;
  for (AQNode *it = this->nodes().first(); it; it = this->nodes().next())
    it->dumpCode(srcCode, dstCode);
}

inline AQNodeList AQNode::nodesWithOutJumps()
{
  AQNodeList ret;
  if (!this->supportNodes())
    return ret;
  for (AQNode *it = this->nodes().first(); it; it = this->nodes().next()) {
    if (it->type() != AQNode::Jump)
      ret.append(it);
  }
  return ret;
}

inline AQFunctionNode *AQClassNode::findFunction(const QString &name, bool checkOrigName)
{
  for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
    if (it->type() != AQNode::Function)
      continue;
    AQFunctionNode *n = CAST_N(Function, it);
    if (n->name_ == name || (checkOrigName && n->origName_ == name))
      return n;
  }
  return 0;
}

inline AQVarNode *AQClassNode::findVar(const QString &name, bool checkOrigName)
{
  for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
    if (it->type() != AQNode::VarConst)
      continue;
    AQVarNode *n = CAST_N(Var, it);
    if (n->name_ == name || (checkOrigName && n->origName_ == name))
      return n;
  }
  return 0;
}

inline AQVarNode *AQFunctionNode::findVar(const QString &name, bool checkOrigName)
{
  for (AQNode *it = nodes_.first(); it; it = nodes_.next()) {
    if (it->type() != AQNode::VarConst)
      continue;
    AQVarNode *n = CAST_N(Var, it);
    if (n->name_ == name || (checkOrigName && n->origName_ == name))
      return n;
  }
  return 0;
}

#endif /* AQNODES_P_H_ */
