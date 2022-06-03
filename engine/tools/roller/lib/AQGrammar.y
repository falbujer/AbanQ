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
#include "AQNodes_p.h"

/* default values for bison */
#define YYDEBUG 1
//#define YYMAXDEPTH 0
# undef YYLSP_NEEDED
# define DBG(l, s, e)

extern int aqryylex();
static int yyerror (const char *);
static bool automatic();

%}

%union {
  int                   ival;
  double                dval;
  const QString         *ustr;
  const QString   *node;
  const QString   *stat;
  const QString   *param;
  const QString   *body;
  const QString   *func;
  const QString   *clss;
  const QString   *prog;
  const QString   *init;
  const QString   *src;
  const QString   *srcs;
  const QString   *slist;
  const QString   *args;
  const QString   *alist;
  const QString   *cblk;
  const QString   *clist;
  const QString   *ccl;
  const QString   *elm;
  const QString   *eli;
  const QString   *type;
  const QString   *tvar;
  const QString   *vbin;
  const QString   *blist;
  const QString   *attrs;
  const QString   *attr;
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
    NULLTOKEN                      	{$$ = new QString("null");}
  | TRUETOKEN                      	{$$ = new QString("true");}
  | FALSETOKEN                     	{$$ = new QString("false");}
  | NUMBER                         	{$$ = new QString(QString::number($1));}
  | STRING                          
  | '/'       /* a RegExp ? */      { AQRLexer *l = AQRLexer::lexer();if (!l->scanRegExp()) YYABORT; }
;

PropertyName:
    IDENT                          
  | STRING                         
  | NUMBER                         {$$ = new QString(QString::number($1));}
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
    THIS                           		{ADD_N(new AQThisNode(@1,@1));$$ = new QString("this");}
  | Literal
  | ArrayLiteral
  | IDENT                          		{ADD_N(new AQAccessorNode(@1,@1,*($1)));$$ = $1;}
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
  | MemberExpr '[' Expr ']'        		/*{ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}*/
  | MemberExpr '.' IDENT              {ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}
  | NEW MemberExpr Arguments          {ADD_N(new AQNewNode(@1,@3,*($2)));}
;

MemberExprNoBF:
    PrimaryExprNoBrace	
  | MemberExprNoBF '[' Expr ']'       /*{ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}*/
  | MemberExprNoBF '.' IDENT          {ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}
  | NEW MemberExpr Arguments       		{ADD_N(new AQNewNode(@1,@3,*($2)));}
;

NewExpr:
    MemberExpr
  | NEW NewExpr                     {ADD_N(new AQNewNode(@1,@2,*($2)));}
;

NewExprNoBF:
    MemberExprNoBF
  | NEW NewExpr							        {ADD_N(new AQNewNode(@1,@2,*($2)));}
;

CallExpr:
    MemberExpr Arguments            {ADD_N(new AQCallNode(@1,@2,*($1)));}
  | CallExpr Arguments              {ADD_N(new AQCallNode(@1,@2,*($1)));}
  | CallExpr '[' Expr ']'           /*{ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}*/
  | CallExpr '.' IDENT              {ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}
;

CallExprNoBF:
    MemberExprNoBF Arguments        {ADD_N(new AQCallNode(@1,@2,*($1)));}
  | CallExprNoBF Arguments          {ADD_N(new AQCallNode(@1,@2,*($1)));}
  | CallExprNoBF '[' Expr ']'       /*{ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}*/
  | CallExprNoBF '.' IDENT          {ADD_N(new AQAccessorNode(@3,@3,*($3)));$$ = $3;}
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
  | CONTINUE error                    { if (automatic()) { DBG($$,@1,@1); } else YYABORT; }
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
    RETURN ';'                     		{ADD_N(new AQReturnNode(@1,@1));}
  | RETURN error                      { if (automatic()) { DBG($$,@1,@1); } else { YYABORT; } ADD_N(new AQReturnNode(@1,@1));}
  | RETURN Expr ';'                		{ADD_N(new AQReturnNode(@1,@2));}
  | RETURN Expr error                 { if (automatic()) { DBG($$,@1,@2); } else { YYABORT; } ADD_N(new AQReturnNode(@1,@2));}
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
    FUNCTION IDENT '(' ')' ResultSignature FunctionBody {ADD_N(new AQFunctionNode(@1,@6,*($2)));}                              
  | FUNCTION IDENT '(' FormalParameterList ')' ResultSignature FunctionBody {ADD_N(new AQFunctionNode(@1,@7,*($2),*($4)));delete $4;}                                
  | Attributes FUNCTION IDENT '(' ')'  ResultSignature FunctionBody {ADD_N(new AQFunctionNode(@2,@7,*($3)));}                               
  | Attributes FUNCTION IDENT '(' FormalParameterList ')' ResultSignature FunctionBody {ADD_N(new AQFunctionNode(@2,@8,*($3),*($5)));delete $5;}
;

ResultSignature: 
                                		
  | ':' TypeExpression             		
;

FunctionExpr:
    FUNCTION '(' ')' FunctionBody		                  {AQFunctionNode *f = new AQFunctionNode(@1,@4); delete f;}
  | FUNCTION '(' FormalParameterList ')' FunctionBody {AQFunctionNode *f = new AQFunctionNode(@1,@5); delete f;}                                  		
;

FormalParameterList:
    IDENT                                             {$$ = $1;}
  | IDENT ':' TypeExpression                          {$$ = new QString(*($1) + ":" + *($3));}
  | FormalParameterList ',' IDENT                     {$$ = new QString(*($1) + ',' + *($3));}
  | FormalParameterList ',' IDENT ':' TypeExpression  {$$ = new QString(*($1) + ',' + *($3) + ":" + *($5));}                                    
;

FunctionBody:
    '{' '}'  /* TODO: spec ??? */     
  | '{' SourceElements '}'            
;

ClassDefinition:
    CLASS IDENT ';'                                   {ADD_N(new AQClassNode(@1,@3,*($2)));}
  | CLASS IDENT FunctionBody                          {ADD_N(new AQClassNode(@1,@3,*($2)));}
  | CLASS IDENT EXTENDS TypeExpression FunctionBody   {ADD_N(new AQClassNode(@1,@5,*($2),*($4)));}
                                      
;

Program:
    /* empty */                       
  | SourceElements                                    {new AQRootNode();}
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
    ABSTRACT                {$$ = new QString("abstract");}
  | FINAL                   {$$ = new QString("final");}
  | PRIVATE                 {$$ = new QString("private");}
  | PUBLIC                  {$$ = new QString("public");}
  | STATIC                  {$$ = new QString("static");}
  | TRUETOKEN               {$$ = new QString("true");}
  | FALSETOKEN              {$$ = new QString("false");}
  | CONSTRUCTOR             {$$ = new QString("constructor");}
;

/************************* Variable Definition ******************************/

VariableDefinition:
    VariableDefinitionKind VariableBindingList ';' { ADD_N(new AQVarNode(@1,@2,*($2),$1));delete $2;DBG($$, @1, @3); }
  | VariableDefinitionKind VariableBindingList error { if (automatic()) {ADD_N(new AQVarNode(@1,@2,*($2),$1));delete $2;DBG($$, @1, @2); } else YYABORT; }
  | Attributes VariableDefinitionKind VariableBindingList ';' { ADD_N(new AQVarNode(@2,@3,*($3),$2));delete $3;DBG($$, @1, @4); }
  | Attributes VariableDefinitionKind VariableBindingList error { if (automatic()) { ADD_N(new AQVarNode(@2,@3,*($3),$2));delete $3;DBG($$, @1, @3); } else YYABORT }
;

VariableDefinitionKind:
    VAR                                         {$$ = 0;}
  | AQ_CONST                                    {$$ = 1;}
;

VariableBindingList:
    VariableBinding                             {$$ = $1;}
  | VariableBindingList ',' VariableBinding     {$$ = new QString(*($1) + ',' + *($3));}
;

VariableBindingListNoIn:
    VariableBindingNoIn               	             {$$ = $1;}
  | VariableBindingListNoIn ',' VariableBindingNoIn  {$$ = new QString(*($1) + ',' + *($3));}                                 		
;

VariableBinding:
    TypedVariable                               {$$ = $1;}
  | TypedVariable '=' AssignmentExpr            {$$ = $1;}
;

VariableBindingNoIn:
    TypedVariable                      	        {$$ = $1;}
  | TypedVariable '=' AssignmentExprNoIn  			{$$ = $1;}							
;

TypedVariable:
    IDENT                                       {$$ = $1;}
  | IDENT ':' TypeExpression                    {$$ = $1;}
;

/************************* Type Expressions *********************************/

TypeExpression:
    IDENT                             
;

%%

int yyerror ( const char *errstr )  /* Called by yyparse on error */
{
  AQRLexer::lexer()->setErrorMessage(QString(errstr) + " " +
                                    AQRLexer::lexer()->errorMessage());
  return 1;
}

/* may we automatically insert a semicolon ? */
bool automatic()
{
  if ( yychar == '}' || yychar == 0 )
    return true;
  else if ( AQRLexer::lexer()->prevTerminator() )
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
