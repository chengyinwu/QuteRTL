
%{
/****************************************************************************
  FileName     [ vlpYacc.y ]
  Package      [ vlp ]
  Synopsis     [ Verilog parser ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include "VLGlobe.h"
#include "vlpModule.h"
#include "vlpCtrlNode.h"
#include "vlpSubElement.h"
#include "vlpDesign.h"
#include "vlpItoStr.h"
#include "vlpStack.h"
#include "vlpDefine.h"
#include <string>
#include <stack>
#include <stdlib.h>
#include <iostream>
#include <set>

using namespace std;

//#define YYPARSE_PARAM param
//#define YYLEX_PARAM &yylval
//forward declare

//union YYSTYPE;
//extern int yylex(YYSTYPE*, void*);

//extern int yylex(YYSTYPE*);

extern int yylex();

void YYTRACE(string);
extern void yyerror(char*);
// External Declarations
extern LY_usage* LY;
extern VlpDesign& VLDesign;

%}

/* YYSTYPE */
%union {
   int             ival;
   string*         stringPtr;
   VlpDesign*      designPtr;
   VlgModule*      modulePtr;
   VlpIONode*      vlpIONodePtr;
   VlpInstance*    instancePtr;
   VlpBaseNode*    basePtr;
   VlpParamNode*   paramPtr;
   VlpAssignNode*  assignPtr;
   VlpAlwaysNode*  alwaysPtr;
   VlpPrimitive*   primitivePtr;
   VlpFunction*    functionPtr;
};


/* starting symbol */
%start source_text

/* tokens */
%token <stringPtr>YYID           /* verilog user defined identifier */
%token <stringPtr>YYDEF_ID       /*`mocro (Louis add)*/
%token <ival>YYINUMBER           /* integer */
%token YYRNUMBER                 /* real */
%token <stringPtr>YYBIT_BASE_STRING         /* Ex : 2'b11, 2'hff....  add by louis*/
/*%token YYSTRING*/                  /* string quoted by "" */
/*%token YYALLPATH*/                 /* *> */
%token YYALWAYS                  /* always */
%token YYAND                     /* and */
%token YYASSIGN                  /* assign */
%token YYBEGIN                   /* begin */
%token YYBUF                     /* buf */
%token YYBUFIF0                  /* bufif0 */
%token YYBUFIF1                  /* bufif1 */
%token YYCASE                    /* case */
%token YYCASEX                   /* casex */
%token YYCASEZ                   /* casez */
/*%token YYCMOS*/                    /* cmos */
/*%token YYCONDITIONAL*/             /* ?: */
/*%token YYDEASSIGN*/                /* deassign */
%token YYDEFAULT                 /* default */
%token YYDEFPARAM                /* defparam */
/*%token YYDISABLE*/                 /* disable */
%token YYELSE                    /* else */
%token YYEDGE                    /* edge */
%token YYEND                     /* end */
%token YYENDCASE                 /* endcase */
%token YYENDMODULE               /* endmodule */
%token YYENDFUNCTION             /* endfunction */
/*%token YYENDPRIMITIVE*/            /* endprimitive */
/*%token YYENDSPECIFY*/              /* endspecify */
/*%token YYENDTABLE*/                /* endtable */
%token YYENDTASK                 /* endtask */
/*%token YYEVENT*/                   /* event */
%token YYFOR                     /* for */
/*%token YYFOREVER*/                 /* forever */
/*%token YYFORK*/                    /* fork */
%token YYFUNCTION                /* function */
%token YYGEQ                     /* >= */
%token YYHIGHZ0                  /* highz0 */
%token YYHIGHZ1                  /* highz1 */
%token YYIF                      /* if */
/*%token YYINITIAL*/                 /* initial */
%token YYINOUT                   /* inout */
%token YYINPUT                   /* input */
%token YYINTEGER                 /* integer */
/*%token YYJOIN*/                    /* join */
/*%token YYLARGE*/                   /* large */
/*%token YYLEADTO*/                  /* => */
%token YYLEQ                     /* =< */
%token YYLOGAND                  /* && */
%token YYCASEEQUALITY            /* === */
%token YYCASEINEQUALITY          /* !== */
%token YYLOGNAND                 /* ~& */
%token YYLOGNOR                  /* ~| */
%token YYLOGOR                   /* || */
%token YYLOGXNOR                 /* ~^ or ^~ */
%token YYLOGEQUALITY             /* == */
%token YYLOGINEQUALITY           /* != */
%token YYLSHIFT                  /* << */
/*%token YYMACROMODULE*/             /* macromodule */
/*%token YYMEDIUM*/                  /* medium */
%token YYMODULE                  /* module */
/*%token YYMREG*/   	         /* mreg */
%token YYNAND                    /* nand */
%token YYNBASSIGN                /* <= non-blocking assignment */
%token YYNEGEDGE                 /* negedge */
/*%token YYNMOS*/                    /* nmos */
%token YYNOR                     /* nor */
%token YYNOT                     /* not */
/*%token YYNOTIF0*/                  /* notif0 */
/*%token YYNOTIF1*/                  /* notif1 */
%token YYOR                      /* or */
%token YYOUTPUT                  /* output */
%token YYPARAMETER               /* parameter */
/*%token YYPMOS*/                    /* pmos */
%token YYPOSEDGE                 /* posedge */
/*%token YYPRIMITIVE*/               /* primitive */
%token YYPULL0                   /* pull0 */
%token YYPULL1                   /* pull1 */
/*%token YYPULLUP*/                  /* pullup */
/*%token YYPULLDOWN*/                /* pulldown */
/*%token YYRCMOS*/                   /* rcmos */
/*%token YYREAL*/                    /* real */
%token YYREG                     /* reg */
/*%token YYREPEAT*/                  /* repeat */
/*%token YYRIGHTARROW*/              /* -> */
/*%token YYRNMOS*/                   /* rnmos */
/*%token YYRPMOS*/                   /* rpmos */
%token YYRSHIFT                  /* >> */
/*%token YYRTRAN*/                   /* rtran */
/*%token YYRTRANIF0*/                /* rtranif0 */
/*%token YYRTRANIF1*/                /* rtranif1 */
/*%token YYSCALARED*/                /* scalared */
/*%token YYSMALL*/                   /* small */
/*%token YYSPECIFY*/                 /* specify */
/*%token YYSPECPARAM*/               /* secparam */
%token YYSTRONG0                 /* strong0 */
%token YYSTRONG1                 /* strong1 */
%token YYSUPPLY0                 /* supply0 */
%token YYSUPPLY1                 /* supply1 */
%token YYSWIRE                   /* swire */
/*%token YYTABLE*/                   /* table */
%token YYTASK                    /* task */
/*%token YYTIME*/                    /* time */
/*%token YYTRAN*/                    /* tran */
/*%token YYTRANIF0*/                 /* tranif0 */
/*%token YYTRANIF1*/                 /* tranif1 */
%token YYTRI                     /* tri */
%token YYTRI0                    /* tri0 */
%token YYTRI1                    /* tri1 */
%token YYTRIAND                  /* triand */
%token YYTRIOR                   /* trior */
/*%token YYTRIREG*/                  /* trireg */
/*%token YYVECTORED*/                /* vectored */
/*%token YYWAIT*/                    /* wait */
%token YYWAND                    /* wand */
%token YYWEAK0                   /* weak0 */
%token YYWEAK1                   /* weak1 */
/*%token YYWHILE*/                   /* while */
%token YYWIRE                    /* wire */
%token YYWOR                     /* wor */
%token YYXNOR                    /* xnor */
%token YYXOR                     /* xor */
/*%token YYsysSETUP*/                /* $setup */
/*%token YYsysID*/                   /* $... */
%token YYDEFINE                  /*`define (add by Louis)*/
%token YYDEF_NULL                /*`define null*/
%token PARALLEL_CASE 
%token FULL_CASE   

//%right YYCONDITIONAL
%right '?' ':'
%left YYOR
%left YYLOGOR
%left YYLOGAND
%left '|'
%left '^' YYLOGXNOR
%left '&'
%left YYLOGEQUALITY YYLOGINEQUALITY YYCASEEQUALITY YYCASEINEQUALITY
%left '<' YYLEQ '>' YYGEQ YYNBASSIGN
%left YYLSHIFT YYRSHIFT
%left '+' '-'
//%left '*' '/' '%'
%left '*'
%right '~' '!' YYUNARYOPERATOR

//%type <A> source_text
%type <modulePtr> description
%type <modulePtr> module
//%type <A> primitive
//%type <A> port_list_opt port_list
%type <vlpIONodePtr> port
//%type <A> port_expression_opt port_expression
//%type <A> port_ref_list
//%type <A> port_reference
//%type <A> port_reference_arg
%type <basePtr> module_item
%type <basePtr> module_item_clr
%type <paramPtr> parameter_declaration
//%type <A> input_declaration output_declaration inout_declaration
//%type <A> reg_declaration
%type <assignPtr> net_declaration
//%type <A> time_declaration event_declaration
%type <basePtr> integer_declaration /*real_declaration*/
//%type <A> gate_instantiation
//%type <A> module_or_primitive_instantiation
%type <paramPtr> parameter_override
%type <assignPtr> continuous_assign
%type <alwaysPtr> /*initial_statement*/ always_statement
%type <basePtr> task
//%type <functionPtr> function
//%type <A> variable_list primitive_declaration_eclr
//%type <A> table_definition
//%type <A> table_entries combinational_entry_eclr sequential_entry_eclr
//%type <A> combinational_entry sequential_entry
//%type <A> input_list level_symbol_or_edge_eclr
//%type <A> output_symbol state next_state 
//%type <A> level_symbol edge level_symbol_or_edge edge_symbol
//%type <A> primitive_declaration
//%type <A> tf_declaration_clr tf_declaration_eclr
%type <basePtr> statement_opt
//%type <A> range_or_type_opt range_or_type
//%type <A> range
//%type <A> tf_declaration
%type <basePtr> assignment_list
//%type <A> range_opt
%type <basePtr> expression
//%type <A> drive_strength_opt drive_strength 
//%type <A> charge_strength_opt charge_strength
//%type <A> nettype
//%type <A> expandrange_opt expandrange
//%type <A> delay_opt delay
//%type <A> register_variable_list
//%type <A> name_of_event_list
%type <stringPtr> identifier
//%type <A> register_variable
%type <stringPtr> name_of_register
//%type <A> name_of_event
%type <ival> strength0 strength1
%type <basePtr> assignment
//%type <A> drive_delay_clr gate_instance_list
//%type <A> gatetype drive_delay
%type <primitivePtr> gate_instance
%type <primitivePtr> terminal_list
%type <stringPtr> name_of_gate_instance name_of_module_or_primitive
%type <basePtr> terminal
//%type <A> module_or_primitive_option_clr
//%type <A> module_or_primitive_option
//%type <A> module_or_primitive_instance_list
//%type <A> delay_or_parameter_value_assignment
%type <instancePtr> module_or_primitive_instance
%type <instancePtr> module_connection_list module_port_connection_list
%type <instancePtr> named_port_connection_list
%type <basePtr> module_port_connection named_port_connection
%type <basePtr> statement
%type <basePtr> statement_clr case_item_eclr
%type <basePtr> case_item
//%type <A> delay_control
%type <basePtr> event_control
%type <basePtr> lvalue
%type <basePtr> expression_list
%type <basePtr> seq_block
//%type <A> par_block
%type <basePtr> name_of_block          //dummy
%type <basePtr> block_declaration_clr  //dummy
//%type <A> block_declaration
%type <basePtr> task_enable
%type <basePtr> concatenation multiple_concatenation
//%type <A> mintypmax_expression_list
%type <basePtr> mintypmax_expression
%type <basePtr> primary
%type <basePtr> function_call
%type <basePtr> event_expression ored_event_expression
%type <basePtr> target_statement
%type <basePtr> bitExpression
%type <basePtr> bit_exp
%type <basePtr> connect_port

%%

/* F.1 Source Text */

source_text
        :
          {
             YYTRACE("sorce_text:");
          }
        | source_text description
          {
             YYTRACE("source_text: source_text description");
             if ($2 != NULL)
                VLDesign.setModule($2);
          }
        ;

description 
	: module
          {
             YYTRACE("description: module");
             $$ = $1;
             //cout << "# module = " << ++(LY->mod) << endl;
          }
        | define
          {
             YYTRACE("description: define");
             $$ = NULL;
          }
/*	| primitive
          {
             YYTRACE("description: primitive");
          }*/
	;


define                                           //new rule (added by Louis) 
        : YYDEFINE { (LY->state) = DEFINE; } YYID//can't use identifier instead of YYID,
                                                 //it will has a strange bug : the point of identifer
                                                 //will be modify to the next YYID token. I still don't know
                                                 //the reason, but use YYID is ok.
          { 
             BaseModule :: setName(*$3);
             YYTRACE("define: YYDEFINE YYID target_statement");
             LY->tempStr3 = *$3; 
          }
          target_statement
          {
             LY->defineMap.insert(LY->tempStr3, $5);
             LY->tempStr3 = "";
             (LY->state) = S_NULL;
          }
        ;

target_statement  //new rule 
        : expression
        {
           YYTRACE("target_statement: expression");
           $$ = $1;
        }
        | expression ':' expression
        {
           YYTRACE("target_statement: expression : expression");
           $$ = new VlpConnectNode($1, $3);
           BaseModule :: setCDFGNode($$);
        }
        | YYDEF_NULL
        {
           YYTRACE("target_statement: YYDEF_NULL");
           $$ = NULL;
        }
        ;

module 
	: YYMODULE YYID 
         { 
            LY->modulePtr = new VlgModule(*$2); 
            LY->modulePtr->setLineCol(LY->lineNo, (LY->colNo)-6-($2->size()) );
            if (LY->blackBox == true) 
               LY->modulePtr->setIsBlackBox();
         }
          port_list_opt ';'
          module_item_clr YYENDMODULE
         {
             YYTRACE("module: YYMODULE YYID port_list_opt ';' module_item_clr YYENDMODULE");
             $$ = LY->modulePtr;
         }
/*	| YYMACROMODULE YYID port_list_opt ';'
		module_item_clr
	  YYENDMODULE
          {
             YYTRACE("module: YYMACROMODULE YYID port_list_opt ';' module_item_clr YYENDMODULE");
          }*/
	;

port_list_opt               
	:
          {
             YYTRACE("port_list_opt:");
          }
	| '(' port_list ')'
          {
             YYTRACE("port_list_opt: '(' port_list ')'");
          }        
	| '(' io_declaration_list ')'//louis add
          {
             YYTRACE("port_list_opt: '(' io_declaration_list ')'");
          }
	;

port_list                      
	: port
          {
             YYTRACE("port_list: port");
          }
	| port_list ',' port
          {
             YYTRACE("port_list: port_list ',' port");
          }
	;

port                           
	: port_expression_opt
          {
             YYTRACE("port: port_expression_opt");
          }
/*	| '.' YYID      //maybe MacroModule or Primitive would use the rule           
          {             //but the parser doesn't supply them
	  }
          '(' port_expression_opt ')'
          {
             YYTRACE("port: ',' YYID '(' port_expression_opt ')'");
          }*/
	;

port_expression_opt            
	:
           {
              YYTRACE("port_expression_opt:");
              cerr << "Strange syntax in line " << LY->lineNo << endl;
              LY->modulePtr->setIO(NULL);              
           }
	|  port_expression
           {
              YYTRACE("port_expression_opt: port_expression");
           }
	;

port_expression                
	: port_reference
          {
             YYTRACE("port_expression: port_reference");
             LY->ioPtr = NULL;
          }
/*	| '{' port_ref_list '}'                               //Supply???
          {
             YYTRACE("port_expression: '{' port_ref_list '}'");
          }*/
        ;
/*
port_ref_list
	: port_reference
          {
             YYTRACE("port_ref_list: port_reference");
          }
	| port_ref_list ',' port_reference
          {
             YYTRACE("port_ref_list: port_ref_list ',' port_reference");
          }
	;
*/
port_reference
	: YYID 
          {
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*$1));
             BaseModule :: setCDFGNode(LY->ioPtr);
	  }
          port_reference_arg
          {
             YYTRACE("port_reference: YYID port_reference_arg");
             LY->modulePtr->setIO(LY->ioPtr);
          }
	;

