%{
/***************************************************************************
 AQGrammar.y
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
#include <string.h>
#include "AQLexer.h"

/* default values for bison */
#define YYDEBUG 0
//#define YYMAXDEPTH 0
# undef YYLSP_NEEDED
# define DBG(l, s, e)

extern int aqyylex();
static int yyerror (const char *);
static bool automatic();

%}

%union {
  int                   ival;
  double                dval;
  const QString         *ustr;
  void            *node;
  void            *stat;
  void            *param;
  void            *body;
  void            *func;
  void            *clss;
  void            *prog;
  void            *init;
  void            *src;
  void            *srcs;
  void            *slist;
  void            *args;
  void            *alist;
  void            *cblk;
  void            *clist;
  void            *ccl;
  void            *elm;
  void            *eli;
  void            *type;
  void            *tvar;
  void            *vbin;
  void            *blist;
  void            *attrs;
  int             attr;
  Operator            	op;
}

%start Program

/* expect a shift/reduce conflict from the "dangling else" problem
   when using bison the warning can be supressed */
// %expect 1

/* literals */
%token NULLTOKEN TRUETOKEN FALSETOKEN

/* keywords */
%token BREAK CASE DEFAULT FOR NEW VAR CONTINUE
%token FUNCTION RETURN AQ_VOID AQ_DELETE
%token IF THIS DO WHILE ELSE AQ_IN INSTANCEOF TYPEOF IS
%token SWITCH WITH RESERVED
%token THROW TRY CATCH FINALLY
%token CLASS CONSTRUCTOR EXTENDS
%token ABSTRACT FINAL PRIVATE PUBLIC STATIC AQ_CONST
%token PACKAGE IMPORT

/* give an if without an else higher precedence than an else to resolve the ambiguity */
%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE

/* punctuators */
%token EQEQ NE                     /* == and != */
%token STREQ STRNEQ                /* === and !== */
%token LE GE                       /* < and > */
%token OR AND                      /* || and && */
%token PLUSPLUS MINUSMINUS         /* ++ and --  */
%token LSHIFT                      /* << */
%token RSHIFT URSHIFT              /* >> and >>> */
%token PLUSEQUAL MINUSEQUAL        /* += and -= */
%token MULTEQUAL DIVEQUAL          /* *= and /= */
%token LSHIFTEQUAL                 /* <<= */
%token RSHIFTEQUAL URSHIFTEQUAL    /* >>= and >>>= */
%token ANDEQUAL MODEQUAL           /* &= and %= */
%token XOREQUAL OREQUAL            /* ^= and |= */

/* terminal types */
%token <dval> NUMBER
%token <ustr> STRING
%token <ustr> IDENT

/* non-terminal types */
%type <node>  Literal ArrayLiteral

%type <node>  PrimaryExpr PrimaryExprNoBrace
%type <node>  MemberExpr MemberExprNoBF
%type <node>  NewExpr NewExprNoBF
%type <node>  CallExpr CallExprNoBF
%type <node>  LeftHandSideExpr LeftHandSideExprNoBF
%type <node>  PostfixExpr PostfixExprNoBF
%type <node>  UnaryExpr UnaryExprNoBF UnaryExprCommon
%type <node>  MultiplicativeExpr MultiplicativeExprNoBF
%type <node>  AdditiveExpr AdditiveExprNoBF
%type <node>  ShiftExpr ShiftExprNoBF
%type <node>  RelationalExpr RelationalExprNoIn RelationalExprNoBF
%type <node>  EqualityExpr EqualityExprNoIn EqualityExprNoBF
%type <node>  BitwiseANDExpr BitwiseANDExprNoIn BitwiseANDExprNoBF
%type <node>  BitwiseXORExpr BitwiseXORExprNoIn BitwiseXORExprNoBF
%type <node>  BitwiseORExpr BitwiseORExprNoIn BitwiseORExprNoBF
%type <node>  LogicalANDExpr LogicalANDExprNoIn LogicalANDExprNoBF
%type <node>  LogicalORExpr LogicalORExprNoIn LogicalORExprNoBF
%type <node>  ConditionalExpr ConditionalExprNoIn ConditionalExprNoBF
%type <node>  AssignmentExpr AssignmentExprNoIn AssignmentExprNoBF
%type <node>  Expr ExprNoIn ExprNoBF

