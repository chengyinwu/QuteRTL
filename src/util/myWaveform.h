/****************************************************************************
  FileName     [ myWaveform.h ]
  PackageName  [ util ]
  Synopsis     [ VCD Format Waveform File Generator. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _MY_WAVEFORM_HEADER
#define _MY_WAVEFORM_HEADER

#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include "myFileUtil.h"
#include "bv4.h"

using namespace std;

// Defines
#define	VCD_HEADER      1ul
#define	VCD_MODULE      2ul
#define	VCD_VARIABLE    4ul
#define	VCD_SIMULATE    8ul

static const string VarTypeString[] = 
{ 
   "event", "wire", "reg", "integer", "parameter", "real", 
   "supply0", "supply1", "time", "tri", "triand", "trior", 
   "trireg", "tri0", "tri1", "wand", "wire", "wor" 
};

// Enumerations
enum VCD_VAR_TYPE
{
   VCD_EVENT = 0, 
   VCD_WIRE,
   VCD_REG,
   VCD_INT,
   VCD_PARA,
   VCD_REAL,
   VCD_SPLY0,
   VCD_SPLY1,
   VCD_TIME,
   VCD_TRI,
   VCD_TRIAND,
   VCD_TRIOR,
   VCD_TRIREG,
   VCD_TRI0,
   VCD_TRI1,
   VCD_WAND,
   VCD_WOR,
   VCD_VAR_TYPE_NUM
};

/* Example of VCD_Writer Usage
   1. addModule
   2. addVariable
   3. Goto 1 if needed
   4. startWaveDump
*/

class VCD_Writer
{
   public : 
      VCD_Writer(const string& fileName, const unsigned t = 1) : _timeScale(t) { _vcdFile.open(fileName.c_str()); _curTime = 0; _curState = 0; }
      ~VCD_Writer() { if (fileExist()) _vcdFile.close(); }
      // Inline Functions
      inline bool fileExist() { return _vcdFile.is_open(); }
      // Interface Functions
      inline void addModule(const string& moduleName) {
         assert (!isModuleState()); assert (!isSimulateState());
         if (!isHeaderState()) dumpHeaders();
         if (isVariableState()) dumpEndModule();
         dumpModule(moduleName);
      }
      inline void addVariable(const VCD_VAR_TYPE type, const unsigned width, const string& varName) {
         assert (isHeaderState()); assert (isModuleState()); 
         assert (!isSimulateState()); assert (type < VCD_VAR_TYPE_NUM);
         if (!isVariableState()) setVariableState();
         _vcdFile << "$var " << VarTypeString[type] << " " << width << " " << varName << " " << varName << " $end" << endl;
      }
      inline void startWaveDump() {
         assert (isHeaderState()); assert (isModuleState());
         assert (isVariableState()); assert (!isSimulateState());
         dumpEndModule(); setSimulateState();
         _vcdFile << "$enddefinitions $end" << endl;
      }
      inline void incCycle(const unsigned c) {
         // Renders _curTime increased by _timeScale
         assert (isSimulateState());
         _curTime += (c * _timeScale);
         _vcdFile << "#" << _curTime << endl;
      }
      inline void setCurTime(const unsigned t) {
         // Renders _curTime to be set to t
         assert (isSimulateState()); assert (t >= _curTime);
         _curTime = t;
         _vcdFile << "#" << _curTime << endl;
      }
      inline void setCurValue(const string& varName, const Bv4& value) {
         assert (isSimulateState());
         _vcdFile << "b";
         unsigned i = value.bits();
         while (i--) {
            switch (value[i]) {
               case _BV4_1 : _vcdFile << "1"; break;
               case _BV4_0 : _vcdFile << "0"; break;
               case _BV4_X : _vcdFile << "x"; break;
               default     : _vcdFile << "z"; break;
            }
         }
         _vcdFile << " " << varName << endl;
      }

      private : 
      // Internal State Check Functions
      inline bool isHeaderState() const { return _curState & VCD_HEADER; }
      inline bool isModuleState() const { return _curState & VCD_MODULE; }
      inline bool isVariableState() const { return _curState & VCD_VARIABLE; }
      inline bool isSimulateState() const { return _curState & VCD_SIMULATE; }
      // Internal State Set / Reset Functions
      inline void setHeaderState() { assert (!isHeaderState()); _curState |= VCD_HEADER; assert (isHeaderState()); }
      inline void setModuleState() { assert (!isModuleState()); _curState |= VCD_MODULE; assert (isModuleState()); }
      inline void setVariableState() { assert (!isVariableState()); _curState |= VCD_VARIABLE; assert (isVariableState()); }
      inline void setSimulateState() { assert (!isSimulateState()); _curState |= VCD_SIMULATE; assert (isSimulateState()); }
      inline void resetModuleState() { assert (isModuleState()); _curState ^= VCD_MODULE; assert (!isModuleState()); }
      inline void resetVariableState() { assert (isVariableState()); _curState ^= VCD_VARIABLE; assert (!isVariableState());}
      // Internal Auto-Completion Functions
      inline void dumpHeaders() {
         // Get Current Time and Date
         time_t cur_time; time (&cur_time);
         string curTimeStr = asctime(localtime(&cur_time));
         _vcdFile << "$date " << curTimeStr.substr(0, curTimeStr.size() - 1) << " $end" << endl;
         // Get Executable Compile Time if possible
         struct stat st;
         char fileTimeStr[100] = "";
         if (!stat(getExecPath(), &st)) 
         strftime(fileTimeStr, 100, "%c", localtime(&st.st_mtime));
         //strftime(fileTimeStr, 100, "%d-%m-%Y %H:%M:%S", localtime(&st.st_mtime));
         _vcdFile << "$version QuteRTL Compiled @ " << __DATE__ << " " << __TIME__ << "\tRun @ " << fileTimeStr << " $end" << endl;
         _vcdFile << "$timescale " << _timeScale << "ps $end" << endl;
         setHeaderState();
      }
      inline void dumpModule(const string& moduleName) {
         _vcdFile << "$scope module " << moduleName << " $end" << endl;
         setModuleState();
      }
      inline void dumpEndModule() {
         _vcdFile << "$upscope $end" << endl;
         resetModuleState(); resetVariableState();
      }
      // Data Members
      ofstream          _vcdFile;
      unsigned          _curTime;
      unsigned char     _curState;
      const unsigned    _timeScale;    // unit : ps, 1ps by default
};

#endif

