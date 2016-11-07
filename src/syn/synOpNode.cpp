/****************************************************************************
  FileName     [ synOpNode.cpp ]
  Package      [ syn ]
  Synopsis     [ Synthesis function of Op Node class ]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SYN_OP_NODE_SOURCE
#define SYN_OP_NODE_SOURCE

//---------------------------
//  system include
//---------------------------

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

//---------------------------
//  user include
//---------------------------
#include "VLGlobe.h"
#include "cktLogicCell.h"
#include "cktArithCell.h"
#include "cktZCell.h"
#include "vlpOpNode.h"

//---------------------------
//  Global variables
//---------------------------
extern SYN_usage*       SYN;
extern bool             busDirectionDefault;
extern int              netNumber;
extern VlpDesign&       VLDesign;
extern const VlgModule* curVlpModule;

//=================================== class VlpIntNode ===================================//
CktOutPin* 
VlpIntNode::synthesis() const
{
   CktOutPin* outPin;
   SYN->insertCktCell(new CktConstCell(outPin, _number));
   // Note : I take integer 32 bits width by default
   int busId = VLDesign.genBusId(32, 0, 31, busDirectionDefault);
   outPin->setBusId(busId);
   return outPin;
}

bool
VlpIntNode::isConst() const
{
   return true;
}

int
VlpIntNode::constPropagate() const
{
   return _number;
}

//================================= class VlpArithOpNode =================================//
CktOutPin* 
VlpArithOpNode::synthesis() const
{
   int busIdO;
   CktCellType type   = typeVlp2Cir( _arithOp );
   CktOutPin* oLeft   = _leftOperand->synthesis();
   CktOutPin* oRight  = _rightOperand->synthesis();
   const SynBus* busL = VLDesign.getBus(oLeft->getBusId());
   const SynBus* busR = VLDesign.getBus(oRight->getBusId());
   unsigned widthL = busL->getWidth();
   unsigned widthR = busR->getWidth();
   unsigned widthO;
   
   oLeft->checkBusWidth(oRight);
   if (widthL >= widthR) widthO = widthL;
   else                  widthO = widthR;
   busIdO = VLDesign.genBusId(widthO, 0, widthO-1, busDirectionDefault);
/*
   if (widthL != widthR) {
      if (oLeft->getCell() != NULL && oLeft->getCell()->getCellType() == CKT_CONST_CELL)
         oLeft->setBusId(busIdO);
      if (oRight->getCell() != NULL && oRight->getCell()->getCellType() == CKT_CONST_CELL)
         oRight->setBusId(busIdO);
   }*/
   //check node sharing
   CktOutPin* oPin = SYN->outPinMap.getSharPin(type, oLeft, oRight);
   if (oPin != 0)
      return oPin;
   else {
      CktInPin *iLeft, *iRight;      
      SYN->insertCktCell( createCell(iLeft, iRight, oPin) );
      //connect between inPin & outPin
      oLeft->connInPin(iLeft);  oRight->connInPin(iRight);
      iLeft->connOutPin(oLeft); iRight->connOutPin(oRight);

      oPin->setBusId(busIdO);
      SYN->outPinMap.insert(type, oLeft, oRight, oPin);
      
      return oPin;
   }
}

CktCellType 
VlpArithOpNode::typeVlp2Cir(ArithOpClass type) const
{
   switch (type) {
     case ARITHOP_ADD :      return CKT_ADD_CELL;
     case ARITHOP_SUBTRACT : return CKT_SUB_CELL;
     case ARITHOP_MULTIPLY : return CKT_MULT_CELL;
     default :               return CKT_ERROR_CELL;
   }
}

CktCell* 
VlpArithOpNode::createCell(CktInPin*& iLeft, CktInPin*& iRight, CktOutPin*& oPin) const
{
   CktCell* cell;
   switch (_arithOp) {
     case ARITHOP_ADD :      cell = new CktAddCell(true, iLeft, iRight, oPin);  break;
     case ARITHOP_SUBTRACT : cell = new CktSubCell(true, iLeft, iRight, oPin);  break;
     case ARITHOP_MULTIPLY : cell = new CktMultCell(true, iLeft, iRight, oPin); break;
     default :               assert(0);
   }

   return cell;
}

bool
VlpArithOpNode::isConst() const
{
   if (_leftOperand->isConst()) {
      if (_rightOperand->isConst()) {
         switch (_arithOp) {
            case ARITHOP_ADD :
            case ARITHOP_SUBTRACT :
            case ARITHOP_MULTIPLY:
               return true;
            default : 
               return false;
         }
      }
      else return false;
   }
   else return false;
}

int
VlpArithOpNode::constPropagate() const
{
   int a = _leftOperand->constPropagate();
   int b = _rightOperand->constPropagate();
   switch (_arithOp) {
     case ARITHOP_ADD :      return (a + b);
     case ARITHOP_SUBTRACT : return (a - b);
     case ARITHOP_MULTIPLY : return (a * b);
     default :               assert(0);
   }
}

bool
VlpSignNode::isConst() const
{
   if (_operand->isConst()) {
      switch (_arithOp) {
         case ARITHOP_UNARY_PLUS :
         case ARITHOP_UNARY_MINUS : 
            return true;
         default : 
            return false;
      }
   }
   else return false;
}

int 
VlpSignNode::constPropagate() const
{
   int a = _operand->constPropagate();
   if (_arithOp == ARITHOP_UNARY_PLUS)
      return a;
   else if (_arithOp == ARITHOP_UNARY_MINUS)
      return (0 - a);
   else
      assert (0);
}