%type <node>  ExprOpt ExprNoInOpt

%type <node>  FunctionExpr
%type <node>  PropertyName PropertyNameAndValueList
%type <node>  Catch Finally

%type <stat>  Statement Block
%type <stat>  VariableDefinition EmptyStatement ExprStatement
%type <stat>  IfStatement IterationStatement ContinueStatement
%type <stat>  BreakStatement ReturnStatement WithStatement
%type <stat>  SwitchStatement LabelledStatement
%type <stat>  ThrowStatement TryStatement
%type <stat>  PackageDefinition ImportStatement
%type <clss>  ClassDefinition

%type <slist> StatementList
%type <func>  FunctionDeclaration
%type <body>  FunctionBody
%type <src>   SourceElement
%type <srcs>  SourceElements
%type <param> FormalParameterList
%type <op>    AssignmentOperator
%type <prog>  Program
%type <args>  Arguments
%type <alist> ArgumentList
%type <cblk>  CaseBlock
%type <ccl>   CaseClause DefaultClause
%type <clist> CaseClauses  CaseClausesOpt
%type <eli>   Elision ElisionOpt
%type <elm>   ElementList
%type <ival>  VariableDefinitionKind
%type <attr>  Attribute
%type <attrs> Attributes
%type <type>  TypeExpression ResultSignature
%type <tvar>  TypedVariable
%type <vbin>  VariableBinding VariableBindingNoIn
%type <blist> VariableBindingList VariableBindingListNoIn
%type <ustr>  PackageName

%expect 1

%%

Literal:
    NULLTOKEN                      	
  | TRUETOKEN                      	
  | FALSETOKEN                     	
  | NUMBER                         	
  | STRING                          
  | '/'       /* a RegExp ? */      { AQLexer *l = AQLexer::lexer(); if (!l->scanRegExp()) YYABORT; }
;

PropertyName:
    IDENT                          
  | STRING                         
  | NUMBER                         
;

PropertyNameAndValueList:
    PropertyName ':' AssignmentExpr 
  | PropertyNameAndValueList ',' PropertyName ':' AssignmentExpr                        
;

PrimaryExpr:
    PrimaryExprNoBrace
  | '{' '}'                           
  | '{' PropertyNameAndValueList '}'  
  | '{' PropertyNameAndValueList ',' '}'										                  
;

PrimaryExprNoBrace:
    THIS                           		
  | Literal
  | ArrayLiteral
  | IDENT                          		
  | '(' Expr ')'                   		
;

ArrayLiteral:
    '[' ElisionOpt ']'                 	
  | '[' ElementList ']'                	
  | '[' ElementList ',' ElisionOpt ']' 	
;

ElementList:
    ElisionOpt AssignmentExpr      		
  | ElementList ',' ElisionOpt AssignmentExpr                                   		
;

ElisionOpt:
    /* nothing */						
  | Elision
;

Elision:
    ','                            		
  | Elision ','                    		
;

MemberExpr:
    PrimaryExpr
  | FunctionExpr                      
  | MemberExpr '[' Expr ']'        		
  | MemberExpr '.' IDENT              
  | NEW MemberExpr Arguments          
;

MemberExprNoBF:
    PrimaryExprNoBrace	
  | MemberExprNoBF '[' Expr ']'       
  | MemberExprNoBF '.' IDENT          
  | NEW MemberExpr Arguments       		
;

NewExpr:
    MemberExpr
  | NEW NewExpr                     
;

NewExprNoBF:
    MemberExprNoBF
  | NEW NewExpr							        
;

CallExpr:
    MemberExpr Arguments            
  | CallExpr Arguments              
  | CallExpr '[' Expr ']'           
  | CallExpr '.' IDENT              
;

CallExprNoBF:
    MemberExprNoBF Arguments        
  | CallExprNoBF Arguments          
  | CallExprNoBF '[' Expr ']'       
  | CallExprNoBF '.' IDENT          
;

Arguments:
    '(' ')'                        
  | '(' ArgumentList ')'           
;

ArgumentList:
    AssignmentExpr                  	
  | ArgumentList ',' AssignmentExpr 	
;

