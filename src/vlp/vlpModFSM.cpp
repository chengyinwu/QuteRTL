/****************************************************************************
  FileName     [ vlpModFSM.cpp ]
  Package      [ vlp ]
  Synopsis     [ FSM Member function of Module Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2008 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_MOD_FSM_SOURCE
#define VLP_MOD_FSM_SOURCE

#include <algorithm>
#include "vlpModule.h"

using namespace std;
extern const VlgModule* curVlpModule;

//------------------------------------------------------------------
//Extract FSM Fumction
//------------------------------------------------------------------

void
VlgModule::setSIGS(vector<VlpFSMSIG>& retSIG, map<MyString, VlpFSMSIG>& SIGS) const
{   
   set<MyString> PSR;
   VlpFSMAssign assign;
   for (unsigned i = 0; i < retSIG.size(); ++i) {
      if (retSIG[i].sizeADS() == 0)
         retSIG[i].addADS(retSIG[i].name());
      SIGS.insert(pair<MyString, VlpFSMSIG>(retSIG[i].name(), retSIG[i]));
      for (unsigned j = 0; j < retSIG[i].sizeAL(); ++j) {
         assign = retSIG[i].getAL(j);
         if (assign.isSeq())
            PSR.insert(assign.strTS());
      }
   }
//==========================================================================//
   MyString sigName, coSIG;
   for (set<MyString>::iterator pos = PSR.begin(); pos != PSR.end(); pos++) {
      sigName = *pos;
      for (unsigned i = 0; i < SIGS[sigName].sizeADS(); ++i) {
         coSIG = SIGS[sigName].getADS(i);
         if (SIGS[coSIG].inACS(sigName) && SIGS[coSIG].sizeADS() == 1) {
            SIGS[coSIG].setEX();
            SIGS[coSIG].setCoSIG(sigName);
         }
         else if (SIGS[coSIG].inACS(sigName) && SIGS[coSIG].sizeADS() > 1) {
            SIGS[coSIG].setEX();
            SIGS[coSIG].setCoSIG(sigName);
            SIGS[sigName].setIMP();
            SIGS[sigName].setCoSIG(coSIG);
         }
      }
      for (unsigned i = 0; i < SIGS[sigName].sizeACS(); ++i) {
         coSIG = SIGS[sigName].getACS(i);
         if (SIGS[coSIG].inADS(sigName) && SIGS[coSIG].sizeADS() == 1) {
            SIGS[sigName].setEX();
            SIGS[sigName].setCoSIG(coSIG);
         }
      }
   }
//===========================================================================//   
   for (set<MyString>::iterator pos = PSR.begin(); pos != PSR.end(); pos++) {
      sigName = *pos;
      for (unsigned i = 0; i < SIGS[sigName].sizeADS(); ++i) {
         coSIG = SIGS[sigName].getADS(i);
         if (SIGS[coSIG].inADS(sigName)) {//&& SIGS[coSIG].sizeADS() > 1 ) {
            if (SIGS[coSIG].isEX() == false) {
               SIGS[coSIG].setIMP();
               SIGS[coSIG].setCoSIG(sigName);
            }
         }
      }
   }     
}

vector<VlpCU*>
VlgModule::extractCU(map<MyString, VlpFSMSIG>& SIGS) const
{
   vector<MyString>  cus;
   MyString sigName;
   cout << "> Possible state register of explicit CU: ";
   for (map<MyString, VlpFSMSIG>::iterator pos = SIGS.begin(); pos != SIGS.end(); pos++) {
      sigName = (pos->second).name();
      if (SIGS[sigName].isEX())  {
         cout << sigName << " ";
         cus.push_back(sigName);
      }
   }
   cout << endl << "> Possible state register of implitic CU: ";
   for (map<MyString, VlpFSMSIG>::iterator pos = SIGS.begin(); pos != SIGS.end(); pos++) {
      sigName = (pos->second).name();
      if (SIGS[sigName].isIMP()) {
         cout << sigName << " ";
         cus.push_back(sigName);
      }
   }
   cout << endl;
//============================================================//  
   vector<VlpCU*> exCU  = genExFSM(SIGS);
   vector<VlpCU*> impCU = genImpFSM(SIGS);
//============================================================//  
   vector<VlpCU*> composeCU;   
   map<VlpCU*, bool> removeMark;
   VlpFSM    *sigCU, *sigCoCU;
   VlpFSMSIG tmpSIG, tmpSIGco;
   
   for (unsigned i = 0; i < impCU.size(); ++i)
      exCU.push_back(impCU[i]);

   for (unsigned i = 0; i < cus.size(); ++i) {
      tmpSIG = SIGS[cus[i]];
      tmpSIGco = SIGS[tmpSIG.coSIG()];

      if ((tmpSIG.isEX() && tmpSIGco.isIMP()) || (tmpSIG.isIMP() && tmpSIGco.isEX())) {
         sigCU   = NULL;
         sigCoCU = NULL;
         for (unsigned j = 0; j < exCU.size(); ++j) {
            if (exCU[j]->name() == tmpSIG.name() && exCU[j]->type() == VLPCU_FSM)
               sigCU = static_cast<VlpFSM*>(exCU[j]);
            if (exCU[j]->name() == tmpSIGco.name() && exCU[j]->type() == VLPCU_FSM)
               sigCoCU = static_cast<VlpFSM*>(exCU[j]);
         }

         if ((sigCU != NULL && sigCoCU != NULL)) {
            if (removeMark.find(sigCU) != removeMark.end() || removeMark.find(sigCoCU) != removeMark.end())
               break;

            cout << "> Compose " << tmpSIG.name() << " and " << tmpSIGco.name() << endl;
            removeMark[sigCU] = true;
            removeMark[sigCoCU] = true;
//            sigCU->genPNG( "m1.png" );
//            sigCoCU->genPNG( "m2.png" );
            if (tmpSIG.inADS(tmpSIGco.name()))
               composeCU.push_back( sigCU->compose(sigCoCU) );
            else if (tmpSIGco.inADS(tmpSIG.name()))
               composeCU.push_back( sigCoCU->compose(sigCU) );
         }
      }
   }
   for (unsigned i = 0; i < composeCU.size(); ++i)
      exCU.push_back(composeCU[i]);
   
   impCU.clear();
   for (unsigned i = 0; i < exCU.size(); ++i) {
      if (removeMark.find(exCU[i]) == removeMark.end()) {
         impCU.push_back(exCU[i]);
         cout << "> CU Name: " << exCU[i]->name() << endl;
/*       MyString fileName;
         if ( exCU[i]->type() == VLPCU_FSM ) {
            fileName  = "FSM_";
            fileName += numFSM;
            exCU[i]->genPNG( fileName + ".png" );
            numFSM++;
         }
         else if ( exCU[i]->type() == VLPCU_CNT ) {
            fileName  = "CNT_";
            fileName += numCNT;
            exCU[i]->genPNG( fileName + ".png" );
            numCNT++;
         }*/
      }
   }
   cout << "Totally, " << impCU.size() << " control units extracted in this module" << endl;
   return impCU;
}

