/****************************************************************************
  FileName     [ cktCmd.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Commands relates ckt. ]
  Author       [ Cheng-Yin Wu, Steggie Chen, Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_CMD_SOURCE
#define CKT_CMD_SOURCE

#include "cktCmd.h"
#include "vlpDesign.h"

#define DEFAULT_WIDTH 1 

bool
initCktCmd() {
   return 
         (
          cmdMgr->regCmd("CREate DEsign",       3, 2, new CreateDesignCmd)    &&
          cmdMgr->regCmd("DEFine NET",          3, 3, new DefineNetCmd)       &&
          cmdMgr->regCmd("DEFine CELL",         3, 4, new DefineCellCmd)      &&
          cmdMgr->regCmd("DEFine INST",         3, 4, new DefineInstCmd)      &&
          cmdMgr->regCmd("DEFine MODule",       3, 3, new DefineModuleCmd)    &&
          cmdMgr->regCmd("CHAnge MODule",       3, 3, new ChangeModuleCmd)    &&
          cmdMgr->regCmd("WRIte CFG",           3, 3, new WriteCfgCmd)        &&
          cmdMgr->regCmd("WRIte DFG",           3, 3, new WriteDfgCmd)        &&
          cmdMgr->regCmd("WHOAMI",              6,    new WhoAmICmd)          &&
          cmdMgr->regCmd("SIMulate",            3,    new SimulateCmd) 
         );
}

extern bool                busDirectionDefault;
extern CmdParser*          cmdMgr;
extern VlpDesign&          VLDesign;
extern CktModule*          curCktModule;

// ============================================================================
// CREate DEsign <(string topModuleName)>
// ============================================================================
CmdExecStatus
CreateDesignCmd::exec(const string& option) {
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   size_t n = options.size();
   if (n == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "<(string topModuleName)>");
   if (n > 1) return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);

   CktModule* module = VLDesign.CreateDesign(options[0]);
   if (!module) return CmdExec::errorOption(CMD_OPT_ILLEGAL, "Create New Design Failed !!!");
   else curCktModule = module;

   Msg(MSG_IFO) << "New Design \"" << curCktModule->getModuleName() << " has been created !!!" << endl;
   return CMD_EXEC_DONE;
}

void
CreateDesignCmd::usage() const {
   Msg(MSG_IFO) << "Usage: CREate DEsign <topModuleName>" << endl;
}

void
CreateDesignCmd::help() const {
   cout << setw(20) << left << "CREate DEsign:" << "Create design from command line interface." << endl;
}

bool
CreateDesignCmd::valid() const {
   if (VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design exists currently !!!" << endl;
   else if (VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// DEFine NET [-PI | -PO | -PIO] [(unsigned width)] <(string NetName)>
// ============================================================================
CmdExecStatus
DefineNetCmd::exec(const string& option) {
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);

   unsigned width = 0;
   string netName = "";
   bool pi, po, pio;
   int temp;
   pi = po = pio = false;

   for (unsigned i = 0; i < options.size(); ++i) {
      const string& token = options[i];
      if (width == 0 && myStr2Int(token, temp)) {
         if (temp <= 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, "(unsigned width)");
         else width = (unsigned)temp;
      }
      else if (myStrNCmp("-PIO", token, 4) == 0) {
         if (pi || po || pio) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         pio = true;
      }
      else if (myStrNCmp("-PO", token, 3) == 0) {
         if (pi || po || pio) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         po = true;
      }
      else if (myStrNCmp("-PI", token, 3) == 0) {
         if (pi || po || pio) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         pi = true;
      }
      else if (netName == "") netName = token;
      else return CmdExec::errorOption(CMD_OPT_EXTRA, token);
   }

   if (netName == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string NetName)>");
   if (width == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned width)");

   // Create Net
   if (pi) {
      if (curCktModule->createPort(CKT_IN, netName, width - 1, 0))
         Msg(MSG_DBG) << "Primary Input \"" << netName << "[" << width - 1 << " : " << 0 << "]\" has been created !!!" << endl;
   }
   else if (po) {
      if (curCktModule->createPort(CKT_OUT, netName, width - 1, 0))
         Msg(MSG_DBG) << "Primary Output \"" << netName << "[" << width - 1 << " : " << 0 << "]\" has been created !!!" << endl;
   }
   else if (pio) {
      if (curCktModule->createPort(CKT_IO, netName, width - 1, 0))
         Msg(MSG_DBG) << "Primary Inout \"" << netName << "[" << width - 1 << " : " << 0 << "]\" has been created !!!" << endl;
   }
   else {
      if (curCktModule->createNet(netName, width - 1, 0))
         Msg(MSG_DBG) << "Net \"" << netName << "[" << width - 1 << " : " << 0 << "]\" Created !!!" << endl;
   }

   return CMD_EXEC_DONE;
}

void
DefineNetCmd::usage() const {
   Msg(MSG_IFO) << "Usage: DEFine NET [-PI | -PO | -PIO] [(unsigned width)] <(string NetName)>" << endl;
}

void
DefineNetCmd::help() const {
   cout << setw(20) << left << "DEFine NET:" << "Create I/O ports and nets on current design." << endl;
}

bool
DefineNetCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized !!!" << endl;
   else if (VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// DEFine CELL <CellType> <Output_NetName> <Input_NetName1> ...
// ============================================================================
CmdExecStatus
DefineCellCmd::exec(const string& option) {
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);

   // Check CellType
   CktCellType cellType; bool invert = false;
   if (options.size() == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "<CellType>");
   else if (myStrNCmp("DFF",  options[0], 3) == 0) { cellType = CKT_DFF_CELL;       invert = false; }
   else if (myStrNCmp("DLAT", options[0], 4) == 0) { cellType = CKT_DLAT_CELL;      invert = false; }
   else if (myStrNCmp("BUF",  options[0], 3) == 0) { cellType = CKT_BUF_CELL;       invert = false; }
   else if (myStrNCmp("INV",  options[0], 3) == 0) { cellType = CKT_BUF_CELL;       invert = true;  }
   else if (myStrNCmp("AND",  options[0], 3) == 0) { cellType = CKT_AND_CELL;       invert = false; }
   else if (myStrNCmp("OR",   options[0], 2) == 0) { cellType = CKT_OR_CELL;        invert = false; }
   else if (myStrNCmp("NAND", options[0], 4) == 0) { cellType = CKT_AND_CELL;       invert = true;  }
   else if (myStrNCmp("NOR",  options[0], 3) == 0) { cellType = CKT_OR_CELL;        invert = true;  }
   else if (myStrNCmp("XOR",  options[0], 3) == 0) { cellType = CKT_XOR_CELL;       invert = false; }
   else if (myStrNCmp("XNOR", options[0], 4) == 0) { cellType = CKT_XOR_CELL;       invert = true;  }
   else if (myStrNCmp("MUX",  options[0], 3) == 0) { cellType = CKT_MUX_CELL;       invert = false; }
   else if (myStrNCmp("ADD",  options[0], 3) == 0) { cellType = CKT_ADD_CELL;       invert = false; }
   else if (myStrNCmp("SUB",  options[0], 3) == 0) { cellType = CKT_SUB_CELL;       invert = false; }
   else if (myStrNCmp("MULT", options[0], 4) == 0) { cellType = CKT_MULT_CELL;      invert = false; }
   else if (myStrNCmp("DIV",  options[0], 3) == 0) { cellType = CKT_DIV_CELL;       invert = false; }
   else if (myStrNCmp("MOD",  options[0], 3) == 0) { cellType = CKT_MODULO_CELL;    invert = false; }
   else if (myStrNCmp("SHL",  options[0], 3) == 0) { cellType = CKT_SHL_CELL;       invert = false; }
   else if (myStrNCmp("SHR",  options[0], 3) == 0) { cellType = CKT_SHR_CELL;       invert = false; }
   else if (myStrNCmp("CONST",options[0], 5) == 0) { cellType = CKT_CONST_CELL;     invert = false; }
   else if (myStrNCmp("SLICE",options[0], 5) == 0) { cellType = CKT_SPLIT_CELL;     invert = false; }
   else if (myStrNCmp("MERGE",options[0], 5) == 0) { cellType = CKT_MERGE_CELL;     invert = false; }
   else if (myStrNCmp("EQ",   options[0], 2) == 0) { cellType = CKT_EQUALITY_CELL;  invert = false; }
   else if (myStrNCmp("GEQ",  options[0], 3) == 0) { cellType = CKT_GEQ_CELL;       invert = false; }
   else if (myStrNCmp("GT",   options[0], 2) == 0) { cellType = CKT_GREATER_CELL;   invert = false; }
   else if (myStrNCmp("LEQ",  options[0], 3) == 0) { cellType = CKT_LEQ_CELL;       invert = false; }
   else if (myStrNCmp("LESS", options[0], 4) == 0) { cellType = CKT_LESS_CELL;      invert = false; }
   else return CmdExec::errorOption(CMD_OPT_ILLEGAL, "<CellType>");

   if (options.size() < 2) return CmdExec::errorOption(CMD_OPT_MISSING, "<Output_NetName>");
   if (options.size() < 3) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName1>");

   vector<string> inName; inName.clear();
   for (unsigned i = 2; i < options.size(); ++i) inName.push_back(options[i]);
   string outName = options[1];
   int msb, lsb;
   bool result = false;
   switch (cellType) {
      case CKT_DFF_CELL : 
         if (inName.size() < 2) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName2> as clock for DFF");
         if (inName.size() < 3) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName3> as reset for DFF");
         if (inName.size() > 4) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[4]);
         result = curCktModule->createFFCell(outName, inName[0], true, inName[1], false, inName[2], ((inName.size() > 3) ? inName[3] : ""));
         break;
      case CKT_DLAT_CELL : 
         if (inName.size() > 2) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[2]);
         result = curCktModule->createLatchCell(outName, inName[0], ((inName.size() > 1) ? inName[1] : ""));
         break;
      case CKT_CONST_CELL : 
         if (inName.size() > 1) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[1]);
         result = curCktModule->createOneInputCell(outName, inName[0]);
         break;
      case CKT_BUF_CELL : 
         if (inName.size() > 1) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[1]);
         result = curCktModule->createOneInputCell(invert, outName, inName[0]);
         break;
      case CKT_AND_CELL : 
      case CKT_OR_CELL : 
      case CKT_XOR_CELL : 
         if (inName.size() > 2) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[2]);
         if (inName.size() == 1) result = curCktModule->createOneInputCell(cellType, invert, outName, inName[0]);
         else result = curCktModule->createTwoInputCell(cellType, outName, inName[0], inName[1]);
         break;
      case CKT_ADD_CELL : 
      case CKT_SUB_CELL : 
      case CKT_MULT_CELL : 
      case CKT_DIV_CELL : 
      case CKT_MODULO_CELL : 
      case CKT_SHL_CELL : 
      case CKT_SHR_CELL : 
      case CKT_EQUALITY_CELL : 
      case CKT_GEQ_CELL : 
      case CKT_GREATER_CELL : 
      case CKT_LEQ_CELL : 
      case CKT_LESS_CELL : 
         if (inName.size() > 2) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[2]);
         if (inName.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName2> for second input of " + options[0]);
         else result = curCktModule->createTwoInputCell(cellType, outName, inName[0], inName[1]);
         break;
      case CKT_MUX_CELL : 
         if (inName.size() > 3) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[3]);
         if (inName.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName2> for true input of MUX");
         if (inName.size() == 2) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName3> for select input of MUX");
         result = curCktModule->createMuxCell(outName, inName[0], inName[1], inName[2]);
         break;
      case CKT_SPLIT_CELL : 
         if (inName.size() > 3) return CmdExec::errorOption(CMD_OPT_EXTRA, inName[3]);
         if (inName.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName2> for msb specification of SLICE");
         if (inName.size() == 2) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName3> for lsb specification of SLICE");
         if (!myStr2Int(inName[1], msb)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, "<Input_NetName2> as msb of SLICE is NOT an integer");
         if (!myStr2Int(inName[2], lsb)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, "<Input_NetName3> as lsb of SLICE is NOT an integer");
         if (msb < 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, "msb of SLICE cannot be negative");
         if (lsb < 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, "lsb of SLICE cannot be negative");
         if (msb < lsb) return CmdExec::errorOption(CMD_OPT_ILLEGAL, "msb cannot be smaller than lsb for SLICE");
         result = curCktModule->createSplitCell(outName, inName[0], msb, lsb);
         break;
      case CKT_MERGE_CELL : 
         if (inName.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING, "<Input_NetName2> for second input of MERGE");
         result = curCktModule->createMergeCell(outName, inName);
         break;
      default : 
         Msg(MSG_WAR) << "Cell Type \"" << options[0] << "\" has not implemented ?!?!?!" << endl;
         break;
   }
   if (result) Msg(MSG_DBG) << options[0] << " CELL \"" << options[1] << "\" has been created !!!" << endl;

   return CMD_EXEC_DONE;
}

void
DefineCellCmd::usage() const {
   Msg(MSG_IFO) << "Usage: DEFine CELL <CellType> <Output_NetName> <Input_NetName1> ..." << endl;
   Msg(MSG_IFO) << "       Supports : 1  Input Cells : BUF, INV, AND, OR, NAND, NOR, XOR, XNOR, CONST" << endl;
   Msg(MSG_IFO) << "                  2  Input Cells : AND, OR, NAND, NOR, XOR, XNOR, ADD, SUB, MULT, DIV, MOD, SHL, SHR, EQ, GEQ, GT, LEQ, LESS" << endl;
   Msg(MSG_IFO) << "                  3  Input Cells : MUX, SLICE" << endl;
   Msg(MSG_IFO) << "                  2+ Input Cells : MERGE" << endl;
   Msg(MSG_IFO) << "                  Sequential     : DFF <Output_NetName> <Input_Name> <Input_ClockName> <Input_ResetName> [Input_DefaultName]" << endl;
   Msg(MSG_IFO) << "                                   DLAT <Output_NetName> <Input_Name> [Input_LoadName]" << endl;
}

void
DefineCellCmd::help() const {
   cout << setw(20) << left << "DEFine CEll:" << "Create cell(s) with specified cell type on current design." << endl;
}

bool
DefineCellCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized !!!" << endl;
   else if (VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// DEFine INST <(string moduleName)> <(string instName)> <port_Name> ...
// ============================================================================
CmdExecStatus
DefineInstCmd::exec(const string& option) {
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);

   if (options.size() == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "<(string moduleName)>");
   if (options.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING, "<(string instName)>");

   vector<string> ports; ports.clear();
   for (unsigned i = 2; i < options.size(); ++i) ports.push_back(options[i]);

   if (curCktModule->createModuleCell(options[0], options[1], ports))
      Msg(MSG_DBG) << "Module Inst \"" << options[1] << "\" of Module \"" << options[0] << "\" has been created !!!" << endl;

   return CMD_EXEC_DONE;
}

void
DefineInstCmd::usage() const {
   Msg(MSG_IFO) << "Usage: DEFine INST <(string moduleName)> <(string instName)> <port_Name> ..." << endl;
}

void
DefineInstCmd::help() const {
   cout << setw(20) << left << "DEFine INST:" << "Create module instance." << endl;
}

bool
DefineInstCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized !!!" << endl;
   else if (VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// DEFine MODule <(string modulename)>
// ============================================================================
CmdExecStatus
DefineModuleCmd::exec(const string& option) {
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   size_t n = options.size();
   if (n == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "<(string modulename)>");
   if (n > 1) return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);

   CktModule* module = VLDesign.CreateModule(options[0]);
   if (!module) Msg(MSG_ERR) << "Create New Module Failed !!!";
   else {
      curCktModule = module;
      Msg(MSG_IFO) << "New Module \"" << curCktModule->getModuleName() << " has been created !!!" << endl;
   }
   return CMD_EXEC_DONE;
}

void
DefineModuleCmd::usage() const {
   Msg(MSG_IFO) << "Usage: DEFine MODule <(string modulename)>" << endl;
}

void
DefineModuleCmd::help() const {
   cout << setw(20) << left << "DEFine MODule:" << "Create a new sub-module on current design." << endl;
}

bool
DefineModuleCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized !!!" << endl;
   else if (VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// CHAnge MODule
// ============================================================================
CmdExecStatus
ChangeModuleCmd::exec(const string& option) {
// check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   if (options.size()) return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);

   CktModule* module = VLDesign.ChangeModule();
   if (!module) Msg(MSG_ERR) << "NO Upper Module in the Hierarchy" << endl;
   else {
      curCktModule = module;
      Msg(MSG_IFO) << "Back to Module \"" << curCktModule->getModuleName() << " !!!" << endl;
   }

   return CMD_EXEC_DONE;
}

void
ChangeModuleCmd::usage() const {
   Msg(MSG_IFO) << "Usage: CHAnge MODule" << endl;
}

void
ChangeModuleCmd::help() const {
   cout << setw(20) << left << "CHAnge MODule:" << "Change current module to its parant module in current design hierarchy." << endl;
}

bool
ChangeModuleCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized !!!" << endl;
   else if (VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// WRIte CFG <(string pinName)> <(string fileName)> [| -DOT | -PNG | -PS] 
//           [| -All | -Bit <(unsigned bit1)> <(unsigned bit2)> ...]
// ============================================================================
CmdExecStatus
WriteCfgCmd::exec(const string& option) {
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   
   string pinName = "";
   string fileName = "";
   bool dot, png, ps, all, bit;
   dot = png = ps = all = bit = false;
   bool insidebits = false;
   set<unsigned> bits;
   int temp;
   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (myStrNCmp("-All", token, 2) == 0) {
         if (all || bit) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         all = true;
      }
      else if (myStrNCmp("-Bit", token, 2) == 0) {
         if (all || bit) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         bit = insidebits = true;
      }
      else if (myStrNCmp("-DOT", token, 4) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         dot = true;
         insidebits = false;
      }
      else if (myStrNCmp("-PNG", token, 4) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         png = true;
         insidebits = false;
      }
      else if (myStrNCmp("-PS", token, 3) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         ps = true;
         insidebits = false;
      }
      else if (bit && insidebits) {
         if (!myStr2Int(token, temp)) {
            if (pinName == "") { pinName = token; insidebits = false; }
            else if (fileName == "") { fileName = token; insidebits = false; }
            else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         }
         bits.insert((unsigned)temp);
      }
      else if (pinName == "") {
         pinName = token;
      }
      else if (fileName == "") {
         fileName = token;
      }
      else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (!(dot || png || ps)) dot = true;
   if (!(all || bit)) all = true;
   if (pinName == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string pinName)>");
   else if (fileName == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");
   else if (bit && (bits.empty())) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned bit1)>");
   
   if (VLDesign.getCktModule((VLDesign.getTopModName()))->writeCDFG(pinName, fileName, bits)) {
      if (png) dotToPng(fileName);
      else if (ps) dotToPs(fileName);
      Msg(MSG_IFO) << "Write file " << fileName << " complete!";
   }

   return CMD_EXEC_DONE;
}

void
WriteCfgCmd::usage() const {
   Msg(MSG_IFO) << "Usage: WRIte CFG <(string pinName)> <(string fileName)> [| -DOT | -PNG | -PS]" << endl;
   Msg(MSG_IFO) << "                 [| -All | -Bit <(unsigned bit1)> <(unsigned bit2)> ...]" << endl;
}

void
WriteCfgCmd::help() const {
   cout << setw(20) << left << "WRIte CFG:" << "Write out the control-flow graph for a specific pin with assigned bits." << endl;
}

bool
WriteCfgCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isVerilogInput())
      Msg(MSG_WAR) << "Input design was not parsed into CDFG !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// WRIte DFG <(string gateName)>
//           <(string fileName)> [| -DOT | -PNG | -PS] 
//           < < -Level <(unsigned nLevel)> > | 
//             < -Depth <(unsigned nDepth) [| -Time_flow | -Bound_reg] > >
//           [| -Name | -GateType]
// ============================================================================
CmdExecStatus
WriteDfgCmd::exec(const string& option) 
{
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   
   bool dot, png, ps;
   bool level, depth;
   bool time_flow, bound_reg;
   bool name, gateType;
   string gateName, filename;
   unsigned nLevel, nDepth;
   dot = png = ps = level = depth = time_flow = bound_reg = name = gateType = false;
   gateName = filename = "";
   nLevel = nDepth = 0;
   int temp = 0;

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (myStrNCmp("-DOT", token, 4) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (level && (nLevel == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nLevel)>");
         if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth)>");
         dot = true;
      }
      else if (myStrNCmp("-PNG", token, 4) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (level && (nLevel == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nLevel)>");
         if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth)>");
         png = true;
      }
      else if (myStrNCmp("-PS", token, 3) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (level && (nLevel == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nLevel)>");
         if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth)>");
         ps = true;
      }
      else if (myStrNCmp("-Name", token, 2) == 0) {
         if (name || gateType) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (level && (nLevel == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nLevel)>");
         if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth)>");
         name = true;
      }
      else if (myStrNCmp("-GateType", token, 2) == 0) {
         if (name || gateType) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (level && (nLevel == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nLevel)>");
         if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth)>");
         gateType = true;
      }
      else if (myStrNCmp("-Time_flow", token, 2) == 0) {
         if (time_flow || bound_reg) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (gateName == "") 
            return CmdExec::errorOption(CMD_OPT_MISSING, "<(string gateName)>");
         if (filename == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string filename)>");
         if (level) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth>");
         time_flow = true;
      }
      else if (myStrNCmp("-Bound_reg", token, 2) == 0) {
         if (time_flow || bound_reg) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (gateName == "") 
            return CmdExec::errorOption(CMD_OPT_MISSING, "<(string gateName)>");
         if (filename == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string filename)>");
         if (level) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth>");
         bound_reg = true;
      }
      else if (level) {
         if (nLevel != 0) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (!myStr2Int(token, temp)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         else if (temp == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         else nLevel = (unsigned)temp;
      }
      else if (depth) {
         if (nDepth != 0) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (!myStr2Int(token, temp)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         else if (temp == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         else nDepth = (unsigned)temp;
      }
      else if (myStrNCmp("-Level", token, 2) == 0) {
         if (level || depth) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (time_flow || bound_reg) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (gateName == "")
            return CmdExec::errorOption(CMD_OPT_MISSING, "<(string gateName)>");
         if (filename == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string filename)>");
         level = true;
      }
      else if (myStrNCmp("-Depth", token, 2) == 0) {
         if (level || depth) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (gateName == "")
            return CmdExec::errorOption(CMD_OPT_MISSING, "<(string gateName)>");
         if (filename == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string filename)>");
         depth = true;
      }
      else if (gateName == "") {
         if (filename != "") return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (level || depth) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (time_flow || bound_reg) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         gateName = token;
      }
      else if (filename == "") {
         if (level || depth) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (time_flow || bound_reg) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         filename = token;
      }
      else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (gateName == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string gateName)>");
   if (filename == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string filename)>");
   if (!(level || depth)) return CmdExec::errorOption(CMD_OPT_MISSING, "-Level or -Depth");
   if (level && (nLevel == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nLevel)>");
   if (depth && (nDepth == 0)) return CmdExec::errorOption(CMD_OPT_MISSING, "<(unsigned nDepth)>");
   if (!(time_flow || bound_reg)) time_flow = true;
   if (!(dot || png || ps)) dot = true;
   if (!(name || gateType)) name = true;

   CktOutPin* OutPin = 0;
   OutPin = VLDesign.getFltModule()->getOutPin(gateName);
   if (!OutPin) { Msg(MSG_ERR) << "Assigned gateName NOT exists!" << endl; return CMD_EXEC_DONE; }
   
   if (level) VLDesign.getFltModule()->writeGateInLevel(OutPin, filename, nLevel, name);
   else if (time_flow) VLDesign.getFltModule()->writeGateInTimeFlow(OutPin, filename, nDepth, name);
   else VLDesign.getFltModule()->writeGateInBoundedDepth(OutPin, filename, nDepth, name);
   
   if (png) dotToPng(filename);
   else if (ps) dotToPs(filename);
   Msg(MSG_IFO) << "Write file " << filename << " complete!";

   return CMD_EXEC_DONE;
}

void
WriteDfgCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: WRIte DFG <(string gateName)> <(string fileName)> [| -DOT | -PNG | -PS] " << endl;
   Msg(MSG_IFO) << "                 < < -Level <(unsigned nLevel)> > | < -Depth <(unsigned nDepth) [| -Time_flow | -Bound_reg] > >" << endl;
   Msg(MSG_IFO) << "       -Level     : # propagated gates level" << endl;
   Msg(MSG_IFO) << "       -Depth     : # past clock cycles" << endl;
   Msg(MSG_IFO) << "       -Time_flow : In output, INs are PIs and FFs, OUT is the gate assigned" << endl;
   Msg(MSG_IFO) << "       -Bound_reg : In output, INs are PIs and FFs, OUTs are the gate assigned and FFs" << endl;
}

void
WriteDfgCmd::help() const 
{
   cout << setw(20) << left << "WRIte DFG:" << "Write out the data-flow graph of assigned gate in flattened design." << endl;
}

bool
WriteDfgCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has not been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// WHOAMI <(string signalName)>
// ============================================================================
CmdExecStatus
WhoAmICmd::exec(const string& option) 
{
   // check option 
   string token;
   if (!CmdExec::lexSingleOption(option, token, false)) {
      usage();
      return CMD_EXEC_ERROR;
   }

   // Exec function
   CktOutPin* OutPin = VLDesign.getFltModule()->getOutPin(token);
   if (!OutPin) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   else Msg(MSG_IFO) << OutPin->getCell()->getExpr(OutPin);
   return CMD_EXEC_DONE;
}

void
WhoAmICmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: WhoAmI <(string signalName)>" << endl;
}

void
WhoAmICmd::help() const 
{
   cout << setw(20) << left << "WHOAMI:" << "Get Expression of signal in terms of declared ones." << endl;
}

bool
WhoAmICmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has not been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// SIMulate <(string testfile)>
// ============================================================================
CmdExecStatus
SimulateCmd::exec(const string& option) 
{
   // check option 
   string token;
   if (!CmdExec::lexSingleOption(option, token, false)) {
      usage();
      return CMD_EXEC_ERROR;
   }

   // Exec function
   bool t = VLDesign.getFltModule()->simMgr(token);
   if (!t) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   return CMD_EXEC_DONE;
}

void
SimulateCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: SIMulate <(string testfile)>" << endl;
}

void
SimulateCmd::help() const 
{
   cout << setw(20) << left << "SIMulate:" << "simulate circuit with testfile" << endl;
}

bool
SimulateCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has not been flattened !!!" << endl;
   else return true;
   return false;
}

#endif
