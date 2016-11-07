#ifndef _CKT_NAME_MAP_NODE_HEADER
#define _CLT_NAME_MAP_NODE_HEADER

#include "nameMapNode.h"

NameMapNode::NameMapNode() 
{
   _name = "";
   _id = -1;
}

NameMapNode::NameMapNode( const char *src ) 
{
   _name = src;
   _id = -1;
}

NameMapNode::NameMapNode( const string &src ) 
{
   _name = src;
   _id = -1;
}

NameMapNode::~NameMapNode() 
{
}

void
NameMapNode::setName( const char *src ) 
{
   _name = src;
}

void
NameMapNode::setName( const string &src ) 
{
   _name = src;
}

const string&
NameMapNode::getName() 
{
   return _name;
}

void
NameMapNode::setId( int id ) 
{
   _id = id;
}

int
NameMapNode::getId() 
{
   return _id;
}

bool
NameMapNode::isNull() 
{
   return _id == -1 || _name == "";
}

void
NameMapNode::operator =( const NameMapNode &src ) 
{
   _name = src._name;
   _id = src._id;
}

bool
NameMapNode::operator <( const NameMapNode &src ) 
{
   return _name < src._name;
}

bool
NameMapNode::operator >( const NameMapNode &src ) 
{
   return _name > src._name;
}

bool
NameMapNode::operator ==( const NameMapNode &src ) 
{
   return _name == src._name;
}


#endif

