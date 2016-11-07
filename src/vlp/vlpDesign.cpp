/****************************************************************************
  FileName     [ vlpDesign.cpp ]
  Package      [ vlp ]
  Synopsis     [ Member function of Design class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_DESIGN_SOURCE
#define VLP_DESIGN_SOURCE

#include "VLGlobe.h"
#include "vlpDesign.h"
#include "vlpItoStr.h"
#include "bst2.h"
#include "vlpStack.h"

#include "cktModule.h"
#include "synVar.h"
#include "cktBLIFile.h"
#include "cktConnect.h"

#include "myFileUtil.h"

#include <set>
#include <stack>
#include <list>

SynOutFile synOutFile;
CktOutFile cktOutFile;

extern LY_usage* LY;
extern LibLY_usage* LibLY;

extern const VlgModule* curVlpModule;
extern CktModule* curCktModule;
extern int globalPrintLevel;
extern CktHierIoMap hierIoMap;

extern int netNumber;
extern int instNumber;
extern CKT_usage* CREMap;

extern int yyparse();
extern int libparse();

VlpDesign::VlpDesign()
{
   _topModule = NULL;
   _cktFlattenDesign = NULL;
   _iFSM = NULL;
   _FSMExtracted = false;
}

VlpDesign::~VlpDesign()
{
   //need to modify the destructor carefully........
   //_moduleAry.clear();//use array.h : double free this==> why
                        //using STL vector is ok.
   //delete _topModule; //if there isn't any behavior code(assign, always)
                        //in modules, the action will cause memory fault
   //if (_topModule != NULL)
      //delete _topModule;
   //cout << "~VlpDesign()" << endl;
   vector<VlgModule*>::iterator pos;
   for (pos = _vlgModuleAry.begin(); pos != _vlgModuleAry.end(); ++pos) 
      delete (*pos);
}

const string
VlpDesign::getCurModuleName() const {
   return getFltModule() ? getFltModName() : isDesignRead() ? getCurHTNode() ? 
          getCurHTNode()->moduleName() : getTopModName() : "";
}
   
CktModule*
VlpDesign::CreateDesign(const string& topName)
{
   // Jump into MODULE_NAME> prompt mode
   CktModule* topModule = new CktModule(); assert (topModule);
   topModule->setModuleName(topName); setModule(topModule);
   VlpHierTreeNode* rootNode = new VlpHierTreeNode(topName, topModule, 0);
   rootNode->setParent(NULL); setTree(rootNode); setCurHTNode(rootNode);
   CREMap = new CKT_usage();  // For mapping between CktOutPin::_netName and CktCell*
   SynSharNodeMap topNodeMap; (CREMap->outPinMap).push(topNodeMap);
   return topModule;
}

CktModule*
VlpDesign::CreateModule(const string& subName) {
   assert (subName.size());
   if (getCktModule(subName)) {
      Msg(MSG_ERR) << "Module \"" << subName << "\" has already exist in current design !!!" << endl;
      return 0;
   }
   // Create Sub Module
   CktModule* subModule = getCktModule(subName);
   subModule = new CktModule(); assert (subModule);
   subModule->setModuleName(subName); setModule(subModule);
   VlpHierTreeNode* parent = getCurHTNode(); assert (parent);
   VlpHierTreeNode* child = new VlpHierTreeNode(subName, subModule, 0);
   parent->setChild(child); child->setParent(parent); setCurHTNode(child);
   SynSharNodeMap childNodeMap; (CREMap->outPinMap).push(childNodeMap);
   return subModule;
}

CktModule*
VlpDesign::ChangeModule() {
   if (getCurHTNode() == getHier()->root()) {
      Msg(MSG_WAR) << "We are already in the top module !!!" << endl;
      return 0;
   }
   // Return Parent Module
   assert (getCurHTNode()->parent());
   setCurHTNode(getCurHTNode()->parent()); (CREMap->outPinMap).pop();
   return getCurHTNode()->getCktModule();
}

void
VlpDesign::CreateInstance(CktModule* module, const string& instName) {
   assert (module); assert (instName.size());
   VlpHierTreeNode* parent = getCurHTNode(); assert (parent);
   VlpHierTreeNode* child = new VlpHierTreeNode(instName, module, 0);
   parent->setChild(child); child->setParent(parent);
}

void
VlpDesign::UpdateInstance(const string& moduleName, const string& instName) {
   assert (moduleName.size()); assert (instName.size());
   VlpHierTreeNode* moduleNode = getCurHTNode()->child(moduleName);
   if (!moduleNode) {
      assert (getCktModule(moduleName));
      CreateInstance(getCktModule(moduleName), instName);
   }
   else moduleNode->setInstName(instName);
}

void 
VlpDesign::setModule(VlgModule* const& modulePtr)
{
   _vlgModuleAry.push_back(modulePtr);
   _moduleMap.insert(BaseModule :: setName(modulePtr->getModuleName()), modulePtr);
}

void 
VlpDesign::setModule(LibModule* const& modulePtr)
{
   _libModuleAry.push_back(modulePtr);
   _moduleMap.insert(BaseModule :: setName(modulePtr->getModuleName()), modulePtr);
}

void
VlpDesign::setModule(CktModule* const& modulePtr)
{
   _cktModuleAry.push_back(modulePtr);
}

unsigned 
VlpDesign::getModuleNum() const 
{ 
   return _vlgModuleAry.size(); 
}

string
VlpDesign::getFltModName() const
{
   return _cktFlattenDesign->getModuleName();
}

VlgModule*
VlpDesign::getTopModule() const
{
   return _topModule;
}

string 
VlpDesign::getTopModName() const
{
   return _topModule->getModuleName();
}

BaseModule* 
VlpDesign::getModule(const string& str) const
{
   BaseModule* r_module;
   bool isExist = _moduleMap.getData(BaseModule :: setName(str), r_module);
   if (isExist == true)
      return r_module;
   else
      return NULL;
}
      
const VlgModule* const 
VlpDesign::getVlgModule(const int& i) const 
{ 
   return _vlgModuleAry[i]; 
}

const LibModule* const 
VlpDesign::getLibModule(const int& i) const 
{ 
   return _libModuleAry[i]; 
}

CktModule*
VlpDesign::getCktModule(int i) const
{
   return _cktModuleAry[i];
}

CktModule*
VlpDesign::getCktModule(string name) const
{
   for (unsigned i = 0; i < _cktModuleAry.size(); ++i)
      if (_cktModuleAry[i]->getModuleName() == name)
         return _cktModuleAry[i];
   return NULL;
}

CktModule*
VlpDesign::getCktModule(char* name) const
{
   string strName = name;
   for (unsigned i = 0; i < _cktModuleAry.size(); ++i)
      if (_cktModuleAry[i]->getModuleName() == strName)
         return _cktModuleAry[i];
   return NULL;
}

CktModule*
VlpDesign::getFltModule() const
{
   return _cktFlattenDesign;
}

void
VlpDesign::setCurHTNode(VlpHierTreeNode* const curNode)
{
   _designHier.setCurrent(curNode);
}

void 
VlpDesign::setTree(VlpHierTreeNode* const& root)
{
   _designHier.setRoot(root);
}

const VlpHierTree*
VlpDesign::getHier() const 
{
   /*
   if (_topModule == NULL) {
      Msg(MSG_ERR) << "Construct hierarchy first" << endl;
      return NULL;
   }
   */
   return &_designHier;
}

