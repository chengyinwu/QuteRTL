/****************************************************************************
  FileName     [ myGraphAlg.h ]
  PackageName  [ util ]
  Synopsis     [ Data structure and algorithms to graph. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2009 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _MY_GRAPH_ALG_HEADER
#define _MY_GRAPH_ALG_HEADER

#include <string>
#include <stack>
#include <queue>
#include <map>
#include <set>

using namespace std;

template<class T>
class SimpleGraph
{
   public : 
      SimpleGraph();
      SimpleGraph(const SimpleGraph&);
      ~SimpleGraph();
      void insertEdge(const T&, const T&);
      void getLeafNodes(queue<T>&);
      void topological_sort(stack<queue<T> >&);
      void clear();
   private : 
      struct Node
      {
         Node(const T& t) : _me(t) { _pNodes.clear(); _cNodes.clear(); }
         Node(const Node& n) : _me(n._me) { n._pNodes = _pNodes; n._cNodes =_cNodes; }
         ~Node();
         T _me;
         set<Node*> _pNodes;  // Node that points to it
         set<Node*> _cNodes;  // Node that it points to
      };
      map<T, Node*>  _nodeList;
      typedef class set<Node*>::iterator nodeSetIterator;
      typedef class map<T, Node*>::iterator nodeMapIterator;
};

template<class T>
SimpleGraph<T>::SimpleGraph() {
   _nodeList.clear();
}

template<class T>
SimpleGraph<T>::SimpleGraph(const SimpleGraph& SG) {
   _nodeList = SG._nodeList;
}

template<class T>
SimpleGraph<T>::~SimpleGraph() {
}

template<class T> void
SimpleGraph<T>::insertEdge(const T& f, const T& t) {
   Node* fNode = 0;
   Node* tNode = 0;
   nodeMapIterator it;
   if ((it = _nodeList.find(f)) != _nodeList.end()) fNode = it->second;
   else _nodeList.insert(pair<T, Node*>(f, fNode = new Node(f)));
   if ((it = _nodeList.find(t)) != _nodeList.end()) tNode = it->second;
   else _nodeList.insert(pair<T, Node*>(t, tNode = new Node(t)));
   assert(fNode);
   assert(tNode);
   fNode->_cNodes.insert(tNode);
   tNode->_pNodes.insert(fNode);
}

template<class T> void
SimpleGraph<T>::getLeafNodes(queue<T>& leafList) {
   while (!leafList.empty()) leafList.pop();
   for (nodeMapIterator it = _nodeList.begin(); it != _nodeList.end(); ++it) 
      if (it->second->_cNodes.size() == 0) leafList.push(it->second->_me);
}

template<class T> void
SimpleGraph<T>::topological_sort(stack<queue<T> >& orderList) {
   SimpleGraph copyGraph(*this);
   while (!orderList.empty()) orderList.pop();
   queue<T> eqList;
   queue<Node*> endNodes;
   Node* node;

   while (true) {
      for (nodeMapIterator it = copyGraph._nodeList.begin(); it != copyGraph._nodeList.end(); ++it) 
         if (it->second->_cNodes.size() == 0)
            if (it->second->_pNodes.size() != 0) { endNodes.push(it->second); eqList.push(it->second->_me); copyGraph._nodeList.erase(it); }
      
      if (endNodes.empty()) {
         for (nodeMapIterator it = copyGraph._nodeList.begin(); it != copyGraph._nodeList.end(); ++it) 
            if (it->second->_cNodes.size() == 0) { endNodes.push(it->second); eqList.push(it->second->_me); copyGraph._nodeList.erase(it); }
      }
      if (endNodes.empty()) break;
      orderList.push(eqList);
      while (!eqList.empty()) eqList.pop();
      while (!endNodes.empty()) {
         node = endNodes.front();
         for (nodeSetIterator it = node->_pNodes.begin(); it != node->_pNodes.end(); ++it) {
            (*it)->_cNodes.erase(node);
         }
         endNodes.pop();
      }
   }
}

template<class T> void
SimpleGraph<T>::clear() {
   _nodeList.clear();
}

inline void addHTMLHeaders(const string title, const string htmlFile) {
   string cmd = "echo \"";
   cmd += ("<HEAD><TITLE>" + title + "</TITLE></HEAD><BODY BGCOLOR = white>");
   cmd += ("<H1 align=CENTER>" + title + "</H1><HR>");
   cmd += ("\" > " + htmlFile + ".html");
   system(cmd.c_str());
}

inline void dotToPng(const string filename) {
   string cmd = "";
   cmd = "dot -Tpng " + filename + " -o QuteRTL_test.png";
   system(cmd.c_str());
   cmd = "mv QuteRTL_test.png " + filename;
   system(cmd.c_str());
}

inline void dotToPs(const string filename) {
   string cmd = "";
   cmd = "dot -Tps " + filename + " -o QuteRTL_test.ps";
   system(cmd.c_str());
   cmd = "mv QuteRTL_test.ps " + filename;
   system(cmd.c_str());
}

inline void dotToCMapX(const string filename, const string dirName = "") {
   string DOTFileName = dirName + "/" + filename;
   string cmd = "";
   //cmd = "dot -Tcmapx -o" + DOTFileName + ".map -Tgif -o" + DOTFileName + ".gif " + DOTFileName;
   cmd = "dot -Tcmapx -o" + DOTFileName + ".map -Tpng -o" + DOTFileName + ".png " + DOTFileName;
   system(cmd.c_str());

   addHTMLHeaders(filename, DOTFileName);
   cmd = "echo \"";
   //cmd += ("<IMG SRC=\\\"" + filename + ".gif\\\" USEMAP=\\\"#" + filename + "_map\\\" /><BR><BR>");
   cmd += ("<IMG SRC=\\\"" + filename + ".png\\\" USEMAP=\\\"#" + filename + "_map\\\" /><BR><BR>");
   cmd += ("\" >> " + DOTFileName + ".html");
   system(cmd.c_str());
   cmd = "cat " + DOTFileName + ".map >> " + DOTFileName + ".html";
   system(cmd.c_str());

   // Remove original DOT File
   cmd = "rm -f " + DOTFileName;
   system(cmd.c_str());
   
   // Remove map File
   cmd = "rm -f " + DOTFileName + ".map";
   system(cmd.c_str());
}

#endif

