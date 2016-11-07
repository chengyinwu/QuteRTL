/******************************************************************************
  FileName    [ bv2.h ]
  PackageName [ utility/bv ]
  Synopsis    [ 2 valued bit-vector ]
  Author      [ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2005-2007 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/
#ifndef _UTIL_BV2_HEADER
#define _UTIL_BV2_HEADER

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctype.h>

#ifndef _UTIL_BV4_HEADER
#include "bv4.h"
#endif

#include "myString.h"

using namespace std;

enum _BV2_VALUE {
   _BV2_0,
   _BV2_1
};

class Bv4;

class Bv2 {
public:
   friend class Bv4;
   // Constructor
   Bv2();
   Bv2( const string & );
   Bv2( const char * );
   Bv2( unsigned int );
   Bv2( const Bv2 & );
   Bv2( const Bv4 & );

   // Destructor
   ~Bv2();

   // Assignment
   Bv2& operator = ( const string & );
   Bv2& operator = ( const char * );
   Bv2& operator = ( unsigned int );
   Bv2& operator = ( const Bv2 & );
   Bv2& operator = ( const Bv4 & );

   // Bit-wise logic operations
   Bv2 operator & ( const Bv2 & ) const;
   Bv2 operator | ( const Bv2 & ) const;
   Bv2 operator ^ ( const Bv2 & ) const;
   Bv2 operator ~ () const;
   Bv2& operator &= ( const Bv2 & );
   Bv2& operator |= ( const Bv2 & );
   Bv2& operator ^= ( const Bv2 & );

   // Arithmetic operation
   Bv2 operator + ( const Bv2 & ) const;
   Bv2 operator - ( const Bv2 & ) const;
   Bv2 operator * ( const Bv2 & ) const;
   Bv2 operator / ( const Bv2 & ) const;
   Bv2 operator % ( const Bv2 & ) const;
   Bv2& operator += ( const Bv2 & );
   Bv2& operator -= ( const Bv2 & );
   Bv2& operator *= ( const Bv2 & );
   Bv2& operator /= ( const Bv2 & );
   Bv2& operator %= ( const Bv2 & );

   // Comparsion
   bool operator == ( const Bv2 & ) const;
   bool operator != ( const Bv2 & ) const;
   bool operator >  ( const Bv2 & ) const;
   bool operator >= ( const Bv2 & ) const;
   bool operator <  ( const Bv2 & ) const;
   bool operator <= ( const Bv2 & ) const;
   
   // Shift operation
   Bv2 operator >> ( unsigned int ) const;
   Bv2 operator << ( unsigned int ) const;
   Bv2& operator >>= ( unsigned int );
   Bv2& operator <<= ( unsigned int );

   // Feature member function
   unsigned int bits() const;

   // Other operations
   Bv4 toBv4() const;
   Bv2 merge( const Bv2 & ) const;
   Bv2& append( const Bv2 & );
   unsigned int value() const;
   void set( unsigned int, _BV2_VALUE );
   _BV2_VALUE operator [] ( unsigned int ) const;

   // Conversion
   MyString str() const;
   MyString strNoSize() const;

   // Output Stream
   friend ostream& operator << ( ostream &, const Bv2 & );
private:
   unsigned int  _bits : 16;
   unsigned int  _words: 11;
   unsigned int  _mask : 5;
   unsigned int* _bit;

   void reset();

   bool octToBin( const char *, char *, unsigned int ) const;
   bool decToBin( const char *, char *, unsigned int ) const;
   bool hexToBin( const char *, char *, unsigned int ) const;

   void fromStr( const char * );
   void fromInt( unsigned int );

   void setTo0( unsigned int, unsigned int & );
   void setTo1( unsigned int, unsigned int & );

   void fill0();
   void fill1();

   void extend( unsigned int );

   _BV2_VALUE bitValue( unsigned int ) const;
   _BV2_VALUE add( _BV2_VALUE, _BV2_VALUE, _BV2_VALUE ) const;
   _BV2_VALUE carry( _BV2_VALUE, _BV2_VALUE, _BV2_VALUE ) const;
};

#endif

