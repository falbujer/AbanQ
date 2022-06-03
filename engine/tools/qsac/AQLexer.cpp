/***************************************************************************
 AQLexer.cpp
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AQLexer.h"
#include <AQGrammar.h>

#include <qdatastream.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Q_UINT16 lastCharOut = 0;

#define AQ_COUT(C) if (out_) { \
    lastCharOut = C; \
    if (lastCharOut & 0x80) \
      (*out_) << (Q_UINT8)~((lastCharOut >> 8) ^ 7); \
    (*out_) << (Q_UINT8)~((lastCharOut & 0x00ff) ^ 7); \
  }

#define AQ_COUT_DST(C,DST) if (DST) { \
    lastCharOut = C; \
    if (lastCharOut & 0x80) \
      (*DST) << (Q_UINT8)~((lastCharOut >> 8) ^ 7); \
    (*DST) << (Q_UINT8)~((lastCharOut & 0x00ff) ^ 7); \
  }

#define record8(C) { \
    if (pos8 >= size8 - 1) { \
      char *tmp = new char[2 * size8]; \
      memcpy(tmp, buffer8, size8 * sizeof(char)); \
      delete [] buffer8; \
      buffer8 = tmp; \
      size8 *= 2; \
    } \
    buffer8[pos8++] = (char) C; \
  }

#define record16(C) { \
    if (pos16 >= size16 - 1) { \
      QChar *tmp = new QChar[2 * size16]; \
      memcpy(tmp, buffer16, size16 * sizeof(QChar)); \
      delete [] buffer16; \
      buffer16 = tmp; \
      size16 *= 2; \
    } \
    buffer16[pos16++] = C; \
  }

#define shift(P) { \
    int p = P; \
    while (p--) { \
      emptyBol_ = emptyBol_ && (current == 0 || isWhiteSpace() || isLineTerminator()); \
      ++pos; ++yycolno; \
      if (out_) AQ_COUT(current); \
      current = next1; \
      next1 = next2; \
      next2 = next3; \
      next3 = (pos + 3 < length) ? code[pos + 3].unicode() : 0; \
    } \
  }

#define shiftWindowsLineBreak() \
  if (((current == '\r') && (next1 == '\n')) \
      || ((current == '\n') && (next1 == '\r'))) \
    shift(1);

#define incrLineNo yycolno=0;yylineno++

#define shiftLineBreak() \
  while(next1 == '\n' || next1 == '\r') \
    {  shift(1); incrLineNo; }

#define quickMatchPunctuator(c1,c2,c3,c4) do { \
    if (c1 == '>' && c2 == '>' && c3 == '>' && c4 == '=') { \
      token = URSHIFTEQUAL; \
      shift(4); break; \
    } else if (c1 == '=' && c2 == '=' && c3 == '=') { \
      token = STREQ; \
      shift(3); break; \
    } else if (c1 == '!' && c2 == '=' && c3 == '=') { \
      token = STRNEQ; \
      shift(3); break; \
    } else if (c1 == '>' && c2 == '>' && c3 == '>') { \
      token = URSHIFT; \
      shift(3); break; \
    } else if (c1 == '<' && c2 == '<' && c3 == '=') { \
      token = LSHIFTEQUAL; \
      shift(3); break; \
    } else if (c1 == '>' && c2 == '>' && c3 == '=') { \
      token = RSHIFTEQUAL; \
      shift(3); break; \
    } else if (c1 == '<' && c2 == '=') { \
      token = LE; \
      shift(2); break; \
    } else if (c1 == '>' && c2 == '=') { \
      token = GE; \
      shift(2); break; \
    } else if (c1 == '!' && c2 == '=') { \
      token = NE; \
      shift(2); break; \
    } else if (c1 == '+' && c2 == '+') { \
      token = PLUSPLUS; \
      shift(2); break; \
    } else if (c1 == '-' && c2 == '-') { \
      token = MINUSMINUS; \
      shift(2); break; \
    } else if (c1 == '=' && c2 == '=') { \
      token = EQEQ; \
      shift(2); break; \
    } else if (c1 == '+' && c2 == '=') { \
      token = PLUSEQUAL; \
      shift(2); break; \
    } else if (c1 == '-' && c2 == '=') { \
      token = MINUSEQUAL; \
      shift(2); break; \
    } else if (c1 == '*' && c2 == '=') { \
      token = MULTEQUAL; \
      shift(2); break; \
    } else if (c1 == '/' && c2 == '=') { \
      token = DIVEQUAL; \
      shift(2); break; \
    } else if (c1 == '&' && c2 == '=') { \
      shift(2); break; \
      token = ANDEQUAL; \
    } else if (c1 == '^' && c2 == '=') { \
      token = XOREQUAL; \
      shift(2); break; \
    } else if (c1 == '%' && c2 == '=') { \
      token = MODEQUAL; \
      shift(2); break; \
    } else if (c1 == '|' && c2 == '=') { \
      token = OREQUAL; \
      shift(2); break; \
    } else if (c1 == '<' && c2 == '<') { \
      token = LSHIFT; \
      shift(2); break; \
    } else if (c1 == '>' && c2 == '>') { \
      token = RSHIFT; \
      shift(2); break; \
    } else if (c1 == '&' && c2 == '&') { \
      token = AND; \
      out_ = 0; shift(2); out_ = out; break; \
    } else if (c1 == '|' && c2 == '|') { \
      token = OR; \
      out_ = 0; shift(2); out_ = out; break; \
    } \
    switch (c1) { \
      case '=': \
      case '>': \
      case '<': \
      case ',': \
      case '!': \
      case '~': \
      case '?': \
      case ':': \
      case '.': \
      case '+': \
      case '-': \
      case '*': \
      case '/': \
      case '&': \
      case '|': \
      case '^': \
      case '%': \
      case '(': \
      case ')': \
      case '{': \
      case '}': \
      case '[': \
      case ']': \
      case ';': { \
        token = static_cast<int>(c1); \
        shift(1); \
      } \
      break; \
      default: \
        token = -1; \
    } \
  } while(0)

AQLEXERCLASS *AQLEXERCLASS::lx = 0;
int AQLEXERCLASS::sid;

#ifdef AQROLLER_GRAMMAR
#define aqyylloc aqryylloc
#define aqyylval aqryylval
int aqryylex()
#else
int aqyylex()
#endif
{
  return AQLEXERCLASS::lexer()->lex();
}

//#define AQ_CHECK_RESERVED

static inline bool toPseudoCode(int token, QDataStream *out)
{
  switch (token) {
    case IF: // 2
      AQ_COUT_DST('S', out);
      AQ_COUT_DST('i', out);
      return true;

    case OR: // 2
      AQ_COUT_DST('O', out);
      AQ_COUT_DST('R', out);
      return true;

    case AND: // 3
      AQ_COUT_DST('A', out);
      AQ_COUT_DST('N', out);
      AQ_COUT_DST('D', out);
      return true;

    case ELSE: // 4
      AQ_COUT_DST('S', out);
      AQ_COUT_DST('i', out);
      AQ_COUT_DST('N', out);
      AQ_COUT_DST('o', out);
      return true;

    case FOR: // 4
      AQ_COUT_DST('P', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('r', out);
      AQ_COUT_DST('a', out);
      return true;

    case CASE: // 4
      AQ_COUT_DST('C', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('s', out);
      AQ_COUT_DST('o', out);
      return true;

    case SWITCH: // 5
      AQ_COUT_DST('S', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('g', out);
      AQ_COUT_DST('u', out);
      AQ_COUT_DST('n', out);
      return true;

    case DO: // 5
      AQ_COUT_DST('H', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('c', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('r', out);
      return true;

    case CLASS: // 5
      AQ_COUT_DST('C', out);
      AQ_COUT_DST('l', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('s', out);
      AQ_COUT_DST('e', out);
      return true;

    case FALSETOKEN: // 5
      AQ_COUT_DST('F', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('l', out);
      AQ_COUT_DST('s', out);
      AQ_COUT_DST('o', out);
      return true;

    case TRUETOKEN: // 6
      AQ_COUT_DST('C', out);
      AQ_COUT_DST('i', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('r', out);
      AQ_COUT_DST('t', out);
      AQ_COUT_DST('o', out);
      return true;

    case BREAK: // 6
      AQ_COUT_DST('R', out);
      AQ_COUT_DST('o', out);
      AQ_COUT_DST('m', out);
      AQ_COUT_DST('p', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('r', out);
      return true;

    case FUNCTION: // 7
      AQ_COUT_DST('F', out);
      AQ_COUT_DST('u', out);
      AQ_COUT_DST('n', out);
      AQ_COUT_DST('c', out);
      AQ_COUT_DST('i', out);
      AQ_COUT_DST('o', out);
      AQ_COUT_DST('n', out);
      return true;

    case DEFAULT: // 7
      AQ_COUT_DST('D', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('f', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('c', out);
      AQ_COUT_DST('t', out);
      AQ_COUT_DST('o', out);
      return true;

    case RETURN: // 8
      AQ_COUT_DST('R', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('t', out);
      AQ_COUT_DST('o', out);
      AQ_COUT_DST('r', out);
      AQ_COUT_DST('n', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('r', out);
      return true;

    case WHILE: // 8
      AQ_COUT_DST('M', out);
      AQ_COUT_DST('i', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('n', out);
      AQ_COUT_DST('t', out);
      AQ_COUT_DST('r', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('s', out);
      return true;

    case EXTENDS: // 8
      AQ_COUT_DST('E', out);
      AQ_COUT_DST('x', out);
      AQ_COUT_DST('t', out);
      AQ_COUT_DST('i', out);
      AQ_COUT_DST('e', out);
      AQ_COUT_DST('n', out);
      AQ_COUT_DST('d', out);
      AQ_COUT_DST('e', out);
      return true;

    case CONTINUE: // 9
      AQ_COUT_DST('C', out);
      AQ_COUT_DST('o', out);
      AQ_COUT_DST('n', out);
      AQ_COUT_DST('t', out);
      AQ_COUT_DST('i', out);
      AQ_COUT_DST('n', out);
      AQ_COUT_DST('u', out);
      AQ_COUT_DST('a', out);
      AQ_COUT_DST('r', out);
      return true;
  }

  return false;
}

static inline int findReservedWordPseudo(const QChar *c, int size)
{
  switch (size) {
    case 2: {
      if (c[0] == 'S' && c[1] == 'i')
        return IF;
      else if (c[0] == 'O' && c[1] == 'R')
        return OR;
    }
    break;

    case 3: {
      if (c[0] == 'A' && c[1] == 'N' && c[2] == 'D')
        return AND;
    }
    break;

    case 4: {
      if (c[0] == 'S' && c[1] == 'i'
          && c[2] == 'N' && c[3] == 'o')
        return ELSE;
      else if (c[0] == 'P' && c[1] == 'a'
               && c[2] == 'r' && c[3] == 'a')
        return FOR;
      else if (c[0] == 'C' && c[1] == 'a'
               && c[2] == 's' && c[3] == 'o')
        return CASE;
    }
    break;

    case 5: {
      if (c[0] == 'S' && c[1] == 'e'
          && c[2] == 'g' && c[3] == 'u'
          && c[4] == 'n')
        return SWITCH;
      else if (c[0] == 'H' && c[1] == 'a'
               && c[2] == 'c' && c[3] == 'e'
               && c[4] == 'r')
        return DO;
      else if (c[0] == 'C' && c[1] == 'l'
               && c[2] == 'a' && c[3] == 's'
               && c[4] == 'e')
        return CLASS;
      else if (c[0] == 'F' && c[1] == 'a'
               && c[2] == 'l' && c[3] == 's'
               && c[4] == 'o')
        return FALSETOKEN;
    }
    break;

    case 6: {
      if (c[0] == 'C' && c[1] == 'i'
          && c[2] == 'e' && c[3] == 'r'
          && c[4] == 't' && c[5] == 'o')
        return TRUETOKEN;
      else if (c[0] == 'R' && c[1] == 'o'
               && c[2] == 'm' && c[3] == 'p'
               && c[4] == 'e' && c[5] == 'r')
        return BREAK;
    }
    break;

    case 7: {
      if (c[0] == 'F' && c[1] == 'u'
          && c[2] == 'n' && c[3] == 'c'
          && c[4] == 'i' && c[5] == 'o'
          && c[6] == 'n')
        return FUNCTION;
      else if (c[0] == 'D' && c[1] == 'e'
               && c[2] == 'f' && c[3] == 'e'
               && c[4] == 'c' && c[5] == 't'
               && c[6] == 'o')
        return DEFAULT;
    }
    break;

    case 8: {
      if (c[0] == 'R' && c[1] == 'e'
          && c[2] == 't' && c[3] == 'o'
          && c[4] == 'r' && c[5] == 'n'
          && c[6] == 'a' && c[7] == 'r')
        return RETURN;
      else if (c[0] == 'M' && c[1] == 'i'
               && c[2] == 'e' && c[3] == 'n'
               && c[4] == 't' && c[5] == 'r'
               && c[6] == 'a' && c[7] == 's')
        return WHILE;
      else if (c[0] == 'E' && c[1] == 'x'
               && c[2] == 't' && c[3] == 'i'
               && c[4] == 'e' && c[5] == 'n'
               && c[6] == 'd' && c[7] == 'e')
        return EXTENDS;
    }
    break;

    case 9: {
      if (c[0] == 'C' && c[1] == 'o'
          && c[2] == 'n' && c[3] == 't'
          && c[4] == 'i' && c[5] == 'n'
          && c[6] == 'u' && c[7] == 'a'
          && c[8] == 'r')
        return CONTINUE;
    }
    break;
  }

  return -1;
}

static inline int findReservedWord(const QChar *c, int size)
{
#ifdef AQ_CHECK_RESERVED
  bool check_reserved = true;
#endif

  if (size < 2 || size > 12)
    return -1;
  if (!c[0].isLetter())
    return -1;
  if (c[0].upper() == c[0])
    return findReservedWordPseudo(c, size);

  switch (size) {
    case 2: {
      if (c[0] == 'd' && c[1] == 'o')
        return DO;
      else if (c[0] == 'i' && c[1] == 'f')
        return IF;
      else if (c[0] == 'i' && c[1] == 'n')
        return AQ_IN;
      else if (c[0] == 'o' && c[1] == 'r')
        return OR;
    }
    break;

    case 3: {
      if (c[0] == 'f' && c[1] == 'o' && c[2] == 'r')
        return FOR;
      else if (c[0] == 'n' && c[1] == 'e' && c[2] == 'w')
        return NEW;
      else if (c[0] == 't' && c[1] == 'r' && c[2] == 'y')
        return TRY;
      else if (c[0] == 'v' && c[1] == 'a' && c[2] == 'r')
        return VAR;
      else if (c[0] == 'a' && c[1] == 'n' && c[2] == 'd')
        return AND;
#ifdef AQ_CHECK_RESERVED
      else if (check_reserved) {
        if (c[0] == 'i' && c[1] == 'n' && c[2] == 't')
          return RESERVED;
      }
#endif
    }
    break;

    case 4: {
      if (c[0] == 'c' && c[1] == 'a'
          && c[2] == 's' && c[3] == 'e')
        return CASE;
      else if (c[0] == 'e' && c[1] == 'l'
               && c[2] == 's' && c[3] == 'e')
        return ELSE;
      else if (c[0] == 't' && c[1] == 'h'
               && c[2] == 'i' && c[3] == 's')
        return THIS;
      else if (c[0] == 'v' && c[1] == 'o'
               && c[2] == 'i' && c[3] == 'd')
        return AQ_VOID;
      else if (c[0] == 'w' && c[1] == 'i'
               && c[2] == 't' && c[3] == 'h')
        return WITH;
      else if (c[0] == 't' && c[1] == 'r'
               && c[2] == 'u' && c[3] == 'e')
        return TRUETOKEN;
      else if (c[0] == 'n' && c[1] == 'u'
               && c[2] == 'l' && c[3] == 'l')
        return NULLTOKEN;
#ifdef AQ_CHECK_RESERVED
      else if (check_reserved) {
        if (c[0] == 'e' && c[1] == 'n'
            && c[2] == 'u' && c[3] == 'm')
          return RESERVED;
        else if (c[0] == 'b' && c[1] == 'y'
                 && c[2] == 't' && c[3] == 'e')
          return RESERVED;
        else if (c[0] == 'l' && c[1] == 'o'
                 && c[2] == 'n' && c[3] == 'g')
          return RESERVED;
        else if (c[0] == 'c' && c[1] == 'h'
                 && c[2] == 'a' && c[3] == 'r')
          return RESERVED;
        else if (c[0] == 'g' && c[1] == 'o'
                 && c[2] == 't' && c[3] == 'o')
          return RESERVED;
      }
#endif
    }
    break;

    case 5: {
      if (c[0] == 'b' && c[1] == 'r'
          && c[2] == 'e' && c[3] == 'a'
          && c[4] == 'k')
        return BREAK;
      else if (c[0] == 'c' && c[1] == 'a'
               && c[2] == 't' && c[3] == 'c'
               && c[4] == 'h')
        return CATCH;
      else if (c[0] == 't' && c[1] == 'h'
               && c[2] == 'r' && c[3] == 'o'
               && c[4] == 'w')
        return THROW;
      else if (c[0] == 'w' && c[1] == 'h'
               && c[2] == 'i' && c[3] == 'l'
               && c[4] == 'e')
        return WHILE;
      else if (c[0] == 'c' && c[1] == 'o'
               && c[2] == 'n' && c[3] == 's'
               && c[4] == 't')
        return AQ_CONST;
      else if (c[0] == 'f' && c[1] == 'a'
               && c[2] == 'l' && c[3] == 's'
               && c[4] == 'e')
        return FALSETOKEN;
      else if (c[0] == 'c' && c[1] == 'l'
               && c[2] == 'a' && c[3] == 's'
               && c[4] == 's')
        return CLASS;
#ifdef AQ_CHECK_RESERVED
      else if (check_reserved) {
        if (c[0] == 's' && c[1] == 'h'
            && c[2] == 'o' && c[3] == 'r'
            && c[4] == 't')
          return RESERVED;
        else if (c[0] == 's' && c[1] == 'u'
                 && c[2] == 'p' && c[3] == 'e'
                 && c[4] == 'r')
          return RESERVED;
        else if (c[0] == 'f' && c[1] == 'i'
                 && c[2] == 'n' && c[3] == 'a'
                 && c[4] == 'l')
          return FINAL;
      }
#endif
    }
    break;

    case 6: {
      if (c[0] == 'd' && c[1] == 'e'
          && c[2] == 'l' && c[3] == 'e'
          && c[4] == 't' && c[5] == 'e')
        return AQ_DELETE;
      else if (c[0] == 'r' && c[1] == 'e'
               && c[2] == 't' && c[3] == 'u'
               && c[4] == 'r' && c[5] == 'n')
        return RETURN;
      else if (c[0] == 's' && c[1] == 'w'
               && c[2] == 'i' && c[3] == 't'
               && c[4] == 'c' && c[5] == 'h')
        return SWITCH;
      else if (c[0] == 't' && c[1] == 'y'
               && c[2] == 'p' && c[3] == 'e'
               && c[4] == 'o' && c[5] == 'f')
        return TYPEOF;
      else if (c[0] == 's' && c[1] == 't'
               && c[2] == 'a' && c[3] == 't'
               && c[4] == 'i' && c[5] == 'c')
        return STATIC;
      else if (c[0] == 'p' && c[1] == 'u'
               && c[2] == 'b' && c[3] == 'l'
               && c[4] == 'i' && c[5] == 'c')
        return PUBLIC;
#ifdef AQ_CHECK_RESERVED
      else if (check_reserved) {
        if (c[0] == 'e' && c[1] == 'x'
            && c[2] == 'p' && c[3] == 'o'
            && c[4] == 'r' && c[5] == 't')
          return RESERVED;
        else if (c[0] == 'd' && c[1] == 'o'
                 && c[2] == 'u' && c[3] == 'b'
                 && c[4] == 'l' && c[5] == 'e')
          return RESERVED;
        else if (c[0] == 'i' && c[1] == 'm'
                 && c[2] == 'p' && c[3] == 'o'
                 && c[4] == 'r' && c[5] == 't')
          return IMPORT;
        else if (c[0] == 'n' && c[1] == 'a'
                 && c[2] == 't' && c[3] == 'i'
                 && c[4] == 'v' && c[5] == 'e')
          return RESERVED;
        else if (c[0] == 't' && c[1] == 'h'
                 && c[2] == 'r' && c[3] == 'o'
                 && c[4] == 'w' && c[5] == 's')
          return RESERVED;
      }
#endif
    }
    break;

    case 7: {
      if (c[0] == 'd' && c[1] == 'e'
          && c[2] == 'f' && c[3] == 'a'
          && c[4] == 'u' && c[5] == 'l'
          && c[6] == 't')
        return DEFAULT;
      else if (c[0] == 'f' && c[1] == 'i'
               && c[2] == 'n' && c[3] == 'a'
               && c[4] == 'l' && c[5] == 'l'
               && c[6] == 'y')
        return FINALLY;
      else if (c[0] == 'e' && c[1] == 'x'
               && c[2] == 't' && c[3] == 'e'
               && c[4] == 'n' && c[5] == 'd'
               && c[6] == 's')
        return EXTENDS;
      else if (c[0] == 'p' && c[1] == 'r'
               && c[2] == 'i' && c[3] == 'v'
               && c[4] == 'a' && c[5] == 't'
               && c[6] == 'e')
        return PRIVATE;
#ifdef AQ_CHECK_RESERVED
      else if (check_reserved) {
        if (c[0] == 'b' && c[1] == 'o'
            && c[2] == 'o' && c[3] == 'l'
            && c[4] == 'e' && c[5] == 'a'
            && c[6] == 'n')
          return RESERVED;
        else if (c[0] == 'p' && c[1] == 'a'
                 && c[2] == 'c' && c[3] == 'k'
                 && c[4] == 'a' && c[5] == 'g'
                 && c[6] == 'e')
          return PACKAGE;
      }
#endif
    }
    break;

    case 8: {
      if (c[0] == 'c' && c[1] == 'o'
          && c[2] == 'n' && c[3] == 't'
          && c[4] == 'i' && c[5] == 'n'
          && c[6] == 'u' && c[7] == 'e')
        return CONTINUE;
      else if (c[0] == 'f' && c[1] == 'u'
               && c[2] == 'n' && c[3] == 'c'
               && c[4] == 't' && c[5] == 'i'
               && c[6] == 'o' && c[7] == 'n')
        return FUNCTION;
#ifdef AQ_CHECK_RESERVED
      else if (check_reserved) {
        if (c[0] == 'd' && c[1] == 'e'
            && c[2] == 'b' && c[3] == 'u'
            && c[4] == 'g' && c[5] == 'g'
            && c[6] == 'e' && c[7] == 'r')
          return RESERVED;
        else if (c[0] == 'a' && c[1] == 'b'
                 && c[2] == 's' && c[3] == 't'
                 && c[4] == 'r' && c[5] == 'a'
                 && c[6] == 'c' && c[7] == 't')
          return ABSTRACT;
        else if (c[0] == 'v' && c[1] == 'o'
                 && c[2] == 'l' && c[3] == 'a'
                 && c[4] == 't' && c[5] == 'i'
                 && c[6] == 'l' && c[7] == 'e')
          return RESERVED;
      }
#endif
    }
    break;

    case 9: {
#ifdef AQ_CHECK_RESERVED
      if (check_reserved) {
        if (c[0] == 'i' && c[1] == 'n'
            && c[2] == 't' && c[3] == 'e'
            && c[4] == 'r' && c[5] == 'f'
            && c[6] == 'a' && c[7] == 'c'
            && c[8] == 'e')
          return RESERVED;
        else if (c[0] == 't' && c[1] == 'r'
                 && c[2] == 'a' && c[3] == 'n'
                 && c[4] == 's' && c[5] == 'i'
                 && c[6] == 'e' && c[7] == 'n'
                 && c[8] == 't')
          return RESERVED;
        else if (c[0] == 'p' && c[1] == 'r'
                 && c[2] == 'o' && c[3] == 't'
                 && c[4] == 'e' && c[5] == 'c'
                 && c[6] == 't' && c[7] == 'e'
                 && c[8] == 'd')
          return RESERVED;
      }
#endif
    }
    break;

    case 10: {
      if (c[0] == 'i' && c[1] == 'n'
          && c[2] == 's' && c[3] == 't'
          && c[4] == 'a' && c[5] == 'n'
          && c[6] == 'c' && c[7] == 'e'
          && c[8] == 'o' && c[9] == 'f')
        return INSTANCEOF;
#ifdef AQ_CHECK_RESERVED
      else if (check_reserved) {
        if (c[0] == 'i' && c[1] == 'm'
            && c[2] == 'p' && c[3] == 'l'
            && c[4] == 'e' && c[5] == 'm'
            && c[6] == 'e' && c[7] == 'n'
            && c[8] == 't' && c[9] == 's')
          return RESERVED;
      }
#endif
    }
    break;

    case 12: {
#ifdef AQ_CHECK_RESERVED
      if (check_reserved) {
        if (c[0] == 's' && c[1] == 'y'
            && c[2] == 'n' && c[3] == 'c'
            && c[4] == 'h' && c[5] == 'r'
            && c[6] == 'o' && c[7] == 'n'
            && c[8] == 'i' && c[9] == 'z'
            && c[10] == 'e' && c[11] == 'd')
          return RESERVED;
      }
#endif
    }
    break;

  }

  return -1;
}

AQLEXERCLASS::AQLEXERCLASS()
  : yylineno(1), yycolno(1),
    size8(32), size16(32768), restrKeyword(false),
    stackToken(-1), pos(0),
    code(0), length(0),
    bol(true),
    current(0), next1(0), next2(0), next3(0),
    localeC(QLocale::C), out_(0),
    parenthesesState(IgnoreParentheses),
    parenthesesCount(0),
    prohibitAutomaticSemicolon(false),
    humanRead_(false), toPseudoCode_(true),
    ignoreCh_(true), emptyBol_(true)
{
  lx = this;

  // allocate space for read buffers
  buffer8 = new char[size8];
  buffer16 = new QChar[size16];
}

AQLEXERCLASS::~AQLEXERCLASS()
{
  delete [] buffer8;
  delete [] buffer16;

  if (lx == this)
    lx = 0;
}

/*!
  Sets the code to be parsed.
  \a lineno specifies which line which is to be interpreted
  as line number zero. \a id specifies the unique identification
  number for this source code unit.
*/
void AQLEXERCLASS::setCode(const QString &c, int id, int lineno,
                           QDataStream *outstream, bool humanRead,
                           bool toPseudoCode)
{
  out_ = outstream;
  errmsg = QString::null;
  yylineno = -lineno;
  yycolno = 1;
  sid = id;
  restrKeyword = false;
  delimited = false;
  stackToken = -1;
  pos = 0;
  code = c.unicode();
  length = c.length();
  bol = true;
  humanRead_ = humanRead;
  toPseudoCode_ = toPseudoCode;
  ignoreCh_ = true;
  emptyBol_ = true;

  // read first characters
  current = (length > 0) ? code[0].unicode() : 0;
  next1 = (length > 1) ? code[1].unicode() : 0;
  next2 = (length > 2) ? code[2].unicode() : 0;
  next3 = (length > 3) ? code[3].unicode() : 0;
}

