/****************************************************************************
  FileName     [ cktWriteOutput.cpp ]
  Package      [ ckt ]
  Synopsis     [ Writing functions for output ]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_WRITE_OUTPUT_SOURCE
#define CKT_WRITE_OUTPUT_SOURCE

//---------------------------
//  system include
//---------------------------
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
//ckt ds
#include "cktPin.h"
#include "cktCell.h"
#include "cktEnum.h"

//syn ds
#include "synVar.h"

//---------------------------
//  Global variables
//---------------------------
extern VlpDesign&      VLDesign;
extern SynOutFile      synOutFile;
extern int             instNumber;
extern int             netNumber;
extern CKT_usage*      CREMap;
//function definition

//this function only be used in the two "writeOutput"
//void CktSplitCell::writeOutput() const
//void CktBusCell::writeOutput() const
string 
CktCell::genModuleInst(string nameMapping, string& name, const SynBus*& bus, bool isShowBit) const
{
   stringstream ss;
   int  begin = bus->getBegin();
   int  end   = bus->getEnd();
   bool isRev = bus->isInverted();
   if (begin == end) {
      if (_cellType == CKT_MERGE_CELL || _cellType == CKT_BUS_CELL)
         ss << "." << nameMapping << "(" << name << "[" << begin << "])";
      else if (_cellType == CKT_SPLIT_CELL) {
         if (isShowBit)
            ss << "." << nameMapping << "(" << name << "[" << begin << "])";
         else
            ss << "." << nameMapping << "(" << name << ")";
      }
      else
         assert(0);
   }
   else {
      if (isRev)
         ss << "." << nameMapping << "(" << name << "[" << end << ":" << begin << "])";
      else
         ss << "." << nameMapping << "(" << name << "[" << begin << ":" << end << "])";
   }         
   return ss.str();
}

string 
CktCell::genModuleInst(string nameMapping, CktOutPin* outPin) const
{
   stringstream ss;
   if (outPin != 0) {
      string name = outPin->getName();
      const SynBus* bus = VLDesign.getBus(outPin->getBusId());
      int  begin = bus->getBegin();
      int  end   = bus->getEnd();
      bool isRev = bus->isInverted();
      ss << "." << nameMapping << "(" << name;
      if (begin == end)
         ss << ")";
      else {
         if (outPin->getCell()->getCellType() == CKT_SPLIT_CELL
          && static_cast<const CktSplitCell*>(outPin->getCell())->isSplitFin() ) 
         {
            int width = bus->getWidth();
            ss << "[" << width-1 << ":" << "0" << "])";
         }
         else {
            if (isRev) ss << "[" << end << ":" << begin << "])";
            else       ss << "[" << begin << ":" << end << "])";
         }
      }         
   }
   else
      ss << "." << nameMapping << "( )";

   return ss.str();
}

//this function only be used in the three "writeOutput"
//void CktMergeCell::writeOutput() const
//void CktSplitCell::writeOutput() const
//void CktModuleCell::writeOutput() const
string 
CktCell::genModuleInst(string nameMapping, string& name, int begin, int end) const
{
   stringstream ss;
   if (begin == end) {
      if (_cellType == CKT_MERGE_CELL)
         ss << "." << nameMapping << "(" << name << "[" << begin << "])";
      else if (_cellType == CKT_SPLIT_CELL || _cellType == CKT_MODULE_CELL) {
         if (begin == 0)
            ss << "." << nameMapping << "(" << name << ")";
         else
            ss << "." << nameMapping << "(" << name << "[" << begin << "])";
      }
      else
         assert(0);
   }
   else 
      ss << "." << nameMapping << "(" << name << "[" << begin << ":" << end << "])";
   return ss.str();
}

// this function only be used in the two functions
// void CktCell::genOutputTwoOprd() const
// void CktMuxCell::writeOutput() const
string
CktCell::genModuleInst(string nameMapping, CktOutPin* outPin, string zeroExtend) const
{
   string name = outPin->getName();
   const SynBus* bus = VLDesign.getBus(outPin->getBusId());

   stringstream ss;
   int  begin = bus->getBegin();
   int  end   = bus->getEnd();
   bool isRev = bus->isInverted();

   ss << "." << nameMapping << "({" << zeroExtend << ", " << name;

   if (begin == end)
      ss << "})";
   else {
      if (outPin->getCell()->getCellType() == CKT_SPLIT_CELL
       && static_cast<const CktSplitCell*>(outPin->getCell())->isSplitFin() ) 
      {
         int width = bus->getWidth();
         ss << "[" << width-1   << ":" << "0" << "]})";
      }
      else {
         if (isRev)  ss << "[" << end   << ":" << begin << "]})";
         else        ss << "[" << begin << ":" << end   << "]})";
      }
   }         
   return ss.str();
}

/*string 
CktCell::genWireName(CktOutPin* opin, int begin, int end) const
{
   stringstream ss;
   cerr << "genWireName!!!!!!!!" << end;
   ss << (CREMap->wireNamePrefix) << netNumber;
   //string name;
   //name= ss.str();
   opin->setName(ss.str());
   //if((begin==0)&& (end==0))
   //   synOutFile<< "wire "<< name<< ";"<< endl;
   //else if(begin== end)
   //   synOutFile<< "wire "<< name<< ";"<< endl;
   //else
   //   synOutFile<< "wire ["<< begin<<":"<< end<<"] "<< name<< ";"<< endl;
   ++netNumber;
   //cout<< "netName: "<< ss.str()<< endl;
   return ss.str();
}*/

