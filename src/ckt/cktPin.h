/****************************************************************************
  FileName     [ cktPin.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit pin data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_PIN_H
#define CKT_PIN_H

//---------------------------
//  system include
//---------------------------
#include "cktDefine.h"
#include "cktEnum.h"
#include <string>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

class CktCell;
class CktDffCell;
class CktOutPin;
class CktInPin;

class CktInPin 
{
   public:
      CktInPin();
      ~CktInPin();
      
      void connCell(CktCell*);
      CktCell* getCell() const;
      void connOutPin(CktOutPin*);
      CktOutPin* getOutPin() const;
      CktCell* getFinCell() const;
      CktInPin* pinDuplicate(CktCell*&, InPinMap&) const;

   private:
      CktCell*   _cell;
      CktOutPin* _fromPin;
};

class CktOutPin 
{
   public:
      CktOutPin();
      ~CktOutPin();
      
      void setName(CktString);
      CktString getName() const;
      
      void connCell(CktCell*);
      CktCell* getCell() const;
      CktCell* getFoutCell(unsigned) const;
      void connInPin(CktInPin*);
      void eraseInPin(CktInPin*);
      CktInPin* getInPin(unsigned) const;
      unsigned getInPinSize() const;
      
      void     setBusId(unsigned);
      unsigned getBusId() const;
      unsigned getBusBegin() const;
      unsigned getBusEnd() const;
      unsigned getBusWidth() const;
      unsigned getBusMSB() const;
      unsigned getBusLSB() const;
      
      void combOutPin(CktOutPin*);
      void connInOutPin(CktOutPin*);
      CktDffCell* connFF(CktOutPin*);
      void erase(CktInPin*);

      void refCtrIncre();
      void refCtrDecre();
      void setRefCtr(int);
      int getRefCtr() const;
      
      void setCoutBits(int);
      int getCoutBits() const;
      
      CktOutPin* genInvCell();
      CktOutPin* genGreaterCell();
      CktOutPin* pinDuplicate(CktCell*&, string&, OutPinMap&) const;
      CktOutPin* getSplitPin(unsigned);
      void checkBusWidth(CktOutPin*);
      bool isConst() const;
      bool isConstAllZ() const;
      bool isConstPartZ() const;
      bool isConstHasX() const;
      bool is1BitIoPin() const;
      
      void connLhs(string, bool);
   
   private:
      void connCell2NewBus(CktOutPin*);
      void connCell2Cell(CktOutPin*);
      void connCell2OldBus(CktOutPin*);
      void connMulitDriven(CktOutPin*);
      
      CktString         _netName;
      CktCell*          _cell;
      CktInPinAry       _toPinList;
      unsigned          _busId;
      int               _coutBits;
      int               _refCtr;
};

#endif // CKT_PIN_H