int AQLEXERCLASS::lex()
{
  QDataStream *out = out_;
  bool commentBol = false;

  int token = 0;
  state = Start;
  ushort stringType = 0; // either single or double quotes
  pos8 = pos16 = 0;
  done = false;
  terminator = false;

  // did we push a token on the stack previously ?
  // (after an automatic semicolon insertion)
  if (stackToken >= 0) {
    setDone(Other);
    token = stackToken;
    stackToken = 0;
  }

  while (!done) {
    switch (state) {
      case Start:
        aqyylloc.first_line = yylineno + 1;
        aqyylloc.first_column = yycolno;
        if (isWhiteSpace()) {
          ignoreCh_ = !humanRead_;
          if (ignoreCh_) {
            current = next1;
            while (isWhiteSpace()) {
              out_ = 0;
              shift(1);
              current = next1;
            }
            current = ' ';
            out_ = out;
          }
        } else if (current == '/' && next1 == '/') {
          commentBol = emptyBol_;
          ignoreCh_ = !humanRead_ || (next2 == '/' && next3 == '/');
          if (ignoreCh_)
            out_ = 0;
          shift(1);
          state = InSingleLineComment;
        } else if (current == '/' && next1 == '*') {
          commentBol = emptyBol_;
          ignoreCh_ = !humanRead_ || code[pos + 4].unicode() == '@';
          if (ignoreCh_)
            out_ = 0;
          shift(1);
          state = InMultiLineComment;
        } else if (current == 0) {
          syncProhibitAutomaticSemicolon();
          if (!terminator && !delimited && !prohibitAutomaticSemicolon) {
            // automatic semicolon insertion if program incomplete
            token = ';';
            if (humanRead_)
              AQ_COUT(token);
            stackToken = 0;
            setDone(Other);
          } else {
            if (ignoreCh_)
              out_ = 0;
            setDone(Eof);
          }
        } else if (isLineTerminator()) {
          shiftWindowsLineBreak();
          incrLineNo;
          bol = true;
          terminator = true;
          emptyBol_ = true;
          syncProhibitAutomaticSemicolon();
          if (restrKeyword) {
#ifdef AQROLLERLEXER
            --yylineno;
#endif
            token = ';';
            if (humanRead_)
              AQ_COUT(token);
            setDone(Other);
          }
        } else if (current == '"' || current == '\'') {
          state = InString;
          stringType = current;
        } else if (isIdentLetter(current)) {
          out_ = 0;
          record16(current);
          state = InIdentifier;
        } else if (current == '0') {
          record8(current);
          state = InNum0;
        } else if (isDecimalDigit(current)) {
          record8(current);
          state = InNum;
        } else if (current == '.' && isDecimalDigit(next1)) {
          record8(current);
          state = InDecimal;
        } else {
          quickMatchPunctuator(current, next1, next2, next3);
          if (token != -1) {
            setDone(Other);
          } else {
            setDone(Bad);
            errmsg = "Illegal character";
          }
        }
        break;
      case InString:
        if (current == stringType) {
          shift(1);
          setDone(String);
        } else if (current == 0 || isLineTerminator()) {
          setDone(Bad);
          errmsg = "Unclosed string at end of line";
        } else if (current == '\\') {
          state = InEscapeSequence;
        } else {
          record16(current);
        }
        break;
        // Escape Sequences inside of strings
      case InEscapeSequence:
        if (isOctalDigit(current)) {
          if (current >= '0' && current <= '3' &&
              isOctalDigit(next1) && isOctalDigit(next2)) {
            record16(convertOctal(current, next1, next2));
            shift(2);
            state = InString;
          } else if (isOctalDigit(current) &&
                     isOctalDigit(next1)) {
            record16(convertOctal('0', current, next1));
            shift(1);
            state = InString;
          } else if (isOctalDigit(current)) {
            record16(convertOctal('0', '0', current));
            state = InString;
          } else {
            setDone(Bad);
            errmsg = "Illegal escape squence";
          }
        } else if (current == 'x')
          state = InHexEscape;
        else if (current == 'u')
          state = InUnicodeEscape;
        else {
          if (isLineTerminator()) {
            shiftWindowsLineBreak();
            incrLineNo;
          }
          record16(singleEscape(current));
          state = InString;
        }
        break;
      case InHexEscape:
        if (isHexDigit(current) && isHexDigit(next1)) {
          state = InString;
          record16(convertHex(current, next1));
          shift(1);
        } else if (current == stringType) {
          record16('x');
          shift(1);
          setDone(String);
        } else {
          record16('x');
          record16(current);
          state = InString;
        }
        break;
      case InUnicodeEscape:
        if (isHexDigit(current) && isHexDigit(next1) &&
            isHexDigit(next2) && isHexDigit(next3)) {
          record16(convertUnicode(current, next1, next2, next3));
          shift(3);
          state = InString;
        } else if (current == stringType) {
          record16('u');
          shift(1);
          setDone(String);
        } else {
          setDone(Bad);
          errmsg = "Illegal unicode escape sequence";
        }
        break;
      case InSingleLineComment:
        if (ignoreCh_)
          out_ = 0;
        if (isLineTerminator()) {
          shiftWindowsLineBreak();
          shiftLineBreak();
          incrLineNo;
          terminator = true;
          bol = true;
          emptyBol_ = true;

          if (!commentBol)
            out_ = out;
          commentBol = false;

          if (restrKeyword) {
            token = ';';
            if (humanRead_)
              AQ_COUT(token);
            setDone(Other);
          } else
            state = Start;
        } else if (current == 0) {
          emptyBol_ = true;
          commentBol = false;
          setDone(Eof);
        }
        break;
      case InMultiLineComment:
        if (ignoreCh_)
          out_ = 0;
        if (current == 0) {
          emptyBol_ = true;
          commentBol = false;
          setDone(Bad);
          errmsg = "Unclosed comment at end of file";
        } else if (isLineTerminator()) {
          shiftWindowsLineBreak();
          incrLineNo;
        } else if (current == '*' && next1 == '/') {
          state = Start;
          shift(1);
          if (next1 == '\n' || next1 == '\r') {
            shift(1);
            shiftWindowsLineBreak();
            shiftLineBreak();
            incrLineNo;
            terminator = true;
            bol = true;
            emptyBol_ = true;

            if (!commentBol)
              out_ = out;
            commentBol = false;
          }
        }
        break;
      case InIdentifier:
        if (isIdentLetter(current) || isDecimalDigit(current)) {
          out_ = 0;
          record16(current);
          break;
        }
        setDone(Identifier);
        break;
      case InNum0:
        if (current == 'x' || current == 'X') {
          record8(current);
          state = InHex;
        } else if (current == '.') {
          record8(current);
          state = InDecimal;
        } else if (current == 'e' || current == 'E') {
          record8(current);
          state = InExponentIndicator;
        } else if (isOctalDigit(current)) {
          record8(current);
          state = InOctal;
        } else if (isDecimalDigit(current)) {
          record8(current);
          state = InDecimal;
        } else {
          setDone(Number);
        }
        break;
      case InHex:
        if (isHexDigit(current))
          record8(current)
          else
            setDone(Hex);
        break;
      case InOctal:
        if (isOctalDigit(current)) {
          record8(current);
        } else if (isDecimalDigit(current)) {
          record8(current);
          state = InDecimal;
        } else {
          setDone(Octal);
        }
        break;
      case InNum:
        if (isDecimalDigit(current)) {
          record8(current);
        } else if (current == '.') {
          record8(current);
          state = InDecimal;
        } else if (current == 'e' || current == 'E') {
          record8(current);
          state = InExponentIndicator;
        } else if (current == 'k') {
          shift(1);
          setDone(KeywordNumber);
        } else {
          setDone(Number);
        }
        break;
      case InDecimal:
        if (isDecimalDigit(current)) {
          record8(current);
        } else if (current == 'e' || current == 'E') {
          record8(current);
          state = InExponentIndicator;
        } else {
          setDone(Number);
        }
        break;
      case InExponentIndicator:
        if (current == '+' || current == '-') {
          record8(current);
        } else if (isDecimalDigit(current)) {
          record8(current);
          state = InExponent;
        } else {
          setDone(Bad);
          errmsg = "Illegal syntax for exponential number";
        }
        break;
      case InExponent:
        if (isDecimalDigit(current)) {
          record8(current);
        } else {
          setDone(Number);
        }
        break;
      default:
        Q_ASSERT(!"Unhandled state in switch statement");
    }

    // move on to the next character
    if (!done)
      shift(1);
    if (state != Start && state != InSingleLineComment)
      bol = false;

    out_ = out;
  }

  // no identifiers allowed directly after numeric literal, e.g. "3in" is bad
  if ((state == Number || state == Octal || state == Hex)
      && isIdentLetter(current)) {
    state = Bad;
    errmsg = "Identifier cannot start with numeric literal";
  }

  // terminate string
  buffer8[pos8] = '\0';

  double dval = 0;
  if (!out_) {
    if (state == Number) {
      dval = localeC.toDouble(buffer8);
    } else if (state == Hex) { // scan hex numbers
      uint i;
      sscanf(buffer8, "%x", &i);
      dval = i;
      state = Number;
    } else if (state == Octal) { // scan octal number
      uint ui;
      sscanf(buffer8, "%o", &ui);
      dval = ui;
      state = Number;
    }
  } else {
    if (state == Hex)
      state = Number;
    else if (state == Octal)
      state = Number;
  }

  restrKeyword = false;
  delimited = false;

  aqyylloc.last_line = yylineno + 1;
  aqyylloc.last_column = yycolno;

  switch (parenthesesState) {
    case IgnoreParentheses:
      break;
    case CountParentheses:
      if (token == ')') {
        --parenthesesCount;
        if (parenthesesCount == 0)
          parenthesesState = BalancedParentheses;
      } else if (token == '(') {
        ++parenthesesCount;
      }
      break;
    case BalancedParentheses:
      parenthesesState = IgnoreParentheses;
      break;
  }

  switch (state) {
    case Eof:
      return 0;
    case Other:
      if (token == '}' || token == ';')
        delimited = true;
      if (humanRead_ && toPseudoCode_) {
        switch (token) {
          case AND:
            if (!isWhiteSpace(lastCharOut))
              AQ_COUT(' ');
            AQ_COUT('A');
            AQ_COUT('N');
            AQ_COUT('D');
            if (!isWhiteSpace())
              AQ_COUT(' ');
            break;

          case OR:
            if (!isWhiteSpace(lastCharOut))
              AQ_COUT(' ');
            AQ_COUT('O');
            AQ_COUT('R');
            if (!isWhiteSpace())
              AQ_COUT(' ');
            break;
        }
      } else {
        switch (token) {
          case AND:
            AQ_COUT('&');
            AQ_COUT('&');
            break;

          case OR:
            AQ_COUT('|');
            AQ_COUT('|');
            break;
        }
      }
      return token;
    case Identifier:
      if ((token = findReservedWord(buffer16, pos16)) < 0) {
        if (out_) {
          int p = 0;
          while (pos16--)
            AQ_COUT(buffer16[p++]);
        } else
          aqyylval.ustr = new QString(buffer16, pos16);
        return IDENT;
      }
      if (token == CONTINUE || token == BREAK ||
          token == RETURN || token == THROW) {
        restrKeyword = true;
      } else if (token == IF || token == FOR
                 || token == WHILE || token == WITH) {
        parenthesesState = CountParentheses;
        parenthesesCount = 0;
      } else if (token == DO) {
        parenthesesState = BalancedParentheses;
      }
      if (!out_)
        return token;
      if (!humanRead_) {
        QString st(localeC.toString(token));
        const QChar *bu = st.unicode();
        uint p = 0;
        while (p < st.length())
          AQ_COUT(bu[p++]);
        AQ_COUT(QChar('k').unicode());
      } else {
        if (!toPseudoCode_ || !toPseudoCode(token, out_)) {
          int p = 0;
          while (pos16--)
            AQ_COUT(buffer16[p++]);
        }
      }
      return token;
    case KeywordNumber:
      token = 0;
      for (int i = 0; i < pos8; ++i)
        token = (token * 10) + (buffer8[i] - '0');
      if (token == CONTINUE || token == BREAK ||
          token == RETURN || token == THROW) {
        restrKeyword = true;
      } else if (token == IF || token == FOR
                 || token == WHILE || token == WITH) {
        parenthesesState = CountParentheses;
        parenthesesCount = 0;
      } else if (token == DO) {
        parenthesesState = BalancedParentheses;
      }
      return token;
    case String:
      if (!out_)
        aqyylval.ustr = new QString(buffer16, pos16);
      return STRING;
    case Number:
      if (!out_)
        aqyylval.dval = dval;
      return NUMBER;
    case Bad:
      return -1;
    default:
      Q_ASSERT(!"unhandled numeration value in switch");
      return -1;
  }
}

bool AQLEXERCLASS::scanRegExp()
{
  pos16 = 0;
  bool lastWasEscape = FALSE;

  while (1) {
    if (isLineTerminator() || current == 0)
      return FALSE;
    else if (current != '/' || lastWasEscape == TRUE) {
      record16(current);
      lastWasEscape = !lastWasEscape && (current == '\\');
    } else {
      pattern = QString(buffer16, pos16);
      pos16 = 0;
      shift(1);
      break;
    }
    shift(1);
  }

  while (isIdentLetter(current)) {
    record16(current);
    shift(1);
  }
  flags = QString(buffer16, pos16);

  return TRUE;
}
