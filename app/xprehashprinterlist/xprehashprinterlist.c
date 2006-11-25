/*
 * $Xorg: xprehashprinterlist.c,v 1.1 2004/06/29 02:19:15 gisburn Exp $
 * 
 * xprehashprinterlist - recalculate the list of available printers
 *
 * 
Copyright 2004 Roland Mainz <roland.mainz@nrubsig.org>

All Rights Reserved.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.
 *
 * Author:  Roland Mainz <roland.mainz@nrubsig.org>
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Print.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define Log(x) { if(verbose) printf x; }

/* Turn a NULL pointer string into an empty string */
#define NULLSTR(x) (((x)!=NULL)?(x):(""))

#define BOOL2STR(x) ((x)?("true"):("false"))

static const char *ProgramName;
static Bool        verbose = False;

static 
void usage(void)
{
    fprintf (stderr, "usage:  %s [options]\n", ProgramName);
    fprintf (stderr, "-v\tverbose\n");
    fprintf (stderr, "\n");
    exit(EXIT_FAILURE);
}


static
void rehash_printers(const char *display)
{
    Display *dpy;
    short dummy;
    
    dpy = XOpenDisplay(display);
    if (!dpy) {
        fprintf(stderr, "%s: Cannot open print server display %s\n", ProgramName, display);
        return;
    }

    if (XpQueryVersion(dpy, &dummy, &dummy)) {
        XpRehashPrinterList(dpy);
    } else {
        fprintf(stderr, "%s: server does not have the XpExtension extension\n", ProgramName);
    }
    XCloseDisplay(dpy);
}

/* Copied from xc/lib/XprintUtil/xprintutil.c:
 * List of tokens which can be used to seperate entries in the 
 * $XPSERVERLIST env var */
static const char XPServerListSeparators[] = " \t\v\n\r\f";

int main (int argc, char *argv[])
{
    int            i;
    char          *tok_lasts;
    char          *display;
    char          *sl;
    Bool           use_threadsafe_api = False; /* Use threadsafe API (for debugging) */

    ProgramName = argv[0];

    for( i = 1 ; i < argc ; i++ )
    {
        char *arg = argv[i];
        int   len = strlen(arg);

        if (!strncmp("-v", arg, len)) {
            verbose = True;
        }
        else if(!strncmp("-debug_use_threadsafe_api", arg, len)) {
            use_threadsafe_api = True;
        }
        else
        {
            usage();
        }  
    }

    if (use_threadsafe_api) {
        if (!XInitThreads()) {
            fprintf(stderr, "%s: XInitThreads() failure.\n", ProgramName);
            exit(EXIT_FAILURE);
        }
    }
    
    sl = getenv("XPSERVERLIST");
    if (!sl) {
        fprintf(stderr, "%s: XPSERVERLIST environment variable not set.\n", ProgramName);
        exit(EXIT_FAILURE);
    }
    
    /* Copy the env value since strtok_r() will write to the string */
    sl = strdup(sl);
    if (!sl) {
        fprintf(stderr, "%s: Out of memory.\n", ProgramName);
        exit(EXIT_FAILURE);
    }
      
    for( display = strtok_r(sl, XPServerListSeparators, &tok_lasts) ; 
         display != NULL ; 
         display = strtok_r(NULL, XPServerListSeparators, &tok_lasts) )
    {
        Log(("Rehashing printer list on server '%s'\n", display));
        rehash_printers(display);
    }
    
    free(sl);
    
    return EXIT_SUCCESS;
}


