/******************************************************************************
  FileName    [ myCout.cpp ]
  PackageName [ quteEDA/util ]
  Synopsis    [ Monitor of command line message ]
  Author      [ Chi-Wen( Kevin ) Chang ]
  Copyright   [ Copyleft(c) 2005-2007 LaDS(III), GIEE, NTU, Taiwan ]
 ******************************************************************************/

#ifndef _UTIL_MYCOUT_SOURCE
#define _UTIL_MYCOUT_SOURCE

#include <cstring>
#include "myCout.h"

MyCout::MyCout( MYCOUT_MODE mode ) {
   if ( mode == MYCOUT_MODE_FILE ) {
      _buffer = new char *[ MYCOUT_BUFFER_BOUND ];
 
      for ( int i = 0; i < MYCOUT_BUFFER_BOUND; ++i )
         _buffer[i] = new char[1024];
   }
   else { 
      _buffer = NULL;
   }

   _col  = 0;
   _row  = 0;
   _mode = mode;
}

MyCout::~MyCout() {
   if ( _row != 0 || _col != 0 )
      dump();

   if ( _buffer != NULL ) {
      for ( int i = 0; i < MYCOUT_BUFFER_BOUND; ++i )
         delete [] _buffer[ i ];
        
      delete [] _buffer;
   }
}

MyCout&
MyCout::operator = ( const MyCout &src ) {
   _col  = src._col;
   _row  = src._row;
   _mode = src._mode;
   
   for ( unsigned int i = 0; i <= _col; ++i )
      memcpy( _buffer[ i ], src._buffer[ i ], 1024 );
      
   return *this;
}

MyCout& 
operator << ( MyCout &base, const bool src ) {
   base.fromBool( src );
   return base;
}

MyCout& 
operator << ( MyCout &base, const char src ) {
   base.fromChar( src );
   return base;
}

MyCout& 
operator << ( MyCout &base, const short int src ) {
   base.fromIntS( static_cast< const long >( src ) );
   return base;
}

MyCout&
operator << ( MyCout &base, const unsigned short int src ) {
   base.fromInt( static_cast< const unsigned long >( src ) );
   return base;
}

MyCout& 
operator << ( MyCout &base, const int src ) {
   base.fromIntS( static_cast< const long >( src ) );
   return base;
}

MyCout& 
operator << ( MyCout &base, const unsigned int src ) {
   base.fromInt( static_cast< const unsigned long >( src ) );
   return base;
}

MyCout& 
operator << ( MyCout &base, const long src ) {
   base.fromIntS( static_cast< const long >( src ) );
   return base;
}

MyCout& 
operator << ( MyCout &base, const unsigned long src ) {
   base.fromInt( static_cast< const unsigned long >( src ) );
   return base;
}

MyCout& 
operator << ( MyCout &base, const float src ) {
   base.fromFloat( static_cast< const double >( src ) );
   return base;
}

MyCout& 
operator << ( MyCout &base, const double src ) {
   base.fromFloat( src );
   return base;
}

MyCout& 
operator << ( MyCout &base, const char *src ) {
   base.fromString( src );
   return base;
}

MyCout& 
operator << ( MyCout &base, const MyString &src ) {
   base.fromString( src.str() );
   return base;
}

MyCout& 
operator << ( MyCout &base, const Bv2 &src ) {
   MyString tmp = src.str();
   
   base.fromString( tmp.str() );
   return base;
}

MyCout& 
operator << ( MyCout &base, const Bv4 &src ) {
   MyString tmp = src.str();

   base.fromString( tmp.str() );
   return base;
}

void 
MyCout::setFileName( const char *src ) {
   _fileName = src;
}

const string
MyCout::fileName() const {
   return _fileName;
}
   
unsigned int 
MyCout::size() const {
   return ( _row * 1024 ) + _col;
}

void
MyCout::fromBool( const bool src ) {
   if ( src ) 
      fromString( "true" );
   else 
      fromString( "false" );
}

