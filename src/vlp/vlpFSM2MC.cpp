/****************************************************************************
  FileName     [ vlpFSM2MC.cpp ]
  PackageName  [ vlp ]
  Synopsis     [ Complementary Extra File Extension Parser. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_FSM2MC_SOURCE
#define VLP_FSM2MC_SOURCE

#include "vlpCU.h"
#include <map>
//#include <gvc.h>

using namespace std;

//-----------------------------------------------------------------------------
// VlpCU
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// VlpFSMCond
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// VlpFSMTrans
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// VlpFSMState
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// VlpFSM
//-----------------------------------------------------------------------------
bool 
VlpFSM::parseFSM(const char* file)
{
   ifstream itf;
   itf.open(file);
   string keyword, state1, state2, trans, stateValue;
   MyString mStr;
   int stateType;
   float prob;
   VlpFSMState* fState;
   VlpFSMTrans  fTrans;
   Bv4* bv;

   while (!itf.eof()) {
      itf >> keyword;
      if (keyword == "FSM") {
         itf >> state1 >> _bits;
         _name = state1;
      }
      else if (keyword == "STATE") {
         itf >> state1 >> stateValue >> stateType;
         bv = new Bv4(stateValue);
         fState = new VlpFSMState(state1, bv, stateType);
         addState(fState);
      }
      else if (keyword == "TRANS") {
         itf >> state1 >> trans >> state2 >> prob;
         mStr = state2;
         fState = getState(mStr);

         fTrans.reset();
         fTrans.setNextState(fState);
         fTrans.setProb(prob);
         
         mStr = state1;
         fState = getState(mStr);
         fState->addTrans(fTrans);
      }
      else 
         ;
   }
   itf.close();
   return true;
}

//-----------------------------------------------------------------------------
// VlpCNTCond
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// VlpCNTAction
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// VlpCNT
//-----------------------------------------------------------------------------




#endif

