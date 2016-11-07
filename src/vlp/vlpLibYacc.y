
%{
/****************************************************************************
  FileName     [ vlpLibYacc.y ]
  Package      [ vlp ]
  Synopsis     [ Liberty parser ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include "VLGlobe.h"
#include "vlpItoStr.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void YYTRACE(string);
extern int liblex();
extern void liberror(char*);
// External Declarations
extern LibLY_usage* LibLY;
extern VlpDesign& VLDesign;

%}

/* YYSTYPE */
%union {
   int                 ival;
   char*               charPtr;
   string*             stringPtr;
   LibModule*          modulePtrLib;
   VlpBaseNode*        basePtr;
};

/* starting symbol */
%start source_text

/* tokens */
%token <stringPtr>LIBID
%token LIBLIBRARY
%token LIBCELL
%token LIBPIN
%token LIBFUNCTION
%token LIBDIRECTION 
%token LIBINPUT
%token LIBOUTPUT
%token LIBINOUT

%left '+'
%nonassoc '\''

%type <modulePtrLib> cell_item
%type <stringPtr> primary
%type <stringPtr> primary_list
%type <stringPtr> expression
%type <basePtr> function


%%

/* Source Text */
source_text
        : LIBLIBRARY '(' LIBID ')'
        {
           YYTRACE("source_text : LIBLIBRARY ( LIBID )");
        }
        '{' library_item  '}'
        {
           YYTRACE("source_text : LIBLIBRARY ( LIBID ) { library_item }");
        }
        ;
                                                       
library_item 
        : cell_item
        {
           YYTRACE("library_item : cell_item");
           VLDesign.setModule($1);
        }
        | library_item cell_item
        {
           YYTRACE("library_item : library_item cell_item");
           VLDesign.setModule($2);
        }
        ;
 
cell_item
        : LIBCELL '(' LIBID ')' 
        {
           YYTRACE("cell_item : LIBCELL { LIBID }");
           LibLY->modulePtrLib = new LibModule(*$3);
           //LibLY->modulePtrLib->setIsLibrary();
        }
        '{' statement_list '}'
        {
           YYTRACE("cell_item : { statement_list }");
           for (unsigned i = 0; i < (LibLY->strVector.size()); ++i) {
              VlpSignalNode* sn = new VlpSignalNode(BaseModule :: setName(LibLY->strVector[i]));
              LibLY->modulePtrLib->setIO(sn);
              VlpPortNode* pn = new VlpPortNode(NULL, NULL);
              pn->setIOType(input);
              pn->setNetType(wire);
              pn->setDriveStr(unspecStr, unspecStr);
              LibLY->modulePtrLib->setPort((LibLY->strVector[i]), pn);
              //cout << "pn = " << LibLY->strVector[i] << endl;
           }
           LibLY->strVector.clear();
           $$ = LibLY->modulePtrLib;
        }
        ;

statement_list
        : statement
        {
           YYTRACE("statement_list : statement");
        }
        | statement_list statement
        {
           YYTRACE("statement_list : statement_list statement");
        }
        ;     

statement
        : pin_statement
        {
           YYTRACE("statement : pin_statement");
        }
        ;

pin_statement
        : LIBPIN '(' LIBID ')'
        {
           YYTRACE("pin_statement : LIBPIN ( LIBID )");
           LibLY->pin = *$3;
        }
        '{' pin_item_list '}'
        {
           YYTRACE("pin_statement : LIBPIN ( LIBID ) { pin_item }");
        }
        ;

pin_item_list
        : pin_item 
        {
           YYTRACE("pin_item_list : pin_item");
        }
        | pin_item_list pin_item
        {
           YYTRACE("pin_item_list : pin_item_list pin_item");
        }
        ;
pin_item 
        : function
        {
           YYTRACE("pin_item : function");
           LibLY->modulePtrLib->setDataFlow($1);
        }
        | direction
        {
           YYTRACE("pin_item : direction");
           if (LibLY->ioType == 1) {
              LibLY->strVector.push_back(LibLY->pin);
           }
           else if ( LibLY->ioType == 2) {
              VlpSignalNode* sn = new VlpSignalNode(BaseModule :: setName(LibLY->pin));
              LibLY->modulePtrLib->setIO(sn);
              VlpPortNode* pn = new VlpPortNode(NULL, NULL);
              pn->setIOType(output);
              pn->setNetType(wire);
              pn->setDriveStr(unspecStr, unspecStr);
              LibLY->modulePtrLib->setPort(LibLY->pin, pn);
              //cout << "out = " << LibLY->pin << endl;
           }
           else {
              cerr << "NO support inout pin" << endl;
              exit (0);
           }
        }
        ;
 
function
        : LIBFUNCTION ':' '"' expression '"' ';'
        {
           YYTRACE("function : LIBFUNCTION : \" expression \" ;");
           VlpSignalNode* lvalue = new VlpSignalNode(BaseModule :: setName(LibLY->pin));
           VlpSignalNode* rvalue = new VlpSignalNode(BaseModule :: setName(*$4));

           VlpBAorNBA_Node* aNode = new VlpBAorNBA_Node(ASSIGNTYPE_BLOCK, lvalue, rvalue, NULL);
           $$ = new VlpAssignNode(aNode, NULL);
        }
        ;

direction
        : LIBDIRECTION ':' iotype ';'
        {
           YYTRACE("direction : LIBDIRECTION : iotype ;");
        }
        ;

iotype
        : LIBINPUT  { YYTRACE("iotype : LIBINPUT"); LibLY->ioType = 1; }
        | LIBOUTPUT { YYTRACE("iotype : LIBOUTPUT"); LibLY->ioType = 2; }
        | LIBINOUT  { YYTRACE("iotype : LIBINOUT"); LibLY->ioType = 3; }
        ;

expression
        : primary_list
        {
           YYTRACE("expression : primary_list");
           $$ = $1;
        }
        | expression '\''
        {
           YYTRACE("expression : expression '");
           VlpPrimitive* tmp = new VlpPrimitive;
           tmp->setKindID(10); // inv gate
           (LibLY->outName) = toString(LibLY->indexNum);
           (LibLY->outName) = "OUT" + (LibLY->outName);
           VlpSignalNode* sn = new VlpSignalNode(BaseModule :: setName(LibLY->outName));      
           tmp->setPosArr(sn);
           sn = new VlpSignalNode(BaseModule :: setName(*$1));      
           tmp->setPosArr(sn);
           LibLY->modulePtrLib->setPrim(tmp);
           (LibLY->indexNum)++;
           $$ = new string(LibLY->outName);

           VlpPortNode* pn = new VlpPortNode(NULL, NULL);
           pn->setIOType(unspecIO);
           pn->setNetType(wire);
           pn->setDriveStr(unspecStr, unspecStr);
           LibLY->modulePtrLib->setPort(LibLY->outName, pn);
        }
        | '(' expression ')'
        {
           YYTRACE("expression : ( expression )");
           $$ = $2;
        }
        | expression '+' { LibLY->inName1 = *$1; } expression 
        {
           YYTRACE("expression : expression + expression");
           VlpPrimitive* tmp = new VlpPrimitive;
           tmp->setKindID(3); // or gate
           (LibLY->outName) = toString(LibLY->indexNum);
           (LibLY->outName) = "OUT" + (LibLY->outName);
           VlpSignalNode* sn = new VlpSignalNode(BaseModule :: setName(LibLY->outName));      
           tmp->setPosArr(sn);
           sn = new VlpSignalNode(BaseModule :: setName(LibLY->inName1));      
           tmp->setPosArr(sn);
           sn = new VlpSignalNode(BaseModule :: setName(*$4));      
           tmp->setPosArr(sn);
           LibLY->modulePtrLib->setPrim(tmp);
           (LibLY->indexNum)++;
           $$ = new string(LibLY->outName);
           LibLY->inName1 = *$$;

           VlpPortNode* pn = new VlpPortNode(NULL, NULL);
           pn->setIOType(unspecIO);
           pn->setNetType(wire);
           pn->setDriveStr(unspecStr, unspecStr);
           LibLY->modulePtrLib->setPort(LibLY->outName, pn);
        }
        ;

primary_list 
        : 
        {
           YYTRACE("primary : ");
           $$ = NULL;
        }
        | primary_list primary
        {
           YYTRACE("primary_list : primary_list primary");
           if ($1 != NULL) {
              VlpPrimitive* tmp = new VlpPrimitive;
              tmp->setKindID(1); // and gate
              (LibLY->outName) = toString(LibLY->indexNum);
              (LibLY->outName) = "OUT" + (LibLY->outName);
              VlpSignalNode* sn = new VlpSignalNode(BaseModule :: setName(LibLY->outName));      
              tmp->setPosArr(sn);
              sn = new VlpSignalNode(BaseModule :: setName(LibLY->inName));      
              tmp->setPosArr(sn);
              sn = new VlpSignalNode(BaseModule :: setName(*$2));      
              tmp->setPosArr(sn);
              LibLY->modulePtrLib->setPrim(tmp);
              (LibLY->indexNum)++;
              $$ = new string(LibLY->outName);
              LibLY->inName = *$$;

              VlpPortNode* pn = new VlpPortNode(NULL, NULL);
              pn->setIOType(unspecIO);
              pn->setNetType(wire);
              pn->setDriveStr(unspecStr, unspecStr);
              LibLY->modulePtrLib->setPort(LibLY->outName, pn);
           }
           else {
              $$ = $2;
              LibLY->inName = *$2;
           }
        }
        ;

primary 
        : LIBID
        {
           YYTRACE("primary : LIBID");
           $$ = $1;
        }
        ;

%%

void liberror(char* str)
{
    cerr << "No support syntax in line " << LibLY->lineNoLib << endl;
    exit (1);
}



