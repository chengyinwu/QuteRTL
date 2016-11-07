/****************************************************************************
  FileName     [ cktModule.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit module data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_MODULE_SOURCE
#define CKT_MODULE_SOURCE

#include "cktModuleCell.h"
#include "cktIoCell.h"
#include "cktLogicCell.h"
#include "cktArithCell.h"
#include "cktSeqCell.h"
#include "cktZCell.h"
#include "cktModelCell.h"

#include "cktModule.h"
#include "cktBLIFile.h"
#include "myString.h"
#include "vlpItoStr.h"
#include "synVar.h"
#include "VLGlobe.h"
#include "util.h"
#include <queue>
#include <set>

extern VlpDesign&          VLDesign;
extern SynOutFile          synOutFile;
extern CktOutFile          cktOutFile;
extern const VlgModule*    curVlpModule;
extern int                 netNumber;
extern int                 instNumber;
extern CKT_usage*          CREMap;
extern bool                busDirectionDefault;
extern int                 globalPrintLevel;

static string tempnet = "net";
static unsigned netnum = 1;

const char* CellTypeString[44] = {
   "MODULE", "PI", "PO", "PIO", "DFF", "DLAT", "BUF", "INV", "AND", "OR", "NAND", "NOR", 
   "XOR", "XNOR", "MUX", "DEMUX", "ONEHOT", "CASE", "ADD", "SUB", "MULT", "DIV", "MODULO", 
   "SHL", "SHR", "RTL", "RTR", "BUFIF", "INVIF", "BUS", "Z2X", "CONST", "SPLIT", "MERGE", 
   "INOUT", "WEAK", "EQUALITY", "GEQ", "GREATER", "LEQ", "LESS", "CONSTANT", "ERROR", "DUMMY END"
};

SimpleGraph<CktCell*> TraversalGraph;

class CktDffCell;
class CktCaseCell;
class CktDlatCell;
class CktConstCell;


void itoa( int value, char *srcbuf, int base )
{
   int    i = 30;
   string buf = "";

   for ( ; value && i; --i, value /= base ) buf = "0123456789abcdef"[ value % base ] + buf;
   strcpy( srcbuf, ( char * )buf.c_str() );
}

CktModule::CktModule()
{
   _vlpModule  = NULL;
   _isLib      = false;
   _isBlackBox = false;
   _refCount   = 0;
   _cellList.clear();
   _modCells.clear();
   _seqCells.clear();
}

CktModule::~CktModule()
{
   if (_cellList.size() != 0) {
      for (CktCellAry::iterator pos = _cellList.begin(); pos != _cellList.end(); ++pos)
         delete *(pos);
   }
   _cellList.clear();
}

void
CktModule::insertIoCell(CktCell* tmpCell, const string& ioName, CktIoType ioType)
{
   _ioTypeList.push_back(ioType);
   _ioNameList.push_back(ioName);

   assert(ioType == CKT_IN || ioType == CKT_OUT || ioType == CKT_IO);
   _ioList[ioType].push_back(tmpCell);
}

void
CktModule::insertIoCell(CktCell* cell, CktIoType ioType)
{
   assert(ioType == CKT_IN || ioType == CKT_OUT || ioType == CKT_IO);
   _ioList[ioType].push_back(cell);
}

void
CktModule::setModuleName(string moduleName) 
{
   _moduleName = moduleName;
}

void
CktModule::setModuleName(const char* moduleName) 
{
   _moduleName = moduleName;
}

string
CktModule::getModuleName() const
{
   return _moduleName;
}

void 
CktModule::printInpin(string& name, CktInPin* inpin, unsigned lv, unsigned level) const
{
   if (inpin->getOutPin() != 0)
      printOutpin(name, inpin->getOutPin(), lv, level);
}

void 
CktModule::printOutpin(string& name, CktOutPin* outpin, unsigned lv, unsigned level) const
{
   /*if((outpin->getName()=="out") && notPrint)
      notPrint= false;
   else if ((outpin->getName()=="out") && !notPrint)
      return ;
   else;
   */
   if (outpin->getName() != "") {
      if (outpin->getName() != name)
         --level;
      else {
         if (level == (unsigned)globalPrintLevel)
            --level;
         else {
            for (unsigned j = 0; j < lv; j++)
               cout << "  ";
            cout << "outpin name: " << outpin->getName();
            const SynBus* tmpBus = VLDesign.getBus(outpin->getBusId());
            if (tmpBus->getWidth() == 1) {
               if (outpin->getCell() == 0)
                  cout << endl;
               else {
                  if (outpin->getCell()->getCellType() == CKT_SPLIT_CELL) {
                     const SynBus* realBus = VLDesign.getBus(outpin->getCell()->getInPin(0)->getOutPin()->getBusId());
                     if (realBus->getWidth() != 1)
                        cout << "[" << tmpBus->getBegin() << "]" << endl;
                  }
               }
            }
            else {
               if (tmpBus->getBegin() == tmpBus->getEnd())
                  cout << "[" << tmpBus->getBegin() << "]" << endl;
               else {
                  if (tmpBus->isInverted() == true)
                     cout << "[" << tmpBus->getEnd() << ":" << tmpBus->getBegin() << "]"<< endl;
                  else
                     cout << "[" << tmpBus->getBegin() << ":" << tmpBus->getEnd() << "]"<< endl;
               }
            }
            return ;
         }
      }
   }
   if (outpin->getName() != "") {
      for (unsigned j = 0; j < lv; j++)
         cout << "  ";
      //cout<< "outpin name: "<< outpin->getName()<< ", addr: "<< outpin<< ", busId: "<< outpin->getBusId()<< ",   ";
      cout << "outpin name: " << outpin->getName();
   }
   else {
      for (unsigned j = 0; j < lv; j++)
         cout << "  ";
      //cout<< "outpin name: N/A, "<<", addr: "<< outpin<< ", busId: "<< outpin->getBusId()<< ",   ";
      cout << "outpin name: N/A,  ";
   }
   
   const SynBus* tmpBus = VLDesign.getBus(outpin->getBusId());
   //cout<< "jojo: "; tmpBus->print();
   if (tmpBus->getWidth() == 1) {
      if (outpin->getCell() == 0)
         //cout<< ",  ";
         cout << "[" << tmpBus->getBegin() << "]" << ",  ";
      else {
         if (outpin->getCell()->getCellType() == CKT_SPLIT_CELL) {
            const SynBus* realBus = VLDesign.getBus(outpin->getCell()->getInPin(0)->getOutPin()->getBusId());
            if (realBus->getWidth() != 1)
               cout << "[" << tmpBus->getBegin() << "]" << ",  ";
         }
         else
            cout << "[" << tmpBus->getBegin() << "]" << ",  ";
      }
   }
   else {
      if (tmpBus->getBegin() == tmpBus->getEnd())
         cout << "[" << tmpBus->getBegin() << "],  ";
      else {
         if (tmpBus->isInverted() == true)
            cout << "[" << tmpBus->getEnd() << ":" << tmpBus->getBegin() << "]" << ",  ";
         else
            cout << "[" << tmpBus->getBegin() << ":" << tmpBus->getEnd() << "]" << ",  ";
      }
   }
   
   if (outpin->getCell() != 0)
      printCell(name, outpin->getCell(), lv + 1, level);
   else
      cout << endl;
}

void 
CktModule::printCell(string& name, CktCell* cell, unsigned lv, unsigned level) const
{
   //cout<< "in cell"<< endl;
   unsigned i, j;
   string cellType = cell->getCellTypeString();
   
   if (level == 0) {
      //cout<< "Cell type: "<< cellType<< ", "<< cell<< endl;
      cout << "Cell type: " << cellType <<  endl;
      return ;
   }
   if (cellType == "dff") {
      //default pin
      //cout<< "Cell type: "<< cellType<< ", "<< cell<< endl;
      cout << "Cell type: " << cellType << ", " << endl;
      if ((static_cast<const CktDffCell*>(cell))->getDefault() != 0) {
         for (i = 0; i < lv; i++)
            cout << "  ";
         cout << "default: " << endl;
         printInpin(name, (static_cast<const CktDffCell*>(cell))->getDefault(), lv + 1, 1);
      }
      else {
         for (i = 0; i < lv; i++)
            cout << "  ";
         cout << "default: null" << endl;         
      }
      //clk pin
      if ((static_cast<const CktDffCell*>(cell))->getClk() != 0) {
         for (i = 0; i < lv; i++)
            cout << "  ";
         cout << "clock: " << endl;
         printInpin(name, (static_cast<const CktDffCell*>(cell))->getClk(), lv + 1, 1);
      }
      else {
         for (i = 0; i < lv; i++)
            cout << "  ";
         cout << "clock: null" << endl;         
      }
      //rst pin
      if ((static_cast<const CktDffCell*>(cell))->getReset() != 0) {
         for (i = 0; i < lv; i++)
            cout << "  ";
         cout << "reset: " << endl;
         printInpin(name, (static_cast<const CktDffCell*>(cell))->getReset(), lv + 1, 1);
      }
   }
   else if (cellType == "dlat") {
      cout << "Cell type: " << cellType << ", " << endl;
      if ((static_cast<const CktDlatCell*>(cell))->getLoad() != 0) {
         for (i = 0; i < lv; i++)
            cout << "  ";
         cout << "load: " << endl;
         printInpin(name, (static_cast<const CktDlatCell*>(cell))->getLoad(), lv + 1, 1);
      }
   }
   else if (cellType == "const") {
      cout << "Cell type: " << cellType << endl;
      for (i = 0; i < lv; i++)
         cout << "  ";
      if ((static_cast<CktConstCell*>(cell))->getBvValue() != 0) {
         if ((static_cast<CktConstCell*>(cell))->getBvValue()->value() != (unsigned)(-1))  // to check!!
            cout << "Bv4 value: " << (static_cast<CktConstCell*>(cell))->getBvValue()->value() << endl;
         else
            cout << "Bv4 value: " << *(static_cast<CktConstCell*>(cell))->getBvValue() << endl;
      }
   }
   else if (cellType == "pi") {
      /*for(i=0; i<lv; i++)
         cout<< "  ";
      cout<< "pi: "<< endl;*/
      cout << "Cell type: " << cellType << endl;
      return;
   }
   else if (cellType == "module")
      cout << "Cell type: " << cellType << ", module name: " 
           << (static_cast<CktModuleCell*>(cell))->getModulePtr()->getModuleName() << endl;
   else if (cellType == "equality") {
      cout << "Cell type: " << cellType;
      if (static_cast<CktEqualityCell*>(cell)->isLogicEq())
         cout << ", eq: logic equality" << endl;
      else
         cout << ", eq: case equality" << endl;      
   }
   else 
      cout << "Cell type: " << cellType << endl;
   for (i = 0; i < cell->getInPinSize(); i++) {
      for (j = 0; j < lv; j++)
         cout << "  ";
      if ((i == 2) && (cellType == "mux"))
         cout << "condition : " << endl;
      else
         cout << "input " << i << " :" << endl;
      /*if (cellType == "case") {
         for (j = 0; j < lv + 2; j++)
            cout << "  ";
         cout << "sub-condition: ";
         if ((static_cast<const CktCaseCell*>(cell))->getVarCond(i) != 0)
            printInpin(name, (static_cast<const CktCaseCell*>(cell))->getVarCond(i), lv - 2, 1);
      }*/
      if (cell->getInPin(i) != 0)
         printInpin(name, cell->getInPin(i), lv + 1, level);
      cout << endl;
   }
}

void
CktModule::print() const
{
   int input = 0, output = 0, inout = 0;

   cout << "IO List: " << endl;

   for (unsigned i = 0; i < _ioNameList.size(); i++) {
      cout << "( " <<  _ioNameList[i] << " , "; 
      if (_ioTypeList[i] == CKT_IN)
         cout << "CKT_IN";
      else if (_ioTypeList[i] == CKT_OUT)
         cout << "CKT_OUT";
      else if (_ioTypeList[i] == CKT_IO)
         cout << "CKT_IO";
      else;

      cout << " , ";

      if ( _ioTypeList[i] == CKT_IN )
         cout << _ioList[ CKT_IN ][ input++ ] << " )" << endl;
      else if ( _ioTypeList[i] == CKT_OUT ) {
         cout << _ioList[ CKT_OUT ][ output ] << ", " << _ioList[CKT_OUT][output]->getOutPin() << " )" << endl;
         ++output;
      }
      else if ( _ioTypeList[i] == CKT_IO )
         cout << _ioList[ CKT_IO ][ inout++ ] << " )" << endl;
      else
         cout << " )" << endl;
   }
   cout << "Lhs: reg/wire" << endl;
   for (unsigned i = 0; i < _varList.size(); i++) 
      cout << "( " << _varList[i]->getName() << ", " << _varList[i] << " )" << endl;
}

void
CktModule::print( string outpinName, int level ) const
{
   unsigned i;
   CktCell* tmpCell;

   for (i = 0; i < _ioList[CKT_OUT].size(); ++i) {
      tmpCell = _ioList[CKT_OUT][i];
      if (tmpCell->getOutPin()->getName() == outpinName) {
         printOutpin( outpinName, tmpCell->getOutPin(), 0, level );
         break;
      }

   }
   if (i == _ioList[CKT_OUT].size()) {
      for (i = 0; i < _varList.size(); i++) {
         if (_varList[i]->getName() == outpinName) {
            printOutpin( outpinName, _varList[i], 0, level );
            break;
         }
      }

      if (i == _varList.size()) 
         cerr << "Error: CktModule::print() ---> This pin name[ " << outpinName << " ] doesn't exist." << endl;
   }
}

void
CktModule::printPIO() const
{
   if (_ioList[CKT_IN].size() > 0) {
      cout << "PI: ";
      for (unsigned i = 0; i < _ioList[CKT_IN].size(); ++i) {
         cout << _ioList[CKT_IN][i]->getOutPin()->getName(); 
         if (i < _ioList[CKT_IN].size()-1) cout << ", ";  
      }
   }
   if (_ioList[CKT_IO].size() > 0) {
      cout << "PIO: ";
      for (unsigned i = 0; i < _ioList[CKT_IO].size(); ++i) {
         cout << _ioList[CKT_IO][i]->getOutPin()->getName(); 
         if (i < _ioList[CKT_IO].size()-1) cout << ", ";  
      }
   }
   if (_ioList[CKT_OUT].size() > 0) {
      cout << "PO: ";
      for (unsigned i = 0; i < _ioList[CKT_OUT].size(); ++i) {
         cout << _ioList[CKT_OUT][i]->getOutPin()->getName(); 
         if (i < _ioList[CKT_OUT].size()-1) cout << ", ";  
      }
   }
}