port_reference_arg
        :
          {
             YYTRACE("port_reference_arg:");             
          }
        | '[' expression ']' /* expression must be constant expression */
          {
             YYTRACE("port_reference_arg: '[' expression ']'");
             if ($2->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression 
             {
                VlpBaseNode* msb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)($2))->getFirst()  ); 
                VlpBaseNode* lsb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)($2))->getSecond() ); 
                LY->ioPtr->setSigWidth(msb, lsb);
             }
             else
                LY->ioPtr->setSigWidth($2, $2);
          }
        | '[' expression ':' expression ']' /* expression must be constant */
          {
             YYTRACE("port_reference_arg: '[' expression ':' expression ']'");
             LY->ioPtr->setSigWidth($2, $4);
          }
        ;

io_declaration_list 
        : io_declaration
          {
          }
	| io_declaration_list ',' io_declaration
          {
          }
        ;

io_declaration   /*louis new add @ 07/01/16*/
        : YYINPUT range_opt identifier
          {
             YYTRACE("io_declaration: YYINPUT range_opt identifier");
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*$3), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(LY->ioPtr);
             LY->modulePtr->setIO(LY->ioPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(input);
             LY->modulePtr->setPort(*$3, target);
          }
        | YYOUTPUT range_opt identifier
          {
             YYTRACE("io_declaration: YYOUTPUT range_opt identifier");
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*$3), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(LY->ioPtr);
             LY->modulePtr->setIO(LY->ioPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(output);
             LY->modulePtr->setPort(*$3, target);
          }
        | YYINOUT range_opt identifier
          {
             YYTRACE("io_declaration: YYINOUT range_opt identifier");
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*$3), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(LY->ioPtr);
             LY->modulePtr->setIO(LY->ioPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(inout);
             LY->modulePtr->setPort(*$3, target);
          }
        ;

module_item_clr                          
        :
          {
             YYTRACE("module_item_clr:");
          }
        | module_item_clr module_item
          {
             YYTRACE("module_item_clr: module_item_clr module_item");
          }
        ;

module_item  /*only using in module descripation*/
	: parameter_declaration            
          {
             YYTRACE("module_item: parameter_declaration");
          }
	| input_declaration                 
          {
             YYTRACE("module_item: input_declaration");
          }
	| output_declaration                
          {
             YYTRACE("module_item: output_declaration");
          }
	| inout_declaration                 
          {
             YYTRACE("module_item: inout_declaration");
          }
	| net_declaration                   //special case will be linked in dataflow
          {
             YYTRACE("module_item: net_declaration");
             if ($1 != NULL) //special case: assignment_list
                LY->modulePtr->setDataFlow($1);
          }
	| reg_declaration                 
          {
             YYTRACE("module_item: reg_declaration");
          }
/*	| time_declaration
          {
             YYTRACE("module_item: time_declaration");
          }*/
	| integer_declaration             
          {
             YYTRACE("module_item: integer_declaration");
          }
/*	| real_declaration
          {
             YYTRACE("module_item: real_declaration");
             LY->modulePtr -> setDataFlow($1);
          }*/
/*	| event_declaration
          {
             YYTRACE("module_item: event_declaration");
          }*/
	| gate_instantiation                           
          {
             YYTRACE("module_item: gate_instantiation");
          }
	| module_or_primitive_instantiation            
          {
             YYTRACE("module_item: module_or_primitive_instantiation");
          }
	| parameter_override                             
          {
             YYTRACE("module_item: parameter_override");
          }
	| continuous_assign                                //link in dataflow
          {
             YYTRACE("module_item: continous_assign");
             LY->modulePtr->setDataFlow($1);
          }
/*	| specify_block
          {
             YYTRACE("module_item: specify_block");
          }
	| initial_statement
          {
             YYTRACE("module_item: initial_statement");
          }*/
	| always_statement                                 //link in dataflow
          {
             YYTRACE("module_item: always_statement");
             LY->modulePtr->setDataFlow($1);
             LY->allSensitive = false;
          }
	| task
          {
             YYTRACE("module_item: task");
          }
	| function
          {
             YYTRACE("module_item: function");
          }
        | define//Louis add
          {
             YYTRACE("description: define");
             $$ = NULL;
          }
	;
/*
primitive
	: YYPRIMITIVE YYID 
          {
          }
          '(' port_list ')' ';'
	  	primitive_declaration_eclr
		table_definition
	  YYENDPRIMITIVE
          {
             YYTRACE("primitive: YYPRMITIVE YYID '(' variable_list ')' ';' primitive_declaration_eclr table_definition YYENDPRIMITIVE");
          }
	;

primitive_declaration_eclr
        : primitive_declaration
          {
             YYTRACE("primitive_declaration_eclr: primitive_declaration");
          }
        | primitive_declaration_eclr primitive_declaration
          {
             YYTRACE("primitive_declaration_eclr: primitive_declaration_eclr primitive_declaration");
          }
        ;

primitive_declaration
	: output_declaration
          {
             YYTRACE("primitive_declaration: output_declaration");
          }
	| reg_declaration
          {
             YYTRACE("primitive_decalration: reg_declaration");
          }
	| input_declaration
          {
             YYTRACE("primitive_decalration: input_declaration");
          }
	;

table_definition
	: YYTABLE table_entries YYENDTABLE
          {
             YYTRACE("table_definition: YYTABLE table_entries YYENDTABLE");
          }
	;

table_entries
	: combinational_entry_eclr
          {
             YYTRACE("table_definition: combinational_entry_eclr");
          }
	| sequential_entry_eclr
          {
             YYTRACE("table_definition: sequential_entry_eclr");
          }
	;

combinational_entry_eclr
	: combinational_entry
          {
             YYTRACE("combinational_entry_eclr: combinational_entry");
          }
	| combinational_entry_eclr combinational_entry
          {
             YYTRACE("combinational_entry_eclr: combinational_entry_eclr combinational_entry");
          }
	;

combinational_entry
	: input_list ':' output_symbol ';'
          {
             YYTRACE("combinational_entry: input_list ':' output_symbol ';'");
          }
	;

sequential_entry_eclr
	: sequential_entry
          {
             YYTRACE("sequential_entry_eclr: sequential_entry");
          }
	| sequential_entry_eclr sequential_entry
          {
             YYTRACE("sequential_entry_eclr: sequential_entry_eclr sequential_entry");
          }
	;

sequential_entry
	: input_list ':' state ':' next_state ';'
          {
             YYTRACE("sequential_entry: input_list ':' state ':' next_state ';'");
          }
	;

input_list
	: level_symbol_or_edge_eclr
          {
             YYTRACE("input_list: level_symbol_or_edge_eclr");
          }
	;

level_symbol_or_edge_eclr
        : level_symbol_or_edge
          {
             YYTRACE("level_symbol_or_edge_eclr: level_symbol_or_edge");
          }
        | level_symbol_or_edge_eclr level_symbol_or_edge
          {
             YYTRACE("level_symbol_or_edge_eclr: level_symbol_or_edge_eclr level_symbol_or_edge");
          }
        ;

level_symbol_or_edge
        : level_symbol
          {
             YYTRACE("level_symbol_or_edge: level_symbol");
          }
        | edge
          {
             YYTRACE("level_symbol_or_edge: edge");
          }
        ;

edge
	: '(' level_symbol level_symbol ')'
          {
             YYTRACE("edge: '(' level_symbol level_symbol ')'");
          }
	| edge_symbol
          {
             YYTRACE("edge: edge_symbol");
          }
	;

state
	: level_symbol
          {
             YYTRACE("state: level_symbol");
          }
	;

next_state
	: output_symbol
          {
             YYTRACE("next_state: output_symbol");
          }
	| '-'
          {
             YYTRACE("next_state: '_'");
          }
	;

output_symbol
	: '0'
          {
             YYTRACE("output_symbol: '0'");
          }
        | '1'
          {
             YYTRACE("output_symbol: '1'");
          }
        | 'x'
          {
             YYTRACE("output_symbol: 'x'");
          }
        | 'X'
          {
             YYTRACE("output_symbol: 'X'");
          }
        ;

level_symbol
	: '0'
          {
             YYTRACE("level_symbol: '0'");
          }
        | '1'
          {
             YYTRACE("level_symbol: '1'");
          }
        | 'x'
          {
             YYTRACE("level_symbol: 'x'");
          }
        | 'X'
          {
             YYTRACE("level_symbol: 'X'");
          }
        | '?'
          {
             YYTRACE("level_symbol: '?'");
          }
        | 'b'
          {
             YYTRACE("level_symbol: 'b'");
          }
        | 'B'
          {
             YYTRACE("level_symbol: 'B'");
          }
        ;

edge_symbol
	: 'r'
          {
             YYTRACE("edge_symbol: 'r'");
          }
        | 'R'
          {
             YYTRACE("edge_symbol: 'R'");
          }
        | 'f'
          {
             YYTRACE("edge_symbol: 'f'");
          }
        | 'F'
          {
             YYTRACE("edge_symbol: 'F'");
          }
        | 'p'
          {
             YYTRACE("edge_symbol: 'p'");
          }
        | 'P'
          {
             YYTRACE("edge_symbol: 'P'");
          }
        | 'n'
          {
             YYTRACE("edge_symbol: 'n'");
          }
        | 'N'
          {
             YYTRACE("edge_symbol: 'N'");
          }
        | '*'
          {
             YYTRACE("edge_symbol: '*'");
          }
        ;
*/
task
	: YYTASK YYID  
          {
             LY->scopeState = TASK;
	     if (LY->modulePtr->lintTF(*$2) == true) {//linting
                cerr << "Error1 : The identifier " << *$2 << " at line "  << LY->lineNo 
                     << " has been declared"<< endl;
                exit(0);
             }//ony lint this, the function implement can be described later        
             LY->taskPtr = new VlpTask(*$2);
	  }
          ';' tf_declaration_clr statement_opt 
          YYENDTASK
          {
             YYTRACE("YYTASK YYID ';' tf_declaration_clr statement_opt YYENDTASK");
             LY->taskPtr->setStatement($6);
             LY->scopeState = MODULE;
             LY->taskPtr = NULL;
          }
	;

function
        : YYFUNCTION range_or_type_opt YYID 
          {
	     if (LY->modulePtr->lintTF(*$3) == true) {//linting
                cerr << "Error2 : The identifier " << *$3 << " at line "  << LY->lineNo 
                     << " has been declared"<< endl;
                exit(0);
             }//ony lint this, the function implement can be described later        
             VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*$3), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(tmp);
             LY->functionPtr = new VlpFunction(LY->isInteger, tmp);
             LY->modulePtr->setFunction(*$3, LY->functionPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(output);
             if (LY->isInteger == true)
                target->setNetType(integer);
             LY->functionPtr->setPort(*$3, target);

             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->scopeState = FUNCTION;
             LY->isInteger = false;
	  }
          ';' tf_declaration_eclr statement_opt
          YYENDFUNCTION
          {
             YYTRACE("YYFUNCTION range_or_type_opt YYID ';' tf_declaration_eclr statement_opt YYENDFUNCTION");
             LY->functionPtr->setStatement($7);
             LY->scopeState = MODULE;
             LY->functionPtr = NULL;
          }
        ;

range_or_type_opt     /*used in function description*/
        :
          {
             YYTRACE("range_or_type_opt:");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
          }
        | range_or_type
          {
             YYTRACE("range_or_type_opt: range_or_type");
          }
        ;

range_or_type         /*used in function description*/
        : range
          {
             YYTRACE("range_or_type: range");
             LY->isInteger = false;
          }
        | YYINTEGER
          {
             YYTRACE("range_or_type: YYINTEGER");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->isInteger = true;
          }
/*        | YYREAL
          {
             YYTRACE("range_or_type: YYREAL");
          }*/
        ;

tf_declaration_clr/*using in task description*/
        :
          {
             YYTRACE("tf_declaration_clr:");
          }
        | tf_declaration_clr tf_declaration
          {
             YYTRACE("tf_declaration_clr: tf_declaration_clr tf_declaration");
          }
        ;

tf_declaration_eclr/*using in function description*/
        : tf_declaration
          {
             YYTRACE("tf_declaration_eclr: tf_declaration");
          }
        | tf_declaration_eclr tf_declaration
          {
             YYTRACE("tf_declaration_eclr: tf_decalration_eclr tf_declaration");
          }
        ;

