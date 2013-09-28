/*

Copyright (c) 1993, 1994, 1998 The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

#include "def.h"
#ifdef hpux
#define sigvec sigvector
#endif /* hpux */

#ifdef X_POSIX_C_SOURCE
#define _POSIX_C_SOURCE X_POSIX_C_SOURCE
#include <signal.h>
#undef _POSIX_C_SOURCE
#else
#if defined(X_NOT_POSIX) || defined(_POSIX_SOURCE)
#include <signal.h>
#else
#define _POSIX_SOURCE
#include <signal.h>
#undef _POSIX_SOURCE
#endif
#endif

#include <stdarg.h>

#ifdef __sun
# include <sys/utsname.h>
#endif

#ifdef DEBUG
int	_debugmask;
#endif

/* #define DEBUG_DUMP */
#ifdef DEBUG_DUMP
#define DBG_PRINT(file, fmt, args)   fprintf(file, fmt, args)
#else
#define DBG_PRINT(file, fmt, args)   /* empty */
#endif

#define DASH_INC_PRE    "#include \""
#define DASH_INC_POST   "\""

const char *ProgramName;

const char * const directives[] = {
	"if",
	"ifdef",
	"ifndef",
	"else",
	"endif",
	"define",
	"undef",
	"include",
	"line",
	"pragma",
	"error",
	"ident",
	"sccs",
	"elif",
	"eject",
	"warning",
	"include_next",
	NULL
};

#include "imakemdep.h"

struct	inclist inclist[ MAXFILES ],
		*inclistp = inclist,
		*inclistnext = inclist,
		maininclist;

static char	*filelist[ MAXFILES ];
const char	*includedirs[ MAXDIRS + 1 ],
		**includedirsnext = includedirs;
char		*notdotdot[ MAXDIRS ];
static int	cmdinc_count = 0;
static char	*cmdinc_list[ 2 * MAXINCFILES ];
const char	*objprefix = "";
const char	*objsuffix = OBJSUFFIX;
static const char	*startat = "# DO NOT DELETE";
int		width = 78;
static boolean	append = FALSE;
boolean		printed = FALSE;
boolean		verbose = FALSE;
boolean		show_where_not = FALSE;
/* Warn on multiple includes of same file */
boolean 	warn_multiple = FALSE;

static void setfile_cmdinc(struct filepointer *filep, long count, char **list);
static void redirect(const char *line, const char *makefile);

static void _X_NORETURN
catch (int sig)
{
	fflush (stdout);
	fatalerr ("got signal %d\n", sig);
}

#if defined(USG) || (defined(i386) && defined(SYSV)) || defined(WIN32) || defined(Lynx_22) || defined(__CYGWIN__)
#define USGISH
#endif

#ifndef USGISH
#ifdef X_NOT_POSIX
#define sigaction sigvec
#define sa_handler sv_handler
#define sa_mask sv_mask
#define sa_flags sv_flags
#endif
static struct sigaction sig_act;
#endif /* USGISH */

