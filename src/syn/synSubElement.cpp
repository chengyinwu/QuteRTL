/****************************************************************************
  FileName     [ synSubElement.cpp ]
  Package      [ syn ]
  Synopsis     [ Synthesis function of subelement classes ]
  Author       [ Chun-Fu(Joe) Huang, Hu-Hsi Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_SUB_ELEMENT_SOURCE
#define SYN_SUB_ELEMENT_SOURCE

//---------------------------
//  system include
//---------------------------
#include <vector>
#include <string>

using namespace std;
//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktModuleCell.h"
#include "cktLogicCell.h"

//---------------------------
//  Global variables
//---------------------------
extern VlpDesign&       VLDesign;   
extern SYN_usage*       SYN;
extern const VlgModule* curVlpModule;
extern bool             busDirectionDefault;

//functions

void
VlpInstance::posMapping(CktModuleCell*& moduleCell) const
{
   // Note : This function generates I/O Pins corresponds to parameter overloaded instance with position mapping.
   CktOutPin*   outpin;
   CktInPin*    inpin;
   string       ioName;
   VlpPortNode* port;
   string modName = BaseModule :: getName(_moduleID);
   const BaseModule* vlpModule = VLDesign.getModule(modName);

   for (int i = 0; i < vlpModule->getIoSize(); ++i) { 
      ioName = BaseModule :: getName(vlpModule->getIoSignal(i)->getNameId());
      vlpModule->getPort(ioName, port);
      if (port->getIoType() == input) {
         outpin = _pArray[i]->synthesis();
         inpin = new CktInPin;
         inpin->connCell(moduleCell); moduleCell->connInPin(inpin);
         outpin->connInPin(inpin);    inpin->connOutPin(outpin);
      }
      else if ((port->getIoType() == output) || (port->getIoType() == inout)) {
         assert (_pArray[i]->getNodeClass() == NODECLASS_SIGNAL);
         outpin = static_cast<const VlpSignalNode*>(_pArray[i])->synModCellOutPin();
         moduleCell->connOutPin(outpin); outpin->connCell(moduleCell);
      }
      else Msg(MSG_ERR) << "Unknown I/O Type for instance " << moduleCell->getInstName() << " (Type = " << port->getIoType() << ") !!!" << endl;
   }
}

void
VlpInstance::handleConcatOut(CktModuleCell*& moduleCell, unsigned& nArrCtr) const
{  
   // Note : When instance with outPin NodeType != VlpSignalNode
   // Note : The function is similar to VlpBAorNBA_Node::setConcatFin
   // hadle the case "ADDER a1(.out({a[3:2], b[1]}), ...);"
   const VlpBaseNode* left = _pArray[nArrCtr];
   vector<unsigned> busIdAry;  // only be used in "{ } = ..."
   vector<string>   sigNameAry;
   unsigned lhsWidth = 0;

   assert (left->getNodeClass() == NODECLASS_OPERATOR);
   assert ((static_cast<const VlpOperatorNode*>(left))->getOperatorClass() == OPCLASS_CONCATENATE);
   (static_cast<const VlpConcatenateNode*>(left))->synLhsConcatenate(busIdAry, sigNameAry, lhsWidth);

   int id = VLDesign.genBusId(lhsWidth, 0, lhsWidth-1, busDirectionDefault);
   CktOutPin* mCellOut = new CktOutPin;
   mCellOut->setBusId(id);
   moduleCell->connOutPin(mCellOut); mCellOut->connCell(moduleCell);

   CktInPin*    newInPin = new CktInPin();
   CktSplitCell* spCell = new CktSplitCell();
   spCell->setNonSpFin();
   SYN->insertCktCell(spCell);
   spCell->connInPin(newInPin);   newInPin->connCell(spCell);
   mCellOut->connInPin(newInPin); newInPin->connOutPin(mCellOut);

   CktOutPin* outPin;
   const SynBus* iBus;
   int msb = lhsWidth;
   unsigned tmpBusId, iWidth;
   for (unsigned i = 0; i < busIdAry.size(); ++i) {
      outPin = new CktOutPin();
      spCell->connOutPin(outPin); outPin->connCell(spCell);
      outPin->setBusId(busIdAry[i]);
      iBus = VLDesign.getBus(busIdAry[i]);
      iWidth = iBus->getWidth();
      tmpBusId = VLDesign.genBusId(iWidth, msb-iWidth, msb-1, busDirectionDefault);
      spCell->insertBusId(tmpBusId);
      msb -= iWidth;

      if (curVlpModule->isMemAry(sigNameAry[i])) { // SynMemory is generated @ synLhsConcatenate
         const VlpSignalNode* signal = (static_cast<const VlpConcatenateNode*>(left))->getConcatMemSignal(i);
         if (signal->isConstIndex())
            SYN->synVarList.setMemFin(sigNameAry[i], outPin, const_cast<VlpBaseNode*>(left), signal->getMemIndex());
         else  // variable index
            SYN->synVarList.setMemFin(sigNameAry[i], outPin, const_cast<VlpBaseNode*>(left), 0.5, signal->getMsb());
      }
      else
         SYN->synVarList.setVarFin(sigNameAry[i], outPin, const_cast<VlpBaseNode*>(left));
   }
}

void
VlpInstance::nameMapping(CktModuleCell*& moduleCell) const
{
   CktOutPin*   outpin;
   CktInPin*    inpin;
   VlpPortNode* port;
   string moduleName = BaseModule :: getName(_moduleID);
   const BaseModule* vlpModule = VLDesign.getModule(moduleName);
   string ioName;

   for (int i = 0; i < vlpModule->getIoSize(); ++i) {
      ioName = BaseModule :: getName(vlpModule->getIoSignal(i)->getNameId());
      vlpModule->getPort(ioName, port);
      unsigned nArrCtr;
      for (nArrCtr = 0; nArrCtr < _nArray.size(); ++nArrCtr)
         if (BaseModule :: getName(_nArray[nArrCtr]) == ioName)
            break;         
      if (nArrCtr == _nArray.size())
         Msg(MSG_ERR) << "Unknown I/O Name for instance " << moduleCell->getInstName() << " (name = " << ioName << ") !!!" << endl;
      if (port->getIoType() == input) {
         if (_pArray[nArrCtr] != 0) {
            outpin = _pArray[nArrCtr]->synthesis();
            inpin = new CktInPin;
            outpin->connInPin(inpin);    inpin->connOutPin(outpin);
            inpin->connCell(moduleCell); moduleCell->connInPin(inpin);
         }
         else { 
            Msg(MSG_WAR) << "Instance with Floating PI : " << moduleCell->getInstName() << "." << ioName << endl;
            outpin = 0; inpin = new CktInPin;
            inpin->connOutPin(outpin);
            inpin->connCell(moduleCell); moduleCell->connInPin(inpin);
         }
      }
      else if ((port->getIoType() == output) || (port->getIoType() == inout)) {
         if (_pArray[nArrCtr] != 0) {
            if (_pArray[nArrCtr]->getNodeClass() == NODECLASS_SIGNAL) {
               outpin = static_cast<const VlpSignalNode*>(_pArray[nArrCtr])->synModCellOutPin();
               moduleCell->connOutPin(outpin); outpin->connCell(moduleCell);
            }
            else handleConcatOut(moduleCell, nArrCtr);
         }
         else {
            Msg(MSG_WAR) << "Instance with High Impedence PO : " << moduleCell->getInstName() << "." << ioName << endl;
            outpin = 0; moduleCell->connOutPin(outpin);
         }
      }
      else Msg(MSG_ERR) << "Unknown I/O Type for instance " << moduleCell->getInstName() << " (Type = " << port->getIoType() << ") !!!" << endl;
   }
}

void 
VlpInstance::synthesis() const
{
   CktModuleCell* modCell = new CktModuleCell;
   SYN->insertCktCell(modCell);
   modCell->setInstName(_name);

   // 1. Perform I/O Mapping and Synthesize into Corresponding I/O Pins
   // 2. Insert I/O consistent with the same order defined in the original module
   if (_isNameMap == 0) posMapping(modCell);  // Mapped by Positions
   else                 nameMapping(modCell);
   
   // Set Name for ModuleCell : Generate new module name if parameters are overloaded
   string newModName;
   bool isGenNewName = genModName(newModName);

   if (isGenNewName) modCell->setModuleName(newModName);
   else              modCell->setModuleName(BaseModule :: getName(_moduleID));
}

bool
VlpInstance::genModName(string& newModName) const
{
   // Note : This function generates a new name for moduleCell if parameters are overloaded
   bool isPOL = false;
   string modName = BaseModule :: getName(_moduleID);
   const ParamAry* paramDeclation = static_cast<VlgModule*>(VLDesign.getModule(modName))->getParamDeclaration();
   stringstream nameStream;   
   nameStream << modName;
   ParamAry np;
   bool exist = curVlpModule->getParamOL(_name, np);

   // 1. Parameter overloaded by Position Mapping
   if (_polAry.size() > 0) {
      //assert (VLDesign.getModule(modName)->isBlackBox() == false);
      for (unsigned i = 0; i < _polAry.size(); ++i) {
         if ((!isPOL) && (_polAry[i]->constPropagate() != paramDeclation->at(i)->content->constPropagate()))
            isPOL = true;

         nameStream << (SYN->newModuleNameMark) << paramDeclation->at(i)->param
                    << _polAry[i]->constPropagate();
      }
   }
   // 2. Parameter overloaded by Name Mapping
   if (exist) {
      //assert (VLDesign.getModule(modName)->isBlackBox() == false);
      unsigned j = 0;
      for (unsigned i = 0; i < paramDeclation->size(); ++i) {
         for (j = 0; j < np.size(); ++j) {
            if (paramDeclation->at(i)->param == np[j]->param) {
               if ((!isPOL) && (np[j]->content->constPropagate() != paramDeclation->at(i)->content->constPropagate())) 
                  isPOL = true;                  
               nameStream << (SYN->newModuleNameMark) << np[j]->param 
                          << np[j]->content->constPropagate();
               break;
            }
         }
         if (j == np.size()) { // Un-Overload Parameter
            nameStream << (SYN->newModuleNameMark) << paramDeclation->at(i)->param
                       << paramDeclation->at(i)->content->constPropagate();
         }
      }
   }
   if (isPOL) newModName = nameStream.str();
   return isPOL;
}

void 
VlpFunction::synthesis() const
{
   const VlpBaseNode* state = _statement;
   while (state != 0) {
      state->varFlatten();
      state = state->getNext();
   }
}

void
VlpPrimitive::synthesis() const
{
   // Primitive Types : 
   // default : 0, and : 1, nand : 2, or : 3, nor : 4, xor : 5, 
   // xnor : 6, buf : 7, bufif0 : 8, bufif1 : 9, not : 10

   CktOutPin* cellOut;
   // and, nand, or, nor, xor, xnor : one output, multiple inputs
   // The first element is the output, and remainings are inputs
   if ((_kind >= 1) && (_kind <= 6)) {
      assert (_pArray.size() >= 2);
      if (_pArray.size() == 2)  // Single Input
         cellOut = _pArray[1]->synthesis();
      else {  // Multiple Inputs
         CktOutPin* finOut1 = _pArray[1]->synthesis();
         CktOutPin* finOut2 = _pArray[2]->synthesis();
         cellOut = genCellPrimitive(finOut1, finOut2, _kind);
	 // Handle with Inputs > 2
         for (unsigned i = 3; i < _pArray.size(); ++i) {
            finOut1 = cellOut;
            finOut2 = _pArray[i]->synthesis();
            cellOut = genCellPrimitive(finOut1, finOut2, _kind);
         }
      }
      assert (_pArray[0]->getNodeClass() == NODECLASS_SIGNAL);
      const VlpSignalNode* out = static_cast<const VlpSignalNode*>(_pArray[0]);
      string name = BaseModule :: getName(out->getNameId());
      
      // Generate synVar for this outPin
      cellOut->setBusId(out->genLhsBusId());
      // Insert This OutPin with its BaseNode into synVarList
      SYN->synVarList.setVarFin(name, cellOut, const_cast<VlpSignalNode*>(out));
   }
   // buf, not : one input, multiple output
   // The last element is the input, and remainings are outputs
   else if ( (_kind == 7) || (_kind == 10) ) {
      CktOutPin* finOut = _pArray[_pArray.size() - 1]->synthesis();
      CktInPin* tmpInpin;
      const VlpSignalNode* out;
      string name;
      for (unsigned i = 0; i < (_pArray.size() - 1); ++i) {
         if (_kind == 7) SYN->insertCktCell(new CktBufCell(true, tmpInpin, cellOut));
         else            SYN->insertCktCell(new CktInvCell(true, tmpInpin, cellOut));

         finOut->connInPin(tmpInpin); tmpInpin->connOutPin(finOut);
         assert (_pArray[i]->getNodeClass() == NODECLASS_SIGNAL);
         out = static_cast<const VlpSignalNode*>(_pArray[i]);
         name = BaseModule :: getName(out->getNameId());
         // Generate synVar for this outPin
         cellOut->setBusId(out->genLhsBusId());
	 // Insert This OutPin with its BaseNode into synVarList
         SYN->synVarList.setVarFin(name, cellOut, const_cast<VlpSignalNode*>(out));
      }
   }
   else if (_kind == 8 || _kind == 9)
      Msg(MSG_WAR) << "Primitive Type \"bufif\" has not supported yet !!!" << endl;
   else
      Msg(MSG_ERR) << "Unknown Primitive Type (Type = " << _kind << ") !!!" << endl;
}

CktOutPin*
VlpPrimitive::genCellPrimitive(CktOutPin* finOut1, CktOutPin* finOut2, int kind) const
{
   CktOutPin* cellOut;
   CktInPin *cellIn1, *cellIn2;

   //and : 1, nand : 2, or : 3, nor : 4, xor : 5, xnor : 6
   if (kind == 1)      //and
      SYN->insertCktCell(new CktAndCell(true, cellIn1, cellIn2, cellOut));
   else if (kind == 2) //nand
      SYN->insertCktCell(new CktNandCell(true, cellIn1, cellIn2, cellOut));
   else if (kind == 3) //or
      SYN->insertCktCell(new CktOrCell(true, cellIn1, cellIn2, cellOut));
   else if (kind == 4) //nor
      SYN->insertCktCell(new CktNorCell(true, cellIn1, cellIn2, cellOut));
   else if (kind == 5) //xor
      SYN->insertCktCell(new CktXorCell(true, cellIn1, cellIn2, cellOut));
   else if (kind == 6) //xnor
      SYN->insertCktCell(new CktXnorCell(true, cellIn1, cellIn2, cellOut));
   else
      Msg(MSG_ERR) << "Unable to Generate Cell : Unknown Primitive Type (Type = " << _kind << ") !!!" << endl;

   finOut1->connInPin(cellIn1); cellIn1->connOutPin(finOut1);
   finOut2->connInPin(cellIn2); cellIn2->connOutPin(finOut2);
   cellOut->setBusId(VLDesign.genBusId(1, 0, 0, false));
   return cellOut;
}

CktIoType
VlpPortNode::ioTypeVlp2Ckt() const
{
   if (getIoType() == input)
      return CKT_IN;
   else if (getIoType() == output)
      return CKT_OUT;
   else if (getIoType() == inout)
      return CKT_IO;
   else
      return CKT_TOTAL;
}

string
VlpPortNode::NetType2Str() const
{
   int type = getNetType();
   if (type == 1)
      return "wire";
   else if (type == 2)
      return "reg";
   else if (type == 3)
      return "wand";
   else if (type == 4)
      return "wor";
   else if (type == 5)
      return "tri";
   else if (type == 6)
      return "tri1";
   else if (type == 7)
      return "tri0";
   else if (type == 8)
      return "triand";
   else if (type == 9)
      return "trior";
   else if (type == 10)
      return "supply0";
   else if (type == 11)
      return "supply1";
   else if (type == 12)
      return "swire";
   else
      return "error";
}

int
VlpPortNode::port2Bus() const
{
   int busId;
   if (_msb != 0 && _lsb != 0) {
      int msbNum = _msb->constPropagate();
      int lsbNum = _lsb->constPropagate();

      if (msbNum == -1)
         Msg(MSG_ERR) << "Port MSB Cannot be Elaborated !!!" << endl;
      else if (lsbNum == -1)
         Msg(MSG_ERR) << "Port LSB Cannot be Elaborated !!!" << endl;
      else if (msbNum > lsbNum)
         busId = VLDesign.genBusId(msbNum - lsbNum + 1, lsbNum, msbNum, true);
      else if (msbNum == lsbNum)
         busId = VLDesign.genBusId(1, lsbNum, msbNum, busDirectionDefault);
      else // lsbNum > msbNum
         busId = VLDesign.genBusId(lsbNum - msbNum + 1, msbNum, lsbNum, false);
   }
   else busId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
   return busId;
}

int
VlpMemAry::getBusId()
{
   // Exactly the same with VlpPortNode::port2Bus() ?!
   int busId;
   if (_msb != 0 && _lsb != 0) {
      int msbNum = _msb->constPropagate();
      int lsbNum = _lsb->constPropagate();

      if (msbNum == -1)
         Msg(MSG_ERR) << "Memory MSB Cannot be Elaborated !!!" << endl;
      if (lsbNum == -1)
         Msg(MSG_ERR) << "Memory LSB Cannot be Elaborated !!!" << endl;
      if (msbNum > lsbNum)
         busId = VLDesign.genBusId(msbNum - lsbNum + 1, lsbNum, msbNum, true);
      else if (msbNum == lsbNum)
         busId = VLDesign.genBusId(1, lsbNum, msbNum, busDirectionDefault);
      else //lsbNum > msbNum
         busId = VLDesign.genBusId(lsbNum - msbNum + 1, msbNum, lsbNum, false);
   }
   else busId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
   return busId;
}

#endif
