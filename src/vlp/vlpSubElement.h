/****************************************************************************
  FileName     [ vlpSubElement.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Sub-Element class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_SUB_ELEMENT_H
#define VLP_SUB_ELEMENT_H

#include "vlpBase.h"
#include "vlpOpNode.h"
#include "vlpHierTree.h"
#include "vlpDefine.h"
#include "bst2.h"
#include "parser.h"
#include "vlpFSM.h"
#include "myString.h"
#include <iomanip>
#include <map>
#include <string>
#include <vector>

using namespace std;

class VlpIONode;
class VlpPortNode;
class VlpInstance;
class VlpMemAry;
class VlgModule;
class VlpPrimitive;
class VlpBaseNode;
class VlpSignalNode;
class VlpHierTreeNode;
class VlpFSMAssign;
class VlpFSMSIG;
class CktModuleCell;


class VlpInstance
{
friend class VlgModule;
public :
   VlpInstance();
   ~VlpInstance();
   void setNameMap();
   void setPosMap();
   void setModuleID(const int);
   void setName(const string&);
   void setRowNo(const int);
   void setColNo(const int);
   void setPosArr(VlpBaseNode* const);
   void setNameArr(const short);
   void setPOL(VlpBaseNode*&); //position mapping usage
   bool isNameMap() const;
   void printInst() const;
   void printParam() const;
   int getModuleID() const;
   int getPOLSize() const;
   const string& getName() const;
   VlpHierTreeNode* const genHierTN(const BaseModule* const) const;

   // genPOLAry(...) only handle position mapping parameter overload
   POLAry* const genPOLAry(const VlgModule* const, const VlpHierTreeNode* const&) const;
   void synthesis() const;

private :
   void handleConcatOut(CktModuleCell*&, unsigned&) const;
   void posMapping(CktModuleCell*&) const;
   void nameMapping(CktModuleCell*&) const;
   bool paramPropagation(const VlpHierTreeNode* const&, paramPair*&) const; // use in genPOLAry()
   bool checkPOL(VlpBaseNode*&, string&, const VlgModule* const&) const;
   bool genModName(string&) const;

   unsigned _isNameMap   :  1; // 0 -> position mapping; 1 -> name mapping
   unsigned _rowNo       : 16; // max row = 2048
   unsigned _colNo       : 15; // max col = 1024
   unsigned _moduleID;
   string _name;
   PArray _pArray; // use in position and name mapping 
   NArray _nArray; // use in name mapping only
   PosParamAry _polAry; // parameter overload : position mapping only
   //Note: typedef vector<VlpBaseNode*>       PosParamAry;
};

class VlpPrimitive
{
friend class VlgModule;
public :
   VlpPrimitive();
   ~VlpPrimitive();
   void setKindID(const int);
   void setNameID(const int);
   void setRowNo(const int);
   void setColNo(const int);
   void setPosArr(VlpBaseNode* const element);
   //const PArray* getPArray(void) const;
   //int getKind() const;
   void printPrim() const;
   void synthesis() const;
private :
   CktOutPin* genCellPrimitive(CktOutPin*, CktOutPin*, int) const;
   //default : 0, and : 1, nand : 2, or : 3, nor : 4, xor : 5, 
   //xnor : 6, buf : 7, bufif0 : 8, bufif1 : 9, not : 10
   unsigned _kind        :  4; // gate type 
   unsigned _rowNo       : 15; 
   unsigned _colNo       : 13;
   unsigned _nameID;
   PArray _pArray; // use in position and name mapping
};

class VlpPortNode//declare
{
public :
   VlpPortNode(VlpBaseNode*, VlpBaseNode*);
   ~VlpPortNode();
   void setIOType(const IOType);
   void setNetType(const NetType);
   void setDriveStr(const DriveStr, const DriveStr);
   void setRowNo(const int);
   void setColNo(const int);
   const VlpBaseNode* getMsb() const;
   const VlpBaseNode* getLsb() const;
   const int getIoType() const;
   const int getNetType() const;
   //void whoAmI(0) const;
   int port2Bus() const;
   CktIoType ioTypeVlp2Ckt() const;
   string NetType2Str() const;
private :
   VlpBaseNode* _msb;
   VlpBaseNode* _lsb;
   unsigned     _ioType      :  2;
   unsigned     _netType     :  4;
   unsigned     _drivStrFir  :  4;
   unsigned     _drivStrSec  :  4;
   unsigned     _rowNo       : 11;
   unsigned     _colNo       :  7;
/*     nettype                           drive_strength         ioType
     unspecified          0000 =  0        unspecified         unspecified
     wire                 0001 =  1        supply1             input
     reg(excecpt mem)     0010 =  2        strong1             output 
     wand                 0011 =  3        pull1               inout
     wor                  0100 =  4        weak1
     tri                  0101 =  5        highz1
     tri1                 0110 =  6        highz0
     tri0                 0111 =  7        weak0
     triand               1000 =  8        pull0
     trior                1001 =  9        strong0 
     supply0              1010 = 10        supply0
     supply1              1011 = 11
     swire                1100 = 12
*/
};

