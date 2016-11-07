/****************************************************************************
  FileName     [ cktArithCell.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit arithmetic cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_ARITH_CELL_SOURCE
#define CKT_ARITH_CELL_SOURCE

#include "cktArithCell.h"
#include "synVar.h"
#include "cktBLIFile.h"
#include "vlpDesign.h"

extern CktOutFile cktOutFile;
extern VlpDesign& VLDesign;

//================================ CktAddCell ================================//

CktAddCell::CktAddCell(): CktCell( CKT_ADD_CELL )
{
   //_carry = NULL;
}

CktAddCell::CktAddCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_ADD_CELL, isNewPin, iA, iB ,oY)
{
}

CktAddCell::~CktAddCell()
{
}

void
CktAddCell::writeOutput() const
{
   genOutputArith();   
}

void
CktAddCell::simulate()
{
   assert (getInPinSize() == 2);
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0) + fin1->getCell()->getSimValue(fin1);
}

CktCell* 
CktAddCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktAddCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktAddCell::writeBLIF() const
{
//Note:can be optimized to use carry look ahead adder
//     Counter maybe be optimized.....
/*  
           Cin                           Cin 
          __|__                         __|__
     A---|     |               const---|     |
         |  +  |----Y  ==>             |  +  |--- Y  Counter
     B---|_____|            variable---|_____|
            |                             |
          Cout                           Cout
*/
/* 
    Cin    A[1] A[0] B[1] B[0]   Cout Y[1] Y[0]  |  Cin    A[1] A[0] B[1] B[0]   Cout Y[1] Y[0]
------------------------------------------------------------------------------------------------
     0      0    0    0    0      0    0    0    |   1      0    0    0    0      0    0    1 
     0      0    0    0    1      0    0    1    |   1      0    0    0    1      0    1    0
     0      0    0    1    0      0    1    0    |   1      0    0    1    0      0    1    1        
     0      0    0    1    1      0    1    1    |   1      0    0    1    1      1    0    0 
     0      0    1    0    0      0    0    1    |   1      0    1    0    0      0    1    0  
     0      0    1    0    1      0    1    0    |   1      0    1    0    1      0    1    1  
     0      0    1    1    0      0    1    1    |   1      0    1    1    0      1    0    0  
     0      0    1    1    1      1    0    0    |   1      0    1    1    1      1    0    1  
     0      1    0    0    0      0    1    0    |   1      1    0    0    0      0    1    1 
     0      1    0    0    1      0    1    1    |   1      1    0    0    1      1    0    0  
     0      1    0    1    0      1    0    0    |   1      1    0    1    0      1    0    1
     0      1    0    1    1      1    0    1    |   1      1    0    1    1      1    1    0 
     0      1    1    0    0      0    1    1    |   1      1    1    0    0      1    0    0
     0      1    1    0    1      1    0    0    |   1      1    1    0    1      1    0    1 
     0      1    1    1    0      1    0    1    |   1      1    1    1    0      1    1    0 
     0      1    1    1    1      1    1    0    |   1      1    1    1    1      1    1    1 
*/
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string cName = yName + "C_IN_OUT";   

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
#ifdef BLIF_DEBUG_MODE
   CktCellType aType = getFinCell(0)->getCellType();
   CktCellType bType = getFinCell(1)->getCellType();
   if (aType == CKT_CONST_CELL || bType == CKT_CONST_CELL) //counter
      outFile << "#counter" << endl;
   else {
      outFile << "#adder " << endl;
      //assert (aWidth == yWidth);
      //assert (bWidth == yWidth);
      if (aWidth != yWidth || bWidth != yWidth) 
         Msg(MSG_WAR) << "Unmatch # bits adder !!! (yWidth, aWidth, bWidth) = (" << yWidth << ", " << aWidth << ", " << bWidth << ")" << endl;
   }
