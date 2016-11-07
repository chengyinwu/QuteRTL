/****************************************************************************
  FileName     [ cktModelCell.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit modeling cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_CONST_CELL_SOURCE
#define CKT_CONST_CELL_SOURCE

#include "VLGlobe.h"
#include "cktModelCell.h"

#include "cktBLIFile.h"
#include "vlpItoStr.h"

#include "util.h"

#define PREFIX_CHAR "k"

extern SynOutFile       synOutFile;
extern CktOutFile       cktOutFile;
extern int              instNumber;
extern VlpDesign&       VLDesign;
extern CKT_usage*       CREMap;

// ========== CktConstCell ==========

CktConstCell::CktConstCell() : CktCell(CKT_CONST_CELL)
{
   _value    = NULL;
   _valueStr = "";
}

CktConstCell::CktConstCell(int num , string sValue)
             : CktCell(CKT_CONST_CELL), _valueStr(sValue)
{
   _value = new Bv4(num);
}

CktConstCell::CktConstCell(CktOutPin*& out, int num, string sValue)
             : CktCell(CKT_CONST_CELL), _valueStr(sValue)
{
   _value = new Bv4(num);
   out = new CktOutPin();
   connOutPin(out);
   out->connCell(this);
}

CktConstCell::CktConstCell(CktOutPin*& out, Bv4* value, string sValue)
             : CktCell(CKT_CONST_CELL), _valueStr(sValue)
{
   _value = value;
   out = new CktOutPin();
   connOutPin(out);
   out->connCell(this);
}

CktConstCell::~CktConstCell()
{
   if ( _value != NULL )
      delete _value;
}

void
CktConstCell::setBvValue( Bv4 *value )
{
   _value = value;
}

Bv4 *
CktConstCell::getBvValue() const
{
   return _value;
}

void
CktConstCell::setValueStr( string str )
{
   _valueStr = str;
}

string
CktConstCell::getValueStr() const
{
   return _valueStr;
}

string
CktConstCell::getExpr(CktOutPin* p) const
{
	assert (p == getOutPin());
	return _value->str().str();
}

void
CktConstCell::writeOutput() const
{
   stringstream input;
   CktOutPin* yOutpin = getOutPin();
   string  yName = yOutpin->getName();
   assert (yName != "");
   const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());

   int yWidth = yBus->getWidth();
   Bv4* value = getBvValue();

   if (value->fullx()) {
      string moduleName = "DVL_X_CELL";
      input << moduleName << " #( " << yWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";
      input << genModuleInst("Y", yOutpin) << " );" << endl;
   }
   else if (value->fullz()) {
      string moduleName = "DVL_Z_CELL";
      input << moduleName << " #( " << yWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";
      input << genModuleInst("Y", yOutpin) << " );" << endl;
   }
   else if (value->value() == (unsigned)(-1)) {   // to check!!
      cerr << "Error code ### - assign non acceptable value: " << *value << endl;
      string moduleName = "DVL_BUF/*error*/";
      input << moduleName << " #( " << yWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";
      input << genModuleInst("Y", yOutpin) << ", ";
      if (getValueStr() == "")
         input << ".A( " << value->value() << " ) );" << endl;
      else
         input << ".A( " << getValueStr() << " ) );" << endl;
   }
   else {
      string moduleName = "DVL_BUF/*const*/";
      input << moduleName << " #( " << yWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";
      input << genModuleInst("Y", yOutpin) << ", ";

      if ( getValueStr() == "" )
         input << ".A( " << value->value() << " ) );" << endl;
      else
         input << ".A( " << getValueStr() << " ) );" << endl;
   }   
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void
CktConstCell::simulate()
{
   //_simValue = *_value;
   _simValue = _valueStr;
   
   unsigned bits = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   if (bits != _simValue.bits())
      cout << "BV = " << _simValue << endl;
   assert (bits == _simValue.bits());
}

CktCell* 
CktConstCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktConstCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   (static_cast<CktConstCell*>(newCell))->setBvValue(getBvValue());
   (static_cast<CktConstCell*>(newCell))->setValueStr(getValueStr());
   return newCell;
}

