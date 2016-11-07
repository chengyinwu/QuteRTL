/****************************************************************************
  FileName     [ synVar.h ]
  Package      [ syn ]
  Synopsis     [ Intermediate data structure for synthesis ]
  Author       [ Chun-Fu(Joe) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_VAR_H
#define SYN_VAR_H

//---------------------------
//  system include
//---------------------------
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;
//---------------------------
//  user include
//---------------------------
#include "cktCell.h"
#include "cktModelCell.h"

#include "vlpBase.h"
#include "vlpCtrlNode.h"
#include "vlpOpNode.h"

#include "bv4.h"

//---------------------------
//  Type definitions
//---------------------------

class SynBus;
class CktModule;
class SynCond;
class CktOutPin;

//---------------------------
//  Global variables
//---------------------------

//================ class PinAry ===============//

class PinAry // And each node in the list
{
public:
   PinAry();
   ~PinAry();
   void insert(CktOutPin*, bool = false);
   void chaState(bool);
   void pop_back();

   CktOutPin* synAndGate();
   CktOutPin* synOrGate();
   CktOutPin* synAnd2Gate();
   CktOutPin* synOr2Gate();
   //CktOutPin* synLatch(CktOutPin*);
   CktOutPin* synMergeCell(bool);
   CktOutPin* synTriStateBuf(CktOutPin*);

   CktOutPin* operator[](const unsigned&);
   void clear();
   unsigned size() const;
   bool isExist(CktOutPin*) const;
private:
   CktOutPin* synAnd();
   CktOutPin* synAnd2();
   CktOutPin* synOr();
   CktOutPin* synOr2();
   CktOutPin* synInv(CktOutPin*);
   
   vector<CktOutPin*> _list;
   vector<bool>       _isPosList;
};

//============= class SynCondState =============//
/* eg.
      always @(state)         data in SynCondState
      begin                   _ctrlCond:   ==
         if(state==2'b00)                 /   \
            a=0;                        state  2'b00
         else                 _state: true or false
            a=1;              _valueExp: "",  null string
      end
      
      always @(state)         data in SynCondState
      begin                   _ctrlCond: 2'b00
         case(state)          _state: although don't use here, still has false by initialization
            2'b00:begin       _valueExp: "2'b00"          
                     a=1;    
                  end       
      end

      So, if I want to know the condition is "if-else" or "case"
      to check _valueExp will tell you answer.      */

class SynCondState //store conditional operator of design, then translate relationship into SynVar and SynCond
{
public:
   SynCondState(CktOutPin*, bool);
   SynCondState();
   ~SynCondState();
   void setCtrlCond(CktOutPin*);
   void setState(bool);
   CktOutPin* getCtrlCond() const;
   bool getState() const;
   void setCtrlNode(const VlpBaseNode*);
   const VlpBaseNode* getCtrlNode() const;
   
private:
   CktOutPin* _ctrlCond;
   bool _state; //use for "if", state indicate which child(true or false) to store
   //string _valueExp; //for use in case
   const VlpBaseNode* _ctrlNode; // for cinstraint generation
};

