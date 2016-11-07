/****************************************************************************
  FileName     [ cktLogicCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit logic cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_LOGIC_CELL_SOURCE
#define CKT_LOGIC_CELL_SOURCE

#include "VLGlobe.h"
#include "cktLogicCell.h"
#include "cktBLIFile.h"
#include <queue>

extern SynOutFile      synOutFile;
extern CktOutFile      cktOutFile;
extern int             instNumber;
extern VlpDesign& VLDesign;
extern CKT_usage* CREMap;

// used in Xor and Xnor WriteBLIF
void
genPattern(stringstream& outFile, string prefix, int width, int ones)
{
   assert (width <= ones);
   if (width == 1) {
      if (ones == 1)
         outFile << prefix << "1 1" << endl;
      else
         outFile << prefix << "0 1" << endl;
   }
   else {
      if (ones > 0)
         genPattern(outFile, prefix + "1", width - 1, ones - 1);
      genPattern(outFile, prefix + "0", width - 1, ones);
   }
}

//======================================= CktBufCell ==========================================//
CktBufCell::CktBufCell(): CktCell( CKT_BUF_CELL )
{
}

CktBufCell::CktBufCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_BUF_CELL, isNewPin, iPin, oPin)
{
}

CktBufCell::~CktBufCell() 
{
}

string
CktBufCell::getExpr(CktOutPin* p) const {
   assert (p == getOutPin());
   assert (getInPin(0));
   return getInPin(0)->getOutPin()->getCell()->getExpr(getInPin(0)->getOutPin());
}

void
CktBufCell::writeOutput() const
{
   genOutputOneOprd();
}

void
CktBufCell::simulate()
{
   assert (getInPinSize() == 1);
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0);
}

CktCell* 
CktBufCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktBufCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktBufCell::writeBLIF() const
{
   //cout << "==================BUF====================(CktBufCell :: writeBLIF() )" << endl;
   stringstream outFile;
   int iWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int oWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   //assert (oWidth == iWidth);
   if (oWidth != iWidth)
      cout << "Warning : unmatch # bit buf !! (iWidth, oWidth) = (" << iWidth << ", " << oWidth << ")" << endl;

   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();
   for (int i = 0; i < oWidth; ++i) {
      outFile << ".names " << input1;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << output;
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #buf ";
#endif
      outFile << endl << "1 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

bool 
CktBufCell::eliminate()
{
   CktOutPin* finOutPin = getInPin(0)->getOutPin();
   CktCellType finType  = finOutPin->getCell()->getCellType();
   CktCellType foutType;
   for (unsigned i = 0; i < getOutPin()->getInPinSize(); ++i) {
      foutType = getOutPin()->getFoutCell(i)->getCellType();
      if (foutType == CKT_DFF_CELL || foutType == CKT_PO_CELL)
         return false;
   }
   const SynBus* aBus = VLDesign.getBus(finOutPin->getBusId());
   const SynBus* yBus = VLDesign.getBus(getOutPin()->getBusId());
   if (aBus->getWidth() != yBus->getWidth()) { // width different => change bus Id
      cout << "Warning : Different widths between I/O of CktBufCell !" << endl
           << "Warning : Change Input Bus to Output Bus to eliminate Buf" << endl;
      finOutPin->setBusId(getOutPin()->getBusId());
   } 

   if (finType != CKT_DFF_CELL) {
      CktInPin* foutInPin;
      (const_cast<CktOutPin*>(finOutPin))->erase(getInPin(0));
      for (unsigned i = 0; i < getOutPin()->getInPinSize(); ++i) {
         foutInPin = getOutPin()->getInPin(i);
         (const_cast<CktOutPin*>(finOutPin))->connInPin(foutInPin);
         foutInPin->connOutPin(finOutPin);
      }
      delete this; // will also delete inPins and the outPin of the cell
      return true;
   }
   else 
      return false;
}

bool
CktBufCell::forceEliminate()
{
   CktOutPin* finOutPin = getInPin(0)->getOutPin();
   CktInPin* foutInPin;
   for (unsigned i = 0; i < getOutPin()->getInPinSize(); ++i) {
      if (getOutPin()->getFoutCell(i)->getCellType() == CKT_PO_CELL) return false;
   }
   (const_cast<CktOutPin*>(finOutPin))->erase(getInPin(0));
   for (unsigned i = 0; i < getOutPin()->getInPinSize(); ++i) {
      foutInPin = getOutPin()->getInPin(i);
      (const_cast<CktOutPin*>(finOutPin))->connInPin(foutInPin);
      foutInPin->connOutPin(finOutPin);
   }
   return true;
}

//======================================= CktInvCell ==========================================//
CktInvCell::CktInvCell() : CktCell(CKT_INV_CELL)
{
}

CktInvCell::CktInvCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_INV_CELL, isNewPin, iPin, oPin)
{
}

CktInvCell::~CktInvCell()
{
}

void
CktInvCell::writeOutput() const
{
   genOutputOneOprd();
}

void
CktInvCell::simulate()
{
   assert (getInPinSize() == 1);
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = ~(fin0->getCell()->getSimValue(fin0));
}

CktCell* 
CktInvCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktInvCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktInvCell::writeBLIF() const //only bitwise inv....Log_Inv is implemented by the GREATER cell
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int oWidth =  VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   assert (oWidth == width1);

   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();
   for (int i = 0; i < oWidth; ++i) {
      outFile << ".names " << input1; 
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]"; 
      outFile << " " << output; 
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #inv ";
#endif
      outFile << endl << "0 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

//======================================= CktAndCell ==========================================//
CktAndCell::CktAndCell(): CktCell( CKT_AND_CELL )
{
}

CktAndCell::CktAndCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_AND_CELL, isNewPin, iPin, oPin)
{
}

CktAndCell::CktAndCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_AND_CELL, isNewPin, iA, iB, oY)
{
}

CktAndCell::~CktAndCell()
{
}

CktOpType
CktAndCell::getType() const
{
   if ( getInPinSize() == 2 )
   {
      if ( ( VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth() == 1 )
        && ( VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth() == 1 ) )
         return CKT_LOG;
      else
         return CKT_BW;
   }
   else
      return CKT_RED;
}

void
CktAndCell::writeOutput() const
{
   if (getType() == CKT_LOG)
      genLogicOutputTwoOprd();
   else
      genOutput();
}

void
CktAndCell::simulate()
{
   if (getType() == CKT_BW | getType() == CKT_LOG) {
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      const CktOutPin* fin1 = getInPin(1)->getOutPin();
      _simValue = fin0->getCell()->getSimValue(fin0) & fin1->getCell()->getSimValue(fin1);
   }
   else {
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      _simValue = fin0->getCell()->getSimValue(fin0).reducedAnd();
   }
}

CktCell* 
CktAndCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktAndCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktAndCell::writeBLIF() const
{
   if (getType() == CKT_LOG) 
      writeLogBLIF();
   else if (getType() == CKT_BW) 
      writeBwBLIF();
   else 
      writeRedBLIF();
}

void
CktAndCell::writeLogBLIF() const
{
   stringstream outFile;
   outFile << ".names "; 
   outFile << getInPin(0)->getOutPin()->getName(); 
   if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[0]";
   outFile << " " << getInPin(1)->getOutPin()->getName(); 
   if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[0]";
   outFile << " " << getOutPin()->getName(); 
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #log-and ";
#endif
   outFile << endl << "11 1" << endl;
   cktOutFile.insert(outFile.str());
}

void
CktAndCell::writeBwBLIF() const
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int width2 =  VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   int oWidth =  VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   assert (width1 == width2);
   assert (oWidth == width1);

   string input1 = getInPin(0)->getOutPin()->getName();
   string input2 = getInPin(1)->getOutPin()->getName();
   string output = getOutPin()->getName();
   for (int i = 0 ; i < width1; ++i) {
      outFile << ".names "; 
      outFile << input1; 
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << input2; 
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << output;
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #bw-and ";
#endif
      outFile << endl << "11 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

void
CktAndCell::writeRedBLIF() const
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();

   outFile << ".names ";

   for (int i = 0; i < width1; ++i) 
      outFile << input1 << "[" << i << "] ";

   outFile << output;
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #red-and ";
#endif
   outFile << endl;
   for (int i = 0; i < width1; ++i) 
      outFile << "1";

   outFile << " 1" << endl;
   cktOutFile.insert(outFile.str());
}

//======================================= CktOrCell ===========================================//
CktOrCell::CktOrCell(): CktCell( CKT_OR_CELL )
{
}

CktOrCell::CktOrCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
         : CktCell(CKT_OR_CELL, isNewPin, iPin, oPin)
{
}

CktOrCell::CktOrCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
         : CktCell(CKT_OR_CELL, isNewPin, iA, iB, oY)
{
}

CktOrCell::~CktOrCell()
{
}

CktOpType
CktOrCell::getType() const
{
   if ( getInPinSize() == 2 )
   {
      if ( ( VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth() == 1 )
        && ( VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth() == 1 ) )
         return CKT_LOG;
      else
         return CKT_BW;
   }
   else
      return CKT_RED;
}

void
CktOrCell::writeOutput() const
{
   if (getType() == CKT_LOG)
      genLogicOutputTwoOprd();
   else
      genOutput();
}

void
CktOrCell::simulate()
{
   if (getType() == CKT_BW | getType() == CKT_LOG) { 
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      const CktOutPin* fin1 = getInPin(1)->getOutPin();
      _simValue = fin0->getCell()->getSimValue(fin0) | fin1->getCell()->getSimValue(fin1);
   }
   else {
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      _simValue = fin0->getCell()->getSimValue(fin0).reducedOr();
   }
}

CktCell* 
CktOrCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktOrCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktOrCell::writeBLIF() const
{
   if (getType() == CKT_LOG) 
      writeLogBLIF();
   else if (getType() == CKT_BW) 
      writeBwBLIF();
   else 
      writeRedBLIF();
}

void
CktOrCell::writeLogBLIF() const
{
   stringstream outFile;
   outFile << ".names ";
   outFile << getInPin(0)->getOutPin()->getName(); 
   if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[0]";
   outFile << " " << getInPin(1)->getOutPin()->getName(); 
   if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[0]";
   outFile << " " << getOutPin()->getName();
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #log-or ";
#endif
   outFile << endl << "1- 1" << endl << "-1 1" << endl;
   cktOutFile.insert(outFile.str());
}

void
CktOrCell::writeBwBLIF() const
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int width2 =  VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   int oWidth =  VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   assert (width1 == width2);
   assert (oWidth == width2);
   string input1 = getInPin(0)->getOutPin()->getName();
   string input2 = getInPin(1)->getOutPin()->getName();
   string output = getOutPin()->getName();
   for (int i = 0 ; i < width1; ++i) {
      outFile << ".names " << input1; 
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << input2; 
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << output; 
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #bw-or ";
#endif
      outFile << endl;
      outFile << "1- 1" << endl;
      outFile << "-1 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

void
CktOrCell::writeRedBLIF() const
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();
   outFile << ".names ";   
   for (int i = 0; i < width1; ++i) 
      outFile << input1 << "[" << i << "] ";
   
   outFile << output;
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #red-or ";
#endif
   outFile << endl;

   for (int i = 0; i < width1; ++i) {
      for (int j = 0; j < width1; ++j) {
         if (i != j)
            outFile << "-";
         else
            outFile << "1";
      }
      outFile << " 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

//====================================== CktNandCell ==========================================//
CktNandCell::CktNandCell(): CktCell( CKT_NAND_CELL )
{
}

CktNandCell::CktNandCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
           : CktCell(CKT_NAND_CELL, isNewPin, iPin, oPin)
{
}

CktNandCell::CktNandCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
           : CktCell(CKT_NAND_CELL, isNewPin, iA, iB, oY)
{
}

CktNandCell::~CktNandCell()
{
}

CktOpType
CktNandCell::getType() const
{
   if ( getInPinSize() == 2 )
   {
      if ( ( VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth() == 1 )
        && ( VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth() == 1 ) )
         return CKT_LOG;
      else
         return CKT_BW;
   }
   else
      return CKT_RED;
}

void
CktNandCell::writeOutput() const
{
   genOutput();
}

void
CktNandCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0).reducedNand();
}

CktCell* 
CktNandCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktNandCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktNandCell::writeBLIF() const //only reduced nand
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();
  
   outFile << ".names "; 
   for (int i = 0; i < width1; ++i) {
      outFile << input1;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " ";
   }  
   outFile << output; 
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #red-nand ";
#endif
   outFile << endl;

   for (int i = 0; i < width1; ++i) {
      for (int j = 0; j < width1; ++j) {
         if (i != j)
            outFile << "-";
         else
            outFile << "0";
      }
      outFile << " 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

//======================================= CktNorCell ==========================================//
CktNorCell::CktNorCell(): CktCell( CKT_NOR_CELL )
{
}

CktNorCell::CktNorCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_NOR_CELL, isNewPin, iPin, oPin)
{
}

CktNorCell::CktNorCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_NOR_CELL, isNewPin, iA, iB, oY)
{
}

CktNorCell::~CktNorCell()
{
}

CktOpType
CktNorCell::getType() const
{
   if ( getInPinSize() == 2 )
   {
      if ( ( VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth() == 1 )
        && ( VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth() == 1 ) )
         return CKT_LOG;
      else
         return CKT_BW;
   }
   else
      return CKT_RED;
}

void
CktNorCell::writeOutput() const
{
   genOutput();
}

void
CktNorCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0).reducedNor();
}

CktCell* 
CktNorCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktNorCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktNorCell::writeBLIF() const //only reduced nor
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();
   outFile << ".names "; 
   for (int i = 0; i < width1; ++i) {
      outFile << input1;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " ";
   }  
   outFile << output; 
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #red-nor ";
#endif
   outFile << endl;

   for (int i = 0; i < width1; ++i) 
      outFile << "0";

   outFile << " 1" << endl;
   cktOutFile.insert(outFile.str());
}

//======================================= CktXorCell ==========================================//
CktXorCell::CktXorCell(): CktCell( CKT_XOR_CELL )
{
}

CktXorCell::CktXorCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
          : CktCell(CKT_XOR_CELL, isNewPin, iPin, oPin)
{
}

CktXorCell::CktXorCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_XOR_CELL, isNewPin, iA, iB, oY)
{
}

CktXorCell::~CktXorCell()
{
}

CktOpType
CktXorCell::getType() const
{
   if (getInPinSize() > 1)
      return CKT_BW;
   else
      return CKT_RED;
}

void
CktXorCell::writeOutput() const
{
   genOutput();
}

void
CktXorCell::simulate()
{
   if (getType() == CKT_BW) { 
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      const CktOutPin* fin1 = getInPin(1)->getOutPin();
      _simValue = fin0->getCell()->getSimValue(fin0) ^ fin1->getCell()->getSimValue(fin1);
   }
   else {
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      _simValue = fin0->getCell()->getSimValue(fin0).reducedXor();
   }
}

CktCell* 
CktXorCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktXorCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktXorCell::writeBLIF() const
{
   // No log xor in Verilog operator, the same as BW-Xor
   if (getType() == CKT_BW) 
      writeBwBLIF();
   else //if (getType() == CKT_RED)
      writeRedBLIF();
}

void
CktXorCell::writeBwBLIF() const
{
   stringstream outFile;
   string output = getOutPin()->getName();
   string input1 = getInPin(0)->getOutPin()->getName();
   string input2 = getInPin(1)->getOutPin()->getName();

   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int width2 =  VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   int oWidth =  VLDesign.getBus(getOutPin()->getBusId())->getWidth();

   //cout << "width1 = " << width1 << " width2 = " << width2 << " oWidth = " << oWidth << endl;
   //assert (width1 == width2);
 
   assert (oWidth <= width2);
   assert (oWidth <= width1);

   for (int i = 0 ; i < oWidth; ++i) {
      outFile << ".names " << input1; 
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << input2; 
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << output; 
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #bw-xor ";
#endif 
      outFile << endl << "10 1" << endl << "01 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

void
CktXorCell::writeRedBLIF() const
{
   //cout << "Warning : CktXorCell::writeRedBLIF() still not finish." << endl;
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();
   
   for (int i = 1; i < width1; i += 2)
      genPattern(outFile, "", width1, i);

   cktOutFile.insert(outFile.str());
}

//====================================== CktXnorCell ==========================================//
CktXnorCell::CktXnorCell(): CktCell( CKT_XNOR_CELL )
{
}

CktXnorCell::CktXnorCell(bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin) 
           : CktCell(CKT_XNOR_CELL, isNewPin, iPin, oPin)
{
}

CktXnorCell::CktXnorCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
           : CktCell(CKT_XNOR_CELL, isNewPin, iA, iB, oY)
{
}

CktXnorCell::~CktXnorCell()
{
}

CktOpType
CktXnorCell::getType() const
{
   if ( getInPinSize() > 1 )
      return CKT_BW;
   else
      return CKT_RED;
}

void
CktXnorCell::writeOutput() const
{
   genOutput();
}

void
CktXnorCell::simulate()
{
   if (getType() == CKT_BW) {
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      const CktOutPin* fin1 = getInPin(1)->getOutPin();
      Bv4 tmp = fin0->getCell()->getSimValue(fin0) ^ fin1->getCell()->getSimValue(fin1);
      _simValue = ~tmp;
   }
   else {
      const CktOutPin* fin0 = getInPin(0)->getOutPin();
      _simValue = fin0->getCell()->getSimValue(fin0).reducedXnor();
   }
}

CktCell* 
CktXnorCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktXnorCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktXnorCell::writeBLIF() const
{
   if (getType() == CKT_BW) 
      writeBwBLIF();
   else //if (getType() == CKT_RED)
      writeRedBLIF();
}

void
CktXnorCell::writeBwBLIF() const
{
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int width2 =  VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   assert (width1 == width2);
   string input1 = getInPin(0)->getOutPin()->getName();
   string input2 = getInPin(1)->getOutPin()->getName();
   string output = getOutPin()->getName();
   for (int i = 0 ; i < width1; ++i) {
      outFile << ".names " << input1; 
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << input2; 
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << output; 
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";     
#ifdef BLIF_DEBUG_MODE
      outFile << " #bw-xnor ";
#endif    
      outFile << endl<< "11 1" << endl << "00 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

void
CktXnorCell::writeRedBLIF() const
{
   // cout << "Warning : CktXnorCell::writeRedBLIF() still not finish." << endl;
   stringstream outFile;
   int width1 =  VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   string input1 = getInPin(0)->getOutPin()->getName();
   string output = getOutPin()->getName();

   for (int i = 0; i < width1; i += 2)
      genPattern(outFile, "", width1, i);

   cktOutFile.insert(outFile.str());
}

//====================================== CktMuxCell =========================================//
CktMuxCell::CktMuxCell() : CktCell(CKT_MUX_CELL)
{
}

CktMuxCell::CktMuxCell(bool isNewPin, CktInPin*& iF, CktInPin*& iT, CktInPin*& iS, CktOutPin*& oY)
           : CktCell(CKT_MUX_CELL)
{
   if (isNewPin) {
      iF = new CktInPin;
      iT = new CktInPin;
      iS = new CktInPin;
      oY = new CktOutPin;
   }
   iF->connCell(this); connInPin(iF);//inPinList[0]: false child
   iT->connCell(this); connInPin(iT);//inPinList[1]: true child
   iS->connCell(this); connInPin(iS);//inPinList[2]: select line
   oY->connCell(this); connOutPin(oY);
}

CktMuxCell::~CktMuxCell()
{
}

string
CktMuxCell::getExpr(CktOutPin* p) const {
   assert (p == getOutPin());
   for (unsigned i = 0; i < 3; ++i) assert (getInPin(i));
   return (getInPin(2)->getOutPin()->getCell()->getExpr(getInPin(2)->getOutPin()) + " ? " + 
           getInPin(1)->getOutPin()->getCell()->getExpr(getInPin(1)->getOutPin()) + " : " + 
           getInPin(0)->getOutPin()->getCell()->getExpr(getInPin(0)->getOutPin())
          );
}

/* module DVL_MUX ( y, A, B, S );
        |---\
   A----|F   \
        |    |---y
   B----|T   /
        |---/
          |
          |
          S                 */
