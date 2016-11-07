/****************************************************************************
  FileName     [ cktCell.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_CELL_SOURCE
#define CKT_CELL_SOURCE

#include "VLGlobe.h"
#include "cktCell.h"
#include "cktPin.h"
#include "vlpItoStr.h"
#include "synVar.h"
#include "cktModule.h"
#include "cktModuleCell.h"

#define PREFIX_CHAR "k"

extern VlpDesign& VLDesign;
extern const vector<paramPair>* curParamOverload;
extern CktModule* curCktModule;
extern CKT_usage* CREMap;

extern SimpleGraph<CktCell*> TraversalGraph;

Bv4 CktCell::_1bitOne  = "1'b1";
Bv4 CktCell::_1bitZero = "1'b0";
unsigned CktCell::_vtRef = 0;

const char *CktCellTypeStr[] = {
   "module",     // 0
   "pi",         // 1
   "po",         // 2
   "pio",        // 3
   "dff",        // 4
   "dlat",       // 5
   "buf",        // 6
   "inv",        // 7
   "and",        // 8
   "or",         // 9
   "nand",       // 10
   "nor",        // 11
   "xor",        // 12
   "xnor",       // 13
   "mux",        // 14
   "demux",      // 15
   "onehot",     // 16
   "case",       // 17
   "add",        // 18
   "sub",        // 29
   "mult",       // 20
   "div",        // 21
   "modulo",     // 22
   "shl",        // 23
   "shr",        // 24
   "rtl",        // 25
   "rtr",        // 26
   "bufif",      // 27
   "invif",      // 28
   "bus",        // 29
   "z2x",        // 30
   "const",      // 31
   "split",      // 32
   "merge",      // 33
   "inout",      // 34
   "weak",       // 35
   "equality",   // 36
   "geq",        // 37
   "greater",    // 38
   "leq",        // 39
   "less",       // 40
   "constant",   // 41
   "error",      // 42
   "dummy"       // 43
};

CktCell::CktCell( CktCellType type )
{
   _cellType = type; 
   _outPin   = NULL;
   _cellID   = 0;
   _util._util_p = NULL;
   _mask = 0;
   _level = 0;
   _vtNum = 0;
}

CktCell::CktCell(CktCellType type, bool isNewPin, CktInPin*& iPin, CktOutPin*& oPin)
{
   _cellType = type; 
   _cellID   = 0;
   _util._util_p = NULL;
   _mask = 0;
   if (isNewPin) {
      iPin = new CktInPin;
      oPin = new CktOutPin;
   }
   //connect between inPin & cell
   iPin->connCell(this); connInPin(iPin);
   //connect between cell & outPin
   oPin->connCell(this); connOutPin(oPin);
}

CktCell::CktCell(CktCellType type, bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY)
{
   _cellType = type; 
   _cellID   = 0;
   _util._util_p = NULL;
   _mask = 0;
   if (isNewPin) {
      iA = new CktInPin;
      iB = new CktInPin;
      oY = new CktOutPin;
   }
   //connect between inPin & cell
   iA->connCell(this); connInPin(iA);
   iB->connCell(this); connInPin(iB);
   //connect between cell & outPin
   oY->connCell(this); connOutPin(oY);
}

CktCell::~CktCell()
{
   if (_inPinAry.size() != 0) {
       for (CktInPinAry::iterator pos = _inPinAry.begin(); pos != _inPinAry.end(); ++pos) {
          delete *(pos);
          *(pos) = NULL;
       }
   }
   _inPinAry.clear();
   delete _outPin;
   _outPin = NULL;
}

void
CktCell::setType( CktCellType type ) 
{
   _cellType = type;
}

CktCellType
CktCell::getCellType() const 
{
   return _cellType;
}

string
CktCell::getCellTypeString() const
{
   return CktCellTypeStr[ _cellType ];
}

string
CktCell::getOpStr() const {
   cerr << "Error code ### - CktCellType: " << CktCellTypeStr[ _cellType ] << ", getOpStr in cktCell.h" << endl;
   return "";
}

string
CktCell::getExpr(CktOutPin* p) const {
   assert (p == getOutPin());
   if ((getCellType() == CKT_PI_CELL || getCellType() == CKT_DFF_CELL || getCellType() == CKT_DLAT_CELL || isExprBlock()) && (p->getName() != "")) 
      return p->getName();
   string opStr = getOpStr();
   if (getInPinSize() == 1) return opStr + _inPinAry[0]->getOutPin()->getCell()->getExpr(_inPinAry[0]->getOutPin());
   string expStr = "";
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      if (i) expStr += (" " + opStr + " ");
      expStr += getInPin(i)->getOutPin()->getCell()->getExpr(getInPin(i)->getOutPin());
   }
   return "(" + expStr + ")";
}

void
CktCell::connOutPin( CktOutPin *pin )
{
   _outPin = pin;
}

void
CktCell::connInPin( CktInPin *pin )
{
   _inPinAry.push_back( pin );
}
           
void 
CktCell::set_sUtil(size_t s)
{
   _util._util_t = s;
}
           
void 
CktCell::set_pUtil(void* p)
{
   _util._util_p = p;
}
           
size_t
CktCell::get_sUtil() const
{
   return _util._util_t;
}
           
void*
CktCell::get_pUtil() const
{
   return _util._util_p;
}

bool 
CktCell::isUseUtilP() const
{
   if (_util._util_p == NULL)
      return false;
   else
      return true;
}
   
void 
CktCell::setID(unsigned long& id)
{
   _cellID = id;
}
   
void 
CktCell::setValue(int v)
{
   assert (_cellType != CKT_MODULE_CELL);
   string str;
   if (v == -1)
      str = "1'bx";
   else if (v == 1)
      str = "1'b1";
   else if (v == 0)
      str = "1'b0";
   else
      assert(0);
   //cout << "set value cell address = " << this << endl;

   _simValue = str;
   unsigned outPinWidth;
   if (_cellType == CKT_SPLIT_CELL) 
      outPinWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   else 
      outPinWidth = VLDesign.getBus(_outPin->getBusId())->getWidth();

   assert (outPinWidth == _simValue.bits());
}

void 
CktCell::setValue(Bv4& inputValue) // usually used for CktPiCell (set up input pattern)
{
   assert (_cellType != CKT_MODULE_CELL && _cellType != CKT_SPLIT_CELL);
   const SynBus* bus = VLDesign.getBus(getOutPin()->getBusId());
   assert (inputValue.bits() == bus->getWidth());
#ifdef EVENT_DRIVEN_SIM
   setSimToggle(!(_simValue == inputValue));
#else
   setSimToggle(true);
#endif
   _simValue = inputValue;
}

int
CktCell::getValue() const
{
   if (_simValue.str() == "1'bx")        return -1;
   else if (_simValue.str() == "1'b1")   return 1;
   else if (_simValue.str() == "1'b0")   return 0;
   else {
      cout << "cell address = " << this << " sim str = " << _simValue.str() << endl;
      assert(0);
   }
}

CktOutPin*
CktCell::getOutPin() const
{
   return _outPin;
}

unsigned 
CktCell::getOutWidth() const
{
   return _outPin->getBusWidth();
}

unsigned   
CktCell::getFoutSize() const
{
   return _outPin->getInPinSize();
}

CktInPin*
CktCell::getInPin(unsigned index) const
{
   assert (index >= 0);
   assert (index < _inPinAry.size());

   return _inPinAry[index];
}

unsigned
CktCell::getInPinSize() const
{
   return _inPinAry.size();
}

unsigned
CktCell::getInPinNum(CktInPin* pin) const
{
   for (unsigned i = 0; i < _inPinAry.size(); ++i) {
      if (_inPinAry[i] == pin) return i;
   }
   return _inPinAry.size();  // If not found.
}

void
CktCell::resetInPinSize(const unsigned num) {
   _inPinAry.clear();
   for (unsigned i = 0; i < num; ++i) {
      connInPin(new CktInPin);
      getInPin(i)->connCell(this);
   }
}

CktCell* 
CktCell::getFinCell(unsigned i) const
{
   return (getInPin(i)->getOutPin()->getCell());
}

CktCell* 
CktCell::getFoutCell(unsigned i) const
{
   return (getOutPin()->getFoutCell(i));
}

CktCell *
CktCell::copy() const 
{
   CktCell* newCell = new CktCell(_cellType);
   return newCell;
}
   
string    
CktCell::getOutPinName() const
{
   if (getCellType() == CKT_MODULE_CELL)
      assert (0);
   else if (getCellType() == CKT_SPLIT_CELL) {
      unsigned u = 0;
      return static_cast<const CktSplitCell*>(this)->getOutPin(u)->getName();
   }
   else
      return getOutPin()->getName();
}

unsigned long  
CktCell::getCellID() const
{ 
   return _cellID; 
}

void 
CktCell::calLevel(unsigned lv)
{
   bool isUpdate;
   CktCell* finCell;
   if (setVtRef())        isUpdate = true;
   else if (_level < lv)  isUpdate = true;
   else                   isUpdate = false;

   if (isUpdate) {
      _level = lv;
      if (get_sUtil() == 0) // get_sUtil() != 0 ==> added mux for debugging
         ++lv;

      unsigned inPinSize = getInPinSize();
      for (unsigned i = 0; i < inPinSize; i++) {
         finCell = getFinCell(i);
         finCell->calLevel(lv);
      }
   }
}

void 
CktCell::collectFin(queue<CktCell*>& cellQueue) const
{
   CktInPin*  inPin;
   CktOutPin* outPin;
   CktCell*   finCell;
   unsigned inPinSize = getInPinSize();
   for (unsigned i = 0; i < inPinSize; i++) {
      inPin  = getInPin(i);
      outPin = inPin->getOutPin();
      if (outPin == 0 && getCellType() == CKT_MODULE_CELL)
         ; // Buf buf (.in(), ... ) ; or Buf buf ( , ...) ; => fanin cell = null
      else { //other condition : outPin must connect to a cell
         finCell = outPin->getCell();
         if (finCell != NULL) 
            cellQueue.push(finCell);
      }
   }
}

void 
CktCell::nameOutPin(int& num)
{
   string netName;
   if (_outPin->getName() == "") {
      netName = PREFIX_CHAR + toString(num);
      _outPin->setName(netName);
      setAddedCell();
      ++num;
   }
}

void
CktCell::writeOutput() const
{
   cerr << "Error code ### - CktCellType: " << CktCellTypeStr[_cellType] 
        << ", writeOutput() in cktCell.h" << endl;
}

void
CktCell::simulate()
{
   cerr << "Error code ### - CktCellType: " << CktCellTypeStr[ _cellType ] << ", simulate in cktCell.h" << endl;
}

Bv4
CktCell::getSimValue(const CktOutPin* foutPin) const // only overload the function in "CktSplitCell" and use "foutPin"
{
   unsigned outPinWidth;
   if (_cellType == CKT_SPLIT_CELL) 
      outPinWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   else 
      outPinWidth = VLDesign.getBus(_outPin->getBusId())->getWidth();

   assert (outPinWidth == _simValue.bits());
   return _simValue;
}

CktCell* 
CktCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   cerr << "Error code ### - CktCellType: " << CktCellTypeStr[ _cellType ] << ", cellDuplicate in cktCell.h" << endl;
   return 0;
}

void
CktCell::pinsDuplicate(CktCell*& newCell, string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktInPin*  oriInPin;
   CktOutPin* oriOutPin = getOutPin();
   //pin <--> cell : bi-direction connection within the function "pinDuplicate(...)"
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      oriInPin = getInPin(i);
      oriInPin->pinDuplicate(newCell, inPinMap); 
   }
   oriOutPin->pinDuplicate(newCell, prefix, outPinMap);
}

void 
CktCell::connDuplicate(CktCell*& dupCell, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
/*                  +--------+
               --<--|        |
               --<--|CktCell |-->--   only signal direction outward 
               --<--|        |  
                    +--------+                   */
   CktInPin*  oriFanOut_InPin;
   CktOutPin* oriFanIn_OutPin;
   //outPin
   assert (dupCell->getOutPin() == outPinMap[getOutPin()]);
   for (unsigned i = 0; i < getOutPin()->getInPinSize(); ++i) {
      oriFanOut_InPin = getOutPin()->getInPin(i);
      if (inPinMap.find(oriFanOut_InPin) != inPinMap.end()) // the same module
         dupCell->getOutPin()->connInPin(inPinMap[oriFanOut_InPin]);
   }
   //inPin
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      assert (dupCell->getInPin(i) == inPinMap[getInPin(i)]);
      oriFanIn_OutPin = getInPin(i)->getOutPin();
      if (outPinMap.find(oriFanIn_OutPin) != outPinMap.end())
         dupCell->getInPin(i)->connOutPin(outPinMap[oriFanIn_OutPin]);
   }
}

