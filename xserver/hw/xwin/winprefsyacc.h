/* A Bison parser, made from /home/anholt/xserver-stable/hw/xwin/winprefsyacc.y, by GNU bison 1.75.  */

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

#ifndef BISON_WINPREFSYACC_H
# define BISON_WINPREFSYACC_H

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NEWLINE = 258,
     MENU = 259,
     LB = 260,
     RB = 261,
     ICONDIRECTORY = 262,
     DEFAULTICON = 263,
     ICONS = 264,
     DEFAULTSYSMENU = 265,
     SYSMENU = 266,
     ROOTMENU = 267,
     SEPARATOR = 268,
     ATSTART = 269,
     ATEND = 270,
     EXEC = 271,
     ALWAYSONTOP = 272,
     DEBUG = 273,
     RELOAD = 274,
     TRAYICON = 275,
     SILENTEXIT = 276,
     STRING = 277
   };
#endif
#define NEWLINE 258
#define MENU 259
#define LB 260
#define RB 261
#define ICONDIRECTORY 262
#define DEFAULTICON 263
#define ICONS 264
#define DEFAULTSYSMENU 265
#define SYSMENU 266
#define ROOTMENU 267
#define SEPARATOR 268
#define ATSTART 269
#define ATEND 270
#define EXEC 271
#define ALWAYSONTOP 272
#define DEBUG 273
#define RELOAD 274
#define TRAYICON 275
#define SILENTEXIT 276
#define STRING 277




#ifndef YYSTYPE
#line 79 "winprefsyacc.y"
typedef union {
  char *sVal;
  int iVal;
} yystype;
/* Line 1237 of /usr/local/share/bison/yacc.c.  */
#line 89 "winprefsyacc.h"
# define YYSTYPE yystype
#endif

extern YYSTYPE yylval;


#endif /* not BISON_WINPREFSYACC_H */

