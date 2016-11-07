/******************************************************************************
  PackageName [ utility/bv ]
  Synopsis    [ 4 valued bit-vector ]
  Author      [ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2005-2007 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/

#ifndef _UTIL_BV2_SOURCE
#define _UTIL_BV2_SOURCE

#include <cstring>
#include "bv4.h"

// Constructor
Bv2::Bv2() {
   _bits  = 0;
   _words = 0;
   _mask  = 0;
   _bit   = NULL;
}

Bv2::Bv2( const string &src ) {
   fromStr( src.c_str() );
}

Bv2::Bv2( const char *src ) {
   fromStr( src );
}

Bv2::Bv2( unsigned int src ) {
   fromInt( src );
}

Bv2::Bv2( const Bv2 &src ) {
   unsigned int i;
   _bits  = src._bits;
   _words = src._words;
   _mask  = src._mask;
   _bit   = new unsigned int[ _words ];

   for ( i = 0; i < _words; ++i ) 
      _bit[i] = src._bit[i];
}

Bv2::Bv2( const Bv4 &src ) {
   unsigned int i,
                wordIndex;

   _bits  = src._bits;
   _words = src._words;
   _mask  = src._mask;
   _bit   = new unsigned int[ _words ];

   for ( i = 0; i < _bits; ++i ) {
      wordIndex = i / 32;
      if ( src[i] == _BV4_1 )
         setTo1( i % 32, _bit[ wordIndex ] );
      else
         setTo0( i % 32, _bit[ wordIndex ] );
   }
}

// Destructor
Bv2::~Bv2() {
   reset();
}

// Assignment
Bv2& 
Bv2::operator = ( const string &src ) {
   reset();
   fromStr( src.c_str() );

   return *this;
}

Bv2& 
Bv2::operator = ( const char *src ) {
   reset();
   fromStr( src );

   return *this;
}

Bv2& 
Bv2::operator = ( unsigned int src ) {
   reset();
   fromInt( src );

   return *this;
}

Bv2& 
Bv2::operator = ( const Bv2 &src ) {
   unsigned int i;

   reset();
   _bits  = src._bits; 
   _words = src._words;
   _mask  = src._mask;
   _bit   = new unsigned int[ _words ];

   for ( i = 0; i < _words; ++i ) 
      _bit[i] = src._bit[i];

   return *this;
}

Bv2&
Bv2::operator = ( const Bv4 &src ) {
   unsigned int i,
                wordIndex;

   reset();

   _bits  = src._bits;
   _words = src._words;
   _mask  = src._mask;
   _bit   = new unsigned int[ _words ];

   for ( i = 0; i < _bits; ++i ) {
      wordIndex = i / 32;
      if ( src[i] == _BV4_1 )
         setTo1( i % 32, _bit[ wordIndex ] );
      else
         setTo0( i % 32, _bit[ wordIndex ] );
   }
   
   return *this;
}

// Bit-wise logic operations
Bv2 
Bv2::operator & ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2;

   unsigned int i,
                a0, b0;

   opn1 = *this;
   opn2 = opn;

   if ( bits() > opn2.bits() ) 
      opn2.extend( opn1.bits() );
   else 
      opn1.extend( opn2.bits() );

   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit[i];
      b0 = opn2._bit[i];
   
      opn1._bit[i] = a0 & b0;
   }

   if ( _mask > 0 ) {
     for ( i = opn1._mask + 1; i < 32; ++i ) 
        opn1.set( opn1.bits() + i, _BV2_0 );
   }

   return opn1;
}

Bv2 
Bv2::operator | ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2;

   unsigned int i,
                a0, b0;

   opn1 = *this;
   opn2 = opn;

   if ( bits() > opn2.bits() ) 
      opn2.extend( opn1.bits() );
   else 
      opn1.extend( opn2.bits() );

   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit[i];
      b0 = opn2._bit[i];
   
      opn1._bit[i] = a0 | b0;
   }

   if ( _mask > 0 ) {
     for ( i = opn1._mask + 1; i < 32; ++i )
        opn1.set( opn1.bits() + i, _BV2_0 );
   }

   return opn1;
}

Bv2 
Bv2::operator ^ ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2;

   unsigned int i,
                a0, b0;

   opn1 = *this;
   opn2 = opn;

   if ( bits() > opn2.bits() ) 
      opn2.extend( opn1.bits() );
   else 
      opn1.extend( opn2.bits() );

   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit[i];
      b0 = opn2._bit[i];
   
      opn1._bit[i] = a0 ^ b0;
   }

   if ( _mask > 0 ) {
     for ( i = opn1._mask + 1; i < 32; ++i )
        opn1.set( opn1.bits() + i, _BV2_0 );
   }

   return opn1;
}

Bv2 
Bv2::operator ~ () const {
   Bv2          opn1;

   unsigned int i,
                a0;

   opn1 = *this;
   
   for ( i = 0; i < opn1._words; ++i ) {
      a0 = opn1._bit[i];

      opn1._bit[i] = ~a0;
   }

   for ( i = opn1._mask + 1; i <= 32; ++i ) 
      opn1.set( opn1.bits() + i, _BV2_0 );

   return opn1;
}

Bv2& 
Bv2::operator &= ( const Bv2 &opn ) {
   *this = *this & opn;

   return *this;
}

Bv2& 
Bv2::operator |= ( const Bv2 &opn ) {
   *this = *this | opn;

   return *this;
}

Bv2& 
Bv2::operator ^= ( const Bv2 &opn ) {
   *this = *this ^ opn;

   return *this;
}

// Arithmetic operation
Bv2 
Bv2::operator + ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2,
                opn3;

   _BV2_VALUE   vA,
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

   opn3 = opn1;
   opn3.extend( opn3.bits() + 1 );
   
   vC1 = _BV2_0;
   vC2 = _BV2_0;
   for ( i = 0; i < opn1.bits(); ++i ) {
      vA = opn1[i];
      vB = opn2[i];
      
      opn3.set( i, add( vA, vB, vC1 ) );
      vC2 = vC1;
      vC1 = carry( vA, vB, vC1 );
   }

   if ( vC1 == _BV2_1 && vC2 == _BV2_1 ) {
      opn3._bits--;
      opn3._mask = opn3._bits % 32;
   }
   else {
      if ( vC1 == _BV2_1 ) {
         opn3.set( i, _BV2_1 );
      }
      else {
         opn3._bits--;
         opn3._mask = opn3._bits % 32;
      }
   }

   return opn3;
}

Bv2 
Bv2::operator - ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2,
                step("1'b1");

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   opn1 = opn1 + (~opn2);
   opn1 = opn1 + step;

   if ( opn1.bits() > bits() ) {
      opn1._bits--;
      opn1.set( opn1._bits, _BV2_0 );
   }

   return opn1;
}

Bv2 
Bv2::operator * ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2,
                opn3;
   unsigned int i;

   opn1 = *this;
   opn2 = opn;

   opn1.extend( opn1.bits() + opn2.bits() );
   opn3 = opn1;
   opn3.fill0();

   for ( i = 0; i < opn2.bits(); ++i ) {
      if ( opn2[i] == _BV2_1 )
         opn3 = opn3 + opn1;

      opn1 = opn1 << 1;
   }

   return opn3;
}

Bv2 
Bv2::operator / ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2,
                opn3;
   int          len;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );
 
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
   while ( opn2[ len ] != _BV2_1 )
      len--;

   len = (int)opn2.bits() - len - 1;
   opn2 = opn2 << len;

   opn3.extend( opn1.bits() );
   opn3.fill0();

   while ( len >= 0 ) {
      if ( opn1 >= opn2 ) {
         opn1 = opn1 - opn2;
         opn3.set( 0, _BV2_1 );
      }
      opn2 = opn2 >> 1;
      opn3 = opn3 << 1;
      len--;
   }

   opn3 = opn3 >> 1;

   return opn3;
}


Bv2 
Bv2::operator % ( const Bv2 &opn ) const {
   Bv2          opn1,
                opn2,
                opn3;
   int          len;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

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
   while ( opn2[ len ] != _BV2_1 )
      len--;

   len = (int)opn2.bits() - len - 1;
   opn2 = opn2 << len;

   opn3.extend( opn1.bits() );
   opn3.fill0();

   while ( len >= 0 ) {
      if ( opn1 >= opn2 ) {
         opn1 = opn1 - opn2;
         opn3.set( 0, _BV2_1 );
      }
      opn2 = opn2 >> 1;
      opn3 = opn3 << 1;
      len--;
   }

   opn3 = opn3 >> 1;

   return opn1;
}

Bv2&
Bv2::operator += ( const Bv2 &opn ) {
   *this = *this + opn;

   return *this;
}

Bv2&
Bv2::operator -= ( const Bv2 &opn ) {
   *this = *this - opn;

   return *this;
}

Bv2&
Bv2::operator *= ( const Bv2 &opn ) {
   *this = *this * opn;
 
   return *this;
}

Bv2&
Bv2::operator /= ( const Bv2 &opn ) {
   *this = *this / opn;

   return *this;
}

Bv2&
Bv2::operator %= ( const Bv2 &opn ) {
   *this = *this % opn;

   return *this;
}

// Comparsion
bool 
Bv2::operator == ( const Bv2 &opn ) const {
   unsigned int i;

   Bv2          opn1, 
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = 0; i < _words; ++i )
      if ( opn1._bit[i] != opn2._bit[i] )
         return false;

   return true;
}

bool 
Bv2::operator != ( const Bv2 &opn ) const {
   unsigned int i;

   Bv2          opn1,
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = 0; i < _words; ++i )
      if ( opn1._bit[i] == opn2._bit[i] )
         return false;

   return true;
}


bool 
Bv2::operator >  ( const Bv2 &opn ) const {
   unsigned int i;

   Bv2          opn1,
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV2_1 && opn2[i] == _BV2_0 )
         return true;
      if ( opn1[i] == _BV2_0 && opn2[i] == _BV2_1 )
         return false;
   }

   return false;
}

bool 
Bv2::operator >= ( const Bv2 &opn ) const {
   unsigned int i;

   Bv2          opn1,
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV2_1 && opn2[i] == _BV2_0 )
         return true;
      if ( opn1[i] == _BV2_0 && opn2[i] == _BV2_1 )
         return false;
   }

   return true;
}

bool 
Bv2::operator <  ( const Bv2 &opn ) const {
   unsigned int i;

   Bv2          opn1,
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV2_1 && opn2[i] == _BV2_0 )
         return false;
      if ( opn1[i] == _BV2_0 && opn2[i] == _BV2_1 )
         return true;
   }

   return false;
}

bool 
Bv2::operator <= ( const Bv2 &opn ) const {
   unsigned int i;

   Bv2          opn1,
                opn2;

   opn1 = *this;
   opn2 = opn;

   if ( _bits > opn2.bits() )
      opn2.extend( _bits );
   else
      opn1.extend( opn2.bits() );

   for ( i = _bits - 1; i >= 0; --i ) {
      if ( opn1[i] == _BV2_1 && opn2[i] == _BV2_0 )
         return false;
      if ( opn1[i] == _BV2_0 && opn2[i] == _BV2_1 )
         return true;
   }

   return true;
}

// Shift operations
Bv2 
Bv2::operator >> ( unsigned int bit ) const {
   unsigned int i;
   Bv2          opn = *this;

   for ( i = 0; i < _bits - bit; ++i )
      opn.set( i, opn[ i + bit ] );

   for ( i = _bits - bit; i < _bits; ++i )
      opn.set( i, _BV2_0 );

   return opn;
}

Bv2&
Bv2::operator >>= ( unsigned int bit ) {
   *this = *this >> bit;
   
   return *this;
}

Bv2 
Bv2::operator << ( unsigned int bit ) const {
   unsigned int i;
   Bv2          opn = *this;

   for ( i = _bits - 1; i >= bit; --i )
      opn.set( i, opn[ i - bit ] );

   for ( i = 0; i < bit; ++i )
      opn.set( i, _BV2_0 );

   return opn;
}

Bv2&
Bv2::operator <<= ( unsigned int bit ) {
   *this = *this << bit;

   return *this;
}

// Feature member function
unsigned int 
Bv2::bits() const {
   return _bits;
}

// Other operations

Bv4
Bv2::toBv4() const {
   unsigned int i;
   Bv4          tmp("1'b0");

   tmp.extend( _bits );

   for ( i = 0; i < _bits; ++i ) {
      if ( bitValue( i ) == _BV2_1 )
         tmp.set( i, _BV4_1 );
      else
         tmp.set( i, _BV4_0 );
   }

   return tmp;
}

Bv2 
Bv2::merge( const Bv2 &opn ) const {
   unsigned int i;
   Bv2          opn1;

   opn1 = *this;

   opn1.extend( opn1.bits() + opn.bits() );
   opn1 = opn1 << opn.bits();

   for ( i = 0; i < opn.bits(); ++i )
      opn1.set( i, opn[i] );
   
   return opn1;
}

Bv2& 
Bv2::append( const Bv2 &opn ) {
   unsigned int i;

   extend( bits() + opn.bits() );
   *this = *this << opn.bits();

   for ( i = 0; i < opn.bits(); ++i )
      set( i, opn[i] );

   return *this;
}

unsigned int 
Bv2::value() const {
   unsigned int i,
                len,
                value,
                bitMask = 0x00000001;
   len = _bits;
   if ( len > 32 )
      len = 32;

   value = 0;
   for ( i = 0; i < len; ++i ) {
      if ( bitValue( i ) == _BV2_1 )
         value += bitMask;

      bitMask = bitMask << 1;
   }

   return value;
}

void 
Bv2::set( unsigned int index, _BV2_VALUE value ) {
   unsigned int bitIndex, 
                wordIndex;

   bitIndex  = index % 32; 
   wordIndex = index / 32;
 
   if ( value == _BV2_0 )
      setTo0( bitIndex, _bit[ wordIndex ] );
   else
      setTo1( bitIndex, _bit[ wordIndex ] );
}

_BV2_VALUE 
Bv2::operator [] ( unsigned int index ) const {
   return bitValue( index );
}

// Conversion
MyString 
Bv2::str() const {
   MyString tmp;

   tmp = (int)_bits;
   tmp += "'b";
   tmp += strNoSize();

   return tmp;   
}

MyString
Bv2::strNoSize() const {
   MyString tmp;
   int      i;
 
   tmp = "";

   for ( i = (int)_bits - 1; i >= 0; --i ) {
      if ( bitValue(i) == _BV2_0 )
         tmp += "0";
      else
         tmp += "1";
   }

   return tmp;
}

// Output Stream
ostream&
operator << ( ostream& os, const Bv2 &src ) 
{
   int i;

   os << src._bits << "'b";

   for ( i = (int)src._bits - 1; i >= 0; --i ) {
      if ( src[i] == _BV2_0 )
         os << "0";
      else 
         os << "1";
   }

   return os;
}

// --------------------- Private member functions ------------------------------
void
Bv2::reset() {
   _bits  = 0; 
   _words = 0;
   _mask  = 0;
   if ( _bit != NULL )
      delete [] _bit;
}

bool
Bv2::octToBin( const char *src, char *buf, unsigned int size ) const {
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
Bv2::decToBin( const char *src, char *buf, unsigned int size ) const {
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
Bv2::hexToBin( const char *src, char *buf, unsigned int size ) const {
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
Bv2::fromStr( const char *src ) {
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
   memset( buffer, 0, sizeof( buffer ) );
   strncpy( buffer, src, sizeBound );
   
   size = (unsigned int)atoi( buffer );
   
   delete [] buffer;

   buffer = new char[ size + 1 ];
   memset( buffer, 0, sizeof( buffer ) );
 
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
            cerr << "ERROR: Bv2::fromStr()---> Faild to create from BIN ";
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
         cerr << "ERROR: Bv2::fromStr()---> Faild to create from OCT value[ ";
         cerr << src << " ]." << endl;
         delete [] buffer;
         return;
      }
      break;
   case 'd':
   case 'D':
      if ( decToBin( &src[ sizeBound + 2 ], buffer, size ) == false ) {
         cerr << "ERROR: Bv2::fromStr()---> Faild to create from DEC value[ ";
         cerr << src << " ]." << endl;
         delete [] buffer;
         return;
      }
      break;
   case 'h':
   case 'H':
      if ( hexToBin( &src[ sizeBound + 2 ], buffer, size ) == false ) {
         cerr << "ERROR: Bv2::fromStr()---> Faild to create from HEX value[ ";
         cerr << src << " ]." << endl;
         delete [] buffer;
         return;
      }
      break;
   default: 
      cerr << "ERROR: Bv2::fromStr() ---> Unspported base '" << src[ sizeBound + 1 ] << "'" << endl;
      delete [] buffer;
      return;
   }

#ifdef _WITH_DEBUG
   cout << "MSG: Bv2::fromStr() ---> Expend " << src << " to " << buffer << endl;
#endif

   _bits  = size;
   _mask  = size % 32;
   _words = size / 32;
   if ( _words * 32 < _bits )
      _words++;

   _bit = new unsigned int[ _words ];

   for ( i = 0; i < (int)size; ++i ) {
      pos = i / 32;
      if ( buffer[i] == '0' )
         setTo0( i % 32, _bit[ pos ] );
      else
         setTo1( i % 32, _bit[ pos ] );
   }

   if ( _mask > 0 ) 
      for ( i = _mask + 1; i <= 32; ++i )
         setTo0( i - 1, _bit[ _words - 1 ] );

   delete [] buffer;
}

void
Bv2::fromInt( unsigned int src ) {
   int bitMask = 0x00000001,
       i;

#ifdef _WITH_DEBUG
   cout << "Bv2::fromInt() ---> Value: " << src << endl;
#endif

   _bits  = 32;
   _words = 1;
   _mask  = 31;
   _bit   = new unsigned int[1];

   for ( i = 0; i < 32; ++i ) {
      if ( ( src & bitMask ) > 0 ) 
         setTo0( i, _bit[0] );
      else 
         setTo1( i, _bit[0] );

      bitMask = bitMask << 1;
   }
}

void
Bv2::setTo0( unsigned int bitIndex, unsigned int &bit ) {
   const int bitMask = 0x00000001;

   bit = bit & ~( bitMask << bitIndex );
}

void
Bv2::setTo1( unsigned int bitIndex, unsigned int &bit ) {
   const int bitMask = 0x00000001;

   bit = bit | ( bitMask << bitIndex );
}

void 
Bv2::fill0() {
   unsigned int i;

   for ( i = 0; i < _words; ++i ) {
      _bit[i] = 0x0;
   }
}

void
Bv2::fill1() {
   unsigned int i;

   for ( i = 0; i < _words; ++i ) {
      _bit[i] = 0xFFFFFFFF;
   }
}

void
Bv2::extend( unsigned int size ) {
   unsigned int* tmp;
   unsigned int  words,
                 i;

   words = _words;         
   if ( size > words * 32 ) {
      words = size / 32;
      if ( words * 32 < size ) 
         words++;
   }

   if ( _words == 0 ) {
      _bit = new unsigned int[ words ];
   }
   else {
      tmp = new unsigned int[ words ];

      for ( i = 0; i < _words; ++i ) 
         tmp[i] = _bit[i];

      delete [] _bit;
     
      _bit  = tmp;
   }
 
   _bits = size;

   if ( _mask > 0 ) {
      for ( i = _mask + 1; i <= 32; ++i )
         setTo0( i - 1, _bit[ _words - 1 ] );
   }

   for ( i = _words; i < words; ++i ) 
      _bit[i] = 0x0;

   _bits  = size;
   _words = words;
   _mask  = _bits % 32;
}

_BV2_VALUE
Bv2::bitValue( unsigned int index ) const {
   unsigned int bitIndex,
                wordIndex,
                bit;
   const int    bitMask = 0x00000001;

   bitIndex  = index % 32;
   wordIndex = index / 32;

   bit = _bit[ wordIndex ] & ( bitMask << bitIndex );

   if ( bit > 0 ) 
      return _BV2_1;
   else
      return _BV2_0;
}

_BV2_VALUE
Bv2::add( _BV2_VALUE a, _BV2_VALUE b, _BV2_VALUE c ) const
{
   if ( a == _BV2_0 && b == _BV2_0 && c == _BV2_0 ) return _BV2_0;
   if ( a == _BV2_0 && b == _BV2_0 && c == _BV2_1 ) return _BV2_1;
   if ( a == _BV2_0 && b == _BV2_1 && c == _BV2_0 ) return _BV2_1;
   if ( a == _BV2_0 && b == _BV2_1 && c == _BV2_1 ) return _BV2_0;
   if ( a == _BV2_1 && b == _BV2_0 && c == _BV2_0 ) return _BV2_1;
   if ( a == _BV2_1 && b == _BV2_0 && c == _BV2_1 ) return _BV2_0;
   if ( a == _BV2_1 && b == _BV2_1 && c == _BV2_0 ) return _BV2_0;
   if ( a == _BV2_1 && b == _BV2_1 && c == _BV2_1 ) return _BV2_1;

   return _BV2_0;
}

_BV2_VALUE
Bv2::carry( _BV2_VALUE a, _BV2_VALUE b, _BV2_VALUE c ) const
{
   if ( a == _BV2_0 && b == _BV2_0 && c == _BV2_0 ) return _BV2_0;
   if ( a == _BV2_0 && b == _BV2_0 && c == _BV2_1 ) return _BV2_0;
   if ( a == _BV2_0 && b == _BV2_1 && c == _BV2_0 ) return _BV2_0;
   if ( a == _BV2_0 && b == _BV2_1 && c == _BV2_1 ) return _BV2_1;
   if ( a == _BV2_1 && b == _BV2_0 && c == _BV2_0 ) return _BV2_0;
   if ( a == _BV2_1 && b == _BV2_0 && c == _BV2_1 ) return _BV2_1;
   if ( a == _BV2_1 && b == _BV2_1 && c == _BV2_0 ) return _BV2_1;
   if ( a == _BV2_1 && b == _BV2_1 && c == _BV2_1 ) return _BV2_1;

   return _BV2_0;
}

#endif