#endif
   
   int num_of_two_bits = -1;
   bool single_one_bit = false;
   int kBits = yWidth;
   while (kBits >= 0) {
      if (kBits == 1)
         single_one_bit = true;
      kBits -= 2;
      ++num_of_two_bits;
   }
   for (int i = 0; i < num_of_two_bits; ++i) {
#ifdef BLIF_DEBUG_MODE
      outFile << "#2-bits adder no. " << i << endl;
#endif
      if (i == 0) {//no Cin
          //Y[0]
          outFile << ".names " << aName << "[1] " << aName << "[0] "
                               << bName << "[1] " << bName << "[0] "
                               << yName << "[0]"  << endl;
          outFile << "0001 1" << endl << "0011 1" << endl << "0100 1" << endl
                  << "0110 1" << endl << "1001 1" << endl << "1011 1" << endl
                  << "1100 1" << endl << "1110 1" << endl;    
          //Y[1]
          outFile << ".names " << aName << "[1] " << aName << "[0] "
                               << bName << "[1] " << bName << "[0] "
                               << yName << "[1]"  << endl;
          outFile << "0010 1" << endl << "0011 1" << endl << "0101 1" << endl
                  << "0110 1" << endl << "1000 1" << endl << "1001 1" << endl
                  << "1100 1" << endl << "1111 1" << endl;  
          //Cout
          
          if (i != (num_of_two_bits-1) || single_one_bit == true)
          {
             outFile << ".names " << aName << "[1] " << aName << "[0] "
                                  << bName << "[1] " << bName << "[0] ";

             /*if (i == (num_of_two_bits-1)  && single_one_bit == false)
                outFile << _carry->getName() << "[0]" << endl;
             else*/
                outFile << cName << "[0]" << endl;
             outFile << "0111 1" << endl << "1010 1" << endl << "1011 1" << endl
                     << "1101 1" << endl << "1110 1" << endl << "1111 1" << endl;
          }
      }
      else { //has cin
          //Y[2*i]
          outFile << ".names " << cName << "[" << (i-1)   << "] "
                               << aName << "[" << (2*i+1) << "] " << aName << "[" << (2*i) << "] "
                               << bName << "[" << (2*i+1) << "] " << bName << "[" << (2*i) << "] "
                               << yName << "[" << (2*i)   << "]"  << endl;
          outFile << "00001 1" << endl << "00011 1" << endl << "00100 1" << endl
                  << "00110 1" << endl << "01001 1" << endl << "01011 1" << endl
                  << "01100 1" << endl << "01110 1" << endl << "10000 1" << endl
                  << "10010 1" << endl << "10101 1" << endl << "10111 1" << endl
                  << "11000 1" << endl << "11010 1" << endl << "11101 1" << endl
                  << "11111 1" << endl;
          //Y[2*i+1]
          outFile << ".names " << cName << "[" << (i-1)   << "] "
                               << aName << "[" << (2*i+1) << "] " << aName << "[" << (2*i) << "] "
                               << bName << "[" << (2*i+1) << "] " << bName << "[" << (2*i) << "] "
                               << yName << "[" << (2*i+1) << "]"  << endl;
          outFile << "00010 1" << endl << "00011 1" << endl << "00101 1" << endl
                  << "00110 1" << endl << "01000 1" << endl << "01001 1" << endl
                  << "01100 1" << endl << "01111 1" << endl << "10001 1" << endl
                  << "10010 1" << endl << "10100 1" << endl << "10101 1" << endl
                  << "11000 1" << endl << "11011 1" << endl << "11110 1" << endl
                  << "11111 1" << endl;
          //Cout
          if (i != (num_of_two_bits-1) || single_one_bit == true)
          {
             outFile << ".names " << cName << "[" << (i-1)   << "] "
                                  << aName << "[" << (2*i+1) << "] " << aName << "[" << (2*i) << "] "
                                  << bName << "[" << (2*i+1) << "] " << bName << "[" << (2*i) << "] ";
             /*if (i == (num_of_two_bits-1)  && single_one_bit == false)
                outFile << _carry->getName() << "[" << i << "]" << endl;
             else*/
                outFile << cName << "[" << i << "]"  << endl;

             outFile << "00111 1" << endl << "01010 1" << endl << "01011 1" << endl
                     << "01101 1" << endl << "01110 1" << endl << "01111 1" << endl
                     << "10011 1" << endl << "10110 1" << endl << "10111 1" << endl
                     << "11001 1" << endl << "11010 1" << endl << "11011 1" << endl
                     << "11100 1" << endl << "11101 1" << endl << "11110 1" << endl
                     << "11111 1" << endl;
          }
      }
   }
   if (single_one_bit == true) {
#ifdef BLIF_DEBUG_MODE
      outFile << "#1-bits adder no. " << num_of_two_bits << endl;
#endif
      if (num_of_two_bits == 0) {
         outFile << ".names " << aName;
         if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[0]";
         outFile << " " << bName;
         if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[0]";        
         outFile << " " << yName;
         if (!getOutPin()->is1BitIoPin()) outFile << "[0] ";        
         outFile << endl << "01 1" << endl << "10 1" << endl;
      }
      else {
         outFile << ".names " << cName << "[" << (num_of_two_bits-1) << "] "
                              << aName << "[" << (2*num_of_two_bits) << "] "
                              << bName << "[" << (2*num_of_two_bits) << "] "
                              << yName << "[" << (2*num_of_two_bits) << "]"  << endl;
         outFile << "001 1" << endl << "010 1" << endl
                 << "100 1" << endl << "111 1" << endl;
      }
   }
   cktOutFile.insert(outFile.str());
}

