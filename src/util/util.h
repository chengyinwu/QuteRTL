#ifndef UTIL_H
#define UTIL_H

#include <istream>
#include <sstream>
#include "rnGen.h"
#include "myUsage.h"

using namespace std;

// Extern global variable defined in util.cpp
extern RandomNumGen  rnGen;
extern MyUsage       myUsage;

// In myString.cpp
extern int myStrNCmp(const string& s1, const string& s2, unsigned n);
extern size_t myStrGetTok(const string& str, string& tok, size_t pos = 0,
                          const char del = ' ');
extern bool myStr2Int(const string& str, int& num);
extern bool isValidVarName(const string& str);

inline string myInt2Str(const int num) {
   // Int to string conversion
   string str = "";
   stringstream ss(str);
   ss << num;
   return ss.str();
}

// In myGetChar.cpp
extern char myGetChar(istream&);
extern char myGetChar();


#endif // UTIL_H
