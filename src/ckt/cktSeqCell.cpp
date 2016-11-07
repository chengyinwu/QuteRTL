/****************************************************************************
  FileName     [ cktSeqCell.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit sequential cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_SEQ_CELL_SOURCE
#define CKT_SEQ_CELL_SOURCE

#include "VLGlobe.h"
#include "cktSeqCell.h"

#include "cktBLIFile.h"

extern SynOutFile      synOutFile;
extern CktOutFile      cktOutFile;
extern int             instNumber;
extern VlpDesign&      VLDesign;
extern CKT_usage*      CREMap;

//======================================= CktDffCell ==========================================//

CktDffCell::CktDffCell() : CktCell(CKT_DFF_CELL) 
{
   _clk     = NULL;
   _rst     = NULL;
   _default = NULL;
}

CktDffCell::CktDffCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_DFF_CELL, isNewPin, iPin, oPin)
{
   _clk     = NULL;
   _rst     = NULL;
   _default = NULL;
}

CktDffCell::~CktDffCell()
{
}

void
CktDffCell::connReset( CktInPin *rst )
{
   _rst = rst;
}

CktInPin*
CktDffCell::getReset() const
{
   return _rst;
}

void
CktDffCell::connDefault( CktInPin *de )
{
  _default = de;
}

CktInPin*
CktDffCell::getDefault() const
{
   return _default;
}

void
CktDffCell::connClk(CktInPin* clk)
{
   _clk = clk;
}

CktInPin *
CktDffCell::getClk() const
{
   return _clk;
}

void
CktDffCell::writeOutput() const
{
   stringstream input;
   CktOutPin* yOutpin = getOutPin();              //output, Y
   CktOutPin* aOutpin = getInPin(0)->getOutPin(); //input,  D
   CktOutPin* clkOutpin = getClk()->getOutPin();  //clock signal
   CktOutPin* rstOutpin = 0; //reset signal
   CktOutPin* defOutpin = 0; //default signal

   string yName   = yOutpin->getName();
   string aName   = aOutpin->getName();
   string clkName = clkOutpin->getName();
   string rstName, defName;
   assert ((yName != "") && (aName != "") && (clkName != ""));

   const SynBus* yBus   = VLDesign.getBus(yOutpin->getBusId());
   //const SynBus* aBus   = VLDesign.getBus(aOutpin->getBusId());
   //const SynBus* clkBus = VLDesign.getBus(clkOutpin->getBusId());
   const SynBus *rstBus, *defBus;
   int yWidth = yBus->getWidth();

   if (getReset() != 0) {
      rstOutpin = getReset()->getOutPin(); //reset signal
      rstName   = rstOutpin->getName();
      assert (rstName != "");
      rstBus    = VLDesign.getBus(rstOutpin->getBusId());
      if (!synOutFile.isExistWire(rstName)) synOutFile.insertWireInst(genWireInst(rstOutpin), rstName);
   }   
   if (getDefault() != 0) {
      defOutpin = getDefault()->getOutPin(); //default signal
      defName   = defOutpin->getName();
      assert (defName != "");
      defBus    = VLDesign.getBus(defOutpin->getBusId());
      if (!synOutFile.isExistWire(defName)) synOutFile.insertWireInst(genWireInst(defOutpin), defName);   
   }
   if (!synOutFile.isExistWire(yName))   synOutFile.insertWireInst(genWireInst(yOutpin), yName);
   if (!synOutFile.isExistWire(aName))   synOutFile.insertWireInst(genWireInst(aOutpin), aName);      
   if (!synOutFile.isExistWire(clkName)) synOutFile.insertWireInst(genWireInst(clkOutpin), clkName);
   
   input.str("");
   if (getReset() == 0) { //SYNChronous
      string moduleName = "DVL_DFF_SYNC";
      input << moduleName << " #( " << yWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";
      input << genModuleInst("Q", yOutpin) << ", ";
      input << genModuleInst("D", aOutpin) << ", ";
      input << genModuleInst("CK", clkOutpin) << " );" << endl;       
   }
   else {                 //ASYNChronous
      string moduleName = "DVL_DFF_ASYNC";
      input << moduleName << " #( " << yWidth <<" ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";
      input << genModuleInst("Q", yOutpin) << ", ";
      input << genModuleInst("D", aOutpin) << ", ";
      input << genModuleInst("DEF", defOutpin) << ", ";
      input << genModuleInst("RN", rstOutpin) << ", ";
      input << genModuleInst("CK", clkOutpin) << " );" << endl;            
   }
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void 
CktDffCell::recordNS()
{
   //if (!isSimToggle()) return;
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   if (getReset() == NULL) // SYNC Reset
      _nsValue = fin0->getCell()->getSimValue(fin0);
   else { // ASYNC Reset
      fin0 = getReset()->getOutPin();
      Bv4 rstValue = fin0->getCell()->getSimValue(fin0);
      if (rstValue == CktCell::_1bitOne) { // Trigger Reset
         fin0 = getDefault()->getOutPin();
         _nsValue = getDefault()->getFinCell()->getSimValue(fin0);
      }     
      else if (rstValue == CktCell::_1bitZero) {
         fin0 = getInPin(0)->getOutPin();        
         _nsValue = fin0->getCell()->getSimValue(fin0);
      }
      else // reset value = 1'bX 
         assert (0);
   }
   unsigned outPinWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   assert (outPinWidth == _nsValue.bits());
}

void 
CktDffCell::seqUpdate()
{
#ifdef EVENT_DRIVEN_SIM
   if (getReset() == NULL) // SYNC Reset
      setSimToggle(getInPin(0)->getOutPin()->getCell()->isSimToggle());
   else // ASYNC Reset
      setSimToggle(getInPin(0)->getOutPin()->getCell()->isSimToggle()
                || getReset()->getFinCell()->isSimToggle()
                || getDefault()->getFinCell()->isSimToggle());

#endif
   _simValue = _nsValue;
   unsigned outPinWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   assert (outPinWidth == _simValue.bits());
}

void 
CktDffCell::setInitState()
{
   const SynBus* bus = VLDesign.getBus(getOutPin()->getBusId());
   unsigned bits = bus->getWidth();
   stringstream ss;
   ss << bits << "'b";
   for (unsigned i = 0; i < bits; ++i)
      ss << '0';

   _nsValue = ss.str();

   if (_default != 0 && _default->getFinCell()->getCellType() == CKT_CONST_CELL) {
      Bv4 value = (static_cast<CktConstCell*>(_default->getFinCell()))->getValueStr();
      if (value.bits() < bits)
         _nsValue |= value;
      else
         _nsValue = value;
   }
   _simValue = _nsValue;
   assert (bits == _simValue.bits());
}

void
CktDffCell::setDefinedValue(Bv4 value) {
   const SynBus* bus = VLDesign.getBus(getOutPin()->getBusId());
   assert (value.bits() == bus->getWidth());
#ifdef EVENT_DRIVEN_SIM
   setSimToggle(!(_simValue == value));
#endif
   _simValue = _nsValue = value;
}
   
Bv4 
CktDffCell::getNsValue() const
{ 
   return _nsValue; 
}

void 
CktDffCell::preLoad(string& valueStr)
{
   _nsValue = valueStr;
}

void 
CktDffCell::calLevel(unsigned lv)
{
   bool isUpdate;
   CktInPin*  inPin;
   CktOutPin* outPin;
   CktCell*   finCell;
   if (setVtRef())        isUpdate = true;
   else if (_level > lv)  isUpdate = true;
   else                   isUpdate = false;

   if (isUpdate) {
      _level = lv;  
      ++lv;
      inPin = getClk();
      if (inPin != NULL) {
         outPin = inPin->getOutPin();
         finCell = outPin->getCell();
         if (finCell != NULL) finCell->calLevel(lv);
      }
      inPin = getDefault();
      if (inPin != NULL) {
         outPin = inPin->getOutPin();
         finCell = outPin->getCell();
         if (finCell != NULL) finCell->calLevel(lv);
      }
      inPin = getReset();
      if (inPin != NULL) {
         outPin = inPin->getOutPin();
         finCell = outPin->getCell();
         if (finCell != NULL) finCell->calLevel(lv);
      }

      unsigned inPinSize = getInPinSize();
      for (unsigned i = 0; i < inPinSize; i++) {
         finCell = getFinCell(i);
         finCell->calLevel(lv);
      }
   }
}

void 
CktDffCell::collectFin(queue<CktCell*>& cellQueue) const
{
   CktInPin*  inPin;
   CktOutPin* outPin;
   CktCell*   finCell;

   inPin = getClk();
   if (inPin != NULL) {
      outPin = inPin->getOutPin();
      finCell = outPin->getCell();
      if (finCell != NULL) cellQueue.push(finCell);
   }
   inPin = getDefault();
   if (inPin != NULL) {
      outPin = inPin->getOutPin();
      finCell = outPin->getCell();
      if (finCell != NULL) cellQueue.push(finCell);
   }
   inPin = getReset();
   if (inPin != NULL) {
      outPin = inPin->getOutPin();
      finCell = outPin->getCell();
      if (finCell != NULL) 
         cellQueue.push(finCell);
   }

   unsigned inPinSize = getInPinSize();
   for (unsigned i = 0; i < inPinSize; ++i) {
      finCell = getFinCell(i);
      if (finCell != NULL) cellQueue.push(finCell);
   }
}

CktCell* 
CktDffCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktInPin* new_pin;
   CktCell* newCell = new CktDffCell();

   assert(getInPinSize() == 1);// assert InPinArray doesn't include clk, rst, and default pins
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   
   if (_clk != 0) {
      new_pin = new CktInPin();
      new_pin->connCell( newCell );
      (static_cast<CktDffCell*>(newCell))->connClk( new_pin );
      inPinMap[_clk] = new_pin;
   }
   if (_rst != 0) {
      new_pin = new CktInPin();
      new_pin->connCell( newCell );
      (static_cast<CktDffCell*>(newCell))->connReset( new_pin );
      inPinMap[_rst] = new_pin;
   }
   if (_default != 0) {
      new_pin = new CktInPin();
      new_pin->connCell( newCell );
      (static_cast<CktDffCell*>(newCell))->connDefault( new_pin );
      inPinMap[_default] = new_pin;
   }   
   return newCell;
}

void 
CktDffCell::connDuplicate(CktCell*& dupCell, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
/*                  +--------+
                    |        |
               -->--|CktCell |-->--   only signal direction outward 
                    |        |  
                    +--------+                   */
   CktInPin*  oriFanOut_InPin;
   CktOutPin* oriFanIn_OutPin;
   //outPin
   assert (dupCell->getOutPin() == outPinMap[getOutPin()]);
   for (unsigned i = 0; i < getOutPin()->getInPinSize(); ++i) {
      oriFanOut_InPin = getOutPin()->getInPin(i);
      if (inPinMap.find(oriFanOut_InPin) != inPinMap.end()) // the same module
         dupCell->getOutPin()->connInPin(inPinMap[oriFanOut_InPin]);
   }
   //inPin
   assert (getInPinSize() == 1);
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      assert (dupCell->getInPin(i) == inPinMap[getInPin(i)]);
      oriFanIn_OutPin = getInPin(i)->getOutPin();
      if (outPinMap.find(oriFanIn_OutPin) != outPinMap.end())
         dupCell->getInPin(i)->connOutPin(outPinMap[oriFanIn_OutPin]);
   }
   //clk
   if (_clk != 0) {
      assert ((static_cast<CktDffCell*>(dupCell))->getClk() == inPinMap[getClk()]);
      oriFanIn_OutPin = getClk()->getOutPin();
      if (outPinMap.find(oriFanIn_OutPin) != outPinMap.end())
         (static_cast<CktDffCell*>(dupCell))->getClk()->connOutPin(outPinMap[oriFanIn_OutPin]);
   }
   //rst
   if (_rst != 0) {
      assert ((static_cast<CktDffCell*>(dupCell))->getReset() == inPinMap[getReset()]);
      oriFanIn_OutPin = getReset()->getOutPin();
      if (outPinMap.find(oriFanIn_OutPin) != outPinMap.end())
         (static_cast<CktDffCell*>(dupCell))->getReset()->connOutPin(outPinMap[oriFanIn_OutPin]);
   }
   //default
   if (_default != 0) {
      assert ((static_cast<CktDffCell*>(dupCell))->getDefault() == inPinMap[getDefault()]);
      oriFanIn_OutPin = getDefault()->getOutPin();
      if (outPinMap.find(oriFanIn_OutPin) != outPinMap.end())
         (static_cast<CktDffCell*>(dupCell))->getDefault()->connOutPin(outPinMap[oriFanIn_OutPin]);
   }
}

