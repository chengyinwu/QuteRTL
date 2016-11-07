/****************************************************************************
  FileName     [ vlpFSM.cpp ]
  Package      [ vlp ]
  Synopsis     [ Implementation of FSM class ]
  Author       [ Chi-Wen (Kevin) Chang ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_FSM_SOURCE
#define VLP_FSM_SOURCE

#include "vlpFSM.h"
#include "vlpCU.h"

//-----------------------------------------------------------------------------
// VlpFSMAssign
//-----------------------------------------------------------------------------
VlpFSMAssign::VlpFSMAssign() {
   _seq  = false;
   _se   = NULL;
   _ts   = NULL;
}

VlpFSMAssign::~VlpFSMAssign() {
   _ac.clear();
}

void
VlpFSMAssign::setAC( const stack< size_t >& src ) {
   stack< size_t > tmp = src;

   _ac.clear();

   while ( tmp.empty() == false ) {
      _ac.push_back( tmp.top() );
      tmp.pop();
   }
}

void
VlpFSMAssign::setSE( const VlpBaseNode* src ) {
   _se = src;
}

void
VlpFSMAssign::setTS( const VlpBaseNode* src ) {
   _ts = src;
}

vector< size_t >
VlpFSMAssign::getAC() const {
   return _ac;
}

const VlpBaseNode*
VlpFSMAssign::getSE() const {
   return _se;
}

const VlpBaseNode*
VlpFSMAssign::getTS() const {
   return _ts;
}

set< MyString >
VlpFSMAssign::ACset() const {
   size_t             tmpCond;
   size_t*            ptrCond;
   const VlpBaseNode* tmpNode;
   unsigned int       i;
   set< MyString >    tmp,
                      tmpR;
   set< MyString >::iterator pos;

   for ( i = 0; i < _ac.size(); ++i ) {
      tmpCond = _ac[i] & 0xFFFFFFFE;

      ptrCond = (size_t*)&tmpNode;
      *ptrCond = tmpCond;

      tmpR = tmpNode->setSIGs();
      for ( pos = tmpR.begin(); pos != tmpR.end(); pos++ )
         tmp.insert( *pos );
   }

   return tmp;
}

set< MyString >
VlpFSMAssign::SEset() const {
   return _se->setSIGs();
}

MyString
VlpFSMAssign::strAC() const {
   size_t             tmpCond;
   size_t*            ptrCond;
   const VlpBaseNode* tmpNode;
   unsigned int       i;
   bool               neg;
   MyString           tmp;

   tmp = "";
   for ( i = 0; i < _ac.size(); i++ ) {
      tmpCond = _ac[ i ];
      if ( tmpCond & 0x00000001 > 0 )
         neg = true;
      else
         neg = false;

      tmpCond = tmpCond & 0xFFFFFFFE;
      ptrCond = (size_t*)&tmpNode;
      *ptrCond = tmpCond;
      if ( neg )
         tmp += "!(";
      tmp += tmpNode->str();
      if ( neg )
         tmp += ")";
      if ( ( i + 1 ) < _ac.size() )
        tmp += " && ";
   }
   return tmp;
}

MyString
VlpFSMAssign::strSE() const {
   return _se->str();
}

MyString
VlpFSMAssign::strTS() const {
   return static_cast< const VlpSignalNode* >( _ts )->strNoIndex();
}

MyString
VlpFSMAssign::strTSwIndex() const {
   return _ts->str();
}

int
VlpFSMAssign::TS_MSB() const {
   return static_cast<const VlpSignalNode*>(_ts)->MSB();
}

int
VlpFSMAssign::TS_LSB() const {
   return static_cast<const VlpSignalNode*>(_ts)->LSB();
}

int
VlpFSMAssign::TS_width() const {
   if ( TS_MSB() > TS_LSB() )
      return TS_MSB() - TS_LSB() + 1;
   else
      return TS_LSB() - TS_MSB() + 1;
}

void
VlpFSMAssign::setSeq() {
   _seq = true;
}

void
VlpFSMAssign::setNonSeq() {
   _seq = false;
}

bool
VlpFSMAssign::isSeq() const {
   return _seq;
}

void
VlpFSMAssign::reset() {
   _ac.clear();
   _se   = NULL;
   _ts   = NULL;
   _seq  = false;
}

void
VlpFSMAssign::toImpView() {
   size_t             tmpCond;
   size_t*            ptrCond;
   VlpBaseNode*       tmpNode,
                      *tmpACNode,
                      *acNode,
                      *seNode;
   unsigned int       i;
   bool               neg;

   if ( _ac.size() > 0 ) {
      acNode = NULL;
      for ( i = 0; i < _ac.size(); i++ ) {
         tmpCond = _ac[ i ];
         if ( tmpCond & 0x00000001 > 0 )
            neg = true;
         else
            neg = false;

         tmpCond = tmpCond & 0xFFFFFFFE;

         ptrCond = (size_t*)&tmpNode;
         *ptrCond = tmpCond;
         if ( neg ) {
            tmpACNode = new VlpLogicNotOpNode( tmpNode );
         }
         else {
            tmpACNode = tmpNode;
         }

         if ( acNode == NULL ) {
            acNode = tmpACNode;
         }
         else {
            acNode = new VlpLogicOpNode( LOGICOP_AND, acNode, tmpNode );
         }
      }

      seNode = const_cast<VlpBaseNode*>( _se );
      seNode = new VlpBitWiseOpNode( BITWISEOP_AND, acNode, seNode );
      _ac.clear();
      _se = seNode;
   }
}

VlpFSMAssign&
VlpFSMAssign::operator = ( const VlpFSMAssign& src ) {
   _ac   = src._ac;
   _se   = src._se;
   _ts   = src._ts;
   _seq  = src._seq;

   return *this;
}

bool
VlpFSMAssign::operator == ( const VlpFSMAssign& src ) const {
   return (_ac == src._ac) && (_se == src._se) && (_ts == src._ts);
}

bool
VlpFSMAssign::operator != ( const VlpFSMAssign& src ) const {
   return (_ac != src._ac) || (_se != src._se) || (_ts != src._ts);
}

bool
VlpFSMAssign::operator < ( const VlpFSMAssign& src ) const {
   return strTS() < src.strTS();
}

bool
VlpFSMAssign::operator > ( const VlpFSMAssign& src ) const {
   return strTS() > src.strTS();
}

ostream&
operator << ( ostream& os, const VlpFSMAssign &src ) {
   size_t             tmpCond;
   size_t*            ptrCond;
   const VlpBaseNode* tmpNode;
   unsigned int       i;
   bool               neg;

   os << "> TS: " << static_cast<const VlpSignalNode*>(src._ts)->strNoIndex()
      << "[" << static_cast<const VlpSignalNode*>(src._ts)->MSB()
      << ":" << static_cast<const VlpSignalNode*>(src._ts)->LSB()
      << "]" << endl;
   os << "> AC: ";
   for ( i = 0; i < src._ac.size(); i++ ) {
      tmpCond = src._ac[ i ];
      if ( tmpCond & 0x00000001 > 0 )
         neg = true;
      else
         neg = false;

      tmpCond = tmpCond & 0xFFFFFFFE;

      ptrCond = (size_t*)&tmpNode;
      *ptrCond = tmpCond;
      if ( neg ) 
         os << "!(";
      os << tmpNode->str();
      if ( neg )
         os << ")";
      if ( ( i + 1 ) < src._ac.size() )
        os << " && ";
   }

   os << endl;
   os << "> SE: " << src._se->str() << endl;

   return os;
}
/*
//-----------------------------------------------------------------------------
// VlpFSMPSR
//-----------------------------------------------------------------------------
VlpFSMPSR::VlpFSMPSR() {
   _psr.clear();
}

VlpFSMPSR::~VlpFSMPSR() {
}

void
VlpFSMPSR::addSig( const VlpBaseNode* src ) {
   _psr.push_back( src->str() );
}

void
VlpFSMPSR::addSig( const MyString& src ) {
   _psr.push_back( src );
}

void
VlpFSMPSR::addSig( const string& src ) {
   MyString tmp = src;

   _psr.push_back( tmp );
}

void
VlpFSMPSR::addSig( const char* src ) {
   MyString tmp = src;

   _psr.push_back( tmp );
}

unsigned int
VlpFSMPSR::size() const {
   return _psr.size();
}

MyString
VlpFSMPSR::getPSR( unsigned int index ) const {
   if ( index < _psr.size() )
      return _psr[ index ];
   else {
      cerr << "VlpFSMPSR::getPSR ---> WRANING: Index out of valid range."
           << endl;
   }

   return "";
}
*/
//-----------------------------------------------------------------------------
// VlpFSMADS
//-----------------------------------------------------------------------------
VlpFSMADS::VlpFSMADS() {
   _ads.clear();
}

