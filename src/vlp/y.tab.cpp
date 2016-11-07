/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     YYID = 258,
     YYDEF_ID = 259,
     YYINUMBER = 260,
     YYRNUMBER = 261,
     YYBIT_BASE_STRING = 262,
     YYALWAYS = 263,
     YYAND = 264,
     YYASSIGN = 265,
     YYBEGIN = 266,
     YYBUF = 267,
     YYBUFIF0 = 268,
     YYBUFIF1 = 269,
     YYCASE = 270,
     YYCASEX = 271,
     YYCASEZ = 272,
     YYDEFAULT = 273,
     YYDEFPARAM = 274,
     YYELSE = 275,
     YYEDGE = 276,
     YYEND = 277,
     YYENDCASE = 278,
     YYENDMODULE = 279,
     YYENDFUNCTION = 280,
     YYENDTASK = 281,
     YYFOR = 282,
     YYFUNCTION = 283,
     YYGEQ = 284,
     YYHIGHZ0 = 285,
     YYHIGHZ1 = 286,
     YYIF = 287,
     YYINOUT = 288,
     YYINPUT = 289,
     YYINTEGER = 290,
     YYLEQ = 291,
     YYLOGAND = 292,
     YYCASEEQUALITY = 293,
     YYCASEINEQUALITY = 294,
     YYLOGNAND = 295,
     YYLOGNOR = 296,
     YYLOGOR = 297,
     YYLOGXNOR = 298,
     YYLOGEQUALITY = 299,
     YYLOGINEQUALITY = 300,
     YYLSHIFT = 301,
     YYMODULE = 302,
     YYNAND = 303,
     YYNBASSIGN = 304,
     YYNEGEDGE = 305,
     YYNOR = 306,
     YYNOT = 307,
     YYOR = 308,
     YYOUTPUT = 309,
     YYPARAMETER = 310,
     YYPOSEDGE = 311,
     YYPULL0 = 312,
     YYPULL1 = 313,
     YYREG = 314,
     YYRSHIFT = 315,
     YYSTRONG0 = 316,
     YYSTRONG1 = 317,
     YYSUPPLY0 = 318,
     YYSUPPLY1 = 319,
     YYSWIRE = 320,
     YYTASK = 321,
     YYTRI = 322,
     YYTRI0 = 323,
     YYTRI1 = 324,
     YYTRIAND = 325,
     YYTRIOR = 326,
     YYWAND = 327,
     YYWEAK0 = 328,
     YYWEAK1 = 329,
     YYWIRE = 330,
     YYWOR = 331,
     YYXNOR = 332,
     YYXOR = 333,
     YYDEFINE = 334,
     YYDEF_NULL = 335,
     PARALLEL_CASE = 336,
     FULL_CASE = 337,
     YYUNARYOPERATOR = 338
   };
#endif
/* Tokens.  */
#define YYID 258
#define YYDEF_ID 259
#define YYINUMBER 260
#define YYRNUMBER 261
#define YYBIT_BASE_STRING 262
#define YYALWAYS 263
#define YYAND 264
#define YYASSIGN 265
#define YYBEGIN 266
#define YYBUF 267
#define YYBUFIF0 268
#define YYBUFIF1 269
#define YYCASE 270
#define YYCASEX 271
#define YYCASEZ 272
#define YYDEFAULT 273
#define YYDEFPARAM 274
#define YYELSE 275
#define YYEDGE 276
#define YYEND 277
#define YYENDCASE 278
#define YYENDMODULE 279
#define YYENDFUNCTION 280
#define YYENDTASK 281
#define YYFOR 282
#define YYFUNCTION 283
#define YYGEQ 284
#define YYHIGHZ0 285
#define YYHIGHZ1 286
#define YYIF 287
#define YYINOUT 288
#define YYINPUT 289
#define YYINTEGER 290
#define YYLEQ 291
#define YYLOGAND 292
#define YYCASEEQUALITY 293
#define YYCASEINEQUALITY 294
#define YYLOGNAND 295
#define YYLOGNOR 296
#define YYLOGOR 297
#define YYLOGXNOR 298
#define YYLOGEQUALITY 299
#define YYLOGINEQUALITY 300
#define YYLSHIFT 301
#define YYMODULE 302
#define YYNAND 303
#define YYNBASSIGN 304
#define YYNEGEDGE 305
#define YYNOR 306
#define YYNOT 307
#define YYOR 308
#define YYOUTPUT 309
#define YYPARAMETER 310
#define YYPOSEDGE 311
#define YYPULL0 312
#define YYPULL1 313
#define YYREG 314
#define YYRSHIFT 315
#define YYSTRONG0 316
#define YYSTRONG1 317
#define YYSUPPLY0 318
#define YYSUPPLY1 319
#define YYSWIRE 320
#define YYTASK 321
#define YYTRI 322
#define YYTRI0 323
#define YYTRI1 324
#define YYTRIAND 325
#define YYTRIOR 326
#define YYWAND 327
#define YYWEAK0 328
#define YYWEAK1 329
#define YYWIRE 330
#define YYWOR 331
#define YYXNOR 332
#define YYXOR 333
#define YYDEFINE 334
#define YYDEF_NULL 335
#define PARALLEL_CASE 336
#define FULL_CASE 337
#define YYUNARYOPERATOR 338




/* Copy the first part of user declarations.  */
#line 2 "vlpYacc.y"

