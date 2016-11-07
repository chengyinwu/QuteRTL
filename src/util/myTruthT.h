#ifndef _UTIL_MY_TRUTHT_HEADER
#define _UTIL_MY_TRUTHT_HEADER

#include <vector>

#include "myString.h"

using namespace std;

//-----------------------------------------------------------------------------
// MyBDDValue
//-----------------------------------------------------------------------------
class MyBDDValue {
public:
   MyBDDValue();
   MyBDDValue( const MyBDDValue& );

   void setName( const MyString& );
   MyString name() const;

   void setValue( const bool& );
   bool value() const;

   MyBDDValue& operator = ( const MyBDDValue& ) ;
   bool operator == ( const MyBDDValue& ) const;
   bool operator != ( const MyBDDValue& ) const;
   friend ostream& operator << ( ostream&, const MyBDDValue& );
private:
   MyString _name;
   bool     _value;
};

//-----------------------------------------------------------------------------
// MyBDDPath
//-----------------------------------------------------------------------------
class MyBDDPath {
public:
   MyBDDPath();

   void setValue( const bool& );
   void flipValue();
   bool value() const;

   void addNode( const MyBDDValue& );
   unsigned int nodeSize() const;
   MyBDDValue getNode( unsigned int ) const;

   MyBDDPath& operator = ( const MyBDDPath& );
   MyBDDValue operator [] ( const unsigned& ) const;
   friend ostream& operator << ( ostream&, const MyBDDPath& );
private:
   vector< MyBDDValue > _nodes;
   bool                 _value;
};

#endif

