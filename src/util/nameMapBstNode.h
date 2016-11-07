#ifndef _CKT_NAME_MAP_BST_NODE
#define _CKT_NAME_MAP_BST_NODE

#include "nameMapNode.h"

class NameMapBstNode {
   public:
      NameMapBstNode( NameMapNode * );
      ~NameMapBstNode();

      void setLeft( NameMapBstNode * );
      void setRight( NameMapBstNode * );
 
      NameMapNode * getNode();
      NameMapBstNode * getLeft();
      NameMapBstNode * getRight();

      bool isLeftNull();
      bool isRightNull();
   private:
      NameMapNode *_node;
      NameMapBstNode *_left;
      NameMapBstNode *_right;
};

#endif