// mark part => asynchorous uses
void 
CktDffCell::writeBLIF() const
{
   stringstream outFile;
   string dName = getInPin(0)->getOutPin()->getName();
   string qName = getOutPin()->getName();
   string cName = getClk()->getOutPin()->getName();
   int dWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int qWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int cWidth = VLDesign.getBus(getClk()->getOutPin()->getBusId())->getWidth();
   bool isInvD = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->isInverted(); 
   bool isInvQ = VLDesign.getBus(getOutPin()->getBusId())->isInverted(); 
 
   assert(dWidth == qWidth);
   assert(cWidth == 1);
   string ffOutName = qName;

//   if (getReset() != 0) // ASYN-RESET
//      ffOutName += getReset()->getOutPin()->getName();

   for (int i = 0; i < qWidth; ++i) {
      outFile << ".latch " << dName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << ffOutName;
      if (!getOutPin()->is1BitIoPin()) {
         if (isInvD == isInvQ) outFile << "[" << i << "]";
         else                  outFile << "[" << qWidth-1-i << "]";
      }
      outFile << " re ";
      outFile << cName;
      if (!getClk()->getOutPin()->is1BitIoPin()) outFile << "[0]";
      outFile << " 0" << endl;
   } 
/*   
   if (getReset() != 0) { // ASYN-RESET
      string rstName = getReset()->getOutPin()->getName();
      string defName = getDefault()->getOutPin()->getName();
      bool isInvDef = VLDesign.getBus(getDefault()->getOutPin()->getBusId())->isInverted(); 
      
      for (int i = 0; i < qWidth; ++i) {
         outFile << ".names " << rstName;
         if (getReset()->getOutPin()->is1BitIoPin()) outFile << "[0]";
      
         outFile << " " << ffOutName;
         // the same as data-pin [getInPin(0)->getOutPin()]
         if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      
         outFile << " " << defName;
         if (!getDefault()->getOutPin()->is1BitIoPin()) {         
            if (isInvDef == isInvQ) outFile << "[" << i << "]";         
            else                    outFile << "[" << qWidth-1-i << "]";
         } 
         outFile << " " << qName;
         if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
         outFile << " #asyn-ff-mux ";
#endif
         outFile << endl << "01- 1" << endl << "1-1 1" << endl;
      }         
   }*/
   cktOutFile.insert(outFile.str());  
}

