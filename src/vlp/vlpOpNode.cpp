/****************************************************************************
  FileName     [ vlpOpNode.cpp ]
  Package      [ vlp ]
  Synopsis     [ Member function of Operation Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_OPNODE_SOURCE
#define VLP_OPNODE_SOURCE

#include "vlpOpNode.h"
#include "vlpModule.h"
#include "VLGlobe.h"

extern const VlgModule* curVlpModule;
extern SYN_usage*       SYN;

static const char* bitwiseopclass[6] = {"~(bitwise)", "&(bitwise)", "|(bitwise)",
                                        "^(bitwise)", "~^(bitwise)", "default"};
static const char* bitwiseopstr[6] = {"~", "&", "|", "^", "~^", "default"};
static const char* arithopclass[6] = {"+", "-", "*", "+(unary)", "-(minus)", "default"};
static const char* arithopstr[6] = {"+", "-", "*", "+", "-", "default" };
static const char* logicopclass[4] = {"&&", "||", "!", "default"};
static const char* reductopclass[7] = {"&(reduction)", "|(reduction)", "~&(reduction)",
                                       "~|(reduction)", "^(reduction)", "~^(reduction)", "default"};
static const char* reductopstr[7] = {"&", "|", "~&",  "~|", "^", "~^", "default" };
static const char* relateopclass[5] = {">=", ">", "<=", "<", "default"};
static const char* shiftopclass[3] = {">>", "<<", "default"};
static const char* equalityclass[5] = {"==", "!=", "===", "!==", "default"};

// ============================================================================
// struct paramPair
// ============================================================================
paramPair::paramPair() 
{
   content = 0;
}

paramPair::paramPair(const string& s, VlpBaseNode* const& bn) 
{
   param = s;
   content = bn;
}

paramPair::paramPair (const paramPair& x)
{
   param = x.param;
   content = x.content;
}   

paramPair::~paramPair() 
{
   if (content) delete content;
}

paramPair& 
paramPair::operator = (const paramPair& x) 
{
   param = x.param;
   content = x.content;
   return *this;
}

void
paramPair::whoAmI(int i, int l) const
{
   cout << param << endl;
   if (l > 0) {
      if (content) {
         cout.width(i);
         cout << " --> ";
         cout << "ctent " << " --> "; content->whoAmI(i+11, l--);
      }
   }
}

paramPair*
paramPair::duplicate() const 
{
   paramPair* tmp = new paramPair;
   tmp->param = param;
   tmp->content = content->duplicate();
   return tmp;
}

// ============================================================================
// VlpIntNode
// ============================================================================
VlpIntNode::VlpIntNode(int number) 
           :VlpBaseNode(NODECLASS_INT)
{
   _number = number;
}

VlpIntNode::~VlpIntNode()
{
}
      
void 
VlpIntNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "int   ";
   if (l > 0)
      cout << " : " << _number << endl;
   else
      cout << endl;
}

void
VlpIntNode::writeWhoAmI(ofstream& os) const
{
   os << TAB_INDENT << (this) << " [shape = plaintext, label = \"" << _number << "\"]" << endl;
}

MyString
VlpIntNode::str() const 
{
   return _number;
}

VlpBaseNode*
VlpIntNode::duplicate() const {
   VlpIntNode* dup = new VlpIntNode( _number );
   return dup;
}

vector< BddNode >
VlpIntNode::bdd( BddManager* bm, int& top, int& bottom,
                 const MyString sig ) const {
   vector< BddNode > ret;
   Bv4 tmpBv( _number );

   for ( unsigned int i = 0; i < tmpBv.bits(); ++i ) {
      if ( tmpBv[i] == _BV4_1 )
         ret.push_back( BddNode::_one );
      else
         ret.push_back( BddNode::_zero );
   }
   return ret;
}

string 
VlpIntNode::writeExp(set<string>& finSigs) const
{
   return str().str();
}

// ============================================================================
// VlpCaseItemNode
// ============================================================================
VlpCaseItemNode::VlpCaseItemNode(VlpBaseNode* exp, VlpBaseNode* inner, 
                                                   VlpBaseNode* next)
                :VlpBaseNode(NODECLASS_CASEITEM)
{
   _exp = exp;
   _inner = inner;
   _next = next;
}

VlpCaseItemNode::~VlpCaseItemNode()
{
   if (_exp)   delete _exp;
   if (_inner) delete _inner;
   if (_next)  delete _next;
}

void 
VlpCaseItemNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "CSitem";
   if (l > 0) {   
      cout << " : ";
      if (_exp) {
         cout << "EXP   " << " --> "; _exp->whoAmI(i+20, --l);
      }
      else 
         cout << "EXP   " << " --> " << "default(0)" << endl;
     
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
   else
      cout << endl;
}

void
VlpCaseItemNode::writeWhoAmI(ofstream& os) const
{
   if (_exp) os << TAB_INDENT << (this) << " [shape = trapezium, label = case " << str() << "]" << endl;
   else os << TAB_INDENT << (this) << " [shape = trapezium, label = default ]" << endl;
   if (_inner) {
      _inner->writeWhoAmI(os);
      os << TAB_INDENT << (this) << " -> " << _inner << " [label = INNER]" << endl;
   }
   if (_next) {
      _next->writeWhoAmI(os);
      os << TAB_INDENT << (this) << " -> " << _next << " [label = NEXT]" << endl;
   }
}

MyString
VlpCaseItemNode::str() const 
{
   return _exp->str();
}

vector< VlpSignalNode* >
VlpCaseItemNode::listSIGs() const {
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

VlpBaseNode*
VlpCaseItemNode::duplicate() const {
   VlpCaseItemNode* dup = new VlpCaseItemNode( _exp->duplicate(), NULL, NULL );
   if ( _inner != NULL )
      dup->setInner( _inner->duplicate() );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );

   return dup;
}

// ============================================================================
// VlpFunCall
// ============================================================================
VlpFunCall::VlpFunCall(const string& name, VlpBaseNode* argument)
           :VlpBaseNode(NODECLASS_FUNCALL)
{
   _nameId = BaseModule :: setName(name);
   _argument = argument;
}

VlpFunCall::~VlpFunCall()
{
   if (_argument) delete _argument;
}

void 
VlpFunCall::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << " FunCall" << endl;
}

void
VlpFunCall::writeWhoAmI(ofstream& os) const
{
   os << TAB_INDENT << (this) << " [shape = ellipse, label = FUNC " << str() << "]" << endl;
}

MyString
VlpFunCall::str() const 
{
   MyString tmp = BaseModule::getName( _nameId );
   tmp += "(";
   tmp += _argument->str();
   tmp += ")";
   return tmp;
}

VlpBaseNode*
VlpFunCall::duplicate() const {
   string name;
   name = BaseModule::getName( _nameId );

   VlpFunCall* dup = new VlpFunCall( name, _argument->duplicate() );

   return dup;
}

vector< VlpSignalNode* >
VlpFunCall::listSIGs() const {
   return _argument->listSIGs();
}

// ============================================================================
// VlpTaskCall
// ============================================================================
VlpTaskCall::VlpTaskCall(const string& str, VlpBaseNode* argument, VlpBaseNode* next)
            :VlpBaseNode(NODECLASS_TASK)
{
   _nameId = BaseModule :: setName(str);
   _argument = argument;
   _next = next;
}

VlpTaskCall::~VlpTaskCall()
{
   if (_argument) delete _argument;
   if (_next) delete _next;
}

void 
VlpTaskCall::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << " taskCall" << endl;
}

void
VlpTaskCall::writeWhoAmI(ofstream& os) const
{
   os << TAB_INDENT << (this) << " [shape = ellipse, label = TASK " << str() << "]" << endl;
}

MyString
VlpTaskCall::str() const 
{
   MyString tmp = BaseModule::getName( _nameId );
   tmp += "(";
   tmp += _argument->str();
   tmp += ")";
   return tmp;
}

VlpBaseNode*
VlpTaskCall::duplicate() const {
   string name;
   name = BaseModule::getName( _nameId );
   VlpTaskCall* dup = new VlpTaskCall( name,
                                       _argument->duplicate(), NULL );
   if ( _next != NULL )
      dup->setNext( _next->duplicate() );
   return dup;
}

// ============================================================================
// VlpSignalNode
// ============================================================================
VlpSignalNode::VlpSignalNode(int nameId, VlpBaseNode* msb, VlpBaseNode* lsb) 
              :VlpBaseNode(NODECLASS_SIGNAL)
{
   _nameId = nameId;
   _msb = msb;
   _lsb = lsb;
}

VlpSignalNode::~VlpSignalNode()
{
   if (_msb) delete _msb;
   if (_lsb) delete _lsb;
}

// This Function is For Debug Usage
bool 
VlpSignalNode::setNext(VlpBaseNode* const next) 
{ 
   Msg(MSG_ERR) << "SignalNode setNext(), error syntax !!!" << endl; exit(0);
	return false; 
}
      
int
VlpSignalNode::MSB() const {
   Bv4                tmpV;
   VlpPortNode*       port;
   const VlpBaseNode* index;

   if ( _msb == NULL ) {
      if ( curVlpModule->getPort( BaseModule::getName( _nameId ), port ) ) {
         index = port->getMsb();
      }
      else {
         index = NULL;
      }
   }
   else {
      index = _msb;
   }

   if ( index != NULL ) {
      switch ( index->getNodeClass() ) {
      case NODECLASS_INT:
         return static_cast<const VlpIntNode*>(index)->getNum();
      case NODECLASS_PARAMETER:
         tmpV = (static_cast<const VlpParamNode*>(index)->strContent()).str();
         return tmpV.value();
      case NODECLASS_SIGNAL:
         tmpV = (index->str()).str();
         return tmpV.value();
      default:
         return 0;
      }
   }
   return 0;
}

int
VlpSignalNode::LSB() const {
   Bv4                tmpV;
   VlpPortNode*       port;
   const VlpBaseNode* index;

   if ( _lsb == NULL ) {
      if ( curVlpModule->getPort( BaseModule::getName( _nameId ), port ) ) {
         index = port->getLsb();
      }
      else {
         index = NULL;
      }
   }
   else {
      index = _lsb;
   }

   if ( index != NULL ) {
      switch ( index->getNodeClass() ) {
      case NODECLASS_INT:
         return static_cast<const VlpIntNode*>(index)->getNum();
      case NODECLASS_PARAMETER:
         tmpV = (static_cast<const VlpParamNode*>(index)->strContent()).str();
         return tmpV.value();
      case NODECLASS_SIGNAL:
         tmpV = (index->str()).str();
         return tmpV.value();
      default:
         return 0;
      }
   }
   return 0;
}
   
bool 
VlpSignalNode::isMatchName(string& s) const
{
   if (BaseModule::getName(_nameId) == s)
      return true;
   else
      return false;
}

string
VlpSignalNode::getSigName() const
{
   return BaseModule::getName(_nameId);
}
   
bool 
VlpSignalNode::isBitBaseStr() const
{
   string str = BaseModule::getName(_nameId);
   int sizeBound = strcspn(str.c_str(), "'");
   if (sizeBound == (int)strlen(str.c_str()))
      return false;
   else
      return true;
}

void 
VlpSignalNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "signal";
   if (l > 0) {
      cout << " : " << BaseModule::getName(_nameId) << endl;
      if (_msb) {
         cout.width(i+9);
         cout << " : ";
         cout << "MSB   " << " --> "; _msb->whoAmI(i+20, --l);
      }
      if (_lsb) {
         cout.width(i+9);
         cout << " : ";
         cout << "LSB   " << " --> "; _lsb->whoAmI(i+20, --l);
      }
   }
   else
      cout << endl;
}

void
VlpSignalNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpSignalNode::str() const 
{
   MyString tmp = BaseModule::getName( _nameId );
   if ( _msb != NULL ) {
      tmp += "[";
      tmp += _msb->str();
      if ( ( _lsb != NULL ) && ( _msb->str() != _lsb->str() ) ) {
         tmp += ":";
         tmp += _lsb->str();
      }
      tmp += "]";
   }
   return tmp;
}

MyString
VlpSignalNode::strNoIndex() const {
   MyString tmp = BaseModule::getName( _nameId );
   return tmp;
}

set<MyString>
VlpSignalNode::setSIGs() const 
{
   set<MyString> tmp;
   if (isBv() == false)
      tmp.insert(strNoIndex());
   return tmp;
}

vector< VlpSignalNode* >
VlpSignalNode::listSIGs() const {
   vector< VlpSignalNode* > tmp;

   if ( isBv() == false )
      tmp.push_back( const_cast< VlpSignalNode* >(this) );

   return tmp;
}

VlpBaseNode*
VlpSignalNode::duplicate() const {
   VlpSignalNode* dup = new VlpSignalNode(_nameId);
   if ( _msb != NULL )
      dup->_msb = _msb->duplicate();

   if ( _lsb != NULL )
      dup->_lsb = _lsb->duplicate();

   return dup;
}

vector< BddNode >
VlpSignalNode::bdd( BddManager* bm, int& top, int& bottom,
                    const MyString sig ) const {
   vector< BddNode > ret;
   MyString          sigName;
   string            nodeName;
   BddNode           node;
   int               k, begin, end;
   bool              reverse;
   Bv4               tmpBv;

   if ( isBv() == false ) {
      begin = LSB();
      end   = MSB();
      if ( begin > end ) {
         reverse = true;
         end--;
      }
      else {
         reverse = false;
         end++;
      }

      k = begin;

      while ( k != end ) {
         if ( strNoIndex() == sig )
            sigName = "n_";
         else
            sigName = "";
         sigName += strNoIndex();
         sigName += '[';
         sigName += k;
         sigName += ']';
         nodeName = sigName.str();
         node = bm->getNode( nodeName );
         if ( node == BddNode::_error ) {
            if ( bottom + 1 > top ) {
               cerr << "The number of BDD supports exceeds "
                    << bm->supportsNum() << "." << endl;
               ret.clear();
               ret.push_back( BddNode::_error );
               return ret;
            }
            bm->setSupports( ++bottom, nodeName );
            node = bm->getNode( nodeName );
         }
         ret.push_back( node );
         if ( reverse )
            k--;
         else
            k++;
      }
   }
   else {
      sigName  = strNoIndex();
      nodeName = sigName.str();
      tmpBv = nodeName;
      for (unsigned i = 0; i < tmpBv.bits(); ++i) {
         if ( tmpBv[i] == _BV4_1 )
            ret.push_back( BddNode::_one );
         else
            ret.push_back( BddNode::_zero );
      }
   }

   return ret;
}

bool
VlpSignalNode::isBv() const {
   MyString tmp = BaseModule::getName( _nameId );
   if ( tmp == "default" )
      return true;

   int j = 0;
   for (int i = 0; i < tmp.length(); ++i) {
      if ( j == 0 ) {
         if ( tmp[i] == '\'' )
            j = 1;
         else if ( tmp[i] < '0' || tmp[i] > '9' )
            return false;
      }
      else if ( j == 1 ) {
         switch ( tmp[i] ) {
         case 'b':
         case 'B':
            j = 10;  break;
         case 'o':
         case 'O':
            j = 20;  break;
         case 'd':
         case 'D':
            j = 30;  break;
         case 'h':
         case 'H':
            j = 40;  break;
         default:
            return false;
         }
      }
      else {
         switch ( j ) {
         case 10:
            if ( tmp[i] != '0' && tmp[i] != '1' && tmp[i] != 'x'
              && tmp[i] != 'X' && tmp[i] != 'z' && tmp[i] != 'Z'
              && tmp[i] != '_' )
               return false;
            break;
         case 20:
            if ( !(tmp[i] >= '0' && tmp[i] <= '7' ) && tmp[i] != 'x'
              && tmp[i] != 'X'&& tmp[i] != 'z' && tmp[i] != 'Z'
              && tmp[i] != '_' )
               return false;
            break;
         case 30:
            if ( !(tmp[i] >= '0' && tmp[i] <= '9' ) && tmp[i] != 'x'
              && tmp[i] != 'X'&& tmp[i] != 'z' && tmp[i] != 'Z'
              && tmp[i] != '_' )
               return false;
            break;
         case 40:
            if ( !(tmp[i] >= '0' && tmp[i] <= '9' ) && tmp[i] != 'x'
              && tmp[i] != 'X'&& tmp[i] != 'z' && tmp[i] != 'Z'
              && tmp[i] != '_' && !( tmp[i] >= 'a' && tmp[i] <= 'f')
              && !(tmp[i] >= 'A' && tmp[i] <= 'F') )
               return false;
            break;
         }
      }
   }
   return true;
}
   
string 
VlpSignalNode::writeExp(set<string>& finSigs) const
{
   if (!isBitBaseStr())
      finSigs.insert(getSigName());
   return str().str();
}

// ============================================================================
// VlpParamNode
// ============================================================================
VlpParamNode::VlpParamNode(paramPair* pPair)
             :VlpBaseNode(NODECLASS_PARAMETER)
{

   //cout<< "para type: "<< (pPair->content)->getNodeClass() << "  (vlpOpNode.cpp)"<< endl;
   assert(  ((pPair->content)->getNodeClass() == NODECLASS_SIGNAL) 
         || ((pPair->content)->getNodeClass() == NODECLASS_INT  ));
   
   /*if ( (pPair->content)->getNodeClass() == NODECLASS_SIGNAL)   //can be parameter aa 3'b011, usage => aa[2]
      assert(   (static_cast<VlpSignalNode*>(pPair->content))->getMsb() == NULL 
             && (static_cast<VlpSignalNode*>(pPair->content))->getLsb() == NULL   );*/

   _pPair = pPair;
}

