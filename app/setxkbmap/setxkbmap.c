/************************************************************
 Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <limits.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBconfig.h>
#include <X11/extensions/XKBrules.h>

#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif

#ifndef DFLT_XKB_CONFIG_ROOT
#define DFLT_XKB_CONFIG_ROOT "/usr/share/X11/xkb"
#endif
#ifndef DFLT_XKB_RULES_FILE
#define DFLT_XKB_RULES_FILE "base"
#endif
#ifndef DFLT_XKB_LAYOUT
#define DFLT_XKB_LAYOUT "us"
#endif
#ifndef DFLT_XKB_MODEL
#define DFLT_XKB_MODEL "pc105"
#endif

/* Constants to state how a value was obtained. The order of these
 * is important, the bigger the higher the priority.
 * e.g. FROM_CONFIG overrides FROM_SERVER */
enum source {
    UNDEFINED = 0,
    FROM_SERVER,          /* Retrieved from server at runtime. */
    FROM_RULES,           /* Xkb rules file. */
    FROM_CONFIG,          /* Command-line specified config file. */
    FROM_CMD_LINE,        /* Specified at the cmdline. */
    NUM_SOURCES
};

/***====================================================================***/
static Bool print = False;
static Bool query = False;
static Bool synch = False;
static int verbose = 5;

static Display *dpy;

/**
 * human-readable versions of FROM_CONFIG, FROM_SERVER, etc. Used for error
 * reporting.
 */
static const char *srcName[NUM_SOURCES] = {
    "undefined", "X server", "rules file", "config file", "command line"
};

struct setting {
    char const  *name;  /* Human-readable setting name. Used for error reporting. */
    char        *value; /* Holds the value. */
    enum source  src;   /* Holds the source. */
};

typedef struct setting setting_t;

struct settings {
    setting_t rules;     /* Rules file */
    setting_t config;    /* Config file (if used) */
    setting_t display;   /* X display name */
    setting_t locale;    /* Machine's locale */
    setting_t model;
    setting_t layout;
    setting_t variant;
    setting_t keycodes;
    setting_t types;
    setting_t compat;
    setting_t symbols;
    setting_t geometry;
    setting_t keymap;
};

typedef struct settings settings_t;

static settings_t settings = {
    { "rules file",         NULL, UNDEFINED },
    { "config file",        NULL, UNDEFINED },
    { "X display",          NULL, UNDEFINED },
    { "locale",             NULL, UNDEFINED },
    { "keyboard model",     NULL, UNDEFINED },
    { "keyboard layout",    NULL, UNDEFINED },
    { "layout variant",     NULL, UNDEFINED },
    { "keycodes",           NULL, UNDEFINED },
    { "types",              NULL, UNDEFINED },
    { "compatibility map",  NULL, UNDEFINED },
    { "symbols",            NULL, UNDEFINED },
    { "geometry",           NULL, UNDEFINED },
    { "keymap",             NULL, UNDEFINED }
};

static XkbConfigRtrnRec cfgResult;

static XkbRF_VarDefsRec rdefs;

static Bool clearOptions = False;

struct list {
    char  **item;   /* Array of items. */
    int     sz;     /* Size of array. */
    int     num;    /* Number of used elements. */
};

typedef struct list list_t;

static list_t options = { NULL, 0, 0 };

static list_t inclPath = { NULL, 0, 0 };

static XkbDescPtr xkb = NULL;

static int deviceSpec = XkbUseCoreKbd;

/***====================================================================***/

#define streq(s1,s2)    (strcmp(s1,s2)==0)
#define strpfx(s1,s2)   (strncmp(s1,s2,strlen(s2))==0)

#define MSG(s)          printf(s)
#define MSG1(s,a)       printf(s,a)
#define MSG2(s,a,b)     printf(s,a,b)
#define MSG3(s,a,b,c)   printf(s,a,b,c)

#define VMSG(l,s)        if (verbose>(l)) printf(s)
#define VMSG1(l,s,a)     if (verbose>(l)) printf(s,a)
#define VMSG2(l,s,a,b)   if (verbose>(l)) printf(s,a,b)
#define VMSG3(l,s,a,b,c) if (verbose>(l)) printf(s,a,b,c)

