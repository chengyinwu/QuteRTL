/****************************************************************************
  FileName     [ parser.cpp ]
  PackageName  [ vlp ]
  Synopsis     [ Implementation of vlp parser ]
  Author       [ Hu-Hsi Yeh, Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef PARSER_SOURCE
#define PARSER_SOURCE

#include <iostream>
#include <cassert>
#include "parser.h"

using namespace std;

//================================
//   For class Parser
//================================
//
// [NOTE] MUST call eatWhite(str) before calling this function!!!!!!!!!!!!
//
// "str" points to the beginning of a string token
//
// Return the address of the next string token
// 
// if the next string token cannot be found (*next == 0)
//    return 0
//
char*
Parser::getString(char* const str) const
{
   assert(!isWhite(*str));
   if (*str == 0)
      return 0;
   char* next = str;
   seekWhite(next);
   if (*next == 0)
      return 0;
   *next = 0;
   ++next;
   eatWhite(next);
   if (*next == 0)
      return 0;
   return next;
}


//================================
//   For class CmdParser
//================================
void
CmdParser1::readLine()
{
   for (int i = 0; i < NUM_TOKEN; ++i)
      _token[i] = 0;
   while (_token[0] == 0) {
      cout << _prompt;
      cin.getline(_buf, BUF_SIZE);
      _token[0] = _buf;
      eatWhite(_token[0]);
      if (*_token[0] == 0)
         _token[0] = 0;
   }
   // make a copy...
   _command = _token[0];
   
   for (int i = 1; i < NUM_TOKEN; ++i) {
      _token[i] = getString(_token[i - 1]);
      if (_token[i] == 0) break;
   }
}

// if command not match
//    return false
// if command match
//    if (arg not match)
//       output error msg
//    else if (catArg)
//       concatenate arguments
//    return true
bool
CmdParser1::checkCmd(const string& cmd0, const string& cmd1,
                    const string& cmd2, int numArg)
{
   _argError = false;
   string cmds[3];
   cmds[0] = cmd0;
   cmds[1] = cmd1;
   cmds[2] = cmd2;
   int i = 0;
   for (; i < 3; ++i) {
      if (cmds[i] == "") break;
      if (cmds[i] != _token[i])
         return false;
   }
   if (numArg > 0) {
      for (int j = 0; j < numArg; ++j, ++i) {
         if (j == NUM_TOKEN) break;
         if (!_token[i]) {
            ;//missingArg();
         }
      }
   }
   else if (numArg < 0) {  // variable number of arguments
      // concatenate the remaining arguments
      for (; i < (NUM_TOKEN - 1); ++i) {
         char* tmp = _token[i];
         seekWhite(tmp);
         *tmp = ' ';
      }
   }
   else {  // numArg == 0
      if (_token[i] != 0) {
         extraArg(_token[i]);
      }
   }
   return true;
}

bool
CmdParser1::checkCmd(const string& cmd0, const string& cmd1,
                    const string& cmd2, const string& cmd3, int numArg)
{
   _argError = false;
   string cmds[4];
   cmds[0] = cmd0;
   cmds[1] = cmd1;
   cmds[2] = cmd2;
   cmds[3] = cmd3;
   int i = 0;
   for (; i < 4; ++i) {
      if (cmds[i] == "") break;
      if (cmds[i] != _token[i])
         return false;
   }
   if (numArg > 0) {
      for (int j = 0; j < numArg; ++j, ++i) {
         if (j == NUM_TOKEN) break;
         if (!_token[i]) {
            missingArg();
         }
      }
   }
   else if (numArg < 0) {  // variable number of arguments
      // concatenate the remaining arguments
      for (; i < (NUM_TOKEN - 1); ++i) {
         char* tmp = _token[i];
         seekWhite(tmp);
         *tmp = ' ';
      }
   }
   else {  // numArg == 0
      if (_token[i] != 0) {
         extraArg(_token[i]);
      }
   }
   return true;
}


void
CmdParser1::errCmd()
{
   cerr << "Error: Illegal command \"" << _command << "\"..." << endl;
}


//================================
//   For class FileParser
//================================
bool
FileParser::openFile(const string& infile)
{
   _infile.close();
   _infile.clear();
   _infile.open(infile.c_str());
   if (!_infile)
      return false;
   _lineNo = 0;
   return true;
}

char*
FileParser::readLine()
{
   _infile.getline(_buf, BUF_SIZE);
   if (_infile.eof())
      return 0;
   ++_lineNo;
   char* ret = _buf;
   eatWhite(ret);
   if (*ret == 0)
      return readLine();
   return ret;
}

bool
FileParser::parseError(ParseError e, char* token)
{
   if (e == SYNTAX_ERROR)
      cerr << "Error: Syntax error \"" << token
           << "\" at line " << _lineNo << endl;
   else {
      assert(e == MISSING_ARG);
      cerr << "Error: Missing argument after \"" << token
           << "\" at line " << _lineNo << endl;
   }
   return false;
}

#endif