//=============================== class VlpConditionalNode ===============================//
CktOutPin* 
VlpConditionalNode::synthesis() const
{
   CktOutPin* oPin;
   bool trueFullz  = false;
   bool falseFullz = false;
   CktOutPin* oTrue   = _trueChild->synthesis();
   CktOutPin* oFalse  = _falseChild->synthesis();
   CktOutPin* oSel    = _conditional->synthesis();
   const SynBus* busT = VLDesign.getBus(oTrue->getBusId());
   const SynBus* busF = VLDesign.getBus(oFalse->getBusId());
   
   if (VLDesign.getBus(oSel->getBusId())->getWidth() > 1)
      oSel = oSel->genGreaterCell();
   if (oTrue->getCell() != 0)
      if (oTrue->getCell()->getCellType() == CKT_CONST_CELL) {
         Bv4* bv = (static_cast <CktConstCell*>(oTrue->getCell()))->getBvValue();
         if (bv->fullz()) {
            oSel      = oSel->genInvCell();
            trueFullz = true;
         }
      }  
   if (oFalse->getCell() != 0)
      if (oFalse->getCell()->getCellType() == CKT_CONST_CELL) {
         Bv4* bv = (static_cast <CktConstCell*>(oFalse->getCell()))->getBvValue();
         if (bv->fullz())
            falseFullz = true;
      }
   if (trueFullz || falseFullz) {
      CktOutPin* oData;
      if (trueFullz && falseFullz)
         cerr << "Warning code ### - both of child are high impedance." << endl;
      
      if (trueFullz) oData = oFalse;
      else           oData = oTrue; // falseFullz

      CktInPin *iData, *iSel;
      SYN->insertCktCell(new CktBufifCell(true, iData, iSel, oPin));   
      oData->connInPin(iData); iData->connOutPin(oData);
      oSel->connInPin(iSel);   iSel->connOutPin(oSel);
      oPin->setBusId(oData->getBusId());      
      return oPin;
   }
   else {
      unsigned tmpWidth;
      if (busT->getWidth() >= busF->getWidth()) tmpWidth = busT->getWidth();
      else                                      tmpWidth = busF->getWidth();

      int busId;
      if (tmpWidth > 0)
         busId = VLDesign.genBusId(tmpWidth, 0, tmpWidth-1, busDirectionDefault);
      else { // iFalse, iTrue => the finOuts of the const cell
         busId = VLDesign.genBusId(1, 0, 0, busDirectionDefault); // generate integer default id
         oTrue->setBusId(busId);  oFalse->setBusId(busId);
      }
      CktInPin *iFalse, *iTrue, *iSel;
      SYN->insertCktCell(new CktMuxCell(true, iFalse, iTrue, iSel, oPin));
      //connect between inPin & outPin
      oFalse->connInPin(iFalse); iFalse->connOutPin(oFalse);
      oTrue->connInPin(iTrue);   iTrue->connOutPin(oTrue);
      oSel->connInPin(iSel);     iSel->connOutPin(oSel);
      oPin->setBusId(busId);   
      return oPin;
   }
}

//================================ class VlpEqualityNode =================================//
CktOutPin* 
VlpEqualityNode::synthesis() const
{
   CktOutPin* oLeft  = _leftOperand->synthesis();
   CktOutPin* oRight = _rightOperand->synthesis();
   oLeft->checkBusWidth(oRight);
   if (VLDesign.getBus(oLeft->getBusId())->getWidth() !=  VLDesign.getBus(oRight->getBusId())->getWidth())
      cerr << "Warning : bus's width are not the same @ VlpEquality::synthesis()." 
           << " LhsName = " << oLeft->getName() 
           << " RhsName = " << oRight->getName() << endl;
   
   CktOutPin* oPin = genSharPin(oLeft, oRight);
   if (oPin != 0) 
      return oPin;
   else { // can't find the sharing pin => to new one
      int tmpBusId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
      CktInPin *iLeft, *iRight;      
      CktCell* cell = new CktEqualityCell(true, iLeft, iRight, oPin);
      SYN->insertCktCell(cell);
      if ( (_equality ==  EQUALITY_LOG_EQU) || (_equality ==  EQUALITY_LOG_INEQU) )
         static_cast<CktEqualityCell*>(cell)->setLogicEq();
      else
         static_cast<CktEqualityCell*>(cell)->setCaseEq();         
      // connect between inPin & outPin
      oLeft->connInPin(iLeft);   iLeft->connOutPin(oLeft);
      oRight->connInPin(iRight); iRight->connOutPin(oRight);
      oPin->setBusId(tmpBusId);      
      SYN->outPinMap.insert(CKT_EQUALITY_CELL, oLeft, oRight, oPin);

      if ( (_equality ==  EQUALITY_LOG_EQU) || (_equality ==  EQUALITY_CASE_EQU) )
         return oPin;
      else { // INEQU => EQUALITY_LOG_INEQU || EQUALITY_CASE_INEQU
         CktInPin* iPin;
         CktOutPin* oInvPin;
         SYN->insertCktCell( new CktInvCell(true, iPin, oInvPin) );
         // connect between outPin & inPin
         oPin->connInPin(iPin); iPin->connOutPin(oPin);     
         oInvPin->setBusId(tmpBusId);
      
         SYN->outPinMap.insert(CKT_INV_CELL, oPin, 0, oInvPin);      
         return oInvPin;
      }
   }
}

// 1. find the existent pin to share
// three situations now
// _equality == EQUALITY_LOG_EQU,   oPin == 0, no equality cell
// _equality == EQUALITY_LOG_INEQU, oPin == 0, no equality cell
// _equality == EQUALITY_LOG_INEQU, oPin != 0, oInvPin == 0, have equality cell, no inv cell.   
CktOutPin* 
VlpEqualityNode::genSharPin(CktOutPin* oLeft, CktOutPin* oRight) const
{
   CktOutPin *oPin, *oInvPin;   
   if ( (_equality == EQUALITY_LOG_EQU) || (_equality == EQUALITY_LOG_INEQU)) {
      oPin = SYN->outPinMap.getSharPin(CKT_EQUALITY_CELL, oLeft, oRight);
      if (oPin != 0) {
         if (static_cast<CktEqualityCell*>(oPin->getCell())->isLogicEq() == true) {
            if (_equality == EQUALITY_LOG_EQU)
               return oPin;
            else {  // _equality ==  EQUALITY_LOG_INEQU
               oInvPin = SYN->outPinMap.getSharPin(CKT_INV_CELL, oPin, 0);
               if (oInvPin != 0)
                  return oInvPin;
               else {
                  CktInPin* iPin;
                  SYN->insertCktCell( new CktInvCell(true, iPin, oInvPin) );
                  // connect between outPin & inPin
                  oPin->connInPin(iPin); iPin->connOutPin(oPin);     
                  oInvPin->setBusId(oPin->getBusId());
      
                  SYN->outPinMap.insert(CKT_INV_CELL, oPin, 0, oInvPin);      
                  return oInvPin;
               }
            }
         }
      }
   }
   else { // (_equality ==  EQUALITY_CASE_EQU) || (_equality ==  EQUALITY_CASE_INEQU) 
      assert(0); // to check if the synthesis is ok ?
      oPin = SYN->outPinMap.getSharPin(CKT_EQUALITY_CELL, oLeft, oRight);
      if (oPin != 0) {
         if (static_cast<CktEqualityCell*>(oPin->getCell())->isLogicEq() == false) {
            if (_equality ==  EQUALITY_CASE_EQU)
               return oPin;
            else { // _equality ==  EQUALITY_CASE_INEQU
               oInvPin = SYN->outPinMap.getSharPin(CKT_INV_CELL, oPin, 0);
               if (oInvPin != 0)
                  return oInvPin;
               else {
                  CktInPin* iPin;
                  SYN->insertCktCell( new CktInvCell(true, iPin, oInvPin) );
                  // connect between outPin & inPin
                  oPin->connInPin(iPin); iPin->connOutPin(oPin);     
                  oInvPin->setBusId(oPin->getBusId());
                  SYN->outPinMap.insert(CKT_INV_CELL, oPin, 0, oInvPin);      
                  return oInvPin;
               }
            }
         }
      }
   }
   return 0;
}