#define ERR(s)          fprintf(stderr,s)
#define ERR1(s,a)       fprintf(stderr,s,a)
#define ERR2(s,a,b)     fprintf(stderr,s,a,b)
#define ERR3(s,a,b,c)   fprintf(stderr,s,a,b,c)

#define OOM(ptr)        do { if ((ptr) == NULL) { ERR("Out of memory.\n"); exit(-1); } } while (0)

/***====================================================================***/

Bool addToList(list_t *list, const char *newVal);
void usage(int argc, char **argv);
void dumpNames(Bool wantRules, Bool wantCNames);
void trySetString(setting_t * setting, char *newVal, enum source src);
Bool setOptString(int *arg, int argc, char **argv, setting_t *setting, enum source src);
int parseArgs(int argc, char **argv);
Bool getDisplay(int argc, char **argv);
Bool getServerValues(void);
FILE *findFileInPath(char *name);
Bool addStringToOptions(char *opt_str, list_t *opts);
char *stringFromOptions(char *orig, list_t *newOpts);
Bool applyConfig(char *name);
XkbRF_RulesPtr tryLoadRules(char *name, char *locale, Bool wantDesc, Bool wantRules);
Bool applyRules(void);
Bool applyComponentNames(void);
void printKeymap(void);

/***====================================================================***/

/*
    If newVal is NULL or empty string, the list is cleared.
    Otherwise newVal is added to the end of the list (if it is not present in the list yet).
*/

Bool
addToList(list_t *list, const char *newVal)
{
    register int i;

    if ((!newVal) || (!newVal[0]))
    {
        list->num = 0;
        return True;
    }
    for (i = 0; i < list->num; i++)
    {
        if (streq(list->item[i], newVal))
            return True;
    }
    if ((list->item == NULL) || (list->sz < 1))
    {
        list->num = 0;
        list->sz = 4;
        list->item = (char **) calloc(list->sz, sizeof(char *));
        OOM(list->item);
    }
    else if (list->num >= list->sz)
    {
        list->sz *= 2;
        list->item = (char **) realloc(list->item, (list->sz) * sizeof(char *));
        OOM(list->item);
    }
    list->item[list->num] = strdup(newVal);
    OOM(list->item[list->num]);
    list->num += 1;
    return True;
}

/***====================================================================***/

void
usage(int argc, char **argv)
{
    MSG1(
        "Usage: %s [options] [<layout> [<variant> [<option> ... ]]]\n"
        "Options:\n"
        "  -?, -help           Print this message\n"
        "  -compat <name>      Specifies compatibility map component name\n"
        "  -config <file>      Specifies configuration file to use\n"
        "  -device <deviceid>  Specifies the device ID to use\n"
        "  -display <dpy>      Specifies display to use\n"
        "  -geometry <name>    Specifies geometry component name\n"
        "  -I <dir>            Add <dir> to list of directories to be used\n"
        "  -keycodes <name>    Specifies keycodes component name\n"
        "  -keymap <name>      Specifies name of keymap to load\n"
        "  -layout <name>      Specifies layout used to choose component names\n"
        "  -model <name>       Specifies model used to choose component names\n"
        "  -option <name>      Adds an option used to choose component names\n"
        "  -print              Print a complete xkb_keymap description and exit\n"
        "  -query              Print the current layout settings and exit\n"
        "  -rules <name>       Name of rules file to use\n"
        "  -symbols <name>     Specifies symbols component name\n"
        "  -synch              Synchronize request with X server\n"
        "  -types <name>       Specifies types component name\n"
        "  -v[erbose] [<lvl>]  Sets verbosity (1..10); higher values yield more messages\n"
        "  -version            Print the program's version number\n"
        "  -variant <name>     Specifies layout variant used to choose component names\n",
        argv[0]
    );
}