void
CktConstCell::writeBLIF() const
{
   stringstream outFile;
   int iWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   string oName = getOutPin()->getName();
/*
   //int fanOutWidth = VLDesign.getBus(getOutPin()->getInPin(0)->getCell()->getOutPin()->getBusId())->getWidth();
   cout << "oName = " << oName << endl;
   cout << "valueStr = " << getValueStr() << endl;
   cout << "InPin size = " << getOutPin()->getInPinSize() << endl;
   cout << "fanout type = " << getOutPin()->getInPin(0)->getCell()->getCellType() << endl;
   //cout << "const bits = " << fanOutWidth << endl;
   cout << "iWidth = " << iWidth << endl; */ 
   for (int i = 0; i < iWidth; ++i) {
     outFile << ".names ";
     if ((*_value)[i] == _BV4_0) {
        outFile << oName << "[" << i << "] ";
#ifdef BLIF_DEBUG_MODE
        outFile << "#const 0 " << endl;
#endif
        outFile << "0" << endl;
     }
     else if ((*_value)[i] == _BV4_1) {
        outFile << oName << "[" << i << "] ";
#ifdef BLIF_DEBUG_MODE
        outFile << "#const 1" << endl;
#endif
        outFile << "1" << endl;
     }
     else if ((*_value)[i] == _BV4_X) {
        outFile << oName << "[" << i << "] ";
#ifdef BLIF_DEBUG_MODE
        outFile << "#const x" << endl;
#endif
        //outFile << "-" << endl;
        outFile << "1" << endl;
     }
     else  //((*_value)[i] == _BV4_Z)
        cout << "no support z(CktConstCell::writeBLIF())" << endl;
   }
   cktOutFile.insert(outFile.str());
   
}

// ========== CktSplitCell ==========

CktSplitCell::CktSplitCell(): CktCell(CKT_SPLIT_CELL)
{
   _isSplitFin = true;
}
   
CktSplitCell::CktSplitCell(bool isNewInPin, CktInPin*& in, vector<CktOutPin*>& outPinAry)
             :CktCell(CKT_SPLIT_CELL)
{
   _isSplitFin = true;
   if (isNewInPin)
      in = new CktInPin();

   connInPin(in); in->connCell(this);
   for (unsigned i = 0; i < outPinAry.size(); ++i) {
      connOutPin(outPinAry[i]);
      outPinAry[i]->connCell(this);
   }
}

CktSplitCell::~CktSplitCell()
{
}

void
CktSplitCell::connOutPin( CktOutPin* const pin )
{
   _splitOut.push_back( pin );
}

void        
CktSplitCell::replaceOutPin(CktOutPin* oldPin, CktOutPin* newPin)
{
   unsigned i;
   for (i = 0; i < _splitOut.size(); ++i)
      if (_splitOut[i] == oldPin) {
         _splitOut[i] = newPin;
         break;
      }

   if (i == _splitOut.size()) {
      cerr << "Error : can't find the oldPin for replacing !!" << endl;
      assert (0);
   }   
}

unsigned
CktSplitCell::getOutPinSize() const
{
   return _splitOut.size();
}

CktOutPin*
CktSplitCell::getOutPin(unsigned i) const
{
   assert( i >= 0 );
   assert( i < _splitOut.size() );

   return _splitOut[i];
}

CktOutPin* 
CktSplitCell::getOutPin() const
{
   cerr << "Error : the cell is mulit outputs cell \"CktSplitCell\". "
        << "Plase use another function \"getOutPin(int)\" " << endl;
   assert (0);
}

unsigned
CktSplitCell::getFoutSize() const
{
   cerr << "Error : the cell is mulit outputs cell \"CktSplitCell\". "
        << "Don't use the function \" getFoutSize() \" " << endl;
   assert (0);
}

/*void
CktSplitCell::eraseOutpin( CktOutPin *pin )
{
   CktOutPinAry::iterator pos = find( _splitOut.begin(), _splitOut.end(), pin );
   if ( pos != _splitOut.end() )
      _splitOut.erase( pos );
}*/
  
