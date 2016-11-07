/****************************************************************************
  FileName     [ cktIoCell.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit I/O cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_IO_CELL_SOURCE
#define CKT_IO_CELL_SOURCE

#include "cktConnect.h"
#include "cktIoCell.h"
#include "cktBLIFile.h"
#include "vlpDesign.h"

extern CktOutFile cktOutFile;
extern SynOutFile synOutFile;
extern CktHierIoMap hierIoMap;
extern VlpDesign& VLDesign;


//=================================== CktPiCell =======================================//

CktPiCell::CktPiCell(): CktCell(CKT_PI_CELL)
{
}

CktPiCell::CktPiCell(bool isNewPin, CktOutPin*& oPin) : CktCell(CKT_PI_CELL)
{
   if (isNewPin) 
      oPin = new CktOutPin;
   //connect between cell & outPin
   oPin->connCell(this);
   connOutPin(oPin);
}

CktPiCell::~CktPiCell()
{
}

void
CktPiCell::writeOutput() const
{
   return;
}

void
CktPiCell::simulate()
{
   // do nothing, for overload usage
}
   
void 
CktPiCell::resetPI()
{
   const SynBus* bus = VLDesign.getBus(getOutPin()->getBusId());
   unsigned bits = bus->getWidth();
   stringstream ss;
   ss << bits << "'b";
   for (unsigned i = 0; i < bits; ++i)
      ss << 'x';

   _simValue = ss.str();
}

void 
CktPiCell::updateHierConn(string& prefix, InPinMap& inPinMap) const //for non-topNodule
{
   string hierName = prefix + "_" + (getOutPin()->getName());   
   CktCell* foutCell;
   IOPinConn* ioPinConnPtr;

   //cout << "PI hierName = " << hierName << endl;
   if (hierIoMap.find(hierName) == hierIoMap.end()) {
      ioPinConnPtr = new IOPinConn(0);
      hierIoMap.insert(make_pair(hierName, ioPinConnPtr));
   }
   else
      ioPinConnPtr = hierIoMap.find(hierName)->second;

   for (unsigned i = 0; i < getOutPin()->getInPinSize(); ++i) {
      foutCell = getOutPin()->getFoutCell(i);
      //cout << "foutCell type = " << foutCell->getCellType() << endl;
      if (foutCell->getCellType() == CKT_MODULE_CELL)  //PI --> M
         ;//handle in CkeModuleCell
      else if (foutCell->getCellType() == CKT_PO_CELL)  // PI --> PO
         cerr << "error : Should have buf between PI and PO @CktPiCell::updateHierConn()" << endl;
      else {  //PI --> C
         //cout << "======================CktPiCell=======================" << endl;
         //cout << "PI Name = " << getOutPin()->getName() << endl;
         //cout << "FO Name = " << getOutPin()->getInPin(i)->getOutPin()->getName() << endl;
         //cout << "FO type = " << foutCell->getCellType() << endl;

         // know bug
         //           |---- b         maybe c doesn't exist in cellList, and c is redundant
         //    a -----|               trace PI -> PO will have problem. but
         //           |---- c         trace PO -> PI will be Ok. This is a bug.
         if (inPinMap.find(getOutPin()->getInPin(i)) != inPinMap.end())//check the cell if redundant
            ioPinConnPtr->insert(inPinMap[getOutPin()->getInPin(i)]);
      }
   }
}

CktCell* 
CktPiCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktPiCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktPiCell::writeBLIF_PI() const
{
   stringstream outFile;
   //cout << "CktPiCell InPinSize = " << getInPinSize() << endl;
   //assert(getInPinSize() == 1);
   int width = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   string output = getOutPin()->getName();
   for (int i  = 0; i < width; ++i ) {
      if (width == 1)
         outFile << output << " ";
      else
         outFile << output << "[" << i << "] ";
      if ((i%5) == 4)      
         outFile << "\\" << endl;
   }
   cktOutFile.insert(outFile.str());
}

//=================================== CktPoCell =======================================//
CktPoCell::CktPoCell(): CktCell(CKT_PO_CELL)
{
}

CktPoCell::CktPoCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
         : CktCell(CKT_PO_CELL, isNewPin, iPin, oPin)
{
}

CktPoCell::~CktPoCell()
{
}

// Ex:
// output [7:0] bout;
// GF16to256_Affine GF16to256_Affine ( .En_out(bout[7:0]), .gin(k0[7:0]) );
// in the case, we need to write out DVL_BUF for CktPoCell
// in others, don't write out DVL_BUF
void
CktPoCell::writeOutput() const
{
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   string     yName   = yOutpin->getName();
   string     aName   = aOutpin->getName();
   if (yName != aName) // write out DVL_BUF
      genOutputOneOprd();
   else {
      if (aName != "") {
         if (!synOutFile.isExistWire(yName))
            synOutFile.insertWireInst(genWireInst(yOutpin), yName);
         if (!synOutFile.isExistWire(aName))
            synOutFile.insertWireInst(genWireInst(aOutpin), aName);
      }
   }
}

void
CktPoCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0);
}

void
CktPoCell::updateHierConn(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const//for non-topModule usage
{
   CktCell* finCell = getInPin(0)->getFinCell();
   CktCellType finCellType = finCell->getCellType();
   string hierName; 
   string hierName2;
   IOPinConn* ioPinConnPtr = NULL;
   CktOutPin* finOut = NULL;

   if (finCellType != CKT_MODULE_CELL && finCellType != CKT_PI_CELL) {
      if (finCellType != CKT_SPLIT_CELL) 
         finOut = finCell->getOutPin();
      else {
         CktSplitCell* spCell = static_cast<CktSplitCell*>(finCell);
         for (unsigned i = 0; i < spCell->getOutPinSize(); ++i)
            if (spCell->getOutPin(i) == getInPin(0)->getOutPin()) {
               finOut = spCell->getOutPin(i); 
               break;
            }
      }
      
      hierName = prefix + "_" + (finOut->getName());
      hierName2 = prefix + "_" + (getOutPin()->getName());
      assert (outPinMap.find(finOut) != outPinMap.end());

      if (hierIoMap.find(hierName) == hierIoMap.end()) {
         ioPinConnPtr = new IOPinConn(outPinMap[finOut]);
         hierIoMap.insert(make_pair(hierName, ioPinConnPtr));
      }
      else {
         ioPinConnPtr = hierIoMap.find(hierName)->second;
         assert (ioPinConnPtr->getOutPin() == 0);
         ioPinConnPtr->setOutPin(outPinMap[finOut]);
      }
   }
   if (hierName != hierName2)
      ioPinConnPtr->insert(hierName2);

   assert (getOutPin()->getInPinSize() == 0);

/* // jojoman have change the connect, so never have the connection "PO->C"
   //PO->C
   CktInPin* inPin;
   CktCellType foutCellType;
   for (int j = 0; j < getOutPin()->getInPinSize(); ++j) {

      inPin = getOutPin()->getInPin(j);//don't eliminate bufs
         //It's very hard to handle the elimination of bufs. In the worst case,
         //there can be many buf-type finouts recurrencely, like buf-tree
         / *while (true) { // find the fanout of the non-buf cell
            if (outPin->getFoutCell(j)->getCellType() != CKT_BUF_CELL) {
               inPin = outPin->getInPin(j);
               break;
            }
            else // skip bufCell
               outPin = outPin->getFoutCell(j)->getOutPin();
         }* / 

      foutCellType = inPin->getCell()->getCellType();

      if (foutCellType == CKT_MODULE_CELL)
         ;
      else if (foutCellType == CKT_PO_CELL) { // PO --> PO (consider if topModule)
         cerr << "warning : XXXXXXXXXXXXXXXXXX" << endl;
         / *if (isTopModule) {
            assert (inPinMap.find(inPin) != inPinMap.end());//should exist
            ioPinConnPtr->insert(inPinMap[inPin]);
         }
         else {
            foutName = prefix + "_" + (inPin->getCell()->getOutPin()->getName());
            ioPinConnPtr->insert(foutName);
         }* /
      }
      else if (foutCellType == CKT_PI_CELL || foutCellType == CKT_PIO_CELL)
         cerr << "Error : CKT_PI_CELL or CKT_PIO_CELL @CktModuleCell::updateHierConn(...)" << endl;
      else {
         // known bug in syn
         //           |---- b         maybe c doesn't exist in cellList
         //    a -----|               trace PI -> PO will have problem. but
         //           |---- c         trace PO -> PI will be Ok. This is a bug.
         //assert (inPinMap.find(inPin) != inPinMap.end());//should exist
         //ioPinConnPtr->insert(inPinMap[inPin]);
         if (inPinMap.find(inPin) != inPinMap.end())//check loss cell avoid bug
            ioPinConnPtr->insert(inPinMap[inPin]);
      }
   }*/
}

