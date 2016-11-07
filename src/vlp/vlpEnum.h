/****************************************************************************
  FileName     [ vlpEnum.h ]
  Package      [ vlp ]
  Synopsis     [ Enum type of vlp package ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_ENUM_H
#define VLP_ENUM_H

using namespace std;

//==========lex and yacc===========//
enum ScopeState
{
   MODULE      = 0,
   TASK        = 1,
   FUNCTION    = 2
};
//=================================//
//static const char* nodeclass[15] = {"parameter", "assign", "always", "or",
//     "edge", "if", "case", "for", "ba_or_nba", "int", "caseitem", "signal", 
//     "operator", "connect", "default"};
enum NodeClass
{
   NODECLASS_PARAMETER   =  0,
   NODECLASS_ASSIGN      =  1,
   NODECLASS_ALWAYS      =  2,
   NODECLASS_OR          =  3,
   NODECLASS_EDGE        =  4,
   NODECLASS_IF          =  5,
   NODECLASS_CASE        =  6,
   NODECLASS_FOR         =  7,
   NODECLASS_BA_OR_NBA   =  8,
   NODECLASS_INT         =  9,
   NODECLASS_CASEITEM    = 10,
   NODECLASS_SIGNAL      = 11,
   NODECLASS_OPERATOR    = 12,
   NODECLASS_CONNECT     = 13,
   NODECLASS_FUNCALL     = 14,
   NODECLASS_TASK        = 15,
   NODECLASS_DEFAULT     = 16
};

enum CaseType
{
   CASETYPE_CASE,
   CASETYPE_CASEX,
   CASETYPE_CASEZ
};

enum EdgeType
{
   EDGETYPE_EDGE,
   EDGETYPE_POSEDGE,
   EDGETYPE_NEGEDGE,
   EDGETYPE_SENSITIVE
};

enum AssignType
{
   ASSIGNTYPE_BLOCK,
   ASSIGNTYPE_NBLOCK
};

//static const char* operatorclass[11] = {"arithmetic", "logic", "bitwise", 
//"reduction", "relation", "shift", "equality", "concatenate", "replication",
//"conditional", "default"};
enum OperatorClass
{
   OPCLASS_ARITHMETIC    =  0,
   OPCLASS_LOGIC         =  1,
   OPCLASS_BITWISE       =  2,
   OPCLASS_REDUCTION     =  3,
   OPCLASS_RELATION      =  4,
   OPCLASS_SHIFT         =  5,
   OPCLASS_EQUALITY      =  6,
   OPCLASS_CONCATENATE   =  7,
   OPCLASS_REPLICATION   =  8,
   OPCLASS_CONDITIONAL   =  9,
   OPCLASS_DEFAULT       = 10
};

enum BitWiseOpClass
{
   BITWISEOP_NOT,
   BITWISEOP_AND,
   BITWISEOP_OR,
   BITWISEOP_XOR,
   BITWISEOP_XNOR,
   BITWISEOP_DEFAULT
};

enum ArithOpClass
{
   ARITHOP_ADD,
   ARITHOP_SUBTRACT,
   ARITHOP_MULTIPLY,
   ARITHOP_UNARY_PLUS,
   ARITHOP_UNARY_MINUS,
   ARITHOP_DEFAULT
};

enum LogicOpClass
{
   LOGICOP_AND,
   LOGICOP_OR,
   LOGICOP_NOT,
   LOGICOP_DEFAULT
};

enum ReductOpClass
{
   REDUCTOP_AND,
   REDUCTOP_OR,
   REDUCTOP_NAND,
   REDUCTOP_NOR,
   REDUCTOP_XOR,
   REDUCTOP_XNOR,
   REDUCTOP_DEFAULT
};

enum RelateOpClass
{
   RELATEOP_GEQ,
   RELATEOP_GREATER,
   RELATEOP_LEQ,
   RELATEOP_LESS,
   RELATEOP_DEFAULT
};

enum ShiftOpClass
{
   SHIFTOP_RSH,
   SHIFTOP_LSH,
   SHIFTOP_DEFAULT
};

enum EqualityClass
{
   EQUALITY_LOG_EQU, //  ==
   EQUALITY_LOG_INEQU, // !=
   EQUALITY_CASE_EQU,  // ===
   EQUALITY_CASE_INEQU, //!==
   EQUALITY_DEFAULT
};

//static const char* nettype[14] = {"unspecNet", "wire", "reg", "wand", "wor",
//"tri", "tri1", "tri0", "triand", "trior", "supply0Net", "supply1Net", "swire", 
//"integer"};
enum NetType
{
   unspecNet   = 0,
   wire        = 1,
   reg         = 2,
   wand        = 3,
   wor         = 4,
   tri         = 5,
   tri1        = 6,
   tri0        = 7,
   triand      = 8,
   trior       = 9,
   supply0Net  =10,
   supply1Net  =11,
   swire       =12,
   integer     =13
};

//static const char* drivestr[11] = {"unspecStr", "supply1", "strong1", "pull1", 
//"weak1", "highz1", "highz0", "weak0", "pull0", "strong0", "supply0"};
enum DriveStr
{
   unspecStr   =  0,
   supply1     =  1,
   strong1     =  2,
   pull1       =  3,
   weak1       =  4,
   highz1      =  5,
   highz0      =  6,
   weak0       =  7,
   pull0       =  8,
   strong0     =  9,
   supply0     = 10
};


//static const char* iotype[4] = {"unspecIO", "input", "output", "inout"};
enum IOType
{
   unspecIO    = 0,
   input       = 1,
   output      = 2,
   inout       = 3 
};

enum ProcessState
{
   EVENT       = 0,
   ASSIGN      = 1,
   IO          = 2,
   NET         = 3,
   REG         = 4,
   INT         = 5,
   PARAM       = 6,
   PARAM_NOL   = 7,
   DEFINE      = 8,
   S_NULL      = 9
};

enum StateLib
{
   UNSET       = 0,
   LIBRARY     = 1,
   CELL        = 2,
   PIN         = 3,
   FUNCT       = 4
};

#endif