VlpParamNode::~VlpParamNode()
{
   if (_pPair) delete _pPair;
}
      
// This Function is For Debug Usage
bool 
VlpParamNode::setNext(VlpBaseNode* const next) 
{
   Msg(MSG_ERR) << "ParamNode setNext(), error syntax !!!" << endl; exit(0);
   return false; 
}
      
VlpBaseNode* 
VlpParamNode::getParamContent() const // consider parameter overload
{
   if (SYN != NULL && SYN->curPOLAry != NULL) // have parameter overload => need to check overload info.
      for (unsigned i = 0; i < SYN->curPOLAry->size(); ++i) 
         if (_pPair->param == SYN->curPOLAry->at(i)->param)  //overload parameter match
            return SYN->curPOLAry->at(i)->content;

   return _pPair->content;
}
      
void 
VlpParamNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "param ";
   if (l > 0) {
      if (_pPair) {
         cout << " : " << "EXP   " << " --> "; _pPair->whoAmI(i+20, --l);
      }
   }
   else
      cout << endl;
}

void
VlpParamNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpParamNode::str() const 
{
   MyString tmp = _pPair->param;
   tmp += "(";
   tmp += _pPair->content->str();
   tmp += ")";
   return tmp;
}

MyString
VlpParamNode::strContent() const {
   MyString tmp;

   tmp = _pPair->content->str();
   return tmp;
}

