/****************************************************************************
  FileName     [ cmdDesign.h ]
  PackageName  [ cmd ]
  Synopsis     [ Main Design Manager ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CMD_DESIGN_H
#define CMD_DESIGN_H

#include "cmdParser.h"
#include "vlpDesign.h"

using namespace std;

const string DesingModeStr[] = { "DESIGN", "VERIFY", "DEBUG" };

enum DesignMode
{
   DESIGN_MODE = 0,
   VERIFY_MODE = 1,
   DEBUG_MODE  = 2
};

class DesignMgr
{
   public : 
      // Constructor and Destructors
      DesignMgr(const string& p) { cmdMgr = new CmdParser(p); _designMode = DESIGN_MODE; }
      ~DesignMgr() { delete cmdMgr; }
      // Managers
      VlpDesign   VLDesign;
      CmdParser*  cmdMgr;
      // Interface Functions
      inline void setDesignMode(const DesignMode m) { _designMode = m; }
      // Update Functions
      inline void updatePrompt() {
         if (!VLDesign.isDesignRead()) cmdMgr->setPrompt();
         else cmdMgr->setPrompt(DesingModeStr[_designMode] + " @ " + VLDesign.getCurModuleName());
      }
   private : 
      // Data Members
      DesignMode  _designMode;
};

#endif

