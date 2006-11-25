
/*
 * $Xorg: xpawhelloworld.c,v 1.1 2003/07/30 14:44:31 gisburn Exp $
 * 
 * xpawhelloworld - Xprint version of hello world using the
 *                  Athena PrintShell widget class
 *
 * 

Copyright 2003-2004 Roland Mainz <roland.mainz@nrubsig.org>

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
 * Author:  Roland Mainz <roland.mainz@informatik.med.uni-giessen.de>
 */

/*
 * Referencess:
 * http://nscp.upenn.edu/aix4.3html/libs/motiftr/XmPrintShell.htm
 * http://nscp.upenn.edu/aix4.3html/libs/motiftr/XmText.htm
 *
 */

#include <X11/IntrinsicP.h> 
#include <X11/ShellP.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h> 
#include <Xm/Text.h>
#include <X11/Xaw/XawInit.h>
#include <X11/Xaw/Print.h>
#include <X11/XprintUtil/xprintutil.h>

#include <stdlib.h>
#include <stdio.h>

/* Turn a NULL pointer string into an empty string */
#define NULLSTR(x) (((x)!=NULL)?(x):(""))

#define Error(x) { printf x ; exit(EXIT_FAILURE); }
#define Log(x) { if(verbose) printf x; }
#define Msg(x) { if(!quiet)  printf x; }

/* Prototypes */
static int do_hello_world( int argc, char *argv[], const char *printername,
                           const char *toFile, const char *sample_string );

/* Global vars */
const char *ProgramName;                /* program name (from argv[0]) */
Bool        verbose            = False; /* verbose output what the program is doing */
Bool        quiet              = False; /* be quiet (no output except errors) */
Bool        doPrint            = False; /* Do we print on a printer ? */
Display    *pdpy               = NULL;  /* (Paper) display */
Screen     *pscreen            = NULL;  /* (Paper) screen (DDX-specific!) */
XPContext   pcontext           = None;  /* Xprint context  */
void       *printtofile_handle = NULL;  /* XprintUtil "context" when printing to file */
Drawable    pdrawable          = None;  /* paper drawable */

