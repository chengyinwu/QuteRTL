/****************************************************************************
  FileName     [ synModule.cpp ]
  Package      [ syn ]
  Synopsis     [ Synthesis function of Module Node class ]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_MODULE_SOURCE
#define SYN_MODULE_SOURCE

//---------------------------
//  system include
//---------------------------
#include <string>
#include <iostream>
//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktIoCell.h"
#include "cktModule.h"
#include "vlpModule.h"
//---------------------------
//  Global variables
//---------------------------
using namespace std;

extern SYN_usage*       SYN;
extern VlpDesign&       VLDesign;
extern CktModule*       curCktModule;
extern int              netNumber;
extern const VlgModule* curVlpModule;
//===================================================//
extern bool busDirectionDefault;

//===================== class BaseModule  ====================//
/* For a module, I will create the following format
   |----------------------------|         |-------------------------------|
   |                            |         ||------|   Module  |------|    |
   |                            |         ||PiCell|--o     o--|PiCell|--o |
   |                            |         ||------|           |------|    |
   |                            |         |                               |
   |                            |      \  ||------|           |------|    |
   |           MODULE           | ======\ ||PiCell|--o     o--|PiCell|--o |
   |                            | ======/ ||------|           |------|    |
   |                            |      /  |                               |
   |                            |         ||------|           |-------|   |
   |                            |         ||PiCell|--o     o--|PioCell|--o|
   |                            |         ||------|           |-------|   |
   |                            |         |                               |
   |----------------------------|         |-------------------------------| 

  null |------| name, busId
    *--|PiCell|--o
       |------| sharing in pinMap

 name, busId               |------| name, busId(the same as the finOut of the cell)
    o-------------------*--|PoCell|--o
 sharing in pinMap         |------|


       |-------| name, busId
    *--|PioCell|--o
       |-------| sharing in pinMap
*/

void
BaseModule::buildIOCell() const
{
   VlpPortNode* port = 0;
   CktOutPin    *outPin, *poOut;
   CktInPin*    inPin;
   CktCell*     cell;
   CktIoType    ioType;
   string       ioName;
   int          busId;

   for (int j = 0; j < getIoSize(); ++j) { 
      ioName = BaseModule :: getName( getIoSignal(j)->getNameId() );
      getPort(ioName, port);
      ioType = port->ioTypeVlp2Ckt();
      busId  = port->port2Bus();
      outPin = new CktOutPin;
      outPin->setName(ioName);
      outPin->setBusId(busId);
      SYN->outPinMap.insert(ioName, outPin);

      if (ioType == CKT_IN)
         cell = new CktPiCell(false, outPin);
      else if (ioType == CKT_OUT) {
         cell  = new CktPoCell(true, inPin, poOut);
         poOut->setName(ioName);
         poOut->setBusId(busId);
         inPin->connOutPin(outPin);
         outPin->connInPin(inPin);
      }
      else if (ioType == CKT_IO) {
         inPin = new CktInPin;
         cell  = new CktPioCell(false, inPin, outPin);
      }
      else {
         Msg(MSG_ERR) << "Unknown I/O Type in module " << getModuleName() << " (Type = " << ioType << ") !!!" << endl;
         exit(1);
      }
      SYN->insertCktCell(cell);
      curCktModule->insertIoCell(cell, ioName, ioType);
   }  
}

void 
BaseModule::synDataFlow() const
{
   bool isSeq  = false;
   bool isSync = false;
   CktOutPin* oClk = 0;
   int count = 0;

   for (const VlpBaseNode* dfp = _dataFlowStart; dfp; dfp = dfp->getNext()) {
      assert (dfp->getNodeClass() == NODECLASS_ALWAYS || dfp->getNodeClass() == NODECLASS_ASSIGN);
      isSeq = false;  // Used for VlpAlwaysNode Only. For VlpAssignNode, don't care about it.
      count = SYN->synVarList.size();
      dfp->synthesis(isSeq, isSync, oClk);
      if (dfp->getNodeClass() == NODECLASS_ALWAYS) {  // Always Block
         for (unsigned i = count; i < SYN->synVarList.size(); ++i)
            SYN->synVarList[i]->setState(isSeq, isSync, oClk);
         SYN->synVarList.setMemState(isSync, oClk);
      }
      else {  // Combinational Assignment
         for (unsigned i = count; i < SYN->synVarList.size(); ++i)
            SYN->synVarList[i]->setState(false, false, 0);
      }
   }
}

