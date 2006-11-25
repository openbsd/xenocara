/*
 * $Xorg: xphelloworld.c,v 1.1 2002/02/10 22:54:18 gisburn Exp $
 * 
 * xphelloworld - Xprint version of hello world
 *
 * 
Copyright 2002-2004 Roland Mainz <roland.mainz@nrubsig.org>

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
#include <X11/XprintAppUtil/xpapputil.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Turn a NULL pointer string into an empty string */
#define NULLSTR(x) (((x)!=NULL)?(x):(""))

#define Log(x) { if(verbose) printf x; }
#define Msg(x) { if(!quiet)  printf x; }

const char *ProgramName;      /* program name (from argv[0]) */
Bool        verbose = False;  /* verbose output what the program is doing */
Bool        quiet   = False;  /* be quiet (no output except errors) */

static 
void usage( void )
{
    fprintf (stderr, "usage:  %s [options]\n", ProgramName);
    fprintf (stderr, "-printer printernname\tprinter to use\n");
    fprintf (stderr, "-printargs args\t[ arg=value , ... ]\n");
    fprintf (stderr, "\targs:\ttofile=<filename>\n");
    fprintf (stderr, "\t\tpapersize=<paper name>\n");
    fprintf (stderr, "\t\torientation=<orientation>\n");
    fprintf (stderr, "\t\tresolution=<DPI>\n");
    fprintf (stderr, "\t\tplex=<plex>\n");
    fprintf (stderr, "\t\ttitle=<string>\n");
    fprintf (stderr, "-v\tverbose output\n");
    fprintf (stderr, "-q\tbe quiet (no output except errors)\n");
    fprintf (stderr, "-text <string>\ttext to print (in ISO-8859-1)\n");
    fprintf (stderr, "\n");
    exit(EXIT_FAILURE);
}

static
void PrintSpoolerCommandResults( Display *pdpy, XPContext pcontext )
{
    char *scr;

    scr = XpGetOneAttribute(pdpy, pcontext, XPJobAttr, "xp-spooler-command-results");
    if( scr )
    {
      if( strlen(scr) > 0 )
      {
        const char *msg = XpuCompoundTextToXmb(pdpy, scr);
        if( msg )
        {
          Msg(("Spooler command returned '%s'.\n", msg));
          XpuFreeXmbString(msg);
        }
        else
        {
          Msg(("Spooler command returned '%s' (unconverted).\n", scr));
        }
      }

      XFree((void *)scr);
    }
}

