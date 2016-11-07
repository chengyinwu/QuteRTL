/****************************************************************************
  FileName     [ cktLogicCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit logic cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_LOGIC_CELL_H
#define CKT_LOGIC_CELL_H

//---------------------------
//  system include
//---------------------------
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

//---------------------------
//  user include
//---------------------------
#include "cktCell.h"
#include "cktEnum.h"

#include "synVar.h"
//---------------------------
//  Global variables
//---------------------------

/* Note :
 * Type
 * ==================================================
 * None          | Buf      
 * Reduced       |         And Or  Nand Nor Xor Xnor
 * BitWise       |     Inv And Or           Xor Xnor
 * Boolean_Logic |         And Or
 * Arith_Logic   | Eq  Gre Geq Leq Less
 * ==================================================
 *  1. No logic xor in Verilog operator, the same as BW-Xor
 *  2. Only bitwise inv, Logic Inv is implemented with a GREATER cell
*/

//---------------------------
//  Forward declaration
//---------------------------
enum CktOpType
{
   CKT_BW  = 0,
   CKT_RED = 1,
   CKT_LOG = 2,
};
//---------------------------
//  class definitions
//---------------------------
class CktBufCell : public CktCell
{
public:
   CktBufCell();
   CktBufCell(bool, CktInPin*&, CktOutPin*&);
   ~CktBufCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   string getExpr(CktOutPin*) const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   void travelDP(CktInPinAry&, const CktInPin*, PinLitMap&, int&, unsigned); 
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void initDP(CktDpMap&);
   bool forceEliminate();
   bool eliminate();
   void simulate();

private:
};

class CktInvCell : public CktCell
{
public:
   CktInvCell();
   CktInvCell(bool, CktInPin*&, CktOutPin*&);
   ~CktInvCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return (getOutWidth() > 1) ? "~" : "!"; }
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
};

class CktAndCell : public CktCell
{
public:
   CktAndCell();
   CktAndCell(bool, CktInPin*&, CktOutPin*&);//reduction cell constructor
   CktAndCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&); //logic and bitWise cell constructor
   ~CktAndCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return (getType() == CKT_LOG) ? "&&" : "&"; }
   CktOpType getType() const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void handleDP(CktInPinAry&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
   void writeLogBLIF() const;
   void writeBwBLIF() const;
   void writeRedBLIF() const;
};

class CktOrCell : public CktCell
{
public:
   CktOrCell();
   CktOrCell(bool, CktInPin*&, CktOutPin*&);//reduction cell constructor
   CktOrCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);//logic and bitWise cell constructor
   ~CktOrCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return (getType() == CKT_LOG) ? "||" : "|"; }
   CktOpType getType() const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void handleDP(CktInPinAry&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
   void writeLogBLIF() const;
   void writeBwBLIF() const;
   void writeRedBLIF() const;
};

class CktNandCell : public CktCell
{
public:
   CktNandCell();
   CktNandCell(bool, CktInPin*&, CktOutPin*&);//reduction cell constructor
   CktNandCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);//bitWise cell constructor
   ~CktNandCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return (getType() == CKT_LOG) ? "!&&" : "!&"; }
   CktOpType getType() const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
};

class CktNorCell : public CktCell
{
public:
   CktNorCell();
   CktNorCell(bool, CktInPin*&, CktOutPin*&);//reduction cell constructor
   CktNorCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);//bitWise cell constructor
   ~CktNorCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return (getType() == CKT_LOG) ? "!||" : "!|"; }
   CktOpType getType() const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
};

class CktXorCell : public CktCell
{
public:
   CktXorCell();
   CktXorCell(bool, CktInPin*&, CktOutPin*&);//reduction cell constructor
   CktXorCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);//bitWise cell constructor
   ~CktXorCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return "^"; }
   CktOpType getType() const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
   void writeBwBLIF() const;
   void writeRedBLIF() const;
};

class CktXnorCell : public CktCell
{
public:
   CktXnorCell();
   CktXnorCell(bool, CktInPin*&, CktOutPin*&);//reduction cell constructor
   CktXnorCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);//bitWise cell constructor
   ~CktXnorCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return "!^"; }
   CktOpType getType() const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
   void writeBwBLIF() const;
   void writeRedBLIF() const;
};

class CktMuxCell : public CktCell
{
public:
   CktMuxCell();
   CktMuxCell(bool, CktInPin*&, CktInPin*&, CktInPin*&, CktOutPin*&);
   ~CktMuxCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return "?:"; }
   string getExpr(CktOutPin*) const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   void travelDP(CktInPinAry&, const CktInPin*, PinLitMap&, int&, unsigned);
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void simulate();

private:
};

class CktEqualityCell : public CktCell
{
public:
   CktEqualityCell();
   CktEqualityCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&); 
   ~CktEqualityCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return "=="; }
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void handleDP(CktInPinAry&) const;
   bool isLogicEq() const;
   void setCaseEq();
   void setLogicEq();
   void simulate();

private:
   bool _eq;// true: ==, false: === // no use... all is the "=="
};

class CktGreaterCell : public CktCell
{
public:
   CktGreaterCell();
   CktGreaterCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
   ~CktGreaterCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ">"; }
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void simulate();

private:
};
   
class CktGeqCell : public CktCell
{
public:
   CktGeqCell();
   CktGeqCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
   ~CktGeqCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ">="; }
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void simulate();

private:
};
   
class CktLeqCell : public CktCell
{
public:
   CktLeqCell();
   CktLeqCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
   ~CktLeqCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return "<="; }
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void simulate();

private:
};

class CktLessCell : public CktCell
{
public:
   CktLessCell();
   CktLessCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
   ~CktLessCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return "<"; }
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void simulate();

private:
};

/*class CktCaseCell : public CktCell  //no use
{
public:
   CktCaseCell();
   ~CktCaseCell();

   void       connCond( CktInPin * );
   CktInPin * getCond() const;

   void       connVarCond( CktInPin * );
   CktInPin * getVarCond(unsigned) const;
   int        getVarCondListSize() const;
   void simulate();
private:
   CktInPin*   _cond; //condition for each case item
   CktInPinAry _varCondList; //condition used to compare with _cond
};*/

#endif // CKT_LOGIC_CELL_H
