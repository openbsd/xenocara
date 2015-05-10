/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be
 used in advertising or publicity pertaining to distribution
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.

 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/X.h>
#include <X11/extensions/XKB.h>

#include "tokens.h"
#include "xkbevd.h"

FILE    *yyin = NULL;

static char scanFileBuf[1024];
char    *scanFile = scanFileBuf;
int      lineNum = 0;

int      scanInt;
char    *scanIntStr;
int      scanIntClass;

char    *scanStr = NULL;
int      scanStrLine = 0;

#define BUFSIZE 512
static int      nInBuf = 0;
static char     buf[BUFSIZE];

#ifdef DEBUG

extern unsigned debugFlags;

static char *
tokText(int tok)
{
    static char buf[32];

    switch (tok) {
    case END_OF_FILE:
        snprintf(buf, sizeof(buf), "END_OF_FILE");
        break;
    case ERROR:
        snprintf(buf, sizeof(buf), "ERROR");
        break;

    case BELL:
        snprintf(buf, sizeof(buf), "BELL");
        break;
    case ACCESSX:
        snprintf(buf, sizeof(buf), "ACCESSX");
        break;
    case MESSAGE:
        snprintf(buf, sizeof(buf), "MESSAGE");
        break;

    case NONE:
        snprintf(buf, sizeof(buf), "NONE");
        break;
    case IGNORE:
        snprintf(buf, sizeof(buf), "IGNORE");
        break;
    case ECHO:
        snprintf(buf, sizeof(buf), "ECHO");
        break;
    case PRINT_EV:
        snprintf(buf, sizeof(buf), "PRINT_EV");
        break;
    case SHELL:
        snprintf(buf, sizeof(buf), "SHELL");
        break;
    case SOUND:
        snprintf(buf, sizeof(buf), "SOUND");
        break;

    case EQUALS:
        snprintf(buf, sizeof(buf), "EQUALS");
        break;
    case PLUS:
        snprintf(buf, sizeof(buf), "PLUS");
        break;
    case MINUS:
        snprintf(buf, sizeof(buf), "MINUS");
        break;
    case DIVIDE:
        snprintf(buf, sizeof(buf), "DIVIDE");
        break;
    case TIMES:
        snprintf(buf, sizeof(buf), "TIMES");
        break;
    case OBRACE:
        snprintf(buf, sizeof(buf), "OBRACE");
        break;
    case CBRACE:
        snprintf(buf, sizeof(buf), "CBRACE");
        break;
    case OPAREN:
        snprintf(buf, sizeof(buf), "OPAREN");
        break;
    case CPAREN:
        snprintf(buf, sizeof(buf), "CPAREN");
        break;
    case OBRACKET:
        snprintf(buf, sizeof(buf), "OBRACKET");
        break;
    case CBRACKET:
        snprintf(buf, sizeof(buf), "CBRACKET");
        break;
    case DOT:
        snprintf(buf, sizeof(buf), "DOT");
        break;
    case COMMA:
        snprintf(buf, sizeof(buf), "COMMA");
        break;
    case SEMI:
        snprintf(buf, sizeof(buf), "SEMI");
        break;
    case EXCLAM:
        snprintf(buf, sizeof(buf), "EXCLAM");
        break;
    case INVERT:
        snprintf(buf, sizeof(buf), "INVERT");
        break;

    case STRING:
        snprintf(buf, sizeof(buf), "STRING (%s)", scanStr);
        break;
    case INTEGER:
        snprintf(buf, sizeof(buf), "INTEGER (0x%x)", scanInt);
        break;
    case FLOAT:
        snprintf(buf, sizeof(buf), "FLOAT (%d.%d)",
                 scanInt / XkbGeomPtsPerMM, scanInt % XkbGeomPtsPerMM);
        break;
    case IDENT:
        snprintf(buf, sizeof(buf), "IDENT (%s)", scanStr);
        break;
    case KEYNAME:
        snprintf(buf, sizeof(buf), "KEYNAME (%s)", scanStr);
        break;
    default:
        snprintf(buf, sizeof(buf), "UNKNOWN");
        break;
    }
    return buf;
}
#endif

int
setScanState(const char *file, int line)
{
    if (file != NULL)
        strncpy(scanFile, file, 1024);
    if (line >= 0)
        lineNum = line;
    return 1;
}

