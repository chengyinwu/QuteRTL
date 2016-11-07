%{
/****************************************************************************
  FileName     [ vlpLex.l ]
  Package      [ vlp ]
  Synopsis     [ vl2cdfg: Verilog to CDFG Translator 
                 Lexical Scanner for Verilog ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include "VLGlobe.h"
#include "vlpDesign.h"
#include "y.tab.h"
#include "vlpStack.h"
#include "myFileUtil.h"
#include <string>
#include <iostream>
#include <cassert>
#include <stack>

//=================pure parser====================//
//#undef YY_DECL
//#define YY_DECL int yylex(YYSTYPE* yylval)

//=================pure parser====================//

//=================pure parser====================//
//union YYSTYPE;
//YYSTYPE yylval;
//=================pure parser====================//

using namespace std;

//forward declare
extern LY_usage* LY;
char* processWhite(char* const str);
void parseInclude(const char* file);

/*Space1 is adder by louis*/
%}

Space       [\r\t\b ]
Space1      [\r\t\b\n ]
Alpha       [a-zA-Z]
AlphaU      [a-zA-Z_]
AlphaNum    [a-zA-Z0-9]
AlphaNumU   [a-zA-Z0-9_$]
Digit       [0-9]
DigitU      [0-9_]
Number      {Digit}{DigitU}*
Decimal     ({Number})?'[dD]{Space1}*{Number}
Octal       ({Number})?'[oO]{Space1}*[0-7xXzZ?][0-7xXzZ?_]*
Hexdecimal  ({Number})?'[hH]{Space1}*[0-9a-fA-FxXzZ?][0-9a-fA-FxXzZ?_]*
Binary      ({Number})?'[bB]{Space1}*[01xXzZ?][01xXzZ?_]*

%x COMMENT
%x COMMENT_L
%x INCLUDE
%x IFDEF
%x MARKED
%x MARKED_S
%x DIRECTIVE
%x DIRECTIVE_L
%x TRAN_OFF
%x TRAN_OFF_L
%x LIBRARY
%x DEFINE

%%

{Space}+          { LY->colNo = LY->colNo + strlen(yytext); continue; }
                           
\n                { 
                     (LY->lineNo)++;
                     LY->colNo = 1;
                     if (LY->defState) {
                        BEGIN 0;
                        LY->defState = false;
                        if (LY->defCount == 1) 
                           return YYDEF_NULL; 
                        else
                           continue;                              
                     }                            
                     else 
                        continue; 
                     LY->defCount = 0;
                  }
<LIBRARY>"//"        { (LY->colNo)+=2; BEGIN COMMENT_L; }
<LIBRARY>"/*"        { (LY->colNo)+=2; BEGIN COMMENT; }
"//"                 { (LY->colNo)+=2; BEGIN COMMENT_L; }
"/*"                 { (LY->colNo)+=2; BEGIN COMMENT; }

<COMMENT>synopsys    { if (LY->blackBox == false) BEGIN DIRECTIVE; }
<COMMENT_L>synopsys  { if (LY->blackBox == false) BEGIN DIRECTIVE_L; }

<COMMENT,COMMENT_L>. { (LY->colNo)++; continue; }

<COMMENT>\n          { (LY->lineNo)++; LY->colNo = 1; continue; }            
<COMMENT_L>\n        { (LY->lineNo)++; LY->colNo = 1;
                       if (LY->blackBox == true && LY->vLibComment == false && LY->modStart == true) 
                          BEGIN LIBRARY; 
                       else 
                          BEGIN 0;
                       
                     }
<COMMENT>"*/"        { (LY->colNo)+=2; 
                       if (LY->blackBox == true && LY->vLibComment == false && LY->modStart == true) 
                          BEGIN LIBRARY; 
                       else 
                          BEGIN 0; 
                     }
<DIRECTIVE>{Space}+         { (LY->colNo)++; continue; }
<DIRECTIVE>translate_off    { BEGIN TRAN_OFF; }
<DIRECTIVE>translate_on     { BEGIN COMMENT;  }
<DIRECTIVE>parallel_case    { return PARALLEL_CASE; }
<DIRECTIVE>full_case        { return FULL_CASE; }
<DIRECTIVE>"*/"             { (LY->colNo)+=2; BEGIN 0; }
<DIRECTIVE>.                { (LY->colNo)++; BEGIN COMMENT; }
<DIRECTIVE>\n               { (LY->lineNo)++; LY->colNo = 1; BEGIN COMMENT; }