void
CktModule::writeGateInLevel(CktOutPin* root, const string filename, const unsigned level, const bool byName) {
   ofstream DOTFile;
   DOTFile.open(filename.c_str());
   
   DOTFile << "strict digraph G {" << endl;
   
   stack<queue<CktCell*> > sorted_cellList;
   queue<CktCell*> eqList, rootList, sourceList, tempList;
   rootList.push(root->getCell());

   string cellName = "";
   CktCell* cell = 0;
   unsigned temp_level = (level + 1);
   unsigned i, j, k;
   i = j = k = 0;
   while (temp_level) {
      // Traverse ckt
      TraversalGraph.clear();
      while (!rootList.empty()) {
         cell = rootList.front();
         if ((i == 0) || (cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)) {
            cell->DFSCktTraversal(temp_level);
            sourceList.push(cell);
         }
         rootList.pop();
      }

      TraversalGraph.getLeafNodes(rootList);
      TraversalGraph.topological_sort(sorted_cellList);
      
      // write dot file
      ++k;
      j = 0;
      while (!sorted_cellList.empty()) {
         eqList = sorted_cellList.top();
         sorted_cellList.pop();
         ++j;
         if (j == 1) { if (i != 0) continue; else ++temp_level; }
         if (i == 0) DOTFile << TAB_INDENT << "{ rank = same; \"(root)\"" << " [shape = plaintext]; ";
         else DOTFile << TAB_INDENT << "{ rank = same; Level_" << i << " [shape = plaintext]; ";
         while (!eqList.empty()) {
            cell = (CktCell*)((size_t)(eqList.front()) & (-2));
            if (cell->getCellType() == CKT_SPLIT_CELL) cellName = static_cast<CktSplitCell*>(cell)->getOutPin(0)->getName();
            else if (cell->getCellType() == CKT_MODULE_CELL) cellName = static_cast<CktModuleCell*>(cell)->getOutPin(0)->getName();
            else cellName = cell->getOutPin()->getName();
            if ((i == 0) && ((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL))) DOTFile << cellName << '_' << k-1 << " [shape = ";
            else DOTFile << cellName << '_' << k << " [shape = ";
            if ((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)) 
               DOTFile << "box, color = indigo, style = filled, fontcolor = snow, ";
            else if ((cell->getCellType() == CKT_PI_CELL) || (cell->getCellType() == CKT_PIO_CELL)) 
               DOTFile << "triangle, color = springgreen, style = bold, ";
            else if (cell->getCellType() == CKT_MUX_CELL) 
               DOTFile << "trapezium, color = red, style = filled, ";
            else if (cell->getCellType() == CKT_MODULE_CELL) 
               DOTFile << "box, peripheries = 2, color = aliceblue, style = filled, ";
            else if (cell->getCellType() == CKT_CONST_CELL) {
               DOTFile << "plaintext, ";
               cellName = "\"" + static_cast<CktConstCell*>(cell)->getValueStr() + "\"";
            }
            else DOTFile << "ellipse, ";
            if (byName || (cell->getCellType() == CKT_CONST_CELL)) DOTFile << "label = " << cellName << "]; ";
            else DOTFile << "label = " << CellTypeString[cell->getCellType()] << "]; ";
            eqList.pop();
         }
         DOTFile << "}" << endl;
         --temp_level;
         ++i;
      }
      while (!rootList.empty()) {
         cell = (CktCell*)((size_t)(rootList.front()) & (-2));
         cell->setTraverse(true);
         tempList.push(cell);
         rootList.pop();
      }
      while (!sourceList.empty()) {
         DOTFile << TAB_INDENT;
         sourceList.front()->DFSWriteCktToBound(DOTFile, k);
         DOTFile << endl;
         sourceList.pop();
      }
      while (!tempList.empty()) {
         cell = tempList.front();
         cell->setTraverse(false);
         rootList.push(cell);
         tempList.pop();
      }
      if (rootList.empty()) break;
   }
   DOTFile << endl;
   DOTFile << TAB_INDENT << "{ " << endl;
   DOTFile << TAB_INDENT << TAB_INDENT << "node [shape = plaintext]; edge [dir = none];" << endl;
   DOTFile << TAB_INDENT << TAB_INDENT << "\"(root)\"";
   for (j = 1; j < i; ++j) DOTFile << " -> Level_" << j;
   DOTFile << "; " << endl;
   DOTFile << TAB_INDENT << "} " << endl;
   DOTFile << "}" << endl;
   DOTFile.close();
}

void
CktModule::writeGateInTimeFlow(CktOutPin* root, const string filename, const unsigned depth, const bool byName) {
   ofstream DOTFile;
   DOTFile.open(filename.c_str());
   
   DOTFile << "strict digraph G {" << endl;
   
   stack<queue<CktCell*> > sorted_cellList;
   queue<CktCell*> eqList, rootList;
   rootList.push(root->getCell());

   string cellName = "";
   CktCell* cell = 0;
   unsigned level;
   unsigned i = 1;
   for (; i <= depth; ++i) {
      // Traverse ckt
      TraversalGraph.clear();
      while (!rootList.empty()) {
         cell = (CktCell*)((size_t)(rootList.front()) & (-2));
         if ((i == 1) || (cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)) {
            DOTFile << TAB_INDENT;
            cell->DFSWriteCktTraversal(DOTFile, i);
            DOTFile << endl;
         }
         rootList.pop();
      }

      TraversalGraph.getLeafNodes(rootList);
      TraversalGraph.topological_sort(sorted_cellList);
      
      // write dot file
      level = 0;
      while (!sorted_cellList.empty()) {
         eqList = sorted_cellList.top();
         sorted_cellList.pop();
         if (level++ == 0) { if (i != 1) continue; }
         if (sorted_cellList.empty()) DOTFile << TAB_INDENT << "{ rank = same; Depth_" << i << " [shape = plaintext]; ";
         else if (level == 1) DOTFile << TAB_INDENT << "{ rank = same; \"(root)\"" << " [shape = plaintext]; ";
         else DOTFile << TAB_INDENT << "{ rank = same; ";
         while (!eqList.empty()) {
            cell = (CktCell*)((size_t)(eqList.front()) & (-2));
            if (cell->getCellType() == CKT_SPLIT_CELL) cellName = static_cast<CktSplitCell*>(cell)->getOutPin(0)->getName();
            else if (cell->getCellType() == CKT_MODULE_CELL) cellName = static_cast<CktModuleCell*>(cell)->getOutPin(0)->getName();
            else cellName = cell->getOutPin()->getName();
            if ((level == 1) && ((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL))) DOTFile << cellName << '_' << i-1 << " [shape = ";
            else DOTFile << cellName << '_' << i << " [shape = ";
            if ((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)) 
               DOTFile << "box, color = indigo, style = filled, fontcolor = snow, ";
            else if ((cell->getCellType() == CKT_PI_CELL) || (cell->getCellType() == CKT_PIO_CELL)) 
               DOTFile << "triangle, color = springgreen, style = bold, ";
            else if (cell->getCellType() == CKT_MUX_CELL) 
               DOTFile << "trapezium, color = red, style = filled, ";
            else if (cell->getCellType() == CKT_MODULE_CELL) 
               DOTFile << "box, peripheries = 2, color = aliceblue, style = filled, ";
            else if (cell->getCellType() == CKT_CONST_CELL) {
               DOTFile << "plaintext, ";
               cellName = "\"" + static_cast<CktConstCell*>(cell)->getValueStr() + "\"";
            }
            else DOTFile << "ellipse, ";
            if (byName || (cell->getCellType() == CKT_CONST_CELL)) DOTFile << "label = " << cellName << "]; ";
            else DOTFile << "label = " << CellTypeString[cell->getCellType()] << "]; ";
            eqList.pop();
         }
         DOTFile << "}" << endl;
      }
      if (rootList.empty()) break;
   }
   DOTFile << endl;
   DOTFile << TAB_INDENT << "{ " << endl;
   DOTFile << TAB_INDENT << TAB_INDENT << "edge [dir = none];" << endl;
   DOTFile << TAB_INDENT << TAB_INDENT << "\"(root)\"";
   for (unsigned j = 1; j < i; ++j) DOTFile << " -> Depth_" << j;
   DOTFile << "; " << endl;
   DOTFile << TAB_INDENT << "} " << endl;
   DOTFile << "}" << endl;
   DOTFile.close();
}

void
CktModule::writeGateInBoundedDepth(CktOutPin* root, const string filename, const unsigned depth, const bool byName) {
   ofstream DOTFile;
   DOTFile.open(filename.c_str());
   DOTFile << "strict digraph G {" << endl;

   TraversalGraph.clear();
   stack<queue<CktCell*> > sorted_cellList;
   queue<CktCell*> eqList, rootList;
   rootList.push(root->getCell());

   string cellName = "";
   CktCell* cell = 0;
   unsigned level;
   for (unsigned i = 1; i <= depth; ++i) {
      // Traverse ckt
      while (!rootList.empty()) {
         cell = (CktCell*)((size_t)(rootList.front()) & (-2));
         if ((i == 1) || (cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)) {
            DOTFile << TAB_INDENT;
            cell->DFSWriteCktTraversal(DOTFile, 1);
            DOTFile << endl;
         }
         rootList.pop();
      }
      TraversalGraph.getLeafNodes(rootList);
      if (rootList.empty()) break;
   }
   
   TraversalGraph.topological_sort(sorted_cellList);
   
   // write dot file
   level = 0;
   while (!sorted_cellList.empty()) {
      eqList = sorted_cellList.top();
      sorted_cellList.pop();
      DOTFile << TAB_INDENT << "{ rank = same; Level_" << level << " [shape = plaintext]; ";
      while (!eqList.empty()) {
         cell = (CktCell*)((size_t)(eqList.front()) & (-2));
         if (cell->getCellType() == CKT_SPLIT_CELL) cellName = static_cast<CktSplitCell*>(cell)->getOutPin(0)->getName();
         else if (cell->getCellType() == CKT_MODULE_CELL) cellName = static_cast<CktModuleCell*>(cell)->getOutPin(0)->getName();
         else cellName = cell->getOutPin()->getName();
         if ((level == 0) && ((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)))
            DOTFile << cellName << '_' << 0 << " [shape = ";
         else DOTFile << cellName << '_' << 1 << " [shape = ";
         if ((cell->getCellType() == CKT_DFF_CELL) || (cell->getCellType() == CKT_DLAT_CELL)) 
            DOTFile << "box, color = indigo, style = filled, fontcolor = snow, ";
         else if ((cell->getCellType() == CKT_PI_CELL) || (cell->getCellType() == CKT_PIO_CELL)) 
            DOTFile << "triangle, color = springgreen, style = bold, ";
         else if (cell->getCellType() == CKT_MUX_CELL) 
            DOTFile << "trapezium, color = red, style = filled, ";
         else if (cell->getCellType() == CKT_MODULE_CELL) 
            DOTFile << "box, peripheries = 2, color = aliceblue, style = filled, ";
         else if (cell->getCellType() == CKT_CONST_CELL) {
            DOTFile << "plaintext, ";
            cellName = "\"" + static_cast<CktConstCell*>(cell)->getValueStr() + "\"";
         }
         else DOTFile << "ellipse, ";
         if (byName || (cell->getCellType() == CKT_CONST_CELL)) DOTFile << "label = " << cellName << "]; ";
         else DOTFile << "label = " << CellTypeString[cell->getCellType()] << "]; ";
         eqList.pop();
      }
      DOTFile << "}" << endl;
      ++level;
   }
   DOTFile << endl;
   DOTFile << TAB_INDENT << "{ " << endl;
   DOTFile << TAB_INDENT << TAB_INDENT << "node [shape = plaintext]; edge [dir = none];" << endl;
   DOTFile << TAB_INDENT << TAB_INDENT << "Level_0";
   for (unsigned i = 1; i < level; ++i) DOTFile << " -> Level_" << i;
   DOTFile << "; " << endl;
   DOTFile << TAB_INDENT << "} " << endl;
   DOTFile << "}" << endl;
   DOTFile.close();
}

bool
CktModule::writeCDFG(const string pinName, const string fileName, set<unsigned>& bits) {
   SynVar* var = _synVars[pinName];
   if (!var) { Msg(MSG_ERR) << "pinName " << pinName << " NOT exists!" << endl; return false; }
   return var->writeCDFG(fileName, bits); 
}

void
CktModule::bfsCellList(list<CktOutPin*>& bfsList) {
   TraversalGraph.clear();
   stack<queue<CktCell*> > sorted_cellList;
   queue<CktCell*> eqList;

   unsigned i;
   CktCell* cell = 0;
   
   for (list<CktOutPin*>::iterator it = bfsList.begin(); it != bfsList.end(); ++it) {
      cell = (CktCell*)(((size_t)(*it)->getCell()) & (-2));
      cell->DFSCktTraversal();
   }

   TraversalGraph.topological_sort(sorted_cellList);
   
   bfsList.clear();
   while (!sorted_cellList.empty()) {
      eqList = sorted_cellList.top();
      sorted_cellList.pop();
      while (!eqList.empty()) {
         cell = (CktCell*)((size_t)(eqList.front()) & (-2));
         if (cell->getCellType() == CKT_SPLIT_CELL) {
            for (i = 0; i < static_cast<CktSplitCell*>(cell)->getOutPinSize(); ++i) 
               bfsList.push_back(static_cast<CktSplitCell*>(cell)->getOutPin(i));
         }
         else if (!(cell->getCellType() == CKT_PI_CELL || cell->getCellType() == CKT_CONST_CELL))
            bfsList.push_back(cell->getOutPin());
         eqList.pop();
      }
   }
}

CktCell *
CktModule::getIoCell( string ioName ) const
{
   int input  = 0,
       output = 0,
       inout  = 0;

   for (unsigned i = 0; i < _ioNameList.size(); ++i) {
      if (_ioNameList[i] == ioName) {
         if (_ioTypeList[i] == CKT_IN)
            return _ioList[CKT_IN][input];
         else if (_ioTypeList[i] == CKT_OUT)
            return _ioList[CKT_OUT][output];
         else if (_ioTypeList[i] == CKT_IO)
            return _ioList[CKT_IO][inout];
      }
      else {
         if (_ioTypeList[i] == CKT_IN)
            input++;
         else if (_ioTypeList[i] == CKT_OUT)
            output++;
         else if (_ioTypeList[i] == CKT_IO)
            inout++;
      }
   }
   return 0;
}

CktCell *
CktModule::getIoCell(CktIoType ioType, unsigned pos) const
{
   assert( ioType == CKT_IN || ioType == CKT_OUT || ioType == CKT_IO );
   assert( pos >= 0 );
   assert( pos < _ioList[ ioType ].size() );

   if (pos > _ioList[ ioType ].size()) {
      cerr << "Error: CktModule::getIoCell() ---> Index out of range." << endl;
      exit(1);
   }

   return _ioList[ioType][pos];
}

CktCell*
CktModule::getSeqCell(unsigned pos) const
{
   assert (pos < _seqCells.size());
   return _seqCells[pos];
}

unsigned
CktModule::getCellListSize() const
{
   return _cellList.size();
}

unsigned
CktModule::getIoSize(CktIoType ioType) const
{
   assert (ioType == CKT_IN || ioType == CKT_OUT || ioType == CKT_IO);
   return _ioList[ioType].size();
}

unsigned
CktModule::getIoSize() const
{
   return _ioNameList.size();
}

unsigned
CktModule::getDffSize() const {
   return _seqCells.size();
}

string
CktModule::getIoName(unsigned index) const
{
   assert(index < _ioNameList.size());

   if (index > _ioNameList.size()) {
      cerr << "Error: CktModule::getIoName() ---> Index out of range." << endl;
      exit(1);
   }

   return _ioNameList[index];
}

int
CktModule::getIoType(unsigned index) const 
{
   assert(index < _ioTypeList.size());

   if (index > _ioTypeList.size()) {
      cerr << "Error: CktModule::getIoType() ---> Index out of range." << endl;
      exit( 1 );
   }
   return _ioTypeList[index];
}

int
CktModule::getIoType(string name) const
{
   unsigned i;
   for (i = 0; i < _ioNameList.size(); i++) 
      if ( _ioNameList[i] == name )
         break;

   if (i == _ioNameList.size()) 
      return CKT_TOTAL;// I use this to represent error.
   else 
      return _ioTypeList[i];
}

unsigned
CktModule::getIoType(CktIoType ioType, string name) const
{
   assert (ioType == CKT_IN || ioType == CKT_OUT || ioType == CKT_IO);
   for (unsigned i = 0; i < _ioList[ioType].size(); ++i) {
      if ((_ioList[ioType][i])->getOutPin()->getName() == name) return i;
   }
   return _ioList[ioType].size();
}

void
CktModule::getTypeCellList(const CktCellType cellType, list<CktCell*>& cellList) const
{
   switch (cellType) {
      case CKT_MODULE_CELL : 
      case CKT_PI_CELL : 
      case CKT_PO_CELL : 
      case CKT_PIO_CELL : 
         break;
      case CKT_DFF_CELL : 
      case CKT_DLAT_CELL : 
         for (unsigned i = 0; i < getDffSize(); ++i) 
            cellList.push_back(_seqCells[i]);
         break;
      default : 
         for (unsigned i = 0; i < getCellListSize(); ++i) 
            if (cellType == _cellList[i]->getCellType()) 
               cellList.push_back(_cellList[i]);
         break;
   }
}

