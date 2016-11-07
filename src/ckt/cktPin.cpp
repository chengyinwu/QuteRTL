/****************************************************************************
  FileName     [ cktPin.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit pin data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_PIN_SOURCE
#define CKT_PIN_SOURCE

#include "VLGlobe.h"
#include "cktPin.h"
#include "synVar.h"

extern VlpDesign&       VLDesign;
extern SYN_usage*       SYN;
extern const VlgModule* curVlpModule;

// ========== CktInPin ==========

CktInPin::CktInPin()
{
   _cell    = NULL;
   _fromPin = NULL;
}
   
CktInPin::~CktInPin()
{
   _cell    = 0;
   _fromPin = 0;
}

void
CktInPin::connCell(CktCell* cell)
{
   _cell = cell;
}

CktCell *
CktInPin::getCell() const
{
   return _cell;
}

CktCell*    
CktInPin::getFinCell() const 
{ 
   return _fromPin->getCell();
}

void
CktInPin::connOutPin(CktOutPin* pin)
{
   _fromPin = pin;
}

CktOutPin *
CktInPin::getOutPin() const
{
   return _fromPin;
}

CktInPin*   
CktInPin::pinDuplicate(CktCell*& new_cell, InPinMap& inPinMap) const
{
   CktInPin* new_pin = new CktInPin();
   new_pin->connCell(new_cell);
   new_cell->connInPin(new_pin);
   inPinMap[this] = new_pin;
   return new_pin;
}

// ========== CktOutPin ==========

CktOutPin::CktOutPin()
{
   _netName  = "";
   _cell     = 0;
   _busId    = 0;
   _refCtr   = 1;
   _coutBits = 0;
   _toPinList.clear();
}

CktOutPin::~CktOutPin() 
{
   _toPinList.clear();
   _netName  = "";
   _cell     = 0;
   _busId    = 0;
   _coutBits = 0;
   _refCtr   = 0;
}

void
CktOutPin::setName(CktString name)
{
   _netName = name;
}

CktString
CktOutPin::getName() const
{
   return _netName;
}

void
CktOutPin::connCell(CktCell* cell)
{
   _cell = cell;
}

CktCell*
CktOutPin::getCell() const
{
   return _cell;
}

CktCell*  
CktOutPin::getFoutCell(unsigned i) const 
{ 
   return getInPin(i)->getCell(); 
}

void
CktOutPin::connInPin(CktInPin* pin)
{
   _toPinList.push_back(pin);
}

void
CktOutPin::eraseInPin(CktInPin* pin) {
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      if (pin == _toPinList[i]) {
         _toPinList.erase(_toPinList.begin() + i);
         return;
      }
   }
}

CktInPin*
CktOutPin::getInPin(unsigned index) const
{
   assert(index < _toPinList.size());
   return _toPinList[index];
}

unsigned
CktOutPin::getInPinSize() const
{    
   return _toPinList.size();
}

void
CktOutPin::setBusId(unsigned busId)
{
   _busId = busId;
}

unsigned
CktOutPin::getBusId() const
{
   return _busId;
}

unsigned
CktOutPin::getBusBegin() const {
   const SynBus* bus = VLDesign.getBus(getBusId());
   return bus->getBegin();
}

unsigned
CktOutPin::getBusEnd() const {
   const SynBus* bus = VLDesign.getBus(getBusId());
   return bus->getEnd();
}

unsigned
CktOutPin::getBusWidth() const
{
   const SynBus* bus = VLDesign.getBus(getBusId());
   return bus->getWidth();
}

unsigned
CktOutPin::getBusMSB() const {
   const SynBus* bus = VLDesign.getBus(getBusId());
   return bus->isInverted() ? bus->getEnd() : bus->getBegin();
}

unsigned
CktOutPin::getBusLSB() const {
   const SynBus* bus = VLDesign.getBus(getBusId());
   return bus->isInverted() ? bus->getBegin() : bus->getEnd();
}

void
CktOutPin::erase(CktInPin* inPin)
{
   CktInPinAry::iterator pos;

   pos = find(_toPinList.begin(), _toPinList.end(), inPin);
   if (pos != _toPinList.end())
      _toPinList.erase(pos);
}

void
CktOutPin::refCtrIncre() 
{
   ++_refCtr;
}

void
CktOutPin::refCtrDecre()
{
  --_refCtr;
}

void
CktOutPin::setRefCtr(int refCtr)
{
   _refCtr = refCtr;
}

int
CktOutPin::getRefCtr() const
{
   return _refCtr;
}

void
CktOutPin::setCoutBits(int coutBits)
{
   _coutBits = coutBits;
}

int
CktOutPin::getCoutBits() const
{
   return _coutBits;
}

CktOutPin*   
CktOutPin::pinDuplicate(CktCell*& new_cell, string& prefix, OutPinMap& outPinMap) const
{
   CktOutPin* new_pin = new CktOutPin();
   string new_name;;
   if (prefix == "top" && _netName.size())
      new_name = _netName;
   else
      new_name = prefix + "_" + _netName;

   new_pin->setName(new_name);
   new_pin->setBusId(getBusId());
   new_pin->setCoutBits(getCoutBits());
   new_pin->setRefCtr(getRefCtr());

   new_pin->connCell(new_cell);
   new_cell->connOutPin(new_pin);
   outPinMap[this] = new_pin;
   return new_pin;
}

bool 
CktOutPin::is1BitIoPin() const
{  
   unsigned width = VLDesign.getBus(getBusId())->getWidth();
   if (width > 1)
      return false;
   else {
      if (_cell->getCellType() == CKT_PI_CELL)
         return true;
      if (_cell->getCellType() == CKT_PO_CELL)
         return true;
      if (_cell->getCellType() == CKT_PIO_CELL)
         return true;
      for (unsigned i = 0; i < getInPinSize(); ++i) {
         if ((getFoutCell(i)->getCellType() == CKT_PO_CELL)
         ||  (getFoutCell(i)->getCellType() == CKT_PO_CELL)) {
            return true;
         }
      }
      return false;
   }
}

#endif


