/****************************************************************************
  FileName     [ synCtrlNode.cpp ]
  Package      [ syn ]
  Synopsis     [ Synthesis function of Control Node class ]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft( c ) 2007 LaDs( III ), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_CTRL_NODE_SOURCE
#define SYN_CTRL_NODE_SOURCE

//---------------------------
//  system include
//---------------------------
#include <iostream>
#include <vector>
#include <sstream>
#include <queue>
#include "math.h"

using namespace std;

//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktPin.h"
#include "cktLogicCell.h"
#include "bv4.h"
#include "bddManager.h"

//---------------------------
//  Global variables
//---------------------------
#define HASH_SIZE 65521
#define CACHE_SIZE 65521

extern SYN_usage*       SYN;
extern const VlgModule* curVlpModule;
extern bool             busDirectionDefault;
extern VlpDesign&       VLDesign;

//================================= class VlpAlwaysNode ==================================//
void
VlpAlwaysNode::synthesis(bool& isSeqB, bool& isSync, CktOutPin*& oClk) const
{
   SensitivityList sensiList;
   isSeqB = isSeq();
   // Simple check
   // Size == 1: It must be clock
   // Size == 2: One is clock, and the other is reset
   // Otherwise: No support. (multiple clk or reset)
   // if there's only one element in sensitivity list, it must be a synchronous block.
   // ; two elements in the sensitivity list, it is a asynchronous block.
   if (isSeqB) {
      edgeNodeSynthesis(sensiList); // synthesize VlpEdgeNode to syn-format( bool, CktOutPin* )
      oClk = genClk(sensiList); 
      if (sensiList.size() == 1)      isSync = true;
      else if (sensiList.size() == 2) isSync = false; 
      else Msg(MSG_ERR) << "More than Two Elements Specified in Sequential Always Block Sensitivity List !!!" << endl;
   }
   else {
      oClk   = 0;
      isSync = false;
   }
   for (const VlpBaseNode* tmp = _inner; tmp != 0; tmp = tmp->getNext())
      tmp->varFlatten();
}

bool 
VlpAlwaysNode::isSeq() const
{
   // eg.
   // always @( posedge clk )  <-- only one VlpEdgeNode
   // always @( state )
   // always @( posedge clk or negedge rst ) <-- sequetial circuit, EDGETYPE_NEGEDGE or EDGETYPE_POSEDGE
   // always @( state1 or state2 ) <-- combinational circuit, EDGETYPE_SENSITIVE
   if (_exp->getNodeClass() == NODECLASS_EDGE) {
      return (!((static_cast<VlpEdgeNode*>(_exp))->getEdgeType() == EDGETYPE_SENSITIVE));
   }
   else if (_exp->getNodeClass() == NODECLASS_OR) {
      const VlpBaseNode* tmp = (static_cast<const VlpOrNode*>(_exp))->getContent();
      return (!((static_cast<const VlpEdgeNode*>(tmp))->getEdgeType() == EDGETYPE_SENSITIVE));
   }
   else {
      Msg(MSG_ERR) << "Syntax Error in Always Block Sensitivity List !!!" << endl;
      return false;
   }
}

bool 
VlpAlwaysNode::isComb() const
{
   return !isSeq();
}

CktOutPin*
VlpAlwaysNode::genClk(SensitivityList& sensiList) const
{
   if (sensiList.size() == 1)
      return sensiList[0]->getClkOrRstPin();
   else if (sensiList.size() == 2) {
      bool b0 = isResetSignal(sensiList[0]->getName());
      bool b1 = isResetSignal(sensiList[1]->getName());
      assert (b0^b1); // only one is true;

      if (b0) 
         return sensiList[1]->getClkOrRstPin();
      else 
         return sensiList[0]->getClkOrRstPin();
   }
   else
      assert (0);
}

// eg. always@( posedge clk or negedge rst )
// in sensiList
// |---|
// |   |---clk
// |   |
// |   |---not---rst
// |---|  
void 
VlpAlwaysNode::edgeNodeSynthesis(SensitivityList& sensiList) const
{
   if (_exp->getNodeClass() == NODECLASS_EDGE) 
      (static_cast<const VlpEdgeNode*>(_exp))->synSensiList(sensiList);
   else if (_exp->getNodeClass() == NODECLASS_OR) {
      const VlpBaseNode* content = (static_cast<const VlpOrNode*>(_exp))->getContent();
      for (; content != 0; content = content->getNext()) {
         if (content->getNodeClass() == NODECLASS_EDGE) 
            (static_cast<const VlpEdgeNode*>(content))->synSensiList(sensiList);
         else
            Msg(MSG_ERR) << "Syntax Error in Always Block Sensitivity List !!!" << endl;
      }
   }
   else
      Msg(MSG_ERR) << "Syntax Error in Always Block Sensitivity List !!!" << endl;
}

bool 
VlpAlwaysNode::isResetSignal(string signal) const
{
// by using BFS to get reset signal name.
// always@( posedge clk or negedge rst )
// ( 1 ) if ( rst )
// ( 2 ) if ( ~rst )
// ( 3 ) if ( !rst )
// ( 4 ) if ( rst == 0 ), if ( 1'b0 == rst ), if ( rst != 1)
// There are many styles.
// By using BFS, I can get reset signal name correctly
// But multiple clocks or resets are not allowed. 
   const VlpBaseNode* exp;
   const VlpBaseNode* expL;
   const VlpBaseNode* expR;
   for (const VlpBaseNode* n = _inner; n != 0; n = n->getNext()) {
      if (n->getNodeClass() == NODECLASS_IF) {
         exp = static_cast<const VlpIfNode*>(n)->getExp();
         if (exp->getNodeClass() == NODECLASS_SIGNAL) {
            if (static_cast<const VlpSignalNode*>(exp)->isMatchName(signal))
               return true;
         }
         else if (exp->getNodeClass() == NODECLASS_OPERATOR) {
            const VlpOperatorNode* op = static_cast<const VlpOperatorNode*>(exp);
            if (op->getOperatorClass() == OPCLASS_BITWISE) {
               const VlpBitWiseOpNode* bop = static_cast<const VlpBitWiseOpNode*>(op);
               if (bop->getBwOpClass() == BITWISEOP_NOT) {
                  const VlpBaseNode* sig = static_cast<const VlpBitNotOpNode*>(bop)->getOprn();
                  if (sig->getNodeClass() == NODECLASS_SIGNAL)
                     if (static_cast<const VlpSignalNode*>(sig)->isMatchName(signal))
                        return true;                     
               }
            }
            else if (op->getOperatorClass() == OPCLASS_LOGIC) {
               const VlpLogicOpNode* lop = static_cast<const VlpLogicOpNode*>(op);
               if (lop->getLogicClass() == LOGICOP_NOT) {
                  const VlpBaseNode* sig = static_cast<const VlpLogicNotOpNode*>(lop)->getOprn();
                  if (sig->getNodeClass() == NODECLASS_SIGNAL)
                     if (static_cast<const VlpSignalNode*>(sig)->isMatchName(signal))
                        return true;                     
               }
            }
            else if (op->getOperatorClass() == OPCLASS_EQUALITY) {
               const VlpEqualityNode* eop = static_cast<const VlpEqualityNode*>(op);
               assert ((eop->getEqualityClass() == EQUALITY_LOG_EQU) || (eop->getEqualityClass() == EQUALITY_LOG_INEQU));
               expL = eop->getLeft();
               expR = eop->getRight();
               if (expL->getNodeClass() == NODECLASS_SIGNAL)
                  if (static_cast<const VlpSignalNode*>(expL)->isMatchName(signal))
                     return true;

               if (expR->getNodeClass() == NODECLASS_SIGNAL)
                  if (static_cast<const VlpSignalNode*>(expR)->isMatchName(signal))
                     return true;
            }
         }
      }
   }
   return false;
}

void
VlpEdgeNode::synSensiList(SensitivityList& sensiList) const
{
   CktOutPin* outpin = getExp()->synthesis();
   if (getEdgeType() == EDGETYPE_POSEDGE) {
      SensitivityNode* node = new SensitivityNode(true, outpin);
      sensiList.insert(node);
   }
   // edgeType == EDGETYPE_NEGEDGE, there will be an CKT_INV_CELL before outpin
   else if (getEdgeType() == EDGETYPE_NEGEDGE) {
      SensitivityNode* node = new SensitivityNode(false, outpin);
      sensiList.insert(node);
   }
   else assert(0);
}


//================================= class VlpBAorNBANode =================================//
// lhs has 2 cases.
// 1. a = b+c; lhs is a variable.
// 2. {cout, sum} = a+b;  lsh is a concatenate operator
// 3. mem[5] = vector;
// Ex: reg cout;
//     reg [3:0] sum, a, b;
//     {cout, sum} = a+b;
//     lhs have 5 bits while rhs has only 4 bits. designer should take care about this.
//     The flow will be
//         |----|                |-----|  ( 1, 4, 4, true )  |---|   cout
//     a---|ADD |----o-----------|split|----o----------------|buf|---o
//     b---|    | ( cout, width )|     |----o-------------|  |---|
//         |----|  =( 1, 4 )     |-----|( 4, 0, 3, true ) |
//             bus = ( 4, 0, 3, true )                    |  |---|   sum
//                                                        |--|buf|---o   
//                                                           |---|
void
VlpBAorNBA_Node::varFlatten() const
{
   //right part
   CktOutPin* rhsOutPin = _rightSide->synthesis();
   //left part  
   if (_leftSide->getNodeClass() == NODECLASS_SIGNAL) {
      string lhsName = BaseModule :: getName(static_cast<const VlpSignalNode*>(_leftSide)->getNameId());
      if (curVlpModule->isMemAry(lhsName)) setMemFin(rhsOutPin);
      else                                 setSignalFin(rhsOutPin);
   }
   else if (_leftSide->getNodeClass() == NODECLASS_OPERATOR) // must be concatenate operator
      setConcatFin(rhsOutPin);
   else {
      Msg(MSG_ERR) << "Syntax Error on LHS of Assignment !!!" << endl;
      assert (0);
   }
}

void
VlpBAorNBA_Node::setMemFin(CktOutPin* rhsOutPin) const
{
   const VlpSignalNode* signal = static_cast<const VlpSignalNode*>(_leftSide);
   unsigned lhsBusId = signal->genMemBusId(); // also generate synMemory
   string lhsName = BaseModule :: getName(signal->getNameId());
   CktOutPin* finalRhs = matchLhsRhsBus(rhsOutPin, lhsBusId);
   
   if (signal->isConstIndex()) {
      int index = signal->getMemIndex();
      SYN->synVarList.setMemFin(lhsName, finalRhs, _rightSide, index);
   }
   else  // variable index
      SYN->synVarList.setMemFin(lhsName, finalRhs, _rightSide, 0.5, signal->getMsb());
}

void
VlpBAorNBA_Node::setSignalFin(CktOutPin* rhsOutPin) const
{
   unsigned lhsBusId = static_cast<const VlpSignalNode*>(_leftSide)->genLhsBusId();//also generate lhs total width pin and SynVar
   string lhsName = BaseModule :: getName(static_cast<const VlpSignalNode*>(_leftSide)->getNameId());

   int funcNum = SYN->synFuncArguArray.size();
   if (funcNum > 0) { //check if the variable is defined in function scope
      vector<synFuncArgu*>* func = SYN->synFuncArguArray[funcNum-1];// support recurrence function
      for (unsigned i = 0; i < func->size(); ++i)
         if (lhsName == func->at(i)->name) {
            lhsName = func->at(i)->rename;
            break;
         }      
   }
   CktOutPin* finalRhs = matchLhsRhsBus(rhsOutPin, lhsBusId);
   SYN->synVarList.setVarFin(lhsName, finalRhs, _rightSide);
}

void
VlpBAorNBA_Node::setConcatFin(CktOutPin* rhsOutPin) const 
{  // The function is similar to VlpInance::handleConcatOut
   // handle the case "{A ,B[3:2], C} = .... "
   unsigned lhsWidth, iWidth, tmpBusId;
   vector<unsigned> busIdAry;  // only be used in "{ } = ..."
   vector<string>   sigNameAry;
   assert (static_cast<const VlpOperatorNode*>(_leftSide)->getOperatorClass() == OPCLASS_CONCATENATE);
   (static_cast<const VlpConcatenateNode*>(_leftSide))->synLhsConcatenate(busIdAry, sigNameAry, lhsWidth);
   if (VLDesign.getBus(rhsOutPin->getBusId())->getWidth() == 0) { //if rhs is a integer, to assign its width.
      tmpBusId = VLDesign.genBusId(lhsWidth, 0, lhsWidth-1, busDirectionDefault);
      rhsOutPin->setBusId(tmpBusId);
   }
   //assert (VLDesign.getBus(rhsOutPin->getBusId())->getWidth() == lhsWidth);
   if (VLDesign.getBus(rhsOutPin->getBusId())->getWidth() != lhsWidth)
      Msg(MSG_WAR) << "Different bits of LHS and RHS. (setConcatFin)" << endl;

   CktInPin*   newInPin = new CktInPin();
   CktSplitCell* spCell = new CktSplitCell();
   spCell->setNonSpFin();
   SYN->insertCktCell(spCell);
   spCell->connInPin(newInPin);    newInPin->connCell(spCell);
   rhsOutPin->connInPin(newInPin); newInPin->connOutPin(rhsOutPin);

   CktOutPin* outPin;
   const SynBus* iBus;
   int msb = lhsWidth;
   for (unsigned i = 0; i < busIdAry.size(); ++i) {
      outPin = new CktOutPin();
      spCell->connOutPin(outPin); outPin->connCell(spCell);
      outPin->setBusId(busIdAry[i]);
      iBus = VLDesign.getBus(busIdAry[i]);
      iWidth = iBus->getWidth();
      tmpBusId = VLDesign.genBusId(iWidth, msb-iWidth, msb-1, busDirectionDefault);
      spCell->insertBusId(tmpBusId);
      msb -= iWidth;

      if (curVlpModule->isMemAry(sigNameAry[i])) { // SynMemory is generated @ synLhsConcatenate
         const VlpSignalNode* signal = (static_cast<const VlpConcatenateNode*>(_leftSide))->getConcatMemSignal(i);
         if (signal->isConstIndex())
            SYN->synVarList.setMemFin(sigNameAry[i], outPin, _rightSide, signal->getMemIndex());
         else  // variable index
            SYN->synVarList.setMemFin(sigNameAry[i], outPin, _rightSide, 0.5, signal->getMsb());
      }
      else
         SYN->synVarList.setVarFin(sigNameAry[i], outPin, _rightSide);
   }
}

CktOutPin*
VlpBAorNBA_Node::matchLhsRhsBus(CktOutPin* rhsOutPin, unsigned lhsBusId) const
{
   //if rhs is a integer, to assign its width.
   unsigned lhsWidth = VLDesign.getBus(lhsBusId)->getWidth();
   if (_rightSide->getNodeClass() == NODECLASS_INT)
      rhsOutPin->setBusId(lhsBusId);
   else if (_rightSide->getNodeClass() == NODECLASS_PARAMETER)
      if (static_cast<const VlpParamNode*>(_rightSide)->getParamContent()->getNodeClass() == NODECLASS_INT)
         rhsOutPin->setBusId(lhsBusId);

   if (lhsWidth == VLDesign.getBus(rhsOutPin->getBusId())->getWidth()) {
      if (_rightSide->getNodeClass() == NODECLASS_OPERATOR)
         rhsOutPin->setBusId(lhsBusId);
      else  // NODECLASS_PARAMETER, NODECLASS_INT, NODECLASS_SIGNAL, NODECLASS_FUNCALL
         rhsOutPin = insertBuf(rhsOutPin, lhsBusId);        
   }
   else if (VLDesign.getBus(rhsOutPin->getBusId())->getWidth() == 0)
      rhsOutPin->setBusId(lhsBusId);
   else {
      Msg(MSG_WAR) << "Widths of LHS and RHS are different!" << endl;
      //whoAmI(6, 6);
      rhsOutPin = insertBuf(rhsOutPin, lhsBusId);
   }
   return rhsOutPin;
}

CktOutPin*
VlpBAorNBA_Node::insertBuf(CktOutPin* inOutPin, unsigned busId) const
{ // insert buf to change busId
   CktInPin* newInPin;
   CktOutPin* newOutPin;
   SYN->insertCktCell( new CktBufCell(true, newInPin, newOutPin) );
   inOutPin->connInPin(newInPin); newInPin->connOutPin(inOutPin);
   newOutPin->setBusId(busId);
   return newOutPin;
}

//=================================== class VlpIfNode  ===================================//
void
VlpIfNode::varFlatten() const
{
   const VlpBaseNode* node;
   SynCondState* cds = new SynCondState;
   CktOutPin* condPin = getExp()->synthesis();
   CktOutPin* outpin  = 0;
   //eg. reg [3:0] a;
   //    if ( a )
   //    mean if ( a > 0 )
   //    in Conformal it will apply "reduction or" operation to a ( it's the same )
   if (VLDesign.getBus(condPin->getBusId())->getWidth() > 1)
      outpin = condPin->genGreaterCell();
   else
      outpin = condPin;

   cds->setCtrlNode(getExp());
   cds->setCtrlCond(outpin);
   cds->setState(true);
   SYN->synCondStack.push_back(cds);

   //flatten true childs
   for (node = getTrueChild(); node != 0; node = node->getNext())
      node->varFlatten();
   //flatten false childs
   cds->setState(false);
   for (node = getFalseChild(); node != 0; node = node->getNext())
      node->varFlatten();

   delete cds;
   SYN->synCondStack.pop_back();
}

//================================== class VlpCaseNode  ==================================//
const VlpCaseItemNode* 
VlpCaseNode::getDefault_sNum(int& stateCtr) const
{
   const VlpCaseItemNode* tmpItemDefault = 0;
   const VlpBaseNode* itemPtr = getInner(); // get caseItem block      
   string name;
   //case( state )
   //   default:begin
   //             a=4;     <----- get default value
   //           end     
   while (itemPtr != 0) {
      if ((static_cast<const VlpCaseItemNode*>(itemPtr))->getExp()->getNodeClass() == NODECLASS_SIGNAL) {
         const VlpBaseNode* state = (static_cast<const VlpCaseItemNode*>(itemPtr))->getExp();
         name  = BaseModule :: getName((static_cast<const VlpSignalNode*>(state))->getNameId());
      }
      else
         name = "";
      //here, isFullAssign means "not casex and casez" and "not case( 1'b1 ) style".
      //So it must be "case( state )". Then I'll enumerate all states.
      //If we have defined synopsys_fullcase, then I needed enumerate states,
      //I just need to know how many states do we have.        
      if (name == "default")
         tmpItemDefault = static_cast<const VlpCaseItemNode*>(itemPtr);                 
      else
         ++stateCtr; //count the # states to determine if full state.
      itemPtr = itemPtr->getNext();
   }
   // casex and casez, I assume designer won't identify all states, but they will give a default state.
   // However if they don't write default state, there will be a latch.
   if ((_caseType == CASETYPE_CASE) && (stateCtr == 0))
      Msg(MSG_ERR) << "NO states in case node." << endl;

   return tmpItemDefault;                 
}

bool
VlpCaseNode::isFullStateX(const CktOutPin* const& opinState, const VlpCaseItemNode*& tmpItemDefault) const//for casex usage only!
{
   //condition  1. case( 3'b001 ), case( 1 )    2. case( state )
   const VlpBaseNode* itemPtr = getInner();// get caseItem block  
   string name;
   string allCube = "allCube";
   Bv4 bv;
   //getExp() has 3 kinds : VlpSignalNode, VlpIntNode, VlpConcatenateNode
   int supportNum = VLDesign.getBus(opinState->getBusId())->getWidth();
   BddManager bm(supportNum, HASH_SIZE, CACHE_SIZE);
   bm.initCube(allCube, supportNum);
   const VlpBaseNode* state;

   // condition 1 : casex (3'b011) or casex (3'b01X)
   if ((opinState->getCell() != 0) && (opinState->getCell()->getCellType() == CKT_CONST_CELL)) {
      int dummy = 0;
      tmpItemDefault = getDefault_sNum(dummy);
      return false;
   }
   // condition 2 : case (state)
   else { // other cell or null cell(Latch cell or FF cell, connect later!)
      while (itemPtr != 0) {
         state = (static_cast<const VlpCaseItemNode*>(itemPtr))->getExp();
         if (state->getNodeClass() == NODECLASS_PARAMETER)
            state = static_cast<const VlpParamNode*>(state)->getParamContent();

         if (state->getNodeClass() == NODECLASS_SIGNAL) {//VlpSignalNode
            name = BaseModule :: getName((static_cast<const VlpSignalNode*>(state))->getNameId());
            if (name == "default")
               tmpItemDefault = static_cast<const VlpCaseItemNode*>(itemPtr);                 
            else {
               bv = name;
               bm.insertCube(allCube, bv);
            }
         }
         else { // VlpConcatenateNode
            const VlpConcatenateNode* ccNode = static_cast<const VlpConcatenateNode*>(state);
            bv = ccNode->getConcatenateBv();
            bm.insertCube(allCube, bv);
         }
         itemPtr = itemPtr->getNext();
      }
      return bm.isFullCube(allCube);
   }
}

bool 
VlpCaseNode::isFullState(const CktOutPin* const& opinState, int stateCtr) const
{
   // 1. case( state )     2. case( 3'b001 ), case( 1 )
   // The two conditions are mutual exclusive. Condition 2 is usually used in one-hot 
   // design style. In this case, I define states are not fully assign by default
   // ex: `define ZERO 0;
   //     `define ONE  1;
   //     case( 1'b1 ) <-- state is number
   //        state[`ZERO] :
   //        state[`ONE]  :
   //     endcase                    
   int bitNumber = VLDesign.getBus(opinState->getBusId())->getWidth();
   assert (bitNumber < 32);
   int stateNum = (int)pow(2.0, (double)(bitNumber));
   if (opinState->getCell() != 0)
      if (opinState->getCell()->getCellType() == CKT_CONST_CELL)  // condition 2
         return false; 
   //null cell or condition 1
   return ((stateCtr == stateNum)? true : false);
}

// reg [1:0] a;
// parameter READ = 2'b10, WRITE = 2'b01, IDLE = 2'b00, GO = 2'b11;
//
// case (a) // fullcase            if (a == READ)        a = f1;
// READ  :   a = f1;               else if (a = WRITE)   a = f2;
// WRITE :   a = f2;       ==>     else                  a = f3;
// IDLE  :   a = f3;               [NO latch(comb) or loop(seq)]
//
// case (a) // fullcase
// READ  :   a = f1;
// WRITE :   a = f2;       ==>     the same as above
// IDLE  :   a = f3;               [ignore "default"]
// default :                       [if no specify "fullcase" => should have latch or loop]
//
// case (a) // fullcase            if (a == READ)        a = f1;
// READ  :   a = f1;               else if (a = WRITE)   a = f2;
// WRITE :   a = f2;       ==>     else if (a = IDLE)    a = f3;
// IDLE  :   a = f3;               else                  a = f4;
// default : a = f4;               
//
// case (a) // (fullcase)          if (a == READ)        a = f1;
// READ  :   a = f1;               else if (a = WRITE)   a = f2;
// WRITE :   a = f2;       ==>     else if (a = IDLE)    a = f3;
// IDLE  :   a = f3;               else                  a = f4;
// GO    :   a = f4                [full conditions => ignore "default"] 
// default : a = f5;               
//

void
VlpCaseNode::handleDefault(bool fullState, const VlpCaseItemNode*& defaultItem) const
{
   // if define synopsys full_case, then _isFullCase = true. synopsys full_case has the 
   // highest priority. Designer will take responsibility for this symbol.
   if (fullState)//if fullState, ignore default state
      return;
   else {        // not fully assigned
      if (_isFullCase) {  
         if (defaultItem != 0)  // exist default state 
            defaultItem->synthesis();
         else { // exist synthesis full case, but not fullState, without default state, need another handle ==> to check!!(louis)
            Msg(MSG_DBG) << "   ==> Exist synthesis full case, but no fullState, without default state, to Check !!" << endl;
            //_exp->whoAmI(6, 6);
         }
      }
      else { // not fully assigned
         if (defaultItem != 0) // since not fully assigned. If default state exist, flatten it.
            defaultItem->synthesis();
         else {
            Msg(MSG_DBG) << "   ==> No synthesis full case, no fullState, and no default state, to Check !!" << endl;
            //_exp->whoAmI(6, 6);
         }
      }
   }
}

bool
VlpCaseNode::preprocess(const CktOutPin* const& opinState, const VlpCaseItemNode*& defaultItem) const
{
   if (_caseType == CASETYPE_CASE) {
      int stateCtr = 0;//count states, exclude "default" state
      defaultItem = getDefault_sNum(stateCtr);//get default state and get number of states
      return isFullState(opinState, stateCtr);//determine if full statements
   }
   else if (_caseType == CASETYPE_CASEX)
      return isFullStateX(opinState, defaultItem);//get default state and return isFullState
   else {
      Msg(MSG_ERR) << "casez is still not finished." << endl;
      return false;
   }
}

void
VlpCaseNode::synCaseItem(CktOutPin* opinState, bool fullState, const VlpCaseItemNode* defaultItem) const
{
   CktOutPin* itemOutPin;
   CktOutPin* eqCellOut;
   SynCondState* cds; //cas: for case node.
   bool isAddCondState;
   const VlpCaseItemNode* itemPtr = static_cast<const VlpCaseItemNode*>(getInner());// get caseItem block
   if (itemPtr->isDefaultItem())//skip default state
      itemPtr = itemPtr->getNext_NonDefault(); 

   while (itemPtr != 0) { //for each case item.
      itemOutPin = itemPtr->getExp()->synthesis();
      eqCellOut  = genEqCell(opinState, itemOutPin);
      //!(fullState && the last item)
      if (!((_isFullCase || fullState) && itemPtr->getNext_NonDefault() == 0))
         isAddCondState = true;
      else if (!fullState && defaultItem != NULL)
         isAddCondState = true;
      else
         isAddCondState = false;

      if (isAddCondState) {
         cds = new SynCondState;
         cds->setCtrlNode( new VlpEqualityNode(EQUALITY_LOG_EQU, 
                           const_cast<VlpBaseNode*>(getExp()), 
                           const_cast<VlpBaseNode*>(itemPtr->getExp()) ) );
         cds->setCtrlCond(eqCellOut);
         cds->setState(true);
         SYN->synCondStack.push_back(cds);
      }
      itemPtr->synthesis();
      cds->setState(false);
      itemPtr = itemPtr->getNext_NonDefault();
   }
}

void
VlpCaseNode::synCasexItem(CktOutPin* opinState, bool fullState, const VlpCaseItemNode* defaultItem) const
{
   // condition 1 : casex (3'b011) or casex (3'b01X) or case ({2'b0x, 3'b01x})
   if (opinState->isConst()) 
      synXItem1(opinState, fullState, defaultItem);
   // condition 2 : casex (stateVar)
   else
      synXItem2(opinState, fullState, defaultItem);
}

void // condition 1 : casex (3'b011) or casex (3'b01X) or case ({2'b0x, 3'b01x})
VlpCaseNode::synXItem1(CktOutPin* bvOut, bool fullState, const VlpCaseItemNode* defaultItem) const
{
   CktOutPin* itemOutPin;
   CktOutPin* eqCellOut;
   SynCondState* cds; //cds: for case node.
   bool isAddCondState;
   const VlpCaseItemNode* itemPtr = static_cast<const VlpCaseItemNode*>(getInner());// get caseItem block
   if (itemPtr->isDefaultItem())//skip default state
      itemPtr = itemPtr->getNext_NonDefault(); 

   if (bvOut->isConstHasX()) { //casex(3'b01X) or casex({2'b0x, 3'b01x})
      Msg(MSG_WAR) << "Using casex(3'bx01) => need verify and test if exist bug!!" << endl;
      while (itemPtr != 0) {
         itemOutPin = itemPtr->getExp()->synthesis();
         eqCellOut = genEqCellX(itemOutPin, bvOut);
         //!(fullState && the last item)
         if (!((_isFullCase || fullState) && itemPtr->getNext_NonDefault() == 0))
            isAddCondState = true;
         else if (!fullState && defaultItem != NULL)
            isAddCondState = true;
         else
            isAddCondState = false;

         if (isAddCondState) {
            cds = new SynCondState;
            cds->setCtrlNode( new VlpEqualityNode(EQUALITY_LOG_EQU, 
                              const_cast<VlpBaseNode*>(getExp()), 
                              const_cast<VlpBaseNode*>(itemPtr->getExp()) ) );
            cds->setCtrlCond(eqCellOut);
            cds->setState(true);
            SYN->synCondStack.push_back(cds);
         }
         itemPtr->synthesis(); // useless return value
         cds->setState(false);
      }
   }
   else  //casex(3'b011) or casex({1'b0, 2'b11}), same as case (3'b011)
      synCaseItem(bvOut, fullState, defaultItem);     
}

void // condition 2 : casex (stateVar)
VlpCaseNode::synXItem2(CktOutPin* opinState, bool fullState, const VlpCaseItemNode* defaultItem) const
{
   CktOutPin *itemOutPin, *eqCellOut;
   SynCondState* cds; //cds: for case node.
   const VlpCaseItemNode* itemPtr = static_cast<const VlpCaseItemNode*>(getInner());// get caseItem block
   bool isAddCondState;

   if (itemPtr->isDefaultItem())//skip default state
      itemPtr = itemPtr->getNext_NonDefault(); 

   while (itemPtr != 0) {
      if (itemPtr->isItemHasX()) { //caseItem => 2'bx1 (exist X)
         const VlpBaseNode* bNode = itemPtr->getExp();
         itemOutPin = bNode->synthesis();
         eqCellOut  = genEqCellX(opinState, itemOutPin);
      }
      else {                                   //caseItem => 2'b01  (no X)
         itemOutPin = itemPtr->getExp()->synthesis();
         eqCellOut  = genEqCell(opinState, itemOutPin);
      }
      //!(fullState && the last item)
      if (!((_isFullCase || fullState) && itemPtr->getNext_NonDefault() == 0)) 
         isAddCondState = true;
      else if (!fullState && defaultItem != NULL)
         isAddCondState = true;
      else
         isAddCondState = false;

      if (isAddCondState || defaultItem != NULL) {
         //getExp()->whoAmI(6 ,6);
         cds = new SynCondState;
         cds->setCtrlNode( new VlpEqualityNode(EQUALITY_LOG_EQU, 
                           const_cast<VlpBaseNode*>(getExp()), 
                           const_cast<VlpBaseNode*>(itemPtr->getExp()) ) );
         cds->setCtrlCond(eqCellOut);
         cds->setState(true);
         SYN->synCondStack.push_back(cds);
      }
      itemPtr->synthesis(); // useless return value
      cds->setState(false);
      itemPtr = itemPtr->getNext_NonDefault();
   }   
}

void
VlpCaseNode::synCasezItem(CktOutPin* opinState, bool fullState, const VlpCaseItemNode* defaultItem) const
{
   Msg(MSG_ERR) << "Still not implemet the \"casez\" !!!" << endl;
}

//synthesis and transform into SynVar and synCond
void
VlpCaseNode::varFlatten() const
{
   CktOutPin* opinState = getExp()->synthesis();
   const VlpCaseItemNode* defaultItem = 0;
   bool fullState = preprocess(opinState, defaultItem);//get isFullState and defaultItem
   int oriCondNum = SYN->synCondStack.size();

   if (_caseType == CASETYPE_CASE) 
      synCaseItem(opinState, fullState, defaultItem);
   else if (_caseType == CASETYPE_CASEX)
      synCasexItem(opinState, fullState, defaultItem);
   else //(_caseType == CASETYPE_CASEZ)
      synCasezItem(opinState, fullState, defaultItem);

   handleDefault(fullState, defaultItem);//cds->_state is false

   // clear conditions generated by case node
   // ef. if ( in1 )
   //     begin
   //        case( state )
   //           1'b0:...
   //           1'b1:...
   //     end
   //     synCondStack[0]: in1
   //     synCondStack[1]: state==1'b0
   //     after case node has been flatten, we should remove conditions generated by case node.
   for (int i = SYN->synCondStack.size() - 1; i >= oriCondNum; i--)
      SYN->synCondStack.pop_back();
}

CktOutPin*
VlpCaseNode::genEqCellX(CktOutPin* signal, CktOutPin* bvOut) const
{ //genEqCell for x, don't care
   assert (bvOut->getCell()->getCellType() == CKT_CONST_CELL);
   assert (VLDesign.getBus(signal->getBusId())->getWidth() 
        == VLDesign.getBus(bvOut->getBusId())->getWidth());
   CktOutPin *eqRight, *eqLeft, *eq;
   PinAry andList;
   int busIdL, busIdR;
   unsigned bitSt = 0;
   int begin = VLDesign.getBus(signal->getBusId())->getBegin();
   int end   = VLDesign.getBus(signal->getBusId())->getEnd();
   Bv4* bv = static_cast<const CktConstCell*>(bvOut->getCell())->getBvValue();
   assert (!bv->fullx());
   bool cond1, cond2;

   for (unsigned i = 0; i < bv->bits(); ++i) { // From LSB to MSB
      if ((*bv)[i] == _BV4_Z)
         Msg(MSG_ERR) << "Use high impendence Z in casex !!" << endl;
      assert ((*bv)[i] != _BV4_Z);
      
      cond1 = ((*bv)[i] == _BV4_0 || (*bv)[i] == _BV4_1) && (i == bv->bits()-1); // MSB
      cond2 = ((*bv)[i] == _BV4_X) && (i > bitSt); // split when meeting X and width > 0
      if (cond1) ++i; // to similar as cond2

      if (cond1 || cond2) {
         if (VLDesign.getBus(signal->getBusId())->isInverted() == true)
            busIdL = VLDesign.genBusId((i-bitSt), begin+bitSt, begin+i-1, true);
         else
            busIdL = VLDesign.genBusId((i-bitSt), end-i+1, end-bitSt, false);

         busIdR = VLDesign.genBusId((i-bitSt), bitSt, i-1, true);
         eqRight =  bvOut->getSplitPin(busIdR);
         eqLeft  = signal->getSplitPin(busIdL);
         eq = genEqCell(eqLeft, eqRight);
         andList.insert(eq, true);
      }
      if ((*bv)[i] == _BV4_X)
         bitSt = i+1;
   }
   return andList.synAnd2Gate();
}

CktOutPin*
VlpCaseNode::genEqCell(CktOutPin* oLeft, CktOutPin* oRight) const
{
   CktOutPin* eqOut;
   CktInPin *iLeft, *iRight;
   oLeft->checkBusWidth(oRight);
   eqOut = SYN->outPinMap.getSharPin(CKT_EQUALITY_CELL, oLeft, oRight);
   if (eqOut != 0)  return eqOut;
   
   CktEqualityCell* eqCell = new CktEqualityCell(true, iLeft, iRight, eqOut);
   eqCell->setLogicEq();
   SYN->insertCktCell(eqCell);
   oLeft->connInPin(iLeft);   iLeft->connOutPin(oLeft);
   oRight->connInPin(iRight); iRight->connOutPin(oRight);

   int id = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
   eqOut->setBusId(id);
   SYN->outPinMap.insert(CKT_EQUALITY_CELL, oLeft, oRight, eqOut);
   return eqOut;
}

//=================================== class VlpForNode  ==================================//
void
VlpForNode::varFlatten() const
{
   // only support "for (i = const; i '<' '>' '<=' '>=' const; i = i '+' '-' const)"
   // 1. initState :
   assert (_initState->getNodeClass() == NODECLASS_BA_OR_NBA);
   const VlpBAorNBA_Node* init = static_cast<const VlpBAorNBA_Node*>(_initState);
   assert (init->getLeft()->getNodeClass() == NODECLASS_SIGNAL);
   const VlpSignalNode* initLeft = static_cast<const VlpSignalNode*>(init->getLeft());
   string ctrName = BaseModule :: getName(initLeft->getNameId());
   const VlpBaseNode* initRight  = init->getRight();
   int initValue = initRight->constPropagate();
   // 2. endCond
   assert (_endCond->getNodeClass() == NODECLASS_OPERATOR);
   const VlpOperatorNode* end  = static_cast<const VlpOperatorNode*>(_endCond);
   assert (end->getOperatorClass() == OPCLASS_RELATION);
   const VlpRelateOpNode* rOp  = static_cast<const VlpRelateOpNode*>(end);
   const VlpBaseNode* endRight = rOp->getRight();
   int endValue  = endRight->constPropagate();
   // 3. variation
   assert (_increState->getNodeClass() == NODECLASS_BA_OR_NBA);
   const VlpBAorNBA_Node* var  = static_cast<const VlpBAorNBA_Node*>(_increState);
   assert (var->getRight()->getNodeClass() == NODECLASS_OPERATOR);
   const VlpOperatorNode* varRight = static_cast<const VlpOperatorNode*>(var->getRight());
   assert (varRight->getOperatorClass() == OPCLASS_ARITHMETIC);
   const VlpArithOpNode* arith = static_cast<const VlpArithOpNode*>(varRight);
   int varValue = arith->getRight()->constPropagate();
   int isAdd;
   if (arith->getArithOpClass() == ARITHOP_SUBTRACT) isAdd = 0;
   else if (arith->getArithOpClass() == ARITHOP_ADD) isAdd = 1;
   else assert (0);

   SynForArgu forArgu(ctrName, initValue, endValue, varValue, isAdd, rOp->getRelOpClass());
   SYN->synForArguArray.push_back(forArgu);

   while (SYN->synForArguArray.back().isRepeat()) {
      for (const VlpBaseNode* inner = _inner; inner; inner = inner->getNext())
         inner->varFlatten();  //flatten inner statements
      SYN->synForArguArray.back().update();
   }
   SYN->synForArguArray.pop_back();
}

//================================= class VlpAssignNode  =================================//
void
VlpAssignNode::synthesis(bool& isSeq, bool& isSync, CktOutPin*& oClk) const
{
   isSeq  = false; 
   isSync = false; 
   oClk   = NULL;
   const VlpBaseNode *left, *right, *ptr;
   VlpPortNode* port;
   string name;
   for (ptr = getExp(); ptr; ptr = ptr->getNext()) {
      left  = (static_cast<const VlpBAorNBA_Node*>(ptr))->getLeft();
      right = (static_cast<const VlpBAorNBA_Node*>(ptr))->getRight();
      if (left->getNodeClass() == NODECLASS_SIGNAL) {
         if (right->getNodeClass() == NODECLASS_SIGNAL) { // legal checking
            name = BaseModule :: getName(static_cast<const VlpSignalNode*>(left)->getNameId());
            curVlpModule->getPort(name, port);                
            assert(port->getIoType() != input);
         }
         ptr->varFlatten();
      }
      else { // lhs is a concatenate operator
         assert (left->getNodeClass() == NODECLASS_OPERATOR); 
         assert (static_cast<const VlpOperatorNode*>(left)->getOperatorClass() == OPCLASS_CONCATENATE);          
         ptr->varFlatten();
      }
   }
}

#endif