int
main(int argc, char *argv[])
{
	char	**fp = filelist;
	const char	**incp = includedirs;
	char	*p;
	struct inclist	*ip;
	char	*makefile = NULL;
	struct filepointer	*filecontent;
	const struct symtab *psymp = predefs;
	const char *endmarker = NULL;
	char *defincdir = NULL;
	char **undeflist = NULL;
	int numundefs = 0, i;

	ProgramName = argv[0];

	while (psymp->s_name)
	{
	    define2(psymp->s_name, psymp->s_value, &maininclist);
	    psymp++;
	}
#ifdef __sun
	/* Solaris predefined values that are computed, not hardcoded */
	{
	    struct utsname name;

	    if (uname(&name) >= 0) {
		char osrevdef[SYS_NMLN + SYS_NMLN + 5];
		snprintf(osrevdef, sizeof(osrevdef), "__%s_%s",
			 name.sysname, name.release);

		for (p = osrevdef; *p != '\0'; p++) {
		    if (!isalnum(*p)) {
			*p = '_';
		    }
		}
		define2(osrevdef, "1", &maininclist);
	    }
	}
#endif
	if (argc == 2 && argv[1][0] == '@') {
	    struct stat ast;
	    int afd;
	    char *args;
	    char **nargv;
	    int nargc;
	    char quotechar = '\0';

	    nargc = 1;
	    if ((afd = open(argv[1]+1, O_RDONLY)) < 0)
		fatalerr("cannot open \"%s\"\n", argv[1]+1);
	    fstat(afd, &ast);
	    args = malloc(ast.st_size + 1);
	    if ((ast.st_size = read(afd, args, ast.st_size)) < 0)
		fatalerr("failed to read %s\n", argv[1]+1);
	    args[ast.st_size] = '\0';
	    close(afd);
	    for (p = args; *p; p++) {
		if (quotechar) {
		    if (quotechar == '\\' ||
			(*p == quotechar && p[-1] != '\\'))
			quotechar = '\0';
		    continue;
		}
		switch (*p) {
		case '\\':
		case '"':
		case '\'':
		    quotechar = *p;
		    break;
		case ' ':
		case '\n':
		    *p = '\0';
		    if (p > args && p[-1])
			nargc++;
		    break;
		}
	    }
	    if (p[-1])
		nargc++;
	    nargv = malloc(nargc * sizeof(char *));
	    nargv[0] = argv[0];
	    argc = 1;
	    for (p = args; argc < nargc; p += strlen(p) + 1)
		if (*p) nargv[argc++] = p;
	    argv = nargv;
	}
	for(argc--, argv++; argc; argc--, argv++) {
	    	/* if looking for endmarker then check before parsing */
		if (endmarker && strcmp (endmarker, *argv) == 0) {
		    endmarker = NULL;
		    continue;
		}
		if (**argv != '-') {
			/* treat +thing as an option for C++ */
			if (endmarker && **argv == '+')
				continue;
			*fp++ = argv[0];
			continue;
		}
		switch(argv[0][1]) {
		case '-':
			endmarker = &argv[0][2];
			if (endmarker[0] == '\0') endmarker = "--";
			break;
		case 'D':
			if (argv[0][2] == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -D\n");
				argv++;
				argc--;
			}
			for (p=argv[0] + 2; *p ; p++)
				if (*p == '=') {
					*p = ' ';
					break;
				}
			define(argv[0] + 2, &maininclist);
			break;
		case 'I':
			if (incp >= includedirs + MAXDIRS)
			    fatalerr("Too many -I flags.\n");
			*incp++ = argv[0]+2;
			if (**(incp-1) == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -I\n");
				*(incp-1) = *(++argv);
				argc--;
			}
			break;
		case 'U':
			/* Undef's override all -D's so save them up */
			numundefs++;
			if (numundefs == 1)
			    undeflist = malloc(sizeof(char *));
			else
			    undeflist = realloc(undeflist,
						numundefs * sizeof(char *));
			if (argv[0][2] == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -U\n");
				argv++;
				argc--;
			}
			undeflist[numundefs - 1] = argv[0] + 2;
			break;
		case 'Y':
			defincdir = argv[0]+2;
			break;
		/* do not use if endmarker processing */
		case 'a':
			if (endmarker) break;
			append = TRUE;
			break;
		case 'w':
			if (endmarker) break;
			if (argv[0][2] == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -w\n");
				argv++;
				argc--;
				width = atoi(argv[0]);
			} else
				width = atoi(argv[0]+2);
			break;
		case 'o':
			if (endmarker) break;
			if (argv[0][2] == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -o\n");
				argv++;
				argc--;
				objsuffix = argv[0];
			} else
				objsuffix = argv[0]+2;
			break;
		case 'p':
			if (endmarker) break;
			if (argv[0][2] == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -p\n");
				argv++;
				argc--;
				objprefix = argv[0];
			} else
				objprefix = argv[0]+2;
			break;
		case 'v':
			if (endmarker) break;
			verbose = TRUE;
#ifdef DEBUG
			if (argv[0][2])
				_debugmask = atoi(argv[0]+2);
#endif
			break;
		case 's':
			if (endmarker) break;
			startat = argv[0]+2;
			if (*startat == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -s\n");
				startat = *(++argv);
				argc--;
			}
			if (*startat != '#')
				fatalerr("-s flag's value should start %s\n",
					"with '#'.");
			break;
		case 'f':
			if (endmarker) break;
			makefile = argv[0]+2;
			if (*makefile == '\0') {
				if (argc < 2)
					fatalerr("Missing argument for -f\n");
				makefile = *(++argv);
				argc--;
			}
			break;

		case 'm':
			warn_multiple = TRUE;
			break;

		/* Ignore -O, -g so we can just pass ${CFLAGS} to
		   makedepend
		 */
		case 'O':
		case 'g':
			break;
		case 'i':
			if (strcmp(&argv[0][1],"include") == 0) {
				char *buf;
				if (argc<2)
					fatalerr("option -include is a "
						 "missing its parameter\n");
				if (cmdinc_count >= MAXINCFILES)
					fatalerr("Too many -include flags.\n");
				argc--;
				argv++;
				buf = malloc(strlen(DASH_INC_PRE) +
					     strlen(argv[0]) +
					     strlen(DASH_INC_POST) + 1);
                		if(!buf)
					fatalerr("out of memory at "
						 "-include string\n");
				cmdinc_list[2 * cmdinc_count + 0] = argv[0];
				cmdinc_list[2 * cmdinc_count + 1] = buf;
				cmdinc_count++;
				break;
			}
			/* intentional fall through */
		default:
			if (endmarker) break;
	/*		fatalerr("unknown opt = %s\n", argv[0]); */
			warning("ignoring option %s\n", argv[0]);
		}
	}
	/* Now do the undefs from the command line */
	for (i = 0; i < numundefs; i++)
	    undefine(undeflist[i], &maininclist);
	if (numundefs > 0)
	    free(undeflist);

	if (!defincdir) {
#ifdef PREINCDIR
	    if (incp >= includedirs + MAXDIRS)
		fatalerr("Too many -I flags.\n");
	    *incp++ = PREINCDIR;
#endif
	    if (incp >= includedirs + MAXDIRS)
		fatalerr("Too many -I flags.\n");
	    *incp++ = INCLUDEDIR;

#ifdef EXTRAINCDIR
	    if (incp >= includedirs + MAXDIRS)
		fatalerr("Too many -I flags.\n");
	    *incp++ = EXTRAINCDIR;
#endif

#ifdef POSTINCDIR
	    if (incp >= includedirs + MAXDIRS)
		fatalerr("Too many -I flags.\n");
	    *incp++ = POSTINCDIR;
#endif
	} else if (*defincdir) {
	    if (incp >= includedirs + MAXDIRS)
		fatalerr("Too many -I flags.\n");
	    *incp++ = defincdir;
	}

	redirect(startat, makefile);

	/*
	 * catch signals.
	 */
#ifdef USGISH
/*  should really reset SIGINT to SIG_IGN if it was.  */
#ifdef SIGHUP
	signal (SIGHUP, catch);
#endif
	signal (SIGINT, catch);
#ifdef SIGQUIT
	signal (SIGQUIT, catch);
#endif
	signal (SIGILL, catch);
#ifdef SIGBUS
	signal (SIGBUS, catch);
#endif
	signal (SIGSEGV, catch);
#ifdef SIGSYS
	signal (SIGSYS, catch);
#endif
#else
	sig_act.sa_handler = catch;
#if defined(_POSIX_SOURCE) || !defined(X_NOT_POSIX)
	sigemptyset(&sig_act.sa_mask);
	sigaddset(&sig_act.sa_mask, SIGINT);
	sigaddset(&sig_act.sa_mask, SIGQUIT);
#ifdef SIGBUS
	sigaddset(&sig_act.sa_mask, SIGBUS);
#endif
	sigaddset(&sig_act.sa_mask, SIGILL);
	sigaddset(&sig_act.sa_mask, SIGSEGV);
	sigaddset(&sig_act.sa_mask, SIGHUP);
	sigaddset(&sig_act.sa_mask, SIGPIPE);
#ifdef SIGSYS
	sigaddset(&sig_act.sa_mask, SIGSYS);
#endif
#else
	sig_act.sa_mask = ((1<<(SIGINT -1))
			   |(1<<(SIGQUIT-1))
#ifdef SIGBUS
			   |(1<<(SIGBUS-1))
#endif
			   |(1<<(SIGILL-1))
			   |(1<<(SIGSEGV-1))
			   |(1<<(SIGHUP-1))
			   |(1<<(SIGPIPE-1))
#ifdef SIGSYS
			   |(1<<(SIGSYS-1))
#endif
			   );
#endif /* _POSIX_SOURCE */
	sig_act.sa_flags = 0;
	sigaction(SIGHUP, &sig_act, (struct sigaction *)0);
	sigaction(SIGINT, &sig_act, (struct sigaction *)0);
	sigaction(SIGQUIT, &sig_act, (struct sigaction *)0);
	sigaction(SIGILL, &sig_act, (struct sigaction *)0);
#ifdef SIGBUS
	sigaction(SIGBUS, &sig_act, (struct sigaction *)0);
#endif
	sigaction(SIGSEGV, &sig_act, (struct sigaction *)0);
#ifdef SIGSYS
	sigaction(SIGSYS, &sig_act, (struct sigaction *)0);
#endif
#endif /* USGISH */

	/*
	 * now peruse through the list of files.
	 */
	for(fp=filelist; *fp; fp++) {
		DBG_PRINT(stderr,"file: %s\n",*fp);
		filecontent = getfile(*fp);
		setfile_cmdinc(filecontent, cmdinc_count, cmdinc_list);
		ip = newinclude(*fp, (char *)NULL);

		find_includes(filecontent, ip, ip, 0, FALSE);
		freefile(filecontent);
		recursive_pr_include(ip, ip->i_file, base_name(*fp));
		inc_clean();
	}
	if (printed)
		printf("\n");
	return 0;
}