VlpFSMADS::~VlpFSMADS() {
}

void 
VlpFSMADS::addSig( const VlpBaseNode* src ) {
   _ads.insert( src->str() );
}

void
VlpFSMADS::addSig( const MyString& src ) {
   _ads.insert( src );
}

void
VlpFSMADS::addSig( const string& src ) {
   MyString tmp = src;
  
   _ads.insert( src );
}

void
VlpFSMADS::addSig( const char* src ) {
   MyString tmp = src;

   _ads.insert( src );
}

unsigned int
VlpFSMADS::size() const {
   return _ads.size();
}

MyString
VlpFSMADS::getSig(unsigned index) const 
{
   if (index < _ads.size()) {
      for (set<MyString>::const_iterator pos = _ads.begin(); pos != _ads.end(); pos++) {
         if (index == 0)
            return *pos;

         --index;
      }
      return "";
   }
   else
      cerr << "VlpFSMADS::getSig() ---> WARNING: Index out of valid range." << endl;

   return "";
}

bool
VlpFSMADS::inADS( const MyString& src ) const {
   if ( _ads.find( src ) != _ads.end() )
      return true;
   else
      return false;
}

void
VlpFSMADS::clear() {
   _ads.clear();
}

VlpFSMADS&
VlpFSMADS::operator = ( const VlpFSMADS& src ) {
   _ads = src._ads;
   return *this;
}

