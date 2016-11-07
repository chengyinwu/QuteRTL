/****************************************************************************
  FileName     [ vlpCmd.h ]
  PackageName  [ vlp ]
  Synopsis     [ Design Flow Commands ]
  Author       [ Cheng-Yin Wu, Hu-Hsi Yeh ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_CMD_H
#define VLP_CMD_H

#include <iostream>
#include <iomanip>
#include <fstream>

#include "cmdDesign.h"
#include "util.h"
#include "myGraphAlg.h"

using namespace std;

// ============================================================================
// Classes for VlpDesign package commands 
// ============================================================================
class ReadDesignCmd : public CmdExec
{
   public:
      ReadDesignCmd() : CmdExec(CMD_TYPE_IO) {}
      ~ReadDesignCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class SynthesisCmd : public CmdExec
{
   public:
      SynthesisCmd() : CmdExec(CMD_TYPE_DESFLOW) {}
      ~SynthesisCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class FlattenCmd : public CmdExec
{
   public:
      FlattenCmd() : CmdExec(CMD_TYPE_DESFLOW) {}
      ~FlattenCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class ReportDesignCmd : public CmdExec
{
   public:
      ReportDesignCmd() : CmdExec(CMD_TYPE_REPORT) {}
      ~ReportDesignCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class ReportCktCmd : public CmdExec
{
   public:
      ReportCktCmd() : CmdExec(CMD_TYPE_REPORT) {}
      ~ReportCktCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class ReportModuleCmd : public CmdExec
{
   public:
      ReportModuleCmd() : CmdExec(CMD_TYPE_REPORT) {}
      ~ReportModuleCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class ReportCDFGCmd : public CmdExec
{
   public:
      ReportCDFGCmd() : CmdExec(CMD_TYPE_REPORT) {}
      ~ReportCDFGCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class ReportGateCmd : public CmdExec
{
   public:
      ReportGateCmd() : CmdExec(CMD_TYPE_REPORT) {}
      ~ReportGateCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class ReportFSMCmd : public CmdExec
{
   public:
      ReportFSMCmd() : CmdExec(CMD_TYPE_REPORT) {}
      ~ReportFSMCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class WriteDesignCmd : public CmdExec
{
   public:
      WriteDesignCmd() : CmdExec(CMD_TYPE_IO) {}
      ~WriteDesignCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class WriteCktCmd : public CmdExec
{
   public:
      WriteCktCmd() : CmdExec(CMD_TYPE_IO) {}
      ~WriteCktCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class WriteCDFGCmd : public CmdExec
{
   public:
      WriteCDFGCmd() : CmdExec(CMD_TYPE_PRINT) {}
      ~WriteCDFGCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

class WriteHierCmd : public CmdExec
{
   public:
      WriteHierCmd() : CmdExec(CMD_TYPE_PRINT) {}
      ~WriteHierCmd() {}
      
      CmdExecStatus exec( const string& );
      void usage() const;
      void help() const;
      bool valid() const;
};

#endif

