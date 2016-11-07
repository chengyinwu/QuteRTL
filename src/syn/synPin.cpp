/****************************************************************************
  FileName     [ synPin.cpp ]
  Package      [ syn ]
  Synopsis     [ CktPin functions for synthesis ]
  Author       [ Chun-Fu(Joe) Huang, Hu-Hsi Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_PIN_SOURCE
#define SYN_PIN_SOURCE

//---------------------------
//  system include
//---------------------------
#include <iostream>
#include <sstream>
#include <string>

//---------------------------
//  user include
//---------------------------
#include "cktModuleCell.h"
#include "cktLogicCell.h"
#include "cktZCell.h"
#include "cktSeqCell.h"
#include "VLGlobe.h"

//---------------------------
//  Global variables
//---------------------------
using namespace std;
extern SYN_usage*       SYN;
extern bool             busDirectionDefault;
extern VlpDesign&       VLDesign;
extern const VlgModule* curVlpModule;

//function definition
/*void
CktOutPin::connLhs(string varName, bool isMergeFull)
{
//louius debug s
   assert (getCell() != 0);
   if (getCell()->getCellType() == CKT_MODULE_CELL) {
      const CktModuleCell* mcell = static_cast<const CktModuleCell*>(getCell());
      unsigned i = 0;
      for (; i < mcell->getOutPinSize(); ++i)
         if (mcell->getOutPin(i) == this)
            break;

      assert (i < mcell->getOutPinSize());
   }
   else if (getCell()->getCellType() == CKT_SPLIT_CELL) {
      const CktSplitCell* scell = static_cast<const CktSplitCell*>(getCell());
      unsigned i = 0;
      for (; i < scell->getOutPinSize(); ++i)
         if (scell->getOutPin(i) == this)
            break;
      
      assert (i < scell->getOutPinSize());
   }
   else
      assert (getCell()->getOutPin() == this);
//louius debug e

   CktOutPin* varOutpin = SYN->outPinMap.getSharPin(varName);
   assert (varOutpin != 0);
  // a = b+c
  // for lhs a     
  // 3 cases:
  //    1. outpin doesn't connect to any cell, we can just return it.
  //    2. outpin connect to CktBusCell, generate a outpin that connect to the CktBusCell
  //       and return this outpin.
  //    3. outpin connect to other cells, we should divide a and original connected cell.
  //       Then use a CktBusCell in the middle of outpin and original cell. 
  //   From this To varOutpin
  // ---------o  > > >  o-------- 
   if (varOutpin != 0) {
      if (varOutpin->getCell() == 0) { // 1st case
         if (isMergeFull)
            connCell2Cell(varOutpin);
         else 
            connCell2NewBus(varOutpin);
      }
      else {                           // 2nd case       
         if (varOutpin->getCell()->getCellType() == CKT_BUS_CELL)
            connCell2OldBus(varOutpin);
         else                          // 3rd case.
            connCell2NewBus(varOutpin);
      }
   }
   else
      assert(0);
}*/


/*
   eg.    a[0:3]          a[0:7]
   |----|    this      outPinMap[id]          |----|
   |cell|-----o          o-----------inpin----|cell|
   |----|                                     |----|
    
           a[0:3]                         a[0:7]
   |----|    this           |----------| outPinMap[id]                 |----|
   |cell|-----o------o------|CktBusCell|---o---------------o--inpin----|cell|
   |----|          inpin    |----------|                               |----|
*/
/*void 
CktOutPin::connCell2NewBus(CktOutPin* varOutpin)
{
   CktCell* cell;
   CktOutPin* outpin;
   CktInPin* inpin;
   //only operation cell has sharing issue.
   CktCellType ct = getCell()->getCellType();
   if ( (ct == CKT_BUF_CELL)    || (ct == CKT_INV_CELL)  || (ct == CKT_AND_CELL)  
     || (ct == CKT_OR_CELL)     || (ct == CKT_NAND_CELL) || (ct == CKT_NOR_CELL)
     || (ct == CKT_XOR_CELL)    || (ct == CKT_XNOR_CELL) || (ct == CKT_ADD_CELL)    
     || (ct == CKT_SUB_CELL)    || (ct == CKT_MULT_CELL) || (ct == CKT_DIV_CELL) 
     || (ct == CKT_MODULO_CELL) || (ct == CKT_SHL_CELL)  || (ct == CKT_SHR_CELL) 
     || (ct == CKT_RTL_CELL)    || (ct == CKT_RTR_CELL) )
   {
      if (getRefCtr() > 1) {
         SYN->insertCktCell(new CktBufCell(true, inpin, outpin));
         connInPin(inpin); inpin->connOutPin(this);
         outpin->setBusId(getBusId());
      }
      else {
         outpin = new CktOutPin;
         cell = getCell();
         outpin->setBusId(getBusId());
         cell->connOutPin(outpin); outpin->connCell(cell);
         refCtrDecre();
      }
      inpin  = new CktInPin;
      SYN->insertCktCell( new CktBusCell(false, inpin, varOutpin) );
      outpin->setName("");
      outpin->connInPin(inpin); inpin->connOutPin(outpin);
   }
   else {
      inpin  = new CktInPin;
      SYN->insertCktCell( new CktBusCell(false, inpin, varOutpin) );
      setName("");
      connInPin(inpin); inpin->connOutPin(this);
   }
}*/