VlpHierTreeNode*
VlpDesign::getCurHTNode() const
{
   return _designHier.getCurNode();
}

VlpHierTreeNode*
VlpDesign::getHTNode(const string& name) const
{
   return _designHier.getNode(_designHier.root(), name);
}

bool
VlpDesign::ParseVerilog(const char* fileName, bool isBBox)
{
   extern FILE* yyin;
   bool success = false;
   if (!(yyin = fopen(fileName, "r"))) Msg(MSG_ERR) << "Verilog File \"" << fileName << "\" Doesn't Exist!!" << endl;
   else {
      // New Verilog Parser
      LY = new LY_usage();
      // Set BlackBox Toggle
      LY->blackBox = isBBox;
      // Push FileName into Stack
      LY->FileNameStack.push(fileName);
      // Start File Parse
      Msg(MSG_IFO) << " > Parsing Verilog File : " << getAbsPath(fileName) << " ..." << endl;
      unsigned moduleNum = getModuleNum();
      if (yyparse()) Msg(MSG_ERR) << "Verilog File : " << fileName << " Parse Error!!" << endl;
      else {
         for (; moduleNum < getModuleNum(); ++moduleNum) _vlgModuleAry[moduleNum]->setFileName(fileName);
         success = true;
      }
      fclose(yyin);
      delete LY;
   }
   return success;
}

