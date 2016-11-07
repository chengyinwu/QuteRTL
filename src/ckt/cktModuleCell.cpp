/****************************************************************************
  FileName     [ cktModuleCell.cpp ]
  PackageName  [ ckt ]
  Synopsis     [ Define circuit module cell data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_MODULE_CELL_SOURCE
#define CKT_MODULE_CELL_SOURCE

#include "cktModuleCell.h"
#include "cktModule.h"

#include "cktConnect.h"
#include "vlpDesign.h"
#include "vlpItoStr.h"
#include "synVar.h"

#define CONJUNCTION "_"
#define PREFIX_CHAR "k"

extern VlpDesign&       VLDesign;
extern SynOutFile       synOutFile;
extern int              instNumber;

extern CktHierIoMap hierIoMap;

using namespace std;

CktModuleCell::CktModuleCell(): CktCell( CKT_MODULE_CELL )
{
   _module     = NULL;
   _instName   = "";
   _moduleName = "";
}

CktModuleCell::~CktModuleCell() 
{
   _module = 0;
}

void
CktModuleCell::connOutPin(CktOutPin *pin) 
{
   _outPinAry.push_back(pin);
}

void        
CktModuleCell::replaceOutPin(CktOutPin* oldPin, CktOutPin* newPin)
{
   unsigned i;
   for (i = 0; i < _outPinAry.size(); ++i)
      if (_outPinAry[i] == oldPin) {
         _outPinAry[i] = newPin;
         break;
      }

   if (i == _outPinAry.size()) {
      cerr << "Error : can't find the oldPin for replacing !!" << endl;
      assert (0);
   }   
}

/*void
CktModuleCell::insertInPinMap( CktCell *cell )
{
   _inPinMap.push_back( cell );
}

void
CktModuleCell::insertOutPinMap( CktCell *cell )
{
   _outPinMap.push_back( cell );
}*/

unsigned
CktModuleCell::getOutPinSize() const
{
   return _outPinAry.size();
}

CktOutPin *
CktModuleCell::getOutPin(unsigned index) const
{
   assert( index < _outPinAry.size() );

   return _outPinAry[ index ];
}

CktOutPin * 
CktModuleCell::getOutPin() const
{
   cerr << "Error : The cell is mulit outputs cell \"CktModuleCell\". "
        << "Plase use another function \"getOutPin(int)\" " << endl;
   //assert (0);
   return 0;
}

unsigned 
CktModuleCell::getFoutSize() const
{
   cerr << "Error : the cell is mulit outputs cell \"CktSplitCell\". "
        << "Don't use the function \" getFoutSize() \" " << endl;
   assert (0);
}

CktModule *
CktModuleCell::getModulePtr() const
{
   return _module;
}

void
CktModuleCell::setModulePtr( CktModule *module )
{ 
   _module = module;
}

string
CktModuleCell::getExpr(CktOutPin* p) const
{
   unsigned i;
   for (i = 0; i < getOutPinSize(); ++i) { if (p == getOutPin(i)) break; }
   assert (i != getOutPinSize());
   CktOutPin* moduleOutPin = getModulePtr()->getIoCell(CKT_OUT, i)->getOutPin();
   return getInstName() + "(" + getModuleName() + ")." + moduleOutPin->getCell()->getExpr(moduleOutPin);
}

void
CktModuleCell::writeOutput() const
{
   CktModule* cktModule = VLDesign.getCktModule(_moduleName);
   int input(0), output(0), ioType,  begin, end;
   bool isRev;

   string ioName, name;
   CktOutPin* outpin = NULL;
   const SynBus* bus;
        
   stringstream inputStream;
   inputStream.str("");
   inputStream << _moduleName << " " << _instName << " ( ";

   for (unsigned i = 0; i < cktModule->getIoSize(); ++i) {
      ioType = cktModule->getIoType(i);
      ioName = cktModule->getIoName(i);
      if (ioType ==  CKT_IN) {
         outpin = getInPin(input)->getOutPin();
         ++input;
      }
      else { // (ioType == CKT_OUT) or (ioType == CKT_IO) {
         outpin = getOutPin(output);
         ++output;
      }
      
      if (outpin != 0) {
         name  = outpin->getName();
         assert (name != "");
         bus   = VLDesign.getBus(outpin->getBusId());
         begin = bus->getBegin();
         end   = bus->getEnd();
         isRev = bus->isInverted();

         if (!synOutFile.isExistWire(name))
            synOutFile.insertWireInst(genWireInst(outpin), name);
         inputStream << genModuleInst(ioName, outpin);
      }
      else {
         name = "";
         inputStream << genModuleInst(ioName, name, 0, 0);
      }

      if (i != (cktModule->getIoSize()-1))
         inputStream << ", ";
   }
   inputStream << " );" << endl;
   ++instNumber;
   synOutFile.insertModuleInst(inputStream.str());
}

