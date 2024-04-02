/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_MESA_PROGRAM_PROGRAM_PARSE_TAB_H_INCLUDED
# define YY_YY_SRC_MESA_PROGRAM_PROGRAM_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ARBvp_10 = 258,                /* ARBvp_10  */
    ARBfp_10 = 259,                /* ARBfp_10  */
    ADDRESS = 260,                 /* ADDRESS  */
    ALIAS = 261,                   /* ALIAS  */
    ATTRIB = 262,                  /* ATTRIB  */
    OPTION = 263,                  /* OPTION  */
    OUTPUT = 264,                  /* OUTPUT  */
    PARAM = 265,                   /* PARAM  */
    TEMP = 266,                    /* TEMP  */
    END = 267,                     /* END  */
    BIN_OP = 268,                  /* BIN_OP  */
    BINSC_OP = 269,                /* BINSC_OP  */
    SAMPLE_OP = 270,               /* SAMPLE_OP  */
    SCALAR_OP = 271,               /* SCALAR_OP  */
    TRI_OP = 272,                  /* TRI_OP  */
    VECTOR_OP = 273,               /* VECTOR_OP  */
    ARL = 274,                     /* ARL  */
    KIL = 275,                     /* KIL  */
    SWZ = 276,                     /* SWZ  */
    TXD_OP = 277,                  /* TXD_OP  */
    INTEGER = 278,                 /* INTEGER  */
    REAL = 279,                    /* REAL  */
    AMBIENT = 280,                 /* AMBIENT  */
    ATTENUATION = 281,             /* ATTENUATION  */
    BACK = 282,                    /* BACK  */
    CLIP = 283,                    /* CLIP  */
    COLOR = 284,                   /* COLOR  */
    DEPTH = 285,                   /* DEPTH  */
    DIFFUSE = 286,                 /* DIFFUSE  */
    DIRECTION = 287,               /* DIRECTION  */
    EMISSION = 288,                /* EMISSION  */
    ENV = 289,                     /* ENV  */
    EYE = 290,                     /* EYE  */
    FOG = 291,                     /* FOG  */
    FOGCOORD = 292,                /* FOGCOORD  */
    FRAGMENT = 293,                /* FRAGMENT  */
    FRONT = 294,                   /* FRONT  */
    HALF = 295,                    /* HALF  */
    INVERSE = 296,                 /* INVERSE  */
    INVTRANS = 297,                /* INVTRANS  */
    LIGHT = 298,                   /* LIGHT  */
    LIGHTMODEL = 299,              /* LIGHTMODEL  */
    LIGHTPROD = 300,               /* LIGHTPROD  */
    LOCAL = 301,                   /* LOCAL  */
    MATERIAL = 302,                /* MATERIAL  */
    MAT_PROGRAM = 303,             /* MAT_PROGRAM  */
    MATRIX = 304,                  /* MATRIX  */
    MATRIXINDEX = 305,             /* MATRIXINDEX  */
    MODELVIEW = 306,               /* MODELVIEW  */
    MVP = 307,                     /* MVP  */
    NORMAL = 308,                  /* NORMAL  */
    OBJECT = 309,                  /* OBJECT  */
    PALETTE = 310,                 /* PALETTE  */
    PARAMS = 311,                  /* PARAMS  */
    PLANE = 312,                   /* PLANE  */
    POINT_TOK = 313,               /* POINT_TOK  */
    POINTSIZE = 314,               /* POINTSIZE  */
    POSITION = 315,                /* POSITION  */
    PRIMARY = 316,                 /* PRIMARY  */
    PROGRAM = 317,                 /* PROGRAM  */
    PROJECTION = 318,              /* PROJECTION  */
    RANGE = 319,                   /* RANGE  */
    RESULT = 320,                  /* RESULT  */
    ROW = 321,                     /* ROW  */
    SCENECOLOR = 322,              /* SCENECOLOR  */
    SECONDARY = 323,               /* SECONDARY  */
    SHININESS = 324,               /* SHININESS  */
    SIZE_TOK = 325,                /* SIZE_TOK  */
    SPECULAR = 326,                /* SPECULAR  */
    SPOT = 327,                    /* SPOT  */
    STATE = 328,                   /* STATE  */
    TEXCOORD = 329,                /* TEXCOORD  */
    TEXENV = 330,                  /* TEXENV  */
    TEXGEN = 331,                  /* TEXGEN  */
    TEXGEN_Q = 332,                /* TEXGEN_Q  */
    TEXGEN_R = 333,                /* TEXGEN_R  */
    TEXGEN_S = 334,                /* TEXGEN_S  */
    TEXGEN_T = 335,                /* TEXGEN_T  */
    TEXTURE = 336,                 /* TEXTURE  */
    TRANSPOSE = 337,               /* TRANSPOSE  */
    TEXTURE_UNIT = 338,            /* TEXTURE_UNIT  */
    TEX_1D = 339,                  /* TEX_1D  */
    TEX_2D = 340,                  /* TEX_2D  */
    TEX_3D = 341,                  /* TEX_3D  */
    TEX_CUBE = 342,                /* TEX_CUBE  */
    TEX_RECT = 343,                /* TEX_RECT  */
    TEX_SHADOW1D = 344,            /* TEX_SHADOW1D  */
    TEX_SHADOW2D = 345,            /* TEX_SHADOW2D  */
    TEX_SHADOWRECT = 346,          /* TEX_SHADOWRECT  */
    TEX_ARRAY1D = 347,             /* TEX_ARRAY1D  */
    TEX_ARRAY2D = 348,             /* TEX_ARRAY2D  */
    TEX_ARRAYSHADOW1D = 349,       /* TEX_ARRAYSHADOW1D  */
    TEX_ARRAYSHADOW2D = 350,       /* TEX_ARRAYSHADOW2D  */
    VERTEX = 351,                  /* VERTEX  */
    VTXATTRIB = 352,               /* VTXATTRIB  */
    IDENTIFIER = 353,              /* IDENTIFIER  */
    USED_IDENTIFIER = 354,         /* USED_IDENTIFIER  */
    MASK4 = 355,                   /* MASK4  */
    MASK3 = 356,                   /* MASK3  */
    MASK2 = 357,                   /* MASK2  */
    MASK1 = 358,                   /* MASK1  */
    SWIZZLE = 359,                 /* SWIZZLE  */
    DOT_DOT = 360,                 /* DOT_DOT  */
    DOT = 361                      /* DOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 137 "../src/mesa/program/program_parse.y"

   struct asm_instruction *inst;
   struct asm_symbol *sym;
   struct asm_symbol temp_sym;
   struct asm_swizzle_mask swiz_mask;
   struct asm_src_register src_reg;
   struct prog_dst_register dst_reg;
   struct prog_instruction temp_inst;
   char *string;
   unsigned result;
   unsigned attrib;
   int integer;
   float real;
   gl_state_index16 state[STATE_LENGTH];
   int negate;
   struct asm_vector vector;
   enum prog_opcode opcode;

   struct {
      unsigned swz;
      unsigned rgba_valid:1;
      unsigned xyzw_valid:1;
      unsigned negate:1;
   } ext_swizzle;

#line 196 "src/mesa/program/program_parse.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (struct asm_parser_state *state);


#endif /* !YY_YY_SRC_MESA_PROGRAM_PROGRAM_PARSE_TAB_H_INCLUDED  */