void
CktMuxCell::writeOutput() const
{  
   stringstream input;
   stringstream zeroExtend;

   string  moduleName = "DVL_MUX";
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   CktOutPin* bOutpin = getInPin(1)->getOutPin();
   CktOutPin* sOutpin = getInPin(2)->getOutPin();
   const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());
   const SynBus* aBus = VLDesign.getBus(aOutpin->getBusId());
   const SynBus* bBus = VLDesign.getBus(bOutpin->getBusId());
   string yName = yOutpin->getName();
   string aName = aOutpin->getName();
   string bName = bOutpin->getName();
   string sName = sOutpin->getName();
   assert ((yName != "") && (aName != "") && (bName != "") && (sName != ""));
   int yWidth = yBus->getWidth();   
   int aWidth = aBus->getWidth();
   int bWidth = bBus->getWidth();
   int width;
   if (aWidth >= bWidth) width = aWidth;
   else                  width = bWidth;
   if ( !synOutFile.isExistWire(yName) )
      synOutFile.insertWireInst( genWireInst(yOutpin), yName );
   if ( !synOutFile.isExistWire(aName) )
      synOutFile.insertWireInst( genWireInst(aOutpin), aName );
   if ( !synOutFile.isExistWire(bName) )
      synOutFile.insertWireInst( genWireInst(bOutpin), bName );      
   if ( !synOutFile.isExistWire(sName) )
      synOutFile.insertWireInst( genWireInst(sOutpin), sName );
   
   input << moduleName << " #( " << width << " ) " << (CREMap->moduleNamePrefix) << instNumber << " ( "
         << genModuleInst("Y", yOutpin) << ", ";
   
   if (yWidth > aWidth) {
      zeroExtend << yWidth - aWidth << "'b0";
      input << genModuleInst("A", aOutpin, zeroExtend.str()) << ", ";
   }
   else // (yWidth == aWidth) or (yWidth < aWidth)
      input << genModuleInst("A", aOutpin) << ", ";
   
   if (yWidth> bWidth) {
      zeroExtend << yWidth-bWidth << "'b0";
      input << genModuleInst("B", bOutpin, zeroExtend.str()) << ", ";
   }
   else // (yWidth == bWidth) or (yWidth < bWidth)
      input << genModuleInst("B", bOutpin) << ", ";
   
   input << genModuleInst("S", sOutpin) << " );" << "//" << getCellID() << endl;   
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void
CktMuxCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   const CktOutPin* fin2 = getInPin(2)->getOutPin();
   Bv4 selValue = fin2->getCell()->getSimValue(fin2);
   assert (selValue.bits() == 1);

   if (selValue == CktCell::_1bitZero)
      _simValue = fin0->getCell()->getSimValue(fin0);
   else if (selValue == CktCell::_1bitOne)
      _simValue = fin1->getCell()->getSimValue(fin1);
   else {
      const SynBus* bus = VLDesign.getBus(getOutPin()->getBusId());
      unsigned bits = bus->getWidth();
      stringstream ss;
      ss << bits << "'b";
      for (unsigned i = 0; i < bits; ++i)
         ss << 'X';

      _simValue = ss.str();
   }
}