CktCell* 
CktPoCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktPoCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktPoCell::writeBLIF_PO() const
{
   stringstream outFile;
   assert(getInPinSize() == 1);
   int width = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   //string output = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();

   for (int i  = 0; i < width; ++i ) {
      if (width == 1)
         outFile << output << " ";
      else
         outFile << output << "[" << i << "] ";
      if ((i%5) == 4)
         outFile << "\\" << endl;
   }
   
   cktOutFile.insert(outFile.str());
}

void 
CktPoCell::writeBLIF() const
{
   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();

   if (input1 != output) {
      stringstream outFile;
      int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
      //int msb = VLDesign.getBus(getOutPin()->getBusId())->getBegin();
      //int lsb = VLDesign.getBus(getOutPin()->getBusId())->getEnd();
      //bool tf = VLDesign.getBus(getOutPin()->getBusId())->isInverted();
      for (int i = 0; i < width1; ++i) {
         outFile << ".names " << input1; 
         if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
         outFile << " " << output;
         if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
         outFile << " #po ";
#endif
         outFile << endl << "1 1" << endl;
      }
      cktOutFile.insert(outFile.str());
   }
}

//================================== CktPioCell =======================================//
CktPioCell::CktPioCell(): CktCell(CKT_PIO_CELL)
{
}

CktPioCell::CktPioCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_PIO_CELL, isNewPin, iPin, oPin)
{
}

CktPioCell::~CktPioCell()
{
}

void
CktPioCell::writeOutput() const
{
   genOutputOneOprd();
}

/*void
CktPioCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0);
}*/

CktCell* 
CktPioCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktPioCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktPioCell::updateHierConn(string&, OutPinMap&, InPinMap&) const
{
   cerr << "Still not finish the function \"CktPioCell::updateHierConn\"" << endl;
   exit(0);
}

#endif