class VlpMemAry // reg[7:0] array[0:1023];
{
public :
   VlpMemAry(string, VlpBaseNode*, VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   string getName() const;
   int getBusId();
   bool getIndex(int&, int&);  // return true if _arrS > _arrE
   bool lintName(const string&);
   ~VlpMemAry();
private :
   string _name;
   VlpBaseNode* _msb;   // 7
   VlpBaseNode* _lsb;   // 0 
   VlpBaseNode* _arrS;  // 0
   VlpBaseNode* _arrE;  // 1023
};

class VlpTask
{
public :
   VlpTask(const string&);
   ~VlpTask();    
   void setIO(VlpSignalNode* const);
   void setStatement(VlpBaseNode*);
   void setPort(const string&, VlpPortNode*); 
   void setParam(const string&, VlpBaseNode* const);
   void setMemAry(VlpMemAry* const);
   bool getPort(const string&, VlpPortNode*&) const;
   bool lintPort(const string&) const;
   bool lintIO(const string&) const;
   bool lintParam(const string&, const paramPair*&) const;
   
private :
   int      _nameId;
   IOAry    _IOAry;    // only input, output, and inout
   ParamAry _paramArr; //array.h have a bug, still doesn't be fixed"
   PortMap  _portMap;  //var declared in module
   MemAry   _memList;  //mem declared in module --> change to map
   VlpBaseNode* _statement;
};

class VlpFunction
{
public :
   VlpFunction(bool, VlpSignalNode*);
   ~VlpFunction();
   void setInput(VlpSignalNode*);
   void setParam(const string&, VlpBaseNode* const);
   bool lintParam(const string&, const paramPair*&) const;
   bool lintPort(const string&) const;
   void setPort(const string&, VlpPortNode*); 
   void setMemAry(VlpMemAry* const);
   void setStatement(VlpBaseNode*);
   bool getPort(const string&, VlpPortNode*&) const;
   bool getPort(const int, VlpPortNode*&) const;
   const IOAry* getIoAry() const;
   const PortMap* getPortMap() const;

   const VlpSignalNode* outputNode() const;
//   vector< VlpFSMSIG > extractSIG( map< int, int >&,
   vector< VlpFSMAssign > extractAL( map<int, int>&,
                                     map<int, const VlpSignalNode*>& ) const;

   void synthesis() const;
private :
   bool           _isIntOutput;
   VlpSignalNode* _fOutput; //the name of function is output
   IOAry          _inputs;  //only inputs
   //because the parameter of function can't be overloaded, so substitute those parameter 
   //in statement directly.
   ParamAry       _paramArr;
   PortMap        _portMap; //var declared in module
   MemAry         _memList; //mem declared in module --> change to map
   VlpBaseNode*   _statement;
   struct FSMCallBack {
      bool               _isCase;
      const VlpBaseNode* _return;
      size_t             _cond;
      void reset() {
         _isCase = false;
         _return = NULL;
         _cond   = size_t( NULL );
      }

      FSMCallBack& operator = (const FSMCallBack& src) {
         _isCase = src._isCase;
         _return = src._return;
         _cond   = src._cond;
         return *this;
      }
   };

   void resolveIndex() const;
};

#endif



