
/*
 * $Xorg: xpxthelloworld.c,v 1.1 2002/07/28 08:44:26 gisburn Exp $
 * 
 * xpxthelloworld - Xprint version of hello world using Athena widgets
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

#include <X11/IntrinsicP.h> 
#include <X11/ShellP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h>

#include <X11/XprintUtil/xprintutil.h>

#include <stdlib.h>
#include <stdio.h>

/* Turn a NULL pointer string into an empty string */
#define NULLSTR(x) (((x)!=NULL)?(x):(""))

#define Error(x) { printf x ; exit(EXIT_FAILURE); }
#define Log(x) { if(verbose) printf x; }
#define Msg(x) { if(!quiet)  printf x; }

/* Local prototypes */
static void PrintSpoolerCommandResults(Display *pdpy, XPContext pcontext);
static void redisplayWidget(Widget widget);
static void MyAppMainLoop(XtAppContext app, Widget printwidget);
static int  do_hello_world( int argc, char *argv[], const char *printername, const char *toFile );

/* Global vars */
const char *ProgramName;                /* program name (from argv[0]) */
Bool        verbose            = False; /* verbose output what the program is doing */
Bool        quiet              = False; /* be quiet (no output except errors) */
Bool        done               = False; /* Done with printing ? */
Bool        doPrint            = False; /* Do we print on a printer ? */
int         xp_eventbase,               /* XpExtension event base */
            xp_errorbase;               /* XpExtension error base */
Display    *pdpy               = NULL;  /* (Paper) display */
Screen     *pscreen            = NULL;  /* (Paper) screen (DDX-specific!) */
XPContext   pcontext           = None;  /* Xprint context  */
void       *printtofile_handle = NULL;  /* XprintUtil "context" when printing to file */
Drawable    pdrawable          = None;  /* paper drawable */
int         numpages           = 0;     /* pages being printed */


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
void MyAppMainLoop(XtAppContext app, Widget printwidget)
{
  XEvent xevent;
  
  /* process events. */
  while( !done ) 
  {   
    XtAppNextEvent(app, &xevent);
  
    if( XtDispatchEvent(&xevent) == False )
    {
      /* XpExtension event ? */
      if( xevent.type == xp_eventbase+XPPrintNotify )
      {
        XPPrintEvent *pev = (XPPrintEvent *)&xevent;
        
        Log(("--> got XPPrintEvent\n"));

        switch( pev->detail )
        {
          case XPStartJobNotify:
            Log(("Starting page...\n"));
            XpStartPage(pdpy, pdrawable);
            break;
          case XPEndJobNotify:
            /* Job done... */
            done = True;
            break;
          case XPStartPageNotify:
            Log(("Rendering page...\n"));
            redisplayWidget(printwidget);

            Log(("Page end reached.\n"));
            XpEndPage(pdpy);
            break;
          case XPEndPageNotify:
            /* next page or exit */
            numpages++;
            
            if( numpages >= 1 )
            {
              Log(("Finishing job...\n"));
              XpEndJob(pdpy);
            }
            break;
          default:
            Log(("--> other XPPrintEvent event\n"));
            break;
        }
      }
      else
      {
        Log(("--> other event\n"));
      }  
    }
  }
}

/* Code from the OpenMotif sources (XmRedisplayWidget) */
static
void redisplayWidget(Widget widget) 
{
    XExposeEvent xev ;
    Region region ;

    xev.type = Expose ;
                 /* is this better than 0 ? shouldn't make much difference
                  * unless the expose method is very tricky... */
    xev.serial = LastKnownRequestProcessed(XtDisplay(widget)) ;  
    xev.send_event = False ;
    xev.display    = XtDisplay(widget);
    xev.window     = XtWindowOfObject(widget);  /* work with gadget too */
    xev.x          = 0 ;
    xev.y          = 0 ;
    xev.width      = widget->core.width ;
    xev.height     = widget->core.height ;
    xev.count      = 0 ;

    region = XCreateRegion();
    XtAddExposureToRegion((XEvent*)&xev, region);    

    if (widget->core.widget_class->core_class.expose)
        (*(widget->core.widget_class->core_class.expose))
            (widget, (XEvent*)&xev, region);

    XDestroyRegion(region);
}

