/* A Bison parser, made from /home/anholt/xserver-stable/hw/dmx/config/parser.y, by GNU bison 1.75.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_VIRTUAL = 258,
     T_DISPLAY = 259,
     T_WALL = 260,
     T_OPTION = 261,
     T_PARAM = 262,
     T_STRING = 263,
     T_DIMENSION = 264,
     T_OFFSET = 265,
     T_ORIGIN = 266,
     T_COMMENT = 267,
     T_LINE_COMMENT = 268
   };
#endif
#define T_VIRTUAL 258
#define T_DISPLAY 259
#define T_WALL 260
#define T_OPTION 261
#define T_PARAM 262
#define T_STRING 263
#define T_DIMENSION 264
#define T_OFFSET 265
#define T_ORIGIN 266
#define T_COMMENT 267
#define T_LINE_COMMENT 268




/* Copy the first part of user declarations.  */
#line 35 "parser.y"

#ifdef HAVE_DMX_CONFIG_H
#include <dmx-config.h>
#endif

#include "dmxparse.h"
#include <string.h>
#include <stdlib.h>
#define YYDEBUG 1
#define YYERROR_VERBOSE
#define YY_USE_PROTOS

DMXConfigEntryPtr dmxConfigEntry = NULL;
#define APPEND(type, h, t)                 \
{                                          \
    type pt;                               \
    for (pt = h; pt->next; pt = pt->next); \
    pt->next = t;                          \
}


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

#ifndef YYSTYPE
#line 56 "parser.y"
typedef union {
    DMXConfigTokenPtr      token;
    DMXConfigStringPtr     string;
    DMXConfigNumberPtr     number;
    DMXConfigPairPtr       pair;
    DMXConfigFullDimPtr    fdim;
    DMXConfigPartDimPtr    pdim;
    DMXConfigDisplayPtr    display;
    DMXConfigWallPtr       wall;
    DMXConfigOptionPtr     option;
    DMXConfigParamPtr      param;
    DMXConfigCommentPtr    comment;
    DMXConfigSubPtr        subentry;
    DMXConfigVirtualPtr    virtual;
    DMXConfigEntryPtr      entry;
} yystype;
/* Line 188 of /usr/local/share/bison/yacc.c.  */
#line 137 "parser.c"
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Copy the second part of user declarations.  */