vector<VlpCU*>
VlgModule::extract() 
{
   if (_CUs.size() > 0) 
      return _CUs;
   
   //static int      numFSM, numCNT;
   map<MyString, VlpFSMSIG> SIGS;
   SIGS.clear();
   
   curVlpModule = this;
   vector<VlpFSMSIG> retSIG = extractSIG();
   setSIGS(retSIG, SIGS); 
   _CUs = extractCU(SIGS);
   
   return _CUs;
}

vector<VlpCU*>
VlgModule::CUs() const 
{
   return _CUs;
}

vector<VlpFSMSIG>
VlgModule::extractSIG() const 
{
   const VlpBaseNode* curNode = _dataFlowStart;
   setSigBus();  
   vector<VlpFSMAssign> local;

   cout << "Module " << _moduleName << ": Extract AL" << endl;
   unsigned c = 0;                        //only used in funCall node
   map<int, int>   nameMap;               //only used in funCall node
   map<int, const VlpSignalNode*> sigMap; //only used in funCall node
   const VlpBaseNode *tmpNode, *intNode;
   const VlpAssignNode*      assignNode;
   const VlpBAorNBA_Node*    banbaNode;
   const VlpConditionalNode* condNode;
   const VlpOperatorNode*    oprNode;
   const VlpFunCall*         funcNode;
   const VlpConnectNode*     connNode;
   VlpFunction*              tmpFunc;
   const IOAry*              funcIO;
   stack<size_t>             condStack;
   stack<const VlpBaseNode*> backNode;
   VlpFSMAssign              newAL;
   unsigned int              nId1, nId2;
   vector<VlpFSMAssign>      bufAL;
   local.clear();
   
   size_t tmpCond;
   while (curNode != NULL) {
      switch (curNode->getNodeClass()) {
      case NODECLASS_ALWAYS:
         bufAL = static_cast<const VlpAlwaysNode*>(curNode)->extractAL();
         for (unsigned i = 0; i < bufAL.size(); ++i) {
            newAL = bufAL[i];
            local.push_back(newAL);
         }
         bufAL.clear();
         break;
      case NODECLASS_ASSIGN:
         assignNode = static_cast<const VlpAssignNode*>(curNode);
         newAL.reset();
         cout << "> From ASSIGN Statement( " << curNode << " )" << endl;
         banbaNode = static_cast<const VlpBAorNBA_Node*>(assignNode->getExp());
         newAL.setTS(banbaNode->getLeft());
         tmpNode = banbaNode->getRight();
         switch (tmpNode->getNodeClass()) {
         case NODECLASS_OPERATOR:
            if (static_cast<const VlpOperatorNode*>(tmpNode)->getOperatorClass() == OPCLASS_CONDITIONAL) {
               intNode = tmpNode;
               while (condStack.size() > 0) // reset
                  condStack.pop();
               while (backNode.size() > 0)  // reset
                  backNode.pop();

               do {
                  switch (intNode->getNodeClass()) {
                  case NODECLASS_OPERATOR:
                     oprNode = static_cast<const VlpOperatorNode*>(intNode);
                     if (oprNode->getOperatorClass() == OPCLASS_CONDITIONAL) {
                        condNode = static_cast<const VlpConditionalNode*>(oprNode);
                        condStack.push(size_t(condNode->getCondition()));
                        backNode.push(condNode->getFalse());
                        intNode = condNode->getTrue();
                     }
                     else {
                        newAL.setAC(condStack);
                        newAL.setSE(intNode);
                        local.push_back(newAL);
                        intNode = NULL;
                     }
                     break;
                  case NODECLASS_INT:
                  case NODECLASS_PARAMETER:
                  case NODECLASS_SIGNAL:
                     newAL.setAC(condStack);
                     newAL.setSE(intNode);
                     local.push_back(newAL); 
                     tmpCond = condStack.top();
                     condStack.pop();
                     if (backNode.size() != 0) {
                        condStack.push(tmpCond | 0x00000001);
                        intNode = backNode.top();
                        backNode.pop();
                     }
                     else {
                        intNode = NULL;
                     }
                     break;
                  default:
                     cerr << "VlgModule::extractAL() ---> Unsupport "
                          << "operator for conditional operator." << endl;
                     intNode = NULL;
                     break;
                  }
               } while (intNode != NULL);
            }
            else {
               newAL.setAC(condStack);
               newAL.setSE(tmpNode);
               local.push_back(newAL);
            }
            break;
         case NODECLASS_FUNCALL:
            funcNode = static_cast<const VlpFunCall*>(tmpNode);
            _functionMap.getData(funcNode->nameId(), tmpFunc);
            nId1 = static_cast<const VlpSignalNode*>(tmpFunc->outputNode())->getNameId();
            nId2 = static_cast<const VlpSignalNode*>(newAL.getTS())->getNameId();
            newAL.setSE(tmpFunc->outputNode());
            newAL.setAC(condStack);
            local.push_back(newAL);

            cout << " > Connect Function Block: " << endl;
            nameMap.clear();
            sigMap.clear();
            nameMap.insert(pair<int, int>(nId1, nId2));
            sigMap.insert(pair<int, const VlpSignalNode*>
                          (nId2, static_cast<const VlpSignalNode*>(newAL.getTS())) );
            connNode = static_cast<const VlpConnectNode*>(funcNode->arguNode());
            funcIO   = tmpFunc->getIoAry();
            c        = 0;
            do {
               tmpNode = (*funcIO)[c++];
               newAL.setSE(static_cast<const VlpConnectNode*>(connNode->getExp()));
               newAL.setTS(tmpNode);
               newAL.setAC(condStack);
               local.push_back(newAL);
               if ( (connNode->getExp())->getNodeClass() == NODECLASS_SIGNAL ) {
                  nId1 = static_cast<const VlpSignalNode*>(tmpNode)->getNameId();
                  nId2 = static_cast<const VlpSignalNode*>(connNode->getExp())->getNameId();
                  nameMap.insert(pair<int, int>(nId1, nId2));
                  sigMap.insert(pair<int, const VlpSignalNode*>
                                (nId2, static_cast< const VlpSignalNode*>(newAL.getTS())) );
               }
               connNode = static_cast<const VlpConnectNode*>(connNode->getNext());
            } while (connNode != NULL);

            bufAL = tmpFunc->extractAL(nameMap, sigMap);
            for (unsigned i = 0; i < bufAL.size(); ++i)
               local.push_back(bufAL[i]);
            break;
         default:
            newAL.setSE(tmpNode);
            local.push_back(newAL);
            break;
         }
         break;
      default:
         cerr << "VlgModule::extractAL() ---> ERROR: Only support ASSIGN"
              << " , FUNCTION call, and ALWAYS block." << endl;
         break;
      }
      curNode = curNode->getNext();
   }
   cout << "Module " << _moduleName << ": Extraction complete, "
        << local.size() << " ALs extracted." << endl;
//==========================================================================//
   map<MyString, VlpFSMSIG> SIGS;
   SIGS.clear();
   VlpFSMSIG tmpSIG;
   Bv4 tmpBv;
   MyString sigName;
   VlpPortNode* port;
   for (unsigned i = 0; i < local.size(); ++i) {
      sigName = local[i].strTS();
      if (SIGS.find(sigName) == SIGS.end()) {
         tmpSIG.setName(sigName);
         SIGS.insert(pair<MyString, VlpFSMSIG>(sigName, tmpSIG));
         nId1 = static_cast<const VlpSignalNode*>(local[i].getTS())->getNameId();
         if (_portMap.getData(nId1, port)) {
            tmpNode = port->getMsb();
            if (tmpNode != NULL) {
               tmpBv = (tmpNode->str()).str();
               SIGS[sigName].setMSB(tmpBv.value());
            }
            else  // not declare, default: 1bit wire
               SIGS[sigName].setMSB(0);
            tmpNode = port->getLsb();
            if (tmpNode != NULL) {
               tmpBv = (tmpNode->str()).str();
               SIGS[sigName].setLSB(tmpBv.value());
            }
            else  // not declare, default: 1bit wire
               SIGS[sigName].setLSB(0);
         }
         else { // not declare, default: 1bit wire
            SIGS[sigName].setMSB(0);
            SIGS[sigName].setLSB(0);
         }
      }
      SIGS[sigName].addAssign(local[i]);
   }
   
   vector<VlpFSMSIG> retSIGS;
   for (map<MyString, VlpFSMSIG>::iterator pos = SIGS.begin(); pos != SIGS.end(); pos++) 
      retSIGS.push_back(pos->second);

   return retSIGS;
}