void
dumpNames(Bool wantRules, Bool wantCNames)
{
    if (wantRules)
    {
        if (settings.rules.value)
            MSG1("rules:      %s\n", settings.rules.value);
        if (settings.model.value)
            MSG1("model:      %s\n", settings.model.value);
        if (settings.layout.value)
            MSG1("layout:     %s\n", settings.layout.value);
        if (settings.variant.value)
            MSG1("variant:    %s\n", settings.variant.value);
        if (options.item)
        {
            char *opt_str = stringFromOptions(NULL, &options);
            MSG1("options:    %s\n", opt_str);
            free(opt_str);
        }
    }
    if (wantCNames)
    {
        if (settings.keymap.value)
            MSG1("keymap:     %s\n", settings.keymap.value);
        if (settings.keycodes.value)
            MSG1("keycodes:   %s\n", settings.keycodes.value);
        if (settings.types.value)
            MSG1("types:      %s\n", settings.types.value);
        if (settings.compat.value)
            MSG1("compat:     %s\n", settings.compat.value);
        if (settings.symbols.value)
            MSG1("symbols:    %s\n", settings.symbols.value);
        if (settings.geometry.value)
            MSG1("geometry:   %s\n", settings.geometry.value);
    }
    return;
}

/***====================================================================***/

/**
 * Set the given string (obtained from src) in the svValue/svSrc globals.
 * If the given item is already set, it is overridden if the original source
 * is less significant than the given one.
 *
 * @param which What value is it (one of RULES_NDX, CONFIG_NDX, ...)
 */
void
trySetString(setting_t *setting, char *newVal, enum source src)
{
    if (setting->value != NULL)
    {
        if (setting->src == src)
        {
            VMSG2(0, "Warning! More than one %s from %s\n",
                  setting->name, srcName[src]);
            VMSG2(0, "         Using \"%s\", ignoring \"%s\"\n",
                  setting->value, newVal);
            return;
        }
        else if (setting->src > src)
        {
            VMSG1(5, "Warning! Multiple definitions of %s\n", setting->name);
            VMSG2(5, "         Using %s, ignoring %s\n",
                  srcName[setting->src], srcName[src]);
            return;
        }
    }
    setting->src = src;
    setting->value = newVal;
    return;
}

Bool
setOptString(int *arg, int argc, char **argv, setting_t *setting, enum source src)
{
    int ndx;
    char *opt;

    ndx = *arg;
    opt = argv[ndx];
    if (ndx >= argc - 1)
    {
        VMSG1(0, "No %s specified on the command line\n", setting->name);
        VMSG1(0, "Trailing %s option ignored\n", opt);
        return True;
    }
    ndx++;
    *arg = ndx;
    if (setting->value != NULL)
    {
        if (setting->src == src)
        {
            VMSG2(0, "More than one %s on %s\n", setting->name, srcName[src]);
            VMSG2(0, "Using \"%s\", ignoring \"%s\"\n", setting->value,
                  argv[ndx]);
            return True;
        }
        else if (setting->src > src)
        {
            VMSG1(5, "Multiple definitions of %s\n", setting->name);
            VMSG2(5, "Using %s, ignoring %s\n", srcName[setting->src],
                  srcName[src]);
            return True;
        }
    }
    setting->src = src;
    setting->value = argv[ndx];
    return True;
}

/***====================================================================***/

/**
 * Parse commandline arguments.
 * Return True on success or False if an unrecognized option has been
 * specified.
 */
