/****************************************************************************
  FileName     [ bddNode.cpp ]
  PackageName  [ ]
  Synopsis     [ Define BDD Node member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <assert.h>
#include "bddNode.h"
#include "bddManager.h"

// Initialize static data members
//
BddManager* BddNode::_BddManager = 0;
BddNodeInt* BddNodeInt::_terminal = 0;
BddNodeInt* BddNodeInt::_faild    = NULL;

BddNode BddNode::_one;
BddNode BddNode::_zero;
BddNode BddNode::_faild;
BddNode BddNode::_error;

// We check the hash when a new BddNodeInt is possibly being created
BddNode::BddNode(size_t l, size_t r, size_t i, BDD_EDGE_FLAG f)
{
   BddNodeInt* n = _BddManager->uniquify(l, r, i);
   // n should not = 0
   _nodeV = size_t(n) + f;
   n->incRefCount();
}

// Copy constructor also needs to increase the _refCount
BddNode::BddNode(const BddNode& n) : _nodeV(n._nodeV)
{
   BddNodeInt* t = getBddNodeInt();
   if (t)
      t->incRefCount();
}

// n must have been uniquified... should not be "0"
BddNode::BddNode(BddNodeInt* n, BDD_EDGE_FLAG f)
{
   assert(n != 0);
   _nodeV = size_t(n) + f;
   n->incRefCount();
}

BddNode::BddNode(size_t v) : _nodeV(v)
{
   BddNodeInt* n = getBddNodeInt();
   if (n)
      n->incRefCount();
}

// Destructor is the only place to decrease _refCount
BddNode::~BddNode()
{
   BddNodeInt* n = getBddNodeInt();
   if (n)
      n->decRefCount();
}

const BddNode&
BddNode::getLeft() const
{
   assert(getBddNodeInt() != 0);
   return getBddNodeInt()->getLeft();
}

const BddNode&
BddNode::getRight() const
{
   assert(getBddNodeInt() != 0);
   return getBddNodeInt()->getRight();
}

// [Note] i SHOULD NOT < getLevel()
BddNode
BddNode::getLeftCofactor(unsigned i) const
{
   assert(getBddNodeInt() != 0);
   if (i > getLevel()) return (*this);
   return isNegEdge()? ~(getBddNodeInt()->getLeft()) :
                         getBddNodeInt()->getLeft();
}

// [Note] i SHOULD NOT < getLevel()
BddNode
BddNode::getRightCofactor(unsigned i) const
{
   assert(getBddNodeInt() != 0);
   if (i > getLevel()) return (*this);
   return isNegEdge()? ~(getBddNodeInt()->getRight()) :
                         getBddNodeInt()->getRight();
}

unsigned
BddNode::getLevel() const
{
   return getBddNodeInt()->getLevel();
}

unsigned
BddNode::getRefCount() const
{
   return getBddNodeInt()->getRefCount();
}

BddNode&
BddNode::operator = (const BddNode& n)
{
   BddNodeInt* t = getBddNodeInt();
   if (t)
      t->decRefCount();
   _nodeV = n._nodeV;
   t = getBddNodeInt();
   if (t)
      t->incRefCount();

   return *this;
}

BddNode
BddNode::operator & (const BddNode& n) const
{
   return _BddManager->ite((*this), n, BddNode::_zero);
}

BddNode&
BddNode::operator &= (const BddNode& n)
{
   (*this) = (*this) & n;
   return (*this);
}

BddNode
BddNode::operator | (const BddNode& n) const
{
   return _BddManager->ite((*this), BddNode::_one , n);
}

BddNode&
BddNode::operator |= (const BddNode& n)
{
   (*this) = (*this) | n;
   return (*this);
}

BddNode
BddNode::operator ^ (const BddNode& n) const
{
   return _BddManager->ite((*this), ~n, n);
}

BddNode&
BddNode::operator ^= (const BddNode& n)
{
   (*this) = (*this) ^ n;
   return (*this);
}


BddNode
BddNode::xnor( const BddNode& src ) const 
{
   return _BddManager->ite( (*this), src, ~src );
}

vector< MyBDDPath >
BddNode::evaluate() const 
{
   MyBDDPath            path;
   MyBDDValue           tmpValue;
   vector< MyBDDPath >  ret,
                        left,
                        right;
   unsigned int         i;

   if ( isTerminal() ) {
      //if ( (*this) == BddNode::_one )
         path.setValue( true );
      //else
        // path.setValue( false );

      ret.push_back( path );
   }
   else {
      left = getLeft().evaluate();
      if ( getLeft().isNegEdge() ) {
         for ( i = 0; i < left.size(); ++i )
            left[i].flipValue();
      }

      tmpValue.setName( getName() );
      tmpValue.setValue( true );
      for ( i = 0; i < left.size(); ++i ) {
         left[i].addNode( tmpValue );
         ret.push_back( left[i] );
      }

      right = getRight().evaluate();
      if ( getRight().isNegEdge() ) {
         for ( i = 0; i < right.size(); ++i )
            right[i].flipValue();
      }

      tmpValue.setValue( false );
      for ( i = 0; i < right.size(); ++i ) {
         right[i].addNode( tmpValue );
         ret.push_back( right[i] );
      }
   }
   return ret;
}

/*
BddNode
BddNode:: posCoFactor(const vector<BddNode>& _supports, 
                      const BddNode& v, const int& level) const
{
   // special case 
   cout << this->getLevel() << endl;
   assert(this->getLevel() < _supports.size());
   assert(this->getLevel() >= 0);
 
   if (_supports[this->getLevel()].getBddNodeInt() == v.getBddNodeInt()) 
   //if (_supports[this->getLevel()].getBddNodeInt() == v.getBddNodeInt()) 
   {
      cout << "hit" << endl;
      if (*this == v) {
         if (this->isNegEdge() == v.isNegEdge())
            return _one;
         else
            return _zero;
      }
      else
         return *this;
   }
   //if (this -> getLevel())
   
   cout << "   " << this->getLevel() << endl;

   BddNode left = this->getLeft().posCoFactor(_supports, v);
   cout << "=====" << endl;
   BddNode right = this->getRight().posCoFactor(_supports, v);
   BddNode c = left | right;
   //BddNode c = _BddManager->ite(*this, left, right);
   
   if (this->isNegEdge())
      return ~c;
   else
      return c;  
}*/