CktCell*    
CktSplitCell::getFoutCell(unsigned i, unsigned j) const
{
   return (getOutPin(i)->getFoutCell(j));
}

void
CktSplitCell::nameOutPin(int& num)
{
   string name;
   CktOutPin* outPin;
   for (unsigned i = 0; i < getOutPinSize(); ++i) {
      outPin = getOutPin(i);
      if (outPin->getName() == "") {
         name = PREFIX_CHAR + toString(num);
         outPin->setName(name);
         setAddedCell();
         ++num;
      }     
   }
}

void 
CktSplitCell::setNonSpFin()
{
   _isSplitFin = false;
}

bool 
CktSplitCell::isSplitFin() const
{
   return _isSplitFin;
}

CktOutPin* 
CktSplitCell::genFoutPin(unsigned spBusId)
{
   assert(_busIdList.size() == _splitOut.size());
   for (unsigned i = 0; i < _busIdList.size(); ++i)
      if (_busIdList[i] == spBusId)
         return _splitOut[i];

   CktOutPin* newSpOut = new CktOutPin();
   connOutPin(newSpOut); newSpOut->connCell(this);

   const SynBus* bus = VLDesign.getBus(spBusId);
   unsigned width = bus->getWidth();
   bool isInv = bus->isInverted();
   unsigned newBusId = VLDesign.genBusId(width, 0, width - 1, isInv);
   newSpOut->setBusId(newBusId);
   insertBusId(spBusId);
   return newSpOut;
}

string
CktSplitCell::getExpr(CktOutPin* p) const
{
   assert (getInPinSize() == 1);
   string expStr = getInPin(0)->getOutPin()->getCell()->getExpr(getInPin(0)->getOutPin());
   
   const SynBus* iBus = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId());
   assert (iBus->isInverted() == true);
   int varBegin = iBus->getBegin();
   unsigned i;
   for (i = 0; i < _splitOut.size(); ++i) { if (p == _splitOut[i]) break; }
   assert (i < _splitOut.size());

   const SynBus* oBus = VLDesign.getBus(_busIdList[i]);
   assert (oBus->isInverted() == true);
   int begin = oBus->getBegin();
   int end = oBus->getEnd();

   if (end == begin) return expStr + "[" + myInt2Str(end - varBegin) + "]";
   else return expStr + "[" + myInt2Str(end - varBegin) + ":" + myInt2Str(begin - varBegin) + "]";
}

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

             |--------|---- inPin 0  A[3:2] (will be renamed)
 A[7:0] -----| spCell |
             |--------|---- inPin 1  A[4:1] (will be renamed)
*/
void
CktSplitCell::writeOutput() const
{
   stringstream input;
   string moduleName, yName, aName;
   const SynBus *yBus, *aBus;
   int    yWidth, aMSB, aLSB;
   CktOutPin *yOutpin, *aOutpin; 

   aOutpin = getInPin(0)->getOutPin();
   aName = aOutpin->getName();
   assert (aName != "");
 
   if (!synOutFile.isExistWire(aName))
      synOutFile.insertWireInst(genWireInst(aOutpin), aName);
   
   if (_isSplitFin)
      moduleName = "DVL_BUF/*SplitR*/";
   else {
      moduleName = "DVL_BUF/*SplitL*/";
      aBus = VLDesign.getBus(aOutpin->getBusId());
      aMSB = (aBus->isInverted()) ? aBus->getEnd() : aBus->getBegin();
   }  
   for (unsigned i = 0; i < getOutPinSize(); ++i) {
      yOutpin = getOutPin(i);
      yName   = yOutpin->getName();
      yWidth  = VLDesign.getBus(yOutpin->getBusId())->getWidth();

      assert (yName != "");
      input.str("");
      input << moduleName << " #( " << yWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";

      if (_isSplitFin) {
         aBus = VLDesign.getBus(_busIdList[i]);
         yBus = VLDesign.getBus(yOutpin->getBusId());

         if (!synOutFile.isExistWire(yName))
            synOutFile.insertWireInst(genWireInst(yName, yBus->getWidth()), yName);
         //input << genModuleInst("Y", yName, yWidth-1, 0, false) << ", ";
         //input << genModuleInst("A", aName, yBus) << " );" << endl;
         
         input << genModuleInst("Y", yName, yBus, false) << ", ";
         input << genModuleInst("A", aName, aBus, true) << " );" << endl;
      }
      else {
         if (!synOutFile.isExistWire(yName))
            synOutFile.insertWireInst(genWireInst(yOutpin), yName);
         
         input << genModuleInst("Y", yOutpin) << ", ";
         if (aBus->isInverted()) {
            aLSB = aMSB - yWidth + 1;
            input << genModuleInst("A", aName, aMSB, aLSB) << " );" << endl;
            aMSB = aLSB - 1;
         }
         else {
            aLSB = aMSB + yWidth - 1;
            input << genModuleInst("A", aName, aMSB, aLSB) << " );" << endl;
            aMSB = aLSB + 1;
         }
      }
      ++instNumber;
      synOutFile.insertModuleInst(input.str());
   }
}