CktCell* 
CktMuxCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktMuxCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktMuxCell::writeBLIF() const
{
/*
   module DVL_MUX ( y, A, B, S );
        |---\
   A----|F   \
        |    |---y
   B----|T   /
        |---/
          |
          |
          S
*/
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string sName = getInPin(2)->getOutPin()->getName();

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   int sWidth = VLDesign.getBus(getInPin(2)->getOutPin()->getBusId())->getWidth();
   bool yIsInv = VLDesign.getBus(getOutPin()->getBusId())->isInverted();
   bool aIsInv = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->isInverted();
   bool bIsInv = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->isInverted();
     
   assert(yWidth == aWidth);
   assert(yWidth == bWidth);
   assert(sWidth == 1);

   for (int i = 0; i < yWidth; ++i) {
      outFile << ".names " << sName;
      if (!getInPin(2)->getOutPin()->is1BitIoPin()) outFile << "[0]";
      
      outFile << " " << aName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) {
         if (yIsInv == aIsInv) 
            outFile << "[" << i << "]";
         else  
            outFile << "[" << yWidth-1-i << "]";
      }
      outFile << " " << bName;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) {
         if (yIsInv == bIsInv)
            outFile << "[" << i << "]";
         else 
            outFile << "[" << yWidth-1-i << "]";
      }
      outFile << " " << yName;
      if (!getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #mux ";
#endif
      outFile << endl << "01- 1" << endl << "1-1 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

//===================================== CktDemuxCell ========================================//
/*CktDemuxCell::CktDemuxCell(): CktCell( CKT_DEMUX_CELL )
{
}

CktDemuxCell::~CktDemuxCell()
{
}

void
CktDemuxCell::simulate()
{
}

//===================================== CktOnehotCell =======================================//
CktOnehotCell::CktOnehotCell(): CktCell( CKT_ONEHOT_CELL )
{
}

CktOnehotCell::~CktOnehotCell()
{
}

string
CktOnehotCell::getCellTypeString() const
{
   return "onehot";
}

void
CktOnehotCell::simulate()
{
}*/

//==================================== CktEqualityCell ======================================//
CktEqualityCell::CktEqualityCell(): CktCell( CKT_EQUALITY_CELL )
{
}

CktEqualityCell::CktEqualityCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
               : CktCell(CKT_EQUALITY_CELL, isNewPin, iA, iB, oY)
{
}

CktEqualityCell::~CktEqualityCell()
{
}

void
CktEqualityCell::writeOutput() const
{
   genOutputRelaCell();
}

void
CktEqualityCell::setCaseEq()
{
   _eq = false;
}

void
CktEqualityCell::setLogicEq()
{
   _eq = true;
}

bool
CktEqualityCell::isLogicEq() const
{
   return _eq;
}

void
CktEqualityCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   if (fin0->getCell()->getSimValue(fin0) == fin1->getCell()->getSimValue(fin1))
      _simValue = CktCell::_1bitOne;
   else
      _simValue = CktCell::_1bitZero;
}