bool
VlpDesign::ParseFileList(const char* fileName)
{
   FILE* fp = fopen(fileName, "r");
   bool success = true;
   if (!fp) Msg(MSG_ERR) << "FileList \"" << fileName << "\" Doesn't Exist!!" << endl;
   else {
      unsigned fileLineNum = 0;
      char buffer[buffer_size];
      char* tokenizer = " ,\t\n;";
      char* tokens = 0;
      bool liberty = false, library = false, file = false;
      Msg(MSG_IFO) << "=== Parsing FileList : " << getAbsPath(fileName) << " ..." << endl;
      while (true) {
         fgets(buffer, buffer_size, fp); ++fileLineNum;
         if (feof(fp)) break;
         tokens = strtok(buffer, tokenizer);
         while (tokens) {
            if (strncmp(tokens, "//", 2) == 0) break;
            else if (strcmp(tokens, "-LIB_LIBERTY") == 0) {
               if (library || liberty || file) break;
               else liberty = true;
            }
            else if (strcmp(tokens, "-LIB_VERILOG") == 0) {
               if (library || liberty || file) break;
               else library = true;
            }
            else if (strcmp(tokens, "-f") == 0) {
               if (library || liberty || file) break;
               else file = true;
            }
            else if (strcmp(tokens, "+incdir") == 0) {
               Msg(MSG_WAR) << "Line " << fileLineNum << " : \"+incdir\" has not been supported yet!!" << endl;
            }
            else {
               if (library) { success = ParseVerilogLib(tokens); library = false; }
               else if (liberty) { success = ParseVerilog(tokens, true); liberty = false; }
               else if (file) { success = ParseFileList(tokens); file = false; }
               else success = ParseVerilog(tokens, false);
               if (!success) break;
            }
            tokens = strtok(NULL, tokenizer);
         }
         if (library || liberty || file) {
            if (library) Msg(MSG_ERR) << "Line " << fileLineNum << " : Expecting Library File after \"-LIB_VERILOG\"!!" << endl;
            else if (liberty) Msg(MSG_ERR) << "Line " << fileLineNum << " : Expecting Liberty File after \"-LIB_LIBERTY\"!!" << endl;
            else if (file) Msg(MSG_ERR) << "Line " << fileLineNum << " : Expecting Liberty File after \"-f\"!!" << endl;
            success = false;
         }
         if (!success) break;
      }
   }
   fclose(fp);
   return success;
}

bool
VlpDesign::ParseVerilogLib(const char* fileName)
{
   extern FILE* libin;
   bool success = false;
   if (!(libin = fopen(fileName, "r"))) Msg(MSG_ERR) << "Library File \"" << fileName << "\" Doesn't Exist!!" << endl;
   else {
   // New Verilog Parser
   LY = new LY_usage(); LY->blackBox = false;
   // New Library Parser
   LibLY = new LibLY_usage();
   // Start Library Parse
   Msg(MSG_IFO) << " > Parsing Verilog Library : " << getAbsPath(fileName) << " ..." << endl;
   unsigned moduleNum = getModuleNum();
   if (libparse()) Msg(MSG_ERR) << "Verilog Library File : " << fileName << " Parse Error!!" << endl;
   else { 
      for (; moduleNum < getModuleNum(); ++moduleNum) _libModuleAry[moduleNum]->setFileName(fileName); 
      success = true;
   }
   delete LY; delete LibLY;
   }
   fclose(libin);
   return success;
}

bool
VlpDesign::ParseBLIF(const char* fileName)
{
   FILE* input = fopen(fileName, "r");
   if (!input) Msg(MSG_ERR) << "BLIF File \"" << fileName << "\" Doesn't Exist!!" << endl;
   else {
      Msg(MSG_IFO) << " > Parsing BLIF File : " << getAbsPath(fileName) << " ..." << endl;
      parseBLIF(input); fclose(input);
      if (!curCktModule) return false;
      _cktFlattenDesign = curCktModule;
      return true;
   }
   return false;
}

bool
VlpDesign::ParseBTOR(const char* fileName)
{
   FILE* input = fopen(fileName, "r");
   if (!input) Msg(MSG_ERR) << "BTOR File \"" << fileName << "\" Doesn't Exist!!" << endl;
   else {
      Msg(MSG_IFO) << " > Parsing BTOR File : " << getAbsPath(fileName) << " ..." << endl;
      parseBTOR(input); fclose(input);
      if (!curCktModule) return false;
      _cktFlattenDesign = curCktModule;
      return true;
   }
   return false;
}

