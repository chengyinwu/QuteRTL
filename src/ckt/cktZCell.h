/****************************************************************************
  FileName     [ cktZCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit Z cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_ZCELL_H
#define CKT_ZCELL_H

//---------------------------
//  system include
//---------------------------
#include <string>
using namespace std;

//---------------------------
//  user include
//---------------------------
#include "cktCell.h"

//---------------------------
//  Forward declaration
//---------------------------

//---------------------------
//  class definitions
//---------------------------
class CktBufifCell : public CktCell
/*
   CktBufifCell is bufif1, that is , if condition is true, then propagate signal, otherwise, high impedence
               |-------|
        data---|0      |---output
               |  Bufif|
    condition--|1      |
               |-------|

*/
{
public:
   CktBufifCell();
   CktBufifCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
   ~CktBufifCell();

   void writeOutput() const;
   void simulate();

   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void writeBLIF() const;

private:
};

class CktInvifCell : public CktCell
{
public:
   CktInvifCell();
   ~CktInvifCell();
   void simulate();
private:
};

// Multidriven
class CktBusCell : public CktCell
{
public:
   CktBusCell();
   CktBusCell(bool, CktInPin*&, CktOutPin*&);
   ~CktBusCell();

   void writeOutput() const;
   void simulate();

   void insertBusId(unsigned short);
   unsigned getBusId(unsigned) const;
   void updateIoWidth() const;
   CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
   void pinsDuplicate(CktCell*&, string&, OutPinMap&, InPinMap&) const;
   void writeBLIF() const;
   bool isMergeCell() const;
   void transToMergeCell(CktCell*&);
   void checkIoWidth() const;
private:
   BusIdArray _busIdList; //use in completeFlatten and retain the original 
                          //busId info. which is used in writeBLIF.
                          
};

class CktZ2XCell : public CktCell
{
public:
private:
};

#endif // CKT_IO_CELL_H