CktCell* 
CktEqualityCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktEqualityCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   if (isLogicEq() == true)
      (static_cast<CktEqualityCell*>(newCell))->setLogicEq();
   else
      (static_cast<CktEqualityCell*>(newCell))->setCaseEq();
   return newCell;
}

void 
CktEqualityCell::writeBLIF() const
{
/*        _____                                      _____
     A---|     |              (A[0] == B[0]) ->  ---|     |
         |  =  |----Y  ==>    (A[1] == B[1]) ->  ---| AND |--- Y
     B---|_____|              (A[2] == B[2]) ->  ---|_____|
*/  
   //assert(_eq == true); _eq no use!
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string tName = aName + bName;

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   bool isInvA = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->isInverted(); 
   bool isInvB = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->isInverted(); 
   //assert(aWidth == bWidth);
   if (aWidth != bWidth)
      cout << "Warning : unmatch # bits comparator !! (aWidth, bWidth) = (" << aWidth << ", " << bWidth << ")" << endl;
   assert(yWidth == 1);
   if (isInvA == isInvB) {
      for (int i = 0; i < aWidth; ++i) {
         outFile << ".names " << aName;
         if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
         outFile << " " << bName;
         if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
         outFile << " " << tName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
         outFile << " #eq_level_1 ";
#endif
         outFile << endl << "11 1" << endl  << "00 1" << endl;
      }
   }
   else {
      for (int i = 0; i < aWidth; ++i) {
         outFile << ".names " << aName;
         if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
         outFile << " " << bName;
         if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << aWidth-1-i << "]";
         outFile << " " << tName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
         outFile << " #eq_level_1 ";
#endif
         outFile << endl << "11 1" << endl << "00 1" << endl;
      }
   }
   outFile << ".names ";
   for (int i = 0; i < aWidth; ++i) 
      outFile << tName << "[" << i << "] ";
   outFile << yName;
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #eq_level_2 ";
#endif
   outFile << endl;

   for (int i = 0; i < aWidth; ++i)
      outFile << "1";
   outFile << " 1" << endl;

   cktOutFile.insert(outFile.str());
}