CktOutPin*
CktModule::getOutPin(const string rootName) const
{
   CktCell* cell = getIoCell(rootName);
   if (cell) {
      if (cell->getCellType() == CKT_PO_CELL) return cell->getInPin(0)->getOutPin();
      else return cell->getOutPin();
   }
   CktSplitCell* s_cell;
   CktModuleCell* m_cell;
   for (unsigned i = 0; i < _cellList.size(); ++i) {
      cell = _cellList[i];
      switch (cell->getCellType()) {
         case CKT_PO_CELL : 
            // We don't want to get the outpin of PO_CELL, cell->getInPin(0)->getOutPin() instead
            continue;
         case CKT_SPLIT_CELL : 
            s_cell = static_cast<CktSplitCell*>(cell);
            for (unsigned j = 0; j < s_cell->getOutPinSize(); ++j) {
               if (s_cell->getOutPin(j)->getName() == rootName) return s_cell->getOutPin(j);
            }
            break;
         case CKT_MODULE_CELL :
            m_cell = static_cast<CktModuleCell*>(cell);
            for (unsigned j = 0; j < m_cell->getOutPinSize(); ++j) {
               if (m_cell->getOutPin(j)->getName() == rootName) return m_cell->getOutPin(j);
            }
            break;
         default : 
            if (cell->getOutPin()->getName() == rootName) return cell->getOutPin();
            break;
      }
   }
   return 0;
}

void
CktModule::insertVar( CktOutPin *outpin )
{
   _varList.push_back( outpin );
}

void
CktModule::insertVarType( string type )
{
   _varTypeList.push_back( type );
}

void 
CktModule::insertIoNameList( string ioName )
{
   _ioNameList.push_back( ioName );
}

void
CktModule::insertIoTypeList( CktIoType ioType )
{
   _ioTypeList.push_back( ioType );
}

void
CktModule::writeIO() const
{
   //ioList declaration : input [7:0] a
   CktOutPin*    outpin;
   CktCell*      cell;
   const SynBus* bus;
   //VlpPortNode*  port;
   string        ioName;
   int input = 0, output = 0, inout  = 0;
   int begin, end;
   bool isRev;

   for (unsigned i = 0; i < _ioNameList.size(); ++i) {
      if (_ioTypeList[i] == CKT_IN) {
         cell = _ioList[CKT_IN][input++];
         synOutFile << "input ";
      }
      else if ( _ioTypeList[i] == CKT_OUT ) {
         cell = _ioList[CKT_OUT][output++];
         synOutFile << "output ";
      }
      else { // ( _ioTypeList[i] == CKT_IO )
         cell = _ioList[CKT_IO][inout++];
         synOutFile << "inout ";
      }

      ioName = _ioNameList[i];
      outpin = cell->getOutPin();
      bus    = VLDesign.getBus(outpin->getBusId());
      begin  = bus->getBegin();
      end    = bus->getEnd();
      isRev  = bus->isInverted();

      if ( (begin == 0) && (end == 0) ) 
         synOutFile << ioName << ";";
      else {
         if (isRev) synOutFile << "[" << end   << ":" << begin << "] " << ioName << ";";
         else       synOutFile << "[" << begin << ":" << end   << "] " << ioName << ";";
      }
      endl(synOutFile);

      synOutFile.insertWireInst(ioName); //just insert name, for it won't be declared again in the future;
/*
      if (_ioTypeList[i] == CKT_IN)//just insert name, for it won't be declared again in the future;
         synOutFile.insertWireInst(ioName);
      else // ( _ioTypeList[i] == CKT_OUT ) or (_ioTypeList[i] == CKT_IO) 
         if (curVlpModule->getPort(ioName, port)) 
            if (port->getNetType() == reg) 
               synOutFile.insertRegInst(genRegInst(ioName, begin, end), ioName);
*/
   }
}

void
CktModule::writeWire() const
{
   CktOutPin* outpin;
   int        begin, end;
   bool       isInverted;
   string     varName;
   const SynBus* bus;
   for (unsigned i = 0; i < _varList.size(); ++i) {
      outpin     = _varList[i];
      varName    = _varList[i]->getName();
      bus        = VLDesign.getBus(outpin->getBusId());
      begin      = bus->getBegin();
      end        = bus->getEnd();
      isInverted = bus->isInverted();

      if ( _varTypeList[i] != "unspecified" )
         synOutFile << _varTypeList[i] << " ";

      if ( ((begin != 0) && (end != 0)) ) {
         if (isInverted)
            synOutFile << "[" << end << ":" << begin << "] ";
         else
            synOutFile << "[" << begin << ":" << end << "] ";
      }
      synOutFile << varName << ";";
      endl(synOutFile);

      if (!synOutFile.isExistWire(varName))
         synOutFile.insertWireInst(varName);
   }
}

void
CktModule::writeCells() const 
{
   //cout << "Total cell size = " << _cellList.size() << endl;
   for (CktCellAry::const_iterator pos = _cellList.begin(); pos != _cellList.end(); ++pos)
      (*pos)->writeOutput();
}

void
CktModule::modWriteOut() const
{
   //setup global variable
   netNumber  = _netNumber;
   instNumber = 0;

   synOutFile << "module " << _moduleName << "(";

   // Fixed by chengyin
   for (unsigned ioIndex = 0; ioIndex < _ioNameList.size(); ++ioIndex) 
      synOutFile << (ioIndex ? ", " : "") << _ioNameList[ioIndex];
   synOutFile << ");";
   endl(synOutFile);
   writeIO();
   writeWire();  //write output: reg/wire declaration  --> should not have reg -> to check  louius @ 071106
   writeCells();

   synOutFile.flushModuleDef();
   synOutFile << "endmodule";
   endl(synOutFile);
}

void
CktModule::connParam(CktOutPin* outpin)
{
   CktOutPin *tmpOutpin;
   CktCell   *tmpCell;

   for (unsigned i = 0; i < _paramList.size(); ++i){
      if (_paramList[i].param == outpin->getName()) {
         tmpOutpin = _paramList[i].content->synthesis();
         tmpCell   = tmpOutpin->getCell();

         tmpCell->connOutPin(outpin);
         outpin->connCell(tmpCell);
         delete tmpOutpin;
      }
   }
}

void
CktModule::setParamPair( ParamPairAry paramPairs ) 
{
   _paramList = paramPairs;
}

const ParamPairAry*
CktModule::getParamPair() const
{
   return &_paramList;
}

void
CktModule::setNetNumber( int netNumber )
{ 
   _netNumber = netNumber;
}

int
CktModule::getNetNumber() const
{
   return _netNumber;
}

void
CktModule::setVlpModule( const VlgModule* ptr )
{
   _vlpModule = ptr;
}

const VlgModule*
CktModule::getVlpModule() const
{
   return _vlpModule;
}

void 
CktModule::nameNoNamePin(int& num) const
{
   CktCellAry::const_iterator pos;
   for ( pos = _cellList.begin(); pos != _cellList.end(); pos++ )
      (*pos)->nameOutPin(num);
}

/*string 
CktModule::genRegInst(string& regName, int begin, int end) const
{
   stringstream input;
   if ((begin == 0) && (end == 0))
      input << "reg " << regName << ";" << endl;
   else if (begin == end)
      input << "reg " << regName << ";" << endl;
   else
      input << "reg [" << begin << ":" << end << "] " << regName << ";" << endl;
   //input<< "reg ["<< begin<<":"<< end<<"] "<< wireName<< ";"<< endl;
   return input.str();
}*/

/* Since the synthesis stage not store the cell list.
   The first work we do it to collect the cell list of design and create the module hierarchy.
   The module hierarchy is the most important info while doing flatten.

   In thie procedure, we do BFS traversal from root to leaf to collect the cell list.
   Once reach CktModuleCell, module hierarchy is created. */
void
CktModule::collectCell()
{
   CktCell* curCell;
   set<CktCell*>   cellSet;
   queue<CktCell*> cellQueue;
   _cellList.clear();
   _modCells.clear();
   _seqCells.clear();
   cellSet.clear();
   
   for (unsigned i = 0; i < _ioList[CKT_IO].size(); ++i)
      cellQueue.push(_ioList[CKT_IO][i]);
   for (unsigned i = 0; i < _ioList[CKT_OUT].size(); ++i)
      cellQueue.push(_ioList[CKT_OUT][i]);

   while (cellQueue.size() > 0) {
      curCell = cellQueue.front();
      if (find(cellSet.begin(), cellSet.end(), curCell) == cellSet.end()) {
         cellSet.insert(curCell);
         if (curCell->getCellType() == CKT_MODULE_CELL)
            _modCells.push_back(curCell);
         else if (curCell->getCellType() == CKT_DFF_CELL || curCell->getCellType() == CKT_DLAT_CELL) 
            _seqCells.push_back(curCell);

         curCell->collectFin(cellQueue);
      }
      cellQueue.pop();
   }
   for (set<CktCell*>::iterator it = cellSet.begin(); it != cellSet.end(); it++)
      _cellList.push_back(*it);
}

bool
CktModule::flatten()
{
   CktModuleCell *ptrCell;
   CktModule     *ptrModule, *newModule;

   if (_modCells.size() > 0) {
      for (CktCellAry::iterator it = _modCells.begin(); it != _modCells.end(); it++) {
         ptrCell = static_cast<CktModuleCell*>(*it);
         ptrModule = ptrCell->getModulePtr();
         if ( ptrModule->getRefCount() == 1 && ptrModule->isLib() == false ) {
            ptrModule->flatten();
         }
         else {
            newModule = ptrModule->duplicate();
 
            newModule->incRefCount();
            ptrModule->decRefCount();

            newModule->flatten();

            ptrCell->setModulePtr( newModule );
            ptrCell->setModuleName( newModule->getModuleName() );

            VLDesign.setModule(newModule);
         }
      }
   }
   return true;
}

