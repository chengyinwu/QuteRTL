/****************************************************************************
  FileName     [ myBitVec.h ]
  PackageName  [ util ]
  Synopsis     [ Bit Vector data structure implementation. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2010 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _MY_BIT_VEC_HEADER
#define _MY_BIT_VEC_HEADER

#include <iostream>
#include <cassert>

using namespace std;

const unsigned char MOD_WORD = 0x07;
const unsigned char IDX_MASK[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

inline unsigned words(const unsigned s) {
   unsigned ws = (s >> 3);
   if (MOD_WORD & s) ++ws;
   return ws;
}

class BitVec
{
   public : 
      // Constructors and Destructor
      BitVec(const unsigned s = 8) { set(s, true); }
      BitVec(const BitVec& b) { set(b._size); memcpy(_data, b._data, sizeof(unsigned char) * words(_size)); }
      ~BitVec() { if (_data) delete[] _data; }
      // Member Functions
      inline const unsigned size() const { return _size; }
      inline void set1 (const unsigned i) { _data[i >> 3] |= IDX_MASK[MOD_WORD & i]; }
      inline void set0 (const unsigned i) { _data[i >> 3] &= ~IDX_MASK[MOD_WORD & i]; }
      inline bool all1 () const {
         for (unsigned i = 0; i < _size; ++i) { if (!(*this)[i]) return false; } return true;
      }
      inline bool all0 () const {
         for (unsigned i = 0, j = words(_size); i < j; ++i) { if (_data[i]) return false; } return true;
      }
      inline const unsigned ones() const {
         unsigned j = 0; for (unsigned i = 0; i < _size; ++i) if ((*this)[i]) ++j; return j;
      }
      inline void resize(const unsigned s, bool init = true) { delete[] _data; set(s, init); }
      // Operator Overloads
      inline bool operator[] (const unsigned i) const { return (_data[i >> 3] & IDX_MASK[MOD_WORD & i]); }
      inline BitVec& operator &= (const BitVec& b) {
         for (unsigned i = 0, j = words(_size); i < j; ++i) _data[i] &= b._data[i]; return *this;
      }
      inline BitVec& operator |= (const BitVec& b) {
         for (unsigned i = 0, j = words(_size); i < j; ++i) _data[i] |= b._data[i]; return *this;
      }
      inline BitVec& operator =  (const BitVec& b) {
         resize(b._size, false); memcpy(_data, b._data, sizeof(unsigned char) * words(_size)); return *this;
      }
      friend ostream& operator << (ostream& os, const BitVec& b) {
         unsigned i = b._size; os << b._size << "'b "; while (i--) { os << b[i]; if (i % 8 == 0) os << "_"; } return os;
      }
   private : 
      inline void set(const unsigned s, bool init = false) {
         unsigned i = words(_size = s);
         _data = new unsigned char[i];
         if (init) while (i--) { _data[i] = 0; }
      }
      unsigned          _size;
      unsigned char*    _data;
};

// BitVec with Don't Care Supports (00 for X, 01 for 0, 10 for 1, 11 illegal)
class BitVecX
{
   public : 
      // Constructors and Destructors
      BitVecX(const unsigned s = 8) { set(s, true); }
      BitVecX(const BitVecX& b) {
         set(b._size); unsigned s = sizeof(unsigned char) * words(_size);
         memcpy(_data0, b._data0, s); memcpy(_data1, b._data1, s);
      }
      ~BitVecX() { if (_data0) delete[] _data0; if (_data1) delete[] _data1; }
      // Inline Member Functions
      inline const unsigned size() const { return _size; }
      inline void set1 (const unsigned i) {
         _data0[i >> 3] &= ~IDX_MASK[MOD_WORD & i]; _data1[i >> 3] |=  IDX_MASK[MOD_WORD & i];
         assert ('1' == (*this)[i]);
      }
      inline void set0 (const unsigned i) {
         _data0[i >> 3] |=  IDX_MASK[MOD_WORD & i]; _data1[i >> 3] &= ~IDX_MASK[MOD_WORD & i];
         assert ('0' == (*this)[i]);
      }
      inline void setX (const unsigned i) {
         _data0[i >> 3] &= ~IDX_MASK[MOD_WORD & i]; _data1[i >> 3] &= ~IDX_MASK[MOD_WORD & i];
         assert ('X' == (*this)[i]);
      }
      inline bool all1 () const {
         for (unsigned i = 0; i < _size; ++i) if ('1' != (*this)[i]) return false; return true;
      }
      inline bool all0 () const {
         for (unsigned i = 0; i < _size; ++i) if ('0' != (*this)[i]) return false; return true;
      }
      inline void resize(const unsigned s, bool init = true) { delete[] _data0; delete[] _data1; set(s, init); }
      // Operator Overloads
      inline char operator[] (const unsigned i) const {
         if (_data0[i >> 3] & IDX_MASK[MOD_WORD & i]) return '0';
         else if (_data1[i >> 3] & IDX_MASK[MOD_WORD & i]) return '1';
         else return 'X';
      }
      inline BitVecX& operator &= (const BitVecX& b) {
         for (unsigned i = 0, j = words(_size); i < j; ++i) { _data0[i] |= b._data0[i]; _data1[i] &= b._data1[i]; }
         return *this;
      }
      inline BitVecX& operator |= (const BitVecX& b) {
         for (unsigned i = 0, j = words(_size); i < j; ++i) { _data0[i] &= b._data0[i]; _data1[i] |= b._data1[i]; }
         return *this;
      }
      inline BitVecX& operator =  (const BitVecX& b) {
         resize(b._size, false); unsigned s = sizeof(unsigned char) * words(_size);
         memcpy(_data0, b._data0, s); memcpy(_data1, b._data1, s); return *this;
      }
      friend ostream& operator << (ostream& os, const BitVecX& b) {
         unsigned i = b._size; os << b._size << "'b ";
         while (i--) { os << b[i]; if (i % 8 == 0) os << "_"; } return os;
      }
   private : 
      inline void set(const unsigned s, bool init = false) {
         unsigned i = words(_size = s);
         _data0 = new unsigned char[i]; _data1 = new unsigned char[i];
         if (init) while (i--) { _data0[i] = _data1[i] = 0; }
      }
      unsigned          _size;
      unsigned char*    _data0;
      unsigned char*    _data1;
};

#endif