static 
void usage( void )
{
    fprintf(stderr, "usage:  %s [options] string\n", ProgramName);
    fprintf(stderr, "-print\tPrint via Xprint instead of displaying on the Xserver\n");
    fprintf(stderr, "-printer printernname\tprinter to use\n");
    fprintf(stderr, "-printfile file\tprint to file instead of printer\n");
    fprintf(stderr, "-v\tverbose output\n");
    fprintf(stderr, "-q\tbe quiet (no output except errors)\n");
    fprintf(stderr, "\n");
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

int main( int argc, char *argv[] )
{
    const char    *printername = NULL;  /* printer to query */
    const char    *toFile      = NULL;  /* output file (instead of printer) */
    XPPrinterList  plist;               /* list of printers */
    int            plist_count;         /* number of entries in |plist|-array */
    int            i;
    int            retval;
    const char    *sample_string;

    ProgramName = argv[0];
    
    if( argc < 2 )
    {
      usage();
    }

    for( i = 1 ; i < (argc-1) ; i++ )
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
    
    sample_string = argv[argc-1];
    
    if( doPrint )
    {
      plist = XpuGetPrinterList(printername, &plist_count);

      if (!plist) {
        fprintf(stderr, "%s:  no printers found for printer spec \"%s\".\n",
           ProgramName, NULLSTR(printername));
        exit(EXIT_FAILURE);
      }
    
      Log(("Using printer '%s'\n", plist[0].name));
    
      retval = do_hello_world(argc, argv, plist[0].name, toFile, sample_string);
    
      XpuFreePrinterList(plist);
    }
    else
    {
      Log(("Displaying on framebuffer Xserver\n"));

      retval = do_hello_world(argc, argv, NULL, NULL, sample_string);
    }
    
    return retval;
}

typedef struct
{
  int          num_pages;
  Widget       printshell_content;
  int          num_visible_rows;
  XtAppContext appcontext; /* for XtAppSetExitFlag() */
} MyPrintCallbackData;

static
int GetCurrPageNum(Widget printshell)
{
    Cardinal n;
    Arg      args[2];
    int      pagenum = -666; /* bah! */
    
    n = 0;
    XtSetArg(args[n], XawNcurrPageNumInJob, &pagenum); n++;
    XtGetValues(printshell, args, n);

    return pagenum;
}

static
void PrintOnePageCB(Widget pshell, XtPointer context, XtPointer call_data)
{
    MyPrintCallbackData         *mpcd = (MyPrintCallbackData *)context;
    XawPrintShellCallbackStruct *psp  = (XawPrintShellCallbackStruct *)call_data;
    int                          curr_page;

    curr_page = GetCurrPageNum(pshell);
    Log(("-->  PrintOnePageCB, printing page %d of %d\n", curr_page, mpcd->num_pages));
 
    /* Get ready for next page
     * Scroll widget to display the next page (except for the first page :)
     */
    if (!psp->last_page_in_job && curr_page > 1)
    {
      /* XmText allows two solutions to scroll a page down
       * - Either scroll num_rows_per_page down (this is XmText-specific)
       *   or
       * - Call the "next-page" action procedure (this works for all widgets
       *   which support this action proc)
       */
#define USE_ACTION_TO_SCROLL_DOWN 1
#ifdef USE_ACTION_TO_SCROLL_DOWN
      Log(("Scrolling down one page ...\n"));
      XtCallActionProc(mpcd->printshell_content, "next-page", NULL, NULL, 0);
#else
      Log(("Scrolling down %d rows (=one page) ...\n", mpcd->num_visible_rows));
      XmTextScroll(mpcd->printshell_content, mpcd->num_visible_rows);
#endif /* USE_ACTION_TO_SCROLL_DOWN */
    }
 
    if (curr_page == (int)mpcd->num_pages)
    {
      Log(("Printing last page.\n"));
      psp->last_page_in_job = True;
    }

    Log(("PrintOnePageCB: done\n"));
}

static
void PrintStartJobCB(Widget pshell, XtPointer context, XtPointer call_data)
{
    XawPrintShellCallbackStruct *psp = (XawPrintShellCallbackStruct *)call_data;

    Log(("-->  PrintStartJobCB\n"));
}

static
void PrintEndJobCB(Widget pshell, XtPointer context, XtPointer call_data)
{
    MyPrintCallbackData         *mpcd = (MyPrintCallbackData *)context;
    XawPrintShellCallbackStruct *psp = (XawPrintShellCallbackStruct *)call_data;

    Log(("--> PrintEndJobCB\n"));
    
    /* We're done with printing, tell |XtAppMainLoop()| that it can exit */
    XtAppSetExitFlag(mpcd->appcontext);
}

int do_hello_world( int argc, char *argv[], const char *printername, const char *toFile, const char *sample_string )
{
    XtAppContext         app;                                                   
    Widget               toplevel,                                              
                         shell,                                                 
                         print_shell,                                           
                         hello;                                                 
    long                 dpi_x = 0L,
                         dpi_y = 0L;                                                 
    char                 fontname[256]; /* BUG: is this really big enougth ? */ 
    XFontStruct         *textFont;                                              
    XmFontList           textFontList;                                          
    Cardinal             n;                                                     
    Arg                  args[10];                                              
    MyPrintCallbackData  mpcd;
       
    if( doPrint )
    {
      /* Get printer, either by "name" (foobar) or "name@display" (foobar@gaja:5) */
      if( XpuGetPrinter(printername, &pdpy, &pcontext) != 1 )
        Error(("XpuGetPrinter failure.\n"));
     
      /* Configure the print context (paper size, title etc.)
       * We must do this before creating any Xt widgets - otherwise they will
       * make wrong assuptions about fonts, resultions etc. ...
       */
      XpuSetJobTitle(pdpy, pcontext, "Simple Xprint XawPrintShell widget demo");
      
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

    toplevel = XawOpenApplication(&app, 
                                  pdpy, pscreen,
                                  "xpawprintshelldemo", "XpXawPrintShellDemo",
                                  applicationShellWidgetClass,
                                  &argc, argv);

    if( !toplevel )
      Error(("XawOpenApplication failure.\n"));

    if( doPrint )
    {
      n = 0;
      /* Override any geometry resource settings as XawPrintShell adjusts it's size
       * to the current page siue when |XawPrintLAYOUTMODE_DRAWABLEAREA| or
       * |XawPrintLAYOUTMODE_PAGESIZE| are used. */
      XtSetArg(args[n], XtNgeometry,    "+0+0");                          n++;
      XtSetArg(args[n], XawNlayoutMode, XawPrintLAYOUTMODE_DRAWABLEAREA); n++;
      print_shell = XtCreatePopupShell("myprintshell", 
                                       xawPrintShellWidgetClass, 
                                       toplevel, args, n);

      /* we're mapping/unmapping at start/end page time */
      XtSetMappedWhenManaged(print_shell, False);
      
      shell = print_shell;
    }
    else
    {
      shell = toplevel;
    }

    sprintf(fontname, "-adobe-courier-medium-r-normal--40-*-%ld-%ld-*-*-iso8859-1", dpi_x, dpi_y);
    textFont = XLoadQueryFont(pdpy, fontname);
    if( !textFont )
    {          
      sprintf(fontname, "-*-*-*-*-*-*-*-160-%ld-%ld-*-*-iso8859-1", dpi_x, dpi_y);
      textFont = XLoadQueryFont(pdpy, fontname);
    }
    if( !textFont )
      Error(("XLoadQueryFont failure.\n"));
    textFontList = XmFontListCreate(textFont, XmSTRING_DEFAULT_CHARSET);     

    n = 0;
    /* Make sure the cursor is off, current Xprt servers do not seem to like
     * blinking cursors that much... ;-/ */
    XtSetArg(args[n], XmNcursorPositionVisible, False);              n++;
    XtSetArg(args[n], XmNvalue,                 sample_string);      n++;
    XtSetArg(args[n], XmNfontList,              textFontList);       n++;
    XtSetArg(args[n], XmNeditMode,              XmMULTI_LINE_EDIT);  n++;
    
    hello = XmCreateText(shell, "mytext", args, n);
    if( !hello )
      Error(("XmCreateText failure.\n"));
      
    XtManageChild(hello);
    XtRealizeWidget(toplevel);
    XtRealizeWidget(shell);
    
    if( doPrint )
    {
      int   num_total_rows;
      short num_visible_rows;
      int   num_pages;

      pdpy      = XtDisplay(toplevel);
      pdrawable = XtWindow(toplevel);
      if( !pdpy || !pdrawable )
        Error(("No display.\n"));
      
      /* Make sure that the Xt machinery is really using the right screen (assertion) */
      if( XpGetScreenOfContext(XtDisplay(toplevel), pcontext) != XtScreen(toplevel) )
        Error(("Widget's screen != print screen. BAD.\n"));
                  
      /* Get number of rows visible per page and the number of total rows 
       * in the whole text widget... */
      n = 0;
      XtSetArg(args[n], XmNrows,       &num_visible_rows); n++ ;
      XtSetArg(args[n], XmNtotalLines, &num_total_rows);   n++ ;
      XtGetValues(hello, args, n);

      /* Take away one row to match the one-line overlapping used by the
       * "next-page" action proc */      
      num_visible_rows -= 1;

      /* Calculate the number of pages */
      num_pages = (num_total_rows+num_visible_rows-1) / num_visible_rows;
      Log(("Printing %d pages (num_total_rows=%d, num_visible_rows=%d)...\n", 
           num_pages, num_total_rows, num_visible_rows));
      
      /* Prepare our own context data for the print shell callbacks */
      mpcd.num_pages          = num_pages;
      mpcd.printshell_content = hello;
      mpcd.num_visible_rows   = num_visible_rows;
      mpcd.appcontext         = app;
      
      /* Setup the print shell callbacks... */
      XtAddCallback(print_shell,  XawNpageSetupCallback, PrintOnePageCB,  (XtPointer)&mpcd);
      XtAddCallback(print_shell,  XawNstartJobCallback,  PrintStartJobCB, NULL);
      XtAddCallback(print_shell,  XawNendJobCallback,    PrintEndJobCB,   (XtPointer)&mpcd);  

      /* ... and finally start the print job. */
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
    }
    
    XtAppMainLoop(app);
    
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