void 
MyCout::fromIntS( const long src ) {
   char buffer[32],
        data[32];
   int  i,
        base,
        index;
   long tmp;

   memset( data, '\0', 32 );

   if ( src == 0 ) {
      strcpy( data, "0" );
   }
   else {
      index = 0;
      tmp   = src;

      base = 0;
      if ( src < 0 ) {
         data[0] = '-';
         base    = 1;
         tmp    *= -1;;
      }

      while ( tmp != 0 ) {
         buffer[ index++ ] = '0' + tmp % 10;

         tmp /= 10;
      }

      for ( i = index - 1; i >= 0; i-- ) 
         data[ base++ ] = buffer[ i ];
      
      data[i] = '\n';
   }
   fromString( data );
}

void
MyCout::fromInt( const unsigned long src ) {
   char          buffer[32],
                 data[32];
   int           i, 
                 base,
                 index;
   unsigned long tmp;

   memset( data, '\0', 32 );

   if ( src == 0 ) {
      strcpy( data, "0" );
   }
   else {
      index = 0;
      tmp   = src;

      base = 0;

      while ( tmp != 0 ) {
         buffer[ index++ ] = '0' + tmp % 10;

         tmp /= 10;
      }

      for ( i = index - 1; i >= 0; i-- ) 
         data[ base++ ] = buffer[ i ];
      
      data[i] = '\n';
   }
   fromString( data );
}
   
void 
MyCout::fromFloat( const double src ) {
   double     limit = 10e-8,
              afterDot,
              source;
   int        beforeDot,
              len;

   MyString   tmp = "";

   source = src;
   if ( src < 0 ) {
      source *= -1.0;
      tmp     = "-";
   }

   beforeDot = (int)( source );
   afterDot  = source - (double)beforeDot;

   tmp += beforeDot;
   tmp += ".";
   len  = 0;
   if ( afterDot > limit ) {
      while ( afterDot > limit && len++ < MYCOUT_PRECISION_LEN ) {
         afterDot *= 10;
         beforeDot = (int)afterDot;
         tmp      += beforeDot;
         afterDot -= (double)beforeDot;
     }
   }
   else {
      tmp += "0";
   }

   fromString( tmp.str() );
}

void
MyCout::fromChar( const char src ) {
   if ( _mode == MYCOUT_MODE_COUT ) {
      cout << src;
   }
   else if ( _mode == MYCOUT_MODE_CERR ) {
      cerr << src;
   }
   else {
      if ( _col == 1023 && _row == MYCOUT_BUFFER_BOUND - 1 ) 
         dump();

      _buffer[ _row ][ _col++ ] = src;
   }
}
   
void 
MyCout::fromString( const char *src ) {
   int len;

   if ( _mode == MYCOUT_MODE_COUT ) {
      cout << src;
   }
   else if ( _mode == MYCOUT_MODE_CERR ) {
      cerr << src;
   }
   else {
      len = strlen(src);   
      
      for ( int i = 0; i < len; ++i ) {
         if ( _row == MYCOUT_BUFFER_BOUND - 1 && _col == 1023 )
            dump();
         
         _buffer[ _row ][ _col++ ] = src[i];

         if ( _col == 1024 ) {
            _row++;
            _col = 0;
         }
      }
   }
}

void
MyCout::dump() {
   FILE *      fp;
   char        tmpName[32];
   time_t      rawtime;
   struct tm * timeinfo;
  
   memset( tmpName, '\0', 32 );

   if ( _fileName == "" ) {
      strcpy( tmpName, "mycout_" );
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      strftime( &tmpName[7], 24, "%H%M%S.log", timeinfo );
      fp = fopen( tmpName, "a" );
   }
   else {
      fp = fopen( _fileName.c_str(), "a" );
   }

   for ( unsigned int i = 0; i <= _row; ++i ) {
      if ( _row == 0 || i == _row - 1 ) 
         fwrite( _buffer[ i ], sizeof( _buffer[i][0] ), _col, fp );
      else
         fwrite( _buffer[ i ], sizeof( _buffer[i][0] ), 1024, fp );
   }

   fclose( fp );

   _row = 0;
   _col = 0;
}
   
#endif

