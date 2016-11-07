/****************************************************************************
  FileName     [ main.cpp ]
  PackageName  [ main ]
  Synopsis     [ Main Function ]
  Author       [ Hu-Hsi Yeh, Cheng-Yin Wu, and Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef QUTERTL_MAIN_SOURCE
#define QUTERTL_MAIN_SOURCE

#include "cmdDesign.h"
#include "util.h"

// Global Members
ofstream MsgOutput::_allout;        // Set Default Standard Output
string MsgOutput::_allName;
DesignMgr quteMgr("QuteRTL");       // QuteRTL Global Manager
CmdMsgMgr Msg;                      // Message Output Manager
VlpDesign& VLDesign = quteMgr.VLDesign;
CmdParser* cmdMgr = quteMgr.cmdMgr;

// Various Commands Registration
extern bool initCommonCmd();
extern bool initVlpCmd();
extern bool initCktCmd();

static void usage() 
{
   cout << "Usage: <exec> [ -File < dofile > ]" << endl;
}

int main(int argc, char** argv)
{
   myUsage.reset();
   ifstream dof;

   // Parse Command Line Arguments
   bool cmdArgOK = false;
   if (argc == 3) {  // -file <doFile>
      if (myStrNCmp("-File", argv[1], 2) == 0) {
         if (!cmdMgr->openDofile(argv[2]))
            cerr << "Cannot Open File \"" << argv[2] << "\"!!\n";
         else cmdArgOK = true;
      }
      else {
         cerr << "Unknown Argument \"" << argv[1] << "\"!!\n"; usage();
      }
   }
   else if (argc == 2) {  // -allow_tab
      if (myStrNCmp("-allow_tab", argv[1], 10) == 0) {
         cmdMgr->setTab(1); cmdArgOK = true;
      }
      else {
         cerr << "Unknown Argument \"" << argv[1] << "\"!!\n"; usage();
      }
   }
   else if (argc != 1) usage();
   else cmdArgOK = true;

   if (!cmdArgOK) exit(-1);

   // Register Commands
   bool regCmdOK = true;
   if (!initCommonCmd()) regCmdOK = false;
   if (!initVlpCmd()) regCmdOK = false;
   if (!initCktCmd()) regCmdOK = false;
   if (!regCmdOK) exit(0);

   // Start Prompt
   CmdExecStatus status = CMD_EXEC_DONE;
   if (!cmdMgr->AllowTab()) {
      while (status != CMD_EXEC_QUIT) {  // until "quit" or command error
         status = cmdMgr->execOneCmd();
         quteMgr.updatePrompt();
         cout << endl;  // a blank line between each command
      }
   }
   else {
      while (status != CMD_EXEC_QUIT) {  // until "quit" or command error
         status = cmdMgr->execOneCmdWithTab();
         quteMgr.updatePrompt();
         cout << endl;  // a blank line between each command
      }
   }

   return 0;
}

#endif