//================================ class VlpReductOpNode =================================//
CktOutPin* 
VlpReductOpNode::synthesis() const
{
   CktOutPin* oOprn = _operand->synthesis();
   CktCellType type = typeVlp2Cir(_reductOp);
   CktOutPin* oPin = SYN->outPinMap.getSharPin(type, oOprn, 0);
   if ( oPin != 0 )
      return oPin;
   else {
      CktInPin* iPin;
      SYN->insertCktCell( createCell(iPin, oPin) );            
      //connect between outPin & inPin
      oOprn->connInPin(iPin);
      iPin->connOutPin(oOprn);
      int busId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
      oPin->setBusId(busId);      
      SYN->outPinMap.insert(type, oOprn, 0, oPin);
      return oPin;
   }
}

CktCellType 
VlpReductOpNode::typeVlp2Cir(ReductOpClass type) const
{
   switch (type) {
     case REDUCTOP_AND :  return CKT_AND_CELL;
     case REDUCTOP_OR :   return CKT_OR_CELL;
     case REDUCTOP_NAND : return CKT_NAND_CELL;
     case REDUCTOP_NOR :  return CKT_NOR_CELL;
     case REDUCTOP_XOR :  return CKT_XOR_CELL;
     case REDUCTOP_XNOR : return CKT_XNOR_CELL;
     default :            assert(0);
   }
}

CktCell* 
VlpReductOpNode::createCell(CktInPin*& iPin, CktOutPin*& oPin) const
{
   CktCell* cell;
   switch (_reductOp) {
     case REDUCTOP_AND :  cell = new CktAndCell(true, iPin, oPin);  break;
     case REDUCTOP_OR :   cell = new CktOrCell(true, iPin, oPin);   break;
     case REDUCTOP_NAND : cell = new CktNandCell(true, iPin, oPin); break;
     case REDUCTOP_NOR :  cell = new CktNorCell(true, iPin, oPin);  break;
     case REDUCTOP_XOR :  cell = new CktXorCell(true, iPin, oPin);  break;
     case REDUCTOP_XNOR : cell = new CktXnorCell(true, iPin, oPin); break;
     default :            assert(0);
   }
   return cell;
}

//================================= class VlpLogicOpNode =================================//
CktOutPin* 
VlpLogicOpNode::synthesis() const
{
   CktOutPin* oLeft  = _leftOperand->synthesis();
   CktOutPin* oRight = _rightOperand->synthesis();
   CktCellType type;
   int id1 = oLeft->getBusId();
   int id2 = oRight->getBusId();
   // Ex:
   //    reg [3:0] a,b;
   //    "if ( a && b )"
   //    will be translate into "if ( (|a) && (|b) )
   //    apply "reduction or" operations on a and b at first.
   if (VLDesign.getBus(id1)->getWidth() > 1)
      oLeft = oLeft->genGreaterCell();
   if (VLDesign.getBus(id2)->getWidth() > 1)
      oRight = oRight->genGreaterCell();
   type = typeVlp2Cir(_logicOp);
   CktOutPin* oPin = SYN->outPinMap.getSharPin(type, oLeft, oRight);
   if (oPin != 0)
      return oPin;
   else {
      CktInPin *iLeft, *iRight;
      SYN->insertCktCell( createCell(iLeft, iRight, oPin) );
      // connect between inPin & outPin
      oLeft->connInPin(iLeft);   iLeft->connOutPin(oLeft);
      oRight->connInPin(iRight); iRight->connOutPin(oRight);
      
      int tmpBusId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
      oPin->setBusId(tmpBusId);
      SYN->outPinMap.insert(type, oLeft, oRight, oPin);      
      return oPin;
   }
}

CktCellType 
VlpLogicOpNode::typeVlp2Cir(LogicOpClass type) const
{
   switch (type) {
     case LOGICOP_AND : return CKT_AND_CELL;
     case LOGICOP_OR :  return CKT_OR_CELL;
     case LOGICOP_NOT : return CKT_INV_CELL;
     default :          assert(0);
   }
}

CktCell*
VlpLogicOpNode::createCell(CktInPin*& iLeft, CktInPin*& iRight, CktOutPin*& oPin) const
{
   CktCell* cell;
   if (_logicOp ==  LOGICOP_AND)     cell = new CktAndCell(true, iLeft, iRight, oPin);
   else if (_logicOp ==  LOGICOP_OR) cell = new CktOrCell(true, iLeft, iRight, oPin);
   else
      assert(0);
   return cell;
}

//=============================== class VlpLogicNotOpNode ================================//
CktOutPin* 
VlpLogicNotOpNode::synthesis() const
{
   CktOutPin* oOprn = _operand->synthesis();  
   CktCellType type = CKT_INV_CELL;
   // Ex:
   //    reg [3:0] a;
   //    "if ( !a )"
   //    will be translate into "if ( !(|a) )
   //    apply "reduction or" operations on a at first.
   if (VLDesign.getBus(oOprn->getBusId())->getWidth() > 1)
      oOprn = oOprn->genGreaterCell();
   CktOutPin* oPin = SYN->outPinMap.getSharPin(type, oOprn, 0);
   if (oPin != 0)
      return oPin;
   else {
      CktInPin* iPin;      
      SYN->insertCktCell( new CktInvCell(true, iPin, oPin) );
      //connect between inPin & outPin
      oOprn->connInPin(iPin);
      iPin->connOutPin(oOprn);
      oPin->setBusId(oOprn->getBusId());
      SYN->outPinMap.insert(type, oOprn, 0, oPin);      
      return oPin;
   }
}