vector<VlpCU*>
VlgModule::genExFSM(const map<MyString, VlpFSMSIG>& SIGS) const 
{
   VlpFSMSIG          sig, sigCo;
   vector<VlpCU*>     local;
   VlpFSM*            newFSM;
   VlpFSMState        *curState, *nxtState;
   VlpFSMTrans        fsmTrans;
   VlpFSMCond         fsmCond;
   VlpFSMAssign       assign;
   
   const VlpBaseNode* node;
   size_t             acSizeT;
   size_t*            ptrCond;
   vector<size_t>     ac;
   bool               isNeg;
   MyString           strCurState, strNxtState, fileName;
   vector<MyString>   Counter, reset;

   for (map<MyString, VlpFSMSIG>::const_iterator pos = SIGS.begin(); pos != SIGS.end(); pos++) {
      sig = pos->second;
      if (sig.isEX()) {
         newFSM = NULL;
         cout << "> Explicit FSM Generation, SR: " << sig.name() << endl;
         for (unsigned i = 0; i < sig.sizeAL(); ++i) {
            assign = sig.getAL(i);
            if (assign.SEset().size() <= 1) {
               if (newFSM == NULL) newFSM = new VlpFSM;

               newFSM->setName(sig.name());
               ac = assign.getAC();
               strCurState = "";
               fsmTrans.reset();
               fsmCond.reset();
               // handle ac
               for (unsigned j = 0; j < ac.size(); ++j) {
                  if ((ac[j] & 0x00000001) > 0) isNeg = true;
                  else                          isNeg = false;

                  acSizeT = ac[j] & 0xFFFFFFFE;
                  ptrCond = (size_t*)&node;
                  *ptrCond = acSizeT;
                  fsmCond.reset();
                  if (node->getNodeClass() == NODECLASS_SIGNAL) {
                     fsmCond.setOP(class2CUOP(node, isNeg));
                     fsmCond.setLeft(node->str());
                     fsmTrans.addCond(fsmCond);
                  }
                  else if (node->getNodeClass() == NODECLASS_OPERATOR) {
                     const VlpBaseNode *left, *right;
                     switch (static_cast<const VlpOperatorNode*>(node)->getOperatorClass()) {
                     case OPCLASS_EQUALITY:
                        left  = static_cast<const VlpEqualityNode*>(node)->getLeft();
                        right = static_cast<const VlpEqualityNode*>(node)->getRight();

                        if (static_cast<const VlpSignalNode*>(left)->strNoIndex() == sig.coSIG()) {
                           if (right->getNodeClass() == NODECLASS_PARAMETER) 
                              strCurState = static_cast<const VlpParamNode*>(right)->strName();
                           else if (right->getNodeClass() == NODECLASS_SIGNAL)
                              strCurState = static_cast<const VlpSignalNode*>(right)->strNoIndex();
                           else 
                              ;
                        }
                        else {
                           fsmCond.setOP(class2CUOP(node, isNeg));
                           fsmCond.setLeft(left->str());
                           fsmCond.setRight(right->str());
                           fsmTrans.addCond(fsmCond);
                        }
                        break;
                     case OPCLASS_RELATION:
                        left  = static_cast<const VlpRelateOpNode*>(node)->getLeft();
                        right = static_cast<const VlpRelateOpNode*>(node)->getRight();
                        fsmCond.setOP(class2CUOP(node, isNeg));
                        fsmCond.setLeft(left->str());
                        fsmCond.setRight(right->str());
                        fsmTrans.addCond(fsmCond);
                        break;
                     case OPCLASS_LOGIC:
                        fsmCond.setOP(class2CUOP(node, isNeg));
                        if (static_cast<const VlpLogicOpNode*>(node)->getLogicClass() == LOGICOP_NOT) {
                           left = static_cast<const VlpLogicNotOpNode*>(node)->getOprn();
                           fsmCond.setLeft(left->str());
                        }
                        else {
                           left  = static_cast<const VlpLogicOpNode*>(node)->getLeft();
                           right = static_cast<const VlpLogicOpNode*>(node)->getRight();
                           fsmCond.setLeft(left->str());
                           fsmCond.setRight(right->str());
                        }
                        fsmTrans.addCond(fsmCond);
                        break;
                     case OPCLASS_BITWISE:
                        fsmCond.setOP( class2CUOP(node, isNeg));
                        fsmCond.setLeft(node->str());
                        fsmTrans.addCond(fsmCond);
                        break;
                     default:
                        break;
                     }
                  }
               }
               // handle se
               node = assign.getSE();
               if (node->getNodeClass() == NODECLASS_PARAMETER) 
                  strNxtState = static_cast<const VlpParamNode*>(node)->strName();
               else if (node->getNodeClass() == NODECLASS_SIGNAL) {
                  strNxtState = static_cast<const VlpSignalNode*>(node)->strNoIndex();

                  if (sig.inADS(strNxtState))    strNxtState = strCurState;
                  if (strNxtState == sig.name()) strNxtState = strCurState;
               }
               else {
                  delete newFSM;
                  newFSM = NULL;
                  Counter.push_back(sig.name());
                  break;
               }

               if (strCurState != "") {
                  curState = newFSM->getState(strCurState);
                  if (curState == NULL) {
                     curState = new VlpFSMState(strCurState);
                     newFSM->addState(curState);
                  }
                  nxtState = newFSM->getState(strNxtState);
                  if (nxtState == NULL) {
                     nxtState = new VlpFSMState(strNxtState);
                     newFSM->addState(nxtState);
                  }
                  fsmTrans.setNextState(nxtState);
                  curState->addTrans(fsmTrans);
                  cout << "  > New trans from state " << strCurState << " to state " << strNxtState << endl;
               }
               else { // reset state;
                  reset.push_back(strNxtState);
                  cout << "  > Reset state " << strNxtState << endl;
               }
            }
            else {
               cout << "  > Faild to gernerate STG, case 2" << endl;
               cout << assign << endl;
               delete newFSM;
               newFSM = NULL;
               break;
            }
         }
//======================================================================================//
         if (newFSM != NULL) {
            if (newFSM->numState() > 1) {
               newFSM->setExType();
               newFSM->setMSB(sig.MSB());
               newFSM->setLSB(sig.LSB());
               for (unsigned i = 0; i < reset.size(); ++i) {
                  nxtState = newFSM->getState(reset[i]);
                  if (nxtState == NULL) {
                     nxtState = new VlpFSMState(strNxtState);
                     newFSM->addState(nxtState);
                  }
                  curState = newFSM->resetState();
                  fsmTrans.reset();
                  fsmTrans.setNextState(nxtState);
                  curState->addTrans(fsmTrans);
               }

               map<MyString, VlpFSMSIG>::const_iterator posCo = SIGS.find(sig.coSIG());
               if (posCo != SIGS.end() && posCo->first != pos->first) {
                  sigCo = posCo->second;
                  for (unsigned i = 0; i < sigCo.sizeAL(); ++i) {
                     assign = sigCo.getAL(i);
                     node = assign.getSE();
                     if (node->getNodeClass() == NODECLASS_INT) {
                        if (sig.MSB() > sig.LSB()) strNxtState = sig.MSB() - sig.LSB() + 1;
                        else                       strNxtState = sig.LSB() - sig.MSB() + 1;
                        strNxtState += "'d";
                        strNxtState += static_cast<const VlpIntNode*>(node)->getNum();
                        Bv4 tmpBv = strNxtState.str();
                        nxtState = newFSM->getState(tmpBv.str());
                        if (nxtState == NULL) {
                           nxtState = new VlpFSMState(strNxtState);
                           newFSM->addState(nxtState);
                        }
                        curState = newFSM->resetState();
                        fsmTrans.reset(); 
                        fsmTrans.setNextState(nxtState);
                        curState->addTrans(fsmTrans);
                     }
                     else if (node->getNodeClass() == NODECLASS_PARAMETER) {
                        strNxtState = static_cast<const VlpParamNode*>(node)->strName();
                        nxtState = newFSM->getState(strNxtState);
                        if (nxtState == NULL) {
                           nxtState = new VlpFSMState(strNxtState);
                           newFSM->addState(nxtState);
                        }
                        curState = newFSM->resetState();
                        fsmTrans.reset(); 
                        fsmTrans.setNextState(nxtState);
                        curState->addTrans(fsmTrans);
                     }
                     else if (node->getNodeClass() == NODECLASS_SIGNAL) {
                        if (static_cast<const VlpSignalNode*>(node)->listSIGs().size() == 0) {
                           strNxtState = static_cast<const VlpSignalNode*>(node)->strNoIndex();
                           nxtState = newFSM->getState(strNxtState);
                           if (nxtState == NULL) {
                              nxtState = new VlpFSMState(strNxtState);
                              newFSM->addState(nxtState);
                           }
                           curState = newFSM->resetState();
                           fsmTrans.reset(); 
                           fsmTrans.setNextState(nxtState);
                           curState->addTrans(fsmTrans);
                        }
                     }
                  }
               }
               newFSM->analysis();
               local.push_back(newFSM);
               cout << "> New FSM added." << endl;
            }
            else
               delete newFSM;

            newFSM = NULL;
         }
//======================================================================================//
      }
   }

   cout << "> Total FSM: " << local.size() << endl;
   vector<VlpCU*> bufCNT = genExCNT(SIGS, Counter);
   for (unsigned i = 0; i < bufCNT.size(); ++i)
      local.push_back(bufCNT[i]);

   return local;
}

