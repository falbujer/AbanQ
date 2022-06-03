/***************************************************************************
 AQLexer.h
 -------------------
 begin                : 06/03/2011
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

#ifndef AQLEXER_H
#define AQLEXER_H

#include <qstring.h>
#include <qlocale.h>

class QDataStream;

enum AQErrorCode {
  AQErrNoError,
  AQErrGeneral,
  AQErrNestedClass,
  AQErrClassRedef,
  AQErrClassBaseInvalid,
  AQErrClassBaseFinal,
  AQErrAttributeDuplication,
  AQErrAttributeConflict,
  AQErrAttrStaticContext,
  AQErrTypeUnknown,
  AQErrPositionGlobal,
  AQErrParseError
};

enum Operator {
  OpEqual,
  OpEqEq,
  OpNotEq,
  OpStrEq,
  OpStrNEq,
  OpPlusEq,
  OpMinusEq,
  OpMultEq,
  OpDivEq,
  OpPlusPlus,
  OpMinusMinus,
  OpLess,
  OpLessEq,
  OpGreater,
  OpGreaterEq,
  OpAndEq,
  OpXOrEq,
  OpOrEq,
  OpModEq,
  OpAnd,
  OpOr,
  OpBitAnd,
  OpBitXOr,
  OpBitOr,
  OpLShift,
  OpRShift,
  OpURShift,
  OpIs,
  OpIn,
  OpInstanceOf
};

class AQLEXERCLASS
{
public:
  enum State {
    Start,
    Identifier,
    InIdentifier,
    InSingleLineComment,
    InMultiLineComment,
    InNum,
    InNum0,
    InHex,
    InOctal,
    InDecimal,
    InExponentIndicator,
    InExponent,
    Hex,
    Octal,
    Number,
    String,
    Eof,
    InString,
    InEscapeSequence,
    InHexEscape,
    InUnicodeEscape,
    Other,
    Bad,
    KeywordNumber
  };

  enum ParenthesesState {
    IgnoreParentheses,
    CountParentheses,
    BalancedParentheses
  };

  AQLEXERCLASS();
  virtual ~AQLEXERCLASS();

  static AQLEXERCLASS *lexer() {
    return lx;
  }

  void setCode(const QString &c, int id, int lineno = 0,
               QDataStream *outstream = 0, bool humanRead = false,
               bool toPseudoCode = true);
  int lex();

  int lineNo() const {
    return yylineno + 1;
  }

  int sourceId() const {
    return sid;
  }

  bool prevTerminator() const {
    return terminator;
  }


  QString pattern, flags;

  bool scanRegExp();

  State lexerState() const {
    return state;
  }

  QString errorMessage() const {
    return errmsg;
  }
  void setErrorMessage(const QString &err) {
    errmsg = err;
  }
  void setErrorMessage(const char *err) {
    setErrorMessage(QString::fromLatin1(err));
  }

private:
  int yylineno;
  int yycolno;
  bool done;
  char *buffer8;
  QChar *buffer16;
  uint size8, size16;
  uint pos8, pos16;
  bool terminator;
  bool restrKeyword;
  // encountered delimiter like "'" and "}" on last run
  bool delimited;
  int stackToken;

  State state;
  void setDone(State s) {
    ignoreCh_ = true;
    state = s;
    done = TRUE;
  }

  uint pos;

  bool isWhiteSpace() const {
    return (current == ' ' || current == '\t' ||
            current == 0x0b || current == 0x0c);
  }

  bool isWhiteSpace(ushort c) const {
    return (c == ' ' || c == '\t' ||
            c == 0x0b || c == 0x0c);
  }

  bool isLineTerminator() const {
    return (current == '\n' || current == '\r');
  }

  bool isHexDigit(ushort c) const {
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'));
  }

  bool isOctalDigit(ushort c) const {
    return (c >= '0' && c <= '7');
  }

  ushort singleEscape(ushort c) const {
    switch (c) {
      case 'b':
        return 0x08;
      case 't':
        return 0x09;
      case 'n':
        return 0x0A;
      case 'v':
        return 0x0B;
      case 'f':
        return 0x0C;
      case 'r':
        return 0x0D;
      case '"':
        return 0x22;
      case '\'':
        return 0x27;
      case '\\':
        return 0x5C;
      default:
        return c;
    }
  }

  ushort convertOctal(ushort c1, ushort c2,
                      ushort c3) const {
    return ((c1 - '0') * 64 + (c2 - '0') * 8 + c3 - '0');
  }

public:
  static uchar convertHex(ushort c) {
    if (c >= '0' && c <= '9')
      return static_cast<uchar>(c - '0');
    else if (c >= 'a' && c <= 'f')
      return static_cast<uchar>(c - 'a' + 10);
    else
      return static_cast<uchar>(c - 'A' + 10);
  }

  static uchar convertHex(ushort c1, ushort c2) {
    return ((convertHex(c1) << 4) + convertHex(c2));
  }

  static QChar convertUnicode(ushort c1, ushort c2,
                              ushort c3, ushort c4) {
    return QChar((convertHex(c3) << 4) + convertHex(c4),
                 (convertHex(c1) << 4) + convertHex(c2));
  }

  static bool isIdentLetter(ushort c) {
    /* TODO: allow other legitimate unicode chars */
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == '$' || c == '_'));
  }

  static bool isDecimalDigit(ushort c) {
    return (c >= '0' && c <= '9');
  }

private:

  static AQLEXERCLASS *lx;
  static int sid;

  void syncProhibitAutomaticSemicolon() {
    if (parenthesesState == BalancedParentheses) {
      // we have seen something like "if (foo)", which means we should
      // never insert an automatic semicolon at this point, since it would
      // then be expanded into an empty statement (ECMA-262 7.9.1)
      prohibitAutomaticSemicolon = true;
      parenthesesState = IgnoreParentheses;
    } else {
      prohibitAutomaticSemicolon = false;
    }
  }

  const QChar *code;
  uint length;
  int yycolumn;
  int bol;

  // current and following unicode characters
  ushort current, next1, next2, next3;

  QString errmsg;
  QLocale localeC;
  QDataStream *out_;

  ParenthesesState parenthesesState;
  int parenthesesCount;
  bool prohibitAutomaticSemicolon;
  bool humanRead_;
  bool toPseudoCode_;
  bool ignoreCh_;
  bool emptyBol_;
};

#endif
