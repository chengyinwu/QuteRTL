/****************************************************************************
  FileName     [ cktAuxiliary.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Interface Implementation for Circuit Construction. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_AUXILIARY_SOURCE
#define CKT_AUXILIARY_SOURCE

#include <cctype>
#include "vlpDesign.h"
#include "VLGlobe.h"
#include "synVar.h"
#include "cktArithCell.h"
#include "cktIoCell.h"
#include "cktLogicCell.h"
#include "cktModuleCell.h"
#include "cktSeqCell.h"
#include "cktModule.h"
#include "util.h"

extern VlpDesign&          VLDesign;
extern const VlgModule*    curVlpModule;
extern CKT_usage*          CREMap;
extern bool                busDirectionDefault;

// Global Variables
const string aux_prefix = "aux_";
unsigned long aux_index = 0;

// Helper Function
inline const string getAuxiliaryName() {
   string name = aux_prefix + myInt2Str(aux_index++);
   while (((CREMap->outPinMap).top()).getSharPin(name)) name = aux_prefix + name;
   return name;
}

inline bool isLegalName(const string& name) {
   assert (name.size());
   for (unsigned i = 0; i < name.size(); ++i)
      if (name[i] < 'A' || name[i] > 'z') return false;
      else if (name[i] > 'Z' && name[i] < 'a' && name[i] != '_') return false;
   return isdigit(name[0]) ? false : true;
}

inline const string getLegalName(const string& name) {
   assert (name.size());
   return isLegalName(name) ? name : getAuxiliaryName();
}

bool
CktModule::createPort(const CktIoType& ioType, const string& ioName, const unsigned& msb, const unsigned& lsb) {
   switch (ioType) {
      case CKT_IN : 
         return createInput(ioName, msb, lsb);
      case CKT_OUT : 
         return createOutput(ioName, msb, lsb);
      case CKT_IO :
         return createInOut(ioName, msb, lsb);
      default : 
         assert (0); return false;
   }
}

bool
CktModule::createInput(const string& ioName, const unsigned& msb, const unsigned& lsb) {
   assert (ioName.size());
   CktOutPin* OutPin = createNet(ioName, msb, lsb); assert (OutPin);
   if (!checkNet(OutPin, msb, lsb)) return false;  // Bus Consistency Check Failed
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << ioName << "\" Found !!!" << endl;
   
   CktPiCell* cell = new CktPiCell(false, OutPin); assert (cell);
   insertIoCell(cell, ioName, CKT_IN); insertCell(cell);
   return true;
}

bool
CktModule::createOutput(const string& ioName, const unsigned& msb, const unsigned& lsb) {
   assert (ioName.size());
   CktOutPin* OutPin = createNet(ioName, msb, lsb); assert (OutPin);
   if (!checkNet(OutPin, msb, lsb)) return false;  // Bus Consistency Check Failed
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << ioName << "\" Found !!!" << endl;
   
   CktInPin* inPin = 0; CktOutPin* outPin = 0;
   CktPoCell* cell = new CktPoCell(true, inPin, outPin); assert (cell);
   assert (inPin); assert (outPin);
   outPin->setName(OutPin->getName());  // In reality, this OutPin will Not be inferred
   outPin->setBusId(OutPin->getBusId());
   inPin->connOutPin(OutPin); OutPin->connInPin(inPin);
   insertIoCell(cell, ioName, CKT_OUT); insertCell(cell);
   return true;
}

bool
CktModule::createInOut(const string& ioName, const unsigned& msb, const unsigned& lsb) {
   assert (ioName.size());
   CktOutPin* OutPin = createNet(ioName, msb, lsb); assert (OutPin);
   if (!checkNet(OutPin, msb, lsb)) return false;  // Bus Consistency Check Failed
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << ioName << "\" Found !!!" << endl;
   
   CktInPin* inPin = new CktInPin();
   CktPioCell* cell = new CktPioCell(false, inPin, OutPin); assert (cell);
   insertIoCell(cell, ioName, CKT_IO); insertCell(cell);
   return true;
}

CktOutPin*
CktModule::createNet(const string& netName, const unsigned& msb, const unsigned& lsb) {
   assert (netName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(netName);
   if (OutPin) return OutPin;
   else OutPin = new CktOutPin(); assert (OutPin);

   const bool invert = (lsb > msb);
   const unsigned width = 1 + ((invert) ? (lsb - msb) : (msb - lsb));
   assert (netName.size());

   if (!invert) OutPin->setBusId(VLDesign.genBusId(width, lsb, msb, busDirectionDefault));
   else OutPin->setBusId(VLDesign.genBusId(width, msb, lsb, !busDirectionDefault));
   OutPin->setName(getLegalName(netName));  // Rename if Needed
   ((CREMap->outPinMap).top()).insert(netName, OutPin);
   return OutPin;
}

// Function checkNet : As net has been created, check bus consistency
bool
CktModule::checkNet(const string& netName, const unsigned& msb, const unsigned& lsb) const {
   return checkNet(((CREMap->outPinMap).top()).getSharPin(netName), msb, lsb);
}

// Function checkNet : As net has been created, check bus consistency
bool
CktModule::checkNet(const CktOutPin* const OutPin, const unsigned& msb, const unsigned& lsb) const {
   assert (OutPin);
   const SynBus* bus = VLDesign.getBus(OutPin->getBusId()); assert (bus);
   if (bus->isInverted()) {
      if (bus->getEnd() != (int)msb || bus->getBegin() != (int)lsb) {
         Msg(MSG_WAR) << "Bus width [" << lsb << ":" << msb << "] is inconsistent with previous declaration [" 
                      << bus->getEnd() << ":" << bus->getBegin() << "] !!!" << endl;
         return false;
      }
   }
   else {
      if (bus->getEnd() != (int)lsb || bus->getBegin() != (int)msb) {
         Msg(MSG_WAR) << "Bus width [" << lsb << ":" << msb << "] is inconsistent with previous declaration [" 
                      << bus->getBegin() << ":" << bus->getEnd() << "] !!!" << endl;
         return false;
      }
   }
   return true;
}

bool
CktModule::createFFCell(const string& outName, const string& inName, const bool& sync, const string& clk, const bool& inv_reset, const string& reset, const string& init) {
   assert (outName.size()); assert (inName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* inOutPin = ((CREMap->outPinMap).top()).getSharPin(inName); assert (inOutPin);
   if (OutPin->getBusWidth() != inOutPin->getBusWidth())
      Msg(MSG_WAR) << "Different I/O BW : "
                   << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                   << "input \"" << inName << "\" (" << inOutPin->getBusWidth() << " !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   CktInPin *inPin = new CktInPin(), *dInPin = new CktInPin();
   CktOutPin *clkOutPin = 0, *resetOutPin = 0, *initOutPin = 0;
   CktDffCell* cell = new CktDffCell(false, dInPin, OutPin); assert (cell);

   if (!clk.size()) {
      Msg(MSG_WAR) << "Missing Clock Signal for DFF !!!" << endl;
      return false;
   }
   else {
      clkOutPin = ((CREMap->outPinMap).top()).getSharPin(clk); assert (clkOutPin);
      if (clkOutPin->getBusWidth() != 1)
         Msg(MSG_WAR) << "Invalid Input BW : " << outName << "\" : "
                      << "Clock Signal \"" << clk << "\" has bus width = " << clkOutPin->getBusWidth() << " !!!" << endl;
      inPin = new CktInPin();  // For clk signal
      inPin->connOutPin(clkOutPin); clkOutPin->connInPin(inPin);
      inPin->connCell(cell); cell->connClk(inPin);
   }
   if (!reset.size()) 
      Msg(MSG_WAR) << "Missing Reset Signal for DFF !!!" << endl;
   else {
      resetOutPin = ((CREMap->outPinMap).top()).getSharPin(reset); assert (resetOutPin);
      if (resetOutPin->getBusWidth() != 1)
         Msg(MSG_WAR) << "Invalid Input BW : \"" << outName << "\" : "
                      << "Reset Signal \"" << reset << "\" has bus width = " << resetOutPin->getBusWidth() << " !!!" << endl;
      if (inv_reset) {
         initOutPin = new CktOutPin(); assert (initOutPin);
         initOutPin->setBusId(resetOutPin->getBusId());
         initOutPin->setName(getAuxiliaryName());
         ((CREMap->outPinMap).top()).insert(initOutPin->getName(), initOutPin);
         
         inPin = new CktInPin();
         CktCell* invCell = new CktInvCell(false, inPin, initOutPin); assert (invCell);
         resetOutPin->connInPin(inPin); inPin->connOutPin(resetOutPin); insertCell(invCell);
         resetOutPin = initOutPin; initOutPin = 0;
      }
   }
   if (init.size()) {
      initOutPin = ((CREMap->outPinMap).top()).getSharPin(init); assert (initOutPin);
      if (initOutPin->getBusWidth() != OutPin->getBusWidth())
         Msg(MSG_WAR) << "Different Init BW for FF : "
                      << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                      << "input \"" << init << "\" (" << initOutPin->getBusWidth() << " !!!" << endl;
   }
   else {  // Set all 0 as default for INIT
      string value = myInt2Str(OutPin->getBusWidth()) + "'b0";
      initOutPin = new CktOutPin(); assert (initOutPin);
      initOutPin->setBusId(OutPin->getBusId());
      initOutPin->setName(getAuxiliaryName());
      ((CREMap->outPinMap).top()).insert(initOutPin->getName(), initOutPin);
      Bv2* bv_value = new Bv2(value); assert (bv_value);
      CktConstCell* initCell = new CktConstCell(bv_value->value(), value); assert (initCell);
      initCell->connOutPin(initOutPin); initOutPin->connCell(initCell); insertCell(initCell);
      delete bv_value;
   }

   if (sync && resetOutPin) {  // Synchronous Reset
      CktInPin *fInPin = new CktInPin(), *tInPin = new CktInPin(), *sInPin = new CktInPin();
      assert (fInPin); assert (tInPin); assert (sInPin);
      CktOutPin* muxOutPin = new CktOutPin(); assert (muxOutPin);
      muxOutPin->setBusId(inOutPin->getBusId());
      muxOutPin->setName(getAuxiliaryName());
      ((CREMap->outPinMap).top()).insert(muxOutPin->getName(), muxOutPin);

      CktMuxCell* muxCell = new CktMuxCell(false, fInPin, tInPin, sInPin, muxOutPin); assert (muxCell);
      resetOutPin->connInPin(sInPin); sInPin->connOutPin(resetOutPin);
      inOutPin->connInPin(fInPin); fInPin->connOutPin(inOutPin);
      initOutPin->connInPin(tInPin); tInPin->connOutPin(initOutPin);
      inOutPin = muxOutPin; insertCell(muxCell);
   }
   else {  // Asynchronous Reset
      if (resetOutPin) {
         inPin = new CktInPin();  // For reset signal
         inPin->connOutPin(resetOutPin); resetOutPin->connInPin(inPin);
         inPin->connCell(cell); cell->connReset(inPin);
      }
      inPin = new CktInPin();  // For init signal
      inPin->connOutPin(initOutPin); initOutPin->connInPin(inPin);
      inPin->connCell(cell); cell->connDefault(inPin);
   }
   // D signal
   dInPin->connOutPin(inOutPin); inOutPin->connInPin(dInPin);

   insertCell(cell); _seqCells.push_back(cell);
   return true;
}

bool
CktModule::createLatchCell(const string& outName, const string& inName, const string& load) {
   assert (outName.size()); assert (inName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* inOutPin = ((CREMap->outPinMap).top()).getSharPin(inName); assert (inOutPin);
   if (OutPin->getBusWidth() != inOutPin->getBusWidth())
      Msg(MSG_WAR) << "Different I/O BW : "
                   << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                   << "input \"" << inName << "\" (" << inOutPin->getBusWidth() << " !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   CktInPin *inPin = new CktInPin(), *loadInPin = new CktInPin();
   CktOutPin* loadOutPin = 0;
   CktDlatCell* cell = new CktDlatCell(false, inPin, loadInPin, OutPin); assert (cell);
   inPin->connOutPin(inOutPin); inOutPin->connInPin(inPin); insertCell(cell);

   if (!load.size())
      Msg(MSG_WAR) << "Missing Load Signal for DFF !!!" << endl;
   else {
      if (loadOutPin->getBusWidth() != 1)
         Msg(MSG_WAR) << "Invalid Input BW : \"" << outName << "\" : "
                      << "Load Signal \"" << load << "\" has bus width = " << loadOutPin->getBusWidth() << " !!!" << endl;
      loadOutPin = ((CREMap->outPinMap).top()).getSharPin(load); assert (loadOutPin);
      loadOutPin->connInPin(loadInPin); loadInPin->connOutPin(loadOutPin);
   }
   _seqCells.push_back(cell);
   return true;
}

// Function createOneInputCell : For BUF, INV
bool
CktModule::createOneInputCell(const bool& invert, const string& outName, const string& inName) {
   assert (outName.size()); assert (inName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* inOutPin = ((CREMap->outPinMap).top()).getSharPin(inName); assert (inOutPin);
   if (OutPin->getBusWidth() != inOutPin->getBusWidth())
      Msg(MSG_WAR) << "Different I/O BW : "
                   << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                   << "input \"" << inName << "\" (" << inOutPin->getBusWidth() << " !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   CktInPin* inPin = new CktInPin();
   CktCell* cell = 0;
   if (invert) cell = new CktInvCell(false, inPin, OutPin);
   else cell = new CktBufCell(false, inPin, OutPin);
   assert (cell); inOutPin->connInPin(inPin); inPin->connOutPin(inOutPin); insertCell(cell);
   return true;
}

// Function createOneInputCell : REDUCED (AND, OR, XOR)
bool
CktModule::createOneInputCell(const CktCellType& cellType, const bool& invert, const string& outName, const string& inName) {
   assert (outName.size()); assert (inName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* inOutPin = ((CREMap->outPinMap).top()).getSharPin(inName); assert (inOutPin);
   if (OutPin->getBusWidth() != 1)
      Msg(MSG_WAR) << "Invalid Output BW : \"" << outName << "\" "
                   << "has bus width = " << OutPin->getBusWidth() << " !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   CktInPin* inPin = new CktInPin();
   CktOutPin* outPin = 0;
   CktCell* cell = 0;

   if (invert) {
      outPin = new CktOutPin(); assert (outPin);
      outPin->setBusId(OutPin->getBusId());
      outPin->setName(getAuxiliaryName());
      ((CREMap->outPinMap).top()).insert(outPin->getName(), outPin);
   }
   else outPin = OutPin;

   switch (cellType) {
      case CKT_AND_CELL : 
         cell = new CktAndCell(false, inPin, outPin);
         break;
      case CKT_OR_CELL : 
         cell = new CktOrCell(false, inPin, outPin);
         break;
      case CKT_XOR_CELL : 
         cell = new CktXorCell(false, inPin, outPin);
         break;
      default : 
         Msg(MSG_ERR) << "Invalid Cell Type for Reduced Operator !!!" << endl;
         delete inPin; return false;
   }
   assert (cell);
   inOutPin->connInPin(inPin); inPin->connOutPin(inOutPin); insertCell(cell);

   if (invert) {
      inPin = new CktInPin();
      cell = new CktInvCell(false, inPin, OutPin); assert (cell);
      outPin->connInPin(inPin); inPin->connOutPin(outPin); insertCell(cell);
   }
   return true;
}

// Function createOneInputCell : ONLY for CONST  (Format of value : <width>'<bhd><bit-wise expression>)
bool
CktModule::createOneInputCell(const string& outName, const string& value) {
   assert (outName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   Bv2* bv_value = new Bv2(value); assert (bv_value);
   if (OutPin->getBusWidth() != bv_value->bits())
      Msg(MSG_WAR) << "Bus width (" << bv_value->bits() << ") for net \"" << outName << "\" is inconsistent with previous declaration (" 
                   << OutPin->getBusWidth() << ") !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   CktConstCell* cell = new CktConstCell(bv_value->value(), value); assert (cell);
   cell->connOutPin(OutPin); OutPin->connCell(cell); insertCell(cell);
   delete bv_value;
   return true;
}

// Function createAssocCell : ONLY for 2+ Input Operators that support Associative Laws in positive form
// E.g. : AND, OR, XOR, ADD, MULT  (NAND, NOR, XNOR)
bool
CktModule::createAssocCell(const CktCellType& cellType, const string& outName, const vector<string>& inputs) {
   assert (outName.size()); assert (inputs.size());
   if (cellType != CKT_AND_CELL && cellType != CKT_OR_CELL && cellType != CKT_XOR_CELL && 
       cellType != CKT_ADD_CELL && cellType != CKT_MULT_CELL && 
       cellType != CKT_NAND_CELL && cellType != CKT_NOR_CELL && cellType != CKT_XNOR_CELL) {
      Msg(MSG_ERR) << "Invalid Cell Type for Associative-Capable Operator !!!" << endl;
      return false;
   }
   bool invert = (cellType == CKT_NAND_CELL || cellType == CKT_NOR_CELL || cellType == CKT_XNOR_CELL);
   if (inputs.size() == 1) return createOneInputCell(invert, outName, *(inputs.begin()));
   else if (inputs.size() == 2) return createTwoInputCell(cellType, outName, *(inputs.begin()), *(++(inputs.begin())));
   assert (inputs.size() > 2);

   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* outPin = 0;
   if (invert) {
      if (OutPin->getCell())
         Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;
      outPin = new CktOutPin(); assert (outPin);
      outPin->setBusId(OutPin->getBusId());
      outPin->setName(getAuxiliaryName());
      ((CREMap->outPinMap).top()).insert(outPin->getName(), outPin);
   }
   else outPin = OutPin;

   // Recursive Transform Multiple Input Cell into Cell Chain
   const CktCellType posType = (cellType == CKT_AND_CELL || cellType == CKT_NAND_CELL) ? CKT_AND_CELL : 
                               (cellType == CKT_OR_CELL || cellType == CKT_NOR_CELL) ? CKT_OR_CELL : 
                               (cellType == CKT_XOR_CELL || cellType == CKT_XNOR_CELL) ? CKT_XOR_CELL : cellType;

   vector<string>::const_iterator it = inputs.begin(), is;
   string in_name = (*it); is = it; ++is;
   CktOutPin* inOutPin = 0;
   while (++it != inputs.end()) {
      if (++is == inputs.end()) {   // Last Input
         if (!createTwoInputCell(posType, outPin->getName(), in_name, *it)) return false;
      }
      else {
         inOutPin = new CktOutPin(); assert (inOutPin);
         inOutPin->setBusId(OutPin->getBusId());
         inOutPin->setName(getAuxiliaryName());
         ((CREMap->outPinMap).top()).insert(inOutPin->getName(), inOutPin);
         if (!createTwoInputCell(posType, inOutPin->getName(), in_name, *it)) return false;
         in_name = inOutPin->getName();
      }
   }

   if (invert) {
      CktInPin* inPin = new CktInPin();
      CktCell* cell = new CktInvCell(false, inPin, OutPin); assert (cell);
      outPin->connInPin(inPin); inPin->connOutPin(outPin); insertCell(cell);
   }
   return true;
}

// Function createTwoInputCell : SUB, DIV, MODULO, SHIFTERS, COMPARATORS Can ONLY be created from this function
bool
CktModule::createTwoInputCell(const CktCellType& cellType, const string& outName, const string& inName1, const string& inName2) {
   assert (outName.size()); assert (inName1.size()); assert (inName2.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* inOutPin1 = ((CREMap->outPinMap).top()).getSharPin(inName1); assert (inOutPin1);
   CktOutPin* inOutPin2 = ((CREMap->outPinMap).top()).getSharPin(inName2); assert (inOutPin2);
   if (OutPin->getBusWidth() != inOutPin1->getBusWidth())
      Msg(MSG_WAR) << "Different I/O BW : "
                   << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                   << "input \"" << inName1 << "\" (" << inOutPin1->getBusWidth() << " !!!" << endl;
   if (OutPin->getBusWidth() != inOutPin2->getBusWidth())
      Msg(MSG_WAR) << "Different I/O BW : "
                   << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                   << "input \"" << inName2 << "\" (" << inOutPin2->getBusWidth() << " !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   CktInPin *inPin1 = new CktInPin(), *inPin2 = new CktInPin();
   CktCell* cell = 0;

   switch (cellType) {
      case CKT_AND_CELL : 
         cell = new CktAndCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_OR_CELL : 
         cell = new CktOrCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_NAND_CELL : 
         cell = new CktNandCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_NOR_CELL : 
         cell = new CktNorCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_XOR_CELL : 
         cell = new CktXorCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_ADD_CELL : 
         cell = new CktAddCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_SUB_CELL : 
         cell = new CktSubCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_MULT_CELL : 
         cell = new CktMultCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_DIV_CELL : 
         cell = new CktDivCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_MODULO_CELL : 
         cell = new CktModuloCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_SHL_CELL : 
         cell = new CktShlCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_SHR_CELL : 
         cell = new CktShrCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_EQUALITY_CELL : 
         cell = new CktEqualityCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_GEQ_CELL : 
         cell = new CktGeqCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_GREATER_CELL : 
         cell = new CktGreaterCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_LEQ_CELL : 
         cell = new CktLeqCell(false, inPin1, inPin2, OutPin);
         break;
      case CKT_LESS_CELL : 
         cell = new CktLessCell(false, inPin1, inPin2, OutPin);
         break;
      default : 
         Msg(MSG_ERR) << "Invalid Cell Type for Two Input Operator !!!" << endl;
         delete inPin1; delete inPin2; return false;
   }
   assert (cell);

   inPin1->connOutPin(inOutPin1); inOutPin1->connInPin(inPin1);
   inPin2->connOutPin(inOutPin2); inOutPin2->connInPin(inPin2);
   insertCell(cell);
   return true;
}

bool
CktModule::createMuxCell(const string& outName, const string& fName, const string& tName, const string& sName) {
   assert (outName.size()); assert (fName.size()); assert (tName.size()); assert (sName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* fOutPin = ((CREMap->outPinMap).top()).getSharPin(fName); assert (fOutPin);
   CktOutPin* tOutPin = ((CREMap->outPinMap).top()).getSharPin(tName); assert (tOutPin);
   CktOutPin* sOutPin = ((CREMap->outPinMap).top()).getSharPin(sName); assert (sOutPin);
   if (OutPin->getBusWidth() != fOutPin->getBusWidth())
      Msg(MSG_WAR) << "Different I/O BW : "
                   << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                   << "input \"" << fName << "\" (" << fOutPin->getBusWidth() << " !!!" << endl;
   if (OutPin->getBusWidth() != tOutPin->getBusWidth())
      Msg(MSG_WAR) << "Different I/O BW : "
                   << "output \"" << outName << "\" (" << OutPin->getBusWidth() << ") != "
                   << "input \"" << tName << "\" (" << tOutPin->getBusWidth() << " !!!" << endl;
   if (sOutPin->getBusWidth() != 1)
      Msg(MSG_WAR) << "Invalid Input BW : \"" << outName << "\" : "
                   << "Select Signal \"" << sName << "\" has bus width = " << sOutPin->getBusWidth() << " !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   CktInPin *fInPin = new CktInPin(), *tInPin = new CktInPin(), *sInPin = new CktInPin();
   CktMuxCell* cell = new CktMuxCell(false, fInPin, tInPin, sInPin, OutPin); assert (cell);
   fInPin->connOutPin(fOutPin); fOutPin->connInPin(fInPin);
   tInPin->connOutPin(tOutPin); tOutPin->connInPin(tInPin);
   sInPin->connOutPin(sOutPin); sOutPin->connInPin(sInPin);
   insertCell(cell);
   return true;
}

bool
CktModule::createSplitCell(const string& outName, const string& inName, const unsigned& msb, const unsigned& lsb) {
   assert (outName.size()); assert (inName.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   CktOutPin* inOutPin = ((CREMap->outPinMap).top()).getSharPin(inName); assert (inOutPin);

   const bool invert = (lsb > msb);
   const unsigned width = 1 + ((invert) ? (lsb - msb) : (msb - lsb));
   if (OutPin->getBusWidth() != width)
      Msg(MSG_WAR) << "Bus width (" << width << ") for net \"" << outName << "\" is inconsistent with previous declaration (" 
                   << OutPin->getBusWidth() << ") !!!" << endl;
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;

   vector<CktOutPin*> outPinAry; outPinAry.clear(); outPinAry.push_back(OutPin);
   CktInPin* inPin = new CktInPin();
   CktSplitCell* cell = new CktSplitCell(false, inPin, outPinAry); assert (cell);
   if (!invert) cell->insertBusId(VLDesign.genBusId(width, lsb, msb, busDirectionDefault));
   else cell->insertBusId(VLDesign.genBusId(width, msb, lsb, !busDirectionDefault));
   inPin->connOutPin(inOutPin); inOutPin->connInPin(inPin); insertCell(cell);
   return true;
}

bool
CktModule::createMergeCell(const string& outName, const vector<string>& inputs) {
   assert (outName.size()); assert (inputs.size());
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(outName); assert (OutPin);
   if (OutPin->getCell())
      Msg(MSG_WAR) << "Multiple Drive Net \"" << outName << "\" Found !!!" << endl;
   if (inputs.size() == 1) return createOneInputCell(false, outName, *(inputs.begin()));
   
   unsigned width = OutPin->getBusWidth(), in_width;
   vector<CktOutPin*> inPins; inPins.clear();
   vector<string>::const_iterator it;
   CktOutPin* inOutPin;
   for (it = inputs.begin(); it != inputs.end(); ++it) {
      inOutPin = ((CREMap->outPinMap).top()).getSharPin(*it); assert (inOutPin);
      in_width = inOutPin->getBusWidth();
      inPins.push_back(inOutPin);
      if (width >= in_width) width -= in_width;
      else break;
   }
   if (it != inputs.end()) {
      Msg(MSG_ERR) << "Total concat bit-width of \"" << outName << "\" is larger than its bit-width ("
                   << OutPin->getBusWidth() << ") !!!" << endl;
      return false;
   }
   else if (width) {
      Msg(MSG_ERR) << "Total concat bit-width of \"" << outName << "\" is less than its bit-width ("
                   << OutPin->getBusWidth() << ") !!!" << endl;
      return false;
   }

   assert (inputs.size() == inPins.size());
   CktMergeCell* cell = new CktMergeCell(false, OutPin);
   CktInPin* inPin = 0;
   for (vector<CktOutPin*>::iterator is = inPins.begin(); is != inPins.end(); ++is) {
      assert (*is); inPin = new CktInPin();
      inPin->connCell(cell); cell->connInPin(inPin);
      inPin->connOutPin(*is); (*is)->connInPin(inPin);
   }
   insertCell(cell);
   return true;
}

bool
CktModule::createModuleCell(const string& moduleName, const string& instName, const vector<string>& ports) {
   assert (moduleName.size());
   CktModule* module = VLDesign.getCktModule(moduleName);
   if (!module) {
      Msg(MSG_ERR) << "Module \"" << moduleName << "\" has not been created !!!" << endl;
      return false;
   }
   else return createModuleCell(module, instName, ports);
}

bool
CktModule::createModuleCell(CktModule* const module, const string& instName, const vector<string>& ports) {
   assert (module); assert (instName.size());
   assert (ports.size() == module->getIoSize());
   if (module == this) {
      Msg(MSG_ERR) << "Module instance cannot be instantiated from module identical to current module !!!" << endl;
      return false;
   }

   CktModuleCell* moduleCell = 0;
   unsigned moduleNum = 0;
   for (unsigned i = 0; i < _modCells.size(); ++i) {
      moduleCell = static_cast<CktModuleCell*>(_modCells[i]);
      if (moduleCell->getInstName() == instName) {
         Msg(MSG_ERR) << "Instance \"" << instName << "\" exists in current module !!!" << endl;
         return false;
      }
      else if (moduleCell->getModuleName() == module->getModuleName()) ++moduleNum;
   }
   if (moduleNum) VLDesign.CreateInstance(module, instName);
   else VLDesign.UpdateInstance(module->getModuleName(), instName);

   CktModuleCell* cell = new CktModuleCell(); assert (cell);
   cell->setModulePtr(const_cast<CktModule*>(module)); cell->setInstName(instName);
   cell->setModuleName(module->getModuleName());

   CktInPin* inPin = 0;
   CktOutPin* OutPin = 0;
   unsigned i = 0, inNum = 0, outNum = 0, ioNum = 0;
   for (vector<string>::const_iterator it = ports.begin(); it != ports.end(); ++it, ++i) {
      OutPin = ((CREMap->outPinMap).top()).getSharPin(*it); assert (OutPin);
      if (module->_ioTypeList[i] == CKT_IN) {
         assert (module->_ioList[CKT_IN][inNum]);
         if (module->_ioList[CKT_IN][inNum]->getOutPin()->getBusWidth() != OutPin->getBusWidth())
            Msg(MSG_WAR) << "Different Instance I/O BW : "
                         << "inst PI \"" << module->_ioNameList[i] << "\" (" << module->_ioList[CKT_IN][inNum]->getOutPin()->getBusWidth() << ") != "
                         << "drive \"" << (*it) << "\" (" << OutPin->getBusWidth() << " !!!" << endl;
         inPin = new CktInPin(); inPin->connCell(cell); cell->connInPin(inPin);
         OutPin->connInPin(inPin); inPin->connOutPin(OutPin); ++inNum;
      }
      else if (module->_ioTypeList[i] == CKT_OUT) {
         assert (module->_ioList[CKT_OUT][outNum]);
         if (module->_ioList[CKT_OUT][outNum]->getOutPin()->getBusWidth() != OutPin->getBusWidth())
            Msg(MSG_WAR) << "Different Instance I/O BW : "
                         << "inst PO \"" << module->_ioNameList[i] << "\" (" << module->_ioList[CKT_OUT][outNum]->getOutPin()->getBusWidth() << ") != "
                         << "load \"" << (*it) << "\" (" << OutPin->getBusWidth() << " !!!" << endl;
         if (OutPin->getCell())
            Msg(MSG_WAR) << "Multiple Drive Net \"" << (*it) << "\" Found !!!" << endl;
         OutPin->connCell(cell); cell->connOutPin(OutPin); ++outNum;
      }
      else {
         assert (module->_ioTypeList[i] == CKT_IO);
         assert (module->_ioList[CKT_IO][ioNum]);
         if (module->_ioList[CKT_IO][ioNum]->getOutPin()->getBusWidth() != OutPin->getBusWidth())
            Msg(MSG_WAR) << "Different Instance I/O BW : "
                         << "inst PIO \"" << module->_ioNameList[i] << "\" (" << module->_ioList[CKT_IO][ioNum]->getOutPin()->getBusWidth() << ") != "
                         << "load \"" << (*it) << "\" (" << OutPin->getBusWidth() << " !!!" << endl;
         if (OutPin->getCell())
            Msg(MSG_WAR) << "Multiple Drive Net \"" << (*it) << "\" Found !!!" << endl;
         OutPin->connCell(cell); cell->connOutPin(OutPin); ++ioNum;
      }
   }
   insertCell(cell); _modCells.push_back(cell);
   return true;
}

#endif
