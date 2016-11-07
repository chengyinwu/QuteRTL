/****************************************************************************
  FileName     [ bddNode.h ]
  PackageName  [ ]
  Synopsis     [ Define basic BDD Node data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BDD_NODE_H
#define BDD_NODE_H

#include <climits>
#include <vector>
#include <iostream>
#include <fstream>
//#include "memMgr.h"


#ifdef _WITH_BDD_DEBUG
#include "myString.h"
#endif

#include "bddManager.h"
#include "myTruthT.h"

using namespace std;

#define BDD_EDGE_BITS      2
#define BDD_NODE_PTR_MASK  ((UINT_MAX >> BDD_EDGE_BITS) << BDD_EDGE_BITS)

class BddManager;
class BddNodeInt;
class MyBDDValue;
class MyBDDPath;

enum BDD_EDGE_FLAG
{
   BDD_POS_EDGE = 0,
   BDD_NEG_EDGE = 1,

   BDD_EDGE_DUMMY  // dummy end
};

class BddNode
{
public:
   static BddNode _one;
   static BddNode _zero;
   static BddNode _faild;
   static BddNode _error;

   // no node association yet
   BddNode() : _nodeV(0) {}
   // We check the hash when a new node is possibly being created
   BddNode(size_t l, size_t r, size_t i, BDD_EDGE_FLAG f = BDD_POS_EDGE);
   // Copy constructor also needs to increase the _refCount
   BddNode(const BddNode& n);
   // n must have been uniquified...
   BddNode(BddNodeInt* n, BDD_EDGE_FLAG f = BDD_POS_EDGE);
   // Used in const 0 and const 1
   BddNode(size_t v);
   // Destructor is the only place to decrease _refCount
   ~BddNode();

   // Basic access functions
   const BddNode& getLeft() const;
   const BddNode& getRight() const;
   BddNode getLeftCofactor(unsigned i) const;
   BddNode getRightCofactor(unsigned i) const;
   unsigned getLevel() const;
   unsigned getRefCount() const;

   // Operators overloading
   size_t   operator () () { return _nodeV; }
   BddNode  operator ~  () const { return (_nodeV ^ BDD_NEG_EDGE); }
   BddNode& operator =  (const BddNode& n);
   BddNode  operator &  (const BddNode& n) const;
   BddNode& operator &= (const BddNode& n);
   BddNode  operator |  (const BddNode& n) const;
   BddNode& operator |= (const BddNode& n);
   BddNode  operator ^  (const BddNode& n) const;
   BddNode& operator ^= (const BddNode& n);
   bool     operator == (const BddNode& n) const { return (_nodeV == n._nodeV); }
   bool     operator != (const BddNode& n) const { return (_nodeV != n._nodeV); }
   BddNode  xnor( const BddNode& ) const;

   vector< MyBDDPath > evaluate() const;
   //BddNode posCoFactor(const vector<BddNode>&, 
   //                    const BddNode& v, const int& level) const;
   BddNode NodeCoFactor(const vector<BddNode>&, int level) const;

   void toNegEdge() { _nodeV = (_nodeV & BDD_NODE_PTR_MASK) | BDD_NEG_EDGE; }
   void toPosEdge() { _nodeV = (_nodeV & BDD_NODE_PTR_MASK) | BDD_POS_EDGE; }
   friend void swap(BddNode& a, BddNode& b) {
      BddNode tmp(a);
      a = b; 
      b = tmp;
   }
   BddNodeInt* getBddNodeInt() const {
      return (BddNodeInt*)(_nodeV & BDD_NODE_PTR_MASK); }
   bool isNegEdge() const { return (_nodeV & BDD_NEG_EDGE); }
   bool isPosEdge() const { return !isNegEdge(); }
   /*void dfsDraw(ostream& outfile, BddNode& p) const {                   
      outfile << "\t{ rank = same; \"" << this->getLevel() << "\"; }" << endl;
      ostream::outfile << "\t\"" << size_t(p.getBddNodeInt()) << "\"->\"" 
                               << size_t(this->getBddNodeInt());

      if (*this == p.getLeft())
         std::outfile << "\" [ style = filled, ";
      else
         std::outfile << "\" [ style = dotted, ";
      
      if (this->isNegEdge() == true)
         std::outfile << "arrowhead = odot ];" << endl;
      else
         std::outfile << "normal ];" << endl;

      if (this->getBddNodeInt() ==  BddNodeInt::_terminal)
         return;
      else {
         this->getLeft().dfsDraw(outfile, *this);
         this->getRight().dfsDraw(outfile, *this);   
      }                            
   }*/


   void draw( ofstream & ) const;
#ifdef _WITH_BDD_DEBUG
   void setName( const MyString & );
   MyString getName() const;
#endif

   friend ostream& operator << (ostream& os, const BddNode& n);
   //friend void operator << (stringstream& st, const string& str);

   // Static functions
   static void setBddManager(BddManager* m) { _BddManager = m; }
   static void resetTerminal() { 
      _one._nodeV   = _zero._nodeV  = 0; 
      _faild._nodeV = _error._nodeV = 0;
   }

private:
   size_t                  _nodeV;//BddNodeInt and edge bubble

   // Static data mebers
   static BddManager*      _BddManager;

   // Private functionsa
   void paint( ofstream & ) const;
   bool isTerminal() const;
   void print(ostream&, size_t) const;
   void unsetVisitedRecur() const;
};

// Private class
class BddNodeInt
{
#define NA  1024

#ifdef MEM_MGR_H
   //DEFINE_MEM_MGR(BddNodeInt, NA);  Chengyin debug : Close MemMgr
#endif // MEM_MGR_H


   friend class BddNode;
   friend class BddManager;

   // For const 1 and const 0
   BddNodeInt() : _level(0), _refCount(0) {}

   // Don't initialize _refCount here...
   // BddNode() will call incRefCount() or decRefCount() instead...
   BddNodeInt(size_t l, size_t r, unsigned ll)
   : _left(l), _right(r), _level(ll), _refCount(0) {}

   const BddNode& getLeft() const { return _left; }
   const BddNode& getRight() const { return _right; }
   unsigned getLevel() const { return _level; }
   unsigned getRefCount() const { return _refCount; }
   void incRefCount() { ++_refCount; }
   void decRefCount() { --_refCount; }
   bool isVisited() const { return (_visited == 1); }
   void setVisited() { _visited = 1; }
   void unsetVisited() { _visited = 0; }
#ifdef _WITH_BDD_DEBUG
   void setName( const MyString & );
   MyString getName() const;

   MyString            _name;
#endif

   BddNode              _left;
   BddNode              _right;
   unsigned             _level    : 16;
   unsigned             _refCount : 15;
   unsigned             _visited  : 1;

   static BddNodeInt*   _terminal;
   static BddNodeInt*   _faild;
};

#endif // BDD_NODE_H
