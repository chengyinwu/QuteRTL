/****************************************************************************
  FileName     [ cktModelCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit modeling cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_MODEL_CELL_H
#define CKT_MODEL_CELL_H

//---------------------------
//  system include
//---------------------------
#include <string>
#include <iostream>

using namespace std;

//---------------------------
//  user include
//---------------------------
#include "cktCell.h"
#include "cktEnum.h"
#include "bv4.h"

//---------------------------
//  Forward declaration
//---------------------------


//---------------------------
//  class definitions
//---------------------------
class CktConstCell : public CktCell
{
public:
   CktConstCell();
   CktConstCell(int, string value = "");
   CktConstCell(CktOutPin*&, int, string sValue = "");
   CktConstCell(CktOutPin*&, Bv4*, string sValue = "");
   ~CktConstCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   string getExpr(CktOutPin*) const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void simulate();

   // CONST Related Operating Functions
   void setBvValue(Bv4*);
   Bv4* getBvValue() const;
   void   setValueStr(string);
   string getValueStr() const;

private:
   Bv4   *_value;
   string _valueStr;
};

/* splitCell have two conditions
condition 1   eg.   {a, b,c } = ...
                 or Adder(.out({a, b, c}), .. )
 _isSplitFin = false
  a, b ,c have order bus  

             |--------|---- inPin 0  a  (MSB)
  {...} -----| spCell |---- inPin 1  b
             |--------|---- inPin 2  c  (LSB)

condition 2   eg.  B = A[3:2]
                   C = A[4:1]
 _isSplitFin = true
 input0, input1 don't have order bus

             |--------|---- outPin 0  A[3:2] (will be renamed)
 A[7:0] -----| spCell |
             |--------|---- outPin 1  A[4:1] (will be renamed)


             |--------|
             |[7:5]   |---- outPin 0  k12[2:0] (renamed)
 A[7:0] -----| spCell |
             |[4:0]   |---- outPin 1  k13[4:0] (renamed)
             |--------|
splited bus info([7:5], [4:0]) is saved in _busIdList
*/
class CktSplitCell : public CktCell
{
public:
   CktSplitCell();
   CktSplitCell(bool, CktInPin*&, vector<CktOutPin*>&);
   ~CktSplitCell();

   // Basic Cell Operation Overloads
   void       connOutPin(CktOutPin* const);
   void       replaceOutPin(CktOutPin*, CktOutPin*);
   unsigned   getOutPinSize() const;
   CktOutPin* getOutPin(unsigned) const;
   CktOutPin* getOutPin() const;
   unsigned   getFoutSize() const;
   //void       eraseOutpin(CktOutPin*);
   CktCell*   getFoutCell(unsigned, unsigned) const;
   CktOutPin* genFoutPin(unsigned);

   void insertBusId(unsigned short);
   void checkIoWidth();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   string getExpr(CktOutPin*) const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void connDuplicate(CktCell*&, OutPinMap&, InPinMap&) const;
   void simulate();

   // SPLIT Related Operating Functions
   Bv4 getSimValue(const CktOutPin*) const;
   void nameOutPin(int&);
   void setNonSpFin();
   bool isSplitFin() const;
   void setFinBusId(unsigned short);
   unsigned getBusId(int) const;
   void reviseBusId(unsigned, unsigned);
   void reviseIoBus();
private:
   bool _isSplitFin;
   CktOutPinAry _splitOut;
   BusIdArray _busIdList;       // use in completeFlatten and retain the original 
   unsigned short _oriFinBusId; // busId info. which is used in writOutput, writeBLIF, and simulation.
};

class CktMergeCell : public CktCell
{
public:
   CktMergeCell();
   CktMergeCell(bool, CktOutPin*&);
   ~CktMergeCell();

   // Basic Cell Operation Overloads
   void insertBusId(unsigned short);
   unsigned getBackUpBusId(int) const;
   void checkIoWidth() const;

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   string getExpr(CktOutPin*) const;
   void writeOutput() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void pinsDuplicate(CktCell*&, string&, OutPinMap&, InPinMap&) const;
   void simulate();

private:
   BusIdArray _busIdList; //use in completeFlatten and retain the original 
};

// Temporary cells for bidirectional IO modeling
// 1. assign a = b;
// 2. inout a, b;
class CktInoutCell : public CktCell
{
public:
   CktInoutCell();
   ~CktInoutCell();
   void simulate();
private:
};

class CktMemCell : public CktCell
{
public:
   CktMemCell(string&, CktOutPin*, CktOutPin*, CktOutPin*);
   ~CktMemCell();
   void setSize(unsigned&, int&, int&);
   void setRdInfo(bool, bool);
   void setWrInfo(bool, bool);
   void setSync(bool);
   void connRowOutPin(int, CktOutPin*);
   void connRowInPin(int, CktInPin*);
   void simulate();
private:
   // Info
   string _name;           // mem name
   unsigned _busId;
   int _szS;
   int _szE;
   bool _isReadVariable;
   bool _isReadConst;
   bool _isWriteVariable;
   bool _isWriteConst;
   bool _isSync;

   // I/O
   CktInPin* _clk;
   CktInPin* _wrAddress;
   CktInPin* _rdAddress;

   map<int, CktInPin*>  _inPinMap;  // input data to mem[const_row]
   map<int, CktOutPin*> _outPinMap; // output data from mem[const_row]
};

#endif // CKT_MODEL_CELL_H
