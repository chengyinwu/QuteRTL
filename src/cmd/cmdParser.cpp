/****************************************************************************
  FileName     [ cmdParser.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Command Line Parser ]
  Author       [ Chung-Yang (Ric) Huang, Cheng-Yin Wu, and Steggie Chen ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CMD_PARSER_SOURCE
#define CMD_PARSER_SOURCE

#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <assert.h>
#include "util.h"
#include "cmdParser.h"
#include "cmdMsgFile.h"

extern "C" {
#include <readline/readline.h>
#include <readline/history.h>
}

// Command Line Manager
extern CmdParser* cmdMgr;

// Command Line Warning Indicator
static void mybeep()
{
   cout << char(BEEP_CHAR);
}

// This Function Performs Operations on Corresponding Character Input
// Here's a sample for DELETE    = ( 27 91 51 126 )
//                     BACKSPACE = 127
ParseChar
CmdParser::getChar(istream& istr) const
{
   char ch = myGetChar(istr);

   if (istr.eof())
      return INPUT_END_CHAR;

   switch (ch) {
      case LINE_BEGIN_CHAR:
      case LINE_END_CHAR:
      case INPUT_END_CHAR:
      // case DELETE_CHAR:
      case TAB_CHAR:
      case NEWLINE_CHAR: return ParseChar(ch);
      case BACK_SPACE_KEY: return BACK_SPACE_CHAR;
      case ESC_CHAR: {
         char ch2 = myGetChar(istr);
         if (ch2 == char(FUNC_BASE_CHAR)) {
            char ch3 = myGetChar(istr);
            if ((ch3 >= char(FUNC_F1_CHAR)) && (ch3 <= char(FUNC_F4_CHAR)))
               return ParseChar(int(ch3) + FUNC_BASE_SHIFT);
            else
               return UNDEFINED_CHAR;
         }
         else if (ch2 == char(ARROW_BASE_CHAR)) {
            char ch3 = myGetChar(istr);
            if ((ch3 >= char(ARROW_UP_CHAR))&&(ch3 <= char(ARROW_LEFT_CHAR)))
               return ParseChar(int(ch3) + ARROW_BASE_SHIFT);
            else if (ch3 == char(DELETE_BASE_CHAR)) {
               if (myGetChar(istr) == char(DELETE_CHAR))
                  return ParseChar(DELETE_CHAR);
            }
            else
               return UNDEFINED_CHAR;
         }
         // else continue for "default"
      }
      default:
         if (isprint(ch)) return ParseChar(ch);
         else return UNDEFINED_CHAR;
   }

   return UNDEFINED_CHAR;
}

// This Function Register Single Command Interface to Command Manager
// Return false if registration fails
bool
CmdParser::regCmd(const string& cmd, unsigned nCmp, CmdExec* e) 
{
   // check if cmd contains space
   if (cmd.find(' ') != string::npos) return false;

   unsigned s = cmd.size();
   if (s < nCmp) return false;

   string str = cmd;
   for (unsigned i = 0; i < nCmp; ++i)
      str[i] = toupper(str[i]);
   string mandCmd = str.substr(0, nCmp);
   string optCmd = str.substr(nCmp);

   assert(e != 0);
   e->setCmdLen(1);
   e->setMandCmd(mandCmd);
   e->setOptCmd(optCmd);

   CmdExecSet::iterator it = _cmdLib.find(e->getCmdType());
   CmdExecSubSet* cmdSet = 0;
   if (it == _cmdLib.end()) {
      cmdSet = new CmdExecSubSet();
      _cmdLib.insert(make_pair(e->getCmdType(), cmdSet));
   }
   else cmdSet = it->second;
   cmdSet->insert(e);

   return true;
}

// This Function Register Tuple Command Interface to Command Manager
// Return false if registration fails
bool
CmdParser::regCmd(const string& cmd, unsigned nCmp1, unsigned nCmp2, CmdExec* e)
{
   size_t idx = cmd.find(' ');
   // check if cmd contains space
   if (idx == string::npos) return false;

   string str1 = cmd.substr(0, idx);
   string str2 = cmd.substr(idx + 1);

   // check if cmd2 contains space
   if (str2.find(' ', idx) != string::npos) return false;

   unsigned s1 = str1.size();
   unsigned s2 = str2.size();
   if ((s1 < nCmp1) || (s2 < nCmp2)) return false;

   for (unsigned i = 0; i < nCmp1; ++i)
      str1[i] = toupper(str1[i]);
   for (unsigned i = 0; i < nCmp2; ++i)
      str2[i] = toupper(str2[i]);

   string mandCmd1 = str1.substr(0, nCmp1);
   string optCmd1 = str1.substr(nCmp1);
   string mandCmd2 = str2.substr(0, nCmp2);
   string optCmd2 = str2.substr(nCmp2);

   assert(e != 0);
   e->setCmdLen(2);
   e->setMandCmd(mandCmd1);
   e->setMandCmd(mandCmd2);
   e->setOptCmd(optCmd1);
   e->setOptCmd(optCmd2);

   CmdExecSet::iterator it = _cmdLib.find(e->getCmdType());
   CmdExecSubSet* cmdSet = 0;
   if (it == _cmdLib.end()) {
      cmdSet = new CmdExecSubSet();
      _cmdLib.insert(make_pair(e->getCmdType(), cmdSet));
   }
   else cmdSet = it->second;
   cmdSet->insert(e);
   return true;
}

// Return false on "quit" or if excetion happens
CmdExecStatus
CmdParser::execOneCmd()
{
   bool newCmd = false;
   if (_dofile.is_open()) newCmd = readCmd(_dofile);
   else newCmd = readCmd(cin);

   // execute the command
   if (newCmd) {
      string option;
      CmdExec* e = parseCmd(option);
      if (e && (e->valid())) 
         return e->exec(option);
   }
   return CMD_EXEC_NOP;
}

// Return false on "quit" or if excetion happens
CmdExecStatus
CmdParser::execOneCmdWithTab()
{
   string option;
   bool newCmd;
   if (_dofile.is_open()) newCmd = readCmd(_dofile);
   else {
      //*_readBuf = NULL;
      char* cptr = NULL;
      if(NULL != cptr) free(cptr);
      cptr = readline(getPrompt().c_str());
      option = cptr;
      for (size_t i = 0; i < option.size(); ++i) _readBuf[i] = cptr[i];
      _readBufEnd = _readBuf + option.size();
      *(_readBufEnd) = 0;
      newCmd = addHistory();
   }
   
   if (newCmd) add_history(_history[_historySize - 1].c_str());
   else return CMD_EXEC_NOP;

   option = "";
   CmdExec* e = parseCmd(option);
   if (e && (e->valid())) 
      return e->exec(option);
   return CMD_EXEC_NOP;
}

void
CmdParser::printHelps(bool revealed) const
{
   CmdExecSet::const_iterator it;
   CmdExecSubSet::iterator is;
   Msg(MSG_IFO) << endl;
   for (it = _cmdLib.begin(); it != _cmdLib.end(); ++it) {
      if ((revealed) || (it->first != CMD_TYPE_REVEALED)) {
         Msg(MSG_IFO) << "========== " << CmdTypeString[it->first] << " Commands : ==========" << endl;
         for (is = it->second->begin(); is != it->second->end(); ++is) (*is)->help();
         Msg(MSG_IFO) << endl;
      }
   }
}

void
CmdParser::printHistory(int nPrint) const
{
   if (_historySize == 0) {
      Msg(MSG_IFO) << "Empty command history!!" << endl;
      return;
   }
   if ((nPrint < 0) || (nPrint > int(_historySize)))
      nPrint = _historySize;
   for (size_t i = _historySize - nPrint; i < _historySize; ++i)
      Msg(MSG_IFO) << "   " << i << ": " << _history[i] << endl;
}

bool
CmdParser::readCmd(istream& istr)
{
   _readBufPtr = _readBufEnd = _readBuf;
   *_readBufPtr = 0;

   printPrompt();

   bool newCmd = false;
   while (!newCmd) {
      ParseChar ch = getChar(istr);
      if (ch == INPUT_END_CHAR) {
         _dofile.clear();
         _dofile.seekg(0, ios::beg);
         _dofile.close();
         break;
      }
      switch (ch) {
         case LINE_BEGIN_CHAR: moveBufPtr(_readBuf); break;
         case LINE_END_CHAR:   moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_CHAR: if (moveBufPtr(_readBufPtr - 1))
                                  deleteChar();
                               break;
         case DELETE_CHAR:     deleteChar(); break;
         case TAB_CHAR:	       mybeep(); break;  // not supported yet
         case NEWLINE_CHAR:    newCmd = addHistory();
                               if (!_dofile.is_open() || newCmd)
                               {
                                  cout << char(NEWLINE_CHAR);
                                  if (!newCmd) printPrompt();
                               }
                               break;
         case FUNC_F1_CHAR:    mybeep(); break;  // not supported yet
         case FUNC_F2_CHAR:    mybeep(); break;  // not supported yet
         case FUNC_F3_CHAR:    mybeep(); break;  // not supported yet
         case FUNC_F4_CHAR:    mybeep(); break;  // not supported yet
         case ARROW_UP_CHAR:
              if (_historyIdx == 0) { mybeep(); break; }
              if (_historyIdx == _historySize)
                 _history[_historySize] = _readBuf;
              --_historyIdx;
              retrieveHistory();
              break;
         case ARROW_DOWN_CHAR:
              if (_historyIdx == _historySize) { mybeep(); break; }
              ++_historyIdx;
              retrieveHistory();
              break;
         case ARROW_RIGHT_CHAR:moveBufPtr(_readBufPtr + 1); break;
         case ARROW_LEFT_CHAR: moveBufPtr(_readBufPtr - 1); break;
         case UNDEFINED_CHAR:  mybeep(); break;
         default:  // printable character
            insertChar(ch); break;
      }
   }
   return newCmd;
}

CmdExec*
CmdParser::parseCmd(string& option)
{
   assert(_historySize > 0);
   string str = _history[_historySize - 1];
   assert(str[0] != 0 && str[0] != ' ');

   CmdExec* e = 0;
   string cmd;

   // normalize: keep only one space between two words
   unsigned delCount = 0;
   for (size_t i = 2, n = str.size(); i < n; ++i)
   {
      if ((str[i - 2] == ' ') && (str[i - 1] == ' '))
      {
         for (size_t j = i; j < n; ++j)
	    str[j - 1] = str[j];
         --i;
	 ++delCount;
      }
   }
   str.erase(str.length() - delCount);

   // space count: indicates how many words there are in cmd
   unsigned spCount = 0;
   for (size_t i = 0, n = str.size(); i < n; ++i)
      if (str[i] == ' ') ++spCount;

   // try to match commands
   size_t idx = 0;
   for (unsigned i = 0; (i < spCount + 1) && i < MAX_CMD_LEN; ++i)
   {
      idx = str.find(' ', idx + 1);
      cmd = str.substr(0, idx);

      e = getCmd(cmd);
      if (e) break;
   }


   // can't match any command
   if (!e)
   {
      // Chengyin added : Support linux commands
      string _cmd = "";
      if (str.size() >= 2) {
         // Support comments in dofile, NO execution
         if ((str[0] == '/') && (str[1] == '/')) return e;
      }
      for (idx = 0; idx < str.size(); ++idx) { if (str[idx] == ' ') break; }
      _cmd = str.substr(0, idx);
      if ((_cmd == "ls") || (_cmd == "vi") || (_cmd == "vim") || (_cmd == "echo") || (_cmd == "cat")) 
         system(str.c_str());
      else Msg(MSG_ERR) << "Illegal command!! (" << str << ")" << endl;
   }
   else if (idx != string::npos)
   {
      size_t opt = str.find_first_not_of(' ', idx);
      if (opt != string::npos)
         option = str.substr(opt);
   }

   return e;
}

// cmd is a copy of the original command
CmdExec*
CmdParser::getCmd(const string& cmd) const
{
   CmdExec* e = 0;

   // space count: indicates how many words there are in cmd
   unsigned spCount = 0;
   for (size_t i = 0, n = cmd.size(); i < n; ++i)
      if (cmd[i] == ' ') ++spCount;

   CmdExecSet::const_iterator it;
   CmdExecSubSet::iterator is;
   for (it = _cmdLib.begin(); it != _cmdLib.end(); ++it) {
      for (is = it->second->begin(); is != it->second->end(); ++is) {
         if (((*is)->getCmdLen() == spCount + 1) && (*is)->checkCmd(cmd)) {
            e = *is;
            break;
         }
      }
   }

   return e;
}

CmdExecSubSet
CmdParser::getCmdListFromPart(const string& cmd) const
{
   CmdExecSubSet result;

   // space count: indicates how many words there are in cmd
   unsigned spCount = 0;
   for (size_t i = 0, n = cmd.size(); i < n; ++i)
      if (cmd[i] == ' ') ++spCount;

   CmdExecSet::const_iterator it;
   CmdExecSubSet::iterator is;
   for (it = _cmdLib.begin(); it != _cmdLib.end(); ++it) {
      if (it->first != CMD_TYPE_REVEALED) {
         for (is = it->second->begin(); is != it->second->end(); ++is) {
            if (((*is)->getCmdLen() > spCount)) {
               bool check = true;
               for (unsigned i = 1; i <= spCount + 1; ++i) {
                  if (!(*is)->checkCmd(cmd, i)) {
                     check = false;
                     break;
                  }
               }
               if (check == true) result.insert(*is);
            }
         }
      }
   }

   return result;
}

bool
CmdParser::moveBufPtr(char* const ptr)
{
   if (ptr < _readBuf || ptr > _readBufEnd) {
      mybeep(); return false;
   }


   while (_readBufPtr > ptr) {
      cout << char(BACK_SPACE_CHAR);
      --_readBufPtr;
   }

   // move right
   while (_readBufPtr < ptr) {
      cout << *_readBufPtr;
      ++_readBufPtr;
   }

   return true;
}

bool
CmdParser::deleteChar()
{
   if (_readBufPtr == _readBufEnd) {
      mybeep(); return false;
   }
   char *tmp = _readBufPtr, *next = _readBufPtr + 1;
   for (; next != _readBufEnd; ++next) {
      *tmp = *next;
      tmp = next;
      cout << *next;
   }
   cout << " " << char(BACK_SPACE_CHAR);
   *(--_readBufEnd) = 0;
   tmp = _readBufPtr;
   _readBufPtr = _readBufEnd;
   moveBufPtr(tmp);
   return true;
}

void
CmdParser::insertChar(char ch)
{
   char* tmp = _readBufPtr;
   for (; tmp != _readBufEnd; ++tmp) {
      cout << ch;
      char ss = *tmp; *tmp = ch; ch = ss;
   }
   cout << ch;
   *_readBufEnd = ch; *(++_readBufEnd) = 0;
   tmp = _readBufPtr + 1;
   _readBufPtr = _readBufEnd;
   moveBufPtr(tmp);
}

void
CmdParser::deleteLine()
{
   moveBufPtr(_readBufEnd);
   for (; _readBufPtr != _readBuf; --_readBufPtr)
      cout << char(BACK_SPACE_CHAR) << " " << char(BACK_SPACE_CHAR);
   _readBufEnd = _readBufPtr;
   *_readBufEnd = 0;
}

// Return true if successfully added to history
// _readBuf[] will NOT be reset here; reset at the reading of next cmd
bool
CmdParser::addHistory()
{
   // remove ' ' at the end
   char* tmp = _readBufEnd - 1;
   while ((tmp >= _readBuf) && (*tmp == ' ')) *(tmp--) = 0;

   // remove ' ' in the beginning
   tmp = _readBuf;
   while (*tmp == ' ') ++tmp;

   // add to _history
   if (*tmp != 0) {
      _history[_historySize++] = tmp;
      _history[_historySize].clear();
      _historyIdx = _historySize;
      
      // add to LOG file
      Msg(MSG_LOG) << tmp << endl;
      return true;
   }
   return false;
}

void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}


//----------------------------------------------------------------------
//    Member Function for class CmdExec
//----------------------------------------------------------------------
bool
CmdExec::operator<(const CmdExec& rhs) const
{
   string lstr, rstr;
   for (size_t idx = 0; idx < _cmdLen; ++idx) 
   {
      if (idx != 0) lstr.append(" ");
      lstr.append(_mandCmd[idx]);
      lstr.append(_optCmd[idx]);
   }
   for (size_t idx = 0; idx < rhs._cmdLen; ++idx) 
   {
      if (idx != 0) rstr.append(" ");
      rstr.append(rhs._mandCmd[idx]);
      rstr.append(rhs._optCmd[idx]);
   }

   return (lstr < rstr);
}

// Return false if error options found
// "optional" = true if the option is optional XD
// "optional": default = true
//
bool
CmdExec::lexSingleOption
(const string& option, string& token, bool optional) const
{
   size_t n = myStrGetTok(option, token);
   if (!optional) {
      if (token.size() == 0) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
   }
   if (n != string::npos) {
      errorOption(CMD_OPT_EXTRA, option.substr(n));
      return false;
   }
   return true;
}

// if nOpts is specified (!= 0), the number of tokens must be exactly = nOpts
// Otherwise, return false.
//
bool
CmdExec::lexOptions
(const string& option, vector<string>& tokens, size_t nOpts) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n);
   }
   if (nOpts != 0) {
      if (tokens.size() < nOpts) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
      if (tokens.size() > nOpts) {
         errorOption(CMD_OPT_EXTRA, tokens[nOpts]);
         return false;
      }
   }
   return true;
}

CmdExecStatus
CmdExec::errorOption(CmdOptionError err, const string& opt) const
{
   switch (err) {
      case CMD_OPT_MISSING:
         if (opt.size()) Msg(MSG_ERR) << "Missing option" << " after (" << opt << ")";
         else Msg(MSG_ERR) << "Missing option" << "!!" << endl;
         break;
      case CMD_OPT_EXTRA:
         Msg(MSG_ERR) << "Extra option!! (" << opt << ")" << endl;
         break;
      case CMD_OPT_ILLEGAL:
         Msg(MSG_ERR) << "Illegal option!! (" << opt << ")" << endl;
         break;
      case CMD_OPT_FOPEN_FAIL:
         Msg(MSG_ERR) << "Error: cannot open file \"" << opt << "\"!!" << endl;
         break;
      default:
         Msg(MSG_ERR) << "Unknown option error type!! (" << err << ")" << endl;
         exit(-1);
   }
   return CMD_EXEC_ERROR;
}

bool
CmdExec::checkCmd(const string& check) const
{
   unsigned len = this->getCmdLen();
   bool result = false;
   size_t space = 0, nxt_space; 

   for (unsigned word = 1; word <= len; ++word)
   {
      nxt_space = check.find(' ', space + 1); 
      if (nxt_space == string::npos) nxt_space = check.size();

      for (unsigned i = space + 1; i <= nxt_space; ++i)
      {
         string checkMand = check.substr(space, i - space);
         string checkOpt = check.substr(i, nxt_space - space - i);

         if (checkMandCmd(checkMand, word) &&
             (checkOpt.empty() || checkOptCmd(checkOpt, word)))
         {
            result = true;
            break;
         }
      }
      if (result == false) return false;

      result = false;
      space = nxt_space + 1;
   }

   return true;
}

bool
CmdExec::checkCmd(const string& check, size_t idx) const
{
   if (idx >= this->getCmdLen()) return false;

   bool result = false;

   for (unsigned i = 1, n = check.size(); i <= n; ++i)
   {
      string checkMand = check.substr(0, i);
      string checkOpt = check.substr(i, n + 1 - i);

      if (checkMandCmd(checkMand, idx) &&
          (checkOpt.empty() || checkOptCmd(checkOpt, idx)))
      {
         result = true;
         break;
      }
   }

   return result;
}

// Check idx'th mand cmd
bool
CmdExec::checkMandCmd(const string& check, size_t idx) const
{
   if (check.size() != _mandCmd[idx - 1].size()) return false;
   for (unsigned i = 0, n = _mandCmd[idx - 1].size(); i < n; ++i) {
      if (!check[i]) return true;
      char ch1 = tolower(_mandCmd[idx - 1].at(i));
      char ch2 = tolower(check[i]);
      if (ch1 != ch2) return false;
   }
   return true;
}

// Check idx'th opt cmd
bool
CmdExec::checkOptCmd(const string& check, size_t idx) const
{
   if (check.size() > _optCmd[idx - 1].size()) return false;
   for (unsigned i = 0, n = _optCmd[idx - 1].size(); i < n; ++i) {
      if (!check[i]) return true;
      char ch1 = tolower(_optCmd[idx - 1].at(i));
      char ch2 = tolower(check[i]);
      if (ch1 != ch2) return false;
   }
   return true;
}

#endif
