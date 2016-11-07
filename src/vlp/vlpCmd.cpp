/****************************************************************************
  FileName     [ vlpCmd.cpp ]
  PackageName  [ vlp ]
  Synopsis     [ Design Flow Commands ]
  Author       [ Cheng-Yin Wu, Hu-Hsi Yeh ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_CMD_SOURCE
#define VLP_CMD_SOURCE

#include "vlpCmd.h"

// External Declarations
extern DesignMgr quteMgr;
extern VlpDesign& VLDesign;

bool
initVlpCmd() 
{
   return 
         ( 
          cmdMgr->regCmd("REAd DEsign",      3, 2, new ReadDesignCmd)   &&
          cmdMgr->regCmd("WRIte DEsign",     3, 2, new WriteDesignCmd)  &&  
          cmdMgr->regCmd("WRIte CKt",        3, 2, new WriteCktCmd)     &&
          cmdMgr->regCmd("WRIte CDfg",       3, 2, new WriteCDFGCmd)    &&
          cmdMgr->regCmd("WRIte HIErarchy",  3, 3, new WriteHierCmd)    &&
          cmdMgr->regCmd("REPort DEsign",    3, 2, new ReportDesignCmd) &&
          cmdMgr->regCmd("REPort CKt",       3, 2, new ReportCktCmd)    &&
          cmdMgr->regCmd("REPort MOdule",    3, 2, new ReportModuleCmd) &&  
          cmdMgr->regCmd("REPort CDfg",      3, 2, new ReportCDFGCmd)   &&
          cmdMgr->regCmd("REPort GAte",      3, 2, new ReportGateCmd)   &&
          cmdMgr->regCmd("SYNthesis",        3,    new SynthesisCmd)    &&
          cmdMgr->regCmd("FLAtten",          3,    new FlattenCmd)
         );
}

// ============================================================================
// REAd DEsign [-Verilog | -Blif | -BTOR] <[-Filelist] (string filename)>
// ============================================================================
CmdExecStatus
ReadDesignCmd::exec(const string& option) 
{
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   size_t n = options.size();
   if (n == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "");
   
   bool verilog = false, blif = false, btor = false;
   bool file = false, strfile = false, finvalid = false;
   string filename = "";

   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (myStrNCmp("-Filelist", token, 2) == 0) {
         if (file) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (finvalid) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         file = true;
      }
      else if (myStrNCmp("-Verilog", token, 2) == 0) {
         if (verilog | blif | btor) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (file && !strfile) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (strfile && !finvalid) finvalid = true;
         verilog = true; 
      }
      else if (myStrNCmp("-Blif", token, 2) == 0) {
         if (verilog | blif | btor) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (file && !strfile) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (strfile && !finvalid) finvalid = true;
         blif = true;
      }
      else if (myStrNCmp("-BTOR", token, 5) == 0) {
         if (verilog | blif | btor) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         if (file && !strfile) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
         if (strfile && !finvalid) finvalid = true;
         btor = true;
      }
      else if (!strfile) {
         filename = token;
         strfile = true;
      }
      else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }   
   
   if (!(verilog | blif | btor)) verilog = true;
   if (!strfile) return CmdExec::errorOption(CMD_OPT_MISSING, "(string filename)");
   else {
      bool success;
      if (verilog) {
         if (file) success = VLDesign.ParseFileList(filename.c_str());
         else success = VLDesign.ParseVerilog(filename.c_str(), false);
         if (success) VLDesign.resolveHier();
      }
      else if (blif) success = VLDesign.ParseBLIF(filename.c_str());
      else if (btor) success = VLDesign.ParseBTOR(filename.c_str());
      else assert (0);
      if (success) quteMgr.setDesignMode(DESIGN_MODE);
   }
   return CMD_EXEC_DONE;
}

void
ReadDesignCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: REAd DEsign [-Verilog | -Blif] <[-Filelist] (string filename)>" << endl;
}

void
ReadDesignCmd::help() const 
{
   cout << setw(20) << left << "REAd DEsign:" << "Read design or the filelist of designs." << endl;
}

bool
ReadDesignCmd::valid() const
{
   if (VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design exists already !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// SYNthesis
// ============================================================================
CmdExecStatus
SynthesisCmd::exec(const string& option) 
{
   VLDesign.synthesis();
   return CMD_EXEC_DONE;
}

void
SynthesisCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: SYNthesis" << endl;
}

void
SynthesisCmd::help() const 
{
   cout << setw(20) << left << "SYNthesis:" << "Perform synthesis on parsed RTL design." << endl;
}

bool
SynthesisCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isVerilogInput())
      Msg(MSG_WAR) << "Input design was not parsed into CDFG !!!" << endl;
   else if (VLDesign.isHierModule() || VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been synthesized !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// FLAtten 
// ============================================================================
CmdExecStatus
FlattenCmd::exec(const string& option) 
{
   VLDesign.completeFlatten();
   return CMD_EXEC_DONE;
}

void
FlattenCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: FLAtten" << endl;
}

void
FlattenCmd::help() const 
{
   cout << setw(20) << left << "FLAtten:" << "Perform flatten on synthesized design." << endl;
}

bool
FlattenCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized!!!" << endl;
   else if (VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has already been flattened !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// REPort DEsign [-STATistics | -VERbose]
// ============================================================================
CmdExecStatus
ReportDesignCmd::exec(const string& option) 
{
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);

   bool stat = false, verb = false;
   size_t n = options.size();
   if (n == 0) stat = true;
   else {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-STATistics", token, 5) == 0) {
            if (stat | verb) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            stat = true; 
         }
         else if (myStrNCmp("-VERbose", token, 4) == 0) {
            if (stat | verb) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            verb = true;
         }
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
   }
   if (stat && !verb) VLDesign.repDeStat();
   else if (!stat && verb) VLDesign.repDeVer();
   else cout << "Programming Error?!" << endl;
   return CMD_EXEC_DONE;
}

void
ReportDesignCmd::usage() const {
   Msg(MSG_IFO) << "Usage: REPort DEsign [-STATistics | -VERbose]" << endl;
}

void
ReportDesignCmd::help() const {
   cout << setw(20) << left << "REPort DEsign:" << "Report hierarchical design information." << endl;
}

bool
ReportDesignCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized!!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// REPort CKt [-STATistics | -VERbose]
// ============================================================================
CmdExecStatus
ReportCktCmd::exec(const string& option) 
{
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);

   bool stat = false, verb = false;
   size_t n = options.size();
   if (n == 0) stat = true;
   else {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-STATistics", token, 5) == 0) {
            if (stat | verb) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            stat = true; 
         }
         else if (myStrNCmp("-VERbose", token, 4) == 0) {
            if (stat | verb) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            verb = true;
         }
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
   }
   if (stat && !verb) VLDesign.repCktStat();
   else if (!stat && verb) VLDesign.repCktVer();
   else cout << "Programming Error?!" << endl;
   return CMD_EXEC_DONE;
}

void
ReportCktCmd::usage() const {
   Msg(MSG_IFO) << "Usage: REPort CKt [-STATistics | -VERbose]" << endl;
}

void
ReportCktCmd::help() const {
   cout << setw(20) << left << "REPort CKt:" << "Report flattened circuit information." << endl;
}

bool
ReportCktCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has not been flattened!!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// REPort MOdule < |(string moduleName)> [-Level <(int nLevel)>]
//               [-Port_list] [-Instance_list]
// ============================================================================
CmdExecStatus
ReportModuleCmd::exec(const string& option) 
{
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   
   bool repPort = false, repIns = false;
   bool strMod = false, parLevel = false;
   int nLevel = -1;
   string moduleName = "";
   
   size_t n = options.size();
   if (n != 0) {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-Level", token, 2) == 0) {
            if (parLevel) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            if (nLevel != -1) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            parLevel = true;
         }
         else if (myStrNCmp("-Port_list", token, 2) == 0) {
            if (repPort) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            if (parLevel) return CmdExec::errorOption(CMD_OPT_MISSING, "(int nLevel)");
            repPort = true;
         }
         else if (myStrNCmp("-Instance_list", token, 2) == 0) {
            if (repIns) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            if (parLevel) return CmdExec::errorOption(CMD_OPT_MISSING, "(int nLevel)");
            repIns = true;
         }
         else if (parLevel) {
            if (!myStr2Int(token, nLevel)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
            else parLevel = false;
         }
         else if ((!repPort) && (!repIns) && (!strMod) && (!parLevel)) {
            moduleName = token;
            strMod = true;
         }
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
   }      
   if (parLevel) return CmdExec::errorOption(CMD_OPT_MISSING, "(int nLevel)");
   //if (moduleName == "") moduleName = VLDesign.getHier()->root()->moduleName();
   if (moduleName == "") moduleName = VLDesign.getTopModName();
   if (nLevel == -1)     nLevel = 99;
   
   unsigned type = 0;
   if (repPort && repIns)       type = 0;
   else if (!repPort && repIns) type = 1;
   else if (repPort && !repIns) type = 2;
   else                         type = 3;

   VLDesign.repModule(moduleName, nLevel, type);
   return CMD_EXEC_DONE;
}

void
ReportModuleCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: REPort MOdule < |(string moduleName)> [-Level <(int nLevel)>] [-Port_list] [-Instance_list]" << endl;
}

void
ReportModuleCmd::help() const 
{
   cout << setw(20) << left << "REPort MOdule:" << "Report module information." << endl;
}

bool
ReportModuleCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isVerilogInput())
      Msg(MSG_WAR) << "Input design was not parsed into CDFG !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// REPort CDfg < | (string topModule) | (int nodeID)> 
//             [-Max_level | -Level <(int nLevel)>]
// ============================================================================
CmdExecStatus
ReportCDFGCmd::exec(const string& option) 
{
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   
   bool strMod = false, repAll = false, parLevel = false;
   int nLevel = -1;
   int nodeID = -1;
   string moduleName = "";
   
   size_t n = options.size();
   if (n == 0) repAll = true;
   else {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-Level", token, 2) == 0) {
            if (repAll) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
            if (parLevel) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            if (nLevel != -1) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            parLevel = true;
         }
         else if (myStrNCmp("-Max_level", token, 2) == 0) {
            if (repAll) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
            if (parLevel) return CmdExec::errorOption(CMD_OPT_MISSING, "(int nLevel)");
            if (nLevel != -1) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            repAll = true;             
         }
         else if (parLevel) {
            if (!myStr2Int(token, nLevel)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
            else parLevel = false;
         }
         else if ((!strMod) && (!repAll) && (!parLevel)) {
            if (!myStr2Int(token, nodeID)) {
               moduleName = token;
               nodeID = -1;
            }
            strMod = true;
         }
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
   }
   
   if (parLevel) return CmdExec::errorOption(CMD_OPT_MISSING, "(int nLevel)");
   if (moduleName == "" && nodeID == -1) moduleName = VLDesign.getTopModName();
   if (nLevel == -1) nLevel = 99;

   VLDesign.repCDFG(moduleName, nLevel, nodeID);
   return CMD_EXEC_DONE;
}

void
ReportCDFGCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: REPort CDfg < | (string topModule) | (int nodeID)> [-Max_level | -Level <(int nLevel)>]" << endl;
}

void
ReportCDFGCmd::help() const 
{
   cout << setw(20) << left << "REPort CDfg:" << "Report assigned node's CDFG information." << endl;
}

bool
ReportCDFGCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isVerilogInput())
      Msg(MSG_WAR) << "Input design was not parsed into CDFG !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// REPort GAte <(string gateName))> [| -FO | -FI] [-Level <(int nLevel)>]
// ============================================================================
CmdExecStatus
ReportGateCmd::exec(const string& option) 
{
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);
   
   bool fo = false, fi = false, parLevel = false;
   int nLevel = -1;
   string gatename = "";
   
   size_t n = options.size();
   if (n == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "<(string GateName)>");
   else {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-FO", token, 3) == 0) {
            if (fo || fi) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            fo = true;
         }
         else if (myStrNCmp("-FI", token, 3) == 0) {
            if (fo || fi) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            fi = true;  
         }
         else if (myStrNCmp("-Level", token, 2) == 0) {
            if (parLevel) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            if (nLevel != -1) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            parLevel = true;
         }
         else if (parLevel) {
            if (!myStr2Int(token, nLevel)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
            else parLevel = false;
         }
         else if (gatename == "") gatename = token;
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
   }
   if (parLevel) return CmdExec::errorOption(CMD_OPT_MISSING, "(int nLevel)");
   if (gatename == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string gateName)>");
   
   if (nLevel == -1) nLevel = 1;
   if (!(fo | fi)) { fo = true; }
            
   string str = "";
   if (fo) VLDesign.repFOGate(gatename, nLevel);
   else if (fi) VLDesign.repFIGate(gatename, nLevel);
   else str = " Programming Error?!";
   return CMD_EXEC_DONE;
}

void
ReportGateCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: REPort GAte <(string gateName)> [| -FO | -FI] [-Level <(int nLevel)>]" << endl;
}

void
ReportGateCmd::help() const 
{
   cout << setw(20) << left << "REPort GAte:" << "Report assigned gate's information." << endl;
}

bool
ReportGateCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has not been flattened!!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// REPort FSM < | (string moduleName)>
// ============================================================================
CmdExecStatus
ReportFSMCmd::exec(const string& option)
{
   vector<string> options;
   CmdExec::lexOptions(option, options);
   string filename = "";
   size_t n = options.size();

   if (n > 1) return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
   else if (n == 1) filename = options[0];

   // Exec Functions
   Msg(MSG_ERR) << "REPort FSM has not been implemented (unusable NOW)!" << endl;
   return CMD_EXEC_DONE;
}

void
ReportFSMCmd::usage() const
{
   Msg(MSG_IFO) << "Usage: REPort FSM < | (string moduleName)>" << endl;
}

void
ReportFSMCmd::help() const
{
   cout << setw(20) << left << "REPort FSM:" << "Report design FSM information." << endl;
}

bool
ReportFSMCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isFSMExtracted())
      Msg(MSG_WAR) << "FSM has not been extracted !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// WRIte DEsign [(string fileName)]
// ============================================================================
CmdExecStatus
WriteDesignCmd::exec(const string& option)
{
   vector<string> options;
   CmdExec::lexOptions(option, options);
   if (options.size() == 0) VLDesign.synWriteDesign("");
   else if (options.size() == 1) VLDesign.synWriteDesign(options[0]);
   else return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
   return CMD_EXEC_DONE;
}

void
WriteDesignCmd::usage() const {
   Msg(MSG_IFO) << "Usage: WRIte DEsign [(string fileName)]" << endl;
}

void
WriteDesignCmd::help() const 
{
   cout << setw(20) << left << "WRIte DEsign:" << "Write out hierarchical Verilog design." << endl;
}

bool
WriteDesignCmd::valid() const
{
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isHierModule())
      Msg(MSG_WAR) << "Design has not been synthesized!!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// WRIte CKt [| -Verilog|-Blif] [(string fileName)]
// ============================================================================
CmdExecStatus
WriteCktCmd::exec(const string& option) 
{
   vector<string> options;
   CmdExec::lexOptions(option, options);
   bool verilog = false, blif = false;
   string filename = "";
   size_t n = options.size();
   
   if (n == 0) verilog = true;
   else {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-Blif", token, 2) == 0) {
            if ((!verilog) && (!blif)) blif = true;
            else return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         }
         else if (myStrNCmp("-Verilog", token, 2) == 0) {
            if ((!verilog) && (!blif)) verilog = true;
            else return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         }
         else if (filename == "") filename = token;
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
   }
   if ((!verilog) && (!blif)) verilog = true;
   if (verilog) VLDesign.fltWriteDesign(filename);
   else VLDesign.writeBLIF(filename);
   return CMD_EXEC_DONE;
}

void
WriteCktCmd::usage() const {
   Msg(MSG_IFO) << "Usage: WRIte CKt [| -Verilog|-Blif] [(string fileName)]" << endl;
}

void
WriteCktCmd::help() const {
   cout << setw(20) << left << "WRIte CKt:" << "Write out flattened Verilog or BLIF circuit." << endl;
}

bool
WriteCktCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isFlatModule())
      Msg(MSG_WAR) << "Design has not been flattened!!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// WRIte CDfg <| (string moduleName) > <(string pinName)> 
//            <(string fileName)> [| -DOT | -PNG | -PS | -TXT]
// ============================================================================
CmdExecStatus
WriteCDFGCmd::exec(const string& option) 
{
   vector<string> options;
   CmdExec::lexOptions(option, options);
   
   string moduleName = "";
   string pinName = "";
   string fileName = "";
   bool dot, png, ps, txt;
   dot = png = ps = txt;
   
   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (myStrNCmp("-DOT", token, 4) == 0) {
         if (dot || png || ps || txt) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         dot = true;
      }
      else if (myStrNCmp("-PNG", token, 4) == 0) {
         if (dot || png || ps || txt) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         png = true;
      }
      else if (myStrNCmp("-PS", token, 3) == 0) {
         if (dot || png || ps || txt) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         ps = true;
      }
      else if (myStrNCmp("-TXT", token, 4) == 0) {
         if (dot || png || ps || txt) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         txt = true;
      }
      else if (moduleName == "") moduleName = token;
      else if (pinName == "") pinName = token;
      else if (fileName == "") fileName = token;
      else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }
   
   if (fileName == "") {
      fileName = pinName;
      pinName = moduleName;
      moduleName = "";
   }
   if (!(dot || png || ps || txt)) dot = true;
   if (pinName == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string pinName)>");
   else if (fileName == "") {
      if (!txt) return CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");
   }
   if (moduleName == "") moduleName = VLDesign.getTopModName();
   
   if (txt) VLDesign.repCDFG(moduleName, pinName);
   else if (VLDesign.writeCDFG(moduleName, pinName, fileName)) {
      if (png) dotToPng(fileName);
      else if (ps) dotToPs(fileName);
      Msg(MSG_IFO) << "Write file " << fileName << " complete!";
   }
   return CMD_EXEC_DONE;
}

void
WriteCDFGCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: WRIte CDfg <| (string moduleName) > <(string pinName)> " << "<(string fileName)> [| -DOT | -PNG | -PS]" << endl;
}

void
WriteCDFGCmd::help() const {
   cout << setw(20) << left << "WRIte CDfg:" << "Write the CDFG of assigned node before synthesis." << endl;
}

bool
WriteCDFGCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else if (!VLDesign.isVerilogInput())
      Msg(MSG_WAR) << "Input design was not parsed into CDFG !!!" << endl;
   else return true;
   return false;
}

// ============================================================================
// WRIte HIErarchy < < | (string moduleName) > <(string fileName)> > 
//                 [| -DOT | -PNG | -PS]
// ============================================================================
CmdExecStatus
WriteHierCmd::exec(const string& option) 
{
   vector<string> options;
   CmdExec::lexOptions(option, options);
   
   string moduleName = "";
   string fileName = "";
   bool dot = false, png = false, ps = false;
   
   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (myStrNCmp("-DOT", token, 4) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         dot = true;
      }
      else if (myStrNCmp("-PNG", token, 4) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         png = true;
      }
      else if (myStrNCmp("-PS", token, 3) == 0) {
         if (dot || png || ps) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         ps = true;
      }
      else if (moduleName == "") {
         moduleName = token;
      }
      else if (fileName == "") {
         fileName = token;
      }
      else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (fileName == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");
   if (!(dot || png || ps)) dot = true;

   if (VLDesign.writeHierarchy(moduleName, fileName)) {
      if (png) dotToPng(fileName);
      else if (ps) dotToPs(fileName);
      Msg(MSG_IFO) << "Write file " << fileName << " complete!";
   }

   return CMD_EXEC_DONE;
}

void
WriteHierCmd::usage() const 
{
   Msg(MSG_IFO) << "Usage: WRIte HIErarchy < < | (string moduleName) > <(string fileName)> > [| -DOT | -PNG | -PS]" << endl;
}

void
WriteHierCmd::help() const {
   cout << setw(20) << left << "WRIte HIErarchy:" << "Write design hierarchical relationship." << endl;
}

bool
WriteHierCmd::valid() const {
   if (!VLDesign.isDesignRead())
      Msg(MSG_WAR) << "Design does not exist !!!" << endl;
   else return true;
   return false;
}

#endif