int
parseArgs(int argc, char **argv)
{
    int i;
    Bool ok;
    unsigned present;

    ok = True;
    addToList(&inclPath, ".");
    addToList(&inclPath, DFLT_XKB_CONFIG_ROOT);
    for (i = 1; (i < argc) && ok; i++)
    {
        if (argv[i][0] != '-')
        {
            /* Allow a call like "setxkbmap us" to work. Layout is default,
               if -layout is given, then try parsing variant, then options */
            if (!settings.layout.src)
                trySetString(&settings.layout, argv[i], FROM_CMD_LINE);
            else if (!settings.variant.src)
                trySetString(&settings.variant, argv[i], FROM_CMD_LINE);
            else
                ok = addToList(&options, argv[i]);
        }
        else if (streq(argv[i], "-compat"))
            ok = setOptString(&i, argc, argv, &settings.compat, FROM_CMD_LINE);
        else if (streq(argv[i], "-config"))
            ok = setOptString(&i, argc, argv, &settings.config, FROM_CMD_LINE);
        else if (streq(argv[i], "-device"))
        {
            if ( ++i < argc ) {
                deviceSpec = atoi(argv[i]); /* only allow device IDs, not names */
            } else {
                usage(argc, argv);
                exit(-1);
            }
        }
        else if (streq(argv[i], "-display"))
            ok = setOptString(&i, argc, argv, &settings.display, FROM_CMD_LINE);
        else if (streq(argv[i], "-geometry"))
            ok = setOptString(&i, argc, argv, &settings.geometry, FROM_CMD_LINE);
        else if (streq(argv[i], "-help") || streq(argv[i], "-?"))
        {
            usage(argc, argv);
            exit(0);
        }
        else if (streq(argv[i], "-I")) /* space between -I and path */
        {
            if ( ++i < argc )
                ok = addToList(&inclPath, argv[i]);
            else
                VMSG(0, "No directory specified on the command line\n"
                     "Trailing -I option ignored\n");
        }
        else if (strpfx(argv[i], "-I")) /* no space between -I and path */
            ok = addToList(&inclPath, &argv[i][2]);
        else if (streq(argv[i], "-keycodes"))
            ok = setOptString(&i, argc, argv, &settings.keycodes, FROM_CMD_LINE);
        else if (streq(argv[i], "-keymap"))
            ok = setOptString(&i, argc, argv, &settings.keymap, FROM_CMD_LINE);
        else if (streq(argv[i], "-layout"))
            ok = setOptString(&i, argc, argv, &settings.layout, FROM_CMD_LINE);
        else if (streq(argv[i], "-model"))
            ok = setOptString(&i, argc, argv, &settings.model, FROM_CMD_LINE);
        else if (streq(argv[i], "-option"))
        {
            if ((i == argc - 1) || (argv[i + 1][0] == '\0')
                || (argv[i + 1][0] == '-'))
            {
                clearOptions = True;
                ok = addToList(&options, "");
                if (i < argc - 1 && argv[i + 1][0] == '\0')
                    i++;
            }
            else
            {
                ok = addToList(&options, argv[++i]);
            }
        }
        else if (streq(argv[i], "-print"))
            print = True;
        else if (streq(argv[i], "-query"))
            query = True;
        else if (streq(argv[i], "-rules"))
            ok = setOptString(&i, argc, argv, &settings.rules, FROM_CMD_LINE);
        else if (streq(argv[i], "-symbols"))
            ok = setOptString(&i, argc, argv, &settings.symbols, FROM_CMD_LINE);
        else if (streq(argv[i], "-synch"))
            synch = True;
        else if (streq(argv[i], "-types"))
            ok = setOptString(&i, argc, argv, &settings.types, FROM_CMD_LINE);
        else if (streq(argv[i], "-version"))
        {
            MSG1("setxkbmap %s\n", PACKAGE_VERSION);
            exit(0);
        }
        else if (streq(argv[i], "-verbose") || (streq(argv[i], "-v")))
        {
            if ((i < argc - 1) && (isdigit(argv[i + 1][0])))
                verbose = atoi(argv[++i]);
            else
                verbose++;
            if (verbose < 0)
            {
                ERR1("Illegal verbose level %d.  Reset to 0\n", verbose);
                verbose = 0;
            }
            else if (verbose > 10)
            {
                ERR1("Illegal verbose level %d.  Reset to 10\n", verbose);
                verbose = 10;
            }
            VMSG1(7, "Setting verbose level to %d\n", verbose);
        }
        else if (streq(argv[i], "-variant"))
            ok = setOptString(&i, argc, argv, &settings.variant, FROM_CMD_LINE);
        else
        {
            ERR1("Error!   Option \"%s\" not recognized\n", argv[i]);
            ok = False;
        }
    }

    present = 0;
    if (settings.types.value)
        present++;
    if (settings.compat.value)
        present++;
    if (settings.symbols.value)
        present++;
    if (settings.keycodes.value)
        present++;
    if (settings.geometry.value)
        present++;
    if (settings.config.value)
        present++;
    if (settings.model.value)
        present++;
    if (settings.layout.value)
        present++;
    if (settings.variant.value)
        present++;
    if (settings.keymap.value && present)
    {
        ERR("No other components can be specified when a keymap is present\n");
        return False;
    }
    return ok;
}

