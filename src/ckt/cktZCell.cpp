/****************************************************************************
  FileName     [ cktZCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit Z cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_ZCELL_SOURCE
#define CKT_ZCELL_SOURCE

#include "VLGlobe.h"
#include "cktZCell.h"

#include "cktBLIFile.h"

#include "vlpItoStr.h"

extern SynOutFile      synOutFile;
extern CktOutFile      cktOutFile;
extern int             instNumber;
extern VlpDesign       VLDesign;
extern CKT_usage*      CREMap;

// ========== cktBufifCell ==========

CktBufifCell::CktBufifCell(): CktCell( CKT_BUFIF_CELL )
{
}

CktBufifCell::CktBufifCell(bool isNewPin, CktInPin*& iPin, CktInPin*& iCnd, CktOutPin*& oPin) 
            : CktCell(CKT_BUFIF_CELL)
{
   if (isNewPin) {
      iPin = new CktInPin;
      iCnd = new CktInPin;
      oPin = new CktOutPin;
   }
   //connect between inPin & cell
   iPin->connCell(this);
   connInPin(iPin);
   iCnd->connCell(this);
   connInPin(iCnd);
   //connect between cell & outPin
   oPin->connCell(this);
   connOutPin(oPin);
}

CktBufifCell::~CktBufifCell()
{
}

void
CktBufifCell::writeOutput() const
{
   stringstream input;
   string moduleName = "DVL_BUFIF1";
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   CktOutPin* bOutpin = getInPin(1)->getOutPin();
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
   input << genModuleInst("Y", yOutpin) << ", ";
   input << genModuleInst("A", aOutpin) << ", ";
   input << genModuleInst("S", bOutpin) << " );" << endl;
   
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void
CktBufifCell::simulate()
{
}

CktCell* 
CktBufifCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktBufifCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void
CktBufifCell::writeBLIF() const
{
/*
CktBufifCell is bufif1, that is , if condition is true, then propagate signal, otherwise, high impedence
               |-------|
        data---|0      |---output
               |  Bufif|
    condition--|1      |
               |-------|
*/
   stringstream outFile;
   int width1 = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int width2 = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   int oWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   assert (width2 == 1);
   assert (width1 == oWidth);
   string input1 = getInPin(0)->getOutPin()->getName();//data
   string input2 = getInPin(1)->getOutPin()->getName();//condition
   string output = getOutPin()->getName();
   for (int i = 0 ; i < width1; ++i) 
   {
      outFile << ".names "; 
      outFile << input1;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << input2;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[0]";
      outFile << " " << output;
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #buf-if ";
#endif
      outFile << endl << "11 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

// ========== CktInvifCell ==========

CktInvifCell::CktInvifCell():CktCell(CKT_INVIF_CELL)
{
}

CktInvifCell::~CktInvifCell()
{
}

void
CktInvifCell::simulate()
{
}

// ========== CktBusCell ==========

CktBusCell::CktBusCell():CktCell(CKT_BUS_CELL)
{
   _busIdList.clear();
}

CktBusCell::CktBusCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_BUS_CELL, isNewPin, iPin, oPin)
{
}

CktBusCell::~CktBusCell()
{
}

/*
eg.  A[3:2] = B;
     A[4:1] = C;

     A[3:2] -----|---------|
                 | busCell |---- A
     A[4:1] -----|---------|
  (will be renamed)
*/
void
CktBusCell::writeOutput() const
{
   string moduleName = "DVL_BUF/*bus*/";
   stringstream input;   
   CktOutPin* yOutpin = getOutPin();
   string yName = yOutpin->getName();
   assert (yName != "");
   //const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());

   if (!synOutFile.isExistWire(yName)) synOutFile.insertWireInst(genWireInst(yOutpin), yName);      

   CktOutPin*    aOutpin;
   string        aName;
   const SynBus* aBus;
   int   aWidth;
   for (unsigned i = 0; i < getInPinSize(); ++i){
      aOutpin = getInPin(i)->getOutPin();
      aName   = aOutpin->getName();
      assert (aName != "");
      aBus    = VLDesign.getBus(aOutpin->getBusId());
      aWidth  = aBus->getWidth();

      if (!synOutFile.isExistWire(aName)) synOutFile.insertWireInst(genWireInst(aOutpin), aName);
      
      input.str("");
      input << moduleName << " #( " << aWidth << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( ";
      input << genModuleInst("Y", yName, aBus, true) << ", ";
      input << genModuleInst("A", aOutpin) << " );" << endl;
      ++instNumber;
      synOutFile.insertModuleInst(input.str());
   }
}

void
CktBusCell::simulate()
{
   assert(isMergeCell() == true);
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0);
   for (unsigned i = 1; i < getInPinSize(); ++i) {
      fin0 = getInPin(i)->getOutPin();
      _simValue.append(fin0->getCell()->getSimValue(fin0));
   }
}

