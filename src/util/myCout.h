/******************************************************************************
  FileName    [ myCout.h ]
  PackageName [ quteEDA/util ]
  Synopsis    [ Monitor of command line message ]
  Author      [ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2005-2007 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/
#ifndef _UTIL_MYCOUT_HEADER
#define _UTIL_MYCOUT_HEADER

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "myString.h"
#include "bv2.h"
#include "bv4.h"

using namespace std;

enum MYCOUT_MODE {
   MYCOUT_MODE_COUT,
   MYCOUT_MODE_CERR,
   MYCOUT_MODE_FILE
};

#define MYCOUT_PRECISION_LEN  10
#define MYCOUT_BUFFER_BOUND   32  // KB

class MyCout {
public:
   MyCout( MYCOUT_MODE = MYCOUT_MODE_COUT );

   ~MyCout();
   
   MyCout& operator = ( const MyCout & );
   
   friend MyCout& operator << ( MyCout &, const bool );
   friend MyCout& operator << ( MyCout &, const char );
   friend MyCout& operator << ( MyCout &, const short int );
   friend MyCout& operator << ( MyCout &, const unsigned short int );
   friend MyCout& operator << ( MyCout &, const int );
   friend MyCout& operator << ( MyCout &, const unsigned int );
   friend MyCout& operator << ( MyCout &, const long );
   friend MyCout& operator << ( MyCout &, const unsigned long );
   friend MyCout& operator << ( MyCout &, const float );
   friend MyCout& operator << ( MyCout &, const double );
   friend MyCout& operator << ( MyCout &, const char * );
   friend MyCout& operator << ( MyCout &, const MyString & );
   friend MyCout& operator << ( MyCout &, const Bv2 & );
   friend MyCout& operator << ( MyCout &, const Bv4 & );

   void setFileName( const char * );
   const string fileName() const;
   
   unsigned int size() const;
private:
   char **     _buffer;
   unsigned    _col,
               _row;
   string      _fileName;
   MYCOUT_MODE _mode;
   
   void fromBool( const bool );
   void fromIntS( const long );
   void fromInt( const unsigned long );
   void fromFloat( const double );
   void fromChar( const char );
   void fromString( const char * );

   void dump();
};

#endif