/* Line 213 of /usr/local/share/bison/yacc.c.  */
#line 158 "parser.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];	\
	}					\
      while (0)
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  13
#define YYLAST   106

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  18
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  25
/* YYNRULES -- Number of rules. */
#define YYNRULES  59
/* YYNRULES -- Number of states. */
#define YYNSTATES  95

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   268

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     6,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     5,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     2,     4,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    14,    19,    25,
      31,    38,    40,    43,    45,    47,    49,    51,    53,    57,
      61,    66,    68,    71,    74,    77,    79,    81,    85,    88,
      90,    96,   101,   106,   111,   115,   119,   122,   128,   133,
     137,   139,   142,   144,   147,   149,   152,   154,   157,   159,
     162,   164,   167,   169,   172,   174,   177,   179,   182,   184
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      19,     0,    -1,    20,    -1,    21,    -1,    20,    21,    -1,
      22,    -1,    17,    -1,     7,    39,    23,    40,    -1,     7,
      35,    39,    23,    40,    -1,     7,    34,    39,    23,    40,
      -1,     7,    34,    35,    39,    23,    40,    -1,    24,    -1,
      23,    24,    -1,    17,    -1,    31,    -1,    32,    -1,    25,
      -1,    26,    -1,    10,    42,    38,    -1,    11,    42,    38,
      -1,    11,    39,    27,    40,    -1,    28,    -1,    27,    28,
      -1,    42,    38,    -1,    35,    36,    -1,    35,    -1,    36,
      -1,    29,     6,    29,    -1,     6,    29,    -1,    29,    -1,
      33,    34,    30,    37,    38,    -1,    33,    30,    37,    38,
      -1,    33,    34,    37,    38,    -1,    33,    34,    30,    38,
      -1,    33,    30,    38,    -1,    33,    34,    38,    -1,    33,
      38,    -1,    41,    35,    35,    42,    38,    -1,    41,    35,
      42,    38,    -1,    41,    42,    38,    -1,     8,    -1,     8,
      16,    -1,    12,    -1,    12,    16,    -1,    13,    -1,    13,
      16,    -1,    14,    -1,    14,    16,    -1,    15,    -1,    15,
      16,    -1,     5,    -1,     5,    16,    -1,     3,    -1,     3,
      16,    -1,     4,    -1,     4,    16,    -1,     9,    -1,     9,
      16,    -1,    34,    -1,    42,    34,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    95,    95,    98,    99,   102,   103,   106,   108,   110,
     112,   116,   117,   120,   121,   122,   123,   124,   127,   131,
     133,   139,   140,   143,   147,   149,   151,   155,   157,   159,
     163,   165,   167,   170,   172,   174,   176,   180,   182,   184,
     188,   189,   192,   193,   196,   197,   200,   201,   204,   205,
     208,   209,   212,   213,   216,   217,   220,   221,   224,   225
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'{'", "'}'", "';'", "'/'", "T_VIRTUAL", 
  "T_DISPLAY", "T_WALL", "T_OPTION", "T_PARAM", "T_STRING", "T_DIMENSION", 
  "T_OFFSET", "T_ORIGIN", "T_COMMENT", "T_LINE_COMMENT", "$accept", 
  "Program", "EntryList", "Entry", "Virtual", "SubList", "Sub", 
  "OptionEntry", "ParamEntry", "ParamList", "Param", "PartialDim", 
  "FullDim", "DisplayEntry", "WallEntry", "Display", "Name", "Dimension", 
  "Offset", "Origin", "Terminal", "Open", "Close", "Wall", "NameList", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   123,   125,    59,    47,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    18,    19,    20,    20,    21,    21,    22,    22,    22,
      22,    23,    23,    24,    24,    24,    24,    24,    25,    26,
      26,    27,    27,    28,    29,    29,    29,    30,    30,    30,
      31,    31,    31,    31,    31,    31,    31,    32,    32,    32,
      33,    33,    34,    34,    35,    35,    36,    36,    37,    37,
      38,    38,    39,    39,    40,    40,    41,    41,    42,    42
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     4,     5,     5,
       6,     1,     2,     1,     1,     1,     1,     1,     3,     3,
       4,     1,     2,     2,     2,     1,     1,     3,     2,     1,
       5,     4,     4,     4,     3,     3,     2,     5,     4,     3,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     6,     0,     2,     3,     5,    52,    42,    44,
       0,     0,     0,     1,     4,    53,    43,    45,     0,     0,
       0,    40,    56,     0,     0,    13,     0,    11,    16,    17,
      14,    15,     0,     0,     0,     0,     0,    41,    57,    58,
       0,     0,     0,    54,    12,     7,    50,     0,    46,    29,
       0,     0,    25,    26,    36,     0,     0,     0,     9,     8,
      59,    18,     0,    21,     0,    19,    55,    51,    28,    47,
       0,    48,     0,    34,     0,     0,    35,    24,     0,     0,
      39,    10,    22,    20,    23,    27,    49,    31,     0,    33,
      32,     0,    38,    30,    37
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     3,     4,     5,     6,    26,    27,    28,    29,    62,
      63,    49,    50,    30,    31,    32,    39,    52,    53,    72,
      54,    12,    45,    33,    64
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -32
static const yysigned_char yypact[] =
{
      -3,    41,   -32,    22,    -3,   -32,   -32,    12,    35,    46,
       5,    62,    75,   -32,   -32,   -32,   -32,   -32,    62,    75,
      75,    51,    54,    59,    18,   -32,    65,   -32,   -32,   -32,
     -32,   -32,    88,    37,    75,    65,    65,   -32,   -32,   -32,
      86,    59,    86,    61,   -32,   -32,    79,    -4,    80,    28,
      31,    74,    67,   -32,   -32,    37,    86,    65,   -32,   -32,
     -32,   -32,    56,   -32,    86,   -32,   -32,   -32,   -32,   -32,
      -4,    81,    94,   -32,    31,    94,   -32,   -32,    59,    86,
     -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,    94,   -32,
     -32,    86,   -32,   -32,   -32
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -32,   -32,   -32,    99,   -32,     6,   -19,   -32,   -32,   -32,
      42,   -28,    55,   -32,   -32,   -32,    -1,     2,    53,   -31,
     -27,    48,   -30,   -32,   -22
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      10,    40,    42,    11,     1,    58,    59,    44,     7,     9,
      48,    56,    18,    61,     2,    65,    44,    44,     9,    68,
      75,     7,    13,    73,    76,    35,    36,    81,    15,    80,
       8,    51,    83,    79,    70,    55,    46,    84,    44,    60,
      57,    60,    85,    88,     7,    87,    71,    89,    90,     8,
       9,    16,    92,     8,     9,    60,    91,    78,    19,    20,
      43,    93,    17,    60,    94,     7,    34,    37,     8,    43,
      38,     8,    41,    21,    22,    23,    24,    66,    60,    46,
      47,    48,    25,    21,    22,    23,    24,     9,    48,    71,
      60,    46,    25,    46,    47,    67,    69,    86,     8,    46,
       8,     9,    48,    14,    82,    77,    74
};

