/******************************************************************************
  PackageName [ utility/bv ]
  Synopsis    [ 4 valued bit-vector ]
  Author      [ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2005-2007 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/
#ifndef _UTIL_BV4_SOURCE
#define _UTIL_BV4_SOURCE

#include <cassert>
#include "bv4.h"
#include "util.h"

// Constructor
Bv4::Bv4( ) {
   _bits  = 0;
   _mask  = 0;
   _words = 0;
   _bit0  = NULL;
   _bit1  = NULL;
}

Bv4::Bv4( const string &src ) {
   fromStr( src.c_str() );
}

Bv4::Bv4( const char *src ) {
   fromStr( src );
}

Bv4::Bv4( unsigned int src, unsigned int bits ) {
   if (bits == 32) fromInt( src );
   else {
      string value = myInt2Str(bits) + "'d" + myInt2Str(src);
      fromStr( value.c_str() );
   }
}

Bv4::Bv4( const Bv2 &src ) {
   unsigned int i,
                wordIndex;

   _bits  = src._bits;
   _words = src._words;
   _mask  = src._mask;
   _bit0  = new unsigned int[ _words ];
   _bit1  = new unsigned int[ _words ];

   for ( i = 0; i < _bits; ++i ) {
      wordIndex = i / 32;
      if ( src[i] == _BV2_1 )
         setTo1( i % 32, _bit0[ wordIndex ], _bit1[ wordIndex ] );
      else
         setTo0( i % 32, _bit0[ wordIndex ], _bit1[ wordIndex ] );
   }
}

Bv4::Bv4( const Bv4 &src ) {
   unsigned int i;
   _bits  = src._bits;
   _words = src._words;
   _mask  = src._mask;
   _bit0  = new unsigned int[ _words ];
   _bit1  = new unsigned int[ _words ];

   for ( i = 0; i < _words; ++i ) {
      _bit0[i] = src._bit0[i];
      _bit1[i] = src._bit1[i];
   }
}

// Destructor
Bv4::~Bv4() {
   reset();
}

// Assignment
Bv4& 
Bv4::operator = ( const string &src ) {
   reset();
   fromStr( src.c_str() );

   return *this;
}

Bv4& 
Bv4::operator = ( const char *src ) {
   reset();
   fromStr( src );

   return *this;
}

Bv4& 
Bv4::operator = ( unsigned int src ) {
   reset();
   fromInt( src );

   return *this;
}

Bv4& 
Bv4::operator = ( const Bv2 &src ) {
   unsigned int i,
                wordIndex;
   reset();

   _bits  = src._bits;
   _words = src._words;
   _mask  = src._mask;
   _bit0  = new unsigned int[ _words ];
   _bit1  = new unsigned int[ _words ];

   for ( i = 0; i < _bits; ++i ) {
      wordIndex = i / 32;
      if ( src[i] == _BV2_1 )
         setTo1( i % 32, _bit0[ wordIndex ], _bit1[ wordIndex ] );
      else
         setTo0( i % 32, _bit0[ wordIndex ], _bit1[ wordIndex ] );
   }

   return *this;
}

Bv4& 
Bv4::operator = ( const Bv4 &src ) {
   unsigned int i;

   if ( this == &src ) // self assign check
      return *this;

   reset();

   _bits  = src._bits; 
   _words = src._words;
   _mask  = src._mask;
   _bit0 = new unsigned int[ _words ];
   _bit1 = new unsigned int[ _words ];

   for ( i = 0; i < _words; ++i ) {
      _bit0[i] = src._bit0[i];
      _bit1[i] = src._bit1[i];
   }

   return *this;
}

// Bit-wise logic operations
Bv4 
Bv4::operator & ( const Bv4 &opn ) const {
// c0 = a1'a0 + b1'b0
// c1 = a1a0'b1b0'
   Bv4          opn1,
                opn2;

   unsigned int i,
                a0, a1, b0, b1;

   opn1 = *this;
   opn2 = opn;

   if ( bits() > opn2.bits() ) 
      opn2.extend( opn1.bits() );
   else 
      opn1.extend( opn2.bits() );

   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit0[i];
      a1 = opn1._bit1[i];
      b0 = opn2._bit0[i];
      b1 = opn2._bit1[i];
   
      opn1._bit0[i] = ( ~a1 & a0 ) | ( ~b1 & b0 );
      opn1._bit1[i] = ( a1 & ~a0 & b1 & ~b0 );
   }

   if ( _mask > 0 ) {
     for ( i = opn1._mask + 1; i < 32; ++i ) 
        opn1.set( opn1.bits() + i, _BV4_0 );
   }

   return opn1;
}

Bv4 
Bv4::operator | ( const Bv4 &opn ) const {
// c0 = a1'a0b1'b0
// c1 = a1a0' + b1b0'
   Bv4          opn1,
                opn2;

   unsigned int i,
                a0, a1, b0, b1;

   opn1 = *this;
   opn2 = opn;

   if ( bits() > opn2.bits() ) 
      opn2.extend( opn1.bits() );
   else 
      opn1.extend( opn2.bits() );

   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit0[i];
      a1 = opn1._bit1[i];
      b0 = opn2._bit0[i];
      b1 = opn2._bit1[i];
   
      opn1._bit0[i] = ~a1 & a0 & ~b1 & b0;
      opn1._bit1[i] = ( a1 & ~a0 ) | ( b1 & ~b0 );
   }

   if ( _mask > 0 ) {
     for ( i = opn1._mask + 1; i < 32; ++i )
        opn1.set( opn1.bits() + i, _BV4_0 );
   }

   return opn1;
}

Bv4 
Bv4::operator ^ ( const Bv4 &opn ) const {
// c0 = a1'a0b1'b0 + a1a0'b1b0'
// c1 = a1'a0b1b0' + a1a0'b1'b0
   Bv4          opn1,
                opn2;

   unsigned int i,
                a0, a1, b0, b1;

   opn1 = *this;
   opn2 = opn;

   if ( bits() > opn2.bits() ) 
      opn2.extend( opn1.bits() );
   else 
      opn1.extend( opn2.bits() );

   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit0[i];
      a1 = opn1._bit1[i];
      b0 = opn2._bit0[i];
      b1 = opn2._bit1[i];
   
      opn1._bit0[i] = ( ~a1 & a0 & ~b1 & b0 ) | ( a1 & ~a0 & b1 & ~b0 );
      opn1._bit1[i] = ( ~a1 & a0 & b1 & ~b0 ) | ( a1 & ~a0 & ~b1 & b0 );
   }

   if ( _mask > 0 ) {
     for ( i = opn1._mask + 1; i < 32; ++i )
        opn1.set( opn1.bits() + i, _BV4_0 );
   }

   return opn1;
}

Bv4 
Bv4::operator ~ () const {
// c0 = a1a0'
// c1 = a1'a0
   Bv4          opn1;

   unsigned int i,
                a0, a1;

   opn1 = *this;
   
   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit0[i];
      a1 = opn1._bit1[i];

      opn1._bit0[i] = a1 & ~a0;
      opn1._bit1[i] = ~a1 & a0;
   }

   for ( i = opn1._mask + 1; i <= 32; ++i ) 
      opn1.set( opn1.bits() + i, _BV4_0 );

   return opn1;
}

Bv4& 
Bv4::operator &= ( const Bv4 &opn ) {
   *this = *this & opn;

   return *this;
}

Bv4& 
Bv4::operator |= ( const Bv4 &opn ) {
   *this = *this | opn;

   return *this;
}

Bv4& 
Bv4::operator ^= ( const Bv4 &opn ) {
   *this = *this ^ opn;

   return *this;
}

// Arithmetic operation
Bv4 
Bv4::operator + ( const Bv4 &opn ) const {
   Bv4          opn1,
                opn2,
                opn3;

   _BV4_VALUE   vA,
                vB, 
                vC1,
                vC2;
 
   unsigned int i;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   if ( opn1.hasx() || opn1.hasz() || opn2.hasx() || opn2.hasz() ) {
      opn1.fillX();
      return opn1;
   }

   opn3 = opn1;
   opn3.extend( opn3.bits() + 1 );
   
   vC1 = _BV4_0;
   vC2 = _BV4_0;
   for ( i = 0; i < opn1.bits(); ++i ) {
      vA = opn1[i];
      vB = opn2[i];
      
      opn3.set( i, add( vA, vB, vC1 ) );
      vC2 = vC1;
      vC1 = carry( vA, vB, vC1 );
   }

   if ( vC1 == _BV4_1 && vC2 == _BV4_1 ) {
      opn3._bits--;
      opn3._mask = opn3._bits % 32;
   }
   else {
      if ( vC1 == _BV4_1 ) {
         opn3.set( i, _BV4_1 );
      }
      else {
         opn3._bits--;
         opn3._mask = opn3._bits % 32;
      }
   }

   return opn3;
}

Bv4 
Bv4::operator - ( const Bv4 &opn ) const {
   Bv4          opn1,
                opn2,
                step("1'b1");

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   if ( opn1.hasx() || opn1.hasz() || opn2.hasx() || opn2.hasz() ) {
      opn1.fillX();
      return opn1;
   }

   opn1 = opn1 + (~opn2);
   opn1 = opn1 + step;

   if ( opn1.bits() > bits() ) {
      opn1._bits--;
      opn1.set( opn1._bits, _BV4_0 );
   }

   return opn1;
}

Bv4 
Bv4::operator * ( const Bv4 &opn ) const {
   Bv4          opn1,
                opn2,
                opn3;
   unsigned int i;

   opn1 = *this;
   opn2 = opn;

   if ( opn1.hasx() || opn1.hasz() || opn2.hasx() || opn2.hasz() ) {
      opn1.fillX();
      return opn1;
   }

   opn1.extend( opn1.bits() + opn2.bits() );
   opn3 = opn1;
   opn3.fill0();

   for ( i = 0; i < opn2.bits(); ++i ) {
      if ( opn2[i] == _BV4_1 )
         opn3 = opn3 + opn1;

      opn1 = opn1 << 1;
   }

   return opn3;
}

Bv4 
Bv4::operator / ( const Bv4 &opn ) const {
   Bv4          opn1,
                opn2,
                opn3;
   int          len;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   if ( opn1.hasx() || opn1.hasz() || opn2.hasx() || opn2.hasz() ) {
      opn1.fillX();
      return opn1;
   }
 
   if ( opn1 < opn2 ) {
      opn3 = "1'b0";
      return opn3;
   }

   opn3 = "1'b0";
   if ( opn2 == opn3 ) {
      opn3.fill1();
      return opn3;
   }

   len = (int)opn2.bits() - 1;
   while ( opn2[ len ] != _BV4_1 )
      len--;

   len = (int)opn2.bits() - len - 1;
   opn2 = opn2 << len;

   opn3.extend( opn1.bits() );
   opn3.fill0();

   while ( len >= 0 ) {
      if ( opn1 >= opn2 ) {
         opn1 = opn1 - opn2;
         opn3.set( 0, _BV4_1 );
      }
      opn2 = opn2 >> 1;
      opn3 = opn3 << 1;
      len--;
   }

   opn3 = opn3 >> 1;

   return opn3;
}


Bv4 
Bv4::operator % ( const Bv4 &opn ) const {
   Bv4          opn1,
                opn2,
                opn3;
   int          len;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   if ( opn1.hasx() || opn1.hasz() || opn2.hasx() || opn2.hasz() ) {
      opn1.fillX();
      return opn1;
   }

   if ( opn1 < opn2 ) {
      opn3 = "1'b0";
      return opn3;
   }

   opn3 = "1'b0";
   if ( opn2 == opn3 ) {
      opn3.fill1();
      return opn3;
   }

   len = (int)opn2.bits() - 1;
   while ( opn2[ len ] != _BV4_1 )
      len--;

   len = (int)opn2.bits() - len - 1;
   opn2 = opn2 << len;

   opn3.extend( opn1.bits() );
   opn3.fill0();

   while ( len >= 0 ) {
      if ( opn1 >= opn2 ) {
         opn1 = opn1 - opn2;
         opn3.set( 0, _BV4_1 );
      }
      opn2 = opn2 >> 1;
      opn3 = opn3 << 1;
      len--;
   }

   opn3 = opn3 >> 1;

   return opn1;
}

Bv4&
Bv4::operator += ( const Bv4 &opn ) {
   *this = *this + opn;

   return *this;
}

Bv4&
Bv4::operator -= ( const Bv4 &opn ) {
   *this = *this - opn;

   return *this;
}

Bv4&
Bv4::operator *= ( const Bv4 &opn ) {
   *this = *this * opn;
 
   return *this;
}

Bv4&
Bv4::operator /= ( const Bv4 &opn ) {
   *this = *this / opn;

   return *this;
}

Bv4&
Bv4::operator %= ( const Bv4 &opn ) {
   *this = *this % opn;

   return *this;
}

// Comparsion
bool 
Bv4::operator == ( const Bv4 &opn ) const {
   unsigned int i;

   Bv4          opn1, 
                opn2;

   if ( hasx() || hasz() || opn.hasx() || opn.hasz() )
      return false;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = 0; i < _words; ++i )
      if ( opn1._bit0[i] != opn2._bit0[i] || opn1._bit1[i] != opn2._bit1[i] )
         return false;

   return true;
}

bool 
Bv4::operator != ( const Bv4 &opn ) const {
   unsigned int i;

   Bv4          opn1,
                opn2;

   if ( hasx() || hasz() || opn.hasx() || opn.hasz() )
      return false;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = 0; i < _words; ++i )
      if ( opn1._bit0[i] == opn2._bit0[i] && opn1._bit1[i] == opn2._bit1[i] )
         return false;

   return true;
}


bool 
Bv4::operator >  ( const Bv4 &opn ) const {
   int i;

   Bv4          opn1,
                opn2;

   if ( hasx() || hasz() || opn.hasx() || opn.hasz() ) 
      return false;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV4_1 && opn2[i] == _BV4_0 )
         return true;
      if ( opn1[i] == _BV4_0 && opn2[i] == _BV4_1 )
         return false;
   }

   return false;
}

bool 
Bv4::operator >= ( const Bv4 &opn ) const {
   int i;

   Bv4          opn1,
                opn2;

   if ( hasx() || hasz() || opn.hasx() || opn.hasz() )
      return false;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV4_1 && opn2[i] == _BV4_0 )
         return true;
      if ( opn1[i] == _BV4_0 && opn2[i] == _BV4_1 )
         return false;
   }

   return true;
}

bool 
Bv4::operator <  ( const Bv4 &opn ) const {
   int i;

   Bv4          opn1,
                opn2;

   if ( hasx() || hasz() || opn.hasx() || opn.hasz() )
      return false;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV4_1 && opn2[i] == _BV4_0 )
         return false;
      if ( opn1[i] == _BV4_0 && opn2[i] == _BV4_1 )
         return true;
   }

   return false;
}

bool 
Bv4::operator <= ( const Bv4 &opn ) const {
   int i;

   Bv4          opn1,
                opn2;

   if ( hasx() || hasz() || opn.hasx() || opn.hasz() )
      return false;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV4_1 && opn2[i] == _BV4_0 )
         return false;
      if ( opn1[i] == _BV4_0 && opn2[i] == _BV4_1 )
         return true;
   }

   return true;
}

bool 
Bv4::eq_xz( const Bv4 &opn ) const {
   unsigned int i;

   Bv4          opn1,
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = 0; i < _words; ++i )
      if ( opn1._bit0[i] != opn2._bit0[i] || opn1._bit1[i] != opn2._bit1[i] )
         return false;

   return true;
}

bool 
Bv4::neq_xz( const Bv4 &opn ) const {
   unsigned int i;

   Bv4          opn1,
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = 0; i < _words; ++i )
      if ( opn1._bit0[i] == opn2._bit0[i] && opn1._bit1[i] == opn2._bit1[i] )
         return false;

   return true;
}

// Shift operations
Bv4 
Bv4::operator >> ( unsigned int bit ) const {
   unsigned int i;
   Bv4          opn = *this;

   if ( bit < _bits ) {
      for ( i = 0; i < _bits - bit; ++i )
         opn.set( i, opn[ i + bit ] );

      for ( i = _bits - bit; i < _bits; ++i )
         opn.set( i, _BV4_0 );
   }
   else {
      opn.fill0();
   }

   return opn;
}

Bv4&
Bv4::operator >>= ( unsigned int bit ) {
   *this = *this >> bit;
   
   return *this;
}

Bv4 
Bv4::operator << ( unsigned int bit ) const {
   unsigned int i;
   Bv4          opn = *this;

   if ( bit < _bits ) {
      for ( i = _bits - 1; i >= bit; --i ) 
         opn.set( i, opn[ i - bit ] );

      for ( i = 0; i < bit; ++i )
        opn.set( i, _BV4_0 );
   }
   else {
      opn.fill0();
   }

   return opn;
}

Bv4&
Bv4::operator <<= ( unsigned int bit ) {
   *this = *this << bit;

   return *this;
}

Bv4& 
Bv4::concat( const Bv4 &opn ) {
   unsigned int i, j = bits();

   extend( bits() + opn.bits() );

   for ( i = 0; i < opn.bits(); ++i )
      set( j + i, opn[i] );

   return *this;
}

Bv4
Bv4::ExQ ( const Bv4& b, bool invert) const {
   assert (bits() == b.bits());
   Bv4          opn1,
                opn2;

   unsigned int i,
                a0, a1, b0, b1;

   opn1 = *this;
   opn2 = b;

   assert (opn1.bits() == opn2.bits());

   if (invert) {
      for (i = 0; i < opn2._words; ++i) {
         a0 = opn2._bit0[i];
         a1 = opn2._bit1[i];
         opn2._bit0[i] = a1;
         opn2._bit1[i] = a0;
      }
   }

   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit0[i];
      a1 = opn1._bit1[i];
      b0 = opn2._bit0[i];
      b1 = opn2._bit1[i];
   
      opn1._bit0[i] = a0 & b0;
      opn1._bit1[i] = a1 & b1;
   }

   if ( _mask > 0 ) {
     for ( i = opn1._mask + 1; i < 32; ++i ) 
        opn1.set( opn1.bits() + i, _BV4_X );
   }

   return opn1;
}

// Feature member function
unsigned int 
Bv4::bits() const {
   return _bits;
}

bool
Bv4::full0() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i )
      if ( bitValue(i) != _BV4_0 )
         return false;

   return true;
}

bool
Bv4::full1() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i )
      if ( bitValue(i) != _BV4_1 )
         return false;

   return true;
}

bool
Bv4::fullx() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i )
      if ( bitValue(i) != _BV4_X )
         return false;

   return true;
}

bool
Bv4::fullz() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i )
      if ( bitValue(i) != _BV4_Z )
         return false;

   return true;
}

bool 
Bv4::has0() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i ) 
      if ( bitValue(i) == _BV4_0 )
         return true;

   return false;
}

bool 
Bv4::has1() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i ) 
      if ( bitValue(i) == _BV4_1 )
         return true;

   return false;
}

bool 
Bv4::hasx() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i ) 
      if ( bitValue(i) == _BV4_X )
         return true;

   return false;
}

bool 
Bv4::hasz() const {
   unsigned int i;

   for ( i = 0; i < _bits; ++i )
      if ( bitValue(i) == _BV4_Z )
         return true;

   return false;
}

// Other operations

Bv2 
Bv4::toBv2() const {
   unsigned int i;
   Bv2          tmp( "1'b0" );
   
   tmp.extend( _bits );

   for ( i = 0; i < _bits; ++i ) {
      if ( bitValue( i ) == _BV4_1 )
         tmp.set( i, _BV2_1 );
      else
         tmp.set( i, _BV2_0 );
   }

   return tmp;
}

Bv4 
Bv4::merge( const Bv4 &opn ) const {
   unsigned int i;
   Bv4          opn1;

   opn1 = *this;

   opn1.extend( opn1.bits() + opn.bits() );
   opn1 = opn1 << opn.bits();

   for ( i = 0; i < opn.bits(); ++i )
      opn1.set( i, opn[i] );
   
   return opn1;
}

Bv4& 
Bv4::append( const Bv4 &opn ) {
   unsigned int i;

   extend( bits() + opn.bits() );

   *this = *this << opn.bits();

   for ( i = 0; i < opn.bits(); ++i )
      set( i, opn[i] );

   return *this;
}
   
Bv4 
Bv4::split( unsigned int msb, unsigned int lsb ) const 
{
   assert (msb >= lsb);
   assert (lsb >= 0);
   assert (msb < bits());
   Bv4          opn1;
   
   opn1.extend(msb-lsb+1);
   for ( unsigned i = 0; i < opn1.bits(); ++i )
      opn1.set( i, bitValue(lsb+i) );
   return opn1;
}

unsigned int 
Bv4::value() const {
   unsigned int i,
                len,
                value,
                bitMask = 0x00000001;
   len = _bits;
   if ( len > 32 )
      len = 32;

   value = 0;
   for ( i = 0; i < len; ++i ) {
      if ( bitValue( i ) == _BV4_1 )
         value += bitMask;

      bitMask = bitMask << 1;
   }

   return value;
}

void 
Bv4::set( unsigned int index, _BV4_VALUE value ) {
   unsigned int bitIndex, 
                wordIndex;

   bitIndex  = index % 32; 
   wordIndex = index / 32;
   
   switch ( value ) {
   case _BV4_0:
      setTo0( bitIndex, _bit0[ wordIndex ], _bit1[ wordIndex ] );
      break;
   case _BV4_1:
      setTo1( bitIndex, _bit0[ wordIndex ], _bit1[ wordIndex ] );
      break;
   case _BV4_X:
      setToX( bitIndex, _bit0[ wordIndex ], _bit1[ wordIndex ] );
      break;
   case _BV4_Z:
      setToZ( bitIndex, _bit0[ wordIndex ], _bit1[ wordIndex ] );
      break;
   }
}

_BV4_VALUE 
Bv4::operator [] ( unsigned int index ) const {
   return bitValue( index );
}

Bv4 
Bv4::reducedAnd () const 
{
   Bv4 tmp;
   if (has0())       tmp = "1'b0";
   else if (full1()) tmp = "1'b1";
   else if (fullz()) tmp = "1'bZ";
   else              tmp = "1'bX"; 
   return tmp;
}
   
Bv4
Bv4::reducedOr () const
{
   Bv4 tmp;
   if (has1())       tmp = "1'b1";
   else if (full0()) tmp = "1'b0";
   else if (fullz()) tmp = "1'bZ";
   else              tmp = "1'bX";  
   return tmp;   
}
   
Bv4
Bv4::reducedNand () const
{
   Bv4 tmp;
   if (has0())       tmp = "1'b1";
   else if (full1()) tmp = "1'b0";
   else if (fullz()) tmp = "1'bZ";
   else              tmp = "1'bX"; 
   return tmp;   
}
   
Bv4 
Bv4::reducedNor () const
{
   Bv4 tmp;
   if (has1())       tmp = "1'b0";
   else if (full0()) tmp = "1'b1";
   else if (fullz()) tmp = "1'bZ";
   else              tmp = "1'bX";
   return tmp;
}
   
Bv4
Bv4::reducedXor () const
{
   Bv4 tmp;
   if (fullz())               tmp = "1'bZ";
   else if (hasx() || hasz()) tmp = "1'bX";
   else {
      int oneCount = 0;
      for (unsigned i = 0; i < _bits; ++i)
         if (bitValue(i) == _BV4_1)
            ++oneCount;
      if ((oneCount%2) == 1)  tmp = "1'b1";
      else                    tmp = "1'b0";
   }
   return tmp;
}
   
Bv4 
Bv4::reducedXnor () const
{
   Bv4 tmp;
   if (fullz())               tmp = "1'bZ";
   else if (hasx() || hasz()) tmp = "1'bX";
   else {
      int oneCount = 0;
      for (unsigned i = 0; i < _bits; ++i)
         if (bitValue(i) == _BV4_1)
            ++oneCount;
      if ((oneCount%2) == 0)  tmp = "1'b1";
      else                    tmp = "1'b0";
   }
   return tmp;
}

// Conversion
MyString 
Bv4::str() const {
   MyString tmp;

   tmp = (int)_bits;
   tmp += "'b";
   tmp += strNoSize();

   return tmp;   
}

MyString
Bv4::strNoSize() const {
   MyString tmp;
   int      i;
 
   tmp = "";

   for ( i = (int)_bits - 1; i >= 0; --i ) {
      if ( bitValue(i) == _BV4_0 )
         tmp += "0";
      else if ( bitValue(i) == _BV4_1 )
         tmp += "1";
      else if ( bitValue(i) == _BV4_X )
         tmp += "X";
      else
         tmp += "Z";
   }

   return tmp;
}

// Output Stream
ostream&
operator << ( ostream& os, const Bv4 &src ) 
{
   int i;

   os << src._bits << "'b";

   for ( i = (int)src._bits - 1; i >= 0; --i ) {
      if ( src[i] == _BV4_0 )
         os << "0";
      else if ( src[i] == _BV4_1 )
         os << "1";
      else if ( src[i] == _BV4_X )
         os << "X";
      else 
         os << "Z";
   }

   return os;
}

// --------------------- Private member functions ------------------------------
void
Bv4::reset() {
   _bits  = 0; 
   _words = 0;
   _mask  = 0;
   if ( _bit0 != NULL )
      delete [] _bit0;
 
   if ( _bit1 != NULL )
      delete [] _bit1;

   _bit0 = NULL;
   _bit1 = NULL;
}

bool
Bv4::octToBin( const char *src, char *buf, unsigned int size ) const {
   unsigned int len,
                j,
                pos;
   int          i;
   char         lastC = '0';
   char         word[3];

   len = strlen( src );
   pos = 0;

   for ( i = (int)len - 1; i >= 0; --i ) {
      switch ( src[i] ) {
      case '0':
         strcpy( word, "000" );   break;
      case '1':
         strcpy( word, "100" );   break;
      case '2':
         strcpy( word, "010" );   break;
      case '3':
         strcpy( word, "110" );   break;
      case '4':
         strcpy( word, "001" );   break;
      case '5':
         strcpy( word, "101" );   break;
      case '6':
         strcpy( word, "011" );   break;
      case '7':
         strcpy( word, "111" );   break;
      case 'x':
      case 'X':
         strcpy( word, "XXX" );   break;
      case 'z':
      case 'Z':
      case '?':
         strcpy( word, "ZZZ" );   break;
      case '_': 
         break;
      default:
         return false;
      }
      for ( j = 0; j < 3 && pos < size; ++j ) {
         buf[ pos++ ] = word[j];
         lastC        = word[j];
      }
   }
 
   if ( lastC == '1' )
      lastC = '0';

   while ( pos < size ) {
      buf[ pos++ ] = lastC;
   }

   return true;
}

bool
Bv4::decToBin( const char *src, char *buf, unsigned int size ) const {
   unsigned int value,
                i,
                bitMask = 0x00000001;

   for ( i = 0; i < strlen( src ); ++i )
      if ( isdigit( src[i] ) == false )
         return false;

   value = (unsigned int)atoi( src );
   for ( i = 0; i < size; ++i ) {
      if ( ( value & bitMask ) > 0 )
         buf[i] = '1';
      else
         buf[i] = '0';
      bitMask = bitMask << 1;
   }
   
   return true;
}

bool
Bv4::hexToBin( const char *src, char *buf, unsigned int size ) const {
   unsigned int len,
                j,
                pos;
   int          i;
   char         lastC = '0';
   char         word[4];

   len = strlen( src );
   pos = 0;

   for ( i = (int)len - 1; i >= 0; --i ) {
      switch ( src[i] ) {
      case '0':
         strcpy( word, "0000" );   break;
      case '1':
         strcpy( word, "1000" );   break;
      case '2':
         strcpy( word, "0100" );   break;
      case '3':
         strcpy( word, "1100" );   break;
      case '4':
         strcpy( word, "0010" );   break;
      case '5':
         strcpy( word, "1010" );   break;
      case '6':
         strcpy( word, "0110" );   break;
      case '7':
         strcpy( word, "1110" );   break;
      case '8':
         strcpy( word, "0001" );   break;
      case '9':
         strcpy( word, "1001" );   break;
      case 'a':
      case 'A':
         strcpy( word, "0101" );   break;
      case 'b':
      case 'B':
         strcpy( word, "1101" );   break;
      case 'c':
      case 'C':
         strcpy( word, "0011" );   break;
      case 'd':
      case 'D':
         strcpy( word, "1011" );   break;
      case 'e':
      case 'E':
         strcpy( word, "0111" );   break;
      case 'f':
      case 'F':
         strcpy( word, "1111" );   break;
      case 'x':
      case 'X':
         strcpy( word, "XXXX" );   break;
      case 'z':
      case 'Z':
      case '?':
         strcpy( word, "ZZZZ" );   break;
      case '_':
         break;
      default:
         return false;
      }

      for ( j = 0; j < 4 && pos < size; ++j ) {
         buf[ pos++ ] = word[j];
         lastC        = word[j];
      }
   }

   if ( lastC == '1' )
      lastC = '0';

   while ( pos < size ) {
      buf[ pos++ ] = lastC;
   }

   return true;
}

void
Bv4::fromStr( const char *src ) {
   unsigned int size;
   int          sizeBound,
                i,
                pos;
   char *       buffer;
   char         lastC = '0';

   sizeBound = strcspn( src, "'" );
   if ( sizeBound == (int)strlen( src ) ) {
      fromInt( atoi( src ) );
      return;
   }

   buffer = new char[ sizeBound + 1 ];
   memset( buffer, 0, sizeBound + 1 );
   strncpy( buffer, src, sizeBound );
   
   size = (unsigned int)atoi( buffer );
   
   delete [] buffer;

   buffer = new char[ size + 1 ];
   memset( buffer, 0, size + 1 );
 
   switch ( src[ sizeBound + 1 ] ) {
   case 'b':
   case 'B':
      pos = 0;
      for ( i = strlen( src ) - 1; i > sizeBound + 1; --i ) {
         switch ( src[i] ) {
         case '0':
         case '1':
            buffer[pos++] = src[i];
            lastC         = src[i];
            break;
         case 'x':
         case 'X':
            buffer[pos++] = 'X';
            lastC         = 'X';
            break;
         case '?':
         case 'z':
         case 'Z':
            buffer[pos++] = 'Z';
            lastC         = 'Z';
            break;
         case '_':
            break;
         default:
            cerr << "ERROR: Bv4::fromStr()---> Faild to create from BIN ";
            cerr << " value[ " << src << " ]." << endl; 
            delete [] buffer;
            return;
         }
      }
      if ( lastC == '1' )
         lastC = '0';

      while ( pos < (int)size )
         buffer[pos++] = lastC;

      break;
   case 'o':
   case 'O':
      if ( octToBin( &src[ sizeBound + 2 ], buffer, size ) == false ) {
         cerr << "ERROR: Bv4::fromStr()---> Faild to create from OCT value[ ";
         cerr << src << " ]." << endl;
         delete [] buffer;
         return;
      }
      break;
   case 'd':
   case 'D':
      if ( decToBin( &src[ sizeBound + 2 ], buffer, size ) == false ) {
         cerr << "ERROR: Bv4::fromStr()---> Faild to create from DEC value[ ";
         cerr << src << " ]." << endl;
         delete [] buffer;
         return;
      }
      break;
   case 'h':
   case 'H':
      if ( hexToBin( &src[ sizeBound + 2 ], buffer, size ) == false ) {
         cerr << "ERROR: Bv4::fromStr()---> Faild to create from HEX value[ ";
         cerr << src << " ]." << endl;
         delete [] buffer;
         return;
      }
      break;
   default: 
      cerr << "ERROR: Bv4::fromStr() ---> Unspported base '" << src[ sizeBound + 1 ] << "'" << endl;
      delete [] buffer;
      return;
   }

#ifdef _WITH_DEBUG
   cout << "MSG: Bv4::fromStr() ---> Expend " << src << " to " << buffer << endl;
#endif

   _bits  = size;
   _mask  = size % 32;
   _words = size / 32;
   if ( _words * 32 < _bits )
      _words++;

   _bit0 = new unsigned int[ _words ];
   _bit1 = new unsigned int[ _words ];

   for ( i = 0; i < (int)size; ++i ) {
      pos = i / 32;
      switch ( buffer[i] ) {
      case '0':
         setTo0( i % 32, _bit0[ pos ], _bit1[ pos ] );
         break;
      case '1':
         setTo1( i % 32, _bit0[ pos ], _bit1[ pos ] );
         break;
      case 'X':
         setToX( i % 32, _bit0[ pos ], _bit1[ pos ] );
         break;
      case 'Z':
         setToZ( i % 32, _bit0[ pos ], _bit1[ pos ] );
         break;
      }
   }

   if ( _mask > 0 )
     for ( i = _mask + 1; i <= 32; ++i )
        setTo0( i - 1, _bit0[ _words - 1 ], _bit1[ _words - 1 ] );

   delete [] buffer;
}

void
Bv4::fromInt( unsigned int src ) {
   int bitMask = 0x00000001,
       i;

#ifdef _WITH_DEBUG
   cout << "Bv4::fromInt() ---> Value: " << src << endl;
#endif

   _bits  = 32;
   _words = 1;
   _mask  = 31;
   _bit0  = new unsigned int[1];
   _bit1  = new unsigned int[1];

   for ( i = 0; i < 32; ++i ) {
      if ( ( src & bitMask ) > 0 ) 
         setTo1( i, _bit0[0], _bit1[0] );
      else 
         setTo0( i, _bit0[0], _bit1[0] );

      bitMask = bitMask << 1;
   }
}

void
Bv4::setTo0( unsigned int bitIndex, unsigned int &bit0, unsigned int &bit1 ) {
//   0: 01
   const int bitMask = 0x00000001;

   bit0 = bit0 | ( bitMask << bitIndex );
   bit1 = bit1 & ~( bitMask << bitIndex );
}

void
Bv4::setTo1( unsigned int bitIndex, unsigned int &bit0, unsigned int &bit1 ) {
//   1: 10
   const int bitMask = 0x00000001;

   bit0 = bit0 & ~( bitMask << bitIndex );
   bit1 = bit1 | ( bitMask << bitIndex );
}

void
Bv4::setToX( unsigned int bitIndex, unsigned int &bit0, unsigned int &bit1 ) {
//   X: 00
   const int bitMask = 0x00000001;

   bit0 = bit0 & ~( bitMask << bitIndex );
   bit1 = bit1 & ~( bitMask << bitIndex );
}

void
Bv4::setToZ( unsigned int bitIndex, unsigned int &bit0, unsigned int &bit1 ) {
//   Z: 11   
   const int bitMask = 0x00000001;

   bit0 = bit0 | ( bitMask << bitIndex );
   bit1 = bit1 | ( bitMask << bitIndex );
}

void 
Bv4::fill0() {
   unsigned int i;

   for ( i = 0; i < _words; ++i ) {
      _bit0[i] = 0xFFFFFFFF;
      _bit1[i] = 0x0;
   }
}

void
Bv4::fill1() {
   unsigned int i;

   for ( i = 0; i < _words; ++i ) {
      _bit0[i] = 0x0;
      _bit1[i] = 0xFFFFFFFF;
   }
}

void
Bv4::fillX() {
   unsigned int i;

   for ( i = 0; i < _words; ++i ) {
      _bit0[i] = 0x0;
      _bit1[i] = 0x0;
   }
}

void
Bv4::fillZ() {
   unsigned int i;

   for ( i = 0; i < _words; ++i ) {
      _bit0[i] = 0xFFFFFFFF;
      _bit1[i] = 0xFFFFFFFF;
   }
}

void
Bv4::extend( unsigned int size ) {
   unsigned int* tmp0, 
               * tmp1;
   unsigned int  words,
                 i;

   words = _words;
   if ( size > words * 32 ) {
      words = size / 32;
      if ( words * 32 < size )
         words++;
   }

   if ( _words == 0 ) {
      _bit0 = new unsigned int[ words ];
      _bit1 = new unsigned int[ words ];
   }
   else {   
      tmp0 = new unsigned int[ words ];
      tmp1 = new unsigned int[ words ];

      for ( i = 0; i < _words; ++i ) {
         tmp0[i] = _bit0[i];
         tmp1[i] = _bit1[i];
      }

      delete [] _bit0;
      delete [] _bit1;
     
      _bit0  = tmp0;
      _bit1  = tmp1;
   }
   
   _bits = size;

   if ( _mask > 0 ) {
      for ( i = _mask + 1; i <= 32; ++i )
         setTo0( i - 1, _bit0[ _words - 1 ], _bit1[ _words - 1 ] );
   }

   for ( i = _words; i < words; ++i ) {
      _bit0[i] = 0xFFFFFFFF;
      _bit1[i] = 0x0;
   }

   _bits  = size;
   _words = words;
   _mask  = _bits % 32;
}

_BV4_VALUE
Bv4::bitValue( unsigned int index ) const {
   unsigned int bitIndex,
                wordIndex,
                bit0, bit1;
   const int    bitMask = 0x00000001;

   bitIndex  = index % 32;
   wordIndex = index / 32;

   bit0 = _bit0[ wordIndex ] & ( bitMask << bitIndex );
   bit1 = _bit1[ wordIndex ] & ( bitMask << bitIndex );

   if ( bit0 == 0 && bit1 == 0 )
      return _BV4_X;
   else if ( bit0 > 0 && bit1 == 0 )
      return _BV4_0;
   else if ( bit0 == 0 && bit1 > 0 )
      return _BV4_1;
   else
      return _BV4_Z;
}

_BV4_VALUE
Bv4::add( _BV4_VALUE a, _BV4_VALUE b, _BV4_VALUE c ) const
{
   if ( a == _BV4_0 && b == _BV4_0 && c == _BV4_0 ) return _BV4_0;
   if ( a == _BV4_0 && b == _BV4_0 && c == _BV4_1 ) return _BV4_1;
   if ( a == _BV4_0 && b == _BV4_1 && c == _BV4_0 ) return _BV4_1;
   if ( a == _BV4_0 && b == _BV4_1 && c == _BV4_1 ) return _BV4_0;
   if ( a == _BV4_1 && b == _BV4_0 && c == _BV4_0 ) return _BV4_1;
   if ( a == _BV4_1 && b == _BV4_0 && c == _BV4_1 ) return _BV4_0;
   if ( a == _BV4_1 && b == _BV4_1 && c == _BV4_0 ) return _BV4_0;
   if ( a == _BV4_1 && b == _BV4_1 && c == _BV4_1 ) return _BV4_1;

   return _BV4_X;
}

_BV4_VALUE
Bv4::carry( _BV4_VALUE a, _BV4_VALUE b, _BV4_VALUE c ) const
{
   if ( a == _BV4_0 && b == _BV4_0 && c == _BV4_0 ) return _BV4_0;
   if ( a == _BV4_0 && b == _BV4_0 && c == _BV4_1 ) return _BV4_0;
   if ( a == _BV4_0 && b == _BV4_1 && c == _BV4_0 ) return _BV4_0;
   if ( a == _BV4_0 && b == _BV4_1 && c == _BV4_1 ) return _BV4_1;
   if ( a == _BV4_1 && b == _BV4_0 && c == _BV4_0 ) return _BV4_0;
   if ( a == _BV4_1 && b == _BV4_0 && c == _BV4_1 ) return _BV4_1;
   if ( a == _BV4_1 && b == _BV4_1 && c == _BV4_0 ) return _BV4_1;
   if ( a == _BV4_1 && b == _BV4_1 && c == _BV4_1 ) return _BV4_1;

   return _BV4_X;
}

#endif