/**
 * Open a connection to the display and print error if it fails.
 *
 * @return True on success or False otherwise.
 */
Bool
getDisplay(int argc, char **argv)
{
    int major, minor, why;

    major = XkbMajorVersion;
    minor = XkbMinorVersion;
    dpy =
        XkbOpenDisplay(settings.display.value, NULL, NULL, &major, &minor,
                       &why);
    if (!dpy)
    {
        if (settings.display.value == NULL)
            settings.display.value = getenv("DISPLAY");
        if (settings.display.value == NULL)
            settings.display.value = "default display";
        switch (why)
        {
        case XkbOD_BadLibraryVersion:
            ERR3("%s was compiled with XKB version %d.%02d\n", argv[0],
                 XkbMajorVersion, XkbMinorVersion);
            ERR2("Xlib supports incompatible version %d.%02d\n",
                 major, minor);
            break;
        case XkbOD_ConnectionRefused:
            ERR1("Cannot open display \"%s\"\n", settings.display.value);
            break;
        case XkbOD_NonXkbServer:
            ERR1("XKB extension not present on %s\n", settings.display.value);
            break;
        case XkbOD_BadServerVersion:
            ERR3("%s was compiled with XKB version %d.%02d\n", argv[0],
                 XkbMajorVersion, XkbMinorVersion);
            ERR3("Server %s uses incompatible version %d.%02d\n",
                 settings.display.value, major, minor);
            break;
        default:
            ERR1("Unknown error %d from XkbOpenDisplay\n", why);
            break;
        }
        return False;
    }
    if (synch)
        XSynchronize(dpy, True);
    return True;
}

/***====================================================================***/

/**
 * Retrieve xkb values from the XKB_RULES_NAMES property and store their
 * contents in svValues.
 * If the property cannot be read, the built-in defaults are used.
 *
 * @return True.
 */
Bool
getServerValues(void)
{
    XkbRF_VarDefsRec vd;
    char *tmp = NULL;

    if (!XkbRF_GetNamesProp(dpy, &tmp, &vd) || !tmp)
    {
        VMSG1(3, "Couldn't interpret %s property\n", _XKB_RF_NAMES_PROP_ATOM);
        tmp = DFLT_XKB_RULES_FILE;
        vd.model = DFLT_XKB_MODEL;
        vd.layout = DFLT_XKB_LAYOUT;
        vd.variant = NULL;
        vd.options = NULL;
        VMSG3(3, "Use defaults: rules - '%s' model - '%s' layout - '%s'\n",
              tmp, vd.model, vd.layout);
    }
    if (tmp)
        trySetString(&settings.rules, tmp, FROM_SERVER);
    if (vd.model)
        trySetString(&settings.model, vd.model, FROM_SERVER);
    if (vd.layout)
        trySetString(&settings.layout, vd.layout, FROM_SERVER);
    if (vd.variant)
        trySetString(&settings.variant, vd.variant, FROM_SERVER);
    if ((vd.options) && (!clearOptions))
    {
        addStringToOptions(vd.options, &options);
        XFree(vd.options);
    }
    return True;
}

/***====================================================================***/

