/****************************************************************************
  FileName     [ cktEnum.h ]
  PackageName  [ ckt ]
  Synopsis     [ Public enumeration types for ckt package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ] 
****************************************************************************/

#ifndef CKT_ENUM_H
#define CKT_ENUM_H

#include <vector>
#include <string>
#include <map>

using namespace std;

//---------------------------
//  Forward declaration
//---------------------------


//---------------------------
//  Type definition
//---------------------------


//---------------------------
//  Enum types
//---------------------------


enum CktIoType
{
   CKT_IN    = 0,
   CKT_OUT   = 1,
   CKT_IO    = 2,
   CKT_TOTAL
};

enum CktCellType
{
   // module
   CKT_MODULE_CELL,  // 0

   // I/O
   CKT_PI_CELL,      // 1
   CKT_PO_CELL,      // 2
   CKT_PIO_CELL,     // 3

   // sequential
   CKT_DFF_CELL,     // 4
   CKT_DLAT_CELL,    // 5

   // logic
   CKT_BUF_CELL,     // 6
   CKT_INV_CELL,     // 7
   CKT_AND_CELL,     // 8
   CKT_OR_CELL,      // 9
   CKT_NAND_CELL,    // 10
   CKT_NOR_CELL,     // 11
   CKT_XOR_CELL,     // 12
   CKT_XNOR_CELL,    // 13
   CKT_MUX_CELL,     // 14
   CKT_DEMUX_CELL,   // 15
   CKT_ONEHOT_CELL,  // 16
   CKT_CASE_CELL,     // 17

   // arithmetic
   CKT_ADD_CELL,     // 18
   CKT_SUB_CELL,     // 19
   CKT_MULT_CELL,    // 20
   CKT_DIV_CELL,     // 21
   CKT_MODULO_CELL,  // 22
   CKT_SHL_CELL,     // 23
   CKT_SHR_CELL,     // 24
   CKT_RTL_CELL,     // 25
   CKT_RTR_CELL,     // 26

   // Z generator
   CKT_BUFIF_CELL,   // 27
   CKT_INVIF_CELL,   // 28
   CKT_BUS_CELL,     // 29
   CKT_Z2X_CELL,     // 30

   // modeling
   CKT_CONST_CELL,   // 31
   CKT_SPLIT_CELL,   // 32
   CKT_MERGE_CELL,   // 33
   CKT_INOUT_CELL,   // 34
   CKT_MEMORY_CELL,  // 35
   //CKT_WEAK_CELL,    // 35

   // equality, now declare in cktLogicCell.h
   CKT_EQUALITY_CELL,// 36
   CKT_GEQ_CELL,     // 37
   CKT_GREATER_CELL, // 38
   CKT_LEQ_CELL,     // 39
   CKT_LESS_CELL,    // 40
   
   //constant
   CKT_CONSTANT_CELL,// 41
   
   //error
   CKT_ERROR_CELL,   // 42

   // dummy end
   CKT_DUMMY_END     // 43
};
/*
const char *CktCellTypeStr[] = {
"module",     // 0
"pi",         // 1	
"po",         // 2
"pio",        // 3
"dff",        // 4
"dlat",       // 5
"buf",        // 6
"inv",        // 7
"and",        // 8
"or",         // 9
"nand",       // 10
"nor",        // 11
"xor",        // 12
"xnor",       // 13
"mux",        // 14
"demux",      // 15
"onehot",     // 16
"case",       // 17
"add",        // 18
"sub",        // 29
"mult",       // 20
"div",        // 21
"modulo",     // 22
"shl",        // 23
"shr",        // 24
"rtl",        // 25
"rtr",        // 26
"bufif",      // 27
"invif",      // 28
"bus",        // 29
"z2x",        // 30
"const",      // 31
"split",      // 32
"merge",      // 33
"inout",      // 34
"weak",       // 35
"equality",   // 36
"geq",        // 37
"greater",    // 38
"leq",        // 39
"less",       // 40
"constant",   // 41
"error",      // 42
"dummy"       // 43
};
*/

enum VAR_TYPE
{
   input_net_transition         = 0,
   total_output_net_capacitance = 1,
   input_transition_time        = 2,
   constrained_pin_transition   = 3,
   related_pin_transition       = 4,
   var_total                     
};

enum TRAN_TYPE
{
   cell_rise          = 0,
   cell_fall          = 1,
   rise_transition    = 2,
   fall_transition    = 3,
   tim_toal
};

#endif // CKT_ENUM_H

