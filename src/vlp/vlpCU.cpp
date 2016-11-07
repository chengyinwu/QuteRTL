/****************************************************************************
  FileName     [ vlpCU.cpp ]
  Package      [ vlp ]
  Synopsis     [ Implementation to Control Unit Class. ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_CU_SOURCE
#define VLP_CU_SOURCE

#include "vlpCU.h"
#include <map>

using namespace std;

//-----------------------------------------------------------------------------
// VlpCU
//-----------------------------------------------------------------------------
VlpCU::VlpCU() 
{
   _name = "";
   _isEx = false;
   _msb  = -1;
   _lsb  = -1;
}

VlpCU::~VlpCU() 
{
}

void
VlpCU::setName(const MyString& src) 
{
   _name = src;
}

MyString
VlpCU::name() const 
{
   return _name;
}

const char*
VlpCU::nameCStr() const 
{
   return _name.str();
}

void
VlpCU::setExType() 
{
   _isEx = true;
}

void
VlpCU::setImpType() 
{
   _isEx = false;
}

bool
VlpCU::isExType() const 
{
   return _isEx == true;
}

bool
VlpCU::isImpType() const 
{
   return _isEx == false;
}
   
void 
VlpCU::setType(bool type)
{
   _isEx = type;
}

void
VlpCU::setMSB(int src) 
{
   _msb = src;
}

void
VlpCU::setLSB(int src) 
{
   _lsb = src;
}

int
VlpCU::MSB() const 
{
   return _msb;
}

int
VlpCU::LSB() const 
{
   return _lsb;
}

void
VlpCU::writeDot(const MyString &file) const 
{
   cerr << "VlpCU::writeDot() ---> ERROR: VlpCU cannot execute writeDot()"
        << endl;
}

void
VlpCU::genPNG(const MyString& file) const 
{
   cerr << "VlpCU::genPNG() ---> ERROR: VlpCU cannot execute gnePNG()"
        << endl;
}

VLPCU_TYPE
VlpCU::type() const 
{
   return VLPCU_BODY;
}
/*
void
VlpCU::genDot( const char* fDot, const char* fPNG ) const {
   GVC_t *gvc;
   graph_t *g;
   FILE *fp;
   FILE *fpout;

   gvc = gvContext();
   
   fp = fopen(fDot, "r");
   fpout = fopen(fPNG, "w");

   g = agread(fp);
   gvLayout(gvc, g, "dot");
   gvRender(gvc, g, "png", fpout);
   gvFreeLayout(gvc, g);

   agclose(g);

   gvFreeContext(gvc);
}
*/
//-----------------------------------------------------------------------------
// VlpFSMCond
//-----------------------------------------------------------------------------
VlpFSMCond::VlpFSMCond() 
{
   _op    = VLPCUCOND_NONE;
   _left  = "";
   _right = "";
}

void
VlpFSMCond::setOP(VLPCUCOND_OP src) 
{
   _op = src;
}

VLPCUCOND_OP
VlpFSMCond::OP() const 
{
   return _op;
}

void
VlpFSMCond::setLeft(const MyString& src) 
{
   _left = src;
}

void
VlpFSMCond::setRight(const MyString& src) 
{
   _right = src;
}

MyString
VlpFSMCond::left() const 
{
   return _left;
}

MyString
VlpFSMCond::right() const 
{
   return _right;
}

MyString
VlpFSMCond::str() const 
{
   MyString tmp;
   if ( _op == VLPCUCOND_NOT ) {
      tmp = "!";
      tmp += _left;
   }
   else {
      tmp = _left;
      switch ( _op ) {
      case VLPCUCOND_EQ:  tmp += "=="; break;
      case VLPCUCOND_NEQ: tmp += "!="; break;
      case VLPCUCOND_LT:  tmp += "<";  break;
      case VLPCUCOND_LE:  tmp += "<="; break;
      case VLPCUCOND_GT:  tmp += ">";  break;
      case VLPCUCOND_GE:  tmp += ">="; break;
      case VLPCUCOND_AND: tmp += "&&"; break;
      case VLPCUCOND_OR:  tmp += "||"; break;
      default:
         break;
      }
      tmp += _right;
   }
   return tmp;
}