//====================================== CktDlatCell ==========================================//

CktDlatCell::CktDlatCell() : CktCell(CKT_DLAT_CELL)
{
   _load = NULL;
}

CktDlatCell::CktDlatCell(bool isNewPin, CktInPin*& iPin, CktInPin*& iLod, CktOutPin*& oPin) 
            : CktCell(CKT_DLAT_CELL)
{
   if (isNewPin) {
      iPin = new CktInPin;
      iLod = new CktInPin;
      oPin = new CktOutPin;
   }
   iPin->connCell(this); connInPin(iPin);
   iLod->connCell(this); connLoad(iLod);
   oPin->connCell(this); connOutPin(oPin);
}

CktDlatCell::~CktDlatCell()
{
}

void
CktDlatCell::connLoad( CktInPin *load )
{
   _load = load;
}

CktInPin *
CktDlatCell::getLoad() const
{
   return _load;
}

void
CktDlatCell::writeOutput() const
{
   string moduleName = "DVL_LATCH";
   stringstream input;
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   CktOutPin* bOutpin = getLoad()->getOutPin(); //load signal
   string yName = yOutpin->getName();
   string aName = aOutpin->getName();
   string bName = bOutpin->getName();
   assert ((yName != "") && (aName != "") && (bName != ""));

   const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());
   //const SynBus* aBus = VLDesign.getBus(aOutpin->getBusId());
   //const SynBus* bBus = VLDesign.getBus(bOutpin->getBusId());
   int yWidth = yBus->getWidth();

   if (!synOutFile.isExistWire(yName)) synOutFile.insertWireInst(genWireInst(yOutpin), yName);
   if (!synOutFile.isExistWire(aName)) synOutFile.insertWireInst(genWireInst(aOutpin), aName);      
   if (!synOutFile.isExistWire(bName)) synOutFile.insertWireInst(genWireInst(bOutpin), bName);
   
   input.str("");
   input << moduleName << " #( " << yWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";     
   input << genModuleInst("Q", yOutpin) << ", ";   
   input << genModuleInst("D", aOutpin) << ", ";   
   input << genModuleInst("L", bOutpin) << " );" << endl;     
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void 
CktDlatCell::recordNS()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _nsValue = fin0->getCell()->getSimValue(fin0);
}

