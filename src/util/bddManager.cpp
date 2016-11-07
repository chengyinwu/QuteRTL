/****************************************************************************
  FileName     [ bddManager.cpp ]
  PackageName  [ ]
  Synopsis     [ BDD Manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <assert.h>
#include <sstream>
#include "bddNode.h"
#include "bddManager.h"

// _level = 0 ==> const 1 & const 0
// _level = 1 ~ nin ==> supports
//
void
BddManager::init(size_t nin, size_t h, size_t c)
{
   _uniqueTable.init(h);
   _computedTable.init(c);

   // This must be called first
   BddNode::setBddManager(this);
   // [Note] Cannot call uniquify(0, 0, 0)
   //        because "new BddNodeInt(0, 0, 0) will crash...
   BddNodeInt::_terminal = new BddNodeInt;
   BddNodeInt::_faild    = new BddNodeInt;

   _uniqueTable.forceInsert(BddHashKey(0, 0, 0), BddNodeInt::_terminal);
   _uniqueTable.forceInsert(BddHashKey(0, 0, 1), BddNodeInt::_faild   );

   BddNode::_one   = BddNode(BddNodeInt::_terminal, BDD_POS_EDGE);
   BddNode::_zero  = BddNode(BddNodeInt::_terminal, BDD_NEG_EDGE);
   BddNode::_faild = BddNode(BddNodeInt::_faild,    BDD_POS_EDGE);
   BddNode::_error = BddNode(BddNodeInt::_faild,    BDD_NEG_EDGE);

   _supports.reserve(nin + 1);
   _supports.push_back(BddNode::_one);
   _supports[0].setName("1");
   //_supports.push_back(BddNode::_faild);

   for (size_t i = 1; i <= nin; ++i)
      _supports.push_back(BddNode(BddNode::_one(), BddNode::_zero(), i));
}
//===========================================================================//
void
BddManager::reset()
{
   // Clean up BddNodeInt*, _supports, _uniqueTable, _computedTable, _bddMap
   _supports.clear();
   _computedTable.reset();
   if (_uniqueTable.getNumBuckets() != 0) {
      BddHash::iterator li(_uniqueTable.getBucketsHead(), 0);
      for (li = _uniqueTable.begin(); li != _uniqueTable.end(); ++li)
         delete (*li);
   }
   BddNode::resetTerminal();
   _uniqueTable.reset();
   _bddMap.clear();
}

bool
BddManager::standardize(BddNode& f, BddNode& g, BddNode& h)
{
   // check identical
   if (f == g)                                       // (F, F, H) => (F, 1, H)
      g = BddNode::_one;
   else if (f.getBddNodeInt() == g.getBddNodeInt())  // (F, -F, H) => (F, 0, H)
      g = BddNode::_zero;
   if (f == h)                                       // (F, G, F) => (F, G, 0)
      h = BddNode::_zero;
   else if (f.getBddNodeInt() == h.getBddNodeInt())  // (F, G, -F) => (F, G, 1)
      h = BddNode::_one;
        
   // check symmetric
   if (g == BddNode::_one)                           // (F, 1, H) <=> (H, 1, F)
      if (f.getLevel() > h.getLevel() ||
         (f.getLevel() == h.getLevel() && f.getBddNodeInt() > h.getBddNodeInt()) )
      {
         swap(f, h);
      }
   else if (h == BddNode::_zero)                     // (F, G, 0) <=> (G, F, 0)
      if (f.getLevel() > g.getLevel() ||
         (f.getLevel() == g.getLevel() && f.getBddNodeInt() > g.getBddNodeInt()) )
      {
         swap(f, g);
      }
   else if (g == BddNode::_zero)                     // (F, 0, H) <=> (-H ,0 ,-F)
      if (f.getLevel() > h.getLevel() ||
         (f.getLevel() == h.getLevel() && f.getBddNodeInt() > h.getBddNodeInt()) )
      {
         f = ~f;
         h = ~h;
         swap(f, h);
      }
   else if (h == BddNode::_one)                      // (F, G, 1) <=> (-G, -F, 1)
      if (f.getLevel() > g.getLevel() ||
         (f.getLevel() == g.getLevel() && f.getBddNodeInt() > g.getBddNodeInt()) )
      {
         f = ~f;
         g = ~g;
         swap(f, g);
      }
   else if (g == ~h)                                 // (F, G, -G) <=> (G, F, -F)
      if (f.getLevel() > g.getLevel() ||
         (f.getLevel() == g.getLevel() && f.getBddNodeInt() > g.getBddNodeInt()) )
      {
         h = ~f;
         swap(f, g);
      }
        
   // check complement
   bool isNegEdge = false;
   if (f.isNegEdge()) {
      if (g.isNegEdge()) {
         if (h.isNegEdge()) {                        // (-F, -G, -H) => -(F,H,G)
            f.toPosEdge(); g.toPosEdge(); h.toPosEdge();
            swap(g, h);
            isNegEdge = true;
         }
         else {                                      // (-F, -G, H) => (F, H, -G)
            f.toPosEdge();
            swap(g, h);
            isNegEdge = false;
         }
      }
      else {
         if (h.isNegEdge()) {                        // (-F, G, -H) => -(F, H, -G)
            f.toPosEdge(); g.toNegEdge(); h.toPosEdge();
            swap(g, h);
            isNegEdge = true;
         }
         else {                                      // (-F, G, H) => (F, H, G)
            f.toPosEdge();
            swap(g, h);
            isNegEdge = false;
         }
      }
   }
   else {
      if (g.isNegEdge()) {
         if (h.isNegEdge()) {                        // (F, -G, -H) => -(F,G,H)
            g.toPosEdge(); h.toPosEdge();
            isNegEdge = true;
         }
         else {                                      // (F, -G, H) => -(F, G, -H)
            g.toPosEdge(); h.toNegEdge();
            isNegEdge = true;
         }
      }
      else                                           // (F, G, H) or (F, G, -H)
         isNegEdge = false;              
   }
   return isNegEdge;
}

//===========================================================================//

BddNode
BddManager::ite(BddNode f, BddNode g, BddNode h)
{
   bool isNegEdge = false;

   // standardize (f, g, h) triplet
   // "isNegEdge" may be updated too
   isNegEdge = standardize(f, g, h);
   assert(f.isPosEdge() && g.isPosEdge());
 // terminal case
   BddNode ret;
   if (checkIteTerminal(f, g, h, ret)) {
      if (isNegEdge) ret = ~ret;
      return ret;  // no need to update tables
   }

   // check computed table
   size_t pos,getNode;
   BddCacheKey key(f(), g(), h());
   if (_computedTable.check(key, pos, getNode)) {
      BddNode computedNode(getNode);
      if (isNegEdge)
         computedNode = ~computedNode;
      return computedNode;
   }
   // check top varaible
   unsigned v = f.getLevel();
#ifdef _WITH_BDD_DEBUG
   MyString nodeName = f.getName();
#endif
   if (g.getLevel() > v) {
      v = g.getLevel();
#ifdef _WITH_BDD_DEBUG
      nodeName = g.getName();
#endif
   }
   if (h.getLevel() > v) {
      v = h.getLevel();
#ifdef _WITH_BDD_DEBUG
      nodeName = h.getName();
#endif
   }
   // recursion
   BddNode fl = f.getLeftCofactor(v),
           gl = g.getLeftCofactor(v),
           hl = h.getLeftCofactor(v);
   BddNode t = ite(fl, gl, hl);

   BddNode fr = f.getRightCofactor(v),
           gr = g.getRightCofactor(v),
           hr = h.getRightCofactor(v);
   BddNode e = ite(fr, gr, hr);

   // get result
   if (t == e) {
      // update computed table      
      _computedTable.forceInsert(pos, BddCacheNode(key, t()));
      if (isNegEdge) t = ~t;
      return t;
   }

   // move bubble if necessary... ==> update isNedEdge
   if (t.isNegEdge()) {
      t.toPosEdge();
      e = ~e;
      isNegEdge = ~isNegEdge;
   }

   // check unique table
   BddNodeInt* ni = uniquify(t(), e(), v);
#ifdef _WITH_BDD_DEBUG
   ni->setName(nodeName);
#endif
   ret = BddNode(size_t(ni));
   // update computed table
   _computedTable.forceInsert(pos, BddCacheNode(key, ret()));
   if (isNegEdge)
      ret = ~ret;
   return ret;
}

BddNode
BddManager::compose( const BddNode& f, unsigned int level,
                     const BddNode& g ) {
   unsigned int u = f.getLevel();
   bool         isNegEdge = f.isNegEdge();

   if ( u < level ) {
      return f;
   }

   BddNode left  = isNegEdge ? ~(f.getLeft())  : f.getLeft();
   BddNode right = isNegEdge ? ~(f.getRight()) : f.getRight();

   if ( u == level ) {
      return ite( g, left, right );
   }

   BddNode t = compose( left, level, g );
   BddNode e = compose( right, level, g );

   return ite( getSupport(u), t, e );
}

void
BddManager::addNodeMap( const string src, BddNode node ) {
   if ( _bddMap.find( src ) == _bddMap.end() )
      _bddMap.insert( make_pair( src, node ) );
   else
      _bddMap[ src ] = node;
}

const BddNode &
BddManager::getNode(const string src ) const {
   map< const string, BddNode >::const_iterator mi;

   mi = _bddMap.find( src );
   if ( mi != _bddMap.end() )
      return mi->second;
   else
      return BddNode::_error;
}

// Check if triplet (l, r, i) is in _uniqueTable,
// If not, create a new node;
// else, return the hashed one
//
BddNodeInt*
BddManager::uniquify(size_t l, size_t r, unsigned i)
{
   size_t b;
   BddNodeInt* n = 0;
   BddHashKey k(l, r, i);
   if (!_uniqueTable.check(k, b, n)) {
      n = new BddNodeInt(l, r, i);
      _uniqueTable.forceInsert(b, BddHashNode(k, n));
   }
   return n;
}

bool
BddManager::checkIteTerminal
(const BddNode& f, const BddNode& g, const BddNode& h, BddNode& n)
{
   if (g == h) { n = g; return true; }
   if (f == BddNode::_one) { n = g; return true; }
   if (f == BddNode::_zero) { n = h; return true; }
   if (g == BddNode::_one && h == BddNode::_zero) { n = f; return true; }
   return false;
}

void
BddManager::setSupports(int no, const string& str)
{
   if ((int)supportsNum() == -1)
      cerr << "Error: no init bdd, init bdd first" << endl;
   else if( (no < 1) || (no > (int)supportsNum()) ) {
      cerr << "Error: Invaild level " << no << endl
           << "       Level must be set between 1~" << supportsNum() << endl;
   }
   else {
      _supports[no].setName(str);
      assert((unsigned)no < _supports.size());
      _bddMap.insert(make_pair(str, _supports[no]));
   }
}

const BddNode&
BddManager::getSupport(size_t i) const 
{
   if ( i <= _supports.size() )
      return _supports[i];
   else
      return BddNode::_error;
}

bool
BddManager::coFactor(istream& Cin)
{
   string CF_type, newNodeName, targetNode, factor;
   Cin >> CF_type >> newNodeName >> targetNode >> factor;

   if (_bddMap.find(targetNode) == _bddMap.end())//Error: missing node in map
      return errorMsg(MISSING_BDD_NODE, targetNode);
   if (_bddMap.find(factor) == _bddMap.end())//Error: missing node in map
      return errorMsg(MISSING_BDD_NODE, factor);
   if (_bddMap[factor].getLeft().getBddNodeInt() != 
       _bddMap[factor].getRight().getBddNodeInt())
   {
      cerr << "Error: The cofactor operation only for variable, not for cube!!" 
           << endl;
      return false;
   }
   unsigned level;//get support level
   for (level = 0; level <= _supports.size(); ++level)
      if (_supports[level].getBddNodeInt() == _bddMap[factor].getBddNodeInt())
         break;
   
   if (CF_type == "-pos") {    
      //If newNodeName exists, map will overwrite it, else insert it.
      _bddMap[newNodeName] = _bddMap[targetNode].NodeCoFactor(_supports, level);
      return true;
   }
   else if (CF_type == "-neg") {// use negative value of level => negtive cofactot
      _bddMap[newNodeName] = _bddMap[targetNode].NodeCoFactor(_supports, -level);
      return true;
   }
   else {
      cerr << "Error: Illegal argument " << CF_type << endl;
      return false;
   }
}

bool
BddManager::bddReport(const string& str) const
{
   map<string, BddNode>::const_iterator li = _bddMap.find(str);
   if (li == _bddMap.end())//Error: missing node in map
      return errorMsg(MISSING_BDD_NODE, str);

   cout << li->second << endl;
   return true;
}

BDD_RELATION
BddManager::compareBdd(string& nodeName1, string& nodeName2) const
{
   map<string, BddNode>::const_iterator li1 = _bddMap.find(nodeName1);   
   map<string, BddNode>::const_iterator li2 = _bddMap.find(nodeName2);

   if (li1 == _bddMap.end()) { //Error: missing node in map
      errorMsg(MISSING_BDD_NODE, nodeName1);
      return BDD_NEQ;
   }
   if (li2 == _bddMap.end()) {//Error: missing node in map
      errorMsg(MISSING_BDD_NODE, nodeName2);
      return BDD_NEQ;
   }

   if (li1->second == li2->second) 
      return BDD_EQ;
   else if ( li1->second.getBddNodeInt() == li2->second.getBddNodeInt() ) 
      return BDD_IEQ;
   else
      return BDD_NEQ;
}

void
BddManager::drawBdd( const string str, const string filename ) const {
   map< string, BddNode >::const_iterator mi = _bddMap.find( str );

   if ( mi == _bddMap.end() ){
      cerr << "BddManager::drawBdd() ---> ERROR: Undefine node " << str
           << " in BDD Manager." << endl;
      return;
   }

   ofstream draw( filename.c_str(), ofstream::out );

   draw << "digraph G {" <<endl;
   draw << "\t\"" << str << "\"" << " -> ";

   (mi->second).draw( draw );

   draw << "}";
   draw.close();
}

bool
BddManager::errorMsg(ErrorType e, string token) const
{
   if (e == SYNTAX_ERROR)
      cerr << "Error: Syntax error \"" << token << endl;
   else if (e == MISSING_BDD_NODE)
   {
      cerr << "Error: BddNode \"" << token
           << "\" is undefined" << endl;
   }
   else if (e == MISSING_GATE)
   {
      cerr << "Error: Gate \"" << token
           << "\" is undefined" << endl;
   }
   else {
      assert(e == MISSING_ARG);
      cerr << "Error: Missing argument after \"" << token
           << "\"" << endl;
   }
   return false;
}

void
BddManager::initCube(string& allCube, int& sNum)
{
   MyString prefix = "SP";
   MyString myStr = "";
   for (int i = 1; i <= sNum; ++i) {
      myStr = prefix + i;
      setSupports(i, myStr.str());
   }

   _bddMap[allCube] = BddNode::_zero;
}

void 
BddManager::insertCube(string& allCube, Bv4 bv_cube)
{
   //int cubeBits = bv_cube.bits();
   unsigned cubeBits = bv_cube.bits();
   assert(cubeBits == supportsNum());//exclude the "ONE" node
   string tmpCube = "tmp";
   _bddMap[tmpCube] = BddNode::_one;//initialize

   //note : _supports[0] is ONE node, so j from 1
   int j = 1;
   for (int i = cubeBits-1; i >= 0; --i, ++j) {//from msb to lsb
      if ((bv_cube[i]) == _BV4_1)
         _bddMap[tmpCube] &= _supports[j];
      else if ((bv_cube[i]) == _BV4_0) 
         _bddMap[tmpCube] &= (~_supports[j]);
      else if (bv_cube[i] == _BV4_X)
         ;//do nothing
      else if ((bv_cube[i]) == _BV4_Z) 
         cerr << "check !! BDD Z" << endl;
      else 
         cerr << "Error : impossible bv4 value!!" << endl;
   }
   _bddMap[allCube] |= _bddMap[tmpCube];
}

bool
BddManager::isFullCube(string& allCube) const
{
   map<string, BddNode>::const_iterator li = _bddMap.find(allCube);   

   if (li == _bddMap.end())//Error: missing node in map
      return errorMsg(MISSING_BDD_NODE, allCube);

   if (li->second == BddNode::_one)
      return true;
   else
      return false;
}