/****************************************************************************
  FileName     [ vlpYacc.y ]
  Package      [ vlp ]
  Synopsis     [ Verilog parser ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include "VLGlobe.h"
#include "vlpModule.h"
#include "vlpCtrlNode.h"
#include "vlpSubElement.h"
#include "vlpDesign.h"
#include "vlpItoStr.h"
#include "vlpStack.h"
#include "vlpDefine.h"
#include <string>
#include <stack>
#include <stdlib.h>
#include <iostream>
#include <set>

using namespace std;

//#define YYPARSE_PARAM param
//#define YYLEX_PARAM &yylval
//forward declare

//union YYSTYPE;
//extern int yylex(YYSTYPE*, void*);

//extern int yylex(YYSTYPE*);

extern int yylex();

void YYTRACE(string);
extern void yyerror(char*);
// External Declarations
extern LY_usage* LY;
extern VlpDesign& VLDesign;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 47 "vlpYacc.y"
{
   int             ival;
   string*         stringPtr;
   VlpDesign*      designPtr;
   VlgModule*      modulePtr;
   VlpIONode*      vlpIONodePtr;
   VlpInstance*    instancePtr;
   VlpBaseNode*    basePtr;
   VlpParamNode*   paramPtr;
   VlpAssignNode*  assignPtr;
   VlpAlwaysNode*  alwaysPtr;
   VlpPrimitive*   primitivePtr;
   VlpFunction*    functionPtr;
}
/* Line 193 of yacc.c.  */
#line 321 "vlpYacc.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 334 "vlpYacc.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2088

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  119
/* YYNRULES -- Number of rules.  */
#define YYNRULES  296
/* YYNRULES -- Number of states.  */
#define YYNSTATES  515

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   338

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    94,     2,   102,     2,     2,    87,     2,
      97,    98,    92,    90,    99,    91,   103,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    84,    96,
      88,   104,    89,    83,   107,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   100,     2,   101,    86,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   105,    85,   106,    93,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    95
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    12,    13,    19,
      21,    25,    27,    28,    36,    37,    41,    45,    47,    51,
      53,    54,    56,    58,    59,    63,    64,    68,    74,    76,
      80,    84,    88,    92,    93,    96,    98,   100,   102,   104,
     106,   108,   110,   112,   114,   116,   118,   120,   122,   124,
     126,   127,   135,   136,   145,   146,   148,   150,   152,   153,
     156,   158,   161,   163,   165,   167,   169,   171,   173,   174,
     180,   181,   187,   188,   194,   195,   201,   207,   213,   215,
     217,   219,   221,   223,   225,   227,   229,   231,   233,   235,
     236,   238,   240,   241,   247,   248,   253,   254,   259,   260,
     265,   267,   271,   273,   277,   279,   286,   291,   293,   294,
     296,   302,   308,   310,   312,   314,   316,   318,   320,   322,
     324,   326,   328,   329,   331,   337,   341,   343,   347,   351,
     353,   355,   357,   359,   361,   363,   365,   367,   369,   371,
     373,   377,   381,   386,   388,   390,   394,   396,   401,   403,
     405,   406,   409,   411,   414,   417,   420,   425,   427,   431,
     435,   436,   442,   444,   446,   448,   452,   454,   458,   459,
     461,   462,   469,   470,   472,   475,   476,   478,   479,   482,
     484,   487,   493,   501,   509,   517,   525,   535,   538,   544,
     550,   552,   554,   558,   559,   561,   563,   566,   569,   573,
     577,   579,   582,   586,   590,   593,   597,   604,   606,   607,
     610,   612,   614,   615,   619,   620,   627,   629,   630,   634,
     636,   639,   640,   642,   644,   648,   650,   652,   656,   658,
     661,   664,   667,   670,   673,   676,   679,   682,   685,   688,
     692,   696,   700,   704,   708,   712,   716,   720,   724,   728,
     732,   736,   740,   744,   748,   752,   756,   760,   764,   768,
     774,   776,   778,   780,   781,   785,   787,   789,   791,   793,
     797,   800,   804,   810,   814,   821,   822,   828,   830,   831,
     836,   839,   842,   845,   850,   855,   860,   863,   868,   870,
     873,   876,   879,   883,   887,   889,   891
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     109,     0,    -1,    -1,   109,   110,    -1,   115,    -1,   111,
      -1,    -1,    -1,    79,   112,     3,   113,   114,    -1,   212,
      -1,   212,    84,   212,    -1,    80,    -1,    -1,    47,     3,
     116,   117,    96,   127,    24,    -1,    -1,    97,   118,    98,
      -1,    97,   125,    98,    -1,   119,    -1,   118,    99,   119,
      -1,   120,    -1,    -1,   121,    -1,   122,    -1,    -1,     3,
     123,   124,    -1,    -1,   100,   212,   101,    -1,   100,   212,
      84,   212,   101,    -1,   126,    -1,   125,    99,   126,    -1,
      34,   166,   220,    -1,    54,   166,   220,    -1,    33,   166,
     220,    -1,    -1,   127,   128,    -1,   138,    -1,   140,    -1,
     142,    -1,   144,    -1,   146,    -1,   150,    -1,   152,    -1,
     169,    -1,   176,    -1,   156,    -1,   154,    -1,   191,    -1,
     129,    -1,   131,    -1,   111,    -1,    -1,    66,     3,   130,
      96,   135,   192,    26,    -1,    -1,    28,   133,     3,   132,
      96,   136,   192,    25,    -1,    -1,   134,    -1,   167,    -1,
      35,    -1,    -1,   135,   137,    -1,   137,    -1,   136,   137,
      -1,   138,    -1,   140,    -1,   142,    -1,   144,    -1,   150,
      -1,   152,    -1,    -1,    55,   166,   139,   168,    96,    -1,
      -1,    34,   166,   141,   158,    96,    -1,    -1,    54,   166,
     143,   158,    96,    -1,    -1,    33,   166,   145,   158,    96,
      -1,   147,   162,   148,   168,    96,    -1,   147,   162,   148,
     158,    96,    -1,    65,    -1,    75,    -1,    67,    -1,    69,
      -1,    63,    -1,    72,    -1,    70,    -1,    68,    -1,    64,
      -1,    76,    -1,    71,    -1,    -1,   149,    -1,   167,    -1,
      -1,    59,   166,   151,   159,    96,    -1,    -1,    35,   153,
     159,    96,    -1,    -1,    10,   155,   168,    96,    -1,    -1,
      19,   157,   168,    96,    -1,   220,    -1,   158,    99,   220,
      -1,   160,    -1,   159,    99,   160,    -1,   161,    -1,   161,
     100,   212,    84,   212,   101,    -1,   161,   100,     4,   101,
      -1,     3,    -1,    -1,   163,    -1,    97,   164,    99,   165,
      98,    -1,    97,   165,    99,   164,    98,    -1,    63,    -1,
      61,    -1,    57,    -1,    73,    -1,    30,    -1,    64,    -1,
      62,    -1,    58,    -1,    74,    -1,    31,    -1,    -1,   167,
      -1,   100,   212,    84,   212,   101,    -1,   100,     4,   101,
      -1,   196,    -1,   168,    99,   196,    -1,   170,   171,    96,
      -1,     9,    -1,    48,    -1,    53,    -1,    51,    -1,    78,
      -1,    77,    -1,    12,    -1,    13,    -1,    14,    -1,    52,
      -1,   172,    -1,   171,    99,   172,    -1,    97,   174,    98,
      -1,   173,    97,   174,    98,    -1,     3,    -1,   175,    -1,
     174,    99,   175,    -1,   212,    -1,   177,   178,   181,    96,
      -1,     3,    -1,     4,    -1,    -1,   178,   179,    -1,   180,
      -1,   102,     5,    -1,   102,     6,    -1,   102,   220,    -1,
     102,    97,   209,    98,    -1,   182,    -1,   181,    99,   182,
      -1,    97,   184,    98,    -1,    -1,   220,   183,    97,   184,
      98,    -1,   185,    -1,   186,    -1,   187,    -1,   185,    99,
     187,    -1,   188,    -1,   186,    99,   188,    -1,    -1,   212,
      -1,    -1,   103,   220,   189,    97,   190,    98,    -1,    -1,
     212,    -1,     8,   194,    -1,    -1,   194,    -1,    -1,   193,
     194,    -1,    96,    -1,   196,    96,    -1,    32,    97,   212,
      98,   194,    -1,    32,    97,   212,    98,   194,    20,   194,
      -1,    15,    97,   212,    98,   195,   197,    23,    -1,    17,
      97,   212,    98,   195,   197,    23,    -1,    16,    97,   212,
      98,   195,   197,    23,    -1,    27,    97,   196,    96,   212,
      96,   196,    98,   194,    -1,   223,   194,    -1,   206,   104,
     222,   212,    96,    -1,   206,    49,   222,   212,    96,    -1,
     199,    -1,   203,    -1,    10,   196,    96,    -1,    -1,    82,
      -1,    81,    -1,    82,    81,    -1,    81,    82,    -1,   206,
     104,   212,    -1,   206,    49,   212,    -1,   198,    -1,   197,
     198,    -1,   211,    84,   194,    -1,    18,    84,   194,    -1,
      18,   194,    -1,    11,   193,    22,    -1,    11,    84,   200,
     201,   193,    22,    -1,     3,    -1,    -1,   201,   202,    -1,
     138,    -1,   150,    -1,    -1,     3,   204,    96,    -1,    -1,
       3,   205,    97,   211,    98,    96,    -1,   220,    -1,    -1,
     220,   207,   215,    -1,   216,    -1,     4,   208,    -1,    -1,
     215,    -1,   210,    -1,   209,    99,   210,    -1,   212,    -1,
     212,    -1,   211,    99,   212,    -1,   213,    -1,    90,   213,
      -1,    91,   213,    -1,    94,   213,    -1,    93,   213,    -1,
      87,   213,    -1,    85,   213,    -1,    86,   213,    -1,    40,
     213,    -1,    41,   213,    -1,    43,   213,    -1,   212,    90,
     212,    -1,   212,    91,   212,    -1,   212,    92,   212,    -1,
     212,    44,   212,    -1,   212,    45,   212,    -1,   212,    38,
     212,    -1,   212,    39,   212,    -1,   212,    37,   212,    -1,
     212,    42,   212,    -1,   212,    88,   212,    -1,   212,    89,
     212,    -1,   212,    87,   212,    -1,   212,    85,   212,    -1,
     212,    86,   212,    -1,   212,    36,   212,    -1,   212,    49,
     212,    -1,   212,    29,   212,    -1,   212,    46,   212,    -1,
     212,    60,   212,    -1,   212,    43,   212,    -1,   212,    83,
     212,    84,   212,    -1,     5,    -1,     6,    -1,   220,    -1,
      -1,   220,   214,   215,    -1,   216,    -1,   217,    -1,   218,
      -1,     7,    -1,    97,   212,    98,    -1,     4,   208,    -1,
     100,   212,   101,    -1,   100,   212,    84,   212,   101,    -1,
     105,   211,   106,    -1,   105,   212,   105,   211,   106,   106,
      -1,    -1,   220,    97,   219,   211,    98,    -1,     3,    -1,
      -1,   220,   103,   221,     3,    -1,   102,     5,    -1,   102,
       6,    -1,   102,   220,    -1,   102,    97,   210,    98,    -1,
     107,    97,   224,    98,    -1,   107,    97,   225,    98,    -1,
     107,    92,    -1,   107,    97,    92,    98,    -1,   212,    -1,
      56,   212,    -1,    50,   212,    -1,    21,   212,    -1,   224,
     226,   224,    -1,   225,   226,   224,    -1,    53,    -1,    42,
      -1,    99,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   312,   312,   315,   324,   330,   343,   347,   343,   361,
     366,   372,   381,   380,   403,   406,   410,   417,   421,   428,
     443,   448,   455,   479,   478,   492,   495,   507,   515,   518,
     524,   535,   546,   561,   564,   571,   575,   579,   583,   587,
     593,   601,   614,   618,   622,   626,   639,   645,   649,   653,
     902,   901,   923,   922,   957,   962,   969,   974,   989,   992,
     999,  1003,  1010,  1014,  1018,  1028,  1038,  1047,  1064,  1064,
    1075,  1074,  1091,  1090,  1107,  1106,  1122,  1134,  1153,  1159,
    1165,  1171,  1177,  1183,  1189,  1195,  1201,  1207,  1213,  1227,
    1230,  1237,  1253,  1252,  1279,  1278,  1306,  1306,  1316,  1316,
    1324,  1390,  1456,  1460,  1467,  1543,  1573,  1620,  1674,  1679,
    1686,  1692,  1701,  1706,  1711,  1716,  1721,  1729,  1734,  1739,
    1744,  1749,  1758,  1763,  1770,  1776,  1798,  1807,  1820,  1853,
    1858,  1863,  1868,  1873,  1878,  1883,  1888,  1893,  1898,  1970,
    1976,  1985,  1990,  1999,  2007,  2013,  2022,  2032,  2042,  2047,
    2069,  2072,  2087,  2094,  2099,  2104,  2109,  2117,  2128,  2145,
    2150,  2150,  2160,  2166,  2175,  2181,  2189,  2197,  2209,  2213,
    2221,  2221,  2230,  2234,  2251,  2267,  2271,  2280,  2284,  2305,
    2310,  2315,  2324,  2333,  2357,  2381,  2417,  2427,  2463,  2474,
    2493,  2502,  2522,  2535,  2536,  2537,  2538,  2539,  2543,  2588,
    2606,  2615,  2637,  2658,  2667,  2678,  2690,  2716,  2724,  2727,
    2735,  2739,  2764,  2763,  2773,  2772,  2956,  2988,  2988,  3026,
    3040,  3065,  3066,  3071,  3076,  3084,  3096,  3108,  3131,  3136,
    3142,  3148,  3156,  3162,  3168,  3174,  3180,  3186,  3192,  3198,
    3204,  3210,  3224,  3234,  3244,  3254,  3264,  3274,  3284,  3294,
    3304,  3310,  3316,  3322,  3332,  3339,  3349,  3355,  3361,  3371,
    3384,  3390,  3395,  3441,  3441,  3562,  3567,  3572,  3577,  3583,
    3588,  3614,  3618,  3635,  3652,  3670,  3670,  3700,  3705,  3705,
    3754,  3758,  3762,  3766,  3778,  3783,  3789,  3801,  3815,  3821,
    3827,  3833,  3842,  3849,  3858,  3859,  3860
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "YYID", "YYDEF_ID", "YYINUMBER",
  "YYRNUMBER", "YYBIT_BASE_STRING", "YYALWAYS", "YYAND", "YYASSIGN",
  "YYBEGIN", "YYBUF", "YYBUFIF0", "YYBUFIF1", "YYCASE", "YYCASEX",
  "YYCASEZ", "YYDEFAULT", "YYDEFPARAM", "YYELSE", "YYEDGE", "YYEND",
  "YYENDCASE", "YYENDMODULE", "YYENDFUNCTION", "YYENDTASK", "YYFOR",
  "YYFUNCTION", "YYGEQ", "YYHIGHZ0", "YYHIGHZ1", "YYIF", "YYINOUT",
  "YYINPUT", "YYINTEGER", "YYLEQ", "YYLOGAND", "YYCASEEQUALITY",
  "YYCASEINEQUALITY", "YYLOGNAND", "YYLOGNOR", "YYLOGOR", "YYLOGXNOR",
  "YYLOGEQUALITY", "YYLOGINEQUALITY", "YYLSHIFT", "YYMODULE", "YYNAND",
  "YYNBASSIGN", "YYNEGEDGE", "YYNOR", "YYNOT", "YYOR", "YYOUTPUT",
  "YYPARAMETER", "YYPOSEDGE", "YYPULL0", "YYPULL1", "YYREG", "YYRSHIFT",
  "YYSTRONG0", "YYSTRONG1", "YYSUPPLY0", "YYSUPPLY1", "YYSWIRE", "YYTASK",
  "YYTRI", "YYTRI0", "YYTRI1", "YYTRIAND", "YYTRIOR", "YYWAND", "YYWEAK0",
  "YYWEAK1", "YYWIRE", "YYWOR", "YYXNOR", "YYXOR", "YYDEFINE",
  "YYDEF_NULL", "PARALLEL_CASE", "FULL_CASE", "'?'", "':'", "'|'", "'^'",
  "'&'", "'<'", "'>'", "'+'", "'-'", "'*'", "'~'", "'!'",
  "YYUNARYOPERATOR", "';'", "'('", "')'", "','", "'['", "']'", "'#'",
  "'.'", "'='", "'{'", "'}'", "'@'", "$accept", "source_text",
  "description", "define", "@1", "@2", "target_statement", "module", "@3",
  "port_list_opt", "port_list", "port", "port_expression_opt",
  "port_expression", "port_reference", "@4", "port_reference_arg",
  "io_declaration_list", "io_declaration", "module_item_clr",
  "module_item", "task", "@5", "function", "@6", "range_or_type_opt",
  "range_or_type", "tf_declaration_clr", "tf_declaration_eclr",
  "tf_declaration", "parameter_declaration", "@7", "input_declaration",
  "@8", "output_declaration", "@9", "inout_declaration", "@10",
  "net_declaration", "nettype", "expandrange_opt", "expandrange",
  "reg_declaration", "@11", "integer_declaration", "@12",
  "continuous_assign", "@13", "parameter_override", "@14", "variable_list",
  "register_variable_list", "register_variable", "name_of_register",
  "drive_strength_opt", "drive_strength", "strength0", "strength1",
  "range_opt", "range", "assignment_list", "gate_instantiation",
  "gatetype", "gate_instance_list", "gate_instance",
  "name_of_gate_instance", "terminal_list", "terminal",
  "module_or_primitive_instantiation", "name_of_module_or_primitive",
  "module_or_primitive_option_clr", "module_or_primitive_option",
  "delay_or_parameter_value_assignment",
  "module_or_primitive_instance_list", "module_or_primitive_instance",
  "@15", "module_connection_list", "module_port_connection_list",
  "named_port_connection_list", "module_port_connection",
  "named_port_connection", "@16", "connect_port", "always_statement",
  "statement_opt", "statement_clr", "statement", "synopsys_directive",
  "assignment", "case_item_eclr", "case_item", "seq_block",
  "name_of_block", "block_declaration_clr", "block_declaration",
  "task_enable", "@17", "@18", "lvalue", "@19", "bit_exp",
  "mintypmax_expression_list", "mintypmax_expression", "expression_list",
  "expression", "primary", "@20", "bitExpression", "concatenation",
  "multiple_concatenation", "function_call", "@21", "identifier", "@22",
  "delay_control", "event_control", "event_expression",
  "ored_event_expression", "or_expression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,    63,    58,   124,    94,    38,    60,    62,
      43,    45,    42,   126,    33,   338,    59,    40,    41,    44,
      91,    93,    35,    46,    61,   123,   125,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   109,   110,   110,   112,   113,   111,   114,
     114,   114,   116,   115,   117,   117,   117,   118,   118,   119,
     120,   120,   121,   123,   122,   124,   124,   124,   125,   125,
     126,   126,   126,   127,   127,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     130,   129,   132,   131,   133,   133,   134,   134,   135,   135,
     136,   136,   137,   137,   137,   137,   137,   137,   139,   138,
     141,   140,   143,   142,   145,   144,   146,   146,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   148,
     148,   149,   151,   150,   153,   152,   155,   154,   157,   156,
     158,   158,   159,   159,   160,   160,   160,   161,   162,   162,
     163,   163,   164,   164,   164,   164,   164,   165,   165,   165,
     165,   165,   166,   166,   167,   167,   168,   168,   169,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   171,
     171,   172,   172,   173,   174,   174,   175,   176,   177,   177,
     178,   178,   179,   180,   180,   180,   180,   181,   181,   182,
     183,   182,   184,   184,   185,   185,   186,   186,   187,   187,
     189,   188,   190,   190,   191,   192,   192,   193,   193,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   195,   195,   195,   195,   195,   196,   196,
     197,   197,   198,   198,   198,   199,   199,   200,   201,   201,
     202,   202,   204,   203,   205,   203,   206,   207,   206,   206,
     206,   208,   208,   209,   209,   210,   211,   211,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     213,   213,   213,   214,   213,   213,   213,   213,   213,   213,
     213,   215,   215,   216,   217,   219,   218,   220,   221,   220,
     222,   222,   222,   222,   223,   223,   223,   223,   224,   224,
     224,   224,   225,   225,   226,   226,   226
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     0,     0,     5,     1,
       3,     1,     0,     7,     0,     3,     3,     1,     3,     1,
       0,     1,     1,     0,     3,     0,     3,     5,     1,     3,
       3,     3,     3,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     7,     0,     8,     0,     1,     1,     1,     0,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     0,     5,
       0,     5,     0,     5,     0,     5,     5,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     0,     5,     0,     4,     0,     4,     0,     4,
       1,     3,     1,     3,     1,     6,     4,     1,     0,     1,
       5,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     5,     3,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     4,     1,     1,     3,     1,     4,     1,     1,
       0,     2,     1,     2,     2,     2,     4,     1,     3,     3,
       0,     5,     1,     1,     1,     3,     1,     3,     0,     1,
       0,     6,     0,     1,     2,     0,     1,     0,     2,     1,
       2,     5,     7,     7,     7,     7,     9,     2,     5,     5,
       1,     1,     3,     0,     1,     1,     2,     2,     3,     3,
       1,     2,     3,     3,     2,     3,     6,     1,     0,     2,
       1,     1,     0,     3,     0,     6,     1,     0,     3,     1,
       2,     0,     1,     1,     3,     1,     1,     3,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     5,
       1,     1,     1,     0,     3,     1,     1,     1,     1,     3,
       2,     3,     5,     3,     6,     0,     5,     1,     0,     4,
       2,     2,     2,     4,     4,     4,     2,     4,     1,     2,
       2,     2,     3,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,     0,     6,     3,     5,     4,    12,     0,
      14,     7,    20,     0,     0,    23,   122,   122,   122,     0,
      17,    19,    21,    22,     0,    28,    33,   277,   221,   260,
     261,   268,     0,     0,     0,    11,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     8,     9,   228,   265,   266,
     267,   262,    25,     0,     0,   123,     0,     0,    15,    20,
      16,     0,     0,     0,   270,   222,   236,   237,   238,   234,
     235,   233,   229,   230,   232,   231,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   275,   278,     0,     0,    24,   221,     0,    32,    30,
      31,    18,    29,   148,   149,     0,   129,    96,   135,   136,
     137,    98,    13,    54,   122,   122,    94,   130,   132,   138,
     131,   122,   122,   122,    82,    86,    78,     0,    80,    85,
      81,    84,    88,    83,    79,    87,   134,   133,    49,    34,
      47,    48,    35,    36,    37,    38,    39,   108,    40,    41,
      45,    44,    42,     0,    43,   150,    46,     0,   269,     0,
     273,     0,   255,   253,   246,   244,   245,   247,   258,   242,
     243,   256,   254,   257,     0,    10,   251,   252,   250,   248,
     249,   239,   240,   241,     0,     0,   264,     0,   125,     0,
     277,   221,     0,   177,     0,     0,     0,     0,     0,   179,
       0,     0,   174,     0,   190,   191,     0,   219,   216,     0,
       0,     0,    57,     0,    55,    56,    74,    70,     0,    72,
      68,    92,    50,     0,    89,   109,   143,     0,     0,   139,
       0,     0,     0,   271,   227,     0,   226,     0,     0,   279,
       0,    26,     0,     0,     0,   220,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   286,     0,   180,     0,     0,
       0,   187,     0,   126,     0,    52,     0,     0,   107,     0,
     102,   104,     0,     0,     0,     0,   116,   121,   114,   119,
     113,   118,   112,   117,   115,   120,     0,     0,     0,    90,
      91,     0,   144,   146,   128,     0,     0,   168,     0,   151,
     152,     0,   157,   160,     0,     0,   259,   276,     0,   124,
     213,     0,   192,     0,     0,   207,   208,   205,   178,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   288,     0,
       0,     0,   199,     0,   198,     0,   218,    97,     0,    99,
       0,     0,   100,     0,    95,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,   100,   141,     0,   140,     0,
       0,     0,   162,   163,   164,   166,   169,   153,   154,     0,
     155,   147,     0,     0,   272,   274,    27,     0,   177,   193,
     193,   193,     0,     0,   291,   290,   289,   287,   295,   294,
     284,   296,     0,   285,     0,   280,   281,     0,   282,     0,
       0,   127,     0,    75,     0,    71,   103,   221,     0,    73,
      69,    93,   175,     0,     0,    77,    76,   145,   142,   170,
     159,   168,     0,     0,   223,   225,   158,   168,     0,   210,
     211,     0,   209,   195,   194,     0,     0,     0,     0,   181,
     292,   293,     0,   189,   188,   175,    60,    62,    63,    64,
      65,    66,    67,   101,   106,     0,    59,     0,   176,   110,
     111,     0,   165,   167,   156,     0,     0,   215,   206,   197,
     196,     0,     0,   200,     0,     0,     0,     0,     0,   283,
      61,     0,     0,    51,   172,   224,   161,     0,   204,   183,
     201,     0,   185,   184,     0,   182,    53,   105,     0,   173,
     203,   202,     0,   171,   186
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     5,     6,     9,    14,    45,     7,    10,    13,
      19,    20,    21,    22,    23,    52,   105,    24,    25,    62,
     149,   150,   285,   151,   350,   223,   224,   422,   455,   456,
     457,   283,   458,   277,   459,   282,   460,   276,   156,   157,
     298,   299,   461,   284,   462,   228,   160,   220,   161,   221,
     351,   279,   280,   281,   234,   235,   296,   297,    54,    55,
     272,   162,   163,   238,   239,   240,   301,   302,   164,   165,
     241,   309,   310,   311,   312,   383,   371,   372,   373,   374,
     375,   471,   508,   166,   467,   259,   328,   445,   213,   482,
     483,   214,   326,   388,   442,   215,   253,   254,   216,   270,
      64,   433,   434,   484,   246,    47,   103,    65,    48,    49,
      50,   194,    51,   195,   343,   219,   339,   340,   402
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -408
static const yytype_int16 yypact[] =
{
    -408,    13,  -408,    42,  -408,  -408,  -408,  -408,  -408,    48,
     -56,  -408,   108,    -5,   678,  -408,     3,     3,     3,   -66,
    -408,  -408,  -408,  -408,    68,  -408,  -408,  -408,    44,  -408,
    -408,  -408,    17,    17,    17,  -408,    17,    17,    17,    17,
      17,    17,    17,   836,   836,  -408,  1656,  -408,  -408,  -408,
    -408,    -3,    50,   865,   129,  -408,   129,   129,  -408,   154,
    -408,    10,  1004,   836,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  1324,   -43,  1005,   836,
     836,   836,   836,   836,   836,   836,   836,   836,   836,   836,
     836,   836,   836,   836,   836,   836,   836,   836,   836,   836,
     836,  -408,  -408,    44,   836,  -408,   120,  1675,    70,    70,
      70,  -408,  -408,  -408,  -408,   473,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,    -4,     3,     3,  -408,  -408,  -408,  -408,
    -408,     3,     3,     3,  -408,  -408,  -408,   175,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,    93,  -408,  -408,
    -408,  -408,  -408,     7,  -408,  -408,  -408,  1069,  -408,   836,
    -408,   836,    69,    69,  1904,   178,   178,  1840,  1939,   178,
     178,   139,    69,   139,  1739,  1822,  1916,  1939,  1996,    69,
      69,   143,   143,  -408,   836,   205,  -408,  1088,  -408,   836,
     168,    44,     5,   160,   152,   189,   203,   240,   248,  -408,
     836,    38,  -408,   155,  -408,  -408,   -42,  -408,    34,   473,
       5,     5,  -408,   273,  -408,  -408,  -408,  -408,   285,  -408,
    -408,  -408,  -408,   241,     3,  -408,  -408,   836,    99,  -408,
     250,    15,   836,  -408,  1822,    32,  1822,   836,   179,  -408,
     836,  -408,  1154,   258,   260,  -408,   262,   -11,   360,   136,
     836,   836,   836,     5,   836,  -408,   453,  -408,   706,   706,
      44,  -408,   106,  -408,   107,  -408,   129,   129,  -408,   116,
    -408,   264,   129,     5,   285,   270,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,   271,   272,     5,  -408,
    -408,   196,  -408,  1822,  -408,     7,   836,   733,    11,  -408,
    -408,   117,  -408,    70,  1173,   263,  1822,  -408,  1239,  -408,
    -408,   836,  -408,   836,   836,  -408,  -408,  -408,  -408,  1343,
    1407,  1426,   279,  1490,   836,   836,   836,   278,  1822,     0,
     112,    31,  1822,   836,  1822,   836,  -408,  -408,     5,  -408,
     283,   141,    70,   146,  -408,   285,   906,   157,   163,   164,
    -408,   227,   277,   191,   220,   122,  -408,   836,  -408,   213,
     129,   284,   282,   287,  -408,  -408,  1822,  -408,  -408,   836,
      70,  -408,    23,   292,  -408,  -408,  -408,   219,     2,   242,
     242,   242,   836,   473,  1822,  1822,  1822,  -408,  -408,  -408,
    -408,  -408,   516,  -408,   516,  -408,  -408,   836,    70,  1509,
    1573,  -408,   142,  -408,   129,  -408,  -408,   232,  1758,  -408,
    -408,  -408,   411,   286,   296,  -408,  -408,  -408,  -408,    70,
    -408,   836,   293,   237,  -408,  1822,  -408,   733,   299,  -408,
    -408,   177,  -408,   315,   318,   810,   810,   810,  1592,   380,
    -408,  -408,   304,  -408,  -408,   411,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,    70,  -408,   836,  -408,   377,  -408,  -408,
    -408,   307,  -408,  -408,  -408,   836,   308,  -408,  -408,  -408,
    -408,   345,   582,  -408,   -49,   611,   637,     5,   473,  -408,
    -408,   382,  1258,  -408,   836,  -408,  -408,   473,  -408,  -408,
    -408,   473,  -408,  -408,   310,  -408,  -408,  -408,   311,  1822,
    -408,  -408,   473,  -408,  -408
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -408,  -408,  -408,   348,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,   352,  -408,  -408,  -408,  -408,  -408,  -408,   357,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -407,
     -60,  -408,   351,  -408,   362,  -408,   363,  -408,  -408,  -408,
    -408,  -408,   -58,  -408,   368,  -408,  -408,  -408,  -408,  -408,
    -134,   135,    76,  -408,  -408,  -408,    71,    73,    -6,  -118,
    -196,  -408,  -408,  -408,   127,  -408,   130,    75,  -408,  -408,
    -408,  -408,  -408,  -408,    53,  -408,    12,  -408,  -408,     6,
       8,  -408,  -408,  -408,    -8,    60,   -96,   -47,  -174,   -95,
    -303,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -162,  -408,
     253,  -408,  -380,   -38,   -14,   492,  -408,  -100,  -114,  -408,
    -408,  -408,    -2,  -408,   186,  -408,  -213,  -408,   123
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -264
static const yytype_int16 yytable[] =
{
      46,   217,   152,   196,   158,   225,    77,   268,    27,   201,
     236,    56,    57,     2,    27,   466,   377,   378,    27,   212,
      27,    28,    29,    30,    31,   274,    27,   452,   256,    76,
      78,   222,    58,    59,    27,   501,   405,   406,   323,   107,
     257,    12,   398,    16,    17,     8,   273,   273,   490,   167,
     169,    11,   108,   399,   109,   110,   169,   132,   257,   257,
       3,   133,   269,   170,    18,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   358,   217,   332,
     197,    26,     4,   324,   101,   495,    53,  -263,   400,   401,
     102,   257,   364,    53,   237,   217,   217,   217,   379,   273,
     210,    15,   307,   218,    43,    88,   300,   308,   226,   227,
     307,   257,    44,   271,   273,   229,   230,   231,   407,    90,
     265,   169,    27,   245,  -217,   266,   257,   102,   315,   200,
     201,    16,    17,   353,    63,   217,   202,   203,   357,   217,
     104,   204,   205,   206,   398,   244,   248,    15,   327,    98,
      99,   100,    18,   207,   363,   399,    60,    61,   208,   217,
     346,  -216,    77,   102,   411,   124,   125,   126,   232,   500,
     200,   201,   500,   500,   217,   252,   257,   202,   203,   450,
     233,   451,   204,   205,   206,   304,   131,   132,   305,   478,
     218,   133,   347,   349,   207,   348,   348,    79,   249,   208,
     403,   401,   354,   381,    80,   355,   382,   218,   218,   218,
      63,   198,  -217,   303,    88,   102,  -216,    89,   314,    98,
      99,   100,   209,   316,   217,   100,   318,   413,    90,   313,
     414,   210,   415,   211,   258,   414,   329,   330,   331,   260,
     333,   267,   338,   419,   342,   344,   414,   218,   287,   420,
     421,   218,   348,   355,  -212,  -214,    96,    97,    98,    99,
     100,   286,   287,   209,   352,   352,   275,   317,   169,   217,
     352,   218,   210,   387,   211,   289,   261,   425,   278,   291,
     414,   293,   303,   376,   366,   367,   365,   449,   288,   289,
     262,   295,   290,   291,   292,   293,   380,   286,   217,   342,
     344,   428,   367,   504,   294,   295,   426,   438,   169,   348,
     394,   395,   396,   443,   444,   257,   468,   217,   439,   409,
     440,   410,    63,   464,   288,   474,   475,   263,   290,   408,
     292,   217,   418,   446,   447,   264,   218,   306,   200,   201,
     294,   485,   486,   303,   320,   202,   203,   321,   322,   468,
     204,   205,   206,   325,   356,   435,   360,   217,   429,   385,
     361,   362,   207,   217,   217,   392,   397,   208,   448,   412,
     313,   431,   430,   217,   469,   498,   432,   217,   338,   437,
     338,   218,   505,   435,   470,   477,   370,   479,   217,   480,
     488,   510,   489,   493,   494,   511,   496,   506,   512,   513,
     148,   111,   463,   153,   200,   201,   514,   376,   112,   359,
     218,   202,   203,   376,   154,   155,   204,   205,   206,   497,
     159,   416,   368,   424,   423,   436,   369,   472,   207,   218,
     473,   209,   427,   208,   124,   125,   126,   491,   441,   476,
     210,   492,   211,   218,   255,   345,    27,    28,    29,    30,
      31,   435,     0,   404,     0,   131,   132,     0,     0,     0,
     133,     0,     0,     0,   334,     0,   200,   201,     0,   218,
     509,     0,     0,   202,   203,   218,   218,     0,   204,   205,
     206,     0,     0,    32,    33,   218,    34,     0,     0,   218,
     207,     0,     0,   335,     0,   208,     0,   209,     0,   336,
     218,     0,     0,     0,     0,     0,   210,     0,   211,    27,
      28,    29,    30,    31,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,     0,   334,    36,    37,
      38,     0,     0,    39,    40,   337,    41,    42,     0,     0,
      43,     0,     0,     0,     0,     0,    32,    33,    44,    34,
       0,     0,     0,     0,     0,     0,   335,     0,     0,   209,
       0,     0,   336,     0,     0,     0,     0,     0,   210,     0,
     211,     0,     0,     0,     0,    27,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     481,    36,    37,    38,     0,   499,    39,    40,     0,    41,
      42,     0,     0,    43,    27,    28,    29,    30,    31,     0,
       0,    44,    32,    33,     0,    34,     0,     0,     0,   481,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
      27,    28,    29,    30,    31,     0,     0,     0,     0,     0,
       0,    32,    33,     0,    34,   481,     0,     0,     0,     0,
     503,     0,     0,     0,     0,     0,     0,    36,    37,    38,
       0,     0,    39,    40,     0,    41,    42,    32,    33,    43,
      34,    27,    28,    29,    30,    31,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,    36,    37,    38,     0,
       0,    39,    40,     0,    41,    42,     0,     0,    43,    27,
      28,    29,    30,    31,     0,     0,    44,     0,    32,    33,
       0,    34,    36,    37,    38,     0,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,    27,    28,    29,    30,
      31,     0,    44,     0,     0,     0,    32,    33,     0,    34,
       0,     0,     0,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    36,    37,    38,     0,     0,    39,    40,
       0,    41,    42,    32,    33,    43,    34,     0,     0,     0,
       0,     0,     0,    44,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,     0,   341,     0,
       0,    44,     0,    27,    28,    29,    30,    31,    36,    37,
      38,     0,     0,    39,    40,     0,    41,    42,   481,     0,
      43,     0,     0,     0,     0,     0,   370,     0,    44,    27,
      28,    29,    30,    31,     0,     0,     0,     0,     0,     0,
      32,    33,     0,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    27,   106,
      29,    30,    31,     0,     0,     0,    32,    33,     0,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,     0,     0,
      39,    40,     0,    41,    42,    32,    33,    43,    34,    27,
     417,    29,    30,    31,     0,    44,     0,     0,     0,     0,
       0,    36,    37,    38,     0,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,     0,     0,     0,
       0,    44,     0,     0,     0,     0,    32,    33,     0,    34,
      36,    37,    38,     0,     0,    39,    40,     0,    41,    42,
       0,     0,    43,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,   113,   114,     0,
       0,    44,   115,   116,   117,     0,   118,   119,   120,     0,
       0,     0,     0,   121,     0,     0,     0,     0,   122,     0,
       0,     0,   123,     0,    79,     0,     0,   124,   125,   126,
       0,    80,    81,    82,    83,     0,     0,    84,    85,    86,
      87,    88,   127,     0,    89,   128,   129,   130,   131,   132,
       0,     0,     0,   133,     0,    90,     0,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,     0,     0,   144,
     145,   146,   147,     4,     0,     0,     0,     0,    91,     0,
      93,    94,    95,    96,    97,    98,    99,   100,    79,     0,
       0,     0,     0,     0,     0,    80,    81,    82,    83,     0,
     171,    84,    85,    86,    87,    88,     0,    79,    89,     0,
       0,     0,     0,     0,    80,    81,    82,    83,     0,    90,
      84,    85,    86,    87,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,    91,   242,    93,    94,    95,    96,    97,    98,
      99,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     243,    91,   250,    93,    94,    95,    96,    97,    98,    99,
     100,     0,     0,    79,     0,     0,     0,     0,     0,   251,
      80,    81,    82,    83,     0,     0,    84,    85,    86,    87,
      88,     0,    79,    89,     0,     0,     0,     0,     0,    80,
      81,    82,    83,     0,    90,    84,    85,    86,    87,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,    91,     0,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   319,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,     0,     0,    79,     0,
       0,     0,     0,     0,   384,    80,    81,    82,    83,     0,
       0,    84,    85,    86,    87,    88,     0,    79,    89,     0,
       0,     0,     0,     0,    80,    81,    82,    83,     0,    90,
      84,    85,    86,    87,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     386,    91,     0,    93,    94,    95,    96,    97,    98,    99,
     100,     0,     0,    79,     0,     0,     0,     0,     0,   507,
      80,    81,    82,    83,     0,     0,    84,    85,    86,    87,
      88,     0,    79,    89,     0,     0,     0,     0,     0,    80,
      81,    82,    83,     0,    90,    84,    85,    86,    87,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,    91,     0,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,     0,
       0,     0,   168,     0,     0,     0,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,    79,     0,     0,     0,
       0,   389,     0,    80,    81,    82,    83,     0,     0,    84,
      85,    86,    87,    88,     0,    79,    89,     0,     0,     0,
       0,     0,    80,    81,    82,    83,     0,    90,    84,    85,
      86,    87,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
      91,     0,    93,    94,    95,    96,    97,    98,    99,   100,
       0,     0,     0,     0,     0,   390,     0,     0,     0,    91,
       0,    93,    94,    95,    96,    97,    98,    99,   100,    79,
       0,     0,     0,     0,   391,     0,    80,    81,    82,    83,
       0,     0,    84,    85,    86,    87,    88,     0,    79,    89,
       0,     0,     0,     0,     0,    80,    81,    82,    83,     0,
      90,    84,    85,    86,    87,    88,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,    91,     0,    93,    94,    95,    96,    97,
      98,    99,   100,     0,     0,     0,     0,     0,   393,     0,
       0,     0,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,    79,     0,     0,   453,     0,     0,     0,    80,
      81,    82,    83,     0,     0,    84,    85,    86,    87,    88,
       0,    79,    89,     0,     0,     0,     0,     0,    80,    81,
      82,    83,     0,    90,    84,    85,    86,    87,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,     0,     0,     0,   454,
       0,     0,     0,     0,     0,    91,     0,    93,    94,    95,
      96,    97,    98,    99,   100,    79,     0,     0,   487,     0,
       0,     0,    80,    81,    82,    83,     0,     0,    84,    85,
      86,    87,    88,     0,    79,    89,     0,     0,     0,     0,
       0,    80,    81,    82,    83,     0,    90,    84,    85,    86,
      87,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,   199,
      93,    94,    95,    96,    97,    98,    99,   100,    79,     0,
       0,     0,     0,     0,     0,    80,    81,    82,    83,     0,
       0,    84,    85,    86,    87,    88,     0,    79,    89,     0,
       0,     0,     0,     0,    80,    81,    82,    83,     0,    90,
      84,    85,    86,    87,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,    91,   247,    93,    94,    95,    96,    97,    98,
      99,   100,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,   465,    93,    94,    95,    96,    97,    98,    99,
     100,    79,     0,     0,     0,     0,     0,     0,    80,    81,
      82,    83,     0,     0,    84,    85,    86,    87,    88,    79,
       0,    89,     0,     0,     0,     0,    80,    81,    82,    83,
       0,     0,    90,    85,    86,    87,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,    91,     0,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
      98,    99,   100,    79,     0,     0,     0,     0,     0,     0,
      80,     0,    82,    83,     0,    79,     0,    85,    86,    87,
      88,     0,    80,    89,    82,    83,     0,     0,     0,    85,
      86,    87,    88,     0,    90,    89,     0,     0,    79,     0,
       0,     0,     0,     0,     0,    80,    90,    82,    83,     0,
       0,     0,     0,    86,    87,    88,     0,     0,    89,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,    90,
       0,     0,    94,    95,    96,    97,    98,    99,   100,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    95,    96,    97,    98,
      99,   100,    80,     0,    82,    83,     0,     0,     0,     0,
      86,    87,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100
};

static const yytype_int16 yycheck[] =
{
      14,   115,    62,   103,    62,   123,    44,    49,     3,     4,
       3,    17,    18,     0,     3,   422,     5,     6,     3,   115,
       3,     4,     5,     6,     7,   221,     3,   407,   202,    43,
      44,    35,    98,    99,     3,    84,     5,     6,    49,    53,
     202,    97,    42,    33,    34,     3,   220,   221,   455,    63,
      99,     3,    54,    53,    56,    57,    99,    55,   220,   221,
      47,    59,   104,   106,    54,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   283,   202,   263,
     104,    96,    79,   104,    97,   475,   100,   100,    98,    99,
     103,   263,   298,   100,    97,   219,   220,   221,    97,   283,
     105,     3,    97,   115,    97,    46,   234,   102,   124,   125,
      97,   283,   105,   219,   298,   131,   132,   133,    97,    60,
      92,    99,     3,   171,   100,    97,   298,   103,   106,     3,
       4,    33,    34,   277,   100,   259,    10,    11,   282,   263,
     100,    15,    16,    17,    42,   169,   194,     3,    22,    90,
      91,    92,    54,    27,   298,    53,    98,    99,    32,   283,
     270,    49,   210,   103,   348,    33,    34,    35,     3,   482,
       3,     4,   485,   486,   298,   199,   348,    10,    11,   402,
      97,   404,    15,    16,    17,    96,    54,    55,    99,    22,
     202,    59,    96,    96,    27,    99,    99,    29,     3,    32,
      98,    99,    96,    96,    36,    99,    99,   219,   220,   221,
     100,   101,   100,   237,    46,   103,   104,    49,   242,    90,
      91,    92,    96,   247,   348,    92,   250,    96,    60,   241,
      99,   105,    96,   107,    84,    99,   260,   261,   262,    97,
     264,    96,   266,    96,   268,   269,    99,   259,    31,    96,
      96,   263,    99,    99,    96,    97,    88,    89,    90,    91,
      92,    30,    31,    96,   276,   277,     3,    98,    99,   393,
     282,   283,   105,   321,   107,    58,    97,    96,     3,    62,
      99,    64,   306,   307,    98,    99,   298,   393,    57,    58,
      97,    74,    61,    62,    63,    64,   308,    30,   422,   323,
     324,    98,    99,   487,    73,    74,    96,    98,    99,    99,
     334,   335,   336,    81,    82,   487,   422,   441,   388,   343,
     388,   345,   100,   101,    57,    98,    99,    97,    61,   341,
      63,   455,   356,   390,   391,    97,   348,    97,     3,     4,
      73,   446,   447,   367,    96,    10,    11,    97,    96,   455,
      15,    16,    17,     3,   100,   379,    96,   481,   370,   106,
      99,    99,    27,   487,   488,    96,    98,    32,   392,    96,
     382,    99,    98,   497,    98,   481,    99,   501,   402,    97,
     404,   393,   488,   407,    98,    96,   103,    82,   512,    81,
      20,   497,    98,    26,    97,   501,    98,    25,    98,    98,
      62,    59,   414,    62,     3,     4,   512,   431,    61,   284,
     422,    10,    11,   437,    62,    62,    15,    16,    17,    84,
      62,   355,   305,   362,   361,   382,   306,   431,    27,   441,
     432,    96,   367,    32,    33,    34,    35,   455,   388,   437,
     105,   465,   107,   455,   201,   269,     3,     4,     5,     6,
       7,   475,    -1,   340,    -1,    54,    55,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    21,    -1,     3,     4,    -1,   481,
     494,    -1,    -1,    10,    11,   487,   488,    -1,    15,    16,
      17,    -1,    -1,    40,    41,   497,    43,    -1,    -1,   501,
      27,    -1,    -1,    50,    -1,    32,    -1,    96,    -1,    56,
     512,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,     3,
       4,     5,     6,     7,    32,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    21,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    40,    41,   105,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    96,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    85,    86,    87,    -1,    23,    90,    91,    -1,    93,
      94,    -1,    -1,    97,     3,     4,     5,     6,     7,    -1,
      -1,   105,    40,    41,    -1,    43,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      -1,    -1,    90,    91,    -1,    93,    94,    40,    41,    97,
      43,     3,     4,     5,     6,     7,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    -1,
      -1,    90,    91,    -1,    93,    94,    -1,    -1,    97,     3,
       4,     5,     6,     7,    -1,    -1,   105,    -1,    40,    41,
      -1,    43,    85,    86,    87,    -1,    -1,    90,    91,    -1,
      93,    94,    -1,    -1,    97,    -1,     3,     4,     5,     6,
       7,    -1,   105,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      -1,    93,    94,    40,    41,    97,    43,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    -1,    93,
      94,    -1,    -1,    97,    -1,    -1,    -1,    -1,   102,    -1,
      -1,   105,    -1,     3,     4,     5,     6,     7,    85,    86,
      87,    -1,    -1,    90,    91,    -1,    93,    94,    18,    -1,
      97,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    -1,    -1,
      90,    91,    -1,    93,    94,    40,    41,    97,    43,     3,
       4,     5,     6,     7,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    -1,    93,
      94,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      85,    86,    87,    -1,    -1,    90,    91,    -1,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    -1,    93,
      94,    -1,    -1,    97,    -1,    -1,    -1,     3,     4,    -1,
      -1,   105,     8,     9,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    28,    -1,    29,    -1,    -1,    33,    34,    35,
      -1,    36,    37,    38,    39,    -1,    -1,    42,    43,    44,
      45,    46,    48,    -1,    49,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    -1,    60,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    -1,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    83,    -1,
      85,    86,    87,    88,    89,    90,    91,    92,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    -1,
     105,    42,    43,    44,    45,    46,    -1,    29,    49,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    39,    -1,    60,
      42,    43,    44,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,   101,
      36,    37,    38,    39,    -1,    -1,    42,    43,    44,    45,
      46,    -1,    29,    49,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    39,    -1,    60,    42,    43,    44,    45,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    83,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    83,    -1,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,   101,    36,    37,    38,    39,    -1,
      -1,    42,    43,    44,    45,    46,    -1,    29,    49,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    39,    -1,    60,
      42,    43,    44,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    83,    -1,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    83,    -1,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,   101,
      36,    37,    38,    39,    -1,    -1,    42,    43,    44,    45,
      46,    -1,    29,    49,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    39,    -1,    60,    42,    43,    44,    45,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    83,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,    -1,    83,    -1,    85,    86,
      87,    88,    89,    90,    91,    92,    29,    -1,    -1,    -1,
      -1,    98,    -1,    36,    37,    38,    39,    -1,    -1,    42,
      43,    44,    45,    46,    -1,    29,    49,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    39,    -1,    60,    42,    43,
      44,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      83,    -1,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    83,
      -1,    85,    86,    87,    88,    89,    90,    91,    92,    29,
      -1,    -1,    -1,    -1,    98,    -1,    36,    37,    38,    39,
      -1,    -1,    42,    43,    44,    45,    46,    -1,    29,    49,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    -1,
      60,    42,    43,    44,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    83,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    -1,    83,    -1,    85,    86,    87,    88,    89,    90,
      91,    92,    29,    -1,    -1,    96,    -1,    -1,    -1,    36,
      37,    38,    39,    -1,    -1,    42,    43,    44,    45,    46,
      -1,    29,    49,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    39,    -1,    60,    42,    43,    44,    45,    46,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    83,    -1,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    29,    -1,    -1,    96,    -1,
      -1,    -1,    36,    37,    38,    39,    -1,    -1,    42,    43,
      44,    45,    46,    -1,    29,    49,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    39,    -1,    60,    42,    43,    44,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    -1,
      -1,    42,    43,    44,    45,    46,    -1,    29,    49,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    39,    -1,    60,
      42,    43,    44,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    29,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    39,    -1,    -1,    42,    43,    44,    45,    46,    29,
      -1,    49,    -1,    -1,    -1,    -1,    36,    37,    38,    39,
      -1,    -1,    60,    43,    44,    45,    46,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    83,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    29,    -1,    43,    44,    45,
      46,    -1,    36,    49,    38,    39,    -1,    -1,    -1,    43,
      44,    45,    46,    -1,    60,    49,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    60,    38,    39,    -1,
      -1,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    60,
      -1,    -1,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    87,    88,    89,    90,
      91,    92,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   109,     0,    47,    79,   110,   111,   115,     3,   112,
     116,     3,    97,   117,   113,     3,    33,    34,    54,   118,
     119,   120,   121,   122,   125,   126,    96,     3,     4,     5,
       6,     7,    40,    41,    43,    80,    85,    86,    87,    90,
      91,    93,    94,    97,   105,   114,   212,   213,   216,   217,
     218,   220,   123,   100,   166,   167,   166,   166,    98,    99,
      98,    99,   127,   100,   208,   215,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   212,   211,   212,    29,
      36,    37,    38,    39,    42,    43,    44,    45,    46,    49,
      60,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    97,   103,   214,   100,   124,     4,   212,   220,   220,
     220,   119,   126,     3,     4,     8,     9,    10,    12,    13,
      14,    19,    24,    28,    33,    34,    35,    48,    51,    52,
      53,    54,    55,    59,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    75,    76,    77,    78,   111,   128,
     129,   131,   138,   140,   142,   144,   146,   147,   150,   152,
     154,   156,   169,   170,   176,   177,   191,   212,    98,    99,
     106,   105,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   219,   221,   215,   212,   101,    84,
       3,     4,    10,    11,    15,    16,    17,    27,    32,    96,
     105,   107,   194,   196,   199,   203,   206,   216,   220,   223,
     155,   157,    35,   133,   134,   167,   166,   166,   153,   166,
     166,   166,     3,    97,   162,   163,     3,    97,   171,   172,
     173,   178,    84,   101,   212,   211,   212,    84,   211,     3,
      84,   101,   212,   204,   205,   208,   196,   206,    84,   193,
      97,    97,    97,    97,    97,    92,    97,    96,    49,   104,
     207,   194,   168,   196,   168,     3,   145,   141,     3,   159,
     160,   161,   143,   139,   151,   130,    30,    31,    57,    58,
      61,    62,    63,    64,    73,    74,   164,   165,   148,   149,
     167,   174,   175,   212,    96,    99,    97,    97,   102,   179,
     180,   181,   182,   220,   212,   106,   212,    98,   212,   101,
      96,    97,    96,    49,   104,     3,   200,    22,   194,   212,
     212,   212,   196,   212,    21,    50,    56,    92,   212,   224,
     225,   102,   212,   222,   212,   222,   215,    96,    99,    96,
     132,   158,   220,   158,    96,    99,   100,   158,   168,   159,
      96,    99,    99,   158,   168,   220,    98,    99,   172,   174,
     103,   184,   185,   186,   187,   188,   212,     5,     6,    97,
     220,    96,    99,   183,   101,   106,   101,   211,   201,    98,
      98,    98,    96,    98,   212,   212,   212,    98,    42,    53,
      98,    99,   226,    98,   226,     5,     6,    97,   220,   212,
     212,   196,    96,    96,    99,    96,   160,     4,   212,    96,
      96,    96,   135,   165,   164,    96,    96,   175,    98,   220,
      98,    99,    99,   209,   210,   212,   182,    97,    98,   138,
     150,   193,   202,    81,    82,   195,   195,   195,   212,   194,
     224,   224,   210,    96,    96,   136,   137,   138,   140,   142,
     144,   150,   152,   220,   101,    84,   137,   192,   194,    98,
      98,   189,   187,   188,    98,    99,   184,    96,    22,    82,
      81,    18,   197,   198,   211,   197,   197,    96,    20,    98,
     137,   192,   212,    26,    97,   210,    98,    84,   194,    23,
     198,    84,    23,    23,   196,   194,    25,   101,   190,   212,
     194,   194,    98,    98,   194
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 312 "vlpYacc.y"
    {
             YYTRACE("sorce_text:");
          ;}
    break;

  case 3:
#line 316 "vlpYacc.y"
    {
             YYTRACE("source_text: source_text description");
             if ((yyvsp[(2) - (2)].modulePtr) != NULL)
                VLDesign.setModule((yyvsp[(2) - (2)].modulePtr));
          ;}
    break;

  case 4:
#line 325 "vlpYacc.y"
    {
             YYTRACE("description: module");
             (yyval.modulePtr) = (yyvsp[(1) - (1)].modulePtr);
             //cout << "# module = " << ++(LY->mod) << endl;
          ;}
    break;

  case 5:
#line 331 "vlpYacc.y"
    {
             YYTRACE("description: define");
             (yyval.modulePtr) = NULL;
          ;}
    break;

  case 6:
#line 343 "vlpYacc.y"
    { (LY->state) = DEFINE; ;}
    break;

  case 7:
#line 347 "vlpYacc.y"
    { 
             BaseModule :: setName(*(yyvsp[(3) - (3)].stringPtr));
             YYTRACE("define: YYDEFINE YYID target_statement");
             LY->tempStr3 = *(yyvsp[(3) - (3)].stringPtr); 
          ;}
    break;

  case 8:
#line 353 "vlpYacc.y"
    {
             LY->defineMap.insert(LY->tempStr3, (yyvsp[(5) - (5)].basePtr));
             LY->tempStr3 = "";
             (LY->state) = S_NULL;
          ;}
    break;

  case 9:
#line 362 "vlpYacc.y"
    {
           YYTRACE("target_statement: expression");
           (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
        ;}
    break;

  case 10:
#line 367 "vlpYacc.y"
    {
           YYTRACE("target_statement: expression : expression");
           (yyval.basePtr) = new VlpConnectNode((yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
           BaseModule :: setCDFGNode((yyval.basePtr));
        ;}
    break;

  case 11:
#line 373 "vlpYacc.y"
    {
           YYTRACE("target_statement: YYDEF_NULL");
           (yyval.basePtr) = NULL;
        ;}
    break;

  case 12:
#line 381 "vlpYacc.y"
    { 
            LY->modulePtr = new VlgModule(*(yyvsp[(2) - (2)].stringPtr)); 
            LY->modulePtr->setLineCol(LY->lineNo, (LY->colNo)-6-((yyvsp[(2) - (2)].stringPtr)->size()) );
            if (LY->blackBox == true) 
               LY->modulePtr->setIsBlackBox();
         ;}
    break;

  case 13:
#line 389 "vlpYacc.y"
    {
             YYTRACE("module: YYMODULE YYID port_list_opt ';' module_item_clr YYENDMODULE");
             (yyval.modulePtr) = LY->modulePtr;
         ;}
    break;

  case 14:
#line 403 "vlpYacc.y"
    {
             YYTRACE("port_list_opt:");
          ;}
    break;

  case 15:
#line 407 "vlpYacc.y"
    {
             YYTRACE("port_list_opt: '(' port_list ')'");
          ;}
    break;

  case 16:
#line 411 "vlpYacc.y"
    {
             YYTRACE("port_list_opt: '(' io_declaration_list ')'");
          ;}
    break;

  case 17:
#line 418 "vlpYacc.y"
    {
             YYTRACE("port_list: port");
          ;}
    break;

  case 18:
#line 422 "vlpYacc.y"
    {
             YYTRACE("port_list: port_list ',' port");
          ;}
    break;

  case 19:
#line 429 "vlpYacc.y"
    {
             YYTRACE("port: port_expression_opt");
          ;}
    break;

  case 20:
#line 443 "vlpYacc.y"
    {
              YYTRACE("port_expression_opt:");
              cerr << "Strange syntax in line " << LY->lineNo << endl;
              LY->modulePtr->setIO(NULL);              
           ;}
    break;

  case 21:
#line 449 "vlpYacc.y"
    {
              YYTRACE("port_expression_opt: port_expression");
           ;}
    break;

  case 22:
#line 456 "vlpYacc.y"
    {
             YYTRACE("port_expression: port_reference");
             LY->ioPtr = NULL;
          ;}
    break;

  case 23:
#line 479 "vlpYacc.y"
    {
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr)));
             BaseModule :: setCDFGNode(LY->ioPtr);
	  ;}
    break;

  case 24:
#line 484 "vlpYacc.y"
    {
             YYTRACE("port_reference: YYID port_reference_arg");
             LY->modulePtr->setIO(LY->ioPtr);
          ;}
    break;

  case 25:
#line 492 "vlpYacc.y"
    {
             YYTRACE("port_reference_arg:");             
          ;}
    break;

  case 26:
#line 496 "vlpYacc.y"
    {
             YYTRACE("port_reference_arg: '[' expression ']'");
             if ((yyvsp[(2) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression 
             {
                VlpBaseNode* msb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(2) - (3)].basePtr)))->getFirst()  ); 
                VlpBaseNode* lsb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(2) - (3)].basePtr)))->getSecond() ); 
                LY->ioPtr->setSigWidth(msb, lsb);
             }
             else
                LY->ioPtr->setSigWidth((yyvsp[(2) - (3)].basePtr), (yyvsp[(2) - (3)].basePtr));
          ;}
    break;

  case 27:
#line 508 "vlpYacc.y"
    {
             YYTRACE("port_reference_arg: '[' expression ':' expression ']'");
             LY->ioPtr->setSigWidth((yyvsp[(2) - (5)].basePtr), (yyvsp[(4) - (5)].basePtr));
          ;}
    break;

  case 28:
#line 516 "vlpYacc.y"
    {
          ;}
    break;

  case 29:
#line 519 "vlpYacc.y"
    {
          ;}
    break;

  case 30:
#line 525 "vlpYacc.y"
    {
             YYTRACE("io_declaration: YYINPUT range_opt identifier");
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(3) - (3)].stringPtr)), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(LY->ioPtr);
             LY->modulePtr->setIO(LY->ioPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(input);
             LY->modulePtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);
          ;}
    break;

  case 31:
#line 536 "vlpYacc.y"
    {
             YYTRACE("io_declaration: YYOUTPUT range_opt identifier");
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(3) - (3)].stringPtr)), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(LY->ioPtr);
             LY->modulePtr->setIO(LY->ioPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(output);
             LY->modulePtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);
          ;}
    break;

  case 32:
#line 547 "vlpYacc.y"
    {
             YYTRACE("io_declaration: YYINOUT range_opt identifier");
             LY->ioPtr = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(3) - (3)].stringPtr)), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(LY->ioPtr);
             LY->modulePtr->setIO(LY->ioPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(inout);
             LY->modulePtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);
          ;}
    break;

  case 33:
#line 561 "vlpYacc.y"
    {
             YYTRACE("module_item_clr:");
          ;}
    break;

  case 34:
#line 565 "vlpYacc.y"
    {
             YYTRACE("module_item_clr: module_item_clr module_item");
          ;}
    break;

  case 35:
#line 572 "vlpYacc.y"
    {
             YYTRACE("module_item: parameter_declaration");
          ;}
    break;

  case 36:
#line 576 "vlpYacc.y"
    {
             YYTRACE("module_item: input_declaration");
          ;}
    break;

  case 37:
#line 580 "vlpYacc.y"
    {
             YYTRACE("module_item: output_declaration");
          ;}
    break;

  case 38:
#line 584 "vlpYacc.y"
    {
             YYTRACE("module_item: inout_declaration");
          ;}
    break;

  case 39:
#line 588 "vlpYacc.y"
    {
             YYTRACE("module_item: net_declaration");
             if ((yyvsp[(1) - (1)].assignPtr) != NULL) //special case: assignment_list
                LY->modulePtr->setDataFlow((yyvsp[(1) - (1)].assignPtr));
          ;}
    break;

  case 40:
#line 594 "vlpYacc.y"
    {
             YYTRACE("module_item: reg_declaration");
          ;}
    break;

  case 41:
#line 602 "vlpYacc.y"
    {
             YYTRACE("module_item: integer_declaration");
          ;}
    break;

  case 42:
#line 615 "vlpYacc.y"
    {
             YYTRACE("module_item: gate_instantiation");
          ;}
    break;

  case 43:
#line 619 "vlpYacc.y"
    {
             YYTRACE("module_item: module_or_primitive_instantiation");
          ;}
    break;

  case 44:
#line 623 "vlpYacc.y"
    {
             YYTRACE("module_item: parameter_override");
          ;}
    break;

  case 45:
#line 627 "vlpYacc.y"
    {
             YYTRACE("module_item: continous_assign");
             LY->modulePtr->setDataFlow((yyvsp[(1) - (1)].assignPtr));
          ;}
    break;

  case 46:
#line 640 "vlpYacc.y"
    {
             YYTRACE("module_item: always_statement");
             LY->modulePtr->setDataFlow((yyvsp[(1) - (1)].alwaysPtr));
             LY->allSensitive = false;
          ;}
    break;

  case 47:
#line 646 "vlpYacc.y"
    {
             YYTRACE("module_item: task");
          ;}
    break;

  case 48:
#line 650 "vlpYacc.y"
    {
             YYTRACE("module_item: function");
          ;}
    break;

  case 49:
#line 654 "vlpYacc.y"
    {
             YYTRACE("description: define");
             (yyval.basePtr) = NULL;
          ;}
    break;

  case 50:
#line 902 "vlpYacc.y"
    {
             LY->scopeState = TASK;
	     if (LY->modulePtr->lintTF(*(yyvsp[(2) - (2)].stringPtr)) == true) {//linting
                cerr << "Error1 : The identifier " << *(yyvsp[(2) - (2)].stringPtr) << " at line "  << LY->lineNo 
                     << " has been declared"<< endl;
                exit(0);
             }//ony lint this, the function implement can be described later        
             LY->taskPtr = new VlpTask(*(yyvsp[(2) - (2)].stringPtr));
	  ;}
    break;

  case 51:
#line 913 "vlpYacc.y"
    {
             YYTRACE("YYTASK YYID ';' tf_declaration_clr statement_opt YYENDTASK");
             LY->taskPtr->setStatement((yyvsp[(6) - (7)].basePtr));
             LY->scopeState = MODULE;
             LY->taskPtr = NULL;
          ;}
    break;

  case 52:
#line 923 "vlpYacc.y"
    {
	     if (LY->modulePtr->lintTF(*(yyvsp[(3) - (3)].stringPtr)) == true) {//linting
                cerr << "Error2 : The identifier " << *(yyvsp[(3) - (3)].stringPtr) << " at line "  << LY->lineNo 
                     << " has been declared"<< endl;
                exit(0);
             }//ony lint this, the function implement can be described later        
             VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(3) - (3)].stringPtr)), LY->msbPtr, LY->lsbPtr);
             BaseModule :: setCDFGNode(tmp);
             LY->functionPtr = new VlpFunction(LY->isInteger, tmp);
             LY->modulePtr->setFunction(*(yyvsp[(3) - (3)].stringPtr), LY->functionPtr);

             VlpPortNode*  target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
             target->setIOType(output);
             if (LY->isInteger == true)
                target->setNetType(integer);
             LY->functionPtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);

             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->scopeState = FUNCTION;
             LY->isInteger = false;
	  ;}
    break;

  case 53:
#line 947 "vlpYacc.y"
    {
             YYTRACE("YYFUNCTION range_or_type_opt YYID ';' tf_declaration_eclr statement_opt YYENDFUNCTION");
             LY->functionPtr->setStatement((yyvsp[(7) - (8)].basePtr));
             LY->scopeState = MODULE;
             LY->functionPtr = NULL;
          ;}
    break;

  case 54:
#line 957 "vlpYacc.y"
    {
             YYTRACE("range_or_type_opt:");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
          ;}
    break;

  case 55:
#line 963 "vlpYacc.y"
    {
             YYTRACE("range_or_type_opt: range_or_type");
          ;}
    break;

  case 56:
#line 970 "vlpYacc.y"
    {
             YYTRACE("range_or_type: range");
             LY->isInteger = false;
          ;}
    break;

  case 57:
#line 975 "vlpYacc.y"
    {
             YYTRACE("range_or_type: YYINTEGER");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->isInteger = true;
          ;}
    break;

  case 58:
#line 989 "vlpYacc.y"
    {
             YYTRACE("tf_declaration_clr:");
          ;}
    break;

  case 59:
#line 993 "vlpYacc.y"
    {
             YYTRACE("tf_declaration_clr: tf_declaration_clr tf_declaration");
          ;}
    break;

  case 60:
#line 1000 "vlpYacc.y"
    {
             YYTRACE("tf_declaration_eclr: tf_declaration");
          ;}
    break;

  case 61:
#line 1004 "vlpYacc.y"
    {
             YYTRACE("tf_declaration_eclr: tf_decalration_eclr tf_declaration");
          ;}
    break;

  case 62:
#line 1011 "vlpYacc.y"
    {
             YYTRACE("tf_declaration: parameter_decalration");
          ;}
    break;

  case 63:
#line 1015 "vlpYacc.y"
    {
             YYTRACE("tf_declaration: input_declaration");
          ;}
    break;

  case 64:
#line 1019 "vlpYacc.y"
    {
             YYTRACE("tf_declaration: output_declaration");
             //function is not allow the output statement ==>lintting
             if (LY->scopeState == FUNCTION) {
                cerr << "Syntax Error3 : Can't have the output " 
                     << "statement in the \"function\" at line " << LY->lineNo << endl;
                exit(0);
             }                
          ;}
    break;

  case 65:
#line 1029 "vlpYacc.y"
    {
             YYTRACE("tf_declaration: inout_declaration");
             //function is not allow the output statement ==>lintting
             if (LY->scopeState == FUNCTION) {
                cerr << "Syntax Error4 : Can't have the inout " 
                     << "statement in the \"function\" at line " << LY->lineNo << endl;
                exit(0);
             }                
          ;}
    break;

  case 66:
#line 1039 "vlpYacc.y"
    {
             YYTRACE("tf_declaration: reg_declaration");
             //local variables
          ;}
    break;

  case 67:
#line 1048 "vlpYacc.y"
    {
             YYTRACE("tf_declaration: integer_declaration");
          ;}
    break;

  case 68:
#line 1064 "vlpYacc.y"
    { (LY->state) = PARAM; ;}
    break;

  case 69:
#line 1065 "vlpYacc.y"
    {                                                           //ignore the syntax "range_opt"
             YYTRACE("parameter_declaration: YYPARAMETER assignment_list ';'");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          ;}
    break;

  case 70:
#line 1075 "vlpYacc.y"
    {
             (LY->state) = IO;
             LY->tempIOType = input;
          ;}
    break;

  case 71:
#line 1080 "vlpYacc.y"
    {
             YYTRACE("input_declaration: YYINPUT range_opt variable_list ';'");
             LY->tempIOType = unspecIO;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          ;}
    break;

  case 72:
#line 1091 "vlpYacc.y"
    {
             (LY->state) = IO;
             LY->tempIOType = output;
          ;}
    break;

  case 73:
#line 1096 "vlpYacc.y"
    {
             YYTRACE("output_declaration: YYOUTPUT range_opt variable_list ';'");
             LY->tempIOType = unspecIO;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          ;}
    break;

  case 74:
#line 1107 "vlpYacc.y"
    {
             (LY->state) = IO;
             LY->tempIOType = inout;
          ;}
    break;

  case 75:
#line 1112 "vlpYacc.y"
    {
             YYTRACE("inout_declaration: YYINOUT range_opt variable_list ';'");
             LY->tempIOType = unspecIO;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL; 
             (LY->state) = S_NULL;
          ;}
    break;

  case 76:
#line 1123 "vlpYacc.y"
    {                                                                           //the condition
             YYTRACE("net_declaration: nettype drive_strength_opt expandrange_opt delay_opt assignment_list ';'");
             (LY->tempNetType) = unspecNet;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->hStr = unspecStr;
             LY->lStr = unspecStr;
             (LY->state) = S_NULL;
             (yyval.assignPtr) = new VlpAssignNode( (yyvsp[(4) - (5)].basePtr), NULL);
             BaseModule :: setCDFGNode((yyval.assignPtr));
          ;}
    break;

  case 77:
#line 1135 "vlpYacc.y"
    {
             YYTRACE("net_declaration: nettype drive_strength_opt expandrange_opt delay_opt variable_list ';'");
             (LY->tempNetType) = unspecNet;
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             LY->hStr = unspecStr;
             LY->lStr = unspecStr;
             (LY->state) = S_NULL;
             (yyval.assignPtr) = NULL;
          ;}
    break;

  case 78:
#line 1154 "vlpYacc.y"
    {
             YYTRACE("nettype: YYSWIRE");
             (LY->tempNetType) = swire;
             (LY->state) = NET;
          ;}
    break;

  case 79:
#line 1160 "vlpYacc.y"
    {
             YYTRACE("nettype: YYWIRE");
             (LY->tempNetType) = wire;
             (LY->state) = NET;
          ;}
    break;

  case 80:
#line 1166 "vlpYacc.y"
    {
             YYTRACE("nettype: YYTRI");
             (LY->tempNetType) = tri;
             (LY->state) = NET;
          ;}
    break;

  case 81:
#line 1172 "vlpYacc.y"
    {
             YYTRACE("nettype: YYTRI1");
             (LY->tempNetType) = tri1;
             (LY->state) = NET;
          ;}
    break;

  case 82:
#line 1178 "vlpYacc.y"
    {
             YYTRACE("nettype: YYSUPPLY0");
             (LY->tempNetType) = supply0Net;
             (LY->state) = NET;
          ;}
    break;

  case 83:
#line 1184 "vlpYacc.y"
    {
             YYTRACE("nettype: YYWAND");
             (LY->tempNetType) = wand;
             (LY->state) = NET;
	  ;}
    break;

  case 84:
#line 1190 "vlpYacc.y"
    {
	     YYTRACE("nettype: YYTRIAND");
             (LY->tempNetType) = triand;
             (LY->state) = NET;
	  ;}
    break;

  case 85:
#line 1196 "vlpYacc.y"
    {
	     YYTRACE("nettype: YYTRI0");
             (LY->tempNetType) = tri0;
             (LY->state) = NET;
	  ;}
    break;

  case 86:
#line 1202 "vlpYacc.y"
    {
	     YYTRACE("nettype: YYSUPPLY1");
             (LY->tempNetType) = supply1Net;
             (LY->state) = NET;
	  ;}
    break;

  case 87:
#line 1208 "vlpYacc.y"
    {
             YYTRACE("nettype: YYWOR");
             (LY->tempNetType) = wor;
             (LY->state) = NET;
          ;}
    break;

  case 88:
#line 1214 "vlpYacc.y"
    {
             YYTRACE("nettype: YYTRIOR");
             (LY->tempNetType) = trior;
             (LY->state) = NET;
          ;}
    break;

  case 89:
#line 1227 "vlpYacc.y"
    {
             YYTRACE("expandrange_opt:");
          ;}
    break;

  case 90:
#line 1231 "vlpYacc.y"
    {
             YYTRACE("expandrange_opt: expandrange");
          ;}
    break;

  case 91:
#line 1238 "vlpYacc.y"
    {
             YYTRACE("expandrange: range");
          ;}
    break;

  case 92:
#line 1253 "vlpYacc.y"
    {
             (LY->state) = REG; 
             YYTRACE("reg_declaration: YYREG range_opt");
          ;}
    break;

  case 93:
#line 1258 "vlpYacc.y"
    {
             YYTRACE("reg_declaration: YYREG range_opt register_variable_list ';'");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          ;}
    break;

  case 94:
#line 1279 "vlpYacc.y"
    {
             (LY->state) = INT;
          ;}
    break;

  case 95:
#line 1283 "vlpYacc.y"
    {
             YYTRACE("integer_declaration: YYINTEGER register_variable_list ';'");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
             (LY->state) = S_NULL;
          ;}
    break;

  case 96:
#line 1306 "vlpYacc.y"
    { (LY->state) = ASSIGN; ;}
    break;

  case 97:
#line 1307 "vlpYacc.y"
    {
             YYTRACE("continuous_assign: YYASSIGN drive_strength_opt delay_opt assignment_list ';'");
             (yyval.assignPtr) = new VlpAssignNode( (yyvsp[(3) - (4)].basePtr), NULL);
             BaseModule :: setCDFGNode((yyval.assignPtr));
             (LY->state) = S_NULL;
          ;}
    break;

  case 98:
#line 1316 "vlpYacc.y"
    { (LY->state) = PARAM_NOL; ;}
    break;

  case 99:
#line 1317 "vlpYacc.y"
    {
             YYTRACE("parameter_override: YYDEFPARAM assign_list ';'");
             (LY->state) = S_NULL;         
          ;}
    break;

  case 100:
#line 1325 "vlpYacc.y"
    {
             YYTRACE("variable_list: identifier");
             VlpPortNode* target = NULL;
             if ((LY->state) == IO)//input, output, inout
             {   
                if (LY->scopeState == MODULE) {
                   if (LY->modulePtr->getPort(*(yyvsp[(1) - (1)].stringPtr), target)) //exist in map(declared)
                      target->setIOType(LY->tempIOType);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      LY->modulePtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                }
                else if (LY->scopeState == FUNCTION) {
                   if (LY->functionPtr->lintPort(*(yyvsp[(1) - (1)].stringPtr))) { //exist in map(declared)
                      cerr << "Error5 : re-declare identifier error at line " << LY->lineNo << endl;
                      exit(0);
                   }             
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      //tempIOType must be "input", this will be lint in the "tf_declaration"
                      LY->functionPtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                   
                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr)), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   LY->functionPtr->setInput(tmp);
                }
                else  {//TASK condition
                   if (LY->taskPtr->lintIO(*(yyvsp[(1) - (1)].stringPtr))) { //exist in map(declared)
                      cerr << "Error6 : re-declare identifier error at line " << LY->lineNo << endl;
                      exit(0);
                   }
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      //LY->tempIOType must be "input", "output", or "inout" this will be lint in the "tf_declaration"
                      LY->taskPtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                   
                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr)), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   LY->taskPtr->setIO(tmp);
                }
             } 
             else if ((LY->state) == NET)//nettype
             {
                //task and function can't declare nettype
                assert (LY->scopeState == MODULE); 
                if (LY->modulePtr->getPort(*(yyvsp[(1) - (1)].stringPtr), target)) { //exist in map(declared)
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                }
                else { // new in map
                   target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                   LY->modulePtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                }                 
             }
             else
                cerr << "Error7 : variable_list: identifier at line " << LY->lineNo << endl;
          ;}
    break;

  case 101:
#line 1391 "vlpYacc.y"
    {
             YYTRACE("variable_list: variable_list ',' identifier");
             VlpPortNode* target = NULL;
             if ((LY->state) == IO)//input, output, inout
             {
                if (LY->scopeState == MODULE) {
                   if (LY->modulePtr->getPort(*(yyvsp[(3) - (3)].stringPtr), target)) //exist in map(declared)
                      target->setIOType(LY->tempIOType);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      LY->modulePtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);
                   }
                }
                else if (LY->scopeState == FUNCTION) {
                   if (LY->functionPtr->getPort(*(yyvsp[(3) - (3)].stringPtr), target)) //exist in map(declared)
                      target->setIOType(LY->tempIOType);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      LY->functionPtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);
                   }

                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(3) - (3)].stringPtr)), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   //LY->tempIOType must be "input", this will be lint in the "tf_declaration"
                   LY->functionPtr->setInput(tmp);                   
                }
                else  {//TASK condition
                   if (LY->taskPtr->lintIO(*(yyvsp[(3) - (3)].stringPtr))) { //exist in map(declared)
                      cerr << "Error8 : re-declare identifier error at line " << LY->lineNo << endl;
                      exit(0);
                   }
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setIOType(LY->tempIOType);
                      //LY->tempIOType must be "input", "output", or "inout" this will be lint in the "tf_declaration"
                      LY->taskPtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);
                   }
                   
                   VlpSignalNode* tmp = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(3) - (3)].stringPtr)), LY->msbPtr, LY->lsbPtr);
                   BaseModule :: setCDFGNode(tmp);
                   LY->taskPtr->setIO(tmp);
                }
             }
             else if ((LY->state) == NET)//nettype
             {
                assert (LY->scopeState == MODULE); 
                if (LY->modulePtr->getPort(*(yyvsp[(3) - (3)].stringPtr), target)) {//exist in map(declared)
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                }
                else { // new in map
                   target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                   LY->modulePtr->setPort(*(yyvsp[(3) - (3)].stringPtr), target);
                }      
             }
             else
                cerr << "Error9 : variable_list: variable_list, identifier at line " << LY->lineNo << endl;
          ;}
    break;

  case 102:
#line 1457 "vlpYacc.y"
    {
             YYTRACE("register_variable_list: register_variable");
          ;}
    break;

  case 103:
#line 1461 "vlpYacc.y"
    {
             YYTRACE("register_variable_list: register_variable_list ',' register_variable");
          ;}
    break;

  case 104:
#line 1468 "vlpYacc.y"
    {
             YYTRACE("register_variable: name_of_register");
             VlpPortNode* target = NULL;
             if ((LY->state) == REG)//reg
             {  
                if (LY->scopeState == MODULE)
                {
                   if (LY->modulePtr->getPort(*(yyvsp[(1) - (1)].stringPtr), target))  //exist in map(declared)
                      target->setNetType(reg);
                   else { // new in map
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(reg);
                      LY->modulePtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                }
                else if (LY->scopeState == FUNCTION)
                {
                   if (LY->functionPtr->getPort(*(yyvsp[(1) - (1)].stringPtr), target))
                      target->setNetType(reg);
                   else {
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(reg);
                      LY->functionPtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                }
                else //TASK
                {
                   if (LY->taskPtr->getPort(*(yyvsp[(1) - (1)].stringPtr), target))
                      target->setNetType(reg);
                   else {
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(reg);
                      LY->taskPtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                }
             }
             else if ((LY->state) == INT)//integer
             {  
                if (LY->scopeState == MODULE)  
                {
                   if (LY->modulePtr->getPort(*(yyvsp[(1) - (1)].stringPtr), target)) {  //exist in map(declared)
                      target->setNetType(integer);
                      cerr << "Error10 : re-declare integer error at line " << LY->lineNo << endl;
                   }
                   else { // new in map
                      target = new VlpPortNode(NULL, NULL);
                      target->setNetType(integer);
                      LY->modulePtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                }
                else if (LY->scopeState == FUNCTION)
                {
                   if (LY->functionPtr->lintPort(*(yyvsp[(1) - (1)].stringPtr))) {
                      cerr << "Error11 : re-declare integer error at line " << LY->lineNo << endl;
                   }
                   else { // new in map
                      target = new VlpPortNode(NULL, NULL);
                      target->setNetType(integer);
                      LY->functionPtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                }
                else { //TASK 
                   if (LY->taskPtr->lintPort(*(yyvsp[(1) - (1)].stringPtr))) {
                      cerr << "Error12 : re-declare integer error at line " << LY->lineNo << endl;
                   }
                   else {
                      target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                      target->setNetType(integer);
                      LY->taskPtr->setPort(*(yyvsp[(1) - (1)].stringPtr), target);
                   }
                }
             }
             else
                cerr << "Error13 : register_variable: name_of_register at line " << LY->lineNo << endl;
          ;}
    break;

  case 105:
#line 1544 "vlpYacc.y"
    {                                                 //ref. 5-11 in Verilog book
             YYTRACE("register_variable: name_of_register '[' expression ':' expression ']'");
             VlpPortNode* target = NULL;
             if (LY->modulePtr->getPort(*(yyvsp[(1) - (6)].stringPtr), target))
                cerr << "Error14 : re-declare Array error at line " << LY->lineNo << endl;
             else
                BaseModule :: setName(*(yyvsp[(1) - (6)].stringPtr));
             if ((LY->state) == REG) {
                VlpMemAry* memAryPtr = new VlpMemAry(*(yyvsp[(1) - (6)].stringPtr), LY->msbPtr, LY->lsbPtr, (yyvsp[(3) - (6)].basePtr), (yyvsp[(5) - (6)].basePtr));
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setMemAry(memAryPtr);
                else if (LY->scopeState == FUNCTION)    
                   LY->functionPtr->setMemAry(memAryPtr);
                else //TASK                                       
                   LY->taskPtr->setMemAry(memAryPtr);
             }
             else if ((LY->state) == INT) {
                VlpPortNode* target = new VlpPortNode((yyvsp[(3) - (6)].basePtr), (yyvsp[(5) - (6)].basePtr));
                target->setNetType(integer);
                if (LY->scopeState == MODULE)                
                   LY->modulePtr->setPort(*(yyvsp[(1) - (6)].stringPtr), target);
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setPort(*(yyvsp[(1) - (6)].stringPtr), target);
                else  //TASK  
                   LY->taskPtr->setPort(*(yyvsp[(1) - (6)].stringPtr), target);
             }                                           
             else
                cerr << "Error15 : name_of_register [ expression : expression ] at line " << LY->lineNo << endl;
          ;}
    break;

  case 106:
#line 1574 "vlpYacc.y"
    {
             YYTRACE("register_variable: name_of_register '[' YYDEF_ID ']'");

             VlpPortNode* target = NULL;
             if (LY->modulePtr->getPort(*(yyvsp[(1) - (4)].stringPtr), target))
                cerr << "Error16 : re-declare Array error at line " << LY->lineNo << endl;
             else
                BaseModule :: setName(*(yyvsp[(1) - (4)].stringPtr));

             const VlpBaseNode* tmp;
             if (LY->defineMap.getData(*(yyvsp[(3) - (4)].stringPtr) ,tmp) == false)
             {
                cerr << "Error17 : the macro " << *(yyvsp[(3) - (4)].stringPtr) << " isn't defined at line " << LY->lineNo << endl;
                exit(0);
             }
             assert (tmp->getNodeClass() == NODECLASS_CONNECT);// Ex `define bus 7:0
                
             VlpBaseNode* msb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getFirst()  ); 
             VlpBaseNode* lsb = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getSecond() ); 

             if ((LY->state) == REG) {
                VlpMemAry* memAryPtr = new VlpMemAry(*(yyvsp[(1) - (4)].stringPtr), LY->msbPtr, LY->lsbPtr, msb, lsb);
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setMemAry(memAryPtr);
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setMemAry(memAryPtr);
                else  // TASK   
                   LY->taskPtr->setMemAry(memAryPtr);
             }
             else if ((LY->state) == INT)
             {
                VlpPortNode* target = new VlpPortNode(msb, lsb);
                target->setNetType(integer);
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setPort(*(yyvsp[(1) - (4)].stringPtr), target);
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setPort(*(yyvsp[(1) - (4)].stringPtr), target);
                else //TASK    
                   LY->taskPtr->setPort(*(yyvsp[(1) - (4)].stringPtr), target);
             }
             else
                cerr << "Error18 : name_of_register [ expression : expression ] at line " << LY->lineNo << endl;
          ;}
    break;

  case 107:
#line 1621 "vlpYacc.y"
    {
             YYTRACE("name_of_register: YYID");   
             (yyval.stringPtr) = (yyvsp[(1) - (1)].stringPtr);
          ;}
    break;

  case 108:
#line 1674 "vlpYacc.y"
    {
             YYTRACE("drive_strength_opt:");
             LY->hStr = unspecStr;
             LY->lStr = unspecStr;
          ;}
    break;

  case 109:
#line 1680 "vlpYacc.y"
    {
             YYTRACE("drive_strength_opt: drive_strength");
          ;}
    break;

  case 110:
#line 1687 "vlpYacc.y"
    {
             YYTRACE("drive_strength: '(' strength0 ',' strength1 ')'");
             LY->hStr = (DriveStr)(yyvsp[(2) - (5)].ival);
             LY->lStr = (DriveStr)(yyvsp[(4) - (5)].ival);             
          ;}
    break;

  case 111:
#line 1693 "vlpYacc.y"
    {
             YYTRACE("drive_strength: '(' strength1 ',' strength0 ')'");
             LY->hStr = (DriveStr)(yyvsp[(2) - (5)].ival);
             LY->lStr = (DriveStr)(yyvsp[(4) - (5)].ival);             
          ;}
    break;

  case 112:
#line 1702 "vlpYacc.y"
    {
             YYTRACE("strength0: YYSUPPLY0");
             (yyval.ival) = supply0;           
          ;}
    break;

  case 113:
#line 1707 "vlpYacc.y"
    {
             YYTRACE("strength0: YYSTRONG0");
             (yyval.ival) = strong0;
          ;}
    break;

  case 114:
#line 1712 "vlpYacc.y"
    {
             YYTRACE("strength0: YYPULL0");
             (yyval.ival) = pull0;
          ;}
    break;

  case 115:
#line 1717 "vlpYacc.y"
    {
             YYTRACE("strength0: YYWEAK0");
             (yyval.ival) = weak0;
          ;}
    break;

  case 116:
#line 1722 "vlpYacc.y"
    {
             YYTRACE("strength0: YYHIGHZ0");
             (yyval.ival) = highz0;
          ;}
    break;

  case 117:
#line 1730 "vlpYacc.y"
    {
             YYTRACE("strength1: YYSUPPLY1");
             (yyval.ival) = supply1;
          ;}
    break;

  case 118:
#line 1735 "vlpYacc.y"
    {
             YYTRACE("strength1: YYSTRONG1");
             (yyval.ival) = strong1;
          ;}
    break;

  case 119:
#line 1740 "vlpYacc.y"
    {
             YYTRACE("strength1: YYPULL1");
             (yyval.ival) = pull1;
          ;}
    break;

  case 120:
#line 1745 "vlpYacc.y"
    {
             YYTRACE("strength1: YYWEAK1");
             (yyval.ival) = weak1;
          ;}
    break;

  case 121:
#line 1750 "vlpYacc.y"
    {
             YYTRACE("strength1: YYHIGHZ1");
             (yyval.ival) = highz1;
          ;}
    break;

  case 122:
#line 1758 "vlpYacc.y"
    {
             YYTRACE("range_opt:");
             LY->msbPtr = NULL;
             LY->lsbPtr = NULL;
          ;}
    break;

  case 123:
#line 1764 "vlpYacc.y"
    {
             YYTRACE("range_opt: range");
          ;}
    break;

  case 124:
#line 1771 "vlpYacc.y"
    {
             YYTRACE("range: '[' expression ':' expression ']'");
             LY->msbPtr = (yyvsp[(2) - (5)].basePtr);
             LY->lsbPtr = (yyvsp[(4) - (5)].basePtr);
          ;}
    break;

  case 125:
#line 1777 "vlpYacc.y"
    {
             YYTRACE("range: '[' YYDEF_ID ']'");
             const VlpBaseNode* tmp;
             if (LY->defineMap.getData(*(yyvsp[(2) - (3)].stringPtr) ,tmp) == false)
             {
                cerr << "Error19 : the macro " << *(yyvsp[(2) - (3)].stringPtr) << " isn't defined at line " << LY->lineNo << endl;
                exit(0);
             }
             assert (tmp->getNodeClass() == NODECLASS_CONNECT);// Ex `define bus 7:0
                
             LY->msbPtr = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getFirst()  ); 
             LY->lsbPtr = const_cast<VlpBaseNode*>( ((VlpConnectNode*)(tmp))->getSecond() ); 
          ;}
    break;

  case 126:
#line 1799 "vlpYacc.y"
    {//linking
             YYTRACE("assignment_list: assignment");
             if ((LY->state) != PARAM && (LY->state) != PARAM_NOL) {
                LY->nodeHeadPtr1 = (yyvsp[(1) - (1)].basePtr);
                LY->nodeTailPtr1 = (yyvsp[(1) - (1)].basePtr);
                (yyval.basePtr) = LY->nodeHeadPtr1;
             }
          ;}
    break;

  case 127:
#line 1808 "vlpYacc.y"
    {
             YYTRACE("assignment_list: assignment_list ',' assignment");
             if ((LY->state) != PARAM && (LY->state) != PARAM_NOL) {
                LY->nodeTailPtr1 -> setNext((yyvsp[(3) - (3)].basePtr));
                LY->nodeTailPtr1 = (yyvsp[(3) - (3)].basePtr);       
             }      
          ;}
    break;

  case 128:
#line 1821 "vlpYacc.y"
    {
             YYTRACE("gate_instantiation: gatetype drive_delay_clr gate_instance_list ';'");
             LY->gateType = 0;
          ;}
    break;

  case 129:
#line 1854 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYAND");
             LY->gateType = 1;
	  ;}
    break;

  case 130:
#line 1859 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYNAND");
             LY->gateType = 2;
	  ;}
    break;

  case 131:
#line 1864 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYOR");
             LY->gateType = 3;
	  ;}
    break;

  case 132:
#line 1869 "vlpYacc.y"
    {
             YYTRACE("gatetype: YYNOR");
             LY->gateType = 4;
	  ;}
    break;

  case 133:
#line 1874 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYXOR");
             LY->gateType = 5;
	  ;}
    break;

  case 134:
#line 1879 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYXNOR");
             LY->gateType = 6;
	  ;}
    break;

  case 135:
#line 1884 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYBUF");
             LY->gateType = 7;
	  ;}
    break;

  case 136:
#line 1889 "vlpYacc.y"
    {
             YYTRACE("gatetype: YYBIFIF0");
             LY->gateType = 8;
	  ;}
    break;

  case 137:
#line 1894 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYBIFIF1");
             LY->gateType = 9;
	  ;}
    break;

  case 138:
#line 1899 "vlpYacc.y"
    {
	     YYTRACE("gatetype: YYNOT");
             LY->gateType = 10;
	  ;}
    break;

  case 139:
#line 1971 "vlpYacc.y"
    {
             YYTRACE("gate_instance_list: gate_instance");
             (yyvsp[(1) - (1)].primitivePtr)->setKindID(LY->gateType);
             LY->modulePtr->setPrim((yyvsp[(1) - (1)].primitivePtr));            
          ;}
    break;

  case 140:
#line 1977 "vlpYacc.y"
    {
             YYTRACE("gate_instance_list: gate_instance_list ',' gate_instance");
             (yyvsp[(3) - (3)].primitivePtr)->setKindID(LY->gateType);
             LY->modulePtr->setPrim((yyvsp[(3) - (3)].primitivePtr));            
          ;}
    break;

  case 141:
#line 1986 "vlpYacc.y"
    {
             YYTRACE("gate_instance: '(' terminal_list ')'");
             (yyval.primitivePtr) = (yyvsp[(2) - (3)].primitivePtr);
          ;}
    break;

  case 142:
#line 1991 "vlpYacc.y"
    {
             YYTRACE("gate_instance: name_of_gate_instance '(' terminal_list ')'");
             (yyvsp[(3) - (4)].primitivePtr)->setNameID(BaseModule :: setName(*(yyvsp[(1) - (4)].stringPtr)));
             (yyval.primitivePtr) = (yyvsp[(3) - (4)].primitivePtr);
          ;}
    break;

  case 143:
#line 2000 "vlpYacc.y"
    {
             YYTRACE("name_of_gate_instance: YYID");
             (yyval.stringPtr) = (yyvsp[(1) - (1)].stringPtr);
          ;}
    break;

  case 144:
#line 2008 "vlpYacc.y"
    {
             YYTRACE("terminal_list: terminal");
             (yyval.primitivePtr) = new VlpPrimitive;
             (yyval.primitivePtr)->setPosArr((yyvsp[(1) - (1)].basePtr)); 
          ;}
    break;

  case 145:
#line 2014 "vlpYacc.y"
    {
             YYTRACE("terminal_list: terminal_list ',' terminal");
             (yyvsp[(1) - (3)].primitivePtr)->setPosArr((yyvsp[(3) - (3)].basePtr));
             (yyval.primitivePtr) = (yyvsp[(1) - (3)].primitivePtr);
          ;}
    break;

  case 146:
#line 2023 "vlpYacc.y"
    {
             YYTRACE("terminal: expression");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
          ;}
    break;

  case 147:
#line 2034 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_instantiation: name_of_module_or_primitive module_or_primitive_option_clr module_or_primitive_instance_list ';'");
             LY->UDM_No = -1;
             LY->array.clear();
          ;}
    break;

  case 148:
#line 2043 "vlpYacc.y"
    {
             YYTRACE("name_of_module_or_primitive: YYID");
             LY->UDM_No = BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr));
          ;}
    break;

  case 149:
#line 2048 "vlpYacc.y"
    {
             YYTRACE("name_of_module_or_primitive: YYDEF_ID");
             const VlpBaseNode* tmp;
             if (LY->defineMap.getData(*(yyvsp[(1) - (1)].stringPtr) ,tmp) == false)
             {
                cerr << "Error36 : the macro " << *(yyvsp[(1) - (1)].stringPtr) << " isn't defined at line " << LY->lineNo << endl;
                exit(0);
             }
             if (tmp->getNodeClass() != NODECLASS_SIGNAL) {
                cerr << "Error37 : Error definite of macro name using in instance name" << endl;
                exit(0);
             }
             else
                LY->UDM_No = (static_cast<const VlpSignalNode*>(tmp))->getNameId();
          ;}
    break;

  case 150:
#line 2069 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_option_clr:");
          ;}
    break;

  case 151:
#line 2073 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_option_clr: module_or_primitive_option_clr module_or_primitive_option");
          ;}
    break;

  case 152:
#line 2088 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_option: delay");
          ;}
    break;

  case 153:
#line 2095 "vlpYacc.y"
    {
	     YYTRACE("delay_or_parameter_value_assignment: '#' YYINUMBER");
             //ignore!!
	  ;}
    break;

  case 154:
#line 2100 "vlpYacc.y"
    {
	     YYTRACE("delay_or_parameter_value_assignment: '#' YYRNUMBER");
             //ignore!!
	  ;}
    break;

  case 155:
#line 2105 "vlpYacc.y"
    {
	     YYTRACE("delay_or_parameter_value_assignment: '#' identifier");
             //ignore!!
	  ;}
    break;

  case 156:
#line 2110 "vlpYacc.y"
    {
	     YYTRACE("delay_or_parameter_value_assignment: '#' '(' mintypmax_expression_list ')'");
	  ;}
    break;

  case 157:
#line 2118 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_instance_list: module_or_primitive_instance");
             (yyvsp[(1) - (1)].instancePtr)->setModuleID(LY->UDM_No);

             if (LY->array.size() != 0) {
                for (unsigned i = 0; i < LY->array.size(); ++i) 
                   (yyvsp[(1) - (1)].instancePtr)->setPOL(LY->array[i]);
             }
             LY->modulePtr->setInst((yyvsp[(1) - (1)].instancePtr));            
          ;}
    break;

  case 158:
#line 2129 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_instance_list: module_or_primitive_instance_list ',' module_or_primitive_instance");
             (yyvsp[(3) - (3)].instancePtr)->setModuleID(LY->UDM_No);

             if (LY->array.size() != 0) {
                for (unsigned i = 0; i < LY->array.size(); ++i)
                   (yyvsp[(3) - (3)].instancePtr)->setPOL(LY->array[i]);
             }
             LY->modulePtr->setInst((yyvsp[(3) - (3)].instancePtr));            
          ;}
    break;

  case 159:
#line 2146 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_instance: '(' module_connection_list ')'");
             (yyval.instancePtr) = (yyvsp[(2) - (3)].instancePtr);
          ;}
    break;

  case 160:
#line 2150 "vlpYacc.y"
    { LY->tempStr2 = *(yyvsp[(1) - (1)].stringPtr); ;}
    break;

  case 161:
#line 2151 "vlpYacc.y"
    {
             YYTRACE("module_or_primitive_instance: '(' module_connection_list ')'");
             (yyvsp[(4) - (5)].instancePtr)->setName(LY->tempStr2);
             LY->tempStr2 = "";
             (yyval.instancePtr) = (yyvsp[(4) - (5)].instancePtr);
          ;}
    break;

  case 162:
#line 2161 "vlpYacc.y"
    {
             YYTRACE("module_connection_list: module_port_connection_list");
             (yyvsp[(1) - (1)].instancePtr)->setPosMap();
             (yyval.instancePtr) = (yyvsp[(1) - (1)].instancePtr);
          ;}
    break;

  case 163:
#line 2167 "vlpYacc.y"
    {
             YYTRACE("module_connection_list: named_port_connection_list");
             (yyvsp[(1) - (1)].instancePtr)->setNameMap();
             (yyval.instancePtr) = (yyvsp[(1) - (1)].instancePtr);
          ;}
    break;

  case 164:
#line 2176 "vlpYacc.y"
    {
             YYTRACE("module_port_connection_list: module_port_connection");
             (yyval.instancePtr) = new VlpInstance;
             (yyval.instancePtr)->setPosArr((yyvsp[(1) - (1)].basePtr));               
          ;}
    break;

  case 165:
#line 2182 "vlpYacc.y"
    {
             YYTRACE("module_port_connection_list: module_port_connection_list ',' module_port_connection");
             (yyval.instancePtr)->setPosArr((yyvsp[(3) - (3)].basePtr));
          ;}
    break;

  case 166:
#line 2190 "vlpYacc.y"
    {
             YYTRACE("named_port_connection_list: named_port_connection");
             (yyval.instancePtr) = new VlpInstance;
             (yyval.instancePtr)->setPosArr((yyvsp[(1) - (1)].basePtr));
             (yyval.instancePtr)->setNameArr(BaseModule :: setName(LY->tempStr1));
             LY->tempStr1 = "";
          ;}
    break;

  case 167:
#line 2198 "vlpYacc.y"
    {
             YYTRACE("named_port_connection_list: named_port_connection_list ',' name_port_connection");
             (yyvsp[(1) - (3)].instancePtr)->setPosArr((yyvsp[(3) - (3)].basePtr));
             (yyvsp[(1) - (3)].instancePtr)->setNameArr(BaseModule :: setName(LY->tempStr1));
             LY->tempStr1 = "";
             (yyval.instancePtr) = (yyvsp[(1) - (3)].instancePtr);
          ;}
    break;

  case 168:
#line 2209 "vlpYacc.y"
    {
             YYTRACE("module_port_connection:");
             (yyval.basePtr) = NULL;
          ;}
    break;

  case 169:
#line 2214 "vlpYacc.y"
    {
             YYTRACE("module_port_connection: expression");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
          ;}
    break;

  case 170:
#line 2221 "vlpYacc.y"
    { LY->tempStr1 = *(yyvsp[(2) - (2)].stringPtr); ;}
    break;

  case 171:
#line 2222 "vlpYacc.y"
    {
             YYTRACE("named_port_connection: '.' identifier '(' connect_port ')'");
             (yyval.basePtr) = (yyvsp[(5) - (6)].basePtr);
          ;}
    break;

  case 172:
#line 2230 "vlpYacc.y"
    {
             YYTRACE("connect_port:");
             (yyval.basePtr) = NULL;
          ;}
    break;

  case 173:
#line 2235 "vlpYacc.y"
    {
             YYTRACE("connect_port: expression");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
          ;}
    break;

  case 174:
#line 2252 "vlpYacc.y"
    {
             if ((LY->state) == EVENT)
             {
                YYTRACE("always_statement: YYALWAYS statement");
                (yyval.alwaysPtr) = new VlpAlwaysNode(LY->alwaysEventPtr, (yyvsp[(2) - (2)].basePtr), NULL);
                BaseModule :: setCDFGNode((yyval.alwaysPtr));
                (LY->state) = S_NULL;
             }
             else
                cerr << "No supply syntax in always_statement at line " << LY->lineNo << endl;
          ;}
    break;

  case 175:
#line 2267 "vlpYacc.y"
    {
             YYTRACE("statement_opt:");
             (yyval.basePtr) = NULL;
          ;}
    break;

  case 176:
#line 2272 "vlpYacc.y"
    {
             YYTRACE("statement_opt: statement");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
          ;}
    break;

  case 177:
#line 2280 "vlpYacc.y"
    {
             YYTRACE("statement_clr:");
             (yyval.basePtr) = NULL;
          ;}
    break;

  case 178:
#line 2285 "vlpYacc.y"
    {
             YYTRACE("statement_clr: statement_clr statement");
             if ((yyvsp[(1) - (2)].basePtr) == NULL)
             {
                LY->stack_s.push((yyvsp[(2) - (2)].basePtr));
                (yyval.basePtr) = (yyvsp[(2) - (2)].basePtr);
             }
             else
             {
                (yyvsp[(1) - (2)].basePtr)->setNext((yyvsp[(2) - (2)].basePtr));
                if ((yyvsp[(2) - (2)].basePtr) != NULL)
                   (yyval.basePtr) = (yyvsp[(2) - (2)].basePtr);
                else
                   (yyval.basePtr) = (yyvsp[(1) - (2)].basePtr);
                   
             }
          ;}
    break;

  case 179:
#line 2306 "vlpYacc.y"
    {  //and {lvalue '=' expression ';'} won't use this.
             YYTRACE("statement: ';'");
             (yyval.basePtr) = NULL;
          ;}
    break;

  case 180:
#line 2311 "vlpYacc.y"
    {              //                               | lvalue YYNBASSIGN delay_control expression ';'
             YYTRACE("statement: assignment ';'");
             (yyval.basePtr) = (yyvsp[(1) - (2)].basePtr);
          ;}
    break;

  case 181:
#line 2316 "vlpYacc.y"
    {
             YYTRACE("statement: YYIF '(' expression ')' statement");
             (yyval.basePtr) = new VlpIfNode((yyvsp[(3) - (5)].basePtr), (yyvsp[(5) - (5)].basePtr), NULL, NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));

             if (LY->allSensitive && (yyvsp[(3) - (5)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (5)].basePtr)))->getNameId()));
          ;}
    break;

  case 182:
#line 2325 "vlpYacc.y"
    {
             YYTRACE("statement: YYIF '(' expression ')' statement YYELSE statement");   
             (yyval.basePtr) = new VlpIfNode((yyvsp[(3) - (7)].basePtr), (yyvsp[(5) - (7)].basePtr), (yyvsp[(7) - (7)].basePtr), NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));
             
             if (LY->allSensitive && (yyvsp[(3) - (7)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (7)].basePtr)))->getNameId()));
	  ;}
    break;

  case 183:
#line 2334 "vlpYacc.y"
    {
             YYTRACE("statement: YYCASE '(' expression ')' case_item_eclr YYENDCASE");
             //VlpBaseNode* tail = LY->stack_c.top();//dummy
             LY->stack_c.pop();
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASE, (yyvsp[(3) - (7)].basePtr), LY->stack_c.top(), NULL);
             LY->stack_c.pop();
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.top());
             LY->stack_d.pop();
             tmp->setFullCase(LY->stack_d.top());
             LY->stack_d.pop();
             (yyval.basePtr) = tmp;
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();//dummy
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASE, $3, LY->stack_c.pop(), NULL);
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.pop());
             tmp->setFullCase(LY->stack_d.pop());
             $$ = tmp;     
*/        
             if (LY->allSensitive && (yyvsp[(3) - (7)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (7)].basePtr)))->getNameId()));
          ;}
    break;

  case 184:
#line 2358 "vlpYacc.y"
    {
             YYTRACE("statement: YYCASEZ '(' expression ')' case_item_eclr YYENDCASE"); 
             //VlpBaseNode* tail = LY->stack_c.top();//dummy
             LY->stack_c.pop();
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEZ, (yyvsp[(3) - (7)].basePtr), LY->stack_c.top(), NULL);
             LY->stack_c.pop();
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.top());
             LY->stack_d.pop();
             tmp->setFullCase(LY->stack_d.top());
             LY->stack_d.pop();
             (yyval.basePtr) = tmp;
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();//dummy
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEZ, $3, LY->stack_c.pop(), NULL);
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.pop());
             tmp->setFullCase(LY->stack_d.pop());
             $$ = tmp;
*/
             if (LY->allSensitive && (yyvsp[(3) - (7)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (7)].basePtr)))->getNameId()));
          ;}
    break;

  case 185:
#line 2382 "vlpYacc.y"
    {
             YYTRACE("statement: YYCASEX '(' expression ')' case_item_eclr YYENDCASE");
             //VlpBaseNode* tail = LY->stack_c.top();//dummy
             LY->stack_c.pop();
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEX, (yyvsp[(3) - (7)].basePtr), LY->stack_c.top(), NULL);
             LY->stack_c.pop();
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.top());
             LY->stack_d.pop();
             tmp->setFullCase(LY->stack_d.top());
             LY->stack_d.pop();
             (yyval.basePtr) = tmp;
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();//dummy
             VlpCaseNode* tmp = new VlpCaseNode(CASETYPE_CASEX, $3, LY->stack_c.pop(), NULL);
             BaseModule :: setCDFGNode(tmp);
             tmp->setParallelCase(LY->stack_d.pop());
             tmp->setFullCase(LY->stack_d.pop());
             $$ = tmp;
*/
             if (LY->allSensitive && (yyvsp[(3) - (7)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (7)].basePtr)))->getNameId()));
          ;}
    break;

  case 186:
#line 2418 "vlpYacc.y"
    {
             YYTRACE("statement: YYFOR '(' assignment ';' expression ';' assignment ')' statement");
             (yyval.basePtr) = new VlpForNode((yyvsp[(3) - (9)].basePtr), (yyvsp[(5) - (9)].basePtr), (yyvsp[(7) - (9)].basePtr), (yyvsp[(9) - (9)].basePtr), NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 187:
#line 2428 "vlpYacc.y"
    {
             YYTRACE("statement: event_control statement");
             if (LY->allSensitive == false)
                LY->alwaysEventPtr = (yyvsp[(1) - (2)].basePtr);
             else {
                assert (LY->nodeSet.size() != 0);
                set<string> :: const_iterator sPos  = LY->nodeSet.end();
                VlpSignalNode* sn = NULL;
                VlpEdgeNode*   en = NULL;
                VlpEdgeNode*   previous_en = NULL;
                if (LY->nodeSet.size() == 1) {
                   sPos = LY->nodeSet.begin();
                   sn = new VlpSignalNode(BaseModule :: setName(*sPos));
                   en = new VlpEdgeNode(EDGETYPE_SENSITIVE, sn);
                   BaseModule :: setCDFGNode(sn);
                   BaseModule :: setCDFGNode(en);
                   LY->alwaysEventPtr = en;
                }
                else {
                   for (sPos = LY->nodeSet.begin(); sPos != LY->nodeSet.end(); sPos++) {
                      sn = new VlpSignalNode(BaseModule :: setName(*sPos));
                      en = new VlpEdgeNode(EDGETYPE_SENSITIVE, sn);
                      en->setNext(previous_en);
                      BaseModule :: setCDFGNode(sn);
                      BaseModule :: setCDFGNode(en);
                      previous_en = en;
                   }
                   LY->alwaysEventPtr = new VlpOrNode(previous_en);
                   BaseModule :: setCDFGNode(LY->alwaysEventPtr);
                }               
                LY->nodeSet.clear();
             }
             (yyval.basePtr) = (yyvsp[(2) - (2)].basePtr);
             (LY->state) = EVENT;
          ;}
    break;

  case 188:
#line 2464 "vlpYacc.y"
    {                                      //but to ignore delay
             YYTRACE("statement: lvalue '=' delay_control expression ';'");
             (yyval.basePtr) = new VlpBAorNBA_Node(ASSIGNTYPE_BLOCK, (yyvsp[(1) - (5)].basePtr), (yyvsp[(4) - (5)].basePtr), NULL);
             if (LY->allSensitive && (yyvsp[(4) - (5)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(4) - (5)].basePtr)))->getNameId()));
          ;}
    break;

  case 189:
#line 2475 "vlpYacc.y"
    {                                             // use the rule, but to ignore dalay
             YYTRACE("statement: lvalue YYNBASSIGN delay_control expression ';'");
             (yyval.basePtr) = new VlpBAorNBA_Node( ASSIGNTYPE_NBLOCK, (yyvsp[(1) - (5)].basePtr), (yyvsp[(4) - (5)].basePtr), NULL);
             if (LY->allSensitive && (yyvsp[(4) - (5)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(4) - (5)].basePtr)))->getNameId()));
          ;}
    break;

  case 190:
#line 2494 "vlpYacc.y"
    {
             YYTRACE("statement: seq_block");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
          ;}
    break;

  case 191:
#line 2503 "vlpYacc.y"
    {            //task can use another task and function
             YYTRACE("statement: task_enable");
             if (LY->scopeState == FUNCTION) {
                cerr << "Error20 : function can't use another task at line " << LY->lineNo << endl;
                exit(0);
             }
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
          ;}
    break;

  case 192:
#line 2523 "vlpYacc.y"
    {
             YYTRACE("statement: YYASSIGN assignment ';'");
             (yyval.basePtr) = new VlpAssignNode((yyvsp[(2) - (3)].basePtr), NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 193:
#line 2535 "vlpYacc.y"
    { LY->stack_d.push(false); LY->stack_d.push(false); ;}
    break;

  case 194:
#line 2536 "vlpYacc.y"
    { LY->stack_d.push(true) ; LY->stack_d.push(false); ;}
    break;

  case 195:
#line 2537 "vlpYacc.y"
    { LY->stack_d.push(false); LY->stack_d.push(true);  ;}
    break;

  case 196:
#line 2538 "vlpYacc.y"
    { LY->stack_d.push(true);  LY->stack_d.push(true);  ;}
    break;

  case 197:
#line 2539 "vlpYacc.y"
    { LY->stack_d.push(true);  LY->stack_d.push(true);  ;}
    break;

  case 198:
#line 2544 "vlpYacc.y"
    {
             YYTRACE("assignment: lvalue '=' expression");
             if ((LY->state) == NET)//nettype
             {
                //only the syntax "lvalue : identifier" is legal.
                assert((yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL);
                const VlpSignalNode* tmp = (static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)));
                VlpPortNode* target = NULL;
                if (LY->modulePtr->getPort(BaseModule :: getName(tmp->getNameId()), target)) 
                { //exist in map(declared)
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                }
                else { // new in map
                   target = new VlpPortNode(LY->msbPtr, LY->lsbPtr);
                   target->setNetType(LY->tempNetType);
                   target->setDriveStr(LY->hStr, LY->lStr);
                   LY->modulePtr->setPort(BaseModule :: getName(tmp->getNameId()), target);
                }
                (yyval.basePtr) = new VlpBAorNBA_Node(ASSIGNTYPE_BLOCK, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr), NULL); 
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
             else if ((LY->state) == PARAM) {
                if (LY->scopeState == MODULE)
                   LY->modulePtr->setParam(LY->tempStr4, (yyvsp[(3) - (3)].basePtr));
                else if (LY->scopeState == FUNCTION)
                   LY->functionPtr->setParam(LY->tempStr4, (yyvsp[(3) - (3)].basePtr));
                else  //TASK    
                   LY->taskPtr->setParam(LY->tempStr4, (yyvsp[(3) - (3)].basePtr));
                LY->tempStr4 = "";  
                (yyval.basePtr) = NULL;
             }
             else if ((LY->state) == PARAM_NOL) {
                const paramPair* pair = new paramPair(LY->tempStr4, (yyvsp[(3) - (3)].basePtr));
                LY->modulePtr->setParamNOL(LY->instName, pair);          
                LY->tempStr4 = "";  
                LY->instName = "";
                (yyval.basePtr) = NULL;
             }
             else {//other ASSIGN, seq_block
                (yyval.basePtr) = new VlpBAorNBA_Node(ASSIGNTYPE_BLOCK, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr), NULL); 
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
          ;}
    break;

  case 199:
#line 2589 "vlpYacc.y"
    {
             YYTRACE("assignment: lvalue YYNBASSIGN expression");
             if ( (LY->state) == NET || (LY->state) == PARAM 
               || (LY->state) == PARAM_NOL || (LY->state) == ASSIGN) {
                cerr << "state = " << LY->state << endl; 
                cerr << "SYNTAX ERROR: using NBASSIGN at line " << LY->lineNo
                     << " (\"wire\" or \"parameter\" or \"assign\")" << endl;
                exit(0);
             }
             else {
                (yyval.basePtr) = new VlpBAorNBA_Node(ASSIGNTYPE_NBLOCK, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr), NULL);
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
          ;}
    break;

  case 200:
#line 2607 "vlpYacc.y"
    {
             YYTRACE("case_item_eclr: case_item");
             LY->stack_c.push((yyvsp[(1) - (1)].basePtr));
             VlpBaseNode* tail = (yyvsp[(1) - (1)].basePtr);
             while(tail->getNext() != NULL)
                tail = const_cast<VlpBaseNode*>(tail->getNext());//const_cast : const VlpBaseNode* -> VlpBaseNode*
             LY->stack_c.push(tail);
          ;}
    break;

  case 201:
#line 2616 "vlpYacc.y"
    {
             YYTRACE("case_item_eclr: case_item_eclr case_item");
             VlpBaseNode* tail = LY->stack_c.top();
             LY->stack_c.pop();
             tail->setNext((yyvsp[(2) - (2)].basePtr));
             tail = (yyvsp[(2) - (2)].basePtr);
             while(tail->getNext() != NULL)
                tail = const_cast<VlpBaseNode*>(tail->getNext());
             LY->stack_c.push((VlpBaseNode*)tail);
/*no use STL
             VlpBaseNode* tail = LY->stack_c.pop();
             tail->setNext($2);
             tail = $2;
             while(tail->getNext() != NULL)
                tail = const_cast<VlpBaseNode*>(tail->getNext());
             LY->stack_c.push((VlpBaseNode*)tail);
*/
          ;}
    break;

  case 202:
#line 2638 "vlpYacc.y"
    {
             YYTRACE("case_item: expression_list ':' statement");
             assert((yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT);
             VlpConnectNode* temp = static_cast<VlpConnectNode*>((yyvsp[(1) - (3)].basePtr));
     
             (yyval.basePtr) = new VlpCaseItemNode(const_cast<VlpBaseNode*>(temp->getExp()), (yyvsp[(3) - (3)].basePtr), NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));
             VlpBaseNode* ptr = (yyval.basePtr);
             VlpCaseItemNode* next;
             temp = static_cast<VlpConnectNode*>( const_cast<VlpBaseNode*>(temp->getNext()) );
             while (temp != NULL) {
                assert(temp->getNodeClass() == NODECLASS_CONNECT);
                next = new VlpCaseItemNode(const_cast<VlpBaseNode*>(temp->getExp()), (yyvsp[(3) - (3)].basePtr), NULL);
                BaseModule :: setCDFGNode(next);
                ptr->setNext(next);
                ptr = next;
                temp = static_cast<VlpConnectNode*>( const_cast<VlpBaseNode*>(temp->getNext()) );
             }
          
          ;}
    break;

  case 203:
#line 2659 "vlpYacc.y"
    {
             YYTRACE("case_item: YYDEFAULT ':' statement");
             VlpBaseNode* ptr = new VlpSignalNode(BaseModule :: setName("default")); 
             BaseModule :: setCDFGNode(ptr);

             (yyval.basePtr) = new VlpCaseItemNode(ptr, (yyvsp[(3) - (3)].basePtr), NULL);          
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 204:
#line 2668 "vlpYacc.y"
    {
             YYTRACE("case_item: YYDEFAULT statement");
             VlpBaseNode* ptr = new VlpSignalNode(BaseModule :: setName("default")); 
             BaseModule :: setCDFGNode(ptr);
             (yyval.basePtr) = new VlpCaseItemNode(ptr, (yyvsp[(2) - (2)].basePtr), NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 205:
#line 2679 "vlpYacc.y"
    {
             YYTRACE("seq_block: YYBEGIN statement_clr YYEND");
             if ((yyvsp[(2) - (3)].basePtr) != NULL) {
                (yyval.basePtr) = LY->stack_s.top();
                LY->stack_s.pop();
             }
/*no use STL
             if ($2 != NULL)
                $$ = LY->stack_s.pop();
*/
          ;}
    break;

  case 206:
#line 2691 "vlpYacc.y"
    {
             YYTRACE("seq_block: YYBEGIN ':' name_of_block block_declaration_clr statement_clr YYEND");
             if ((yyvsp[(5) - (6)].basePtr) != NULL) {
                (yyval.basePtr) = LY->stack_s.top();
                LY->stack_s.pop();
             }
/*no use STL
             if ($5 != NULL)
                $$ = LY->stack_s.pop();
*/
          ;}
    break;

  case 207:
#line 2717 "vlpYacc.y"
    {
             YYTRACE("name_of_block: YYID");
          ;}
    break;

  case 208:
#line 2724 "vlpYacc.y"
    {
             YYTRACE("block_declaration_clr:");
          ;}
    break;

  case 209:
#line 2728 "vlpYacc.y"
    {
             YYTRACE("block_declaration_clr: block_declaration_clr block_declaration");
             cerr << "No supply syntax in block_declaration_clr at line " << LY->lineNo << endl;
          ;}
    break;

  case 210:
#line 2736 "vlpYacc.y"
    {
             YYTRACE("block_declaration: parameter_declaration");
          ;}
    break;

  case 211:
#line 2740 "vlpYacc.y"
    {
             YYTRACE("block_declaration: reg_declaration");
          ;}
    break;

  case 212:
#line 2764 "vlpYacc.y"
    { 
          ;}
    break;

  case 213:
#line 2767 "vlpYacc.y"
    {
             YYTRACE("task_enable: YYID ';'");
             (yyval.basePtr) = new VlpTaskCall(*(yyvsp[(1) - (3)].stringPtr), NULL, NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 214:
#line 2773 "vlpYacc.y"
    {
             LY->instName = *(yyvsp[(1) - (1)].stringPtr);   
	  ;}
    break;

  case 215:
#line 2777 "vlpYacc.y"
    {
             YYTRACE("task_enable: YYID '(' expression_list ')' ';'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
             (yyval.basePtr) = new VlpTaskCall(LY->instName, head, NULL);
             BaseModule :: setCDFGNode((yyval.basePtr));
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
             $$ = new VlpTaskCall(LY->instName, head, NULL);
             BaseModule :: setCDFGNode($$);
*/
          ;}
    break;

  case 216:
#line 2957 "vlpYacc.y"
    {
	     YYTRACE("lvalue: YYID");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) { //net or parameter declarement
                BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr));
                LY->tempStr4 = *(yyvsp[(1) - (1)].stringPtr);
             }
             else if ( (LY->state) == NET) {
                (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr))); 
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
             else {//ASSIGN, seq_block ...
                if (LY->modulePtr->lintPort(*(yyvsp[(1) - (1)].stringPtr)) == false)//lintting
                {                                    
                   const paramPair* dummy;                        //parameter only can be used in lvalue under declaring
                   if (LY->modulePtr->lintParam(*(yyvsp[(1) - (1)].stringPtr), dummy) == true) {//condition, other condition it can't be used 
                      cerr << "Error21 : parameter " << *(yyvsp[(1) - (1)].stringPtr) << " can't be assigned at line "  << LY->lineNo << endl;
                      exit(0);                    
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(*(yyvsp[(1) - (1)].stringPtr)) == true))                  
                      ;//the identifier is declared in function scope ==> It don't need to lint     
                   else if ((LY->scopeState == TASK) && (LY->taskPtr->lintPort(*(yyvsp[(1) - (1)].stringPtr)) == true))  
                      ;//the identifier is declared in task scope ==> It don't need to lint
                   else {
                      cerr << "Error22 : un-declare identifier " << *(yyvsp[(1) - (1)].stringPtr) << " at line "  << LY->lineNo << endl;
                      exit(0);
                   }
                }
                (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr))); 
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
	  ;}
    break;

  case 217:
#line 2988 "vlpYacc.y"
    { LY->sigNames.push(*(yyvsp[(1) - (1)].stringPtr)) ;}
    break;

  case 218:
#line 2989 "vlpYacc.y"
    {
	     YYTRACE("lvalue: YYID '[' expression ']'");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) {
                cerr << "Syntax Error23 : paramter can't use '[' at line " << LY->lineNo << endl;
                exit(0);
             }
             else if ((LY->state) == NET) { //EX ; wire [7:0] a[3]
                cerr << "Syntax Error24 : net-type can't use array at line " << LY->lineNo << endl;
                exit(0);                
             }             
             string sigName = LY->sigNames.top();
             LY->sigNames.pop();
             //if ((LY->state) != NET && LY->modulePtr->lintPort(*$1) == false)//lintting
             if (LY->modulePtr->lintPort(sigName) == false)//linting
             {
                if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(sigName) == true))           
                   ;//the identifier is declared in function scope ==> It don't need to lint                     
                else if ((LY->scopeState == TASK) && (LY->taskPtr->lintPort(sigName) == true))    
                   ;//the identifier is declared in task scope ==> It don't need to lint
                else {
                   cerr << "error : un-declare identifier " << sigName << " at line "  << LY->lineNo << endl;
                   exit(0);
                }
             }
             if ((yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression or [ exp : exp ]
             {
                (yyval.basePtr) = new VlpSignalNode (
                     BaseModule :: setName(sigName), 
                     const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getFirst() ), 
                     const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getSecond() ) ); 
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
             else {
                (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(sigName), (yyvsp[(3) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr)); 
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
          ;}
    break;

  case 219:
#line 3027 "vlpYacc.y"
    {
	     YYTRACE("lvalue: concatenation");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) {
                cerr << "Syntax Error25 : paramter can't use '{' at line " << LY->lineNo << endl;
                exit(0);
             }
             else if ((LY->state) == NET) {
                cerr << "Syntax Error26 : net-type declaration error at line " << LY->lineNo << endl;
                exit(0);                
             }             
             
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
	  ;}
    break;

  case 220:
#line 3041 "vlpYacc.y"
    { 
	     YYTRACE("lvalue: YYDEF_ID");
             const VlpBaseNode* tmp;
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) {
                cerr << "Syntax Error27 : paramter can't use defined macro at line " <<  LY->lineNo <<endl;
                exit(0);
             }
             if ((yyvsp[(2) - (2)].basePtr) == NULL)
             {
                if (LY->defineMap.getData(*(yyvsp[(1) - (2)].stringPtr) ,tmp) == false)
                {
                   cerr << "Error28 : the macro " << *(yyvsp[(1) - (2)].stringPtr) << " isn't defined at line " << LY->lineNo << endl;
                   exit(0);
                }
                (yyval.basePtr) = const_cast<VlpBaseNode*>(tmp);
             }
             else // need to new a new node, and copy the defined content to the new node,
             {    // can't change the original defined content
                  // still not finish 2006 7/10
             }
          ;}
    break;

  case 221:
#line 3065 "vlpYacc.y"
    { (yyval.basePtr) = NULL; ;}
    break;

  case 222:
#line 3066 "vlpYacc.y"
    { (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr); ;}
    break;

  case 223:
#line 3072 "vlpYacc.y"
    {
	     YYTRACE("mintypmax_expression_list: mintypmax_expression");
             LY->array.push_back((yyvsp[(1) - (1)].basePtr));
	  ;}
    break;

  case 224:
#line 3077 "vlpYacc.y"
    {
             YYTRACE("mintypmax_expression_list: mintypmax_expression_list ',' mintypmax_expression");
             LY->array.push_back((yyvsp[(3) - (3)].basePtr));
	  ;}
    break;

  case 225:
#line 3085 "vlpYacc.y"
    {
	     YYTRACE("mintypmax_expression: expression");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
	  ;}
    break;

  case 226:
#line 3097 "vlpYacc.y"
    {/*expression is used in case, task, and concatenate*/
	     YYTRACE("expression_list: expression");
             if ((yyvsp[(1) - (1)].basePtr)->checkNext() == false) {//many signal node and OPNodes don't have setNext() function
                (yyval.basePtr) = new VlpConnectNode((yyvsp[(1) - (1)].basePtr));
                BaseModule :: setCDFGNode((yyval.basePtr));
             }
             else 
                (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
             LY->stack_e.push((yyval.basePtr));//head of expression_list
             LY->stack_e.push((yyval.basePtr));//tail of expression_list
	  ;}
    break;

  case 227:
#line 3109 "vlpYacc.y"
    {
	     YYTRACE("expression_list: expression_list ',' expression");
             VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
*/
             if (tail->getNodeClass() == NODECLASS_CONNECT) {        
                VlpBaseNode* tmp = new VlpConnectNode((yyvsp[(3) - (3)].basePtr));
                BaseModule :: setCDFGNode(tmp);
                tail->setNext(tmp);
                tail = tmp;
             }
             else {
                tail->setNext((yyvsp[(3) - (3)].basePtr));
                tail = (yyvsp[(3) - (3)].basePtr);
             }
             LY->stack_e.push(tail);
	  ;}
    break;

  case 228:
#line 3132 "vlpYacc.y"
    {
	     YYTRACE("expression: primary");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
	  ;}
    break;

  case 229:
#line 3137 "vlpYacc.y"
    {
	     YYTRACE("expression: '+' primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpSignNode(ARITHOP_UNARY_PLUS, (yyvsp[(2) - (2)].basePtr)); 
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 230:
#line 3143 "vlpYacc.y"
    {
	     YYTRACE("expression: '-' primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpSignNode(ARITHOP_UNARY_MINUS, (yyvsp[(2) - (2)].basePtr)); 
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 231:
#line 3149 "vlpYacc.y"
    {
	     YYTRACE("expression: '!' primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpLogicNotOpNode((yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(2) - (2)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(2) - (2)].basePtr)))->getNameId()));
	  ;}
    break;

  case 232:
#line 3157 "vlpYacc.y"
    {
             YYTRACE("expression: '~' primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpBitNotOpNode((yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 233:
#line 3163 "vlpYacc.y"
    {
	     YYTRACE("expression: '&' primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpReductOpNode(REDUCTOP_AND, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 234:
#line 3169 "vlpYacc.y"
    {
	     YYTRACE("expression: '|' primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpReductOpNode(REDUCTOP_OR, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 235:
#line 3175 "vlpYacc.y"
    {/*The rule is added by Louis.*/
	     YYTRACE("expression: '^' primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpReductOpNode(REDUCTOP_XOR, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 236:
#line 3181 "vlpYacc.y"
    {
	     YYTRACE("expression: YYLOGNAND primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpReductOpNode(REDUCTOP_NAND, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 237:
#line 3187 "vlpYacc.y"
    {
             YYTRACE("expression: YYLOGNOR primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpReductOpNode(REDUCTOP_NOR, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 238:
#line 3193 "vlpYacc.y"
    {
	     YYTRACE("expression: YYLOGXNOR primary %prec YYUNARYOPERATOR");
             (yyval.basePtr) = new VlpReductOpNode(REDUCTOP_XNOR, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 239:
#line 3199 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '+' expression");
             (yyval.basePtr) = new VlpArithOpNode(ARITHOP_ADD, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 240:
#line 3205 "vlpYacc.y"
    {
	     YYTRACE("expression: expressio '-' expression");
             (yyval.basePtr) = new VlpArithOpNode(ARITHOP_SUBTRACT, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 241:
#line 3211 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '*' expression");
             (yyval.basePtr) = new VlpArithOpNode(ARITHOP_MULTIPLY, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 242:
#line 3225 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLOGEQUALITY expression");
             (yyval.basePtr) = new VlpEqualityNode(EQUALITY_LOG_EQU, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 243:
#line 3235 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLOGINEQUALITY expression");
             (yyval.basePtr) = new VlpEqualityNode(EQUALITY_LOG_INEQU, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 244:
#line 3245 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYCASEEQUALITY expression");
             (yyval.basePtr) = new VlpEqualityNode(EQUALITY_CASE_EQU, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 245:
#line 3255 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYCASEINEQUALITY expression");
             (yyval.basePtr) = new VlpEqualityNode(EQUALITY_CASE_INEQU, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 246:
#line 3265 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLOGAND expression");
             (yyval.basePtr) = new VlpLogicOpNode(LOGICOP_AND, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 247:
#line 3275 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLOGOR expression");
             (yyval.basePtr) = new VlpLogicOpNode(LOGICOP_OR, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 248:
#line 3285 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '<' expression");
             (yyval.basePtr) = new VlpRelateOpNode(RELATEOP_LESS, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 249:
#line 3295 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '>' expression");
             (yyval.basePtr) = new VlpRelateOpNode(RELATEOP_GREATER, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 250:
#line 3305 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '&' expression");
             (yyval.basePtr) = new VlpBitWiseOpNode(BITWISEOP_AND, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 251:
#line 3311 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '|' expression");
             (yyval.basePtr) = new VlpBitWiseOpNode(BITWISEOP_OR, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 252:
#line 3317 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '^' expression");
             (yyval.basePtr) = new VlpBitWiseOpNode(BITWISEOP_XOR, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 253:
#line 3323 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLEQ expression");
             (yyval.basePtr) = new VlpRelateOpNode(RELATEOP_LEQ, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 254:
#line 3333 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLEQ expression");           
             (yyval.basePtr) = new VlpRelateOpNode(RELATEOP_LEQ, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             //$$ = new VlpBAorNBA_Node(ASSIGNTYPE_NBLOCK, $1, $3, NULL);
	  ;}
    break;

  case 255:
#line 3340 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYGEQ expression");
             (yyval.basePtr) = new VlpRelateOpNode(RELATEOP_GEQ, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 256:
#line 3350 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLSHIFT expression");
             (yyval.basePtr) = new VlpShiftOpNode(SHIFTOP_LSH, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 257:
#line 3356 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYRSHIFT expression");
             (yyval.basePtr) = new VlpShiftOpNode(SHIFTOP_RSH, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 258:
#line 3362 "vlpYacc.y"
    {
	     YYTRACE("expression: expression YYLOGXNOR expression");
             (yyval.basePtr) = new VlpBitWiseOpNode(BITWISEOP_XNOR, (yyvsp[(1) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
             if (LY->allSensitive && (yyvsp[(1) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(1) - (3)].basePtr)))->getNameId()));
             if (LY->allSensitive && (yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_SIGNAL)
                LY->nodeSet.insert(BaseModule :: getName((static_cast<VlpSignalNode*>((yyvsp[(3) - (3)].basePtr)))->getNameId()));
	  ;}
    break;

  case 259:
#line 3372 "vlpYacc.y"
    {
	     YYTRACE("expression: expression '?' expression ':' expression");
             (yyval.basePtr) = new VlpConditionalNode((yyvsp[(1) - (5)].basePtr), (yyvsp[(3) - (5)].basePtr), (yyvsp[(5) - (5)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 260:
#line 3385 "vlpYacc.y"
    {
	     YYTRACE("primary: YYINUMBER");
             (yyval.basePtr) = new VlpIntNode((yyvsp[(1) - (1)].ival));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 261:
#line 3391 "vlpYacc.y"
    {
             YYTRACE("primary: YYRNUMBER");
             cerr << "No supply : primary : YYRNUMBER at line " << LY->lineNo << endl;
	  ;}
    break;

  case 262:
#line 3396 "vlpYacc.y"
    {
	     YYTRACE("primary: identifier");
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) { //linting
                const paramPair* target;
                if (LY->modulePtr->lintParam(*(yyvsp[(1) - (1)].stringPtr), target) == true) { //parameter 
                   (yyval.basePtr) = new VlpParamNode(const_cast<paramPair*>(target));
                   BaseModule :: setCDFGNode((yyval.basePtr));
                }
                else {
                   cerr << "Error29 : Symbol (" << *(yyvsp[(1) - (1)].stringPtr) << ") is illegal, constant expected at line" 
                        << LY->lineNo << endl;
                   exit(0);
                }
             }
             //(LY->state) != PARAM && (LY->state) != PARAM_NOL
             else {
                if (((LY->state) != DEFINE) && (LY->modulePtr->lintPort(*(yyvsp[(1) - (1)].stringPtr)) == false)) {//linting 
                   const paramPair* target;
                   if (LY->modulePtr->lintParam(*(yyvsp[(1) - (1)].stringPtr), target) == true) {//module scope parameter 
                      (yyval.basePtr) = new VlpParamNode(const_cast<paramPair*>(target));
                      BaseModule :: setCDFGNode((yyval.basePtr));
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(*(yyvsp[(1) - (1)].stringPtr)) == true)) {//function scope variable
                      (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr)));            
                      BaseModule :: setCDFGNode((yyval.basePtr));
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintParam(*(yyvsp[(1) - (1)].stringPtr), target) == true))//function scope param.
                      (yyval.basePtr) = target->content;//substitute the parameter directly, because it can't be overload.                    
                   else if ((LY->scopeState == TASK) && (LY->taskPtr->lintParam(*(yyvsp[(1) - (1)].stringPtr), target) == true)) //task scope param.
                      (yyval.basePtr) = target->content;//substitute the parameter directly, because it can't be overload.                    
                   else {//un-declare variable => default 1 bit wire
                      VlpPortNode* pn = new VlpPortNode(NULL, NULL);
                      pn->setNetType(wire);
                      LY->modulePtr->setPort(*(yyvsp[(1) - (1)].stringPtr), pn);
                      (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr)));
                      //cerr << "Error30 : un-declare identifier " << *$1 << " at line "  << LY->lineNo << endl;
                      //exit(0);
                   }
                }
                else {//(LY->state) == DEFINE or LY->modulePtr->lintPort == true
                   (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr)));            
                   BaseModule :: setCDFGNode((yyval.basePtr));
                }
             }  
	  ;}
    break;

  case 263:
#line 3441 "vlpYacc.y"
    { LY->sigNames.push(*(yyvsp[(1) - (1)].stringPtr)); ;}
    break;

  case 264:
#line 3442 "vlpYacc.y"
    {
	     YYTRACE("primary: identifier bitExpression");
             string sigName = LY->sigNames.top();
             LY->sigNames.pop();
             const paramPair* target;
             if ((LY->state) == PARAM || (LY->state) == PARAM_NOL) { //linting;
                if (LY->modulePtr->lintParam(sigName, target) == true) { //parameter 
                   if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                      paramPair* newNode = new paramPair(*target);

                      if ((yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression
                      {                                           //or Ex: a[5:0]
                         ((VlpSignalNode*)(newNode->content))->setSigWidth( 
                         const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getFirst() ),
                         const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getSecond()) );
                      }
                      else                                        //Ex: a[3]
                         ((VlpSignalNode*)(newNode->content))->setSigWidth((yyvsp[(3) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
                      (yyval.basePtr) = new VlpParamNode(newNode);                   
                      BaseModule :: setCDFGNode((yyval.basePtr));
                   }
                   else {
                      cerr << "Error31 : Symbol (" << sigName << ") is illegal, error bit-expression at line " 
                           << LY->lineNo << endl;
                      exit(0);                      
                   }
                }
                else {
                   cerr << "Error32 : Symbol (" << sigName << ") is illegal, constant expected at line " 
                        << LY->lineNo << endl;
                   exit(0);
                }
             }             
             else { //(LY->state) != PARAM && (LY->state) != PARAM_NOL
                if (((LY->state) != DEFINE) && (LY->modulePtr->lintPort(sigName) == false))//linting
                {
                   if (LY->modulePtr->lintParam(sigName, target) == true) { //module scope parameter 
                   //Ex : parameter ab 4'b1100      ab[0:2] or ab[3]
                      if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                         paramPair* newNode = new paramPair(*target);
                         if ((yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression
                         {                                           //or Ex: 4'b111[2:0]
                            ((VlpSignalNode*)(newNode->content))->setSigWidth( 
                            const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getFirst() ),
                            const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getSecond()) );
                         }
                         else                                        //Ex: 4'b0011[3]
                            ((VlpSignalNode*)(newNode->content))->setSigWidth((yyvsp[(3) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
                         (yyval.basePtr) = new VlpParamNode(newNode);
                         BaseModule :: setCDFGNode((yyval.basePtr));
                      }                   
                      else {
                         cerr << "Error33 : Symbol (" << sigName << ") is illegal, error bit-expression at line "
                              << LY->lineNo << endl;
                         exit(0);
                      }
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintPort(sigName) == true)) {//function scope variable
                      VlpSignalNode* newNode = new VlpSignalNode(BaseModule :: setName(sigName));
                      BaseModule :: setCDFGNode(newNode);
                      if ((yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)
                      {                                       
                         newNode->setSigWidth( const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getFirst() ),
                                               const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getSecond()) );
                      }
                      else                                      
                         newNode->setSigWidth((yyvsp[(3) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
                      (yyval.basePtr) = newNode;
                   }
                   else if ((LY->scopeState == FUNCTION) && (LY->functionPtr->lintParam(sigName, target) == true)) {//function scope param.
                      if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                         //substitute the parameter directly,because it can't be overload.
                         VlpSignalNode* newNode = new VlpSignalNode(((VlpSignalNode*)(target->content))->getNameId());
                         BaseModule :: setCDFGNode(newNode);
                         if ((yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)                      
                         {                                       
                            newNode->setSigWidth( const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getFirst() ),
                                                  const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getSecond()) );
                         }
                         else                                      
                            newNode->setSigWidth((yyvsp[(3) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
                         (yyval.basePtr) = newNode;
                      }
                   }
                   else if ((LY->scopeState == TASK) && (LY->taskPtr->lintParam(sigName, target) == true)) {//task scope param.
                      if (target->content->getNodeClass() == NODECLASS_SIGNAL) {
                      //substitute the parameter directly, because it can't be overload.
                         VlpSignalNode* newNode = new VlpSignalNode(((VlpSignalNode*)(target->content))->getNameId());
                         BaseModule :: setCDFGNode(newNode);
                         if ((yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)                      
                         {                                       
                            newNode->setSigWidth( const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getFirst() ),
                                                  const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getSecond()) );
                         }
                         else                                      
                            newNode->setSigWidth((yyvsp[(3) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));
                         (yyval.basePtr) = newNode;
                      }
                   }
                   else {
                      cerr << "Error34 : un-declare identifier " << sigName << " at line "  << LY->lineNo << endl;
                      exit(0);
                   }
                }
                else {
                   if ((yyvsp[(3) - (3)].basePtr)->getNodeClass() == NODECLASS_CONNECT)//via YYDEF_ID->primary->expression 
                   {                                           //or Ex: a[5:0]
                      (yyval.basePtr) = new VlpSignalNode (
                           BaseModule :: setName(sigName), 
                           const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getFirst() ), 
                           const_cast<VlpBaseNode*>( ((VlpConnectNode*)((yyvsp[(3) - (3)].basePtr)))->getSecond() ) ); 
                      BaseModule :: setCDFGNode((yyval.basePtr));
                   }
                   else {                                        //Ex: a[3]
                      (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(sigName), (yyvsp[(3) - (3)].basePtr), (yyvsp[(3) - (3)].basePtr));      
                      BaseModule :: setCDFGNode((yyval.basePtr));
                   }
                }
             }       
          ;}
    break;

  case 265:
#line 3563 "vlpYacc.y"
    {
	     YYTRACE("primary: concatenation");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
	  ;}
    break;

  case 266:
#line 3568 "vlpYacc.y"
    {
	     YYTRACE("primary: multiple_concatenatin");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
	  ;}
    break;

  case 267:
#line 3573 "vlpYacc.y"
    {
	     YYTRACE("primary: function_call");
             (yyval.basePtr) = (yyvsp[(1) - (1)].basePtr);
	  ;}
    break;

  case 268:
#line 3578 "vlpYacc.y"
    {                //but put in different BNF to make modify easily in the future
             YYTRACE("primary: BIT_BASE_NUMBER");
             (yyval.basePtr) = new VlpSignalNode(BaseModule :: setName(*(yyvsp[(1) - (1)].stringPtr)));    
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 269:
#line 3584 "vlpYacc.y"
    {
	     YYTRACE("expression: ( expression )");
             (yyval.basePtr) = (yyvsp[(2) - (3)].basePtr);
          ;}
    break;

  case 270:
#line 3589 "vlpYacc.y"
    { 
             YYTRACE("primary: YYDEF_ID");
             const VlpBaseNode* tmp;
             if ((yyvsp[(2) - (2)].basePtr) == NULL) 
             {
                if (LY->defineMap.getData(*(yyvsp[(1) - (2)].stringPtr) ,tmp) == false)
                {
                   cerr << "Error35 : the macro " << *(yyvsp[(1) - (2)].stringPtr) << " isn't defined at line " << LY->lineNo << endl;
                   exit(0);
                }
                (yyval.basePtr) = const_cast<VlpBaseNode*>(tmp);
             }
             else // need to new a new node, and copy the defined content to the new node,
             {    // can't change the original defined content
                  // still not finish 2006 7/10
             }
          ;}
    break;

  case 271:
#line 3615 "vlpYacc.y"
    {
             (yyval.basePtr) = (yyvsp[(2) - (3)].basePtr);
          ;}
    break;

  case 272:
#line 3619 "vlpYacc.y"
    {
             (yyval.basePtr) = new VlpConnectNode((yyvsp[(2) - (5)].basePtr), (yyvsp[(4) - (5)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 273:
#line 3636 "vlpYacc.y"
    {
	     YYTRACE("concatenation: '{' expression_list '}'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
*/
             (yyval.basePtr) = new VlpConcatenateNode(head);    
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 274:
#line 3653 "vlpYacc.y"
    {
	     YYTRACE("multiple_concatenation: '{' expression '{' expression_list '}' '}'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
*/

             (yyval.basePtr) = new VlpReplicationNode((yyvsp[(2) - (6)].basePtr), head);
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 275:
#line 3670 "vlpYacc.y"
    { LY->instName = *(yyvsp[(1) - (2)].stringPtr); ;}
    break;

  case 276:
#line 3672 "vlpYacc.y"
    {
	     YYTRACE("function_call: identifier '(' expression_list ')'");
             //VlpBaseNode* tail = LY->stack_e.top();
             LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.top();
             LY->stack_e.pop();
/*no use STL
             VlpBaseNode* tail = LY->stack_e.pop();
             VlpBaseNode* head = LY->stack_e.pop();
*/
             (yyval.basePtr) = new VlpFunCall(LY->instName, head);
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 277:
#line 3701 "vlpYacc.y"
    {//identifier => stringPtr 
             YYTRACE("identifier: YYID");
             (yyval.stringPtr) = (yyvsp[(1) - (1)].stringPtr);
          ;}
    break;

  case 278:
#line 3705 "vlpYacc.y"
    { LY->instName = *(yyvsp[(1) - (2)].stringPtr); ;}
    break;

  case 279:
#line 3708 "vlpYacc.y"
    {
             YYTRACE("identifier: identifier '.' YYID");
             (yyval.stringPtr) = (yyvsp[(4) - (4)].stringPtr);
          ;}
    break;

  case 280:
#line 3755 "vlpYacc.y"
    {
	     YYTRACE("delay_control: '#' YYINUMBER");
	  ;}
    break;

  case 281:
#line 3759 "vlpYacc.y"
    {
	     YYTRACE("delay_control: '#' YYRNUMBER");
	  ;}
    break;

  case 282:
#line 3763 "vlpYacc.y"
    {
	     YYTRACE("delay_control: '#' identifier");
	  ;}
    break;

  case 283:
#line 3767 "vlpYacc.y"
    {
	     YYTRACE("delay_control: '#' '(' mintypmax_expression ')'");
	  ;}
    break;

  case 284:
#line 3779 "vlpYacc.y"
    {
	     YYTRACE("event_control: '@' '(' event_expression ')'");
             (yyval.basePtr) = (yyvsp[(3) - (4)].basePtr);
	  ;}
    break;

  case 285:
#line 3784 "vlpYacc.y"
    {
	     YYTRACE("event_control: '@' '(' ored_event_expression ')'");
             (yyval.basePtr) = new VlpOrNode((yyvsp[(3) - (4)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 286:
#line 3790 "vlpYacc.y"
    {
             /*list the sensitive list
               statement : assignment
                         | if
                         | case casex casez
                         | lvalue '='
                         | lvalue '<='
                         | for (X).... not support 
             */             
             LY->allSensitive = true;
          ;}
    break;

  case 287:
#line 3802 "vlpYacc.y"
    {
             LY->allSensitive = true;
          ;}
    break;

  case 288:
#line 3816 "vlpYacc.y"
    {
	     YYTRACE("event_expression: expression");
             (yyval.basePtr) = new VlpEdgeNode(EDGETYPE_SENSITIVE, (yyvsp[(1) - (1)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 289:
#line 3822 "vlpYacc.y"
    {
	     YYTRACE("event_expression: YYPOSEDGE expression");
             (yyval.basePtr) = new VlpEdgeNode(EDGETYPE_POSEDGE, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 290:
#line 3828 "vlpYacc.y"
    {
	     YYTRACE("event_expression: YYNEGEDGE expression");
             (yyval.basePtr) = new VlpEdgeNode(EDGETYPE_NEGEDGE, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
	  ;}
    break;

  case 291:
#line 3834 "vlpYacc.y"
    {
	     YYTRACE("event_expression: YYEDGE expression");
             (yyval.basePtr) = new VlpEdgeNode(EDGETYPE_EDGE, (yyvsp[(2) - (2)].basePtr));
             BaseModule :: setCDFGNode((yyval.basePtr));
          ;}
    break;

  case 292:
#line 3843 "vlpYacc.y"
    {
	     YYTRACE("ored_event_expression: event_expression YYOR event_expression");
             (yyvsp[(1) - (3)].basePtr)->setNext((yyvsp[(3) - (3)].basePtr));
             LY->nodeTailPtr2 = (yyvsp[(3) - (3)].basePtr);
             (yyval.basePtr) = (yyvsp[(1) - (3)].basePtr);
	  ;}
    break;

  case 293:
#line 3850 "vlpYacc.y"
    {
	     YYTRACE("ored_event_expression: ored_event_expression YYOR event_expression");
             LY->nodeTailPtr2->setNext((yyvsp[(3) - (3)].basePtr));
             LY->nodeTailPtr2 = (yyvsp[(3) - (3)].basePtr);
	  ;}
    break;


/* Line 1267 of yacc.c.  */
#line 5676 "vlpYacc.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 3863 "vlpYacc.y"


void yyerror(char* str)
{
    cerr << "YYERROR : No support syntax in line " << LY->lineNo << endl;
    exit (1);
}