//============== class SynVar ==============//
// old note : just for references
/* SynVar note :
   `define  IDLE         1
   `define  WRITE        2  
   `define  READ         3  
   
   reg [1:3] cs;
   reg [1:3] ns;

   always@(cs)
   begin
      ns = 3'b000;
      case(1'b1)
      cs[`IDLE]:  ns[`WRITE] = 1;
      cs[`WRITE]: ns[`READ]  = 1;
      cs[`READ]:  ns[`IDLE]  = 1;
   end
   
  SynVar-- _name  = ns      -->SynVar-- _name  = ns
         | _msb   = NULL    |         | _msb   -> 1 
         | _lsb   = NULL    |         | _lsb   -> 1 
         | _value = 3'b000  |         | _value = NULL
         | _cond  = 0       |         | _cond --------->--  _value
         --_next------------|         --_next-           |  _ctrlCond: cs[1]==1'b1
                                                         |  _trueChild ------>-  _value= 1
                                                         -- _falseChild       |  _ctrlCond= NULL
                                                                              |  _trueChild= NULL
                                                                              -- _falseChild= NULL 
   SynCond Note :
   eg.
      b=2;         SynVar-- _cond ------> -- _value
      if(state)          |  _name="b"     |  _ctrlCond: state
         b=0;            -- _value= 2     |  _trueChild ------>-- _value      = 0
      else                                -- _falseChild       |  _ctrlCond   = NULL
         b=1;                                   |              |  _trueChild  = NULL
                                                |              -- _falseChild = NULL
                                                |
                                                |--> -- _value      = 1
                                                     |  _ctrlCond   = NULL
                                                     |  _trueChild  = NULL
                                                     -- _falseChild = NULL           
   eg.
      b=2;              b=2;                                b=2;
                       -----------
      if(state)       | if(state) |                         if(otherState)
         b=0;         |    b=0;   |                            b=3;
      else            | else      |    should convert to   ----------------
         b=1;         |    b=1;   |    -----------------> | else if(state) |
                       -----------                        |    b=0;        | <--- jojoman call this part as _lastValue
      if(otherState)    if(otherState)                    | else           |
         b=3;              b=3;                           |    b=1;        |
                                                           ----------------
      original data                                                                               
      b=2;         SynVar-- _cond ------> -- _value
      if(state)          |  _name  = "b"  |  _ctrlCond: state
         b=0;            -- _value = 2    |  _trueChild ------>-- _value      = 0
      else                                -- _falseChild---    |  _ctrlCond   = NULL
         b=1;                                             |    |  _trueChild  = NULL
                                                          |    -- _falseChild = NULL
                                                          |
                                                          |--->-- _value      = 1
                                                               |  _ctrlCond   = NULL
                                                               |  _trueChild  = NULL
                                                               -- _falseChild = NULL           

                                         |-------------------------------------------|
     data after modified                 |                                           |
      b=2;         synvar-- _cond ------>|-- _value                                  |
      if(state)          |  _name  = "b" ||  _ctrlCond: state                        |
         b=0;            -- _value = 2   ||  _trueChild ------>-- _value      = 0    |
      else                               |-- _falseChild---    |  _ctrlCond   = NULL |
         b=1;                            |                |    |  _trueChild  = NULL |
      if(otherState)                     |                |    -- _falseChild = NULL |
         b=3;                            |                |                          |  <--- this part will be _lastValue
                                         |                |--->-- _value      = 1    |
                                         |                     |  _ctrlCond   = NULL |
                                         |                     |  _trueChild  = NULL |
                                         |                     -- _falseChild = NULL |
                                         |                                           |
                                         |-------------------------------------------|
                                      ||
                                      ||
                                     \||/
                                      \/

       synvar-- _cond ------>  -- _value                                
             |  _name="b"      |  _ctrlCond: otherState  
             -- _value= 2      |  _trueChild --------> - _value= 3        
                               |  _falseChild= NULL    |   _ctrlCond   = NULL   
                               -- _lastValue           |   _trueChild  = NULL 
                                     |                 - - _falseChild = NULL
                                     |
                                     |--> |-------------------------------------------|
                                          |                                           |
                                          |-- _value                                  |
                                          ||  _ctrlCond: state                        |
                                          ||  _trueChild ------>-- _value      = 0    |
                                          |-- _falseChild---    |  _ctrlCond   = NULL |
                                          |                |    |  _trueChild  = NULL |
                                          |                |    -- _falseChild = NULL |
                                          |                |                          |
                                          |                |--->-- _value      = 1    |
                                          |                     |  _ctrlCond   = NULL |
                                          |                     |  _trueChild  = NULL |
                                          |                     -- _falseChild = NULL |
                                          |                                           |
                                          |-------------------------------------------|
    eg.
       case(state)            synvar-- _cond ------>  -- _value                                
          1'b0: begin               |  _name="a"      |  _ctrlCond: state == 1'b0
                   a=1;             -- _value= NULL   |  _trueChild --------> -- _value      = 1        
                end                                   |  _falseChild          |  _ctrlCond   = NULL   
          1'b1: begin                                 --    |                 |  _trueChild  = NULL 
                   a=2;                                     |                 -- _falseChild = NULL
                end                                         |-> _value= 2       
       endcase                                              |   _ctrlCond   = NULL   
                                                            |   _trueChild  = NULL 
                                                            |-  _falseChild = NULL
*/