tf_declaration/*using in task and function description in the above rules*/
        : parameter_declaration
          {
             YYTRACE("tf_declaration: parameter_decalration");
          }
        | input_declaration
          {
             YYTRACE("tf_declaration: input_declaration");
          }
        | output_declaration
          {
             YYTRACE("tf_declaration: output_declaration");
             //function is not allow the output statement ==>lintting
             if (LY->scopeState == FUNCTION) {
                cerr << "Syntax Error3 : Can't have the output " 
                     << "statement in the \"function\" at line " << LY->lineNo << endl;
                exit(0);
             }                
          }
        | inout_declaration
          {
             YYTRACE("tf_declaration: inout_declaration");
             //function is not allow the output statement ==>lintting
             if (LY->scopeState == FUNCTION) {
                cerr << "Syntax Error4 : Can't have the inout " 
                     << "statement in the \"function\" at line " << LY->lineNo << endl;
                exit(0);
             }                
          }
        | reg_declaration
          {
             YYTRACE("tf_declaration: reg_declaration");
             //local variables
          }
/*      | time_declaration
          {
             YYTRACE("tf_declaration: time_declaration");
          }*/
        | integer_declaration
          {
             YYTRACE("tf_declaration: integer_declaration");
          }
/*      | real_declaration
          {
             YYTRACE("tf_declaration: real_declaration");
          }*/
/*      | event_declaration
          {
             YYTRACE("tf_declaration: event_declaration");
          }*/
        ;

/* F.2 Declarations */

parameter_declaration /*using in module, task, and function descripation*/
        : YYPARAMETER range_opt { (LY->state) = PARAM; } assignment_list ';'//range_opt is dummy! useless!!
          {                                                           //ignore the syntax "range_opt"
             YYTRACE("parameter_declaration: YYPARAMETER assignment_list ';'");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          }
        ;

input_declaration
        : YYINPUT range_opt 
          {
             (LY->state) = IO;
             LY->tempIOType = input;
          }
          variable_list ';'
          {
             YYTRACE("input_declaration: YYINPUT range_opt variable_list ';'");
             LY->tempIOType = unspecIO;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          }
        ;

output_declaration
        : YYOUTPUT range_opt 
          {
             (LY->state) = IO;
             LY->tempIOType = output;
          }
          variable_list ';'
          {
             YYTRACE("output_declaration: YYOUTPUT range_opt variable_list ';'");
             LY->tempIOType = unspecIO;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          }
        ;

inout_declaration
        : YYINOUT range_opt 
          {
             (LY->state) = IO;
             LY->tempIOType = inout;
          }
          variable_list ';'
          {
             YYTRACE("inout_declaration: YYINOUT range_opt variable_list ';'");
             LY->tempIOType = unspecIO;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL; 
             (LY->state) = S_NULL;
          }
        ;

net_declaration
        : nettype drive_strength_opt expandrange_opt /*delay_opt*/ assignment_list ';'//link in dataflow under 
          {                                                                           //the condition
             YYTRACE("net_declaration: nettype drive_strength_opt expandrange_opt delay_opt assignment_list ';'");
             (LY->tempNetType) = unspecNet;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->hStr = unspecStr;
             LY->lStr = unspecStr;
             (LY->state) = S_NULL;
             $$ = new VlpAssignNode( $4, NULL);
             BaseModule :: setCDFGNode($$);
          }
        | nettype drive_strength_opt expandrange_opt /*delay_opt*/ variable_list ';'
          {
             YYTRACE("net_declaration: nettype drive_strength_opt expandrange_opt delay_opt variable_list ';'");
             (LY->tempNetType) = unspecNet;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->hStr = unspecStr;
             LY->lStr = unspecStr;
             (LY->state) = S_NULL;
             $$ = NULL;
          }
/*      | YYTRIREG charge_strength_opt expandrange_opt delay_opt 
            variable_list ';'
          {
             YYTRACE("net_declaration: YYTRIREG charge_strength_opt expandrange_opt delay_opt variable_list ';'");
          }*/
        ;

nettype
        : YYSWIRE
          {
             YYTRACE("nettype: YYSWIRE");
             (LY->tempNetType) = swire;
             (LY->state) = NET;
          }
        | YYWIRE
          {
             YYTRACE("nettype: YYWIRE");
             (LY->tempNetType) = wire;
             (LY->state) = NET;
          }
        | YYTRI
          {
             YYTRACE("nettype: YYTRI");
             (LY->tempNetType) = tri;
             (LY->state) = NET;
          }
        | YYTRI1
          {
             YYTRACE("nettype: YYTRI1");
             (LY->tempNetType) = tri1;
             (LY->state) = NET;
          }
        | YYSUPPLY0
          {
             YYTRACE("nettype: YYSUPPLY0");
             (LY->tempNetType) = supply0Net;
             (LY->state) = NET;
          }
        | YYWAND
	  {
             YYTRACE("nettype: YYWAND");
             (LY->tempNetType) = wand;
             (LY->state) = NET;
	  }
        | YYTRIAND
          {
	     YYTRACE("nettype: YYTRIAND");
             (LY->tempNetType) = triand;
             (LY->state) = NET;
	  }
        | YYTRI0
          {
	     YYTRACE("nettype: YYTRI0");
             (LY->tempNetType) = tri0;
             (LY->state) = NET;
	  }
        | YYSUPPLY1
          {
	     YYTRACE("nettype: YYSUPPLY1");
             (LY->tempNetType) = supply1Net;
             (LY->state) = NET;
	  } 
        | YYWOR
          {
             YYTRACE("nettype: YYWOR");
             (LY->tempNetType) = wor;
             (LY->state) = NET;
          }
        | YYTRIOR
          {
             YYTRACE("nettype: YYTRIOR");
             (LY->tempNetType) = trior;
             (LY->state) = NET;
          }
       ;

/*
 * it's still not clear difference between <range>, SCALAREd <range>, and
 * VECTORED <range>. All of them are treated as <range> now. 10/8/92
 */
expandrange_opt
        :
          {
             YYTRACE("expandrange_opt:");
          }
        | expandrange
          {
             YYTRACE("expandrange_opt: expandrange");
          }
        ;

expandrange
        : range
          {
             YYTRACE("expandrange: range");
          }
/*      | YYSCALARED range
          {
             YYTRACE("expandrange: YYSCALARED range");
          }
        | YYVECTORED range
          {
             YYTRACE("expandrange: YYVECTORED range");
          }*/
        ;

reg_declaration
        : YYREG range_opt 
          {
             (LY->state) = REG; 
             YYTRACE("reg_declaration: YYREG range_opt");
          }
          register_variable_list ';'
          {
             YYTRACE("reg_declaration: YYREG range_opt register_variable_list ';'");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          }
/*      | YYMREG range_opt register_variable_list ';'
          {
             YYTRACE("reg_declaration: YYMREG range_opt register_variable_list ';'");
          }*/
        ;
/*
time_declaration
        : YYTIME register_variable_list ';'
          {
             YYTRACE("time_declaration: YYTIME register_variable_list ';'");
          }
        ;*/

integer_declaration
        : YYINTEGER 
          {
             (LY->state) = INT;
          }
          register_variable_list ';'
          {
             YYTRACE("integer_declaration: YYINTEGER register_variable_list ';'");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          }
        ;
/*
real_declaration
        : YYREAL variable_list ';'
          {
             YYTRACE("real_declaration: YYREAL variable_list ';'");
          }
        ;*/
/*
event_declaration
        : YYEVENT name_of_event_list ';'
          {
             YYTRACE("event_declaration: YYEVENT name_of_event_list ';'");
          }
        ;
*/
continuous_assign              
        : YYASSIGN { (LY->state) = ASSIGN; }/*drive_strength_opt delay_opt*/ assignment_list ';'
          {
             YYTRACE("continuous_assign: YYASSIGN drive_strength_opt delay_opt assignment_list ';'");
             $$ = new VlpAssignNode( $3, NULL);
             BaseModule :: setCDFGNode($$);
             (LY->state) = S_NULL;
          }
        ;
//====================================still need to modify====================================//
parameter_override
        : YYDEFPARAM { (LY->state) = PARAM_NOL; } assignment_list ';'//use parameter node
          {
             YYTRACE("parameter_override: YYDEFPARAM assign_list ';'");
             (LY->state) = S_NULL;         
          }
        ;
//=============================================================================================//
variable_list //for "input, output, inout, nettype, trireg, real" usage
        : identifier
          {
             YYTRACE("variable_list: identifier");
             VlpPortNode* target = NULL;
             if ((LY->state) == IO)//input, output, inout
             {   
                if (LY->scopeState == MODULE) {
                   if (LY->modulePtr->getPort(*$1, target)) //exist in map(declared)
                      target->setIOType(LY->tempIOType);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      LY->modulePtr->setPort(*$1, target);
                   }
                }
                else if (LY->scopeState == FUNCTION) {
                   if (LY->functionPtr->lintPort(*$1)) { //exist in map(declared)
                      cerr << "Error5 : re-declare identifier error at line " << LY->lineNo << endl;
                      exit(0);
                   }             
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      //tempIOType must be "input", this will be lint in the "tf_declaration"
                      LY->functionPtr->setPort(*$1, target);
                   }
                   
                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*$1), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   LY->functionPtr->setInput(tmp);
                }
                else  {//TASK condition
                   if (LY->taskPtr->lintIO(*$1)) { //exist in map(declared)
                      cerr << "Error6 : re-declare identifier error at line " << LY->lineNo << endl;
                      exit(0);
                   }
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      //LY->tempIOType must be "input", "output", or "inout" this will be lint in the "tf_declaration"
                      LY->taskPtr->setPort(*$1, target);
                   }
                   
                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*$1), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   LY->taskPtr->setIO(tmp);
                }
             } 
             else if ((LY->state) == NET)//nettype
             {
                //task and function can't declare nettype
                assert (LY->scopeState == MODULE); 
                if (LY->modulePtr->getPort(*$1, target)) { //exist in map(declared)
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                }
                else { // new in map
                   target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                   LY->modulePtr->setPort(*$1, target);
                }                 
             }
             else
                cerr << "Error7 : variable_list: identifier at line " << LY->lineNo << endl;
          }
        | variable_list ',' identifier
          {
             YYTRACE("variable_list: variable_list ',' identifier");
             VlpPortNode* target = NULL;
             if ((LY->state) == IO)//input, output, inout
             {
                if (LY->scopeState == MODULE) {
                   if (LY->modulePtr->getPort(*$3, target)) //exist in map(declared)
                      target->setIOType(LY->tempIOType);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      LY->modulePtr->setPort(*$3, target);
                   }
                }
                else if (LY->scopeState == FUNCTION) {
                   if (LY->functionPtr->getPort(*$3, target)) //exist in map(declared)
                      target->setIOType(LY->tempIOType);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      LY->functionPtr->setPort(*$3, target);
                   }

                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*$3), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   //LY->tempIOType must be "input", this will be lint in the "tf_declaration"
                   LY->functionPtr->setInput(tmp);                   
                }
                else  {//TASK condition
                   if (LY->taskPtr->lintIO(*$3)) { //exist in map(declared)
                      cerr << "Error8 : re-declare identifier error at line " << LY->lineNo << endl;
                      exit(0);
                   }
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      //LY->tempIOType must be "input", "output", or "inout" this will be lint in the "tf_declaration"
                      LY->taskPtr->setPort(*$3, target);
                   }
                   
                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*$3), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   LY->taskPtr->setIO(tmp);
                }
             }
             else if ((LY->state) == NET)//nettype
             {
                assert (LY->scopeState == MODULE); 
                if (LY->modulePtr->getPort(*$3, target)) {//exist in map(declared)
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                }
                else { // new in map
                   target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                   LY->modulePtr->setPort(*$3, target);
                }      
             }
             else
                cerr << "Error9 : variable_list: variable_list, identifier at line " << LY->lineNo << endl;
          }
        ;

register_variable_list    //use in reg and integer declaration
        : register_variable
          {
             YYTRACE("register_variable_list: register_variable");
          }
        | register_variable_list ',' register_variable
          {
             YYTRACE("register_variable_list: register_variable_list ',' register_variable");
          }
        ;
        
