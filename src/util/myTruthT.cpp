#ifndef _UTIL_MY_TRUTHT_SOURCE
#define _UTIL_MY_TRUTHE_SOURCE

#include "myTruthT.h"

//-----------------------------------------------------------------------------
// MyBDDValue
//-----------------------------------------------------------------------------
MyBDDValue::MyBDDValue() {
   _name  = "";
   _value = false;
}

MyBDDValue::MyBDDValue( const MyBDDValue& src ) {
   _name  = src._name;
   _value = src._value;
}

void
MyBDDValue::setName( const MyString& src ) {
   _name = src;
}

MyString
MyBDDValue::name() const {
   return _name;
}

void
MyBDDValue::setValue( const bool& src ) {
   _value = src;
}

bool
MyBDDValue::value() const {
   return _value;
}

MyBDDValue&
MyBDDValue::operator = ( const MyBDDValue& src ) {
   _name  = src._name;
   _value = src._value;

   return *this;
}

bool
MyBDDValue::operator == ( const MyBDDValue& src ) const {
   return (_name == src._name) && (_value == src._value);
}

bool
MyBDDValue::operator != ( const MyBDDValue& src ) const {
   return (_name != src._name) || (_value != src._value);
}

ostream&
operator << ( ostream& os, const MyBDDValue& src ) {
   os << src._name << "( ";
   if ( src._value )
      os << "1";
   else
      os << "0";

   os << " )";

   return os;
}
//-----------------------------------------------------------------------------
// MyBDDPath
//-----------------------------------------------------------------------------
MyBDDPath::MyBDDPath() {
   _nodes.clear();
   _value = false;
}

void
MyBDDPath::setValue( const bool& src ) {
   _value = src;
}

void
MyBDDPath::flipValue() {
   if ( _value )
      _value = false;
   else
      _value = true;
}

bool
MyBDDPath::value() const {
   return _value;
}

void
MyBDDPath::addNode( const MyBDDValue& src ) {
   _nodes.push_back( src );
}

unsigned int
MyBDDPath::nodeSize() const {
   return _nodes.size();
}

MyBDDValue
MyBDDPath::getNode( unsigned int index ) const {
   MyBDDValue tmp;

   if ( index < _nodes.size() )
      return _nodes[index];
   else
      return tmp;
}

MyBDDPath&
MyBDDPath::operator = ( const MyBDDPath& src ) {
   _nodes = src._nodes;
   _value = src._value;

   return *this;
}

MyBDDValue
MyBDDPath::operator [] ( const unsigned& index ) const {
   MyBDDValue tmp;

   if ( index < _nodes.size() )
      return _nodes[index];
   else
      return tmp;
}

ostream&
operator << ( ostream& os, const MyBDDPath& src ) {
   for ( int i = (int)src.nodeSize() - 1; i >=0; --i ) {
      os << src._nodes[i];
      if ( i > 0 )
         os << "  ";
   }

   os << " --> ";
   if ( src._value )
      os << "VALID";
   else
      os << "INVALID";

   return os;
}

#endif

