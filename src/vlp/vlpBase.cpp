/****************************************************************************
  FileName     [ vlpBase.cpp ]
  Package      [ vlp ]
  Synopsis     [ Member function of Base Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_BASE_SOURCE
#define VLP_BASE_SOURCE

#include "VLGlobe.h"
#include "vlpBase.h"

using namespace std;
extern LY_usage* LY;

int VlpBaseNode::_totalNode = 0;

VlpBaseNode::VlpBaseNode(NodeClass nodeClass)
{
   _nodeClass = nodeClass;
   nodeNo = ++_totalNode;
}

VlpBaseNode::~VlpBaseNode()
{
}

bool 
VlpBaseNode::setNext(VlpBaseNode* const dummy) 
{ 
   cout << "Error setNext() in vlpBase.h at line " << LY->lineNo << endl; 
   return false;
}
      
const NodeClass 
VlpBaseNode::getNodeClass() const 
{ 
   return _nodeClass; 
}

const VlpBaseNode* 
VlpBaseNode::getNext() const 
{ 
   cout << "error getNext() in vlpBase.h" << endl; 
   return NULL; 
}
     
void 
VlpBaseNode::whoAmI(int i, int l) const 
{ 
   cout << "Error : I am VlpBaseNode" << endl; 
}

void
VlpBaseNode::writeWhoAmI(ofstream& os) const
{
   cout << "Error : I am VlpBaseNode" << endl; 
}

MyString
VlpBaseNode::str() const {
   return "";
}

set< MyString >
VlpBaseNode::setSIGs() const {
   set< MyString > tmp;
   return tmp;
}

vector< VlpSignalNode* >
VlpBaseNode::listSIGs() const {
   vector< VlpSignalNode* > tmp;
   return tmp;
}

VlpBaseNode*
VlpBaseNode::duplicate() const {
   return NULL;
}

vector< BddNode >
VlpBaseNode::bdd( BddManager* bm, int& top, int& bottom,
                  const MyString sig ) const {
   vector< BddNode > ret;

   ret.push_back( BddNode::_error );

   return ret;
}

string 
VlpBaseNode::writeExp(set<string>& finSigs) const
{
   cout << "Error : In VlpBase::writeExp() NodeClass = " 
        << _nodeClass << endl; 
   assert (0);
}

#endif