register_variable
        : name_of_register
          {
             YYTRACE("register_variable: name_of_register");
             VlpPortNode* target = NULL;
             if ((LY->state) == REG)//reg
             {  
                if (LY->scopeState == MODULE)
                {
                   if (LY->modulePtr->getPort(*$1, target))  //exist in map(declared)
                      target->setNetType(reg);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(reg);
                      LY->modulePtr->setPort(*$1, target);
                   }
                }
                else if (LY->scopeState == FUNCTION)
                {
                   if (LY->functionPtr->getPort(*$1, target))
                      target->setNetType(reg);
                   else {
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(reg);
                      LY->functionPtr->setPort(*$1, target);
                   }
                }
                else //TASK
                {
                   if (LY->taskPtr->getPort(*$1, target))
                      target->setNetType(reg);
                   else {
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(reg);
                      LY->taskPtr->setPort(*$1, target);
                   }
                }
             }
             else if ((LY->state) == INT)//integer
             {  
                if (LY->scopeState == MODULE)  
                {
                   if (LY->modulePtr->getPort(*$1, target)) {  //exist in map(declared)
                      target->setNetType(integer);
                      cerr << "Error10 : re-declare integer error at line " << LY->lineNo << endl;
                   }
                   else { // new in map
                      target = new VlpPortNode(NULL, NULL);
                      target->setNetType(integer);
                      LY->modulePtr->setPort(*$1, target);
                   }
                }
                else if (LY->scopeState == FUNCTION)
                {
                   if (LY->functionPtr->lintPort(*$1)) {
                      cerr << "Error11 : re-declare integer error at line " << LY->lineNo << endl;
                   }
                   else { // new in map
                      target = new VlpPortNode(NULL, NULL);
                      target->setNetType(integer);
                      LY->functionPtr->setPort(*$1, target);
                   }
                }
                else { //TASK 
                   if (LY->taskPtr->lintPort(*$1)) {
                      cerr << "Error12 : re-declare integer error at line " << LY->lineNo << endl;
                   }
                   else {
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(integer);
                      LY->taskPtr->setPort(*$1, target);
                   }
                }
             }
             else
                cerr << "Error13 : register_variable: name_of_register at line " << LY->lineNo << endl;
          }
        | name_of_register '[' expression ':' expression ']'//Arrays  Ex:  reg  [7:0]Memory[0:1023]                     //need to test
          {                                                 //ref. 5-11 in Verilog book
             YYTRACE("register_variable: name_of_register '[' expression ':' expression ']'");
             VlpPortNode* target = NULL;
             if (LY->modulePtr->getPort(*$1, target))
                cerr << "Error14 : re-declare Array error at line " << LY->lineNo << endl;
             else
                BaseModule :: setName(*$1);
             if ((LY->state) == REG) {
                VlpMemAry* memAryPtr = new VlpMemAry(*$1, LY->msbPtr, LY->lsbPtr, $3, $5);
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setMemAry(memAryPtr);
                else if (LY->scopeState == FUNCTION)    
                   LY->functionPtr->setMemAry(memAryPtr);
                else //TASK                                       
                   LY->taskPtr->setMemAry(memAryPtr);
             }
             else if ((LY->state) == INT) {
                VlpPortNode* target = new VlpPortNode($3, $5);
                target->setNetType(integer);
                if (LY->scopeState == MODULE)                
                   LY->modulePtr->setPort(*$1, target);
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setPort(*$1, target);
                else  //TASK  
                   LY->taskPtr->setPort(*$1, target);
             }                                           
             else
                cerr << "Error15 : name_of_register [ expression : expression ] at line " << LY->lineNo << endl;
          }
        | name_of_register '[' YYDEF_ID ']' // Louis adds the rule used in mocro definite.
          {
             YYTRACE("register_variable: name_of_register '[' YYDEF_ID ']'");

             VlpPortNode* target = NULL;
             if (LY->modulePtr->getPort(*$1, target))
                cerr << "Error16 : re-declare Array error at line " << LY->lineNo << endl;
             else
                BaseModule :: setName(*$1);

             const VlpBaseNode* tmp;
             if (LY->defineMap.getData(*$3 ,tmp) == false)
             {
                cerr << "Error17 : the macro " << *$3 << " isn't defined at line " << LY->lineNo << endl;
                exit(0);
             }
             assert (tmp->getNodeClass() == NODECLASS_CONNECT);// Ex `define bus 7:0
                
             VlpBaseNode* msb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getFirst()  ); 
             VlpBaseNode* lsb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getSecond() ); 

             if ((LY->state) == REG) {
                VlpMemAry* memAryPtr = new VlpMemAry(*$1, LY->msbPtr, LY->lsbPtr, msb, lsb);
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setMemAry(memAryPtr);
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setMemAry(memAryPtr);
                else  // TASK   
                   LY->taskPtr->setMemAry(memAryPtr);
             }
             else if ((LY->state) == INT)
             {
                VlpPortNode* target = new VlpPortNode(msb, lsb);
                target->setNetType(integer);
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setPort(*$1, target);
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setPort(*$1, target);
                else //TASK    
                   LY->taskPtr->setPort(*$1, target);
             }
             else
                cerr << "Error18 : name_of_register [ expression : expression ] at line " << LY->lineNo << endl;
          }
        ;

name_of_register
        : YYID
          {
             YYTRACE("name_of_register: YYID");   
             $$ = $1;
          }
        ;
/*
name_of_event_list
        : name_of_event
          {
             YYTRACE("name_of_event_list: name_of_event");
          }
        | name_of_event_list ',' name_of_event
          {
             YYTRACE("name_of_event_list: name_of_event_list ',' name_of_event");
          }
        ;

name_of_event
        : YYID
          {
             YYTRACE("name_of_event: YYID");
          }
        ;
*/
/*
charge_strength_opt   //SUPPLY??   //use only in "trireg"
        :
          {
             YYTRACE("charge_strength_opt:");
          }
        | charge_strength //use only in "trireg"
          {
             YYTRACE("charge_strength_opt: charge_strength");
          }
        ;

charge_strength      //SUPPLY??   //use only in "trireg"
        : '(' YYSMALL ')'
          {
             YYTRACE("charge_strength: '(' YYSMALL ')'");
          }
        | '(' YYMEDIUM ')'
          {
             YYTRACE("charge_strength: '(' YYMEDIUM ')'");
          }
        | '(' YYLARGE ')'
          {
             YYTRACE("charge_strength: '(' YYLARGE ')'");
          }
        ;
*/
drive_strength_opt  
        :
          {
             YYTRACE("drive_strength_opt:");
             LY->hStr = unspecStr;
             LY->lStr = unspecStr;
          }
        | drive_strength
          {
             YYTRACE("drive_strength_opt: drive_strength");
          }
        ;

drive_strength     
        : '(' strength0 ',' strength1 ')'
          {
             YYTRACE("drive_strength: '(' strength0 ',' strength1 ')'");
             LY->hStr = (DriveStr)$2;
             LY->lStr = (DriveStr)$4;             
          }
        | '(' strength1 ',' strength0 ')'
          {
             YYTRACE("drive_strength: '(' strength1 ',' strength0 ')'");
             LY->hStr = (DriveStr)$2;
             LY->lStr = (DriveStr)$4;             
          }
        ;

strength0    
        : YYSUPPLY0
          {
             YYTRACE("strength0: YYSUPPLY0");
             $$ = supply0;           
          }
        | YYSTRONG0
          {
             YYTRACE("strength0: YYSTRONG0");
             $$ = strong0;
          }
        | YYPULL0
          {
             YYTRACE("strength0: YYPULL0");
             $$ = pull0;
          }
        | YYWEAK0
          {
             YYTRACE("strength0: YYWEAK0");
             $$ = weak0;
          }
        | YYHIGHZ0
          {
             YYTRACE("strength0: YYHIGHZ0");
             $$ = highz0;
          }
        ;

strength1   
        : YYSUPPLY1
          {
             YYTRACE("strength1: YYSUPPLY1");
             $$ = supply1;
          }
        | YYSTRONG1
          {
             YYTRACE("strength1: YYSTRONG1");
             $$ = strong1;
          }
        | YYPULL1
          {
             YYTRACE("strength1: YYPULL1");
             $$ = pull1;
          }
        | YYWEAK1
          {
             YYTRACE("strength1: YYWEAK1");
             $$ = weak1;
          }
        | YYHIGHZ1
          {
             YYTRACE("strength1: YYHIGHZ1");
             $$ = highz1;
          }
        ;

range_opt//used in "input" "output" "inout" "reg"
        :
          {
             YYTRACE("range_opt:");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
          }
        | range
          {
             YYTRACE("range_opt: range");
          }
        ;

range    //range_opt(input, output, inout, reg), range_or_type(function), expandrange(nettype)   
        : '[' expression ':' expression ']'
          {
             YYTRACE("range: '[' expression ':' expression ']'");
             LY->msbPtr = $2;
             LY->lsbPtr = $4;
          }
        | '[' YYDEF_ID ']'
          {
             YYTRACE("range: '[' YYDEF_ID ']'");
             const VlpBaseNode* tmp;
             if (LY->defineMap.getData(*$2 ,tmp) == false)
             {
                cerr << "Error19 : the macro " << *$2 << " isn't defined at line " << LY->lineNo << endl;
                exit(0);
             }
             assert (tmp->getNodeClass() == NODECLASS_CONNECT);// Ex `define bus 7:0
                
             LY->msbPtr = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getFirst()  ); 
             LY->lsbPtr = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getSecond() ); 
          }
        ;

//assignment_list using in 
//    1. parameter(parameter_declaration)
//    2. nettype(net_declaration)(swire wire wand wor tri tri0 tri1 triand trior supply0 supply1)
//    3. assign(continuous_assign)
//    4. defparam(parameter_override)
assignment_list  
        : assignment
          {//linking
             YYTRACE("assignment_list: assignment");
             if ((LY->state) != PARAM && (LY->state) != PARAM_NOL) {
                LY->nodeHeadPtr1 = $1;
                LY->nodeTailPtr1 = $1;
                $$ = LY->nodeHeadPtr1;
             }
          }
        | assignment_list ',' assignment
          {
             YYTRACE("assignment_list: assignment_list ',' assignment");
             if ((LY->state) != PARAM && (LY->state) != PARAM_NOL) {
                LY->nodeTailPtr1 -> setNext($3);
                LY->nodeTailPtr1 = $3;       
             }      
          }
        ;

/* F.3 Primitive Instances */

gate_instantiation
        : gatetype /*drive_delay_clr*/ gate_instance_list ';'
          {
             YYTRACE("gate_instantiation: gatetype drive_delay_clr gate_instance_list ';'");
             LY->gateType = 0;
          }
        ;
/*
drive_delay_clr
        :
          {
             YYTRACE("drive_delay_clr:");
          }
        | drive_delay_clr drive_delay
          {
             YYTRACE("drive_delay_clr: drive_delay_clr drive_delay");
          }
        ;*/
/*
drive_delay
        : drive_strength
          {
             YYTRACE("drive_delay: drive_strength");
          }
        | delay
          {
             YYTRACE("drive_delay: delay");*/
	      /* to save space and encode/decode effort */
              /* we use lsb as an indicator whether drive_delay */
	      /* is drive (lsb==0) or delay (lsb==1) */
//        }
//      ;

gatetype
        : YYAND
          {
	     YYTRACE("gatetype: YYAND");
             LY->gateType = 1;
	  }
        | YYNAND
          {
	     YYTRACE("gatetype: YYNAND");
             LY->gateType = 2;
	  }
        | YYOR
          {
	     YYTRACE("gatetype: YYOR");
             LY->gateType = 3;
	  }
        | YYNOR
          {
             YYTRACE("gatetype: YYNOR");
             LY->gateType = 4;
	  }
        | YYXOR
          {
	     YYTRACE("gatetype: YYXOR");
             LY->gateType = 5;
	  }
        | YYXNOR
          {
	     YYTRACE("gatetype: YYXNOR");
             LY->gateType = 6;
	  }
        | YYBUF
          {
	     YYTRACE("gatetype: YYBUF");
             LY->gateType = 7;
	  }
        | YYBUFIF0
          {
             YYTRACE("gatetype: YYBIFIF0");
             LY->gateType = 8;
	  }
        | YYBUFIF1
          {
	     YYTRACE("gatetype: YYBIFIF1");
             LY->gateType = 9;
	  }
        | YYNOT
          {
	     YYTRACE("gatetype: YYNOT");
             LY->gateType = 10;
	  }
/*      | YYNOTIF0
          {
	     YYTRACE("gatetype: YYNOTIF0");
	  }
        | YYNOTIF1
          {
	     YYTRACE("gatetype: YYNOTIF1");
	  }
        | YYPULLDOWN
          {
             YYTRACE("gatetype: YYPULLDOWN");
	  }
        | YYPULLUP
          {
	     YYTRACE("gatetype: YYPULLUP");
	  }
        | YYNMOS
          {
	     YYTRACE("gatetype: YYNMOS");
	  }
        | YYPMOS
          {
             YYTRACE("gatetype: YYPMOS");
	  }
        | YYRNMOS
          {
	     YYTRACE("gatetype: YYRNMOS");
	  }
        | YYRPMOS
          {
	     YYTRACE("gatetype: YYRPMOS");
	  }
        | YYCMOS
          {
             YYTRACE("gatetype: YYCMOS");
	  }
        | YYRCMOS
          {
	     YYTRACE("gatetype: YYRCMOS");
	  }
        | YYTRAN
          {
	     YYTRACE("gatetype: YYTRAN");
	  }
        | YYRTRAN
          {
             YYTRACE("gatetype: YYRTRAN");
	  }
        | YYTRANIF0
          {
	     YYTRACE("gatetype: YYTRANIF0");
	  }
        | YYRTRANIF0
          {
	     YYTRACE("gatetype: YYRTRANIF0");
	  }
        | YYTRANIF1
          {
	     YYTRACE("gatetype: YYTRANIF1");
	  }
        | YYRTRANIF1
          {
	     YYTRACE("gatetype: YYRTRANIF1");
	  }*/
        ;

gate_instance_list
        : gate_instance
          {
             YYTRACE("gate_instance_list: gate_instance");
             $1->setKindID(LY->gateType);
             LY->modulePtr->setPrim($1);            
          }
        | gate_instance_list ',' gate_instance
          {
             YYTRACE("gate_instance_list: gate_instance_list ',' gate_instance");
             $3->setKindID(LY->gateType);
             LY->modulePtr->setPrim($3);            
          }
        ;

gate_instance
        : '(' terminal_list ')'//omit gate name
          {
             YYTRACE("gate_instance: '(' terminal_list ')'");
             $$ = $2;
          }
        | name_of_gate_instance '(' terminal_list ')'
          {
             YYTRACE("gate_instance: name_of_gate_instance '(' terminal_list ')'");
             $3->setNameID(BaseModule :: setName(*$1));
             $$ = $3;
          }
        ;

name_of_gate_instance
        : YYID
          {
             YYTRACE("name_of_gate_instance: YYID");
             $$ = $1;
          }
        ;

terminal_list
        : terminal
          {
             YYTRACE("terminal_list: terminal");
             $$ = new VlpPrimitive;
             $$->setPosArr($1); 
          }
        | terminal_list ',' terminal
          {
             YYTRACE("terminal_list: terminal_list ',' terminal");
             $1->setPosArr($3);
             $$ = $1;
          }
        ;

terminal
        : expression
          {
             YYTRACE("terminal: expression");
             $$ = $1;
          }
        ;

/* F.4 Module Instantiations */

module_or_primitive_instantiation
        : name_of_module_or_primitive module_or_primitive_option_clr
             module_or_primitive_instance_list ';' /*delay and drive strength don't supply*/
          {
             YYTRACE("module_or_primitive_instantiation: name_of_module_or_primitive module_or_primitive_option_clr module_or_primitive_instance_list ';'");
             LY->UDM_No = -1;
             LY->array.clear();
          }
        ;

name_of_module_or_primitive
        : YYID
          {
             YYTRACE("name_of_module_or_primitive: YYID");
             LY->UDM_No = BaseModule :: setName(*$1);
          }
        | YYDEF_ID
          {
             YYTRACE("name_of_module_or_primitive: YYDEF_ID");
             const VlpBaseNode* tmp;
             if (LY->defineMap.getData(*$1 ,tmp) == false)
             {
                cerr << "Error36 : the macro " << *$1 << " isn't defined at line " << LY->lineNo << endl;
                exit(0);
             }
             if (tmp->getNodeClass() != NODECLASS_SIGNAL) {
                cerr << "Error37 : Error definite of macro name using in instance name" << endl;
                exit(0);
             }
             else
                LY->UDM_No = (static_cast<const VlpSignalNode*>(tmp))->getNameId();
          }
        ;