void
CktSplitCell::simulate()
{
   assert (getInPinSize() == 1);
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0);
}

Bv4
CktSplitCell::getSimValue(const CktOutPin* foutPin) const
{
   const SynBus* iBus = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId());
   assert (iBus->isInverted() == true);
   int varBegin = iBus->getBegin();
   unsigned index;
   for (index = 0; index < _splitOut.size(); ++index)
      if (_splitOut[index] == foutPin)
         break;

   assert (index < _splitOut.size());

   const SynBus* oBus = VLDesign.getBus(_busIdList[index]);
   assert (oBus->isInverted() == true);
   int  begin = oBus->getBegin();
   int  end   = oBus->getEnd();

   Bv4 spValue = _simValue.split(end-varBegin, begin-varBegin);
   //cout << "Original Bv = " << _simValue << " Split Bv = " << spValue << endl;
   return spValue;
}

void
CktSplitCell::insertBusId(unsigned short i)
{
   _busIdList.push_back(i);
}

void 
CktSplitCell::setFinBusId(unsigned short i)
{
   _oriFinBusId = i;
}

unsigned 
CktSplitCell::getBusId(int i) const
{
   return _busIdList[i];
}

void
CktSplitCell::reviseIoBus()
{
   unsigned finId = getInPin(0)->getOutPin()->getBusId();
   if (finId != _oriFinBusId) {
      const SynBus* oriBus = VLDesign.getBus(_oriFinBusId);
      const SynBus* finBus = VLDesign.getBus(finId);
      assert (oriBus->isInverted() == finBus->isInverted());
      assert (oriBus->getWidth()   == finBus->getWidth());
      int shift = finBus->getBegin() - oriBus->getBegin();
      int newBegin, newEnd;
      unsigned newId;
      for (unsigned i = 0; i < _busIdList.size(); ++i) {
         oriBus = VLDesign.getBus(_busIdList[i]);
         newBegin = oriBus->getBegin() + shift;
         newEnd = oriBus->getEnd() + shift;
         newId = VLDesign.genBusId(oriBus->getWidth(), newBegin, newEnd, oriBus->isInverted());
         reviseBusId(i, newId);
      }
   }
}
   
void 
CktSplitCell::reviseBusId(unsigned pos, unsigned id)
{
   _busIdList[pos] = id;
}

CktCell* 
CktSplitCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktSplitCell();
   //pin <--> cell : bi-direction connection within the function "pinDuplicate(...)"   
   CktInPin*  oriInPin = getInPin(0);
   oriInPin->pinDuplicate(newCell, inPinMap); 
   CktOutPin* oriOutPin;

   assert (getOutPinSize() == _busIdList.size());
   for (unsigned i = 0; i < getOutPinSize(); ++i) {
      oriOutPin = getOutPin(i);
      oriOutPin->pinDuplicate(newCell, prefix, outPinMap);//Does connOutPin() within it overload to 
                                                          //the connOutPin() of the CktSplit?  To check!!
      //In the local split cell, must save the fanin pin busId info. which will
      //loss in the completeFlatten stage.
      (static_cast<CktSplitCell*>(newCell))->insertBusId(_busIdList[i]);
   }
   (static_cast<CktSplitCell*>(newCell))->setFinBusId(oriInPin->getOutPin()->getBusId());
   return newCell;
}