/*
   |----|  oPinLhs     outPinMap[id]             |----|
   |cell|-----o          o--------------inpin----|cell|
   |----|                                        |----|
                     ||
                     ||
                    \||/
                     \/ 
                (varOutpin)
   |----|       outPinMap[id]              |----|
   |cell|----------o--------------inpin----|cell|
   |----|                                  |----|               
*/
//Note : varOutpin->getCell() is null
/*void 
CktOutPin::connCell2Cell(CktOutPin* varOutpin)
{
   CktCell* cell;
   CktInPin* inpin;
   if (getCell() != 0) {
      if (getBusId() == varOutpin->getBusId()) {
         if (this != varOutpin) {
            if (getRefCtr() > 1) {
               inpin = new CktInPin;
               cell = new CktBufCell(false, inpin, varOutpin);
               SYN->insertCktCell(cell);
               connInPin(inpin); inpin->connOutPin(this);
            }
            else {
               refCtrDecre();
               cell = getCell();
               cell->connOutPin(varOutpin); // second
               varOutpin->connCell(cell);
            }

            if (cell->getCellType() == CKT_SPLIT_CELL)
               (static_cast<CktSplitCell*>(cell))->eraseOutpin(this);
            if (cell->getCellType() == CKT_MODULE_CELL) {
               for (unsigned i = 0; i < getInPinSize(); ++i) {
                  inpin = getInPin(i);
                  varOutpin->connInPin(inpin); inpin->connOutPin(varOutpin);
                  //varOutPin->setName();
               }
               (static_cast<CktModuleCell*>(cell))->eraseOutpin(this);
               cout << "getCell2() = " << getCell() << endl;
               delete this;
               cout << "getCell3() = " << getCell() << endl;
            }
            if (getCell()->getCellType() == CKT_BUF_CELL)
               delete this;
         }
      }
      // eg.
      //    |----|    a[3:0]     a[7:0]                   |----|
      //    |cell|-----o          o--------------inpin----|cell|
      //    |----|                                        |----|
      //    in this case, I will generate a CktBusCell which connect to a[7:0]
      else {
         cell  = new CktBusCell;
         SYN->insertCktCell(cell);
         inpin = new CktInPin;
         connInPin(inpin); inpin->connOutPin(this);
         inpin->connCell(cell); cell->connInPin(inpin);
         cell->connOutPin(varOutpin); varOutpin->connCell(cell);
      }
   }
   else {
   // always@(a)
   // begin                |---|
   //    b = a;         a--|buf|---b
   // end                  |---|
      
   // in next always block , I will get a, and a is in outPinMap, so I have to replace it, and do constant propagation
   // always@(in)
   // begin             |-----|
   //    a = 6;         |const|-- 6, connect 6 with a
   // end               |-----|                                    
   // cerr << "---No Cell!!---" << endl;
   }
}*/