string 
CktCell::genWireInst(CktOutPin* outPin) const
{
   stringstream input;
   string wireName = outPin->getName();
   const SynBus* bus = VLDesign.getBus(outPin->getBusId());

   bool isRev = bus->isInverted();
   int begin = bus->getBegin();
   int end   = bus->getEnd();
   if (begin == end)
      input << "wire " << wireName << ";";
   else {
      input << "wire [";
      if (outPin->getCell()->getCellType() == CKT_SPLIT_CELL
       && static_cast<const CktSplitCell*>(outPin->getCell())->isSplitFin() ) 
      {
         int width = bus->getWidth();
         input << width-1 << ":0] ";
      }
      else {
         if (isRev)  input << end   << ":" << begin << "] ";
         else        input << begin << ":" << end   << "] ";
      }
      input << wireName << ";";
   }         
   input << "/*cell type = " << getCellType() << "*/" << endl;   
   return input.str();
}

string 
CktCell::genWireInst(string& wireName, int width) const // only use in signal split spCell
{
   stringstream input;
   if (width == 1)
      input << "wire " << wireName << ";";
   else
      input << "wire [" << width-1 << ":0] " << wireName << ";";
         
   input << "/*cell type = " << getCellType() << "*/" << endl;   
   return input.str();
}

/*string 
CktCell::genRegInst(string regName, int begin, int end)
{
   stringstream input;
   if((begin==0)&& (end==0))
      input<< "reg "<< regName<< ";"<< endl;
   else if(begin== end)
      input<< "reg "<< regName<< ";"<< endl;
   else
      input<< "reg ["<< begin<<":"<< end<<"] "<< regName<< ";"<< endl;
   //input<< "reg ["<< begin<<":"<< end<<"] "<< wireName<< ";"<< endl;
   return input.str();
}*/