vector<VlpCU*>
VlgModule::genExCNT(const map<MyString, VlpFSMSIG>& SIGS, const vector<MyString>& counter) const 
{
   VlpFSMSIG          sig;
   vector<VlpCU*>     local;
   VlpCNT*            newCNT;
   VlpFSMAssign       assign;
   VlpCNTCond         cntCond;
   VlpCNTAction       cntAction;
   size_t             acSizeT;
   size_t*            ptrCond;
   const VlpBaseNode  *node, *left, *right;
   vector<size_t>     ac;
   bool               isNeg;

   map<MyString, VlpFSMSIG>::const_iterator pos;
   for (unsigned i = 0; i < counter.size(); ++i) {
      pos = SIGS.find(counter[i]);
      if (pos == SIGS.end()) continue;

      sig = pos->second;
      newCNT = new VlpCNT(counter[i], true, sig.MSB(), sig.LSB());
      for (unsigned j = 0; j < sig.sizeAL(); ++j) {
         assign = sig.getAL(j);
         ac = assign.getAC();
         cntAction.reset();
         cntCond.reset();
         for (unsigned k = 0; k < ac.size(); ++k) {
            if ( (ac[k] & 0x00000001) > 0 ) isNeg = true;
            else                            isNeg = false;
            acSizeT = ac[k] & 0xFFFFFFFE;
            ptrCond = (size_t*)&node;
            *ptrCond = acSizeT;
            cntCond.reset();
            if (node->getNodeClass() == NODECLASS_SIGNAL) {
               cntCond.setOP(class2CUOP(node, isNeg));
               cntCond.setLeft(node->str());
               cntAction.addCond(cntCond);
            }
            else if (node->getNodeClass() == NODECLASS_OPERATOR) {
               switch (static_cast<const VlpOperatorNode*>(node)->getOperatorClass()) {
               case OPCLASS_EQUALITY:
                  left  = static_cast<const VlpEqualityNode*>(node)->getLeft();
                  right = static_cast<const VlpEqualityNode*>(node)->getRight();
                  cntCond.setOP(class2CUOP(node, isNeg));
                  cntCond.setLeft(left->str());
                  cntCond.setRight(right->str());
                  cntAction.addCond(cntCond);
                  break;
               case OPCLASS_RELATION:
                  left  = static_cast<const VlpRelateOpNode*>(node)->getLeft();
                  right = static_cast<const VlpRelateOpNode*>(node)->getRight();
                  cntCond.setOP(class2CUOP(node, isNeg));
                  cntCond.setLeft(left->str());
                  cntCond.setRight(right->str());
                  cntAction.addCond(cntCond);
                  break;
               case OPCLASS_LOGIC:
                  cntCond.setOP(class2CUOP(node, isNeg));
                  if (static_cast<const VlpLogicOpNode*>(node)->getLogicClass() == LOGICOP_NOT) {
                     left = static_cast<const VlpLogicNotOpNode*>(node)->getOprn();
                     cntCond.setLeft(left->str());
                  }
                  else {
                     left  = static_cast<const VlpLogicOpNode*>(node)->getLeft();
                     right = static_cast<const VlpLogicOpNode*>(node)->getRight();
                     cntCond.setLeft(left->str());
                     cntCond.setRight(right->str());
                  }
                  cntAction.addCond(cntCond);
                  break;
               case OPCLASS_BITWISE:
                  cntCond.setOP(class2CUOP(node, isNeg));
                  cntCond.setLeft(node->str());
                  cntAction.addCond(cntCond);
                  break;
               default:
                  break;
               }
            }
         }
         node = assign.getSE();
         cntAction.setAction(node->str());
         newCNT->addAction(cntAction);
      }
      local.push_back(newCNT);
      cout << "> New Explicit Counter added." << endl;
   }
   return local;
}

