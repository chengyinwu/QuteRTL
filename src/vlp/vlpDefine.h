/****************************************************************************
  FileName     [ vlpDefine.h ]
  Package      [ vlp ]
  Synopsis     [ Type define of vlp package ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_DEFINE_H
#define VLP_DEFINE_H

#include "bst2.h"
//#include "array.h"
#include <vector>
#include <string>
#include <map>

#include "cmdMsgFile.h"

const string TAB_INDENT = "   ";  // 3 spaces
const unsigned buffer_size = 4096;

class VlpBaseNode;
class VlpPortNode;
class VlpPrimitive;
class VlpInstance;
class VlpMemAry;
class VlpTask;
class VlpFunction;
class BaseModule;
class LibModule;
class VlgModule;
class VlpHierTreeNode;
class VlpSignalNode;
class CktModule;
class IOPinConn;
class SynBus;

struct paramPair;

//parameter is a constant(3, 3'b010..), can't be lvalue, ex a[3:0], a.. are not allowed     

typedef struct yy_buffer_state*    YY_BUFFER_STATE;

//================using STL vector========================//

typedef vector<VlpPrimitive*>    PrimitiveAry;
typedef vector<VlpInstance*>     InstanceAry;
typedef vector<VlpMemAry*>       MemAry;
typedef vector<VlgModule*>       VlgModuleAry;
typedef vector<LibModule*>       LibModuleAry;
typedef vector<CktModule*>       CktModuleAry;
typedef vector<VlpBaseNode*>     CDFGAry;
typedef vector<const paramPair*> POLAry;
typedef vector<VlpHierTreeNode*> ChildAry;
typedef vector<VlpBaseNode*>     PArray;
typedef vector<short>            NArray;
typedef vector<VlpBaseNode*>     PosParamAry;
typedef vector<VlpSignalNode*>   IOAry;
typedef vector<const paramPair*> ParamAry;
typedef vector<SynBus*>          BusAry;
typedef map<string, POLAry*>     POLMap;

//========================================================//
/*
typedef Array<VlpPrimitive*>       PrimitiveAry;
typedef Array<VlpInstance*>        InstanceAry;
typedef Array<VlpMemAry*>          MemAry;
typedef Array<VlgModule*>          VlgModuleAry;
typedef Array<VlpBaseNode*>        CDFGAry;
typedef Array<const paramPair*>    POLAry;
typedef Array<VlpHierTreeNode*>    ChildAry;
typedef Array<VlpBaseNode*>        PArray;
typedef Array<short>               NArray;
typedef Array<VlpBaseNode*>        PosParamAry;
typedef Array<VlpSignalNode*>      IOAry;
typedef Array<const paramPair*>    ParamAry;
typedef bst2<POLAry*>              POLMap;
*/
//========================================================//
typedef bst2<string, const VlpBaseNode*> DefineMap;
typedef bst2<unsigned, VlpPortNode*>     PortMap;
typedef bst2<unsigned, VlpTask*>         TaskMap;
typedef bst2<unsigned, VlpFunction*>     FunctionMap;
typedef bst2<unsigned, BaseModule*>      ModuleMap;
typedef bst2<string, POLAry>             MapArray;/*if (position mapping overload)
                                                    PosParamAry in class VlpInatance
                                                 else (name mapping overload)
                                                    MapArray in class VlgModule  */
typedef map<string, IOPinConn*> CktHierIoMap;

//ckt
typedef vector<paramPair> ParamPairAry;
//Debugger
typedef map<string, vector<string> >            TraceMap;

#endif