void 
CktCell::genOutputOneOprd() const
{
   assert ((_cellType == CKT_AND_CELL)  || (_cellType == CKT_OR_CELL)
        || (_cellType == CKT_NAND_CELL) || (_cellType == CKT_NOR_CELL)
        || (_cellType == CKT_XOR_CELL)  || (_cellType == CKT_XNOR_CELL)
        || (_cellType == CKT_BUF_CELL)  || (_cellType == CKT_INV_CELL) 
        || (_cellType == CKT_PO_CELL)   || (_cellType == CKT_PI_CELL)  
        || (_cellType == CKT_PIO_CELL)       );
   stringstream input;
   string  moduleName = getDVLType1(getCellType());
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   string     yName   = yOutpin->getName();
   string     aName   = aOutpin->getName();
   const SynBus* aBus = VLDesign.getBus(aOutpin->getBusId());
   int         yWidth = aBus->getWidth();
      
   //po maybe connect to null fanin
   if ((_cellType != CKT_PO_CELL) && (_cellType != CKT_PIO_CELL)) 
      assert ((yName != "") && (aName != "")); 
   
   if (aName != "") {
      if (!synOutFile.isExistWire(yName))
         synOutFile.insertWireInst(genWireInst(yOutpin), yName);
      if (!synOutFile.isExistWire(aName))
         synOutFile.insertWireInst(genWireInst(aOutpin), aName);
      
      input << moduleName << " #(" << yWidth << ") " << (CREMap->moduleNamePrefix) << instNumber << " ("
            << genModuleInst("Y", yOutpin) << ", "
            << genModuleInst("A", aOutpin) << ");" << endl;
   
      ++instNumber;
      synOutFile.insertModuleInst(input.str());
   }
}

void 
CktCell::genOutputTwoOprd() const
{
   assert ((_cellType == CKT_AND_CELL)  || (_cellType == CKT_OR_CELL)
        || (_cellType == CKT_NAND_CELL) || (_cellType == CKT_NOR_CELL)
        || (_cellType == CKT_XOR_CELL)  || (_cellType == CKT_XNOR_CELL) );

   stringstream input;  
   string  moduleName = getDVLType2(getCellType());
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   CktOutPin* bOutpin = getInPin(1)->getOutPin();
   const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());
   const SynBus* aBus = VLDesign.getBus(aOutpin->getBusId());
   const SynBus* bBus = VLDesign.getBus(bOutpin->getBusId());
   string yName = yOutpin->getName();
   string aName = aOutpin->getName();
   string bName = bOutpin->getName();  
   assert ((yName != "") && (aName != "") && (bName != ""));
   int   yWidth = yBus->getWidth();  
   int   aWidth = aBus->getWidth();
   int   bWidth = bBus->getWidth();
   int width = 0;
   if (aWidth >= bWidth) width = aWidth;
   else                  width = bWidth;
      
   if (!synOutFile.isExistWire(yName))
      synOutFile.insertWireInst(genWireInst(yOutpin), yName);   
   if (!synOutFile.isExistWire(aName))
      synOutFile.insertWireInst(genWireInst(aOutpin), aName);      
   if (!synOutFile.isExistWire(bName))
      synOutFile.insertWireInst(genWireInst(bOutpin), bName);
   
   input << moduleName << " #(" << width << ") " << (CREMap->moduleNamePrefix) << instNumber << " ("
         << genModuleInst("Y", yOutpin) << ", ";
   if (yWidth > aWidth) {
      stringstream zeroExtend;
      zeroExtend << yWidth-aWidth << "'b0";
      input << genModuleInst("A", aOutpin, zeroExtend.str()) << ", ";
   }
   else //(yWidth == aWidth) or (yWidth< aWidth)
      input << genModuleInst("A", aOutpin) << ", ";
   
   if (yWidth > bWidth) {
      stringstream zeroExtend;
      zeroExtend << yWidth-bWidth << "'b0";
      input << genModuleInst("B", bOutpin, zeroExtend.str()) << ");" << endl;
   }
   else //(yWidth <= bWidth)
      input << genModuleInst("B", bOutpin) << ");" << endl;

   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void 