ostream&
operator << ( ostream& os, const VlpFSMADS& src ) {
   set< MyString >::const_iterator pos;
   unsigned int                    index = 0;

   for ( pos = src._ads.begin(); pos != src._ads.end(); pos++ ) {
      os << *pos;
      if ( ( index + 1 ) < src._ads.size() )
         os << " ";
   }

   return os;
}

//-----------------------------------------------------------------------------
// VlpFSMACS
//-----------------------------------------------------------------------------
VlpFSMACS::VlpFSMACS() {
   _acs.clear();
}

VlpFSMACS::~VlpFSMACS() {
}

void
VlpFSMACS::addSig( const VlpBaseNode* src ) {
   _acs.insert( src->str() );
}

void
VlpFSMACS::addSig( const MyString& src ) {
   _acs.insert( src );
}

void
VlpFSMACS::addSig( const string& src ) {
   MyString tmp = src;

   _acs.insert( tmp );
}

void
VlpFSMACS::addSig( const char* src ) {
   MyString tmp = src;

   _acs.insert( tmp );
}

unsigned int
VlpFSMACS::size() const {
   return _acs.size();
}

MyString
VlpFSMACS::getSig( unsigned int index ) const {
   if ( index < _acs.size() ) {
      set< MyString >::const_iterator pos;
      for ( pos = _acs.begin(); pos != _acs.end(); pos++ ) {
         if ( index == 0 )
            return *pos;

         --index;
      }
      return "";
   }
   else {
      cerr << "VlpFSMACS::getSig() ---> WARNING: Index out of valid range."
           << endl;
   }

   return "";
}

bool
VlpFSMACS::inACS( const MyString& src ) const {
   if ( _acs.find( src ) != _acs.end() )
      return true;
   else
      return false;
}

void
VlpFSMACS::clear() {
   _acs.clear();
}

VlpFSMACS&
VlpFSMACS::operator = ( const VlpFSMACS& src ) {
   _acs = src._acs;
   return *this;
}

ostream&
operator << ( ostream& os, const VlpFSMACS& src ) {
   set< MyString >::const_iterator pos;
   unsigned int                    index = 0;

   for ( pos = src._acs.begin(); pos != src._acs.end(); pos++ ) {
      os << *pos;
      if ( ( index + 1 ) < src._acs.size() )
         os << " ";
   }

   return os;
}

//-----------------------------------------------------------------------------
// VlpFSMSIG
//-----------------------------------------------------------------------------
VlpFSMSIG::VlpFSMSIG() {
   _name  = "";
   _msb   = -1; 
   _lsb   = -1;
   _isEX  = false;
   _isIMP = false;
   _coSIG = "";
   _acs.clear();
   _ads.clear();
   _al.clear();
}

