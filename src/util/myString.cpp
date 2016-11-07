/******************************************************************************
  FileName    [ myString.cpp ]
  PackageName [ utility/myString ]
  Synopsis    [ string class with funny operation ]
  Author[ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2006 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/

#ifndef _MY_STRING_SOURCE_
#define _MY_STRING_SOURCE_

#include <cstring>
#include "myString.h"

MyString::MyString( const int src ) {
   char   buffer[12];
   int    base, tmp, index;

   _size = _STR_BLOCK_SIZE;
   _data = new char[ _size ];
   memset( _data, '\0', _size );

   if ( src == 0 ) {
      _data[0] = '0';
      _len     = 1;
   }
   else {
      index = 0;
      tmp   = src;
      base  = 0;
      if ( src < 0 ) {
         _data[0] = '-';
         base     = 1;
         tmp      *= -1;;
      }

      while ( tmp != 0 ) {
         buffer[ index++ ] = '0' + tmp % 10;
         tmp /= 10;
      }
      for ( int i = index - 1; i >= 0; --i )
         _data[ base++ ] = buffer[ i ];
  
      _len = base;  
   }
}

MyString::MyString( const unsigned src ) {
   char   buffer[12];
   int    base, tmp, index;

   _size = _STR_BLOCK_SIZE;
   _data = new char[ _size ];
   memset( _data, '\0', _size );

   if ( src == 0 ) {
      _data[0] = '0';
      _len     = 1;
   }
   else {
      index = 0;
      tmp   = src;
      base  = 0;

      while ( tmp != 0 ) {
         buffer[ index++ ] = '0' + tmp % 10;
         tmp /= 10;
      }
      for ( int i = index - 1; i >= 0; --i )
         _data[ base++ ] = buffer[ i ];
  
      _len = base;  
   }
}

MyString::MyString( const double src ) {
   double     limit = 10e-8,
              afterDot,
              source;
   int        beforeDot,
              len;
   
   MyString   tmp = "";

   source = src;
   if ( src < 0 ) {
      source *= -1.0;
      tmp     = "-";
   }

   beforeDot = (int)( source );
   afterDot  = source - (double)beforeDot;

   tmp  = beforeDot;
   tmp += ".";
   len  = 0;
   if ( afterDot > limit ) {
      while ( afterDot > limit && len++ < _PRECISION_LEN ) {
         afterDot *= 10;
         beforeDot = (int)afterDot;
         tmp      += beforeDot;
         afterDot -= (double)beforeDot;
     }
   }
   else {
      tmp += "0";
   }

   _size = _STR_BLOCK_SIZE;
   _data = new char[ _size ];
   memset( _data, '\0', _size );
   _len = 0;

   *this = tmp;   
}

MyString::MyString( const char* src ) {
   if ( src == NULL ) {
      _size = _STR_BLOCK_SIZE;
      _len  = 0;
   }
   else {
      _len  = strlen( src );
      _size = (int)( ceil( (double)_len / _STR_BLOCK_SIZE ) ) * _STR_BLOCK_SIZE;
   }

   _data = new char[ _size ];
   memset( _data, '\0', _size );

   if ( src != NULL ) {
      memcpy( _data, src, _len );
   }
}

MyString::MyString( const char src ) {
   _len  = 1;
   _size = _STR_BLOCK_SIZE;

   _data = new char[ _size ];
   memset( _data, '\0', _size );

   _data[0] = src;
}

MyString::MyString( const string& src ) {
   _len  = strlen( src.c_str() );
   _size = (int)( ceil( (double)_len / _STR_BLOCK_SIZE ) ) * _STR_BLOCK_SIZE;

   _data = new char[ _size ];
   memset( _data, '\0', _size );

   memcpy( _data, src.c_str(), _len );
}

MyString::MyString( const MyString& src ) {
   _len  = src._len;
   _size = src._size;

   _data = new char[ _size ];
   memset( _data, '\0', _size );

   memcpy( _data, src._data, _len );
}

MyString::~MyString() {
   if ( _data != NULL )
      delete [] _data;
}

MyString 
MyString::operator + ( const int src ) const {
   MyString tmp = *this; 
   MyString tmp1(src);

   tmp += src;
   return tmp;
}

MyString 
MyString::operator + ( const unsigned src ) const {
   MyString tmp = *this; 
   MyString tmp1(src);
   tmp += src;
   return tmp;
}

MyString 
MyString::operator + ( const double src ) const {
   MyString tmp = *this;
   tmp += src;
   return tmp;
}

MyString 
MyString::operator + ( const char* src ) const {
   MyString tmp = *this;
   tmp += src;
   return tmp;
}

MyString
MyString::operator + ( const char src ) const {
   MyString tmp = *this;
   tmp += src;
   return tmp;
}

MyString 
MyString::operator + ( const string& src ) const {
   MyString tmp = *this;
   tmp += src;
   return tmp;
}

MyString 
MyString::operator + ( const MyString& src ) const {
   MyString tmp = *this;
   tmp += src;
   return tmp;
}

MyString&
MyString::operator += ( const int src ) {
   MyString tmp( src );
   *this += tmp;
   return *this;
}

MyString&
MyString::operator += ( const unsigned src ) {
   MyString tmp( src );
   *this += tmp;
   return *this;
}

MyString&
MyString::operator += ( const double src ) {
   MyString tmp( src );
   *this += src;
   return *this;
}

MyString&
MyString::operator += ( const char* src ) {
   int       newLen, newSize;
   char*     buffer;

   newLen  = _len + strlen( src );
   newSize = (int)(ceil( (double)newLen / _STR_BLOCK_SIZE )) * _STR_BLOCK_SIZE;

   if ( newSize > _size ) {
      buffer = new char[ newSize ];
      memset( buffer, '\0', newSize );
      memcpy( buffer, _data, _len );
   }
   else
      buffer = _data;
  
   memcpy( &buffer[_len], src, strlen( src ) );

   if ( newSize > _size ) {
      delete [] _data;
      _data = buffer;
      _size = newSize;
   }
   _len  = newLen;
   return *this;
}

MyString&
MyString::operator += ( const char src ) {
   int   newLen, newSize;
   char* buffer;
   
   newLen  = _len + 1;
   newSize = (int)(ceil( (double)newLen / _STR_BLOCK_SIZE )) * _STR_BLOCK_SIZE;

   if ( newSize > _size ) {
      buffer = new char[ newSize ];
      memset( buffer, '\0', newSize );
      memcpy( buffer, _data, _len );
   }
   else
      buffer = _data;

   buffer[ _len ] = src;
   if ( newSize > _size ) {
      delete [] _data;
      _data = buffer;
      _size = newSize;
   }

   _len  = newLen;
   return *this;   
}

MyString&
MyString::operator += ( const string& src ) {
   *this += src.c_str();
   return *this;
}

MyString&
MyString::operator += ( const MyString& src ) {
   int   newLen, newSize;
   char* buffer;

   newLen  = _len + src._len;
   newSize = (int)( ceil( (double)newLen / _STR_BLOCK_SIZE ) ) * _STR_BLOCK_SIZE;

   if ( newSize > _size ) {
      buffer = new char[ newSize ];
      memset( buffer, '\0', newSize );
      memcpy( buffer, _data, _len );
   }
   else 
      buffer = _data;
   
   memcpy( &buffer[_len], src._data, src._len );

   if ( newSize > _size ) {
      delete [] _data;
      _data = buffer;
      _size = newSize;
   }

   _len  = newLen;   
   return *this;
}

MyString&
MyString::operator << ( const int src ) {
   MyString tmp( src );
   *this += tmp;
   return *this;
}

MyString&
MyString::operator << ( const unsigned src ) {
   MyString tmp( src );
   *this += tmp;
   return *this;
}

MyString&
MyString::operator << ( const double src ) {
   MyString tmp( src );
   *this += tmp;
   return *this;
}

MyString&
MyString::operator << ( const char* src ) {
   *this += src;
   return *this;
}

MyString&
MyString::operator << ( const char src ) {
   *this += src;
   return *this;
}

MyString&
MyString::operator << ( const string& src ) {
   *this += src.c_str();
   return *this;
}

MyString&
MyString::operator << ( const MyString& src ) {
   *this += src;
   return *this;
}

MyString
MyString::substr( unsigned int begin, unsigned int len ) const {
   MyString     tmp = "";
   unsigned limit = begin + len;

   if ( (int)limit > _len )
      limit = _len;
   for (unsigned i = begin; i < limit; ++i )
      tmp += _data[i];

   return tmp;
}

MyString& 
MyString::operator = ( const int src ) {
   MyString tmp( src );
   *this = tmp;
   return *this;
}

MyString& 
MyString::operator = ( const unsigned src ) {
   MyString tmp( src );
   *this = tmp;
   return *this;
}

MyString& 
MyString::operator = ( const double src ) {
   MyString tmp( src );
   *this = tmp;
   return *this;
}

MyString& 
MyString::operator = ( const char* src ) {
   _len  = strlen( src );
   int tmp = (int)( ceil( (double)_len / _STR_BLOCK_SIZE ) ) * _STR_BLOCK_SIZE;   

   if ( _size < tmp ) {
      _size = tmp;
      delete [] _data;
      _data = new char[ _size ];
   }
   memset( _data, '\0', _size );
   memcpy( _data, src, _len );

   return *this;
}

MyString&
MyString::operator = ( const char src ) {
   _len = 1;
   memset( _data, '\0', _size );
   _data[0] = src;
   return *this;
}

MyString& 
MyString::operator = ( const string& src ) {
   *this = src.c_str();
   return *this;
}

MyString& 
MyString::operator = ( const MyString& src ) {
   _len  = src._len;
   if ( _size < src._size ) {
      _size = src._size;
      delete [] _data;
      _data = new char[ _size ];
   }
   memset( _data, '\0', _size );
   memcpy( _data, src._data, _len );

   return *this;
}

bool
MyString::operator == ( const MyString& src ) const {
   int i;

   if ( _len == src._len ) {
      for ( i = 0; i < _len; ++i ) {
         if ( _data[i] != src[i] )
            return false;
      }
      return true;
   }

   return false;
}

bool
MyString::operator == ( const char* src ) const {
   int len = strlen( src ),
       i;

   if ( _len == len ) {
      for ( i = 0; i < len; ++i ) {
         if ( _data[i] != src[i] )
            return false;
      }
      return true;
   }

   return false;
}

bool
MyString::operator != ( const MyString& src ) const {
   int i;
   
   if ( _len == src._len ) {
      for ( i = 0; i < _len; ++i ) {
         if ( _data[i] != src[i] )
            return true;
      }
      return false;
   }
   return true;
}

bool
MyString::operator < ( const MyString& src ) const {
   string a, b;

   a = str();
   b = src.str();

   return a < b;
}

bool
MyString::operator > ( const MyString& src ) const {
   string a, b;

   a = str();
   b = src.str();

   return a > b;
}

const char*
MyString::str() const {
   return _data;
}

char 
MyString::operator [] ( int index ) const {
   if ( index < _len ) {
      return _data[ index ];
   }
   return ' ';   
}

int 
MyString::length() const {
   return _len;
}

int 
MyString::toInt() const {
   int base;
   if ( _data[0] == '-' ) 
      base = 1;
   else
      base = 0;
   int tmp = 0;

   for ( int i = base; i < 12; ++i ) {
      if ( _data[ i ] < '0' || _data[ i ] > '9' ) 
         break;

      tmp += _data[i] - '0';
      tmp *= 10;
   }

   if ( _data[0] == '-' )
      tmp *= -1;

   tmp /= 10;
   return tmp;
}

double 
MyString::toDouble() const {
   int    base;
   if ( _data[0] == '-' )
      base = 1;
   else
      base = 0;
   double tmp = 0.0;
   bool   dot = false;
   
   for ( int i = base; i < _len; ++i ) {
      if ( ( _data[ i ] < '0' || _data[ i ] > '9' ) && _data[i] != '.' )
         break;

      if ( _data[i] == '.' ) {
         dot = true;
         base = 1;
         tmp /= 10;
      }
      else {
         if ( dot == false ) {
            tmp += (double)( _data[i] - '0' );
            tmp *= 10;
         }
         else {
            tmp += (double)( _data[i] - '0' ) * ( pow( 10.0, base * -1 ) );
            base++;
         }
      }
   }

   if ( _data[0] == '-' )
      tmp *= -1.0;

   return tmp;
}

ostream& 
operator << ( ostream& out, const MyString& src ) {
   for ( int i = 0; i < src._len; ++i ) {
      out << src._data[ i ];
   }

   return out;
}

#endif