CktCell::genLogicOutputTwoOprd() const
{
   assert ((_cellType == CKT_AND_CELL) || (_cellType == CKT_OR_CELL));

   stringstream input;
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   CktOutPin* bOutpin = getInPin(1)->getOutPin();
   const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());
   //const SynBus* aBus = VLDesign.getBus(aOutpin->getBusId());
   //const SynBus* bBus = VLDesign.getBus(bOutpin->getBusId());
   string yName = yOutpin->getName();
   string aName = aOutpin->getName();
   string bName = bOutpin->getName();
   string moduleName;
   if (getCellType() == CKT_AND_CELL)
      moduleName = "DVL_LOG_AND";
   else if (getCellType() == CKT_OR_CELL)
      moduleName = "DVL_LOG_OR";
   else
      moduleName = "DVL_ERROR";

   int yWidth = yBus->getWidth();   
   
   assert ((yName != "") && (aName != "") && (bName != ""));
   if (!synOutFile.isExistWire(yName))
      synOutFile.insertWireInst(genWireInst(yOutpin), yName);   
   if (!synOutFile.isExistWire(aName))
      synOutFile.insertWireInst(genWireInst(aOutpin), aName);      
   if (!synOutFile.isExistWire(bName))
      synOutFile.insertWireInst(genWireInst(bOutpin), bName);
   
   input << moduleName << " #(" << yWidth << ") " << (CREMap->moduleNamePrefix) << instNumber << " ("
         << genModuleInst("Y", yOutpin) << ", "
         << genModuleInst("A", aOutpin) << ", "
         << genModuleInst("B", bOutpin) << ");" << endl;
   
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void 
CktCell::genOutputRelaCell() const
{
   assert ((_cellType == CKT_GEQ_CELL)  || (_cellType == CKT_GREATER_CELL)
        || (_cellType == CKT_LEQ_CELL)  || (_cellType == CKT_LESS_CELL)   
        || (_cellType == CKT_EQUALITY_CELL) );

   stringstream input;
   string  moduleName = getDVLType2(getCellType());
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   CktOutPin* bOutpin = getInPin(1)->getOutPin();
   //const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());
   const SynBus* aBus = VLDesign.getBus(aOutpin->getBusId());
   const SynBus* bBus = VLDesign.getBus(bOutpin->getBusId());
   string yName = yOutpin->getName();
   string aName = aOutpin->getName();
   string bName = bOutpin->getName();
   assert ((yName != "") && (aName != "") && (bName != ""));
   int   aWidth = aBus->getWidth();
   int   bWidth = bBus->getWidth();

   if (!synOutFile.isExistWire(yName))
      synOutFile.insertWireInst(genWireInst(yOutpin), yName);
   if (!synOutFile.isExistWire(aName))
      synOutFile.insertWireInst(genWireInst(aOutpin), aName);      
   if (!synOutFile.isExistWire(bName))
      synOutFile.insertWireInst(genWireInst(bOutpin), bName);
   
   input << moduleName << " #(" << aWidth << ", " << bWidth << ") " 
         << (CREMap->moduleNamePrefix) << instNumber << " ("
         << genModuleInst("Y", yOutpin) << ", "
         << genModuleInst("A", aOutpin) << ", "
         << genModuleInst("B", bOutpin) << ");" << endl;
   
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

void 
CktCell::genOutput() const
{
   if (getInPinSize() == 1)
      genOutputOneOprd();
   else if (getInPinSize() == 2)
      genOutputTwoOprd();
   else {
      Msg(MSG_WAR) << "Cell " << getOutPin()->getName() << " has InPinSize = " << getInPinSize() << " !" << endl;
      //assert (0);
      //cerr << "Warning : @ CktCell::genOutput()" << endl;
   }
}

void 
CktCell::genOutputArith() const
{
   assert ((_cellType == CKT_ADD_CELL)  || (_cellType == CKT_SUB_CELL) 
        || (_cellType == CKT_MULT_CELL) || (_cellType == CKT_MODULE_CELL)
        || (_cellType == CKT_SHL_CELL)  || (_cellType == CKT_SHR_CELL) );

   stringstream input;
   string moduleName  = getDVLType2(getCellType());
   CktOutPin* yOutpin = getOutPin();
   CktOutPin* aOutpin = getInPin(0)->getOutPin();
   CktOutPin* bOutpin = getInPin(1)->getOutPin();
   string     yName   = yOutpin->getName();
   string     aName   = aOutpin->getName();
   string     bName   = bOutpin->getName();
   const SynBus* yBus = VLDesign.getBus(yOutpin->getBusId());
   const SynBus* aBus = VLDesign.getBus(aOutpin->getBusId());
   const SynBus* bBus = VLDesign.getBus(bOutpin->getBusId());
   assert ((yName != "") && (aName != "") && (bName != ""));

   if (!synOutFile.isExistWire(yName))
      synOutFile.insertWireInst(genWireInst(yOutpin), yName);
   if (!synOutFile.isExistWire(aName))
      synOutFile.insertWireInst(genWireInst(aOutpin), aName);      
   if (!synOutFile.isExistWire(bName))
      synOutFile.insertWireInst(genWireInst(bOutpin), bName);
   
   input << moduleName << " #(" << yBus->getWidth() << ", " << aBus->getWidth() << ", " << bBus->getWidth() << ") " 
         << (CREMap->moduleNamePrefix) << instNumber << " ("
         << genModuleInst("Y", yOutpin) << ", "
         << genModuleInst("A", aOutpin) << ", "
         << genModuleInst("B", bOutpin) << ");" << endl;
   
   ++instNumber;
   synOutFile.insertModuleInst(input.str());
}

string 
CktCell::getDVLType1(int type) const
{
   if (type == CKT_PI_CELL)
      return "DVL_BUF/*PI*/";
   else if (type == CKT_PO_CELL)
      return "DVL_BUF/*PO*/";
   else if (type == CKT_PIO_CELL)
      return "DVL_BUF/*PIO*/";
   //CKT_PIO_CELL

   // sequential
   //CKT_DFF_CELL,//4
   //CKT_DLAT_CELL,//5

   // logic
   else if (type == CKT_BUF_CELL)
      return "DVL_BUF/*buf*/";
   else if (type == CKT_INV_CELL)
      return "DVL_BW_NOT";
   else if (type == CKT_AND_CELL)
      return "DVL_RED_AND";
   else if (type == CKT_NAND_CELL)
      return "DVL_RED_NAND";
   else if (type == CKT_OR_CELL)
      return "DVL_RED_OR";
   else if (type == CKT_NOR_CELL)
      return "DVL_RED_NOR";
   else if (type == CKT_XOR_CELL)
      return "DVL_RED_XOR";
   else if (type == CKT_XNOR_CELL)
      return "DVL_RED_XNOR";
   else 
      return "DVL_ERROR";
}

string 
CktCell::getDVLType2(int type) const
{
   //bitwise
   if (type == CKT_AND_CELL)
      return "DVL_BW_AND";
   else if (type == CKT_OR_CELL)
      return "DVL_BW_OR";
   else if (type == CKT_NAND_CELL)
      return "DVL_BW_NAND";
   else if (type == CKT_NOR_CELL)
      return "DVL_BW_NOR";
   else if (type == CKT_XOR_CELL)
      return "DVL_BW_XOR";
   else if (type == CKT_XNOR_CELL)
      return "DVL_BW_XNOR";
   //relational
   else if (type == CKT_EQUALITY_CELL)
      return "DVL_EQ";
   else if (type == CKT_GEQ_CELL)
      return "DVL_GEQ";
   else if (type == CKT_GREATER_CELL)
      return "DVL_GREATER";
   else if (type == CKT_LEQ_CELL)
      return "DVL_LEQ";
   else if (type == CKT_LESS_CELL)
      return "DVL_LESS";
   //arithmetic
   else if (type == CKT_ADD_CELL)
      return "DVL_ARI_ADD";
   else if (type == CKT_SUB_CELL)
      return "DVL_ARI_SUB";
   else if (type == CKT_MULT_CELL)
      return "DVL_ARI_MUL";
   else if (type == CKT_MODULO_CELL)
      return "DVL_ARI_MOD";
   //shift
   else if (type == CKT_SHL_CELL)
      return "DVL_SH_L";
   else if (type == CKT_SHR_CELL)
      return "DVL_SH_R";   
   else
      return "DVL_ERROR";   
}

#endif
