/****************************************************************************
  FileName     [ cktModule.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit module data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_MODULE_H
#define CKT_MODULE_H

#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "cktEnum.h"
#include "cktCell.h"
#include "cktPin.h"
#include "cktLiberty.h"
#include "synVar.h"
#include "vlpModule.h"

using namespace std;

class CktModule
{
   public:
      // Constructors and Destructor
      CktModule();
      ~CktModule();

      // Set Module Attribute Functions : Module
      const VlgModule* getVlpModule() const;
      void setLibrary();
      void setBlackBox();
      void setModCell();
      void setModuleName(string);
      void setModuleName(const char*);
      // Set Module Attribute Functions : Cell
      unsigned getCellListSize() const;
      unsigned getDffSize() const;
      void getTypeCellList(const CktCellType, list<CktCell*>&) const;  // chengyin add
      // Set Module Attribute Functions : I/O
      int getIoType(unsigned) const;
      int getIoType(string) const;
      string getIoName(unsigned) const;
      unsigned getIoSize() const;
      unsigned getIoSize(CktIoType) const;
      unsigned getIoType(CktIoType, string) const; // chengyin add
      // Set Module Attribute Functions : Parameter Overload
      const ParamPairAry* getParamPair() const;
      // Set Module Attribute Functions : Net
      void incRefCount();
      void decRefCount();
   
      // Get Module Attribute Functions
      string getModuleName() const;
      CktCell* getCell(unsigned) const;
      CktCell* getIoCell(string) const;
      CktCell* getIoCell(CktIoType, unsigned) const;
      CktCell* getSeqCell(unsigned) const;
      CktOutPin* getOutPin(const string) const;
      bool isLib() const;
      bool isBlackBox() const;
      bool isCellInMod(const CktCell*) const;
      int getRefCount() const;
      int getNetNumber() const;

      // Circuit Construction Functions : General
      void insertIoCell(CktCell*, const string&, CktIoType);
      void insertIoCell(CktCell*, CktIoType);
      void insertCell(CktCell* = NULL);
      // Circuit Construction Functions : Synthesis
      void setVlpModule(const VlgModule*);
      void insertVar(CktOutPin*);
      void insertVarType(string); 
      void insertIoNameList(string); 
      void insertIoTypeList(CktIoType); 
      void connParam(CktOutPin*);
      void setParamPair(ParamPairAry);
      void setNetNumber(int);
      void nameNoNamePin(int&) const;
      CktModule* duplicate() const;
      // Circuit Construction Functions : Flatten
      bool flatten();
      void collectCell();
      void moduleDuplicate(string, CktModule*&, bool, CktCellAry&) const; // use for complete flatten
      void updateFltCkt();
      // Circuit Construction Functions : Auxiliary
      bool createPort(const CktIoType&, const string&, const unsigned&, const unsigned&);
      bool createInput(const string&, const unsigned&, const unsigned&);
      bool createOutput(const string&, const unsigned&, const unsigned&);
      bool createInOut(const string&, const unsigned&, const unsigned&);
      CktOutPin* createNet(const string&, const unsigned&, const unsigned&);
      bool checkNet(const string&, const unsigned&, const unsigned&) const;
      bool checkNet(const CktOutPin* const, const unsigned&, const unsigned&) const;
      bool createFFCell(const string&, const string&, const bool&, const string&, const bool& = false, const string& = "", const string& = "");
      bool createLatchCell(const string&, const string&, const string& = "");
      bool createOneInputCell(const bool&, const string&, const string&);
      bool createOneInputCell(const CktCellType&, const bool&, const string&, const string&);
      bool createOneInputCell(const string&, const string&);
      bool createAssocCell(const CktCellType&, const string&, const vector<string>&);
      bool createTwoInputCell(const CktCellType&, const string&, const string&, const string&);
      bool createMuxCell(const string&, const string&, const string&, const string&);
      bool createSplitCell(const string&, const string&, const unsigned&, const unsigned&);
      bool createMergeCell(const string&, const vector<string>&);
      bool createModuleCell(const string&, const string&, const vector<string>&);
      bool createModuleCell(CktModule* const, const string&, const vector<string>&);

      /////////////////// THOSE ARE PRIMITIVE FUNCTIONS ////////////////////
      CktModule* defModule(bool, string&, string&);
      void cellReg(CktCell*, CktOutPin*, const string&, const int&, bool, const CktCellType&);
      bool creIO(int*, string*, const int&, const CktCellType&);
      bool creIO(const int&, vector<string>&, const CktCellType&);
      CktCell* creCell(const CktCellType&, const int&, int&, const string&);
      void creModuleCell(CktModule*, const string&);
      bool checkSameName(string&);
      int checkConn(string&);

      /////////////////////////////////////////////////////////////////////
      int getOutPinWidth(const string&) const;
      void creConstCell(const int&, const string&, const string&);
      void creDffCell(const int&, const string&, const string&);
      void creMuxCell(const int&, const string&);
      void creSplitCell(const int&, const string&, const int&, const int&);
      void constrainInput(CktCell*);
      int creConn(string&, string&, string&, string&);
      void crePreOut(const string&, vector<string>&);
      void SOPGate(vector<string>&, string&, set<string>&, bool);
      CktModule* getParentModule();

      // Print Info Functions
      void printPI(CktCell*, int, int, unsigned) const;
      void printPO(CktCell*, int, int, unsigned) const;
      bool printPIGate(string, int) const;
      bool printPOGate(string, int) const;
      bool printMod(string& , bool) const;
      bool printFlatMod() const;
      void print() const;
      void print(string, int) const;
      void printPIO() const;
      void printCellList() const;
      void printStatistics(bool) const;
      void printSummary(bool) const;
      void printVerbose(bool) const;
      void debug() const;

      // Write Verilog Functions
      void writeIO() const;
      void writeWire() const;
      void writeCells() const;
      void modWriteOut() const;
      // Write BLIF Functions
      void writeBLIF() const;
      void writeBLIF_Input() const;
      void writeBLIF_Output() const;
      // Write Graphviz Functions
      void writeGateInLevel(CktOutPin* root, const string, const unsigned, bool);
      void writeGateInTimeFlow(CktOutPin* root, const string, const unsigned, bool);
      void writeGateInBoundedDepth(CktOutPin* root, const string, const unsigned, bool);
      bool writeCDFG(const string, const string, set<unsigned>&);

      // This is for temporary usage (verifyExtend)
      void bfsCellList(list<CktOutPin*>&);

      // Constraint Generation Functions
      void setOutPinMap(SynSharNodeMap&);
      void setSynVars(SynVarList&);
      set<string> writeConstraint(string&, ofstream&) const;
      void writeFinFunc(const string, ofstream&, set<string>&) const;

      // Constraint Functions
      void travelDP(CktCell*, CktDpMap&, PinLitMap&, int&, unsigned);
      void printDP(CktDpMap&, unsigned);

      // Circuit Simulation Functions
      bool simMgr(const string&);
      bool probSim(const string&);
      void initSim(CktCellAry&);
      void oneCycleSim(CktCellAry&, ifstream&);
      void genSimCellAry(CktCellAry&) const;
      void combSim(CktCellAry&);
      void seqUpdate();
      void recordNS();
      bool getDumpSignals(list<CktOutPin*>&, const string&) const;
      
   private:
      // Operating Helper Functions
      void duplicateIO(CktModule*&, CellMap&, OutPinMap&, InPinMap&) const;
      void connModuleIOPin(CellMap&, InPinMap&, OutPinMap&, bool) const;
      unsigned expandWidth(CktCell* foutCell, CktInPin* constIn) const;

      // Print Info Helper Functions
      void printInpin(string&, CktInPin*, unsigned, unsigned) const;
      void printOutpin(string&, CktOutPin*, unsigned, unsigned) const;
      void printCell(string&, CktCell*, unsigned, unsigned) const;

      // Circuit Construction Functions : Flatten
      void forceEliminateBuf();
      void reviseSplitCell();
      void adjustConst();

      // Simulation Helper Functions
      void assignPiVector(ifstream&);
      void assignInitState();
      void resetPI();
      void outputInfo(CktCellAry&) const;
      void preLoad(unsigned, string);
      void randomPi();
      void recordSt(unsigned);

      // Data Structure : Module Info
      const VlgModule*    _vlpModule;
      string              _moduleName;
      bool                _isLib;
      bool                _isBlackBox;
      // Data Structure : Cell Arrays
      CktCellAry          _cellList;
      CktCellAry          _ioList[CKT_TOTAL];  // PI/PO/PIO
      CktCellAry          _modCells;           // moduleCell Array
      CktCellAry          _seqCells;           // squential cell Array  (latch and ff)
      // Data Structure : Circuit Info
      vector<string>      _varTypeList;
      vector<string>      _ioNameList;
      vector<CktIoType>   _ioTypeList;
      CktOutPinAry        _varList;  // variable excluding PI, PO, PIO
      ParamPairAry        _paramList;
      int                 _netNumber;
      int                 _refCount;
      // Data Structure : Constraint Generation
      SynSharNodeMap      _outPinMap;
      SynVarList          _synVars;
      // Data Structure : Timing
      CktDelayInfo*       _cktDelayInfo;
};

#endif // CKT_MODULE_H
