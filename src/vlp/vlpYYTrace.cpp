/****************************************************************************
  FileName     [ vlpYYtrace.cpp ]
  Package      [ vlp ]
  Synopsis     [ Trace of parsing process for debugging ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_YY_TRACE_SOURCE
#define VLP_YY_TRACE_SOURCE

#include "VLGlobe.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//extern unsigned short lineNo;
extern LY_usage* LY;

ofstream outFile("Trace.txt");

void YYTRACE(string parseMsg)
{
   if (LY != 0)
      outFile << LY->lineNo << " : " << parseMsg << endl;
}

#endif