//================================= class VlpShiftOpNode =================================//
CktOutPin* 
VlpShiftOpNode::synthesis() const
{
   CktOutPin* oLeft  = _leftOperand->synthesis();
   CktOutPin* oRight = _rightOperand->synthesis();
   CktCellType type  = typeVlp2Cir(_shiftOp);
   
   oLeft->checkBusWidth(oRight);
   
   CktOutPin* oPin = SYN->outPinMap.getSharPin(type, oLeft, oRight);   
   if ( oPin != 0 )
      return oPin;
   else {
      CktInPin *iLeft, *iRight;
      SYN->insertCktCell( createCell(iLeft, iRight, oPin) );
      //connect between inPin & outPin
      oLeft->connInPin(iLeft);
      oRight->connInPin(iRight);
      iLeft->connOutPin(oLeft);
      iRight->connOutPin(oRight);
   
      oPin->setBusId(oLeft->getBusId());
      SYN->outPinMap.insert(type, oLeft, oRight, oPin);      
      return oPin;
   }
}

CktCellType 
VlpShiftOpNode::typeVlp2Cir(ShiftOpClass type) const
{
   if (type ==  SHIFTOP_RSH)      return CKT_SHL_CELL;
   else if (type ==  SHIFTOP_LSH) return CKT_SHR_CELL;
   else
      assert(0);
}


CktCell* 
VlpShiftOpNode::createCell(CktInPin*& iLeft, CktInPin*& iRight, CktOutPin*& oPin) const
{
   CktCell* cell;
   if (_shiftOp ==  SHIFTOP_RSH)      cell = new CktShrCell(true, iLeft, iRight, oPin);
   else if (_shiftOp ==  SHIFTOP_LSH) cell = new CktShlCell(true, iLeft, iRight, oPin);
   else
      assert(0);
   return cell;
}

//================================ class VlpRelateOpNode =================================//
CktOutPin* 
VlpRelateOpNode::synthesis() const
{
   CktOutPin* oLeft  = _leftOperand->synthesis();
   CktOutPin* oRight = _rightOperand->synthesis();
      
   oLeft->checkBusWidth(oRight);
      
   int id1 = oLeft->getBusId();
   int id2 = oRight->getBusId();
   if (VLDesign.getBus(id1)->getWidth() != VLDesign.getBus(id2)->getWidth())
      cerr << "Warning : bus's width are not the same @ VlpRelateOpNode::synthesis()." 
           << " LhsName = " << oLeft->getName() 
           << " RhsName = " << oRight->getName() << endl;

   int tmpBusId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
   CktCellType type = typeVlp2Cir(_relateOp);
   CktOutPin* oPin = SYN->outPinMap.getSharPin(type, oLeft, oRight);  
   if (oPin != 0)
      return oPin;
   else {
      CktInPin *iLeft, *iRight;
      SYN->insertCktCell( createCell(iLeft, iRight, oPin) );
      //connect between inPin & outPin
      oLeft->connInPin(iLeft);   iLeft->connOutPin(oLeft);
      oRight->connInPin(iRight); iRight->connOutPin(oRight);
      
      oPin->setBusId(tmpBusId);      
      SYN->outPinMap.insert(type, oLeft, oRight, oPin);      
      return oPin;
   }
}

CktCellType 
VlpRelateOpNode::typeVlp2Cir(RelateOpClass type) const
{
   switch (type) {
     case RELATEOP_GEQ :     return CKT_GEQ_CELL;
     case RELATEOP_GREATER : return CKT_GREATER_CELL;
     case RELATEOP_LEQ :     return CKT_LEQ_CELL;
     case RELATEOP_LESS :    return CKT_LESS_CELL;
     default :               assert(0);
   }
}


CktCell* 
VlpRelateOpNode::createCell(CktInPin*& iLeft, CktInPin*& iRight, CktOutPin*& oPin) const
{
   CktCell* cell;
   switch (_relateOp) {
     case RELATEOP_GEQ :     cell = new CktGeqCell(true, iLeft, iRight, oPin);     break;
     case RELATEOP_GREATER : cell = new CktGreaterCell(true, iLeft, iRight, oPin); break;
     case RELATEOP_LEQ :     cell = new CktLeqCell(true, iLeft, iRight, oPin);     break;
     case RELATEOP_LESS :    cell = new CktLessCell(true, iLeft, iRight, oPin);    break;
     default :               assert(0);
   }
   return cell;
}

//=============================== class VlpBitWiseOpNode =================================//
CktOutPin* 
VlpBitWiseOpNode::synthesis() const
{
   CktOutPin* oLeft  = _leftOperand->synthesis();
   CktOutPin* oRight = _rightOperand->synthesis();
   CktCellType type  = typeVlp2Cir(_bitWiseOp);
   oLeft->checkBusWidth(oRight);
   int id1 = oLeft->getBusId();
   int id2 = oRight->getBusId();

   if (VLDesign.getBus(id1)->getWidth() !=  VLDesign.getBus(id2)->getWidth()) {
      cerr << "Warning : bus's width are not the same @ VlpBitWiseOpNode::synthesis()." 
           << " LhsName = " << oLeft->getName() 
           << " RhsName = " << oRight->getName() << endl;
   }
   unsigned tmpWidth = VLDesign.getBus(id1)->getWidth();
   int tmpBusId = VLDesign.genBusId(tmpWidth, 0, tmpWidth-1, busDirectionDefault);   
   CktOutPin* oPin = SYN->outPinMap.getSharPin(type, oLeft, oRight);         
   if (oPin != 0)
      return oPin;
   else {
      CktInPin *iLeft, *iRight;
      SYN->insertCktCell( createCell(iLeft, iRight, oPin) );
      //connect between inPin & outPin
      oLeft->connInPin(iLeft);   iLeft->connOutPin(oLeft);
      oRight->connInPin(iRight); iRight->connOutPin(oRight);
   
      oPin->setBusId(tmpBusId);
      SYN->outPinMap.insert(type, oLeft, oRight, oPin);         
      return oPin;
   }
}

CktOutPin* 
VlpBitNotOpNode::synthesis() const
{
   CktOutPin* oLeft = _operand->synthesis();
   CktCellType type = CKT_INV_CELL;
   CktOutPin* oPin  = SYN->outPinMap.getSharPin(type, oLeft, 0);
   if (oPin != 0)
      return oPin;
   else {
      CktInPin* iPin;
      SYN->insertCktCell( new CktInvCell(true, iPin, oPin) );
      oLeft->connInPin(iPin); iPin->connOutPin(oLeft);

      oPin->setBusId(oLeft->getBusId());      
      SYN->outPinMap.insert(type, oLeft, 0, oPin);
      return oPin;
   }
}