static
int do_hello_world( const char *printername, const char *printerargs, const char *hello_world_message )
{
    char            *printerfile = NULL;
    Window           pwin;
    XGCValues        gcvalues;
    GC               pgc;
    unsigned short   dummy;
    XRectangle       winrect;
    char             fontname[256]; /* BUG: is this really big enougth ? */
    XFontStruct      *font;
    XpauContext      *context;
    XpauFlags         docvalueflags = 0UL;
    XpauDocValues  x_docvalues;
    XpauDocValues *docvalues = &x_docvalues;
    XpAuErrorValue   result;
    
    XpauFlags         jobvalueflags = 0UL;
    XpauJobValues  x_jobvalues;
    XpauJobValues *jobvalues = &x_jobvalues;
    memset(&x_jobvalues, 0, sizeof(x_jobvalues));
    memset(&x_docvalues, 0, sizeof(x_docvalues));
     
    context = XpauGetContext(printername);
    if( !context )
    {
      fprintf(stderr, "XpauGetContext() failure.\n");
      return(EXIT_FAILURE);
    }

    if( (result = XpauParseArgs(context, &jobvalueflags, jobvalues, &docvalueflags, docvalues, &printerfile, printerargs)) != XpAuError_success )
    {
      fprintf(stderr, "XpuParseArgs() failure: %s.\n", XpAuErrorValueToString(result));
      XpauReleaseContext(context);
      return(EXIT_FAILURE);
    }
    
    if( (result = XpauSetJobValues(context, jobvalueflags, jobvalues)) != XpAuError_success )
    {
      fprintf(stderr, "XpauSetJobValues() failure: %s.\n", XpAuErrorValueToString(result));
      XpauReleaseContext(context);
      return(EXIT_FAILURE);
    }
    
    if( (result = XpauSetDocValues(context, docvalueflags, docvalues)) != XpAuError_success )
    {
      fprintf(stderr, "XpauSetDocValues() failure: %s.\n", XpAuErrorValueToString(result));
      XpauReleaseContext(context);
      return(EXIT_FAILURE);
    }        
    
    /* Listen to XP(Start|End)(Job|Doc|Page)Notify events).
     * This is mantatory as Xp(Start|End)(Job|Doc|Page) functions are _not_ 
     * syncronous !!
     * Not waiting for such events may cause that subsequent data may be 
     * destroyed/corrupted!!
     */
    XpSelectInput(context->pdpy, context->pcontext, XPPrintMask);
         
    if( printerfile )
    {
      Log(("starting job (to file '%s').\n", printerfile));
    }
    else
    {
      Log(("starting job.\n"));
    }

    if( (result = XpauStartJob(context, printerfile)) != XpAuError_success )
    {
      fprintf(stderr, "%s: Error: %s while trying to print.\n", 
              ProgramName, XpAuErrorValueToString(result));
      XpauReleaseContext(context);
      return(EXIT_FAILURE);
    }

    XpauWaitForPrintNotify(context, XPStartJobNotify);
  
    /* Obtain some info about page geometry */
    XpGetPageDimensions(context->pdpy, context->pcontext, &dummy, &dummy, &winrect);

    pwin = XCreateSimpleWindow(context->pdpy,  XRootWindowOfScreen(context->pscreen),
                               winrect.x, winrect.y, winrect.width, winrect.height,
                               10,
                               XBlackPixel(context->pdpy, context->pscreennumber),
                               XWhitePixel(context->pdpy, context->pscreennumber));

    gcvalues.background = XWhitePixel(context->pdpy, context->pscreennumber);
    gcvalues.foreground = XBlackPixel(context->pdpy, context->pscreennumber);
            
    pgc = XCreateGC(context->pdpy, pwin, GCBackground|GCForeground, &gcvalues);
      
    Log(("start page.\n"));
    XpauStartPage(context, pwin);
    XpauWaitForPrintNotify(context, XPStartPageNotify);

    /* Mapping the window inside XpStartPage()/XpEndPage()
     * Set XCreateWindow/border_width to 0 or move XMapWindow in front of 
     * XpStartPage() to get rid of the surrounding black border lines.
     * (This is usually done before XpStartPage() in real applications)
     */     
    XMapWindow(context->pdpy, pwin);
      
    /* usual rendering stuff..... */

    sprintf(fontname, "-*-*-*-*-*-*-*-180-%ld-%ld-*-*-iso8859-1", context->document_dpi_x, context->document_dpi_y);
    font = XLoadQueryFont(context->pdpy, fontname);
    XSetFont(context->pdpy, pgc, font->fid);
    if (!hello_world_message)
      hello_world_message = "hello world from X11 print system";
    XDrawString(context->pdpy, pwin, pgc, 100, 100, hello_world_message, strlen(hello_world_message));
    
    XpauEndPage(context);
    XpauWaitForPrintNotify(context, XPEndPageNotify);
    Log(("end page.\n"));
    
    Log(("end job.\n"));

    if( (result = XpauEndJob(context)) != XpAuError_success )
    {
      fprintf(stderr, "%s: Error while printing: %s.\n", 
              ProgramName, XpAuErrorValueToString(result));
      XpauReleaseContext(context);
      return(EXIT_FAILURE);
    }

    XpauWaitForPrintNotify(context, XPEndJobNotify);    

    /* end of spooled job - get spooler command results and print them */
    PrintSpoolerCommandResults(context->pdpy, context->pcontext);

    XpauReleaseContext(context);
    return(EXIT_SUCCESS);
}


int main (int argc, char *argv[])
{
    const char    *printername         = NULL;  /* printer to query */
    const char    *printargs           = NULL;
    const char    *hello_world_message = NULL;
    Bool           use_threadsafe_api = False;  /* Use threadsafe API (for debugging) */
    XPPrinterList  plist;               /* list of printers */
    int            plist_count;         /* number of entries in |plist|-array */
    int            i;
    int            retval;

    ProgramName = argv[0];

    for (i = 1; i < argc; i++)
    {
      char *arg = argv[i];
      int len = strlen(arg);
    
      if (!strncmp("-printer", arg, len))
      {
        if (++i >= argc)
          usage();
        printername = argv[i];
      }
      else if (!strncmp("-printargs", arg, len))
      {
        if (++i >= argc)
          usage();
                 
        printargs = argv[i];
      }
      else if (!strncmp("-text", arg, len))
      {
        if (++i >= argc)
          usage();
                 
        hello_world_message = argv[i];
      }
      else if( !strncmp("-debug_use_threadsafe_api", arg, len) )
      {
        use_threadsafe_api = True;
      }
      else if (!strncmp("-v", arg, len))
      {
        verbose = True;
        quiet   = False;
      }
      else if (!strncmp("-q", arg, len))
      {
        verbose = False;
        quiet   = True;
      }
      else
      {
        usage();
      }  
    }

    if( use_threadsafe_api )
    {
      if( !XInitThreads() )
      {
        fprintf(stderr, "%s: XInitThreads() failure.\n", ProgramName);
        exit(EXIT_FAILURE);
      }
    }
        
    plist = XpuGetPrinterList(printername, &plist_count);

    if (!plist) {
      fprintf(stderr, "%s:  no printers found for printer spec \"%s\".\n",
              ProgramName, NULLSTR(printername));
      exit(EXIT_FAILURE);
    }
    
    Log(("Using printer '%s'\n", plist[0].name));
    
    retval = do_hello_world(plist[0].name, printargs, hello_world_message);
    
    XpuFreePrinterList(plist);
    
    return(retval);
}


