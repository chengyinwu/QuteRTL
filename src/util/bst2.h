/****************************************************************************
  FileName     [ bst2.h ]
  Synopsis     [ header of binary search tree ]
  Author       [ Chi-Wen (Kevin) Chang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
  Others       [ Operator ==, =, and < of key field must be overloaded. ]
****************************************************************************/

#ifndef _UTIL_BST2_HEADER
#define _UTIL_BST2_HEADER

#include <iostream>
#include <vector>

using namespace std;

template< class keyType, class dataType >
struct bst2_cell {
   keyType                         _key;
   dataType                        _data;
   bst2_cell< keyType, dataType > *_left,
                                  *_right;
};

template< class Type, class Data >
class bst2 {
public:
   bst2();
   ~bst2();
   void insert( const Type &, const Data & );
   void remove( const Type & );

   bool exist( const Type & ) const;
 
   bool getData( const Type &, Data & ) const;

   void dfs() const;
   void dfs_key( vector< Type > & ) const;
   void dfs_data( vector< Data > & ) const;

   int getSize() const;

   void clear();
private:
   bst2_cell< Type, Data > *_root;
   int                     _size;
   bool bst_insert( bst2_cell< Type, Data > *, const Type &, const Data & );
   bool bst_delete( bst2_cell< Type, Data > *, bst2_cell< Type, Data > *, bool, const Type & );

   bst2_cell< Type, Data >* delete_min( bst2_cell< Type, Data > *, bst2_cell< Type, Data > *, bool );

   void delete_all( bst2_cell< Type, Data > * );
   void bst_dfs( bst2_cell< Type, Data > * ) const;
   void bst_dfs_key( bst2_cell< Type, Data > *, vector< Type > & ) const;
   void bst_dfs_data( bst2_cell< Type, Data > *, vector< Data > & ) const;
};

template< class Type, class Data >
bst2< Type, Data >::bst2() 
{
   _root = NULL;
   _size = 0;
}

template< class Type, class Data >
bst2< Type, Data >::~bst2() 
{
   if ( _root != NULL ) 
      delete_all( _root );
}

template< class Type, class Data >
void
bst2< Type, Data >::insert( const Type &src, const Data &data ) 
{ 
   if ( _root == NULL ) 
   {
      _root         = new bst2_cell< Type, Data >;
      _root->_key   = src;
      _root->_data  = data;
      _root->_left  = NULL;
      _root->_right = NULL;
      _size         = 1;
   }   
   else {
      if ( bst_insert( _root, src, data ) ) 
         _size++;
   }
}

template< class Type, class Data >
void
bst2< Type, Data >::remove( const Type &src ) 
{
   if ( bst_delete( _root, NULL, false, src ) ) 
      _size--;
}

template< class Type, class Data >
bool
bst2< Type, Data >::exist( const Type &src ) const 
{
   bst2_cell< Type, Data > *tmp;

   tmp = _root;
   while ( tmp != NULL ) 
   {
      if ( tmp->_key == src ) 
         return true;
      else if ( src < tmp->_key ) 
         tmp = tmp->_left;
      else 
         tmp = tmp->_right;
   }  

   return false;
}

template< class Type, class Data >
bool
bst2< Type, Data >::getData( const Type &key, Data &data ) const 
{
   bst2_cell< Type, Data > *tmp;

   tmp = _root;
   while ( tmp != NULL ) 
   {
      if ( tmp->_key == key ) 
      {
         data = tmp->_data;
         return true;
      }
      else if ( key < tmp->_key ) 
      {
         tmp = tmp->_left;
      }
      else 
      {
         tmp = tmp->_right;
      }
   }

   return false;
}

template< class Type, class Data >
int
bst2< Type, Data >::getSize() const 
{
   return _size;
}

template< class Type, class Data >
void
bst2< Type, Data >::clear() {
   delete_all( _root );
   _root = NULL;
}

template< class Type, class Data >
void
bst2< Type, Data >::dfs() const 
{
   bst_dfs( _root );
   cout << endl;
}

template< class Type, class Data >
void
bst2< Type, Data >::dfs_key( vector< Type > &pool ) const 
{
   bst_dfs_key( _root, pool );
}

template< class Type, class Data >
void
bst2< Type, Data >::dfs_data( vector< Data > &pool) const 
{
   bst_dfs_data( _root, pool );
}