void
VlpDesign::printCDFG( const string& mName, int level ) const {
   if ( _topModule == NULL ) {
      Msg(MSG_ERR) << "Construct hierarchy first" << endl;
      return;
   }

   BaseModule* mn = getModule( mName );
   if ( mn == NULL ) {
      Msg(MSG_ERR) << "Invalid module name \"" << mName << "\""
           << endl;
      return;
   }
   if (mn->isLibrary() == true)
      Msg(MSG_ERR) << "Module \"" << mName << "\" is a library module" << endl;
   else
      (static_cast<VlgModule*>(mn))->getDataFlow()->whoAmI(0, level );
}

bool 
VlpDesign::repCDFG(string& modName, int level, int nID) const
{
   if (nID != -1) {
      const VlpBaseNode* ptr = BaseModule :: getCDFGNode(nID);
      if (ptr == NULL) {
         Msg(MSG_ERR) << "Cannot find the CDFG node with ID = " << nID << "!!" << endl;
         return false;
      }
      ptr->whoAmI(0, level);
   }
   else {
      BaseModule* repMod = getModule(modName);   
      if (repMod == NULL) {
         Msg(MSG_ERR) << "Cannot find module \"" << modName << "\"!!" << endl;
         return false;
      }
      if (repMod->getDataFlow() != NULL)
         repMod->getDataFlow()->whoAmI(0, level);
      else
         Msg(MSG_ERR) << "No CDFG in the Module!!" << endl;
   }
   return true;
}

bool
VlpDesign::repCDFG(const string& moduleName, const string& pinName) const
{
   BaseModule* curModule = getModule(moduleName);   
   if (!curModule) {
      Msg(MSG_ERR) << "Cannot find module \"" << moduleName << "\"!!" << endl;
      return false;
   }
   if (!curModule->getDataFlow()) {
      Msg(MSG_ERR) << "No CDFG in the Module!!" << endl;
      return false;
   }

   const VlpSignalNode* pinNode = curModule->getSignalNode(pinName);
   if (!pinNode) {
      Msg(MSG_ERR) << "Cannot find pin \"" << pinName << "\"!!" << endl;
      return false;
   }
   pinNode->whoAmI(0, 99);
   return true;
}

bool
VlpDesign::repModule(string& modName, int level, int type) const
{
   //if (repPort && repIns)       type = 0;
   //else if (!repPort && repIns) type = 1;
   //else if (repPort && !repIns) type = 2;
   //else                         type = 3;
   BaseModule* repMod = getModule(modName);   
   if (repMod == NULL) {
      Msg(MSG_ERR) << "Cannot find module \"" << modName << "\"!!" << endl;
      return false;
   }
   
   if (type == 0)
      _designHier.repNode(repMod, level, true, true);
   else if (type == 1)
      _designHier.repNode(repMod, level, true, false);
   else if (type == 2)
      _designHier.repNode(repMod, level, false, true);
   else if (type == 3)
      _designHier.repNode(repMod, level, false, false);
   else
      assert (0);
   
   return true;
}

bool
VlpDesign::repDeStat() const
{
   (_designHier.root())->getCktModule()->printStatistics(true);
   return true;
}

bool
VlpDesign::repDeSum() const
{
   CktModule* ckt = (_designHier.root())->getCktModule();
   ckt->printSummary(true);
   _designHier.printinfo();
   return true;
}

bool
VlpDesign::repDeVer() const
{
   CktModule* ckt = (_designHier.root())->getCktModule();
   ckt->printVerbose(true);
   cout << "  " << "instList : ";
   _designHier.printinst_mod(_designHier.root());
   cout << endl;
   return true;
}

bool
VlpDesign::repCktStat() const
{
   _cktFlattenDesign->printStatistics(false);
   return true;
}

bool
VlpDesign::repCktSum() const
{
   _cktFlattenDesign->printSummary(false);
   cout << "  " << "Total Cell Num : " << _cktFlattenDesign->getCellListSize() << endl;
   return true;
}

bool
VlpDesign::repCktVer() const
{
   _cktFlattenDesign->printVerbose(false);
   return true;
}

