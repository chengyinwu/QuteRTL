#ifndef _CKT_NAME_MAP_BST_NODE_SOURCE
#define _CKT_NAME_MAP_BST_NODE_SOURCE

#include "nameMapBstNode.h"

NameMapBstNode::NameMapBstNode( NameMapNode *src ) 
{
   _node = src;
   _left = NULL;
   _right = NULL;
}

NameMapBstNode::~NameMapBstNode() 
{
   if ( _node != NULL ) delete _node;
   if ( _left != NULL ) delete _left;
   if ( _right != NULL ) delete _right;
}

void
NameMapBstNode::setLeft( NameMapBstNode *src ) 
{
   _left = src;
}

void
NameMapBstNode::setRight( NameMapBstNode *src ) 
{
   _right = src;
}

NameMapNode *
NameMapBstNode::getNode() 
{
   return _node;
}

NameMapBstNode *
NameMapBstNode::getLeft() 
{
   return _left;
}

NameMapBstNode *
NameMapBstNode::getRight() 
{
   return _right;
}

bool
NameMapBstNode::isLeftNull() 
{
   return _left == NULL;
}

bool
NameMapBstNode::isRightNull() 
{
   return _right == NULL;
}

#endif