/*void 
CktOutPin::connCell2OldBus(CktOutPin* varOutpin)
{
   CktCell* cell;
   CktOutPin* outpin;
   CktInPin* inpin;
   if( ((getCell()->getCellType() >= CKT_INV_CELL) && (getCell()->getCellType() <= CKT_XNOR_CELL))
     ||((getCell()->getCellType() >= CKT_ADD_CELL) && (getCell()->getCellType() <= CKT_RTR_CELL)) )
   //only operation cell has sharing issue.
   {
      if (getRefCtr() > 1) {
         SYN->insertCktCell( new CktBufCell(true, inpin, outpin) );
         connInPin(inpin); inpin->connOutPin(this);
         outpin->setBusId(getBusId());
      }
      else {
         outpin = new CktOutPin;
         cell = getCell();
         outpin->setBusId(getBusId());
         cell->connOutPin(outpin); outpin->connCell(cell);
         refCtrDecre();
      }

      cell = varOutpin->getCell();//variable's bus cell
      inpin = new CktInPin;
   
      outpin->setName("");
      outpin->connInPin(inpin); inpin->connOutPin(outpin);
      inpin->connCell(cell); cell->connInPin(inpin);            
      //outpin->setBusId(0);
   }
   else {
      cell = varOutpin->getCell();//variable's bus cell
      inpin = new CktInPin;
   
      setName("");
      connInPin(inpin); inpin->connOutPin(this);
      inpin->connCell(cell); cell->connInPin(inpin);            
      //setBusId(0);
   }
}*/

/* 3rd case.
    eg.
       always@(..)                |-----|
       begin         ---\     b---| ADD |---outpin a
          a=b+c;     ---/     c---|     |
       end                        |-----|
    
    now outpin a is assigned by another cell(mulit-driven)
    eg.
       always@(..)
       begin
          a=b+c;
       end
       
            originalCell
          |-----|   outPinMap[id], varOutpin
      b---| ADD |----a
      c---|     |
          |-----|

       always@(..)
       begin
          a=d+e;
       end
           originalCell  
          |-----| originalCellOutpin                 busCell
      d---| ADD |--------|   inpinOriginalCell    |-----------|
      e---|     |        |----------------------->|           |     outPinMap[id]
          |-----|                                 |CktBusCell |-----outpin a
                         |----------------------->|           |
          |-----|        |    inpinLhs            |-----------|
      b---| ADD |--------|
      c---|     |  this
          |-----|
 */
/*void 
CktOutPin::connMulitDriven(CktOutPin* varOutpin)
{
   CktCell* cell;
   CktOutPin* outpin;
   CktInPin* inpin;

   CktOutPin* originalCellOutpin = new CktOutPin;
   CktCell* originalCell;
   CktCell* busCell = new CktBusCell;
   SYN->insertCktCell(busCell);
   CktInPin* inpinLhs = new CktInPin;
   CktInPin* inpinOriginalCell = new CktInPin;
   
   originalCellOutpin->setBusId(varOutpin->getBusId());
   //setBusId(0);            
   originalCell = varOutpin->getCell();
   
   originalCell->connOutPin(originalCellOutpin);
   originalCellOutpin->connCell(originalCell);
   originalCellOutpin->connInPin(inpinOriginalCell);
   inpinOriginalCell->connOutPin(originalCellOutpin);
   inpinOriginalCell->connCell(busCell);
   busCell->connInPin(inpinOriginalCell);
               
   if( ((getCell()->getCellType() >= CKT_INV_CELL) && (getCell()->getCellType() <= CKT_XNOR_CELL))
     ||((getCell()->getCellType() >= CKT_ADD_CELL) && (getCell()->getCellType() <= CKT_RTR_CELL)) )
   //only operation cell has sharing issue.
   {
      if (getRefCtr() > 1) {                 
         SYN->insertCktCell( new CktBufCell(true, inpin, outpin) );
         connInPin(inpin); inpin->connOutPin(this);
         outpin->setBusId(getBusId());
      }
      else {
         outpin = new CktOutPin;
         cell = getCell();
         outpin->setBusId(getBusId());
         cell->connOutPin(outpin); outpin->connCell(cell);
         refCtrDecre();
      }
      outpin->setName("");
      outpin->connInPin(inpinLhs); inpinLhs->connOutPin(outpin);
      inpinLhs->connCell(busCell); busCell->connInPin(inpinLhs);            
      busCell->connOutPin(varOutpin); varOutpin->connCell(busCell);
   }
   else {
      setName("");
      connInPin(inpinLhs); inpinLhs->connOutPin(this);
      inpinLhs->connCell(busCell); busCell->connInPin(inpinLhs);            
      busCell->connOutPin(varOutpin); varOutpin->connCell(busCell);
   }
}*/