int
VlgModule::addBddSupports(BddManager& bm, VlpFSMSIG& sig, const map<MyString, VlpFSMSIG>& SIGS) const
{
   int begin, end;
   MyString sigName;
   string nodeName;
   BddNode tmpNode;
   int top = bm.supportsNum()-1;
   if (sig.MSB() > sig.LSB()) {
      begin = sig.LSB(); end = sig.MSB();
   }
   else {
      begin = sig.MSB(); end = sig.LSB();
   }
   for (int i = begin; i <= end; ++i) {
      sigName = ""; sigName = sig.name();
      if (i >= 0) sigName << '[' << i << ']';

      nodeName = sigName.str();
      tmpNode = bm.getNode(nodeName);
      if (tmpNode == BddNode::_error)
         bm.setSupports(top--, nodeName);
   }

   map<MyString, VlpFSMSIG>::const_iterator posCo = SIGS.find(sig.coSIG());
   if (posCo != SIGS.end()) {
      VlpFSMSIG sigCo = posCo->second;
      if (sigCo.MSB() > sigCo.LSB()) {
         begin = sigCo.LSB(); end = sigCo.MSB();
      }
      else {
         begin = sigCo.MSB(); end = sigCo.LSB();
      }

      for (int i = begin; i <= end; ++i) {
         if (sigCo.name() == sig.name()) sigName = "n_";
         else                            sigName = "";
         sigName += sigCo.name();
         if (i >= 0) sigName << '[' << i << ']';

         nodeName = sigName.str();
         tmpNode = bm.getNode(nodeName);
         if (tmpNode == BddNode::_error)
            bm.setSupports(top--, nodeName);
      }

      if (sig.MSB() > sig.LSB()) {
         begin = sig.LSB(); end = sig.MSB();
      }
      else {
         begin = sig.MSB(); end = sig.LSB();
      }

      for (int i = begin; i <= end; ++i) {
         sigName = "n_";
         sigName += sig.name();
         if (i >= 0)
            sigName << '[' << i << ']';

         nodeName = sigName.str();
         tmpNode = bm.getNode(nodeName);
         if (tmpNode == BddNode::_error)
            bm.setSupports(top--, nodeName);
      }
   }
   return top;
}

