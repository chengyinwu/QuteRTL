/******************************************************************************
  FileName    [ bv4.h ]
  PackageName [ utility/bv ]
  Synopsis    [ 4 valued bit-vector ]
  Author      [ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2005-2007 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/
#ifndef _UTIL_BV4_HEADER
#define _UTIL_BV4_HEADER

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <iomanip>
#include <math.h>
#include <sstream>

#ifndef _UTIL_BV2_HEADER
#include "bv2.h"
#endif

#include "myString.h"

using namespace std;

/* Notes:

   0: 01
   1: 10
   X: 00
   Z: 11

   A: a1a0
   B: b1b0

  +-------------+-----+--------+--------+--------+--------+
  | a1 a0 b1 b0 | A B | & AND  | | OR   | ^ XOR  | ~ NOT  |
  +-------------+-----+--------+--------+--------+--------+
  |  0  0  0  0 | X X | X [00] | X [00] | X [00] | X [00] |
  |  0  0  0  1 | X 0 | 0 [01] | X [00] | X [00] | 1 [10] |
  |  0  0  1  0 | X 1 | X [00] | 1 [10] | X [00] | 0 [01] |
  |  0  0  1  1 | X Z | X [00] | X [00] | X [00] | X [00] |
  +-------------+-----+--------+--------+--------+--------+
  |  0  1  0  0 | 0 X | 0 [01] | X [00] | X [00] |        |
  |  0  1  0  1 | 0 0 | 0 [01] | 0 [01] | 0 [01] |        |
  |  0  1  1  0 | 0 1 | 0 [01] | 1 [10] | 1 [10] |        |
  |  0  1  1  1 | 0 Z | 0 [01] | X [00] | X [00] |        |
  +-------------+-----+--------+--------+--------+        |
  |  1  0  0  0 | 1 X | X [00] | 1 [10] | X [00] |        |
  |  1  0  0  1 | 1 0 | 0 [01] | 1 [10] | 1 [10] |        |
  |  1  0  1  0 | 1 1 | 1 [10] | 1 [10] | 0 [01] |        |
  |  1  0  1  1 | 1 Z | X [00] | 1 [10] | X [00] |        |
  +-------------+-----+--------+--------+--------+        |
  |  1  1  0  0 | Z X | X [00] | X [00] | X [00] |        |
  |  1  1  0  1 | Z 0 | 0 [01] | X [00] | X [00] |        |
  |  1  1  1  0 | Z 1 | X [00] | 1 [10] | X [00] |        |
  |  1  1  1  1 | Z Z | X [00] | X [00] | X [00] |        |
  +-------------+-----+--------+--------+--------+--------+

  AND:
  c0 = a1'a0 + b1'b0
  c1 = a1a0'b1b0'

  OR:
  c0 = a1'a0b1'b0
  c1 = a1a0' + b1b0'

  XOR:
  c0 = a1'a0b1'b0 + a1a0'b1b0'
  c1 = a1'a0b1b0' + a1a0'b1'b0

  NOT:
  c0 = a1'a0 
  c1 = a1a0'

*/

enum _BV4_VALUE {
   _BV4_X, 
   _BV4_0, 
   _BV4_1,
   _BV4_Z
};

class Bv2;

class Bv4 {
public:
   friend class Bv2;
   // Constructors   
   Bv4( );
   Bv4( const string & );
   Bv4( const char * );
   Bv4( unsigned int, unsigned int = 32 );
   Bv4( const Bv2 & );
   Bv4( const Bv4 & );

   // Destructor
   ~Bv4();

   // Assignment
   Bv4& operator = ( const string & );
   Bv4& operator = ( const char * );
   Bv4& operator = ( unsigned int );
   Bv4& operator = ( const Bv2 & );
   Bv4& operator = ( const Bv4 & );

