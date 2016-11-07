/****************************************************************************
  FileName     [ vlpSubElement.cpp ]
  Package      [ vlp ]
  Synopsis     [ Member function of Sub-Element class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_SUB_ELEMENT_SOURCE
#define VLP_SUB_ELEMENT_SOURCE

#include "vlpSubElement.h"
#include "vlpModule.h"

using namespace std;

// ============================================================================
// Vlpinstance
// ============================================================================
VlpInstance::VlpInstance()
{
   _isNameMap   =  0;
   _moduleID    =  0;//note : bst2 start at 0 !! //to change "bst2" to start at 1
   _rowNo       =  0;
   _colNo       =  0;
}

VlpInstance::~VlpInstance()
{
   //cout << "~VlpInstance()" << endl;
}

void 
VlpInstance::setNameMap()               
{ 
   _isNameMap = 1; 
}
      
void 
VlpInstance::setPosMap()                
{ 
   _isNameMap = 0; 
}

void 
VlpInstance::setModuleID(const int moduleID)  
{ 
   _moduleID = moduleID; 
}

void 
VlpInstance::setName(const string& str)       
{ 
   _name = str; 
}
      
void 
VlpInstance::setRowNo(const int rowNo)        
{ 
   _rowNo = rowNo; 
}

void 
VlpInstance::setColNo(const int colNo)        
{ 
   _colNo = colNo; 
}
      
void 
VlpInstance::setPosArr(VlpBaseNode* const element) 
{ 
   _pArray.push_back(element); 
}
      
void 
VlpInstance::setNameArr(const short no)       
{ 
   _nArray.push_back(no); 
}
      
void 
VlpInstance::setPOL(VlpBaseNode*& pn)    
{ 
   _polAry.push_back(pn); 
}
      
bool 
VlpInstance::isNameMap() const                
{ 
   return (_isNameMap ? true : false); 
}
      
int 
VlpInstance::getModuleID() const 
{ 
   return _moduleID; 
}
      
int 
VlpInstance::getPOLSize() const 
{ 
   return _polAry.size(); 
}
      
const string& 
VlpInstance::getName() const 
{ 
   return _name; 
}
      
void 
VlpInstance::printInst() const
{
   int j = 0;
   cout << "VlpInstance:" << endl;
   cout << "> Module: " << BaseModule::getName(_moduleID)
        << ">   Name: " << _name << endl;
   if (_nArray.size() != 0) {
      cout << "=======================================" << endl;
      cout << "Index    Signal Name" << endl;
      cout << "-------- ------------------------------" << endl;
      for (unsigned i = 0; i < _nArray.size(); ++i) {
         cout << setw(8) << i << " " << BaseModule::getName(_nArray[i])
              << endl;
         j = 0;
         _pArray[i]->whoAmI(j , 100);
      }
      cout << "=======================================" << endl;
   }
   else {
      cout << "=======================================" << endl;
      cout << "Position Signal Name" << endl;
      cout << "-------- ------------------------------" << endl;
      for (unsigned i = 0; i < _pArray.size(); ++i) {
         cout << setw(8) << i << " "<< endl;
         j = 0;
         _pArray[i]->whoAmI(j, 100);
      }
      cout << "=======================================" << endl;
   }

   if (_polAry.size() > 0) {
      cout << "> Parameter overload: " << endl;
      for (unsigned i = 0; i < _polAry.size(); ++i)
         _polAry[i]->whoAmI(0, 100);
   }
}

VlpHierTreeNode* const 
VlpInstance::genHierTN(const BaseModule* const modulePtr) const
{ 
   VlpHierTreeNode* r_node = new VlpHierTreeNode(_name, modulePtr, this);
   return r_node;
}

POLAry* const 
VlpInstance::genPOLAry(const VlgModule* const selfModule,
                       const VlpHierTreeNode* const& parentHTN) const
{// the function only handle parameter position overload
   POLAry* pa = new POLAry;
   paramPair* pPair;

   if (_polAry.size() != 0) {//pos mapping
      if ((_polAry.size()) > (selfModule->getParamSize())) {
         cerr << "Error : The number of overloaded parameters exceeds "
              << "the number of parameters defined in module." << endl;
         exit(0);
      }
      else {
         cout << "=========== overload info =============" << endl;
         for (unsigned i = 0; i < (_polAry.size()); ++i) {
            pPair = new paramPair(*(selfModule->getParamPair(i)));
            // if NodeClass == PARAMETER, need to check parameter overload of parents' module first
            if (_polAry[i]->getNodeClass() == NODECLASS_PARAMETER) {
               if (paramPropagation(parentHTN, pPair) == false) {
                  if (_polAry[i]->getNodeClass() == NODECLASS_PARAMETER)
                     pPair->content = static_cast<const VlpParamNode*>(_polAry[i])->getParamContent();
                  else
                     pPair->content = _polAry[i];
               }
            }
            else
               pPair->content = _polAry[i];

            pa->push_back(pPair);
            cout << i << " : " << endl;
            pPair->whoAmI(0, 100);
         }
         cout << "=======================================" << endl;
      }
   }
   return pa;
}

bool 
VlpInstance::paramPropagation(const VlpHierTreeNode* const& parentHTN, paramPair*& OLPair) const
{
   VlpBaseNode* olParam;
   const VlpInstance* const& parent_inst = parentHTN->getInst();
   assert (parentHTN->getModule()->isLibrary() == false);
   const VlgModule* const& parent_module = static_cast<const VlgModule*>(parentHTN->getModule());

   if (parent_inst == NULL) // parentHTN is root, ie. topMdoule, so parent_inst will be NULL
      return false;
   else if (parent_inst->checkPOL(olParam, OLPair->param, parent_module)) 
   {//the upper POL, ie. check the parameter overload of parents
      OLPair->content = olParam;
      return true;
   }
   else //this level POL, ie. the parent inatance doesn't overload the parameter
      return false;

}

bool 
VlpInstance::checkPOL(VlpBaseNode*& olParam, string& param, const VlgModule* const& parent_module) const
{
   for (unsigned i = 0; i < (parent_module->getParamSize()); ++i) {
      if (param == (parent_module->getParamPair(i)->param)) {
         if (i >= _polAry.size()) // the parents' module doesn't overload the parameter
            return false;
         else {
            olParam = _polAry[i]; // parents'
            return true;
         }
      }
   }
   return false;
}

// ============================================================================
// VlpPrimitive
// ============================================================================
VlpPrimitive::VlpPrimitive()
{
   _kind   = 0;
   _nameID = 0;// note : bst2 start at 0 !! //to change "bst2" to start at 1
   _rowNo  = 0;
   _colNo  = 0;
}

VlpPrimitive::~VlpPrimitive()
{
   //cout << "~VlpPrimitive()" << endl;
}
     
void 
VlpPrimitive::setKindID(const int kind)        
{ 
   _kind = kind; 
}
      
void 
VlpPrimitive::setNameID(const int nameID)      
{ 
   _nameID = nameID; 
}
      
void 
VlpPrimitive::setRowNo(const int rowNo)        
{ 
   _rowNo = rowNo; 
}
      
void 
VlpPrimitive::setColNo(const int colNo)        
{ 
   _colNo = colNo; 
}
      
void 
VlpPrimitive::setPosArr(VlpBaseNode* const element) 
{ 
   _pArray.push_back(element); 
}
/*      
const PArray* 
VlpPrimitive::getPArray() const 
{ 
   return &_pArray; 
}
      
int 
VlpPrimitive::getKind() const 
{ 
   return _kind; 
}*/