MyString
VlpParamNode::strName() const {
   MyString tmp;

   tmp = _pPair->param;

   return tmp;
}

VlpBaseNode*
VlpParamNode::duplicate() const {
   VlpParamNode *dup = new VlpParamNode( _pPair->duplicate() );
   return dup;
}

vector< BddNode >
VlpParamNode::bdd( BddManager* bm, int& top, int& bottom,
                   const MyString sig ) const {
   vector< BddNode > ret;
   Bv4 tmpBv( strContent().str() );

   for ( unsigned int i = 0; i < tmpBv.bits(); ++i ) {
      if ( tmpBv[i] == _BV4_1 )
         ret.push_back( BddNode::_one );
      else
         ret.push_back( BddNode::_zero );
   }

   return ret;
}

string 
VlpParamNode::writeExp(set<string>& finSigs) const
{
   return _pPair->content->writeExp(finSigs);
}

// ============================================================================
// VlpOperatorNode
// ============================================================================
VlpOperatorNode::VlpOperatorNode(OperatorClass operatorClass) 
                :VlpBaseNode(NODECLASS_OPERATOR)
{
   _operatorClass = operatorClass;
}

VlpOperatorNode::~VlpOperatorNode()
{
}

void 
VlpOperatorNode::whoAmI(int, int) const 
{ 
   cerr << "VlpOperatorNode::whoAmI() ---> ERROR: I am operator node. " << endl;
}

void
VlpOperatorNode::writeWhoAmI(ofstream& os) const
{
   cerr << "VlpOperatorNode::writeWhoAmI() ---> ERROR: I am operator node. " << endl;
}

MyString
VlpOperatorNode::str() const 
{
   return "Operator";
}

set<MyString>
VlpOperatorNode::setSIGs() const 
{
   set<MyString> tmp;
   return tmp;
}

vector< VlpSignalNode* >
VlpOperatorNode::listSIGs() const {
   vector< VlpSignalNode* > tmp;
   return tmp;
}

VlpBaseNode*
VlpOperatorNode::duplicate() const {
   VlpOperatorNode* dup = new VlpOperatorNode( _operatorClass );
   return dup;
}

vector< BddNode >
VlpOperatorNode::bdd( BddManager* bm, int& top, int& bottom,
                      const MyString sig ) const {
   vector< BddNode > ret;
   ret.push_back( BddNode::_error );
   return ret;
}

