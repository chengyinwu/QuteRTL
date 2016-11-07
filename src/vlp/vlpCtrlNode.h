/****************************************************************************
  FileName     [ vlpCtrlNode.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Control Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_CTRLNODE_H
#define VLP_CTRLNODE_H

#include "vlpBase.h"
#include "vlpModule.h"
#include "vlpFSM.h"
#include "bv4.h"
#include <string>
#include <vector>


class SensitivityList;

class VlpAssignNode : public VlpBaseNode
{
public :
   VlpAssignNode(VlpBaseNode*, VlpBaseNode*);
   ~VlpAssignNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline bool checkNext() const { return true; }
   inline const VlpBaseNode* getExp() const { return _exp; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   void synthesis(bool&, bool&, CktOutPin*&) const;
   // FSM Functions
   MyString str() const;
   vector< VlpSignalNode* > listSIGs() const;
private :
   VlpBaseNode* _exp;
   VlpBaseNode* _next;
};

class VlpAlwaysNode : public VlpBaseNode
{
public :
   VlpAlwaysNode(VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   ~VlpAlwaysNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline bool checkNext() const { return true; }
   inline bool isFullAssign(CktOutPin*) const { return true; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   void synthesis(bool&, bool&, CktOutPin*&) const;
   bool isSeq() const;
   bool isComb() const;
   // FSM Functions
   vector< VlpFSMAssign > extractAL() const;
   vector< VlpSignalNode* > listSIGs() const;
   CktOutPin* genClk(SensitivityList&) const;
   void edgeNodeSynthesis(SensitivityList&) const;
   bool isResetSignal(string) const;
private :
   VlpBaseNode* _exp;
   VlpBaseNode* _inner;
   VlpBaseNode* _next;
   // FSM
   struct FSMCallBack 
   {
      bool               _isCase;
      const VlpBaseNode* _return;
      size_t             _cond;
      void reset() {
         _isCase = false;
         _return = NULL;
         _cond   = size_t( NULL );
      }

      FSMCallBack& operator = ( const FSMCallBack& src ) {
         _isCase = src._isCase;
         _return = src._return;
         _cond   = src._cond;
         return *this;
      }
   };      
};

class VlpOrNode : public VlpBaseNode
{
public :
   VlpOrNode(VlpBaseNode*);
   ~VlpOrNode();
   // VLP Functions
   inline bool checkNext() const { return false; }
   inline const VlpBaseNode* getContent() const { return _content; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // FSM Functions
   vector< VlpSignalNode* > listSIGs() const;
private :
   VlpBaseNode* _content;
};

class VlpEdgeNode : public VlpBaseNode
{
public :
   VlpEdgeNode(EdgeType, VlpBaseNode*);
   ~VlpEdgeNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline bool checkNext() const { return true; }
   inline const EdgeType getEdgeType() const { return _edgeType; }
   inline const VlpBaseNode* getExp() const { return _exp; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   void synSensiList(SensitivityList&) const;
   // FSM Functions
   vector< VlpSignalNode* > listSIGs() const;
private :
   EdgeType _edgeType;
   VlpBaseNode* _exp;
   VlpBaseNode* _next;
};

class VlpIfNode : public VlpBaseNode // ElseIfNode
{
public :
   VlpIfNode(VlpBaseNode*, VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   ~VlpIfNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline bool checkNext() const { return true; }
   inline const VlpBaseNode* getExp() const { return _exp; }
   inline const VlpBaseNode* getTrueChild() const { return _trueChild; }
   inline const VlpBaseNode* getFalseChild() const { return _falseChild; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   void varFlatten() const;
   // FSM Functions
   vector< VlpSignalNode* > listSIGs() const;
private :
   VlpBaseNode* _exp;
   VlpBaseNode* _next;
   VlpBaseNode* _trueChild;
   VlpBaseNode* _falseChild;
};

class VlpCaseNode : public VlpBaseNode
{
public :
   VlpCaseNode(CaseType, VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   ~VlpCaseNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline bool checkNext() const { return true; }
   inline void setFullCase(bool b) { _isFullCase = b; }
   inline void setParallelCase(bool b) { _isParallelCase = b; }
   inline const VlpBaseNode* getExp() const { return _exp; }
   inline const VlpBaseNode* getInner() const { return _inner; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   void varFlatten() const;
   // FSM Functions
   vector< VlpSignalNode* > listSIGs() const;
private : 
   // SYN Functions
   const VlpCaseItemNode* getDefault_sNum(int&) const;
   bool isFullState(const CktOutPin* const&, int) const;
   bool isFullStateX(const CktOutPin* const&, const VlpCaseItemNode*&) const;
   bool preprocess(const CktOutPin* const&, const VlpCaseItemNode*&) const;
   CktOutPin* genEqCell(CktOutPin*, CktOutPin*) const;
   CktOutPin* genEqCellX(CktOutPin*, CktOutPin*) const;
   void synCaseItem(CktOutPin*, bool, const VlpCaseItemNode*) const;
   void synCasexItem(CktOutPin*, bool, const VlpCaseItemNode*) const;
   void synXItem1(CktOutPin*, bool, const VlpCaseItemNode*) const;
   void synXItem2(CktOutPin*, bool, const VlpCaseItemNode*) const;
   void synCasezItem(CktOutPin*, bool, const VlpCaseItemNode*) const;
   void handleDefault(bool, const VlpCaseItemNode*&) const;
#ifdef SYN_LINE_TRACE
   void lineTrace(VlpBaseNode*&);
#endif

   CaseType _caseType;
   bool _isParallelCase;
   bool _isFullCase;
   VlpBaseNode* _exp;
   VlpBaseNode* _inner;
   VlpBaseNode* _next;
};

class VlpForNode : public VlpBaseNode
{
public :
   VlpForNode(VlpBaseNode*, VlpBaseNode*, VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   ~VlpForNode();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline bool checkNext() const { return true; }
   inline const VlpBaseNode* getInit() const { return _initState; }
   inline const VlpBaseNode* getEndCond() const { return _endCond; }
   inline const VlpBaseNode* getIncre() const { return _increState; }
   inline const VlpBaseNode* getInner() const { return _inner; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   void varFlatten() const;
   // FSM Functions
   vector< VlpSignalNode* > listSIGs() const;
private :
   VlpBaseNode* _initState;
   VlpBaseNode* _endCond;
   VlpBaseNode* _increState;
   VlpBaseNode* _inner;
   VlpBaseNode* _next;
};

class VlpBAorNBA_Node : public VlpBaseNode
{
public :
   VlpBAorNBA_Node(AssignType, VlpBaseNode*, VlpBaseNode*, VlpBaseNode*);
   ~VlpBAorNBA_Node();
   // VLP Functions
   inline bool setNext(VlpBaseNode* const next) { _next = next; return true; }
   inline const VlpBaseNode* getNext() const { return _next; }
   inline bool checkNext() const { return true; }
   inline const VlpBaseNode* getLeft() const { return _leftSide; }
   inline const VlpBaseNode* getRight() const { return _rightSide; }
   VlpBaseNode* duplicate() const;
   void whoAmI(int, int) const;
   void writeWhoAmI(ofstream&) const;
   // SYN Functions
   void varFlatten() const;
   // FSM Functions
   MyString str() const;
   vector< VlpSignalNode* > listSIGs() const;
private : 
   // SYN Functions
   void setSignalFin(CktOutPin*) const;
   void setMemFin(CktOutPin*) const;
   void setConcatFin(CktOutPin*) const;
   CktOutPin* matchLhsRhsBus(CktOutPin*, unsigned) const;
   CktOutPin* insertBuf(CktOutPin*, unsigned) const;

   AssignType   _assignType;
   VlpBaseNode* _leftSide;
   VlpBaseNode* _rightSide;
   VlpBaseNode* _next;
};

#endif