CktModule*
CktModule::duplicate() const
{
   CktCellAry::const_iterator pos;
   CktCell   *curCell, *newCell;
   //CktCell   *orgCell, *tarCell;
   CktModule *newModule;
   CktInPin  *inPin,
             *newInPin;
   CktOutPin *outPin,
             *newOutPin;
   string    moduleName;

   CktModuleCell *newModuleCell,
                 *orgModuleCell;

   map< CktCell *, CktCell * > util;
   map< CktOutPin *, CktOutPin * > pinmap;
   map< CktInPin *, CktInPin * > pinmap_in;

   MyString newName;
   unsigned pinSize;

   util.clear();
   pinmap.clear();
   pinmap_in.clear();
   newModule = new CktModule;
   // first step: create cell replication
   for ( pos = _cellList.begin(); pos != _cellList.end(); pos++ ) {
      curCell = *pos;
  
      if ( curCell == NULL )
         continue;

      switch ( curCell->getCellType() ) {
      case CKT_MODULE_CELL:
         newCell = static_cast< CktCell * >( new CktModuleCell() );

         orgModuleCell = static_cast< CktModuleCell * >( curCell );
         newModuleCell = static_cast< CktModuleCell * >( newCell );

         newModuleCell->setModulePtr(  orgModuleCell->getModulePtr() );
         newModuleCell->setModuleName( orgModuleCell->getModuleName() );
         newModuleCell->setInstName(   orgModuleCell->getInstName() );

         (newModule->_modCells).push_back( newModuleCell );

         break;
      case CKT_PI_CELL:
         newCell = new CktPiCell();
         break;
      case CKT_PO_CELL:
         newCell = new CktPoCell();
         break;
      case CKT_PIO_CELL:
         newCell = new CktPioCell();
         break;
      case CKT_DFF_CELL:
         newCell = new CktDffCell();
         break;
      case CKT_DLAT_CELL:
         newCell = new CktDlatCell();
         break;
      case CKT_BUF_CELL:
         newCell = new CktBufCell();
         break;
      case CKT_INV_CELL:
         newCell = new CktInvCell();
         break;
      case CKT_AND_CELL:
         newCell = new CktAndCell();
         break;
      case CKT_OR_CELL:
         newCell = new CktOrCell();
         break;
      case CKT_NAND_CELL:
         newCell = new CktNandCell();
         break;
      case CKT_NOR_CELL:
         newCell = new CktNorCell();
         break;
      case CKT_XOR_CELL:
         newCell = new CktXorCell();
         break;
      case CKT_XNOR_CELL:
         newCell = new CktXnorCell();
         break;
      case CKT_MUX_CELL:
         newCell = new CktMuxCell();
         break;
      case CKT_ADD_CELL:
         newCell = new CktAddCell();
         break;
      case CKT_SUB_CELL:
         newCell = new CktSubCell();
         break;
      case CKT_MULT_CELL:
         newCell = new CktMultCell();
         break;
      case CKT_SHL_CELL:
         newCell = new CktShlCell();
         break;
      case CKT_SHR_CELL:
         newCell = new CktShrCell();
         break;
      case CKT_BUFIF_CELL:
         newCell = new CktBufifCell();
         break;
      case CKT_BUS_CELL:
         newCell = new CktBusCell();
         break;
      case CKT_CONST_CELL:
         newCell = new CktConstCell();
         (static_cast< CktConstCell*>(newCell))->setBvValue((static_cast<CktConstCell*>(curCell))->getBvValue());
         (static_cast< CktConstCell*>(newCell))->setValueStr( (static_cast<CktConstCell*>(curCell))->getValueStr());
         break;
      case CKT_SPLIT_CELL:
         newCell = new CktSplitCell();
         break;
      case CKT_MERGE_CELL:
         newCell = new CktMergeCell();
         break;
      case CKT_INOUT_CELL:
         newCell = new CktInoutCell();
         break;
      case CKT_EQUALITY_CELL:
         newCell = new CktEqualityCell();
         break;
      case CKT_GEQ_CELL:
         newCell = new CktGeqCell();
         break;
      case CKT_GREATER_CELL:
         newCell = new CktGreaterCell();
         break;
      case CKT_LEQ_CELL:
         newCell = new CktLeqCell();
         break;
      case CKT_LESS_CELL:
         newCell = new CktLessCell();
         break;
      default:
         cout << "ERROR: Unsupport cell type( " << curCell->getCellTypeString() << "/" << curCell->getCellType() << " )" << endl;
         exit(1);
      }

      if ( curCell->getCellType() == CKT_DFF_CELL ) {
         inPin = (static_cast< CktDffCell * >( curCell ))->getClk();
         if ( inPin != NULL ) {
            newInPin = new CktInPin();
            (static_cast< CktDffCell * >( newCell ))->connClk( newInPin );
            newInPin->connCell( newCell );
            pinmap_in[ inPin ] = newInPin;
         }

         inPin = (static_cast< CktDffCell * >( curCell ))->getDefault();
         if ( inPin != NULL ) {
            newInPin = new CktInPin();
            (static_cast< CktDffCell * >( newCell ))->connDefault( newInPin );
            newInPin->connCell( newCell );
            pinmap_in[ inPin ] = newInPin;
         }

         inPin = (static_cast< CktDffCell * >( curCell ))->getReset();
         if ( inPin != NULL ) {
            newInPin = new CktInPin();
            (static_cast< CktDffCell * >( newCell ))->connReset( newInPin );
            newInPin->connCell( newCell );
            pinmap_in[ inPin ] = newInPin;
         }
      }
      else if ( curCell->getCellType() == CKT_DLAT_CELL ) {
         inPin    = (static_cast< CktDlatCell * >( curCell ))->getLoad();
         newInPin = new CktInPin();
         (static_cast< CktDlatCell * >( newCell ))->connLoad( newInPin );
         newInPin->connCell( newCell );
         pinmap_in[ inPin ] = newInPin;
      }

      pinSize = curCell->getInPinSize();
      for (unsigned i = 0; i < pinSize; ++i) {
         inPin = curCell->getInPin(i);
         newInPin = new CktInPin();
         newCell->connInPin(newInPin);
         newInPin->connCell(newCell);
         pinmap_in[inPin] = newInPin;
      }

      if (curCell->getCellType() == CKT_MODULE_CELL || curCell->getCellType() == CKT_SPLIT_CELL) {
         if (curCell->getCellType() == CKT_MODULE_CELL)
            pinSize = (static_cast<CktModuleCell*>(curCell))->getOutPinSize();
         else
            pinSize = (static_cast<CktSplitCell*>(curCell))->getOutPinSize();            
         for (unsigned i = 0; i < pinSize; ++i) {
            if (curCell->getCellType() == CKT_MODULE_CELL)
               outPin = (static_cast<CktModuleCell*>(curCell))->getOutPin(i);
            else
               outPin = (static_cast<CktSplitCell*>(curCell))->getOutPin(i);

            newOutPin = new CktOutPin();
            newOutPin->setName(outPin->getName());
            newOutPin->setBusId(outPin->getBusId());
            newCell->connOutPin(newOutPin);
            newOutPin->connCell(newCell);
            pinmap[outPin] = newOutPin;
         }
      }
      else {
         outPin = curCell->getOutPin();
         newOutPin = new CktOutPin();  
         newOutPin->setName( outPin->getName() );
         newOutPin->setBusId( outPin->getBusId() );
         newCell->connOutPin( newOutPin );
         newOutPin->connCell( newCell );
         pinmap[ outPin ] = newOutPin;
      }

      if (newCell == NULL) {
         cout << "ERROR: Cell Duplication fault.." << endl;
         exit(1);
      }

      util[curCell] = newCell;
/*
 Information about setUtility:

 When doing duplication, we should to make sure two the cell relationship of two replications must be the same.
 At first step, we have already duplicate all cells.
 But we just duplicate them, not connection info created. 
 The purpose of setUtility is to record the relationship of cells.
 During duplicating, once we duplicate a new cell, we should record pointer to its original cell.
 
 How do we use setUtility to create connection of cell?
 In the step, all cell have be duplicated and the original cell knows its replication.
 For each cell in original cell list, traveral all cells connected to it and doing connection in replication cell list.
*/
      newModule->insertCell( newCell );
   }
   // link connection
   for ( pos = _cellList.begin(); pos != _cellList.end(); pos++ ) {
      curCell = *pos;

      if ( curCell == NULL )
         continue;
      newCell = util[ curCell ];
/*

      Original           Replication
      +---------+        +---------+
      |         |        |         |
      | orgCell |        | tarCell |
      |         |        |         |
      +----+----+        +----+----+
           |                  | 
           O                  O   <----- Outpin
           |    =========>    | 
           O                  O   <----- Inpin
           |                  |
      +----+----+        +----+----+
      |         |        |         |
      | curCell |        | newCell |
      |         |        |         |
      +---------+        +---------+

*/
      // connect in pin start

      if ( curCell->getCellType() == CKT_DFF_CELL ) {
         inPin = (static_cast< CktDffCell * >( curCell ))->getClk();
         if ( inPin != NULL ) {
            outPin = inPin->getOutPin();

            newInPin = pinmap_in[ inPin ];
            newOutPin = pinmap[ outPin ];
            newInPin->connOutPin( newOutPin );
            newOutPin->connInPin( newInPin );
         }

         inPin = (static_cast< CktDffCell * >( curCell ))->getReset();
         if ( inPin != NULL ) {
            outPin = inPin->getOutPin();

            newInPin = pinmap_in[ inPin ];
            newOutPin = pinmap[ outPin ];
            newInPin->connOutPin( newOutPin );
            newOutPin->connInPin( newInPin );
         }

         inPin = (static_cast< CktDffCell * >( curCell ))->getDefault();
         if ( inPin != NULL ) {
            outPin = inPin->getOutPin();

            newInPin = pinmap_in[ inPin ];
            newOutPin = pinmap[ outPin ];
            newInPin->connOutPin( newOutPin );
            newOutPin->connInPin( newInPin );
         }
      }
      else if ( curCell->getCellType() == CKT_DLAT_CELL ) {
         inPin = ( static_cast< CktDlatCell * >( curCell ))->getLoad();
         if ( inPin != NULL ) {
            outPin = inPin->getOutPin();
 
            newInPin = pinmap_in[ inPin ];
            if ( outPin->getCell() != NULL ) {
               newOutPin = pinmap[ outPin ];
            }
            else {
               newOutPin = new CktOutPin();
               pinmap[ outPin ] = newOutPin;
            }
            newInPin->connOutPin( newOutPin );
            newOutPin->connInPin( newInPin );
         }
      }

      pinSize = curCell->getInPinSize();

      for (unsigned i = 0; i < pinSize; ++i) {
         inPin = curCell->getInPin(i);
         outPin = inPin->getOutPin();

         newInPin = pinmap_in[inPin];
         if (outPin->getCell() != NULL)
            newOutPin = pinmap[outPin];
         else {
            newOutPin = new CktOutPin();
            pinmap[outPin] = newOutPin;
         }
         newInPin->connOutPin(newOutPin);
         newOutPin->connInPin(newInPin);
      }
   }
   for (pos = _ioList[CKT_IN].begin(); pos != _ioList[CKT_IN].end(); pos++) { 
      curCell = *pos;
      newCell = util[curCell];
      newModule->_ioList[CKT_IN].push_back(newCell);
   }

   for (pos = _ioList[CKT_OUT].begin(); pos != _ioList[CKT_OUT].end(); pos++) {
      curCell = *pos;
      newCell = util[curCell];
      newModule->_ioList[CKT_OUT].push_back(newCell);
   }

   for (pos = _ioList[CKT_IO].begin(); pos != _ioList[CKT_IO].end(); pos++) {
      curCell = *pos;
      newCell = util[curCell];
      newModule->_ioList[CKT_IO].push_back(newCell);
   }

   for (CktOutPinAry::const_iterator pin = _varList.begin(); pin != _varList.end(); pin++) {
      outPin = *pin;
      newOutPin = pinmap[outPin];
      newModule->_varList.push_back(newOutPin);
   }

   newModule->_varTypeList = _varTypeList; // record the type of interval signals.
   
   newName = _moduleName;
   newName += "_";
   newName += _refCount;
 
   newModule->_moduleName = newName.str();   // record module name

   newModule->_ioNameList = _ioNameList;   // record IO name list
   newModule->_ioTypeList = _ioTypeList;   // record IO type list
   newModule->_paramList  = _paramList;     // record parameter list
   newModule->_vlpModule  = _vlpModule;     // record its souce RTL
   newModule->_netNumber  = _netNumber;     
   newModule->_isLib      = _isLib;
 
 //  newModule->_modCells = _modCells;   // record the module hierarchy.
   return newModule;
}

void
CktModule::insertCell(CktCell* src) 
{
   if (src != NULL) 
      _cellList.push_back(src);
}

void
CktModule::printCellList() const
{
   CktCell* tmpCell;

   cout << "cell num: " << _cellList.size() << endl;

   for (unsigned i = 0; i < _cellList.size(); ++i) {
      tmpCell = _cellList[i];
      if ( ( tmpCell->getCellType() == CKT_SPLIT_CELL )
            || ( tmpCell->getCellType() == CKT_MODULE_CELL ) ) {
         cout << "cell: " << tmpCell->getCellTypeString() << endl;
      }
      else
         cout << "cell: " << tmpCell->getCellTypeString() << " \t, " << tmpCell << ",  " << tmpCell->getOutPin()->getName() << endl;
   }
}

void
CktModule::setLibrary()
{
   _isLib = true;
}

bool
CktModule::isLib() const
{
   return _isLib;
}

void
CktModule::setBlackBox()
{
   _isBlackBox = true;
}

bool
CktModule::isBlackBox() const
{
   return _isBlackBox;
}

bool 
CktModule::isCellInMod(const CktCell* cell) const
{
   for (CktCellAry::const_iterator it = _cellList.begin(); it != _cellList.end(); ++it)
      if ((*it) == cell)
         return true;
   return false;
}

void
CktModule::incRefCount() 
{ 
   _refCount++; 
}

void
CktModule::decRefCount() 
{ 
   _refCount--; 
}

int
CktModule::getRefCount() const 
{ 
   return _refCount; 
}

void
CktModule::setModCell()
{
   CktModuleCell* cktModCell;
   CktModule*     cktModule;
   string         moduleName;

   for (CktCellAry::iterator pos = _modCells.begin(); pos != _modCells.end(); ++pos) {
      cktModCell = static_cast<CktModuleCell*>(*pos);
      if (cktModCell->getModulePtr() == NULL) {
         moduleName = cktModCell->getModuleName();
         cktModule = VLDesign.getCktModule(moduleName);
         cktModCell->setModulePtr(cktModule);
      }
      cktModule = cktModCell->getModulePtr();
      cktModule->setModCell();
   }
   ++_refCount;
}

void 
CktModule::duplicateIO(CktModule*& ckt, CellMap& cellMap, OutPinMap& outPinMap, InPinMap& inPinMap) const
{                            //Only top module use the function. The argument "ckt" is the new top module.
   CktCell* newCell;
   CktCellAry::const_iterator cPos;
   CktOutPinAry::const_iterator oPos;
   vector<string>::const_iterator sPos;
   vector<CktIoType>::const_iterator tPos;
   string prefix = "top";

   //copy _ioList[] and duplicate top module IO cells to new top module
   for (cPos = _ioList[CKT_IN].begin(); cPos != _ioList[CKT_IN].end(); cPos++) {
      assert ((*cPos)->getCellType() == CKT_PI_CELL);
      newCell = (*cPos)->cellDuplicate(prefix, outPinMap, inPinMap);
      cellMap[(*cPos)] = newCell;
      ckt->insertIoCell(newCell, CKT_IN);
      //ckt->insertCell(newCell);
   } 
     
   for (cPos = _ioList[CKT_OUT].begin(); cPos != _ioList[CKT_OUT].end(); cPos++) {
      assert ((*cPos)->getCellType() == CKT_PO_CELL);
      newCell = (*cPos)->cellDuplicate(prefix, outPinMap, inPinMap);
      cellMap[(*cPos)] = newCell; 
      ckt->insertIoCell(newCell, CKT_OUT);
      //ckt->insertCell(newCell);
   }

   for (cPos = _ioList[CKT_IO].begin(); cPos != _ioList[CKT_IO].end(); cPos++) {
      assert ((*cPos)->getCellType() == CKT_PIO_CELL);
      newCell = (*cPos)->cellDuplicate(prefix, outPinMap, inPinMap);
      cellMap[(*cPos)] = newCell; 
      ckt->insertIoCell(newCell, CKT_IO);
      //ckt->insertCell(newCell);
   }
   //copy _varList, _varTypeList, _ioNameList, _ioTypeList, _netNumber
   for (oPos = _varList.begin(); oPos != _varList.end(); oPos++) 
      ckt->insertVar((*oPos));          //no copy object(sharing)
   for (sPos = _varTypeList.begin(); sPos != _varTypeList.end(); sPos++) 
      ckt->insertVarType((*sPos));
   for (sPos = _ioNameList.begin(); sPos != _ioNameList.end(); sPos++) 
      ckt->insertIoNameList((*sPos));
   for (tPos = _ioTypeList.begin(); tPos != _ioTypeList.end(); tPos++)
      ckt->insertIoTypeList((*tPos));
   ckt->setNetNumber(getNetNumber());
}

void
CktModule::debug() const
{
   cout << "=====================inPin fanout Cell=================="<< endl;
   for (CktCellAry::const_iterator pos = _ioList[CKT_IN].begin(); pos != _ioList[CKT_IN].end(); pos++) {
      //cout << "ioList[CKT_IN] = " << (*pos)->getCellType() << endl;
      cout << "InPin Name =  " << (*pos)->getOutPin()->getName() << endl;
      for (unsigned i = 0; i < (*pos)->getOutPin()->getInPinSize(); ++i) {
         cout << "InPin fanType " << i << " = " << (*pos)->getOutPin()->getFoutCell(i)->getCellType() << endl;
      }
   }
   cout << "======================all cell=========================="<< endl;
   for ( CktCellAry::const_iterator pos = _cellList.begin(); pos != _cellList.end(); pos++ ) 
      cout << "cellType = " << (*pos)->getCellType() << endl;

   cout << "==========================end==========================="<< endl;
}

//HierIoMap hierIoMap is define in the file "CktEnum.h". 
//It holds the connection relation between modules. 
//Hadling function : void updateHierConn(...)
/*Step 2 : Connection Info.

       PI : PICell, PO :  POCell, M : ModuleCell, C : OtherCell      
          <Level 1>
                      +------------------------+
                      |        <Level 2>       |
           C ---> M --|-> PI ----> M ----> PO -|-> M ---> C
                  M --|-> PI ------------> PO -|-> M   
                  M --|-> PI ----> C ----> PO -|-> M 
                      +------------------------+
                            <inPin>
                 |   C     PI     PO     M 
         ^ ------+--------------------------
         o    C  |  NH     NA     ST2    ST1     NH : no handle here
         u    PI |   2     NA      2     T1      S  : Pin Start
         t    PO |  NA     NA     NA      1      T  : Consider if topModule
         v    M  |  T1     T1     T1      1      12 : Level 1 and Level 2
  
 +==============================================================================+
 |CktModuleCell(inPin)|CktModuleCell(outPin)|CktPiCell(outPin)|CktPoCell(inPin) |
 +====================+=====================+=================+=================+
 |     C --> M        |     PO --> M        |    PI --> C     |    C --> PO     |
 |     M --> PI       |      M --> C        |    PI --> PO    |                 |
 |    PI --> M        |      M --> M        |                 |                 |
 |(diff in topModule) |      M --> PO       |                 |                 |
 |                    | (diff in topModule) |                 |                 |
 +==============================================================================+
*/
void
CktModule::moduleDuplicate(string prefix, CktModule*& fltCkt, bool isTopModule, CktCellAry& cells) const
{
   CellMap   cellMap;    //use in mapping cells within module
   OutPinMap outPinMap;  //use in mapping cells within module
   InPinMap  inPinMap;   //use in mapping cells within module
   CktCell*  newCell;
   CktCellType cellType;
   CktCellAry::const_iterator pos;
   //=======Step 1: duplicate non-buf and non-module cells and their pins=======//
   // collect all cells later 
   // => after duplicating all cells and making connections between these cells, call "collectCell()" !!)
   if (isTopModule) // Only the top module will duplicate the IO cells.
      duplicateIO(fltCkt, cellMap, outPinMap, inPinMap);

   for ( pos = _cellList.begin(); pos != _cellList.end(); pos++ ) {
      cellType = (*pos)->getCellType();
      if (!( cellType == CKT_PI_CELL  || cellType == CKT_PO_CELL
          || cellType == CKT_PIO_CELL || cellType == CKT_MODULE_CELL) ) //skip those cell         
      {
         newCell = (*pos)->cellDuplicate(prefix, outPinMap, inPinMap);
         cellMap[(*pos)] = newCell;
         cells.push_back(newCell); // keeping cell info in VlpHierTree
      }
   }
   //=====Step 2: update connection info. between cells in different modules=====//
   for (pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
      cellType = (*pos)->getCellType();
      if (cellType == CKT_MODULE_CELL) 
         (static_cast<CktModuleCell*>(*pos))->updateHierConn(prefix, inPinMap, outPinMap, isTopModule);
      else if (cellType == CKT_PI_CELL && !isTopModule)
         (static_cast<CktPiCell*>(*pos))->updateHierConn(prefix, inPinMap);
      else if (cellType == CKT_PO_CELL && !isTopModule) 
         (static_cast<CktPoCell*>(*pos))->updateHierConn(prefix, outPinMap, inPinMap);
      else if (cellType == CKT_PIO_CELL && !isTopModule)
         (static_cast<CktPioCell*>(*pos))->updateHierConn(prefix, outPinMap, inPinMap);
         //cerr << "Still not finish pioCell @ CktModule::moduleDuplicate(...)" << endl;
   }
   //====Step 3: make connection between cells within current module===========//
   connModuleIOPin(cellMap ,inPinMap, outPinMap, isTopModule);
   //====================Step 4: make connection between modules===================//
   //wait for all mopdules have duplicated, finally, connect the connection between//
   //modules in the up-level function "VlpHierTree :: flatten(...)"                //

}