static 
void usage( void )
{
    fprintf (stderr, "usage:  %s [options]\n", ProgramName);
    fprintf (stderr, "-print\tPrint via Xprint instead of displaying on the Xserver\n");
    fprintf (stderr, "-printer printernname\tprinter to use\n");
    fprintf (stderr, "-printfile file\tprint to file instead of printer\n");
    fprintf (stderr, "-v\tverbose output\n");
    fprintf (stderr, "-q\tbe quiet (no output except errors)\n");
    fprintf (stderr, "\n");
    exit(EXIT_FAILURE);
}

int main( int argc, char *argv[] )
{
    const char    *printername = NULL;  /* printer to query */
    const char    *toFile      = NULL;  /* output file (instead of printer) */
    XPPrinterList  plist;               /* list of printers */
    int            plist_count;         /* number of entries in |plist|-array */
    int            i;
    int            retval;

    ProgramName = argv[0];

    for( i = 1 ; i < argc ; i++ )
    {
      char *arg = argv[i];
      int len = strlen(arg);
    
      if (!strncmp("-print", arg, len))
      {
        doPrint = True;
      }
      else if (!strncmp("-printer", arg, len))
      {
        if (++i >= argc)
          usage();
        printername = argv[i];
        doPrint = True;
      }
      else if (!strncmp("-printfile", arg, len))
      {
        if (++i >= argc)
          usage();
        toFile = argv[i];
        doPrint = True;
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
    
    if( doPrint )
    {
      plist = XpuGetPrinterList(printername, &plist_count);

      if (!plist) {
        fprintf(stderr, "%s:  no printers found for printer spec \"%s\".\n",
           ProgramName, NULLSTR(printername));
        exit(EXIT_FAILURE);
      }
    
      Log(("Using printer '%s'\n", plist[0].name));
    
      retval = do_hello_world(argc, argv, plist[0].name, toFile);
    
      XpuFreePrinterList(plist);
    }
    else
    {
      Log(("Displaying on framebuffer Xserver\n"));

      retval = do_hello_world(argc, argv, NULL, NULL);
    }
    
    return(retval);
}

/* xt_xp_openapplication() - mainly identical to XtOpenApplication() but
 * takes a |Display *| and |Screen *| as arguments, too... */
static
Widget xt_xp_openapplication(XtAppContext *app_context_return,
                             Display      *dpy,
                             Screen       *screen,
                             String        application_name,
                             String        application_class,
                             WidgetClass   widget_class,
                             int          *argc,
                             String       *argv)
{
    Widget   toplevel;
    Cardinal n;
    Arg      args[2];

    XtToolkitInitialize();
    *app_context_return = XtCreateApplicationContext();
    if( *app_context_return == NULL )
      return NULL;
      
    XtDisplayInitialize(*app_context_return, dpy,
                        application_name, application_class,
                        NULL, 0,
                        argc, argv);

    n = 0;
    XtSetArg(args[n], XtNscreen, screen); n++;
    toplevel = XtAppCreateShell(application_name, 
                                application_class,
                                widget_class,
                                dpy,
                                args, n);

    return toplevel;
}

int do_hello_world( int argc, char *argv[], const char *printername, const char *toFile )
{
    XtAppContext     app;
    Widget           toplevel,
                     hello;
    long             dpi_x = 0L,
                     dpi_y = 0L;
    char             fontname[256]; /* BUG: is this really big enougth ? */
    XFontStruct     *labelFont;
    Cardinal         n;
    Arg              args[10];
       
    if( doPrint )
    {
      /* Get printer, either by "name" (foobar) or "name@display" (foobar@gaja:5) */
      if( XpuGetPrinter(printername, &pdpy, &pcontext) != 1 )
        Error(("XpuGetPrinter failure.\n"));

      if( XpQueryExtension(pdpy, &xp_eventbase, &xp_errorbase) == False )
        Error(("XpQueryExtension failure.\n"));

      XpSelectInput(pdpy, pcontext, XPPrintMask);
      
      /* Configure the print context (paper size, title etc.)
       * We must do this before creating any Xt widgets - otherwise they will
       * make wrong assuptions about fonts, resultions etc. ...
       */
      XpuSetJobTitle(pdpy, pcontext, "Simple Xprint Athena widget demo");
      
      /* Configuration done, set the context */
      XpSetContext(pdpy, pcontext);

      /* Get default printer resolution */   
      if( XpuGetResolution(pdpy, pcontext, &dpi_x, &dpi_y) != 1 )
      {
        fprintf(stderr, "No default resolution for printer '%s'\n", printername);
        XpuClosePrinterDisplay(pdpy, pcontext);
        return(EXIT_FAILURE);
      }

      pscreen = XpGetScreenOfContext(pdpy, pcontext);
    }
    else
    {
      pdpy = XOpenDisplay(NULL);
      if( !pdpy )
        Error(("XOpenDisplay failure.\n"));

      dpi_x = dpi_y = 0L;
      
      pscreen = XDefaultScreenOfDisplay(pdpy);
    }  

    toplevel = xt_xp_openapplication(&app, 
                                     pdpy, pscreen,
                                     "xpxtawhenademo", "XpXtAthenaDemo",
                                     applicationShellWidgetClass,
                                     &argc, argv);

    if( !toplevel )
      Error(("xt_xp_openapplication failure.\n"));
    
    sprintf(fontname, "-*-*-*-*-*-*-*-180-%ld-%ld-*-*-iso8859-1", dpi_x, dpi_y);
    labelFont = XLoadQueryFont(pdpy, fontname);
    if( !labelFont )
      Error(("XLoadQueryFont failure.\n"));

    n = 0;
    XtSetArg(args[n], XtNlabel, "Hello world\n"
                                "(Xprint/Athena widget version)"); n++;
    XtSetArg(args[n], XtNfont,  labelFont);                        n++;
    hello = XtCreateManagedWidget(
              "hello",            /* arbitrary widget name */
              labelWidgetClass,   /* widget class from Label.h */
              toplevel,           /* parent widget*/
              args,               /* argument list */
              n                   /* arg list size */
              );
    if( !hello )
      Error(("XtCreateManagedWidget failure.\n"));

    XtRealizeWidget(toplevel);
    
    if( doPrint )
    {
      unsigned short pwidth, pheight;
      XRectangle     pagerect;
     
      pdpy      = XtDisplay(toplevel);
      pdrawable = XtWindow(toplevel);
      if( !pdpy || !pdrawable )
        Error(("No display.\n"));
      
      /* Make sure that the Xt machinery is really using the right screen (assertion) ... */
      if( XpGetScreenOfContext(XtDisplay(toplevel), pcontext) != XtScreen(toplevel) )
        Error(("Widget's screen != print screen. BAD.\n"));
        
      /* Get the page dimensions and resize the widget based on that info ... */
      XpGetPageDimensions(pdpy, pcontext, &pwidth, &pheight, &pagerect);
      XMoveResizeWindow(pdpy, pdrawable, pagerect.x, pagerect.y, pagerect.width, pagerect.height);
      
      /* ... and then start the print job. */
      if( toFile )
      {
        printtofile_handle = XpuStartJobToFile(pdpy, pcontext, toFile);
        if( !printtofile_handle )
        {
          perror("XpuStartJobToFile failure");
          Error(("XpuStartJobToFile failure."));
        }
      }
      else
      {
        XpuStartJobToSpooler(pdpy);
      }
           
      numpages = 0;
    }
    
    MyAppMainLoop(app, toplevel);
    
    if( doPrint )
    {
      if( toFile )
      {
        if( XpuWaitForPrintFileChild(printtofile_handle) != XPGetDocFinished )
        {
          fprintf(stderr, "%s: Error while printing to file.\n", ProgramName);
        }
      }
      
      PrintSpoolerCommandResults(pdpy, pcontext);
    
      /* We have to use XpDestroyContext() and XtCloseDisplay() instead
       * of XpuClosePrinterDisplay() to make libXt happy... */
      if( pcontext != None )
        XpDestroyContext(pdpy, pcontext);
      XtCloseDisplay(pdpy);
    }
   
    return EXIT_SUCCESS;
}

