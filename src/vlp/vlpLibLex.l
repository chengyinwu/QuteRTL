%{
/****************************************************************************
  FileName     [ vlpLibLex.l ]
  Synopsis     [ vl2cdfg: Verilog to CDFG Translator
                 Lexical Scanner for Liberty           ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include "VLGlobe.h"
#include "vlpDesign.h"
#include "lib.tab.h"
#include <string>
#include <iostream>

using namespace std;

extern LibLY_usage* LibLY;

/*Space1 is adder by louis*/

%}

Space       [\r\t\b ]
Space1      [\r\t\b\n ]
Alpha       [a-zA-Z]
AlphaU      [a-zA-Z_]
AlphaNum    [a-zA-Z0-9]
AlphaNumU   [a-zA-Z0-9_]
Digit       [0-9]
DigitU      [0-9_]
Number      {Digit}{DigitU}*

%x COMMENT
%x COMMENT_L
%x ID
%x FUNC
%x DIRE
%x LIB
%x CONT
%x PRE

%%
{Space}+              { continue; }                           
\n                    { (LibLY->lineNoLib)++; continue; }
"//"                  { BEGIN COMMENT_L; }
"/*"                  { BEGIN COMMENT; }

<COMMENT,COMMENT_L>.  { continue; }
<COMMENT>\n           { (LibLY->lineNoLib)++; continue; }            
<COMMENT_L>\n         { (LibLY->lineNoLib)++; BEGIN 0; }
<COMMENT>"*/"         { BEGIN 0; }

<CONT,DIRE,FUNC,ID>\n  { (LibLY->lineNoLib)++; continue; }            

^{Space}*"library"    { LibLY->stateLib = LIBRARY; BEGIN LIB; return LIBLIBRARY;  }


<CONT>^{Space}*"cell"{AlphaNumU}+   { continue; }
<CONT>^{Space}*"cell"   {
                           //cout << "cell" << endl; 
                           LibLY->stateLib = CELL; 
                           BEGIN ID; 
                           return LIBCELL; 
                        }

<CONT>^{Space}*"pin"{AlphaNumU}+   { continue; }
<CONT>^{Space}*"pin"    { 
                           //cout << "pin" << endl; 
                           LibLY->stateLib = PIN; 
                           BEGIN ID; 
                           return LIBPIN;   
                        }

<CONT>^{Space}*"function"{AlphaNumU}+   { continue; }
<CONT>^{Space}*"function"  { 
                              //cout << "funct" << endl; 
                              LibLY->stateLib = FUNCT; 
                              BEGIN FUNC; 
                              return LIBFUNCTION;    
                           }

<CONT>^{Space}*"direction"{AlphaNumU}+   { continue; }
<CONT>^{Space}*"direction" { 
                              //cout << "direct" << endl; 
                              BEGIN DIRE; 
                              return LIBDIRECTION;   
                           }
<CONT>{Space}+             { continue; }                           
<CONT>{AlphaU}{AlphaNumU}* { continue; }
<CONT>\\[^\n\t\b\r ]*      { continue; }
<DIRE>"input"         { return LIBINPUT;  }
<DIRE>"output"        { return LIBOUTPUT; }
<DIRE>"inout"         { return LIBINOUT;  }
<DIRE>:               { return yytext[0]; }
<DIRE>{Space}+        { continue;         }                           
<FUNC>{Space}+        { continue;         }                           
<DIRE>;               { BEGIN CONT; return yytext[0]; }
<FUNC>:               { BEGIN ID;   return yytext[0]; }
<LIB>"("              { BEGIN ID;   return yytext[0]; }

<LIB>{Space}+         { continue; }                           
<ID>{Space}+          { continue; }                           
<ID>{AlphaU}{AlphaNumU}* {
                            LibLY->libid = yytext;
                            liblval.stringPtr = &(LibLY->libid);
                            return LIBID;
                         }
<ID>\\[^\n\t\b\r ]*      {
                            LibLY->libid = yytext;
                            liblval.stringPtr = &(LibLY->libid);
                            return LIBID;
                         }

<ID>")"                  { 
                           if (LibLY->stateLib == FUNCT)
                              ;
                           else
                              BEGIN CONT; 
                           return yytext[0]; 
                         }
<ID>;                    { 
                            if (LibLY->stateLib == FUNCT)
                               LibLY->stateLib = PIN;
                            BEGIN CONT; 
                            return yytext[0]; 
                         }
<ID>.                    {  return yytext[0]; }
<CONT>"{"                { 
                            (LibLY->counter)++;
                            //cout << "stateLib = " << LibLY->stateLib << "  counter = " << LibLY->counter << endl;
                            if (LibLY->stateLib == LIBRARY && LibLY->counter ==1) 
                               return yytext[0]; 
                            else if (LibLY->stateLib == CELL && LibLY->counter == 2) 
                               return yytext[0];                                
                            else if (LibLY->stateLib == PIN && LibLY->counter == 3) 
                               return yytext[0]; 
                            else 
                               continue;
                         }
<CONT>"}"                {
                            (LibLY->counter)--;
                            if (LibLY->stateLib == LIBRARY && LibLY->counter == 0) {
                               LibLY->stateLib = UNSET;
                               return yytext[0]; 
                            }
                            else if (LibLY->stateLib == CELL && LibLY->counter == 1) {
                               LibLY->stateLib = LIBRARY;
                               return yytext[0]; 
                            }
                            else if (LibLY->stateLib == PIN && LibLY->counter == 2) {
                               LibLY->stateLib = CELL;
                               return yytext[0]; 
                            }
                            else 
                               continue;
                         } 

<CONT>.   { continue; }


{AlphaU}{AlphaNumU}*  { continue; }
\\[^\n\t\b\r ]*       { continue; }
{Number}*\.{Number}+  { continue; }
{Number}+\.{Number}*  { continue; }
{Number}              { continue; }
.                     { continue; }

%%    

int libwrap()
{
   return 1;
}