string
VlpOperatorNode::writeExp(set<string>& finSigs) const
{
   if (_operatorClass == OPCLASS_REPLICATION) {
      cerr << "Error: No Support operator class \"replication\"" << endl;
      assert (0);
   }
   else if (_operatorClass == OPCLASS_CONDITIONAL) {
      cerr << "Error: No Support operator class \"conditional\"" << endl;
      assert (0);
   }
   else {
      set<MyString> tmp = setSIGs();
      for (set<MyString>::iterator it = tmp.begin(); it != tmp.end(); it++)
         finSigs.insert((*it).str());
      return str().str();
   }
}

// ============================================================================
// VlpBitWiseOpNode
// ============================================================================
VlpBitWiseOpNode::VlpBitWiseOpNode():VlpOperatorNode(OPCLASS_BITWISE) 
{ 
   _leftOperand  = NULL;
   _rightOperand = NULL;
}

VlpBitWiseOpNode::VlpBitWiseOpNode(BitWiseOpClass bitWiseOp, 
                  VlpBaseNode* leftOperand, VlpBaseNode* rightOperand) 
                 :VlpOperatorNode(OPCLASS_BITWISE)
{
   _bitWiseOp    = bitWiseOp;
   _leftOperand  = leftOperand;
   _rightOperand = rightOperand;
}

VlpBitWiseOpNode::~VlpBitWiseOpNode()
{
   if (_leftOperand)  delete _leftOperand;
   if (_rightOperand) delete _rightOperand;
}

void 
VlpBitWiseOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "BWOP  " << " : " << bitwiseopclass[_bitWiseOp] << endl;
   if (l > 0) {
      if (_leftOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "LEFT  " << " --> "; _leftOperand->whoAmI(i+20, --l);
      }
      if (_rightOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "RIGHT " << " --> "; _rightOperand->whoAmI(i+20, --l);
      }
   }
}

void
VlpBitWiseOpNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpBitWiseOpNode::str() const 
{
   MyString tmp = _leftOperand->str();
   tmp += " ";
   tmp += bitwiseopstr[ _bitWiseOp ];
   tmp += " ";
   tmp += _rightOperand->str();

   return tmp;
}

set<MyString>
VlpBitWiseOpNode::setSIGs() const 
{
   set<MyString> tmp  = _leftOperand->setSIGs();
   set<MyString> tmpR = _rightOperand->setSIGs();
   for (set< MyString >::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   return tmp;
}

vector< VlpSignalNode* >
VlpBitWiseOpNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;
   tmp  = _leftOperand->listSIGs();
   tmpR = _rightOperand->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   return tmp;
}

VlpBaseNode*
VlpBitWiseOpNode::duplicate() const {
   VlpBitWiseOpNode* dup = new VlpBitWiseOpNode( _bitWiseOp,
                                                 _leftOperand->duplicate(),
                                                 _rightOperand->duplicate() );
   return dup;
}

vector< BddNode >
VlpBitWiseOpNode::bdd( BddManager* bm, int& top, int& bottom,
                       const MyString sig ) const {
   vector< BddNode > ret,
                     left,
                     right;
   unsigned int      i, max;
   BddNode           leftNode,
                     rightNode,
                     resNode;

   left  = _leftOperand->bdd( bm, top, bottom, sig );
   right = _rightOperand->bdd( bm, top, bottom, sig );

   if ( left[0] == BddNode::_error || right[0] == BddNode::_error ) {
      ret.push_back( BddNode::_error );
      return ret;
   }

   max = left.size();
   if ( right.size() > max )
      max = right.size();

   for ( i = 0; i < max; ++i ) {
      if ( i < left.size() )
         leftNode = left[i];
      else
         leftNode = BddNode::_zero;

      if ( i < right.size() )
         rightNode = right[i];
      else
         rightNode = BddNode::_zero;

      switch ( _bitWiseOp ) {
      case BITWISEOP_AND:
         resNode = leftNode & rightNode;       break;
      case BITWISEOP_OR:
         resNode = leftNode | rightNode;       break;
      case BITWISEOP_XOR:
         resNode = leftNode ^ rightNode;       break;
      case BITWISEOP_XNOR:
         resNode = leftNode.xnor( rightNode ); break;
      default:
         resNode = BddNode::_error;
      }

      ret.push_back( resNode );
   }

   return ret;
}
// ============================================================================
// VlpBitNotOpNode
// ============================================================================
VlpBitNotOpNode::VlpBitNotOpNode(VlpBaseNode* operand)
                :VlpBitWiseOpNode()
{
   _bitWiseOp = BITWISEOP_NOT;
   _operand = operand;
}

VlpBitNotOpNode::~VlpBitNotOpNode()
{
   if (_operand) delete _operand;
}

void 
VlpBitNotOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "BWOP  " << " : " << "(~)   "<< endl;
   if (l > 0) {
      if (_operand) {
         cout.width(i+9);
         cout << " : ";
         cout << "OPER  " << " --> "; _operand->whoAmI(i+20, --l);
      }
   }
}

void
VlpBitNotOpNode::writeWhoAmI(ofstream& os) const
{
}
   
MyString
VlpBitNotOpNode::str() const 
{
   MyString tmp = "~";
   tmp += _operand->str();
   return tmp;
}

set<MyString>
VlpBitNotOpNode::setSIGs() const 
{
   return _operand->setSIGs();
}

VlpBaseNode*
VlpBitNotOpNode::duplicate() const {
   VlpBitNotOpNode* dup = new VlpBitNotOpNode( _operand->duplicate() );
   return dup;
}

vector< VlpSignalNode* >
VlpBitNotOpNode::listSIGs() const {
   return _operand->listSIGs();
}

vector< BddNode >
VlpBitNotOpNode::bdd( BddManager* bm, int& top, int& bottom,
                      const MyString sig ) const {
   vector< BddNode > ret,
                     op;

   op = _operand->bdd( bm, top, bottom, sig );
   for ( unsigned int i = 0; i < op.size(); ++i ) {
      if ( op[i] == BddNode::_error ) {
         ret.clear();
         ret.push_back( BddNode::_error );
         return ret;
      }
      else
         ret.push_back( ~op[i] );
   }

   return ret;
}

// ============================================================================
// VlpArithOpNode
// ============================================================================
VlpArithOpNode::VlpArithOpNode() 
               :VlpOperatorNode(OPCLASS_ARITHMETIC)
{
   _leftOperand  = NULL;
   _rightOperand = NULL; 
}

VlpArithOpNode::VlpArithOpNode(ArithOpClass arithOp, 
                               VlpBaseNode* leftOperand, 
                               VlpBaseNode* rightOperand ) 
               :VlpOperatorNode(OPCLASS_ARITHMETIC)
{
   _arithOp      = arithOp;
   _leftOperand  = leftOperand;
   _rightOperand = rightOperand;
}

VlpArithOpNode::~VlpArithOpNode()
{
   if (_leftOperand)  delete _leftOperand;
   if (_rightOperand) delete _rightOperand;
}
      
void 
VlpArithOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "AROP  " << " : " << arithopclass[_arithOp] << endl;
   if (l > 0) {
      if (_leftOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "LEFT  " << " --> "; _leftOperand->whoAmI(i+20, --l);
      }
      if (_rightOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "RIGHT " << " --> "; _rightOperand->whoAmI(i+20, --l);
      }
   }
}

void
VlpArithOpNode::writeWhoAmI(ofstream& os) const
{
   cerr << "VlpArithOpNode::writeWhoAmI() ---> ERROR: I am arithmetic node. " << endl;
}