bool
VlpDesign::repFIGate(string gateName, int level) const
{
   if (!_cktFlattenDesign->printPIGate(gateName, level)) { 
      Msg(MSG_ERR) << "\"" << gateName << "\" not found!." << endl;
      return false;
   }
   return true;
}

bool
VlpDesign::repFOGate(string gateName, int level) const
{
   if (!_cktFlattenDesign->printPOGate(gateName, level)) {
      Msg(MSG_ERR) << "\"" << gateName << "\" not found!." << endl;
      return true;
   }
   return true;
}

bool
VlpDesign::writeCDFG(const string moduleName, const string pinName, const string fileName) const
{
   BaseModule* curModule = getModule(moduleName);   
   if (!curModule) {
      Msg(MSG_ERR) << "Cannot find module \"" << moduleName << "\"!!" << endl;
      return false;
   }
   if (!curModule->getDataFlow()) {
      Msg(MSG_ERR) << "No CDFG in the Module!!" << endl;
      return false;
   }

   const VlpSignalNode* pinNode = curModule->getSignalNode(pinName);
   if (!pinNode) {
      Msg(MSG_ERR) << "Cannot find pin \"" << pinName << "\"!!" << endl;
      return false;
   }
   
   ofstream DOTFile;
   DOTFile.open(fileName.c_str());
   DOTFile << "digraph G {" << endl;
   pinNode->writeWhoAmI(DOTFile);
   DOTFile << "}" << endl;
   DOTFile.close();
   return true;
}

bool
VlpDesign::writeHierarchy(const string moduleName, const string fileName) const
{
   VlpHierTreeNode* curNode;
   if (moduleName == "") curNode = _designHier.root();
   else curNode = const_cast<VlpDesign*>(this)->getHTNode(moduleName);
   if (!curNode) { Msg(MSG_ERR) << "Module " << moduleName << " NOT exists in current design!" << endl; return false; }
   ofstream DOTFile;
   DOTFile.open(fileName.c_str());
   DOTFile << "digraph G {" << endl;
   curNode->writeChild(DOTFile, 1);
   DOTFile << "}" << endl;
   DOTFile.close();
   return true;
}

bool
VlpDesign::extract() {
   VlgModule *curModule;
   unsigned int i;

   if ( _topModule == NULL )
      resolveHier();

   for ( i = 0; i < _vlgModuleAry.size(); ++i ) {
      curModule = _vlgModuleAry[i];
      curModule->extract();
   }
   return true;
}

bool 
VlpDesign::parseFSM(string inFile)
{
   _iFSM = new VlpFSM();
   if (_iFSM->parseFSM(inFile.c_str()))
      return true;
   else {
      delete _iFSM;
      _iFSM = NULL;
      return false;
   }
}

void
VlpDesign::reset() {
   _topModule = NULL;
   if ( _cktFlattenDesign != NULL )
      delete _cktFlattenDesign;
   _cktFlattenDesign = NULL;
   vector< VlgModule*>::iterator pos1;
   vector< LibModule*>::iterator pos2;
   for ( pos1 = _vlgModuleAry.begin(); pos1 != _vlgModuleAry.end(); pos1++ )
      delete (*pos1);

   for ( pos2 = _libModuleAry.begin(); pos2 != _libModuleAry.end(); pos2++ )
      delete (*pos2);

   _vlgModuleAry.clear();
   _libModuleAry.clear();
   _moduleMap.clear();
}

void
VlpDesign::repCU( const char *src ) const {
   VlgModule* curNode;
   vector< VlpCU* > bufCU;
   vector< VlgModule* > bufModule;

   if ( src == NULL ) {
      for ( unsigned int i = 0; i < _vlgModuleAry.size(); ++i ) {
         curNode = _vlgModuleAry[i];
         bufModule.push_back( curNode );
      }
   }
   else {
      BaseModule* tmpModule = getModule( src );
      if ( curNode == NULL ) {
         Msg(MSG_ERR) << "Module " << src << " not existed." << endl;
      }
      else if (tmpModule->isLibrary() == true)
         Msg(MSG_ERR) << "Module " << src << " is LibModule." << endl;
      else {
         curNode = static_cast<VlgModule*>(tmpModule);
         bufModule.push_back( curNode );
      }
   }

   for ( unsigned int m = 0; m < bufModule.size(); ++m ) {
      curNode = bufModule[m];
      bufCU = curNode->CUs();
      cout << "Module: " << curNode->getModuleName() << endl;
      cout << "=======================================" << endl;
      cout << "Name                 Type              " << endl;
      cout << "-------------------- ------------------" << endl;

      for ( unsigned int i = 0; i < bufCU.size(); ++i ) {
         cout << left << setw(20) << bufCU[i]->name().str();
         cout << " ";

         if ( bufCU[i]->isExType() )
            cout << " Explicit";
         else
            cout << " Implicit";

         if ( bufCU[i]->type() == VLPCU_FSM )
            cout << " FSM";
         else
            cout << " Counter";
         cout << endl;
      }

      cout << "======================================" << endl;
      cout << "Number of CUs: " << bufCU.size() << endl;
   }
}