void 
CktModule::updateFltCkt() //using in "Step4: make connection between modules"
{
   int count1 = 0;
   int count2 = 0;
   for (CktCellAry::iterator pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
      if ((*pos)->getCellType() == CKT_BUF_CELL) {
         if (static_cast<CktBufCell*>(*pos)->eliminate() == true) {
            _cellList.erase(pos);
            --pos;
            ++count1;
         }
         else
            ++count2;
      }
      /*else if ((*pos)->getCellType() == CKT_BUS_CELL) {
         //(*pos)->checkPin();
         //static_cast<CktBusCell*>(*pos)->checkIoWidth();
         //static_cast<CktBusCell*>(*pos)->updateIoWidth();
         //static_cast<CktBusCell*>(*pos)->transToMergeCell(*pos);
      }
      else if ((*pos)->getCellType() == CKT_MERGE_CELL) {
         ;//(*pos)->checkPin();
         //static_cast<CktMergeCell*>(*pos)->checkIoWidth();
      }
      else
         ;//(*pos)->checkPin();
      */
   }  
   cout << "Eliminate " << count1 << " Bufs, retain " << count2 << " Bufs !!" << endl;
   forceEliminateBuf();
   adjustConst();
   // after buf elimination, set cell ID and revise I/O bus of CktSplitCell 
   reviseSplitCell(); 
}

void 
CktModule::forceEliminateBuf()
{
   cout << "Eliminate all Bufs ... " << endl;
   CktCellAry::iterator pos = _cellList.begin();
   while (pos != _cellList.end()) {
      if ((*pos)->getCellType() == CKT_BUF_CELL) {
         if (static_cast<CktBufCell*>(*pos)->forceEliminate()) {
            delete (*pos);  // will also delete inPins and the outPin of the cell
            _cellList.erase(pos);
            continue;
         }
      }
      ++pos;
   }
   for (pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
      if ((*pos)->getCellType() == CKT_BUF_CELL) assert(!(static_cast<CktBufCell*>(*pos)->forceEliminate()));
   }
}

void
CktModule::reviseSplitCell()
{
   unsigned long id = 0;
   for (CktCellAry::iterator pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
      (*pos)->setID(++id);
      if ((*pos)->getCellType() == CKT_SPLIT_CELL)
         static_cast<CktSplitCell*>(*pos)->reviseIoBus();
   }
}

unsigned
CktModule::expandWidth(CktCell* foutCell, CktInPin* constIn) const
{
   CktCellType foutType = foutCell->getCellType();
   unsigned foutWidth = 0;
   switch(foutType) {
      case CKT_LEQ_CELL:
      case CKT_GEQ_CELL:
      case CKT_GREATER_CELL:
      case CKT_LESS_CELL:
      case CKT_EQUALITY_CELL:
         if (foutCell->getInPin(0) == constIn)
            foutWidth = VLDesign.getBus(foutCell->getInPin(1)->getOutPin()->getBusId())->getWidth();
         else
            foutWidth = VLDesign.getBus(foutCell->getInPin(0)->getOutPin()->getBusId())->getWidth();
         break;
      case CKT_MUX_CELL:
         if (foutCell->getInPin(2) == constIn) // sel pin
            foutWidth = 1;
         else
            foutWidth = VLDesign.getBus(foutCell->getOutPin()->getBusId())->getWidth();
         break;
      case CKT_ADD_CELL:
      case CKT_SUB_CELL:
      case CKT_SHL_CELL:
      case CKT_SHR_CELL:
      case CKT_BUF_CELL:
      case CKT_INV_CELL:
      case CKT_DFF_CELL:
         foutWidth = VLDesign.getBus(foutCell->getOutPin()->getBusId())->getWidth();
         break;
      case CKT_AND_CELL:
         if ( (static_cast<CktAndCell*>(foutCell))->getType() != CKT_RED)
            foutWidth = VLDesign.getBus(foutCell->getOutPin()->getBusId())->getWidth();
         break;
      case CKT_OR_CELL:
         if ( (static_cast<CktOrCell*>(foutCell))->getType() != CKT_RED)
            foutWidth = VLDesign.getBus(foutCell->getOutPin()->getBusId())->getWidth();
         break;
      case CKT_XOR_CELL:
         if ( (static_cast<CktXorCell*>(foutCell))->getType() != CKT_RED)
            foutWidth = VLDesign.getBus(foutCell->getOutPin()->getBusId())->getWidth();
         break;
      case CKT_XNOR_CELL:
         if ( (static_cast<CktXnorCell*>(foutCell))->getType() != CKT_RED)
            foutWidth = VLDesign.getBus(foutCell->getOutPin()->getBusId())->getWidth();
         break;
      case CKT_BUFIF_CELL:
         if (foutCell->getInPin(1) == constIn) // cond pin
            foutWidth = 1;
         else
            foutWidth = VLDesign.getBus(foutCell->getOutPin()->getBusId())->getWidth();
         break;
      default: // merge, Split, Bus, Po, Nand & Nor(only reducted type)
         foutWidth = 0;
         break;
   }
   return foutWidth;
}

void
CktModule::adjustConst()
{
   CktCell* foutCell;
   CktCellType foutType;
   CktConstCell* cCell;
   CktOutPin *constOut, *newOut;
   CktInPin* foutInPin;
   unsigned constWidth, foutWidth;
   int p = 0;
   string newName, bitStr;
   for (CktCellAry::iterator pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
      if ((*pos)->getCellType() == CKT_CONST_CELL) {
         cCell = static_cast<CktConstCell*>(*pos);
         constOut = cCell->getOutPin();
         constWidth = VLDesign.getBus(constOut->getBusId())->getWidth();
         if (cCell->getValueStr() == "") {
            bitStr = toString(constWidth)+ "'d" + toString(cCell->getBvValue()->value());
            cCell->setValueStr(bitStr);
         }

         for (unsigned i = 0; i < cCell->getFoutSize(); ++i) {
            foutCell = cCell->getFoutCell(i);
            foutType = foutCell->getCellType();
            foutWidth = expandWidth(foutCell, constOut->getInPin(i));
            if (foutWidth != 0 && foutWidth != constWidth) {
               unsigned busId = VLDesign.genBusId(foutWidth, 0, foutWidth-1, busDirectionDefault);
               if (cCell->getFoutSize() == 1)
                  constOut->setBusId(busId);
               else {
                  foutInPin = constOut->getInPin(i);
                  bitStr = toString(foutWidth)+ "'d" + toString(cCell->getBvValue()->value());
                  insertCell( new CktConstCell(newOut, cCell->getBvValue(), bitStr) );
                  newOut->setBusId(busId);
                  newName = constOut->getName() + "$" + toString(++p);
                  newOut->setName(newName);
                  newOut->connInPin(foutInPin); foutInPin->connOutPin(newOut);
                  constOut->erase(foutInPin);
                  --i;
               }
            }            
         }
      }
   }
}

void 
CktModule::connModuleIOPin(CellMap& cellMap, InPinMap& inPinMap, OutPinMap& outPinMap, bool isTopModule) const
{
   CktCellAry::const_iterator pos;
   CktCellType cellType;
   CktCell* dupCell;
   CktCell* oriCell;
   for (pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
      cellType = (*pos)->getCellType();
      oriCell = *pos;
      if (cellType == CKT_MODULE_CELL) { ;/*skip*/}      
      else if (cellType == CKT_PI_CELL || cellType == CKT_PO_CELL || cellType == CKT_PIO_CELL) {
         if (isTopModule) {
            assert (cellMap.find(oriCell) != cellMap.end());
            dupCell = cellMap[oriCell];
            oriCell->connDuplicate(dupCell, outPinMap, inPinMap);
         }
      }
      else {
         assert (cellMap.find(oriCell) != cellMap.end());
         dupCell = cellMap[oriCell];
         oriCell->connDuplicate(dupCell, outPinMap, inPinMap);
      }    
   }
}

void
CktModule::printStatistics(bool design) const
{
   int cell_count[44] = {0};  // All CellType of enum CktCellType
   int sum = 0;
   for ( CktCellAry::const_iterator pos = _cellList.begin(); pos != _cellList.end(); pos++ ) {
      ++cell_count[(*pos)->getCellType()];
      ++sum;
   }
   if (design) cout << "  " << "Report Statistics : " << "Design is " << "\"" << getModuleName() << "\"" << endl;
   else cout << "  " << "Report Statistics : " << "Ckt is " << "\"" << getModuleName() << "\"" << endl;
   cout << "  " << "==================================================" << endl;
   for (int i = 0; i < 44; ++i) {
      if (cell_count[i] != 0) {
         cout.width(15); 
         cout << right << CellTypeString[i] << " CELL";
         cout.width(20);
         cout << right << cell_count[i] << endl;
      }
   }
   cout << "  " << "==================================================" << endl;
   cout.width(20);
   cout << right << "Total Cell";
   cout.width(20);
   cout << right << sum << endl;
}

void
CktModule::printSummary(bool design) const
{
   if (design) cout << "  " << "Report Summary : " << "Design is " << "\"" << getModuleName() << "\"" << endl;
   else cout << "  " << "Report Summary : " << "Ckt is " << "\"" << getModuleName() << "\"" << endl;
   cout << "  " << "==================================================" << endl;
   int bit_count;
   // PIs
   if (getIoSize(CKT_IN) > 0) {
      bit_count = 0;
      for ( CktCellAry::const_iterator pos = _ioList[CKT_IN].begin(); pos != _ioList[CKT_IN].end(); pos++ ) bit_count += (*pos)->getOutWidth();
      cout << "  " << "Input Num : " << getIoSize(CKT_IN) << endl;
      cout << "  " << "Input Bits : " << bit_count << endl;
   }
   // POs
   if (getIoSize(CKT_OUT) > 0) {
      bit_count = 0;
      for ( CktCellAry::const_iterator pos = _ioList[CKT_OUT].begin(); pos != _ioList[CKT_OUT].end(); pos++ ) bit_count += (*pos)->getOutWidth();
      cout << "  " << "Output Num : " << getIoSize(CKT_OUT) << endl;
      cout << "  " << "Output Bits : " << bit_count << endl;
   }
   // PIOs
   if (getIoSize(CKT_IO) > 0) {
      bit_count = 0;
      for ( CktCellAry::const_iterator pos = _ioList[CKT_IO].begin(); pos != _ioList[CKT_IO].end(); pos++ ) bit_count += (*pos)->getOutWidth();
      cout << "  " << "InOut Num : " << getIoSize(CKT_IO) << endl;
      cout << "  " << "InOut Bits : " << bit_count << endl;
   }
}

void
CktModule::printVerbose(bool design) const
{
   if (design) cout << "  " << "Report Verbose : " << "Design is " << "\"" << getModuleName() << "\"" << endl;
   else cout << "  " << "Report Verbose : " << "Ckt is " << "\"" << getModuleName() << "\"" << endl;
   cout << "  " << "==================================================" << endl;
   // PIs
   if (getIoSize(CKT_IN) > 0) {
      cout << "  " << "input : ";
      for ( CktCellAry::const_iterator pos = _ioList[CKT_IN].begin(); pos != _ioList[CKT_IN].end(); pos++ ) 
         cout << (*pos)->getOutPin()->getName() << "[" << ((*pos)->getOutWidth() - 1) << ":0]  ";
   }
   // POs
   if (getIoSize(CKT_OUT) > 0) {
      cout << endl;
      cout << "  " << "output : ";
      for ( CktCellAry::const_iterator pos = _ioList[CKT_OUT].begin(); pos != _ioList[CKT_OUT].end(); pos++ ) 
         cout << (*pos)->getOutPin()->getName() << "[" << ((*pos)->getOutWidth() - 1) << ":0]  ";
   }
   // PIOs
   if (getIoSize(CKT_IO) > 0) {
      cout << endl;
      cout << "  " << "inout : ";
      for ( CktCellAry::const_iterator pos = _ioList[CKT_IO].begin(); pos != _ioList[CKT_IO].end(); pos++ ) 
         cout << (*pos)->getOutPin()->getName() << "[" << ((*pos)->getOutWidth() - 1) << ":0]  ";
   }
   cout << endl;
}

void
CktModule::writeBLIF() const
{
   for (CktCellAry::const_iterator pos = _cellList.begin(); pos != _cellList.end(); pos++ ) {
      if ( ((*pos)->getCellType() != CKT_PI_CELL)     ) {//&&
           //((*pos)->getCellType() != CKT_PO_CELL)        )
         (*pos)->writeBLIF();
      }
   }
}

void
CktModule::writeBLIF_Input() const //.inputs ...
{
   int i = 0;
   CktPiCell* piCell;
   for(CktCellAry::const_iterator pos = _ioList[CKT_IN].begin(); pos != _ioList[CKT_IN].end(); pos++) {
      ++i;
      assert ((*pos)->getCellType() == CKT_PI_CELL);
      piCell = static_cast< CktPiCell* > (*pos);
      piCell->writeBLIF_PI();
      if ((i%5) == 0) {
         cktOutFile << "\\";
         endl(cktOutFile);
      }
   }
}

void
CktModule::writeBLIF_Output() const //.outputs ...
{
   int i = 0;
   CktPoCell* poCell;
   for(CktCellAry::const_iterator pos = _ioList[CKT_OUT].begin(); pos != _ioList[CKT_OUT].end(); pos++) {
      ++i;
      assert ((*pos)->getCellType() == CKT_PO_CELL);
      poCell = static_cast< CktPoCell* > (*pos);
      poCell->writeBLIF_PO();
      if ((i%5) == 0) {
         cktOutFile << "\\";
         endl(cktOutFile);
      }
   }
}

CktModule*
CktModule::defModule(bool isNew, string& modName, string& insName)
{
   CktModule* toAdd;
   VlpHierTreeNode* curNode = VLDesign.getCurHTNode();

   if (isNew) {
      toAdd = new CktModule();
      toAdd->setModuleName(modName);
      VLDesign.setModule(toAdd);
      
      VlpHierTreeNode* newChild = new VlpHierTreeNode(insName, toAdd, 0);
      curNode->setChild(newChild);
      newChild->setParent(curNode);
      VLDesign.setCurHTNode(newChild); 
      SynSharNodeMap childNodeMap;
      (CREMap->outPinMap).push(childNodeMap);
   }
   else {
      VlpHierTreeNode* newChild = VLDesign.getHTNode(modName);
      // curNode->setChild(newChild);
      // newChild->setParent(curNode);
      SynSharNodeMap newNodeMap;
      (CREMap->outPinMap).push(newNodeMap);
      toAdd = newChild->dfsBuildCktModule(insName);
      (CREMap->outPinMap).pop();
      creModuleCell(VLDesign.getCktModule(modName), insName);
   }

   return toAdd;
}

bool 
CktModule::checkSameName(string& name)
{
   CktOutPin* OutPin = ((CREMap->outPinMap).top()).getSharPin(name);
   if (!(OutPin == 0)) {
      cout << "string name " << name << " exists!" << endl;
      return false;
   }
   return true;
}

/*bool
CktModule::checkModuleName(string& modName)
{
   if (VLDesign.getCktModule(modName) != 0) return false;
   else return true;
}*/

int
CktModule::getOutPinWidth(const string& outName) const {
   CktOutPin* myOutPin = ((CREMap->outPinMap).top()).getSharPin(outName);
   if (myOutPin) return myOutPin->getBusWidth();
   else return 0;
}

int
CktModule::checkConn(string& modulename)
{
   CktModule* myModule = VLDesign.getCktModule(modulename);
   if (myModule == 0) return 1;  // modulename does not exist

   bool result = printMod(modulename, false);
   if (!result) return 2;
   return 0;  // NO connection error.
}

void 
CktModule::cellReg(CktCell* Cell, CktOutPin* OutPin, const string& name, const int& width, bool t, const CktCellType& Type)
{
   OutPin->setBusId(VLDesign.genBusId(width, 0, width - 1, t));
   OutPin->setName(name);
   ((CREMap->outPinMap).top()).insert(name, OutPin);
   if (Type == CKT_PI_CELL)
      insertIoCell( Cell, name, CKT_IN );
   else if (Type == CKT_PO_CELL)
      insertIoCell( Cell, name, CKT_OUT );
   insertCell(Cell);
}