void 
CktCell::writeBLIF() const
{
   cerr << "Error code ### - CktCellType: " << CktCellTypeStr[ _cellType ] << ", writeBLIF in cktCell.h" << endl;
}
   
void 
CktCell::checkPin() const
{
   if (_outPin == 0)
      cout << "Error1 : no outPin cellType = " << getCellType() << endl;
   if (_outPin->getInPinSize() == 0)
      cout << "Error2 : cellType = " << getCellType() << " name = " << _outPin->getName() << " has no foutCell" << endl;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      if (getInPin(i) == 0) {
         cout << "Error3 : cellType = " << getCellType() << " name = " << _outPin->getName() 
              << " has null inpin : number " << i << endl;
      }
      else {
         if (getInPin(i)->getOutPin() == 0) {
            cout << "Error4 : cellType = " << getCellType() << " name = " << _outPin->getName() 
                 << " has null finCell : number " << i << endl;
         }
      }
   }
}

void
CktCell::DFSCktTraversal(unsigned int& index) const {
   // This function traverse ckt to bound only, and insert edge to graph
   CktCell* cell = 0;
   if (--index) {
      for (unsigned i = 0; i < getInPinSize(); ++i) {
         cell = getInPin(i)->getOutPin()->getCell();
         //cout << "Edge = " << this->getOutPin()->getName() << " --> " << cell->getOutPin()->getName() << endl;
         if (!((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL))) {
            TraversalGraph.insertEdge(const_cast<CktCell*>(this), cell);
            cell->DFSCktTraversal(index);
         }
         else TraversalGraph.insertEdge(const_cast<CktCell*>(this), (CktCell*)((size_t)cell ^ 1));
      }
   }
   ++index;
}

