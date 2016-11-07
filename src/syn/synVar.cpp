/****************************************************************************
  FileName     [ synVar.cpp ]
  Package      [ syn ]
  Synopsis     [ Function definition of synVar.h ]
  Author       [ Chun-Fu(Joe) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_VAR_SOURCE
#define SYN_VAR_SOURCE

//---------------------------
//  system include
//---------------------------

#include <string>
#include <iostream>
#include <vector>

using namespace std;
//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktModule.h"
#include "cktLogicCell.h"
#include "cktSeqCell.h"
#include "cktZCell.h"
#include "vlpItoStr.h"
#include "cktBLIFile.h"
#include "synVar.h"
#include "util.h"

//---------------------------
//  Global variables
//---------------------------
extern SYN_usage*       SYN;
extern bool             busDirectionDefault;
extern CktOutFile       cktOutFile;
extern const VlgModule* curVlpModule;
extern CktModule*       curCktModule;
extern VlpDesign&       VLDesign;

// ==========  class  PinAry   ========== //
PinAry::PinAry()
{
}

PinAry::~PinAry()
{
   _list.clear();
   _isPosList.clear();
}

void 
PinAry::insert(CktOutPin* pin, bool isInv)
{
   _list.push_back(pin);
   _isPosList.push_back(isInv);
}

void
PinAry::chaState(bool state)
{
   _isPosList[_isPosList.size()-1] = state;
}

void
PinAry::pop_back()
{
   _list.pop_back();
   _isPosList.pop_back();
}

CktOutPin* 
PinAry::synAnd2Gate()
{
   if (_list.size() == 0) {
      Msg(MSG_ERR) << "NO load signal" << endl;
      return 0;
   }
   else if (_list.size() == 1) {
      if (_isPosList[0])
         return _list[0];
      else
         return synInv(_list[0]);
   }
   else  //_list >= 2
      return synAnd2();
}

CktOutPin*
PinAry::synAnd2()
{
   CktOutPin *andFin0, *andFin1, *andOut;
   CktInPin *inpin0, *inpin1;

   if (_isPosList[0]) andFin0 = _list[0];      
   else               andFin0 = synInv(_list[0]);
      
   if (_isPosList[1]) andFin1 = _list[1];      
   else               andFin1 = synInv(_list[1]);

   andOut = SYN->outPinMap.getSharPin(CKT_AND_CELL, andFin0, andFin1);

   if (andOut == 0) {
      SYN->insertCktCell(new CktAndCell(true, inpin0, inpin1, andOut));
      andFin0->connInPin(inpin0); inpin0->connOutPin(andFin0);
      andFin1->connInPin(inpin1); inpin1->connOutPin(andFin1);
      unsigned width = VLDesign.getBus(andFin0->getBusId())->getWidth();
      int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
      andOut->setBusId(busId);
      SYN->outPinMap.insert(CKT_AND_CELL, andFin0, andFin1, andOut);
   }

   for (unsigned i = 2; i < _list.size(); ++i) {
      andFin0 = andOut;
      if (_isPosList[i])
         andFin1 = _list[i];
      else 
         andFin1 = synInv(_list[i]);

      andOut = SYN->outPinMap.getSharPin(CKT_AND_CELL, andFin0, andFin1);
      if (andOut == 0) {
         SYN->insertCktCell(new CktAndCell(true, inpin0, inpin1, andOut));
         andFin0->connInPin(inpin0);  inpin0->connOutPin(andFin0);
         andFin1->connInPin(inpin1);  inpin1->connOutPin(andFin1);

         andOut->setBusId(andFin0->getBusId());
         SYN->outPinMap.insert(CKT_AND_CELL, andFin0, andFin1, andOut);
      }
   }
   return andOut;
}

CktOutPin* 
PinAry::synAndGate()
{
   if (_list.size() == 0) {
      Msg(MSG_ERR) << "NO load signal" << endl;
      return 0;
   }
   else if (_list.size() == 1) {
      if (_isPosList[0])
         return _list[0];
      else
         return synInv(_list[0]);
   }
   else  //_list >= 2
      return synAnd();
}

CktOutPin*
PinAry::synAnd()
{
   CktOutPin* andFin = NULL;
   CktOutPin* andOut = NULL;
   CktInPin* inPin = NULL;
   CktCell* andCell = new CktAndCell();
   SYN->insertCktCell(andCell);

   for (unsigned i = 0; i < _list.size(); ++i) {
      if (_isPosList[i]) 
         andFin = _list[i];      
      else 
         andFin = synInv(_list[i]);
      
      inPin = new CktInPin();
      andCell->connInPin(inPin); inPin->connCell(andCell);
      andFin->connInPin(inPin);  inPin->connOutPin(andFin);
   }
   andOut = new CktOutPin();
   andOut->connCell(andCell); andCell->connOutPin(andOut);
   unsigned width = VLDesign.getBus(andFin->getBusId())->getWidth();
   int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
   andOut->setBusId(busId);

   return andOut;
}

CktOutPin*
PinAry::synInv(CktOutPin* oInvIn)
{
   CktOutPin* invOut = SYN->outPinMap.getSharPin(CKT_INV_CELL, oInvIn, 0);
   if (invOut == 0) {
      CktInPin* invIn;
      SYN->insertCktCell(new CktInvCell(true, invIn, invOut));
      oInvIn->connInPin(invIn); invIn->connOutPin(oInvIn); 
      invOut->setBusId(oInvIn->getBusId());
      SYN->outPinMap.insert(CKT_INV_CELL, oInvIn, 0, invOut);               
   }
   return invOut;
}

CktOutPin* 
PinAry::synOr2Gate()
{
   if (_list.size() == 0) {
      Msg(MSG_ERR) << "NO load signal" << endl;
      return 0;
   }
   else if (_list.size() == 1) {
      if (_isPosList[0])
         return _list[0];
      else
         return synInv(_list[0]);
   }
   else  //_list >= 2
      return synOr2();
}

CktOutPin*
PinAry::synOr2()
{
   CktOutPin *orFin0, *orFin1, *orOut;
   CktInPin *inpin0, *inpin1;

   if (_isPosList[0]) 
      orFin0 = _list[0];      
   else 
      orFin0 = synInv(_list[0]);
      
   if (_isPosList[1]) 
      orFin1 = _list[1];      
   else 
      orFin1 = synInv(_list[1]);

   orOut = SYN->outPinMap.getSharPin(CKT_OR_CELL, orFin0, orFin1);
   if (orOut == 0) {
      SYN->insertCktCell(new CktOrCell(true, inpin0, inpin1, orOut));
      orFin0->connInPin(inpin0); inpin0->connOutPin(orFin0);
      orFin1->connInPin(inpin1); inpin1->connOutPin(orFin1);
      unsigned width = VLDesign.getBus(orFin0->getBusId())->getWidth();
      int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
      orOut->setBusId(busId);
      SYN->outPinMap.insert(CKT_OR_CELL, orFin0, orFin1, orOut);
   }

   for (unsigned i = 2; i < _list.size(); ++i) {
      orFin0 = orOut;
      if (_isPosList[i])
         orFin1 = _list[i];
      else 
         orFin1 = synInv(_list[i]);

      orOut = SYN->outPinMap.getSharPin(CKT_OR_CELL, orFin0, orFin1);
      if (orOut == 0) {
         SYN->insertCktCell(new CktOrCell(true, inpin0, inpin1, orOut));
         orFin0->connInPin(inpin0);  inpin0->connOutPin(orFin0);
         orFin1->connInPin(inpin1);  inpin1->connOutPin(orFin1);

         orOut->setBusId(orFin0->getBusId());
         SYN->outPinMap.insert(CKT_OR_CELL, orFin0, orFin1, orOut);
      }
   }
   return orOut;
}

CktOutPin* 
PinAry::synOrGate()
{
   if (_list.size() == 0) {
      Msg(MSG_ERR) << "NO load signal" << endl;
      return 0;
   }
   else if (_list.size() == 1) {
      if (_isPosList[0])
         return _list[0];
      else
         return synInv(_list[0]);
   }
   else  //_list >= 2
      return synOr();
}

CktOutPin*
PinAry::synOr()
{
   CktOutPin *orFin = NULL;
   CktOutPin* orOut = NULL;
   CktInPin* inPin = NULL;
   CktCell* orCell = new CktOrCell();
   SYN->insertCktCell(orCell);

   for (unsigned i = 0; i < _list.size(); ++i) {
      if (_isPosList[i]) 
         orFin = _list[i];      
      else 
         orFin = synInv(_list[i]);
      
      inPin = new CktInPin();
      orCell->connInPin(inPin); inPin->connCell(orCell);
      orFin->connInPin(inPin);  inPin->connOutPin(orFin);
   }
   orOut = new CktOutPin();
   orOut->connCell(orCell); orCell->connOutPin(orOut);
   unsigned width = VLDesign.getBus(orFin->getBusId())->getWidth();
   int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
   orOut->setBusId(busId);

   return orOut;
}

/*CktOutPin*
PinAry::synLatch(CktOutPin* oPinRhs)
{
   if (size() != 0) { //should put a latch
      CktInPin *iPinLatch, *iLoad, *invInpin;
      CktOutPin *oPinLatch, *invOutpin;
      SYN->insertCktCell(new CktDlatCell(true, iPinLatch, iLoad, oPinLatch));
      //connect between data and latch
      oPinRhs->connInPin(iPinLatch); iPinLatch->connOutPin(oPinRhs);
      //oLoad is the collection of null branch, we have to put a invert in front to translate into load signal
      CktOutPin* oLoad = synOr2Gate();
      invOutpin = SYN->outPinMap.getSharPin(CKT_INV_CELL, oLoad, 0);
      if (invOutpin == 0) {
         SYN->insertCktCell(new CktInvCell(true, invInpin, invOutpin));
         oLoad->connInPin(invInpin); invInpin->connOutPin(oLoad);

         int tmpBusIdLat = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
         invOutpin->setBusId(tmpBusIdLat);
         SYN->outPinMap.insert(CKT_INV_CELL, oLoad, 0, invOutpin);
      }
      //connect between load and latch
      invOutpin->connInPin(iLoad); iLoad->connOutPin(invOutpin);
      return oPinLatch;
   }
   else// no generate latch
      return oPinRhs;
}*/

CktOutPin*
PinAry::synMergeCell(bool isRev)
{
   CktCell* cell = new CktMergeCell();
   CktOutPin* mergeOut = new CktOutPin();
   SYN->insertCktCell(cell);
   cell->connOutPin(mergeOut); mergeOut->connCell(cell);

   unsigned width = 0;
   CktInPin* inPin;
   if (isRev) {
      vector<CktOutPin*>::reverse_iterator it;
      for (it = _list.rbegin(); it != _list.rend(); ++it) {
         width += VLDesign.getBus((*it)->getBusId())->getWidth();
         inPin = new CktInPin();
         inPin->connCell(cell); cell->connInPin(inPin);
         inPin->connOutPin(*it); (*it)->connInPin(inPin);
      }
   }
   else {
      vector<CktOutPin*>::iterator it;
      for (it = _list.begin(); it != _list.end(); ++it) {
         width += VLDesign.getBus((*it)->getBusId())->getWidth();
         inPin = new CktInPin();
         inPin->connCell(cell); cell->connInPin(inPin);
         inPin->connOutPin(*it); (*it)->connInPin(inPin);
      }
   }
   int busId = VLDesign.genBusId(width, 0, width-1, isRev);
   mergeOut->setBusId(busId);
   return mergeOut;
}

void 
PinAry::clear() 
{
   _list.clear();
   _isPosList.clear();
}

unsigned 
PinAry::size() const
{
   return _list.size();
}

CktOutPin* 
PinAry::operator[](const unsigned& i)
{
   if (i >= _list.size()) {
      Msg(MSG_ERR) << "Error index of SynVarList " << endl;
      return 0;
   }
   return _list[i];
}
   
bool 
PinAry::isExist(CktOutPin* oPin) const
{
   for (vector<CktOutPin*>::const_iterator it = _list.begin(); it != _list.end(); ++it)
      if ((*it) == oPin)
         return true;

   return false;
}