void
VlpFSMCond::reset() 
{
   _op    = VLPCUCOND_NONE;
   _left  = "";
   _right = "";
}

VlpFSMCond&
VlpFSMCond::operator = (const VlpFSMCond& src) 
{
   _op    = src._op;
   _left  = src._left;
   _right = src._right;

   return *this;
}

bool
VlpFSMCond::operator == (const VlpFSMCond& src) const 
{
   return (_op == src._op) && (_left == src._left) && (_right == src._right);
}

//-----------------------------------------------------------------------------
// VlpFSMTrans
//-----------------------------------------------------------------------------
VlpFSMTrans::VlpFSMTrans() 
{
   _nextState = NULL;
   _cond.clear();
}

void
VlpFSMTrans::addCond(const VlpFSMCond& src) 
{
   _cond.push_back(src);
}

unsigned int
VlpFSMTrans::numCond() const 
{
   return _cond.size();
}

VlpFSMCond
VlpFSMTrans::getCond(unsigned int index) const 
{
   VlpFSMCond tmp;

   if (index < _cond.size())
      tmp = _cond[index];

   return tmp;
}

MyString
VlpFSMTrans::str() const 
{
   MyString tmp = "";
   for (unsigned i = 0; i < _cond.size(); ++i) {
      tmp += _cond[i].str();
      if ((i+1) < _cond.size())
         tmp += "&&";
   }
   return tmp;
}

void
VlpFSMTrans::setNextState(VlpFSMState* src) 
{
   _nextState = src;
}

VlpFSMState*
VlpFSMTrans::nextState() const 
{
   return _nextState;
}
   
void
VlpFSMTrans::setProb(float& p)
{
   _prob = p;
}

void
VlpFSMTrans::reset() 
{
   _nextState = NULL;
   _cond.clear();
   _prob = 0.0;
}

VlpFSMTrans&
VlpFSMTrans::operator = (const VlpFSMTrans& src) 
{
   _nextState = src._nextState;
   _cond      = src._cond;
   _prob      = src._prob;

   return *this;
}

bool
VlpFSMTrans::operator == (const VlpFSMTrans& src) const 
{
   return ( _nextState == src._nextState ) && ( _cond == src._cond );
}

//-----------------------------------------------------------------------------
// VlpFSMState
//-----------------------------------------------------------------------------
VlpFSMState::VlpFSMState(const MyString& src) 
{
   _name = src;
   _trans.clear();
}

VlpFSMState::VlpFSMState(const char* src) 
{
   _name = src;
   _trans.clear();
}

VlpFSMState::VlpFSMState(const string& src, Bv4* bv, unsigned t) 
{
   _name  = src;
   _value = bv;
   _type  = t;
   _trans.clear();
}

VlpFSMState::~VlpFSMState() 
{
   _trans.clear();
}

MyString
VlpFSMState::name() const 
{
   return _name;
}

void
VlpFSMState::addTrans(const VlpFSMTrans& src) 
{
   _trans.push_back(src);
}

unsigned int
VlpFSMState::numTrans() const 
{
   return _trans.size();
}

VlpFSMTrans
VlpFSMState::getTrans(unsigned int index) const 
{
   VlpFSMTrans tmp;
   if ( index < _trans.size() )
      tmp = _trans[index];

   return tmp;
}

VlpFSMState&
VlpFSMState::operator = (const VlpFSMState& src) 
{
   _name  = src._name;
   _trans = src._trans;
   _value = src._value;
   _type  = src._type;

   return *this;
}

bool
VlpFSMState::operator == (const VlpFSMState& src) const 
{
   return ( _name == src._name );
}

