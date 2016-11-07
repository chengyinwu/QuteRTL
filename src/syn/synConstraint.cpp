/****************************************************************************
  FileName     [ synConstraint.cpp ]
  Package      [ syn ]
  Synopsis     [ Constraints generation function of vlpDesign class ]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft(c) 2008 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_CONSTRAINT_SOURCE
#define SYN_CONSTRAINT_SOURCE

//---------------------------
//  system include
//---------------------------
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktModule.h"
#include "vlpDesign.h"

using namespace std;
//---------------------------
//  Global variables
//---------------------------
extern SYN_usage*       SYN;
extern CktModule*       curCktModule;
extern const VlgModule* curVlpModule;
extern VlpDesign&       VLDesign;
extern bool             busDirectionDefault;

bool
VlpDesign::constraintGen()
{
   SYN = new SYN_usage();
   cout << "Constraint Generation Start ..." << endl;
   //It is redundant. These code are generated under old coding styles. But we can't delete it,
   VLDesign.genBusId(0, -1, -1, false);            // generate error busId
   //For CktConstCell with an integer without width declaration
   VLDesign.genBusId(0, 0, 0, busDirectionDefault); // generate integer default id

   for (unsigned i = 0; i < _vlgModuleAry.size(); ++i) { //synthesize each module
      curVlpModule = _vlgModuleAry[i]; 
      SYN->curPOLAry = 0;
      cout << "Targeted Module : " << curVlpModule->getModuleName() << endl;

      //get information about module instantiation with parameter overloading.
      if (curVlpModule->getPOLMap() != 0) {// exist overload module
         cerr << "No Support parameter overload of module!!" << endl;
         assert (0);
      }
      //1.  initialization
      string newModuleName = "";
      initCktModule(newModuleName);
      //2.  create input/output/inout cells.
      curVlpModule->buildIOCell();
      //3.  create cells within module.
      curVlpModule->constraintGen(); 
      curCktModule->setOutPinMap(SYN->outPinMap);
      curCktModule->setSynVars(SYN->synVarList);
   }
   string topName = _topModule->getModuleName();
   CktModule* cktModule = getCktModule(topName);
   
   cktModule->setModCell();   
   if (_designHier.setCktModule() == true)
      cout << "> Set CktModule Complete!!" << endl;

   cout << "Constraint Generation Complete!!" << endl;
   return true;
}

void 
VlgModule::constraintGen() const
{
   // if this module is a library module, synthesis won't applied.
   if (isBlackBox() == true) {
      curCktModule->setBlackBox();
      return;
   }
   //synthesize primitives and inatances 
   synSubInstance();
   //synthesize dataFlow, include always block , assign
   synDataFlow(); // flatten all variables
}

bool
VlpDesign::writeConstraint(const char* sigFile)
{
   //map<string, set<string> > sigsMap;
   //string modName;
   FileParser fp;
   ofstream constraintOut("constraint.v");

   if (fp.openFile(sigFile) == false) {
      cerr << "Cannot open file \"" << sigFile << "\", file doesn't exist!!" << endl;
      return false;
   }
   char* line;
   char* remainder;
   set<string> sigs;
   while ((line = fp.readLine()) != 0) {
      while (line != 0) {
         remainder = fp.getString(line);  // line is the cut token
         _designHier.setCurrent(line);
         line = remainder;
         //modName = _designHier.getCurNode()->moduleName();
         //if (sigsMap.find(modName) == sigsMap.end())
         //   sigsMap.insert(pair<string, set<string> >(modName, sigs));
      }
      line = fp.readLine(); // second line
      constraintOut << "====================== " << line << " =======================" << endl;
      _designHier.writeHierConstraint(line, constraintOut);
   }
   constraintOut.close();
   return true;
}

void
VlpHierTree::writeHierConstraint(string sigName, ofstream& csOut) const
{
   CktModule* cMod = _current->getCktModule();
   set<string> inputs = cMod->writeConstraint(sigName, csOut);

   csOut << "========================== input list ==========================" << endl;
   for (set<string>::iterator it = inputs.begin(); it != inputs.end(); ++it) {
      csOut << "------> " << (*it) << endl;
   }
}

set<string> 
CktModule::writeConstraint(string& sigName, ofstream& csOut) const
{
   set<string> finSigs, modBund, bundSigs;
   _synVars[sigName]->writeVarCst(csOut, finSigs);

   //for (set<string>::iterator it = finSigs.begin(); it != finSigs.end(); ++it)
      //cout << "fin Sig = " << (*it) << endl;
   // generate intra signal constraints
   for (set<string>::iterator it = finSigs.begin(); it != finSigs.end(); ++it) {
      csOut << "============ " << (*it) << " ===========" << endl;
      bundSigs.clear();
      writeFinFunc(*it, csOut, bundSigs);
      for (set<string>::iterator it = bundSigs.begin(); it != bundSigs.end(); ++it)
         modBund.insert(*it);
   }

   // only inputs of module or outputs of moduleCell in modBund
   return modBund;
}

void
CktModule::writeFinFunc(const string sigName, ofstream& csOut, set<string>& bundSigs) const
{
   CktOutPin* sigPin = _outPinMap.getSharPin(sigName);
   CktCell* cell = sigPin->getCell();
   cout << "sigName = " << sigName;
   if ((cell != 0) && (cell->getCellType() == CKT_PI_CELL)) {
      cout << " cell type = " << cell->getCellType() << endl;
      bundSigs.insert(sigName);
      return;
   }
   else
      cout << endl;

   set<string> finSigs;
   _synVars[sigName]->writeFunc(csOut, finSigs);

   for (set<string>::iterator it = finSigs.begin(); it != finSigs.end(); ++it) {
      if (sigName == *it) // The finCell is a moduleCell
         bundSigs.insert(sigName);
      else // recurrence
         writeFinFunc(*it, csOut, bundSigs);
   }
}

void 
SynVar::writeVarCst(ofstream& csOut, set<string>& finSigs) const
{
   for (vector<finPair>::const_iterator it = _finOutPins.begin(); it != _finOutPins.end(); ++it) {
      if ((*it)._cond)
         (*it)._cond->writeCtrlSig(csOut, finSigs);
      else 
         finSigs.insert(_name);
   }
}

void 
SynCond::writeCtrlSig(ofstream& csOut, set<string>& finSigs) const
{
   if (_ctrlNode != NULL) {
      csOut << _ctrlNode->writeExp(finSigs) << " dist { 1 := 30, 0 := 70 }; " << endl;

      if (_trueCond != 0)
         _trueCond->writeCtrlSig(csOut, finSigs);
      if (_falseCond != 0)
         _falseCond->writeCtrlSig(csOut, finSigs);
   }
}

void
SynCond::writeCtrlSrc(string& tSig, ofstream& csOut, set<string>& finSigs) const
{
   if (_seNode != NULL) {
      if ( (_seNode->getNodeClass() == NODECLASS_OPERATOR) 
      ||   (_seNode->getNodeClass() == NODECLASS_SIGNAL && !(static_cast<const VlpSignalNode*>(_seNode)->isBitBaseStr()) ))
      {
         csOut << tSig << " == " << _seNode->writeExp(finSigs) << endl;
         if (_trueCond != 0)
            _trueCond->writeCtrlSrc(tSig, csOut, finSigs);
         if (_falseCond != 0)
            _falseCond->writeCtrlSrc(tSig, csOut, finSigs);
      }
   }
}

void
SynVar::writeFunc(ofstream& csOut, set<string>& finSigs) const
{
   string tSig = _name;
   const SynBus* bus;
   for (vector<finPair>::const_iterator it = _finOutPins.begin(); it != _finOutPins.end(); ++it) { 
      bus = VLDesign.getBus((*it)._busId);
      if (bus->getBegin() != 0 && bus->getEnd() != 0) {
         tSig += "[";
         tSig += bus->getBegin();
      }
      if (bus->getWidth() > 1) {
         tSig += ":";
         tSig += bus->getEnd();
      }
      if (bus->getBegin() != 0 && bus->getEnd() != 0)
         tSig += "]";
      //cout << (*it)._busId << " " << bus->getWidth() << " " << bus->getBegin() << " " << bus->getEnd() << endl;
      if ((*it)._finPin != 0) {
         if ((*it)._seNode->getNodeClass() == NODECLASS_SIGNAL)        // fin from moduleCell => skip
            if (_name == static_cast<VlpSignalNode*>((*it)._seNode)->getSigName()) {
               finSigs.insert(_name);     
               return;
            }

         csOut << tSig << " == " << (*it)._seNode->writeExp(finSigs) << endl;
      }
      else { // hard to complete specify function
         (*it)._cond->writeCtrlSig(csOut, finSigs);
         (*it)._cond->writeCtrlSrc(tSig, csOut, finSigs);
      }
   }
}

#endif