// ==========  class  SynVar   ========== //
SynVar::SynVar()
{
   _busId        = 0;
   //_index        = 0;
   _isFuncOrTask = false;
   _isSeq        = false;
   _isSync       = false;
   _isParallCase = false;
   _clk          = 0;
   _next         = 0;
}

SynVar::SynVar(string& name, unsigned busId) 
{
   _name         = name; 
   _busId        = busId; 
   //_index        = 0;
   _isFuncOrTask = false;
   _isSeq        = false;
   _isSync       = false;
   _isParallCase = false;
   _next         = 0;
}
   
SynVar::~SynVar()
{
}
   
void 
SynVar::setName(const string& name)
{
   _name = name;
}
   
string 
SynVar::getName() const
{
   return _name;
}
  
void 
SynVar::setBusId(int busId) 
{
   _busId = busId;
}

int 
SynVar::getBusId() const
{
   return _busId;
}

void 
SynVar::setFuncOrTask() 
{
   _isFuncOrTask = true;
}
   
void 
SynVar::setParallCase()
{
   _isParallCase = true;
}
   
void 
SynVar::resetFuncOrTask() 
{
   _isFuncOrTask = false;
}
   
bool 
SynVar::isFuncOrTask() const
{
   return _isFuncOrTask;
}

bool
SynVar::isParallCase() const
{
   return _isParallCase;
}

bool 
SynVar::isSeq() const
{
   return _isSeq;
}
   
bool 
SynVar::isSync() const
{
   return _isSync;
}

void 
SynVar::setState(bool isSeq, bool isSync, CktOutPin* clk)
{
   _isSeq  = isSeq;
   _clk    = clk;
   _isSync = isSync;
}

void 
SynVar::setFinInfo(CktOutPin* finPin, VlpBaseNode* dfg)
{
   //(SYN->synCondStack.size() == 0) ==> no condition, that is no "if-else" or "case"
   unsigned busId = finPin->getBusId();  
   vector<finPair>::iterator it;
   for (it = _finOutPins.begin(); it != _finOutPins.end(); ++it) {
      if (it->_busId == busId) {
         if (SYN->synCondStack.size() != 0) {
            it->setCondFin(finPin, dfg);
            return;
         }
         else 
            Msg(MSG_WAR) << "Mulit-driven the same bus signal!!" << " Module Name = " 
                         << curVlpModule->getModuleName() << " SynVar Name = " << _name << endl;
      }
   }
   // new fin bus
   finPair newPair(finPin, 0, dfg);
   if (SYN->synCondStack.size() != 0)
      newPair.cha2Cond();
  
   _finOutPins.push_back(newPair);
}

CktOutPin*
SynVar::connCombFin(PinAry& mergePins, bool& isMemVar) // comb part usage
{
   CktInPin*  inPin;
   CktOutPin* varPin;
   if (!isMemVar)
      varPin = SYN->outPinMap.getSharPin(_name);
   else {
      varPin = new CktOutPin();
      varPin->setBusId(_busId);
   }

   if (mergePins.size() == 1) {
      int varWidth = VLDesign.getBus(_busId)->getWidth();
      int finWidth = VLDesign.getBus(mergePins[0]->getBusId())->getWidth();
      if (varWidth != finWidth)
         Msg(MSG_WAR) << "LHS and RHS have different bus width!!(connFinPins)" << endl;
      if (varPin->getCell() != 0) {
         assert (varPin->getCell()->getCellType() == CKT_PIO_CELL);
         varPin->connInOutPin(mergePins[0]);
      }
      else
         varPin->combOutPin(mergePins[0]);
   }
   else { // need mergeCell or BusCell
      bool isRev = VLDesign.getBus(_busId)->isInverted();
      CktCell* cell;
      if (isBusOverLap()) // ==> isMultiDriven
         cell = new CktBusCell();
      else                 
         cell = new CktMergeCell();
      SYN->insertCktCell(cell);
      cell->connOutPin(varPin); varPin->connCell(cell);

      if (isRev) {
         for (int i = mergePins.size()-1; i >= 0; --i) { // need use sign "int i"
            inPin = new CktInPin();
            inPin->connCell(cell);           cell->connInPin(inPin);
            inPin->connOutPin(mergePins[i]); mergePins[i]->connInPin(inPin);
         }
      }
      else {
         for (unsigned i = 0; i < mergePins.size(); ++i) {
            inPin = new CktInPin();
            inPin->connCell(cell);           cell->connInPin(inPin);
            inPin->connOutPin(mergePins[i]); mergePins[i]->connInPin(inPin);
         }
      }
   }
   return varPin;
}
      
CktOutPin*
SynVar::connSeqFin(bool& isMemVar)
{
   CktOutPin* varPin;
   if (!isMemVar)
      varPin = SYN->outPinMap.getSharPin(_name);
   else {
      varPin = new CktOutPin();
      varPin->setBusId(_busId);
   }
   assert (varPin->getCell() == 0);
   SYN->loopMuxFinAry.clear();

   CktOutPin* FFfanIn = synComb(isMemVar);
   // insert ff between them(varPin & FFfanIn)
   CktDffCell* dffCell = varPin->connFF(FFfanIn);
   CktInPin* inPin = new CktInPin();
   dffCell->connClk(inPin); inPin->connCell(dffCell);
   _clk->connInPin(inPin);  inPin->connOutPin(_clk);
   if (!_isSync)  //asynchronous reset
      connAsynRst(dffCell);

   CktOutPin* bufFin;
   unsigned muxInBusId;
   for (unsigned i = 0; i < SYN->loopMuxFinAry.size(); ++i) {
      muxInBusId = SYN->loopMuxFinAry[i]->getBusId();
      if (muxInBusId == _busId || isMemVar)
         bufFin = varPin;
      else
         bufFin = SYN->outPinMap.genVarOutpinFout(_name, muxInBusId);
      inPin = new CktInPin();
      inPin->connOutPin(bufFin); bufFin->connInPin(inPin); 
      SYN->insertCktCell(new CktBufCell(false, inPin, SYN->loopMuxFinAry[i]));
   }
   return varPin;
}

//Note : asynchronous
//code style must be below
//always (posedge clk or negedge rst) begin
// if (rst)
//      ...
// else      ==> _cond->getSubCond(false)
//      ...
//end
CktOutPin*
SynVar::synComb(bool& isMemVar)
{
   PinAry mergePins;
   mergePins.clear();
   int finWidth = 0;
   int varWidth = VLDesign.getBus(_busId)->getWidth();
   for (unsigned i = 0; i < _finOutPins.size(); ++i) {
      SYN->isHasTriStateBuf = false;  // need another handle when _isSeq == true
      SYN->isHasNullBranch  = false;  // Reset isHasNullBranch
      assert ((_finOutPins[i]._finPin == 0) || (_finOutPins[i]._cond == 0));
      SYN->muxOutBusId = _finOutPins[i]._busId;
      finWidth += _finOutPins[i].synMergeFin(_isSync, _isSeq); // set new mergeFin in finPair::_finPin
   }
   for (unsigned i = 0; i < _finOutPins.size(); ++i)  // collect vaild merge fanin
      mergePins.insert(_finOutPins[i]._finPin, true);
   // latch synthesis(comb) or loop mux(seq) -> if use comb SynVar method, the step is useless

   if (finWidth != 0 && varWidth != finWidth)
      Msg(MSG_WAR) << "LHS and RHS have different bus width!! VarName = " << getName() 
           << " (varWidth, finWidth) = (" << varWidth << ", " << finWidth << ")" << endl;
   //insert busCell or mergeCell
   if (_isSeq) {
      if (mergePins.size() > 1) {
         bool isRev = VLDesign.getBus(_busId)->isInverted();
         return mergePins.synMergeCell(isRev);
      }
      else
         return mergePins[0];
   }
   else
      return connCombFin(mergePins, isMemVar); // no use   
}

/*int
SynVar::findSameCondRoot(unsigned t) const
{
   const SynBus* tBus = VLDesign.getBus(_finOutPins[t]._busId);
   const SynBus* bus;
   for (unsigned i = 0; i < t; ++i) {
      bus = VLDesign.getBus(_finOutPins[i]._busId);
      if (bus->isContain(tBus) && _finOutPins[i].isSameCondRoot(_finOutPins[t]))
         return i;

      if (!bus->isContain(tBus) && _finOutPins[i].isSameCondRoot(_finOutPins[t]))
         assert (0);
   }
   return -1;
}*/

void
SynVar::connAsynRst(CktDffCell* dffCell)
{
   CktInPin*  inPin;
   CktOutPin* rstPin   = _finOutPins[0]._cond->getCtrlCond();
   //CktOutPin* rstValue = _finOutPins[0]._cond>getFinPin(); // what if this (top default value) ==> to handle
   
   CktOutPin* rstValue;
   if (_finOutPins.size() == 1)
      rstValue = _finOutPins[0]._cond->getSubCond(true)->getFinPin(); 
   else {
      PinAry mergePins;
      for (unsigned i = 0; i < _finOutPins.size(); ++i) // collect merge fanin
         mergePins.insert(_finOutPins[i]._cond->getSubCond(true)->getFinPin(), true);

      bool isRev = VLDesign.getBus(_busId)->isInverted();
      rstValue = mergePins.synMergeCell(isRev);
   }

   inPin = new CktInPin();
   dffCell->connReset(inPin); inPin->connCell(dffCell);
   rstPin->connInPin(inPin);  inPin->connOutPin(rstPin);
   inPin = new CktInPin();
   dffCell->connDefault(inPin); inPin->connCell(dffCell);
   rstValue->connInPin(inPin);  inPin->connOutPin(rstValue);            
}

bool
SynVar::isBusOverLap() const
{
   //known : _finOutPins.size() > 1
	//assert (_finOutPins.size() > 1);
   vector<finPair>::const_iterator it = _finOutPins.begin();
   const SynBus* bus = VLDesign.getBus(it->_busId);
   int end = bus->getEnd();
   for (it = it+1; it != _finOutPins.end(); ++it) {
      bus = VLDesign.getBus(it->_busId);
      if (bus->getBegin() <= end)
         return true;
      else
         end = bus->getEnd();
   }
   return false;
}

//NOTE : compact default signals
         //In insert oPinLhs into mergeList, I will sort the variable by its bus definition and return its location
         //if in asynchronous sequetial block, we have to compact the default signals. In previous code, we know the
         //data signal location in the mergeList. I insert to the same location in the dffDefaultList
         /* eg.
               reg [7:0]
               always@(posedge clk or negedge rst)
               begin
                  if(!rst)
                  begin
                     a[5:3]=3'b000;
                     a[7:6]=2'b11;
                     a[2:0]=3'b010;
                  end
                  else
                  begin
                     a[5:3]=3'b110;
                     a[7:6]=2'b01;
                     a[2:0]=3'b010;
                  end
               end
         in mergeList, I have said that I will sort the SynVar by bus definition
                   |---------------------|                 |---------------------|
         mergeList |[7:6]   [5:3]   [2:0]|  dffDefaultList |[7:6]   [5:3]   [2:0]|
                   |--o--| |--o--| |--o--|                 |--o--| |--o--| |--o--|
                      |       |       |                       |       |       |
                    2'b11   3'b000  3'b010                  2'b01   3'b110  3'b010 */

void
SynVar::combSameCond()
{  
   vector<vector<int> > combArray;
   vector<int> iAry;
   combArray.clear();
   iAry.clear();
   unsigned totalIndex = _finOutPins.size();
   for (unsigned i = 0; i < _finOutPins.size(); ++i) {
      if (_finOutPins[i]._busId != _busId) { // finPin._busId != total SynVar busId
         unsigned j = 0;
         for (j = 0; j < combArray.size(); ++j) {
            if (_finOutPins[ combArray[j][0] ].isSameCond(_finOutPins[i])) {
               combArray[j].push_back(i);
               break;
            }
         }
         if (j == combArray.size()) { // add a vector<finPair*> and insert into combArray
            iAry.clear();
            iAry.push_back(i);
            combArray.push_back(iAry);
         }
      }
      else {
         assert (totalIndex == _finOutPins.size()); // only exist one total bus finPair
         totalIndex = i;
      }
   }

   unsigned sortFinBusId = 0;
   for (unsigned i = 0; i < combArray.size(); ++i) {
      if (combArray[i].size() > 1) {
         sortFinBusId = sortFin(combArray[i]);
         // comb .. and set _isValid false
         if ((totalIndex != _finOutPins.size()) && (sortFinBusId == _busId)) 
            _finOutPins[totalIndex].synPartSelFin(combArray[i], _finOutPins, sortFinBusId);
         else if (sortFinBusId != 0) {  // New Tree is added            
            finPair fp(NULL, copyCond(_finOutPins[ combArray[i][0] ]._cond), NULL);
            fp._busId = sortFinBusId;
            fp.synPartSelFin(combArray[i], _finOutPins, sortFinBusId);
            _finOutPins.push_back(fp);
         }
      }
   }
   // remove invalid finPair
   for (vector<finPair>::iterator it = _finOutPins.begin(); it != _finOutPins.end(); ++it) {
      if (it->_isValid == false) {
         _finOutPins.erase(it);
         --it;
      }
   }
}