   // Bit-wise logic operations
   Bv4 operator & ( const Bv4 & ) const;
   Bv4 operator | ( const Bv4 & ) const;
   Bv4 operator ^ ( const Bv4 & ) const;
   Bv4 operator ~ () const;
   Bv4& operator &= ( const Bv4 & );
   Bv4& operator |= ( const Bv4 & );
   Bv4& operator ^= ( const Bv4 & );

   // Arithmetic operation
   Bv4 operator + ( const Bv4 & ) const;
   Bv4 operator - ( const Bv4 & ) const;
   Bv4 operator * ( const Bv4 & ) const;
   Bv4 operator / ( const Bv4 & ) const;
   Bv4 operator % ( const Bv4 & ) const;
   Bv4& operator += ( const Bv4 & );
   Bv4& operator -= ( const Bv4 & );
   Bv4& operator *= ( const Bv4 & );
   Bv4& operator /= ( const Bv4 & );
   Bv4& operator %= ( const Bv4 & );

   // Comparsion
   bool operator == ( const Bv4 & ) const;
   bool operator != ( const Bv4 & ) const;
   bool operator >  ( const Bv4 & ) const;
   bool operator >= ( const Bv4 & ) const;
   bool operator <  ( const Bv4 & ) const;
   bool operator <= ( const Bv4 & ) const;
   bool eq_xz( const Bv4 & ) const;  // ===
   bool neq_xz( const Bv4 & ) const; // !==

   // Shift operations
   Bv4 operator >> ( unsigned int ) const;
   Bv4 operator << ( unsigned int ) const;
   Bv4& operator >>= ( unsigned int );
   Bv4& operator <<= ( unsigned int );

   // Special Operation : Exist Quantification (by Cheng-Yin)
   Bv4 ExQ ( const Bv4 &, bool = false) const;
   Bv4& concat( const Bv4 & );

   // Feature member function
   unsigned int bits() const;
   bool full0() const;
   bool full1() const;
   bool fullx() const;
   bool fullz() const;
   bool has0() const;
   bool has1() const;
   bool hasx() const;
   bool hasz() const;

   void fill0();
   void fill1();
   void fillX();
   void fillZ();

   // Other operations
   Bv2 toBv2() const;
   Bv4 merge( const Bv4 & ) const;
   Bv4& append( const Bv4 & );
   Bv4 split( unsigned int, unsigned int ) const;
   unsigned int value() const;
   void set( unsigned int, _BV4_VALUE );
   void extend( unsigned int );
   _BV4_VALUE operator [] ( unsigned int ) const;

   Bv4 reducedAnd () const;
   Bv4 reducedOr () const;
   Bv4 reducedNand () const;
   Bv4 reducedNor () const;
   Bv4 reducedXor () const;
   Bv4 reducedXnor () const;

   // Conversion
   MyString str() const;
   MyString strNoSize() const;

   // Output Stream
   friend ostream& operator << ( ostream &, const Bv4 & );
private:
   unsigned int  _bits : 16;
   unsigned int  _words: 11;
   unsigned int  _mask : 5;
   unsigned int* _bit0,
               * _bit1;

   void reset();

   bool octToBin( const char *, char *, unsigned int ) const;
   bool decToBin( const char *, char *, unsigned int ) const;
   bool hexToBin( const char *, char *, unsigned int ) const;

   void fromStr( const char * );
   void fromInt( unsigned int );

   void setTo0( unsigned int, unsigned int &, unsigned int & );
   void setTo1( unsigned int, unsigned int &, unsigned int & );
   void setToX( unsigned int, unsigned int &, unsigned int & );
   void setToZ( unsigned int, unsigned int &, unsigned int & );

   _BV4_VALUE bitValue( unsigned int ) const;
   _BV4_VALUE add( _BV4_VALUE, _BV4_VALUE, _BV4_VALUE ) const;
   _BV4_VALUE carry( _BV4_VALUE, _BV4_VALUE, _BV4_VALUE ) const;
};

#endif