MyString
VlpArithOpNode::str() const 
{
   MyString tmp = _leftOperand->str();
   tmp += " ";
   tmp += arithopstr[_arithOp];
   tmp += " ";
   tmp += _rightOperand->str();
   return tmp;
}

set<MyString>
VlpArithOpNode::setSIGs() const 
{
   set<MyString> tmp  = _leftOperand->setSIGs();
   set<MyString> tmpR = _rightOperand->setSIGs();

   for (set< MyString >::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   return tmp;
}

vector< VlpSignalNode* >
VlpArithOpNode::listSIGs() const 
{
   vector< VlpSignalNode* > tmp  = _leftOperand->listSIGs();
   vector< VlpSignalNode* > tmpR = _rightOperand->listSIGs();

   for (unsigned i = 0; i < tmpR.size(); ++i)
      tmp.push_back( tmpR[i] );

   return tmp;
}

VlpBaseNode*
VlpArithOpNode::duplicate() const 
{
   VlpArithOpNode* dup = new VlpArithOpNode( _arithOp,
                                             _leftOperand->duplicate(),
                                             _rightOperand->duplicate() );
   return dup;
}

// ============================================================================
// VlpSignNode
// ============================================================================
VlpSignNode::VlpSignNode(ArithOpClass arithOp, VlpBaseNode* operand)
            :VlpArithOpNode()
{
   _arithOp = arithOp;
   _operand = operand;
}

VlpSignNode::~VlpSignNode()
{
   if (_operand) delete _operand;
}

void 
VlpSignNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "AROP  " << " : " << arithopclass[_arithOp] << endl;
   if (l > 0) {
      if (_operand) {
         cout.width(i+9);
         cout << " : ";
         cout << "OPER  " << " --> "; _operand->whoAmI(i+20, --l);
      }
   }
}

void
VlpSignNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpSignNode::str() const 
{
   MyString tmp = arithopstr[ _arithOp ];
   tmp += _operand->str();
   return tmp;
}

set<MyString>
VlpSignNode::setSIGs() const 
{
   return _operand->setSIGs();
}

vector< VlpSignalNode* >
VlpSignNode::listSIGs() const {
   return _operand->listSIGs();
}

VlpBaseNode*
VlpSignNode::duplicate() const {
   VlpSignNode* dup = new VlpSignNode( _arithOp, _operand->duplicate() );
   return dup;
}

// ============================================================================
// VlpLogicOpNode
// ============================================================================
VlpLogicOpNode::VlpLogicOpNode():VlpOperatorNode(OPCLASS_LOGIC)
{
   _leftOperand  = NULL;
   _rightOperand = NULL; 
} //For NOT usage

VlpLogicOpNode::VlpLogicOpNode(LogicOpClass logicOp, 
                               VlpBaseNode* leftOperand, 
                               VlpBaseNode* rightOperand) 
               :VlpOperatorNode(OPCLASS_LOGIC) 
{
   _logicOp      = logicOp;
   _leftOperand  = leftOperand;
   _rightOperand = rightOperand;
}

VlpLogicOpNode::~VlpLogicOpNode()
{
   if (_leftOperand) delete _leftOperand;
   if (_rightOperand) delete _rightOperand;
}

void 
VlpLogicOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "LGOP  " << " : " << logicopclass[_logicOp] << endl;
   if (l > 0) {
      if (_leftOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "LEFT  " << " --> "; _leftOperand->whoAmI(i+20, --l);
      }
      if (_rightOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "RIGHT " << " --> "; _rightOperand->whoAmI(i+20, --l);
      }
   }
}

void
VlpLogicOpNode::writeWhoAmI(ofstream& os) const
{
   cerr << "VlpLogicOpNode::writeWhoAmI() ---> ERROR: I am operator node. " << endl;
}

MyString
VlpLogicOpNode::str() const 
{
   MyString tmp = _leftOperand->str();
   tmp += " ";
   tmp += logicopclass[ _logicOp ];
   tmp += " ";
   tmp += _rightOperand->str();
   return tmp;
}

set<MyString>
VlpLogicOpNode::setSIGs() const 
{
   set<MyString> tmp  = _leftOperand->setSIGs();
   set<MyString> tmpR = _rightOperand->setSIGs();
   for(set<MyString>::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   return tmp;
}

vector< VlpSignalNode* >
VlpLogicOpNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _leftOperand->listSIGs();
   tmpR = _rightOperand->listSIGs();

   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   return tmp;
}

VlpBaseNode*
VlpLogicOpNode::duplicate() const {
   VlpLogicOpNode* dup = new VlpLogicOpNode( _logicOp,
                                             _leftOperand->duplicate(),
                                             _rightOperand->duplicate());
   return dup;
}

vector< BddNode >
VlpLogicOpNode::bdd( BddManager* bm, int& top, int& bottom,
                     const MyString sig ) const {
   vector< BddNode > ret,
                     left,
                     right;
   unsigned int      i, max;
   BddNode           leftNode,
                     rightNode,
                     resNode;

   left  = _leftOperand->bdd( bm, top, bottom, sig );
   right = _rightOperand->bdd( bm, top, bottom, sig );

   if ( left[0] == BddNode::_error || right[0] == BddNode::_error ) {
      ret.push_back( BddNode::_error );
      return ret;
   }

   max = left.size();
   if ( right.size() > max )
      max = right.size();

   for ( i = 0; i < max; ++i ) {
      if ( i < left.size() )
         leftNode = left[i];
      else
         leftNode = BddNode::_zero;

      if ( i < right.size() )
         rightNode = right[i];
      else
         rightNode = BddNode::_zero;

      switch ( _logicOp ) {
      case LOGICOP_AND:
         resNode = leftNode & rightNode;       break;
      case LOGICOP_OR:
         resNode = leftNode | rightNode;       break;
      default:
         resNode = BddNode::_error;
      }

      ret.push_back( resNode );
   }

   return ret;
}

// ============================================================================
// VlpLogicNotOpNode
// ============================================================================
VlpLogicNotOpNode::VlpLogicNotOpNode(VlpBaseNode* operand) 
                  :VlpLogicOpNode()
{
   _logicOp = LOGICOP_NOT;
   _operand = operand;
}

VlpLogicNotOpNode::~VlpLogicNotOpNode()
{
   if (_operand) delete _operand;
}
      
void 
VlpLogicNotOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "LGOP  " << " : " << logicopclass[_logicOp] << endl;
   if (l > 0) {
      if (_operand) {
         cout.width(i+9);
         cout << " : ";
         cout << "OPER  " << " --> "; _operand->whoAmI(i+20, --l);
      }
   }
}

void
VlpLogicNotOpNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpLogicNotOpNode::str() const 
{
   MyString tmp = logicopclass[ _logicOp ];
   tmp += _operand->str();
   return tmp;
}

set<MyString>
VlpLogicNotOpNode::setSIGs() const 
{
   return _operand->setSIGs();
}

vector< VlpSignalNode* >
VlpLogicNotOpNode::listSIGs() const {
   return _operand->listSIGs();
}

VlpBaseNode*
VlpLogicNotOpNode::duplicate() const {
   VlpLogicNotOpNode* dup = new VlpLogicNotOpNode( _operand->duplicate() );
   return dup;
}