void 
VlpPrimitive::printPrim() const 
{
   cout << "VlpPrimitive: " << endl;
   cout << "> Kind: " << _kind << endl;
   cout << "> Name: " <<  BaseModule::getName( _nameID ) << endl;
   cout << "==========================================" << endl;
   cout << "Index    Primitive                        " << endl;
   cout << "-------- ---------------------------------" << endl;
   for ( unsigned int i = 0; i < _pArray.size(); ++i ) {
      cout << setw(8) << i;
      _pArray[i]->whoAmI(9, 100);
   }
   cout << "==========================================" << endl;
}

// ============================================================================
// VlpPortNode
// ============================================================================
VlpPortNode::VlpPortNode(VlpBaseNode* msb, VlpBaseNode* lsb) 
{
   _msb = msb;
   _lsb = lsb;
   _ioType     = 0;
   _netType    = 0;
   _drivStrFir = 0;
   _drivStrSec = 0;
   _rowNo      = 0;
   _colNo      = 0;
}

VlpPortNode::~VlpPortNode()
{
   //delete _msb;
   //delete _lsb;
   //cout << "~VlpPortNode()" << endl;
}

void 
VlpPortNode::setIOType(const IOType ioType)    
{ 
   _ioType = ioType; 
}

void 
VlpPortNode::setNetType(const NetType netType) 
{ 
   _netType = netType; 
}
     