void 
CktSplitCell::connDuplicate(CktCell*& dupCell, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
/*                  +--------+
                    |        |-->--
               --<--|SplitC  |-->--   only signal direction outward 
                    |        |-->--  
                    +--------+                   */
   CktInPin*  oriFanOut_InPin;
   CktOutPin* oriFanIn_OutPin;
   CktOutPin* oriCur_OutPin;
   //outPin
   for (unsigned i = 0; i < getOutPinSize(); ++i) {
      oriCur_OutPin = getOutPin(i);
      assert ((static_cast<CktSplitCell*>(dupCell))->getOutPin(i) == outPinMap[oriCur_OutPin]);
      for (unsigned j = 0; j < oriCur_OutPin->getInPinSize(); ++j) {
         oriFanOut_InPin = oriCur_OutPin->getInPin(j);
         if (inPinMap.find(oriFanOut_InPin) != inPinMap.end()) // the same module
            (static_cast<CktSplitCell*>(dupCell))->getOutPin(i)->connInPin(inPinMap[oriFanOut_InPin]);
      }
   }
   //inPin
   assert(getInPinSize() == 1);
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      assert (dupCell->getInPin(i) == inPinMap[getInPin(i)]);
      oriFanIn_OutPin = getInPin(i)->getOutPin();
      if (outPinMap.find(oriFanIn_OutPin) != outPinMap.end())
         dupCell->getInPin(i)->connOutPin(outPinMap[oriFanIn_OutPin]);
   }
}

void
CktSplitCell::writeBLIF() const
{
   assert (_isSplitFin == true);
   stringstream outFile;
   assert(getInPinSize() == 1);
   string iName;
   if (getInPin(0)->getOutPin() != 0)
      iName = getInPin(0)->getOutPin()->getName();
   string oName;
   assert (iName != oName);

   unsigned totalSplitNum = _splitOut.size();
   int num_of_outPin;
   int begin, end;
   bool isReverse;

   for (unsigned i = 0; i < totalSplitNum; ++i) {
      num_of_outPin = 0;
      begin     = VLDesign.getBus(_busIdList[i])->getBegin();
      end       = VLDesign.getBus(_busIdList[i])->getEnd();
      isReverse = VLDesign.getBus(_busIdList[i])->isInverted();
      oName = getOutPin(i)->getName();

      if (iName != oName) {
         if (isReverse) {
            for (int j = begin; j <= end; ++j) {
               outFile << ".names ";
               outFile << iName << "[" << j << "] ";
               outFile << oName;
               if (!getOutPin(i)->is1BitIoPin()) outFile << "[" << num_of_outPin << "]";
#ifdef BLIF_DEBUG_MODE
               outFile << " #split ";
#endif
               outFile << endl;
               outFile << "1 1" << endl;
               ++num_of_outPin;
            }
         }
         else {
            for (int j = end; j >= begin; --j) {
               outFile << ".names ";
               outFile << iName << "[" << j << "] ";
               outFile << oName;
               if (!getOutPin(i)->is1BitIoPin()) outFile << "[" << num_of_outPin << "]";
#ifdef BLIF_DEBUG_MODE
               outFile << " #split ";
#endif
               outFile << endl;
               outFile << "1 1" << endl;
               ++num_of_outPin;
            }
         }
      }
   }
   cktOutFile.insert(outFile.str());
}

// ========== CktMergeCell ==========

CktMergeCell::CktMergeCell() : CktCell(CKT_MERGE_CELL)
{
}

CktMergeCell::CktMergeCell(bool isNewPin, CktOutPin*& out) : CktCell(CKT_MERGE_CELL)
{
   if (isNewPin)
      out = new CktOutPin;
   
   out->connCell(this);
   connOutPin(out);   
}