vector< BddNode >
VlpLogicNotOpNode::bdd( BddManager* bm, int& top, int& bottom,
                        const MyString sig ) const {
   vector< BddNode > ret,
                     op;

   op = _operand->bdd( bm, top, bottom, sig );

   for ( unsigned int i = 0; i < op.size(); ++i ) {
      if ( op[i] == BddNode::_error ) {
         ret.clear();
         ret.push_back( BddNode::_error );
         return ret;
      }
      else
         ret.push_back( ~op[i] );
   }

   return ret;
}

// ============================================================================
// VlpReductOpNode
// ============================================================================
VlpReductOpNode::VlpReductOpNode(ReductOpClass reductOp, 
                                   VlpBaseNode* operand )
                :VlpOperatorNode(OPCLASS_REDUCTION)
{
   _reductOp = reductOp;
   _operand  = operand;
}

VlpReductOpNode::~VlpReductOpNode()
{
   if (_operand) delete _operand;
}
      
void 
VlpReductOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "RDOP  "  << " : " << reductopclass[_reductOp] << endl;
   if (l > 0) {
      if (_operand) {
         cout.width(i+9);
         cout << " : ";
         cout << "OPER  " << " --> "; _operand->whoAmI(i+20, --l);
      }
   }
}

void
VlpReductOpNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpReductOpNode::str() const 
{
   MyString tmp = reductopstr[ _reductOp ];
   tmp += "(";
   tmp += _operand->str();
   tmp += ")";
   return tmp;
}

set<MyString>
VlpReductOpNode::setSIGs() const 
{
   return _operand->setSIGs();
}

vector< VlpSignalNode * >
VlpReductOpNode::listSIGs() const {
   return _operand->listSIGs();
}

VlpBaseNode*
VlpReductOpNode::duplicate() const {
   VlpReductOpNode* dup = new VlpReductOpNode( _reductOp,
                                               _operand->duplicate() );
   return dup;
}

// ============================================================================
// VlpRelateOpNode
// ============================================================================
VlpRelateOpNode::VlpRelateOpNode(RelateOpClass relateOp, 
                                 VlpBaseNode* leftOperand, 
                                 VlpBaseNode* rightOperand )
                :VlpOperatorNode(OPCLASS_RELATION)
{
   _relateOp     = relateOp;
   _leftOperand  = leftOperand;
   _rightOperand = rightOperand;
}

VlpRelateOpNode::~VlpRelateOpNode()
{
   if (_leftOperand)  delete _leftOperand;
   if (_rightOperand) delete _rightOperand;
}
      
void 
VlpRelateOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "RLOP  " << " : " << relateopclass[_relateOp] << endl;
   if (l > 0) {
      if (_leftOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "LEFT  " << " --> "; _leftOperand->whoAmI(i+20, --l);
      }
      if (_rightOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "RIGHT " << " --> "; _rightOperand->whoAmI(i+20, --l);
      }
   }
}

void
VlpRelateOpNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpRelateOpNode::str() const 
{
   MyString tmp = _leftOperand->str();
   tmp += " ";
   tmp += relateopclass[ _relateOp ];
   tmp += " ";
   tmp += _rightOperand->str();
   return tmp;
}

set<MyString>
VlpRelateOpNode::setSIGs() const 
{
   set<MyString> tmp  = _leftOperand->setSIGs();
   set<MyString> tmpR = _rightOperand->setSIGs();
   for(set< MyString >::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   return tmp;
}

vector< VlpSignalNode* >
VlpRelateOpNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _leftOperand->listSIGs();
   tmpR = _rightOperand->listSIGs();

   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   return tmp;
}

VlpBaseNode*
VlpRelateOpNode::duplicate() const {
   VlpRelateOpNode* dup = new VlpRelateOpNode( _relateOp,
                                               _leftOperand->duplicate(),
                                               _rightOperand->duplicate() );
   return dup;
}

vector< BddNode >
VlpRelateOpNode::bdd( BddManager* bm, int& top, int& bottom,
                      const MyString sig ) const {
   vector< BddNode > ret;

   ret.push_back( BddNode::_error );

   return ret;
}

// ============================================================================
// VlpShiftOpNode
// ============================================================================
VlpShiftOpNode::VlpShiftOpNode(ShiftOpClass shiftOp, 
                               VlpBaseNode* leftOperand, 
                               VlpBaseNode* rightOperand)
               :VlpOperatorNode(OPCLASS_SHIFT)
{
   _shiftOp      = shiftOp;
   _leftOperand  = leftOperand;
   _rightOperand = rightOperand;
}

VlpShiftOpNode::~VlpShiftOpNode()
{
   if (_leftOperand)  delete _leftOperand;
   if (_rightOperand) delete _rightOperand;
}

void 
VlpShiftOpNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "SFOP  " << " : " << shiftopclass[_shiftOp] << endl;
   if (l > 0) {
      if (_leftOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "LEFT  " << " --> "; _leftOperand->whoAmI(i+20, --l); 
      }
      if (_rightOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "RIGHT " << " --> "; _rightOperand->whoAmI(i+20, --l);
      }
   }
}

void
VlpShiftOpNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpShiftOpNode::str() const 
{
   MyString tmp = _leftOperand->str();
   tmp += " ";
   tmp += shiftopclass[ _shiftOp ];
   tmp += " ";
   tmp += _rightOperand->str();
   return tmp;
}

set<MyString>
VlpShiftOpNode::setSIGs() const 
{
   set<MyString> tmp  = _leftOperand->setSIGs();
   set<MyString> tmpR = _rightOperand->setSIGs();
   for (set<MyString>::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   return tmp;
}

vector< VlpSignalNode* >
VlpShiftOpNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _leftOperand->listSIGs();
   tmpR = _rightOperand->listSIGs();

   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   return tmp;
}

VlpBaseNode*
VlpShiftOpNode::duplicate() const {
   VlpShiftOpNode* dup = new VlpShiftOpNode( _shiftOp,
                                             _leftOperand->duplicate(),
                                             _rightOperand->duplicate() );
   return dup;
}

// ============================================================================
// VlpEqualityNode
// ============================================================================
VlpEqualityNode::VlpEqualityNode(EqualityClass equality, 
                                 VlpBaseNode* leftOperand, 
                                 VlpBaseNode* rightOperand)
                :VlpOperatorNode(OPCLASS_EQUALITY)
{
   _equality     = equality;
   _leftOperand  = leftOperand;
   _rightOperand = rightOperand;
}

VlpEqualityNode::~VlpEqualityNode()
{
   if (_leftOperand)  delete _leftOperand;
   if (_rightOperand) delete _rightOperand;
}

void 
VlpEqualityNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "EQOP  "  << " : " << equalityclass[_equality] << endl;
   if (l > 0) {
      if (_leftOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "LEFT  " << " --> "; _leftOperand->whoAmI(i+20, --l);
      }
      if (_rightOperand) {
         cout.width(i+9);
         cout << " : ";
         cout << "RIGHT " << " --> "; _rightOperand->whoAmI(i+20, --l);
      }
   }
}

void
VlpEqualityNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpEqualityNode::str() const 
{
   MyString tmp  = _leftOperand->str();
   tmp += " ";
   tmp += equalityclass[ _equality ];
   tmp += " ";
   tmp += _rightOperand->str();
   return tmp;
}

