/****************************************************************************
  FileName     [ VLGlobe.h ]
  PackageName  [ vlp syn ckt ]
  Synopsis     [ Define globe variables ]
  Author       [ Hu-Hsi (Louis) Yeh ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VL_GLOBE_H
#define VL_GLOBE_H

#include "vlpDesign.h"
#include "vlpStack.h"
#include "vlpModule.h"
#include "vlpDefine.h"
//#include "array.h"
#include "synVar.h"
#include "vlpEnum.h"

#include "myGraphAlg.h"

#include <string>
#include <stack>
#include <set>
#include <map>
#include <queue>


//===============temporary object,pointer for parsing=================//
class LY_usage
{
public:
   LY_usage();
   ~LY_usage();
//==============vlpLex.l==============//
   string yyid;
   unsigned short lineNo;
   unsigned short  colNo;

   bool defState;
   bool blackBox;
   bool vLibComment;
   bool modStart;
   bool isInclude;
   int defCount;

   stack<YY_BUFFER_STATE> LexBufStack;
   stack<FILE*>  FILEStack;
   stack<short>  LNStack;
   stack<short>  CNStack;
   stack<string> FileNameStack;//STL
/*no use STL
   Stack<YY_BUFFER_STATE> LexBufStack;
   Stack<FILE*>  FILEStack;
   Stack<short>  LNStack;
   Stack<short>  CNStack;
   Stack<string> FileNameStack;//STL   
*/

//=============vlpYacc.y==============//
   bool isInteger;
   bool allSensitive; //To suopport the syntax always@* and always@(*)
   enum ProcessState state;
   ScopeState        scopeState;
   IOType            tempIOType;
   NetType           tempNetType;
   DriveStr          hStr;
   DriveStr          lStr;
   short int         gateType;
   short int         UDM_No;
   int               mod;//use in debugging of parsing large design

   VlgModule*        modulePtr;    //pointer of the parsing module
   VlpFunction*      functionPtr;  //pointer of the parsing function
   VlpTask*          taskPtr;      //pointer of the parsing task
   VlpBaseNode*      alwaysEventPtr; //using in always descripation
   VlpSignalNode*    ioPtr;
   VlpBaseNode*      nodeHeadPtr1;//using in linking
   VlpBaseNode*      nodeTailPtr1;//for assignment_list
   VlpBaseNode*      nodeTailPtr2;//using in linking for ored_event_expression
   VlpBaseNode*      msbPtr;
   VlpBaseNode*      lsbPtr;

   string tempStr1;
   string tempStr2;
   string tempStr3;//use for " define :... "
   string tempStr4;//use for " parameter lvalue" and "defparam lvalue"
   string instName;//use for instance name in defparam and function_call
   stack<string> sigNames;//use in temp storage for signal name

   set<string>         nodeSet;//using in linking "always@* and always@(*)"
   PosParamAry         array;  //parameter pos-mapping overload
   DefineMap           defineMap;//file scope
   stack<VlpBaseNode*> stack_c;//case link(recursive structure is ok)
   stack<VlpBaseNode*> stack_e;//expression link(recursive structure is ok)
   stack<VlpBaseNode*> stack_s;//seq_block link(recursive structure is ok)
   stack<bool>         stack_d;//synopsys directive parallel case, full case
/*no use STL
   Stack<VlpBaseNode*> stack_c;//case link(recursive structure is ok)
   Stack<VlpBaseNode*> stack_e;//expression link(recursive structure is ok)
   Stack<VlpBaseNode*> stack_s;//seq_block link(recursive structure is ok)
   Stack<bool>         stack_d;//synopsys directive parallel case, full case
*/
};
//==================================================================//
class LibLY_usage
{
public:
   LibLY_usage();
   ~LibLY_usage();
//================vlpLibLex.y=================//
   string libid;
   unsigned short lineNoLib;
   unsigned short colNoLib;
   short counter;
   StateLib stateLib;
//===============vlpLibYacc.y=================//
   int indexNum;
   LibModule* modulePtrLib;
   string inName;
   string inName1;
   string outName;
   int ioType;
   vector<string> strVector;
   string pin;
};

class SYN_usage
{
public:
   SYN_usage();
   ~SYN_usage();

   void insertCktCell(CktCell*);
   //void modReset();

   //SynNodeSharingList  outPinList;//store all regs, wires...
   SynSharNodeMap      outPinMap;//store all regs, wires...
   SynVarList          synVarList;
   SynConstAry         constAry;
   ParamAry*           curPOLAry;

   vector<SynCondState*>          synCondStack;
   vector<SynForArgu>             synForArguArray;
   vector<vector<synFuncArgu*>*>  synFuncArguArray;
   vector<CktCell*>               cktCellAry;
   vector<CktOutPin*>             loopMuxFinAry;
   PinAry                         redundantPins;

   string newModuleNameMark;
   string wireNamePrefix;

   bool   isHasNullBranch;
   bool   isHasTriStateBuf;
   int    muxOutBusId;
};

// New Added Class for Map
class CKT_usage
{
public:
   CKT_usage();
   ~CKT_usage();

   string moduleNamePrefix;
   stack<SynSharNodeMap> outPinMap;//store all regs, wires...
};


#endif