//|---|---o rhs     o lhs(this) |---|------o lhs(this)    delete rhs pin
//| OP|   |                -->  | OP|      |
//|---|   |---o oriFout         |---|      |-----o oriFout
// if oriFoutCell exist CktSplitCell => need another handle for the busId of the splitCell outPins
void
CktOutPin::combOutPin(CktOutPin* rhsOutPin)
{
   assert (getCell() == 0);
   CktCell* rhsCell = rhsOutPin->getCell();
   SYN->outPinMap.replace(rhsOutPin, this); // update sharing pin

   int lhsWidth = VLDesign.getBus(_busId)->getWidth();
   int rhsWidth = VLDesign.getBus(rhsOutPin->getBusId())->getWidth();
   if (lhsWidth != rhsWidth && rhsWidth > 0) {
      cerr << "Warnning : combinate different width outPins" << endl;
      cerr << "rhs name = " << rhsOutPin->getName() << " lhs name = " << getName() << endl;
   }
   if (rhsWidth == 0)
      cerr << "Warning : finWidth is zero" << endl;
   
   connCell(rhsCell);
   if (rhsCell->getCellType() == CKT_MODULE_CELL)
      static_cast<CktModuleCell*>(rhsCell)->replaceOutPin(rhsOutPin, this);
   else if (rhsCell->getCellType() == CKT_SPLIT_CELL)
      static_cast<CktSplitCell*>(rhsCell)->replaceOutPin(rhsOutPin, this);
   else
      rhsCell->connOutPin(this);

   CktInPin* inPin;
   for (unsigned i = 0; i < rhsOutPin->getInPinSize(); ++i) {
      inPin = rhsOutPin->getInPin(i);
      connInPin(inPin); inPin->connOutPin(this);
      if (inPin->getCell()->getCellType() == CKT_SPLIT_CELL) {
         assert (lhsWidth == rhsWidth);
         if (_busId != rhsOutPin->getBusId()) { // need shift bus begin and end
            CktSplitCell* foutSpCell = static_cast<CktSplitCell*>(inPin->getCell());
            if (foutSpCell->isSplitFin()) { // signal split
               int oriBegin, oriEnd; 
               unsigned oriWidth, newBusId;
               bool oriIsInv;
               int shiftNum = VLDesign.getBus(_busId)->getEnd() 
                            - VLDesign.getBus(rhsOutPin->getBusId())->getEnd();
               const SynBus* bus;
               foutSpCell->setFinBusId(_busId);
               for (unsigned j = 0; j < foutSpCell->getOutPinSize(); ++j) {
                  bus = VLDesign.getBus(foutSpCell->getBusId(j));
                  oriBegin = bus->getBegin();
                  oriEnd   = bus->getEnd();
                  oriWidth = bus->getWidth();
                  oriIsInv = bus->isInverted();
                  if (VLDesign.getBus(_busId)->isInverted() == VLDesign.getBus(rhsOutPin->getBusId())->isInverted())
                     newBusId = VLDesign.genBusId(oriWidth, oriBegin+shiftNum, oriEnd+shiftNum, oriIsInv);
                  else {
                     unsigned reverseTranNum = VLDesign.getBus(_busId)->getBegin() + VLDesign.getBus(_busId)->getEnd();
                     newBusId = VLDesign.genBusId(oriWidth, reverseTranNum - (oriEnd+shiftNum), 
                                                            reverseTranNum - (oriBegin+shiftNum), oriIsInv);
                  }              
                  foutSpCell->reviseBusId(j, newBusId);
               }
            }
         }
      }
   }
}
// if exist PIO ...                                       rhs
//|---|---o rhs         |---|   |---o lhs(this)    |---|---o---------|---|----o lhs (this)
//| OP|   |             |PIO|---|          -->     |OP |   |         |PIO|
//|---|   |             |---|                      |---|   |         |---|
//        |---o oriFout                                    |---o oriFout
void
CktOutPin::connInOutPin(CktOutPin* rhsOutPin)
{
   int lhsWidth = VLDesign.getBus(_busId)->getWidth();
   int rhsWidth = VLDesign.getBus(rhsOutPin->getBusId())->getWidth();
   if (lhsWidth != rhsWidth)
      cerr << "Warnning : combinate different width outPins" << endl;
   CktCell* pioCell = getCell();
   CktInPin* inPin = 0;
   for (unsigned i = 0; i < pioCell->getInPinSize(); ++i)
      if (pioCell->getInPin(i)->getOutPin() == 0)
         inPin = pioCell->getInPin(i);
   if (inPin == 0) {
      inPin = new CktInPin();
      inPin->connCell(pioCell); pioCell->connInPin(inPin);
   }
   inPin->connOutPin(rhsOutPin); rhsOutPin->connInPin(inPin);
}