void
VlpDesign::writeCU(const char *src, bool dot) const 
{
   VlgModule* curNode;
   vector<VlpCU*> bufCU;
   vector<VlgModule*> bufModule;
   MyString fileName;

   if (src == NULL) {
      for (unsigned i = 0; i < _vlgModuleAry.size(); ++i) {
         curNode = _vlgModuleAry[i];
         bufModule.push_back(curNode);
      }
   }
   else {
      BaseModule* tmpModule = getModule(src);
      if (tmpModule == NULL)
         Msg(MSG_ERR) << "Module " << src << " not existed." << endl;
      else if (tmpModule->isLibrary() == true)
         Msg(MSG_ERR) << "Module " << src << " is LibModule." << endl;
      else {
         curNode = static_cast<VlgModule*>(tmpModule);
         bufModule.push_back(curNode);
      }
   }
   for (unsigned i = 0; i < bufModule.size(); ++i) {
      curNode = bufModule[i];
      bufCU = curNode->CUs();
      for (unsigned i = 0; i < bufCU.size(); ++i) {
         if (bufCU[i]->type() == VLPCU_FSM)
            fileName = "F";
         else
            fileName = "C";
         fileName += curNode->getModuleName();
         fileName += "_";
         if (dot) {
            fileName += bufCU[i]->name() + ".dot";
            bufCU[i]->writeDot(fileName);
         }
         else {
            fileName += bufCU[i]->name() + ".png";
            bufCU[i]->genPNG(fileName);
         }
         Msg(MSG_IFO) << "> Write out: " << fileName << endl;
      }
   }
}

bool 
VlpDesign::synWriteDesign(string file) const
{
   Msg(MSG_IFO) << "Write design start..." << endl;
   if (CREMap == NULL)
      CREMap = new CKT_usage();

   string fileName;
   CktModule* cktModule;
   
   if (file == "") fileName = "design.v";
   else fileName = file;
   
   //write output of each module
   synOutFile.open(fileName.c_str());

   for (unsigned i = 0; i < _cktModuleAry.size(); ++i) {
      cktModule = getCktModule(i);
      curVlpModule = cktModule->getVlpModule();
      if (cktModule->isBlackBox() == false) {
         //cout << "Writing output Module " << i+1 << " " 
         //     << cktModule->getModuleName() << "..." << endl;
         netNumber = cktModule->getNetNumber();
         instNumber = 0;
         cktModule->modWriteOut();
         //synOutFile << endl;
         endl(synOutFile);
      }
   }
   synOutFile.close();
   Msg(MSG_IFO) << "Write design successfully!!" << endl;
   return true;
}

bool 
VlpDesign::fltWriteDesign(string file) const
{
   Msg(MSG_IFO) << "Write ckt start..." << endl;
   if (CREMap == NULL)
      CREMap = new CKT_usage();

   string fileName;   
   if (file == "") fileName = "cktGate.v";
   else fileName = file;

   synOutFile.open(fileName.c_str());
   curVlpModule = _topModule; //set global variable
   instNumber   = 0;          //set global variable
   netNumber    = _cktFlattenDesign->getNetNumber();
   _cktFlattenDesign->modWriteOut();
   //synOutFile << endl;
   endl(synOutFile);
   synOutFile.close();
   Msg(MSG_IFO) << "Write ckt successfully !!" << endl;
   return true;
}

