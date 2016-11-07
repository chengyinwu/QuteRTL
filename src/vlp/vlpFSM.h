/****************************************************************************
  FileName     [ vlpFSM.h ]
  Package      [ vlp ]
  Synopsis     [ Header of FSM class ]
  Author       [ Chi-Wen (Kevin) Chang ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_FSM_H
#define VLP_FSM_H

#include <set>
#include <iostream>
#include <iomanip>
#include <stack>
#include <vector>

#include "vlpOpNode.h"
#include "myString.h"
#include "bv4.h"

using namespace std;
class VlpFSM;

//-----------------------------------------------------------------------------
// VlpFSMAssign
//-----------------------------------------------------------------------------
class VlpFSMAssign 
{
public:
   VlpFSMAssign();

   ~VlpFSMAssign();

   void setAC( const stack< size_t >& );
   void setSE( const VlpBaseNode* );
   void setTS( const VlpBaseNode* );

   vector< size_t >   getAC() const;
   const VlpBaseNode* getSE() const;
   const VlpBaseNode* getTS() const;

   set< MyString > ACset() const;
   set< MyString > SEset() const;

   MyString strAC() const;
   MyString strSE() const;
   MyString strTS() const;
   MyString strTSwIndex() const;

   int TS_MSB() const;
   int TS_LSB() const;
   int TS_width() const;

   void setSeq();
   void setNonSeq();
   bool isSeq() const;

   void reset();

   void toImpView();

   VlpFSMAssign& operator =  ( const VlpFSMAssign& );
   bool      operator == ( const VlpFSMAssign& ) const; 
   bool      operator != ( const VlpFSMAssign& ) const;
   bool      operator <  ( const VlpFSMAssign& ) const;
   bool      operator >  ( const VlpFSMAssign& ) const;

   friend ostream& operator << ( ostream&, const VlpFSMAssign& );
private:
   bool               _seq;
   vector< size_t >   _ac;
   const VlpBaseNode* _se; // Source Expression
   const VlpBaseNode* _ts; // Target Signal
};

//-----------------------------------------------------------------------------
// VlpFSMPSR
//-----------------------------------------------------------------------------
/*class VlpFSMPSR 
{
public:
   VlpFSMPSR();

   ~VlpFSMPSR();

   void addSig( const VlpBaseNode* );
   void addSig( const MyString& );
   void addSig( const string& );
   void addSig( const char* );

   unsigned int size() const;

   MyString getPSR( unsigned int ) const;
private:
   vector< MyString > _psr;
};
*/
//-----------------------------------------------------------------------------
// VlpFSMADS
//-----------------------------------------------------------------------------
class VlpFSMADS 
{
public:
   VlpFSMADS();

   ~VlpFSMADS();

   void addSig( const VlpBaseNode* );
   void addSig( const MyString& );
   void addSig( const string& );
   void addSig( const char* );

   unsigned int size() const;
   MyString     getSig( unsigned int ) const;
   bool         inADS( const MyString& ) const;

   void clear();

   VlpFSMADS& operator = ( const VlpFSMADS& );

   friend ostream& operator << ( ostream&, const VlpFSMADS& );
private:
   set< MyString > _ads;
};

//-----------------------------------------------------------------------------
// VlpFSMACS
//-----------------------------------------------------------------------------
class VlpFSMACS 
{
public:
   VlpFSMACS();

   ~VlpFSMACS();

   void addSig( const VlpBaseNode* );
   void addSig( const MyString& );
   void addSig( const string& );
   void addSig( const char* );

   unsigned int size() const;
   MyString     getSig( unsigned int ) const;
   bool         inACS( const MyString& ) const;

   void clear();

   VlpFSMACS& operator = ( const VlpFSMACS& );

   friend ostream& operator << ( ostream&, const VlpFSMACS& );
private:
   set< MyString > _acs;
};

//-----------------------------------------------------------------------------
// VlpFSMSIG
//-----------------------------------------------------------------------------
class VlpFSMSIG 
{
public:
   VlpFSMSIG();
   ~VlpFSMSIG();

   void setName( const MyString& );
   void setName( const string& );
   void setName( const char* );
   MyString name() const;

   void setMSB( const int );
   void setLSB( const int );
   
   int MSB() const;
   int LSB() const;

   void addAssign( const VlpFSMAssign& );
  
   unsigned int sizeAL() const;
   VlpFSMAssign getAL( unsigned int ) const;
   unsigned int sizeACS() const;
   MyString     getACS( unsigned int ) const;
   bool         inACS( const MyString& ) const;

   void addADS( const VlpBaseNode* );
   void addADS( const MyString& );
   void addADS( const string& );
   void addADS( const char* );

   unsigned int sizeADS() const;
   MyString     getADS( unsigned int ) const;
   bool         inADS( const MyString& ) const;

   void setEX();
   void setIMP();
   void resetType();
   bool isEX() const;
   bool isIMP() const;
   bool isSR() const;
   
   void setCoSIG( const MyString& );
   MyString coSIG() const;
   bool build_X2Y(bool&, vector<MyString>&, int&, int&, BddManager&) const;
   bool build_Y2Z(VlpFSMSIG&, vector<MyString>&, vector<VlpFSMAssign>&, int&, int&, BddManager&) const;
   VlpFSM* build_tr(VlpFSMSIG&, vector<VlpFSMAssign>&, BddManager&) const;

   bool operator == ( const VlpFSMSIG& ) const;
   bool operator != ( const VlpFSMSIG& ) const;
   bool operator <  ( const VlpFSMSIG& ) const;
   bool operator >  ( const VlpFSMSIG& ) const;
   VlpFSMSIG& operator = ( const VlpFSMSIG& );

   friend ostream& operator << ( ostream&, const VlpFSMSIG& );
private:
   void addACS( const VlpBaseNode* );
   void addACS( const MyString& );
   void addACS( const string& );
   void addACS( const char* );
   int readIndex(const MyString& src) const ;
   MyString nameNoIndex(const MyString& src) const;
   vector<Bv4> expandBV(const Bv4&) const;
   void composeTr(VlpFSMSIG&, BddNode&, BddManager&) const;
   VlpFSM* buildFSM(VlpFSMSIG&, BddNode&, BddManager&) const;

   MyString               _name;
   int                    _msb, 
                          _lsb;
   VlpFSMACS              _acs;
   VlpFSMADS              _ads;
   vector< VlpFSMAssign > _al;
   bool                   _isEX,
                          _isIMP;
   MyString               _coSIG;
};

#endif