//==================================== CktGreaterCell =======================================//
CktGreaterCell::CktGreaterCell(): CktCell( CKT_GREATER_CELL )
{
}

CktGreaterCell::CktGreaterCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
              : CktCell(CKT_GREATER_CELL, isNewPin, iA, iB, oY)
{
}

CktGreaterCell::~CktGreaterCell() 
{
}

void
CktGreaterCell::writeOutput() const
{
   genOutputRelaCell();
}

void
CktGreaterCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   if (fin0->getCell()->getSimValue(fin0) > fin1->getCell()->getSimValue(fin1))
      _simValue = CktCell::_1bitOne;
   else
      _simValue = CktCell::_1bitZero;
}

CktCell* 
CktGreaterCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktGreaterCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktGreaterCell::writeBLIF() const
{
//Maybe the implementions of writeBLIF in CktGreaterCell, CktGeqCell, CktLeqCell, and CktLessCell 
//will produce many redundant BLIF cells, but ABC will optimize them.
/*     _____                                                                       _____
  A---|     |           (A[2] == B[2]) && (A[1] == B[1]) && (A[0] >  B[0]) ->  ---|     |
      |  >  |----Y  ==> (A[2] == B[2]) && (A[1] >  B[1])                   ->  ---| OR  |--- Y
  B---|_____|           (A[2] >  B[2])                                     ->  ---|_____|
*/  
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string tName = aName + bName;
   string uName = bName + aName;

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();

   assert(aWidth == bWidth);
   assert(yWidth == 1);
   //1. EQUALITY
   for (int i = 1; i < aWidth; ++i) {//don't produce the "[0] gate" and "all-eq gate"
      outFile << ".names ";
      outFile << aName << "[" << i << "] "
              << bName << "[" << i << "] "
              << tName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #eq_level_1 ";
#endif
      outFile << endl << "11 1" << endl << "00 1" << endl;
   }
   //2. GREATER
   for (int i = 0; i < aWidth; ++i) {
      //2-1 comparing bit
      outFile << ".names " << aName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << bName;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << tName << "_gre" << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #gre_level_1(cmp_bit)";
#endif
      outFile << endl << "10 1" << endl;
      //2-2 AND gate
      outFile << ".names ";
      for (int k = aWidth-1; k > i; --k) 
         outFile << tName << "[" << k << "] ";
      outFile << tName << "_gre" << "[" << i << "] ";
      outFile << uName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #gre_level_2 ";
#endif
      outFile << endl;

      for (int k = aWidth-1; k >= i; --k)
         outFile << "1";
      outFile << " 1" << endl;
   }
   //3. OR gate
   outFile << ".names ";
   for (int i = 0; i < aWidth; ++i)
      outFile << uName << "[" << i << "] ";
   outFile << yName;
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #gre_level_3 ";
#endif
   outFile << endl;

   for (int i = 0; i < aWidth; ++i) {
      for (int j = 0; j < aWidth; ++j) {
         if (i != j)
            outFile << "-";
         else
            outFile << "1";
      }
      outFile << " 1" << endl;
   }
   cktOutFile.insert(outFile.str());
}

