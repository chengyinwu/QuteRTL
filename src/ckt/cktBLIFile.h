/****************************************************************************
  FileName     [ cktBLIFile.h ]
  PackageName  [ ckt ]
  Synopsis     [ Define output file of BILF]
  Author       [ Hu-Hsi(Louis) Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CKT_BLIF_H
#define CKT_BLIF_H

const string space = " ";

//use for generateing BLIF output
class CktOutFile
{
public:
   void open(const char* fileName) { _cktOutFile.open(fileName); }
   void openA(const char* fileName) { _cktOutFile.open(fileName,ios::app);}
   void close() { _cktOutFile.close(); }
   void insert(string str) {_cktOutFile<< str; }

   friend CktOutFile& operator << (CktOutFile& file, const string& str)
   {
      file._cktOutFile << str;
      return file;
   }
   friend CktOutFile& operator << (CktOutFile& file, const int& str)
   {
      file._cktOutFile << str;
      return file;
   }
   friend CktOutFile& operator << (CktOutFile& file, const unsigned& str)
   {
      file._cktOutFile << str;
      return file;
   }
   friend CktOutFile& operator << (CktOutFile& file, const unsigned long& str)
   {
      file._cktOutFile << str;
      return file;
   }

   friend CktOutFile& endl(CktOutFile& file)
   {
      file._cktOutFile << endl;
      return file;
   }

private:
   ofstream _cktOutFile;
};

#endif
