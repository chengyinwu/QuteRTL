/****************************************************************************
  FileName     [ bddManager.h ]
  PackageName  [ ]
  Synopsis     [ Define BDD Manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _BDD_MANAGER_HEADER
#define _BDD_MANAGER_HEADER


#include <map>
#include <string>
#include "bddHash.h"
#include "bddNode.h"
#include "bv4.h"
#include "myString.h"

class BddNode;

enum BDD_RELATION {
   BDD_NEQ,
   BDD_IEQ,
   BDD_EQ
};


class BddManager
{
//friend BddNode BddNode::posCoFactor(const BddNode& v) const;

public:
   BddManager() {}
   BddManager(size_t nin, size_t h, size_t c) { init(nin, h, c); }
   ~BddManager() { reset(); }

   void init(size_t nin, size_t h, size_t c);
   void reset();

   void setSupports(int, const string&);
   const BddNode& getSupport(size_t i) const;

   BddNode ite(BddNode f, BddNode g, BddNode h);
   BddNode compose(const BddNode&, unsigned int, const BddNode&);
   void addNodeMap( const string, BddNode );
   const BddNode& getNode( const string ) const;
   BddNodeInt* uniquify(size_t l, size_t r, unsigned i);

   bool bddGateBuild(istream&);
   bool coFactor(istream&);
   bool bddReport(const string&) const;   
   BDD_RELATION compareBdd(string&, string&) const;
   void drawBdd(const string, const string) const;

   void insertCube(string&, Bv4);
   bool isFullCube(string&) const;
   void initCube(string&, int&);

   //Command line Parser
   enum ErrorType
   {
      SYNTAX_ERROR = 0,
      MISSING_ARG  = 1,
      MISSING_BDD_NODE = 2,
      MISSING_GATE = 3
   };

   bool errorMsg(ErrorType e, string msg) const;
   size_t supportsNum() { return (_supports.size() - 1 ); }

private:
   // level = 0: const 1; level = 1 ~ nin: input variables
   vector<BddNode>        _supports;
   BddHash                _uniqueTable;
   BddCache               _computedTable;

   map<string, BddNode>   _bddMap;

   bool checkIteTerminal(const BddNode&, const BddNode&, const BddNode&,
                         BddNode&);
   bool standardize(BddNode& f, BddNode& g, BddNode& h);
   /*void Msg(MsgType mt);*/
};

#endif