struct finPair 
{
   finPair(CktOutPin*, SynCond*, VlpBaseNode*);
   finPair(const finPair&);
   ~finPair();
   void cha2Cond();
   void setCondFin(CktOutPin*, VlpBaseNode*);
   CktOutPin* synLatch(CktOutPin*) const;
   CktOutPin* synLatchCtrl() const;
   CktOutPin* synTriStateBuf(CktOutPin*) const;
   CktOutPin* synTsBufCtrl() const;
   bool isSameCond(const finPair&) const;
   bool isSameCondRoot(const finPair&) const;
   int synMergeFin(bool, bool);
   //void synMuxFin(finPair&);
   void synPartSelFin(vector<int>&, vector<finPair>&, unsigned&);
   bool operator < (const finPair&) const;
   void print() const;
   //_finPin, _cond : Only one of them will be used. when exist _cond, 
   // original _finPin should be assign to _cond->_finPin
   // after synMergeFin(), store mergeCell finPin in _finPin;
   unsigned   _busId;
   CktOutPin* _finPin;
   SynCond*   _cond;   // null: no conditional assignment, 
                       // otherwise has "if-else" or "case" condition
   bool       _isValid;
   VlpBaseNode* _seNode;   // for constraint generation
};

class SynCond //store conditional value in SynVar
{
public:
   SynCond();
   SynCond(CktOutPin*, VlpBaseNode*);
   ~SynCond();

   void setFinPin(CktOutPin*);
   void setCtrlCond(CktOutPin*);
   void setSubCond(bool, SynCond*);

   CktOutPin* getCtrlCond() const;
   CktOutPin* getFinPin() const;
   CktOutPin* synthesis(CktOutPin*, bool); 
   SynCond* getSubCond(bool) const;
   void synNullCond(PinAry&, PinAry&, CktOutPin*) const;
   void synZCond(PinAry&, PinAry&, CktOutPin*) const;
   void addFin(vector<int>&, vector<finPair>&);
   void addFin(vector<SynCond*>&);
   void mergeCondTree(vector<SynCond*>&);
   void setSpDefPin(CktOutPin*, unsigned);
   SynCond* createCond(SynCond*);
   void RecursiveSetSpDefPin(SynCond*, unsigned);

   SynCond& operator = (const SynCond&);
   bool operator == (const SynCond&) const;
   void print(int);
   void print(int, SynCond*);  // Chengyin debug
   //void synPartSelectFin(CktOutPin*);
   void writeCtrlSig(ofstream&, set<string>&) const;
   void writeCtrlSrc(string&, ofstream&, set<string>&) const;
   void setCtrlNode(const VlpBaseNode*);
   void setSeNode(const VlpBaseNode*);
   const VlpBaseNode* getCtrlNode() const;
   const VlpBaseNode* getSeNode() const;
   
   CktOutPin* writeCDFG(ofstream&, const unsigned&) const;
private:
   CktOutPin* synMux(CktOutPin*, CktOutPin*);
   SynCond* setSpDefCond(CktOutPin*, unsigned);
   // when both _finPin and {_trueCond or _falseCond or both} 
   // are used, the _finPin is top default assignment
   CktOutPin* _ctrlCond;   //condition expression
   CktOutPin* _finPin;     //value of variable
   SynCond* _trueCond;  //true  cond of "if-esle"
   SynCond* _falseCond; //false cond of "if-esle"
   const VlpBaseNode* _ctrlNode; // for constraint generation
   const VlpBaseNode* _seNode;   // for constraint generation
};

class SynSharNodeMap;
class SynVarList;
class SynVar //flatten all variable in SynVar data structure
{
public:
   SynVar();
   SynVar(string&, unsigned);
   ~SynVar();
   void setName(const string&);
   void setBusId(int);
   void setFuncOrTask();
   void setParallCase();
   void setState(bool, bool, CktOutPin*);

   void setFinInfo(CktOutPin*, VlpBaseNode*);

   string getName() const;
   int    getBusId() const;
   int    getIndex() const;
   void   setIndex(int);

   bool isFuncOrTask() const;
   bool isParallCase() const;
   bool isSeq() const;
   bool isSync() const;

   void resetFuncOrTask();
   CktOutPin* connLhsRhs(bool);
   SynCond* copyCond(SynCond*);
   void writeVarCst(ofstream&, set<string>&) const;
   void writeFunc(ofstream&, set<string>&) const;

