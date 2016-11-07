/****************************************************************************
  FileName     [ synBaseNode.cpp ]
  Package      [ syn ]
  Synopsis     [ Function for synthesis in class VlpBaseNode ]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_BASE_NODE_SOURCE
#define SYN_BASE_NODE_SOURCE

//---------------------------
//  system include
//---------------------------
#include <iostream>
#include <string>

using namespace std;

//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktLogicCell.h"
#include "vlpBase.h"

//vlp ds
//---------------------------
//  Global variables
//---------------------------
extern SYN_usage*    SYN;
extern bool          busDirectionDefault;
extern VlpDesign&    VLDesign;

//flatten all rhs to corresponding SynVar
void
VlpBaseNode::varFlatten() const
{
   Msg(MSG_ERR) << "Calling virtual function VlpBaseNode::varFlatten() (Type = " << getNodeClass() << " !!!" << endl;
}

CktOutPin* 
VlpBaseNode::synthesis() const 
{ 
   Msg(MSG_ERR) << "Calling virtual function VlpBaseNode::synthesis() (Type = " << getNodeClass() << " !!!" << endl;
   return 0;
}
      
void 
VlpBaseNode::synthesis(bool&, bool&, CktOutPin*&) const 
{ 
   Msg(MSG_ERR) << "Calling virtual function VlpBaseNode::synthesis() (Type = " << getNodeClass() << " !!!" << endl;
}
   
CktOutPin* 
VlpBaseNode::synthesisAssign(const VlgModule*) const 
{ 
   Msg(MSG_ERR) << "Calling virtual function VlpBaseNode::synthesisAssign() (Type = " << getNodeClass() << " !!!" << endl;
   return 0;
}

bool
VlpBaseNode::isConst() const
{
   Msg(MSG_ERR) << "Calling virtual function VlpBaseNode::isConst() (Type = " << getNodeClass() << " !!!" << endl;
   return false;
}

int
VlpBaseNode::constPropagate() const
{
   Msg(MSG_ERR) << "Calling virtual function VlpBaseNode::constPropagate() (Type = " << getNodeClass() << " !!!" << endl;
   return -1;
}

#endif