// separate default
/*   s = 2'b000;
     if (c1) s[0] = 1;
     else    s[1] = 1;
*/
// separate general
/*   if (c1) s = 2'b000;
     else if (c2) s[0] = 1;
     else s[1] = 1;
*/
void
SynVar::separateCond()
{
   vector<finPair>::iterator it = _finOutPins.end();
   vector<finPair>::iterator is;
   unsigned index = 0;
   for (index = 0; index < _finOutPins.size(); ++index)
      if (_finOutPins[index]._busId == _busId)  { // finPin._busId == total SynVar busId
         it = _finOutPins.begin() + index;
         break;
      }

   if (it != _finOutPins.end()) {
      if (it->_cond == 0) { // default assign
         for (is = _finOutPins.begin(); is != _finOutPins.end(); ++is)
            if (is->_busId != _busId && is->_cond != 0)
               is->_cond->setSpDefPin(it->_finPin, is->_busId);
      }
      else { // general design
         for (is = _finOutPins.begin(); is != _finOutPins.end(); ++is)
            if (is->_busId != _busId && is->_cond != 0) 
               is->_cond->RecursiveSetSpDefPin(it->_cond, is->_busId);
         checkFullSp(it->_cond);
      }
      _finOutPins.erase(_finOutPins.begin() + index);
   }
}

/*               |--------|
      [3:1]  ----|1       |
                 | Merge  |----- [3:0]
        [0]  ----|2       |
                 |--------|
   the inPin order is from the msb to lsb of the outPin
*/
unsigned
SynVar::sortFin(vector<int>& iAry)
{
   unsigned a, b, c;
   int temp;
   bool isInv = VLDesign.getBus(_busId)->isInverted();
   // bubble sort
   for (unsigned i = 0; i < iAry.size(); ++i) {
      a = VLDesign.getBus(_finOutPins[iAry[i]]._busId)->getEnd();
      c = 0;
      for (unsigned j = i+1; j < iAry.size(); ++j) {
         b = VLDesign.getBus(_finOutPins[iAry[j]]._busId)->getEnd();           
         if (!isInv && b < a)
            c = j;
         if ( isInv && b > a)
            c = j;
      }
      if (c != 0) { // swap
         temp    = iAry[i];
         iAry[i] = iAry[c];
         iAry[c] = temp;
      }
   }
   // check no support condition
   // total bus [7:0], fins' bus [5:3] , [1:0] => loss [2] and [7:6]
   //int tB  = VLDesign.getBus(_busId)->getBegin();
   //int tE  = VLDesign.getBus(_busId)->getEnd();
   int bgn, end;
   if (isInv) {
      bgn = VLDesign.getBus(_finOutPins[iAry[iAry.size()-1]]._busId)->getBegin();
      end = VLDesign.getBus(_finOutPins[iAry[0]]._busId)->getEnd();
   }
   else {
      bgn = VLDesign.getBus(_finOutPins[iAry[0]]._busId)->getBegin();
      end = VLDesign.getBus(_finOutPins[iAry[iAry.size()-1]]._busId)->getEnd();
   }

   unsigned ret = VLDesign.genBusId((end-bgn+1), bgn, end, busDirectionDefault);
   if (isInv) {
      for (unsigned i = 1; i < iAry.size(); ++i) {
         bgn = VLDesign.getBus(_finOutPins[iAry[i-1]]._busId)->getBegin();
         end = VLDesign.getBus(_finOutPins[iAry[i]]._busId)->getEnd();
         if (!(bgn == end + 1)) return 0;
      }
   }
   else {
      for (unsigned i = 1; i < iAry.size(); ++i) {
         bgn = VLDesign.getBus(_finOutPins[iAry[i]]._busId)->getBegin();
         end = VLDesign.getBus(_finOutPins[iAry[i-1]]._busId)->getEnd();
         if (!(bgn == end + 1)) return 0;
      }
   }
   return ret;
}

void
SynVar::checkFullSp(SynCond* syncond)
{
   unsigned tB  = VLDesign.getBus(_busId)->getBegin();
   unsigned tE  = VLDesign.getBus(_busId)->getEnd();
   int _tB = (tE > tB) ? tB : tE;
   unsigned bus_size = (tE > tB) ? (tE - tB + 1) : (tB - tE + 1);
   bool busArr[bus_size];
   
   // Initial to FALSE
   for (unsigned i = 0; i < bus_size; ++i) 
      busArr[i] = 0;
   
   for (vector<finPair>::iterator is = _finOutPins.begin(); is != _finOutPins.end(); ++is) {
      if (is->_busId != _busId && is->_cond != 0) {
         tB = VLDesign.getBus(is->_busId)->getBegin() - _tB;
         tE = VLDesign.getBus(is->_busId)->getEnd() - _tB;
         if (tE > tB) 
            for (unsigned j = tB; j <= tE; ++j) busArr[j] = 1;
         else 
            for (unsigned j = tE; j <= tB; ++j) busArr[j] = 1;
      }
   }

   // Check if there is a bus without being set
   for (unsigned i = 0; i < bus_size; ++i) {
      if (!busArr[i]) {  // New a tree for this bus
         tB = i;
         while (i < bus_size) { if (busArr[i]) break; ++i; }
         if (i < bus_size) tE = i - 1;
         else              tE = bus_size - 1;
         finPair fp(NULL, copyCond(syncond), NULL);
         fp._busId = VLDesign.genBusId((tE-tB+1), tB, tE, busDirectionDefault);
         fp._isValid = true;
         fp._cond->RecursiveSetSpDefPin(syncond, fp._busId);
         _finOutPins.push_back(fp);
      }
   }
}

/* in sequential block, if there exists null pin, it will use previous value
   eg. always@(posedge clk)
       begin
          if(cs)
             a= 1;
       end
   In the upper example, the false child of "a" is not identified. In combinational
   blocks, we will generate a latch.In sequential blocks, we will use previous value
   of "a". Illustrated by the following ascii.

   In combinational blocks     |-------|  a   In sequential blocks  <----------------------------|
   always@()                1--| latch |--o   always@(posedge clk)  |     |---\                  |
   begin                       |       |      begin                 |  1--|T   \  |-------|  a   |
      if(cs)               cs--|load   |         if(cs)             |     | mux|--|  DFF  |--o-->|
         a= 1;                 |-------|            a= 1;           |-->--|F   /  |       |
   end                                          end                       |---/   |ck     |
                                                                            |     |-------|
                                                                           cs                     */
//NOTE : synthesis combinaton logic
      /* 1. if-part
            combinational block or synchronous sequetial block are treated the same.
            The left part is the same.
            eg.
            combinational circuit           synchronous sequetial circuit
                               mux                                     mux
            always @(sel)     |---\         always @(posedge clk)     |---\   |-----|
            begin          1--|1   \        begin                  1--|1   \  |     |
               if(sel)        |    |---a       if(sel)                |    |--| DFF |--a
                  a=1;     0--|0   /              a=1;             0--|0   /  |     |
               else           |---/            else                   |---/   |-----|
                  a=0;          |                 a=0;                  |
            end                sel          end                        sel
         2. else-part
            always @(posedge clk or negedge rst)
               begin
                  if(!rst)
                     out<= 0;
                  else
                     out<= a;
               end

               put true child into "default value", connect !rst to reset pin
               put false child into input of DFF
                      |-----|
                  a---| DFF |--> out
                      |     |
                clk---|>    |
                      |-----|
                         |
                  !rst --|            */
CktOutPin*
SynVar::connLhsRhs(bool isMemVar)
{
   if (_finOutPins.size() > 1)
      combSameCond(); // combination : will reduce _finOutPins.sise()
   
   if (_finOutPins.size() > 1)
      separateCond(); // separation  : will reduce _finOutPins.size()

   if (_finOutPins.size() > 1)
      sort (_finOutPins.begin(), _finOutPins.end(), SynVar::my_comp);
  
   if (_isSeq) { //data signal is at oPinRhs
      if (isBusOverLap()) {
         Msg(MSG_WAR) << "BusOverLap !! finPin size = " << _finOutPins.size() << endl;
         assert(0); // not finish
         return 0;
      }
      else
         return connSeqFin(isMemVar);
   }
   else 
      return synComb(isMemVar); // it will handle bus overlap within the function.
}

SynCond*
SynVar::copyCond(SynCond* O_Cond)
{
   SynCond* syncond = new SynCond();
   SynCond* T_Cond = 0;
   SynCond* F_Cond = 0;
   
   if (O_Cond) {
      T_Cond = O_Cond->getSubCond(true);
      F_Cond = O_Cond->getSubCond(false);
      if (T_Cond)
         syncond->setSubCond(true,  copyCond(T_Cond));
      if (F_Cond)
         syncond->setSubCond(false, copyCond(F_Cond));
      
      syncond->setCtrlCond(O_Cond->getCtrlCond());
      syncond->setCtrlNode(O_Cond->getCtrlNode());
      syncond->setSeNode(O_Cond->getSeNode());
      return syncond;
   }
   else return 0;
}

void
SynVar::print() const
{
   cout << "SynVar ==> " << "Signal Name = " << getName() << endl;
   for (unsigned x = 0; x < _finOutPins.size(); ++x) _finOutPins[x].print();

}

bool
SynVar::writeCDFG(const string fileName, set<unsigned>& bits) const {
   vector<const finPair*> finPairList;
   unsigned latestBegin = (unsigned)-1;
   unsigned curBegin, curEnd;
   unsigned i = 0;
   if (bits.empty()) {
      for (; i < _finOutPins.size(); ++i) finPairList.push_back(&_finOutPins[i]);
   }
   else {
      for (set<unsigned>::const_iterator it = bits.begin(); it != bits.end(); ++it) {
         for (; i < _finOutPins.size(); ++i) {
            curBegin = VLDesign.getBus(_finOutPins[i]._busId)->getBegin();
            curEnd = VLDesign.getBus(_finOutPins[i]._busId)->getEnd();
            if (((*it) >= curBegin) && ((*it) <= curEnd)) {
               if (curBegin != latestBegin) {
                  latestBegin = curBegin;
                  finPairList.push_back(&_finOutPins[i]);
               }
               break;
            }
         }
         if (i == _finOutPins.size()) {
            Msg(MSG_ERR) << "Bit " << (*it) << " is NOT in the bit scope of " << getName() << endl;
            return false;
         }
      }
   }

   ofstream DOTFile;
   DOTFile.open(fileName.c_str());
   DOTFile << "digraph G {" << endl;

   CktOutPin* outPin = 0;
   for (i = 0; i < finPairList.size(); ++i) {
      DOTFile << TAB_INDENT << "subgraph cluster" << i << " {" << endl;
      if (finPairList[i]->_cond) {
         // recursive
         DOTFile << TAB_INDENT << TAB_INDENT << "root_" << i << " [shape = circle, label = " << getName() << "]" << endl;
         outPin = finPairList[i]->_cond->writeCDFG(DOTFile, i);
         DOTFile << TAB_INDENT << TAB_INDENT << "root_" << i << " -> " << "\"" << outPin << i << "\" [dir = back] " << endl;
      }
      else {
         // assign x = a;
         DOTFile << TAB_INDENT << TAB_INDENT << "root_" << i << " [shape = circle, label = " << getName() << "]" << endl;
         outPin = finPairList[i]->_finPin;
         DOTFile << TAB_INDENT << TAB_INDENT << "\"" << outPin << i << "\"" << " [shape = box, label = " << outPin->getName() << "]" << endl;
         DOTFile << TAB_INDENT << TAB_INDENT << "root_" << i << " -> " << "\"" << outPin << i << "\" [dir = back] " << endl;
      }
      curBegin = VLDesign.getBus(finPairList[i]->_busId)->getBegin();
      curEnd = VLDesign.getBus(finPairList[i]->_busId)->getEnd();
      DOTFile << TAB_INDENT << TAB_INDENT << "label = \"" << getName() << " [";
      if (curBegin == curEnd) DOTFile << curBegin;
      else DOTFile << " " << curEnd << " : " << curBegin << " ";
      DOTFile << "]\"" << endl;
      DOTFile << TAB_INDENT << "}" << endl;
   }
   
   DOTFile << "}" << endl;
   DOTFile.close();
   return true;
}