CktCellType 
VlpBitWiseOpNode::typeVlp2Cir(BitWiseOpClass type) const
{
   switch (type) {
     case BITWISEOP_NOT :  return CKT_INV_CELL;
     case BITWISEOP_AND :  return CKT_AND_CELL;
     case BITWISEOP_OR :   return CKT_OR_CELL;
     case BITWISEOP_XOR :  return CKT_XOR_CELL;
     case BITWISEOP_XNOR : return CKT_XNOR_CELL;
     default :             assert(0);
   }
}

CktCell* 
VlpBitWiseOpNode::createCell(CktInPin*& iLeft, CktInPin*& iRight, CktOutPin*& oPin) const
{
   CktCell* cell;
   switch (_bitWiseOp) {
     case BITWISEOP_AND :  cell = new CktAndCell(true, iLeft, iRight, oPin);  break;
     case BITWISEOP_OR :   cell = new CktOrCell(true, iLeft, iRight, oPin);   break;
     case BITWISEOP_XOR :  cell = new CktXorCell(true, iLeft, iRight, oPin);  break;
     case BITWISEOP_XNOR : cell = new CktXnorCell(true, iLeft, iRight, oPin); break;
     default :             assert(0);
   }
   return cell;
}

//=============================== class VlpConcatenateNode ================================//
CktOutPin* 
VlpConcatenateNode::synthesis() const // Rhs usage
{
   const VlpBaseNode* ptr = _content;
   //a = {data1}, only one data in assignment
   if (ptr->getNext() == 0)
      return ptr->synthesis();
   //a = {data1, data2}, more than one assignment
   else {
      if (isBvConcatenate()) {
         Bv4* bv = new Bv4(getConcatenateBv());
         int busId = VLDesign.genBusId(bv->bits(), 0, bv->bits()-1, busDirectionDefault);
         int id = SYN->constAry.isExist(bv, busId);
         CktOutPin* outPin;
         if (id != -1) {
            delete bv;
            outPin = SYN->constAry[id];
         }
         else {
            SYN->insertCktCell(new CktConstCell(outPin, bv));
            outPin->setBusId(busId);
            SYN->constAry.insert(outPin);
         }
         return outPin;
      }
      else {
         CktOutPin* mergeOut;
         CktCell* mergeCell = new CktMergeCell(true, mergeOut);
         SYN->insertCktCell(mergeCell);
        
         CktInPin* iData;
         CktOutPin* oData;
         unsigned width = 0;
         for (; ptr != 0; ptr = ptr->getNext()) {
            iData = new CktInPin;
            iData->connCell(mergeCell); mergeCell->connInPin(iData);
         
            oData = ptr->synthesis();
            oData->connInPin(iData); iData->connOutPin(oData);
         
            if (VLDesign.getBus(oData->getBusId())->getWidth() != 0)
               width += VLDesign.getBus(oData->getBusId())->getWidth();
            else
               width += 1; // [toCheck]
         }
         int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
         mergeOut->setBusId(busId); 
         return mergeOut;
      }
   }
}

void 
VlpConcatenateNode::synLhsConcatenate(vector<unsigned>& busIdAry, 
                    vector<string>& sigNameAry, unsigned& lhsWidth) const // Lhs usage
{
   unsigned busId = 0;
   const VlpBaseNode* exp = 0;
   string name;

   for (const VlpBaseNode* content = getContent(); content; content = content->getNext()) {
      assert(content->getNodeClass() == NODECLASS_CONNECT); // content is a connect node
      exp = static_cast<const VlpConnectNode*>(content)->getExp();
      assert (exp->getNodeClass() == NODECLASS_SIGNAL);

      name = BaseModule :: getName(static_cast<const VlpSignalNode*>(exp)->getNameId());
      if (curVlpModule->isMemAry(name))
         busId = static_cast<const VlpSignalNode*>(exp)->genMemBusId(); //gen SynMemory if mem not exist
      else
         busId = static_cast<const VlpSignalNode*>(exp)->genLhsBusId(); //also generate lhs total width pin

      lhsWidth += VLDesign.getBus(busId)->getWidth();     
      busIdAry.push_back(busId);
      sigNameAry.push_back(name);
   }
}

const VlpSignalNode*
VlpConcatenateNode::getConcatMemSignal(unsigned num) const
{
   unsigned i = 0;
   const VlpBaseNode* exp = 0;
   for (const VlpBaseNode* content = getContent(); content; content = content->getNext()) {
      exp = static_cast<const VlpConnectNode*>(content)->getExp();
      if (i == num)
         return static_cast<const VlpSignalNode*>(exp); // exp must be VlpSignalNode*
      ++i;
   }
   cerr << "Error: Miss Memory signal in concatenate operator !!" << endl;
   assert (0);
   return 0;
} 

//=============================== class VlpReplicationNode ===============================//
CktOutPin* 
VlpReplicationNode::synthesis() const
{   
   int times = _times->constPropagate();
   CktOutPin* oData = _content->synthesis();
   if (times > 1) {
      unsigned width = 0;
      CktOutPin* mergeOut;
      CktCell* mergeCell = new CktMergeCell(true, mergeOut);
      SYN->insertCktCell(mergeCell);
      CktInPin* iData;

      for (int i = 0; i < times; ++i) {
         iData = new CktInPin;
         iData->connCell(mergeCell); mergeCell->connInPin(iData);
         oData->connInPin(iData);    iData->connOutPin(oData);

         if (VLDesign.getBus(oData->getBusId())->getWidth() != 0)
            width += VLDesign.getBus(oData->getBusId())->getWidth();
         else {
            if (oData->getCell() != 0) {
               if (oData->getCell()->getCellType() == CKT_CONST_CELL) {
                  cerr << "Error : In replication operation but has no width information, value = "
                       << static_cast<CktConstCell*>(oData->getCell())->getBvValue()->value()
                       << endl;
               }
               else
                  cerr << "Error : In replication operation but has no width information, name = "
                       << oData->getName() << endl;
            }
            width += 1;
         }
      }  
      int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
      mergeOut->setBusId(busId);
      return mergeOut;
   }
   else if (times == 1)
      return oData;
   else // times == 0
      assert (0);
}