void 
CktDlatCell::seqUpdate()
{
#ifdef EVENT_DRIVEN_SIM
   setSimToggle(_simValue != _nsValue);
#endif
   if (isSimToggle()) _simValue = _nsValue;
}

void 
CktDlatCell::setInitState()
{
   const SynBus* bus = VLDesign.getBus(getOutPin()->getBusId());
   unsigned bits = bus->getWidth();
   stringstream ss;
   ss << bits << "'b";
   for (unsigned i = 0; i < bits; ++i)
      ss << '0';

   _nsValue = ss.str();

   if (_load != 0 && _load->getFinCell()->getCellType() == CKT_CONST_CELL) {
      Bv4 value = (static_cast<CktConstCell*>(_load->getFinCell()))->getValueStr();
      if (value.bits() < bits)
         _nsValue |= value;
      else
         _nsValue = value;
   }
   _simValue = _nsValue;
}

void
CktDlatCell::setDefinedValue(Bv4 value) {
   const SynBus* bus = VLDesign.getBus(getOutPin()->getBusId());
   assert (value.bits() == bus->getWidth());
#ifdef EVENT_DRIVEN_SIM
   setSimToggle(!(_simValue == value));
#endif
   _simValue = _nsValue = value;
}

Bv4 
CktDlatCell::getNsValue() const
{ 
   return _nsValue; 
}