<DIRECTIVE_L>{Space}+       { (LY->colNo)++; continue; }
<DIRECTIVE_L>translate_off  { BEGIN TRAN_OFF_L; }
<DIRECTIVE_L>translate_on   { BEGIN COMMENT_L;  }
<DIRECTIVE_L>parallel_case  { return PARALLEL_CASE; }
<DIRECTIVE_L>full_case      { return FULL_CASE; }
<DIRECTIVE_L>.              { (LY->colNo)++; BEGIN COMMENT_L; }
<DIRECTIVE_L>\n             { (LY->lineNo)++; LY->colNo = 1; BEGIN 0; }


<TRAN_OFF>synopsys { BEGIN DIRECTIVE; }
<TRAN_OFF>.        { (LY->colNo)++; continue; }
<TRAN_OFF>\n       { (LY->lineNo)++; LY->colNo = 1; continue; }
<TRAN_OFF>"*/"     { (LY->colNo)+=2; BEGIN MARKED_S; }

<TRAN_OFF_L>synopsys { BEGIN DIRECTIVE_L; }
<TRAN_OFF_L>.        { (LY->colNo)++; continue; }
<TRAN_OFF_L>\n       { (LY->lineNo)++; LY->colNo = 1; BEGIN MARKED_S; }

<MARKED_S>"//"       { (LY->colNo)+=2; BEGIN TRAN_OFF_L; }
<MARKED_S>"/*"       { (LY->colNo)+=2; BEGIN TRAN_OFF; }
<MARKED_S>.          { (LY->colNo)++; continue; }                    
<MARKED_S>\n         { (LY->lineNo)++; LY->colNo = 1; continue; }            

`define            { (LY->colNo)+=7; 
                     BEGIN DEFINE;
                     LY->defState = true;
                     LY->defCount = 0;
                     return YYDEFINE; 
                   
   /*
   `include{Space}*\" { BEGIN INCLUDE; colNo+= strlen(yytext); continue; }
   <INCLUDE>[^\"]*    { BEGIN 0; parseInclude(yytext); BEGIN INCLUDE; continue; }
   <INCLUDE>[\"]{Space}*\n  { cout << "================hit===============" << endl; lineNo++; colNo = 1; BEGIN 0; }
   */
                   }

<DEFINE>`{AlphaU}{AlphaNumU}* {  
                                 LY->colNo = (LY->colNo) + strlen(yytext);               
                                 LY->yyid = yytext+1; //ignore the char '`'
                                 yylval.stringPtr = &(LY->yyid);
                                 (LY->defCount)++;
                                 BEGIN 0;
                                 return YYDEF_ID;
                              }
<DEFINE>{AlphaU}{AlphaNumU}*  {
                                 LY->colNo = (LY->colNo) + strlen(yytext);               
                                 LY->yyid = yytext;
                                 yylval.stringPtr = &(LY->yyid);
                                 (LY->defCount)++;
                                 BEGIN 0;
                                 return YYID;
                              }
<DEFINE>{Space}+   { (LY->colNo)++; continue; }

`include{Space}*\" { BEGIN INCLUDE; (LY->colNo)+= strlen(yytext); continue; }
<INCLUDE>[^\"]*    { LY->yyid = yytext; continue; }
<INCLUDE>\"{Space}*\n   { (LY->lineNo)++; LY->colNo = 1; parseInclude(LY->yyid.c_str()); BEGIN 0; }

`ifdef               { BEGIN IFDEF; }
<IFDEF>{Space}+      { LY->colNo = (LY->colNo) + strlen(yytext); continue; }

<IFDEF>{AlphaU}{AlphaNumU}*  {  if (LY->defineMap.exist(yytext))
                                   BEGIN 0;
                                else
                                   BEGIN MARKED;
                             }