//                                         rhs  |--|
//|---|---o rhs     o lhs           |---|---o---|FF|---o lhs
//| OP|   |                    -->  | OP|   |   |--|
//|---|   |---------o oriFout       |---|   |----------o oriFout
CktDffCell*
CktOutPin::connFF(CktOutPin* rhsOutPin)
{
   int lhsWidth = VLDesign.getBus(_busId)->getWidth();
   int rhsWidth = VLDesign.getBus(rhsOutPin->getBusId())->getWidth();
   if (lhsWidth != rhsWidth)
      cerr << "Warnning : connect FF with different width outPins" << endl;

   CktInPin*   inPin   = new CktInPin();
   CktDffCell* dffCell = new CktDffCell();
   SYN->insertCktCell(dffCell);
   dffCell->connInPin(inPin);   inPin->connCell(dffCell);
   dffCell->connOutPin(this);   connCell(dffCell);
   rhsOutPin->connInPin(inPin); inPin->connOutPin(rhsOutPin);

   return dffCell;
}

CktOutPin*
CktOutPin::getSplitPin(unsigned spBusId)
{
   string pinName = getName();
   if (pinName != "") {
      if (SYN->outPinMap.isSigExist(pinName) == false) {
         assert (0); // to check why the var doesn't exist
         SYN->outPinMap.insert(pinName, this);
      }         
      return SYN->outPinMap.genVarOutpinFout(pinName, spBusId);
   }
   else {
      CktSplitCell* spCell;
      for (unsigned i = 0; i < getInPinSize(); ++i) {
         if (getFoutCell(i)->getCellType() == CKT_SPLIT_CELL) {
            spCell = static_cast<CktSplitCell*>(getFoutCell(i));
            return spCell->genFoutPin(spBusId);
         }
      }
      spCell = new CktSplitCell();
      //if (isConst())
      //   if (!isConstHasX())
      //      spCell->setNonSpFin();
      SYN->insertCktCell(spCell);
      CktInPin* inPin = new CktInPin();
      spCell->connInPin(inPin); inPin->connCell(spCell);
      connInPin(inPin);         inPin->connOutPin(this);
      return spCell->genFoutPin(spBusId);
   }
}