vector<VlpCU*>
VlgModule::genImpFSM(const map< MyString, VlpFSMSIG >& SIGS) const
{
   VlpFSMSIG         sig, sigCo;
   vector<VlpCU*>    local;
   vector<MyString>  Counter;
   BddManager        bm(768, 7919, 1039);
   int               top, bottom;
   bool              buildFaild1, buildFaild2;
   vector<VlpFSMAssign> reset;
   for (map<MyString, VlpFSMSIG>::const_iterator pos = SIGS.begin(); pos != SIGS.end(); pos++) {
      sig = pos->second;
      if (sig.isIMP()) {
         cout << "> Constructing BDD for signal: " << sig.name()
              << "[" << sig.MSB() << ":" << sig.LSB() << "]" << endl;
         buildFaild1 = false;
         buildFaild2 = false;
         reset.clear();
         if (abs(sig.MSB()-sig.LSB()) > 16) {
            cout << "  > Too wide, skip" << endl;
            continue;
         }
         top    = addBddSupports(bm, sig, SIGS);
         bottom = 2;
         map<MyString, VlpFSMSIG>::const_iterator posCo = SIGS.find(sig.coSIG());
         if (posCo != SIGS.end())
            sigCo = posCo->second;

         cout << "> Construct Mapping from X --> Y (" << sig.name() << " --> " << sigCo.name() << " )" << endl;
         if (sig.build_X2Y(buildFaild1, Counter, top, bottom, bm))
            continue;      
         if (buildFaild1 == false)
            buildFaild2 = sig.build_Y2Z(sigCo, Counter, reset, top, bottom, bm);
         if (buildFaild1 == false && buildFaild2 == false) {
            VlpFSM* newFSM = sig.build_tr(sigCo, reset, bm);
            local.push_back(newFSM);
            cout << " > New Implicit FSM added" << endl;
         }
         else 
            cout << "  > Construction faild, possibly is Counter" << endl;
         bm.reset();
         bm.init(768, 7919, 1039);
      }
   }
   vector<VlpCU*> bufCNT = genImpCNT(SIGS, Counter);
   for (unsigned i = 0; i < bufCNT.size(); ++i)
      local.push_back(bufCNT[i]);

   return local;
}