static int
yyGetString(void)
{
    int ch;

    nInBuf = 0;
    while (((ch = getc(yyin)) != EOF) && (ch != '"')) {
        if (ch == '\\') {
            if ((ch = getc(yyin)) != EOF) {
                if (ch == 'n')
                    ch = '\n';
                else if (ch == 't')
                    ch = '\t';
                else if (ch == 'v')
                    ch = '\v';
                else if (ch == 'b')
                    ch = '\b';
                else if (ch == 'r')
                    ch = '\r';
                else if (ch == 'f')
                    ch = '\f';
                else if (ch == 'e')
                    ch = '\033';
                else if (ch == '0') {
                    int tmp, stop;

                    ch = stop = 0;
                    if (((tmp = getc(yyin)) != EOF) && (isdigit(tmp)) &&
                        (tmp != '8') && (tmp != '9')) {
                        ch = (ch * 8) + (tmp - '0');
                    }
                    else {
                        stop = 1;
                        ungetc(tmp, yyin);
                    }
                    if ((!stop) && ((tmp = getc(yyin)) != EOF) && (isdigit(tmp))
                        && (tmp != '8') && (tmp != '9')) {
                        ch = (ch * 8) + (tmp - '0');
                    }
                    else {
                        stop = 1;
                        ungetc(tmp, yyin);
                    }
                    if ((!stop) && ((tmp = getc(yyin)) != EOF) && (isdigit(tmp))
                        && (tmp != '8') && (tmp != '9')) {
                        ch = (ch * 8) + (tmp - '0');
                    }
                    else {
                        stop = 1;
                        ungetc(tmp, yyin);
                    }
                }
            }
            else
                return ERROR;
        }

        if (nInBuf < BUFSIZE - 1)
            buf[nInBuf++] = ch;
    }
    if (ch == '"') {
        buf[nInBuf++] = '\0';
        if (scanStr)
            free(scanStr);
        scanStr = uStringDup(buf);
        scanStrLine = lineNum;
        return STRING;
    }
    return ERROR;
}

static int
yyGetKeyName(void)
{
    int ch;

    nInBuf = 0;
    while (((ch = getc(yyin)) != EOF) && (ch != '>')) {
        if (ch == '\\') {
            if ((ch = getc(yyin)) != EOF) {
                if (ch == 'n')
                    ch = '\n';
                else if (ch == 't')
                    ch = '\t';
                else if (ch == 'v')
                    ch = '\v';
                else if (ch == 'b')
                    ch = '\b';
                else if (ch == 'r')
                    ch = '\r';
                else if (ch == 'f')
                    ch = '\f';
                else if (ch == 'e')
                    ch = '\033';
                else if (ch == '0') {
                    int tmp, stop;

                    ch = stop = 0;
                    if (((tmp = getc(yyin)) != EOF) && (isdigit(tmp)) &&
                        (tmp != '8') && (tmp != '9')) {
                        ch = (ch * 8) + (tmp - '0');
                    }
                    else {
                        stop = 1;
                        ungetc(tmp, yyin);
                    }
                    if ((!stop) && ((tmp = getc(yyin)) != EOF) && (isdigit(tmp))
                        && (tmp != '8') && (tmp != '9')) {
                        ch = (ch * 8) + (tmp - '0');
                    }
                    else {
                        stop = 1;
                        ungetc(tmp, yyin);
                    }
                    if ((!stop) && ((tmp = getc(yyin)) != EOF) && (isdigit(tmp))
                        && (tmp != '8') && (tmp != '9')) {
                        ch = (ch * 8) + (tmp - '0');
                    }
                    else {
                        stop = 1;
                        ungetc(tmp, yyin);
                    }
                }
            }
            else
                return ERROR;
        }

        if (nInBuf < BUFSIZE - 1)
            buf[nInBuf++] = ch;
    }
    if ((ch == '>') && (nInBuf < 5)) {
        buf[nInBuf++] = '\0';
        if (scanStr)
            free(scanStr);
        scanStr = uStringDup(buf);
        scanStrLine = lineNum;
        return KEYNAME;
    }
    return ERROR;
}

static struct _Keyword {
    const char  *keyword;
    int         token;
} keywords[] = {
    { "bell",           BELL },
    { "accessx",        ACCESSX },
    { "message",        MESSAGE },
    { "none",           NONE },
    { "ignore",         IGNORE },
    { "echo",           ECHO },
    { "printevent",     PRINT_EV },
    { "shell",          SHELL },
    { "sound",          SOUND }
};
static int numKeywords = sizeof(keywords) / sizeof(struct _Keyword);