//================================ class VlpConnectNode ==================================//
CktOutPin* 
VlpConnectNode::synthesis() const
{
   return _exp->synthesis();
}

//================================== class VlpParamNode ==================================//
CktOutPin* 
VlpParamNode::synthesis() const
{
   //paramter is used as const substitution
   string& name = _pPair->param;
   CktOutPin* oPin = SYN->outPinMap.getSharPin(name); // find in outPinMap
   if (oPin != 0)
      return oPin;
   else if (SYN->curPOLAry == 0) { // find in constAry at no POL condition  
      if (_pPair->content->getNodeClass() == NODECLASS_SIGNAL) {
         const VlpSignalNode* sNode = static_cast<const VlpSignalNode*>(_pPair->content);
         if (sNode->isBitBaseStr()) {
            Bv4 bv(BaseModule::getName(sNode->getNameId()));
            int width = bv.bits();
            int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
            int id = SYN->constAry.isExist(&bv, busId);
            if (id != -1) return SYN->constAry[id];
         }
      }
   }
   if (SYN->curPOLAry != 0) { // check parameter overload first
      for (unsigned i = 0; i < SYN->curPOLAry->size(); ++i)
         if (name == SYN->curPOLAry->at(i)->param)
            return SYN->curPOLAry->at(i)->content->synthesis();
   }
   return _pPair->content->synthesis();
}

bool
VlpParamNode::isConst() const
{
   if (SYN->curPOLAry != 0) // have parameter overload => need to check overload info.
      for (unsigned i = 0; i < SYN->curPOLAry->size(); ++i) 
         if (_pPair->param == SYN->curPOLAry->at(i)->param)  //overload parameter match
            return SYN->curPOLAry->at(i)->content->isConst();
      
   return _pPair->content->isConst();
}

int 
VlpParamNode::constPropagate() const
{
   if (SYN->curPOLAry != 0) // have parameter overload => need to check overload info.
      for (unsigned i = 0; i < SYN->curPOLAry->size(); ++i) {
         if (_pPair->param == SYN->curPOLAry->at(i)->param) //overload parameter match
            return SYN->curPOLAry->at(i)->content->constPropagate();
      }
   return _pPair->content->constPropagate();
}

//================================= class VlpSignalNode ==================================//
//for RHS signal synthesis
CktOutPin* 
VlpSignalNode::synthesis() const
{
   string name = BaseModule :: getName(_nameId);
   if (name.find("'", 0) != string::npos)
      return synConstPin(name);
   else {
      int funcNum = SYN->synFuncArguArray.size();
      if (funcNum > 0) { // check if the variable is defined in function scope
         vector<synFuncArgu*>* func = SYN->synFuncArguArray[funcNum-1];// support recurrence function
         for (unsigned i = 0; i < func->size(); ++i) {
            if (name == func->at(i)->name) {
               // 1. for input: input has been declared in "function", so it has a real 
               //    pointer. Store name and ptr for it.
               if (func->at(i)->ptr != 0) { //input in Func
                  if (_msb == 0 && _lsb == 0)
                     return func->at(i)->ptr;
                  else {
                     unsigned sigBusId = port2Bus();
                     if (sigBusId == func->at(i)->ptr->getBusId())
                        return func->at(i)->ptr;
                     else
                        return SYN->outPinMap.genVarOutpinFout(func->at(i)->rename, sigBusId);
                  }
               }
               // 2. for local variable: since local varible has not been declared 
               //    in "function". Just store name and rename for it. Since I'll flatten 
               //    all statements in the function, I have to give the local varible 
               //    a real name. Rename is for this purpose.
               else { // local variable in Func, use outPinMap.isExist() to get outpin.
	          // Note : Fix the issue like func_out[i] = |(i & 1) by commented out this line
		  //        Error message : Error code ### - "nz0" is not declared.
	          //        qutertl: synVar.cpp: CktOutPin* SynSharNodeMap::genVarDeclaredOutpin(const std::string&) const: Assertion `0' failed.
                  //name = func->at(i)->rename;
                  break;
               }
            }
         }
      }
      // variable search inward for-statement
      if (SYN->synForArguArray.size() > 0)
         for (unsigned i = 0; i < SYN->synForArguArray.size(); ++i) {
            if (name == SYN->synForArguArray[i].getName())
               return SYN->synForArguArray[i].syn2ConstCell();
         }

      if (curVlpModule->isMemAry(name)) {
         genMemBusId(); // gen SynMemory
         if (isConstIndex()) // const index
            return SYN->synVarList.getMemOutPin(name, getMemIndex());
         else                // variable index
            return SYN->synVarList.getMemOutPin(name, 0.5, _msb);
      }
      else
         return synSignalPin(name);
   }
}

CktOutPin*
VlpSignalNode::synConstPin(string& name) const
{
   CktOutPin* sigOutpin;
   Bv4* bvValue = new Bv4(name);
   unsigned width = bvValue->bits();;
   int busId = VLDesign.genBusId(width, 0, width-1, busDirectionDefault);
   int id = SYN->constAry.isExist(bvValue, busId);
   if (id == -1) {      
      SYN->insertCktCell(new CktConstCell(sigOutpin, bvValue, name));
      sigOutpin->setBusId(busId);
      SYN->constAry.insert(sigOutpin);
      return sigOutpin;
   }
   else {
      delete bvValue;
      return SYN->constAry[id];
   }
}

CktOutPin*
VlpSignalNode::synSignalPin(string& name) const
{  // if(!exist) only gen CktOutPin, no gen SynVar
   CktOutPin* sigOutpin = SYN->outPinMap.genVarDeclaredOutpin(name);
   assert (sigOutpin != 0);
   // 4 cases:
   //    1. if variable doesn't identify which bit it need, just return 
   //       eg. a
   //    2. if variable identify bits, eg. a[0]
   //       but reg a is declared one-bit, just return a
   //    3. variable has identifide bits, eg. a[3:0]
   //       and reg a has more than one bits.
   //       (1) if reg a has CktSplitCell, generate a CktOutPin
   //       (2) if reg a doesn't have CktSplitCell, generate a CktSplitCell and a CktOutPin
   //    4. x <= reg_value[variable_index];
   if (_msb == 0 && _lsb == 0)
      return sigOutpin;
   else { // identify msb and lsb
      const SynBus* busDeclared = VLDesign.getBus(sigOutpin->getBusId());
      if (busDeclared->getWidth() == 1)
         return sigOutpin;
      else if (!(_msb->isConst() && _lsb->isConst())) { // case 4
         if (_msb != _lsb) {
            Msg(MSG_ERR) << "Variable MSB and LSB in Signal Slicing is NOT Supported yet !!!" << endl;
            return 0;
         }
         // Note that I didn't assert variable_index should be single SignalNode Variable !!!
         return SYN->outPinMap.genVarOutPinAllFout(name, _msb->synthesis());
      }
      else {  // try to find split cell
         unsigned sigBusId = port2Bus();
         if (sigBusId == sigOutpin->getBusId())
            return sigOutpin;
         else
            return SYN->outPinMap.genVarOutpinFout(name, sigBusId);
      }
   }
}