<MARKED>`else        { BEGIN 0; } 
<MARKED>`endif       { BEGIN 0; }       
<MARKED>.            { (LY->colNo)++; continue; }
<MARKED>\n           { (LY->lineNo)++; LY->colNo = 1; continue; }             
`else                { BEGIN MARKED; }
`endif               { BEGIN 0; }
`timescale[^\n]*     { continue; }
`celldefine          { continue; }
`endcelldefine       { continue; }

<LIBRARY>input     { if (LY->scopeState == MODULE) 
                        { BEGIN 0; LY->vLibComment = true; return YYINPUT; } }
<LIBRARY>output    { if (LY->scopeState == MODULE) 
                        { BEGIN 0; LY->vLibComment = true; return YYOUTPUT; } }
<LIBRARY>inout     { if (LY->scopeState == MODULE) 
                        { BEGIN 0; LY->vLibComment = true; return YYINOUT; } }
<LIBRARY>parameter { if (LY->scopeState == MODULE) 
                        { BEGIN 0; LY->vLibComment = true; return YYPARAMETER; } }
<LIBRARY>endmodule { BEGIN 0; LY->modStart = false; return YYENDMODULE; }
<LIBRARY>.         { continue; }
<LIBRARY>\n        { (LY->lineNo)++; LY->colNo = 1; continue; }

">="               { (LY->colNo)+=2; return YYGEQ; }
"=<"               { (LY->colNo)+=2; return YYLEQ; }
"&&"               { (LY->colNo)+=2; return YYLOGAND; }
"||"               { (LY->colNo)+=2; return YYLOGOR; }
"==="              { (LY->colNo)+=3; return YYCASEEQUALITY; }
"=="               { (LY->colNo)+=2; return YYLOGEQUALITY; }
"!=="              { (LY->colNo)+=3; return YYCASEINEQUALITY; }
"!="               { (LY->colNo)+=2; return YYLOGINEQUALITY; }
"^~"               { (LY->colNo)+=2; return YYLOGXNOR; }
"~^"               { (LY->colNo)+=2; return YYLOGXNOR; }
"~&"               { (LY->colNo)+=2; return YYLOGNAND; }
"~|"               { (LY->colNo)+=2; return YYLOGNOR; }
"<<"               { (LY->colNo)+=2; return YYLSHIFT; }
">>"               { (LY->colNo)+=2; return YYRSHIFT; }
"?:"               { cerr << "No supply conditional" << endl;/*return YYCONDITIONAL;*/ }
\"[^"]*            { cerr << "No supply string" << endl;/*return YYSTRING;*/ }

always             { (LY->colNo)+=6; return YYALWAYS; }
"*>"               { /*return YYALLPATH;*/ }
and                { (LY->colNo)+=3; return YYAND; }
assign             { (LY->colNo)+=6; return YYASSIGN; }
begin              { (LY->colNo)+=5; return YYBEGIN; }
buf                { (LY->colNo)+=3; return YYBUF; }
bufif0             { (LY->colNo)+=6; return YYBUFIF0; }
bufif1             { (LY->colNo)+=6; return YYBUFIF1; }
case               { (LY->colNo)+=4; return YYCASE; }
casex              { (LY->colNo)+=5; return YYCASEX; }
casez              { (LY->colNo)+=5; return YYCASEZ; }
cmos               { /*return YYCMOS;*/ }
deassign           { /*return YYDEASSIGN;*/ }
default            { (LY->colNo)+=7; return YYDEFAULT; }
defparam           { (LY->colNo)+=8; return YYDEFPARAM; }
disable            { /*return YYDISABLE;*/ }
edge               { (LY->colNo)+=4; return YYEDGE; }
else               { (LY->colNo)+=4; return YYELSE; }
end                { (LY->colNo)+=3; return YYEND; }
endcase            { (LY->colNo)+=7; return YYENDCASE; }
endfunction        { (LY->colNo)+=11; return YYENDFUNCTION; }
endmodule          { (LY->colNo)+=9; return YYENDMODULE; }
endprimitive       { /*return YYENDPRIMITIVE;*/ }
endspecify         { /*return YYENDSPECIFY;*/ }
endtable           { /*scan_table = 0; return YYENDTABLE;*/ }
endtask            { (LY->colNo)+=7; return YYENDTASK; }
event              { /*return YYEVENT;*/ }
for                { (LY->colNo)+=3; return YYFOR; }
forever            { /*return YYFOREVER;*/ }
fork               { /*return YYFORK;*/ }
function           { (LY->colNo)+=8; return YYFUNCTION; }
highz0             { (LY->colNo)+=6; return YYHIGHZ0; }
highz1             { (LY->colNo)+=6; return YYHIGHZ1; }
if                 { (LY->colNo)+=2; return YYIF; }
initial            { /*return YYINITIAL;*/ }
inout              { (LY->colNo)+=5; return YYINOUT; }
input              { (LY->colNo)+=5; return YYINPUT; }
integer            { (LY->colNo)+=7; return YYINTEGER; }
join               { /*return YYJOIN;*/ }
large              { /*return YYLARGE;*/ }
"=>"               { /*return YYLEADTO;*/ }
macromodule        { /*return YYMACROMODULE;*/ }
medium             { /*return YYMEDIUM;*/ }
module             { 
                     if (LY->blackBox == true) {
                        LY->vLibComment = true;
                        LY->modStart = true;
                     }
                     (LY->colNo)+=6; return YYMODULE; }
mreg               { /*return YYMREG;*/ }
"<="               { (LY->colNo)+=2; return YYNBASSIGN;  }
nand               { (LY->colNo)+=4; return YYNAND; }
negedge            { (LY->colNo)+=7; return YYNEGEDGE; }
nmos               { /*return YYNMOS;*/ }
nor                { (LY->colNo)+=3; return YYNOR; }
not                { (LY->colNo)+=3; return YYNOT; }
notif0             { /*return YYNOTIF0;*/ }
notif1             { /*return YYNOTIF1;*/ }
or                 { (LY->colNo)+=2; return YYOR; }
output             { (LY->colNo)+=6; return YYOUTPUT; }
parameter          { (LY->colNo)+=9; return YYPARAMETER; }
pmos               { /*return YYPMOS;*/ }
posedge            { (LY->colNo)+=7; return YYPOSEDGE; }
primitive          { /*return YYPRIMITIVE;*/ }
pull0              { (LY->colNo)+=5; return YYPULL0; }
pull1              { (LY->colNo)+=5; return YYPULL1; }
pulldown           { /*return YYPULLDOWN;*/ }
pullup             { /*return YYPULLUP;*/ }
rcmos              { /*return YYRCMOS;*/ }
real               { /*return YYREAL;*/ }
reg                { (LY->colNo)+=3; return YYREG; }
repeat             { /*return YYREPEAT;*/ }
"->"               { /*return YYRIGHTARROW;*/ }
rnmos              { /*return YYRNMOS;*/ }
rpmos              { /*return YYRPMOS;*/ }
rtran              { /*return YYRTRAN;*/ }
rtranif0           { /*return YYRTRANIF0;*/ }
rtranif1           { /*return YYRTRANIF1;*/ }
scalered           { /*return YYSCALARED;*/ }
small              { /*return YYSMALL;*/ }
specify            { /*return YYSPECIFY;*/ }
specparam          { /*return YYSPECPARAM;*/ }
strong0            { (LY->colNo)+=7; return YYSTRONG0; }
strong1            { (LY->colNo)+=7; return YYSTRONG1; }
supply0            { (LY->colNo)+=7; return YYSUPPLY0; }
supply1            { (LY->colNo)+=7; return YYSUPPLY1; }
swire              { (LY->colNo)+=5; return YYSWIRE; }
table              { /*scan_table = 1; return YYTABLE;*/ }
task               { (LY->colNo)+=4; return YYTASK; }
time               { /*return YYTIME;*/ }
tran               { /*return YYTRAN;*/ }
tranif0            { /*return YYTRANIF0;*/ }
tranif1            { /*return YYTRANIF1;*/ }
tri                { (LY->colNo)+=3; return YYTRI; }
tri0               { (LY->colNo)+=4; return YYTRI0; }
tri1               { (LY->colNo)+=4; return YYTRI1; }
triand             { (LY->colNo)+=6; return YYTRIAND; }
trior              { (LY->colNo)+=5; return YYTRIOR; }
vectored           { /*return YYVECTORED;*/ }
wait               { /*return YYWAIT;*/ }
wand               { (LY->colNo)+=4; return YYWAND; }
weak0              { (LY->colNo)+=5; return YYWEAK0; }
weak1              { (LY->colNo)+=5; return YYWEAK1; }
while              { /*return YYWHILE;*/ }
wire               { (LY->colNo)+=4; return YYWIRE; }
wor                { (LY->colNo)+=3; return YYWOR; }
xnor               { (LY->colNo)+=4; return YYXNOR; }
xor                { (LY->colNo)+=3; return YYXOR; }

\$setup              { cerr << "Error: no supply $setup" << endl;/*return YYsysSETUP;*/ }
\${Alpha}+           { cerr << "Error: no supply ${Alpha}+" << endl;/*return YYsysID;*/ }
`{AlphaU}{AlphaNumU}* {  
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         LY->yyid = yytext+1; //ignore the char '`'
                         yylval.stringPtr = &(LY->yyid);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYDEF_ID;
                      }
{AlphaU}{AlphaNumU}*  {
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         LY->yyid = yytext;
                         yylval.stringPtr = &(LY->yyid);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYID;
                      }