static const unsigned char yycheck[] =
{
       1,    23,    24,     1,     7,    35,    36,    26,     3,    13,
      14,    33,    10,    40,    17,    42,    35,    36,    13,    47,
      51,     3,     0,    50,    51,    19,    20,    57,    16,    56,
      12,    32,    62,    55,     6,    33,     5,    64,    57,    40,
      34,    42,    70,    74,     3,    72,    15,    74,    75,    12,
      13,    16,    79,    12,    13,    56,    78,    55,    10,    11,
       4,    88,    16,    64,    91,     3,    18,    16,    12,     4,
      16,    12,    24,     8,     9,    10,    11,    16,    79,     5,
       6,    14,    17,     8,     9,    10,    11,    13,    14,    15,
      91,     5,    17,     5,     6,    16,    16,    16,    12,     5,
      12,    13,    14,     4,    62,    52,    51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     7,    17,    19,    20,    21,    22,     3,    12,    13,
      34,    35,    39,     0,    21,    16,    16,    16,    35,    39,
      39,     8,     9,    10,    11,    17,    23,    24,    25,    26,
      31,    32,    33,    41,    39,    23,    23,    16,    16,    34,
      42,    39,    42,     4,    24,    40,     5,     6,    14,    29,
      30,    34,    35,    36,    38,    35,    42,    23,    40,    40,
      34,    38,    27,    28,    42,    38,    16,    16,    29,    16,
       6,    15,    37,    38,    30,    37,    38,    36,    35,    42,
      38,    40,    28,    40,    38,    29,    16,    38,    37,    38,
      38,    42,    38,    38,    38
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

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
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#define YYLEX	yylex ()

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
} while (0)
# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yysymprint (FILE* yyout, int yytype, YYSTYPE yyvalue)
#else
yysymprint (yyout, yytype, yyvalue)
    FILE* yyout;
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[yytype], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yydestruct (int yytype, YYSTYPE yyvalue)
#else
yydestruct (yytype, yyvalue)
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  switch (yytype)
    {
      default:
        break;
    }
}



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of parse errors so far.  */
int yynerrs;


int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
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

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with.  */

  if (yychar <= 0)		/* This means end of input.  */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more.  */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, yychar1, yylval));
      YYDPRINTF ((stderr, "\n"));
    }

  /* If the proper action on seeing token YYCHAR1 is to reduce or to
     detect an error, take that action.  */
  yyn += yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yychar1)
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

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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



#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn - 1, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] >= 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    {
        case 2:
#line 95 "parser.y"
    { dmxConfigEntry = yyvsp[0].entry; }
    break;

  case 4:
#line 99 "parser.y"
    { APPEND(DMXConfigEntryPtr,yyvsp[-1].entry,yyvsp[0].entry); yyval.entry = yyvsp[-1].entry; }
    break;

  case 5:
#line 102 "parser.y"
    { yyval.entry = dmxConfigEntryVirtual(yyvsp[0].virtual); }
    break;

  case 6:
#line 103 "parser.y"
    { yyval.entry = dmxConfigEntryComment(yyvsp[0].comment); }
    break;

  case 7:
#line 107 "parser.y"
    { yyval.virtual = dmxConfigCreateVirtual(yyvsp[-3].token, NULL, NULL, yyvsp[-2].token, yyvsp[-1].subentry, yyvsp[0].token); }
    break;

  case 8:
#line 109 "parser.y"
    { yyval.virtual = dmxConfigCreateVirtual(yyvsp[-4].token, NULL, yyvsp[-3].pair, yyvsp[-2].token, yyvsp[-1].subentry, yyvsp[0].token); }
    break;

  case 9:
#line 111 "parser.y"
    { yyval.virtual = dmxConfigCreateVirtual(yyvsp[-4].token, yyvsp[-3].string, NULL, yyvsp[-2].token, yyvsp[-1].subentry, yyvsp[0].token); }
    break;

  case 10:
#line 113 "parser.y"
    { yyval.virtual = dmxConfigCreateVirtual(yyvsp[-5].token, yyvsp[-4].string, yyvsp[-3].pair, yyvsp[-2].token, yyvsp[-1].subentry, yyvsp[0].token ); }
    break;

  case 12:
#line 117 "parser.y"
    { APPEND(DMXConfigSubPtr,yyvsp[-1].subentry,yyvsp[0].subentry); yyval.subentry = yyvsp[-1].subentry; }
    break;

  case 13:
#line 120 "parser.y"
    { yyval.subentry = dmxConfigSubComment(yyvsp[0].comment); }
    break;

  case 14:
#line 121 "parser.y"
    { yyval.subentry = dmxConfigSubDisplay(yyvsp[0].display); }
    break;

  case 15:
#line 122 "parser.y"
    { yyval.subentry = dmxConfigSubWall(yyvsp[0].wall); }
    break;

  case 16:
#line 123 "parser.y"
    { yyval.subentry = dmxConfigSubOption(yyvsp[0].option); }
    break;

  case 17:
#line 124 "parser.y"
    { yyval.subentry = dmxConfigSubParam(yyvsp[0].param); }
    break;

  case 18:
#line 128 "parser.y"
    { yyval.option = dmxConfigCreateOption(yyvsp[-2].token, yyvsp[-1].string, yyvsp[0].token); }
    break;

  case 19:
#line 132 "parser.y"
    { yyval.param = dmxConfigCreateParam(yyvsp[-2].token, NULL, yyvsp[-1].string, NULL, yyvsp[0].token); }
    break;

  case 20:
#line 134 "parser.y"
    { yyval.param = dmxConfigCreateParam(yyvsp[-3].token, yyvsp[-2].token, NULL, yyvsp[0].token, NULL);
               yyval.param->next = yyvsp[-1].param;
             }
    break;

  case 22:
#line 140 "parser.y"
    { APPEND(DMXConfigParamPtr,yyvsp[-1].param,yyvsp[0].param); yyval.param = yyvsp[-1].param; }
    break;

  case 23:
#line 144 "parser.y"
    { yyval.param = dmxConfigCreateParam(NULL, NULL, yyvsp[-1].string, NULL, yyvsp[0].token); }
    break;

  case 24:
#line 148 "parser.y"
    { yyval.pdim = dmxConfigCreatePartDim(yyvsp[-1].pair, yyvsp[0].pair); }
    break;

  case 25:
#line 150 "parser.y"
    { yyval.pdim = dmxConfigCreatePartDim(yyvsp[0].pair, NULL); }
    break;

  case 26:
#line 152 "parser.y"
    { yyval.pdim = dmxConfigCreatePartDim(NULL, yyvsp[0].pair); }
    break;

  case 27:
#line 156 "parser.y"
    { yyval.fdim = dmxConfigCreateFullDim(yyvsp[-2].pdim, yyvsp[0].pdim); }
    break;

  case 28:
#line 158 "parser.y"
    { yyval.fdim = dmxConfigCreateFullDim(NULL, yyvsp[0].pdim); }
    break;

  case 29:
#line 160 "parser.y"
    { yyval.fdim = dmxConfigCreateFullDim(yyvsp[0].pdim, NULL); }
    break;

  case 30:
#line 164 "parser.y"
    { yyval.display = dmxConfigCreateDisplay(yyvsp[-4].token, yyvsp[-3].string, yyvsp[-2].fdim, yyvsp[-1].pair, yyvsp[0].token); }
    break;

  case 31:
#line 166 "parser.y"
    { yyval.display = dmxConfigCreateDisplay(yyvsp[-3].token, NULL, yyvsp[-2].fdim, yyvsp[-1].pair, yyvsp[0].token); }
    break;

  case 32:
#line 168 "parser.y"
    { yyval.display = dmxConfigCreateDisplay(yyvsp[-3].token, yyvsp[-2].string, NULL, yyvsp[-1].pair, yyvsp[0].token); }
    break;

  case 33:
#line 171 "parser.y"
    { yyval.display = dmxConfigCreateDisplay(yyvsp[-3].token, yyvsp[-2].string, yyvsp[-1].fdim, NULL, yyvsp[0].token); }
    break;

  case 34:
#line 173 "parser.y"
    { yyval.display = dmxConfigCreateDisplay(yyvsp[-2].token, NULL, yyvsp[-1].fdim, NULL, yyvsp[0].token); }
    break;

  case 35:
#line 175 "parser.y"
    { yyval.display = dmxConfigCreateDisplay(yyvsp[-2].token, yyvsp[-1].string, NULL, NULL, yyvsp[0].token); }
    break;

  case 36:
#line 177 "parser.y"
    { yyval.display = dmxConfigCreateDisplay(yyvsp[-1].token, NULL, NULL, NULL, yyvsp[0].token); }
    break;

  case 37:
#line 181 "parser.y"
    { yyval.wall = dmxConfigCreateWall(yyvsp[-4].token, yyvsp[-3].pair, yyvsp[-2].pair, yyvsp[-1].string, yyvsp[0].token); }
    break;

  case 38:
#line 183 "parser.y"
    { yyval.wall = dmxConfigCreateWall(yyvsp[-3].token, yyvsp[-2].pair, NULL, yyvsp[-1].string, yyvsp[0].token); }
    break;

  case 39:
#line 185 "parser.y"
    { yyval.wall = dmxConfigCreateWall(yyvsp[-2].token, NULL, NULL, yyvsp[-1].string, yyvsp[0].token); }
    break;

  case 41:
#line 189 "parser.y"
    { yyval.token = yyvsp[-1].token; yyval.token->comment = yyvsp[0].comment->comment; }
    break;

  case 43:
#line 193 "parser.y"
    { yyval.string = yyvsp[-1].string; yyval.string->comment = yyvsp[0].comment->comment; }
    break;

  case 45:
#line 197 "parser.y"
    { yyval.pair = yyvsp[-1].pair; yyval.pair->comment = yyvsp[0].comment->comment; }
    break;

  case 47:
#line 201 "parser.y"
    { yyval.pair = yyvsp[-1].pair; yyval.pair->comment = yyvsp[0].comment->comment; }
    break;

  case 49:
#line 205 "parser.y"
    { yyval.pair = yyvsp[-1].pair; yyval.pair->comment = yyvsp[0].comment->comment; }
    break;

  case 51:
#line 209 "parser.y"
    { yyval.token = yyvsp[-1].token; yyval.token->comment = yyvsp[0].comment->comment; }
    break;

  case 53:
#line 213 "parser.y"
    { yyval.token = yyvsp[-1].token; yyval.token->comment = yyvsp[0].comment->comment; }
    break;

  case 55:
#line 217 "parser.y"
    { yyval.token = yyvsp[-1].token; yyval.token->comment = yyvsp[0].comment->comment; }
    break;

  case 57:
#line 221 "parser.y"
    { yyval.token = yyvsp[-1].token; yyval.token->comment = yyvsp[0].comment->comment; }
    break;

  case 59:
#line 225 "parser.y"
    { APPEND(DMXConfigStringPtr, yyvsp[-1].string, yyvsp[0].string); yyval.string = yyvsp[-1].string; }
    break;


    }

/* Line 1016 of /usr/local/share/bison/yacc.c.  */
#line 1281 "parser.c"

  yyvsp -= yylen;
  yyssp -= yylen;


#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyssp > yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*yyssp],
			    *yyvsp));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*yyssp], *yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yydestruct (yychar1, yylval);
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

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

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*yyssp], *yyvsp));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp);
      yyvsp--;
      yystate = *--yyssp;


#if YYDEBUG
      if (yydebug)
	{
	  short *yyssp1 = yyss - 1;
	  YYFPRINTF (stderr, "Error: state stack now");
	  while (yyssp1 != yyssp)
	    YYFPRINTF (stderr, " %d", *++yyssp1);
	  YYFPRINTF (stderr, "\n");
	}
#endif
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 95 "parser.y"

