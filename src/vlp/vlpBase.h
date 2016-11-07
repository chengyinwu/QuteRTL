/****************************************************************************
  FileName     [ vlpBase.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Base Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_BASE_H
#define VLP_BASE_H

#include "vlpEnum.h"

#include "myString.h"
#include "bddManager.h"
#include "bddNode.h"

#include "cktPin.h"

#include <string>
#include <iostream>
#include <set>

class VlgModule;
class CktOutPin;
class VlpSignalNode;

using namespace std;

class VlpBaseNode
{
public :
   VlpBaseNode(NodeClass);
   virtual ~VlpBaseNode();
   virtual bool setNext(VlpBaseNode* const); 
   virtual bool checkNext() const = 0;
   virtual const VlpBaseNode* getNext() const;
   const NodeClass getNodeClass() const;
   // CDFG
   virtual void whoAmI(int, int) const;
   virtual void writeWhoAmI(ofstream&) const;
   // FSM
   virtual MyString str() const;
   virtual set< MyString > setSIGs() const;
   virtual vector< VlpSignalNode* > listSIGs() const;
   virtual VlpBaseNode* duplicate() const;
   virtual vector< BddNode > bdd(BddManager*, int&, int&, const MyString) const;
   // SYNTHESIS      
   virtual CktOutPin* synthesis() const;
   virtual void       synthesis(bool&, bool&, CktOutPin*&) const;
   virtual CktOutPin* synthesisAssign(const VlgModule*) const; 
   virtual void varFlatten() const;
   virtual bool isConst() const;
   virtual int constPropagate() const;
   // MISC
   virtual string writeExp(set<string>&) const;
   static int _totalNode;
protected :
   NodeClass _nodeClass;
   int nodeNo;
private :
};

#endif