//-----------------------------------------------------------------------------
// VlpFSM
//-----------------------------------------------------------------------------
VlpFSM::VlpFSM() 
{
   _name = "";
   _bits = 0;
   _states.clear();
   _reset = new VlpFSMState("FSMReset");
   _states.push_back( _reset );
}

VlpFSM::~VlpFSM() 
{
   for (unsigned i = 0; i < _states.size(); ++i)
      delete _states[i];
}

void
VlpFSM::addState(VlpFSMState* src) 
{
   for (unsigned i = 0; i < _states.size(); ++i)
      if (src->name() == _states[i]->name())
         return;

   _states.push_back(src);
}

unsigned int
VlpFSM::numState() const 
{
   return _states.size();
}

VlpFSMState*
VlpFSM::getState(unsigned int index) const 
{
   if (index < _states.size())
      return _states[index];
   else
      return NULL;
}

VlpFSMState*
VlpFSM::getState(const MyString& name) const 
{
   for (unsigned i = 0; i < _states.size(); ++i)
      if (name == _states[i]->name())
         return _states[i];

   return NULL;
}

VlpFSMState*
VlpFSM::resetState() const 
{
   return _reset;
}

void
VlpFSM::setName(const MyString& src) 
{
   _name = src;
}

MyString
VlpFSM::name() const 
{
   return _name;
}

const char*
VlpFSM::nameCStr() const 
{
   return _name.str();
}

void
VlpFSM::writeDot(const MyString& file) const 
{
   ofstream     out(file.str(), ofstream::out);
   VlpFSMState *curState, *nxtState;
   VlpFSMTrans  trans;
   MyString     acStr;
   bool         isReset = false;
   vector<VlpFSMState*> reset;
   reset.clear();

   out << "digraph " << _name << "{" << endl;
   out << "\tlabel=\"" << _name << "[" << MSB() << ":" << LSB() << "], Type: ";
   if (isExType())
      out << "Explicit FSM\";" << endl;
   else
      out << "Implicit FSM\";" << endl;

   for (unsigned i = 0; i < _reset->numTrans(); ++i) {
      trans = _reset->getTrans(i);
      reset.push_back(trans.nextState());
   }
   // states
   for (unsigned i = 0; i < _states.size(); ++i) {
      if (_states[i]->numTrans() > 0 && _states[i] != _reset) {
         isReset = false;
         for (unsigned j = 0; j < reset.size(); ++j) {
            if ( _states[i] == reset[j] ) {
               isReset = true;
               break;
            }
         }
         if (isReset) 
            out << "\t\"" << _states[i]->name() << "\"[fontsize=18, style=filled, color=yellow];" << endl;
         else
            out << "\t\"" << _states[i]->name() << "\"[fontsize=18];" << endl;
      }
   }
   // transitions
   for (unsigned i = 0; i < _states.size(); ++i) {
      curState = _states[i];
      if (curState == _reset)
         continue;
      for (unsigned j = 0; j < curState->numTrans(); ++j) {
         trans    = curState->getTrans(j);
         nxtState = trans.nextState();
         out << "\t\"" << curState->name() << "\" -> \"" << nxtState->name() << "\"[label=\"";
         acStr = trans.str();
         /*if (acStr.length() > 16) {
            acStr =  acStr.substr(0,16);
            acStr += "...";
         }*/
         out << acStr << "\", fontsize=12];" << endl;
      }
   }
   out << "}" << endl;
   out.close();
}

void
VlpFSM::genPNG(const MyString& file) const 
{

   MyString sysCmd,
            tmpFile;
   tmpFile = "VlpCU_gen_PNG.dot";
   writeDot( tmpFile );

   sysCmd = "";
   sysCmd << "dot -Tpng -o " << file << " " << tmpFile;
   sysCmd << "; rm -f " << tmpFile;
   system( sysCmd.str() );
/*
   sysCmd = "rm -f ";
   sysCmd += tmpFile;
   genDot( tmpFile.str(), file.str() );
   system( sysCmd.str() );
*/
}