void 
BaseModule::connLhsRhs() const
{
   SYN->synVarList.connLhsRhs();
}

//================== class LibModule  ====================//
void
LibModule::synSubInstance() const
{
   // Primitive synthesis
   for (unsigned i = 0; i < _primitiveAry.size(); ++i)
      _primitiveAry[i]->synthesis();
}

void
LibModule::synthesis() const
{
   curCktModule->setLibrary();
   if (isBlackBox()) { curCktModule->setBlackBox(); return; }
   // 1. Synthesize Primitives Only
   synSubInstance(); 
   // 2. Synthesize dataFlow
   synDataFlow();  // flatten all variables
   // 3. Synthesize Connections : Lhs and Rhs of all SynVars
   connLhsRhs();
   // Note : During synthesis process, netNumber may increase.
   //        Remember it! When we're writing output, net number will increase by this number.
   curCktModule->setNetNumber(netNumber);
}

//================== class VlgModule  ====================//
void
VlgModule::synSubInstance() const
{
   // Instance Synthesis
   for (unsigned i = 0; i < _instanceAry.size(); ++i)
      _instanceAry[i]->synthesis();
   
   // Primitive Synthesis
   for (unsigned i = 0; i < _primitiveAry.size(); ++i)
      _primitiveAry[i]->synthesis();
}

void 
VlgModule::synthesis() const
{
   // If this module is a library module, synthesis won't applied.
   if (isBlackBox()) { curCktModule->setBlackBox(); return; }
   // 1. Synthesize Primitives and Inatances 
   synSubInstance();
   // 2. Synthesize DataFlow (including always block, assign)
   synDataFlow();  // flatten all variables
   // 3. Synthesize Connections : Lhs and Rhs of all SynVars
   connLhsRhs();
   // Note : During synthesis process, netNumber may increase.
   //        Remember it! When we're writing output, net number will increase by this number.
   curCktModule->setNetNumber(netNumber);
}

/* eg.
      module AA(a, y);
      parameter data=3;
      input a;
      output y;
      reg y;
         always@()
         begin
            y = a + width;
         end
      endmodule
   
      module BB(a, y)
      input a;
      output y;
      reg y;
         AA #(2) m0(a, y);  <--- this  AA instantiation will produce a new module "AA2", 
                                 append a newModuleNameMark and "2" to "AA"
      endmodule
   
        ||
        ||
        \/
      module BB(a, y)
      input a;
      output y;
      reg y;
         AA$2  m0(a, y);  <-- remove parameter overload.
      endmodule */
string
VlgModule::genPOLInfo(POLMap::const_iterator& it) const
{ // return newModuleName and setup SYN->curPOLAry
   string newModuleName = "";
   ParamAry* paramOL;

   if (_POLMap != 0)
      for (; it != _POLMap->end(); ++it) {
         paramOL = it->second;
         newModuleName = genNewModuleName(getModuleName(), paramOL);
         CktModule* tmpCktModule = VLDesign.getCktModule(newModuleName);
         if (tmpCktModule != 0)
            ;
         else {
            SYN->curPOLAry = paramOL;
            break;
         }
      }

   return newModuleName;
}

string
VlgModule::genNewModuleName(string moduleName, ParamAry* POLAry) const
{
   stringstream nameStream;
   nameStream << moduleName;
   const paramPair* tmp;

   for (unsigned i = 0; i < POLAry->size(); ++i) {
      tmp = POLAry->at(i);
      nameStream << (SYN->newModuleNameMark) << tmp->param;

      if (tmp->content->getNodeClass() == NODECLASS_INT)
         nameStream << (static_cast<VlpIntNode*>(tmp->content))->getNum();
      else if (tmp->content->getNodeClass() == NODECLASS_SIGNAL) {
         const VlpSignalNode* sNode = static_cast<const VlpSignalNode*>(tmp->content);
         assert (sNode->isBitBaseStr());
         Bv4 bv(BaseModule::getName(sNode->getNameId()));
         nameStream << bv.value();
      }
      else {
         cerr << "Error : No support parameter overload NodeType" 
              << " @ VlpModule::genNewModuleName()" << endl;
         assert (0);
      }
   }
   return nameStream.str();
}

#endif
