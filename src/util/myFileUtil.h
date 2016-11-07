/****************************************************************************
  FileName     [ myFileUtil.h ]
  PackageName  [ util ]
  Synopsis     [ Functions for file and directory handeling. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2010 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _MY_FILE_UTIL_HEADER
#define _MY_FILE_UTIL_HEADER

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_MOD_MASK 0777

static char AbsPath[4096];

static inline const char* getExecPath() {
   char buf[PATH_MAX + 1];
   if (readlink("/proc/self/exe", buf, sizeof(buf) - 1) == -1) return NULL;
   string str = buf;
   return str.substr(0, str.rfind('/')).c_str();
}

static inline const char* getAbsPath(const char* fileName) {
   // Note that we didn't check if the fileName exists, i.e. return value of realpath()
   if (!fileName) return 0;
   realpath(fileName, AbsPath);
   char* absPath = (char*)(malloc(sizeof(char) * (strlen(AbsPath) + 1)));
   strcpy(absPath, AbsPath);
   return absPath;
}

static inline bool existDir(const char* dirName) {
   struct stat st;
   return (stat(dirName,&st) == 0);
}

static inline void deleteDir(const char* dirName) {
   // Lazy version...
   char* cmd = new char[100];
   strcpy(cmd, "rm -rf ");
   strcat(cmd, dirName);
   system(cmd);
}

static inline bool myGenDir(const char* dirName, bool replace = true) {
   if (existDir(dirName)) {
      if (!replace) return false;
      else deleteDir(dirName);
   }
   int temp = umask(0);
   if ((temp = mkdir(dirName, DIR_MOD_MASK)) != 0) return false;
   return true;
}

#endif