VLPCU_TYPE
VlpFSM::type() const 
{
   return VLPCU_FSM;
}

void
VlpFSM::analysis() 
{
   bool         dead;
   VlpFSMTrans  trans;
   VlpFSMState *curState, *nxtState;
   map<VlpFSMState*, bool> unReach;
   _deadState.clear();
   _unReachState.clear();

   for (unsigned i = 0; i < _states.size(); ++i)
      unReach[_states[i]] = true; 

   for (unsigned i = 0; i < _states.size(); ++i) {
      curState = _states[i];
      dead = true;
      for (unsigned j = 0; j < curState->numTrans(); ++j) {
         trans    = curState->getTrans(j);
         nxtState = trans.nextState();
         unReach[nxtState] = false;

         if (curState != nxtState) dead = false;

         if (curState == _reset) cout << "> Reset state: " << nxtState->name() << endl;
      }
      if (dead && curState != _reset) {
         _deadState.push_back(curState);
         cout << "> Dead state: " << curState->name() << endl;
      }
   }

   for (map< VlpFSMState*, bool >::iterator pos = unReach.begin(); pos != unReach.end(); pos++) {
      if (pos->second == true && pos->first->name() != "FSMReset") {
         _unReachState.push_back(pos->first);
         cout << "> Unreach state: " << pos->first->name() << endl;
      }
   }
}

unsigned int
VlpFSM::numDeadState() const 
{
   return _deadState.size();
}

VlpFSMState*
VlpFSM::getDeadState(unsigned index) const 
{
   if ( index < _deadState.size() )
      return _deadState[index];
   else
      return NULL;
}

unsigned int
VlpFSM::numUnreachState() const 
{
   return _unReachState.size();
}

VlpFSMState*
VlpFSM::getUnreachState(unsigned index) const 
{
   if (index < _unReachState.size())
      return _unReachState[index];
   else
      return NULL;
}

VlpFSM*
VlpFSM::compose(VlpFSM* src) 
{
   VlpFSM *newFSM;
   VlpFSMState* curState;
   VlpFSMState* nxtState;
   VlpFSMState* newCurState;
   VlpFSMState* newNxtState;
   VlpFSMTrans  trans,
                newTrans;
   unsigned    i, j;

   newFSM = new VlpFSM();

   for ( i = 0; i < src->numState(); ++i ) {
      curState = src->getState(i);
      if ( getState( curState->name() ) != NULL ) {
         nxtState = new VlpFSMState(curState->name());
         newFSM->addState( nxtState );
      }
   }
   
   for ( i = 0; i < src->numState(); ++i ) {
      curState = src->getState(i);
      if ( newFSM->getState( curState->name() ) != NULL ) {
         newCurState = newFSM->getState( curState->name() );
         for ( j = 0; j < curState->numTrans(); ++j ) {
            trans = curState->getTrans(j);
            nxtState = trans.nextState();
            if ( newFSM->getState( nxtState->name() ) != NULL ) {
               newNxtState = newFSM->getState( nxtState->name() );
               newTrans = trans;
               newTrans.setNextState( newNxtState );
               newCurState->addTrans( newTrans );
            }
         }
      }
   }

   for ( i = 0; i < numState(); ++i ) {
      curState = getState(i);
      if ( newFSM->getState( curState->name() ) != NULL ) {
         newCurState = newFSM->getState( curState->name() );
         for ( j = 0; j < curState->numTrans(); ++j ) {
            trans = curState->getTrans(j);
            nxtState = trans.nextState();
            if ( newFSM->getState( nxtState->name() ) != NULL ) {
               newNxtState = newFSM->getState( nxtState->name() );
               newTrans = trans;
               newTrans.setNextState( newNxtState );
               newCurState->addTrans( newTrans );
            }
         }
      }
   }

   curState = src->resetState();
   newCurState = newFSM->resetState();
   cout << "SRC Reset state: " << curState->numTrans() << endl;
   for ( j = 0; j < curState->numTrans(); ++j ) {
      trans = curState->getTrans(j);
      nxtState = trans.nextState();
      cout << nxtState->name() << endl;
      if ( newFSM->getState( nxtState->name() ) != NULL ) {
         newNxtState = newFSM->getState( nxtState->name() );
         newTrans = trans;
         newTrans.setNextState( newNxtState );
         newCurState->addTrans( newTrans );
         cout << "Add reset state: " << newNxtState->name() << endl;
      }
   }

   curState = resetState();
   newCurState = newFSM->resetState();
   cout << "Reset State: " << curState->numTrans() << endl;
   for ( j = 0; j < curState->numTrans(); ++j ) {
      trans = curState->getTrans(j);
      nxtState = trans.nextState();
      if ( newFSM->getState( nxtState->name() ) != NULL ) {
         newNxtState = newFSM->getState( nxtState->name() );
         newTrans = trans;
         newTrans.setNextState( newNxtState );
         newCurState->addTrans( newTrans );
         cout << "Add reset state: " << newNxtState->name() << endl;
      }
   }

   newFSM->setName( name() );
   newFSM->setMSB( MSB() );
   newFSM->setLSB( LSB() );
   newFSM->analysis();
   return newFSM;
}

