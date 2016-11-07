/****************************************************************************
  FileName     [ vlpHierTree.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Hierarchy Tree class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_HIER_TREE_H
#define VLP_HIER_TREE_H

#include "vlpDefine.h"
#include "cktDefine.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class VlgModule;
class VlpHierTreeNode;
class CktModule;

//typedef Array<VlpHierTreeNode*>    ChildAry;


class VlpHierTreeNode
{
public :
   VlpHierTreeNode(const string&, const BaseModule* const, const VlpInstance* const);
   VlpHierTreeNode(const string&, CktModule*, const VlpInstance* const);
   ~VlpHierTreeNode();

   inline void setParent(VlpHierTreeNode* const pNode) { _parent = pNode; }
   inline void setCktCell(CktCell* const cell)         { _cells.push_back(cell); }
   inline unsigned childSize() const                   { return _childAry.size(); }
   inline unsigned cellSize() const                    { return _cells.size(); }
   inline const BaseModule* const& getModule() const   { return _modulePtr; }
   inline CktModule* const getCktModule() const        { return _cktModule; }
   inline const VlpInstance* const& getInst() const    { return _instPtr; }
   inline VlpHierTreeNode* parent() const              { return _parent; }
   
   void setChild(VlpHierTreeNode* const);
   VlpHierTreeNode* child(unsigned) const;
   VlpHierTreeNode* child(const string&) const;
   CktCell* getCktCell(unsigned) const;

   void setInstName(const string&);
   string moduleName() const;
   inline string instName() const                      { return _instName; }

   CktModule* dfsBuildCktModule(string);
   bool dfsSetCktModule();
   void dfsFlatten(string, CktModule*&, bool);
   void dfsUpdate(CktModule*&);
   //void setOLParam(const paramPair* pm) { _OLInfoArr.push_back(pm); }
   void dfsPrintNode(int, int, bool, bool) const;
   void printPort() const;
   void writeChild(ofstream&, const unsigned&) const;
private :
   string               _instName;
   const BaseModule*    _modulePtr;
   CktModule*           _cktModule;
   const VlpInstance*   _instPtr;
   ChildAry             _childAry;
   VlpHierTreeNode*     _parent;
   CktCellAry           _cells;

   //POLAry _OLInfoArr;
};


class VlpHierTree
{
   friend class VlpHierTreeNode;
   public :
      VlpHierTree();
      ~VlpHierTree();
      inline void setRoot(VlpHierTreeNode* const root)       { _root = root; }
      inline void setCurrent(VlpHierTreeNode* const current) { _current = current; }
      inline bool setCktModule()                             { return _root->dfsSetCktModule(); }
      inline VlpHierTreeNode* getCurNode() const             { return _current; }
      inline VlpHierTreeNode* root() const                   { return _root; }
      void setCurrent(string);
      VlpHierTreeNode* getNode(VlpHierTreeNode*, const string&) const;
      void printTree(int&) const;
      void repNode(BaseModule*, int&, bool, bool) const;
      void flatten(CktModule*&) const;
      void updateFltCkt(CktModule*&);
      void clear(VlpHierTreeNode*) const;
      int TreeHeight(VlpHierTreeNode*) const;  // Level of the tree
      int TreeNodeNum(VlpHierTreeNode*) const;  // ModuleCell nums
      int ModuleNum(VlpHierTreeNode*) const;  // Total Module Num
      int CellNum(VlpHierTreeNode*) const;  // Total CellNum
      void printinst_mod(VlpHierTreeNode*) const;
      void printinfo() const;
      mutable set <CktModule*> _cktModules;
      void writeHierConstraint(string, ofstream&) const;
   private :
      VlpHierTreeNode* getNode(VlpHierTreeNode*, BaseModule*) const;
      
      VlpHierTreeNode* _root;
      VlpHierTreeNode* _current;
};

#endif



