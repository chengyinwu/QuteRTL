/****************************************************************************
  FileName     [ cmdCommon.h ]
  PackageName  [ cmd ]
  Synopsis     [ Global Commands Implementation ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CMD_COMMON_H
#define CMD_COMMON_H

#include "cmdDesign.h"
#include "cmdMsgFile.h"

class HelpCmd : public CmdExec
{
   public:
      HelpCmd() : CmdExec(CMD_TYPE_COMMON) {}
      ~HelpCmd() {}
      
      CmdExecStatus exec(const string& option);
      void usage() const;
      void help() const;
};

class QuitCmd : public CmdExec
{
   public:
      QuitCmd() : CmdExec(CMD_TYPE_COMMON) {}
      ~QuitCmd() {}
      
      CmdExecStatus exec(const string& option);
      void usage() const;
      void help() const;
};

class HistoryCmd : public CmdExec
{
   public:
      HistoryCmd() : CmdExec(CMD_TYPE_COMMON) {}
      ~HistoryCmd() {}
      
      CmdExecStatus exec(const string& option);
      void usage() const;
      void help() const;
};

class DofileCmd : public CmdExec
{
   public:
      DofileCmd() : CmdExec(CMD_TYPE_COMMON) {}
      ~DofileCmd() {}
      
      CmdExecStatus exec(const string& option);
      void usage() const;
      void help() const;
};

class UsageCmd : public CmdExec
{
   public:
      UsageCmd() : CmdExec(CMD_TYPE_COMMON) {}
      ~UsageCmd() {}
      
      CmdExecStatus exec(const string& option);
      void usage() const;
      void help() const;
};

class SLogFileCmd : public CmdExec
{
   public:
      SLogFileCmd() : CmdExec(CMD_TYPE_COMMON) {}
      ~SLogFileCmd() {}
      
      CmdExecStatus exec(const string& option);
      void usage() const;
      void help() const;
};

#endif // CMD_COMMON_H