void
VlpFSM::addExState(vector<VlpFSMAssign>& reset)
{
   MyString strNxtState;
   const VlpBaseNode* node;
   Bv4   ns;
   VlpFSMTrans newTrans;
   VlpFSMState *curState, *nxtState;

   for (unsigned i = 0; i < reset.size(); ++i) {
      node = reset[i].getSE();

      if (node->getNodeClass() == NODECLASS_INT) {
         if (MSB() > LSB()) strNxtState = MSB()-LSB()+1;
         else               strNxtState = LSB()-MSB()+1;
         strNxtState += "'d";
         strNxtState += static_cast<const VlpIntNode*>(node)->getNum();
         ns = strNxtState.str();
         nxtState = getState(ns.str());

         if (nxtState == NULL) {
            nxtState = new VlpFSMState(strNxtState);
            addState(nxtState);
         }
         curState = resetState();
         newTrans.reset();
         newTrans.setNextState(nxtState);
         curState->addTrans(newTrans);
      }
      else if (node->getNodeClass() == NODECLASS_PARAMETER) {
         strNxtState = static_cast<const VlpParamNode*>(node)->strName();
         curState = resetState();
         nxtState = getState(strNxtState);
         if (nxtState == NULL) {
            nxtState = new VlpFSMState(strNxtState);
            addState(nxtState);
         }
         newTrans.reset();
         newTrans.setNextState( nxtState );
         curState->addTrans( newTrans );
      }
      else if (node->getNodeClass() == NODECLASS_SIGNAL) {
         if (static_cast<const VlpSignalNode*>(node)->listSIGs().size() == 0) {
            strNxtState = static_cast<const VlpSignalNode*>(node)->strNoIndex();
            curState = resetState();
            nxtState = getState(strNxtState);
            if (nxtState == NULL) {
               nxtState = new VlpFSMState(strNxtState);
               addState( nxtState );
            }
            newTrans.reset();
            newTrans.setNextState( nxtState );
            curState->addTrans( newTrans );
         }
      }
   }
}

//-----------------------------------------------------------------------------
// VlpCNTCond
//-----------------------------------------------------------------------------
VlpCNTCond::VlpCNTCond() 
{
   _left  = "";
   _right = "";
   _op    = VLPCUCOND_NONE;
}

void
VlpCNTCond::setOP(VLPCUCOND_OP op) 
{
   _op = op;
}

void
VlpCNTCond::setLeft(const MyString& src) 
{
   _left = src;
}

void
VlpCNTCond::setRight(const MyString& src) 
{
   _right = src;
}

