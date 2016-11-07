/****************************************************************************
  FileName     [ parser.h ]
  PackageName  [ vlp ]
  Synopsis     [ Definition to vlp parser ]
  Author       [ Hu-Hsi Yeh, Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum ParseError
{
   SYNTAX_ERROR = 0,
   MISSING_ARG  = 1
};

class Parser
{
   #define BUF_SIZE 65536

public:
   Parser() {}

   char* getString(char* const str) const;

protected:
   char            _buf[BUF_SIZE];

   static bool isWhite(char ch) {
      return (ch == ' ' || ch == '\t' || ch == '\n'); }
   static void eatWhite(char*& str) {
      while (isWhite(*str) && *(++str) != 0); }
   static void seekWhite(char*& next) {
      while (!isWhite(*next) && *next != 0) ++next; }
};

class CmdParser1 : public Parser
{
   #define NUM_TOKEN    8

public:
   CmdParser1(const string& p) : _prompt(p) {}
   void readLine();
   bool checkCmd(const string&, const string&, const string&, int numArg=0);
   bool checkCmd(const string&, const string&, const string&, const string&, int numArg=0);
   char* getToken(int i) const { return ((i < NUM_TOKEN)? _token[i] : 0); }
   bool isArgError() const { return _argError; }
   void errCmd();

private:
   const string    _prompt;
   string          _command;
   char*           _token[NUM_TOKEN];
   bool            _argError;

   void missingArg() { _argError = true;
      cerr << "Error: missing argument!!" << endl;
   }
   void extraArg(const string& arg) { _argError = true;
      cerr << "Error: extra argument \"" << arg << "\"!!" << endl;
   }
};

class FileParser : public Parser
{
public:
   bool openFile(const string& infile);
   char* readLine();
   void resetLineNo() { _lineNo = 0; }
   void rewind() {
      _infile.clear(); _infile.seekg(0, ios::beg); _lineNo = 0; }
   const size_t getLineNo()const {return _lineNo ; }
   bool parseError(ParseError e, char* msg);

private:
   ifstream        _infile;
   size_t          _lineNo;
};

#endif // PARSER_H