finPair::finPair(CktOutPin* p, SynCond* c, VlpBaseNode* n = 0) 
{ 
   _busId   = (p) ? p->getBusId() : 0; 
   _finPin  = (p) ? p : 0; 
   _cond    = c;   //null: no conditional assignment, otherwise has "if-else" or "case" condition
   _isValid = true;
   _seNode  = n;
}
      
finPair::finPair(const finPair& p) 
{ 
   _busId   = p._busId; 
   _finPin  = p._finPin;
   _cond    = p._cond;
   _isValid = p._isValid; 
   _seNode  = p._seNode;;
}

finPair::~finPair()
{
}

void
finPair::cha2Cond()
{
   assert (_cond == 0);

   SynCond* tmpCond;
   vector<SynCondState*>::reverse_iterator it;
   _cond = new SynCond(_finPin, _seNode);
   for (it = SYN->synCondStack.rbegin(); it != SYN->synCondStack.rend(); ++it) {
      //assert ((*it)->getState() == true);
      tmpCond = _cond;
      _cond = new SynCond();
      _cond->setCtrlCond((*it)->getCtrlCond());
      _cond->setCtrlNode((*it)->getCtrlNode());
      _cond->setSubCond((*it)->getState(), tmpCond);
   }
   _finPin = 0;
   _seNode = 0;
}

//1. according to the state of synCondList to get the tree-node that store the condition
//2. parallel cond :
//
//   if (c1)                            |    if (c1)        \    if (c2)
//       A = 1;     \       if (c2)     |       A = 1;   ----\     A = 2;
//   if (c2)     ----\         A = 2;   |    if (c2)     ----/   else              
//       A = 2;  ----/      else        |       A = 2;      /      A = 1;
//   else           /          A = 3;   |                      
//       A = 3;                         |          
//
//   case
//       ...
//   case           is similar as "parallel if"
//       ...
void
finPair::setCondFin(CktOutPin* finPin, VlpBaseNode* dfg)
{
   //Note : SYN->synCondStack.size() != 0
   if (_cond == 0) { 
      assert (_finPin != 0);
      _cond = new SynCond();
   }
   SynCond* curCond = _cond;//root
   SynCond* parCond = 0; 
   SynCond* tmpCond = 0;
   vector<SynCondState*>::const_iterator it;
   unsigned level = 0;
   for (it = SYN->synCondStack.begin(); it != SYN->synCondStack.end(); ++it, ++level) {
      if ((curCond->getCtrlCond() != 0) && (curCond->getCtrlCond() != (*it)->getCtrlCond())) {
         tmpCond = new SynCond(curCond->getFinPin(), const_cast<VlpBaseNode*>(curCond->getSeNode()));
         curCond->setFinPin(0);
         tmpCond->setCtrlCond((*it)->getCtrlCond());
         tmpCond->setCtrlNode((*it)->getCtrlNode());
         tmpCond->setSubCond(false, curCond);  // parallel cond

         if (parCond == 0) {
            _cond = tmpCond;
            curCond = _cond;
         }
         else {
            parCond->setSubCond((*it)->getState(), tmpCond);
            curCond = tmpCond;
         }        
      }
      else
         if (curCond->getCtrlCond() == 0) {
            curCond->setCtrlCond((*it)->getCtrlCond());
            curCond->setCtrlNode((*it)->getCtrlNode());
         }
               
      parCond = curCond;
      curCond = curCond->getSubCond((*it)->getState());
      if (curCond == 0) {
         tmpCond = new SynCond();
         parCond->setSubCond((*it)->getState(), tmpCond);
         curCond = tmpCond;
      }
   }
   // move finPair::_finPin to topDefault
   if (_finPin != NULL) {
      _cond->setFinPin(_finPin);
      _cond->setSeNode(_seNode);
      _finPin = 0;
      _seNode = 0;
   }
   curCond->setFinPin(finPin);
   curCond->setSeNode(dfg);
}

CktOutPin*
finPair::synLatch(CktOutPin* latchIn) const
{
   CktInPin *iPin, *iLoad;
   CktOutPin *oPin, *loadIn;
   CktDlatCell* latch = new CktDlatCell(true, iPin, iLoad, oPin);
   SYN->insertCktCell(latch);
   loadIn = synLatchCtrl();
   iPin->connOutPin(latchIn); latchIn->connInPin(iPin);
   iLoad->connOutPin(loadIn); loadIn->connInPin(iLoad);

   oPin->setBusId(latchIn->getBusId());
   return oPin;
}

CktOutPin*
finPair::synLatchCtrl() const
{
   PinAry orGate1;
   PinAry andGate2;
   _cond->synNullCond(orGate1, andGate2, 0);
   return orGate1.synOr2Gate()->genInvCell();
}

/* combinational circuit
                        mux
always @(sel)          |---\                       bufif1
begin             1----|1   \            \         |------|  a
   if(sel)             |    |---a   ------\    1---|D    Q|--o
      a=1;        z----|0   /       ------/  sel---|en    |
   else                |---/             /         |------|
      a=1'bz;            |
end                     sel

synchronous sequetial circuit
                         mux                                mux
always @(posedge clk)   |---\    |-----|                   |---\    |-----|     bufif1
begin              1----|1   \   |     |          \   1----|1   \   |     |    |------|  a
   if(sel)              |    |---| DFF |--- a  ----\       |    |---| DFF |----|D    Q|--o
      a=1;         z----|0   /   |     |       ----/  x----|0   /   |     |  |-|en    |
   else                 |---/    |-----|          /        |---/    |-----|  | |------|
      a=1'bz;             |                                  |               |
end                      sel                                sel             sel           */
//[jojoman comment]high impedence branchs in sequetial blocks still have bug.
CktOutPin*
finPair::synTriStateBuf(CktOutPin* tsBufIn) const
{
   CktInPin *iPin, *iLoad;
   CktOutPin *oPin, *loadIn;
   CktBufifCell* tsBuf = new CktBufifCell(true, iPin, iLoad, oPin);
   SYN->insertCktCell(tsBuf);
   loadIn = synTsBufCtrl();
   iPin->connOutPin(tsBufIn); tsBufIn->connInPin(iPin);
   iLoad->connOutPin(loadIn); loadIn->connInPin(iLoad);

   oPin->setBusId(tsBufIn->getBusId());
   return oPin;
}

CktOutPin*
finPair::synTsBufCtrl() const
{
   PinAry orGate1;
   PinAry andGate2;
   _cond->synZCond(orGate1, andGate2, 0);
   return orGate1.synOr2Gate()->genInvCell();
}

bool
finPair::isSameCond(const finPair& fp) const
{
   if (_cond == 0 && fp._cond == 0)
      return true;
   else if (_cond != 0 && fp._cond != 0)
      return (*(_cond) == *(fp._cond));
   else
      return false;
}

bool
finPair::isSameCondRoot(const finPair& fp) const
{
   if (_cond == 0 && fp._cond == 0)
      return true;
   else if (_cond != 0 && fp._cond != 0)
      if (_cond->getCtrlCond() == fp._cond->getCtrlCond())
         return true;

   return false;
}

int
finPair::synMergeFin(bool isSync, bool isSeq)
{
   CktOutPin* mergeFin;
   if (isSync || !isSeq) { // (seq and sync) or (comb)
      if (_finPin != 0) {
         SYN->outPinMap.reNewPin(_finPin);
         mergeFin = _finPin;
      }
      else mergeFin = _cond->synthesis(0, isSeq);
   }
   else { // asynchronous
      assert (_cond != 0);
      mergeFin = _cond->getSubCond(false)->synthesis(0, isSeq);         
   }
   if (SYN->isHasNullBranch && !isSeq) // only comb block, latchs will be synthesized
      mergeFin = synLatch(mergeFin); // insert latch
   // insert comb triStateBuf, but seq triStateBuf connect after FF
   if (SYN->isHasTriStateBuf && !isSeq)
      mergeFin = synTriStateBuf(mergeFin);

   _finPin = mergeFin; // set new mergeFin in _finPin
   return VLDesign.getBus(_busId)->getWidth();
}

/*void
finPair::synMuxFin(finPair& newFPair)
{
   if (newFPair._cond == 0) {  //newFPair._finPin != 0
      CktOutPin* busOut = 0;
      CktInPin*  busIn  = 0;
      CktBusCell busCell = new CktBusCell(true, busIn, busOut);
      busIn->connOutPin(_finPin); _finPin->connInPin(busIn);
      busIn = new CktInPin();
      busIn->connCell(busCell); busCell->connInPin(busIn);
      busIn->connOutPin(newFPair._finPin); newFPair._finPin->connInPin(busIn);
      _finPin = busOut;
   }
   else
      newFPair._cond->synPartSelectFin(_finPin);
}*/

void
finPair::synPartSelFin(vector<int>& iAry, vector<finPair>& finOutPins, unsigned& totalBusId)
{
   CktInPin* in;
   if (finOutPins[iAry[0]]._cond == 0) {
      if (_cond == 0) {
         CktOutPin* cellOut;
         CktCell* cell;
         if (_finPin != NULL) {
            CktBusCell* bCell = new CktBusCell(true, in, cellOut);
            in->connOutPin(_finPin); _finPin->connInPin(in);
            cellOut->setBusId(_finPin->getBusId());
            cell = bCell;
         }
         else {
            CktMergeCell* mCell = new CktMergeCell(true, cellOut);
            cellOut->setBusId(totalBusId);
            cell = mCell;
         }
         SYN->insertCktCell(cell);
         for (unsigned i = 0; i < iAry.size(); ++i) {
            in = new CktInPin();
            in->connCell(cell); cell->connInPin(in);
            in->connOutPin(finOutPins[iAry[i]]._finPin); 
            finOutPins[iAry[i]]._finPin->connInPin(in);
         }
         _finPin = cellOut;
      }
      else
         _cond->addFin(iAry, finOutPins);
   }
   else { // recurrence
      vector<SynCond*> condAry;
      for (unsigned i = 0; i < iAry.size(); ++i) 
         condAry.push_back(finOutPins[iAry[i]]._cond);
      if (_cond == 0) 
         _cond = new SynCond();
      _cond->mergeCondTree(condAry);
   }
   for (unsigned i = 0; i < iAry.size(); ++i) 
      finOutPins[iAry[i]]._isValid = false;
}

bool
finPair::operator<(const finPair& f) const
{
   const SynBus* a = VLDesign.getBus(_busId);
   const SynBus* b = VLDesign.getBus(f._busId);
   return ( a->getBegin() < b->getBegin() );
}

void
finPair::print() const
{
   cout << "finPair ==> " << "_bus = [" << VLDesign.getBus(_busId)->getBegin() << " : " 
        << VLDesign.getBus(_busId)->getEnd() << "]" << "  ";
   cout << "_finPin = " << _finPin << "  ";
   cout << "_cond = " << _cond << endl;
   if (_cond != NULL)
      _cond->print(3);
}

// ==========  class  SynMemory   ========== //
SynMemory::SynMemory(VlpMemAry* vlpmem)
{
   _isReadVariable  = false;
   _isReadConst     = false;
   _isWriteVariable = false;
   _isWriteConst    = false;
   _isSync          = false; 
   _name  = vlpmem->getName();      
   _busId = vlpmem->getBusId();
   vlpmem->getIndex(_szS, _szE); //set mem size start and end
   _clk = _dout = _wrAddress = _rdAddress = NULL;
   _dinVar = NULL;
   _varMap.clear();
   _pinMap.clear();
}

SynMemory::~SynMemory()
{
}