VLPCUCOND_OP
VlpCNTCond::cond() const 
{
   return _op;
}

MyString
VlpCNTCond::left() const 
{
   return _left;
}

MyString
VlpCNTCond::right() const 
{
   return _right;
}

MyString
VlpCNTCond::str() const 
{
   MyString tmp = "";
   if ( _op == VLPCUCOND_NOT ) {
      tmp = "!";
      tmp += _left;
   }
   else {
      tmp += _left;
      switch ( _op ) {
      case VLPCUCOND_EQ:  tmp += "=="; break;
      case VLPCUCOND_NEQ: tmp += "!="; break;
      case VLPCUCOND_LT:  tmp += "<";  break;
      case VLPCUCOND_LE:  tmp += "<="; break;
      case VLPCUCOND_GT:  tmp += ">";  break;
      case VLPCUCOND_GE:  tmp += ">="; break;
      case VLPCUCOND_AND: tmp += "&&"; break;
      case VLPCUCOND_OR:  tmp += "||"; break;
      default:
         break;
      }
      tmp += _right;
   }
   return tmp;
}

MyString
VlpCNTCond::strActive() const 
{
   MyString tmp;
   if ( _op == VLPCUCOND_NOT )
      return tmp;
   else {
      switch ( _op ) {
      case VLPCUCOND_EQ:  tmp += " == "; break;
      case VLPCUCOND_NEQ: tmp += " != "; break;
      case VLPCUCOND_LT:  tmp += " < ";  break;
      case VLPCUCOND_LE:  tmp += " <= "; break;
      case VLPCUCOND_GT:  tmp += " > ";  break;
      case VLPCUCOND_GE:  tmp += " >= "; break;
      case VLPCUCOND_AND: tmp += " && "; break;
      case VLPCUCOND_OR:  tmp += " || "; break;
      default:
         break;
      }
      tmp += _right;
   }
   return tmp;
}

void
VlpCNTCond::reset() 
{
   _left  = "";
   _right = "";
   _op    = VLPCUCOND_NONE;
}

VlpCNTCond&
VlpCNTCond::operator = (const VlpCNTCond& src) 
{
   _left  = src._left;
   _right = src._right;
   _op    = src._op;

   return *this;
}

bool
VlpCNTCond::operator == (const VlpCNTCond& src) const 
{
   return (_left == src._left) && (_right == src._right) && (_op && src._op);
}

//-----------------------------------------------------------------------------
// VlpCNTAction
//-----------------------------------------------------------------------------
VlpCNTAction::VlpCNTAction() 
{
   _cond.clear();
   _action = "";
}

void
VlpCNTAction::addCond(const VlpCNTCond& src) 
{
   _cond.push_back( src );
}

unsigned
VlpCNTAction::numCond() const 
{
   return _cond.size();
}

VlpCNTCond
VlpCNTAction::getCond(unsigned index) const 
{
   VlpCNTCond tmp;
   if ( index < _cond.size() )
      tmp = _cond[index];

   return tmp;
}

void
VlpCNTAction::setAction(const MyString& src) 
{
   _action = src;
}

MyString
VlpCNTAction::action() const
{
   return _action;
}

MyString
VlpCNTAction::strCond() const 
{
   MyString tmp = "";
   for (unsigned i = 0; i < _cond.size(); ++i) {
      tmp = tmp + "(";
      tmp = tmp + _cond[i].str();
      tmp = tmp + ")";
      if ( (i+1) < _cond.size() )
         tmp += "&&";
   } 
   return tmp;
}

MyString
VlpCNTAction::strCondDot() const 
{
   MyString tmp = "";
   for (unsigned i = 0; i < _cond.size(); ++i) {
      tmp = tmp + "(";
      tmp = tmp + _cond[i].str();
      tmp = tmp + ")";
      if ( (i+1) < _cond.size() )
         tmp += "&amp;&amp;";
   }
   return tmp;
}