void
CktBusCell::insertBusId(unsigned short i)
{
   _busIdList.push_back(i);
}

unsigned 
CktBusCell::getBusId(unsigned i) const
{
   return _busIdList[i];
}

void 
CktBusCell::updateIoWidth() const
{
   CktOutPin* outPin;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      outPin = getInPin(i)->getOutPin();
      outPin->setBusId(getBusId(i));
   }
}

CktCell* 
CktBusCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktBusCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void
CktBusCell::pinsDuplicate(CktCell*& newCell, string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
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
      (static_cast<CktBusCell*>(newCell))->insertBusId(oriInPin->getOutPin()->getBusId());
   }
   oriOutPin->pinDuplicate(newCell, prefix, outPinMap);
}

// The flatten version
// use after completeFlatten()
void 
CktBusCell::writeBLIF() const
{
   typedef vector<string> StrAry;

   //cout << "no finish in CktBusCell::writeBLIF()" << endl;
   //for (int i = 0; i < getInPinSize(); ++i)
   //   cout << i << " : " << getFinCell(i)->getCellType() << endl;   
   stringstream outFile;
   //int iWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   CktOutPin *aOutpin;

   unsigned oWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   string tempInName;
   string iName;
   string oName = getOutPin()->getName();
   int begin, end;
   bool isReverse = VLDesign.getBus(getOutPin()->getBusId())->isInverted();
   StrAry* strAryPtr = new StrAry[oWidth];

   if (isReverse == false) // check the outpin isReverse
      cerr << "Warning : Bits reverse! Need to handle!! It doesn't finish(@CktBusCell::writeBLIF())" << endl;
//pre-process
   for (unsigned i = 0; i < getInPinSize(); ++i)
   {
      //known bug
      //            B --+---+
      //                |bus|---- A
      //         Null --+---+
      assert (getInPin(i)->getOutPin() != 0);
      if (getInPin(i)->getOutPin() != 0) {
         aOutpin = getInPin(i)->getOutPin();
         tempInName = getInPin(i)->getOutPin()->getName();
         begin     = VLDesign.getBus(_busIdList[i])->getBegin();
         end       = VLDesign.getBus(_busIdList[i])->getEnd();
         isReverse = VLDesign.getBus(_busIdList[i])->isInverted();

         if (isReverse == false) // check every inpin isReverse
            cerr << "Warning : Bits reverse! Need to handle!! It doesn't finish(@CktBusCell::writeBLIF())" << endl;

         if (tempInName != oName) {
            for (int j = begin; j <= end; ++ j) {
               if ( getFinCell(i)->getCellType() == CKT_PI_CELL && begin == end )
                  iName = tempInName;
               else
                  iName = tempInName + "[" + toString(j-begin) + "]";
               strAryPtr[j].push_back(iName);
               //iName = "";
            }
         }
      }
   }
//write blif
   for (unsigned i = 0; i < oWidth; ++i) {
      //if (strAryPtr[i].size() > 0) {
         outFile << ".names ";
         for (unsigned j = 0; j < strAryPtr[i].size(); ++j) 
            outFile << strAryPtr[i][j] << " ";
      
         outFile << oName;
         if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
         outFile << " #bus ";
#endif
         outFile << endl;
         for (unsigned j = 0; j < strAryPtr[i].size(); ++j)
            outFile << "1";
         outFile << " 1" << endl;
      //}
   }
   delete [] strAryPtr;
   cktOutFile.insert(outFile.str());
}
   
bool 
CktBusCell::isMergeCell() const
{
   pair<int, int> intPair;
   vector<pair<int,int> > intPairArray;
   vector<pair<int,int> > :: const_iterator pos;
   const SynBus* bus;
   //string str = getInPin(0)->getOutPin()->getName();

   for (unsigned i = 0; i < getInPinSize(); ++i) {
      //assert (str == (getInPin(i)->getOutPin()->getName()));
      bus = VLDesign.getBus(getInPin(i)->getOutPin()->getBusId());
      if (bus->getBegin() < bus->getEnd()) {
         intPair.first  = bus->getBegin();
         intPair.second = bus->getEnd();
      }
      else {
         intPair.first  = bus->getEnd();
         intPair.second = bus->getBegin();
      }
      intPairArray.push_back(intPair);
   }
   sort (intPairArray.begin(), intPairArray.end());

   for (pos = intPairArray.begin(); pos != intPairArray.end(); ++pos) {
      if ((pos+1) != intPairArray.end()) 
         if (pos->second >= (pos+1)->first)
            return false;
   }
   return true;
}

