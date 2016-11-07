/****************************************************************************
  FileName     [ vlpHierTree.cpp ]
  Package      [ vlp ]
  Synopsis     [ Member function of Hierarchy Tree class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_HIER_TREE_SOURCE
#define VLP_HIER_TREE_SOURCE

#include "vlpHierTree.h"
#include "vlpModule.h"
#include "vlpDesign.h"

#include "synVar.h"
#include "cktModule.h"
#include "cktConnect.h"

#define CONJUNCTION "_"

// External Declarations
extern CktHierIoMap hierIoMap;
extern VlpDesign& VLDesign;

class CktCell;
class CktOutPin;
class CktInPin;

VlpHierTreeNode::VlpHierTreeNode(const string& instName, 
                                 const BaseModule* const module,
                                 const VlpInstance* const inst)
{
   _instName  = instName;
   _modulePtr = module;
   _cktModule = NULL;
   _instPtr   = inst;
   _parent    = NULL;
   _childAry.clear();
   _cells.clear();
}

VlpHierTreeNode::VlpHierTreeNode(const string& instName,
                                 CktModule* cktmodule,
                                 const VlpInstance* const inst)
{
   _modulePtr = NULL;
   _instName  = instName;
   _cktModule = cktmodule;
   _instPtr   = inst;
   _parent    = NULL;
   _childAry.clear();
   _cells.clear();
}

VlpHierTreeNode::~VlpHierTreeNode()
{
   _modulePtr = NULL;
   _cktModule = NULL;
   _instPtr   = NULL;
   _parent    = NULL;
   _childAry.clear();
   _cells.clear();
}

void 
VlpHierTreeNode::setChild(VlpHierTreeNode* const cNode)
{
   cNode->setParent(this);
   _childAry.push_back(cNode);
}

void 
VlpHierTreeNode::dfsPrintNode(int l, int w, bool isShowInst, bool isShowPort) const
{       
   string modName = moduleName();
   w += 5; 
   int hNum = w/5;
   cout << "(" << hNum << ") ";
   cout << modName << " ";
   if (isShowInst)
      cout << _instName << " ";
   if (isShowPort)
      printPort();
   
   if (_modulePtr != NULL)
      if (_modulePtr->isLibrary() == true)
         cout << "[Lib]";
   cout << endl;
   --l;
   if (l > 0) {
      for (unsigned i = 0; i < _childAry.size(); ++i) {  
         cout.width(w);
         _childAry[i]->dfsPrintNode(l, w, isShowInst, isShowPort);
      }
   }
}

void
VlpHierTreeNode::printPort() const
{
   if (_modulePtr != NULL)
      _modulePtr->printIO(false);
   else
      _cktModule->printPIO();
}

void
VlpHierTreeNode::writeChild(ofstream& os, const unsigned& indent) const
{
   string myIndent = "";
   for (unsigned i = 0; i < indent; ++i) myIndent += TAB_INDENT;
   os << myIndent << "subgraph cluster" << this << "{" << endl;
   for (ChildAry::const_iterator it = _childAry.begin(); it != _childAry.end(); ++it)
      (*it)->writeChild(os, indent + 1);
   os << myIndent << TAB_INDENT << "\"" << _instPtr << "\"" << " [shape = plaintext, label = " << _instName << "]" << endl;
   os << myIndent << TAB_INDENT << "label = \"" << moduleName();
   if (_modulePtr != NULL) {
      if (_modulePtr->isLibrary() == true) os << "[Lib]";
   }
   os << "\"" << endl;
   os << myIndent << "}" << endl;
}

VlpHierTreeNode*
VlpHierTreeNode::child(unsigned index) const 
{
   if (index < _childAry.size())
      return _childAry[index];
   else
      return NULL;
}
   
VlpHierTreeNode* 
VlpHierTreeNode::child(const string& instName) const
{
   for (ChildAry::const_iterator it = _childAry.begin(); it != _childAry.end(); ++it)
      if ((*it)->instName() == instName)
         return (*it);

   return NULL;
}

void
VlpHierTreeNode::setInstName(const string& instName) {
   assert (instName.size());
   _instName = instName;
}

CktCell*
VlpHierTreeNode::getCktCell(unsigned index) const
{
   if (index < _cells.size())
      return _cells[index];
   else
      return NULL;
}

string
VlpHierTreeNode::moduleName() const 
{   
   string str = "";
   if (_modulePtr != NULL)
      str = _modulePtr->getModuleName();
   else
      str = _cktModule->getModuleName();
   return str;
}

// for DEFine MODule command
CktModule*
VlpHierTreeNode::dfsBuildCktModule(string insName)
{
   cout << "building... " << insName << endl;
   CktModule* toAdd = getCktModule(); 

   VlpHierTreeNode* newChild = new VlpHierTreeNode(insName, toAdd, NULL);
   VLDesign.getCurHTNode()->setChild(newChild); 
   VLDesign.setCurHTNode(newChild);

   for (unsigned i = 0, n = childSize(); i < n; ++i) {
      string newInsName = insName + "_" + child(i)->instName();
      child(i)->dfsBuildCktModule(newInsName);
/*
      VlpHierTreeNode* toAddChild = VLDesign.getCurHTNode()->child(i);
      CktModule* temp = toAddChild->getCktModule();
      cout << "creCell: " << temp->getModuleName() << " <-> " << newInsName << endl;
      toAdd->creCell(temp, newInsName);
*/
   }
   VLDesign.setCurHTNode(newChild->parent());
   return toAdd;
}