LeftHandSideExpr:
    NewExpr
  | CallExpr
;

LeftHandSideExprNoBF:
    NewExprNoBF
  | CallExprNoBF
;

PostfixExpr:
    LeftHandSideExpr
  | LeftHandSideExpr PLUSPLUS      		
  | LeftHandSideExpr MINUSMINUS    		
;

PostfixExprNoBF:
    LeftHandSideExprNoBF
  | LeftHandSideExprNoBF PLUSPLUS		  
  | LeftHandSideExprNoBF MINUSMINUS		
;

UnaryExprCommon:
    AQ_DELETE UnaryExpr            		
  | AQ_VOID UnaryExpr              		
  | TYPEOF UnaryExpr               		
  | PLUSPLUS UnaryExpr             		
  | MINUSMINUS UnaryExpr           		
  | '+' UnaryExpr                  		
  | '-' UnaryExpr                  		
  | '~' UnaryExpr                  		
  | '!' UnaryExpr                  		
;

UnaryExpr:
    PostfixExpr
  | UnaryExprCommon
;

UnaryExprNoBF:
    PostfixExprNoBF
  | UnaryExprCommon
;

MultiplicativeExpr:
    UnaryExpr
  | MultiplicativeExpr '*' UnaryExpr 	
  | MultiplicativeExpr '/' UnaryExpr 	
  | MultiplicativeExpr '%' UnaryExpr 	
;

MultiplicativeExprNoBF:
    UnaryExprNoBF
  | MultiplicativeExprNoBF '*' UnaryExpr                                        
  | MultiplicativeExprNoBF '/' UnaryExpr                                        
  | MultiplicativeExprNoBF '%' UnaryExpr                                        
;

AdditiveExpr:
    MultiplicativeExpr
  | AdditiveExpr '+' MultiplicativeExpr 
  | AdditiveExpr '-' MultiplicativeExpr 
;

AdditiveExprNoBF:
    MultiplicativeExprNoBF
  | AdditiveExprNoBF '+' MultiplicativeExpr                                        
  | AdditiveExprNoBF '-' MultiplicativeExpr                                        
;

ShiftExpr:
    AdditiveExpr
  | ShiftExpr LSHIFT AdditiveExpr  		
  | ShiftExpr RSHIFT AdditiveExpr  		
  | ShiftExpr URSHIFT AdditiveExpr 		
;

ShiftExprNoBF:
    AdditiveExprNoBF
  | ShiftExprNoBF LSHIFT AdditiveExpr  	
  | ShiftExprNoBF RSHIFT AdditiveExpr  	
  | ShiftExprNoBF URSHIFT AdditiveExpr 	
;

RelationalExpr:
    ShiftExpr
  | RelationalExpr '<' ShiftExpr    
  | RelationalExpr '>' ShiftExpr    
  | RelationalExpr LE ShiftExpr     
  | RelationalExpr GE ShiftExpr     
  | RelationalExpr IS ShiftExpr     
  | RelationalExpr INSTANCEOF ShiftExpr 
  | RelationalExpr AQ_IN ShiftExpr    
;

RelationalExprNoIn:
    ShiftExpr
  | RelationalExprNoIn '<' ShiftExpr	
  | RelationalExprNoIn '>' ShiftExpr	
  | RelationalExprNoIn LE ShiftExpr		
  | RelationalExprNoIn GE ShiftExpr		
  | RelationalExprNoIn INSTANCEOF ShiftExpr  										
;

RelationalExprNoBF:
    ShiftExprNoBF
  | RelationalExprNoBF '<' ShiftExpr	
  | RelationalExprNoBF '>' ShiftExpr	
  | RelationalExprNoBF LE ShiftExpr		
  | RelationalExprNoBF GE ShiftExpr		
  | RelationalExprNoBF IS ShiftExpr		
  | RelationalExprNoBF INSTANCEOF ShiftExpr  										
  | RelationalExprNoBF AQ_IN ShiftExpr	
;

EqualityExpr:
    RelationalExpr
  | EqualityExpr EQEQ RelationalExpr   	
  | EqualityExpr NE RelationalExpr     	
  | EqualityExpr STREQ RelationalExpr  	
  | EqualityExpr STRNEQ RelationalExpr 	
;