   void print() const;
   
   bool writeCDFG(const string, set<unsigned>&) const;

private:

   static int my_comp(const finPair& a, const finPair& b) { 
      return a < b; 
   }

   bool isBusOverLap() const;
   void combSameCond();
   void separateCond();
   unsigned sortFin(vector<int>&);
   void checkFullSp(SynCond*);
   CktOutPin* connCombFin(PinAry&, bool&);
   CktOutPin* connSeqFin(bool&);
   CktOutPin* synComb(bool&);
   int findSameCondRoot(unsigned) const;
   void connAsynRst(CktDffCell*);
//=================== class member ======================//
   string   _name;  // variable name
   unsigned _busId; // variable total bus
   vector<finPair> _finOutPins; // different condRoot;

   bool _isFuncOrTask;
   bool _isSeq;
   bool _isSync;
   bool _isParallCase;   // [toSupport] need another handle
   CktOutPin* _clk;
   //_next : for same variable, but (_isSeq, _isSync, _isParallelCase, _clk) is different.
   SynVar* _next;  // still no support!
};

class SynMemory
{
public:
   SynMemory(VlpMemAry*);
   ~SynMemory();

   string getName() const;
   CktOutPin* genMemOutPin(float&);
   SynVar* genDinVar();
   SynVar* genMemVar(int);
   void synWrAddress(const VlpBaseNode*);
   void synRdAddress(const VlpBaseNode*);
   void setState(bool, CktOutPin*);
   void connMemInOut();
private:
   bool _isReadVariable;
   bool _isReadConst;
   bool _isWriteVariable;
   bool _isWriteConst;
   bool _isSync;

   string _name;           // mem variable name
   unsigned _busId;
   int _szS;
   int _szE;

   // Write Memory
   map<int, SynVar*> _varMap; // const index;
   SynVar*    _dinVar;     // don't insert the pin to SynVarList::_list
   
   // Read Memory
   map<int, CktOutPin*> _pinMap; // don't insert those pins to SynVarList::_list
   CktOutPin* _dout;             // don't insert the pin to SynSharMap
   
   CktOutPin* _clk;
   CktOutPin* _wrAddress;
   CktOutPin* _rdAddress;
};

class SynVarList
{
public:
   SynVarList();
   ~SynVarList();
   unsigned size() const;
   void clear();
   SynVar* operator[](const unsigned&) const;
   SynVar* operator[](const string&) const;
   bool insertVar(string&, unsigned);
   bool insertSynMem(VlpMemAry*);
   bool setVarFin(string&, CktOutPin*, VlpBaseNode*);
   bool setMemFin(string&, CktOutPin*, VlpBaseNode*, float = 0.5, const VlpBaseNode* = NULL);
   CktOutPin* getMemOutPin(string&, float, VlpBaseNode* = NULL) const;
   void setMemState(bool, CktOutPin*);
   SynVarList& operator =(const SynVarList&);
   void connLhsRhs();
private:
   vector<SynVar*>    _list;
   vector<SynMemory*> _memlist;
};

class SynSharNodeMap
{
public:
   SynSharNodeMap();
   ~SynSharNodeMap();
   //for operator node
   void insert(CktCellType, CktOutPin*, CktOutPin*, CktOutPin*);
   CktOutPin* getSharPin(CktCellType, CktOutPin*, CktOutPin*) const;
   //for variable
   void insert(const string&, CktOutPin*);
   void insert(string, CktOutPin*, int);
   void replace(CktOutPin*&, CktOutPin*);
   void replace(string, CktOutPin*);
   void reNewPin(CktOutPin*&);
   CktOutPin* getSharPin(const string&) const;

   void clear();
   bool isSigExist(string&) const;
   CktOutPin* genVarDeclaredOutpin(const string&) const;
   CktOutPin* genVarOutpinFout(string&, int);
   CktOutPin* genVarOutPinAllFout(string&, CktOutPin*);
   SynSharNodeMap& operator =(const SynSharNodeMap&);
private:
   struct mapKey 
   {
      mapKey(CktCellType cp, CktOutPin* ia, CktOutPin* ib) { _type = cp; _left = ia; _right = ib; }
      bool operator ==(const mapKey&) const;
      mapKey& operator =(const mapKey&);
      bool operator <(const mapKey&) const;
      CktCellType _type;
      CktOutPin* _left;
      CktOutPin* _right;
   };