void 
VlpPortNode::setDriveStr(const DriveStr _fir, const DriveStr _sec) 
{
   _drivStrFir = _fir;
   _drivStrSec = _sec; 
}
      
void 
VlpPortNode::setRowNo(const int rowNo) 
{ 
   _rowNo = rowNo; 
}
      
void 
VlpPortNode::setColNo(const int colNo) 
{ 
   _colNo = colNo; 
}
      
const VlpBaseNode* 
VlpPortNode::getMsb() const 
{
   return _msb;
}
      
const VlpBaseNode* 
VlpPortNode::getLsb() const 
{
   return _lsb;
}
      
const int 
VlpPortNode::getIoType() const 
{
   return _ioType;
}
      
const int 
VlpPortNode::getNetType() const 
{
   return _netType;
}
// ============================================================================
// VlpMemAry
// ============================================================================
VlpMemAry::VlpMemAry(string name, VlpBaseNode* msb, VlpBaseNode* lsb, VlpBaseNode* arrS, VlpBaseNode* arrE)
{
   _name = name;
   _msb  = msb;
   _lsb  = lsb;
   _arrS = arrS;
   _arrE = arrE;
}

VlpMemAry::~VlpMemAry()
{
   //delete _arrMin;
   //delete _arrMax;
   //cout << "~VlpMemAry()" << endl;
}
   
string 
VlpMemAry::getName() const
{
   return _name;
}

bool 
VlpMemAry::lintName(const string& name) 
{ 
   return name == _name ? true : false; 
}

bool
VlpMemAry::getIndex(int& top, int& down)
{
   top = _arrS->constPropagate();
   down = _arrE->constPropagate();
   if (top > down) {
      int temp = top;
      top = down;
      down = temp;
      return true;
   }
   else return false;
}

// ============================================================================
// VlpTask
// ============================================================================

VlpTask::VlpTask(const string& key)
{
   _nameId = BaseModule :: setName(key);
}

VlpTask::~VlpTask()
{
   //cout << "~VlpTask()" << endl;
}

void 
VlpTask::setIO(VlpSignalNode* const io)       
{ 
   _IOAry.push_back(io); 
}
      
void 
VlpTask::setStatement(VlpBaseNode* statement) 
{ 
   _statement = statement; 
}
      
void 
VlpTask::setMemAry(VlpMemAry* const ma)        
{ 
   _memList.push_back(ma); 
}

void 
VlpTask::setPort(const string& key, VlpPortNode* pn)
{
   _portMap.insert(BaseModule :: setName(key), pn); 
}

bool 
VlpTask::getPort(const string& key, VlpPortNode*& pn) const 
{
   return _portMap.getData(BaseModule :: setName(key), pn);
}

bool 
VlpTask::lintIO(const string& key) const
{
   for (unsigned i = 0; i < _IOAry.size(); ++i) {
      if (BaseModule :: setName(key) == _IOAry[i]->getNameId())
         return true;
   }
   return false;
}

bool 
VlpTask::lintPort(const string& key) const
{
   if (_portMap.exist(BaseModule :: setName(key)))
      return true;
   else { // memElement
      for (unsigned i = 0; i < _memList.size(); ++i)
         if (_memList[i]->lintName(key))
            return true;
      return false;
   }
}

void 
VlpTask::setParam(const string& param, VlpBaseNode* const content)
{
   paramPair* tmp = new paramPair(param, content);
   _paramArr.push_back(tmp);
}

bool 
VlpTask::lintParam(const string& key, const paramPair*& bn) const
{
   for (unsigned i = 0; i < _paramArr.size(); ++i) {
      if (_paramArr[i]->param == key) {
         bn = (_paramArr[i]);
         return true;
      }
   }
   bn = NULL;
   return false;
}

// ============================================================================
// VlpFunction
// ============================================================================
VlpFunction::VlpFunction(bool isIntOutput, VlpSignalNode* fOutput)
{
   _isIntOutput = isIntOutput;
   _fOutput = fOutput;
}

