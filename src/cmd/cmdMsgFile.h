/****************************************************************************
  FileName     [ cmdMsgFile.h ]
  PackageName  [ cmd ]
  Synopsis     [ Definition of class for Output message direction control. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2009 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _CMD_MSGFILE_H
#define _CMD_MSGFILE_H

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;

class MsgOutput;
class CmdMsgMgr;

extern CmdMsgMgr Msg;

class MsgOutput
{
   public : 
      MsgOutput(ostream& os) : _default(os) { _defaultON = true; _filename = ""; }
      ~MsgOutput() {}
      bool isOutFileON() { return _outfile.is_open(); }
      void setOutFile(const string filename) { _filename = filename; }
      void stopOutFile() { if (isOutFileON()) _outfile.close(); }
      void startOutFile(bool append) {
         stopOutFile();
         if (append) _outfile.open(_filename.c_str(), ios::app);
         else _outfile.open(_filename.c_str());
      }
      void stopDefault() { _defaultON = false; }
      void startDefault() { _defaultON = true; }

      // operator overload
      template<class T> friend MsgOutput& operator << (MsgOutput& m, const T& s) {
         if (m._defaultON) m._default << s;
         if (m.isOutFileON()) m._outfile << s;
         if (MsgOutput::_allout.is_open()) MsgOutput::_allout << s;
         return m;
      }

      typedef ostream& (*StdEndLine)(ostream&);  // overload operator << for std::endl
      friend MsgOutput& operator << (MsgOutput& m, StdEndLine e) {
         if (m._defaultON) e(m._default);
         if (m.isOutFileON()) e(m._outfile);
         if (MsgOutput::_allout.is_open()) e(MsgOutput::_allout);
         return m;
      }
      static ofstream   _allout;
      static string     _allName;
   private : 
      bool              _defaultON;
      string            _filename;
      ofstream          _outfile;
      ostream&          _default;
};

enum CmdMsgType
{
   MSG_LOG = 0,
   MSG_ERR = 1,
   MSG_WAR = 2,
   MSG_DBG = 3,
   MSG_IFO = 4
};

class CmdMsgMgr
{
   public :
      CmdMsgMgr() {
         Msgs.clear();
         MsgTypeString.clear();
         Msgs.push_back(new MsgOutput(cout)); MsgTypeString.push_back("");             // 0 : CmdLogMsg
         Msgs.push_back(new MsgOutput(cerr)); MsgTypeString.push_back("[ERROR]: ");    // 1 : ErrorMsg
         Msgs.push_back(new MsgOutput(cerr)); MsgTypeString.push_back("[WARNING]: ");  // 2 : WarningMsg
         Msgs.push_back(new MsgOutput(cout)); MsgTypeString.push_back("");             // 3 : DebugMsg
         Msgs.push_back(new MsgOutput(cout)); MsgTypeString.push_back("");             // 4 : InfoMsg
         Msgs[0]->stopDefault();  // NO std output needed for command Log
      }
      ~CmdMsgMgr() { Msgs.clear(); }
      MsgOutput& operator () (const CmdMsgType type) { (*Msgs[type]) << MsgTypeString[type]; return *Msgs[type]; }
      void setOutFile(const string filename, const CmdMsgType type) { Msgs[type]->setOutFile(filename); }
      void startOutFile(const CmdMsgType type, bool append = false) { Msgs[type]->startOutFile(append); }
      void stopOutFile(const CmdMsgType type) { Msgs[type]->stopOutFile(); }
      void startDefault(const CmdMsgType type) { Msgs[type]->startDefault(); }
      void stopDefault(const CmdMsgType type) { Msgs[type]->stopDefault(); }
      
      bool isAllOutFileON() { return MsgOutput::_allout.is_open(); }
      void setAllOutFile(const string filename) { MsgOutput::_allName = filename; }
      void startAllOutFile(bool append = false) {
         stopAllOutFile();
         if (append) MsgOutput::_allout.open(MsgOutput::_allName.c_str(), ios::app);
         else MsgOutput::_allout.open(MsgOutput::_allName.c_str());
      }
      void stopAllOutFile() { if (isAllOutFileON()) MsgOutput::_allout.close(); }
      void startAllDefault() { for (unsigned i = 1; i < 5; ++i) Msgs[i]->startDefault(); }
      void stopAllDefault() { for (unsigned i = 1; i < 5; ++i) Msgs[i]->stopDefault(); }
   private : 
      vector<string> MsgTypeString;
      vector<MsgOutput*> Msgs;
};

#endif

