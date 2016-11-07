/****************************************************************************
  FileName     [ vlpModule.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Module Node class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_MODULE_H
#define VLP_MODULE_H

#include "vlpSubElement.h"
#include "vlpCtrlNode.h"
#include "vlpOpNode.h"
#include "vlpHierTree.h"
#include "vlpDefine.h"
#include "parser.h"

#include "vlpFSM.h"
#include "vlpCU.h"
#include "myTruthT.h"
#include "myString.h"
#include "nameMap.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

class CktOutPin;
class VlpFSMSIG;

class BaseModule
{
public :
   BaseModule(string&);
   virtual ~BaseModule();
   //static finction
   static int setName(const string& name) {
      return _nameMap.insert(name);
   }
   static const string& getName(int id) {
      return _nameMap.getName(id);
   }
   static void setCDFGNode(VlpBaseNode* const bn) {
      _cdfgArr.push_back(bn);
   }
   static const VlpBaseNode* getCDFGNode(int& n) {
      return _cdfgArr[n-1];
   }
   const VlpSignalNode* getSignalNode(const string&);

   virtual const char* getModuleName() const;
   virtual void setFileName(const string&);
   virtual const string getFileName() const;
   virtual void setLineCol(unsigned short, unsigned short);
   virtual unsigned short getLineNum() const;
   virtual void setIsBlackBox();
   virtual bool isBlackBox() const;
   virtual bool isLibrary() const = 0;
   //port && IO
   virtual void setIO( VlpSignalNode* const );
   virtual const VlpSignalNode* getIoSignal(int) const;
   virtual int getIoSize() const;
   virtual void printIO(bool) const;

   virtual void setPort(const string&, VlpPortNode*);
   virtual bool getPort(const string&, VlpPortNode*&) const;
   virtual const VlpPortNode* getPortNode(unsigned int) const; // kevin add
   virtual void dumpPortMap() const;
   virtual bool lintPort(const string&) const = 0;
   //dataflow
   virtual const VlpBaseNode* getDataFlow() const;
   virtual void setDataFlow(VlpBaseNode* const);
   virtual void printFlow() const;
   //Primitive
   virtual void setPrim(VlpPrimitive* const);
   virtual void listPrim() const;
   virtual const PrimitiveAry* getPrimAry() const;
   // SYNTHESIS
   virtual void synthesis() const = 0;
   virtual void synSubInstance()const = 0;
   void buildIOCell() const;
   void synDataFlow() const;
   void connLhsRhs() const;
protected :
   static NameMap    _nameMap;//globe
   static CDFGAry    _cdfgArr;//globe
   //Synthesis data
   VlpBaseNode*      _dataFlowStart;
   VlpBaseNode*      _dataFlowEnd;
   PrimitiveAry      _primitiveAry;//need map??  ==> lintting name
   IOAry             _IOAry;
   PortMap           _portMap;//var declared in module
   //module info
   string            _moduleName;
   string            _fileName;
   unsigned          _line       : 16;
   unsigned          _col        : 15;
   unsigned          _isBlackBox : 1 ;
};

class LibModule : public BaseModule
{
public :
   LibModule(string);
   ~LibModule();

   bool lintPort(const string&) const;
   bool isLibrary() const;
   void synthesis() const;
   void synSubInstance()const;

private :

};

class VlgModule : public BaseModule
{
public :
   VlgModule(string);
   ~VlgModule();

   //set function
   void setMemAry(VlpMemAry* const);
   void setInst(VlpInstance* const);
   void setParam(const string&, VlpBaseNode* const&);              
   void setParamNOL(string&, const paramPair*&);//only name overload
   void setFunction(const string&, VlpFunction*);
   void setPOLMap(string, POLAry*);

   //get function
   bool isMemAry(const string&) const;
   VlpMemAry* getMemAry(const string&) const;
   bool getFunction(const string&, VlpFunction*&) const;
   unsigned getInstSize() const;
   VlpInstance* getInst(unsigned) const;
   VlpInstance* getInst(const string&) const;
   unsigned getParamSize() const;
   const POLMap* getPOLMap() const;
   const paramPair* getParamPair(unsigned) const;
   const ParamAry* getParamDeclaration() const;
   bool getParamOL(const string&, POLAry&) const;

   //lintting function
   bool lintPort(const string&) const;
   bool lintTF(const string&) const;
   bool lintParam(const string&, const paramPair*&) const;
   bool lintInst(const string&) const;

   //verify function
   void printCDFG(const VlpBaseNode* const) const;
   void printModInfo() const;
   void dumpInstance() const;
   bool isLibrary() const;

   //use for debug
   int getPOLSize() const;
   void printPOLT() const; 

   //utility function
   void cstTree(VlpHierTreeNode* const, const ModuleMap&) const;
   void NamePOL_Module(const ModuleMap&) const;
   
   // FSM
   vector<VlpCU*> extract();
   vector<VlpCU*> CUs() const;

   // SYNTHESIS      
   void synthesis() const;
   void synSubInstance()const;
   string genPOLInfo(POLMap::const_iterator&) const;
   string genNewModuleName(string, ParamAry*) const;
   // Constraint Generation
   void constraintGen() const;
private :
   // FSM
   void setSIGS(vector<VlpFSMSIG>&, map<MyString, VlpFSMSIG>&) const;
   vector<VlpCU*> extractCU(map<MyString, VlpFSMSIG>&) const;
   vector<VlpFSMSIG> extractSIG() const;
   vector<VlpCU*>    genExFSM(const map<MyString, VlpFSMSIG>&) const;
   vector<VlpCU*>    genExCNT(const map<MyString, VlpFSMSIG>&, const vector<MyString>&) const;
   vector<VlpCU*>    genImpFSM(const map<MyString, VlpFSMSIG>&) const;
   vector<VlpCU*>    genImpCNT(const map<MyString, VlpFSMSIG>&, const vector<MyString>&) const;
   void setSigBus() const;
   int addBddSupports(BddManager&, VlpFSMSIG&, const map<MyString, VlpFSMSIG>&) const;

   VLPCUCOND_OP class2CUOP(const VlpBaseNode*, bool) const;
   set<MyString> intersect(const set<MyString>&, const set<MyString>&) const;

   vector<VlpCU*>  _CUs;
   //Synthesis data
   MemAry            _memAry; // mem declared in module --> change to map
   ParamAry          _paramAry; // The order is need for parameter overload, so put in array
   TaskMap           _taskMap;
   FunctionMap       _functionMap;
   MapArray          _paramNOLMap; //parameter overload : name mapping only
   InstanceAry       _instanceAry;//need map??   ==> lintting name
   POLMap*           _POLMap;
   

};

#endif


