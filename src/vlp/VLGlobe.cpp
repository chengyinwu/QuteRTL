/****************************************************************************
  FileName     [ VLGlobe.cpp ]
  PackageName  [ vlp syn ckt ]
  Synopsis     [ Define globe variables ]
  Author       [ Hu-Hsi (Louis) Yeh ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VL_GLOBE_SOURCE
#define VL_GLOBE_SOURCE

#include "VLGlobe.h"
#include "vlpDesign.h"
#include "vlpStack.h"
#include "vlpModule.h"
#include "vlpDefine.h"
//#include "array.h"
#include "synVar.h"
#include "vlpEnum.h"
#include "y.tab.h"

#include <string>
#include <stack>
#include <set>
#include <map>
#include <queue>

LY_usage* LY = NULL;
LibLY_usage* LibLY = NULL;
SYN_usage* SYN = NULL;

// New Added Class for Map
CKT_usage* CREMap = NULL;

//syn
ParamAry*            curParamOverload;

CktModule*           curCktModule;
const VlgModule*     curVlpModule;

int netNumber;
int instNumber;
unsigned globalPrintLevel;

//ckt usage
queue<CktCell*> cellQueue;
CktHierIoMap hierIoMap;
//constraint generate
int netNum2 = 0;


LY_usage::LY_usage()
{
   lineNo   = 1; 
   colNo    = 1;
   defCount = 0;
   gateType = 0;
   UDM_No   = -1;
   mod      = 0;//use in debugging of parsing large design

   defState     = false;
   blackBox     = false;
   vLibComment  = false;
   modStart     = false;
   isInclude    = false;
   isInteger    = false;
   allSensitive = false; //To suopport the syntax always@* and always@(*)

   tempIOType  = unspecIO;
   tempNetType = unspecNet;
   hStr        = unspecStr;
   lStr        = unspecStr;

   ioPtr        = NULL;
   msbPtr       = NULL;
   lsbPtr       = NULL;
   nodeHeadPtr1 = NULL;//using in linking
   nodeTailPtr1 = NULL;//for assignment_list

   tempStr1   = "";
   tempStr2   = "";
   tempStr3   = "";//use for " define :... "
   tempStr4   = "";//use for " parameter lvalue" and "defparam lvalue"
   instName   = "";//use for instance name in defparam and function_call
}

LY_usage::~LY_usage()
{
}

LibLY_usage::LibLY_usage()
{
//================vlpLibLex.y=================//
   lineNoLib = 1;
   colNoLib  = 1;
   counter   = 0;

//===============vlpLibYacc.y=================//
   indexNum = 0;
   ioType   = 0;
   inName   = "";
   inName1  = "";
   outName  = "";
}

LibLY_usage::~LibLY_usage()
{
}

SYN_usage::SYN_usage()
{
   newModuleNameMark = "$";
   wireNamePrefix    = "nz";
   curPOLAry         = NULL;
   isHasNullBranch   = false;
   isHasTriStateBuf  = false;
   muxOutBusId       = -1;
}

SYN_usage::~SYN_usage()
{
}

void 
SYN_usage::insertCktCell(CktCell* cell)
{
   cktCellAry.push_back(cell);
}

/*void
SYN_usage::modReset()
{
   outPinMap;//store all regs, wires...
   SynVarList;
   constAry;
   curPOLAry;

   synCondStack;
   synForArguArray;
   synFuncArguArray;
   cktCellAry;
   latchCtrList;
   isHasNullBranch;
}*/

CKT_usage::CKT_usage() 
{
   moduleNamePrefix = "Ce";
}
   
CKT_usage::~CKT_usage() 
{
}

#endif