//for moduleCell outPin synthesis
CktOutPin* 
VlpSignalNode::synModCellOutPin() const
{  // if(!exist) gen CktOutPin and SynVar
   string name = BaseModule :: getName(_nameId);
   unsigned busId;
   CktOutPin* mOutpin = new CktOutPin;
   mOutpin->setName("");

   if (curVlpModule->isMemAry(name)) {
      busId = genMemBusId(); // gen SynMomory if mem not exist
      mOutpin->setBusId(busId);
      if (isConstIndex())
         SYN->synVarList.setMemFin(name, mOutpin, const_cast<VlpSignalNode*>(this), getMemIndex());
      else
         SYN->synVarList.setMemFin(name, mOutpin, const_cast<VlpSignalNode*>(this), 0.5, _msb);
   }
   else {
      CktOutPin* varOutpin = SYN->outPinMap.genVarDeclaredOutpin(name); // gen CktOutPin
      SYN->synVarList.insertVar(name, varOutpin->getBusId());           // gen SynVar
      if (_msb == 0 && _lsb == 0)//no spicify bus => the same as the declared variable
         busId = varOutpin->getBusId();
      else
         busId = port2Bus();
      assert (busId != 0); // unassinged bus
      mOutpin->setBusId(busId);
      SYN->synVarList.setVarFin(name, mOutpin, const_cast<VlpSignalNode*>(this));
   }
   return mOutpin;
}

unsigned
VlpSignalNode::genLhsBusId() const
{
   //also generate lhs total width pin and SynVar
   string name = BaseModule :: getName(_nameId);
   int funcNum = SYN->synFuncArguArray.size();
   if (funcNum > 0) { // in a VlpFunCall
      vector<synFuncArgu*>* func = SYN->synFuncArguArray[funcNum - 1];//to support recurrence funCall
      for (unsigned i = 0; i < func->size(); ++i) {
         if (name == func->at(i)->name) {
            if (func->at(i)->ptr != 0) //only inputs have ptr !=0
               cerr << "Error code ### - in VlpFunCall, input cannot be lhs." << endl;
            else { //rename local variable.
               name = func->at(i)->rename;
               break;
            }
         }
      }
   }
   CktOutPin* outPin = SYN->outPinMap.genVarDeclaredOutpin(name);
   SYN->synVarList.insertVar(name, outPin->getBusId());//gen SynVar

   if (_msb == 0 && _lsb == 0)//no spicify bus => the same as the declared variable
      return outPin->getBusId();
   else
      return port2Bus();
}

unsigned 
VlpSignalNode::port2Bus() const
{
   unsigned busId;
      
   if (_msb != 0 && _lsb != 0) {
      int msbBit = _msb->constPropagate();
      int lsbBit = _lsb->constPropagate();
      
      if ((msbBit != -1) && (lsbBit != -1)) {
         unsigned width = abs(msbBit - lsbBit) + 1;
         if (msbBit > lsbBit)
            busId = VLDesign.genBusId(width, lsbBit, msbBit, true);
         else if (msbBit == lsbBit)
            busId = VLDesign.genBusId(1, lsbBit, msbBit, busDirectionDefault);
         else
            busId = VLDesign.genBusId(width, msbBit, lsbBit, false);
      }
      else
         busId = VLDesign.genBusId(1, 0, 0, busDirectionDefault);
      
      return busId;
   }
   else {
      assert (0);
      return 0;
   }
}

unsigned
VlpSignalNode::genMemBusId() const
{
   string varName = BaseModule :: getName(_nameId);
   VlpMemAry* vlpmem = curVlpModule->getMemAry(varName);
   SYN->synVarList.insertSynMem(vlpmem);   // gen SynMemory
   
   return vlpmem->getBusId();
}

unsigned
VlpSignalNode::getMemIndex() const
{
   assert (_msb != 0 && _lsb != 0);
   int msbIndex = _msb->constPropagate();
   int lsbIndex = _lsb->constPropagate();
   assert (msbIndex == lsbIndex);
   return msbIndex;
}

bool
VlpSignalNode::isConstIndex() const
{
   assert(_msb == _lsb);
   if (_msb->isConst()) {
      if (_lsb->isConst()) return true;
      else return false;
   }
   else return false;
}

bool
VlpSignalNode::isConst() const
{
   string name = BaseModule :: getName(_nameId);
   if (SYN->synForArguArray.size() > 0)
      for (unsigned i = 0; i < SYN->synForArguArray.size(); ++i)
         if (name == SYN->synForArguArray[i].getName())
            return true;
   if (isBitBaseStr()) {
      Bv4 bv(name);
      if (bv.hasx()) return false;
      if (bv.hasz()) return false;
      return true;
   }
   else return false;
}

int
VlpSignalNode::constPropagate() const
{
   string name = BaseModule :: getName(_nameId);
   if (SYN->synForArguArray.size() > 0)
      for (unsigned i = 0; i < SYN->synForArguArray.size(); ++i)
         if (name == SYN->synForArguArray[i].getName())
            return SYN->synForArguArray[i].getCurValue();
   if (isBitBaseStr() == false)
      whoAmI(6, 6);

   assert (isBitBaseStr());
   Bv4 bv(name);
   assert ( !(bv.hasx()) );
   assert ( !(bv.hasz()) );
   return bv.value();
}

//=============================== class VlpCaseItemNode ===============================//
const VlpCaseItemNode* 
VlpCaseItemNode::getNext_NonDefault() const
{
   if (_next == 0)
      return 0;

   assert (_next->getNodeClass() == NODECLASS_CASEITEM);
   const VlpCaseItemNode* itemPtr = static_cast<const VlpCaseItemNode*>(_next);
   const VlpBaseNode* state = 0;
   string name;

   if (itemPtr->getExp()->getNodeClass() == NODECLASS_SIGNAL) {
      state = itemPtr->getExp();
      name = BaseModule::getName((static_cast<const VlpSignalNode*>(state))->getNameId());
   }
   else
      name = "";
   if (name == "default")//ignore default state
      return itemPtr->getNext_NonDefault();
   return itemPtr;
}

