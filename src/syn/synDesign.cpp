/****************************************************************************
  FileName     [ synDesign.cpp ]
  Package      [ syn ]
  Synopsis     [ Synthesis function of vlpDesign class ]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_DESIGN_SOURCE
#define SYN_DESIGN_SOURCE

//---------------------------
//  system include
//---------------------------
#include <fstream>
#include <string>
#include <iostream>
//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktModule.h"
#include "vlpDesign.h"

#include "util.h"

using namespace std;
//---------------------------
//  Global variables
//---------------------------
extern SYN_usage*       SYN;
extern CktModule*       curCktModule;
extern int              netNumber;
extern const VlgModule* curVlpModule;
extern VlpDesign&       VLDesign;
//===================================================//

#ifndef BUS_DIRECTION
//that is, default direction is true.
bool busDirectionDefault = true;
#else
bool busDirectionDefault = false;
#endif

//========== class VlpDesign  ==========//
void
VlpDesign::initCktModule(string& newModuleName)
{
   netNumber = 0;
   curCktModule = new CktModule;
   setModule(curCktModule);
   /* If a module is instantiated with parameter overloading, I'll generate a new module for it
      eg.
         module A();
            B (4) jojo();
         endmodule
               
         module B();
            parameter width= 8;
         endmodule
          
      I will generate a new module Bwidth4, original module name followed by parameter name and its value.
      Inside module B and Bwidth4, no parameters will exist.                      */               
   if (newModuleName == "") {
      string moduleName = curVlpModule->getModuleName();
      curCktModule->setModuleName(moduleName);
   }
   else
      curCktModule->setModuleName(newModuleName);

   curCktModule->setVlpModule(curVlpModule);
}

bool
VlpDesign::synthesis()
{
   CktModule* cktModule;
   string     topName;
   int num = 0;

   SYN = new SYN_usage();
   Msg(MSG_IFO) << "=== Start Synthesis" << " ..." << endl;
   // It is redundant. These code are generated under old coding styles.
   // But we can't delete it,
   VLDesign.genBusId(0, -1, -1, false);  // generate error busId
   // For CktConstCell with an integer without width declaration
   VLDesign.genBusId(0, 0, 0, busDirectionDefault); // generate integer default id

   // Perform Synthesis to Each Module
   // Note that we can also alter to syn from top module only
   for (unsigned i = 0; i < _vlgModuleAry.size(); ++i) {
      curVlpModule = _vlgModuleAry[i]; 
      SYN->curPOLAry = 0;
      
      // Get Parameter Overloading
      POLMap::const_iterator it;
      if (curVlpModule->getPOLMap() != 0) 
         it = curVlpModule->getPOLMap()->begin();

      // Synthesize original module and each overloaded modules
      Msg(MSG_IFO) << " > Synthesizing Module : " << curVlpModule->getModuleName() << " ..." << endl;
      string newModuleName = "";
      do {
         // 1.  initialization
         initCktModule(newModuleName);
         // 2.  create input/output/inout cells.
         curVlpModule->buildIOCell();
         // 3.  create cells within module.
         curVlpModule->synthesis(); 
         // 4.  generate newModuleName, setup SYN->curPOLAry(overload info.), and clear temp data
         newModuleName = curVlpModule->genPOLInfo(it);
         SYN->outPinMap.clear();
         SYN->constAry.clear();
         SYN->synVarList.clear();
      } while (curVlpModule->getPOLMap() != 0 && it != curVlpModule->getPOLMap()->end());
   }
	
   Msg(MSG_IFO) << "=== Cell Collection and Naming" << " ..." << endl;
   // [ToCheck]synthesis lib module
   for (unsigned i = 0; i < _cktModuleAry.size(); ++i) {
      cktModule = getCktModule(i);
      cktModule->collectCell();
      cktModule->nameNoNamePin(num);//num will increase within the function
   }
   Msg(MSG_IFO) << "=== Set CktModule" << " ..." << endl;
   topName = _topModule->getModuleName();
   cktModule = getCktModule(topName);
   cktModule->setModCell();
   _designHier.setCktModule();

   Msg(MSG_IFO) << "==================================================" << endl;
   Msg(MSG_IFO) << "==> Total " << _cktModuleAry.size() << " Module(s)" << endl;
   Msg(MSG_IFO) << "==> Top Module : " << topName << endl;
   return true;
}