EqualityExprNoIn:
    RelationalExprNoIn
  | EqualityExprNoIn EQEQ RelationalExprNoIn   	  										
  | EqualityExprNoIn NE RelationalExprNoIn  										
  | EqualityExprNoIn STREQ RelationalExprNoIn  										
  | EqualityExprNoIn STRNEQ RelationalExprNoIn  										
;

EqualityExprNoBF:
    RelationalExprNoBF
  | EqualityExprNoBF EQEQ RelationalExpr  										
  | EqualityExprNoBF NE RelationalExpr    										
  | EqualityExprNoBF STREQ RelationalExpr  										
  | EqualityExprNoBF STRNEQ RelationalExpr  										
;

BitwiseANDExpr:
    EqualityExpr
  | BitwiseANDExpr '&' EqualityExpr 	
;

BitwiseANDExprNoIn:
    EqualityExprNoIn
  | BitwiseANDExprNoIn '&' EqualityExprNoIn  										
;

BitwiseANDExprNoBF:
    EqualityExprNoBF
  | BitwiseANDExprNoBF '&' EqualityExpr 
;

BitwiseXORExpr:
    BitwiseANDExpr
  | BitwiseXORExpr '^' BitwiseANDExpr 	
;

BitwiseXORExprNoIn:
    BitwiseANDExprNoIn
  | BitwiseXORExprNoIn '^' BitwiseANDExprNoIn
  										
;

BitwiseXORExprNoBF:
    BitwiseANDExprNoBF
  | BitwiseXORExprNoBF '^' BitwiseANDExpr   										
;

BitwiseORExpr:
    BitwiseXORExpr
  | BitwiseORExpr '|' BitwiseXORExpr 	
;

BitwiseORExprNoIn:
    BitwiseXORExprNoIn
  | BitwiseORExprNoIn '|' BitwiseXORExprNoIn   										
;

BitwiseORExprNoBF:
    BitwiseXORExprNoBF
  | BitwiseORExprNoBF '|' BitwiseXORExpr  										
;

LogicalANDExpr:
    BitwiseORExpr
  | LogicalANDExpr AND BitwiseORExpr  
;

LogicalANDExprNoIn:
    BitwiseORExprNoIn
  | LogicalANDExprNoIn AND BitwiseORExprNoIn                           				
;

LogicalANDExprNoBF:
    BitwiseORExprNoBF
  | LogicalANDExprNoBF AND BitwiseORExpr                           				
;

LogicalORExpr:
    LogicalANDExpr
  | LogicalORExpr OR LogicalANDExpr   
;

LogicalORExprNoIn:
    LogicalANDExprNoIn
  | LogicalORExprNoIn OR LogicalANDExprNoIn                           				
;

LogicalORExprNoBF:
    LogicalANDExprNoBF
  | LogicalORExprNoBF OR LogicalANDExpr                           				
;

ConditionalExpr:
    LogicalORExpr
  | LogicalORExpr '?' AssignmentExpr ':' AssignmentExpr                           				
;

ConditionalExprNoIn:
    LogicalORExprNoIn
  | LogicalORExprNoIn '?' AssignmentExprNoIn ':' AssignmentExprNoIn                           				
;

ConditionalExprNoBF:
    LogicalORExprNoBF
  | LogicalORExprNoBF '?' AssignmentExpr ':' AssignmentExpr                           				
;

AssignmentExpr:
    ConditionalExpr
  | LeftHandSideExpr AssignmentOperator AssignmentExpr                           				
;

AssignmentExprNoIn:
    ConditionalExprNoIn
  | LeftHandSideExpr AssignmentOperator AssignmentExprNoIn                           				
;

AssignmentExprNoBF:
    ConditionalExprNoBF
  | LeftHandSideExprNoBF AssignmentOperator AssignmentExpr                           				
;

AssignmentOperator:
    '='                            		
  | PLUSEQUAL                      		
  | MINUSEQUAL                     		
  | MULTEQUAL                      		
  | DIVEQUAL                       		
  | LSHIFTEQUAL                    		
  | RSHIFTEQUAL                    		
  | URSHIFTEQUAL                   		
  | ANDEQUAL                       		
  | XOREQUAL                       		
  | OREQUAL                        		
  | MODEQUAL                       		