//================================ CktSubCell ================================//

CktSubCell::CktSubCell(): CktCell( CKT_SUB_CELL )
{
   //_borrow = NULL;
}

CktSubCell::CktSubCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_SUB_CELL, isNewPin, iA, iB, oY)
{
}

CktSubCell::~CktSubCell()
{
}

void
CktSubCell::writeOutput() const
{
   genOutputArith();
}

void
CktSubCell::simulate()
{
   assert (getInPinSize() == 2);
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0) - fin1->getCell()->getSimValue(fin1);
}

CktCell* 
CktSubCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktSubCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktSubCell::writeBLIF() const
{
/*  
           Bin                           Bin 
          __|__                         __|__
     A---|     |               const---|     |
         |  -  |----Y  ==>             |  -  |--- Y  Counter
     B---|_____|            variable---|_____|
            |                             |
          Bout                           Bout
*/
/* 
    Bin    A[1] A[0] B[1] B[0]   Bout Y[1] Y[0]  |  Bin    A[1] A[0] B[1] B[0]   Bout Y[1] Y[0]
------------------------------------------------------------------------------------------------
     0      0    0    0    0      0    0    0    |   1      0    0    0    0      1    1    1 
     0      0    0    0    1      1    1    1    |   1      0    0    0    1      1    1    0
     0      0    0    1    0      1    1    0    |   1      0    0    1    0      1    0    1        
     0      0    0    1    1      1    0    1    |   1      0    0    1    1      1    0    0 
     0      0    1    0    0      0    0    1    |   1      0    1    0    0      0    0    0  
     0      0    1    0    1      0    0    0    |   1      0    1    0    1      1    1    1  
     0      0    1    1    0      1    1    1    |   1      0    1    1    0      1    1    0  
     0      0    1    1    1      1    1    0    |   1      0    1    1    1      1    0    1  
     0      1    0    0    0      0    1    0    |   1      1    0    0    0      0    0    1 
     0      1    0    0    1      0    0    1    |   1      1    0    0    1      0    0    0  
     0      1    0    1    0      0    0    0    |   1      1    0    1    0      1    1    1
     0      1    0    1    1      1    1    1    |   1      1    0    1    1      1    1    0 
     0      1    1    0    0      0    1    1    |   1      1    1    0    0      0    1    0
     0      1    1    0    1      0    1    0    |   1      1    1    0    1      0    0    1 
     0      1    1    1    0      0    0    1    |   1      1    1    1    0      0    0    0 
     0      1    1    1    1      0    0    0    |   1      1    1    1    1      1    1    1 
*/
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string cName = yName + "B_IN_OUT";   

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
#ifdef BLIF_DEBUG_MODE
   CktCellType aType = getFinCell(0)->getCellType();
   CktCellType bType = getFinCell(1)->getCellType();
   if (aType == CKT_CONST_CELL || bType == CKT_CONST_CELL) //counter
      outFile << "#counter" << endl;
   else {
      outFile << "#subtract " << endl;
      //assert (aWidth == yWidth);
      //assert (bWidth == yWidth);
      if (aWidth != yWidth || bWidth != yWidth) 
         Msg(MSG_WAR) << "Unmatch # bits subtractor !!! (yWidth, aWidth, bWidth) = (" << yWidth << ", " << aWidth << ", " << bWidth << ")" << endl;
   }
#endif
   int num_of_two_bits = -1;
   bool single_one_bit = false;
   int kBits = yWidth;
   while (kBits >= 0) {
      if (kBits == 1)
         single_one_bit = true;
      kBits -= 2;
      ++num_of_two_bits;
   }
   for (int i = 0; i < num_of_two_bits; ++i) {
#ifdef BLIF_DEBUG_MODE
      outFile << "#2-bits subtract no. " << i << endl;
#endif
      if (i == 0) {//no Cin
          //Y[0]
          outFile << ".names " << aName << "[1] " << aName << "[0] "
                               << bName << "[1] " << bName << "[0] "
                               << yName << "[0]"  << endl;
          outFile << "0001 1" << endl << "0011 1" << endl << "0100 1" << endl
                  << "0110 1" << endl << "1001 1" << endl << "1011 1" << endl
                  << "1100 1" << endl << "1110 1" << endl;    
          //Y[1]
          outFile << ".names " << aName << "[1] " << aName << "[0] "
                               << bName << "[1] " << bName << "[0] "
                               << yName << "[1]"  << endl;
          outFile << "0001 1" << endl << "0010 1" << endl << "0110 1" << endl
                  << "0111 1" << endl << "1000 1" << endl << "1011 1" << endl
                  << "1100 1" << endl << "1101 1" << endl;
          //Bout
          if (i != (num_of_two_bits-1)  || single_one_bit == true)
          {
             outFile << ".names " << aName << "[1] " << aName << "[0] "
                                  << bName << "[1] " << bName << "[0] ";

             /*if (i == (num_of_two_bits-1)  && single_one_bit == false)
                outFile << _borrow->getName() << "[0]" << endl;
             else*/
                outFile << cName << "[0]" << endl;
             outFile << "0001 1" << endl << "0010 1" << endl << "0011 1" << endl
                     << "0110 1" << endl << "0111 1" << endl << "1011 1" << endl;
          }
      }
      else { //has bin
          //Y[2*i]
          outFile << ".names " << cName << "[" << (i-1)   << "] "
                               << aName << "[" << (2*i+1) << "] " << aName << "[" << (2*i) << "] "
                               << bName << "[" << (2*i+1) << "] " << bName << "[" << (2*i) << "] "
                               << yName << "[" << (2*i)   << "]"  << endl;
          outFile << "00001 1" << endl << "00011 1" << endl << "00100 1" << endl
                  << "00110 1" << endl << "01001 1" << endl << "01011 1" << endl
                  << "01100 1" << endl << "01110 1" << endl << "10000 1" << endl
                  << "10010 1" << endl << "10101 1" << endl << "10111 1" << endl
                  << "11000 1" << endl << "11010 1" << endl << "11101 1" << endl
                  << "11111 1" << endl;
          //Y[2*i+1]
          outFile << ".names " << cName << "[" << (i-1)   << "] "
                               << aName << "[" << (2*i+1) << "] " << aName << "[" << (2*i) << "] "
                               << bName << "[" << (2*i+1) << "] " << bName << "[" << (2*i) << "] "
                               << yName << "[" << (2*i+1) << "]"  << endl;
          outFile << "00001 1" << endl << "00010 1" << endl << "00110 1" << endl
                  << "00111 1" << endl << "01000 1" << endl << "01011 1" << endl
                  << "01100 1" << endl << "01101 1" << endl << "10000 1" << endl
                  << "10001 1" << endl << "10101 1" << endl << "10110 1" << endl
                  << "11010 1" << endl << "11011 1" << endl << "11100 1" << endl
                  << "11111 1" << endl;
          //Borrow
          if (i != (num_of_two_bits-1) || single_one_bit == true)
          {
             outFile << ".names " << cName << "[" << (i-1)   << "] "
                                  << aName << "[" << (2*i+1) << "] " << aName << "[" << (2*i) << "] "
                                  << bName << "[" << (2*i+1) << "] " << bName << "[" << (2*i) << "] ";
             /*if (i == (num_of_two_bits-1)  && single_one_bit == false)
                outFile << _borrow->getName() << "[" << i << "]" << endl;
             else*/
                outFile << cName << "[" << i << "]"  << endl;

             outFile << "00001 1" << endl << "00010 1" << endl << "00011 1" << endl
                     << "00110 1" << endl << "00111 1" << endl << "01011 1" << endl
                     << "10000 1" << endl << "10001 1" << endl << "10010 1" << endl
                     << "10011 1" << endl << "10101 1" << endl << "10110 1" << endl
                     << "10111 1" << endl << "11010 1" << endl << "11011 1" << endl
                     << "11111 1" << endl;
          }
      }
   }
   if (single_one_bit == true) {
#ifdef BLIF_DEBUG_MODE
      outFile << "#1-bits subtract no. " << num_of_two_bits << endl;
#endif
      if (num_of_two_bits == 0) {
         outFile << ".names " << aName;
         if (!getInPin(0)->getOutPin()->is1BitIoPin()) outFile << "[0]";
         outFile << " " << bName;
         if (!getInPin(1)->getOutPin()->is1BitIoPin()) outFile << "[0]";        
         outFile << " " << yName;
         if (!getOutPin()->is1BitIoPin()) outFile << "[0] ";        
         outFile << endl << "01 1" << endl << "10 1" << endl;
      }
      else {
         outFile << ".names " << cName << "[" << (num_of_two_bits-1) << "] "
                              << aName << "[" << (2*num_of_two_bits) << "] "
                              << bName << "[" << (2*num_of_two_bits) << "] "
                              << yName << "[" << (2*num_of_two_bits) << "]"  << endl;
         outFile << "001 1" << endl << "010 1" << endl
                 << "100 1" << endl << "111 1" << endl;
      }
   }
   cktOutFile.insert(outFile.str());
}