void
CktBusCell::transToMergeCell(CktCell*& NewCell)
{
   assert (this == NewCell);
   if (isMergeCell()) {
      NewCell = new CktMergeCell;
      NewCell->connOutPin(getOutPin());
      (const_cast<CktOutPin*>(getOutPin()))->connCell(NewCell);

      const SynBus* bus;
      pair<int, CktInPin*> matchPair;
      vector<pair<int, CktInPin*> > matchPairArray;

      for (unsigned i = 0; i < getInPinSize(); ++i) {
         bus = VLDesign.getBus(getInPin(i)->getOutPin()->getBusId());
         assert (bus->isInverted() == true);//false need additional handle.
         matchPair.first  = bus->getEnd();   //false part not finish
         matchPair.second = getInPin(i);
         matchPairArray.push_back(matchPair);
      }             
      sort (matchPairArray.begin(), matchPairArray.end());

      for (int i = matchPairArray.size()-1; i >= 0; --i) {
         NewCell->connInPin(matchPairArray[i].second);
         (const_cast<CktInPin*>(matchPairArray[i].second))->connCell(NewCell);
      }        
     
      delete this;
   }
}
   
void 
CktBusCell::checkIoWidth() const
{
   CktOutPin* outPin = getOutPin();
   const SynBus* bus = VLDesign.getBus(outPin->getBusId());
   cout << "Bus Cell name = " << getOutPin()->getName();
   cout << " begin = " << bus->getBegin() << " end = " << bus->getEnd() << endl;
   cout << "===fanin Cell===" << endl;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      outPin = getInPin(i)->getOutPin();
      bus = VLDesign.getBus(outPin->getBusId());
      cout << "   No " << i 
           << " cellType = " << getFinCell(i)->getCellType() 
           << " name = "     << outPin->getName()
           << " begin = "    << bus->getBegin() 
           << " end = "      << bus->getEnd() 
           << endl;
   }
}

// The original version
// use before completeFlatten()
/*
void 
CktBusCell::writeBLIF() const
{
   typedef vector<string> StrAry;

   //cout << "no finish in CktBusCell::writeBLIF()" << endl;
   //for (int i = 0; i < getInPinSize(); ++i)
   //   cout << i << " : " << getFinCell(i)->getCellType() << endl;   
   stringstream outFile;
   //int iWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   CktOutPin *yOutpin = getOutPin();
   CktOutPin *aOutpin;

   int oWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   string tempInName;
   string iName;
   string oName = getOutPin()->getName();
   int totalBusNum = getInPinSize();
   int curMergeNum = 0;
   int begin, end, width;
   bool isReverse = VLDesign.getBus(getOutPin()->getBusId())->isInverted();

   solveBusWidth( yOutpin, oName, width, begin, end, isReverse, yOutpin );
   cout << "1. begin, end = " << begin << ", " << end << endl;

   StrAry* strAryPtr = new StrAry[oWidth];

   if (isReverse == false) // check the outpin isReverse
      cerr << "Warning : Bits reverse! Need to handle!! It doesn't finish(@CktBusCell::writeBLIF())" << endl;
//pre-process
   cout << "totalBusNum = " << totalBusNum << endl;
   cout << "oWidth = " << oWidth << endl;
   cout << "oName = " << oName << endl;
   for (int i = 0; i < totalBusNum; ++i)
   {
      //known bug
      //            B --+---+
      //                |bus|---- A
      //         Null --+---+
      if (getInPin(i)->getOutPin() != 0) {
         aOutpin = getInPin(i)->getOutPin();
         tempInName = getInPin(i)->getOutPin()->getName();
         begin     = VLDesign.getBus(getInPin(i)->getOutPin()->getBusId())->getBegin();
         end       = VLDesign.getBus(getInPin(i)->getOutPin()->getBusId())->getEnd();
         isReverse = VLDesign.getBus(getInPin(i)->getOutPin()->getBusId())->isInverted();


         if (isReverse == false) // check every inpin isReverse
            cerr << "Warning : Bits reverse! Need to handle!! It doesn't finish(@CktBusCell::writeBLIF())" << endl;
         cout << "2. finCell type = " << aOutpin->getCell()->getCellType() << endl;
         cout << "2. finCell type = " << aOutpin->getName() << endl;
         cout << "2. begin ,end = " << begin << ", " << end << endl;
         solveBusWidth( aOutpin, iName, width, begin, end, isReverse, yOutpin );
         cout << "3. begin ,end = " << begin << ", " << end << endl;

         for (int j = begin; j <= end; ++ j) {
            if ( getFinCell(i)->getCellType() == CKT_PI_CELL && begin == end )
               iName = tempInName;
            else
               iName = tempInName + "[" + toString(j-begin) + "]";

            strAryPtr[j].push_back(iName);
            //iName = "";
         }
      }
   }
//write blif
   for (int i = 0; i < oWidth; ++i) {
      if (strAryPtr[i].size() > 0) {
         outFile << ".names ";
         for (int j = 0; j < strAryPtr[i].size(); ++j) 
            outFile << strAryPtr[i][j] << " ";
      
         outFile << oName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
         outFile << " #bus ";
#endif
         outFile << endl;
         for (int j = 0; j < strAryPtr[i].size(); ++j)
            outFile << "1";

         outFile << " 1" << endl;
      }
   }
   delete [] strAryPtr;
   cktOutFile.insert(outFile.str());
}
*/

