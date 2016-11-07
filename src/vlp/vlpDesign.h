/****************************************************************************
  FileName     [ vlpDesign.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Design class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_DESIGN_H
#define VLP_DESIGN_H

#include "vlpModule.h"

using namespace std;

class VlpDesign
{
   public :
      VlpDesign();
      ~VlpDesign();

      // Design State Supports
      inline bool isHierDesign() const { return _vlgModuleAry.size(); }
      inline bool isVerilogInput() const { return _topModule && isHierDesign(); }
      inline bool isHierModule() const { return _cktModuleAry.size(); }
      inline bool isFlatModule() const { return _cktFlattenDesign; }
      inline bool isDesignRead() const { return isVerilogInput() || isHierModule() || isFlatModule(); }
      inline bool isFSMExtracted() const { return _FSMExtracted; }
      inline void setFSMExtracted() { _FSMExtracted = true; }
      const string getCurModuleName() const;
      
      // Interactive Design Construction Interface Functions
      CktModule* CreateDesign(const string&);
      CktModule* CreateModule(const string&);
      CktModule* ChangeModule();
      // Helper Functions (Not Recommend to Call Directly)
      void CreateInstance(CktModule*, const string&);
      void UpdateInstance(const string&, const string&);

      // Design Parse Functions
      bool ParseVerilog(const char*, bool);
      bool ParseFileList(const char*);
      bool ParseVerilogLib(const char*);
      bool ParseBLIF(const char*);
      bool ParseBTOR(const char*);

      // Verilog Parse
      void setModule(VlgModule* const&);
      void setModule(LibModule* const&);
      void setCurHTNode(VlpHierTreeNode* const);
      void setTree(VlpHierTreeNode* const&);
      const VlpHierTree* getHier() const; 
      VlpHierTreeNode* getCurHTNode() const;
      VlpHierTreeNode* getHTNode(const string&) const;

      // CDFG Synthesis
      void setModule(CktModule* const&);
      CktModule* getCktModule(int) const;
      CktModule* getCktModule(string) const;
      CktModule* getCktModule(char*) const;

      // Hier Design Flatten
      CktModule* getFltModule() const; // chang add
      string getFltModName() const;
      void resolveHier();
      
      // Retrieval Functions
      unsigned getModuleNum() const;   
      string getTopModName() const;
      VlgModule* getTopModule() const;
      BaseModule* getModule(const string&) const; //for map
      const VlgModule* const getVlgModule(const int& i) const; //for array
      const LibModule* const getLibModule(const int& i) const; //for array
   
      // User Interface Function
      void printCDFG(const string&, int = 100) const;
      bool repCDFG(string&, int, int) const;
      bool repCDFG(const string&, const string&) const;
      bool repModule(string&, int, int) const;
      bool repDeStat() const;
      bool repDeSum() const;
      bool repDeVer() const;
      bool repCktStat() const;
      bool repCktSum() const;
      bool repCktVer() const;
      bool repFIGate(string, int) const;
      bool repFOGate(string, int) const;
      bool writeCDFG(const string, const string, const string) const;
      bool writeHierarchy(const string, const string) const;
   
      // FSM
      bool extract();
      bool parseFSM(string);
      void reset();
      void repCU(const char*) const;
      void writeCU(const char*, bool) const;

      // SYNTHESIS
      bool synthesis();
      bool flatten();
      bool completeFlatten();

      bool synWriteDesign(string) const;
      bool fltWriteDesign(string) const;
      bool writeBLIF(string) const;
      unsigned genBusId(unsigned, int, int, bool);
      unsigned genBusId(const SynBus*);
      const SynBus* getBus(const unsigned&) const;

      void resetCkt();

      bool constraintGen();
      bool writeConstraint(const char* sigFile);

   private :
      // BLIF PARSER
      void parseBLIF(FILE*);
      bool lexLineBLIF(FILE*);
      void creGateBLIF(vector<string>&, string&, set<string>&, bool);
      // BTOR PARSER
      void parseBTOR(FILE*);
      bool lexLineBTOR(FILE*);
      string creGateBTOR(const int&);
      void creGateBTOR(const int&, const string&, const string&);
      void creGateBTOR(const CktCellType&, const int&, const string&, const string& = "", const string& = "");

      // Utility Function
      void hierCSTer();
      void NamePOL_Design();

      // SYNTHESIS
      void initCktModule(string&);

      // Data Members
      FileParser FP;
      // if use ModuleAry, it don't need to clear() in "~VlpDesign()"
      VlgModuleAry _vlgModuleAry;
      LibModuleAry _libModuleAry;
      CktModuleAry _cktModuleAry;

      ModuleMap    _moduleMap;
      VlgModule*   _topModule;
      VlpHierTree  _designHier;
      BusAry       _busArr;
      VlpFSM*      _iFSM;
      bool         _FSMExtracted;

      // total Ckt flatten Cell
      CktModule*   _cktFlattenDesign; //no hierarchy, ie. no moduleCell
};

#endif