bool 
VlpHierTreeNode::dfsSetCktModule()
{
   string modName = moduleName();
   POLMap::const_iterator it;
   if (_modulePtr->isLibrary() == false) {
      const VlgModule* vMod = static_cast<const VlgModule*>(_modulePtr);
      if (vMod->getPOLMap() != NULL) {
         it = vMod->getPOLMap()->find(instName());
         if (it != vMod->getPOLMap()->end()) // change to new overloaded module name
            modName = vMod->genNewModuleName(modName, it->second);
      }
   }

   CktModule* cktModule = VLDesign.getCktModule(modName);
   if (cktModule == NULL)
      return false;

   _cktModule = cktModule;

   for (unsigned i = 0; i < _childAry.size(); ++i)
      _childAry[i]->dfsSetCktModule();

   return true;
}

void
flatten_debug()
{
   IOPinConn* ioPinConn;
   int count = 0;
   cout << "The Size of CktHierIoMap = " << hierIoMap.size() << endl;
   cout << "========== Un-connecting ioPinConn List =============" << endl;
   for (CktHierIoMap::iterator pos = hierIoMap.begin(); pos != hierIoMap.end(); pos++) {
      ioPinConn = (*pos).second;
      if (ioPinConn->isTraverse() == false) {
         ++count;
         ioPinConn->printDebugInfo();
      }
   }
   cout << "Total number of unconnecting wire = " << count << endl;
   cout << "=====================================================" << endl;
}

void 
VlpHierTreeNode::dfsFlatten(string prefix, CktModule*& fltCkt, bool isTopModule)
{
   //cout << "---------> flatten module : " << moduleName() << endl;
   //cout << "---------> flatten inst   : " << _instName   << endl;   
   if (isTopModule) prefix = "top";
   else             prefix = prefix + CONJUNCTION + _instName;

   _cktModule->moduleDuplicate(prefix ,fltCkt, isTopModule, _cells);
   for (unsigned i = 0; i < _childAry.size(); ++i)
      _childAry[i]->dfsFlatten(prefix, fltCkt, false);
}

void
VlpHierTreeNode::dfsUpdate(CktModule*& fltCkt)
{
   for (CktCellAry::iterator it = _cells.begin(); it != _cells.end(); ++it)
      if (fltCkt->isCellInMod(*it) == false) {
         _cells.erase(it);
         --it;
      }

   for (unsigned i = 0; i < _childAry.size(); ++i)
      _childAry[i]->dfsUpdate(fltCkt);
}

void 
VlpHierTree::flatten(CktModule*& fltCkt) const
{
   _root->dfsFlatten("", fltCkt, true); //duplicate CktCell within CktModule  
   CktOutPin* outPin;
   IOPinConn* ioPinConn;

   //cout << "hierIoMap size = " << hierIoMap.size() << endl;
   //Final step : connect all connection between modules
   //"CktHierIoMap hierIoMap" is defined in the file "cktDefine.h".
   for (CktHierIoMap::iterator pos = hierIoMap.begin(); pos != hierIoMap.end(); ++pos) {
      ioPinConn = (*pos).second;
      if (ioPinConn->isStart()) {
         //cout << "==============" << ioPinConn->getOutPin()->getName() << "================" << endl;
         assert (ioPinConn->isTraverse() == false);
         outPin = ioPinConn->getOutPin();      
         ioPinConn->dfsConnect(outPin);      
      }
   }
   flatten_debug();   
}