FILE *
findFileInPath(char *name)
{
    register int i;
    char buf[PATH_MAX];
    FILE *fp;

    if (name[0] == '/')
    {
        fp = fopen(name, "r");
        if ((verbose > 7) || ((!fp) && (verbose > 0)))
            MSG2("%s file %s\n", (fp ? "Found" : "Didn't find"), name);
        return fp;
    }
    for (i = 0; (i < inclPath.num); i++)
    {
        if (snprintf(buf, PATH_MAX, "%s/%s", inclPath.item[i], name) >=
            PATH_MAX)
        {
            VMSG2(0, "Path too long (%s/%s). Ignored.\n", inclPath.item[i],
                  name);
            continue;
        }
        fp = fopen(buf, "r");
        if ((verbose > 7) || ((!fp) && (verbose > 5)))
            MSG2("%s file %s\n", (fp ? "Found" : "Didn't find"), buf);
        if (fp != NULL)
            return fp;
    }
    return NULL;
}

/***====================================================================***/

Bool
addStringToOptions(char *opt_str, list_t *opts)
{
    char *tmp, *str, *next;
    Bool ok = True;

    str = strdup(opt_str);
    OOM(str);
    for (tmp = str; (tmp && *tmp != '\0') && ok; tmp = next)
    {
        next = strchr(str, ',');
        if (next)
        {
            *next = '\0';
            next++;
        }
        ok = addToList(opts, tmp) && ok;
    }
    free(str);
    return ok;
}

/***====================================================================***/

char *
stringFromOptions(char *orig, list_t *newOpts)
{
    size_t len;
    int i, nOut;

    if (orig)
        len = strlen(orig) + 1;
    else
        len = 0;
    for (i = 0; i < newOpts->num; i++)
    {
        if (newOpts->item[i])
            len += strlen(newOpts->item[i]) + 1;
    }
    if (len < 1)
        return NULL;
    if (orig)
    {
        orig = (char *) realloc(orig, len);
        OOM(orig);
        nOut = 1;
    }
    else
    {
        orig = (char *) calloc(len, 1);
        OOM(orig);
        nOut = 0;
    }
    for (i = 0; i < newOpts->num; i++)
    {
        if (!newOpts->item[i])
            continue;
        if (nOut > 0)
        {
            strcat(orig, ",");
            strcat(orig, newOpts->item[i]);
        }
        else
            strcpy(orig, newOpts->item[i]);
        nOut++;
    }
    return orig;
}

/***====================================================================***/

Bool
applyConfig(char *name)
{
    FILE *fp;
    Bool ok;

    if ((fp = findFileInPath(name)) == NULL)
        return False;
    ok = XkbCFParse(fp, XkbCFDflts, NULL, &cfgResult);
    fclose(fp);
    if (!ok)
    {
        ERR1("Couldn't find configuration file \"%s\"\n", name);
        return False;
    }
    if (cfgResult.rules_file)
    {
        trySetString(&settings.rules, cfgResult.rules_file, FROM_CONFIG);
        cfgResult.rules_file = NULL;
    }
    if (cfgResult.model)
    {
        trySetString(&settings.model, cfgResult.model, FROM_CONFIG);
        cfgResult.model = NULL;
    }
    if (cfgResult.layout)
    {
        trySetString(&settings.layout, cfgResult.layout, FROM_CONFIG);
        cfgResult.layout = NULL;
    }
    if (cfgResult.variant)
    {
        trySetString(&settings.variant, cfgResult.variant, FROM_CONFIG);
        cfgResult.variant = NULL;
    }
    if (cfgResult.options)
    {
        addStringToOptions(cfgResult.options, &options);
        cfgResult.options = NULL;
    }
    if (cfgResult.keymap)
    {
        trySetString(&settings.keymap, cfgResult.keymap, FROM_CONFIG);
        cfgResult.keymap = NULL;
    }
    if (cfgResult.keycodes)
    {
        trySetString(&settings.keycodes, cfgResult.keycodes, FROM_CONFIG);
        cfgResult.keycodes = NULL;
    }
    if (cfgResult.geometry)
    {
        trySetString(&settings.geometry, cfgResult.geometry, FROM_CONFIG);
        cfgResult.geometry = NULL;
    }
    if (cfgResult.symbols)
    {
        trySetString(&settings.symbols, cfgResult.symbols, FROM_CONFIG);
        cfgResult.symbols = NULL;
    }
    if (cfgResult.types)
    {
        trySetString(&settings.types, cfgResult.types, FROM_CONFIG);
        cfgResult.types = NULL;
    }
    if (cfgResult.compat)
    {
        trySetString(&settings.compat, cfgResult.compat, FROM_CONFIG);
        cfgResult.compat = NULL;
    }
    if (verbose > 5)
    {
        MSG("After config file:\n");
        dumpNames(True, True);
    }
    return True;
}