;

Expr:
    AssignmentExpr
  | Expr ',' AssignmentExpr        		
;

ExprNoIn:
    AssignmentExprNoIn
  | ExprNoIn ',' AssignmentExprNoIn     
;

ExprNoBF:
    AssignmentExprNoBF
  | ExprNoBF ',' AssignmentExpr         
;

Statement:
    Block
  | VariableDefinition
  | ClassDefinition                		
  | Attributes ClassDefinition
  | PackageDefinition
  | EmptyStatement
  | ExprStatement
  | IfStatement
  | IterationStatement
  | ContinueStatement
  | BreakStatement
  | ReturnStatement
  | WithStatement
  | SwitchStatement
  | LabelledStatement
  | ThrowStatement
  | TryStatement
  | ImportStatement
;

Block:
    '{' '}'                        		
  | '{' StatementList '}'          		
;

StatementList:
    Statement                      		
  | StatementList Statement        		
;

EmptyStatement:
    ';'                            		
;

ExprStatement:
    ExprNoBF ';'                        
  | ExprNoBF error                      { if (automatic()) { DBG($$, @1, @1); } else YYABORT; }
;

IfStatement:
    IF '(' Expr ')' Statement  %prec IF_WITHOUT_ELSE 
  | IF '(' Expr ')' Statement ELSE Statement         
;

IterationStatement:
	DO Statement WHILE '(' Expr ')' ';' { DBG($$,@1,@3); }
  | DO Statement WHILE '(' Expr ')'	error { DBG($$,@1,@3); }
  | WHILE '(' Expr ')' Statement   		{ DBG($$,@1,@4); }
  | FOR '(' ExprNoInOpt ';' ExprOpt ';' ExprOpt ')' Statement { DBG($$,@1,@8); }
  | FOR '(' VAR VariableBindingListNoIn ';' ExprOpt ';' ExprOpt ')' Statement	{ DBG($$,@1,@9); }
  | FOR '(' LeftHandSideExpr AQ_IN Expr ')' Statement { DBG($$,@1,@6); }
  | FOR '(' VAR VariableBindingNoIn AQ_IN Expr ')' Statement { DBG($$,@1,@7); }
;

ExprOpt:
    /* nothing */                  		
  | Expr
;

ExprNoInOpt:
    /* nothing */                  		
  | ExprNoIn
;

ContinueStatement:
    CONTINUE ';'                   		
  | CONTINUE error                    { if (automatic()) { DBG($$,@1,@2); } else YYABORT; }
  | CONTINUE IDENT ';'                
  | CONTINUE IDENT error              { if (automatic()) { DBG($$,@1,@2); } else YYABORT; }
;

BreakStatement:
    BREAK ';'                      		
  | BREAK error                       { if (automatic()) { DBG($$,@1,@1); } else YYABORT; }
  | BREAK IDENT ';'                   
  | BREAK IDENT error                 { if (automatic()) { DBG($$,@1,@2); } else YYABORT; }
;

ReturnStatement:
    RETURN ';'                     		
  | RETURN error                      { if (automatic()) { DBG($$,@1,@1); } else YYABORT; }
  | RETURN Expr ';'                		
  | RETURN Expr error                 { if (automatic()) { DBG($$,@1,@2); } else YYABORT; }
;

WithStatement:
    WITH '(' Expr ')' Statement       
;

SwitchStatement:
    SWITCH '(' Expr ')' CaseBlock     
;

CaseBlock:
    '{' CaseClausesOpt '}'         		
  | '{' CaseClausesOpt DefaultClause CaseClausesOpt '}'
                                   		
;

CaseClausesOpt:
    /* nothing */                  		
  | CaseClauses
;

CaseClauses:
    CaseClause                     		
  | CaseClauses CaseClause            
;

CaseClause:
    CASE Expr ':'                  		
  | CASE Expr ':' StatementList    		
;

DefaultClause:
    DEFAULT ':'                       
  | DEFAULT ':' StatementList      		
;

LabelledStatement:
    IDENT ':' IDENT '=' error       { DBG($$,@1,@3); YYABORT; }   
  | IDENT ':' Statement 
;

ThrowStatement:
    THROW Expr ';'                    
;

