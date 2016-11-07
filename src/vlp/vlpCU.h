/****************************************************************************
  FileName     [ vlpCU.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Control Unit Class. ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_CU_H
#define VLP_CU_H

#include <vector>
#include <fstream>
#include <iostream>

#include "myString.h"
#include "vlpFSM.h"

using namespace std;

class VlpFSMState;

enum VLPCU_TYPE 
{
   VLPCU_FSM,
   VLPCU_CNT,
   VLPCU_BODY
};

enum VLPCUCOND_OP 
{
   VLPCUCOND_NOT,
   VLPCUCOND_EQ,
   VLPCUCOND_NEQ,
   VLPCUCOND_LT,
   VLPCUCOND_LE,
   VLPCUCOND_GT,
   VLPCUCOND_GE,
   VLPCUCOND_AND,
   VLPCUCOND_OR,
   VLPCUCOND_NONE
};

//-----------------------------------------------------------------------------
// VlpCU
//-----------------------------------------------------------------------------
class VlpCU 
{
public:
   VlpCU();
   virtual ~VlpCU();

   virtual void setName( const MyString& );
   virtual MyString name() const;
   virtual const char* nameCStr() const;

   void setExType();
   void setImpType();
   void setType(bool);

   bool isExType() const;
   bool isImpType() const;

   void setMSB( int );
   void setLSB( int );
   
   int MSB() const;
   int LSB() const;

   virtual void writeDot( const MyString& ) const;
   virtual void genPNG( const MyString& ) const;

   virtual VLPCU_TYPE type() const;

  // void genDot( const char*, const char* ) const;
private:
   MyString _name;
   bool     _isEx;
   int      _msb,
            _lsb;
};

//-----------------------------------------------------------------------------
// VlpFSMCond
//-----------------------------------------------------------------------------
class VlpFSMCond 
{
public:
   VlpFSMCond();

   void          setOP( VLPCUCOND_OP = VLPCUCOND_NONE );
   VLPCUCOND_OP OP() const;

   void     setLeft( const MyString& );
   void     setRight( const MyString& );
   MyString left() const;
   MyString right() const;

   MyString str() const;

   void reset();

   VlpFSMCond& operator =  ( const VlpFSMCond& );
   bool        operator == ( const VlpFSMCond& ) const;
private:
   VLPCUCOND_OP _op;
   MyString     _left,
                _right;
};

//-----------------------------------------------------------------------------
// VlpFSMTrans
//-----------------------------------------------------------------------------
class VlpFSMTrans 
{
public:
   VlpFSMTrans();

   void         addCond( const VlpFSMCond& );
   unsigned int numCond() const;
   VlpFSMCond   getCond( unsigned int ) const;

   MyString str() const;

   void         setNextState( VlpFSMState* );
   VlpFSMState* nextState() const;

   void         setProb(float&);

   void reset();

   VlpFSMTrans& operator =  ( const VlpFSMTrans& );
   bool         operator == ( const VlpFSMTrans& ) const;
private:
   VlpFSMState*         _nextState;
   vector< VlpFSMCond > _cond;
   float                _prob;
};

//-----------------------------------------------------------------------------
// VlpFSMState
//-----------------------------------------------------------------------------
class VlpFSMState
{
public:
   VlpFSMState(const MyString&);
   VlpFSMState(const char*);
   VlpFSMState(const string&, Bv4* = NULL, unsigned = 0);
   ~VlpFSMState();

   MyString name() const;
   void         addTrans( const VlpFSMTrans& );
   unsigned int numTrans() const;
   VlpFSMTrans  getTrans( unsigned int ) const;

   VlpFSMState& operator =  ( const VlpFSMState& );
   bool         operator == ( const VlpFSMState& ) const;
private:
   MyString              _name;
   Bv4*                  _value;
   unsigned              _type;
   vector< VlpFSMTrans > _trans;
};

//-----------------------------------------------------------------------------
// VlpFSM
//-----------------------------------------------------------------------------
class VlpFSM : public VlpCU 
{
public:
   VlpFSM();
   ~VlpFSM();

   void         addState( VlpFSMState* );
   unsigned int numState() const;
   VlpFSMState* getState( unsigned int ) const;
   VlpFSMState* getState( const MyString& ) const;

   VlpFSMState* resetState() const;

   void setName( const MyString& );
   MyString name() const;
   const char* nameCStr() const;

   void writeDot( const MyString& ) const;
   void genPNG( const MyString& ) const;
   VLPCU_TYPE type() const;
   void analysis();

   unsigned int numDeadState() const;
   VlpFSMState* getDeadState( unsigned int ) const;

   unsigned int numUnreachState() const;
   VlpFSMState* getUnreachState( unsigned int ) const;

   VlpFSM* compose( VlpFSM* );
   void addExState(vector<VlpFSMAssign>&);
   bool parseFSM(const char*);
private:
   MyString               _name;
   unsigned               _bits;
   VlpFSMState*           _reset;
   vector< VlpFSMState* > _states;
   vector< VlpFSMState* > _deadState;
   vector< VlpFSMState* > _unReachState;
};

//-----------------------------------------------------------------------------
// VlpCNTCond
//-----------------------------------------------------------------------------
class VlpCNTCond 
{
public:
   VlpCNTCond();
   
   void setOP( VLPCUCOND_OP );
   void setLeft( const MyString& );
   void setRight( const MyString& );

   VLPCUCOND_OP cond() const;
   MyString      left() const;
   MyString      right() const;

   MyString str() const;
   MyString strActive() const;

   void reset();

   VlpCNTCond& operator =  ( const VlpCNTCond& );
   bool        operator == ( const VlpCNTCond& ) const;
private:
   MyString      _left,
                 _right;
   VLPCUCOND_OP _op;
};

//-----------------------------------------------------------------------------
// VlpCNTAction
//-----------------------------------------------------------------------------
class VlpCNTAction 
{
public:
   VlpCNTAction();

   void         addCond( const VlpCNTCond& );
   unsigned int numCond() const;
   VlpCNTCond   getCond( unsigned int ) const; 

   void     setAction( const MyString& );
   MyString action() const;
   
   MyString strCond() const;
   MyString strCondDot() const;

   void reset();

   VlpCNTAction& operator =  ( const VlpCNTAction& );
   bool          operator == ( const VlpCNTAction& ) const;

   friend ostream& operator << ( ostream&, const VlpCNTAction& );
private:
   vector< VlpCNTCond >  _cond;
   MyString             _action;
};

//-----------------------------------------------------------------------------
// VlpCNT
//-----------------------------------------------------------------------------
class VlpCNT : public VlpCU 
{
public:
   VlpCNT(const MyString&, bool, int, int);
   ~VlpCNT();

   void         addAction( const VlpCNTAction& );
   unsigned int numAction() const;
   VlpCNTAction getAction( unsigned int ) const;

   MyString name() const;
   const char* nameCStr() const;

   void writeDot( const MyString& ) const;
   void genPNG( const MyString& ) const;
   VLPCU_TYPE type() const;
private:
   MyString               _name;
   vector< VlpCNTAction > _actions;
};

#endif