void
VlpCNTAction::reset() 
{
   _cond.clear();
   _action = "";
}

VlpCNTAction&
VlpCNTAction::operator = (const VlpCNTAction& src) 
{
   _cond   = src._cond;
   _action = src._action;
   return *this;
}

bool
VlpCNTAction::operator == (const VlpCNTAction& src) const 
{
   return (_cond == src._cond) && (_action == src._action);
}

ostream&
operator << (ostream& os, const VlpCNTAction& src) 
{
   os << "  Cond: " << src.strCond() << endl;
   os << "Action: " << src.action() << endl;
   return os;
}

//-----------------------------------------------------------------------------
// VlpCNT
//-----------------------------------------------------------------------------
VlpCNT::VlpCNT(const MyString& src, bool isEx, int msb, int lsb) 
{
   _name = src;
   setType(isEx);
   setMSB(msb);
   setLSB(lsb);
   _actions.clear();
}

VlpCNT::~VlpCNT() 
{
   _actions.clear();
}

void
VlpCNT::addAction(const VlpCNTAction& src) 
{
   _actions.push_back(src);
}

unsigned
VlpCNT::numAction() const 
{
   return _actions.size();
}

VlpCNTAction
VlpCNT::getAction(unsigned index) const 
{
   VlpCNTAction tmp;  
   if ( index < _actions.size() )
      tmp = _actions[index];

   return tmp;
}

MyString
VlpCNT::name() const 
{
   return _name;
}

const char*
VlpCNT::nameCStr() const 
{
   return _name.str();
}

void
VlpCNT::writeDot(const MyString& file) const 
{
   ofstream     out( file.str(), ofstream::out );
   VlpCNTAction act;
   MyString     tmp, tmp1;

   out << "digraph table {" << endl;
   out << "\tlabel=\"" << _name << "[" << MSB() << ":" << LSB() << "], Type: ";
   if ( isExType() ) 
      out << "Explicit Counter\"" << endl;
   else
      out << "Implicit Counter\"" << endl;

   out << "\tnode [shape=plaintext];" << endl;
   out << "\tstruct1 [label=<<TABLE BORDER=\"1\" CELLPADDING=\"5px\"";
   out << " CELLSPACING=\"1px\">" << endl;
   out << "\t\t<TR BGCOLOR=\"#FFFFFF\">" << endl;
   out << "\t\t\t<TD>Condition</TD><TD>Action</TD>" << endl;
   out << "\t\t</TR>" << endl;
   for (unsigned i = 0; i < _actions.size(); ++i) {
      tmp = _actions[i].strCondDot();
      tmp1 = "";
      for (int j = 0; j < tmp.length(); ++j) {
         if ( tmp[j] == '&' )
            tmp1 += "&amp;";
         else if ( tmp[j] == ' ' )
            tmp1 += "&nbsp;";
         else
            tmp1 += tmp[j];
      }

      out << "\t\t<TR BGCOLOR=\"#FFFFFF\">" << endl;
      out << "\t\t\t<TD>" << tmp1  << "</TD>" << endl;
      out << "\t\t\t<TD>" << _actions[i].action() << "</TD>" << endl;
      out << "\t\t</TR>" << endl;
   }
   
   out << "\t</TABLE>>];" << endl;
   out << "}" << endl;
}

void
VlpCNT::genPNG(const MyString& file) const 
{
   MyString tmpFile = "VlpCU_gen_PNG.dot";
   writeDot(tmpFile);

   MyString sysCmd = "";
   sysCmd << "dot -Tpng -o " << file << " " << tmpFile;
   sysCmd << "; rm -f " << tmpFile;
   system(sysCmd.str());
/*
   sysCmd  = "rm -f ";
   sysCmd += tmpFile;
   genDot( tmpFile.str(), file.str() );
   system( sysCmd.str() );
*/
}

VLPCU_TYPE
VlpCNT::type() const 
{ 
   return VLPCU_CNT;
}

#endif