/*
void 
CktBusCell::writeBLIF() const
{
   QuteString fromName, toName;

   CktOutPin *yOutpin,
             *aOutpin;

   int        i;

   string     yName;
   SynBus    *yBus;
   int        yWidth,
              yBegin,
              yEnd,
              yBusIdBackUp = -1;

   bool       yInverted;

   string  aName;
   SynBus* aBus;
   int     aWidth,
           aBegin,
           aEnd,
           aBusIdBackUp = -1;

   bool    aInverted;

   string       moduleName;
   stringstream input;

   int     aTmp;
   int     aCtr;

   yOutpin = getOutPin();
   yName   = yOutpin->getName();
   yBus    = VLDesign.getBus(yOutpin->getBusId());
   yWidth  = yBus->getWidth();

   solveBusWidth( yOutpin, yName, yWidth, yBegin, yEnd, yInverted, yOutpin );

   for ( i = 0; i < getInPinSize(); i++ )
   {
      aOutpin = getInPin( i )->getOutPin();
      aName   = aOutpin->getName();
      aBus    = VLDesign.getBus(aOutpin->getBusId());
      aWidth  = aBus->getWidth();

      solveBusWidth( aOutpin, aName, aWidth, aBegin, aEnd, aInverted, yOutpin );

      if ( aWidth == yWidth )
      {
         aTmp = aBegin;
         for ( int l = yBegin; l <= yEnd; l++ ) {
            fromName = prefix + "_" + aName + "[" + aTmp + "]";
            toName   = prefix + "_" + yName + "[" + l + "]";

  
            input << ".names " << fromName << " " << toName << " #bus1" << endl;
            input << "1 1" << endl;


            if ( aInverted == false )
               ++aTmp;
            else
               --aTmp;
         }
      }
      else
      {
         if ( yInverted != aInverted )
         {
            aTmp = aBegin;
            for ( aCtr = 0; aCtr < aWidth; ++aCtr )
            {
               fromName = prefix + "_" + aName + "[" + aTmp + "]";
               toName   = prefix + "_" + yName + "[" + aTmp + "]";

               input << ".names " << fromName << " " << toName << " #bus2 " << endl;
               input << "1 1" << endl;

               if ( aInverted == false )
                  ++aTmp;
               else
                  --aTmp;
            }
         }
         else
         { 
            if ( aInverted ) {
               aTmp = aBegin; 
               aBegin = aEnd;  
               aEnd = aTmp;
            }
            for ( int l = aBegin; l <= aEnd; l++ ) {
               if ( aWidth == 1 )
                  fromName = prefix + "_" + aName + "[0]";
               else
                  fromName = prefix + "_" + aName + "[" + l + "]";

               toName   = prefix + "_" + yName + "[" + l + "]";

               input << ".names " << fromName << " " << toName << " #bus3" << endl;
               input << "1 1" << endl;
            }
         }
      }
   }
   cktOutFile.insert(input.str());
}*/


// ========== CktZ2XCell ==========

#endif