struct filepointer *
getfile(const char *file)
{
	int	fd;
	struct filepointer	*content;
	struct stat	st;

	content = malloc(sizeof(struct filepointer));
	content->f_name = file;
	if ((fd = open(file, O_RDONLY)) < 0) {
		warning("cannot open \"%s\"\n", file);
		content->f_p = content->f_base = content->f_end = malloc(1);
		*content->f_p = '\0';
		return(content);
	}
	fstat(fd, &st);
	content->f_base = malloc(st.st_size+1);
	if (content->f_base == NULL)
		fatalerr("cannot allocate mem\n");
	if ((st.st_size = read(fd, content->f_base, st.st_size)) < 0)
		fatalerr("failed to read %s\n", file);
	close(fd);
	content->f_len = st.st_size+1;
	content->f_p = content->f_base;
	content->f_end = content->f_base + st.st_size;
	*content->f_end = '\0';
	content->f_line = 0;
	content->cmdinc_count = 0;
	content->cmdinc_list = NULL;
	content->cmdinc_line = 0;
	return(content);
}

void
setfile_cmdinc(struct filepointer* filep, long count, char** list)
{
	filep->cmdinc_count = count;
	filep->cmdinc_list = list;
	filep->cmdinc_line = 0;
}

void
freefile(struct filepointer *fp)
{
	free(fp->f_base);
	free(fp);
}