bool
VlpDesign::flatten()
{
   CktModule* cktModule;
   string topName;
	
   Msg(MSG_IFO) << "==================================================" << endl;
   Msg(MSG_IFO) << "===>          Module Instance Summary         <===" << endl;
   Msg(MSG_IFO) << "     ==================== ===================="     << endl;
   Msg(MSG_IFO) << "     ModuleName :         InstanceName : "          << endl;
   Msg(MSG_IFO) << "     ==================== ===================="     << endl;

   for (unsigned i = 0; i < _cktModuleAry.size(); ++i) {
      cktModule = getCktModule(i);
      cout << "     " << setiosflags(ios::left) << setw(19) << cktModule->getModuleName() 
           << " " << cktModule->getRefCount() << endl;
   }
   Msg(MSG_IFO) << "     ========================================="     << endl;

   //topName = _topModule->getModuleName();
   topName = getHier()->root()->moduleName();  // Fixed by chengyin
   cktModule = getCktModule(topName);

   Msg(MSG_IFO) << "===> Top Module : " << topName << "(" << cktModule << ")" << endl;
   Msg(MSG_IFO) << "==================================================\n" << endl;
   Msg(MSG_IFO) << "=== Start Flatten" << " ..." << endl;

   cktModule->flatten();

   Msg(MSG_IFO) << "==================================================" << endl;
   Msg(MSG_IFO) << "==> Total " << _cktModuleAry.size() << " Module(s)" << endl;
   Msg(MSG_IFO) << "==> Top Module : " << topName << endl;
   return true;
}

bool 
VlpDesign::completeFlatten()
{
   CktModule* cktModule;
   string topName;

   Msg(MSG_IFO) << "==================================================" << endl;
   Msg(MSG_IFO) << "===>          Module Instance Summary         <===" << endl;
   Msg(MSG_IFO) << "     ==================== ===================="     << endl;
   Msg(MSG_IFO) << "     ModuleName :         InstanceName : "          << endl;
   Msg(MSG_IFO) << "     ==================== ===================="     << endl;

   string refCountStr;
   for (unsigned i = 0; i < _cktModuleAry.size(); ++i) {
      cktModule = getCktModule(i);
      refCountStr = "(" + myInt2Str(cktModule->getRefCount()) + ")";
      cout << "     " << setw(15) << left << cktModule->getModuleName() 
           << setw(5) << right << refCountStr << endl;
   }
   Msg(MSG_IFO) << "     ========================================="     << endl;

   topName = getHier()->root()->moduleName();  // Fixed by chengyin
   cktModule = getCktModule(topName);

   Msg(MSG_IFO) << "===> Top Module : " << topName << "(" << cktModule << ")" << endl;
   Msg(MSG_IFO) << "==================================================\n" << endl;
   Msg(MSG_IFO) << "=== Start Complete Flatten" << " ..." << endl;

   _cktFlattenDesign = new CktModule;
   _cktFlattenDesign->setVlpModule(_topModule);
   _cktFlattenDesign->setModuleName(_designHier.root()->moduleName());
   _designHier.flatten(_cktFlattenDesign);

   Msg(MSG_IFO) << "=== Start Collect Cells" << " ..." << endl;
   _cktFlattenDesign->collectCell();
   Msg(MSG_IFO) << "=== Start Buffer Elimination" << " ..." << endl;
   _designHier.updateFltCkt(_cktFlattenDesign); // recover the loss bus info of hierarchy

   Msg(MSG_IFO) << "==================================================" << endl;
   Msg(MSG_IFO) << "==> Total 1 Module" << endl;
   Msg(MSG_IFO) << "==> Top Module : " << _cktFlattenDesign->getModuleName() << endl;
   return true;
}

unsigned 
VlpDesign::genBusId(unsigned width, int begin, int end, bool isInverted)
{
   for (unsigned i = 0; i < _busArr.size(); i++)
      if (_busArr[i]->isEqual(width, begin, end, isInverted))
         return i;

   unsigned id = _busArr.size();
   SynBus* bus = new SynBus(width, begin , end, isInverted);
   _busArr.push_back(bus);
   return id;
}
      
unsigned
VlpDesign::genBusId(const SynBus* bus)
{
   for (unsigned i = 0; i < _busArr.size(); i++) 
      if ((*_busArr[i]) == (*bus))
         return i;
                     
   unsigned id = _busArr.size();
   _busArr.push_back(const_cast<SynBus*>(bus));
   return id;
}

const SynBus* 
VlpDesign::getBus(const unsigned& i) const
{
   if (i >= _busArr.size()) {
      Msg(MSG_ERR) << "Exceeds Bus Array Index (" << i << ", MAX = " << (_busArr.size() - 1) << ") !!!" << endl;
      return 0;
   }
   return _busArr[i];
}

#endif