void
VlpHierTree::updateFltCkt(CktModule*& fltCkt)
{
   fltCkt->updateFltCkt();   // remove buf and adjust constant width
   _root->dfsUpdate(fltCkt); // for each hier CktModule, remove the cells which are not in flatten CktModule
}

VlpHierTree::VlpHierTree()
{
   _root    = NULL;
   _current = NULL;
}

VlpHierTree::~VlpHierTree()
{
   if (_root != NULL) 
      delete _root;
      
   _root = NULL;
   _current = NULL;
}

void 
VlpHierTree::repNode(BaseModule* repMod, int& level, bool isShowInst, bool isShowPort) const
{
   VlpHierTreeNode* repNode = getNode(_root, repMod);
   unsigned w = 0;
   repNode->dfsPrintNode(level, w, isShowInst, isShowPort);
}
      
void 
VlpHierTree::setCurrent(string instName) 
{
   if (instName == "TOP")
      _current = _root;
   else 
      _current = _current->child(instName);
   
   // instName must be a instance of curent VlpHierTreeNode
   assert (_current != NULL);
}

VlpHierTreeNode*
VlpHierTree::getNode(VlpHierTreeNode* node, const string& name) const
{
   if (node->getCktModule()->getModuleName() == name)
      return node;

   for (unsigned i = 0, n = node->childSize(); i < n; ++i)
   {
      VlpHierTreeNode* result = getNode(node->child(i), name);
      if (result != NULL) return result;
   }
   return NULL;
}

void
VlpHierTree::clear(VlpHierTreeNode* root) const
{
   if (root != NULL) {
      cout << "Child size = " << root->childSize() << endl;
      for (unsigned i = 0; i < root->childSize(); ++i) {
         if (root->child(i) != 0) clear(root->child(i));
         cout << "End for" << endl;
      }
      cout << "========== Clear Node : " << root->moduleName() << " ( " << root->instName() << " ) ==========" << endl;
      delete root;
   }
}
      
VlpHierTreeNode* 
VlpHierTree::getNode(VlpHierTreeNode* node, BaseModule* target) const
{
   if (node->getModule() == target)
      return node;

   for (unsigned i = 0, n = node->childSize(); i < n; ++i)
   {
      VlpHierTreeNode* result = getNode(node->child(i), target);
      if (result != NULL) return result;
   }
   return NULL;
   
}

int
VlpHierTree::TreeHeight(VlpHierTreeNode* root) const
{
   int temp_level = 1, temp = 0;
   if (root != NULL) {
      for (unsigned i = 0; i < root->childSize(); ++i) {
         if (root->child(i) != NULL) {
            temp = TreeHeight(root->child(i));
            if (temp > temp_level) temp_level = temp;
         }
      }
   }
   return (temp_level + 1);
}

int
VlpHierTree::TreeNodeNum(VlpHierTreeNode* root) const
{
   int temp = 1;
   for (unsigned i = 0; i < root->childSize(); ++i) {
      if (root->child(i) != NULL) temp += TreeNodeNum(root->child(i));
   }
   return temp;
}

int
VlpHierTree::ModuleNum(VlpHierTreeNode* root) const
{
   int temp = 0;
   set <CktModule*>::iterator it;
   it = _cktModules.find(root->getCktModule());
   if (it == _cktModules.end()) {
      temp = 1;
      _cktModules.insert(root->getCktModule());
   }
   for (unsigned i = 0; i < root->childSize(); ++i) {
      if (root->child(i) != NULL) temp += ModuleNum(root->child(i));
   }
   return temp;
}

int
VlpHierTree::CellNum(VlpHierTreeNode* root) const
{
   int temp = root->getCktModule()->getCellListSize();
   for (unsigned i = 0; i < root->childSize(); ++i) {
      if (root->child(i) != NULL) temp += CellNum(root->child(i));
   }
   return temp;
}

void
VlpHierTree::printinst_mod(VlpHierTreeNode* root) const
{
   for (unsigned i = 0; i < root->childSize(); ++i) {
      if (root->child(i) != NULL) printinst_mod(root->child(i));
   }
   cout << root->instName() << "(" << root->moduleName() << ")  ";
}

void
VlpHierTree::printinfo() const
{
   _cktModules.clear();
   cout << "  " << "CktModule Num : " << ModuleNum(_root) << endl;
   cout << "  " << "ModuleInst Num : " << TreeNodeNum(_root) << endl;
   cout << "  " << "Design Level : " << TreeHeight(_root) << endl;
   cout << "  " << "Total Cell Num : " << CellNum(_root) << endl;
}

#endif
