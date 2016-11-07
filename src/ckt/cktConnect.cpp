/****************************************************************************
  FileName     [ cktConnection.cpp ]
  Package      [ ckt ]
  Synopsis     [ Member function of IOPinConn class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_CONNECT_SOURCE
#define CKT_CONNECT_SOURCE

#include <cassert>
#include "cktConnect.h"
#include "cktPin.h"
#include "cktCell.h" //debug use

extern CktHierIoMap hierIoMap;


IOPinConn::IOPinConn(CktOutPin* o)
{
   _outPin = o;
   _inPinSet.clear();
   _isTraverse = false;
} // need copy constructor

IOPinConn::~IOPinConn()                  
{ 
   _outPin = 0;
   _inPinSet.clear(); 
   _hierNameSet.clear();
   _isTraverse = false;
}

void 
IOPinConn::insert(CktInPin* i)      
{ 
   _inPinSet.insert(i);    
}

void 
IOPinConn::insert(string& s)        
{ 
   _hierNameSet.insert(s); 
}

void 
IOPinConn::setOutPin(CktOutPin* o)  
{ 
   _outPin = o; 
}

bool 
IOPinConn::isStart() const
{ 
   return (_outPin ? true : false); 
}

CktOutPin* 
IOPinConn::getOutPin() const
{
   return _outPin;
}

void 
IOPinConn::setTraverse()
{
   _isTraverse = true;
}

bool
IOPinConn::isTraverse() const
{
   return _isTraverse;
}

void
IOPinConn::dfsConnect(CktOutPin* outPin) const
{
   assert (isTraverse() == false);
   //if (isTraverse() == true) return;

   _isTraverse = true;
   IOPinConn* toPinConnect;

   for (set<CktInPin*>::iterator cPos = _inPinSet.begin(); cPos != _inPinSet.end(); cPos++) {
      assert ((*cPos)->getOutPin() == 0);
      outPin->connInPin(*cPos); (*cPos)->connOutPin(outPin);
   }

   for (set<string>::const_iterator sPos = _hierNameSet.begin(); sPos != _hierNameSet.end(); sPos++) {
      //assert (hierIoMap.find(*sPos) != hierIoMap.end());
      if (hierIoMap.find(*sPos) != hierIoMap.end()) { // no connect pin (float pin) => skip
         toPinConnect = hierIoMap[*sPos];
         toPinConnect->dfsConnect(outPin);
      }
      else
         cout << "Warning : Exist Float Pins !! Hierarchical Pin Name : " << *sPos << endl;
   }
} 

void 
IOPinConn::printDebugInfo() const
{
   cout << "==============================S================================" << endl;
   cout << "_outPin = " << _outPin << endl;
   for (set<CktInPin*>::iterator cPos = _inPinSet.begin(); cPos != _inPinSet.end(); cPos++) {
      cout << "CktInPin = " << (*cPos) << ", Cell Type = " 
           << (*cPos)->getCell()->getCellType() 
           << ", Cell OutPin Name = "<< (*cPos)->getCell()->getOutPin()->getName() << endl;
   }
   for (set<string>::const_iterator sPos = _hierNameSet.begin(); sPos != _hierNameSet.end(); sPos++) 
      cout << "hierName = " << (*sPos) << endl;
   cout << "==============================E================================" << endl;
}

#endif