//================================ CktMultCell ===============================//

CktMultCell::CktMultCell(): CktCell( CKT_MULT_CELL )
{
}

CktMultCell::CktMultCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
           : CktCell(CKT_MULT_CELL, isNewPin, iA, iB, oY)
{
}

CktMultCell::~CktMultCell()
{
}

void
CktMultCell::writeOutput() const
{
   genOutputArith();
}

CktCell* 
CktMultCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktMultCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void
CktMultCell::writeBLIF() const
{
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
#ifdef BLIF_DEBUG_MODE
   outFile << "#multiplier " << endl;
   if (aWidth != yWidth || bWidth != yWidth)
      Msg(MSG_WAR) << "Unmatch # bits multiplier !!! (yWidth, aWidth, bWidth) = (" << yWidth << ", " << aWidth << ", " << bWidth << ")" << endl;
#endif

   const string mult_and_name = yName + "_MULTIPLIER_AND";
   const string mult_add_name = yName + "_MULTIPLIER_ADD";
   const string mult_cin_name = yName + "_MULTIPLIER_CARRY";
   
   // 1. AND results for corresponding ai * bj
   outFile << ".names " << aName << "[0] " << bName << "[0] " << yName << "[0]" << endl;
   outFile << "11 1" << endl;
   for (int i = 0; i < yWidth; ++i) {
      for (int j = (i == 0) ? i + 1 : i; j < yWidth; ++j) {
         outFile << ".names " << aName << "[" << j - i << "] " << bName << "[" << i << "] " 
                 << mult_and_name << "[" << j - i << "][" << i << "]" << endl;
         outFile << "11 1" << endl;
      }
   }
   // 2. Adders for sum_out and carry
   for (int i = 1; i < yWidth; ++i) {
      if (i == 1) {
         outFile << ".names " << mult_and_name << "[1][0] " << mult_and_name << "[0][1] " << yName << "[" << i << "]" << endl;
         outFile << "10 1" << endl << "01 1" << endl;
         if (yWidth > 2) {
            outFile << ".names " << mult_and_name << "[1][0] " << mult_and_name << "[0][1] " << mult_cin_name << "[0][1]" << endl;
            outFile << "11 1" << endl;
         }
         for (int j = 2; j < yWidth; ++j) {
            outFile << ".names " << mult_and_name << "[" << j - i + 1 << "][" << i - 1 << "] " << mult_and_name << "[" << j - i << "][" << i << "] "
                    << mult_cin_name << "[" << j - i - 1 << "][" << i << "] " << mult_add_name << "[" << j - i << "][" << i << "]" << endl;
            outFile << "100 1" << endl << "010 1" << endl << "001 1" << endl << "111 1" << endl;
            if (j != (yWidth - 1)) {
               outFile << ".names " << mult_and_name << "[" << j - i + 1 << "][" << i - 1 << "] " << mult_and_name << "[" << j - i << "][" << i << "] "
                       << mult_cin_name << "[" << j - i - 1 << "][" << i << "] " << mult_cin_name << "[" << j - i << "][" << i << "]" << endl;
               outFile << "110 1" << endl << "101 1" << endl << "011 1" << endl << "111 1" << endl;
            }
         }
      }
      else {
         outFile << ".names " << mult_add_name << "[1][" << i - 1 << "] " << mult_and_name << "[0][" << i << "] "
                 << yName << "[" << i << "]" << endl;
         outFile << "10 1" << endl << "01 1" << endl;
         if (i != (yWidth - 1)) {
            outFile << ".names " << mult_add_name << "[1][" << i - 1 << "] " << mult_and_name << "[0][" << i << "] "
                    << mult_cin_name << "[0][" << i << "]" << endl;
            outFile << "11 1" << endl;
         }
         for (int j = i + 1; j < yWidth; ++j) {
            outFile << ".names " << mult_add_name << "[" << j - i + 1 << "][" << i - 1 << "] " << mult_and_name << "[" << j - i << "][" << i << "] "
                    << mult_cin_name << "[" << j - i - 1 << "][" << i << "] " << mult_add_name << "[" << j - i << "][" << i << "]" << endl;
            outFile << "100 1" << endl << "010 1" << endl << "001 1" << endl << "111 1" << endl;
            if (j != (yWidth - 1)) {
               outFile << ".names " << mult_add_name << "[" << j - i + 1 << "][" << i - 1 << "] " << mult_and_name << "[" << j - i << "][" << i << "] "
                       << mult_cin_name << "[" << j - i - 1 << "][" << i << "] " << mult_cin_name << "[" << j - i << "][" << i << "]" << endl;
               outFile << "110 1" << endl << "101 1" << endl << "011 1" << endl << "111 1" << endl;
            }
         }
      }
   }
   cktOutFile.insert(outFile.str());
}