vector<VlpCU*>
VlgModule::genImpCNT(const map<MyString, VlpFSMSIG>& SIGS,const vector< MyString >& counter ) const 
{
   map<MyString, VlpFSMSIG>::const_iterator pos;
   unsigned int       i, j, k;
   VlpFSMSIG          sig;
   vector< VlpCU* >   local;
   VlpCNT*            newCNT;
   VlpFSMAssign       assign;
   VlpCNTCond         cntCond;
   VlpCNTAction       cntAction;
   size_t             acSizeT;
   size_t*            ptrCond;
   const VlpBaseNode *node, *left, *right;
   vector< size_t >   ac;
   bool               isNeg, hasArith;
   MyString           name;

   for ( i = 0; i < counter.size(); ++i ) {
      name = counter[i];
      pos = SIGS.find( name );
      if ( pos == SIGS.end() )
         continue;

      sig = pos->second;
      newCNT = new VlpCNT(counter[i], false, sig.MSB(), sig.LSB());
      hasArith = false;
      for ( j = 0; j < sig.sizeAL(); ++j ) {
         assign = sig.getAL(j);

         ac = assign.getAC();
         cntAction.reset();
         cntCond.reset();
         for ( k = 0; k < ac.size(); ++k ) {
            if ( ( ac[k] & 0x00000001 ) > 0 )
               isNeg = true;
            else
               isNeg = false;

            acSizeT = ac[k] & 0xFFFFFFFE;
            ptrCond = (size_t*)&node;
            *ptrCond = acSizeT;

            cntCond.reset();

            if ( node->getNodeClass() == NODECLASS_SIGNAL ) {
               cntCond.setOP( class2CUOP( node, isNeg ) );
               cntCond.setLeft( node->str() );
               cntAction.addCond( cntCond );
            }
            else if ( node->getNodeClass() == NODECLASS_OPERATOR ) {
               switch ( static_cast<const VlpOperatorNode*>(
                                    node)->getOperatorClass() ) {
               case OPCLASS_EQUALITY:
                  left  = static_cast<const VlpEqualityNode*>(node)->getLeft();
                  right = static_cast<const VlpEqualityNode*>(node)->getRight();

                  cntCond.setOP( class2CUOP( node, isNeg ) );
                  cntCond.setLeft( left->str() );
                  cntCond.setRight( right->str() );
                  cntAction.addCond( cntCond );
                  break;
               case OPCLASS_RELATION:
                  left  = static_cast<const VlpRelateOpNode*>(
                                      node)->getLeft();
                  right = static_cast<const VlpRelateOpNode*>(
                                      node)->getRight();
                  cntCond.setOP( class2CUOP( node, isNeg ) );
                  cntCond.setLeft( left->str() );
                  cntCond.setRight( right->str() );
                  cntAction.addCond( cntCond );
                  break;
               case OPCLASS_LOGIC:
                  cntCond.setOP( class2CUOP( node, isNeg ) );
                  if ( static_cast<const VlpLogicOpNode*>(
                                   node)->getLogicClass() == LOGICOP_NOT) {
                     left = static_cast<const VlpLogicNotOpNode*>(
                                      node)->getOprn();
                     cntCond.setLeft( left->str() );
                  }
                  else {
                     left  = static_cast<const VlpLogicOpNode*>(
                                         node)->getLeft();
                     right = static_cast<const VlpLogicOpNode*>(
                                         node)->getRight();
                     cntCond.setLeft( left->str() );
                     cntCond.setRight( right->str() );
                  }
                  cntAction.addCond( cntCond );
                  break;
               case OPCLASS_BITWISE:
                  cntCond.setOP( class2CUOP( node, isNeg ) );
                  cntCond.setLeft( node->str() );
                  cntAction.addCond( cntCond );
                  break;
               default:
                  break;
               }
            }
         }
         node = assign.getSE();
         cntAction.setAction( node->str() );
         newCNT->addAction( cntAction );

         if ( node->getNodeClass() == NODECLASS_OPERATOR ) {
            if ( static_cast<const VlpOperatorNode*>(node)->getOperatorClass() == OPCLASS_ARITHMETIC )
               hasArith = true;
         }
      }
      if ( hasArith ) {
         local.push_back( newCNT );
         cout << "> New Implicit Counter added." << endl;
      }
      else {
         delete newCNT;
      }
   }
   return local;
}