\\[^\n\t\b\r ]*       { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         LY->yyid = yytext;
                         yylval.stringPtr = &(LY->yyid);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYID;
                      }

{Number}*\.{Number}+  { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYRNUMBER; 
                      }
{Number}+\.{Number}*  { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);                                       
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYRNUMBER; 
                      }
{Number}              { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         //yylval->ival = atoi(yytext);
                         yylval.ival = atoi(yytext);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYINUMBER;  
                      }
{Binary}              { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         LY->yyid = processWhite(yytext);
                         //yylval->stringPtr = &yyid;
                         yylval.stringPtr = &(LY->yyid);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYBIT_BASE_STRING; 
                      }
{Octal}               { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         LY->yyid = processWhite(yytext);
                         //yylval->stringPtr = &yyid;
                         yylval.stringPtr = &(LY->yyid);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYBIT_BASE_STRING; 
                      }
{Decimal}             { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         LY->yyid = processWhite(yytext);
                         //yylval->stringPtr = &yyid;
                         yylval.stringPtr = &(LY->yyid);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYBIT_BASE_STRING; 
                      }
{Hexdecimal}          { 
                         (LY->colNo) = (LY->colNo) + strlen(yytext);               
                         LY->yyid = processWhite(yytext);
                         //yylval->stringPtr = &yyid;
                         yylval.stringPtr = &(LY->yyid);
                         if (LY->defState)
                            (LY->defCount)++;
                         return YYBIT_BASE_STRING; 
                      }