CktMergeCell::~CktMergeCell()
{
}

string
CktMergeCell::getExpr(CktOutPin* p) const
{
	assert (p == getOutPin());
	if (getInPinSize() == 1) return getInPin(0)->getOutPin()->getCell()->getExpr(getInPin(0)->getOutPin());
	string expStr = "";
	for (unsigned i = 0; i < getInPinSize(); ++i) {
		assert (getInPin(i));
		if (i) expStr += ", ";
		expStr += getInPin(i)->getOutPin()->getCell()->getExpr(getInPin(i)->getOutPin());
	}
	return "{" + expStr + "}";
}

void
CktMergeCell::writeOutput() const
{
   stringstream input;
   string     moduleName = "DVL_BUF/*Merge*/";
   CktOutPin* yOutpin = getOutPin();
   string     yName   = yOutpin->getName();
   assert (yName != "");
   const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());
   int yBegin = yBus->getBegin();
   int yEnd   = yBus->getEnd();
   bool yRev  = yBus->isInverted();

   if (!synOutFile.isExistWire(yName))
      synOutFile.insertWireInst(genWireInst(yOutpin), yName);

   CktOutPin*    aOutpin;
   string        aName;
   const SynBus* aBus;
   int           aWidth;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      aOutpin = getInPin(i)->getOutPin();
      aName   = aOutpin->getName();
      aBus    = VLDesign.getBus(aOutpin->getBusId());
      aWidth  = aBus->getWidth();
      assert (aName != "");
      if (!synOutFile.isExistWire(aName))
         synOutFile.insertWireInst(genWireInst(aOutpin), aName);

      input.str("");
      input << moduleName << " #(" << aWidth << ") " << (CREMap->moduleNamePrefix) << instNumber
            << " ( ";
      if (yRev) {
         input << genModuleInst("Y", yName, yEnd, yEnd-aWidth+1) << ", ";
         yEnd -= aWidth;
      }
      else {
         input << genModuleInst("Y", yName, yBegin, yBegin+aWidth-1) << ", ";
         yBegin += aWidth;
      }
      input << genModuleInst("A", aOutpin);
      input << " );" << endl;
      ++instNumber;
      synOutFile.insertModuleInst(input.str());
   }
}

void
CktMergeCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0);
   for (unsigned i = 1; i < getInPinSize(); ++i) {
      fin0 = getInPin(i)->getOutPin();
      _simValue.append(fin0->getCell()->getSimValue(fin0));
   }
}

void
CktMergeCell::insertBusId(unsigned short i)
{
   _busIdList.push_back(i);
}

unsigned 
CktMergeCell::getBackUpBusId(int i) const
{
   return _busIdList[i];
}
   
void 
CktMergeCell::checkIoWidth() const
{
   CktOutPin* outPin;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      outPin = getInPin(i)->getOutPin();
      assert (outPin->getBusId() == _busIdList[i]);
   }
}

CktCell* 
CktMergeCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktMergeCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void
CktMergeCell::pinsDuplicate(CktCell*& newCell, string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktInPin*  oriInPin;
   CktOutPin* oriOutPin = getOutPin();
   //pin <--> cell : bi-direction connection within the function "pinDuplicate(...)"
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      oriInPin = getInPin(i);
      oriInPin->pinDuplicate(newCell, inPinMap); 

      assert (oriInPin->getOutPin() != 0);
      //In the local bus cell, must save the fanin pin busId info. which will
      //loss in the completeFlatten stage.
      (static_cast<CktMergeCell*>(newCell))->insertBusId(oriInPin->getOutPin()->getBusId());
   }
   oriOutPin->pinDuplicate(newCell, prefix, outPinMap);
}