TryStatement:
    TRY Block Catch                		
  | TRY Block Finally              		
  | TRY Block Catch Finally        		
;

Catch:
    CATCH '(' IDENT ')' Block         
;

Finally:
    FINALLY Block                  		
;

PackageDefinition:
    PACKAGE PackageName FunctionBody  
;

ImportStatement:
    IMPORT PackageName ';'            
;

PackageName:
      IDENT                        		
    | PackageName '.' IDENT 
;

FunctionDeclaration:
    FUNCTION IDENT '(' ')' ResultSignature FunctionBody                                
  | FUNCTION IDENT '(' FormalParameterList ')' ResultSignature FunctionBody                                
  | Attributes FUNCTION IDENT '(' ')'  ResultSignature FunctionBody                                
  | Attributes FUNCTION IDENT '(' FormalParameterList ')' ResultSignature FunctionBody
;

ResultSignature: 
                                		
  | ':' TypeExpression             		
;

FunctionExpr:
    FUNCTION '(' ')' FunctionBody		
  | FUNCTION '(' FormalParameterList ')' FunctionBody                                   		
;

FormalParameterList:
    IDENT                             
  | IDENT ':' TypeExpression          
  | FormalParameterList ',' IDENT     
  | FormalParameterList ',' IDENT ':' TypeExpression                                      
;

FunctionBody:
    '{' '}'  /* TODO: spec ??? */     
  | '{' SourceElements '}'            
;

ClassDefinition:
    CLASS IDENT ';'                   
  | CLASS IDENT FunctionBody          
  | CLASS IDENT EXTENDS TypeExpression FunctionBody
                                      
;

Program:
    /* empty */                       
  | SourceElements                    
;

SourceElements:
    SourceElement                  		
  | SourceElements SourceElement   		
;

SourceElement:
    Statement                      		
  | FunctionDeclaration            		
;

Attributes:
    Attribute                      		
  | Attributes Attribute
;

Attribute:
    ABSTRACT                       		
  | FINAL                          		
  | PRIVATE                        		
  | PUBLIC                         		
  | STATIC                         		
  | TRUETOKEN                      		
  | FALSETOKEN                     		
  | CONSTRUCTOR                    		
;

/************************* Variable Definition ******************************/

VariableDefinition:
    VariableDefinitionKind VariableBindingList ';' { DBG($$, @1, @3); }                            			
  | VariableDefinitionKind VariableBindingList error { if (automatic()) {DBG($$, @1, @2); } else YYABORT; }
  | Attributes VariableDefinitionKind VariableBindingList ';' { DBG($$, @1, @4); }
  | Attributes VariableDefinitionKind VariableBindingList error { if (automatic()) { DBG($$, @1, @3); } else YYABORT }
;

VariableDefinitionKind:
    VAR 								
  | AQ_CONST 							
;

VariableBindingList:
    VariableBinding                		
  | VariableBindingList ',' VariableBinding                                   		
;

VariableBindingListNoIn:
    VariableBindingNoIn               	
  | VariableBindingListNoIn ',' VariableBindingNoIn                                   		
;

VariableBinding:
    TypedVariable                      	
  | TypedVariable '=' AssignmentExpr   	
;

VariableBindingNoIn:
    TypedVariable                      	
  | TypedVariable '=' AssignmentExprNoIn  										
;

TypedVariable:
    IDENT                             
  | IDENT ':' TypeExpression          
;

/************************* Type Expressions *********************************/

TypeExpression:
    IDENT                             
;

%%

int yyerror ( const char *errstr )  /* Called by yyparse on error */
{
  AQLexer::lexer()->setErrorMessage(errstr);
  return 1;
}

/* may we automatically insert a semicolon ? */
bool automatic()
{
  if ( yychar == '}' || yychar == 0 )
    return true;
  else if ( AQLexer::lexer()->prevTerminator() )
    return true;
  return false;
}

/****************************************************************************
** $Id$
**
** Copyright (C) 2001-2003 Trolltech AS.  All rights reserved.
**
** This file is part of the Qt Script for Applications framework (AQA).
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding a valid Qt Script for Applications license may use
** this file in accordance with the Qt Script for Applications License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about AQA Commercial License Agreements.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
*****************************************************************************/