void
CktModuleCell::setParamPair( ParamPairAry& paramAry )
{
   _paramAry = paramAry;
}

void
CktModuleCell::setOneParam( const paramPair tmp ) 
{
   _paramAry.push_back( tmp );
} 

const ParamPairAry*
CktModuleCell::getParamPair() const
{
   return &_paramAry;
}

void
CktModuleCell::setModuleName( string name )
{
   _moduleName = name;
}

string
CktModuleCell::getModuleName() const
{
   return _moduleName;
}

void
CktModuleCell::setInstName( string name )
{
   _instName = name;
}

string
CktModuleCell::getInstName() const
{
   return _instName;
}

void
CktModuleCell::nameOutPin(int& num)
{
   string name;
   CktOutPin* outPin;
   for (unsigned i = 0; i < getOutPinSize(); ++i) {
      outPin = getOutPin(i);
      if (outPin != 0) { // outPin == 0 => Ex : Buf bf (.out(), ...)
         if (outPin->getName() == "") {
            name = PREFIX_CHAR + toString(num);
            outPin->setName(name);
            setAddedCell();
            ++num;
         }
      }
   }
}

void
CktModuleCell::updateHierIn(string& prefix, OutPinMap& outPinMap, bool isTopModule) const
{
   CktInPin*  inPin;
   CktOutPin* outPin;
   CktCellType finCellType;
   bool condition1, condition2, condition3;
   string     hierName, foutName;
   IOPinConn* ioPinConnPtr;
   CktModule* curModule = VLDesign.getCktModule(_moduleName);
   unsigned num_of_io = 0;//inward

   for (unsigned i = 0; i < getInPinSize(); ++i) {
      inPin = getInPin(i);
      outPin = inPin->getOutPin();
      finCellType = outPin->getCell()->getCellType();
      assert (finCellType != CKT_PO_CELL);
      condition1 = (finCellType != CKT_MODULE_CELL) && (finCellType != CKT_PIO_CELL);    // C --> 
      condition2 = (finCellType == CKT_PI_CELL) && (isTopModule); // PI of topModule -->
      condition3 = (finCellType == CKT_MODULE_CELL); // M -->
      if (condition1 || condition2 || condition3) {        
         //1. do fin cell mapping   condition1: (C --> M)  condition2 : (PO --> M)
         if (isTopModule) hierName = outPin->getName();
         else             hierName = prefix + "_" + (outPin->getName());

         if (finCellType == CKT_SPLIT_CELL) {//revised
            int begin = VLDesign.getBus(outPin->getBusId())->getBegin();
            int end   = VLDesign.getBus(outPin->getBusId())->getEnd();
            hierName = hierName + toString(begin) + toString(end);
         }
         if (condition1 || condition2) { //condition 3 skip the step
            if (isTopModule == true || finCellType != CKT_PI_CELL) {
               assert(outPinMap.find(outPin) != outPinMap.end());//should exist

               if (hierIoMap.find(hierName) == hierIoMap.end()) {
                  ioPinConnPtr = new IOPinConn(outPinMap[outPin]);
                  hierIoMap.insert(make_pair(hierName, ioPinConnPtr));
               }
               else 
                  ioPinConnPtr = (hierIoMap.find(hierName))->second;
            }
            else { // isTopModule == false && finCellType == CKT_PI_CELL 
               if (hierIoMap.find(hierName) == hierIoMap.end()) {
                  ioPinConnPtr = new IOPinConn(0);
                  hierIoMap.insert(make_pair(hierName, ioPinConnPtr));
               }
               else 
                  ioPinConnPtr = (hierIoMap.find(hierName))->second;
            }
         }
         //2. do the cell within the moduleCell mapping   M --> M(PI) (CrossHier)
         num_of_io = 0;
         for (unsigned j = 0; j < curModule->getIoSize(); ++j) { //find the PI name within the module
            if (curModule->getIoType(j) == CKT_IN) {
               if (i == num_of_io) {
                  num_of_io = j;
                  break;
               }
               ++num_of_io;
            }
         }
         foutName = prefix + CONJUNCTION + getInstName() + "_" + (curModule->getIoName(num_of_io));
         if (hierIoMap.find(hierName) == hierIoMap.end()) {
            assert ((condition1 == false) && (condition2 == false));
            ioPinConnPtr = new IOPinConn(0);
            ioPinConnPtr->insert(foutName);
            hierIoMap.insert(make_pair(hierName, ioPinConnPtr));
         }
         else {
            ioPinConnPtr = hierIoMap.find(hierName)->second;
            ioPinConnPtr->insert(foutName);
         }
      }
   }
}