// isReverse = true      isReverse = true
//        [2:0]  |----------|
//    ------/----|in0       |   [3:0]
//         [0]   |   Merge  |-----/-----
//    ------/----|in1       |
//               |----------|
void
CktMergeCell::writeBLIF() const//input pin
{
   stringstream outFile;
   int iWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int oWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   string iName = getInPin(0)->getOutPin()->getName();
   string oName = getOutPin()->getName();
   assert (iName != oName);
   int curMergeNum = 0;
   int curInBitNum = iWidth-1;
   bool isReverse = VLDesign.getBus(getOutPin()->getBusId())->isInverted();

   if (isReverse == false) // check the outpin isReverse
      cerr << "Warning : Bits reverse! Need to handle!! We doesn't finish(@CktMergeCell::writeBLIF())" << endl;

   isReverse = VLDesign.getBus(getInPin(curMergeNum)->getOutPin()->getBusId())->isInverted();
   if (isReverse == false) // check the outpin isReverse
      cerr << "Warning : Bits reverse! Need to handle!! We doesn't finish(@CktMergeCell::writeBLIF())" << endl;

   //for (int i = 0; i < oWidth; ++i) {   // pos connection   //Question : How to decide the connection phase??
   for (int i = oWidth-1; i >= 0; --i) {  // inv connection   // the same question as the SplitCell
      if (curInBitNum == -1) {
	 ++curMergeNum;
         iWidth      = VLDesign.getBus(getInPin(curMergeNum)->getOutPin()->getBusId())->getWidth();
         curInBitNum = iWidth-1; 
         iName       = getInPin(curMergeNum)->getOutPin()->getName();
         isReverse   = VLDesign.getBus(getInPin(curMergeNum)->getOutPin()->getBusId())->isInverted();
         assert (iName != oName);
         if (isReverse == false) // check every inpin isReverse
            cerr << "Warning : Bits reverse! Need to handle!! It doesn't finish(@CktMergeCell::writeBLIF())" << endl;
      }
      outFile << ".names " << iName;
      if (!getInPin(curMergeNum)->getOutPin()->is1BitIoPin()) outFile << "[" << curInBitNum << "]";
      
      outFile << " " << oName << "[" << i << "]";      
#ifdef BLIF_DEBUG_MODE
      outFile << " #merge ";
#endif
      outFile << endl;
      outFile << "1 1" << endl;
      --curInBitNum;
   }
   cktOutFile.insert(outFile.str());
}

// ========== CktInoutCell ==========

CktInoutCell::CktInoutCell(): CktCell( CKT_INOUT_CELL )
{
}

CktInoutCell::~CktInoutCell()
{
}

void
CktInoutCell::simulate()
{
}

// ========== CktMemoryCell ==========

CktMemCell::CktMemCell(string& n, CktOutPin* c, CktOutPin* w, CktOutPin* r): CktCell( CKT_MEMORY_CELL )
{
   _name = n;
   if (c != NULL) {
      _clk = new CktInPin(); _clk->connCell(this);     
      _clk->connOutPin(c); c->connInPin(_clk);
   }
   if (w != NULL) {
      _wrAddress = new CktInPin(); _wrAddress->connCell(this);
      _wrAddress->connOutPin(w); w->connInPin(_wrAddress);
   }
   if (r != NULL) {
      _rdAddress = new CktInPin(); _rdAddress->connCell(this);
      _rdAddress->connOutPin(r); r->connInPin(_rdAddress);
   }

   _inPinMap.clear();
   _outPinMap.clear();
}

CktMemCell::~CktMemCell()
{
}

void
CktMemCell::setSize(unsigned& b, int& s, int& e)
{
   _busId = b;
   _szS   = s;
   _szE   = e;
}

void
CktMemCell::setRdInfo(bool rv, bool rc)
{
   _isReadVariable = rv;
   _isReadConst    = rc;
}

void
CktMemCell::setWrInfo(bool wv, bool wc)
{
   _isWriteVariable = wv;
   _isWriteConst    = wc;
}

void
CktMemCell::setSync(bool sync)
{
   _isSync = sync;
}

void
CktMemCell::connRowOutPin(int no, CktOutPin* oPin)
{
   _outPinMap.insert(make_pair(no, oPin));
}

void
CktMemCell::connRowInPin(int no, CktInPin* iPin)
{
   _inPinMap.insert(make_pair(no, iPin));
}

void
CktMemCell::simulate()
{
}

#endif