VlpFunction::~VlpFunction()
{
   delete _fOutput;
}

void 
VlpFunction::setInput(VlpSignalNode* pn) 
{ 
   _inputs.push_back(pn); 
}
      
void 
VlpFunction::setMemAry(VlpMemAry* const ma)        
{ 
   _memList.push_back(ma); 
}
      
void 
VlpFunction::setStatement(VlpBaseNode* statement) 
{ 
   _statement = statement; 
}
      
const IOAry* 
VlpFunction::getIoAry() const 
{ 
   return &_inputs; 
}
      
const PortMap* 
VlpFunction::getPortMap() const 
{ 
   return &_portMap; 
}

void 
VlpFunction::setParam(const string& param, VlpBaseNode* const content)
{
   paramPair* tmp = new paramPair(param, content);
   _paramArr.push_back(tmp);
}

bool 
VlpFunction::lintParam(const string& key, const paramPair*& bn) const
{
   for (unsigned i = 0; i < _paramArr.size(); ++i) {
      if (_paramArr[i]->param == key) {
         bn = (_paramArr[i]);
         return true;
      }
   }
   bn = NULL;
   return false;
}

bool 
VlpFunction::lintPort(const string& key) const
{
   if (_portMap.exist(BaseModule :: setName(key)))
      return true;
   else if (BaseModule :: setName(key) == _fOutput->getNameId())
      return true;
   else { // memElement
      for (unsigned i = 0; i < _memList.size(); ++i)
         if (_memList[i]->lintName(key))
            return true;
      return false;
   }
}

void 
VlpFunction::setPort(const string& key, VlpPortNode* pn)
{
   _portMap.insert(BaseModule :: setName(key), pn); 
}

bool 
VlpFunction::getPort(const string& key, VlpPortNode*& pn) const 
{
   return _portMap.getData(BaseModule :: setName(key), pn);
}

bool
VlpFunction::getPort( const int key, VlpPortNode*& pn ) const 
{
   return _portMap.getData( key, pn );
}

const VlpSignalNode*
VlpFunction::outputNode() const 
{
   return _fOutput;
}