void 
CktModuleCell::updateHierOut(string& prefix, InPinMap& inPinMap, bool isTopModule) const
{
   CktInPin*  inPin;
   CktOutPin* outPin;
   string     hierName, foutName;
   IOPinConn* ioPinConnPtr;
   CktModule* curModule = VLDesign.getCktModule(_moduleName);
   CktCellType foutCellType;
   unsigned num_of_io = 0;//inward

   for (unsigned i = 0; i < getOutPinSize(); ++i) {
      outPin = getOutPin(i);
      if (outPin != NULL) { // if outPin == NULL => skip this~
         //1. do the cell within the moduleCell mapping   M(PO) --> M   (CrossHier)
         num_of_io = 0;
         for (unsigned j = 0; j < curModule->getIoSize(); ++j) { //find the PO name within the module
            if (curModule->getIoType(j) == CKT_OUT) {
               if (i == num_of_io) {
                  num_of_io = j;
                  break;
               }
               ++num_of_io;
            }
         }
         hierName = prefix + CONJUNCTION + getInstName() + "_" + (curModule->getIoName(num_of_io));
         if (isTopModule) foutName = outPin->getName();
         else             foutName = prefix + "_" + (outPin->getName());

         ioPinConnPtr = new IOPinConn(0);
         ioPinConnPtr->insert(foutName);
         assert (hierIoMap.find(hierName) == hierIoMap.end());
         hierIoMap.insert(make_pair(hierName, ioPinConnPtr));

         //2. do fout cell mapping (M --> C, M --> PO(consider if topModule?) )
         if (hierIoMap.find(foutName) == hierIoMap.end()) {
            ioPinConnPtr = new IOPinConn(0);
            hierIoMap.insert(make_pair(foutName, ioPinConnPtr));
         }
         else
            ioPinConnPtr = hierIoMap[foutName];

         for (unsigned j = 0; j < outPin->getInPinSize(); ++j) {
            inPin = outPin->getInPin(j);
            foutCellType = inPin->getCell()->getCellType();

            if (foutCellType == CKT_MODULE_CELL)
               ;
            else if (foutCellType == CKT_PO_CELL) { // M --> PO (consider if topModule)
               if (isTopModule) {
                  assert (inPinMap.find(inPin) != inPinMap.end());//should exist
                  assert (hierIoMap.find(foutName) != hierIoMap.end());
                  ioPinConnPtr->insert(inPinMap[inPin]);
                  //cout << "address = " << inPinMap[inPin] << endl;
               }
               else // The two names maybe will be different!!
                  ;// assert (outPin->getName() == inPin->getCell()->getOutPin()->getName());
            }
            else if (foutCellType == CKT_PI_CELL || foutCellType == CKT_PIO_CELL)
               cerr << "Error : CKT_PI_CELL or CKT_PIO_CELL @CktModuleCell::updateHierConn(...)" << endl;
            else {
               //           |---- b         maybe c doesn't exist in cellList, c is redundant.
               //    a -----|               trace PI -> PO will have problem. but
               //           |---- c         trace PO -> PI will be Ok.
               if (inPinMap.find(inPin) != inPinMap.end())//check cell if redundant
                  ioPinConnPtr->insert(inPinMap[inPin]);
            }
         }
      }
   }
}

void 
CktModuleCell::updateHierConn(string& prefix, InPinMap& inPinMap, OutPinMap& outPinMap, bool isTopModule) const
{
//Hierarchical Connection between inPin of ModuleCell//
   updateHierIn(prefix, outPinMap, isTopModule);
//Hierarchical Connection between outPin of ModuleCell//
   updateHierOut(prefix, inPinMap, isTopModule);
}

void
CktModuleCell::simulate()
{
}

/*
void 
CktModuleCell::writeBLIF() const
{

}*/

#endif

