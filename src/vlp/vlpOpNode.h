/****************************************************************************
  FileName     [ vlpOpNode.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Operation Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_OPNODE_H
#define VLP_OPNODE_H

#include "vlpBase.h"
#include "vlpDefine.h"
#include "cktPin.h"
#include "bv4.h"

#include <string>

using std::string;

struct paramPair
{
   paramPair();
   paramPair(const string&, VlpBaseNode* const&);
   paramPair(const paramPair&);
   ~paramPair();

   paramPair& operator = (const paramPair&);
   paramPair* duplicate() const;
   void whoAmI(int, int) const;

   string param;
   VlpBaseNode* content;
};

class VlpIntNode : public VlpBaseNode  // Verilog integer
{
public :
   VlpIntNode(int);
   ~VlpIntNode();
   // VLP Functions
   inline bool checkNext() const { return false; }
   inline int getNum() const { return _number; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   bool isConst() const;
   int constPropagate() const;
   CktOutPin* synthesis() const;
   string writeExp(set<string>&) const;
   // FSM Functions
   MyString str() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   int _number;
};

class VlpCaseItemNode : public VlpBaseNode
{
public :
   VlpCaseItemNode(VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   ~VlpCaseItemNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline void setInner(VlpBaseNode* const inner) { _inner = inner; }
   inline bool checkNext() const { return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline const VlpBaseNode* getExp() const { return _exp; }
   inline const VlpBaseNode* getInner() const { return _inner; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   const VlpCaseItemNode* getNext_NonDefault() const;
   bool isDefaultItem() const;
   CktOutPin* synthesis() const;
   bool isItemHasX() const;
   // FSM Functions
   MyString str() const;
   vector< VlpSignalNode* > listSIGs() const;
private :
   VlpBaseNode* _exp;
   VlpBaseNode* _inner;
   VlpBaseNode* _next;
};

class VlpFunCall : public VlpBaseNode
{
public :
   VlpFunCall(const string&, VlpBaseNode*);
   ~VlpFunCall();
   // VLP Functions
   inline bool checkNext() const { return true; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   inline int nameId() const { return _nameId; }
   inline const VlpBaseNode* arguNode() const { return _argument; }
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   vector< VlpSignalNode* > listSIGs() const;
private :
   int _nameId;
   VlpBaseNode* _argument;
};

class VlpTaskCall : public VlpBaseNode
{
public :
   VlpTaskCall(const string&, VlpBaseNode*, VlpBaseNode*);
   ~VlpTaskCall();
   // VLP Functions
   inline bool checkNext() const { return true; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // FSM Functions
   MyString str() const;
private :
   int _nameId;
   VlpBaseNode* _argument;
   VlpBaseNode* _next;
};

class VlpSignalNode : public VlpBaseNode
{
public :
   VlpSignalNode(int, VlpBaseNode* = NULL, VlpBaseNode* = NULL);
   ~VlpSignalNode();
   // VLP Functions
   inline void setSigWidth(VlpBaseNode* msb, VlpBaseNode* lsb) { _msb = msb; _lsb = lsb; }
   inline bool checkNext() const { return false; }
   inline bool isPartSelect() const { return _msb; }
   inline void setNameId(int src) { _nameId = src; }
   inline const int& getNameId() const { return _nameId; }
   inline void setMSB(VlpBaseNode* src) { _msb = src; }
   inline void setLSB(VlpBaseNode* src) { _lsb = src; }
   inline const VlpBaseNode* getMsb() const { return _msb; }
   inline const VlpBaseNode* getLsb() const { return _lsb; }
   bool setNext(VlpBaseNode* const);  // Debug Usage
   bool isMatchName(string&) const;
   bool isBitBaseStr() const;
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   string getSigName() const;
   // SYN Functions
   CktOutPin* synthesis() const;
   CktOutPin* synModCellOutPin() const;
   unsigned genLhsBusId() const;
   unsigned genMemBusId() const;
   unsigned getMemIndex() const;
   bool isConst() const;
   bool isConstIndex() const;
   int constPropagate() const;
   string writeExp(set<string>&) const;
   // FSM Functions
   MyString str() const;
   MyString strNoIndex() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   int MSB() const;
   int LSB() const;
   vector<BddNode> bdd( BddManager*, int&, int&, const MyString ) const;
private : 
   // SYN Functions
   bool isBv() const;
   unsigned port2Bus() const;    
   CktOutPin* synConstPin(string&) const;
   CktOutPin* synSignalPin(string&) const;

   int _nameId;
   VlpBaseNode* _msb;
   VlpBaseNode* _lsb;
};

// Note : VlpParamNode is similar to VlpSignalNode
//        However, VlpParamNode are signals that can be overloaded
class VlpParamNode : public VlpBaseNode
{
public :
   VlpParamNode(paramPair*);
   ~VlpParamNode();
   // VLP Functions
   inline bool checkNext() const { return false; }
   bool setNext(VlpBaseNode* const);  // Debug Usage
   VlpBaseNode* getParamContent() const;
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const ;
   bool isConst() const;
   int constPropagate() const;
   string writeExp(set<string>&) const;
   // FSM Functions
   MyString str() const;
   MyString strContent() const;
   MyString strName() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   paramPair* _pPair;
};

class VlpOperatorNode : public VlpBaseNode
{
public :
   VlpOperatorNode(OperatorClass);
   virtual ~VlpOperatorNode();
   // VLP Functions
   inline OperatorClass getOperatorClass() const { return _operatorClass; }
   inline virtual bool isBvConcatenate() const { return false; }
   inline virtual bool checkNext() const { return false; }
   virtual VlpBaseNode* duplicate() const;
   virtual void whoAmI(int, int) const;
   virtual void writeWhoAmI(ofstream&) const;
   // SYN Functions
   virtual string writeExp(set<string>&) const;
   // FSM Functions
   virtual MyString str() const;
   virtual set< MyString > setSIGs() const;
   virtual vector< VlpSignalNode* > listSIGs() const;
   virtual vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
protected :
   OperatorClass _operatorClass;
};

class VlpBitWiseOpNode : public VlpOperatorNode
{ 
public :
   VlpBitWiseOpNode();  // For Child Class Constructor ONLY
   VlpBitWiseOpNode(BitWiseOpClass, VlpBaseNode*, VlpBaseNode*);
   virtual ~VlpBitWiseOpNode();
   // VLP Functions
   inline BitWiseOpClass getBwOpClass() const { return _bitWiseOp; }
   virtual VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   virtual MyString str() const;
   virtual set< MyString > setSIGs() const;
   virtual vector< VlpSignalNode* > listSIGs() const;
   virtual vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
protected : 
   // SYN Functions
   CktCellType typeVlp2Cir(BitWiseOpClass) const;
   BitWiseOpClass _bitWiseOp;
private : 
   // SYN Functions
   CktCell* createCell(CktInPin*&, CktInPin*&, CktOutPin*&) const;
   VlpBaseNode* _leftOperand;      
   VlpBaseNode* _rightOperand;      
};

class VlpBitNotOpNode : public VlpBitWiseOpNode
{
public :
   VlpBitNotOpNode(VlpBaseNode*);
   ~VlpBitNotOpNode();
   // VLP Functions
   inline const VlpBaseNode* getOprn() const { return _operand; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   VlpBaseNode* _operand;
};

//=========================Arithmetic=====================//
class VlpArithOpNode : public VlpOperatorNode
{
public :  
   VlpArithOpNode();
   VlpArithOpNode(ArithOpClass, VlpBaseNode*, VlpBaseNode*);
   virtual ~VlpArithOpNode();
   // VLP Functions
   inline ArithOpClass getArithOpClass() const { return _arithOp; }
   inline const VlpBaseNode* getLeft() const { return _leftOperand; }
   inline const VlpBaseNode* getRight() const { return _rightOperand; }
   virtual VlpBaseNode* duplicate() const;
   virtual void whoAmI(int, int) const;
   virtual void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   bool isConst() const;
   int constPropagate() const;
   // FSM Functions
   virtual MyString str() const;
   virtual set< MyString > setSIGs() const;
   virtual vector< VlpSignalNode* > listSIGs() const;
protected : 
   ArithOpClass _arithOp;
private : 
   // SYN Functions
   CktCell* createCell(CktInPin*&, CktInPin*&, CktOutPin*&) const;
   CktCellType typeVlp2Cir(ArithOpClass) const;
   VlpBaseNode* _leftOperand;
   VlpBaseNode* _rightOperand;
};

class VlpSignNode : public VlpArithOpNode
{
public :
   VlpSignNode(ArithOpClass, VlpBaseNode*);
   ~VlpSignNode();
   // VLP Functions
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   bool isConst() const;
   int constPropagate() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
private :
   VlpBaseNode* _operand;
};

//========================Logic===========================//
class VlpLogicOpNode : public VlpOperatorNode
{
public : 
   VlpLogicOpNode();  // For Child Class Constructor ONLY
   VlpLogicOpNode(LogicOpClass, VlpBaseNode*, VlpBaseNode*);
   virtual ~VlpLogicOpNode();
   // VLP Functions
   inline LogicOpClass getLogicClass() const { return _logicOp; }
   inline const VlpBaseNode* getLeft() const { return _leftOperand; }
   inline const VlpBaseNode* getRight() const { return _rightOperand; }
   virtual VlpBaseNode* duplicate() const;
   virtual void whoAmI(int, int) const;
   virtual void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   virtual MyString str() const;
   virtual set< MyString > setSIGs() const;
   virtual vector< VlpSignalNode* > listSIGs() const;
   virtual vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
protected :
   // SYN Functions
   CktCellType typeVlp2Cir(LogicOpClass) const;
   LogicOpClass _logicOp;
private :
   // SYN Functions
   CktCell* createCell(CktInPin*&, CktInPin*&, CktOutPin*&) const;
   VlpBaseNode* _leftOperand;
   VlpBaseNode* _rightOperand;
};

class VlpLogicNotOpNode : public VlpLogicOpNode
{
public :
   VlpLogicNotOpNode(VlpBaseNode*); 
   ~VlpLogicNotOpNode();
   // VLP Functions
   inline const VlpBaseNode* getOprn() const { return _operand; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   VlpBaseNode* _operand;
};

//======================Reduction=========================//
class VlpReductOpNode : public VlpOperatorNode
{
public : 
   VlpReductOpNode(ReductOpClass, VlpBaseNode*);
   ~VlpReductOpNode();
   // VLP Functions
   inline ReductOpClass getRedOpCalss() const { return _reductOp; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
private :
   // SYN Functions
   CktCell* createCell(CktInPin*&, CktOutPin*&) const;
   CktCellType typeVlp2Cir(ReductOpClass) const;
   ReductOpClass _reductOp;
   VlpBaseNode* _operand;
};

//======================Relational========================//
class VlpRelateOpNode : public VlpOperatorNode
{
public : 
   VlpRelateOpNode(RelateOpClass, VlpBaseNode*, VlpBaseNode*);
   ~VlpRelateOpNode();
   // VLP Functions
   inline RelateOpClass getRelOpClass() const { return _relateOp; }
   inline const VlpBaseNode* getLeft() const { return _leftOperand; }
   inline const VlpBaseNode* getRight() const { return _rightOperand; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   // SYN Functions
   CktCellType typeVlp2Cir(RelateOpClass) const;
   CktCell* createCell(CktInPin*& iLeft, CktInPin*& iRight, CktOutPin*& oPin) const;
   RelateOpClass _relateOp;
   VlpBaseNode* _leftOperand;
   VlpBaseNode* _rightOperand;
};

//========================Shift===========================//
class VlpShiftOpNode : public VlpOperatorNode
{
public :
   VlpShiftOpNode(ShiftOpClass, VlpBaseNode*, VlpBaseNode*);
   ~VlpShiftOpNode();
   // VLP Functions
   inline ShiftOpClass getShOpClass() const { return _shiftOp; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
private :
   CktCellType typeVlp2Cir(ShiftOpClass) const;
   CktCell* createCell(CktInPin*&, CktInPin*&, CktOutPin*&) const;
   ShiftOpClass _shiftOp;
   VlpBaseNode* _leftOperand;
   VlpBaseNode* _rightOperand;
};

//========================Equality========================//
class VlpEqualityNode : public VlpOperatorNode
{
public :  
   VlpEqualityNode(EqualityClass, VlpBaseNode*, VlpBaseNode*);
   ~VlpEqualityNode();
   // VLP Functions
   inline const VlpBaseNode* getLeft() const { return _leftOperand; }
   inline const VlpBaseNode* getRight() const { return _rightOperand; }
   inline EqualityClass getEqualityClass() const { return _equality; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   // SYN Functions
   CktOutPin* genSharPin(CktOutPin*, CktOutPin*) const;
   EqualityClass _equality;
   VlpBaseNode* _leftOperand;
   VlpBaseNode* _rightOperand;
};

//====================Concatenation {}====================//
class VlpConcatenateNode : public VlpOperatorNode
{
public :
   VlpConcatenateNode(VlpBaseNode*);
   ~VlpConcatenateNode();
   // VLP Functions
   inline const VlpBaseNode* getContent() const { return _content; }
   bool isBvConcatenate() const;
   Bv4 getConcatenateBv() const;
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   void synLhsConcatenate(vector<unsigned>&, vector<string>&, unsigned&) const;
   const VlpSignalNode* getConcatMemSignal(unsigned) const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   VlpBaseNode* _content;
};

//====================Replication {{}}====================//
class VlpReplicationNode : public VlpOperatorNode   
{
public :
   VlpReplicationNode(VlpBaseNode*, VlpBaseNode*);
   ~VlpReplicationNode();
   // VLP Functions
   inline const VlpBaseNode* getContent() const { return _content; }
   bool isBvConcatenate() const;
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   VlpBaseNode* _times;
   VlpBaseNode* _content;
};

//=====================Conditional ?:=====================//
class VlpConditionalNode : public VlpOperatorNode
{
public :
   VlpConditionalNode(VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   ~VlpConditionalNode();
   // VLP Functions
   inline const VlpBaseNode* getCondition() const { return _conditional; }
   inline const VlpBaseNode* getTrue() const { return _trueChild; }
   inline const VlpBaseNode* getFalse() const { return _falseChild; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
private :
   VlpBaseNode* _conditional;
   VlpBaseNode* _trueChild;
   VlpBaseNode* _falseChild;
};

// Note : Used in connect of Concatenate, function, and task
//        Used in EX : `define bus 5:0 
//        Used in '[' expression ':' expression ']'
class VlpConnectNode : public VlpBaseNode
{
public : 
   VlpConnectNode(VlpBaseNode*);
   VlpConnectNode(VlpBaseNode*, VlpBaseNode*);  // For `define MACRO in YACC
   ~VlpConnectNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getExp() const { return _exp; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline const VlpBaseNode* getFirst() const { return getExp(); }  // For YACC temp
   inline const VlpBaseNode* getSecond() const { return getNext(); }  // For YACC storage
   inline bool checkNext() const { return true; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   CktOutPin* synthesis() const;
   // FSM Functions
   MyString str() const;
   set< MyString > setSIGs() const;
   vector< VlpSignalNode* > listSIGs() const;
   vector< BddNode > bdd( BddManager*, int&, int&, const MyString ) const;
private :
   VlpBaseNode* _exp;
   VlpBaseNode* _next;
};

/*
=========================================================================
Verilog Operator        Name                Functional Group
=========================================================================
     []        bit-select or part-select   
     ()        parenthesis
=========================================================================
      &        reduction AND                Reduction         unary
      |        reduction OR                 Reduction         unary
     ~&        reduction NAND               Reduction         unary
     ~|        reduction NOR                Reduction         unary
      ^        reduction XOR                Reduction         unary
     ~^  ^~    reduction XNOR               Reduction         unary
=========================================================================
      +        unary (sign) plus            Arithmetic
      -        unary (sign) minus           Arithmetic
=========================================================================
     {}        concatenation                Concatenation
=========================================================================
    {{}}       replication                  Replication
=========================================================================
     *         multiply                     Arithmetic
     /         divide                       Arithmetic
     %         modulus                      Arithmetic
     +         binary plus                  Arithmetic
     -         binary minus                 Arithmetic
=========================================================================
     >>        shift left                   Shift
     <<        shift right                  Shift
=========================================================================
     >         greater than                 Relational
     >=        graeter than or equal to     Relatiobal
     <         less than                    Relational
     <=        less than or equal to        Relational
=========================================================================
     ==        logical equality             Equality
     !=        logical inequality           Equality
     ===       case equality                Equality
     !==       case inequality              Equality
=========================================================================
      ~        bit-wise NOT                 Bit-wise          unary
      &        bit-wise AND                 Bit-wise
      ^        bit-wise XOR                 Bit-wise
     ^~  ~^    bit-wise XNOR                Bit-wise
      |        bit-wise OR                  Bit-wise
=========================================================================
      !        logical NOT                  Logical           unary
     &&        logical AND                  Logical
     ||        logical OR                   Logical
=========================================================================
     ?:        conditional                  Conditional
=========================================================================

">="    GEQ             "=<"   LEQ             "&&"    LOGAND           "||"   LOGOR
"=="    LOGEQUALITY     "!="   LOGINEQUALITY   "!"     LOGNOT           "~"    UNARY_BITNOT
"&"     UNARYAND        "&"    BITAND          "|"     UNARYOR          "|"    BITOR
"^"     UNARYXOR        "^"    BITXOR       "~^" "^~"  UNARYXNOR    "~^" "^~"  BITXNOR
"~&"    UNARYNAND       "~|"   UNARYNOR

//LOG:      and or not(!) equality inequality
//UNARY:    and or nand nor xor xnor
//BIT-Wise: and or xor xnor
//UNARY Bit-Wise  not(~)

"<<"   LSHIFT           ">>"   RSHIFT          "?:"    CONDITIONAL      "="    ASSIGN(EQ)
"<="   NBASSIGN         ">"    GREATER         "<"     LESS             "+"    ADD
"+"    POSITIVE         "-"    SUBTRACT        "-"     NEGATIVE         "*"    MULTIPLY
"posedge" POSEDGE        "negedge" NEGEDGE     ( )     LVSEN            "or"   OR
"edge" EDGE_
*/

#endif