BddNode
BddNode:: NodeCoFactor(const vector<BddNode>& _supports, int level) const
{
   cout << "level = " << level << endl;
   bool isPosCoFactor = true;
   int thisLevel = this->getLevel();
   if (level < 0)
      level = -level; isPosCoFactor = false;

   if (thisLevel == level) {
      BddNode d;
      if (isPosCoFactor == true)
         d = this->getLeft();
      else
         d = this->getRight();

      if (this->isNegEdge())
         return ~d;
      else
         return d;
   }
   else if (thisLevel < level)
      return *this;

   else {//thisLevel > level
      BddNode left = this->getLeft().NodeCoFactor(_supports, level);//postive cofactor
      BddNode right = this->getRight().NodeCoFactor(_supports, -level);//negtive cofactor
      BddNode c = (_supports[thisLevel] & left) | 
                  ((~_supports[thisLevel]) & right);
   
      if (this->isNegEdge())
         return ~c;
      else
         return c;
   }
}

void
BddNode::draw( ofstream &draw ) const {
   draw << "a_" << size_t( getBddNodeInt() ) % 100000;

   if( isNegEdge() )
      draw << " [arrowtail  = odot]";

   draw << endl;
   paint( draw );

   unsetVisitedRecur();
}

void
BddNode::paint( ofstream &draw ) const {
   BddNodeInt *n = getBddNodeInt();

   if ( isTerminal() || n->isVisited() )
      return;
   draw << "\ta_" << size_t(n) % 100000 << " -> a_";
   draw << size_t(getLeft().getBddNodeInt()) % 100000;

   if ( getLeft().isNegEdge() )
      draw << " [arrowtail  = odot]";

   draw << endl;

   draw << "\ta_" << size_t(n) % 100000 << " -> a_"
        << size_t(getRight().getBddNodeInt()) % 100000;
   if ( getRight().isNegEdge() )
      draw << " [arrowtail  = odot]";

   draw << " [style = dotted]" << endl;

#ifdef _WITH_BDD_DEBUG
   draw << "\ta_" << size_t(n) % 100000 << " [label=\"" << n->getName()
        << "\"]" << endl;

   draw << "\ta_" << size_t(getLeft().getBddNodeInt()) % 100000
        << " [label=\"" << getLeft().getName() << "\"]" << endl;

   draw << "\ta_" << size_t(getRight().getBddNodeInt()) % 100000
        << " [label=\"" << getRight().getName() << "\"]" << endl;
#else
   draw << "\ta_" << size_t(n) % 100000 << " [label=\"" << size_t(n) % 100000
        << "\"]" << endl;

   draw << "\ta_" << size_t(getLeft().getBddNodeInt()) % 100000
        << " [label=\"" << size_t(getLeft().getBddNodeInt()) % 100000
        << "\"]" << endl;

   draw << "\ta_" << size_t(getRight().getBddNodeInt()) % 100000
        << " [label=\"" << size_t(getRight().getBddNodeInt()) % 100000
        << "\"]" << endl;
#endif
   if ( !isTerminal() ) {
      n->setVisited();
      n->getLeft().paint( draw );
      n->getRight().paint( draw );
   }
}

#ifdef _WITH_BDD_DEBUG

void
BddNode::setName( const MyString &src ) {
   getBddNodeInt()->setName( src );
}

MyString
BddNode::getName() const {
   return getBddNodeInt()->getName();
}

#endif

bool
BddNode::isTerminal() const
{
   return (getBddNodeInt() == BddNodeInt::_terminal);
}

ostream&
operator << (ostream& os, const BddNode& n)
{
   n.print(os, 0);
   n.unsetVisitedRecur();
   return os;
}

void
BddNode::print(ostream& os, size_t indent) const
{
   for (size_t i = 0; i < indent; ++i)
      os << ' ';
   BddNodeInt* n = getBddNodeInt();
   os << '[' << getLevel() << "](" << (isNegEdge()? '-' : '+') << ") "
      << n << " (" << n->getRefCount() << ")";
   if (n->isVisited()) {
      os << " (*)";
      return;
   }
   n->setVisited();
   if (!isTerminal()) {
      os << endl;
      n->getLeft().print(os, indent + 2);
      os << endl;
      n->getRight().print(os, indent + 2);
   }
}

void
BddNode::unsetVisitedRecur() const
{
   BddNodeInt* n = getBddNodeInt();
   if (!n->isVisited()) return;
   n->unsetVisited();
   if (!isTerminal()) {
      n->getLeft().unsetVisitedRecur();
      n->getRight().unsetVisitedRecur();
   }
}

#ifdef _WITH_BDD_DEBUG

void
BddNodeInt::setName( const MyString &src ) {
   _name = src;
}

MyString
BddNodeInt::getName() const {
   return _name;
}

#endif