string
SynMemory::getName() const
{
   return _name;
}

CktOutPin*
SynMemory::genMemOutPin(float& index) 
{
   if (index == 0.5) { // variable index
      if (!_dout) {
         _isReadVariable = true;
         _dout = new CktOutPin();
         _dout->setName(_name);
         _dout->setBusId(_busId);
      }
      return _dout;
   }
   else { // const index
      int row = (int)index;
      int start, end;
      VlpMemAry* vlpmem = curVlpModule->getMemAry(_name);
      vlpmem->getIndex(start, end); // end >= start
      assert (row >= start && row <= end);
      _isReadConst = true;

      map<int, CktOutPin*>::const_iterator it = _pinMap.find(row);
      if (it == _pinMap.end()) {
         CktOutPin* outPin = new CktOutPin();
         outPin->setBusId(_busId);
         _pinMap.insert( pair<int, CktOutPin*>(row, outPin) );
         return outPin;
      }
      else
         return it->second;
   }
}

SynVar*
SynMemory::genDinVar() // for variable index
{
   if (!_dinVar) {
      _isWriteVariable = true;
      _dinVar = new SynVar();
      _dinVar->setBusId(_busId);
   }
   return _dinVar;
}

SynVar*
SynMemory::genMemVar(int row) // for const index
{
   _isWriteConst = true;
   map<int, SynVar*>::const_iterator it = _varMap.find(row);
   if (it == _varMap.end()) { // Add synVar
      SynVar* newVar = new SynVar();
      newVar->setBusId(_busId);
      _varMap.insert( pair<int, SynVar*>(row, newVar) );
      return newVar;
   }
   else return it->second;
}
   
void 
SynMemory::synWrAddress(const VlpBaseNode* address)
{
   if (_wrAddress == NULL)
      _wrAddress = address->synthesis();
   else
      Msg(MSG_WAR) << "Manually check if the variable index of memory is consistent with others" << endl;
}

void 
SynMemory::synRdAddress(const VlpBaseNode* address)
{
   if (_rdAddress == NULL)
      _rdAddress = address->synthesis();
   else
      Msg(MSG_WAR) << "Manually check if the variable index of memory is consistent with others" << endl;
}

void
SynMemory::setState(bool isSync, CktOutPin* oClk)
{
   if (_clk != NULL) {
      _isSync = isSync;
      _clk = oClk;
   }
}

void
SynMemory::connMemInOut()
{
   CktMemCell* mCell = new CktMemCell(_name, _clk, _wrAddress, _rdAddress);
   mCell->setSize(_busId, _szS, _szE);
   mCell->setRdInfo(_isReadVariable, _isReadConst);
   mCell->setWrInfo(_isWriteVariable, _isWriteConst);
   mCell->setSync(_isSync);
   if (_dout != NULL) {
      mCell->connOutPin(_dout); //
      _dout->connCell(mCell);
   }
   for (map<int, CktOutPin*>::iterator it = _pinMap.begin(); it != _pinMap.end(); ++it) {
      mCell->connRowOutPin(it->first, it->second);
      it->second->connCell(mCell);
   }
   _pinMap.clear();
   
   CktOutPin* oDin;
   CktInPin*  iDin;
   string name;
   if (_dinVar != NULL) {
      oDin = _dinVar->connLhsRhs(true);
      name = _name + "$DIN";
      oDin->setName(name);
      iDin = new CktInPin();
      iDin->connOutPin(oDin); oDin->connInPin(iDin);
      iDin->connCell(mCell); mCell->connInPin(iDin);
   }

   for (map<int, SynVar*>::iterator it = _varMap.begin(); it != _varMap.end(); ++it) {
      oDin = it->second->connLhsRhs(true);
      name = _name + "$iRow_" + toString(it->first);
      oDin->setName(name);
      iDin = new CktInPin();
      iDin->connOutPin(oDin); oDin->connInPin(iDin);
      iDin->connCell(mCell); mCell->connRowInPin(it->first, iDin);
   }
   _varMap.clear();
   SYN->insertCktCell(mCell);
}

// ==========  class  SynVarList   ========== //
SynVarList::SynVarList()
{
   clear();
}
      
SynVarList::~SynVarList()
{
   clear();
}

unsigned 
SynVarList::size() const
{
   return _list.size();
}
      
void 
SynVarList::clear() 
{
   _list.clear();
}
   
SynVar* 
SynVarList::operator[](const unsigned& i) const
{
   if (i >= _list.size()) {
      Msg(MSG_ERR) << "Error index of SynVarList " << endl;
      return 0;
   }
   return _list[i];
}

SynVar* 
SynVarList::operator[](const string& str) const
{
   for (vector<SynVar*>::const_iterator it = _list.begin(); it != _list.end(); ++it)
      if ((*it)->getName() == str)
         return (*it);
   return 0;
}

bool 
SynVarList::insertVar(string& varName, unsigned varBusId)
{
   vector<SynVar*>::const_iterator it;
   for (it = _list.begin(); it != _list.end(); ++it)
      if ((*it)->getName() == varName)
         return false;

   SynVar* var = new SynVar(varName, varBusId);
   _list.push_back(var);
   return true;
}

bool
SynVarList::insertSynMem(VlpMemAry* vlpmem)
{
   vector<SynMemory*>::const_iterator it;
   string varName = vlpmem->getName();
   for (it = _memlist.begin(); it != _memlist.end(); ++it)
      if ((*it)->getName() == varName)
         return false;
         
   SynMemory* mem = new SynMemory(vlpmem);
   _memlist.push_back(mem);
   return true;
}

bool
SynVarList::setVarFin(string& varName, CktOutPin* fin, VlpBaseNode* dfg)
{
   for (unsigned i = 0; i < _list.size(); ++i) {
      if (varName == _list[i]->getName()) {
         _list[i]->setFinInfo(fin, dfg);
         return false;
      }
   }
   return false;
}

bool
SynVarList::setMemFin(string& varName, CktOutPin* fin, VlpBaseNode* dfg, float index, const VlpBaseNode* address)
{
   vector<SynMemory*>::const_iterator it;
   for (it = _memlist.begin(); it != _memlist.end(); ++it) {
      if ((*it)->getName() == varName) {
         if (index == 0.5) {
            (*it)->genDinVar()->setFinInfo(fin, dfg);
            (*it)->synWrAddress(address);
         }
         else 
            (*it)->genMemVar((int)index)->setFinInfo(fin, dfg);
         return true;
      }
   }
   return false;
}

CktOutPin* 
SynVarList::getMemOutPin(string& name, float index, VlpBaseNode* address) const
{   
   vector<SynMemory*>::const_iterator it;
   for (it = _memlist.begin(); it != _memlist.end(); ++it)
      if ((*it)->getName() == name)
         break;

   assert (it != _memlist.end());
   if (index == 0.5)  // variable index
      (*it)->synRdAddress(address);

   return (*it)->genMemOutPin(index);
}

void 
SynVarList::setMemState(bool isSync, CktOutPin* clk)
{
   vector<SynMemory*>::iterator it;
   for (it = _memlist.begin(); it != _memlist.end(); ++it)
      (*it)->setState(isSync, clk);
}
   
SynVarList& 
SynVarList::operator =(const SynVarList& vars)
{
   _list = vars._list;
   _memlist = vars._memlist;
   return *this;
}

void
SynVarList::connLhsRhs()
{
   for (vector<SynVar*>::iterator it = _list.begin(); it != _list.end(); ++it) {
      //cout << "BaseModule::connLhsRhs => handle var name = " << (*it)->getName() << endl;
      (*it)->connLhsRhs(false);
   }
   for (vector<SynMemory*>::iterator im = _memlist.begin(); im != _memlist.end(); ++im)
      (*im)->connMemInOut(); //synthesize CktMemCell and connect its I/O
}

// ==========  class  SynCond   ========== //
SynCond::SynCond()
{
   _finPin     = 0; 
   _ctrlCond   = 0; 
   _trueCond   = 0; 
   _falseCond  = 0; 
   _ctrlNode   = 0;
   _seNode     = 0;
}

SynCond::SynCond(CktOutPin* finPin, VlpBaseNode* node = 0)
{
   _finPin     = finPin;
   _ctrlCond   = 0;
   _trueCond   = 0; 
   _falseCond  = 0; 
   _ctrlNode   = 0;
   _seNode     = node;
}

SynCond::~SynCond()
{
   _finPin = 0;   
   _ctrlCond = 0;  
   if (_trueCond != 0) {
      delete _trueCond;
      _trueCond = 0;
   }
   if (_falseCond != 0) {
      delete _falseCond;
      _falseCond = 0;
   }
}
   
void 
SynCond::setFinPin(CktOutPin* finPin)
{
   _finPin = finPin;
}
   
CktOutPin* 
SynCond::getFinPin() const 
{
   return _finPin; 
}
   
void 
SynCond::setCtrlCond(CktOutPin* ctrlCond)
{
   _ctrlCond = ctrlCond;
}

CktOutPin* 
SynCond::getCtrlCond() const 
{ 
   return _ctrlCond; 
}
   
SynCond* 
SynCond::getSubCond(bool state) const 
{ 
   return state ? _trueCond : _falseCond; 
}
   
void 
SynCond::setSubCond(bool state, SynCond* cond)
{
   if (state)
      _trueCond = cond;
   else
      _falseCond = cond;
}

SynCond& 
SynCond::operator = (const SynCond& node)
{
   _finPin     = node._finPin;
   _ctrlCond   = node._ctrlCond;
   _trueCond   = node._trueCond;
   _falseCond  = node._falseCond;
   return *this;
}
   
bool 
SynCond::operator == (const SynCond& node) const
{
   if ((_trueCond == 0) && (_falseCond == 0) 
     && node.getSubCond(true) == 0 && node.getSubCond(false) == 0) {
      return true;
   }

   if (_ctrlCond == node.getCtrlCond()) {
      if (_trueCond != 0 && node.getSubCond(true) != 0 && _falseCond != 0 && node.getSubCond(false) != 0) {
         if (*(_trueCond) == *(node.getSubCond(true)) && *(_falseCond) == *(node.getSubCond(false)))
            return true;
      }
      else if (_trueCond != 0 && node.getSubCond(true) != 0 && _falseCond == 0 && node.getSubCond(false) == 0) {
         if (*(_trueCond) == *(node.getSubCond(true)))
            return true;
      }
      else if (_trueCond == 0 && node.getSubCond(true) == 0 && _falseCond != 0 && node.getSubCond(false) != 0) {
         if (*(_falseCond) == *(node.getSubCond(false)))
            return true;
      }
   }
   return false;
}
   
void 
SynCond::print(int ctr)
{
   for (int i = 0; i < ctr; i++)
      cout << "   ";
   cout << "SynCond ==> " << "finPin: " << _finPin << endl;
   for (int i = 0; i < ctr; i++)
      cout << "   ";

   cout << "cond: " << _ctrlCond << endl;
   if (_trueCond != 0) {
      for (int i = 0; i < ctr; i++)
         cout << "   ";
      cout << "true child: " << endl; 
      _trueCond->print(ctr + 1);
   }
   if (_falseCond != 0) {
      for (int i = 0; i < ctr; i++)
         cout << "   ";
      cout << "false child: " << endl; 
      _falseCond->print(ctr + 1);
   }
}

void
SynCond::print(int ctr, SynCond* refCond)  // Chengyin debug
{
   for (int i = 0; i < ctr; ++i) cout << "   ";
   cout << "finPin: ";
   if ((refCond) && (_finPin == refCond->_finPin)) cout << "* ";
   cout << _finPin << endl;
   for (int i = 0; i < ctr; ++i) cout << "   ";

   cout << "cond: ";
   if ((refCond) && (_ctrlCond == refCond->_ctrlCond)) cout << "* ";
   cout << _ctrlCond << endl;
   
   if (_trueCond != 0) {
      for (int i = 0; i < ctr; ++i) cout << "   ";
      cout << "true child: " << endl;
      if (refCond) _trueCond->print(ctr + 1, refCond->_trueCond);
      else _trueCond->print(ctr + 1);
   }
   if (_falseCond != 0) {
      for (int i = 0; i < ctr; ++i) cout << "   ";
      cout << "false child: " << endl; 
      if (refCond) _falseCond->print(ctr + 1, refCond->_falseCond);
      else _falseCond->print(ctr + 1);
   }
}