int
match(const char *str, const char * const *list)
{
	int	i;

	for (i=0; *list; i++, list++)
		if (strcmp(str, *list) == 0)
			return(i);
	return(-1);
}

/*
 * Get the next line.  We only return lines beginning with '#' since that
 * is all this program is ever interested in.
 */
char *getnextline(struct filepointer *filep)
{
	char	*p,	/* walking pointer */
		*eof,	/* end of file pointer */
		*bol;	/* beginning of line pointer */
	int	lineno;	/* line number */
	boolean whitespace = FALSE;

	/*
	 * Fake the "-include" line files in form of #include to the
	 * start of each file.
	 */
	if (filep->cmdinc_line < filep->cmdinc_count) {
		char *inc = filep->cmdinc_list[2 * filep->cmdinc_line + 0];
		char *buf = filep->cmdinc_list[2 * filep->cmdinc_line + 1];
		filep->cmdinc_line++;
		sprintf(buf,"%s%s%s",DASH_INC_PRE,inc,DASH_INC_POST);
		DBG_PRINT(stderr,"%s\n",buf);
		return(buf);
	}

	p = filep->f_p;
	eof = filep->f_end;
	if (p >= eof)
		return((char *)NULL);
	lineno = filep->f_line;

	for (bol = p--; ++p < eof; ) {
		if ((bol == p) && ((*p == ' ') || (*p == '\t')))
		{
			/* Consume leading white-spaces for this line */
			while (((p+1) < eof) && ((*p == ' ') || (*p == '\t')))
			{
				p++;
				bol++;
			}
			whitespace = TRUE;
		}

		if (*p == '/' && (p+1) < eof && *(p+1) == '*') {
			/* Consume C comments */
			*(p++) = ' ';
			*(p++) = ' ';
			while (p < eof && *p) {
				if (*p == '*' && (p+1) < eof && *(p+1) == '/') {
					*(p++) = ' ';
					*(p++) = ' ';
					break;
				}
				if (*p == '\n')
					lineno++;
				*(p++) = ' ';
			}
			--p;
		}
		else if (*p == '/' && (p+1) < eof && *(p+1) == '/') {
			/* Consume C++ comments */
			*(p++) = ' ';
			*(p++) = ' ';
			while (p < eof && *p) {
				if (*p == '\\' && (p+1) < eof &&
				    *(p+1) == '\n') {
					*(p++) = ' ';
					lineno++;
				}
				else if (*p == '?' && (p+3) < eof &&
					 *(p+1) == '?' &&
					 *(p+2) == '/' &&
					 *(p+3) == '\n') {
					*(p++) = ' ';
					*(p++) = ' ';
					*(p++) = ' ';
					lineno++;
				}
				else if (*p == '\n')
					break;	/* to process end of line */
				*(p++) = ' ';
			}
			--p;
		}
		else if (*p == '\\' && (p+1) < eof && *(p+1) == '\n') {
			/* Consume backslash line terminations */
			*(p++) = ' ';
			*p = ' ';
			lineno++;
		}
		else if (*p == '?' && (p+3) < eof &&
			 *(p+1) == '?' && *(p+2) == '/' && *(p+3) == '\n') {
			/* Consume trigraph'ed backslash line terminations */
			*(p++) = ' ';
			*(p++) = ' ';
			*(p++) = ' ';
			*p = ' ';
			lineno++;
		}
		else if (*p == '\n') {
			lineno++;
			if (*bol == '#') {
				char *cp;

				*(p++) = '\0';
				/* punt lines with just # (yacc generated) */
				for (cp = bol+1;
				     *cp && (*cp == ' ' || *cp == '\t'); cp++);
				if (*cp) goto done;
				--p;
			}
			bol = p+1;
			whitespace = FALSE;
		}
	}
	if (*bol != '#')
		bol = NULL;
done:
	filep->f_p = p;
	filep->f_line = lineno;
#ifdef DEBUG_DUMP
	if (bol)
		DBG_PRINT(stderr,"%s\n",bol);
#endif
	return(bol);
}