/*int 
CktCell::computeCarryOutBits() const
{
   if (_cellType == CKT_MODULE_CELL)
      return 0;
   if (_cellType == CKT_SPLIT_CELL) {
      if (getInPin(0)->getOutPin()->getName() != "")
         return 0;
      else {
         if (getInPin(0)->getOutPin()->getCell() == 0)
            return 0;   
         return getInPin(0)->getOutPin()->getCell()->computeCarryOutBits();
      }
   }   
   CktOutPin* lastOutpin = getOutPin();
   if (lastOutpin->getName() != "")
      return 0;
   
   if ((_cellType == CKT_ADD_CELL)
    || (_cellType == CKT_SUB_CELL)
    || (_cellType == CKT_MULT_CELL)
    || (_cellType == CKT_SHL_CELL)
     //||(_cellType==CKT_SHR_CELL)
      )
   {
      int bits1, bits2;
      CktOutPin* outpin1;
      CktOutPin* outpin2;
      int width1, width2;
      int bits_plus_width;
      int carryOut;
      outpin1 = getInPin(0)->getOutPin();
      outpin2 = getInPin(1)->getOutPin();
      if (outpin1->getCell() != 0)
         bits1 = outpin1->getCell()->computeCarryOutBits();
      else
         bits1 = 0;      
      if(outpin2->getCell() != 0)
         bits2 = outpin2->getCell()->computeCarryOutBits();
      else
         bits2 = 0;
      width1 = VLDesign.getBus(outpin1->getBusId())->getWidth();
      width2 = VLDesign.getBus(outpin2->getBusId())->getWidth();
      if ((_cellType == CKT_ADD_CELL)
       || (_cellType == CKT_SUB_CELL) )
      {
         if ((bits1 + width1) >= (bits2 + width2) )
            bits_plus_width = bits1 + width1;
         else
            bits_plus_width = bits2 + width2;
         if (width1 >= width2)
            carryOut = bits_plus_width - width1 + 1;
         else
            carryOut = bits_plus_width - width2 + 1;
      }
      else if (_cellType == CKT_MULT_CELL) {
         int width;
         if (width1 >= width2)
            width = width1;
         else
            width = width2;
         carryOut = (bits1 + width1) + (bits2 + width2) - width;
      }
      else if (_cellType == CKT_SHL_CELL) {
         if (outpin2->getCell() != 0) {
         //it can be variable or const cell
            if (outpin2->getCell()->getCellType() == CKT_CONST_CELL)
               carryOut = static_cast<CktConstCell*>(outpin2->getCell())->getBvValue()->value();
            else {
               int bitWidth = VLDesign.getBus(outpin2->getBusId())->getWidth();
               carryOut = (int)pow(2.0, bitWidth) - 1;
            }
         }
         else {
         // it must be a variable
            int bitWidth = VLDesign.getBus(outpin2->getBusId())->getWidth();
            carryOut = (int)pow(2.0, bitWidth);
         }
      }
      else;
      lastOutpin->setCoutBits(carryOut);
      return carryOut;
   }
   else {
      int max_bits_plus_width = 0;
      int max_width = 0;
      int bits;
      int width;
      int traceSize;
      if (getInPinSize() >= 2)
         traceSize = 2;
      else
         traceSize = getInPinSize();
      for (int i = 0; i < traceSize; ++i) {
         CktOutPin* tmpOutpin = getInPin(i)->getOutPin();
         if (tmpOutpin->getCell() != 0)
            bits = tmpOutpin->getCell()->computeCarryOutBits();
         else
            bits = 0;
         width = VLDesign.getBus(tmpOutpin->getBusId())->getWidth();
         if (width > max_width)
            max_width = width;
         if ((width + bits) > max_bits_plus_width)
            max_bits_plus_width = width+ bits;
         if (_cellType == CKT_MERGE_CELL)
            tmpOutpin->setCoutBits(0);
      }
      if (_cellType == CKT_MERGE_CELL) {
         lastOutpin->setCoutBits(0);
         return 0;
      }
      else {
         bits = max_bits_plus_width - max_width;
         lastOutpin->setCoutBits(bits);
         return bits;
      }
   }
}*/

CktOutPin* 
CktOutPin::genGreaterCell()
{
   CktOutPin* cellOut;
   CktInPin*  cellIn1;
   CktInPin*  cellIn2;
   CktOutPin* opin2;
   unsigned int value = 0;
   Bv4* bvValue = new Bv4(value);
   const SynBus* tmpBus = VLDesign.getBus(getBusId());
   
   int busId = VLDesign.genBusId(tmpBus->getWidth(), 0, tmpBus->getWidth() - 1, busDirectionDefault);
   int id = SYN->constAry.isExist(bvValue, busId);
   if (id != -1) {
      delete bvValue;
      opin2 = SYN->constAry[id];
   }
   else {
      SYN->insertCktCell(new CktConstCell(opin2, bvValue));
      opin2->setBusId(busId);
      SYN->constAry.insert(opin2);
   }   
   checkBusWidth(opin2);   
   SYN->insertCktCell(new CktGreaterCell(true, cellIn1, cellIn2, cellOut));
   connInPin(cellIn1);        cellIn1->connOutPin(this);
   opin2->connInPin(cellIn2); cellIn2->connOutPin(opin2);
   busId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
   cellOut->setBusId(busId);   
   return cellOut;
}

CktOutPin* 
CktOutPin::genInvCell()
{
   CktOutPin* out;
   CktInPin*  in;
   SYN->insertCktCell(new CktInvCell(true, in, out));   
   connInPin(in); in->connOutPin(this);
   out->setBusId(_busId);
   return out;
}