void
VlgModule::setSigBus() const 
{
   if (_dataFlowStart == NULL)
      return;

   vector<VlpSignalNode*> SIGS = _dataFlowStart->listSIGs();
   VlpPortNode* port;
   for (unsigned i = 0; i < SIGS.size(); ++i) {
      if (SIGS[i]->MSB() < 0) {
         if (_portMap.getData(SIGS[i]->getNameId(), port)) {
            SIGS[i]->setMSB(const_cast<VlpBaseNode*>(port->getMsb()) );
            SIGS[i]->setLSB(const_cast<VlpBaseNode*>(port->getLsb()) );
         }
      }
   }
}

VLPCUCOND_OP
VlgModule::class2CUOP(const VlpBaseNode* node, bool isNeg) const 
{
   if ( node->getNodeClass() == NODECLASS_SIGNAL ) {
      if ( isNeg )
         return VLPCUCOND_NOT;
      else
         return VLPCUCOND_NONE;
   }
   else if ( node->getNodeClass() == NODECLASS_OPERATOR ) {
      switch ( static_cast<const VlpOperatorNode*>(node)->getOperatorClass() ) {
      case OPCLASS_EQUALITY:
         if ( isNeg ) {
            switch (static_cast<const VlpEqualityNode*>(
                               node)->getEqualityClass() ) {
            case EQUALITY_LOG_EQU:
            case EQUALITY_CASE_EQU:
               return VLPCUCOND_NEQ; break;
            case EQUALITY_LOG_INEQU:
            case EQUALITY_CASE_INEQU:
               return VLPCUCOND_EQ; break;
            default: break;
            }
         }
         else {
            switch (static_cast<const VlpEqualityNode*>(
                                node)->getEqualityClass() ) {
            case EQUALITY_LOG_EQU:
            case EQUALITY_CASE_EQU:
               return VLPCUCOND_EQ; break;
            case EQUALITY_LOG_INEQU:
            case EQUALITY_CASE_INEQU:
               return VLPCUCOND_NEQ; break;
            default: break;
            }
         }
         break;
      case OPCLASS_RELATION:
         if ( isNeg ) {
            switch(static_cast<const VlpRelateOpNode*>(node)->getRelOpClass()) {
            case RELATEOP_GEQ: // >=
               return VLPCUCOND_LT; break;
            case RELATEOP_GREATER: // >
               return VLPCUCOND_LE; break;
            case RELATEOP_LEQ: // <=
               return VLPCUCOND_GT; break;
            case RELATEOP_LESS: // <
               return VLPCUCOND_GE; break;
            default: break;
            }
         }
         else {
            switch(static_cast<const VlpRelateOpNode*>(node)->getRelOpClass()) {
            case RELATEOP_GEQ: // >=
               return VLPCUCOND_GE; break;
            case RELATEOP_GREATER: // >
               return VLPCUCOND_GT; break;
            case RELATEOP_LEQ: // <=
               return VLPCUCOND_LE; break;
            case RELATEOP_LESS: // <
               return VLPCUCOND_LT; break;
            default: break;
            }
         }
         break;
      case OPCLASS_LOGIC:
         if ( isNeg ) {
            switch(static_cast<const VlpLogicOpNode*>(node)->getLogicClass() ) {
/*
            case LOGICOP_AND: // &&
               return VLPCUCOND_OR;
            case LOGICOP_OR: //||
               return VLPCUCOND_AND;
*/
            case LOGICOP_AND:
            case LOGICOP_OR:
               return VLPCUCOND_NOT; break;
            case LOGICOP_NOT:
               return VLPCUCOND_NONE; break;
            default: break;
            }
         }
         else {
            switch(static_cast<const VlpLogicOpNode*>(node)->getLogicClass() ) {
            case LOGICOP_AND: // &&
               return VLPCUCOND_AND; break;
            case LOGICOP_OR: //||
               return VLPCUCOND_OR; break;
            case LOGICOP_NOT:
               return VLPCUCOND_NOT; break;
            default: break;
            }
         }
         break;
      case OPCLASS_BITWISE:
         if ( isNeg )
            return VLPCUCOND_NOT;
         break;
      default:
         break;
      }
   }
   return VLPCUCOND_NONE;
}

set<MyString>
VlgModule::intersect(const set<MyString>& a, const set<MyString>& b) const 
{
   set<MyString>                 tmp;
   set<MyString>::const_iterator pos;
   vector<MyString>              listA, listB;

   for (pos = a.begin(); pos != a.end(); pos++)
      listA.push_back(*pos);
   for (pos = b.begin(); pos != b.end(); pos++)
      listB.push_back(*pos);

   sort(listA.begin(), listA.end());
   sort(listB.begin(), listB.end());

   unsigned min = listA.size();
   if (listB.size() < min)
      min = listB.size();

   unsigned iA = 0;
   unsigned iB = 0;

   while (min > iA && min > iB) {
      if (listA[iA] > listB[iB])
         iB++;
      else if (listA[iA] < listB[iB])
         iA++;
      else if (listA[iA] == listB[iB]) {
         tmp.insert(listA[iA]);
         iA++;
         iB++;
      }
   }
   return tmp;
}

#endif