bool 
CktModule::creIO(int* width, string* name, const int& num, const CktCellType& Type)
{
   CktInPin* myInPin;
   CktOutPin* myOutPin;
   CktCell* myCell;

   int _width;
   string _name;
   for (int i = 0; i < num; ++i) {
      _width = width[i];
      _name = name[i];
      //if (!(checkSameName(_name))) {
      //   num = i;
      //   return false;
      //}
      if (Type == CKT_PO_CELL) {
         myCell = new CktPoCell(true, myInPin, myOutPin);
         myCell->getOutPin()->setName(_name);
         // For PO Only
         myOutPin = ((CREMap->outPinMap).top()).getSharPin(_name);
         if (!myOutPin) myOutPin = new CktOutPin;
         myCell->getInPin(0)->connOutPin(myOutPin);
         myOutPin->connInPin(myCell->getInPin(0));
      }
      else myCell = new CktPiCell(true, myOutPin);
      cellReg(myCell, myOutPin, _name, _width, busDirectionDefault, Type);
   }
   return true;
}

bool
CktModule::creIO(const int& width, vector<string>& name, const CktCellType& Type)
{
   CktInPin* myInPin;
   CktOutPin* myOutPin;
   CktCell* myCell;

   string _name;
   for (size_t i = 0; i < name.size(); ++i) {
      _name = name[i];
      //if (!(checkSameName(_name))) return false;
      if (Type == CKT_PO_CELL) {
         myCell = new CktPoCell(true, myInPin, myOutPin);
         myCell->getOutPin()->setName(_name);
         // For PO Only
         myOutPin = ((CREMap->outPinMap).top()).getSharPin(_name);
         if (!myOutPin) myOutPin = new CktOutPin;
         myCell->getInPin(0)->connOutPin(myOutPin);
         myOutPin->connInPin(myCell->getInPin(0));
      }
      else myCell = new CktPiCell(true, myOutPin);
      cellReg(myCell, myOutPin, _name, width, busDirectionDefault, Type);
   }
   return true;
}

CktCell*
CktModule::creCell(const CktCellType& CellType, const int& num, int& width, const string& name)
{
   CktOutPin* myOutPin;
   myOutPin = ((CREMap->outPinMap).top()).getSharPin(name);
   if (!myOutPin) myOutPin = new CktOutPin;
   CktCell* myCell = NULL;

   CktInPin* myInPin = new CktInPin;
   
   if (num == 1) {
      if (CellType == CKT_BUF_CELL) myCell = new CktBufCell(false, myInPin, myOutPin);
      else if (CellType == CKT_INV_CELL) myCell = new CktInvCell(false, myInPin, myOutPin);
      else if (
            CellType == CKT_AND_CELL || CellType == CKT_OR_CELL || CellType == CKT_NAND_CELL || 
            CellType == CKT_NOR_CELL || CellType == CKT_XOR_CELL || CellType == CKT_XNOR_CELL
            )
      {
         if (CellType == CKT_AND_CELL) myCell = new CktAndCell(false, myInPin, myOutPin);
         else if (CellType == CKT_OR_CELL) myCell = new CktOrCell(false, myInPin, myOutPin);
         else if (CellType == CKT_NAND_CELL) myCell = new CktNandCell(false, myInPin, myOutPin);
         else if (CellType == CKT_NOR_CELL) myCell = new CktNorCell(false, myInPin, myOutPin);
         else if (CellType == CKT_XOR_CELL) myCell = new CktXorCell(false, myInPin, myOutPin);
         else if (CellType == CKT_XNOR_CELL) myCell = new CktXnorCell(false, myInPin, myOutPin);
         else { Msg(MSG_ERR) << "Non-Logical Gates should have more than 2 inputs!" << endl; return 0; }
         width = 1;  // Reduced Logic Gates
      }
   }
   else {
      CktInPin* myInPin2 = new CktInPin;
      if (CellType == CKT_AND_CELL) myCell = new CktAndCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_OR_CELL) myCell = new CktOrCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_NAND_CELL) myCell = new CktNandCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_NOR_CELL) myCell = new CktNorCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_XOR_CELL) myCell = new CktXorCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_XNOR_CELL) myCell = new CktXnorCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_ADD_CELL) myCell = new CktAddCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_SUB_CELL) myCell = new CktSubCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_MULT_CELL) myCell = new CktMultCell(false, myInPin, myInPin2, myOutPin);
      //else if (CellType == CKT_DIV_CELL) myCell = new CktDivCell(false, myInPin, myInPin2, myOutPin);
      //else if (CellType == CKT_MODULO_CELL) myCell = new CktModuloCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_EQUALITY_CELL) myCell = new CktEqualityCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_LESS_CELL) myCell = new CktLessCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_LEQ_CELL) myCell = new CktLeqCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_GREATER_CELL) myCell = new CktGreaterCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_GEQ_CELL) myCell = new CktGeqCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_SHL_CELL) myCell = new CktShlCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_SHR_CELL) myCell = new CktShrCell(false, myInPin, myInPin2, myOutPin);
      else if (CellType == CKT_MERGE_CELL) {
         myCell = new CktMergeCell(false, myOutPin);
         myInPin->connCell(myCell); myCell->connInPin(myInPin);
         myInPin2->connCell(myCell); myCell->connInPin(myInPin2);
      }
      else if (CellType == CKT_BUF_CELL || CellType == CKT_INV_CELL) {
         Msg(MSG_ERR) << "BUF / INV should contain only one input!" << endl; return 0;
      }
      else { Msg(MSG_ERR) << "Specified Gate should not contain 2 inputs!" << endl; return 0; }

      if (num > 2) {
         for (int i = 0; i < num - 2; ++i) {
            myInPin = new CktInPin;
            myInPin->connCell(myCell);
            myCell->connInPin(myInPin);
         }
      }
   }
   cellReg(myCell, myOutPin, name, width, busDirectionDefault, CellType);
   return myCell;
}

void
CktModule::creConstCell(const int& width, const string& name, const string& value_str) {
   CktOutPin* myOutPin = ((CREMap->outPinMap).top()).getSharPin(name);
   CktCell* myCell = NULL;
   if (!myOutPin) myOutPin = new CktOutPin;
   Bv2 value(myInt2Str(width) + value_str);
   myCell = new CktConstCell(value.value(), myInt2Str(width) + value_str);
   myCell->connOutPin(myOutPin); myOutPin->connCell(myCell);
   cellReg(myCell, myOutPin, name, width, busDirectionDefault, CKT_CONST_CELL);
}

void
CktModule::creModuleCell(CktModule* module, const string& insname)
{
   CktModuleCell* modcell = new CktModuleCell();
   modcell->setModulePtr(module);
   modcell->setModuleName(module->getModuleName());
   modcell->setInstName(insname);
   
   CktInPin* InPins = NULL;;
   CktOutPin* OutPins = NULL;
   CktOutPin* pincell = NULL;
   unsigned pinnum;
   
   // Connect InPins to cktModuleCell*
   pinnum = module->getIoSize(CKT_IN);
   for (unsigned i = 0; i < pinnum; ++i) {
      InPins = new CktInPin;
      InPins->connCell(modcell);
      modcell->connInPin(InPins);
   }
   // Connect OutPins to cktModuleCell*
   pinnum = module->getIoSize(CKT_OUT);
   for (unsigned i = 0; i <pinnum; ++i) {
      OutPins = new CktOutPin;
      OutPins->connCell(modcell);
      modcell->connOutPin(OutPins);
      
      // reg CktOutPin name
      pincell = module->getIoCell(CKT_OUT, i)->getOutPin();
      string name = insname + '.' + pincell->getName(); // format : insname.pinname
      OutPins->setName(name);
      OutPins->setBusId(pincell->getBusId());
      ((CREMap->outPinMap).top()).insert(name, OutPins);
   }   
   // regCell
   ((CREMap->outPinMap).top()).insert(insname, OutPins);
   insertCell(modcell);
}

void
CktModule::creDffCell(const int& width, const string& name, const string& clk) {
   CktInPin* myInPin = new CktInPin;
   CktOutPin* myOutPin = ((CREMap->outPinMap).top()).getSharPin(name);
   if (!myOutPin) myOutPin = new CktOutPin;
   CktDffCell* myCell = new CktDffCell(false, myInPin, myOutPin);
   CktOutPin* clkOutPin = ((CREMap->outPinMap).top()).getSharPin(clk);
   if (!clkOutPin) { Msg(MSG_ERR) << "Clock signal should be declared first!" << endl; assert (0); }
   CktInPin* clkInPin = new CktInPin;
   clkInPin->connOutPin(clkOutPin); clkOutPin->connInPin(clkInPin);
   clkInPin->connCell(myCell); myCell->connClk(clkInPin);
   cellReg(myCell, myOutPin, name, width, busDirectionDefault, CKT_DFF_CELL);
}

void
CktModule::creMuxCell(const int& width, const string& name) {
   CktInPin *finPin = new CktInPin, *tinPin = new CktInPin, *sinPin = new CktInPin;
   CktOutPin* myOutPin = ((CREMap->outPinMap).top()).getSharPin(name);
   if (!myOutPin) myOutPin = new CktOutPin;
   CktMuxCell* myCell = new CktMuxCell(false, finPin, tinPin, sinPin, myOutPin);
   cellReg(myCell, myOutPin, name, width, busDirectionDefault, CKT_MUX_CELL);
}

void
CktModule::creSplitCell(const int& width, const string& name, const int& msb, const int& lsb) {
   CktInPin* myInPin = new CktInPin;
   CktOutPin* myOutPin = ((CREMap->outPinMap).top()).getSharPin(name);
   if (!myOutPin) myOutPin = new CktOutPin;
   vector<CktOutPin*> outPinAry; outPinAry.clear(); outPinAry.push_back(myOutPin);
   CktSplitCell* myCell = new CktSplitCell(false, myInPin, outPinAry);
   myCell->insertBusId(VLDesign.genBusId(width, lsb, msb, busDirectionDefault));
   cellReg(myCell, myOutPin, name, width, busDirectionDefault, CKT_SPLIT_CELL);
}

void
CktModule::constrainInput(CktCell* myCell) {
   if (myCell->getInPinSize() <= 2) return;
   if (myCell->getCellType() == CKT_MUX_CELL || myCell->getCellType() == CKT_MERGE_CELL || myCell->getCellType() == CKT_MODULE_CELL) return;
   int width = myCell->getOutPin()->getBusWidth();
   CktOutPin* tempOutPin = ((CREMap->outPinMap).top()).getSharPin(myCell->getInPin(0)->getOutPin()->getName());
   assert (tempOutPin); tempOutPin->eraseInPin(myCell->getInPin(0));
   CktCell* tempCell = 0;
   for (unsigned i = 1; i < myCell->getInPinSize() - 1; ++i) {
      tempCell = creCell(myCell->getCellType(), 2, width, "qutertl_inter_temp_" + myCell->getOutPin()->getName() + "_" + myInt2Str(i - 1));
      assert (tempCell->getInPinSize() == 2);
      tempCell->getInPin(0)->connOutPin(tempOutPin);
      tempOutPin->connInPin(tempCell->getInPin(0));
      tempOutPin = ((CREMap->outPinMap).top()).getSharPin(myCell->getInPin(i)->getOutPin()->getName());
      assert (tempOutPin); tempOutPin->eraseInPin(myCell->getInPin(i));
      tempCell->getInPin(1)->connOutPin(tempOutPin);
      tempOutPin->connInPin(tempCell->getInPin(1));
      tempOutPin = tempCell->getOutPin();
   }
   CktOutPin* restOutPin = myCell->getInPin(myCell->getInPinSize() - 1)->getOutPin();
   assert (restOutPin); restOutPin->eraseInPin(myCell->getInPin(myCell->getInPinSize() - 1));
   myCell->resetInPinSize(2); assert (myCell->getInPinSize() == 2);
   tempOutPin->connInPin(myCell->getInPin(0));
   myCell->getInPin(0)->connOutPin(tempOutPin);
   restOutPin->connInPin(myCell->getInPin(1));
   myCell->getInPin(1)->connOutPin(restOutPin);
}

int 
CktModule::creConn(string& f_cell, string& t_cell, string& fpin_index, string& tpin_index)
{
   int findex;
   int tindex;
   CktCell* myFromCell;  // From_ModuleCell
   CktCell* myToCell;   // To_ModuleCell
   CktInPin* myToInPin;
   CktCellType myCellType;  // To_Cell celltype
   unsigned f_busid;
   unsigned t_busid;
 
   // Ckeck FromCell and its FromPin assignment
   CktOutPin* myFromOutPin = ((CREMap->outPinMap).top()).getSharPin(f_cell);
   if (myFromOutPin == 0) return 1;  // f_cell ERROR
   else { 
      myFromCell = myFromOutPin->getCell();
      if (myFromCell->getCellType() == CKT_MODULE_CELL) { // Ckeck if it is a ModuleCell
         string RealName = f_cell + '.' + fpin_index;
         myFromOutPin = ((CREMap->outPinMap).top()).getSharPin(RealName);
         if (myFromOutPin == 0) return 4; // ModuleCell, fpin_index ERROR
         else f_busid = myFromOutPin->getBusId();
      }
      else {
         if (!myStr2Int(fpin_index, findex)) return 3;  // NOT ModuleCell, fpin_index ERROR
         else if (findex < 0) return 3;  // NOT ModuleCell, fpin_index ERROR
         else f_busid = myFromOutPin->getBusId();
      }
   }
   // Ckeck ToCell and its ToPin assignment
   CktOutPin* myToOutPin = ((CREMap->outPinMap).top()).getSharPin(t_cell);
   if (myToOutPin == 0) return 2;  // t_cell ERROR
   else {
      myToCell = myToOutPin->getCell();
      myCellType = myToCell->getCellType();
      if (myCellType != CKT_MODULE_CELL) {
         if (!myStr2Int(tpin_index, tindex)) return 5;  // NOT ModuleCell, tpin_index ERROR
         if (tindex < 0) return 5;  // NOT ModuleCell, tpin_index ERROR
         if ((size_t)tindex >= myToCell->getInPinSize()) return 5;  // NOT ModuleCell, tpin_index ERROR
         myToInPin = myToCell->getInPin((unsigned)tindex);  // Get its CktInPin
         t_busid = myToOutPin->getBusId();
      }
      else {
         CktModule* RealModule = static_cast<CktModuleCell* >(myToCell)->getModulePtr();
         if (RealModule->getIoCell(tpin_index) == 0) return 6; // ModuleCell, tpin_index ERROR
         else {
            // Get the real InPin from ModuleCell
            t_busid = RealModule->getIoCell(tpin_index)->getOutPin()->getBusId();
            tindex = RealModule->getIoType(CKT_IN, tpin_index);
            assert(tindex < (int)RealModule->getIoSize(CKT_IN));
            myToInPin = static_cast<CktModuleCell* >(myToCell)->getInPin(tindex);  // Get its CktInPin
         }
      }
   }
  
  // check if the connection is invalid :
   switch(myCellType) {
      // myCellType can't be CKT_PI_CELL
      case CKT_PI_CELL:
         return 2;
         break;
      case CKT_MODULE_CELL:
      // For myCellType = CKT_BUF_CELL or CKT_INV_CELL, two outpins should have the same _busId
      case CKT_BUF_CELL:
      case CKT_INV_CELL:
         if ( f_busid != t_busid ) return 7;
         break;
      // These cases, if #myToCell::InPin > 1, check two outpins should have the same _busid
      case CKT_AND_CELL:
      case CKT_OR_CELL:
      case CKT_NAND_CELL:
      case CKT_NOR_CELL:
      case CKT_XOR_CELL:
      case CKT_XNOR_CELL:
         if (myToCell->getInPinSize() > 1) {
           if ( f_busid != t_busid ) return 7;
         }
         break;
      case CKT_PO_CELL:
      case CKT_PIO_CELL:
         break;
      default:
         return 8;
         break;
   }
   // create connection
   myToInPin->connOutPin(myFromOutPin);
   myFromOutPin->connInPin(myToInPin);
   
   return 0;
}

