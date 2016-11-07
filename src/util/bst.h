/****************************************************************************
  FileName     [ bst.h ]
  Synopsis     [ header of binary search tree ]
  Author       [ Chi-Wen (Kevin) Chang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
  Others       [ Operator ==, =, and < of key field must be overloaded. ]
****************************************************************************/

#ifndef _BST_HEADER
#define _BST_HEADER

#include <iostream>
#include <vector>

using namespace std;

template< class dataType >
struct bst_cell 
{
   dataType              _key;
   bst_cell< dataType > *_left,
                        *_right;
};

template< class Type >
class bst {
public:
   bst();
   ~bst();
   void insert( const Type & );
   void remove( const Type & );

   bool exist( const Type & ) const;
   void dfs() const;
   void dfs( vector< Type > & ) const;

   int getSize() const;
private:
   bst_cell< Type > *_root;
   int              _size;

   bool bst_insert( bst_cell< Type > *, const Type & );
   bool bst_delete( bst_cell< Type > *, bst_cell< Type > *, bool, const Type & );
   bst_cell< Type >* delete_min( bst_cell< Type > *, bst_cell< Type > *, bool );
   void delete_all( bst_cell< Type > * );
   void bst_dfs( bst_cell< Type > * ) const;
   void bst_dfs2( bst_cell< Type > *, vector< Type > & ) const;
};

template< class Type >
bst< Type >::bst() 
{
   _root = NULL;
   _size = 0;
}

template< class Type >
bst< Type >::~bst() 
{
   if ( _root != NULL ) 
      delete_all( _root );
}

template< class Type >
void
bst< Type >::insert( const Type &src ) 
{ 
   if ( _root == NULL ) 
   {
      _root         = new bst_cell< Type >;
      _root->_key   = src;
      _root->_left  = NULL;
      _root->_right = NULL;
      _size = 1;
   }   
   else 
   {
      if ( bst_insert( _root, src ) ) 
         _size++;
   }
}

template< class Type >
void
bst< Type >::remove( const Type &src ) 
{
   if ( bst_delete( _root, NULL, false, src ) )  
      _size--;
}

template< class Type >
bool
bst< Type >::exist( const Type &src ) const 
{
   bst_cell< Type > *tmp;

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

template< class Type >
int
bst< Type >::getSize() const 
{
   return _size;
}

template< class Type >
void
bst< Type >::dfs() const 
{
   bst_dfs( _root );
   cout << endl;
}

template< class Type >
void
bst< Type >::dfs( vector< Type > &pool ) const 
{
   bst_dfs2( _root, pool );
}

template< class Type >
bool
bst< Type >::bst_insert( bst_cell< Type > *root, const Type &src ) 
{
   if ( root->_key == src ) 
   {
      return false;
   }
   else if ( src < root->_key ) 
   {
      if ( root->_left == NULL ) 
      {
         root->_left         = new bst_cell< Type >;
         root->_left->_key   = src;
         root->_left->_left  = NULL;
         root->_left->_right = NULL;
         return true;
      }
      else 
      {
         return bst_insert( root->_left, src );
      }
   }
   else 
   {
      if ( root->_right == NULL ) 
      {
         root->_right         = new bst_cell< Type >;
         root->_right->_key   = src;
         root->_right->_left  = NULL;
         root->_right->_right = NULL;

         return true;
      }
      else 
      {
         return bst_insert( root->_right, src );
      }
   }
}

template< class Type >
bool
bst< Type >::bst_delete( bst_cell< Type > *root, bst_cell< Type > *prev, bool dir, const Type &src ) 
{
   bst_cell< Type > *tmp;

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
            tmp         = delete_min( root->_right, root, true );
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

template< class Type >
bst_cell< Type > *
bst< Type >::delete_min( bst_cell< Type > *root, bst_cell< Type > *prev, bool dir ) 
{
   if ( root == NULL ) 
      return NULL;

   if ( root->_left != NULL ) 
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

template< class Type >
void
bst< Type >::delete_all( bst_cell< Type > *root ) 
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

template< class Type >
void
bst< Type >::bst_dfs( bst_cell< Type > *root ) const 
{
   if ( root->_left != NULL )
      bst_dfs( root->_left );
   
   if ( root->_right != NULL )
      bst_dfs( root->_right );

   cout << root->_key << ", ";
}

template< class Type >
void
bst< Type >::bst_dfs2( bst_cell< Type > *root, vector< Type > &pool ) const 
{
   if ( root->_left != NULL )
      bst_dfs2( root->_left, pool );

   if ( root->_right != NULL )
      bst_dfs2( root->_right, pool );

   pool.push_back( root->_key );
}

#endif

