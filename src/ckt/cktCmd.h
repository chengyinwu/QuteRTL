/****************************************************************************
  FileName     [ cktCmd.h ]
  PackageName  [ ckt ]
  Synopsis     [ Commands relates ckt. ]
  Author       [ Cheng-Yin Wu, Steggie Chen, Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_CMD_H
#define CKT_CMD_H

#include <iostream>
#include <iomanip>
#include <fstream>

#include "cktModule.h"
#include "cmdDesign.h"
#include "util.h"
#include "myGraphAlg.h"

using namespace std;

// class cktDesign;
// ============================================================================
// Classes for cktDesign package commands 
// ============================================================================
class CreateDesignCmd : public CmdExec
{
   public:
      CreateDesignCmd() : CmdExec(CMD_TYPE_BUILDCKT) {}
      ~CreateDesignCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class DefineNetCmd : public CmdExec
{
   public:
      DefineNetCmd() : CmdExec(CMD_TYPE_BUILDCKT) {}
      ~DefineNetCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class DefineCellCmd : public CmdExec
{
   public:
      DefineCellCmd() : CmdExec(CMD_TYPE_BUILDCKT) {}
      ~DefineCellCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class DefineInstCmd : public CmdExec
{
   public:
      DefineInstCmd() : CmdExec(CMD_TYPE_BUILDCKT) {}
      ~DefineInstCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class DefineModuleCmd : public CmdExec
{
   public:
      DefineModuleCmd() : CmdExec(CMD_TYPE_BUILDCKT) {}
      ~DefineModuleCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class ChangeModuleCmd : public CmdExec
{
   public:
      ChangeModuleCmd() : CmdExec(CMD_TYPE_BUILDCKT) {}
      ~ChangeModuleCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class WriteDfgCmd : public CmdExec
{
   public:
      WriteDfgCmd() : CmdExec(CMD_TYPE_PRINT) {}
      ~WriteDfgCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class WriteCfgCmd : public CmdExec
{
   public:
      WriteCfgCmd() : CmdExec(CMD_TYPE_PRINT) {}
      ~WriteCfgCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class WhoAmICmd : public CmdExec
{
   public:
      WhoAmICmd() : CmdExec(CMD_TYPE_REVEALED) {}
      ~WhoAmICmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

class SimulateCmd : public CmdExec
{
   public:
      SimulateCmd() : CmdExec(CMD_TYPE_REVEALED) {}
      ~SimulateCmd() {}
      
      CmdExecStatus exec(const string&);
      void usage() const;
      void help() const;
      bool valid() const;
};

#endif

