/****************************************************************************
  FileName     [ cktArithCell.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit arithmetic cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_ARITH_CELL_H
#define CKT_ARITH_CELL_H

#include "cktCell.h"
#include <string>

using namespace std;

class CktAddCell : public CktCell
{
   public:
      CktAddCell();
      CktAddCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
      ~CktAddCell();
      
      // Attribute Descriptive Functions
      inline string getOpStr() const { return "+"; }
      void writeOutput() const;
      void writeBLIF() const;
      
      // Operating Functions
      CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
      void simulate();
   
   private:
      //CktOutPin *_carry; //no use temporarily!
};

class CktSubCell : public CktCell
{
   public:
      CktSubCell();
      CktSubCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
      ~CktSubCell();
      
      // Attribute Descriptive Functions
      inline string getOpStr() const { return "-"; }
      void writeOutput() const;
      void writeBLIF() const;
      
      // Operating Functions
      CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
      void simulate();
   
   private:
      //CktOutPin *_borrow;
};

class CktMultCell : public CktCell
{
   public:
      CktMultCell();
      CktMultCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
      ~CktMultCell();
      
      // Attribute Descriptive Functions
      inline string getOpStr() const { return "*"; }
      void writeOutput() const;
      void writeBLIF() const;
      
      // Operating Functions
      CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
      void simulate();
   
   private: 
};

////////// DIVIDOR HAS NOT SUPPORTED YET !!! //////////
class CktDivCell : public CktCell
{
   public:
      CktDivCell();
      CktDivCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
      ~CktDivCell();
      
      // Attribute Descriptive Functions
      inline string getOpStr() const { return "/"; }
      
      // Operating Functions
      void simulate();

   private:
      //CktOutPin *_remainder;
};

////////// MODULOR HAS NOT SUPPORTED YET !!! //////////
class CktModuloCell : public CktCell
{
   public:
      CktModuloCell();
      CktModuloCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
      ~CktModuloCell();
      
      // Attribute Descriptive Functions
      inline string getOpStr() const { return "%"; }
      void writeOutput() const;
      
      // Operating Functions
      void simulate();
   
   private:
};

class CktShlCell : public CktCell
{
   public:
      CktShlCell();
      CktShlCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
      ~CktShlCell();
      
      // Attribute Descriptive Functions
      inline string getOpStr() const { return "<<"; }
      void writeOutput() const;
      void writeBLIF() const;
      
      // Operating Functions
      CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
      void simulate();
   
   private:
};

class CktShrCell : public CktCell
{
   public:
      CktShrCell();
      CktShrCell(bool, CktInPin*&, CktInPin*&, CktOutPin*&);
      ~CktShrCell();
      
      // Attribute Descriptive Functions
      inline string getOpStr() const { return ">>"; }
      void writeOutput() const;
      void writeBLIF() const;
      
      // Operating Functions
      CktCell* cellDuplicate(string&, OutPinMap&, InPinMap&) const;
      void simulate();
   
   private:
};

#endif // CKT_ARITH_CELL_H