//====================================== CktGeqCell =========================================//
CktGeqCell::CktGeqCell(): CktCell( CKT_GEQ_CELL )
{
}

CktGeqCell::CktGeqCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_GEQ_CELL, isNewPin, iA, iB, oY)
{
}

CktGeqCell::~CktGeqCell()
{
}

void
CktGeqCell::writeOutput() const
{
   genOutputRelaCell();
}

void
CktGeqCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   if (fin0->getCell()->getSimValue(fin0) >= fin1->getCell()->getSimValue(fin1))
      _simValue = CktCell::_1bitOne;
   else
      _simValue = CktCell::_1bitZero;
}

CktCell* 
CktGeqCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktGeqCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
} 

void 
CktGeqCell::writeBLIF() const
{
//   GEQ = GREATER + EQUALITY
/*     _____                                                                       _____
  A---|     |           (A[2] == B[2]) && (A[1] == B[1]) && (A[0] >= B[0]) ->  ---|     |
      |  >= |----Y  ==> (A[2] == B[2]) && (A[1] >  B[1])                   ->  ---| OR  |--- Y
  B---|_____|           (A[2] >  B[2])                                     ->  ---|_____|
*/  
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string tName = aName + bName;
   string uName = bName + aName;

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();

   assert(aWidth == bWidth);
   assert(yWidth == 1);

   //1. EQUALITY
   for (int i = 0; i < aWidth; ++i) {
      outFile << ".names ";
      outFile << aName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << bName;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << tName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #eq_level_1 ";
#endif
      outFile << endl << "11 1" << endl << "00 1" << endl;
   }
   outFile << ".names ";
   for (int i = 0; i < aWidth; ++i) 
      outFile << tName << "[" << i << "] ";
   outFile << uName;
#ifdef BLIF_DEBUG_MODE
   outFile << " #eq_level_2 ";
#endif
   outFile << endl;
   for (int i = 0; i < aWidth; ++i)
      outFile << "1";
   outFile << " 1" << endl;

   //2. GREATER
   for (int i = 0; i < aWidth; ++i) {
      //2-1 comparing bit
      outFile << ".names ";
      outFile << aName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << bName;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << tName << "_gre" << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #gre_level_1(cmp_bit)";
#endif
      outFile << endl << "10 1" << endl;
      //2-2 AND gate
      outFile << ".names ";
      for (int k = aWidth-1; k > i; --k) 
         outFile << tName << "[" << k << "] ";
      outFile << tName << "_gre" << "[" << i << "] ";
      outFile << uName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #gre_level_2 ";
#endif
      outFile << endl;

      for (int k = aWidth-1; k >= i; --k)
         outFile << "1";
      outFile << " 1" << endl;
   }
   //OR gate
   outFile << ".names ";
   for (int i = 0; i < aWidth; ++i)
      outFile << uName << "[" << i << "] ";
   outFile << uName << " ";
   outFile << yName;
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #gre_level_3 ";
#endif
   outFile << endl;

   //To "or" Greater part
   for (int i = 0; i < aWidth; ++i) {
      for (int j = 0; j < aWidth; ++j) {
         if (i != j) outFile << "-";
         else        outFile << "1";
      }
      outFile << "- 1" << endl;
   }
   //To "or" EQ part
   for (int j = 0; j < aWidth; ++j)
      outFile << "-";
   outFile << "1 1" << endl;
   cktOutFile.insert(outFile.str());
}