   class SynSharSigNode
   {
   public:
      SynSharSigNode(CktOutPin*);
      ~SynSharSigNode();
      CktOutPin* getOutPin() const;
      CktOutPin* genFout(int&);
      void genAllFout(vector<CktOutPin*>&, const unsigned&);
      void update(CktOutPin*); //The function will set _spCell to null in order to mis-use wrong _spCell
   private:
      //for a variable
      CktOutPin* _outPin;
      CktSplitCell* _spCell;
   };

   SynSharSigNode* getSharNode(const string&) const;
//=================== class member ======================//
   map<string, SynSharSigNode*> _sigMap;
   map<mapKey, CktOutPin*>      _opMap;
   OutPinMap                    _replaceMap;
};

//========== class  synSensitivityList  ==========//
class SensitivityNode
{
public:
   SensitivityNode(bool, CktOutPin*);
   ~SensitivityNode();
   string getName() const;
   CktOutPin* getOutpin() const;
   bool getEdge() const;
   CktOutPin* getClkOrRstPin();
private:
   bool _edge; //true: posedge, false:negedge
   CktOutPin* _pin;
};

class SensitivityList
{
public:
   SensitivityList();
   ~SensitivityList();
   void insert(SensitivityNode*);
   unsigned size() const;
   SensitivityNode* operator[](const unsigned&) const;
private:
   vector<SensitivityNode*> _list;
};

//============= class synBusList =============//
/*
   A[6:0] => _begin = 0, _end = 6, _isInverted = true;
   A[0:6] => _begin = 0, _end = 6, _isInverted = false;
   _end is always greater than or equal to _begin
*/
class SynBus
{
public:
   SynBus(unsigned, int, int, bool);
   ~SynBus();
   bool operator == (const SynBus& node) const;
   bool isEqual(unsigned&, int&, int&, bool&) const;
   bool isContain(const SynBus*&) const;
   unsigned getWidth() const;
   int getBegin() const;
   int getEnd() const;
   bool isInverted() const;
   
   CktOutPin* getMsb() const;
   CktOutPin* getLsb() const;
   void print() const;
private:
   unsigned _width;
   int      _begin;
   int      _end;
   bool     _isInverted;

   CktOutPin* _msb;
   CktOutPin* _lsb;
};

class SynConstAry //store constant node, include 3 or 2'b11.
{
public:
   SynConstAry();
   ~SynConstAry();
   void insert(CktOutPin*);
   int isExist(const Bv4*, int);
   CktOutPin* operator[](const unsigned&) const;
   int size() const;
   void clear();
private:
   vector <CktOutPin*> _list;
};

//use for generateing structural Verilog output
class SynOutFile
{
public:
   SynOutFile();
   ~SynOutFile();
   bool isExistWire(string&) const;
   void insertWireInst(string, string);
   void insertRegInst(string, string);
   void insertWireInst(string);
   void insertModuleInst(string);
   void open(const char*);
   void close();
   void flushModuleDef();
   void insert(string&);   
   friend SynOutFile& operator << (SynOutFile&, const string&);
   friend SynOutFile& operator << (SynOutFile&, const int&);
   friend SynOutFile& endl( SynOutFile&);
private:
   ofstream _synOutFile;
   stringstream _wireInst;
   set<string>  _wireSet;
   stringstream _moduleInst;
};

struct synFuncArgu
{
   string name;
   string rename;
   CktOutPin* ptr;
};

class SynForArgu
{
public:
   SynForArgu(string, int, int, int, int, RelateOpClass);
   SynForArgu(const SynForArgu&);
   ~SynForArgu();
   
   const string& getName() const;
   int getCurValue() const;
   int getEndValue() const;
   int getVarValue() const;
   int getIsAdd() const;
   RelateOpClass getType() const;
   bool isAdd() const;

   bool isRepeat() const;
   void update();
   CktOutPin* syn2ConstCell() const;
private:
   string _name;
   unsigned _curValue : 12;
   unsigned _endValue : 12;
   unsigned _varValue : 7;
   unsigned _isAdd    : 1; //isAdd = 1 => '+'; isAdd = 0 => '-'
   RelateOpClass _type;
};

#endif
