/****************************************************************************
  FileName     [ cmdParser.h ]
  PackageName  [ cmd ]
  Synopsis     [ Command Line Parser ]
  Author       [ Chung-Yang (Ric) Huang, Cheng-Yin Wu, and Steggie Chen ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <iostream>
#include <fstream>
#include <climits>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

#define MAX_CMD_LEN 2 // maximum number of words in a command

// Forward Declarations
class CmdExec;
class CmdParser;
class DesignMgr;

// External Declarations
extern CmdParser* cmdMgr;

// Input Characters
enum ParseChar
{  
   // Meta type
   FUNC_BASE_SHIFT  = 1 << 8,  // shifted 8 bits (char size)
   ARROW_BASE_SHIFT = 1 << 9,
   DELETE_BASE_SHIFT= 1 << 10,
   UNDEFINED_CHAR   = INT_MAX,
   
   // char #1
   LINE_BEGIN_CHAR  = 1,  // ctrl-a
   LINE_END_CHAR    = 5,  // ctrl-e
   INPUT_END_CHAR   = 4,  // ctrl-d
   BEEP_CHAR        = 7,
   BACK_SPACE_CHAR  = 8,
   BACK_SPACE_KEY   = 127,
   ESC_CHAR         = 27, 
   DELETE_CHAR      = 126 + DELETE_BASE_SHIFT,
   TAB_CHAR         = int('\t'),
   NEWLINE_CHAR     = int('\n'),
   
   // char #2
   FUNC_BASE_CHAR   = 79,
   ARROW_BASE_CHAR  = 91,
   
   // char #3
   FUNC_F1_CHAR     = 80 + FUNC_BASE_SHIFT,
   FUNC_F2_CHAR     = 81 + FUNC_BASE_SHIFT,
   FUNC_F3_CHAR     = 82 + FUNC_BASE_SHIFT,
   FUNC_F4_CHAR     = 83 + FUNC_BASE_SHIFT,
   ARROW_UP_CHAR    = 65 + ARROW_BASE_SHIFT,
   ARROW_DOWN_CHAR  = 66 + ARROW_BASE_SHIFT,
   ARROW_RIGHT_CHAR = 67 + ARROW_BASE_SHIFT,
   ARROW_LEFT_CHAR  = 68 + ARROW_BASE_SHIFT,
   DELETE_BASE_CHAR = 51,
   
   // dummy end
   PARSE_CHAR_END
};

// Command Categories to String
const string CmdTypeString[] = {
   "Revealed",
   "Common",
   "I/O",
   "Report",
   "Print",
   "Design Flow",
   "Ckt Construction",
   "Verification",
   "Extraction",
   "UnClassified"
};

// Command Categories Enum
enum CmdType
{
   // Revealed command
   CMD_TYPE_REVEALED = 0,
   
   CMD_TYPE_COMMON   = 1,
   CMD_TYPE_IO       = 2,
   CMD_TYPE_REPORT   = 3,
   CMD_TYPE_PRINT    = 4,
   CMD_TYPE_DESFLOW  = 5,
   CMD_TYPE_BUILDCKT = 6,
   CMD_TYPE_VERIFY   = 7,
   CMD_TYPE_EXTRACT  = 8,
   CMD_TYPE_DEFAULT  = 9
};

// Command Execution Status
enum CmdExecStatus
{  
   CMD_EXEC_DONE  = 0,
   CMD_EXEC_ERROR = 1,
   CMD_EXEC_QUIT  = 2,
   CMD_EXEC_NOP   = 3,

   // dummy
   CMD_EXEC_TOT
};

// Command Execution Error Types
enum CmdOptionError
{
   CMD_OPT_MISSING    = 0,
   CMD_OPT_EXTRA      = 1,
   CMD_OPT_ILLEGAL    = 2,
   CMD_OPT_FOPEN_FAIL = 3,

   // dummy
   CMD_OPT_ERROR_TOT
};

// Class CmdExec
class CmdExec
{
   public :
      CmdExec(CmdType t) : _cmdType(t) {}
      virtual ~CmdExec() {}

      // Command Argument Judgement and Action
      virtual CmdExecStatus exec(const string&) = 0;
      virtual void usage() const = 0;
      virtual void help() const = 0;
      virtual bool valid() const { return true; }
      
      inline CmdType getCmdType() const { return _cmdType; }
      inline void setCmdLen(unsigned n) { _cmdLen = n; }
      inline unsigned getCmdLen() const { return _cmdLen; }
      
      inline void setMandCmd(const string& str) { _mandCmd.push_back(str);}
      inline const string& getMandCmd(size_t idx) const { return _mandCmd[idx]; }
      inline void setOptCmd(const string& str) { _optCmd.push_back(str); }
      inline const string& getOptCmd(size_t idx) const { return _optCmd[idx]; }
      
      bool checkCmd(const string& check) const;
      bool checkCmd(const string& check, size_t idx) const;
      bool checkMandCmd(const string& check, size_t idx) const;
      bool checkOptCmd(const string& check, size_t idx) const;
      
      bool operator < (const CmdExec&) const;
   
   protected :
      bool lexSingleOption(const string&, string&, bool optional = true) const;
      bool lexOptions(const string&, vector<string>&, size_t nOpts = 0) const;
      CmdExecStatus errorOption(CmdOptionError err, const string& opt) const;
   
   private :
      CmdType           _cmdType;
      unsigned          _cmdLen;
      vector<string>    _mandCmd;
      vector<string>    _optCmd;
};

struct ltcontent
{
   bool operator()(const CmdExec* lhs, const CmdExec* rhs) const
   {
      return (*lhs < *rhs);
   }
};

typedef set<CmdExec*, ltcontent>       CmdExecSubSet;
typedef map<CmdType, CmdExecSubSet*>   CmdExecSet;

#define READ_BUF_SIZE    65536
#define MAX_HISTORY      65536

class CmdParser
{
   public :
      CmdParser(const string& p) : _moduleDepth(0), 
                                   _prompt(p), _defaultPrompt(p), _readBufPtr(_readBuf), _readBufEnd(_readBuf), 
                                   _historyIdx(0), _historySize(0), _allow_tab(0) { setPrompt(); }
      virtual ~CmdParser() {}
      
      // Command Registration
      bool regCmd(const string&, unsigned, CmdExec*);
      bool regCmd(const string&, unsigned, unsigned, CmdExec*);
      
      // Command Retrieval
      CmdExec* getCmd(const string&) const;
      CmdExecSubSet getCmdListFromPart(const string&) const;

      // Command Parse and Exec
      CmdExecStatus execOneCmd();
      CmdExecStatus execOneCmdWithTab();

      // For Command Dofile
      bool openDofile(const string& dof) { _dofile.open(dof.c_str()); return _dofile.is_open(); }
      void closeDofile() { _dofile.close(); _dofile.clear(); }

      // For Command Help
      void printHelps(bool = false) const;

      // For Command History
      void printHistory(int nPrint = -1) const;

      // Prompt
      inline string getPrompt() const { return _prompt; }
      inline void setPrompt() { _prompt = _defaultPrompt + "> "; }
      inline void setPrompt(string s) { _prompt = s + "> "; }

      // File Completion Support
      inline void setTab(bool t) { _allow_tab = t; }
      inline bool AllowTab() { return _allow_tab; }
      
////////////////////////// THOSE SHOULD BE MOVED TO CKT PACKAGE AFTER /////////////////////////////
      int getModuleDepth() const { return _moduleDepth; }
      int incModuleDepth() { return ++_moduleDepth; }
      int decModuleDepth() { return --_moduleDepth; }
      int             _moduleDepth;     // for DEFine MODule and CHAnge MODule use
//////////////////////////////////////////////////////////////////////////////////////////////////
   
   private :
      // Command Helper Functions
      bool readCmd(istream&);
      CmdExec* parseCmd(string&);

      // Helper functions
      inline void printPrompt() const { cout << _prompt; }
      ParseChar getChar(istream&) const;
      bool moveBufPtr(char* const);
      bool deleteChar();
      void insertChar(char);
      void deleteLine();
      bool addHistory();
      void retrieveHistory();
      
      // Data members
      string         _prompt;                // Current Command Prompt
      const string   _defaultPrompt;         // Default Command Prompt
      ifstream       _dofile;                // For Script Parsing
      char           _readBuf[READ_BUF_SIZE];// Save Current Command Line Input
      char*          _readBufPtr;            // Pointer to the Cursor's Position
      char*          _readBufEnd;            // Pointer to End Position of Cursor (*_readBufEnd = 0)
      string         _history[MAX_HISTORY];  // Command History Stack
      size_t         _historyIdx;            // Position for Next History Insersion
      size_t         _historySize;           // Number of Valid History Stack Size
      CmdExecSet     _cmdLib;                // All Available Commands
      bool           _allow_tab;             // Filename Completion Toggling
};


#endif // CMD_PARSER_H