.                     { 
                        (LY->colNo)++;
                        if (LY->blackBox == true && yytext[0] == ';') {
                           LY->vLibComment = false;
                           BEGIN LIBRARY;
                        }
                        return yytext[0]; 
                      }


%%    

int yywrap()
{
   //cout << "file end : yywrap()" << endl;
   if (!(LY->FileNameStack.empty())) {
      if (LY->isInclude == true) {
         LY->FileNameStack.pop(); LY->isInclude = false;
			//Msg(MSG_IFO) << " > Parsing Verilog File : " << getAbsPath(LY->FileNameStack.top().c_str()) << " ..."<< endl;
      }
      else LY->FileNameStack.pop();
   }
   if (LY->FILEStack.size() != 0)
   {
      yyin = LY->FILEStack.top();
      LY->FILEStack.pop();
      LY->lineNo = LY->LNStack.top();
      LY->LNStack.pop();
      (LY->colNo) = LY->CNStack.top();
      LY->CNStack.pop();
      yy_delete_buffer(YY_CURRENT_BUFFER);
      yy_switch_to_buffer(LY->LexBufStack.top());
      LY->LexBufStack.pop();
      return 0;
/*no use STL
      yyin = LY->FILEStack.pop();
      LY->lineNo = LY->LNStack.pop();
      (LY->colNo) = LY->CNStack.pop();
      yy_delete_buffer(YY_CURRENT_BUFFER);
      yy_switch_to_buffer(LY->LexBufStack.pop());
      return 0;
*/
   }
   else
      return 1;
}

bool isWhite(char ch) 
{
   return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\b'); 
}

void eatWhite(char*& str) 
{
   while (isWhite(*str) && *(++str) != 0); 
}

void seekWhite(char*& next) 
{
   while (!isWhite(*next) && *next != 0) ++next; 
}

char* getString(char* const str)
{
   assert(!isWhite(*str));
   if (*str == 0)
      return 0;
   char* next = str;
   seekWhite(next);
   if (*next == 0)    //The end of a line
      return 0;
   *next = 0;
   ++next;
   eatWhite(next);
   if (*next == 0)
      return 0;
   return next;
}

char* processWhite(char* const str)
{
   char* returnStr = str;
   char* next = getString(returnStr);
   if (next != 0)
   {
      char* tmp = returnStr;
      returnStr = new char[strlen(tmp)+strlen(next)];
      strcpy(returnStr, tmp);
      strcat(returnStr, next);
   }
   return returnStr;
}

void parseInclude(const char* file)
{
   string fileName = file;

   LY->FILEStack.push(yyin);
   LY->LNStack.push(LY->lineNo);
   LY->CNStack.push((LY->colNo));
   LY->LexBufStack.push(YY_CURRENT_BUFFER);
   LY->FileNameStack.push(file);
   LY->isInclude = true;

   yyin = fopen(file, "r");
   LY->lineNo = 1;
   (LY->colNo) = 1;

   if (!yyin)
   {
      fprintf(stderr,"could not open %s\n",file);
      exit(1);
   }
   YY_BUFFER_STATE newBuf = yy_create_buffer(yyin, YY_BUF_SIZE);
   yy_switch_to_buffer(newBuf);
	Msg(MSG_IFO) << " > Parsing (Included) Verilog File : " << getAbsPath(fileName.c_str()) << " ..."<< endl;
}