XkbRF_RulesPtr
tryLoadRules(char *name, char *locale, Bool wantDesc, Bool wantRules)
{
    XkbRF_RulesPtr rules = NULL;
    VMSG1(7, "Trying to load rules file %s...\n", name);
    rules = XkbRF_Load(name, locale, wantDesc, wantRules);
    if (rules)
    {
        VMSG(7, "Success.\n");
    }
    return rules;
}

/**
 * If any of model, layout, variant or options is specified, then compile the
 * options into the
 *
 * @return True on success or false otherwise.
 */
Bool
applyRules(void)
{
    int i;
    char *rfName;
    XkbRF_RulesPtr rules = NULL;

    if (settings.model.src || settings.layout.src || settings.variant.src
        || options.item)
    {
        char buf[PATH_MAX];
        XkbComponentNamesRec rnames;

        if (settings.variant.src < settings.layout.src)
            settings.variant.value = NULL;

        rdefs.model = settings.model.value;
        rdefs.layout = settings.layout.value;
        rdefs.variant = settings.variant.value;
        if (options.item)
            rdefs.options =
                stringFromOptions(rdefs.options, &options);

        if (settings.rules.src)
            rfName = settings.rules.value;
        else
            rfName = DFLT_XKB_RULES_FILE;

        if (rfName[0] == '/')
        {
            rules = tryLoadRules(rfName, settings.locale.value, True, True);
        }
        else
        {
            /* try to load rules files from all include paths until the first
             * we succeed with */
            for (i = 0; (i < inclPath.num) && (!rules); i++)
            {
                if (snprintf(buf, PATH_MAX, "%s/rules/%s",
                             inclPath.item[i], rfName) >= PATH_MAX)
                {
                    VMSG2(0, "Path too long (%s/rules/%s). Ignored.\n",
                          inclPath.item[i], rfName);
                    continue;
                }
                rules = tryLoadRules(buf, settings.locale.value, True, True);
            }
        }
        if (!rules)
        {
            ERR1("Couldn't find rules file (%s) \n", rfName);
            return False;
        }
        /* Let the rules file to the magic, then update the svValues with
         * those returned after processing the rules */
        XkbRF_GetComponents(rules, &rdefs, &rnames);
        if (rnames.keycodes)
        {
            trySetString(&settings.keycodes, rnames.keycodes, FROM_RULES);
            rnames.keycodes = NULL;
        }
        if (rnames.symbols)
        {
            trySetString(&settings.symbols, rnames.symbols, FROM_RULES);
            rnames.symbols = NULL;
        }
        if (rnames.types)
        {
            trySetString(&settings.types, rnames.types, FROM_RULES);
            rnames.types = NULL;
        }
        if (rnames.compat)
        {
            trySetString(&settings.compat, rnames.compat, FROM_RULES);
            rnames.compat = NULL;
        }
        if (rnames.geometry)
        {
            trySetString(&settings.geometry, rnames.geometry, FROM_RULES);
            rnames.geometry = NULL;
        }
        if (rnames.keymap)
        {
            trySetString(&settings.keymap, rnames.keymap, FROM_RULES);
            rnames.keymap = NULL;
        }
        if (verbose > 6)
        {
            MSG1("Applied rules from %s:\n", rfName);
            dumpNames(True, False);
        }
    }
    else if (verbose > 6)
    {
        MSG("No rules variables specified.  Rules file ignored\n");
    }
    return True;
}