void
CktMultCell::simulate()
{
   assert (getInPinSize() == 2);
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   const CktOutPin* fin1 = getInPin(1)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0) * fin1->getCell()->getSimValue(fin1);
}

//================================ CktDivCell ================================//

CktDivCell::CktDivCell(): CktCell( CKT_DIV_CELL )
{
   //_remainder = NULL;
}

CktDivCell::CktDivCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
           : CktCell(CKT_DIV_CELL, isNewPin, iA, iB, oY)
{
}

CktDivCell::~CktDivCell()
{
}

void
CktDivCell::simulate()
{
}

//============================== CktModuloCell ===============================//

CktModuloCell::CktModuloCell(): CktCell( CKT_MODULO_CELL )
{
}

CktModuloCell::CktModuloCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
           : CktCell(CKT_MODULO_CELL, isNewPin, iA, iB, oY)
{
}

CktModuloCell::~CktModuloCell()
{
}

void
CktModuloCell::writeOutput() const
{
   genOutputArith();
}

void
CktModuloCell::simulate()
{
}

//================================ CktShlCell ================================//

CktShlCell::CktShlCell(): CktCell( CKT_SHL_CELL )
{
}

CktShlCell::CktShlCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_SHL_CELL, isNewPin, iA, iB, oY)
{
}