VlpFSMSIG::~VlpFSMSIG() {
}

void
VlpFSMSIG::setName( const MyString& src ) {
   _name = src;
}

void
VlpFSMSIG::setName( const string& src ) {
   _name = src;
}

void
VlpFSMSIG::setName( const char* src ) {
   _name = src;
}

MyString
VlpFSMSIG::name() const {
   return _name;
}

void
VlpFSMSIG::setMSB( const int src ) {
   _msb = src;
}

void 
VlpFSMSIG::setLSB( const int src ) {  
   _lsb = src;
}

int
VlpFSMSIG::MSB() const {
   return _msb;
}

int
VlpFSMSIG::LSB() const {
   return _lsb;
}

void
VlpFSMSIG::addAssign(const VlpFSMAssign& src) 
{
   _al.push_back(src);
   set<MyString> tmp = src.ACset();
   
   set< MyString >::iterator pos;
   for (pos = tmp.begin(); pos != tmp.end(); pos++)
      addACS(*pos);

   tmp = src.SEset();
   for (pos = tmp.begin(); pos != tmp.end(); pos++)
      addADS(*pos);
}

unsigned int
VlpFSMSIG::sizeAL() const {
   return _al.size();
}

VlpFSMAssign
VlpFSMSIG::getAL( unsigned int index ) const {
   if ( index < _al.size() ) {
      return _al[ index ];
   }
   else {
      cerr << "VlpFSMSIG::getAL() ---> WARNING: Index out of valid range."
           << endl;
   }

   return VlpFSMAssign();
}

bool
VlpFSMSIG::inACS( const MyString& src ) const {
   return _acs.inACS( src );
}

void
VlpFSMSIG::addACS( const VlpBaseNode* src ) {
   _acs.addSig( src );
}

void
VlpFSMSIG::addACS( const MyString& src ) {
   _acs.addSig( src );
}

void
VlpFSMSIG::addACS( const string& src ) {
   _acs.addSig( src );
}

void
VlpFSMSIG::addACS( const char* src ) {
   _acs.addSig( src );
}

unsigned int 
VlpFSMSIG::sizeACS() const {
   return _acs.size();
}

MyString
VlpFSMSIG::getACS( unsigned int index ) const {
   return _acs.getSig( index );
}

void
VlpFSMSIG::addADS( const VlpBaseNode* src ) {
   _ads.addSig( src );
}

void
VlpFSMSIG::addADS( const MyString& src ) {
   _ads.addSig( src );
}

void
VlpFSMSIG::addADS( const string& src ) {
   _ads.addSig( src );
}

void
VlpFSMSIG::addADS( const char* src ) {
   _ads.addSig( src );
}

unsigned int
VlpFSMSIG::sizeADS() const {
   return _ads.size();
}

MyString
VlpFSMSIG::getADS( unsigned int index ) const {
    return _ads.getSig( index );
}

bool
VlpFSMSIG::inADS( const MyString& src ) const {
   return _ads.inADS( src );
}

void
VlpFSMSIG::setEX() {
   _isEX = true;
}

void
VlpFSMSIG::setIMP() {
   _isIMP = true;
}

void
VlpFSMSIG::resetType() {
   _isEX  = false;
   _isIMP = false;
}

bool
VlpFSMSIG::isEX() const {
   return _isEX;
}

bool
VlpFSMSIG::isIMP() const {
   return _isIMP;
}

bool
VlpFSMSIG::isSR() const {
   return _isEX || _isIMP;
}

void
VlpFSMSIG::setCoSIG( const MyString& src ) {
   _coSIG = src;
}

MyString
VlpFSMSIG::coSIG() const {
   return _coSIG;
}

