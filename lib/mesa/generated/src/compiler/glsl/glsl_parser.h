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

#ifndef YY__MESA_GLSL_SRC_COMPILER_GLSL_GLSL_PARSER_H_INCLUDED
# define YY__MESA_GLSL_SRC_COMPILER_GLSL_GLSL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int _mesa_glsl_debug;
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
    ATTRIBUTE = 258,               /* ATTRIBUTE  */
    CONST_TOK = 259,               /* CONST_TOK  */
    BASIC_TYPE_TOK = 260,          /* BASIC_TYPE_TOK  */
    BREAK = 261,                   /* BREAK  */
    BUFFER = 262,                  /* BUFFER  */
    CONTINUE = 263,                /* CONTINUE  */
    DO = 264,                      /* DO  */
    ELSE = 265,                    /* ELSE  */
    FOR = 266,                     /* FOR  */
    IF = 267,                      /* IF  */
    DEMOTE = 268,                  /* DEMOTE  */
    DISCARD = 269,                 /* DISCARD  */
    RETURN = 270,                  /* RETURN  */
    SWITCH = 271,                  /* SWITCH  */
    CASE = 272,                    /* CASE  */
    DEFAULT = 273,                 /* DEFAULT  */
    CENTROID = 274,                /* CENTROID  */
    IN_TOK = 275,                  /* IN_TOK  */
    OUT_TOK = 276,                 /* OUT_TOK  */
    INOUT_TOK = 277,               /* INOUT_TOK  */
    UNIFORM = 278,                 /* UNIFORM  */
    VARYING = 279,                 /* VARYING  */
    SAMPLE = 280,                  /* SAMPLE  */
    NOPERSPECTIVE = 281,           /* NOPERSPECTIVE  */
    FLAT = 282,                    /* FLAT  */
    SMOOTH = 283,                  /* SMOOTH  */
    IMAGE1DSHADOW = 284,           /* IMAGE1DSHADOW  */
    IMAGE2DSHADOW = 285,           /* IMAGE2DSHADOW  */
    IMAGE1DARRAYSHADOW = 286,      /* IMAGE1DARRAYSHADOW  */
    IMAGE2DARRAYSHADOW = 287,      /* IMAGE2DARRAYSHADOW  */
    COHERENT = 288,                /* COHERENT  */
    VOLATILE = 289,                /* VOLATILE  */
    RESTRICT = 290,                /* RESTRICT  */
    READONLY = 291,                /* READONLY  */
    WRITEONLY = 292,               /* WRITEONLY  */
    SHARED = 293,                  /* SHARED  */
    STRUCT = 294,                  /* STRUCT  */
    VOID_TOK = 295,                /* VOID_TOK  */
    WHILE = 296,                   /* WHILE  */
    IDENTIFIER = 297,              /* IDENTIFIER  */
    TYPE_IDENTIFIER = 298,         /* TYPE_IDENTIFIER  */
    NEW_IDENTIFIER = 299,          /* NEW_IDENTIFIER  */
    FLOATCONSTANT = 300,           /* FLOATCONSTANT  */
    FLOAT16CONSTANT = 301,         /* FLOAT16CONSTANT  */
    DOUBLECONSTANT = 302,          /* DOUBLECONSTANT  */
    INTCONSTANT = 303,             /* INTCONSTANT  */
    UINTCONSTANT = 304,            /* UINTCONSTANT  */
    BOOLCONSTANT = 305,            /* BOOLCONSTANT  */
    INT64CONSTANT = 306,           /* INT64CONSTANT  */
    UINT64CONSTANT = 307,          /* UINT64CONSTANT  */
    FIELD_SELECTION = 308,         /* FIELD_SELECTION  */
    LEFT_OP = 309,                 /* LEFT_OP  */
    RIGHT_OP = 310,                /* RIGHT_OP  */
    INC_OP = 311,                  /* INC_OP  */
    DEC_OP = 312,                  /* DEC_OP  */
    LE_OP = 313,                   /* LE_OP  */
    GE_OP = 314,                   /* GE_OP  */
    EQ_OP = 315,                   /* EQ_OP  */
    NE_OP = 316,                   /* NE_OP  */
    AND_OP = 317,                  /* AND_OP  */
    OR_OP = 318,                   /* OR_OP  */
    XOR_OP = 319,                  /* XOR_OP  */
    MUL_ASSIGN = 320,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 321,              /* DIV_ASSIGN  */
    ADD_ASSIGN = 322,              /* ADD_ASSIGN  */
    MOD_ASSIGN = 323,              /* MOD_ASSIGN  */
    LEFT_ASSIGN = 324,             /* LEFT_ASSIGN  */
    RIGHT_ASSIGN = 325,            /* RIGHT_ASSIGN  */
    AND_ASSIGN = 326,              /* AND_ASSIGN  */
    XOR_ASSIGN = 327,              /* XOR_ASSIGN  */
    OR_ASSIGN = 328,               /* OR_ASSIGN  */
    SUB_ASSIGN = 329,              /* SUB_ASSIGN  */
    INVARIANT = 330,               /* INVARIANT  */
    PRECISE = 331,                 /* PRECISE  */
    LOWP = 332,                    /* LOWP  */
    MEDIUMP = 333,                 /* MEDIUMP  */
    HIGHP = 334,                   /* HIGHP  */
    SUPERP = 335,                  /* SUPERP  */
    PRECISION = 336,               /* PRECISION  */
    VERSION_TOK = 337,             /* VERSION_TOK  */
    EXTENSION = 338,               /* EXTENSION  */
    LINE = 339,                    /* LINE  */
    COLON = 340,                   /* COLON  */
    EOL = 341,                     /* EOL  */
    INTERFACE_TOK = 342,           /* INTERFACE_TOK  */
    OUTPUT = 343,                  /* OUTPUT  */
    PRAGMA_DEBUG_ON = 344,         /* PRAGMA_DEBUG_ON  */
    PRAGMA_DEBUG_OFF = 345,        /* PRAGMA_DEBUG_OFF  */
    PRAGMA_OPTIMIZE_ON = 346,      /* PRAGMA_OPTIMIZE_ON  */
    PRAGMA_OPTIMIZE_OFF = 347,     /* PRAGMA_OPTIMIZE_OFF  */
    PRAGMA_WARNING_ON = 348,       /* PRAGMA_WARNING_ON  */
    PRAGMA_WARNING_OFF = 349,      /* PRAGMA_WARNING_OFF  */
    PRAGMA_INVARIANT_ALL = 350,    /* PRAGMA_INVARIANT_ALL  */
    LAYOUT_TOK = 351,              /* LAYOUT_TOK  */
    DOT_TOK = 352,                 /* DOT_TOK  */
    ASM = 353,                     /* ASM  */
    CLASS = 354,                   /* CLASS  */
    UNION = 355,                   /* UNION  */
    ENUM = 356,                    /* ENUM  */
    TYPEDEF = 357,                 /* TYPEDEF  */
    TEMPLATE = 358,                /* TEMPLATE  */
    THIS = 359,                    /* THIS  */
    PACKED_TOK = 360,              /* PACKED_TOK  */
    GOTO = 361,                    /* GOTO  */
    INLINE_TOK = 362,              /* INLINE_TOK  */
    NOINLINE = 363,                /* NOINLINE  */
    PUBLIC_TOK = 364,              /* PUBLIC_TOK  */
    STATIC = 365,                  /* STATIC  */
    EXTERN = 366,                  /* EXTERN  */
    EXTERNAL = 367,                /* EXTERNAL  */
    LONG_TOK = 368,                /* LONG_TOK  */
    SHORT_TOK = 369,               /* SHORT_TOK  */
    HALF = 370,                    /* HALF  */
    FIXED_TOK = 371,               /* FIXED_TOK  */
    UNSIGNED = 372,                /* UNSIGNED  */
    INPUT_TOK = 373,               /* INPUT_TOK  */
    HVEC2 = 374,                   /* HVEC2  */
    HVEC3 = 375,                   /* HVEC3  */
    HVEC4 = 376,                   /* HVEC4  */
    FVEC2 = 377,                   /* FVEC2  */
    FVEC3 = 378,                   /* FVEC3  */
    FVEC4 = 379,                   /* FVEC4  */
    SAMPLER3DRECT = 380,           /* SAMPLER3DRECT  */
    SIZEOF = 381,                  /* SIZEOF  */
    CAST = 382,                    /* CAST  */
    NAMESPACE = 383,               /* NAMESPACE  */
    USING = 384,                   /* USING  */
    RESOURCE = 385,                /* RESOURCE  */
    PATCH = 386,                   /* PATCH  */
    SUBROUTINE = 387,              /* SUBROUTINE  */
    ERROR_TOK = 388,               /* ERROR_TOK  */
    COMMON = 389,                  /* COMMON  */
    PARTITION = 390,               /* PARTITION  */
    ACTIVE = 391,                  /* ACTIVE  */
    FILTER = 392,                  /* FILTER  */
    ROW_MAJOR = 393,               /* ROW_MAJOR  */
    THEN = 394                     /* THEN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 101 "../src/compiler/glsl/glsl_parser.yy"

   int n;
   int64_t n64;
   float real;
   double dreal;
   const char *identifier;

   struct ast_type_qualifier type_qualifier;

   ast_node *node;
   ast_type_specifier *type_specifier;
   ast_array_specifier *array_specifier;
   ast_fully_specified_type *fully_specified_type;
   ast_function *function;
   ast_parameter_declarator *parameter_declarator;
   ast_function_definition *function_definition;
   ast_compound_statement *compound_statement;
   ast_expression *expression;
   ast_declarator_list *declarator_list;
   ast_struct_specifier *struct_specifier;
   ast_declaration *declaration;
   ast_switch_body *switch_body;
   ast_case_label *case_label;
   ast_case_label_list *case_label_list;
   ast_case_statement *case_statement;
   ast_case_statement_list *case_statement_list;
   ast_interface_block *interface_block;
   ast_subroutine_list *subroutine_list;
   struct {
      ast_node *cond;
      ast_expression *rest;
   } for_rest_statement;

   struct {
      ast_node *then_statement;
      ast_node *else_statement;
   } selection_rest_statement;

   const glsl_type *type;

#line 244 "src/compiler/glsl/glsl_parser.h"

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




int _mesa_glsl_parse (struct _mesa_glsl_parse_state *state);


#endif /* !YY__MESA_GLSL_SRC_COMPILER_GLSL_GLSL_PARSER_H_INCLUDED  */
