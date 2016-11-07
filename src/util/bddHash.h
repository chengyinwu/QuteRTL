/****************************************************************************
  FileName     [ bddHash.h ]
  PackageName  [ ]
  Synopsis     [ Define Hash and Cache classes for BDDs ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BDD_HASH_H
#define BDD_HASH_H

#include <vector>

using namespace std;

class BddNodeInt;

//--------------------
// Define Hash classes
//--------------------
class BddHashKey
{
public:
   BddHashKey(size_t l, size_t r, unsigned i) : _l(l), _r(r), _i(i) {}

   // Get a size_t number;
   // ==> to get bucket number, need to % _numBuckers in BddHash
   size_t operator() () const { return ((_l << 8) + (_r << 16) + _i); }

   bool operator == (const BddHashKey& k) {
      return (_l == k._l) && (_r == k._r) && (_i == k._i); }

private:
   size_t     _l;
   size_t     _r;
   unsigned   _i;
};

struct BddHashNode
{
   BddHashNode(const BddHashKey& k, BddNodeInt* const n): _key(k),_node(n) {}
   BddHashKey        _key;
   BddNodeInt*       _node;
};

class BddHash
{
public:
   BddHash() : _numBuckets(0), _buckets(0) {}
   BddHash(size_t b) { init(b); }
   ~BddHash() { reset(); }

   void init(size_t b) {
      _numBuckets = b; _buckets = new vector<BddHashNode>[b]; }
   void reset() { 
      if (_numBuckets != 0) {
         _numBuckets = 0; 
         delete [] _buckets; 
      }
   }
   size_t bucketNum(const BddHashKey& k) const {
      return (k() % _numBuckets); }

   // check if k is in the hash...
   // if yes, update b & n, and return true;
   // else update b and return false;
   bool check(const BddHashKey& k, size_t& b, BddNodeInt*& n) {
      b = bucketNum(k);
      for (size_t i = 0, bn = _buckets[b].size(); i < bn; ++i)
         if (_buckets[b][i]._key == k) {
            n = _buckets[b][i]._node;
            return true;
         }
      return false;
   }
   // Need to be sure that n is not in the hash
   void forceInsert(size_t b, const BddHashNode& n) {
      _buckets[b].push_back(n); }
   // Need to be sure that n is not in the hash
   void forceInsert(const BddHashKey& k, BddNodeInt* const n) {
      _buckets[bucketNum(k)].push_back(BddHashNode(k, n)); }
//====================================================================//
   class iterator {
   public:
      iterator() : _p(0), _vp(0), _number(0), _pos(0) {}
      iterator(vector<BddHashNode>* vp, int i) : _p(0), _vp(vp), _number(0), _pos(0) {}
      iterator(BddNodeInt* p) : _p(p) {}
      iterator& operator ++ () {
         ++_pos; 
         while (_pos == _vp[_number].size()) {
            _pos = 0;
            ++_number;
         }         
         _p = _vp[_number][_pos]._node; 
         return *this; 
      }
      BddNodeInt*& operator * () { return _p; }
      iterator& operator = (const iterator& i) { _p = i._p; return *this; }
      bool operator != (const iterator& i) const { return _p != i._p; }
   private:
      BddNodeInt* _p;
      vector<BddHashNode>* _vp;
      size_t _number;
      size_t _pos;
   };
  
   class const_iterator { //use for "draw bdd"
   public:
      const_iterator() : _p(0), _vp(0), _number(0), _pos(0) {}
      const_iterator(vector<BddHashNode>* vp, int i) : _p(0), _vp(vp), _number(0), _pos(0) {}
      const_iterator(BddNodeInt* p) : _p(p) {}
      const_iterator& operator ++ () {
         ++_pos;
         while (_pos == _vp[_number].size()) {
            _pos = 0;
            ++_number;
         }
         _p = _vp[_number][_pos]._node;
         return *this;
      }
      const BddNodeInt*& operator * () { return _p; }
      const_iterator& operator = (const const_iterator& i) { _p = i._p; return *this; }
      bool operator != (const const_iterator& i) const { return _p != i._p; }
   private:
      const BddNodeInt* _p;
      const vector<BddHashNode>* _vp;
      size_t _number;
      size_t _pos;
   };

   iterator begin() const {
      if (_buckets != 0) {
         int i = 0;
         while (_buckets[i].size() == 0)
            ++i; 
         return _buckets[i][0]._node; 
      }
      else
         return 0;
   }
   iterator end() const {
      if (_buckets != 0) {
         int i = _numBuckets - 1;
         while (_buckets[i].size() == 0)
            --i;
         int pos = _buckets[i].size() - 1; 
         return _buckets[i][pos]._node; 
      }
      else
         return 0;
   }
   size_t getNumBuckets() { return _numBuckets; }
   vector<BddHashNode>* getBucketsHead() { return _buckets; }
   size_t totalNode() {
      size_t total = 0;
      for (unsigned i = 0; i < _numBuckets; ++i)
         total+=_buckets[i].size();
      return total;
   }
//====================================================================//

private:
   size_t                    _numBuckets;
   vector<BddHashNode>*      _buckets;
};


//---------------------
// Define Cache classes
//---------------------
class BddCacheKey
{
public:
   BddCacheKey() {}
   BddCacheKey(size_t f, size_t g, size_t h) : _f(f), _g(g), _h(h) {}
   
   // Get a size_t number;
   // ==> to get cache address, need to % _size in BddCache
   //size_t operator() () const { return ((_f << 8)+(_g << 16)+(_h << 24)); }
   size_t operator() () const { return ((_f)+(_g << 8)+(_h << 16)); }
   
   bool operator == (const BddCacheKey& k) {
      return (_f == k._f) && (_g == k._g) && (_h == k._h); }
      
private:
   size_t       _f;
   size_t       _g;
   size_t       _h;
}; 

struct BddCacheNode
{
   BddCacheNode() {}
   BddCacheNode(const BddCacheKey& k, size_t n) : _key(k), _node(n) {}

   BddCacheKey       _key;
   size_t            _node;
}; 

class BddCache
{
public:
   BddCache() : _size(0), _cache(0) {}
   BddCache(size_t s) { init(s); }
   ~BddCache() { reset(); }

   void init(size_t s) { _size = s; _cache = new BddCacheNode[s]; }
   size_t sizeNum(const BddCacheKey& k) const {
      return (k() % _size); }
   // Need to be sure that n is not in the cache
   void forceInsert(size_t b, const BddCacheNode& n) { _cache[b] = n; }
   // Need to be sure that n is not in the cache
   void forceInsert(const BddCacheKey& k, size_t const n) {
      _cache[sizeNum(k)] = BddCacheNode(k, n); }

   bool check(const BddCacheKey& k, size_t& b, size_t& n) {
      b = sizeNum(k);
      if (_cache[b]._key == k) {
         n = _cache[b]._node;
         return true;
      }
      return false;
   }
   void reset() { 
      if (_size != 0) {
         _size = 0; 
         delete [] _cache; 
      }
   }
   size_t size() { return _size; }
private:
   size_t            _size;
   BddCacheNode*     _cache;
};


#endif // BDD_HASH_H
