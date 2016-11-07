/****************************************************************************
  FileName     [ myUsage.h ]
  PackageName  [ ]
  Synopsis     [ Report the run time and memory usage ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_USAGE_H
#define MY_USAGE_H

#include <sys/times.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>

using namespace std;


#undef MYCLK_TCK
#define MYCLK_TCK sysconf(_SC_CLK_TCK)


class MyUsage
{
public:
   MyUsage() { reset(); }

   void reset() {
      _initMem = checkMem();
      _currentTick =  checkTick();
      _periodUsedTime = _totalUsedTime = 0.0;
   }

   void report(bool repTime, bool repMem) {
      if (repTime) {
         setTimeUsage();
         cout << "Period time used : " << setprecision(4)
              << _periodUsedTime << " seconds" << endl;
         cout << "Total time used  : " << setprecision(4)
              << _totalUsedTime << " seconds" << endl;
      }
      if (repMem) {
         setMemUsage();
         cout << "Total memory used: " << setprecision(4)
              << _currentMem << " M Bytes" << endl;
      }
   }

private:
   // for Memory usage
   double     _initMem;
   double     _currentMem;

   // for CPU time usage
   double     _currentTick;
   double     _periodUsedTime;
   double     _totalUsedTime;

   // private functions
   double checkMem() const {
      ifstream inf("/proc/self/status");
      if (!inf) {
         cerr << "Cannot get memory usage" << endl;
         return 0.0;
      }
      const size_t bufSize = 128;
      char bufStr[bufSize];
      while (!inf.eof()) {
         inf.getline(bufStr, bufSize);
         if (strncmp(bufStr, "VmSize", 6) == 0) {
            long memSizeLong = atol(strchr(bufStr, ' '));
            return (memSizeLong / 1024.0);
         }
      }
      return 0.0;
   }
   double checkTick() const {
      tms tBuffer;
      times(&tBuffer);
      return tBuffer.tms_utime;
   }
   void setMemUsage() { _currentMem = checkMem() - _initMem; }
   void setTimeUsage() {
      double thisTick = checkTick();
      _periodUsedTime = (thisTick - _currentTick) / double(MYCLK_TCK);
      _totalUsedTime += _periodUsedTime;
      _currentTick = thisTick;
   }
      
};

#endif // MY_USAGE_H
