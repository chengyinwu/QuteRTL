/****************************************************************************
  FileName     [ cktIoCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit I/O cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_IO_CELL_H
#define CKT_IO_CELL_H

//---------------------------
//  system include
//---------------------------
#include <sstream>
#include <string>
#include <fstream>
#include <queue>

using namespace std;

//---------------------------
//  user include
//---------------------------
#include "cktCell.h"
#include "cktEnum.h"
#include "synVar.h"

//---------------------------
//  Forward declaration
//---------------------------


//---------------------------
//  class definitions
//---------------------------
class CktPiCell : public CktCell 
{
public:
   CktPiCell();
   CktPiCell(bool, CktOutPin*&);
   ~CktPiCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   void writeOutput() const;
   void writeBLIF_PI() const;

   // Operating Functions
   void travelDP(CktInPinAry&, const CktInPin*, PinLitMap&, int&, unsigned);
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void updateHierConn(string&, InPinMap&) const;
   void simulate();

   // PI Related Operating Functions
   void updateSimToggle() {;}
   void setInputValue(Bv4&);
   void resetPI();
private:
};

class CktPoCell : public CktCell 
{
public:
   CktPoCell();
   CktPoCell(bool, CktInPin*&, CktOutPin*&);
   ~CktPoCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   void writeOutput() const;
   void writeBLIF_PO() const;
   void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void updateHierConn(string&, OutPinMap&, InPinMap&) const;
   void initDP(CktDpMap&);
   void simulate();

private:
};

class CktPioCell : public CktCell 
{
public:
   CktPioCell();
   CktPioCell(bool, CktInPin*&, CktOutPin*&);
   ~CktPioCell();

   // Attribute Descriptive Functions
   inline string getOpStr() const { return ""; }
   void writeOutput() const;
   //void writeBLIF_PIO() const;
   //void writeBLIF() const;

   // Operating Functions
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void updateHierConn(string&, OutPinMap&, InPinMap&) const;
   //void simulate();
private:
};

#endif // CKT_IO_CELL_H
