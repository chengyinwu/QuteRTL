#ifndef _CKT_NAME_MAP_HEADER
#define _CKT_NAME_MAP_HEADER

#include <iostream>
#include <string>
#include <vector>

#ifdef DEBUG
#include <iomanip>
#endif

#include "nameMapBstNode.h"

using namespace std;

typedef vector< string > NameMapPool;

class NameMap {
   public:
      NameMap();
      ~NameMap();
      int insert( char * );
      int insert( const string & );
      
      const string& getName( int ) const;
#ifdef DEBUG
      void listAll();
#endif
   private:
      NameMapBstNode *_root;
      NameMapPool    _pool;
      int _count;
      
      int insertBst( NameMapBstNode *, NameMapNode & );
};


#endif