void 
CktDlatCell::preLoad(string& valueStr)
{
   _nsValue = valueStr;
}

void 
CktDlatCell::calLevel(unsigned lv)
{
   bool isUpdate;
   CktInPin*  inPin;
   CktOutPin* outPin;
   CktCell*   finCell;
   if (setVtRef())        isUpdate = true;
   else if (_level > lv)  isUpdate = true;
   else                   isUpdate = false;

   if (isUpdate) {
      _level = lv;  
      ++lv;

      inPin = getLoad();
      if (inPin != NULL) {
         outPin = inPin->getOutPin();
         finCell = outPin->getCell();
         if (finCell != NULL) finCell->calLevel(lv);
      }

      unsigned inPinSize = getInPinSize();
      for (unsigned i = 0; i < inPinSize; i++) {
         finCell = getFinCell(i);
         finCell->calLevel(lv);
      }
   }
}

void 
CktDlatCell::collectFin(queue<CktCell*>& cellQueue) const
{
   CktInPin*  inPin;
   CktOutPin* outPin;
   CktCell*   finCell;

   inPin = getLoad();
   if (inPin != NULL) {
      outPin = inPin->getOutPin();
      finCell = outPin->getCell();
      if (finCell != NULL)
         cellQueue.push(finCell);
   }

   unsigned inPinSize = getInPinSize();
   for (unsigned i = 0; i < inPinSize; ++i) {
      finCell = getFinCell(i);
      cellQueue.push(finCell);
   }
}

#endif