template< class Type, class Data >
bool
bst2< Type, Data >::bst_insert( bst2_cell< Type, Data > *root, const Type &src, const Data &data ) 
{
   if ( root->_key == src ) 
   {
      return false;
   }
   else if ( src < root->_key ) 
   {
      if ( root->_left == NULL ) 
      {
         root->_left         = new bst2_cell< Type, Data >;
         root->_left->_key   = src;
         root->_left->_data  = data;
         root->_left->_left  = NULL;
         root->_left->_right = NULL;

         return true;
      }
      else 
      {
         return bst_insert( root->_left, src, data );
      }
   }
   else {
      if ( root->_right == NULL ) 
      {
         root->_right         = new bst2_cell< Type, Data >;
         root->_right->_key   = src;
         root->_right->_data  = data;
         root->_right->_left  = NULL;
         root->_right->_right = NULL;

         return true;
      }
      else 
      {
         return bst_insert( root->_right, src, data );
      }
   }
}

template< class Type, class Data >
bool
bst2< Type, Data >::bst_delete( bst2_cell< Type, Data > *root, bst2_cell< Type, Data > *prev, bool dir, const Type &src ) 
{
   bst2_cell< Type, Data > *tmp;

   if ( root == NULL ) 
   {
      return false;
   }
   if ( src == root->_key ) 
   {
      if ( prev == NULL ) 
      {
         tmp = delete_min( _root->_right, _root, true );

         if ( tmp != NULL ) 
         {
            tmp->_left  = root->_left;
            tmp->_right = root->_right;
            _root       = tmp;
         }
         else 
         {
            _root = NULL;
         }

         delete root;
      }
      else 
      {
         if ( root->_left == NULL && root->_right == NULL ) 
         {
            if ( dir ) 
               prev->_right = NULL;
            else 
               prev->_left = NULL;
         }
         else if ( root->_left == NULL && root->_right != NULL ) 
         {
            if ( dir ) 
               prev->_right = root->_right;
            else 
               prev->_left = root->_right;
         }
         else if ( root->_left != NULL && root->_right == NULL ) 
         {
            if ( dir ) 
               prev->_right = root->_left;
            else 
               prev->_left = root->_left;
         }
         else 
         {
            tmp = delete_min( root->_right, root, true );

            tmp->_left  = root->_left;
            tmp->_right = root->_right;

            if ( dir ) 
               prev->_right = tmp;
            else 
               prev->_left = tmp;
         }

         delete root;
      }
   }
   else if ( src < root->_key ) 
   {
      return bst_delete( root->_left, root, false, src );
   }
   else 
   {
      return bst_delete( root->_right, root, true, src );
   }

   return true;
}

template< class Type, class Data >
bst2_cell< Type, Data > *
bst2< Type, Data >::delete_min( bst2_cell< Type, Data > *root, bst2_cell< Type, Data > *prev, bool dir ) 
{
   if ( root == NULL ) 
   {
      return NULL;
   }
   else if ( root->_left != NULL ) 
   {
      return delete_min( root->_left, root, false );
   }
   else 
   {
      if ( dir ) 
         prev->_right = root->_right;
      else
         prev->_left = root->_right;

      return root;
   }
}

template< class Type, class Data >
void
bst2< Type, Data >::delete_all( bst2_cell< Type, Data > *root ) 
{
   if ( root->_left != NULL ) 
   {
      delete_all( root->_left );
      root->_left = NULL;
   }

   if ( root->_right != NULL ) 
   {
      delete_all( root->_right );
      root->_right = NULL;
   }
   delete root;
}

template< class Type, class Data >
void
bst2< Type, Data >::bst_dfs( bst2_cell< Type, Data > *root ) const 
{
   if ( root->_left != NULL )
      bst_dfs( root->_left );
   
   if ( root->_right != NULL )
      bst_dfs( root->_right );

   cout << root->_key << ", ";
}

template< class Type, class Data >
void
bst2< Type, Data >::bst_dfs_key( bst2_cell< Type, Data > *root, vector< Type > &pool ) const 
{
   if ( root->_left != NULL )
      bst_dfs_key( root->_left, pool );

   if ( root->_right != NULL )
      bst_dfs_key( root->_right, pool );

   pool.push_back( root->_key );
}

template< class Type, class Data >
void
bst2< Type, Data >::bst_dfs_data( bst2_cell< Type, Data > *root, vector< Data > &pool ) const 
{
   if ( root->_left != NULL )
      bst_dfs_data( root->_left, pool );

   if ( root->_right != NULL )
      bst_dfs_data( root->_right, pool );

   pool.push_back( root->_data );
}

#endif

