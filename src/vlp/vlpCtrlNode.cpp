/****************************************************************************
  FileName     [ vlpCtrlNode.cpp ]
  Package      [ vlp ]
  Synopsis     [ Member function of Control Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_CTRLNODE_SOURCE
#define VLP_CTRLNODE_SOURCE

#include "vlpCtrlNode.h"
#include <iostream>

using namespace std;

static const char* casetype[3] = {"case", "casex", "casez"};
static const char* edgetype[4] = {"edge", "posedge", "negedge", "sensitive"};
static const char* assigntype[2] = {"block-assignment", "nonblock-assignment"};

// ============================================================================
// VlpAssignNode
// ============================================================================
VlpAssignNode::VlpAssignNode(VlpBaseNode* exp, VlpBaseNode* next) 
              : VlpBaseNode(NODECLASS_ASSIGN)
{
   _exp  = exp;
   _next = next;
}

VlpAssignNode::~VlpAssignNode()
{
   if (_exp)  delete _exp;
   if (_next) delete _next;
}

void 
VlpAssignNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "assign";
   if (l > 0) {
      if (_exp) {
         cout << " : " << "EXP   " << " --> ";  _exp->whoAmI(i+20, --l);
      }
      if (_next) {
         cout.width(i+6);
         cout << "NEXT  " << endl;
         _next->whoAmI(i, --l);
      }
   }
   else
     cout << endl;
}

void
VlpAssignNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpAssignNode::str() const {
   return _exp->str();
}

VlpBaseNode*
VlpAssignNode::duplicate() const {
   VlpAssignNode* dup = new VlpAssignNode( _exp->duplicate(), NULL );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );
   return dup;
}

vector< VlpSignalNode* >
VlpAssignNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp = _exp->listSIGs();
   if ( _next != NULL ) {
      tmpR = _next->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }
   return tmp;
}

// ============================================================================
// VlpAlwaysNode
// ============================================================================
VlpAlwaysNode::VlpAlwaysNode(VlpBaseNode* exp, VlpBaseNode* inner, 
                                               VlpBaseNode* next  )
              : VlpBaseNode(NODECLASS_ALWAYS)
{
   _exp   = exp;
   _inner = inner;
   _next  = next;
}

VlpAlwaysNode::~VlpAlwaysNode()
{
   if (_exp)   delete _exp;
   if (_inner) delete _inner;
   if (_next)  delete _next;
}

void 
VlpAlwaysNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "always";
   if (l > 0) {
      if (_exp) {
         cout << " : " << "EXP   " << " --> "; _exp->whoAmI(i+20, --l);
      }
      if (_inner) {
         cout.width(i+9);
         cout << " : ";
         cout << "INNER " << " --> "; _inner->whoAmI(i+20, --l);
      }
      if (_next) {
         cout.width(i+6);
         cout << "NEXT  " << endl;
         _next->whoAmI(i, --l);
      }
   }
   else
      cout << endl;
}

void
VlpAlwaysNode::writeWhoAmI(ofstream& os) const
{
}

VlpBaseNode*
VlpAlwaysNode::duplicate() const {
   VlpAlwaysNode* dup = new VlpAlwaysNode( _exp->duplicate(),
                                           _inner->duplicate(), NULL );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );
   return dup;
}

vector<VlpSignalNode*>
VlpAlwaysNode::listSIGs() const 
{
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp = _exp->listSIGs();
   if ( _inner != NULL ) {
      tmpR = _inner->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }
   if ( _next != NULL ) {
      tmpR = _next->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }
   return tmp;
}

vector<VlpFSMAssign>
VlpAlwaysNode::extractAL() const 
{
   const VlpBaseNode*        tmpNode;
   const VlpCaseItemNode*    itemNode;
   const VlpIfNode*          ifNode;
   const VlpConditionalNode* condNode;
   const VlpOperatorNode*    oprNode;

   vector<VlpFSMAssign>      local;
   stack<size_t>             condStack;
   stack<FSMCallBack>        callBack;
   int                       tab;
   cout << "> From ALWAYS Block( " << this << " )";
   const VlpBaseNode* curNode  = _inner;
   const VlpBaseNode* caseNode = NULL;
   FSMCallBack bufCallBack;
   bufCallBack.reset();
   
   VlpFSMAssign newAL;
   if (_exp->getNodeClass() == NODECLASS_EDGE) {
      if (static_cast<VlpEdgeNode*>(_exp)->getEdgeType() == EDGETYPE_SENSITIVE)
         newAL.setNonSeq();
      else
         newAL.setSeq();
   }
   else if (_exp->getNodeClass() == NODECLASS_OR) {
      tmpNode = static_cast<const VlpOrNode*>(_exp)->getContent();
      if (static_cast<const VlpEdgeNode*>(tmpNode)->getEdgeType() == EDGETYPE_SENSITIVE)
         newAL.setNonSeq();
      else
         newAL.setSeq();
   }
   else {
      cerr << "VlpAlwaysNode::extractAL() ---> ERROR: Faild to recognize "
           << "sensitive type from sensitivity list." << endl;
      newAL.setNonSeq();
   }
   if (newAL.isSeq()) cout << " Sequential";
   else               cout << " Combinational";
   cout << " Type" << endl;
   while (curNode != NULL) {
      newAL.setAC(condStack);
      tab = (condStack.size() * 2) + 2;
      switch (curNode->getNodeClass()) {
      case NODECLASS_IF:
         cout << setw(tab) << right << ">" << " Deep to IF block" << endl;
         ifNode = static_cast<const VlpIfNode*>(curNode);
         bufCallBack._isCase = false;
         bufCallBack._return = curNode->getNext();
         bufCallBack._cond   = size_t(NULL);
         tmpNode             = ifNode->getExp();
         callBack.push(bufCallBack);

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
      case NODECLASS_OPERATOR: // only handle conditional operator
         if (static_cast<const VlpOperatorNode*>(curNode)->getOperatorClass() == OPCLASS_CONDITIONAL ) {
             cout << setw(tab) << right << ">" << " Conditional node, apply SPLIT procedure" << endl;
             bufCallBack._isCase = false;
             bufCallBack._return = curNode->getNext();
             bufCallBack._cond   = size_t(NULL);
             callBack.push(bufCallBack);

             condNode = static_cast<const VlpConditionalNode*>(curNode);
             bufCallBack._isCase = false;
             bufCallBack._return = condNode->getFalse();
             bufCallBack._cond   = size_t(condNode->getCondition())|0x00000001;
             callBack.push(bufCallBack);

             condStack.push(size_t(condNode->getCondition()));
             curNode = condNode->getTrue();
             continue;
         }
         else {

         }
         break;
      case NODECLASS_BA_OR_NBA:
         cout << setw(tab) << right << ">" << " Assignment: " << curNode->str() << endl;
         tmpNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getRight(); // RHS
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
               bufCallBack._cond = size_t(condNode->getCondition())|0x00000001;
               callBack.push(bufCallBack);
               condStack.push(size_t(condNode->getCondition()));
               curNode = condNode->getTrue();
               continue;
            }
            else {
               newAL.setAC(condStack);
               newAL.setSE(tmpNode);
               tmpNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getLeft();
               if (tmpNode->getNodeClass() == NODECLASS_SIGNAL) {
                  newAL.setTS(tmpNode);
                  local.push_back(newAL);
               }
            }
         }
         else {
            newAL.setAC(condStack);
            newAL.setSE(tmpNode);
            tmpNode = static_cast<const VlpBAorNBA_Node*>(curNode)->getLeft();
            if ( tmpNode->getNodeClass() == NODECLASS_SIGNAL ) {
               newAL.setTS( tmpNode );
               local.push_back( newAL );
            }
         }
         break;
      case NODECLASS_INT:
      case NODECLASS_PARAMETER:
      case NODECLASS_SIGNAL:
         newAL.setSE(curNode);
         newAL.setAC(condStack);
         local.push_back(newAL);
         break;
      default: // no NODECLASS_FUNCALL => need to implement if necessary 
         cerr << "VlpAlwaysNode::extractAL() ---> WARNING: Unsupported class"
              << endl;
      }

      curNode = curNode->getNext();
      if (curNode == NULL && callBack.size() > 0) {
         do {
            bufCallBack = callBack.top();
            curNode = bufCallBack._return;
            if (bufCallBack._isCase == false && condStack.size() > 0)
               condStack.pop();

            callBack.pop();
         } while (curNode == NULL && callBack.size() > 0);

         if (bufCallBack._cond != size_t(NULL))
            condStack.push(bufCallBack._cond);
      }
   }
   cout << "> End of ALWAYS block( " << this << " ), " << local.size()
        << " ALs extracted." << endl;
   return local;
}

// ============================================================================
// VlpOrNode
// ============================================================================
VlpOrNode::VlpOrNode(VlpBaseNode* content) 
          :VlpBaseNode(NODECLASS_OR)
{
   _content = content;
}

VlpOrNode::~VlpOrNode()
{
   if (_content) delete _content;
} 

void 
VlpOrNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "or    ";
   if (l > 0) {
      if (_content) {
         cout << " : " << "CTENT " << " --> "; 
         _content->whoAmI(i+20, --l);
      }
   }
   else
      cout << endl;
}

void
VlpOrNode::writeWhoAmI(ofstream& os) const
{
}

VlpBaseNode*
VlpOrNode::duplicate() const {
   VlpOrNode* dup = new VlpOrNode( _content->duplicate() );
   return dup;
}

vector< VlpSignalNode* >
VlpOrNode::listSIGs() const {
   return _content->listSIGs();
}

// ============================================================================
// VlpEdgeNode
// ============================================================================
VlpEdgeNode::VlpEdgeNode(EdgeType edgeType, VlpBaseNode* exp)
            :VlpBaseNode(NODECLASS_EDGE)
{
   _edgeType = edgeType;
   _exp      = exp;
   _next     = NULL;
}

VlpEdgeNode::~VlpEdgeNode()
{
   if (_exp)  delete _exp;
   if (_next) delete _next;
}
      
void 
VlpEdgeNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "edge  " << " : " << edgetype[_edgeType] << endl;
   if (l > 0) {
      if (_exp) {
         cout.width(i+9);
         cout << " : ";
         cout << "EXP   " << " --> "; _exp->whoAmI(i+20, --l);
      }
      if (_next) {
         cout.width(i+6);
         cout << "NEXT  " << endl;
         cout.width(i);
         cout << " --> ";
         _next->whoAmI(i, --l);
      }
   }
}

void
VlpEdgeNode::writeWhoAmI(ofstream& os) const
{
}

VlpBaseNode*
VlpEdgeNode::duplicate() const {
   VlpEdgeNode* dup = new VlpEdgeNode( _edgeType, _exp->duplicate() );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );

   return dup;
}

vector< VlpSignalNode* >
VlpEdgeNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp = _exp->listSIGs();
   if ( _next != NULL ) {
      tmpR = _next->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }
   return tmp;
}

// ============================================================================
// VlpIfNode
// ============================================================================
VlpIfNode::VlpIfNode(VlpBaseNode* exp, VlpBaseNode* trueChild, 
                     VlpBaseNode* falseChild, VlpBaseNode* next)
          : VlpBaseNode(NODECLASS_IF)
{  
   _exp        = exp;
   _trueChild  = trueChild;
   _falseChild = falseChild;
   _next       = next;
}

VlpIfNode::~VlpIfNode()
{
   if (_exp)  delete _exp;
   if (_next) delete _next;
   if (_trueChild)  delete _trueChild;
   if (_falseChild) delete _falseChild;
}

void 
VlpIfNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "if    ";
   if (l > 0) {
      if (_exp) {
         cout << " : " << "EXP   " << " --> "; _exp->whoAmI(i+20, --l);
      }
      if (_trueChild) {
         cout.width(i+9);
         cout << " : ";
         cout << "TRUE  " << " --> "; _trueChild->whoAmI(i+20, --l);
      }
      if (_falseChild) {
         cout.width(i+9);
         cout << " : ";
         cout << "FALSE " << " --> "; _falseChild->whoAmI(i+20, --l);
      }
      if (_next) {
         cout.width(i+6);
         cout << "NEXT  " << endl;
         cout.width(i);
         cout << " --> ";
         _next->whoAmI(i, --l);
      }
   }
   else
      cout << endl;
}

void
VlpIfNode::writeWhoAmI(ofstream& os) const
{
}

VlpBaseNode*
VlpIfNode::duplicate() const {
   VlpIfNode* dup = new VlpIfNode( _exp->duplicate(),
                                   _trueChild->duplicate(), NULL, NULL );

   if ( _falseChild != NULL )
      dup->_falseChild = _falseChild->duplicate();
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );
   return dup;
}

vector< VlpSignalNode* >
VlpIfNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _exp->listSIGs();
   tmpR = _trueChild->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   if ( _falseChild != NULL ) {
      tmpR = _falseChild->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }

   if ( _next != NULL ) {
      tmpR = _next->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }

   return tmp;
}

// ============================================================================
// VlpCaseNode
// ============================================================================
VlpCaseNode::VlpCaseNode(CaseType caseType, VlpBaseNode* exp, 
                         VlpBaseNode* inner, VlpBaseNode* next)
            : VlpBaseNode(NODECLASS_CASE)
{
   _caseType       = caseType;
   _exp            = exp;
   _inner          = inner;
   _next           = next;
   _isParallelCase = false;
   _isFullCase     = false;
}

VlpCaseNode::~VlpCaseNode()
{
   if (_exp)   delete _exp;
   if (_inner) delete _inner;
   if (_next)  delete _next;
}

void 
VlpCaseNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "case  " << " : " << casetype[_caseType]; 
   cout << "(isFullCase, isParallelCase) = ( " << _isFullCase << ", " 
        << _isParallelCase << ")" << endl;
   if (l > 0) {
      if (_exp) {
         cout.width(i+9);
         cout << " : ";
         cout << "EXP   " << " --> "; _exp->whoAmI(i+20, --l);  
      }
      if (_inner) {
         cout.width(i+9);
         cout << " : ";
         cout << "INNER " << " --> "; _inner->whoAmI(i+20, --l);
      }
      if (_next) {
         cout.width(i+6);
         cout << "NEXT  " << endl;
         cout.width(i);
         cout << " --> ";
         _next->whoAmI(i, --l);
      }
   }
}

void
VlpCaseNode::writeWhoAmI(ofstream& os) const
{
}

VlpBaseNode*
VlpCaseNode::duplicate() const {
   VlpCaseNode* dup = new VlpCaseNode( _caseType, _exp->duplicate(),
                                       _inner->duplicate(), NULL );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );

   dup->setParallelCase( _isParallelCase );

   dup->setFullCase( _isFullCase );

   return dup;
}

vector< VlpSignalNode* >
VlpCaseNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _exp->listSIGs();
   tmpR = _inner->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[ i ] );

   if ( _next != NULL ) {
      tmpR = _next->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }
   return tmp;
}

// ============================================================================
// VlpForNode
// ============================================================================
VlpForNode::VlpForNode(VlpBaseNode* initState, VlpBaseNode* endCond,
                       VlpBaseNode* increState, VlpBaseNode* inner,
                       VlpBaseNode* next)
           : VlpBaseNode(NODECLASS_FOR)
{
   _initState  = initState;
   _endCond    = endCond;
   _increState = increState;
   _inner      = inner;
   _next       = next;
}

VlpForNode::~VlpForNode()
{
   if (_next)  delete _next;
   if (_inner) delete _inner;
   if (_endCond) delete _endCond;
   if (_initState)  delete _initState;
   if (_increState) delete _increState;
}
      
void 
VlpForNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "for   ";
   if (l > 0) {
      if (_initState) {
         cout << " : " << "INITST" << " --> "; _initState->whoAmI(i+20, --l);
      }
      if (_endCond) {
         cout.width(i+9);
         cout << " : ";
         cout << "ENDCD " << " --> "; _endCond->whoAmI(i+20, --l); 
      }
      if (_increState) {
         cout.width(i+9);
         cout << " : ";
         cout << "INCRST" << " --> "; _increState->whoAmI(i+20, --l);
      }
      if (_inner) {
         cout.width(i+9);
         cout << " : ";
         cout << "INNER " << " --> "; _inner->whoAmI(i+20, --l);
      }
      if (_next) {
         cout.width(i+6);
         cout << "NEXT  " << endl;
         _next->whoAmI(i+9, --l);
      }
   }
   else
      cout << endl;
}

void
VlpForNode::writeWhoAmI(ofstream& os) const
{
}

VlpBaseNode*
VlpForNode::duplicate() const {
   VlpForNode* dup = new VlpForNode( _initState->duplicate(),
                                     _endCond->duplicate(),
                                     _increState->duplicate(),
                                     _inner->duplicate(), NULL );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );
   return dup;
}

vector< VlpSignalNode* >
VlpForNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _initState->listSIGs();
   tmpR = _endCond->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   tmpR = _increState->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   tmpR = _inner->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   if ( _next != NULL ) {
      tmpR = _next->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }
   return tmp;
}

// ============================================================================
// VlpBAorNBA_Node
// ============================================================================
VlpBAorNBA_Node::VlpBAorNBA_Node( AssignType assignType,
                                  VlpBaseNode* leftSide, 
                                  VlpBaseNode* rightSide, 
                                  VlpBaseNode* next       )
                : VlpBaseNode(NODECLASS_BA_OR_NBA)
{
   _assignType = assignType;
   _leftSide   = leftSide;
   _rightSide  = rightSide;
   _next       = next;
}

VlpBAorNBA_Node::~VlpBAorNBA_Node()
{
   if (_next) delete _next;
   if (_leftSide)  delete _leftSide;
   if (_rightSide) delete _rightSide;
}

void 
VlpBAorNBA_Node::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "BA_NBA" << " : " << assigntype[_assignType] << endl;
   if (l > 0) {
      if (_leftSide) {
         cout.width(i+9);
         cout << " : ";
         cout << "LSIDE " << " --> "; _leftSide->whoAmI(i+20, --l);
      }
      if (_rightSide) {
         cout.width(i+9);
         cout << " : ";
         cout << "RSIDE " << " --> "; _rightSide->whoAmI(i+20, --l);
      }
      if (_next) {
         cout.width(i+6);
         cout << "NEXT  " << endl;
         cout.width(i);
         cout << " --> ";
         _next->whoAmI(i, --l);
      }
   }
}

void
VlpBAorNBA_Node::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpBAorNBA_Node::str() const {
   MyString ret;

   ret = _leftSide->str();
   ret += " = ";
   ret += _rightSide->str();

   return ret;
}

VlpBaseNode*
VlpBAorNBA_Node::duplicate() const {
   VlpBAorNBA_Node* dup = new VlpBAorNBA_Node( _assignType,
                                               _leftSide->duplicate(),
                                               _rightSide->duplicate(), NULL );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );
   return dup;
}

vector< VlpSignalNode* >
VlpBAorNBA_Node::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _leftSide->listSIGs();
   tmpR = _rightSide->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   if ( _next != NULL ) {
      tmpR = _next->listSIGs();
      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }

   return tmp;
}

#endif