bool 
VlpDesign::writeBLIF(string filename) const
{
   Msg(MSG_IFO) << "Write blif start..." << endl;
   if (filename == "") cktOutFile.open("cktGate.blif");
   else cktOutFile.open(filename.c_str());
   //.module
   //cout << "BLIF .model (VlpDesign)" << endl;
   cktOutFile << ".model BLIF_Design";
   endl(cktOutFile);
   //.input
   cktOutFile << "#=======================================inputs======================================#";
   endl(cktOutFile);
   //cout << "BLIF .inputs (VlpDesign)" << endl;
   cktOutFile << ".inputs ";
   _cktFlattenDesign->writeBLIF_Input();
   endl(cktOutFile);
   //.output
   cktOutFile << "#======================================outputs======================================#";
   endl(cktOutFile);
   //cout << "BLIF .outputs (VlpDesign)" << endl;
   cktOutFile << ".outputs ";
   _cktFlattenDesign->writeBLIF_Output();
   endl(cktOutFile);
   cktOutFile << "#=====================================functions=====================================#";
   endl(cktOutFile);
   //<command>
   //cout << "BLIF <command> (VlpDesign)" << endl;
   _cktFlattenDesign->writeBLIF();
   //.end
   //cout << "BLIF .end (VlpDesign)" << endl;
   cktOutFile << ".end";
   endl(cktOutFile);
   cktOutFile.close();
   Msg(MSG_IFO) << "Write blif successfully !!" << endl;
   return true;
}

void
VlpDesign::resetCkt()
{
   _topModule = NULL;
   if ( _cktFlattenDesign != NULL )
      delete _cktFlattenDesign;
   _cktFlattenDesign = NULL;
   _designHier.clear(_designHier.root());
   _designHier.setRoot(NULL);
   _designHier.setCurrent(NULL);
   for (vector<CktModule*>::iterator it = _cktModuleAry.begin(); it != _cktModuleAry.end(); ++it)
      delete *(it);
   _cktModuleAry.clear();
   for (map<string, IOPinConn*>::iterator it = hierIoMap.begin(); it != hierIoMap.end(); ++it)
      delete it->second;
   hierIoMap.clear();
}

void
VlpDesign::resolveHier() 
{
   hierCSTer(); // construct the hierarchy relation and handle pos-mapping parameter overload
   NamePOL_Design();
}

void 
VlpDesign::hierCSTer()
{
   set<string> top;
   set<string>::iterator pos;
   string moduleName;
   int num;
   for (unsigned i = 0; i < _vlgModuleAry.size(); ++i) {
      if (_vlgModuleAry[i]->isBlackBox() == false && _vlgModuleAry[i]->isLibrary() == false) {
         moduleName = _vlgModuleAry[i]->getModuleName();
         top.insert(moduleName);
      }
   }

   for (unsigned i = 0; i < _vlgModuleAry.size(); ++i) {
      for (unsigned j = 0; j < _vlgModuleAry[i]->getInstSize(); ++j) {
         num = _vlgModuleAry[i]->getInst(j)->getModuleID();
         if (_moduleMap.exist(num)) 
             top.erase(BaseModule::getName(num));
      }
   }

   //Msg(MSG_IFO) << "Start hierarchy construct..." << endl;
   num = 0;
   if ( top.size() != 1 ) {
      cout << "VlpDesign::hierCSTer() ---> WARNING: " << top.size()
           << " TOP modules in the design." << endl;

      cout << "TOP Module List:" << endl;
      cout << "=====================================" << endl;
      cout << "Index    Module Name" << endl;
      cout << "-------- ----------------------------" << endl;

      for ( pos = top.begin(); pos != top.end(); ++pos )
         cout << setw(8) << num++ << " " << *pos << endl;
      cout << "=====================================" << endl;
      cout << "Choose one as top module[0~" << top.size()-1 << "]: ";
      cin  >> num;
   }
   pos = top.begin();
   for (int i = 0; i < num; ++i)
      ++pos;
   _topModule = static_cast<VlgModule*>(getModule(*pos));

	Msg(MSG_IFO) << "==================================================" << endl;
	Msg(MSG_IFO) << "==> Total " << getModuleNum() << " Module(s)" << endl;
   Msg(MSG_IFO) << "==> Top Module : " << _topModule->getModuleName() << endl;
      
   VlpHierTreeNode* root = new VlpHierTreeNode("top", _topModule, NULL);//Design Top
   _designHier.setRoot(root);
   _topModule->cstTree(root, _moduleMap);//also handle pos-mapping parameter overload
}

void 
VlpDesign::NamePOL_Design()
{
   for (unsigned i = 0; i < _vlgModuleAry.size(); ++i) {
      _vlgModuleAry[i]->NamePOL_Module(_moduleMap);
      //_vlgModuleAry[i]->printPOLT();
   }
}

#endif