//==========================================still not complete========================================// 

module_or_primitive_option_clr
        :
          {
             YYTRACE("module_or_primitive_option_clr:");
          }
        | module_or_primitive_option_clr module_or_primitive_option
          {
             YYTRACE("module_or_primitive_option_clr: module_or_primitive_option_clr module_or_primitive_option");
          }
/*        | module_or_primitive_option // modify syntax above by Louis
          {
             YYTRACE("module_or_primitive_option_clr: module_or_primitive_option");
          }*/
        ;

module_or_primitive_option // only support parameter, no support drive_strength and delay
/*        : drive_strength
          {
             YYTRACE("module_or_primitive_option:");
          }*/
        : delay_or_parameter_value_assignment // only support parameter
          {
             YYTRACE("module_or_primitive_option: delay");
          }
        ;

delay_or_parameter_value_assignment
        : '#' YYINUMBER    // delay 
          {
	     YYTRACE("delay_or_parameter_value_assignment: '#' YYINUMBER");
             //ignore!!
	  }
        | '#' YYRNUMBER    // delay 
          {
	     YYTRACE("delay_or_parameter_value_assignment: '#' YYRNUMBER");
             //ignore!!
	  }
        | '#' identifier   // delay 
          {
	     YYTRACE("delay_or_parameter_value_assignment: '#' identifier");
             //ignore!!
	  }
        | '#' '(' mintypmax_expression_list ')' // delay | parameter_assign 
          {
	     YYTRACE("delay_or_parameter_value_assignment: '#' '(' mintypmax_expression_list ')'");
	  }
        ;

//========================================================================================================// 
module_or_primitive_instance_list
        : module_or_primitive_instance
          {
             YYTRACE("module_or_primitive_instance_list: module_or_primitive_instance");
             $1->setModuleID(LY->UDM_No);

             if (LY->array.size() != 0) {
                for (unsigned i = 0; i < LY->array.size(); ++i) 
                   $1->setPOL(LY->array[i]);
             }
             LY->modulePtr->setInst($1);            
          }
        | module_or_primitive_instance_list ',' module_or_primitive_instance
          {
             YYTRACE("module_or_primitive_instance_list: module_or_primitive_instance_list ',' module_or_primitive_instance");
             $3->setModuleID(LY->UDM_No);

             if (LY->array.size() != 0) {
                for (unsigned i = 0; i < LY->array.size(); ++i)
                   $3->setPOL(LY->array[i]);
             }
             LY->modulePtr->setInst($3);            
          }
        ;

/* terminal_list in primitive_instance ca be derived from 
 * module_connection -> module_port ->expression
 */
module_or_primitive_instance
        : '(' module_connection_list ')'
          {
             YYTRACE("module_or_primitive_instance: '(' module_connection_list ')'");
             $$ = $2;
          }
        | identifier { LY->tempStr2 = *$1; } '(' module_connection_list ')'
          {
             YYTRACE("module_or_primitive_instance: '(' module_connection_list ')'");
             $4->setName(LY->tempStr2);
             LY->tempStr2 = "";
             $$ = $4;
          }
        ;

module_connection_list
        : module_port_connection_list
          {
             YYTRACE("module_connection_list: module_port_connection_list");
             $1->setPosMap();
             $$ = $1;
          }
        | named_port_connection_list
          {
             YYTRACE("module_connection_list: named_port_connection_list");
             $1->setNameMap();
             $$ = $1;
          }
        ;

module_port_connection_list
        : module_port_connection
          {
             YYTRACE("module_port_connection_list: module_port_connection");
             $$ = new VlpInstance;
             $$->setPosArr($1);               
          }
        | module_port_connection_list ',' module_port_connection
          {
             YYTRACE("module_port_connection_list: module_port_connection_list ',' module_port_connection");
             $$->setPosArr($3);
          }
        ;

named_port_connection_list
        : named_port_connection
          {
             YYTRACE("named_port_connection_list: named_port_connection");
             $$ = new VlpInstance;
             $$->setPosArr($1);
             $$->setNameArr(BaseModule :: setName(LY->tempStr1));
             LY->tempStr1 = "";
          }
        | named_port_connection_list ',' named_port_connection
          {
             YYTRACE("named_port_connection_list: named_port_connection_list ',' name_port_connection");
             $1->setPosArr($3);
             $1->setNameArr(BaseModule :: setName(LY->tempStr1));
             LY->tempStr1 = "";
             $$ = $1;
          }
        ;

module_port_connection
        :
          {
             YYTRACE("module_port_connection:");
             $$ = NULL;
          }
        | expression
          {
             YYTRACE("module_port_connection: expression");
             $$ = $1;
          }
        ;

named_port_connection
        : '.' identifier { LY->tempStr1 = *$2; } '(' connect_port ')'
          {
             YYTRACE("named_port_connection: '.' identifier '(' connect_port ')'");
             $$ = $5;
          }
        ;

connect_port  //new rule
        :
          {
             YYTRACE("connect_port:");
             $$ = NULL;
          } 
        | expression
          {
             YYTRACE("connect_port: expression");
             $$ = $1;
          }
        ;

/* F.5 Bahavioral Statements */
/*
initial_statement
        : YYINITIAL statement
          {
             YYTRACE("initial_statement: YYINITIAL statement");
          }
        ;
*/
always_statement
        : YYALWAYS statement                                                
          {
             if ((LY->state) == EVENT)
             {
                YYTRACE("always_statement: YYALWAYS statement");
                $$ = new VlpAlwaysNode(LY->alwaysEventPtr, $2, NULL);
                BaseModule :: setCDFGNode($$);
                (LY->state) = S_NULL;
             }
             else
                cerr << "No supply syntax in always_statement at line " << LY->lineNo << endl;
          }
        ;

statement_opt   //using task and function
        :
          {
             YYTRACE("statement_opt:");
             $$ = NULL;
          }
        | statement
          {
             YYTRACE("statement_opt: statement");
             $$ = $1;
          }
        ;     

statement_clr /* using in "seq_block : YYBEGIN statement_clr YYEND" */      /*linking*/
        :
          {
             YYTRACE("statement_clr:");
             $$ = NULL;
          }
        | statement_clr statement                                           
          {
             YYTRACE("statement_clr: statement_clr statement");
             if ($1 == NULL)
             {
                LY->stack_s.push($2);
                $$ = $2;
             }
             else
             {
                $1->setNext($2);
                if ($2 != NULL)
                   $$ = $2;
                else
                   $$ = $1;
                   
             }
          }
        ;

statement
        : ';'//The three rules {assignment ';'}, {lvalue YYNBASSIGN expression ';'},
          {  //and {lvalue '=' expression ';'} won't use this.
             YYTRACE("statement: ';'");
             $$ = NULL;
          }
        | assignment ';' //The rule includes => statement : lvalue '=' expression ';'
          {              //                               | lvalue YYNBASSIGN delay_control expression ';'
             YYTRACE("statement: assignment ';'");
             $$ = $1;
          }
        | YYIF '(' expression ')' statement
          {
             YYTRACE("statement: YYIF '(' expression ')' statement");
             $$ = new VlpIfNode($3, $5, NULL, NULL);
             BaseModule :: setCDFGNode($$);

             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
          }
        | YYIF '(' expression ')' statement YYELSE statement
          {
             YYTRACE("statement: YYIF '(' expression ')' statement YYELSE statement");   
             $$ = new VlpIfNode($3, $5, $7, NULL);
             BaseModule :: setCDFGNode($$);
             
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  } 
        | YYCASE '(' expression ')' synopsys_directive case_item_eclr YYENDCASE    // case, casex, casez are modified by Louis.
          {
             YYTRACE("statement: YYCASE '(' expression ')' case_item_eclr YYENDCASE");
             //VlpBaseNode* tail = LY->stack_c.top();//dummy
             LY->stack_c.pop();
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASE, $3, LY->stack_c.top(), NULL);
             LY->stack_c.pop();
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.top());
             LY->stack_d.pop();
             tmp->setFullCase(LY->stack_d.top());
             LY->stack_d.pop();
             $$ = tmp;
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();//dummy
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASE, $3, LY->stack_c.pop(), NULL);
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.pop());
             tmp->setFullCase(LY->stack_d.pop());
             $$ = tmp;     
*/        
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
          }
        | YYCASEZ '(' expression ')' synopsys_directive  case_item_eclr YYENDCASE
          {
             YYTRACE("statement: YYCASEZ '(' expression ')' case_item_eclr YYENDCASE"); 
             //VlpBaseNode* tail = LY->stack_c.top();//dummy
             LY->stack_c.pop();
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEZ, $3, LY->stack_c.top(), NULL);
             LY->stack_c.pop();
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.top());
             LY->stack_d.pop();
             tmp->setFullCase(LY->stack_d.top());
             LY->stack_d.pop();
             $$ = tmp;
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();//dummy
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEZ, $3, LY->stack_c.pop(), NULL);
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.pop());
             tmp->setFullCase(LY->stack_d.pop());
             $$ = tmp;
*/
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
          }
        | YYCASEX '(' expression ')' synopsys_directive case_item_eclr YYENDCASE
          {
             YYTRACE("statement: YYCASEX '(' expression ')' case_item_eclr YYENDCASE");
             //VlpBaseNode* tail = LY->stack_c.top();//dummy
             LY->stack_c.pop();
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEX, $3, LY->stack_c.top(), NULL);
             LY->stack_c.pop();
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.top());
             LY->stack_d.pop();
             tmp->setFullCase(LY->stack_d.top());
             LY->stack_d.pop();
             $$ = tmp;
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();//dummy
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEX, $3, LY->stack_c.pop(), NULL);
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.pop());
             tmp->setFullCase(LY->stack_d.pop());
             $$ = tmp;
*/
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
          }
/*      | YYFOREVER statement
          {
             YYTRACE("statement: YYFOREVER statement");
          }
        | YYREPEAT '(' expression ')' statement
          {
             YYTRACE("statement: YYREPEAT '(' expression ')' statement");
          } 
        | YYWHILE '(' expression ')' statement
          {
             YYTRACE("statement: YYWHILE '(' expression ')' statement");
          }*/
        | YYFOR '(' assignment ';' expression ';' assignment ')' statement 
          {
             YYTRACE("statement: YYFOR '(' assignment ';' expression ';' assignment ')' statement");
             $$ = new VlpForNode($3, $5, $7, $9, NULL);
             BaseModule :: setCDFGNode($$);
          }
 /*     | delay_control statement
          {
             YYTRACE("statement: delay_control statement");
          }*/
        | event_control statement //always      //if function or task uses the rule ==> stntax error ==> lintting
          {
             YYTRACE("statement: event_control statement");
             if (LY->allSensitive == false)
                LY->alwaysEventPtr = $1;
             else {
                assert (LY->nodeSet.size() != 0);
                set<string> :: const_iterator sPos  = LY->nodeSet.end();
                VlpSignalNode* sn = NULL;
                VlpEdgeNode*   en = NULL;
                VlpEdgeNode*   previous_en = NULL;
                if (LY->nodeSet.size() == 1) {
                   sPos = LY->nodeSet.begin();
                   sn = new VlpSignalNode(BaseModule :: setName(*sPos));
                   en = new VlpEdgeNode(EDGETYPE_SENSITIVE, sn);
                   BaseModule :: setCDFGNode(sn);
                   BaseModule :: setCDFGNode(en);
                   LY->alwaysEventPtr = en;
                }
                else {
                   for (sPos = LY->nodeSet.begin(); sPos != LY->nodeSet.end(); sPos++) {
                      sn = new VlpSignalNode(BaseModule :: setName(*sPos));
                      en = new VlpEdgeNode(EDGETYPE_SENSITIVE, sn);
                      en->setNext(previous_en);
                      BaseModule :: setCDFGNode(sn);
                      BaseModule :: setCDFGNode(en);
                      previous_en = en;
                   }
                   LY->alwaysEventPtr = new VlpOrNode(previous_en);
                   BaseModule :: setCDFGNode(LY->alwaysEventPtr);
                }               
                LY->nodeSet.clear();
             }
             $$ = $2;
             (LY->state) = EVENT;
          }
        | lvalue '=' delay_control expression ';'//when state the delay expression, then use the rule,
          {                                      //but to ignore delay
             YYTRACE("statement: lvalue '=' delay_control expression ';'");
             $$ = new VlpBAorNBA_Node(ASSIGNTYPE_BLOCK, $1, $4, NULL);
             if (LY->allSensitive && $4->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($4))->getNameId()));
          }
/*      | lvalue '=' event_control expression ';'  
          {
             YYTRACE("statement: lvalue '=' event_control expression ';'");
          }*/
        | lvalue YYNBASSIGN delay_control expression ';'// when state the delay expression, then
          {                                             // use the rule, but to ignore dalay
             YYTRACE("statement: lvalue YYNBASSIGN delay_control expression ';'");
             $$ = new VlpBAorNBA_Node( ASSIGNTYPE_NBLOCK, $1, $4, NULL);
             if (LY->allSensitive && $4->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($4))->getNameId()));
          }
/*      | lvalue YYNBASSIGN event_control expression ';'  
          {
             YYTRACE("statement: lvalue YYNBASSIGN event_control expression ';'");
          }*/
/*      | YYWAIT '(' expression ')' statement
          {
             YYTRACE("statement: YYWAIT '(' expression ')' statement");
          }
        | YYRIGHTARROW name_of_event ';'
          {
             YYTRACE("statement: YYRIGHTARROW name_of_event ';'");
          }*/
        | seq_block
          {
             YYTRACE("statement: seq_block");
             $$ = $1;
          }
/*      | par_block
          {
             YYTRACE("statement: par_block");
          }*/
        | task_enable  //function can't use another task ==> lintting
          {            //task can use another task and function
             YYTRACE("statement: task_enable");
             if (LY->scopeState == FUNCTION) {
                cerr << "Error20 : function can't use another task at line " << LY->lineNo << endl;
                exit(0);
             }
             $$ = $1;
          }