CktOutPin*
SynCond::synthesis(CktOutPin* topDefault, bool isSeq)
{
   if (_finPin != 0)   SYN->outPinMap.reNewPin(_finPin);
   if (_ctrlCond != 0) SYN->outPinMap.reNewPin(_ctrlCond);

   CktOutPin *oF, *oT;
   if (_trueCond == 0 && _falseCond == 0) {
      assert (_ctrlCond == 0);
      return _finPin;
   }
   else if (_trueCond != 0 && _falseCond != 0) {
      if (_finPin != 0) // topDefault
         topDefault = _finPin;
      oF = _falseCond->synthesis(topDefault, isSeq);
      oT = _trueCond->synthesis(topDefault, isSeq);
      return synMux(oF, oT);
   }
   else { // if topDefault = 0  =>  synthesize latch
      if (_finPin != 0) // topDefault
         topDefault = _finPin;

      if (topDefault == 0) {
         if (isSeq) {
            if (_trueCond != 0 && _falseCond == 0) {
               oF = new CktOutPin();
               oF->setBusId(SYN->muxOutBusId);
               SYN->loopMuxFinAry.push_back(oF);
               oT = _trueCond->synthesis(topDefault, isSeq);

            }
            else {// (_trueCond == 0 && _falseCond != 0) {
               oF = _falseCond->synthesis(topDefault, isSeq);
               oT = new CktOutPin();
               oT->setBusId(SYN->muxOutBusId);
               SYN->loopMuxFinAry.push_back(oT);
            }
            return synMux(oF, oT);
         }
         else {
            SYN->isHasNullBranch = true;
            if (_trueCond != 0 && _falseCond == 0)
               return _trueCond->synthesis(topDefault, isSeq);
            else // (_trueCond == 0 && _falseCond != 0) {
               return _falseCond->synthesis(topDefault, isSeq);
         }
      }
      else {
         if (_trueCond != 0 && _falseCond == 0) {
            oF = topDefault;
            oT = _trueCond->synthesis(topDefault, isSeq);
         }
         else {
            oF = _falseCond->synthesis(topDefault, isSeq);
            oT = topDefault;
         }
         return synMux(oF, oT);
      }         
   }
}

CktOutPin*
SynCond::synMux(CktOutPin* oF, CktOutPin* oT)
{
   CktInPin *iF, *iT, *iS;
   CktOutPin* oY;
   if (oF->isConstAllZ() || oT->isConstAllZ()) {
      assert ( !(oF->isConstAllZ()  && oT->isConstAllZ()) );
      assert ( !(oF->isConstPartZ() || oT->isConstPartZ()) );

      SYN->isHasTriStateBuf = true;
      if (oT->isConstAllZ())  oY = oF;
      else                    oY = oT;
   }
   else {
      SYN->insertCktCell(new CktMuxCell(true, iF, iT, iS, oY));
      oF->connInPin(iF);  iF->connOutPin(oF);
      oT->connInPin(iT);  iT->connOutPin(oT);
      _ctrlCond->connInPin(iS); iS->connOutPin(_ctrlCond);

      unsigned oF_Width = VLDesign.getBus(oF->getBusId())->getWidth();
      unsigned oT_Width = VLDesign.getBus(oT->getBusId())->getWidth();
      assert (oF_Width == oT_Width);
      int busId = VLDesign.genBusId(oF_Width, 0, oF_Width - 1, busDirectionDefault);
      oY->setBusId(busId);
   }
   return oY;
}

void
SynCond::synNullCond(PinAry& orGate1, PinAry& andGate2, CktOutPin* topDefault) const
{
   if (_trueCond == 0 && _falseCond == 0)
      ;
   else if (_trueCond != 0 && _falseCond != 0) {
      if (_finPin != 0) // topDefault
         topDefault = _finPin;
      andGate2.insert(_ctrlCond, true);
      _trueCond->synNullCond(orGate1, andGate2, topDefault);
      andGate2.chaState(false);
      _falseCond->synNullCond(orGate1, andGate2, topDefault);
      andGate2.pop_back();
   }
   else { // if topDefault = 0  =>  synthesize latch
      if (_finPin != 0) // topDefault
         topDefault = _finPin;

      if (topDefault == 0) {
         assert (_ctrlCond != 0);
         if (_trueCond != 0 && _falseCond == 0) {
            andGate2.insert(_ctrlCond, true);
            _trueCond->synNullCond(orGate1, andGate2, topDefault);
            andGate2.chaState(false);
            orGate1.insert(andGate2.synAnd2Gate(), true);
         }
         else {
            andGate2.insert(_ctrlCond, true);
            orGate1.insert(andGate2.synAnd2Gate(), true);
            andGate2.chaState(false);
            _falseCond->synNullCond(orGate1, andGate2, topDefault);
         }
         andGate2.pop_back();
      }
      else // has topDefault => no null condition !!
         ;
   }
}

void
SynCond::synZCond(PinAry& orGate1, PinAry& andGate2, CktOutPin* topDefault) const
{
   if (_trueCond == 0 && _falseCond == 0) {
      if (_finPin->isConstAllZ())
         orGate1.insert(andGate2.synAnd2Gate(), true);  
   }
   else if (_trueCond != 0 && _falseCond != 0) {
      if (_finPin != 0) // topDefault
         topDefault = _finPin;
      andGate2.insert(_ctrlCond, true);
      _trueCond->synZCond(orGate1, andGate2, topDefault);
      andGate2.chaState(false);
      _falseCond->synZCond(orGate1, andGate2, topDefault);
      andGate2.pop_back();
   }
   else { // if topDefault = 0  =>  synthesize latch
      if (_finPin != 0) // topDefault
         topDefault = _finPin;

      if (topDefault == 0) {
         assert (_ctrlCond != 0);
         if (_trueCond != 0 && _falseCond == 0) {
            andGate2.insert(_ctrlCond, true);
            _trueCond->synZCond(orGate1, andGate2, topDefault);
         }
         else {
            andGate2.insert(_ctrlCond, false);
            _falseCond->synZCond(orGate1, andGate2, topDefault);
         }
      }
      else { // has topDefault => check if Z
         if (_trueCond != 0 && _falseCond == 0) {
            andGate2.insert(_ctrlCond, true);
            _trueCond->synZCond(orGate1, andGate2, topDefault);
            andGate2.chaState(false);
            if (topDefault->isConstAllZ())
               orGate1.insert(andGate2.synAnd2Gate(), true);
         }
         else {
            andGate2.insert(_ctrlCond, true);
            if (topDefault->isConstAllZ())
               orGate1.insert(andGate2.synAnd2Gate(), true);
            andGate2.chaState(false);
            _falseCond->synZCond(orGate1, andGate2, topDefault);
         }       
      }
      andGate2.pop_back();      
   }
}

void
SynCond::addFin(vector<int>& iAry, vector<finPair>& finOutPins)
{
   CktOutPin* cellOut;
   CktInPin*  in;
   CktCell*   cell;
   unsigned   width = 0;
   if (_finPin == 0)
      cell = new CktMergeCell(true, cellOut);
   else {
      cell = new CktBusCell(true, in, cellOut);
      in->connOutPin(_finPin); _finPin->connInPin(in);         
   }
   SYN->insertCktCell(cell);
   for (unsigned i = 0; i < iAry.size(); ++i) {
      in = new CktInPin();
      in->connCell(cell); cell->connInPin(in);
      in->connOutPin(finOutPins[iAry[i]]._finPin); finOutPins[iAry[i]]._finPin->connInPin(in);
      width += VLDesign.getBus(finOutPins[iAry[i]]._finPin->getBusId())->getWidth();
   }
   if (_finPin == 0) {
      int busId = VLDesign.genBusId(width, 0, width - 1, busDirectionDefault);
      cellOut->setBusId(busId);      
   }
   else
      cellOut->setBusId(_finPin->getBusId());

   _finPin = cellOut;   
}

void
SynCond::addFin(vector<SynCond*>& condAry)
{
   CktOutPin* cellOut;
   CktInPin*  in;
   CktCell*   cell;
   unsigned   width = 0;
   if (_finPin == 0)
      cell = new CktMergeCell(true, cellOut);
   else {
      cell = new CktBusCell(true, in, cellOut);
      in->connOutPin(_finPin); _finPin->connInPin(in);         
   }
   SYN->insertCktCell(cell);
   for (unsigned i = 0; i < condAry.size(); ++i) {
      in = new CktInPin();
      in->connCell(cell); cell->connInPin(in);
      in->connOutPin(condAry[i]->getFinPin()); condAry[i]->getFinPin()->connInPin(in);
      width += VLDesign.getBus(condAry[i]->getFinPin()->getBusId())->getWidth();
   }
   if (_finPin == 0) {
      int busId = VLDesign.genBusId(width, 0, width - 1, busDirectionDefault);
      cellOut->setBusId(busId);      
   }
   else
      cellOut->setBusId(_finPin->getBusId());

   _finPin = cellOut;
}

void
SynCond::mergeCondTree(vector<SynCond*>& condAry)
{
   if (_ctrlCond != 0)
      assert (_ctrlCond == condAry[0]->getCtrlCond());
   else
      _ctrlCond = condAry[0]->getCtrlCond();

   if (condAry[0]->getFinPin() != 0)
      addFin(condAry);

   vector<SynCond*> conds;
   conds.clear();
   if (condAry[0]->getSubCond(true) != 0) {
      for (unsigned i = 0; i < condAry.size(); ++i)
         conds.push_back(condAry[i]->getSubCond(true));
      if (_trueCond == 0)
         _trueCond = new SynCond();
      _trueCond->mergeCondTree(conds);
   }

   conds.clear();
   if (condAry[0]->getSubCond(false) != 0) {
      for (unsigned i = 0; i < condAry.size(); ++i)
         conds.push_back(condAry[i]->getSubCond(false));
      if (_falseCond == 0)
         _falseCond = new SynCond();
      _falseCond->mergeCondTree(conds);
   }
}

/* Assume SynCond is a full tree, that is, no ctrlCond have to be added
   So if _trueCond or _falseCond == 0, the leaf found, use setSpDefCond to insert the node with _finPin
   else, NOT leaf case, recursively find leaf node using this function 
   defaultPin is the CktOutPin that used to fill in _finPins of SynCond */
void
SynCond::setSpDefPin(CktOutPin* defaultPin, unsigned busId)
{
   if (_finPin == 0) {
      if (_trueCond == 0)
         _trueCond = setSpDefCond(defaultPin, busId);
      else
         _trueCond->setSpDefPin(defaultPin, busId);

      if (_falseCond == 0)
         _falseCond = setSpDefCond(defaultPin, busId);
      else
         _falseCond->setSpDefPin(defaultPin, busId);            
   }
}

/* Assume SynCond is NOT a full tree, that is, if cond is NOT a leaf but SynCond is, createCond for SynCond
   If cond is a leaf, call function setSpDefPin
   else, recursively call this function to find if cond is a leaf */

void
SynCond::RecursiveSetSpDefPin(SynCond* cond, unsigned busId)
{
   if (getCtrlCond() == cond->getCtrlCond()) {
      if (cond->_trueCond) {
         if (cond->_trueCond->_finPin) _trueCond = setSpDefCond(cond->_trueCond->_finPin, busId);
         else {
            if (!_trueCond) _trueCond = createCond(cond->_trueCond);
            _trueCond->RecursiveSetSpDefPin(cond->_trueCond, busId);
         }
      }
      if (cond->_falseCond) {
         if (cond->_falseCond->_finPin) _falseCond = setSpDefCond(cond->_falseCond->_finPin, busId);
         else {
            if (!_falseCond) _falseCond = createCond(cond->_falseCond);
            _falseCond->RecursiveSetSpDefPin(cond->_falseCond, busId);
         }
      }
   }
   
   /*
   if (refCond->getFinPin() != NULL)
      setSpDefPin(refCond->getFinPin(), busId);

   if (getCtrlCond() == refCond->getCtrlCond()) {
      if (refCond->_trueCond) {
         if (!_trueCond) _trueCond = createCond(refCond->_trueCond);
         _trueCond->RecursiveSetSpDefPin(refCond->_trueCond, busId);
      }

      if (refCond->_falseCond) {
         if (!_falseCond) _falseCond = createCond(refCond->_falseCond);
         _falseCond->RecursiveSetSpDefPin(refCond->_falseCond, busId);
      }
   }
   */
}