bool
VlpFSMSIG::build_X2Y(bool& buildFaild1, vector<MyString>& Counter, int& top, int& bottom, BddManager& bm) const
{
   VlpFSMAssign assign;
   const VlpBaseNode* node;
   vector<BddNode> resNode;
   int index;
   MyString sigName;
   string nodeName;
   //set<MyString> setADS;
   bool allSingle = true;
   for (unsigned i = 0; i < sizeAL() && buildFaild1 == false; ++i) {
      assign = getAL(i);
      cout << assign << endl;
      node = assign.getSE();

/*    if ( coSIG() == name() ) {
         setADS = node->setSIGs();
         if ( setADS.find( name() ) == setADS.end() ) {
            buildFaild1 = true;
            break;
         }
      }*/
      resNode = (assign.getSE())->bdd(&bm, top, bottom, name());
      index = assign.TS_LSB();
      if ( !(   node->getNodeClass() == NODECLASS_INT 
             || node->getNodeClass() == NODECLASS_SIGNAL 
             || node->getNodeClass() == NODECLASS_PARAMETER ) ) {
         allSingle = false;
      }   

      for (unsigned j = 0; j < resNode.size(); ++j) {
         if (resNode[j] == BddNode::_error) { // Bdd Construction faild
            cout << "  > Constructing BDD for signal " << name() << " failed. " << endl;
            Counter.push_back(name());
            buildFaild1 = true;
            break;
         }
         else if (resNode[j] == BddNode::_one || resNode[j] == BddNode::_zero) {
            cout << "  > Ignoring constant assignment." << endl;
            break;
         }
         else {
            sigName = "bdd_";
            sigName += name();
            if (index >= 0)
               sigName << '[' << index << ']';

            nodeName = sigName.str();
            bm.addNodeMap(nodeName, resNode[j]);
            cout << "  > Constructing BDD for signal " << nodeName << " success. " << endl;
            if (assign.TS_MSB() > assign.TS_LSB()) {
               if ( (++index) > assign.TS_MSB() )
                  break;
            }
            else {
               if ((--index) < assign.TS_MSB())
                  break;
            }
         }
      }
   }
   return allSingle;
}

bool         
VlpFSMSIG::build_Y2Z(VlpFSMSIG& sigCo, vector<MyString>& Counter, vector<VlpFSMAssign>& reset, int& top, int& bottom, BddManager& bm) const
{
   bool buildFaild2 = false;
   VlpFSMAssign      assign;
   vector<BddNode>   resNode;
   int               index;
   MyString          sigName;
   string            nodeName;
   if ((name() != sigCo.name()) && sigCo.isEX() == false) {
      cout << "> Construct Mapping Y ---> Z" << endl;
      for (unsigned i = 0; i < sigCo.sizeAL() && buildFaild2 == false; ++i) {
         assign  = sigCo.getAL(i);
         resNode = (assign.getSE())->bdd(&bm, top, bottom, name());
         index   = assign.TS_LSB();

         for (unsigned j = 0; j < resNode.size() && index >= 0; ++j) {
            if (resNode[j] == BddNode::_error) { // Bdd faild
               cout << "  > Constructing BDD for signal " << sigCo.name() << " failed. " << endl;
               Counter.push_back(sigCo.name());
               buildFaild2 = true;
               break;
            }
            else if (resNode[j] == BddNode::_one || resNode[j] == BddNode::_zero) {
               cout << "Ignoring constant assignment." << endl;
               reset.push_back( assign );
               break;
            }
            else {
               sigName = "bdd_";
               sigName += sigCo.name();
               if ( index >= 0 )
                  sigName << '[' << index << ']';
               nodeName = sigName.str();
               bm.addNodeMap( nodeName, resNode[j] );
               cout << "  > Constructing BDD for signal " << nodeName << " success. " << endl;
               if (assign.TS_MSB() > assign.TS_LSB()) {
                  if ( (++index) > assign.TS_MSB() )
                     break;
               }
               else {
                  if ( (--index) < assign.TS_MSB() )
                     break;
               }
            }
         }
      }
   }
   return buildFaild2;
}