set<MyString>
VlpEqualityNode::setSIGs() const 
{
   set<MyString> tmp  = _leftOperand->setSIGs();
   set<MyString> tmpR = _rightOperand->setSIGs();
   for (set<MyString>::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   return tmp;
}

vector< VlpSignalNode* >
VlpEqualityNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _leftOperand->listSIGs();
   tmpR = _rightOperand->listSIGs();

   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   return tmp;
}

VlpBaseNode*
VlpEqualityNode::duplicate() const {
   VlpEqualityNode* dup = new VlpEqualityNode( _equality,
                                               _leftOperand->duplicate(),
                                               _rightOperand->duplicate() );
   return dup;
}

vector< BddNode >
VlpEqualityNode::bdd( BddManager* bm, int& top, int& bottom,
                      const MyString sig ) const {
/*
   vector< BddNode > ret;
   ret.push_back( BddNode::_error);

   return ret;
*/
   vector< BddNode > ret;
   MyString          sigName;
   string            nodeName;
   BddNode           node;

   sigName  = str();
   nodeName = sigName.str();
   node     = bm->getNode( nodeName );
   if ( node == BddNode::_error ) {
      if ( bottom + 1 > top ) {
         cerr << "The number of BDD supports exceeds "
         << bm->supportsNum() << "." << endl;
         ret.clear();
         ret.push_back( BddNode::_error );
         return ret;
      }
      bm->setSupports( ++bottom, nodeName );
      node = bm->getNode( nodeName );
   }
   ret.push_back( node );

   return ret;
}

// ============================================================================
// VlpConcatenateNode
// ============================================================================
VlpConcatenateNode::VlpConcatenateNode(VlpBaseNode* content)
                   :VlpOperatorNode(OPCLASS_CONCATENATE)
{
   _content = content;
}

VlpConcatenateNode::~VlpConcatenateNode()
{
   if (_content) delete _content;
}

void 
VlpConcatenateNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "CONCAT";
   if (l > 0) {
      if (_content) {
         cout << " --> "; _content->whoAmI(i+11, --l);
      }
   }
   else
      cout << endl;
}

void
VlpConcatenateNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpConcatenateNode::str() const 
{
   MyString tmp = "{";
   tmp += _content->str();
   tmp += "}";
   return tmp;
}

vector< BddNode >
VlpConcatenateNode::bdd( BddManager* bm, int& top, int& bottom,
                         const MyString sig ) const {
   vector< BddNode > ret;

   ret.push_back( BddNode::_error );

   return ret;
//   return _content->bdd( bm, top, bottom, sig );
}

set<MyString>
VlpConcatenateNode::setSIGs() const 
{
   return _content->setSIGs();
}

vector< VlpSignalNode* >
VlpConcatenateNode::listSIGs() const {
   return _content->listSIGs();
}

VlpBaseNode*
VlpConcatenateNode::duplicate() const {
   VlpConcatenateNode* dup = new VlpConcatenateNode( _content->duplicate() );
   return dup;
}

Bv4 
VlpConcatenateNode::getConcatenateBv() const
{
   Bv4 ccBv;
   Bv4 tmpBv;
   assert (_content->getNodeClass() == NODECLASS_CONNECT);
   const VlpConnectNode* cNode = static_cast<const VlpConnectNode*>(_content);
   const VlpSignalNode*  sNode;
   string str;
   while (cNode) {
      sNode = static_cast<const VlpSignalNode*>(cNode->getExp());
      str = BaseModule::getName(sNode->getNameId());
      tmpBv = str;
      ccBv.append(tmpBv);
      cNode = static_cast<const VlpConnectNode*>(cNode->getNext());
   }
   return ccBv;
}

bool 
VlpConcatenateNode::isBvConcatenate() const
{ // only concatenate or replication node usage
   assert (_content->getNodeClass() == NODECLASS_CONNECT);
   const VlpConnectNode* cNode = static_cast<const VlpConnectNode*>(_content);
   const VlpSignalNode*  sNode;
   string name;
   int bvCount = 0;
   int totalCount = 0;
   while (cNode) {
      ++totalCount;
      if (cNode->getExp()->getNodeClass() == NODECLASS_SIGNAL) {
         sNode = static_cast<const VlpSignalNode*>(cNode->getExp());
         name = BaseModule::getName(sNode->getNameId());
         if (name.find("'", 0) != string::npos) 
            ++bvCount;
      }
      else if (cNode->getExp()->getNodeClass() == NODECLASS_PARAMETER) {
         const VlpParamNode* pNode = static_cast<const VlpParamNode*>(cNode->getExp());
         sNode = static_cast<const VlpSignalNode*>(pNode->getParamContent());
         name = BaseModule::getName(sNode->getNameId());
         if (name.find("'", 0) != string::npos) 
            ++bvCount;
      }
      else {
         // no support {3'b011 ^ 3'b110, ... } operator @ concatenate
         // just support replication node or concatenate node within it.
         const VlpBaseNode* oNode = cNode->getExp();
         assert (oNode->getNodeClass() == NODECLASS_OPERATOR);
         if (static_cast<const VlpOperatorNode*>(oNode)->isBvConcatenate())
            ++bvCount;
      }
      cNode = static_cast<const VlpConnectNode*>(cNode->getNext());
   }
   if (bvCount == totalCount)
      return true;
   else if (bvCount == 0)
      return false;
   else {
      cerr << "Warning : Concatenate bv str and signal !!" << endl;
      return false;
   }
}

// ============================================================================
// VlpReplicationNode
// ============================================================================
VlpReplicationNode::VlpReplicationNode(VlpBaseNode* times, VlpBaseNode* content)
                   : VlpOperatorNode(OPCLASS_REPLICATION)
{
   _times   = times;
   _content = content;
}

VlpReplicationNode::~VlpReplicationNode()
{
   if (_times) delete _times;
   if (_content) delete _content;
}

void 
VlpReplicationNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "REPLIC";
   if (l > 0) {
      if (_times) {
         cout << " : " << "TIMES " << " --> "; _times->whoAmI(i+20, --l);
      }
      if (_content) {
         cout.width(i+9);
         cout << " : " << "CONT  " << " --> "; _content->whoAmI(i+20, --l);
      }
   }
   else
      cout << endl;
}

void
VlpReplicationNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpReplicationNode::str() const 
{
   MyString tmp = "{";
   tmp += _times->str();
   tmp += "{";
   tmp += _content->str();
   tmp += "}}";
   return tmp;
}

vector< BddNode >
VlpReplicationNode::bdd( BddManager* bm, int& top, int& bottom,
                         const MyString sig ) const {
   vector< BddNode > ret,
                     op;
   Bv4               tmpBv;
   op = _content->bdd( bm, top, bottom, sig );
   tmpBv = (_times->str()).str();

   for ( unsigned int i = 0; i < tmpBv.value(); ++i ) {
      for ( unsigned int j = 0; j < op.size(); ++j )
         ret.push_back( op[j] );
   }
   return ret;
}

set<MyString>
VlpReplicationNode::setSIGs() const 
{
   return _content->setSIGs();
}

vector< VlpSignalNode* >
VlpReplicationNode::listSIGs() const {
   return _content->listSIGs();
}

VlpBaseNode*
VlpReplicationNode::duplicate() const {
   VlpReplicationNode* dup = new VlpReplicationNode( _times->duplicate(),
                                                     _content->duplicate() );
   return dup;
}