SynCond*
SynCond::setSpDefCond(CktOutPin* defaultPin, unsigned busId)
{
   CktSplitCell* spCell = 0;
   CktOutPin*    finPin = 0;
   for (unsigned i = 0; i < defaultPin->getInPinSize(); ++i) {
      if (defaultPin->getFoutCell(i)->getCellType() == CKT_SPLIT_CELL) {
         spCell = static_cast<CktSplitCell*>(defaultPin->getFoutCell(i));
         break;
      }
   }
   if (spCell == 0) {
      spCell = new CktSplitCell();
      SYN->insertCktCell(spCell);

      CktInPin* in = new CktInPin();
      in->connCell(spCell);       spCell->connInPin(in);
      in->connOutPin(defaultPin); defaultPin->connInPin(in);
   }
   for (unsigned i = 0; i < spCell->getOutPinSize(); ++i) {
      if (spCell->getBusId(i) == busId) {
         finPin = spCell->getOutPin(i);
         break;
      }
   }
   if (finPin == 0) {
      finPin = new CktOutPin();
      const SynBus* bus = VLDesign.getBus(busId);
      unsigned width = bus->getWidth();
      bool isInv = bus->isInverted();
      unsigned newBusId = VLDesign.genBusId(width, 0, width - 1, isInv);
      finPin->setBusId(newBusId);
      finPin->connCell(spCell); spCell->connOutPin(finPin);
      spCell->insertBusId(busId);
   }
   SynCond* cond = new SynCond(finPin);
   return cond;
}

SynCond*
SynCond::createCond(SynCond* cond)
{
   SynCond* newCond = new SynCond();
   newCond->setCtrlCond(cond->_ctrlCond);
   if (cond->_trueCond) newCond->_trueCond = createCond(cond->_trueCond);
   if (cond->_falseCond) newCond->_falseCond = createCond(cond->_falseCond);
   return newCond;
}

void 
SynCond::setCtrlNode(const VlpBaseNode* n)
{
   _ctrlNode = n;
}
   
void 
SynCond::setSeNode(const VlpBaseNode* n)
{
   _seNode = n;
}

const VlpBaseNode* 
SynCond::getCtrlNode() const
{
   return _ctrlNode;
}
   
const VlpBaseNode* 
SynCond::getSeNode() const
{
   return _seNode;
}

CktOutPin*
SynCond::writeCDFG(ofstream& os, const unsigned& i) const {
   if (_finPin) {
      os << TAB_INDENT << TAB_INDENT << "\"" << _finPin << i << "\"" << " [shape = box, label = " << _finPin->getName() << "]" << endl;
      return _finPin;
   }
   else {
      assert(_ctrlCond);
      CktOutPin* outPin = 0;
      os << TAB_INDENT << TAB_INDENT << "\"" << _ctrlCond << i << "\"" << " [shape = trapezium, label = " << _ctrlCond->getName() << "]" << endl;
      if (_trueCond != 0) {
         outPin = _trueCond->writeCDFG(os, i);
         os << TAB_INDENT << TAB_INDENT << "\"" << _ctrlCond << i << "\"" << " -> " << "\"" << outPin << i << "\"";
         os << " [dir = back]" << endl;
      }
      if (_falseCond != 0) {
         outPin = _falseCond->writeCDFG(os, i);
         os << TAB_INDENT << TAB_INDENT << "\"" << _ctrlCond << i << "\"" << " -> " << "\"" << outPin << i << "\"";
         os << " [dir = back, style = dotted]" << endl;
      }
      return _ctrlCond;
   }
}

//========== class SynCondState ==========//
SynCondState::SynCondState(CktOutPin* ctrlCond, bool state)
{
   _ctrlCond = ctrlCond; 
   _state = state; 
}

SynCondState::SynCondState()
{
   _ctrlCond = 0; 
   _state   = false; 
}

SynCondState::~SynCondState()
{
}
   
void 
SynCondState::setCtrlCond(CktOutPin* ctrlCond)
{
   _ctrlCond = ctrlCond;
}
   
void 
SynCondState::setState(bool state)
{
   _state = state;
}
   
CktOutPin* 
SynCondState::getCtrlCond() const
{
   return _ctrlCond;
}

bool 
SynCondState::getState() const 
{
   return _state;
}

void 
SynCondState::setCtrlNode(const VlpBaseNode* n)
{
   _ctrlNode = n;
}

const VlpBaseNode* 
SynCondState::getCtrlNode() const
{
   return _ctrlNode;
}

// ==========  class  SynSharNodeMap   ========== //
SynSharNodeMap::SynSharSigNode::SynSharSigNode(CktOutPin* outPin) 
{ 
   _outPin = outPin; 
   _spCell = 0;
}

SynSharNodeMap::SynSharSigNode::~SynSharSigNode()
{
}

CktOutPin* 
SynSharNodeMap::SynSharSigNode::getOutPin() const
{
   return _outPin;
}

CktOutPin*
SynSharNodeMap::SynSharSigNode::genFout(int& foutBusId)
{
   if (_spCell == 0) {
      _spCell = new CktSplitCell();
      SYN->insertCktCell(_spCell);

      CktInPin* inPin = new CktInPin();
      _spCell->connInPin(inPin); inPin->connCell(_spCell);
      _outPin->connInPin(inPin); inPin->connOutPin(_outPin);
   }
   return _spCell->genFoutPin(foutBusId);
}

// Note : This function sets a complete single bit slicing outPin vector in acscending bit-index order
//        e.g. x[3:0] --> outPins = {0, 1, 2, 3}
//        e.g. x[5:2] --> outPins = {2, 3, 4, 5}
//        e.g. x[1:4] --> outPins = {1, 2, 3, 4}
void
SynSharNodeMap::SynSharSigNode::genAllFout(vector<CktOutPin*>& outPins, const unsigned& max_sel)
{
   outPins.clear();
   unsigned begin = getOutPin()->getBusBegin();
   unsigned end   = getOutPin()->getBusEnd();
   unsigned i = (begin < end) ? begin : end;
   unsigned j = (begin < end) ? end : begin;
   if (j > max_sel) j = max_sel;  // If maximum possible select index is less than msb, we split till maximum possible index only
   assert (i < j);

   int busId;
   do {
      busId = VLDesign.genBusId(1, i, i, busDirectionDefault);
      outPins.push_back(genFout(busId));
   } while (++i <= j);
   assert (outPins[0]->getBusBegin() == begin);
   assert (outPins[outPins.size() - 1]->getBusBegin() == j);
}

//The function will set _spCell to null in order to mis-use wrong _spCell
void
SynSharNodeMap::SynSharSigNode::update(CktOutPin* newPin)
{
   _outPin = newPin;
   _spCell = 0;
}
     
bool 
SynSharNodeMap::mapKey::operator ==(const mapKey& key) const
{
   if (_type == key._type && _left == key._left && _right == key._right)
      return true;
   else
      return false;
}
      
SynSharNodeMap::mapKey& 
SynSharNodeMap::mapKey::operator =(const mapKey& key)
{
   _type  = key._type;
   _left  = key._left;
   _right = key._right;
   return *this;
}
      
bool 
SynSharNodeMap::mapKey::operator <(const mapKey& key) const
{
   if (_type < key._type)
      return true;
   else if (_type == key._type) {
      if (_left < key._left)
         return true;
      else if (_left == key._left) {
         if (_right < key._right)
            return true;
         else
            return false;
      }
      else
         return false;
   }
   else
      return false;
}

SynSharNodeMap::SynSharNodeMap()
{
   clear();
}

SynSharNodeMap::~SynSharNodeMap()
{
   clear();
}

void 
SynSharNodeMap::clear()
{
   // delete redundant pins
   for (OutPinMap::iterator it = _replaceMap.begin(); it != _replaceMap.end(); ++it) 
      delete (it->first);

   _sigMap.clear();
   _opMap.clear();
   _replaceMap.clear();
}

//for variable
void 
SynSharNodeMap::insert(const string& name, CktOutPin* outpin)
{
   assert (name != ""); //louis debug
   assert (outpin->getName() != "");
   //assert (name == outpin->getName());  //louis debug

   SynSharSigNode* node = new SynSharSigNode(outpin);
   _sigMap.insert(pair<string, SynSharSigNode*>(name, node));
}

//for operator node
void 
SynSharNodeMap::insert(CktCellType type, CktOutPin* left, CktOutPin* right, CktOutPin* outpin)
{
   mapKey key(type, left, right);
   _opMap.insert( pair<mapKey, CktOutPin*>(key, outpin) );
}

void
SynSharNodeMap::replace(CktOutPin*& oriPin, CktOutPin* newPin) // update sharing pin
{
   if (oriPin->getName() != "") {
      map<string, SynSharSigNode*>::iterator it = _sigMap.find(oriPin->getName());
      assert (it != _sigMap.end());
      it->second->update(newPin);
   }
   OutPinMap::iterator is = _replaceMap.find(oriPin);
   while (is != _replaceMap.end()) {
      oriPin = is->second; // change target, the cell oriPin connected doesn't connect oriPin
      is = _replaceMap.find(oriPin);
   }
   _replaceMap.insert(make_pair(oriPin, newPin)); 
}

void
SynSharNodeMap::replace(string oriPinName, CktOutPin* newPin)
{
   map<string, SynSharSigNode*>::iterator it = _sigMap.find(oriPinName);
   assert (it != _sigMap.end());
   it->second->update(newPin);
}

void 
SynSharNodeMap::reNewPin(CktOutPin*& pin) 
{                                         
// some pin may be remove in other synVar by this function [SynVar::connCombFin -> combOutPin],
// so we use the function to renew and update pin                                      
   OutPinMap::iterator is = _replaceMap.find(pin);
   while (is != _replaceMap.end()) {
      pin = is->second; // update to new pin
      is = _replaceMap.find(pin);
   }
}

CktOutPin*
SynSharNodeMap::getSharPin(CktCellType type, CktOutPin* left, CktOutPin* right) const
{
   mapKey key(type, left, right);
   map<mapKey, CktOutPin*>::const_iterator it = _opMap.find(key);
   if (it == _opMap.end()) return 0;
   else return it->second;
}
   
CktOutPin*
SynSharNodeMap::getSharPin(const string& s) const
{
   map<string, SynSharSigNode*>::const_iterator it = _sigMap.find(s);
   if (it == _sigMap.end()) 
      return 0;
   else 
      return it->second->getOutPin();
}

CktOutPin*
SynSharNodeMap::genVarDeclaredOutpin(const string& varName) const
{
   CktOutPin* outPin = getSharPin(varName);
   if (outPin != 0)
      return outPin;

   //generate a CktOutPin for a variable that doesn't exist in outPinMap
   VlpPortNode* port;
   bool exist = curVlpModule->getPort(varName, port);

   if (!exist) {
      Msg(MSG_ERR) << "\"" << varName << "\" is not declared." << endl;
      assert (0);
      return 0;
   }
   else {
      //variable is not input, outpin, and inout
      int busId = port->port2Bus();
      outPin = new CktOutPin;
      outPin->setName(varName);
      outPin->setBusId(busId);
      assert (port->getIoType() != input);
      assert (port->getIoType() != output);
      assert (port->getIoType() != inout);

      string typeStr = port->NetType2Str();
      //curCktModule->insertVar(outPin);
      //curCktModule->insertVarType(typeStr);     // ----> for writeDesign usage ?? 
      SYN->outPinMap.insert(varName, outPin);
      return outPin;
   }
}

CktOutPin* 
SynSharNodeMap::genVarOutpinFout(string& s, int busId)
{
   assert (s != "");
   SynSharSigNode* sharNode = getSharNode(s);
   assert (sharNode != 0);
   return sharNode->genFout(busId);
}