void
VlpFSMSIG::composeTr(VlpFSMSIG& sigCo, BddNode& trNode, BddManager& bm) const
{
   BddNode  tmpNode, leftNode, rightNode;
   int      begin, end;
   string   nodeName;
   MyString sigName;
   if (MSB() > LSB()) {
      begin = LSB(); end = MSB();
   }
   else {
      begin = MSB(); end = LSB();
   }

   for (int i = begin; i <= end; ++i) {
      sigName = "";
      sigName << name() << '[' << i << ']';
      nodeName = sigName.str();
      leftNode = bm.getNode(nodeName);

      sigName = "bdd_";
      sigName << name() << '[' << i << ']';
      nodeName = sigName.str();
      rightNode = bm.getNode(nodeName);
      tmpNode = leftNode.xnor(rightNode);
      if (i == begin) trNode = tmpNode;
      else            trNode = trNode & tmpNode;
   }
   if (trNode != BddNode::_error) {
      sigName  = "TR_"; 
      sigName += name();
      nodeName = sigName.str();
      bm.addNodeMap(nodeName, trNode);
/*    sigName  = "BDD_TR_";
      sigName << name() << ".dot";
      fileName = sigName.str();
      bm.drawBdd( nodeName, fileName );
      cout << "  > Drawing BDD TR_" << name() << endl;
      sigName  = "dot -Tpng -o BDD_TR_";
      sigName << name() << ".png " << fileName << "; rm -f "
              << fileName;
      system( sigName.str() ); */
      trNode = bm.getNode( nodeName );
   }

   if (sigCo.isEX() == false) {
      if (sigCo.MSB() > sigCo.LSB()) {
         begin = sigCo.LSB(); end = sigCo.MSB();
      }
      else {
         begin = sigCo.MSB(); end = sigCo.LSB();
      }
      for (int i = begin; i <= end; ++i) {
         sigName = "";
         sigName << sigCo.name() << '[' << i << ']';
         nodeName = sigName.str();
         leftNode = bm.getNode( nodeName );

         sigName = "bdd_";
         sigName << sigCo.name() << '[' << i << ']';
         nodeName = sigName.str();
         rightNode = bm.getNode( nodeName );
         unsigned level = leftNode.getLevel();
         cout << "  > Compose " << trNode.getName() << " and " << leftNode.getName() << endl;
         trNode = bm.compose( trNode, level, rightNode );
         sigName = "com_";
         sigName << sigCo.name() << '[' << i << ']';
         nodeName = sigName.str();
         bm.addNodeMap( nodeName, trNode );
      }
   }
}

VlpFSM*
VlpFSMSIG::buildFSM(VlpFSMSIG& sigCo, BddNode& trNode, BddManager& bm) const
{
   MyString sigName = "";
   sigName += name();
   string nodeName = sigName.str();
   bm.addNodeMap( nodeName, trNode );
/* sigName  = "BDD_";
   sigName << name() << ".dot";
   fileName = sigName.str();

   bm.drawBdd( nodeName, fileName );
   cout << "  > Drawing BDD " << name() << endl;

   sigName  = "dot -Tpng -o BDD_";
   sigName << name() << ".png " << fileName << "; rm -f "
           << fileName;
   system( sigName.str() ); */
   int begin = LSB();
   int end   = MSB();
   if (begin > end) {
      int tmp = end; end = begin; begin = tmp;
   }

   VlpFSM* newFSM = new VlpFSM(); newFSM->setImpType(); newFSM->setName(name()); newFSM->setMSB(MSB()); newFSM->setLSB(LSB());
   MyString coName;
   if (name() == sigCo.name()) {
      coName = "n_";
      coName += name();
   }
   else                            
      coName = sigCo.name();

   Bv4         cs, ns;
   vector<Bv4> listCS, listNS;
   VlpFSMState *curState, *nxtState;  
   VlpFSMCond  newCond;
   VlpFSMTrans newTrans;
   MyBDDPath   eachPath;
   vector<MyBDDPath> paths = trNode.evaluate();
   for (unsigned i = 0; i < paths.size(); ++i) {
      eachPath = paths[i];
      if (eachPath.value()) {
         newTrans.reset();
         sigName = (end-begin)+1;
         sigName += "'bx";
         cs = sigName.str();
         ns = sigName.str();
         for (unsigned j = 0; j < eachPath.nodeSize(); ++j) {
            int index = readIndex(eachPath[j].name());
            sigName = nameNoIndex(eachPath[j].name());
            if (sigName == name()) { //next state
               if (eachPath[j].value()) ns.set(index-begin, _BV4_1);
               else                     ns.set(index-begin, _BV4_0);
            }
            else if (sigName == coName) { // current state
               if (eachPath[j].value()) cs.set(index-begin, _BV4_1);
               else                     cs.set(index-begin, _BV4_0);
            }
            else {
               newCond.reset();
               newCond.setLeft(eachPath[j].name());
               if (eachPath[j].value() == false)
                  newCond.setOP(VLPCUCOND_NOT);
               newTrans.addCond(newCond);
            }
         }
         listCS = expandBV(cs);
         listNS = expandBV(ns);
         for (unsigned j = 0; j < listCS.size(); ++j) {
            cs = listCS[j];
            curState = newFSM->getState( cs.str() );
            if (curState == NULL) {
               curState = new VlpFSMState(cs.str());
               newFSM->addState(curState);
            }
            for (unsigned k = 0; k < listNS.size(); ++k) {
               ns = listNS[k];
               nxtState = newFSM->getState(ns.str());
               if (nxtState == NULL) {
                  nxtState = new VlpFSMState(ns.str());
                  newFSM->addState(nxtState);
               }
               newTrans.setNextState(nxtState);
               curState->addTrans(newTrans);
            }
         }
      }
   }
   return newFSM;
}

