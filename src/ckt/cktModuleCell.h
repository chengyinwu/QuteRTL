/****************************************************************************
  FileName     [ cktModuleCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit module cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_MODULE_CELL_H
#define CKT_MODULE_CELL_H

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
#include "vlpDefine.h"

//---------------------------
//  Forward declaration
//---------------------------
class CktModule;


//---------------------------
//  class definitions
//---------------------------
class CktModuleCell : public CktCell
{
public:
   CktModuleCell();
   ~CktModuleCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   string getExpr(CktOutPin*) const;
   void writeOutput() const;
   //void writeBLIF() const;

   // Basic Cell Function Overloads
   void       connOutPin(CktOutPin*);
   void       replaceOutPin(CktOutPin*, CktOutPin*);
   CktOutPin* getOutPin(unsigned) const;
   CktOutPin* getOutPin() const;
   unsigned   getFoutSize() const;
   unsigned   getOutPinSize() const;
   //void       eraseOutpin(CktOutPin*);
   //void insertInPinMap( CktCell * );
   //void insertOutPinMap( CktCell * );
   
   CktModule* getModulePtr() const;
   void       setModulePtr(CktModule*);
   void nameOutPin(int&);
   void                 setParamPair(ParamPairAry&);
   void                 setOneParam(paramPair);
   const ParamPairAry*  getParamPair() const;

   void   setModuleName(string);
   string getModuleName() const;
   void   setInstName(string);
   string getInstName() const;

   // Operating Functions
   void updateHierConn(string&, InPinMap&, OutPinMap&, bool) const;
   void simulate();

// No any uses the members "_inPinMap" and "_outPinMap", so I command
// the relational functions.(Louis)
private:
   void updateHierIn(string&, OutPinMap&, bool) const;
   void updateHierOut(string&, InPinMap&, bool) const;

   CktModule*     _module;    //null pointer..need a handling function to set it.(Louis)
                              //can use VLDesign.getCktModule(_moduleName) to find the CktModule*
   string         _instName;
   string         _moduleName;
   CktOutPinAry   _outPinAry;
   //CktCellAry   _inPinMap;   // map to the PI/PO/PIO of _module
   //CktCellAry   _outPinMap;  // map to the PI/PO/PIO of _module
   ParamPairAry   _paramAry;   //for parameter overloading
};

#endif // CKT_MODULE_CELL_H