void
CktModule::crePreOut(const string& output, vector<string>& inputs)
{
   CktInPin* myInPin;
   CktOutPin* myOutPin;
   CktCell* myCell = (((CREMap->outPinMap).top()).getSharPin(output))->getCell();

   assert(myCell);
   assert((myCell->getInPinSize()) == (inputs.size()));
   
   for (size_t i = 0; i < myCell->getInPinSize(); ++i) {
      myInPin = myCell->getInPin(i);
      myOutPin = ((CREMap->outPinMap).top()).getSharPin(inputs[i]);
      if (!myOutPin) {
         myOutPin = new CktOutPin;
         myOutPin->setName(inputs[i]);
         myOutPin->setBusId(VLDesign.genBusId(1, 0, 0, busDirectionDefault));
         ((CREMap->outPinMap).top()).insert(inputs[i], myOutPin);
      }
      myInPin->connOutPin(myOutPin);
      myOutPin->connInPin(myInPin);
   }
   constrainInput(myCell);  // Constrain gate input size to 2
}

void
CktModule::SOPGate(vector<string>& inputs, string& output, set<string>& sop, bool onset)
{
   CktInPin* myInPin;
   CktOutPin* myOutPin;
   CktCell* myCell;
   vector<CktOutPin*> gateoutpins;
   vector<CktOutPin*> andoutpins;
   char num[32];
   size_t arraysize, i, j;
   int width = 1;

   itoa(netnum, num, 10);
   string name = tempnet + num;
   
   for (set<string>::const_iterator it = sop.begin(); it != sop.end(); ++it) {
      for (j = 0; j < inputs.size(); ++j) {
         if ((*it)[j] == '0') {
            // Create INV --> Temp Cell
            myCell = new CktInvCell(true, myInPin, myOutPin);
            while(((CREMap->outPinMap).top()).getSharPin(name)) {
               itoa(++netnum, num, 10);
               name = tempnet + num;
            }
            cellReg(myCell, myOutPin, name, width, busDirectionDefault, CKT_INV_CELL);

            // Chengyin debug
            //cout << "INV added : " << "input = " << inputs[j] << ", output = " << name << endl;

            itoa(++netnum, num, 10);
            name = tempnet + num;
            gateoutpins.push_back(myOutPin);
            myOutPin = ((CREMap->outPinMap).top()).getSharPin(inputs[j]);
            if (!myOutPin) {
               myOutPin = new CktOutPin;
               myOutPin->setName(inputs[j]);
               myOutPin->setBusId(VLDesign.genBusId(1, 0, 0, busDirectionDefault));
               ((CREMap->outPinMap).top()).insert(inputs[j], myOutPin);
            }
            myInPin->connOutPin(myOutPin);
            myOutPin->connInPin(myInPin);
         }
         else if ((*it)[j] == '1') {
            myOutPin = ((CREMap->outPinMap).top()).getSharPin(inputs[j]);
            if (!myOutPin) {
               myOutPin = new CktOutPin;
               myOutPin->setName(inputs[j]);
               myOutPin->setBusId(VLDesign.genBusId(1, 0, 0, busDirectionDefault));
               ((CREMap->outPinMap).top()).insert(inputs[j], myOutPin);
            }
            gateoutpins.push_back(myOutPin);
         }
      }
      // Build AND Gate
      // Add Gate OutPins
      myCell = new CktAndCell(true, myInPin, myOutPin);
      arraysize = gateoutpins.size();
      if (arraysize >= 2) {
         for (i = 0; i < arraysize - 1; ++i) {
            myInPin = new CktInPin;
            myInPin->connCell(myCell);
            myCell->connInPin(myInPin);
         }
      }
      while(((CREMap->outPinMap).top()).getSharPin(tempnet + num)) {
         itoa(++netnum, num, 10);
         name = tempnet + num;
      }
      cellReg(myCell, myOutPin, name, width, busDirectionDefault, CKT_AND_CELL);

      // Chengyin debug
      //cout << "AND Gate : " << "\ninputs = ";
      //for(i = 0; i < gateoutpins.size(); ++i) cout << gateoutpins[i]->getName() << "\t";
      //cout << "\noutputs = " << name << endl;

      itoa(++netnum, num, 10);
      name = tempnet + num;
      andoutpins.push_back(myOutPin);
      for (i = 0; i < myCell->getInPinSize(); ++i) {
         myInPin = myCell->getInPin(i);
         myOutPin = gateoutpins[i];
         myInPin->connOutPin(myOutPin);
         myOutPin->connInPin(myInPin);
      }
      gateoutpins.clear();
      
      constrainInput(myCell);
   }
   // Build OR Gate
   myInPin = new CktInPin;
   myOutPin = ((CREMap->outPinMap).top()).getSharPin(output);
   if (!myOutPin) {
      myOutPin = new CktOutPin;
      myOutPin->setName(output);
      myOutPin->setBusId(VLDesign.genBusId(1, 0, 0, busDirectionDefault));
      ((CREMap->outPinMap).top()).insert(output, myOutPin);
   }
   myCell = new CktOrCell(false, myInPin, myOutPin);
   arraysize = andoutpins.size();
   if (arraysize >= 2) {
      for (i = 0; i < arraysize - 1; ++i) {
         myInPin = new CktInPin;
         myInPin->connCell(myCell);
         myCell->connInPin(myInPin);
      }
   }
   cellReg(myCell, myOutPin, output, width, busDirectionDefault, CKT_OR_CELL);

   // Chengyin debug
   //cout << "OR Gate : " << "\ninputs = ";
   //for(i = 0; i < andoutpins.size(); ++i) cout << andoutpins[i]->getName() << "\t";
   //cout << "\noutputs = " << output << endl;
   
   for (i = 0; i < myCell->getInPinSize(); ++i) {
      myInPin = myCell->getInPin(i);
      myOutPin = andoutpins[i];
      myInPin->connOutPin(myOutPin);
      myOutPin->connInPin(myInPin);
   }
   
   constrainInput(myCell);
}

CktModule*
CktModule::getParentModule() 
{
   VlpHierTreeNode* curModNode = VLDesign.getCurHTNode();
   curModNode = curModNode->parent();
   if (curModNode == 0) {
      // Jump out of MODULE_NAME> prompt mode, back to FE> prompt mode
      cout << "Goodbye, Create Module mode!" << endl;
      return 0;
   }
   else {
      VLDesign.setCurHTNode(curModNode);
      cout << "Back to Module : ModuleName = " << curModNode->getCktModule()->getModuleName()
           << ", InstanceName = " << curModNode->instName() << endl;
      (CREMap->outPinMap).pop();
      return  const_cast<CktModule*>(curModNode->getCktModule());
   }
}

bool
CktModule::printMod(string& modulename, bool print) const
{
   CktModule* myModule = VLDesign.getCktModule(modulename);
   if (myModule == 0) return 0;  // modulename does not exist
   
   CktCell* myCell;
   CktCell* tempCell;
   CktInPin* myInPin;
   CktOutPin* myOutPin;
   CktModule* F_Module; // If from cell is CKT_MODULE_CELL, set F_Module to this module
   CktModule* T_Module; // If to cell is CKT_MODULE_CELL, set T_Module to this module
   unsigned mod_index;
   unsigned index;
   bool iserror = false;
   
   if (print) {
      cout << "Module name = " << myModule->getModuleName() << endl;
      cout << "------------------------------------------" << endl;
      cout << "Total Cell Num = " << myModule->_cellList.size() << endl;
      cout << "The format: OutCell(CellType) --> InCell[FaninIndex](CellType)" << endl;
   }

   for (unsigned i = 0; i < myModule->_cellList.size(); ++i) {
      myCell = myModule->_cellList[i];
      if (myCell->getCellType() == CKT_MODULE_CELL) { // ModuleCell has many OutPins
         mod_index = static_cast<CktModuleCell* >(myCell)->getOutPinSize();
      }
      else mod_index = 1;
      for (unsigned outnum = 0; outnum < mod_index ; outnum++) {
         if (myCell->getCellType() == CKT_MODULE_CELL)
            myOutPin = static_cast<CktModuleCell* >(myCell)->getOutPin(outnum);
         else myOutPin = myCell->getOutPin();

         if (myOutPin->getInPinSize() == 0) {  // NO fanout
            if (print) {
               cout << myOutPin->getName() << "(" << myCell->getCellTypeString()
                    << ")" << " --> " << "NULL" << endl;
            }
            else if (myCell->getCellType() != CKT_PO_CELL) {
               if (!iserror) cout << "Invalid cell list: " << endl;
               cout << myOutPin->getName() << "(" << myCell->getCellTypeString() << ")" 
                    << " --> " << "NULL" << endl;
               iserror = true;
            }
         }
         else {
            if (print) {
               for (unsigned j = 0; j < myOutPin->getInPinSize(); ++j) {
                  myInPin = myOutPin->getInPin(j);
                  tempCell = myInPin->getCell();
                  index = tempCell->getInPinNum(myInPin);  // to get FaninIndex
                  assert (index < tempCell->getInPinSize());  // if not, myInPin is not found.

                  if (myCell->getCellType() == CKT_MODULE_CELL) {
                     F_Module = static_cast<CktModuleCell* >(myCell)->getModulePtr();
                     cout << static_cast<CktModuleCell* >(myCell)->getInstName()
                          << "." << (F_Module->getIoCell(CKT_OUT, outnum))->getOutPin()->getName() << "(";
                     cout << "Module : "
                          << static_cast<CktModuleCell* >(myCell)->getModuleName(); 
                  }
                  else cout << myOutPin->getName() << "(" << myCell->getCellTypeString();
                  cout << ")" << " --> ";
                  if (tempCell->getCellType() == CKT_MODULE_CELL) {
                     T_Module = static_cast<CktModuleCell* >(tempCell)->getModulePtr();
                     cout << static_cast<CktModuleCell* >(tempCell)->getInstName()
                          << "." << T_Module->getIoName(index) << "(";
                     cout << "Module : " 
                          << static_cast<CktModuleCell* >(tempCell)->getModuleName(); 
                  }
                  else {
                     cout << tempCell->getOutPin()->getName() 
                          << "(index = " << index << ") " << "(" << tempCell->getCellTypeString();
                  } 
                  cout << ")" << endl;
               }
            }
         }
      }
   }
   if (iserror) return 0;
   return 1;
}

bool
CktModule::printFlatMod() const
{
   CktModule* myModule = VLDesign.getFltModule();
   if (myModule == 0) {// modulename does not exist
      cout << "You have to do \"Flatten\" first!" << endl;
      return 0;
   }
   CktCell* myCell;
   CktCell* tempCell;
   CktInPin* myInPin;
   CktOutPin* myOutPin;
   unsigned mod_index = 1;
   unsigned index;
   unsigned temp = 0;
   bool iserror = false;
   
   cout << "Module name = " << myModule->getModuleName() << endl;
   cout << "------------------------------------------" << endl;
   cout << "Total Cell Num = " << myModule->_cellList.size() << endl;
   cout << "The format: OutCell(CellType) --> InCell[FaninIndex](CellType)" << endl;

   for (unsigned i = 0; i < myModule->_cellList.size(); ++i) {
      myCell = myModule->_cellList[i];
      if (myCell->getCellType() == CKT_SPLIT_CELL) { // SplitCell has many OutPins
         mod_index = static_cast<CktSplitCell* >(myCell)->getOutPinSize();
      }
      for (unsigned outnum = 0; outnum < mod_index ; outnum++) {
         if (myCell->getCellType() == CKT_SPLIT_CELL)
            myOutPin = static_cast<CktSplitCell* >(myCell)->getOutPin(outnum);
         else myOutPin = myCell->getOutPin();

         if (myOutPin->getInPinSize() == 0) {  // NO fanout
            cout << myOutPin->getName() << "(" << myCell->getCellTypeString()
                 << ")" << " --> " << "NULL" << endl;
         }
         else {
            for (unsigned j = 0; j < myOutPin->getInPinSize(); ++j) {
               myInPin = myOutPin->getInPin(j);
               tempCell = myInPin->getCell();
               index = tempCell->getInPinNum(myInPin);  // to get FaninIndex
               //assert (index < tempCell->getInPinSize());  // if not, myInPin is not found.

               cout << myOutPin->getName() << "(" << myCell->getCellTypeString() << ")" << " --> ";
               if (tempCell->getCellType() == CKT_SPLIT_CELL) 
                  cout << static_cast<CktSplitCell* >(tempCell)->getOutPin(temp)->getName();
               else cout << tempCell->getOutPin()->getName();
               cout << "(index = " << index << ") " << "(" << tempCell->getCellTypeString() << ")"; 
               cout << endl;
            }
         }
      }
   }
   if (iserror) return 0;
   return 1;
}

bool
CktModule::printPIGate(string gateName, int level) const
{
   if (_cellList.size() != 0) {
      CktOutPin* tempOutPin;
      unsigned temp = 0;
      for (CktCellAry::const_iterator pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
         if ((*pos)->getCellType() == CKT_SPLIT_CELL) tempOutPin = static_cast<CktSplitCell* >((*pos))->getOutPin(temp);
         else tempOutPin = (*pos)->getOutPin();
         if (tempOutPin->getName() == gateName) {
            cout << "  " << "Report Gate Fanins : " << "Gate is " << "\"" << gateName << "\""
                 << "  " << "Design is " << "\"" << getModuleName() << "\"" << endl;
            cout << "  " << "Format : " << "CellType Level CellName" << endl;
            cout << "  " << "         " << "|-> 1st Level Fanins ..." << endl;
            cout <<  "  " << CellTypeString[(*pos)->getCellType()] << " " << (*pos)->getCellID() << " " << gateName << endl;
            printPI((*pos), 1, level, 2);
            return true;
         }
      }
   }
   return false;
}

bool
CktModule::printPOGate(string gateName, int level) const
{
   if (_cellList.size() != 0) {
      CktOutPin* tempOutPin;
      unsigned temp = 0;
      for (CktCellAry::const_iterator pos = _cellList.begin(); pos != _cellList.end(); ++pos) {
         if ((*pos)->getCellType() == CKT_SPLIT_CELL) tempOutPin = static_cast<CktSplitCell* >((*pos))->getOutPin(temp);
         else tempOutPin = (*pos)->getOutPin();
         if (tempOutPin->getName() == gateName) {
            cout << "  " << "Report Gate Fanouts : " << "Gate is " << "\"" << gateName << "\""
                 << "  " << "Design is " << "\"" << getModuleName() << "\"" << endl;
            cout << "  " << "Format : " << "CellType Level CellName" << endl;
            cout << "  " << "         " << "|-> 1st Level Fanouts ..." << endl;
            cout << "  " << CellTypeString[(*pos)->getCellType()] << " " << (*pos)->getCellID() << " " << gateName << endl;
            printPO((*pos), 1, level, 2);
            return true;
         }
      }
   }
   return false;
}

string int2str(int x)
{
   string s;
   stringstream ss(s);
   ss << x;
   return ss.str();
}