bool 
VlpReplicationNode::isBvConcatenate() const
{ // only concatenate or replication node usage
   assert (_content->getNodeClass() == NODECLASS_CONNECT);
   const VlpConnectNode* cNode = static_cast<const VlpConnectNode*>(_content);
   const VlpSignalNode*  sNode;
   string name;
   int bvCount = 0;
   int totalCount = 0;
   while (cNode) {
      ++totalCount;
      if (cNode->getExp()->getNodeClass() == NODECLASS_SIGNAL) {
         sNode = static_cast<const VlpSignalNode*>(cNode->getExp());
         name = BaseModule::getName(sNode->getNameId());
         if (name.find("'", 0) != string::npos) 
            ++bvCount;
      }
      else if (cNode->getExp()->getNodeClass() == NODECLASS_PARAMETER) {
         const VlpParamNode* pNode = static_cast<const VlpParamNode*>(cNode->getExp());
         sNode = static_cast<const VlpSignalNode*>(pNode->getParamContent());
         name = BaseModule::getName(sNode->getNameId());
         if (name.find("'", 0) != string::npos) 
            ++bvCount;
      }
      else {
         // no support {3'b011 ^ 3'b110, ... } operator @ concatenate
         // just support replication node or concatenate node within it.
         const VlpBaseNode* oNode = cNode->getExp();
         assert (oNode->getNodeClass() == NODECLASS_OPERATOR);
         if (static_cast<const VlpOperatorNode*>(oNode)->isBvConcatenate())
            ++bvCount;
      }
      cNode = static_cast<const VlpConnectNode*>(cNode->getNext());
   }
   if (bvCount == totalCount)
      return true;
   else if (bvCount == 0)
      return false;
   else {
      cerr << "Warning : Concatenate bv str and signal!! No handle!!" << endl;
      return false;
   }
}

// ============================================================================
// VlpconditionalNode
// ============================================================================
VlpConditionalNode :: VlpConditionalNode(VlpBaseNode* conditional,
                                         VlpBaseNode* trueChild, 
                                         VlpBaseNode* falseChild)
                    : VlpOperatorNode(OPCLASS_CONDITIONAL)
{
   _conditional = conditional;
   _trueChild   = trueChild;
   _falseChild  = falseChild;
}

VlpConditionalNode :: ~VlpConditionalNode()
{
   if (_conditional) delete _conditional;
   if (_trueChild)   delete _trueChild;
   if (_falseChild)  delete _falseChild;
}

void 
VlpConditionalNode::whoAmI(int i, int l) const
{
   cout << " --> ";
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "CONDOP";
   if (l > 0) {
      if (_conditional) {
         cout << " : " << "COND  " << " --> "; _conditional->whoAmI(i+20, --l);
      }
      if (_trueChild) {
         cout.width(i+9);
         cout << " : ";
         cout << "TRUE  " << " --> "; _trueChild->whoAmI(i+20, --l);
      }
      if (_falseChild) {
         cout.width(i+9);
         cout << " : ";
         cout << "FALSE " << " --> "; _falseChild->whoAmI(i+20,--l);
      }
   }
   else
      cout << endl;
}

void
VlpConditionalNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpConditionalNode::str() const 
{
   MyString tmp = _conditional->str();
   tmp += " ? ";
   tmp += _trueChild->str();
   tmp += " : ";
   tmp += _falseChild->str();
   return tmp;
}

set<MyString>
VlpConditionalNode::setSIGs() const 
{
   set<MyString> tmp  = _conditional->setSIGs();
   set<MyString> tmpR = _trueChild->setSIGs();
   for (set<MyString>::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   
   tmpR = _falseChild->setSIGs();
   for (set<MyString>::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
      tmp.insert(*pos);
   return tmp;
}

vector< VlpSignalNode * >
VlpConditionalNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;

   tmp  = _conditional->listSIGs();
   tmpR = _trueChild->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   tmpR = _falseChild->listSIGs();
   for ( i = 0; i < tmpR.size(); ++i )
      tmp.push_back( tmpR[i] );

   return tmp;
}

VlpBaseNode*
VlpConditionalNode::duplicate() const {
   VlpConditionalNode* dup = new VlpConditionalNode( _conditional->duplicate(),
                                                     _trueChild->duplicate(),
                                                     _falseChild->duplicate());
   return dup;
}

// ============================================================================
// VlpConnectNode
// ============================================================================
VlpConnectNode::VlpConnectNode(VlpBaseNode* exp)
               : VlpBaseNode(NODECLASS_CONNECT)
{
   _exp  = exp;
   _next = NULL;
}

VlpConnectNode::VlpConnectNode(VlpBaseNode* first, VlpBaseNode* second)
               : VlpBaseNode(NODECLASS_CONNECT)
{
   _exp  = first;
   _next = second;
}

VlpConnectNode::~VlpConnectNode()
{
   if (_exp) delete _exp;
   if (_next) delete _next;
}
      
void 
VlpConnectNode::whoAmI(int i, int l) const
{
   cout << "(" << nodeNo << ")" << endl;
   cout.width(i+6);
   cout << "CNECT ";
   if (l > 0) {
      if (_exp) {
         cout << " : " << "EXP   " << " --> "; _exp->whoAmI(i+20, --l);
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
VlpConnectNode::writeWhoAmI(ofstream& os) const
{
}

MyString
VlpConnectNode::str() const 
{
   MyString tmp = _exp->str();
   if ( _next != NULL ) {
      tmp += ", ";
      tmp += _next->str();
   }
   return tmp;
}

set<MyString>
VlpConnectNode::setSIGs() const 
{
   set<MyString> tmp = _exp->setSIGs();
   if (_next != NULL) {
      set<MyString> tmpR = _next->setSIGs();
      for (set<MyString>::iterator pos = tmpR.begin(); pos != tmpR.end(); pos++)
         tmp.insert(*pos);
   }
   return tmp;
}

vector< VlpSignalNode* >
VlpConnectNode::listSIGs() const {
   vector< VlpSignalNode* > tmp,
                            tmpR;
   unsigned int             i;
   tmp  = _exp->listSIGs();
   if ( _next != NULL ) {
      tmpR = _next->listSIGs();

      for ( i = 0; i < tmpR.size(); ++i )
         tmp.push_back( tmpR[i] );
   }

   return tmp;
}

VlpBaseNode*
VlpConnectNode::duplicate() const 
{
   VlpConnectNode* dup;
   if (_next != NULL)
      dup = new VlpConnectNode(_exp->duplicate(), _next->duplicate());
   else
      dup = new VlpConnectNode(_exp->duplicate());

   return dup;
}

vector< BddNode >
VlpConnectNode::bdd( BddManager* bm, int& top, int& bottom,
                     const MyString sig ) const {
   vector< BddNode > ret,
                     exp;

   if ( _next != NULL ) {
      exp.clear();
      exp = _next->bdd( bm, top, bottom, sig );
      for ( unsigned int i = 0; i < exp.size(); ++i )
         ret.push_back( exp[i] );
   }

   exp = _exp->bdd( bm, top, bottom, sig );
   for ( unsigned int i = 0; i < exp.size(); ++i )
      ret.push_back( exp[i] );

   return ret;
}

#endif