bool 
VlpCaseItemNode::isDefaultItem() const
{
   string name;
   if (getExp()->getNodeClass() == NODECLASS_SIGNAL)
      name = BaseModule::getName((static_cast<const VlpSignalNode*>(_exp))->getNameId());
   else
      name = "";
   if (name == "default") // ignore default state
      return true;
   else
      return false;
}
 
CktOutPin* 
VlpCaseItemNode::synthesis() const
{
   for (const VlpBaseNode* innerPtr = _inner; innerPtr; innerPtr = innerPtr->getNext())
      innerPtr->varFlatten();

   return 0; // useless return value
}

bool
VlpCaseItemNode::isItemHasX() const
{
   const VlpBaseNode* node = getExp();
   if (node->getNodeClass() == NODECLASS_PARAMETER)
      node = static_cast<const VlpParamNode*>(node)->getParamContent();
   
   if (node->getNodeClass() == NODECLASS_SIGNAL) {
      const VlpSignalNode* sNode = static_cast<const VlpSignalNode*>(node);
      string name = BaseModule :: getName(sNode->getNameId());
      if (name.find("'", 0) != string::npos) {
         Bv4 bv = name;
         return bv.hasx();
      }
      else
         return false;
   }
   else { // VlpConcatenateNode
      assert (node->getNodeClass() == NODECLASS_OPERATOR);
      assert ((static_cast<const VlpOperatorNode*>(node))->getOperatorClass() == OPCLASS_CONCATENATE); 
      const VlpConcatenateNode* ccNode = static_cast<const VlpConcatenateNode*>(node);
      if (ccNode->isBvConcatenate()) {
         Bv4 bv = ccNode->getConcatenateBv();
         return bv.hasx();
      }
      else
         return false;
   }
}

//================================== class VlpFunCall ====================================//
/* while in VlpFunCall, I'll 
   synFuncArguArray is used in 
      1. genVarDeclaredOutpin: it means the variable is a local variable in function scope, gen a CktOutPin for it
      2. VlpSignalNode::synLhsSignal: generate CktOutPin for lhs, but if variable is input, it will be error
      3. VlpSignalNode::synthesis: generate CktOutPin, local variable or input are both ok
   
   in the following procedure, I'll generate corresponding variable to push into synFuncArguArray.
   
   1. for input: input has been declared in module, so it has a real pointer. I store name and ptr for it.
   2. for local variable: since local varible has not been declared in module. I just store name and rename for it.
      since I'll flatten all statements in the function, I have to give the local varible a real name.
      rename is for this use.    */

CktOutPin* 
VlpFunCall::synthesis() const
{
   CktOutPin* outPin;
   stringstream  wire;
   VlpPortNode* port;
   unsigned varNum = SYN->synVarList.size();

   VlpFunction* func;
   vector<synFuncArgu* >* funcArguAry = new vector<synFuncArgu* >;
   string name = BaseModule :: getName(_nameId);
   curVlpModule->getFunction(name, func);

   //1. generate input
   int j = 0;
   const VlpBaseNode* exp;
   synFuncArgu* argu;
   const IOAry* ioAry = func->getIoAry();
   for (const VlpBaseNode* arguConn = _argument; arguConn; arguConn = arguConn->getNext()) {
      // argu is the signal in the FunCall list
      // ioAry is the input list declared in "Function"
      exp  = (static_cast<const VlpConnectNode*>(arguConn))->getExp(); 
      argu = new synFuncArgu;
      argu->ptr = exp->synthesis();
      // must insert the outpin in the outPinMap for sharing
      if (argu->ptr->getName() == "") {
         wire.str("");
         wire << (SYN->wireNamePrefix) << netNumber++;
         argu->ptr->setName(wire.str());
         argu->rename = wire.str();
         SYN->outPinMap.insert(argu->rename, argu->ptr);
      }
      else { // should be in the outPinMap
         argu->rename = argu->ptr->getName();
         assert (SYN->outPinMap.getSharPin(argu->ptr->getName()) != 0);
      }
      argu->name = BaseModule :: getName(ioAry->at(j)->getNameId());
      funcArguAry->push_back(argu);      
      ++j;
   }
   //2. generate local varible
   vector<unsigned> keyList;
   const PortMap* portMap = func->getPortMap();
   portMap->dfs_key(keyList);
   for (unsigned i = 0; i < keyList.size(); ++i) {
      name = BaseModule :: getName(keyList[i]);
      func->getPort(name, port);
      if (port->getIoType() == unspecIO) {
         argu       = new synFuncArgu;
         argu->name = name;
         wire.str("");
         wire << (SYN->wireNamePrefix) << netNumber++;
         argu->rename = wire.str();
         argu->ptr    = 0;
         funcArguAry->push_back(argu);

         //insert it into outPinMap
         //outPin = new CktOutPin;
         //outPin->setName(argu->rename);
         //outPin->setBusId(port->port2Bus());
         //here I won't set outPin to argu->ptr
         //so that I can check if it is local variable in VlpSignalNode::synthesis
         //SYN->outPinMap.insert(argu->rename, outPin);
      }
   }
   //3. generate output
   name = BaseModule :: getName(_nameId);//name is output name, too.
   func->getPort(name, port);
   wire.str("");
   wire << (SYN->wireNamePrefix) << netNumber++;
   argu         = new synFuncArgu;
   argu->name   = name;
   argu->rename = wire.str();
   argu->ptr    = 0;
   funcArguAry->push_back(argu);
   // insert it into outPinMap
   outPin = new CktOutPin;
   outPin->setName(argu->rename);
   outPin->setBusId(port->port2Bus());
   SYN->outPinMap.insert(argu->rename, outPin);
   SYN->synVarList.insertVar(argu->rename, outPin->getBusId());//gen SynVar

   SYN->synFuncArguArray.push_back(funcArguAry);
   func->synthesis();
   SYN->synFuncArguArray.pop_back(); 
   delete funcArguAry;// will this delete internal pointer??, if yes, it will error   

   for (unsigned i = varNum; i < SYN->synVarList.size(); ++i)
      SYN->synVarList[i]->setFuncOrTask();
         
   return outPin;
}

#endif
