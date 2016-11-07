/****************************************************************************
  FileName     [ cmdCommon.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Global Commands Implementation ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CMD_COMMON_SOURCE
#define CMD_COMMON_SOURCE

#include <iomanip>
#include <string>
#include "util.h"
#include "cmdCommon.h"

using namespace std;

bool
initCommonCmd()
{
   return 
         (
          cmdMgr->regCmd("DOfile",        2,    new DofileCmd)    &&
          cmdMgr->regCmd("HELp",          3,    new HelpCmd)      &&
          cmdMgr->regCmd("HIStory",       3,    new HistoryCmd)   &&
          cmdMgr->regCmd("USAGE",         5,    new UsageCmd)     &&
          cmdMgr->regCmd("Quit",          1,    new QuitCmd)      &&
          cmdMgr->regCmd("SET LOgfile",   3, 2, new SLogFileCmd)
         );
}

//----------------------------------------------------------------------
// HELp [(string cmd) | -Revealed]
//----------------------------------------------------------------------
CmdExecStatus
HelpCmd::exec(const string& option)
{
   // check option
   vector<string> tokens;
   if (!CmdExec::lexOptions(option, tokens))
      return CMD_EXEC_ERROR;
   if (tokens.size() == 0) // without cmd
      cmdMgr->printHelps();
   else if ((tokens.size() == 1) && (myStrNCmp("-Revealed", tokens[0], 2) == 0))
      cmdMgr->printHelps(true);
   else // with cmd
   {
      string cmd = "";
      CmdExec* e = 0;
      CmdExecSubSet list;

      for (unsigned i = 0, n = tokens.size(); i < n; ++i)
      {
         if (i > 0) cmd.append(" ");
         cmd.append(tokens[i]);

         e = cmdMgr->getCmd(cmd);
         list = cmdMgr->getCmdListFromPart(cmd);

         if (e != 0) // if exact match
         {
            e->usage();
            return CMD_EXEC_DONE;
         }
      }
      
      // if partial match
      if (!list.empty())
      {
         for (CmdExecSubSet::iterator it = list.begin(); it != list.end(); ++it)
         {
            (*it)->help();
         }
         return CMD_EXEC_DONE;
      }
      else
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, cmd); 
   }
   
   return CMD_EXEC_DONE;
}

void
HelpCmd::usage() const
{
   Msg(MSG_IFO) << "Usage: HELp [(string cmd)]" << endl;
}

void
HelpCmd::help() const
{
   cout << setw(20) << left << "HELp: " << "Print this help message." << endl;
}

//----------------------------------------------------------------------
// Quit [-Force]
//----------------------------------------------------------------------
CmdExecStatus
QuitCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      if (myStrNCmp("-Forced", token, 2) != 0)
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      else
         return CMD_EXEC_QUIT;  // ready to quit
   }

   cout << "Are you sure to quit (Yes/No)? [No] ";
   char str[READ_BUF_SIZE];
   cin.getline(str, READ_BUF_SIZE);
   string ss = string(str);
   if (ss.size()) {
      size_t s = ss.find_first_not_of(' ', 0);
      if (s != string::npos)
         ss = ss.substr(s);
   }
   if (myStrNCmp("Yes", ss, 1) == 0)
      return CMD_EXEC_QUIT;  // ready to quit
   return CMD_EXEC_DONE;     // not yet to quit
}

void
QuitCmd::usage() const
{
   Msg(MSG_IFO) << "Usage: Quit [-Force]" << endl;
}

void
QuitCmd::help() const
{
   cout << setw(20) << left << "Quit: " << "Quit the execution." << endl;
}

//----------------------------------------------------------------------
// HIStory [(int nPrint)]
//----------------------------------------------------------------------
CmdExecStatus
HistoryCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   int nPrint = -1;
   if (token.size()) {
      if (!myStr2Int(token, nPrint))
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   cmdMgr->printHistory(nPrint);

   return CMD_EXEC_DONE;
}

void
HistoryCmd::usage() const
{
   Msg(MSG_IFO) << "Usage: HIStory [(int nPrint)]" << endl;
}

void
HistoryCmd::help() const
{
   cout << setw(20) << left << "HIStory: " << "Print command history." << endl;
}


//----------------------------------------------------------------------
// DOfile <(string filename)>
//----------------------------------------------------------------------
CmdExecStatus
DofileCmd::exec(const string& option)
{     
   // check option 
   string token;
   if (!CmdExec::lexSingleOption(option, token, false))
      return CMD_EXEC_ERROR;
   if (!cmdMgr->openDofile(token)) {
      cmdMgr->closeDofile(); 
      return CmdExec::errorOption(CMD_OPT_FOPEN_FAIL, token);
   }
   return CMD_EXEC_DONE;
}

void
DofileCmd::usage() const
{  
  Msg(MSG_IFO) << "Usage: DOfile <(string filename)>" << endl;
}  
      
void
DofileCmd::help() const
{
   cout << setw(20) << left << "DOfile: " << "Execute the commands in the dofile." << endl;
}


//----------------------------------------------------------------------
// USAGE [| -All | -Time | -Memory]
//----------------------------------------------------------------------
CmdExecStatus
UsageCmd::exec(const string& option)
{     
   // check option 
   vector<string> options;
   CmdExec::lexOptions(option, options);

   bool repTime = false, repMem = false, repAll = false;
   size_t n = options.size();
   if (n == 0)
      repAll = true;
   else {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-All", token, 2) == 0) {
            if (repTime | repMem | repAll)
               return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            repAll = true; 
         }
         else if (myStrNCmp("-Time", token, 2) == 0) {
            if (repTime | repMem | repAll)
               return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            repTime = true; 
         }
         else if (myStrNCmp("-Memory", token, 2) == 0) {
            if (repTime | repMem | repAll)
               return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            repMem = true;
         }
         else
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
   }
   
   if (repAll) repTime = repMem = true;
   
   myUsage.report(repTime, repMem);

   return CMD_EXEC_DONE;
}

void
UsageCmd::usage() const
{  
   Msg(MSG_IFO) << "Usage: USAGE [| -All | -Time | -Memory]" << endl;
}  
      
void
UsageCmd::help() const
{
   cout << setw(20) << left << "USAGE: " << "Report the runtime and / or memory usage." << endl;
}

//----------------------------------------------------------------------
// SET LOgfile [| -All | -Cmd | -Error | -Warning | -Info | -Debug]
//             <(string filename)> [| -Fileonly | -Both] [-APPend]
//----------------------------------------------------------------------
CmdExecStatus
SLogFileCmd::exec(const string& option)
{
   // check option
   vector<string> options;
   CmdExec::lexOptions(option, options);

   bool all = false, cmd = false, error = false, warning = false, info = false, debug = false;
   bool fileonly = false, both = false;
   bool append = false;
   string filename = "";
   size_t n = options.size();

   if (n == 0) {
      all = true;
   }
   else {
      for (size_t i = 0; i < n; ++i) {
         const string& token = options[i];
         if (myStrNCmp("-APPend", token, 4) == 0) {
            if (append) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            append = true;
         }
         else if (myStrNCmp("-All", token, 2) == 0) {
            if (all || cmd || error || warning || info || debug) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            all = true;
         }
         else if (myStrNCmp("-Cmd", token, 2) == 0) {
            if (all || cmd || error || warning || info || debug) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            cmd = true;
         }
         else if (myStrNCmp("-Error", token, 2) == 0) {
            if (all || cmd || error || warning || info || debug) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            error = true;
         }
         else if (myStrNCmp("-Warning", token, 2) == 0) {
            if (all || cmd || error || warning || info || debug) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            warning = true;
         }
         else if (myStrNCmp("-Info", token, 2) == 0) {
            if (all || cmd || error || warning || info || debug) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            info = true;
         }
         else if (myStrNCmp("-Debug", token, 2) == 0) {
            if (all || cmd || error || warning || info || debug) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            debug = true;
         }
         else if (myStrNCmp("-Fileonly", token, 2) == 0) {
            if (fileonly || both) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            fileonly = true;
         }
         else if (myStrNCmp("-Both", token, 2) == 0) {
            if (fileonly || both) return CmdExec::errorOption(CMD_OPT_EXTRA, token);
            both = true;
         }
         else {
            if (filename == "") filename = token;
            else return CmdExec::errorOption(CMD_OPT_EXTRA, token);
         }
      }
   }

   if (filename == "") return CmdExec::errorOption(CMD_OPT_MISSING, "<(string filename)>");
   if (!(all || cmd || error || warning || info || debug)) all = true;
   if (!(fileonly || both)) fileonly = true;

   if (all) { 
      Msg.setAllOutFile(filename);
      Msg.startAllOutFile(append);
      if (fileonly) Msg.stopAllDefault();
      else Msg.startAllDefault();
   }
   else {
      CmdMsgType type;
      if (cmd) type = MSG_LOG;
      else if (error) type = MSG_ERR;
      else if (warning) type = MSG_WAR;
      else if (info) type = MSG_IFO;
      else if (debug) type = MSG_DBG;
      Msg.setOutFile(filename, type);
      Msg.startOutFile(type, append);
      if (fileonly) Msg.stopDefault(type);
      else if (type != MSG_LOG) Msg.startDefault(type);
   }
   return CMD_EXEC_DONE;
}

void
SLogFileCmd::usage() const
{
   Msg(MSG_IFO) << "Usage: SET LOgfile [| -All | -Cmd | -Error | -Warning | -Info | -Debug]" << endl;
   Msg(MSG_IFO) << "                   <(string filename)> [| -Fileonly | -Both] [-APPend]" << endl;
}

void
SLogFileCmd::help() const
{
   cout << setw(20) << left << "SET LOgfile: " << "Copy and/or redirect messages to assigned file." << endl;
}

#endif
