/****************************************************************************
  FileName     [ cktDefine.h ]
  Package      [ ckt ]
  Synopsis     [ Type define of ckt package ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_DEFINE_H
#define CKT_DEFINE_H

#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

class CktOutPin;
class CktInPin;
class IOPinConn;
class CktCell;

struct CktLibInPin;
struct CktLibOutPin;

typedef string                CktString;
typedef vector<CktOutPin*>    CktOutPinAry;
typedef vector<CktLibInPin*>  CktLibInPinAry;
typedef vector<CktLibOutPin*> CktLibOutPinAry;

typedef map<string, IOPinConn*>    CktHierIoMap;

typedef map<const CktOutPin* const, CktOutPin*>  OutPinMap;
typedef map<const CktInPin*  const, CktInPin* >  InPinMap;
typedef vector<CktInPin*>                        CktInPinAry;
typedef vector<CktOutPin*>                       CktOutPinAry;
typedef vector<unsigned short>                   BusIdArray;

typedef vector<CktCell*>                         CktCellAry;
typedef map<const CktCell* const, CktCell*>      CellMap;

typedef map<CktOutPin*, int>                     PinLitMap;
typedef set<const CktInPin*>                     CktInPinSet;
typedef map<const CktCell*, CktInPinSet>         CktDpMap;

#endif