/* Primitive sanity check - filter out 'map names' (inside parenthesis) */
/* that can confuse xkbcomp parser */
static Bool
checkName(char *name, const char *string)
{
    char *i = name, *opar = NULL;
    Bool ret = True;

    if (!name)
        return True;

    while (*i)
    {
        if (opar == NULL)
        {
            if (*i == '(')
                opar = i;
        }
        else
        {
            if ((*i == '(') || (*i == '|') || (*i == '+'))
            {
                ret = False;
                break;
            }
            if (*i == ')')
                opar = NULL;
        }
        i++;
    }
    if (opar)
        ret = False;
    if (!ret)
    {
        char c;
        int n = 1;
        for (i = opar + 1; *i && n; i++)
        {
            if (*i == '(')
                n++;
            if (*i == ')')
                n--;
        }
        if (*i)
            i++;
        c = *i;
        *i = '\0';
        ERR1("Illegal map name '%s' ", opar);
        *i = c;
        ERR2("in %s name '%s'\n", string, name);
    }
    return ret;
}

void
printKeymap(void)
{
    MSG("xkb_keymap {\n");
    if (settings.keycodes.value)
        MSG1("\txkb_keycodes  { include \"%s\"\t};\n", settings.keycodes.value);
    if (settings.types.value)
        MSG1("\txkb_types     { include \"%s\"\t};\n", settings.types.value);
    if (settings.compat.value)
        MSG1("\txkb_compat    { include \"%s\"\t};\n", settings.compat.value);
    if (settings.symbols.value)
        MSG1("\txkb_symbols   { include \"%s\"\t};\n", settings.symbols.value);
    if (settings.geometry.value)
        MSG1("\txkb_geometry  { include \"%s\"\t};\n", settings.geometry.value);
    MSG("};\n");
}

Bool
applyComponentNames(void)
{
    if (!checkName(settings.types.value, "types"))
        return False;
    if (!checkName(settings.compat.value, "compat"))
        return False;
    if (!checkName(settings.symbols.value, "symbols"))
        return False;
    if (!checkName(settings.keycodes.value, "keycodes"))
        return False;
    if (!checkName(settings.geometry.value, "geometry"))
        return False;
    if (!checkName(settings.keymap.value, "keymap"))
        return False;

    if (verbose > 5)
    {
        MSG("Trying to build keymap using the following components:\n");
        dumpNames(False, True);
    }
    /* Upload the new description to the server. */
    if (dpy && !print && !query)
    {
        XkbComponentNamesRec cmdNames = {
            .keymap = settings.keymap.value,
            .keycodes = settings.keycodes.value,
            .types = settings.types.value,
            .compat = settings.compat.value,
            .symbols = settings.symbols.value,
            .geometry = settings.geometry.value
        };

        xkb = XkbGetKeyboardByName(dpy, deviceSpec, &cmdNames,
                                   XkbGBN_AllComponentsMask,
                                   XkbGBN_AllComponentsMask &
                                   (~XkbGBN_GeometryMask), True);
        if (!xkb)
        {
            ERR("Error loading new keyboard description\n");
            return False;
        }
        /* update the XKB root property */
        if (settings.rules.value && (rdefs.model || rdefs.layout))
        {
            if (!XkbRF_SetNamesProp(dpy, settings.rules.value, &rdefs))
            {
                VMSG(0, "Error updating the XKB names property\n");
            }
        }
    }
    if (print)
    {
        printKeymap();
    }
    if (query)
    {
        dumpNames(True, False);
    }
    return True;
}


int
main(int argc, char **argv)
{
    if ((!parseArgs(argc, argv)) || (!getDisplay(argc, argv)))
        exit(-1);
    settings.locale.value = setlocale(LC_ALL, settings.locale.value);
    settings.locale.src = FROM_SERVER;
    VMSG1(7, "locale is %s\n", settings.locale.value);
    if (dpy)
        getServerValues();
    if (settings.config.value && (!applyConfig(settings.config.value)))
        exit(-3);
    if (!applyRules())
        exit(-4);
    if (!applyComponentNames())
        exit(-5);
    if (dpy)
        XCloseDisplay(dpy);
    exit(0);
}