/*      | system_task_enable
          {
             YYTRACE("statement: system_task_enable");
          }
        | YYDISABLE YYID 
          {
	  }
          ';'  // name of task or block 
          {
             YYTRACE("statement: YYDISABLE YYID ';'");
          }*/
        | YYASSIGN assignment ';'
          {
             YYTRACE("statement: YYASSIGN assignment ';'");
             $$ = new VlpAssignNode($2, NULL);
             BaseModule :: setCDFGNode($$);
          }
/*      | YYDEASSIGN lvalue ';'
          {
             YYTRACE("statement: YYDEASSIGN lvalue ';'");
          }*/
        ;

synopsys_directive
        :                              { LY->stack_d.push(false); LY->stack_d.push(false); }
        | FULL_CASE                    { LY->stack_d.push(true) ; LY->stack_d.push(false); }
        | PARALLEL_CASE                { LY->stack_d.push(false); LY->stack_d.push(true);  }
        | FULL_CASE PARALLEL_CASE      { LY->stack_d.push(true);  LY->stack_d.push(true);  }
        | PARALLEL_CASE FULL_CASE      { LY->stack_d.push(true);  LY->stack_d.push(true);  }
        

assignment
        : lvalue '=' expression
          {
             YYTRACE("assignment: lvalue '=' expression");
             if ((LY->state) == NET)//nettype
             {
                //only the syntax "lvalue : identifier" is legal.
                assert($1->getNodeClass() == NODECLASS_SIGNAL);
                const VlpSignalNode* tmp = (static_cast<VlpSignalNode*>($1));
                VlpPortNode* target = NULL;
                if (LY->modulePtr->getPort(BaseModule :: getName(tmp->getNameId()), target)) 
                { //exist in map(declared)
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                }
                else { // new in map
                   target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                   LY->modulePtr->setPort(BaseModule :: getName(tmp->getNameId()), target);
                }
                $$ = new VlpBAorNBA_Node(ASSIGNTYPE_BLOCK, $1, $3, NULL); 
                BaseModule :: setCDFGNode($$);
             }
             else if ((LY->state) == PARAM) {
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setParam(LY->tempStr4, $3);
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setParam(LY->tempStr4, $3);
                else  //TASK    
                   LY->taskPtr->setParam(LY->tempStr4, $3);
                LY->tempStr4 = "";  
                $$ = NULL;
             }
             else if ((LY->state) == PARAM_NOL) {
                const paramPair* pair = new paramPair(LY->tempStr4, $3);
                LY->modulePtr->setParamNOL(LY->instName, pair);          
                LY->tempStr4 = "";  
                LY->instName = "";
                $$ = NULL;
             }
             else {//other ASSIGN, seq_block
                $$ = new VlpBAorNBA_Node(ASSIGNTYPE_BLOCK, $1, $3, NULL); 
                BaseModule :: setCDFGNode($$);
             }
          }
        | lvalue YYNBASSIGN expression
          {
             YYTRACE("assignment: lvalue YYNBASSIGN expression");
             if ( (LY->state) == NET || (LY->state) == PARAM 
               || (LY->state) == PARAM_NOL || (LY->state) == ASSIGN) {
                cerr << "state = " << LY->state << endl; 
                cerr << "SYNTAX ERROR: using NBASSIGN at line " << LY->lineNo
                     << " (\"wire\" or \"parameter\" or \"assign\")" << endl;
                exit(0);
             }
             else {
                $$ = new VlpBAorNBA_Node(ASSIGNTYPE_NBLOCK, $1, $3, NULL);
                BaseModule :: setCDFGNode($$);
             }
          }
        ;

case_item_eclr
        : case_item
          {
             YYTRACE("case_item_eclr: case_item");
             LY->stack_c.push($1);
             VlpBaseNode* tail = $1;
             while(tail->getNext() != NULL)
                tail = const_cast<VlpBaseNode*>(tail->getNext());//const_cast : const VlpBaseNode* -> VlpBaseNode*
             LY->stack_c.push(tail);
          }
        | case_item_eclr case_item
          {
             YYTRACE("case_item_eclr: case_item_eclr case_item");
             VlpBaseNode* tail = LY->stack_c.top();
             LY->stack_c.pop();
             tail->setNext($2);
             tail = $2;
             while(tail->getNext() != NULL)
                tail = const_cast<VlpBaseNode*>(tail->getNext());
             LY->stack_c.push((VlpBaseNode*)tail);
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();
             tail->setNext($2);
             tail = $2;
             while(tail->getNext() != NULL)
                tail = const_cast<VlpBaseNode*>(tail->getNext());
             LY->stack_c.push((VlpBaseNode*)tail);
*/
          }
        ;

case_item
        : expression_list ':' statement
          {
             YYTRACE("case_item: expression_list ':' statement");
             assert($1->getNodeClass() == NODECLASS_CONNECT);
             VlpConnectNode* temp = static_cast<VlpConnectNode*>($1);
     
             $$ = new VlpCaseItemNode(const_cast<VlpBaseNode*>(temp->getExp()), $3, NULL);
             BaseModule :: setCDFGNode($$);
             VlpBaseNode* ptr = $$;
             VlpCaseItemNode* next;
             temp = static_cast<VlpConnectNode*>( const_cast<VlpBaseNode*>(temp->getNext()) );
             while (temp != NULL) {
                assert(temp->getNodeClass() == NODECLASS_CONNECT);
                next = new VlpCaseItemNode(const_cast<VlpBaseNode*>(temp->getExp()), $3, NULL);
                BaseModule :: setCDFGNode(next);
                ptr->setNext(next);
                ptr = next;
                temp = static_cast<VlpConnectNode*>( const_cast<VlpBaseNode*>(temp->getNext()) );
             }
          
          }
        | YYDEFAULT ':' statement
          {
             YYTRACE("case_item: YYDEFAULT ':' statement");
             VlpBaseNode* ptr = new VlpSignalNode(BaseModule :: setName("default")); 
             BaseModule :: setCDFGNode(ptr);

             $$ = new VlpCaseItemNode(ptr, $3, NULL);          
             BaseModule :: setCDFGNode($$);
          }
        | YYDEFAULT  statement      //guess "default ; " (ie. null default expression)  
          {
             YYTRACE("case_item: YYDEFAULT statement");
             VlpBaseNode* ptr = new VlpSignalNode(BaseModule :: setName("default")); 
             BaseModule :: setCDFGNode(ptr);
             $$ = new VlpCaseItemNode(ptr, $2, NULL);
             BaseModule :: setCDFGNode($$);
          }
        ;

seq_block
        : YYBEGIN statement_clr YYEND
          {
             YYTRACE("seq_block: YYBEGIN statement_clr YYEND");
             if ($2 != NULL) {
                $$ = LY->stack_s.top();
                LY->stack_s.pop();
             }
/*no use STL
             if ($2 != NULL)
                $$ = LY->stack_s.pop();
*/
          }
        | YYBEGIN ':' name_of_block block_declaration_clr statement_clr YYEND                                        
          {
             YYTRACE("seq_block: YYBEGIN ':' name_of_block block_declaration_clr statement_clr YYEND");
             if ($5 != NULL) {
                $$ = LY->stack_s.top();
                LY->stack_s.pop();
             }
/*no use STL
             if ($5 != NULL)
                $$ = LY->stack_s.pop();
*/
          }
        ;

/*par_block
        : YYFORK statement_clr YYJOIN
          {
             YYTRACE("par_block: YYFORK statement_clr YYJOIN");
          }
        | YYFORK ':' name_of_block block_declaration_clr statement_clr YYJOIN
          {
             YYTRACE("par_block: YYFORK ':' name_of_block block_declaration_clr statement_clr YYJOIN");
          }
        ;*/

name_of_block
        : YYID
          {
             YYTRACE("name_of_block: YYID");
          }
        ;

block_declaration_clr     //seq_block 
        :
          {
             YYTRACE("block_declaration_clr:");
          }
        | block_declaration_clr block_declaration
          {
             YYTRACE("block_declaration_clr: block_declaration_clr block_declaration");
             cerr << "No supply syntax in block_declaration_clr at line " << LY->lineNo << endl;
          }
        ;

block_declaration        
        : parameter_declaration
          {
             YYTRACE("block_declaration: parameter_declaration");
          }
        | reg_declaration
          {
             YYTRACE("block_declaration: reg_declaration");
          }
/*        | integer_declaration
          {
             YYTRACE("block_declaration: integer_declaration");
          }
        | real_declaration
          {
             YYTRACE("block_declaration: real_declaration");
          }
        | time_declaration
          {
             YYTRACE("block_delcaration: time_declaration");
          }
        | event_declaration
          {
             YYTRACE("block_declaration: event_declaration");
          }*/
        ;

/* YYID is the name of a task */
task_enable
        : YYID 
          { 
          }
          ';'
          {
             YYTRACE("task_enable: YYID ';'");
             $$ = new VlpTaskCall(*$1, NULL, NULL);
             BaseModule :: setCDFGNode($$);
          }
        | YYID 
          {
             LY->instName = *$1;   
	  }
         '(' expression_list ')' ';'
          {
             YYTRACE("task_enable: YYID '(' expression_list ')' ';'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
             $$ = new VlpTaskCall(LY->instName, head, NULL);
             BaseModule :: setCDFGNode($$);
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
             $$ = new VlpTaskCall(LY->instName, head, NULL);
             BaseModule :: setCDFGNode($$);
*/
          }
        ;

/*system_task_enable
        : name_of_system_task ';'
          {
             YYTRACE("system_task_enable: name_of_system_task ';'");
          }
        | name_of_system_task '(' expression_list ')'
          {
             YYTRACE("system_task_enable: name_of_system_task '(' expression_list ')'");
          }
        ;

name_of_system_task
        : system_identifier
          {
             YYTRACE("name_of_system_task: system_identifier");
          }
        ;*/

/* F.6 Specify section */

/*specify_block
        : YYSPECIFY specify_item_clr YYENDSPECIFY
          {
             YYTRACE("specify_block: YYSPECIFY specify_item_clr YYENDSPECIFY");
          } 
        ;

specify_item_clr
        :
        | specify_item_clr specify_item
        ;

specify_item
        : specparam_declaration
        | path_declaration
        | level_sensitive_path_declaration
        | edge_sensitive_path_declaration
        | system_timing_check
        ;

specparam_declaration
        : YYSPECPARAM assignment_list ';'
        ;

path_declaration
        : path_description '=' path_delay_value ';'
        ;
                                         
path_description
        : '(' specify_terminal_descriptor YYLEADTO specify_terminal_descriptor ')'
        | '(' path_list YYALLPATH path_list_or_edge_sensitive_path_list ')'
        ;

path_list
        : specify_terminal_descriptor
        | path_list ',' specify_terminal_descriptor
        ;

specify_terminal_descriptor
        : YYID
        | YYID '[' expression ']'
        | YYID '[' expression ';' expression ']'
        ;

path_list_or_edge_sensitive_path_list
        : path_list
        | '(' path_list ',' specify_terminal_descriptor
              polarity_operator YYCONDITIONAL 
              expression ')'
        ;

path_delay_value
        : path_delay_expression
        | '(' path_delay_expression ',' path_delay_expression ')'
        | '(' path_delay_expression ',' path_delay_expression ',' 
              path_delay_expression ')'
        | '(' path_delay_expression ',' path_delay_expression ','
              path_delay_expression ',' path_delay_expression ','
              path_delay_expression ',' path_delay_expression ')'
        ;

path_delay_expression
        : expression
        ;
*/
/* grammar rules for system timing check hasn't been done yet, includes:
 *  <system_timing_check>
 *  <timign_check_event>
 *  <controlled_timing_check_event>
 *  <timing_check_event_control>
 *  <timing_check_condidtion>
 *  <SCALAR_EXPRESSION>
 *  <timing_check_limit>
 *  <scalar_constant>
 *  <notify_register>
 *  in page 211. 
*/
/*
system_timing_check
        : YYsysSETUP '(' ')' ';'

level_sensitive_path_declaration
        : YYIF '(' expression ')'
            '(' specify_terminal_descriptor polarity_operator_opt YYLEADTO
                spec_terminal_desptr_or_edge_sensitive_spec_terminal_desptr
        | YYIF '(' expression ')'
            '(' path_list ',' specify_terminal_descriptor 
                polarity_operator_opt YYALLPATH path_list ')'
                path_list '=' path_delay_value ';'
        ;

spec_terminal_desptr_or_edge_sensitive_spec_terminal_desptr
        : specify_terminal_descriptor ')' path_list '=' path_delay_value ';'
        | '(' specify_terminal_descriptor polarity_operator YYCONDITIONAL
              expression ')' ')' '=' path_delay_value ';'


polarity_operator_opt
        :
        | polarity_operator
        ;

polarity_operator
        : '+'
        | '-'
        ;

edge_sensitive_path_declaration
        : '(' specify_terminal_descriptor YYLEADTO
            '(' specify_terminal_descriptor polarity_operator YYCONDITIONAL
                expression ')' ')' '=' path_delay_value ';'
        | '(' edge_identifier specify_terminal_descriptor YYLEADTO
            '(' specify_terminal_descriptor polarity_operator YYCONDITIONAL
                expression ')' ')' '=' path_delay_value ';'
        | '(' edge_identifier specify_terminal_descriptor YYALLPATH
            '(' path_list ',' specify_terminal_descriptor
                polarity_operator YYCONDITIONAL 
                expression ')' ')' '=' path_delay_value ';'
        | YYIF '(' expression ')'
            '(' specify_terminal_descriptor YYALLPATH
              '(' path_list ',' specify_terminal_descriptor
                  polarity_operator YYCONDITIONAL 
                  expression ')' ')' '=' path_delay_value ';'
        | YYIF '(' expression ')'
            '(' edge_identifier specify_terminal_descriptor YYLEADTO
              '(' specify_terminal_descriptor polarity_operator YYCONDITIONAL
                  expression ')' ')' '=' path_delay_value ';'
        | YYIF '(' expression ')'
            '(' edge_identifier specify_terminal_descriptor YYALLPATH
              '(' path_list ',' specify_terminal_descriptor
                  polarity_operator YYCONDITIONAL 
                  expression ')' ')' '=' path_delay_value ';'
        ;

edge_identifier
        : YYPOSEDGE
        | YYNEGEDGE
        ;*/

/* F.7 Expressions */

lvalue
        : identifier
          {
	     YYTRACE("lvalue: YYID");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) { //net or parameter declarement
                BaseModule :: setName(*$1);
                LY->tempStr4 = *$1;
             }
             else if ( (LY->state) == NET) {
                $$ = new VlpSignalNode(BaseModule :: setName(*$1)); 
                BaseModule :: setCDFGNode($$);
             }
             else {//ASSIGN, seq_block ...
                if (LY->modulePtr->lintPort(*$1) == false)//lintting
                {                                    
                   const paramPair* dummy;                        //parameter only can be used in lvalue under declaring
                   if (LY->modulePtr->lintParam(*$1, dummy) == true) {//condition, other condition it can't be used 
                      cerr << "Error21 : parameter " << *$1 << " can't be assigned at line "  << LY->lineNo << endl;
                      exit(0);                    
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(*$1) == true))                  
                      ;//the identifier is declared in function scope ==> It don't need to lint     
                   else if ((LY->scopeState == TASK) && (LY->taskPtr->lintPort(*$1) == true))  
                      ;//the identifier is declared in task scope ==> It don't need to lint
                   else {
                      cerr << "Error22 : un-declare identifier " << *$1 << " at line "  << LY->lineNo << endl;
                      exit(0);
                   }
                }
                $$ = new VlpSignalNode(BaseModule :: setName(*$1)); 
                BaseModule :: setCDFGNode($$);
             }
	  }
        | identifier { LY->sigNames.push(*$1) } bitExpression
          {
	     YYTRACE("lvalue: YYID '[' expression ']'");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) {
                cerr << "Syntax Error23 : paramter can't use '[' at line " << LY->lineNo << endl;
                exit(0);
             }
             else if ((LY->state) == NET) { //EX ; wire [7:0] a[3]
                cerr << "Syntax Error24 : net-type can't use array at line " << LY->lineNo << endl;
                exit(0);                
             }             
             string sigName = LY->sigNames.top();
             LY->sigNames.pop();
             //if ((LY->state) != NET && LY->modulePtr->lintPort(*$1) == false)//lintting
             if (LY->modulePtr->lintPort(sigName) == false)//linting
             {
                if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(sigName) == true))           
                   ;//the identifier is declared in function scope ==> It don't need to lint                     
                else if ((LY->scopeState == TASK) && (LY->taskPtr->lintPort(sigName) == true))    
                   ;//the identifier is declared in task scope ==> It don't need to lint
                else {
                   cerr << "error : un-declare identifier " << sigName << " at line "  << LY->lineNo << endl;
                   exit(0);
                }
             }
             if ($3->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression or [ exp : exp ]
             {
                $$ = new VlpSignalNode (
                     BaseModule :: setName(sigName), 
                     const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getFirst() ), 
                     const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getSecond() ) ); 
                BaseModule :: setCDFGNode($$);
             }
             else {
                $$ = new VlpSignalNode(BaseModule :: setName(sigName), $3, $3); 
                BaseModule :: setCDFGNode($$);
             }
          }
        | concatenation
          {
	     YYTRACE("lvalue: concatenation");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) {
                cerr << "Syntax Error25 : paramter can't use '{' at line " << LY->lineNo << endl;
                exit(0);
             }
             else if ((LY->state) == NET) {
                cerr << "Syntax Error26 : net-type declaration error at line " << LY->lineNo << endl;
                exit(0);                
             }             
             
             $$ = $1;
	  }
        | YYDEF_ID bit_exp// added by Louis
          { 
	     YYTRACE("lvalue: YYDEF_ID");
             const VlpBaseNode* tmp;
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) {
                cerr << "Syntax Error27 : paramter can't use defined macro at line " <<  LY->lineNo <<endl;
                exit(0);
             }
             if ($2 == NULL)
             {
                if (LY->defineMap.getData(*$1 ,tmp) == false)
                {
                   cerr << "Error28 : the macro " << *$1 << " isn't defined at line " << LY->lineNo << endl;
                   exit(0);
                }
                $$ = const_cast<VlpBaseNode*>(tmp);
             }
             else // need to new a new node, and copy the defined content to the new node,
             {    // can't change the original defined content
                  // still not finish 2006 7/10
             }
          }
        ;