bool
CktCell::DFSWriteCktToBound(ofstream& os, const unsigned index) const {
   // This function traverse ckt to bound, write ckt connection
   CktCell* cell = 0;
   bool ret = false;
   bool temp_ret = false;
   unsigned bitWidth = 0;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      cell = getInPin(i)->getOutPin()->getCell();
      if ((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)) temp_ret = cell->isTraversed();
      else if (!cell->isTraversed()) temp_ret = cell->DFSWriteCktToBound(os, index);
      else temp_ret = true;
      if (temp_ret) {
         if ((getCellType() == CKT_DFF_CELL) || (getCellType() == CKT_DLAT_CELL)) os << getOutPin()->getName() << '_' << (index - 1);
         else if (getCellType() == CKT_SPLIT_CELL) 
            os << static_cast<CktSplitCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getName() << '_' << index;
         else if (getCellType() == CKT_MODULE_CELL) 
            os << static_cast<CktModuleCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getName() << '_' << index;
         else os << getOutPin()->getName() << '_' << index;
         os << " -> ";
         if (cell->getCellType() == CKT_SPLIT_CELL) os << static_cast<CktSplitCell*>(cell)->getOutPin(0)->getName() << '_' << index;
         else if (cell->getCellType() == CKT_MODULE_CELL) os << static_cast<CktModuleCell*>(cell)->getOutPin(0)->getName() << '_' << index;
         else os << cell->getOutPin()->getName() << '_' << index;
         os << " [dir = back";
         /*
         if (getCellType() == CKT_SPLIT_CELL) 
            bitWidth = VLDesign.getBus(static_cast<CktSplitCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getBusId())->getWidth();
         else if (getCellType() == CKT_MODULE_CELL) 
            bitWidth = VLDesign.getBus(static_cast<CktModuleCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getBusId())->getWidth();
         else bitWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
         */
         bitWidth = getInPin(i)->getOutPin()->getBusWidth();
         if (bitWidth != 1) os << ", style = bold, label = " << bitWidth << ", labeldistance = 0.5";
         if (getCellType() == CKT_MUX_CELL) {
            if (i == 0) os << ", arrowtail = odot";  // false
            else if (i == 1) os << ", arrowtail = dot";  // true
            /*
            if (i == 0) os << ", arrowtail = odot, color = blue";  // false
            else if (i == 1) os << ", arrowtail = dot, color = red";  // true
            else os << ", color = green";  // select
            */
         }
         else if ((getCellType() == CKT_GEQ_CELL) || (getCellType() == CKT_GREATER_CELL) || (getCellType() == CKT_LEQ_CELL) ||
                  (getCellType() == CKT_LESS_CELL) || (getCellType() == CKT_SHL_CELL) || (getCellType() == CKT_SHR_CELL)) {
            if (i == 0) os << ", arrowtail = dot";  // a
            else os << ", arrowtail = odot";  // b
         }
         os << "]; ";
      }
      ret |= temp_ret;
   }
   return ret;
}

