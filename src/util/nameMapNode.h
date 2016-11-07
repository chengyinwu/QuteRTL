#ifndef _CKT_NAME_MAP_NODE_HEADER
#define _CKT_NAME_MAP_NODE_HEADER

#include <iostream>
#include <string>

using namespace std;

class NameMapNode {
   public:
      NameMapNode();
      NameMapNode( const char * );
      NameMapNode( const string & );
      ~NameMapNode();
     
      void setName( const char * );
      void setName( const string & );
      const string& getName();

      void setId( int = -1 );
      int getId();

      bool isNull();

      void operator =( const NameMapNode & );
      bool operator <( const NameMapNode & );
      bool operator >( const NameMapNode & );
      bool operator ==( const NameMapNode & );
   private:
      string _name;
      int    _id;
};

#endif