// Note : This Functions will generate all single bit slicing and multiplex single bit output by sel values
CktOutPin*
SynSharNodeMap::genVarOutPinAllFout(string& s, CktOutPin* sel)
{
   if (sel->getBusWidth() >= (sizeof(unsigned) << 3)) {
      Msg(MSG_ERR) << "Index Range for Signal Slicing is Too Large (RHS = " << s
                   << ", Width = " << sel->getBusWidth() << " >= " << (sizeof(unsigned) << 3) << " !!!" << endl;
      return 0;
   }

   assert (s != "");
   SynSharSigNode* sharNode = getSharNode(s);
   assert (sharNode != 0);
   
   // Generate all Single Bit Slice OutPins
   const unsigned selWidth = sel->getBusWidth();
   const unsigned idxSize = (1 << selWidth) - 1;
   vector<CktOutPin*> allOutPins;
   sharNode->genAllFout(allOutPins, idxSize);
   
   // CktPins with Corresponding BV
   CktOutPin *OutPin = 0, *oFalse = 0, *oTrue = 0, *oSel = 0;
   CktInPin *iFalse, *iTrue, *iSel;
   Bv4* value = 0;
   
   // Build Case Tree by MUX Chain
   int begin = allOutPins[0]->getBusBegin();
   int end   = begin + allOutPins.size();
   int i, j;
   for (i = end - 1, j = allOutPins.size() - 1; i >= begin; --i, --j) {
      // Set True and False Pin for MUX
      if (!OutPin) {  // First MUX
         //value = new Bv4("1'bx"); assert (value);
	 value = new Bv4("1'b0"); assert (value);  // In fact, this should be set to default or illegal value
	 SYN->insertCktCell(new CktConstCell(oFalse, value, value->str().str()));
	 assert (oFalse);
      }
      else oFalse = OutPin;
      oTrue = allOutPins[j];
      // Generate a CONST for the Index
      value = new Bv4(myInt2Str(selWidth) + "'d" + myInt2Str(i)); assert (value);
      SYN->insertCktCell(new CktConstCell(OutPin, value, value->str().str()));
      OutPin->setBusId(VLDesign.genBusId(selWidth, 0, selWidth - 1, busDirectionDefault));
      // Generate a EQUALITY for Select Signal
      SYN->insertCktCell(new CktEqualityCell(true, iFalse, iTrue, oSel));
      sel->connInPin(iFalse);   iFalse->connOutPin(sel);
      OutPin->connInPin(iTrue); iTrue->connOutPin(OutPin);
      oSel->setBusId(VLDesign.genBusId(1, 0, 0, busDirectionDefault));
      // Generate a new MUX
      SYN->insertCktCell(new CktMuxCell(true, iFalse, iTrue, iSel, OutPin));
      oFalse->connInPin(iFalse); iFalse->connOutPin(oFalse);
      oTrue->connInPin(iTrue);   iTrue->connOutPin(oTrue);
      oSel->connInPin(iSel);     iSel->connOutPin(oSel);
      OutPin->setBusId(VLDesign.genBusId(1, 0, 0, busDirectionDefault));
      assert (OutPin);
   }
   return OutPin;
}
   
SynSharNodeMap& 
SynSharNodeMap::operator =(const SynSharNodeMap& nMap)
{
   _sigMap = nMap._sigMap;
   _opMap  = nMap._opMap;
   return *this;
}

SynSharNodeMap::SynSharSigNode* 
SynSharNodeMap::getSharNode(const string& s) const
{
   map<string, SynSharSigNode*>::const_iterator it = _sigMap.find(s);  
   if (it == _sigMap.end())
      return 0;
   else
      return it->second;
}

bool 
SynSharNodeMap::isSigExist(string& s) const
{
   assert (s != "");
   SynSharSigNode* sharNode = getSharNode(s);
   if (sharNode == 0)
      return false;
   else
      return true;
}

// ==========  class  synSensitivityList   ========== //
SensitivityNode::SensitivityNode(bool edge, CktOutPin* pin) 
{ 
   _edge = edge; 
   _pin = pin; 
}

SensitivityNode::~SensitivityNode()
{
}

string 
SensitivityNode::getName() const 
{
   return _pin->getName();
}

CktOutPin* 
SensitivityNode::getOutpin() const 
{
   return _pin;
}

bool 
SensitivityNode::getEdge() const 
{
   return _edge;
}

CktOutPin* 
SensitivityNode::getClkOrRstPin()
{
   CktOutPin* invClk;

   if (_edge)
      return _pin;
   else {
      invClk = SYN->outPinMap.getSharPin(CKT_INV_CELL, _pin, 0);
      if (invClk != 0)
         return invClk;
      else {
         CktInPin* inpin;
         SYN->insertCktCell(new CktInvCell(true, inpin, invClk));
         _pin->connInPin(inpin);
         inpin->connOutPin(_pin);
         invClk->setBusId(_pin->getBusId());
         SYN->outPinMap.insert(CKT_INV_CELL, _pin, 0, invClk);

         return invClk;
      }
   }
}

SensitivityList::SensitivityList()
{
}
      
SensitivityList::~SensitivityList()
{
   for (unsigned i = 0; i < _list.size(); ++i)
      delete _list[i];
}

void 
SensitivityList::insert(SensitivityNode* node) 
{ 
   _list.push_back(node); 
}
   
unsigned 
SensitivityList::size() const 
{
   return _list.size();
}

SensitivityNode* 
SensitivityList::operator[](const unsigned& i) const
{
   if (i >= _list.size()) {
      Msg(MSG_ERR) << "Error index of synCondNodeList " << endl;
      assert (0);
      return 0;
   }
   return _list[i];
}

// ==========  class  SynBus   ========== //
SynBus::SynBus(unsigned width, int begin, int end, bool isInverted)
{
   //assert (width > 0);
   _width      = width;
   _begin      = begin;
   _end        = end;
   _isInverted = isInverted;
   _msb        = 0;
   _lsb        = 0;
}
      
SynBus::~SynBus()
{
}
      
bool 
SynBus::operator == (const SynBus& node) const
{
   if( _width      == node._width
    && _begin      == node._begin
    && _end        == node._end
    && _isInverted == node._isInverted)
       return true;
   else
       return false;
}

bool 
SynBus::isEqual(unsigned& width, int& begin, int& end, bool& isInverted) const
{
   if( _width      == width
    && _begin      == begin
    && _end        == end
    && _isInverted == isInverted)
       return true;
   else
       return false;
}

bool
SynBus::isContain(const SynBus*& bus) const
{
   return (_begin <= bus->getBegin() && _end >= bus->getEnd());
}
      
unsigned
SynBus::getWidth() const
{
   return _width;
}
      
int 
SynBus::getBegin() const
{
   return _begin;
}
      
int
SynBus::getEnd() const
{
   return _end;
}
      
bool 
SynBus::isInverted() const
{
   return _isInverted;
}

CktOutPin* 
SynBus::getMsb() const
{ 
   return _msb; 
}
      
CktOutPin* 
SynBus::getLsb() const
{ 
   return _lsb; 
}

void
SynBus::print() const
{
   cout << "(width, begin, end, isInv ) = ( " 
        << _width << ", " << _begin << ", " << _end << ", " << _isInverted << " ) ";

   if (_isInverted)
      cout << "[" << _end << " : " << _begin << endl;
   else
      cout << "[" << _begin << " : " << _end << endl;
}

// ==========  class  SynConstAry   ========== //

SynConstAry::SynConstAry()
{
   clear();
}
      
SynConstAry::~SynConstAry()
{
   clear();
}
      
void 
SynConstAry::insert(CktOutPin* outpin) 
{
   _list.push_back(outpin);
   assert (outpin->getCell() != 0);
}
      
int
SynConstAry::isExist(const Bv4* value, int busId)
{
   const Bv4* tmp;
   for (unsigned i = 0; i < _list.size(); i++) {
      tmp = static_cast<CktConstCell*>(_list[i]->getCell())->getBvValue();
      if (( ((int)_list[i]->getBusId()) == busId) && (*tmp == *value) )
         return i;
   }
   return -1;
}
      
CktOutPin* 
SynConstAry::operator[](const unsigned& i) const
{
   if (i >= _list.size()) {
      Msg(MSG_ERR) << "Error index of synCondNodeList "<< endl;
      return 0;
   }
   return _list[i];
}     

int 
SynConstAry::size() const
{
   return _list.size();
}
      
void 
SynConstAry::clear() 
{
   _list.clear();
}

// ==========  class  SynOutFile   ========== //

SynOutFile::SynOutFile()
{
}
   
SynOutFile::~SynOutFile()
{
}

bool 
SynOutFile::isExistWire(string& name) const
{
   if (_wireSet.find(name) != _wireSet.end())
      return true;

   return false;
}
   
void 
SynOutFile::insertWireInst(string wireInst, string wireName) 
{ 
   _wireInst << wireInst; 
   _wireSet.insert(wireName); 
}
   
void 
SynOutFile::insertRegInst(string regInst, string regName) 
{ 
   _wireInst << regInst; 
   _wireSet.insert(regName); 
}
   
void 
SynOutFile::insertWireInst(string wireName) 
{ 
   _wireSet.insert(wireName); 
}
   
void 
SynOutFile::insertModuleInst(string moduleInst) 
{ 
   _moduleInst << moduleInst; 
}
   
void 
SynOutFile::open(const char* fileName) 
{ 
   _synOutFile.open(fileName); 
}
   
void 
SynOutFile::close() 
{ 
   _synOutFile.close(); 
}

void 
SynOutFile::flushModuleDef()
{
   _synOutFile << _wireInst.str();
   _synOutFile << _moduleInst.str();
   _wireInst.str("");
   _moduleInst.str("");
   _wireSet.clear();
}
   
void 
SynOutFile::insert(string& str) 
{
   _synOutFile << str; 
}
   
SynOutFile& 
operator << (SynOutFile& file, const string& str)
{
   file._synOutFile << str;
   return file;
}
   
SynOutFile& 
operator << (SynOutFile& file, const int& str)
{
   file._synOutFile << str;
   return file;
}
   
SynOutFile& 
endl(SynOutFile& file)
{
   file._synOutFile << endl;
   return file;
}

SynForArgu::SynForArgu(string s, int i, int e, int v, int a, RelateOpClass t)
{
   _name     = s;
   _curValue = i;
   _endValue = e;
   _varValue = v;
   _isAdd    = a;
   _type     = t;
}

SynForArgu::SynForArgu(const SynForArgu& argu)
{
   _name     = argu.getName();
   _curValue = argu.getCurValue();
   _endValue = argu.getEndValue();
   _varValue = argu.getVarValue();
   _isAdd    = argu.getIsAdd();
   _type     = argu.getType();
}

SynForArgu::~SynForArgu()
{
}

const string& 
SynForArgu::getName() const
{
   return _name;
}

int
SynForArgu::getCurValue() const
{
   return _curValue;
}
   
int 
SynForArgu::getEndValue() const
{
   return _endValue;
}
   
int 
SynForArgu::getVarValue() const
{
   return _varValue;
}
   
int 
SynForArgu::getIsAdd() const
{
   return _isAdd;
}
   
RelateOpClass 
SynForArgu::getType() const
{
   return _type;
}
   
bool 
SynForArgu::isAdd() const
{
   if (_isAdd == 1)
      return true;
   else
      return false;
}
   
bool 
SynForArgu::isRepeat() const
{   
   if (_type == RELATEOP_LESS)
      if (_curValue < _endValue)
         return true;
   else if (_type == RELATEOP_LEQ)
      if (_curValue <= _endValue)
         return true;
   else if (_type == RELATEOP_GREATER)
      if (_curValue > _endValue)
         return true;
   else if (_type == RELATEOP_GEQ)
      if (_curValue >= _endValue)
         return true;
   else
      assert(0);

   return false;
}
   
void 
SynForArgu::update()
{
   if (isAdd())
      _curValue += _varValue;
   else
      _curValue -= _varValue;
}
   
CktOutPin* 
SynForArgu::syn2ConstCell() const
{
   CktOutPin* outPin;
   Bv4* bvValue = new Bv4(_curValue);
   //get bit number
   unsigned width = bvValue->bits();;
   int busId = VLDesign.genBusId(width, 0, width - 1, busDirectionDefault);
   int id = SYN->constAry.isExist(bvValue, busId);
   if (id == -1) {
      SYN->insertCktCell(new CktConstCell(outPin, bvValue, ""));
      outPin->setBusId(busId);
      SYN->constAry.insert(outPin);
      return outPin;
   }
   else {
      delete bvValue;
      return SYN->constAry[id];
   }
}

#endif