vector< VlpFSMAssign >
//vector< VlpFSMSIG >
VlpFunction::extractAL(map<int, int>& nameMap, map<int, const VlpSignalNode*>& sigMap) const 
{
   vector< VlpFSMAssign >     local;
   map< MyString, VlpFSMSIG > SIGS;
   map< MyString, VlpFSMSIG >::iterator pos;
   map< int, int >::iterator posName;
   vector< VlpFSMSIG >        ret;
   VlpFSMSIG                  sig;
   MyString                   sigName;

   const VlpBaseNode          *curNode, *tmpNode, *newNode, *sameNode, *caseNode;
   const VlpCaseItemNode*     itemNode;
   const VlpIfNode*           ifNode;
   const VlpConditionalNode*  condNode;
   const VlpOperatorNode*     oprNode;

   VlpFSMAssign               newAL;
   stack< size_t >            condStack;
   FSMCallBack                bufCallBack;
   stack< FSMCallBack >       callBack;
   unsigned int               i;
   bool*                      sigMark;
   int                        nId1,
                              nId2;
   vector< VlpSignalNode* >   lstSIG;
   int                        tab;

   cout << "> From FUNCTION Block( " << this << " )";
   resolveIndex();
   newNode = _statement->duplicate();
   lstSIG  = newNode->listSIGs();
   curNode = newNode;
   nId1 = static_cast<const VlpSignalNode*>(outputNode())->getNameId();
   while ( curNode != NULL ) {
      if ( curNode->getNodeClass() == NODECLASS_BA_OR_NBA ) {
         tmpNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getLeft();
         if ( static_cast<const VlpSignalNode*>(tmpNode)->getNameId() == nId1 ){
            tmpNode = static_cast<const VlpBAorNBA_Node*>(
                                  curNode)->getRight();
            nId2 = static_cast<const VlpSignalNode*>(tmpNode)->getNameId();
            nameMap[nId2] = nameMap[nId1];
         }
      }
      curNode = curNode->getNext();
   }
   sigMark = new bool[ lstSIG.size() ];
   for ( i = 0; i < lstSIG.size(); ++i )
      sigMark[i] = false;
   cout << "  Name Mapping Information: " << endl;
   cout << "  =================================================" << endl;
   cout << "  Internal Declaration     External Function Call" << endl;
   cout << "  ------------------------ ------------------------" << endl;
   for ( posName = nameMap.begin(); posName != nameMap.end(); posName++ ) {
      cout << "  " << setw(24) << left << BaseModule::getName( posName->first )
           << " " << setw(24) << left << BaseModule::getName( posName->second )
           << "[" << sigMap[ posName->second ]->MSB() << ":"
           << sigMap[ posName->second ]->LSB() << "]"
           << endl;
      for ( i = 0; i < lstSIG.size(); ++i ) {
         if ( lstSIG[i]->getNameId() == posName->first && sigMark[i] == false ) {
            lstSIG[i]->setNameId( posName->second );
            lstSIG[i]->setMSB( const_cast<VlpBaseNode*>(sigMap[ posName->second ]->getMsb()) );
            lstSIG[i]->setLSB( const_cast<VlpBaseNode*>(sigMap[ posName->second ]->getLsb()) );
            sigMark[i] = true;
         }
      }
   }
   cout << "  =================================================" << endl;
   delete [] sigMark;
//   curNode  = _statement;
   caseNode = NULL;
   bufCallBack.reset();
   curNode = newNode;
   newAL.setNonSeq();
   if (newAL.isSeq()) cout << " Sequential";
   else               cout << " Combinational";

   cout << " Type" << endl;
   while (curNode != NULL) {
      tab = (condStack.size() * 2) + 2;
      switch (curNode->getNodeClass()) {
      case NODECLASS_IF:
         cout << setw(tab) << right << ">" << " Deep to IF block" << endl;
         ifNode = static_cast< const VlpIfNode* >( curNode );

         bufCallBack._isCase = false;
         bufCallBack._return = curNode->getNext();
         bufCallBack._cond   = size_t( NULL );
         tmpNode             = ifNode->getExp();
         callBack.push( bufCallBack );
         if (ifNode->getFalseChild() != NULL) {
            bufCallBack._return = ifNode->getFalseChild();
            bufCallBack._cond   = size_t(tmpNode) | 0x00000001;
            callBack.push(bufCallBack);
         }
         condStack.push(size_t(tmpNode));
         curNode = ifNode->getTrueChild();
         continue;
         break;
      case NODECLASS_CASE:
         caseNode = static_cast<const VlpCaseNode*>(curNode)->getExp();
         cout << setw(tab) << right << ">" << " Deep to CASE block( "
              << caseNode->str() << " )" << endl;
         bufCallBack._isCase = true;
         bufCallBack._return = curNode->getNext();
         bufCallBack._cond   = size_t(NULL);
         callBack.push(bufCallBack);
         curNode = static_cast<const VlpCaseNode*>(curNode)->getInner();
         continue;
         break;
      case NODECLASS_CASEITEM:
         itemNode = static_cast<const VlpCaseItemNode*>(curNode);
         cout << setw(tab) << right << ">" << " Deep to CASEITEM block( "
              << itemNode->getExp()->str() << " )" << endl;
         assert(caseNode != NULL);
         tmpNode = new VlpEqualityNode(EQUALITY_LOG_EQU, const_cast<VlpBaseNode*>(caseNode),
                                       const_cast<VlpBaseNode*>(itemNode->getExp()));
         condStack.push(size_t(tmpNode));
         bufCallBack._isCase = false;
         bufCallBack._return = curNode->getNext();
         bufCallBack._cond   = size_t(NULL);
         callBack.push(bufCallBack);
         curNode = itemNode->getInner();
         continue;
         break;
      case NODECLASS_OPERATOR:
         if (static_cast<const VlpOperatorNode*>(curNode)->getOperatorClass() == OPCLASS_CONDITIONAL ) {
             cout << setw(tab) << right << ">" << " Conditional node, apply "
                  << "SPLIT procedure" << endl;
             bufCallBack._isCase = false;
             bufCallBack._return = curNode->getNext();
             bufCallBack._cond   = size_t(NULL);
             callBack.push(bufCallBack);
             condNode = static_cast<const VlpConditionalNode*>(curNode);
             bufCallBack._isCase = false;
             bufCallBack._return = condNode->getFalse();
             bufCallBack._cond   = size_t(condNode->getCondition()) | 0x00000001;
             callBack.push(bufCallBack);
             condStack.push(size_t(condNode->getCondition()));
             curNode = condNode->getTrue();
             continue;
         }
         else 
            ;
         break;
      case NODECLASS_BA_OR_NBA:
         cout << setw(tab) << right << ">" << " Assignment" << endl;
         tmpNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getRight();
         if (tmpNode->getNodeClass() == NODECLASS_OPERATOR) {
            oprNode = static_cast<const VlpOperatorNode*>(tmpNode);
            if (oprNode->getOperatorClass() == OPCLASS_CONDITIONAL) {
               bufCallBack._isCase = false;
               bufCallBack._return = curNode->getNext();
               bufCallBack._cond = size_t(NULL);
               callBack.push(bufCallBack);
               condNode = static_cast<const VlpConditionalNode*>(oprNode);
               newAL.setTS(static_cast<const VlpBAorNBA_Node*>(curNode)->getLeft());
               bufCallBack._isCase = false;
               bufCallBack._return = condNode->getFalse();
               bufCallBack._cond = size_t(condNode->getCondition()) | 0x00000001;
               callBack.push(bufCallBack);
               condStack.push( size_t(condNode->getCondition()) );
               curNode = condNode->getTrue();
               continue;
            }
            else {
               newAL.setAC(condStack);
               newAL.setSE(tmpNode);
               tmpNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getLeft();
               newAL.setTS(tmpNode);
               local.push_back(newAL);
            }
         }
         else {
            sameNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getLeft();
            if ( tmpNode->getNodeClass() == NODECLASS_SIGNAL 
             && sameNode->getNodeClass() == NODECLASS_SIGNAL ) 
            {
               if (static_cast<const VlpSignalNode*>(tmpNode)->getNameId() ==
                   static_cast<const VlpSignalNode*>(sameNode)->getNameId() )
                  break;
            }
            newAL.setAC(condStack);
            newAL.setSE(tmpNode);
            tmpNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getLeft();
            newAL.setTS(tmpNode);
            local.push_back(newAL);
         }
         break;
      case NODECLASS_INT:
      case NODECLASS_PARAMETER:
      case NODECLASS_SIGNAL:
         newAL.setSE(curNode);
         newAL.setAC(condStack);
         local.push_back(newAL);
         break;
      default:
         cerr << "VlpAlwaysNode::extractAL() ---> WARNING: Unsupported class" << endl;
      }

      curNode = curNode->getNext();
      if (curNode == NULL && callBack.size() > 0) {
         do {
            bufCallBack = callBack.top();
            curNode = bufCallBack._return;
            if (bufCallBack._isCase == false && condStack.size() > 0) {
               condStack.pop();
               cout << "pop cond" << endl;
            }
            cout << "up level" << endl;
            callBack.pop();
         } while (curNode == NULL && callBack.size() > 0);

         if (bufCallBack._cond != size_t(NULL))
            condStack.push(bufCallBack._cond);
      }
   }
   cout << "> End of FUNCTION block( " << this << " ), " << local.size()
        << " ALs extracted." << endl;
/*
   for ( i = 0; i < local.size(); ++i ) {
      cout << "AL: " << i << endl;
      cout << local[i] << endl;
      sigName = local[i].strTS();
      if ( SIGS.find( sigName ) == SIGS.end() ) {
         sig.setName( sigName );
         SIGS.insert( pair< MyString, VlpFSMSIG >( sigName, sig ) );
      }
      SIGS[sigName].addAssign( local[i] );
   }

   for ( pos = SIGS.begin(); pos != SIGS.end(); pos++ ) {
      ret.push_back( pos->second );
   }

   return ret;
*/
   return local;
}

void
VlpFunction::resolveIndex() const {
   vector< VlpSignalNode* > SIGS;
   VlpPortNode*             port;
   unsigned int             i;

   if ( _statement == NULL )
      return;

   SIGS = _statement->listSIGs();

   for ( i = 0; i < SIGS.size(); ++i ) {
      if ( static_cast<VlpSignalNode*>(SIGS[i])->getMsb() == NULL ) {
         if ( _portMap.getData( SIGS[i]->getNameId(), port ) ) {
            SIGS[i]->setMSB( const_cast<VlpBaseNode*>(port->getMsb()) );
            SIGS[i]->setLSB( const_cast<VlpBaseNode*>(port->getLsb()) );
         }
      }
   }
}

#endif
