/****************************************************************************
  FileName     [ cktCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_CELL_H
#define CKT_CELL_H

#include <string>
#include <iostream>
#include <queue>
#include <stack>

#include "cktDefine.h"
#include "cktEnum.h"
#include "bv4.h"

using namespace std;

//#define EVENT_DRIVEN_SIM

#define CELL_DFS_MASK      1
#define CELL_ADDED_MASK    2
#define CELL_SIM_CHANGE    4
#define CELL_EXPR_BLOCK    16

//---------------------------
//  Forward declaration
//---------------------------
class CktOutPin;
class CktInPin;
class SynBus;


//---------------------------
//  class definitions
//---------------------------
class CktCell 
{
   public:
      // Constructors and Destructor
      CktCell(CktCellType = CKT_ERROR_CELL);
      CktCell(CktCellType, bool, CktInPin*&, CktOutPin*&);//1 inpin
      CktCell(CktCellType, bool, CktInPin*&, CktInPin*&, CktOutPin*&);//2 inpin
      virtual ~CktCell();

      // Attribute Setting Functions
      void setType(CktCellType = CKT_ERROR_CELL);
      void setID(unsigned long&);
      void setValue(int);
      void setValue(Bv4&);

      // Attribute Descriptive Functions
      virtual string getOpStr() const;
      virtual string getExpr(CktOutPin*) const;
      virtual CktOutPin* getOutPin() const;
      virtual unsigned getOutWidth() const;
      virtual unsigned getFoutSize() const;
      CktCellType getCellType() const;
      string getCellTypeString() const;
      unsigned long getCellID() const;
      unsigned getInPinSize() const;
      unsigned getInPinNum(CktInPin*) const;
      string getOutPinName() const;
      CktInPin* getInPin(unsigned) const;
      CktCell* getFinCell(unsigned) const;
      CktCell* getFoutCell(unsigned) const;
      int getValue() const;

      // Operating Functions
      virtual void connOutPin(CktOutPin* = NULL);
      virtual void collectFin(queue<CktCell*>&) const;
      virtual void nameOutPin(int&);
      virtual CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
      virtual void pinsDuplicate(CktCell*&, string&, OutPinMap&, InPinMap&) const; // only BusCell overload pinsDuplicate(...)
      virtual void connDuplicate(CktCell*&, OutPinMap&, InPinMap&) const;
      virtual void checkPin() const;
      void connInPin(CktInPin* = NULL);
      void resetInPinSize(const unsigned);
      CktCell* copy() const;
      virtual void calLevel(unsigned);

      // Write Output Functions
      virtual void writeOutput() const;
      virtual void writeBLIF() const;

      // Write Output Helper Functions
      string genModuleInst(string, string&, const SynBus*&, bool) const;
      string genModuleInst(string, CktOutPin*) const;
      string genModuleInst(string, CktOutPin*, string) const;
      string genModuleInst(string, string&, int, int) const;
      //string genWireName(CktOutPin*, int, int) const;
      string genWireInst(CktOutPin*) const;
      string genWireInst(string&, int) const;
      //string genRegInst(string, int, int);
      void genOutputOneOprd() const;
      void genOutputTwoOprd() const;
      void genLogicOutputTwoOprd() const;
      void genOutputRelaCell() const;
      void genOutput() const;
      void genOutputArith() const;
      void printCell() const;
      
      // Circuit Traversal Functions
      inline void setTraverse(bool mask) { _mask = (mask) ? (_mask | CELL_DFS_MASK) : (_mask & ~CELL_DFS_MASK); }
      inline bool isTraversed() const { return _mask & CELL_DFS_MASK; }
      void DFSCktTraversal(unsigned&) const;
      bool DFSWriteCktToBound(ofstream&, const unsigned = 0) const;
      void DFSWriteCktTraversal(ofstream&, const unsigned = 0, const bool = true) const;
      
      // This is for temporary usage (verifyExtend)
      void DFSCktTraversal() const;

      // Simulation Functions
      virtual void simulate();
      virtual Bv4 getSimValue(const CktOutPin*) const;
      inline const Bv4 getSimValue() const { return _simValue; }

      // Event-Driven Simulation Usage : Note that value == 0 indicates value changed
      virtual void updateSimToggle();
      inline void setSimToggle(bool o) { _mask = (o) ? (_mask & ~CELL_SIM_CHANGE) : (_mask | CELL_SIM_CHANGE); }
      inline bool isSimToggle() const { return !(_mask & CELL_SIM_CHANGE); }

      void set_sUtil(size_t = 0);
      void set_pUtil(void* = NULL);
      size_t get_sUtil() const;
      void*  get_pUtil() const;
      bool isUseUtilP() const;
   
      // Expression Blockage Indication
      inline void setExprBlock() { _mask |= CELL_EXPR_BLOCK; }
      inline void resetExprBlock() { _mask &= (~CELL_EXPR_BLOCK); }
      inline bool isExprBlock() const { return _mask & CELL_EXPR_BLOCK; }
   
      inline void setAddedCell() { _mask |= CELL_ADDED_MASK; }
      inline bool isAddedCell() const { return _mask & CELL_ADDED_MASK; }

      bool setVtRef();
      static void incVtRef() { ++_vtRef; }

      static Bv4 _1bitOne;
      static Bv4 _1bitZero;
      static unsigned _vtRef;
   protected:
      Bv4            _simValue;
      unsigned       _level;
      unsigned       _vtNum;
   private:
      // Write Output Helper Functions
      string getDVLType1(int) const;
      string getDVLType2(int) const;
      // Data Members
      unsigned long  _cellID;
      CktInPinAry    _inPinAry;
      CktOutPin*     _outPin;
      CktCellType    _cellType;
      char           _mask;  // Miscellaneous Markers
      // Utility Union
      union { size_t _util_t; void*  _util_p; } _util;
};

#endif // CKT_CELL_H