void
CktModule::printPI(CktCell* MyCell, int level, int Maxlevel, unsigned shift) const
{
   if (level <= Maxlevel) {
      //unsigned length = 0;
      //unsigned temp = 0;
      //string str = "";
      /*
      for (unsigned i = 0; i < MyCell->getInPinSize(); ++i) {
         str = "";
         str = " --> " + MyCell->getInPin(i)->getOutPin()->getName() + "(" + MyCell->getInPin(i)->getFinCell()->getCellTypeString() + ", " + int2str(level) + ")";
         if (str.size() > length) length = str.size();
      }
      */
      for (unsigned i = 0; i < MyCell->getInPinSize(); ++i) {
         for (unsigned t = 0; t < shift; ++t) cout << " ";
         switch (MyCell->getCellType()) {
            case CKT_DFF_CELL : 
            	cout << "|-> " << CellTypeString[MyCell->getInPin(i)->getFinCell()->getCellType()] << " "
                               << MyCell->getInPin(i)->getFinCell()->getCellID() << " "
                               << MyCell->getInPin(i)->getOutPin()->getName() << endl;
            	break;
            /*
            case CKT_SPLIT_CELL : 
            	cout << setw(length) << left << " --> " + static_cast<CktSplitCell* >(MyCell)->getOutPin(temp)->getName() + "(" + MyCell->getInPin(i)->getFinCell()->getCellTypeString() + ", " + int2str(level) + ")";
            	printPI(MyCell->getInPin(i)->getFinCell(), level + 1, Maxlevel, shift + length);
            	break;
            */
            default : 
                cout << "|-> " << CellTypeString[MyCell->getInPin(i)->getFinCell()->getCellType()] << " "
                               << MyCell->getInPin(i)->getFinCell()->getCellID() << " "
                               << MyCell->getInPin(i)->getOutPin()->getName() << endl;
            	printPI(MyCell->getInPin(i)->getFinCell(), level + 1, Maxlevel, shift + 4);
            	break;
         }
      }
   }
}

void
CktModule::printPO(CktCell* MyCell, int level, int Maxlevel, unsigned shift) const
{
   if (level <= Maxlevel) {
      //unsigned length = 0;
      unsigned temp = 0;
      unsigned FoutSize;
      CktCell* tempCell;
      //string str = "";
      /*
      if (MyCell->getCellType() == CKT_SPLIT_CELL) {
         FoutSize = static_cast<CktSplitCell* >(MyCell)->getOutPin(temp)->getInPinSize();
         for (unsigned i = 0; i < FoutSize; ++i) {
            str = "";
            tempCell = static_cast<CktSplitCell* >(MyCell)->getFoutCell(temp, i);
            if (tempCell->getCellType() == CKT_SPLIT_CELL)
               str = " --> " + static_cast<CktSplitCell* >(tempCell)->getOutPin(temp)->getName() + "(" + tempCell->getCellTypeString() + ", " + int2str(level) + ")";
            else
               str = " --> " + tempCell->getOutPin()->getName() + "(" + tempCell->getCellTypeString() + ", " + int2str(level) + ")";
            if (str.size() > length) length = str.size();
         }
      }
      else {
         FoutSize = MyCell->getFoutSize();
         for (unsigned i = 0; i < FoutSize; ++i) {
            str = "";
            tempCell = MyCell->getFoutCell(i);
            if (tempCell->getCellType() == CKT_SPLIT_CELL)
               str = " --> " + static_cast<CktSplitCell* >(tempCell)->getOutPin(temp)->getName() + "(" + tempCell->getCellTypeString() + ", " + int2str(level) + ")";
            else 
               str = " --> " + tempCell->getOutPin()->getName() + "(" + tempCell->getCellTypeString() + ", " + int2str(level) + ")";
            if (str.size() > length) length = str.size();
         }
      }
      */
      if (MyCell->getCellType() == CKT_SPLIT_CELL)
         FoutSize = static_cast<CktSplitCell* >(MyCell)->getOutPin(temp)->getInPinSize();
      else FoutSize = MyCell->getFoutSize();

      for (unsigned i = 0; i < FoutSize; ++i) {
         for (unsigned t = 0; t < shift; ++t) cout << " ";
         if (MyCell->getCellType() == CKT_SPLIT_CELL) tempCell = static_cast<CktSplitCell* >(MyCell)->getFoutCell(temp, i);
         else tempCell = MyCell->getFoutCell(i);
         switch (tempCell->getCellType()) {
            case CKT_DFF_CELL :
                cout << "|-> " << CellTypeString[MyCell->getFoutCell(i)->getCellType()] << " "
                               << tempCell->getCellID() << " "
                               << tempCell->getOutPin()->getName() << endl;
            	break;
            case CKT_SPLIT_CELL :
                cout << "|-> " << CellTypeString[tempCell->getCellType()] << " "
                               << tempCell->getCellID() << " "
                               << static_cast<CktSplitCell* >(tempCell)->getOutPin(temp)->getName() << endl;
                printPO(tempCell, level + 1, Maxlevel, shift + 4);
            	break;
            default :
                cout << "|-> " << CellTypeString[tempCell->getCellType()] << " "
                               << tempCell->getCellID() << " "
                               << tempCell->getOutPin()->getName() << endl;
                printPO(tempCell, level + 1, Maxlevel, shift + 4);
            	break;
         }
      }
   }
}

void
CktModule::setOutPinMap(SynSharNodeMap& map)
{
   _outPinMap = map;
}

void 
CktModule::setSynVars(SynVarList& vars)
{
   _synVars = vars;
}

bool
CktModule::simMgr(const string& testFile)
{
   CktCellAry simCellAry;
   ifstream inTestFile;
   inTestFile.open(testFile.c_str());
   cktOutFile.openA("simResult.txt");
   if (!inTestFile) {
      cerr << "File " << testFile << " could not be opened." << endl;
      return false;
   }
   cout << "Module : " << getModuleName() << endl;
   genSimCellAry(simCellAry);
   initSim(simCellAry);
   while (!inTestFile.eof()) {
      oneCycleSim(simCellAry, inTestFile);
      outputInfo(simCellAry);
   }
   cktOutFile.close();
   return true;
   
/*
   cktOutFile.openA("simResult.txt");
   CktCellAry simCellAry;
   unsigned NUM = 10;
   cout << "Module : " << getModuleName() << endl;
   genSimCellAry(simCellAry);
   initSim(simCellAry);
   for (unsigned i = 0; i < NUM; ++i) {
      //preLoad(369, "5'b00000");
      randomPi();

      seqUpdate();    // copy _nsValue to _simValue
      combSim(simCellAry);
      recordNS();
      outputInfo(simCellAry);
      recordSt(369);
   }
   return true;*/
}

void
CktModule::genSimCellAry(CktCellAry& scAry) const
{
   CktCell* curCell;
   set<CktCell*>   cellSet;
   queue<CktCell*> cellQueue;
   stack<CktCell*> cellStack;
   cellSet.clear();

   for (unsigned i = 0; i < _ioList[CKT_IO].size(); ++i)
      cellQueue.push(_ioList[CKT_IO][i]);
   for (unsigned i = 0; i < _ioList[CKT_OUT].size(); ++i) 
      cellQueue.push(_ioList[CKT_OUT][i]);
   for (CktCellAry::const_iterator it = _seqCells.begin(); it != _seqCells.end(); ++it) {
      cellQueue.push((*it)->getFinCell(0));
      if (static_cast<const CktDffCell*>(*it)->getReset() != NULL)
         cellQueue.push(static_cast<const CktDffCell*>(*it)->getReset()->getFinCell());
      if (static_cast<const CktDffCell*>(*it)->getDefault() != NULL)
         cellQueue.push(static_cast<const CktDffCell*>(*it)->getDefault()->getFinCell());    
   }

   while (!cellQueue.empty()) {
      curCell = cellQueue.front();
      if (curCell->getCellType() != CKT_DFF_CELL && curCell->getCellType() != CKT_DLAT_CELL) {
         cellStack.push(curCell);
         curCell->collectFin(cellQueue);
      }
      cellQueue.pop();
   }

   while (!cellStack.empty()) {
      curCell = cellStack.top();
      if (find(cellSet.begin(), cellSet.end(), curCell) == cellSet.end()) {
         cellSet.insert(curCell);
         scAry.push_back(curCell);
      }
      cellStack.pop();
   }
}

void 
CktModule::initSim(CktCellAry& scAry)
{
   assignInitState(); // assign initial state
   resetPI();         // assign pi x
   combSim(scAry);
}
   
void 
CktModule::oneCycleSim(CktCellAry& scAry, ifstream& itf)
{
   assignPiVector(itf);
   seqUpdate();    // copy _nsValue to _simValue
   combSim(scAry);
   recordNS();
}
   
void 
CktModule::assignPiVector(ifstream& itf)
{
   bool quit = false; 
   string enumPI;
   string testData;
   CktCellAry::iterator it;
   size_t tPos;
   int patternFlag = 0;
   Bv4 bValue;

   if (itf.eof()) return;

   while (quit == false) {
      if (!getline(itf, enumPI)) break;
      istringstream iss(enumPI, istringstream::in);   
 
      while (iss >> testData) {
         tPos = testData.find('#', 0);
         if (tPos != 0 && tPos != string::npos)
            return;
   
         if (tPos == 0) {         
            if (patternFlag == 0) {               
               ++patternFlag;
               continue;
            }
            else if (patternFlag == 1) {
               quit = true;
               testData = testData.substr(tPos + 1);
               itf.seekg(-1 * enumPI.length() - 1, ios::cur);
               break;
            }
         }  
         else if (tPos == string::npos) {
            for (it = _ioList[CKT_IN].begin(); it != _ioList[CKT_IN].end(); ++it) {
               if (testData == (*it)->getOutPinName()) {                  
                  iss >> testData;
                  bValue = testData;
                  (*it)->setValue(bValue);
               }
            } 
         }        
      }
   }
}

void 
CktModule::assignInitState()
{
   for (CktCellAry::iterator it = _seqCells.begin(); it != _seqCells.end(); ++it) {
      if ((*it)->getCellType() == CKT_DFF_CELL)
         (static_cast<CktDffCell*>(*it))->setInitState();
      else
         (static_cast<CktDlatCell*>(*it))->setInitState();
   }
}
   
void 
CktModule::resetPI()
{
   for (CktCellAry::iterator pos = _ioList[CKT_IN].begin(); pos != _ioList[CKT_IN].end(); ++pos)
      (static_cast<CktPiCell*>(*pos))->resetPI();
}
   
void 
CktModule::combSim(CktCellAry& simCellAry)
{
   Bv4 curValue;
#ifdef EVENT_DRIVEN_SIM
   for (CktCellAry::iterator pos = simCellAry.begin(); pos != simCellAry.end(); ++pos) {
      (*pos)->updateSimToggle();  // Update Simulation Toggle
      if ((*pos)->isSimToggle()) {
         curValue = (*pos)->getSimValue(); (*pos)->simulate();
         (*pos)->setSimToggle(curValue != (*pos)->getSimValue());
      }
   }
#else
   for (CktCellAry::iterator pos = simCellAry.begin(); pos != simCellAry.end(); ++pos) {
      curValue = (*pos)->getSimValue(); 
      (*pos)->simulate();
   }
#endif
}
   
void 
CktModule::recordNS()
{
   for (CktCellAry::iterator it = _seqCells.begin(); it != _seqCells.end(); ++it) {
      if ((*it)->getCellType() == CKT_DFF_CELL)
         (static_cast<CktDffCell*>(*it))->recordNS();
      else
         (static_cast<CktDlatCell*>(*it))->recordNS();
   }
}
   
void 
CktModule::seqUpdate()
{
   for (CktCellAry::iterator it = _seqCells.begin(); it != _seqCells.end(); ++it) {
      if ((*it)->getCellType() == CKT_DFF_CELL)
         (static_cast<CktDffCell*>(*it))->seqUpdate();
      else
         (static_cast<CktDlatCell*>(*it))->seqUpdate();
   }
}

void
CktModule::outputInfo(CktCellAry& simCells) const
{
   CktCellAry::const_iterator it;
   /*cout << "Current pattern > ";
   for (it = _ioList[CKT_IN].begin(); it != _ioList[CKT_IN].end(); ++it) 
      cout << (*it)->getOutPinName() << ":" << (*it)->getSimValue(0) << "  ";
   cout << endl;

   cout << "FF output       > ";
   for (it = _seqCells.begin(); it != _seqCells.end(); ++it)
      cout << (*it)->getOutPinName() << ":" << (*it)->getSimValue(0) << "  ";
   cout << endl;
   
   cout << "Simulated PO    > ";
   for (it = _ioList[CKT_OUT].begin(); it != _ioList[CKT_OUT].end(); ++it) 
      cout << (*it)->getOutPinName() << ":" << (*it)->getSimValue(0) << "  ";
   cout << endl;*/

   cktOutFile << "Current pattern > ";
   for (it = _ioList[CKT_IN].begin(); it != _ioList[CKT_IN].end(); ++it) 
      cktOutFile << (*it)->getOutPinName() << ":" << (*it)->getSimValue(0).str().str() << "  ";
   endl(cktOutFile);
/*
   cktOutFile << "FF output       >  (ID) D_Name (simValue, NsValue) ";
   endl(cktOutFile);
   for (it = _seqCells.begin(); it != _seqCells.end(); ++it) {
      if ((*it)->getCellType() == CKT_DFF_CELL) {
         cktOutFile << "(" << (*it)->getCellID() << ") "
                    << (*it)->getOutPinName() << "(" << (*it)->getSimValue(0).str().str() << ",  "
                    << (static_cast<CktDffCell*>(*it))->getNsValue().str().str() << ") ";
      }
      else {
         cktOutFile << "(" << (*it)->getCellID() << ") "
                    << (*it)->getOutPinName() << "(" << (*it)->getSimValue(0).str().str() << ",  "
                    << (static_cast<CktDlatCell*>(*it))->getNsValue().str().str() << ") ";
      }
      endl(cktOutFile);
   }
   endl(cktOutFile);
  */ 
   cktOutFile << "Simulated PO    > ";
   endl(cktOutFile);
   for (it = _ioList[CKT_OUT].begin(); it != _ioList[CKT_OUT].end(); ++it) {
      cktOutFile << (*it)->getOutPinName() << ":" << (*it)->getSimValue(0).str().str() << "  ";
      endl(cktOutFile);
   }
   endl(cktOutFile);
   cktOutFile << "===================================================================";
   endl(cktOutFile);
   
   //cout << "============================================================================" 
   //     << "============================================================================" << endl;
   //for (CktCellAry::iterator pos = simCells.begin(); pos != simCells.end(); ++pos) 
   //   (*pos)->printCell();
      
}

void
CktModule::preLoad(unsigned cellID, string valueStr)
{
   for (CktCellAry::iterator it = _seqCells.begin(); it != _seqCells.end(); ++it) {
      if ((*it)->getCellID() == cellID) {
         if ((*it)->getCellType() == CKT_DFF_CELL)
            (static_cast<CktDffCell*>(*it))->preLoad(valueStr);
         else
            (static_cast<CktDlatCell*>(*it))->preLoad(valueStr);
      }
   }
}
      
void
CktModule::randomPi()
{
   CktCellAry::iterator it;
   const SynBus* bus;
   unsigned bits, range, value;
   string valueStr;
   Bv4 bValue;

   for (it = _ioList[CKT_IN].begin(); it != _ioList[CKT_IN].end(); ++it) {
      bus = VLDesign.getBus((*it)->getOutPin()->getBusId());
      bits = bus->getWidth();
      if (bits < 32) {
         range = (unsigned)pow(2.0, bits);
         value = rnGen(range);
         valueStr = toString(bits) + "'d" + toString(value);
         bValue = valueStr;
         (*it)->setValue(bValue);
      }
      else {
         unsigned partBits;
         Bv4 tmpBv;
         bValue = "0'b0";
         do {
            if (bits > 31) partBits = 31;
            else           partBits = bits;
            
            bits -= partBits;
            range = (unsigned)pow(2.0, partBits);
            value = rnGen(range);
            valueStr = toString(bits) + "'d" + toString(value);
            tmpBv = valueStr;                     
            bValue.append(tmpBv);
         } while (bits > 0);
      }
   }
}

void
CktModule::recordSt(unsigned cellID)
{

}

bool
CktModule::getDumpSignals(list<CktOutPin*>& signals, const string& fileName) const {
   ifstream input;
   input.open(fileName.c_str());
   if (!input.is_open()) {
      Msg(MSG_ERR) << "Input Signal File \"" << fileName << "\" does not exist !!!" << endl;
      return false;
   }
   string sigName = "";
   CktOutPin* OutPin;
   while (input >> sigName) {
      OutPin = getOutPin(sigName);
      if (!OutPin) {
         Msg(MSG_ERR) << "Unresolved Signal Name \"" << sigName << "\" !!!" << endl;
         return false;
      }
      signals.push_back(OutPin);
   }
   input.close();
   return true;
}

#endif