void
CktCell::DFSWriteCktTraversal(ofstream& os, const unsigned index, const bool mux_sel_enable) const {
   // This function traverse ckt to DFF, insert edge to graph and write ckt connection
   CktCell* cell = 0;
   unsigned bitWidth = 0;
   bool mux_enable = (getCellType() != CKT_MUX_CELL) || mux_sel_enable;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      if (!mux_enable) { if (i == 2) return; }
      cell = getInPin(i)->getOutPin()->getCell();
      if ((getCellType() == CKT_DFF_CELL) || (getCellType() == CKT_DLAT_CELL)) os << getOutPin()->getName() << '_' << (index - 1);
      else if (getCellType() == CKT_SPLIT_CELL) 
         os << static_cast<CktSplitCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getName() << '_' << index;
      else if (getCellType() == CKT_MODULE_CELL) 
         os << static_cast<CktModuleCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getName() << '_' << index;
      else os << getOutPin()->getName() << '_' << index;
      os << " -> ";
      if (cell->getCellType() == CKT_SPLIT_CELL) os << static_cast<CktSplitCell*>(cell)->getOutPin(0)->getName() << '_' << index;
      else if (cell->getCellType() == CKT_MODULE_CELL) os << static_cast<CktModuleCell*>(cell)->getOutPin(0)->getName() << '_' << index;
      else os << cell->getOutPin()->getName() << '_' << index;
      os << " [dir = back";
      /*
      if (getCellType() == CKT_SPLIT_CELL) 
         bitWidth = VLDesign.getBus(static_cast<CktSplitCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getBusId())->getWidth();
      else if (getCellType() == CKT_MODULE_CELL) 
         bitWidth = VLDesign.getBus(static_cast<CktModuleCell*>(const_cast<CktCell*>(this))->getOutPin(0)->getBusId())->getWidth();
      else bitWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
      */
      bitWidth = getInPin(i)->getOutPin()->getBusWidth();
      if (bitWidth != 1) os << ", style = bold, label = " << bitWidth << ", labeldistance = 0.5";
      if (getCellType() == CKT_MUX_CELL) {
         if (i == 0) os << ", arrowtail = odot";  // false
         else if (i == 1) os << ", arrowtail = dot";  // true
         /*
         if (i == 0) os << ", arrowtail = odot, color = blue";  // false
         else if (i == 1) os << ", arrowtail = dot, color = red";  // true
         else os << ", color = green";  // select
         */
      }
      else if ((getCellType() == CKT_GEQ_CELL) || (getCellType() == CKT_GREATER_CELL) || (getCellType() == CKT_LEQ_CELL) ||
               (getCellType() == CKT_LESS_CELL) || (getCellType() == CKT_SHL_CELL) || (getCellType() == CKT_SHR_CELL)) {
         if (i == 0) os << ", arrowtail = dot";  // a
         else os << ", arrowtail = odot";  // b
      }
      os << "]; ";
      if (!((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL))) {
         TraversalGraph.insertEdge(const_cast<CktCell*>(this), cell);
         cell->DFSWriteCktTraversal(os, index, mux_sel_enable);
      }
      else TraversalGraph.insertEdge(const_cast<CktCell*>(this), (CktCell*)((size_t)cell ^ 1));
   }
}

void
CktCell::DFSCktTraversal() const {
   // This function traverse ckt to DFF, insert edge to graph only
   CktCell* cell = 0;
   for (unsigned i = 0; i < getInPinSize(); ++i) {
      cell = getInPin(i)->getOutPin()->getCell();
      if (!((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL))) {
         TraversalGraph.insertEdge(const_cast<CktCell*>(this), cell);
         cell->DFSCktTraversal();
      }
      else TraversalGraph.insertEdge(const_cast<CktCell*>(this), (CktCell*)((size_t)cell ^ 1));
   }
}

void
CktCell::updateSimToggle() {
   for (unsigned i = 0; i < getInPinSize(); ++i)
      if (getFinCell(i)->isSimToggle()) {
         setSimToggle(true);
         return;
      }
   setSimToggle(false);
}

bool 
CktCell::setVtRef()
{
   if (_vtNum != CktCell::_vtRef) {
      _vtNum = CktCell::_vtRef;
      return true;
   }
   else
      return false;
}
   
#endif