bit_exp 
        :                  { $$ = NULL; }
        | bitExpression    { $$ = $1; }
        ;
   
//==========================================still not complete========================================// 
mintypmax_expression_list// only use in instance parameter overload
        : mintypmax_expression
          {
	     YYTRACE("mintypmax_expression_list: mintypmax_expression");
             LY->array.push_back($1);
	  }
        | mintypmax_expression_list ',' mintypmax_expression
          {
             YYTRACE("mintypmax_expression_list: mintypmax_expression_list ',' mintypmax_expression");
             LY->array.push_back($3);
	  }
        ;
//=================================================================================================//
mintypmax_expression
        : expression
          {
	     YYTRACE("mintypmax_expression: expression");
             $$ = $1;
	  }
/*        | expression ':' expression ':' expression // It may be used in UDP, so won't be supported
          {
	     YYTRACE("mintypmax_expression: expression ':' expression ':' expression");
	  }*/
        ;

expression_list   //case_item, concatenation, multiple_concatenation, function_call, task_enable
        : expression
          {/*expression is used in case, task, and concatenate*/
	     YYTRACE("expression_list: expression");
             if ($1->checkNext() == false) {//many signal node and OPNodes don't have setNext() function
                $$ = new VlpConnectNode($1);
                BaseModule :: setCDFGNode($$);
             }
             else 
                $$ = $1;
             LY->stack_e.push($$);//head of expression_list
             LY->stack_e.push($$);//tail of expression_list
	  }
        | expression_list ',' expression     
          {
	     YYTRACE("expression_list: expression_list ',' expression");
             VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
*/
             if (tail->getNodeClass() == NODECLASS_CONNECT) {        
                VlpBaseNode* tmp = new VlpConnectNode($3);
                BaseModule :: setCDFGNode(tmp);
                tail->setNext(tmp);
                tail = tmp;
             }
             else {
                tail->setNext($3);
                tail = $3;
             }
             LY->stack_e.push(tail);
	  }
        ;