VlpFSM*
VlpFSMSIG::build_tr(VlpFSMSIG& sigCo, vector<VlpFSMAssign>& reset, BddManager& bm) const
{
   cout << "  > Constructing TR for signal " << name() << "[" << MSB() << ":" << LSB() << "]" << endl;
   BddNode  trNode = BddNode::_error;
   composeTr(sigCo, trNode, bm);
   VlpFSM* newFSM = buildFSM(sigCo, trNode, bm);

   newFSM->addExState(reset);
   newFSM->analysis();
   return newFSM;
}

MyString
VlpFSMSIG::nameNoIndex(const MyString& src) const 
{
   string tmp = src.str();
   size_t begin = tmp.find_first_of( '[' );
   MyString ret;

//BEGIN modified by timrau
   if (begin != string::npos)
      ret = src.substr( 0, begin  );
   else
      ret = src;
//END modified by timrau
   return ret;
}

int
VlpFSMSIG::readIndex(const MyString& src) const 
{
   string tmp = src.str();
   int begin = tmp.find_first_of( '[' );
   int end   = tmp.find_first_of( ']' );

   string value;
   if ( (begin+1) <= (end-1) ) {
      value = tmp.substr( begin+1, end-1 );
      return (atoi(value.c_str()));
   }
   else 
      return -1;
}

vector<Bv4>
VlpFSMSIG::expandBV(const Bv4& src) const 
{
   vector<Bv4> ret;
   int xCount = 0;

   for (unsigned i = 0; i < src.bits(); ++i)
      if (src[i] == _BV4_X)
         ++xCount;
   if (xCount == 0) {
      ret.push_back( src );
      return ret;
   }

   unsigned limit = unsigned( pow(2.0, xCount) );
   int index;
   Bv4 tmpBv;

   for (unsigned i = 0; i < limit; ++i) {
      index = 0;
      tmpBv = src;
      for (unsigned j = 0; j < src.bits(); ++j) {
         if (src[j] == _BV4_X) {
            if ( (i&(0x00000001 << index)) > 0 )
               tmpBv.set(j, _BV4_1);
            else
               tmpBv.set(j, _BV4_0);
            ++index;
         }
      }
      ret.push_back(tmpBv);
   }
   return ret;
}

bool
VlpFSMSIG::operator == ( const VlpFSMSIG& src ) const {
   return _name == src._name;
}

bool
VlpFSMSIG::operator != ( const VlpFSMSIG& src ) const {
   return _name != src._name;
}

bool
VlpFSMSIG::operator < ( const VlpFSMSIG& src ) const {
   return _name < src._name;
}

bool
VlpFSMSIG::operator > ( const VlpFSMSIG& src ) const {
   return _name > src._name;
}

VlpFSMSIG&
VlpFSMSIG::operator = ( const VlpFSMSIG& src ) {
   _name  = src._name;
   _msb   = src._msb;
   _lsb   = src._lsb;
   _acs   = src._acs;
   _ads   = src._ads;
   _al    = src._al;
   _isEX  = src._isEX;
   _isIMP = src._isIMP;
   _coSIG = src._coSIG;
   return *this;
}

ostream&
operator << ( ostream& os, const VlpFSMSIG& src ) {
   os << "Signal: " << src._name << endl;
   os << "> ADS: " << src._ads << endl;
   os << "> ACS: " << src._acs << endl;
   os << "> ALs: " << src._al.size() << endl;
   return os;
}

#endif