//====================================== CktLeqCell =========================================//
CktLeqCell::CktLeqCell(): CktCell( CKT_LEQ_CELL )
{
}

CktLeqCell::CktLeqCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_LEQ_CELL, isNewPin, iA, iB, oY)
{
}

CktLeqCell::~CktLeqCell()
{
}

void
CktLeqCell::writeOutput() const
{
   genOutputRelaCell();
}

void
CktLeqCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   if (fin0->getCell()->getSimValue(fin0) <= fin1->getCell()->getSimValue(fin1))
      _simValue = CktCell::_1bitOne;
   else
      _simValue = CktCell::_1bitZero;
}

CktCell* 
CktLeqCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktLeqCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktLeqCell::writeBLIF() const
{
/*     _____                                                                       _____
  A---|     |           (A[2] == B[2]) && (A[1] == B[1]) && (A[0] <= B[0]) ->  ---|     |
      | <=  |----Y  ==> (A[2] == B[2]) && (A[1] >  B[1])                   ->  ---| OR  |--- Y
  B---|_____|           (A[2] >  B[2])                                     ->  ---|_____|
*/ 
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string tName = aName + bName;
   string uName = bName + aName;

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();

   assert(aWidth == bWidth);
   assert(yWidth == 1);

   //1. EQUALITY
   for (int i = 0; i < aWidth; ++i) {
      outFile << ".names " << aName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << bName;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << tName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #eq_level_1 ";
#endif
      outFile << endl << "11 1" << endl << "00 1" << endl;
   }
   outFile << ".names ";
   for (int i = 0; i < aWidth; ++i) 
      outFile << tName << "[" << i << "] ";
   outFile << uName;
#ifdef BLIF_DEBUG_MODE
   outFile << " #eq_level_2 ";
#endif
   outFile << endl;
   for (int i = 0; i < aWidth; ++i)
      outFile << "1";
   outFile << " 1" << endl;

   //2. LESS
   for (int i = 0; i < aWidth; ++i) {
      //2-1 comparing bit
      outFile << ".names ";
      outFile << aName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << bName;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << tName << "_less" << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #lse_level_1(cmp_bit)";
#endif
      outFile << endl << "01 1" << endl;
      //2-2 AND gate
      outFile << ".names ";
      for (int k = aWidth-1; k > i; --k) 
         outFile << tName << "[" << k << "] ";
      outFile << tName << "_less" << "[" << i << "] ";
      outFile << uName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #lse_level_2 ";
#endif
      outFile << endl;

      for (int k = aWidth-1; k >= i; --k)
         outFile << "1";
      outFile << " 1" << endl;
   }
   //OR gate
   outFile << ".names ";
   for (int i = 0; i < aWidth; ++i)
      outFile << uName << "[" << i << "] ";
   outFile << uName << " ";
   outFile << yName; 
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #lse_level_3 ";
#endif
   outFile << endl;

   //To "or" Less part
   for (int i = 0; i < aWidth; ++i) {
      for (int j = 0; j < aWidth; ++j) {
         if (i != j) outFile << "-";
         else        outFile << "1";
      }
      outFile << "- 1" << endl;
   }
   //To "or" EQ part
   for (int j = 0; j < aWidth; ++j)
      outFile << "-";
   outFile << "1 1" << endl;

   cktOutFile.insert(outFile.str()); 
}