CktShlCell::~CktShlCell()
{
}

void
CktShlCell::writeOutput() const
{
   genOutputArith();
}

void
CktShlCell::simulate()
{
   assert (getInPinSize() == 2);
   assert (getFinCell(1)->getCellType() == CKT_CONST_CELL);
   unsigned shift = (static_cast<CktConstCell*>(getFinCell(1)))->getBvValue()->value();
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0) << shift;
}

CktCell* 
CktShlCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktShlCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktShlCell::writeBLIF() const
{
/*                 _____
       variable---|[0]  |
                  |  << |--- Y
          const---|[1]  |
                  |_____|
                  
*/
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string shiftName;
   bool isConstShift;

   CktCellType bType = getFinCell(1)->getCellType();

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   int shiftBit = 0;

   bool isReverse1 = VLDesign.getBus(getOutPin()->getBusId())->isInverted();
   bool isReverse2 = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->isInverted();
   bool isReverse3 = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->isInverted();
   if (isReverse1 == false || isReverse2 == false || isReverse3 == false)
      cerr << "WARNING : bit reverse @ CktShlCell::writeBLIF(). Need additional procession!!" << endl;

   assert (bType == CKT_CONST_CELL);
   assert (aWidth == yWidth);
   assert (bWidth == yWidth);
   bool isShIoPin;
   if (bType == CKT_CONST_CELL) {
      shiftBit = (static_cast<CktConstCell*>(getFinCell(1)))->getBvValue()->value();
      shiftName = aName;
      isConstShift = true;
      isShIoPin = getInPin(0)->getOutPin()->is1BitIoPin();
   }
   else {
      cout << "error : non-const shifter isn't been finished(@CktShlCell::writeBLIF)" << endl;
      isConstShift = false;
      isShIoPin = false;
   }

   if (isConstShift) {
      for (int i = yWidth; i > 0; --i) {
         //if ((i-1-shiftBit) >= 0) {
         if ((i-shiftBit) > 0) {
            outFile << ".names ";
            outFile << shiftName; 
            if (!isShIoPin) outFile << "[" << ((i-1)-shiftBit) << "]"; 
            outFile << " " << yName;     
            if (!getOutPin()->is1BitIoPin()) outFile << "[" << (i-1) << "]";
#ifdef BLIF_DEBUG_MODE
            outFile << " #shl ";
#endif
            outFile << endl << "1 1" << endl;
         }
         else {
            outFile << ".names Zero0[0] " << yName;
            if (!getOutPin()->is1BitIoPin()) outFile << "[" << (i-1) << "]";
#ifdef BLIF_DEBUG_MODE
            outFile << " #shl ";
#endif
            outFile << endl << "- 0" << endl;
         }
      }
   }
   else {
   }

   cktOutFile.insert(outFile.str());
}