expression
        : primary
          {
	     YYTRACE("expression: primary");
             $$ = $1;
	  }
        | '+' primary %prec YYUNARYOPERATOR
          {
	     YYTRACE("expression: '+' primary %prec YYUNARYOPERATOR");
             $$ = new VlpSignNode(ARITHOP_UNARY_PLUS, $2); 
             BaseModule :: setCDFGNode($$);
	  } 
        | '-' primary %prec YYUNARYOPERATOR
          {
	     YYTRACE("expression: '-' primary %prec YYUNARYOPERATOR");
             $$ = new VlpSignNode(ARITHOP_UNARY_MINUS, $2); 
             BaseModule :: setCDFGNode($$);
	  }
        | '!' primary %prec YYUNARYOPERATOR
          {
	     YYTRACE("expression: '!' primary %prec YYUNARYOPERATOR");
             $$ = new VlpLogicNotOpNode($2);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $2->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($2))->getNameId()));
	  }
        | '~' primary %prec YYUNARYOPERATOR
          {
             YYTRACE("expression: '~' primary %prec YYUNARYOPERATOR");
             $$ = new VlpBitNotOpNode($2);
             BaseModule :: setCDFGNode($$);
	  }
        | '&' primary %prec YYUNARYOPERATOR
          {
	     YYTRACE("expression: '&' primary %prec YYUNARYOPERATOR");
             $$ = new VlpReductOpNode(REDUCTOP_AND, $2);
             BaseModule :: setCDFGNode($$);
	  }
        | '|' primary %prec YYUNARYOPERATOR
          {
	     YYTRACE("expression: '|' primary %prec YYUNARYOPERATOR");
             $$ = new VlpReductOpNode(REDUCTOP_OR, $2);
             BaseModule :: setCDFGNode($$);
	  }
        | '^' primary %prec YYUNARYOPERATOR
          {/*The rule is added by Louis.*/
	     YYTRACE("expression: '^' primary %prec YYUNARYOPERATOR");
             $$ = new VlpReductOpNode(REDUCTOP_XOR, $2);
             BaseModule :: setCDFGNode($$);
	  }
        | YYLOGNAND primary %prec YYUNARYOPERATOR
          {
	     YYTRACE("expression: YYLOGNAND primary %prec YYUNARYOPERATOR");
             $$ = new VlpReductOpNode(REDUCTOP_NAND, $2);
             BaseModule :: setCDFGNode($$);
	  }
        | YYLOGNOR primary %prec YYUNARYOPERATOR
          {
             YYTRACE("expression: YYLOGNOR primary %prec YYUNARYOPERATOR");
             $$ = new VlpReductOpNode(REDUCTOP_NOR, $2);
             BaseModule :: setCDFGNode($$);
	  }
        | YYLOGXNOR primary %prec YYUNARYOPERATOR
          {
	     YYTRACE("expression: YYLOGXNOR primary %prec YYUNARYOPERATOR");
             $$ = new VlpReductOpNode(REDUCTOP_XNOR, $2);
             BaseModule :: setCDFGNode($$);
	  }
        | expression '+' expression
          {
	     YYTRACE("expression: expression '+' expression");
             $$ = new VlpArithOpNode(ARITHOP_ADD, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
        | expression '-' expression
          {
	     YYTRACE("expression: expressio '-' expression");
             $$ = new VlpArithOpNode(ARITHOP_SUBTRACT, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
        | expression '*' expression
          {
	     YYTRACE("expression: expression '*' expression");
             $$ = new VlpArithOpNode(ARITHOP_MULTIPLY, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
/*      | expression '/' expression
          {
	     YYTRACE("expression: expression '/' expression");
	  }
        | expression '%' expression
          {
             YYTRACE("expression: expression '%' expression");
	  }*/
        | expression YYLOGEQUALITY expression
          {
	     YYTRACE("expression: expression YYLOGEQUALITY expression");
             $$ = new VlpEqualityNode(EQUALITY_LOG_EQU, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression YYLOGINEQUALITY expression
          {
	     YYTRACE("expression: expression YYLOGINEQUALITY expression");
             $$ = new VlpEqualityNode(EQUALITY_LOG_INEQU, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression YYCASEEQUALITY expression
          {
	     YYTRACE("expression: expression YYCASEEQUALITY expression");
             $$ = new VlpEqualityNode(EQUALITY_CASE_EQU, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression YYCASEINEQUALITY expression
          {
	     YYTRACE("expression: expression YYCASEINEQUALITY expression");
             $$ = new VlpEqualityNode(EQUALITY_CASE_INEQU, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression YYLOGAND expression
          {
	     YYTRACE("expression: expression YYLOGAND expression");
             $$ = new VlpLogicOpNode(LOGICOP_AND, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression YYLOGOR expression
          {
	     YYTRACE("expression: expression YYLOGOR expression");
             $$ = new VlpLogicOpNode(LOGICOP_OR, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression '<' expression
          {
	     YYTRACE("expression: expression '<' expression");
             $$ = new VlpRelateOpNode(RELATEOP_LESS, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression '>' expression
          {
	     YYTRACE("expression: expression '>' expression");
             $$ = new VlpRelateOpNode(RELATEOP_GREATER, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression '&' expression
          {
	     YYTRACE("expression: expression '&' expression");
             $$ = new VlpBitWiseOpNode(BITWISEOP_AND, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
        | expression '|' expression
          {
	     YYTRACE("expression: expression '|' expression");
             $$ = new VlpBitWiseOpNode(BITWISEOP_OR, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
        | expression '^' expression
          {
	     YYTRACE("expression: expression '^' expression");
             $$ = new VlpBitWiseOpNode(BITWISEOP_XOR, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
        | expression YYLEQ expression
          {
	     YYTRACE("expression: expression YYLEQ expression");
             $$ = new VlpRelateOpNode(RELATEOP_LEQ, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression YYNBASSIGN expression
          {
	     YYTRACE("expression: expression YYLEQ expression");           
             $$ = new VlpRelateOpNode(RELATEOP_LEQ, $1, $3);
             BaseModule :: setCDFGNode($$);
             //$$ = new VlpBAorNBA_Node(ASSIGNTYPE_NBLOCK, $1, $3, NULL);
	  }
        | expression YYGEQ expression
          {
	     YYTRACE("expression: expression YYGEQ expression");
             $$ = new VlpRelateOpNode(RELATEOP_GEQ, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression YYLSHIFT expression
          {
	     YYTRACE("expression: expression YYLSHIFT expression");
             $$ = new VlpShiftOpNode(SHIFTOP_LSH, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
        | expression YYRSHIFT expression
          {
	     YYTRACE("expression: expression YYRSHIFT expression");
             $$ = new VlpShiftOpNode(SHIFTOP_RSH, $1, $3);
             BaseModule :: setCDFGNode($$);
	  }
        | expression YYLOGXNOR expression
          {
	     YYTRACE("expression: expression YYLOGXNOR expression");
             $$ = new VlpBitWiseOpNode(BITWISEOP_XNOR, $1, $3);
             BaseModule :: setCDFGNode($$);
             if (LY->allSensitive && $1->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($1))->getNameId()));
             if (LY->allSensitive && $3->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>($3))->getNameId()));
	  }
        | expression '?' expression ':' expression
          {
	     YYTRACE("expression: expression '?' expression ':' expression");
             $$ = new VlpConditionalNode($1, $3, $5);
             BaseModule :: setCDFGNode($$);
	  }
/*      | YYSTRING//     \"[^"]*  (string quoted by "" *) ==>  return YYSTRING        
          {
	     YYTRACE("expression: YYSTRING"); //name of case item, name of parameter
	  }*/
        ;

primary
        : YYINUMBER
          {
	     YYTRACE("primary: YYINUMBER");
             $$ = new VlpIntNode($1);
             BaseModule :: setCDFGNode($$);
	  }
        | YYRNUMBER
          {
             YYTRACE("primary: YYRNUMBER");
             cerr << "No supply : primary : YYRNUMBER at line " << LY->lineNo << endl;
	  }
        | identifier
          {
	     YYTRACE("primary: identifier");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) { //linting
                const paramPair* target;
                if (LY->modulePtr->lintParam(*$1, target) == true) { //parameter 
                   $$ = new VlpParamNode(const_cast<paramPair*>(target));
                   BaseModule :: setCDFGNode($$);
                }
                else {
                   cerr << "Error29 : Symbol (" << *$1 << ") is illegal, constant expected at line" 
                        << LY->lineNo << endl;
                   exit(0);
                }
             }
             //(LY->state) != PARAM && (LY->state) != PARAM_NOL
             else {
                if (((LY->state) != DEFINE) && (LY->modulePtr->lintPort(*$1) == false)) {//linting 
                   const paramPair* target;
                   if (LY->modulePtr->lintParam(*$1, target) == true) {//module scope parameter 
                      $$ = new VlpParamNode(const_cast<paramPair*>(target));
                      BaseModule :: setCDFGNode($$);
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(*$1) == true)) {//function scope variable
                      $$ = new VlpSignalNode(BaseModule :: setName(*$1));            
                      BaseModule :: setCDFGNode($$);
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintParam(*$1, target) == true))//function scope param.
                      $$ = target->content;//substitute the parameter directly, because it can't be overload.                    
                   else if ((LY->scopeState == TASK) && (LY->taskPtr->lintParam(*$1, target) == true)) //task scope param.
                      $$ = target->content;//substitute the parameter directly, because it can't be overload.                    
                   else {//un-declare variable => default 1 bit wire
                      VlpPortNode* pn = new VlpPortNode(NULL, NULL);
                      pn->setNetType(wire);
                      LY->modulePtr->setPort(*$1, pn);
                      $$ = new VlpSignalNode(BaseModule :: setName(*$1));
                      //cerr << "Error30 : un-declare identifier " << *$1 << " at line "  << LY->lineNo << endl;
                      //exit(0);
                   }
                }
                else {//(LY->state) == DEFINE or LY->modulePtr->lintPort == true
                   $$ = new VlpSignalNode(BaseModule :: setName(*$1));            
                   BaseModule :: setCDFGNode($$);
                }
             }  
	  }
        | identifier { LY->sigNames.push(*$1); } bitExpression
          {
	     YYTRACE("primary: identifier bitExpression");
             string sigName = LY->sigNames.top();
             LY->sigNames.pop();
             const paramPair* target;
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) { //linting;
                if (LY->modulePtr->lintParam(sigName, target) == true) { //parameter 
                   if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                      paramPair* newNode = new paramPair(*target);

                      if ($3->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression
                      {                                           //or Ex: a[5:0]
                         ((VlpSignalNode*)(newNode->content))->setSigWidth( 
                         const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getFirst() ),
                         const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getSecond()) );
                      }
                      else                                        //Ex: a[3]
                         ((VlpSignalNode*)(newNode->content))->setSigWidth($3, $3);
                      $$ = new VlpParamNode(newNode);                   
                      BaseModule :: setCDFGNode($$);
                   }
                   else {
                      cerr << "Error31 : Symbol (" << sigName << ") is illegal, error bit-expression at line " 
                           << LY->lineNo << endl;
                      exit(0);                      
                   }
                }
                else {
                   cerr << "Error32 : Symbol (" << sigName << ") is illegal, constant expected at line " 
                        << LY->lineNo << endl;
                   exit(0);
                }
             }             
             else { //(LY->state) != PARAM && (LY->state) != PARAM_NOL
                if (((LY->state) != DEFINE) && (LY->modulePtr->lintPort(sigName) == false))//linting
                {
                   if (LY->modulePtr->lintParam(sigName, target) == true) { //module scope parameter 
                   //Ex : parameter ab 4'b1100      ab[0:2] or ab[3]
                      if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                         paramPair* newNode = new paramPair(*target);
                         if ($3->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression
                         {                                           //or Ex: 4'b111[2:0]
                            ((VlpSignalNode*)(newNode->content))->setSigWidth( 
                            const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getFirst() ),
                            const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getSecond()) );
                         }
                         else                                        //Ex: 4'b0011[3]
                            ((VlpSignalNode*)(newNode->content))->setSigWidth($3, $3);
                         $$ = new VlpParamNode(newNode);
                         BaseModule :: setCDFGNode($$);
                      }                   
                      else {
                         cerr << "Error33 : Symbol (" << sigName << ") is illegal, error bit-expression at line "
                              << LY->lineNo << endl;
                         exit(0);
                      }
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(sigName) == true)) {//function scope variable
                      VlpSignalNode* newNode = new VlpSignalNode(BaseModule :: setName(sigName));
                      BaseModule :: setCDFGNode(newNode);
                      if ($3->getNodeClass() == NODECLASS_CONNECT)
                      {                                       
                         newNode->setSigWidth( const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getFirst() ),
                                               const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getSecond()) );
                      }
                      else                                      
                         newNode->setSigWidth($3, $3);
                      $$ = newNode;
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintParam(sigName, target) == true)) {//function scope param.
                      if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                         //substitute the parameter directly,because it can't be overload.
                         VlpSignalNode* newNode = new VlpSignalNode(((VlpSignalNode*)(target->content))->getNameId());
                         BaseModule :: setCDFGNode(newNode);
                         if ($3->getNodeClass() == NODECLASS_CONNECT)                      
                         {                                       
                            newNode->setSigWidth( const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getFirst() ),
                                                  const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getSecond()) );
                         }
                         else                                      
                            newNode->setSigWidth($3, $3);
                         $$ = newNode;
                      }
                   }
                   else if ((LY->scopeState == TASK) && (LY->taskPtr->lintParam(sigName, target) == true)) {//task scope param.
                      if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                      //substitute the parameter directly, because it can't be overload.
                         VlpSignalNode* newNode = new VlpSignalNode(((VlpSignalNode*)(target->content))->getNameId());
                         BaseModule :: setCDFGNode(newNode);
                         if ($3->getNodeClass() == NODECLASS_CONNECT)                      
                         {                                       
                            newNode->setSigWidth( const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getFirst() ),
                                                  const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getSecond()) );
                         }
                         else                                      
                            newNode->setSigWidth($3, $3);
                         $$ = newNode;
                      }
                   }
                   else {
                      cerr << "Error34 : un-declare identifier " << sigName << " at line "  << LY->lineNo << endl;
                      exit(0);
                   }
                }
                else {
                   if ($3->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression 
                   {                                           //or Ex: a[5:0]
                      $$ = new VlpSignalNode (
                           BaseModule :: setName(sigName), 
                           const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getFirst() ), 
                           const_cast<VlpBaseNode*>( ((VlpConnectNode*)($3))->getSecond() ) ); 
                      BaseModule :: setCDFGNode($$);
                   }
                   else {                                        //Ex: a[3]
                      $$ = new VlpSignalNode(BaseModule :: setName(sigName), $3, $3);      
                      BaseModule :: setCDFGNode($$);
                   }
                }
             }       
          }
        | concatenation
          {
	     YYTRACE("primary: concatenation");
             $$ = $1;
	  }
        | multiple_concatenation
	  {
	     YYTRACE("primary: multiple_concatenatin");
             $$ = $1;
	  }
        | function_call
          {
	     YYTRACE("primary: function_call");
             $$ = $1;
	  }
        | YYBIT_BASE_STRING//action is same as "primary: identifier", 
          {                //but put in different BNF to make modify easily in the future
             YYTRACE("primary: BIT_BASE_NUMBER");
             $$ = new VlpSignalNode(BaseModule :: setName(*$1));    
             BaseModule :: setCDFGNode($$);
          }
        | '(' expression ')'//add by Louis   Ex: a = ~( b ^ c )    c = ~(clk)        
          {
	     YYTRACE("expression: ( expression )");
             $$ = $2;
          }
        | YYDEF_ID bit_exp// added by Louis
          { 
             YYTRACE("primary: YYDEF_ID");
             const VlpBaseNode* tmp;
             if ($2 == NULL) 
             {
                if (LY->defineMap.getData(*$1 ,tmp) == false)
                {
                   cerr << "Error35 : the macro " << *$1 << " isn't defined at line " << LY->lineNo << endl;
                   exit(0);
                }
                $$ = const_cast<VlpBaseNode*>(tmp);
             }
             else // need to new a new node, and copy the defined content to the new node,
             {    // can't change the original defined content
                  // still not finish 2006 7/10
             }
          }

/*        | '(' mintypmax_expression ')'        
          {
	     YYTRACE("primary: '(' mintypmax_expression ')'");
	  }*/
        ;

bitExpression //Add by Louis 
        : '[' expression ']'
          {
             $$ = $2;
          }
        | '[' expression ':' expression ']'
          {
             $$ = new VlpConnectNode($2, $4);
             BaseModule :: setCDFGNode($$);
          }
        ;

/*
 * number : decimal_number
 *        | unsigned number? base number
 *        | real_number
 *        ;
 *
 * is put in lex rules
 */

concatenation
        : '{' expression_list '}'
          {
	     YYTRACE("concatenation: '{' expression_list '}'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
*/
             $$ = new VlpConcatenateNode(head);    
             BaseModule :: setCDFGNode($$);
	  }
        ;

multiple_concatenation
        : '{' expression '{' expression_list '}' '}'
	  {
	     YYTRACE("multiple_concatenation: '{' expression '{' expression_list '}' '}'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
*/

             $$ = new VlpReplicationNode($2, head);
             BaseModule :: setCDFGNode($$);
	  }
        ;

function_call
        : identifier '(' { LY->instName = *$1; }
          expression_list ')'
          {
	     YYTRACE("function_call: identifier '(' expression_list ')'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
*/
             $$ = new VlpFunCall(LY->instName, head);
             BaseModule :: setCDFGNode($$);
	  }
/*
 *      | system_function_call
 *
 * is not implemented, it will cause a r/r error, you have to implement that
 * using semantic routine.
 */
        ;

/*system_identifier
        : YYsysID
        ;*/

/* F.8 General */

identifier
        : YYID
          {//identifier => stringPtr 
             YYTRACE("identifier: YYID");
             $$ = $1;
          }
        | identifier '.' { LY->instName = *$1; } YYID
          //defparam use the rule(name mapping of parameter overload)
          //ps:it isn't instance name mapping syntax                                                                        
          {
             YYTRACE("identifier: identifier '.' YYID");
             $$ = $4;
          }
        ;

/*delay_opt
        :
          {
	     YYTRACE("delay_opt:");
	  }
        | delay
          {
	     YYTRACE("delay_opt: delay");
	  }
        ;

delay
        : '#' YYINUMBER
          {
	     YYTRACE("delay: '#' YYINUMBER");
	  }
        | '#' YYRNUMBER
          {
	     YYTRACE("delay: '#' YYRNUMBER");
	  }  
        | '#' identifier
          {
	     YYTRACE("delay: '#' identifier");
	  }
        | '#' '(' mintypmax_expression ')'
          {
	     YYTRACE("delay: '#' '(' mintypmax_expression ')'");
	  }
        | '#' '(' mintypmax_expression ',' mintypmax_expression ')'
          {
	     YYTRACE("delay: '#' '(' mintypmax_expression ',' mintypmax_expression ')'");
	  }
        | '#' '(' mintypmax_expression ',' mintypmax_expression ',' 
                  mintypmax_expression ')'
          {
	     YYTRACE("delay: '#' '(' mintypmax_expression ',' mintypmax_expression ',' mintypmax_expression ')'");
	  }
        ;
*/
delay_control//ignore delay
        : '#' YYINUMBER
          {
	     YYTRACE("delay_control: '#' YYINUMBER");
	  }
        | '#' YYRNUMBER
          {
	     YYTRACE("delay_control: '#' YYRNUMBER");
	  }  
        | '#' identifier
          {
	     YYTRACE("delay_control: '#' identifier");
	  }
        | '#' '(' mintypmax_expression ')'
          {
	     YYTRACE("delay_control: '#' '(' mintypmax_expression ')'");
	  }
        ;

event_control // use in always
 /*       : '@' identifier      // It may be used in the BNF "lvalue = event_control expression"
          {                     // No supply
	     YYTRACE("event_control: '@' identifier");
             
	  }*/
        : '@' '(' event_expression ')'
          {
	     YYTRACE("event_control: '@' '(' event_expression ')'");
             $$ = $3;
	  }
        | '@' '(' ored_event_expression ')'
          {
	     YYTRACE("event_control: '@' '(' ored_event_expression ')'");
             $$ = new VlpOrNode($3);
             BaseModule :: setCDFGNode($$);
	  }
        | '@' '*'
          {
             /*list the sensitive list
               statement : assignment
                         | if
                         | case casex casez
                         | lvalue '='
                         | lvalue '<='
                         | for (X).... not support 
             */             
             LY->allSensitive = true;
          }
        | '@' '(' '*' ')'
          {
             LY->allSensitive = true;
          }
        ;

/*
 * event_expression: event_expression YYOR event_expression 
 *
 * is replaced with
 *
 * event_expression: event_expression YYOR expression
 */
event_expression
        : expression
          {
	     YYTRACE("event_expression: expression");
             $$ = new VlpEdgeNode(EDGETYPE_SENSITIVE, $1);
             BaseModule :: setCDFGNode($$);
	  }
        | YYPOSEDGE expression
          {
	     YYTRACE("event_expression: YYPOSEDGE expression");
             $$ = new VlpEdgeNode(EDGETYPE_POSEDGE, $2);
             BaseModule :: setCDFGNode($$);
	  }
        | YYNEGEDGE expression
          {
	     YYTRACE("event_expression: YYNEGEDGE expression");
             $$ = new VlpEdgeNode(EDGETYPE_NEGEDGE, $2);
             BaseModule :: setCDFGNode($$);
	  }
	| YYEDGE expression
	  {
	     YYTRACE("event_expression: YYEDGE expression");
             $$ = new VlpEdgeNode(EDGETYPE_EDGE, $2);
             BaseModule :: setCDFGNode($$);
          }
        ;

ored_event_expression
        : event_expression or_expression event_expression
          {
	     YYTRACE("ored_event_expression: event_expression YYOR event_expression");
             $1->setNext($3);
             LY->nodeTailPtr2 = $3;
             $$ = $1;
	  }
        | ored_event_expression or_expression event_expression
          {
	     YYTRACE("ored_event_expression: ored_event_expression YYOR event_expression");
             LY->nodeTailPtr2->setNext($3);
             LY->nodeTailPtr2 = $3;
	  }
        ;

or_expression  // new rule, added @090102
        : YYOR
        | YYLOGOR
        | ','
        ;

%%

void yyerror(char* str)
{
    cerr << "YYERROR : No support syntax in line " << LY->lineNo << endl;
    exit (1);
}



