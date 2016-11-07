#ifndef _CKT_NAME_MAP_SOURCE
#define _CKT_NAME_MAP_SOURCE

#include "nameMap.h"

NameMap::NameMap() 
{
   _root = NULL;
   _pool.clear();
   _pool.push_back( "" );
   _count = 0;
}

NameMap::~NameMap() 
{
   if ( _root != NULL ) delete _root;
   _pool.clear();
}

int
NameMap::insert( char *src ) 
{
   NameMapNode *go = new NameMapNode( src );
   int id;
   if ( _root == NULL ) {
      _root = new NameMapBstNode( go );
      _root->getNode()->setId( _count );
      _pool.push_back( string(src) );
      return _count++;
   }
   
   id = insertBst( _root, *go );

   if ( id != _count - 1 )
      delete go;

   return id;
}

int
NameMap::insert( const string &src ) 
{
   NameMapNode *go = new NameMapNode( src );
   int id;   

   if ( _root == NULL ) {
      _root = new NameMapBstNode( go );
      _root->getNode()->setId( _count );
      _pool.push_back( src );
      return _count++;
   }

   id = insertBst( _root, *go );
    
   if ( id != _count - 1 )
      delete go;
   
   return id;
}

const string&
NameMap::getName( int id ) const 
{
   //cout << ( id + 1 ) << "\t" << _count << endl;//marked by Louis
   if ( ( id + 1 ) > _count ) {
      cerr << "NameMap::getName() ---> Warning: Index out of range, return an empty name." << endl;
      return _pool[ 0 ];
   }
   else {
      return _pool[ id + 1 ];
   }
}

int
NameMap::insertBst( NameMapBstNode *base, NameMapNode &target ) 
{
   if ( *base->getNode() < target ) { 
      if ( base->isLeftNull() ) {
        target.setId( _count );
        base->setLeft( new NameMapBstNode( &target ) );
        _pool.push_back( target.getName() );
        return _count++;
      }
      else {
        return insertBst( base->getLeft(), target );
      }
   }
   else if ( *base->getNode() > target ) {
      if ( base->isRightNull() ) {
        target.setId( _count );
        base->setRight( new NameMapBstNode( &target ) );
        _pool.push_back( target.getName() );
        return _count++;
      }
      else {
        return insertBst( base->getRight(), target );
      }
   }
   else {
      return base->getNode()->getId();
   }
}

#ifdef DEBUG
void
NameMap::listAll() 
{
   cout << "Id       Name" << endl;
   cout << "======== ===================" << endl;
   for ( int i = 1; i <= _count; i++ ) {
      cout << setw(8) << i - 1 << " " <<_pool[i] << endl;
   }
   cout << "======== ===================" << endl;
}
#endif

#endif

