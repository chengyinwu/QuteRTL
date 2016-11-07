/******************************************************************************
  FileName    [ myString.h ]
  PackageName [ utility/myString ]
  Synopsis    [ string class with funny operation ]
  Author      [ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2006 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/
#ifndef _MY_STRING_HEADER_
#define _MY_STRING_HEADER_

#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#define _STR_BLOCK_SIZE	32 // block size must >= 32

#define _PRECISION_LEN  10 // precision for double 

class MyString {
   public:
      MyString( const int );
      MyString( const unsigned );
      MyString( const double );
      MyString( const char* = NULL );
      MyString( const char );
      MyString( const string& );
      MyString( const MyString& );

      ~MyString();

      MyString operator + ( const int ) const;
      MyString operator + ( const unsigned ) const;
      MyString operator + ( const double ) const;
      MyString operator + ( const char* ) const;
      MyString operator + ( const char ) const;
      MyString operator + ( const string& ) const;
      MyString operator + ( const MyString& ) const;

      MyString& operator += ( const int );
      MyString& operator += ( const unsigned );
      MyString& operator += ( const double );
      MyString& operator += ( const char* );
      MyString& operator += ( const char );
      MyString& operator += ( const string& );
      MyString& operator += ( const MyString& );

      MyString& operator = ( const int );
      MyString& operator = ( const unsigned );
      MyString& operator = ( const double );
      MyString& operator = ( const char* );
      MyString& operator = ( const char );
      MyString& operator = ( const string& );
      MyString& operator = ( const MyString& );

      MyString& operator << ( const int );
      MyString& operator << ( const unsigned );
      MyString& operator << ( const double );
      MyString& operator << ( const char* );
      MyString& operator << ( const char );
      MyString& operator << ( const string& );
      MyString& operator << ( const MyString& );

      MyString substr( unsigned int, unsigned int ) const;
      bool operator == ( const MyString& ) const;
      bool operator == ( const char * ) const;
      bool operator != ( const MyString& ) const;
      bool operator <  ( const MyString& ) const;
      bool operator >  ( const MyString& ) const;

      const char* str() const;

      char operator [] ( int ) const;

      int length() const;

      int toInt() const;
      double toDouble() const;

      friend ostream& operator << ( ostream&, const MyString& );
   private:
      int   _size,
            _len;
      char* _data;
};

#endif