/*
 * Strip the file name down to what we want to see in the Makefile.
 * It will have objprefix and objsuffix around it.
 */
char *base_name(const char *in_file)
{
	char	*p;
	char	*file = strdup(in_file);
	for(p=file+strlen(file); p>file && *p != '.'; p--) ;

	if (*p == '.')
		*p = '\0';
	return(file);
}

#ifndef HAVE_RENAME
int rename (char *from, char *to)
{
    (void) unlink (to);
    if (link (from, to) == 0) {
	unlink (from);
	return 0;
    } else {
	return -1;
    }
}
#endif /* !HAVE_RENAME */

static void
redirect(const char *line, const char *makefile)
{
	struct stat	st;
	FILE	*fdin, *fdout;
	char	backup[ BUFSIZ ],
		buf[ BUFSIZ ];
	boolean	found = FALSE;
	size_t	len;

	/*
	 * if makefile is "-" then let it pour onto stdout.
	 */
	if (makefile && *makefile == '-' && *(makefile+1) == '\0') {
		puts(line);
		return;
	}

	/*
	 * use a default if makefile is not specified.
	 */
	if (!makefile) {
		if (stat("Makefile", &st) == 0)
			makefile = "Makefile";
		else if (stat("makefile", &st) == 0)
			makefile = "makefile";
		else
			fatalerr("[mM]akefile is not present\n");
	}
	else {
		if (stat(makefile, &st) != 0)
			fatalerr("\"%s\" is not present\n", makefile);
	}

	snprintf(backup, sizeof(backup), "%s.bak", makefile);
	unlink(backup);

	/* rename() won't work on WIN32, CYGWIN, or CIFS if src file is open */
	if (rename(makefile, backup) < 0)
		fatalerr("cannot rename %s to %s\n", makefile, backup);
	if ((fdin = fopen(backup, "r")) == NULL) {
		if (rename(backup, makefile) < 0)
			warning("renamed %s to %s, but can't move it back\n",
				makefile, backup);
		fatalerr("cannot open \"%s\"\n", makefile);
	}
	if ((fdout = freopen(makefile, "w", stdout)) == NULL)
		fatalerr("cannot open \"%s\"\n", backup);
	len = strlen(line);
	while (!found && fgets(buf, BUFSIZ, fdin)) {
		if (*buf == '#' && strncmp(line, buf, len) == 0)
			found = TRUE;
		fputs(buf, fdout);
	}
	if (!found) {
		if (verbose)
		warning("Adding new delimiting line \"%s\" and dependencies...\n",
			line);
		puts(line); /* same as fputs(fdout); but with newline */
	} else if (append) {
	    while (fgets(buf, BUFSIZ, fdin)) {
		fputs(buf, fdout);
	    }
	}
	fflush(fdout);
#ifndef HAVE_FCHMOD
	chmod(makefile, st.st_mode);
#else
        fchmod(fileno(fdout), st.st_mode);
#endif /* HAVE_FCHMOD */
}

void
fatalerr(const char *msg, ...)
{
	va_list args;
	fprintf(stderr, "%s: error:  ", ProgramName);
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
	exit (1);
}

void
warning(const char *msg, ...)
{
	va_list args;
	fprintf(stderr, "%s: warning:  ", ProgramName);
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
}

void
warning1(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
}
