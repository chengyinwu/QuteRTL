/****************************************************************************
  FileName     [ vlpModule.cpp ]
  Package      [ vlp ]
  Synopsis     [ Member function of Module Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_MODULE_SOURCE
#define VLP_MODULE_SOURCE

#include "vlpModule.h"
using namespace std;

//initialize static data member
NameMap BaseModule :: _nameMap;
CDFGAry BaseModule :: _cdfgArr;

//=============================class BaseModule================================//
BaseModule::BaseModule(string& name) 
{
   _moduleName    = name;
   _fileName      = "";
   _line          = 0;
   _col           = 0;
   _isBlackBox    = 0;
   _dataFlowStart = NULL;
   _dataFlowEnd   = NULL;
}

BaseModule::~BaseModule(){

}

void 
BaseModule::setFileName(const string& name)
{
   _fileName = name;
}

void 
BaseModule::setLineCol(unsigned short l, unsigned short c) 
{ 
   _line = l; 
   _col = c; 
}

void 
BaseModule::setIsBlackBox()  
{ 
   _isBlackBox = 1; 
}

bool 
BaseModule::isBlackBox() const 
{ 
   return (_isBlackBox ? true : false); 
}

const VlpSignalNode*
BaseModule::getSignalNode(const string& name) {
   const VlpSignalNode* sigNode = NULL;
   const VlpBaseNode* pos = _dataFlowStart;
   while (pos && (pos != _dataFlowEnd)) {
      if (pos->getNodeClass() == NODECLASS_SIGNAL) {
         sigNode = static_cast<VlpSignalNode*>(const_cast<VlpBaseNode*>(pos));
         if (sigNode->getSigName() == name) return sigNode;
      }
      pos = pos->getNext();
   }
   return NULL;
}

const char* 
BaseModule::getModuleName() const 
{ 
   return _moduleName.c_str(); 
}

const string 
BaseModule::getFileName() const 
{ 
   return _fileName; 
}

unsigned short 
BaseModule::getLineNum() const 
{ 
   return _line; 
}

void 
BaseModule::setPrim(VlpPrimitive* const p)        
{ 
   _primitiveAry.push_back(p); 
}
      
const PrimitiveAry* 
BaseModule::getPrimAry() const 
{ 
   return &_primitiveAry;
}

void 
BaseModule::listPrim() const
{
   for (unsigned i = 0; i < _primitiveAry.size(); ++i)
      _primitiveAry[i]->printPrim();
}

void 
BaseModule::setIO(VlpSignalNode* const io)        
{ 
   _IOAry.push_back(io); 
} 
     
int 
BaseModule::getIoSize() const 
{ 
   return _IOAry.size(); 
}
      
const VlpSignalNode*
BaseModule::getIoSignal(int i) const
{
   return _IOAry[i];
}
      
void  
BaseModule::printIO(bool isComplete) const
{
   if (isComplete) {
      cout << "===================IO List====================" << endl;
      for (unsigned i = 0; i < _IOAry.size(); ++i) {
         cout << i << " : ";
         _IOAry[i]->whoAmI(0, 100);
      }
      cout << "==============================================" << endl;
   }
   else {
      cout << "(";
      for (unsigned i = 0; i < _IOAry.size(); ++i) { 
         cout << _IOAry[i]->getSigName();
         if (i != _IOAry.size()-1)
            cout << ", ";
      }
      cout << ") ";
   }
} 

void 
BaseModule::setPort(const string& str, VlpPortNode* pn) 
{ 
   _portMap.insert(setName(str), pn); 
}

bool 
BaseModule::getPort(const string& key, VlpPortNode*& pn) const 
{ 
   return _portMap.getData(setName(key), pn);
}

const VlpPortNode*
BaseModule::getPortNode(unsigned int index) const 
{
   VlpPortNode* tmp;
   _portMap.getData(index, tmp);
   return tmp;
}
      
void 
BaseModule::dumpPortMap() const
{
   vector<unsigned> intArr;
   //vector<VlpPortNode*> pnArr;
   _portMap.dfs_key(intArr);
   for (unsigned i = 0; i < intArr.size(); ++i)
      cout << i << " : " << getName(intArr[i]) << endl;
}

void 
BaseModule::setDataFlow(VlpBaseNode* dataFlow)//use in linking
{
   if (_dataFlowStart != NULL) {
      _dataFlowEnd -> setNext(dataFlow);
      _dataFlowEnd = dataFlow;
   }
   else {
      _dataFlowStart = dataFlow;
      _dataFlowEnd = _dataFlowStart;
   }
}

const VlpBaseNode* 
BaseModule::getDataFlow(void) const 
{ 
   return _dataFlowStart; 
}

void 
BaseModule::printFlow() const
{
  const VlpBaseNode* pos = _dataFlowStart;
  for ( ; pos != NULL; pos = pos->getNext())
     cout << pos->getNodeClass() << " -> ";
  cout << "END" << endl;
}
           
//===========================class LibModule===============================//
LibModule::LibModule( string name )
          :BaseModule(name) {
}

LibModule::~LibModule() {
}

bool
LibModule::lintPort( const string& key ) const {
   if ( _portMap.exist( setName( key ) ) )
      return true;
   else
      return false;
}

bool
LibModule::isLibrary() const { //It is a liberty module.
   return true;
}

//===========================class VlgModule===============================//


VlgModule::VlgModule(string name)
          :BaseModule(name)
{
   _POLMap = NULL;
   _instanceAry.clear();
   //_paramAry.reserve(16);
}

VlgModule::~VlgModule()
{
   //need process carefully
   //dataflow linking list and instance linking list
   //if (_dataFlowStart != NULL)
   //   delete _dataFlowStart;
   //if (_dataFlowEnd != NULL)
   //   delete _dataFlowEnd;
   //if (_POLMap != NULL)
   //   delete _POLMap;
   //cerr << "~VlgModule()" << endl;
}

void 
VlgModule::setParam(const string& param, VlpBaseNode* const& content)
{
   paramPair* tmp = new paramPair(param, content);
   _paramAry.push_back(tmp);
}


void 
VlgModule::setParamNOL(string& instName, const paramPair*& pp)
{
   POLAry paramOLAry;
   if (_paramNOLMap.exist(instName)) {
      _paramNOLMap.getData(instName, paramOLAry);
      paramOLAry.push_back(pp);
   }
   else {
      paramOLAry.clear();
      paramOLAry.push_back(pp);
      _paramNOLMap.insert(instName, paramOLAry);
   }
}

void 
VlgModule::setPOLMap(string key, POLAry* pa) 
{ 
   if (_POLMap == NULL)
      _POLMap = new POLMap;
   
   _POLMap->insert(make_pair(key, pa)); 
}

void 
VlgModule::setMemAry(VlpMemAry* const ma)        
{ 
   _memAry.push_back(ma); 
}
      
void 
VlgModule::setInst(VlpInstance* const i)         
{ 
   _instanceAry.push_back(i); 
}
      
void 
VlgModule::setFunction(const string& str, VlpFunction* fn) 
{ 
   _functionMap.insert(setName(str), fn); 
}
   
bool 
VlgModule::isMemAry(const string& name) const
{
   for (MemAry::const_iterator it = _memAry.begin(); it != _memAry.end(); it++)
      if ((*it)->lintName(name)) {
         return true;
      }
   return false;
}

VlpMemAry*
VlgModule::getMemAry(const string& name) const
{
   for (MemAry::const_iterator it = _memAry.begin(); it != _memAry.end(); it++)
      if ((*it)->lintName(name)) {
         return (*it);
      }
   return NULL;
}

bool 
VlgModule::getFunction(const string& key, VlpFunction*& pn) const 
{ 
   return _functionMap.getData(setName(key), pn);
}
      
unsigned 
VlgModule::getInstSize() const 
{ 
   return _instanceAry.size(); 
}

VlpInstance* 
VlgModule::getInst(unsigned i) const 
{ 
   return _instanceAry[i]; 
}

unsigned 
VlgModule::getParamSize() const 
{ 
   return _paramAry.size(); 
}
      
const paramPair* 
VlgModule::getParamPair(unsigned i) const 
{ 
   return _paramAry[i]; 
}

VlpInstance* 
VlgModule::getInst(const string& instName) const
{   
   for (unsigned i = 0; i < _instanceAry.size(); ++i)
      if (_instanceAry[i]->getName() == instName)
         return _instanceAry[i];
   return NULL;
}

const POLMap* 
VlgModule::getPOLMap() const 
{ 
   return _POLMap; 
}

const ParamAry* 
VlgModule::getParamDeclaration() const 
{
   return &_paramAry;
}
      
bool 
VlgModule::getParamOL(const string& key, POLAry& pn) const 
{ 
   return _paramNOLMap.getData(key, pn);
}
      
void 
VlgModule::printCDFG(const VlpBaseNode* const node) const 
{ 
   node->whoAmI(0, 100); 
}
 
bool 
VlgModule::isLibrary() const 
{ 
   return false; 
}

bool 
VlgModule::lintPort(const string& key) const
{
   if (_portMap.exist(setName(key)))
      return true;
   else { // memElement
      for (unsigned i = 0; i < _memAry.size(); ++ i)
         if (_memAry[i]->lintName(key))
            return true;   
      return false;
   }
}

bool 
VlgModule::lintTF(const string& key) const
{
   if (lintPort(key) == true)
      return true;
   if (_taskMap.exist(setName(key)) == true)
      return true;
   if (_functionMap.exist(setName(key)) == true)
      return true;
   return false;
}

bool 
VlgModule::lintParam(const string& key, const paramPair*& bn) const
{
   for (unsigned i = 0; i < _paramAry.size(); ++i) {
      if (_paramAry[i]->param == key) {
         bn = (_paramAry[i]);
         return true;
      }
   }
   bn = NULL;
   return false;
}

bool 
VlgModule::lintInst(const string& key) const
{
   for (unsigned i = 0; i < _instanceAry.size(); ++i)
      if (key == getName(_instanceAry[i]->_moduleID))
         return true;

   return false;
}

void 
VlgModule::printModInfo() const
{
   cout << "File : " << _fileName << "    (line, col) = ( " << _line << ", " << _col << " )" << endl;
}

void 
VlgModule::dumpInstance() const
{
   for (unsigned i = 0; i < _instanceAry.size(); ++i)
      _instanceAry[i]->printInst();
}

void 
VlgModule::cstTree(VlpHierTreeNode* const parent, const ModuleMap& moduleMap) const
{
   VlpInstance* inst = NULL;
   BaseModule* mn = NULL;
   VlpHierTreeNode* newNode = NULL;

   for (unsigned i = 0; i < getInstSize(); ++i) {
      inst = _instanceAry[i];
      unsigned instID = inst->getModuleID();
      if (moduleMap.getData(instID, mn) == false) {
         cerr << "Error : The sub-module \"" << BaseModule :: getName(instID) 
              << "\" is not defined in the module \"" 
              << parent->getModule()->getModuleName() << "\"" << endl;
         exit(0);
      }
      newNode = inst->genHierTN(mn);
      parent->setChild(newNode);

      //============Handle pos mapping parameter overload===========//
      if (inst->getPOLSize() > 0) {//pos mapping
         assert (mn->isLibrary() == false);
         //pre-consider the parent module paramter overload ==> i.e. parameter propagation
         (static_cast<VlgModule*>(mn))
         ->setPOLMap(inst->getName(), inst->genPOLAry(static_cast<VlgModule*>(mn), parent));
      }
      //============================================================//
      if (!(mn->isBlackBox() || mn->isLibrary()))
         (static_cast<VlgModule*>(mn))->cstTree(newNode, moduleMap);
   }
}
      
//=======The function handle name mapping parameter overload=========//
void 
VlgModule::NamePOL_Module(const ModuleMap& moduleMap) const
{
   if (_paramNOLMap.getSize() > 0) { //parameter overload (name mapping)
      vector<string> instNameAry;
      vector<POLAry> NPA_vector; //OL Param Array
      _paramNOLMap.dfs_key(instNameAry);
      _paramNOLMap.dfs_data(NPA_vector);
      VlpInstance* instance;
      BaseModule* mn;

      for (unsigned i = 0; i < instNameAry.size(); ++i) {
         instance = getInst(instNameAry[i]);
         if (instance == NULL) {
            cerr << "Error : Instance \"" << instNameAry[i] << "\" doesn't exist in " 
                 << getFileName() << endl;
            exit(0);
         }
         moduleMap.getData(instance->getModuleID(), mn);
         assert (mn->isLibrary() == false);
         (static_cast<VlgModule*>(mn))->setPOLMap( instance->getName(), &(NPA_vector[i]) );
      }
   }
}

int 
VlgModule::getPOLSize() const 
{ 
   return _POLMap->size(); 
}

void 
VlgModule::printPOLT() const 
{
   cout << "********************************************" << endl;
   cout << "POL PTR = "<< _POLMap << endl;
   cout << "moduleName = " << _moduleName << endl;
   /*if (_POLMap != NULL) {
      cout << "number of OL instances = " << _POLMap->size() << endl;
      for (unsigned i = 0; i < _POLMap->size(); ++i) 
         cout << "NO "<< i << " : number of OL parameters = " << ((*_POLMap)[i])->size() << endl;
   }*/
}

#endif
