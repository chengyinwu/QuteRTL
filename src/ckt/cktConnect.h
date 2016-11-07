/****************************************************************************
  FileName     [ cktConnect.h ]
  PackageName  [ ckt ]
  Synopsis     [ DS for ckt flatten ]
  Author       [ Hu-Hsi (Louis) Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ] 
****************************************************************************/

#ifndef CKT_CONNECT_H
#define CKT_CONNECT_H

#include <set>
#include <string>
#include "cktEnum.h"

using namespace std;

class CktOutPin;
class CktInPin;

//use in eliminating bufs and mapping cells between modules
class IOPinConn
{
public:
   IOPinConn(CktOutPin*); //need copy constructor
   ~IOPinConn();
   void insert(CktInPin*);
   void insert(string&); 
   void setOutPin(CktOutPin*);
   CktOutPin* getOutPin() const;
   bool isStart() const;
   void setTraverse();
   bool isTraverse() const;
   void dfsConnect(CktOutPin*) const;
   void printDebugInfo() const;
private:
   CktOutPin*      _outPin;
   set<CktInPin*>  _inPinSet;
   set<string>     _hierNameSet;
   mutable bool    _isTraverse;
};

#endif