//====================================== CktLessCell ========================================//
CktLessCell::CktLessCell(): CktCell( CKT_LESS_CELL )
{
}

CktLessCell::CktLessCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
           : CktCell(CKT_LESS_CELL, isNewPin, iA, iB, oY)
{
}

CktLessCell::~CktLessCell()
{
}

void
CktLessCell::writeOutput() const
{
   genOutputRelaCell();
}

void
CktLessCell::simulate()
{
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   if (fin0->getCell()->getSimValue(fin0) < fin1->getCell()->getSimValue(fin1))
      _simValue = CktCell::_1bitOne;
   else
      _simValue = CktCell::_1bitZero;
}

CktCell* 
CktLessCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktLessCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktLessCell::writeBLIF() const
{
/*     _____                                                                       _____
  A---|     |           (A[2] == B[2]) && (A[1] == B[1]) && (A[0] <  B[0]) ->  ---|     |
      |  <  |----Y  ==> (A[2] == B[2]) && (A[1] >  B[1])                   ->  ---| OR  |--- Y
  B---|_____|           (A[2] >  B[2])                                     ->  ---|_____|
*/ 
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string tName = aName + bName;
   string uName = bName + aName;

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();

   assert(aWidth == bWidth);
   assert(yWidth == 1);

   //1. EQUALITY
   for (int i = 1; i < aWidth; ++i) {//don't produce the "[0] gate" and "all-eq gate"
      outFile << ".names ";
      outFile << aName << "[" << i << "] "
              << bName << "[" << i << "] "
              << tName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #eq_level_1 ";
#endif
      outFile << endl << "11 1" << endl << "00 1" << endl;
   }
   //2. LESS
   for (int i = 0; i < aWidth; ++i) {
      //2-1 comparing bit
      outFile << ".names " << aName;
      if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << bName;
      if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[" << i << "]";
      outFile << " " << tName << "_less" << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #less_level_1(cmp_bit)";
#endif
      outFile << endl << "01 1" << endl;
      //2-2 AND gate
      outFile << ".names ";
      for (int k = aWidth-1; k > i; --k) 
         outFile << tName << "[" << k << "] ";
      outFile << tName << "_less" << "[" << i << "] ";
      outFile << uName << "[" << i << "]";
#ifdef BLIF_DEBUG_MODE
      outFile << " #less_level_2 ";
#endif
      outFile << endl;

      for (int k = aWidth-1; k >= i; --k)
         outFile << "1";
      outFile << " 1" << endl;
   }
   //3. OR gate
   outFile << ".names ";
   for (int i = 0; i < aWidth; ++i)
      outFile << uName << "[" << i << "] ";
   outFile << yName;
   if (!getOutPin()->is1BitIoPin()) outFile << "[0]";
#ifdef BLIF_DEBUG_MODE
   outFile << " #less_level_3 ";
#endif
   outFile << endl;

   for (int i = 0; i < aWidth; ++i) {
      for (int j = 0; j < aWidth; ++j) {
         if (i != j)
            outFile << "-";
         else
            outFile << "1";
      }
      outFile << " 1" << endl;
   }
   cktOutFile.insert(outFile.str()); 
}

//====================================== CktCaseCell ========================================//
/*
CktCaseCell::CktCaseCell(): CktCell( CKT_CASE_CELL )
{
   _cond = NULL;
}

CktCaseCell::~CktCaseCell()
{
}

void
CktCaseCell::connCond( CktInPin *pin )
{
   _cond = pin;
}

void
CktCaseCell::connVarCond( CktInPin * pin )
{
   _varCondList.push_back( pin );
}

CktInPin *
CktCaseCell::getCond() const
{
   return _cond;
}

CktInPin *
CktCaseCell::getVarCond( unsigned index ) const 
{
   assert( index >= 0 );
   assert( index < _varCondList.size() );

   return _varCondList[ index ];
}

int
CktCaseCell::getVarCondListSize() const
{
   return _varCondList.size();
}

void
CktCaseCell::simulate()
{
}
*/
#endif