//================================ CktShrCell ================================//

CktShrCell::CktShrCell(): CktCell( CKT_SHR_CELL )
{
}

CktShrCell::CktShrCell(bool isNewPin, CktInPin*& iA, CktInPin*& iB, CktOutPin*& oY) 
          : CktCell(CKT_SHR_CELL, isNewPin, iA, iB, oY)
{
}

CktShrCell::~CktShrCell()
{
}

void
CktShrCell::writeOutput() const
{
   genOutputArith();
}

void
CktShrCell::simulate()
{
   assert (getInPinSize() == 2);
   assert (getFinCell(1)->getCellType() == CKT_CONST_CELL);
   unsigned shift = (static_cast<CktConstCell*>(getFinCell(1)))->getBvValue()->value();
   const CktOutPin* fin0 = getInPin(0)->getOutPin();
   _simValue = fin0->getCell()->getSimValue(fin0) >> shift;
}

CktCell* 
CktShrCell::cellDuplicate(string& prefix, OutPinMap& outPinMap, InPinMap& inPinMap) const
{
   CktCell* newCell = new CktShrCell();
   pinsDuplicate(newCell, prefix, outPinMap, inPinMap);
   return newCell;
}

void 
CktShrCell::writeBLIF() const
{
/*                 _____
       variable---|[0]  |
                  |  >> |--- Y
          const---|[1]  |
                  |_____|
*/
   stringstream outFile;
   string yName = getOutPin()->getName();
   string aName = getInPin(0)->getOutPin()->getName();
   string bName = getInPin(1)->getOutPin()->getName();
   string shiftName;
   bool isConstShift;

   CktCellType bType = getFinCell(1)->getCellType();

   int yWidth = VLDesign.getBus(getOutPin()->getBusId())->getWidth();
   int aWidth = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->getWidth();
   int bWidth = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->getWidth();
   int shiftBit = 0;

   bool isReverse1 = VLDesign.getBus(getOutPin()->getBusId())->isInverted();
   bool isReverse2 = VLDesign.getBus(getInPin(0)->getOutPin()->getBusId())->isInverted();
   bool isReverse3 = VLDesign.getBus(getInPin(1)->getOutPin()->getBusId())->isInverted();
   if (isReverse1 == false || isReverse2 == false || isReverse3 == false)
      cerr << "WARNING : bit reverse @ CktShlCell::writeBLIF(). Need additional procession!!" << endl;

   assert (bType == CKT_CONST_CELL);
   assert (aWidth == yWidth);
   assert (bWidth == yWidth);

   bool isShIoPin;
   if (bType == CKT_CONST_CELL) {
      shiftBit = (static_cast<CktConstCell*>(getFinCell(1)))->getBvValue()->value();
      shiftName = aName;
      isConstShift = true;
      isShIoPin = getInPin(0)->getOutPin()->is1BitIoPin();
   }
   else {
      cout << "error : non-const shifter isn't been finished(@CktShlCell::writeBLIF)" << endl;
      isConstShift = false;
      isShIoPin = false;
   }

   if (isConstShift) {
      for (int i = yWidth; i > 0; --i) {
         //if ((yWidth-(i-1)) <= shiftBit) {
         if ((yWidth-i) < shiftBit) {
            outFile << ".names Zero0[0] " << yName;
            if (!getOutPin()->is1BitIoPin()) outFile << "[" << (i-1) << "]";
#ifdef BLIF_DEBUG_MODE
            outFile << " #shr ";
#endif
            outFile << endl << "- 0" << endl;
         }
         else {
            outFile << ".names ";
            outFile << shiftName;
            if (!isShIoPin) outFile << "[" << ((i-1)+shiftBit) << "]"; 
            outFile << " " << yName;
            if (!getOutPin()->is1BitIoPin()) outFile << "[" << (i-1) << "]";
#ifdef BLIF_DEBUG_MODE
            outFile << " #shr ";
#endif
            outFile << endl << "1 1" << endl;
         }
      }
   }
   else
   {
   }
   cktOutFile.insert(outFile.str());
}

/*
//================================ CktRtlCell ================================//

CktRtlCell::CktRtlCell(): CktCell( CKT_RTL_CELL )
{
}

CktRtlCell::~CktRtlCell()
{
}

void
CktRtlCell::simulate()
{
}
//================================ CktRtrCell ================================//

CktRtrCell::CktRtrCell(): CktCell( CKT_RTR_CELL )
{
}

CktRtrCell::~CktRtrCell()
{
}

void
CktRtrCell::simulate()
{
}*/

#endif