static int
yyGetIdent(int first)
{
    int ch, i, found;

    int rtrn = -1;

    buf[0] = first;
    nInBuf = 1;
    while (((ch = getc(yyin)) != EOF) && (isalnum(ch) || (ch == '_'))) {
        if (nInBuf < BUFSIZE - 1)
            buf[nInBuf++] = ch;
    }
    buf[nInBuf++] = '\0';
    found = 0;

    for (i = 0; (!found) && (i < numKeywords); i++) {
        if (uStrCaseCmp(buf, keywords[i].keyword) == 0) {
            rtrn = keywords[i].token;
            found = 1;
        }
    }
    if (!found) {
        if (scanStr)
            free(scanStr);
        scanStr = uStringDup(buf);
        scanStrLine = lineNum;
        rtrn = IDENT;
    }

    if ((ch != EOF) && (!isspace(ch)))
        ungetc(ch, yyin);
    else if (ch == '\n')
        lineNum++;

    return rtrn;
}

static int
yyGetNumber(int ch)
{
    int isFloat = 0;

    buf[0] = ch;
    nInBuf = 1;
    while (((ch = getc(yyin)) != EOF) &&
           (isxdigit(ch) || ((nInBuf == 1) && (ch == 'x')))) {
        buf[nInBuf++] = ch;
    }
    if (ch == '.') {
        isFloat = 1;
        buf[nInBuf++] = ch;
        while (((ch = getc(yyin)) != EOF) && (isxdigit(ch))) {
            buf[nInBuf++] = ch;
        }
    }
    buf[nInBuf++] = '\0';
    if ((ch != EOF) && (!isspace(ch)))
        ungetc(ch, yyin);

    if (isFloat) {
        float tmp;

        if (sscanf(buf, "%g", &tmp) == 1) {
            scanInt = tmp * XkbGeomPtsPerMM;
            return FLOAT;
        }
    }
    else if (sscanf(buf, "%i", &scanInt) == 1)
        return INTEGER;
    fprintf(stderr, "Malformed number %s\n", buf);
    return ERROR;
}

int
yylex(void)
{
    int ch;
    int rtrn;

    do {
        ch = getc(yyin);
        if (ch == '\n') {
            lineNum++;
        }
        else if (ch == '/') {   /* handle C++ style double-/ comments */
            int newch = getc(yyin);

            if (newch == '/') {
                do {
                    ch = getc(yyin);
                } while ((ch != '\n') && (ch != EOF));
                lineNum++;
            }
            else if (newch != EOF) {
                ungetc(newch, yyin);
            }
        }
    } while ((ch != EOF) && (isspace(ch)));
    if (ch == '=')
        rtrn = EQUALS;
    else if (ch == '+')
        rtrn = PLUS;
    else if (ch == '-')
        rtrn = MINUS;
    else if (ch == '/')
        rtrn = DIVIDE;
    else if (ch == '*')
        rtrn = TIMES;
    else if (ch == '{')
        rtrn = OBRACE;
    else if (ch == '}')
        rtrn = CBRACE;
    else if (ch == '(')
        rtrn = OPAREN;
    else if (ch == ')')
        rtrn = CPAREN;
    else if (ch == '[')
        rtrn = OBRACKET;
    else if (ch == ']')
        rtrn = CBRACKET;
    else if (ch == '.')
        rtrn = DOT;
    else if (ch == ',')
        rtrn = COMMA;
    else if (ch == ';')
        rtrn = SEMI;
    else if (ch == '!')
        rtrn = EXCLAM;
    else if (ch == '~')
        rtrn = INVERT;
    else if (ch == '"')
        rtrn = yyGetString();
    else if (ch == '<')
        rtrn = yyGetKeyName();
    else if (isalpha(ch) || (ch == '_'))
        rtrn = yyGetIdent(ch);
    else if (isdigit(ch))
        rtrn = yyGetNumber(ch);
    else if (ch == EOF)
        rtrn = END_OF_FILE;
    else {
        fprintf(stderr, "Unexpected character %c (%d) in input stream\n",
                ch, ch);
        rtrn = ERROR;
    }
#ifdef DEBUG
    if (debugFlags & 0x2)
        fprintf(stderr, "scan: %s\n", tokText(rtrn));
#endif
    return rtrn;
}