void 
CktOutPin::checkBusWidth(CktOutPin* oRight)
{
   int busIdLeft  = getBusId();
   int busIdRight = oRight->getBusId();
   
   if ((VLDesign.getBus(busIdLeft)->getWidth() == 0) || (VLDesign.getBus(busIdRight)->getWidth() == 0)) {
      if ((VLDesign.getBus(busIdLeft)->getWidth() == 0) && (VLDesign.getBus(busIdRight)->getWidth() == 0))
         return ;
      else {
         if (VLDesign.getBus(busIdLeft)->getWidth() == 0) {
            if ((VLDesign.getBus(busIdRight)->getBegin() == 0) 
             && (VLDesign.getBus(busIdRight)->isInverted() == busDirectionDefault))
               setBusId(busIdRight);
            else {
               int busId = VLDesign.genBusId(VLDesign.getBus(busIdRight)->getWidth(), 0, 
                           VLDesign.getBus(busIdRight)->getWidth() - 1, busDirectionDefault);
               setBusId(busId);
            }
         }
         else { // VLDesign.getBus(busIdRight)->getWidth() == 0
            if ((VLDesign.getBus(busIdLeft)->getBegin() == 0) 
             && (VLDesign.getBus(busIdLeft)->isInverted() == busDirectionDefault))
               oRight->setBusId(busIdLeft);
            else {
               int busId = VLDesign.genBusId(VLDesign.getBus(busIdLeft)->getWidth(), 
                           0, VLDesign.getBus(busIdLeft)->getWidth() - 1, busDirectionDefault);
               oRight->setBusId(busId);
            }
         }
      }
   }
}

/* 3 case
    _____
   |const|---o outPin
   |_____|
    _____     ___
   |const|---|buf|---o outPin
   |_____|   |___|
    _____
   |const|__
   |_____|  \___|-----|  
    _____    ___|merge|---o outPin
   |const|__/   |-----|
   |_____|
*/ 
// isConst() will check the illegal format {sigmal, bv},
// but isConstAllZ() and isConstPartZ() won't.
bool
CktOutPin::isConst() const
{
   if (_cell != 0) {
      CktCell* cell = _cell;
      CktOutPin* finPin;
      if (cell->getCellType() == CKT_BUF_CELL)
         cell = cell->getFinCell(0);

      if (cell != 0) {
         if (cell->getCellType() == CKT_CONST_CELL)
            return true;
         else if (cell->getCellType() == CKT_MERGE_CELL) {
            unsigned count = 0;
            for (unsigned i = 0; i < cell->getInPinSize(); ++i) {
               finPin = cell->getInPin(i)->getOutPin();
               if (finPin->isConst())
                  ++count;
            }
            // no support {sigal, bitVector}
            assert (count == 0 || count == cell->getInPinSize());
            if (count == cell->getInPinSize())
               return true;
         }
      }
   }
   return false;
}

bool 
CktOutPin::isConstAllZ() const
{
   if (_cell != 0) {
      CktCell* cell = _cell;
      CktOutPin* finPin;
      if (cell->getCellType() == CKT_BUF_CELL)
         cell = cell->getFinCell(0);

      if (cell != 0) {
         if (cell->getCellType() == CKT_CONST_CELL) {
            Bv4* bv = static_cast<CktConstCell*>(cell)->getBvValue();
            if (bv->fullz())
               return true;   
         }  
         else if (cell->getCellType() == CKT_MERGE_CELL) {
            for (unsigned i = 0; i < cell->getInPinSize(); ++i) {
               finPin = cell->getInPin(i)->getOutPin();
               if ( !(finPin->isConstAllZ()) )
                  return false;
            }
            return true;
         }
      }
   }
   return false;
}

bool 
CktOutPin::isConstPartZ() const
{
   if (_cell != 0) {
      CktCell* cell = _cell;
      CktOutPin* finPin;
      if (cell->getCellType() == CKT_BUF_CELL)
         cell = cell->getFinCell(0);

      if (cell != 0) {
         if (cell->getCellType() == CKT_CONST_CELL) {
            Bv4* bv = static_cast<CktConstCell*>(cell)->getBvValue();
            if (bv->hasz() && !(bv->fullz()) )
               return true;         
         }  
         else if (cell->getCellType() == CKT_MERGE_CELL) {
            for (unsigned i = 0; i < cell->getInPinSize(); ++i) {
               finPin = cell->getInPin(i)->getOutPin();
               if (finPin->isConstPartZ())
                  return true;
            }
            return false;
         }
      }
   }
   return false;
}

bool
CktOutPin::isConstHasX() const
{
   assert (_cell->getCellType() == CKT_CONST_CELL 
        || _cell->getCellType() == CKT_MERGE_CELL);
   assert (isConst() == true);

   if (_cell->getCellType() == CKT_CONST_CELL)
      return static_cast<const CktConstCell*>(_cell)->getBvValue()->hasx();
   else {
      for (unsigned i = 0; i < getInPinSize(); ++i) {
         if (getInPin(i)->getOutPin()->isConstHasX())
            return true;
      }
      return false;
   }
}

#endif

