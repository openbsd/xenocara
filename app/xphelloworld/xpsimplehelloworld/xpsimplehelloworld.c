/*
 * $Xorg: xphelloworld.c,v 1.2 2002/05/10 06:54:^1 gisburn Exp $
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
#include <X11/XprintUtil/xprintutil.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Turn a NULL pointer string into an empty string */
#define NULLSTR(x) (((x)!=NULL)?(x):(""))

#define Log(x) { if(verbose) printf x; }
#define Msg(x) { if(!quiet)  printf x; }

static const char *ProgramName;      /* program name (from argv[0]) */
static Bool        verbose = False;  /* verbose output what the program is doing */
Bool        quiet   = False;  /* be quiet (no output except errors) */

static 
void usage(void)
{
    fprintf (stderr, "usage:  %s [options]\n", ProgramName);
    fprintf (stderr, "-printer printernname\tprinter to use\n");
    fprintf (stderr, "-printfile file\tprint to file instead of printer\n");
    fprintf (stderr, "-embedpsl2data string\tPostScript level 2 fragment to embed\n"
                     "\t\t(use 'xppsembeddemo1' to embed demo data)\n");
    fprintf (stderr, "-v\tverbose output\n");
    fprintf (stderr, "-q\tbe quiet (no output except errors)\n");
    fprintf (stderr, "\n");
    exit(EXIT_FAILURE);
}

/* strstr(), case-insensitive */
static 
char *str_case_str(const char *s, const char *find)
{
  size_t len;
  char   c, 
         sc;

  if ((c = tolower(*find++)) != '\0')
  {
    len = strlen(find);
    do 
    {
      do
      {
        if ((sc = tolower(*s++)) == '\0')
          return NULL;
      } while (sc != c);
    } while (strncasecmp(s, find, len) != 0);
    s--;
  }
  return ((char *)s);
}

static
int do_hello_world(const char *printername, const char *printerfile, const char *psembeddata )
{
    Display       *pdpy;               /* X connection */
    XPContext      pcontext;           /* Xprint context */
    void          *printtofile_handle; /* "context" when printing to file */
    int            xp_event_base,      /* XpExtension even base */
                   xp_error_base;      /* XpExtension error base */
    long           dpi_x = 0L,
                   dpi_y = 0L;
    Screen        *pscreen;
    int            pscreennumber;
    Window         pwin;
    XGCValues      gcvalues;
    GC             pgc;
    unsigned short dummy;
    XRectangle     winrect;
    char           fontname[256]; /* BUG: is this really big enougth ? */
    XFontStruct   *font;
    char          *scr;

    if( XpuGetPrinter(printername, &pdpy, &pcontext) != 1 )
    {
      fprintf(stderr, "Cannot open printer '%s'\n", printername);
      return(EXIT_FAILURE);
    }
    
    if( XpQueryExtension(pdpy, &xp_event_base, &xp_error_base) == False )
    {
      fprintf(stderr, "XpQueryExtension() failed.\n");
      XpuClosePrinterDisplay(pdpy, pcontext);
      return(EXIT_FAILURE);
    }

    /* Listen to XP(Start|End)(Job|Doc|Page)Notify events).
     * This is mantatory as Xp(Start|End)(Job|Doc|Page) functions are _not_ 
     * syncronous !!
     * Not waiting for such events may cause that subsequent data may be 
     * destroyed/corrupted!!
     */
    XpSelectInput(pdpy, pcontext, XPPrintMask);
    
    /* Set job title */
    XpuSetJobTitle(pdpy, pcontext, "Hello world for Xprint");
 
    /* Set print context
     * Note that this modifies the available fonts, including builtin printer prints.
     * All XListFonts()/XLoadFont() stuff should be done _after_ setting the print 
     * context to obtain the proper fonts.
     */ 
    XpSetContext(pdpy, pcontext);

    /* Get default printer reolution */   
    if( XpuGetResolution(pdpy, pcontext, &dpi_x, &dpi_y) != 1 )
    {
      fprintf(stderr, "No default resolution for printer '%s'.\n", printername);
      XpuClosePrinterDisplay(pdpy, pcontext);
      return(EXIT_FAILURE);
    }
      
    if( printerfile )
    {
      Log(("starting job (to file '%s').\n", printerfile));
      printtofile_handle = XpuStartJobToFile(pdpy, pcontext, printerfile);
      if( !printtofile_handle )
      {
        fprintf(stderr, "%s: Error: %s while trying to print to file.\n", 
                ProgramName, strerror(errno));
        XpuClosePrinterDisplay(pdpy, pcontext);
        return(EXIT_FAILURE);
      }
      
      XpuWaitForPrintNotify(pdpy, xp_event_base, XPStartJobNotify);
    }
    else
    {
      Log(("starting job.\n"));
      XpuStartJobToSpooler(pdpy);    
      XpuWaitForPrintNotify(pdpy, xp_event_base, XPStartJobNotify);
    }

#ifdef MULTIPLE_DOCUMENTS_IN_ONE_JOB
    /* Start document (one job can contain any number of "documents")
     * XpStartDoc() isn't mandatory if job only contains one document - first 
     * XpStartPage() will generate a "synthetic" XpStartDoc() if one had not 
     * already been done.
     */   
    XpStartDoc(pdpy, XPDocNormal);
    XpuWaitForPrintNotify(pdpy, xp_event_base, XPStartDocNotify);    
#endif /* MULTIPLE_DOCUMENTS_IN_ONE_JOB */

    pscreen = XpGetScreenOfContext(pdpy, pcontext);
    pscreennumber = XScreenNumberOfScreen(pscreen);
    
    /* Obtain some info about page geometry */
    XpGetPageDimensions(pdpy, pcontext, &dummy, &dummy, &winrect);

    pwin = XCreateSimpleWindow(pdpy,  XRootWindowOfScreen(pscreen),
                               winrect.x, winrect.y, winrect.width, winrect.height,
                               10,
                               XBlackPixel(pdpy, pscreennumber),
                               XWhitePixel(pdpy, pscreennumber));

    gcvalues.background = XWhitePixel(pdpy, pscreennumber);
    gcvalues.foreground = XBlackPixel(pdpy, pscreennumber);
            
    pgc = XCreateGC(pdpy, pwin, GCBackground|GCForeground, &gcvalues);
      
    Log(("start page.\n"));
    XpStartPage(pdpy, pwin);
    XpuWaitForPrintNotify(pdpy, xp_event_base, XPStartPageNotify);

    /* Mapping the window inside XpStartPage()/XpEndPage()
     * Set XCreateWindow/border_width to 0 or move XMapWindow in front of 
     * XpStartPage() to get rid of the surrounding black border lines.
     * (This is usually done before XpStartPage() in real applications)
     */     
    XMapWindow(pdpy, pwin);
      
    /* usual rendering stuff..... */

    sprintf(fontname, "-*-*-*-*-*-*-*-180-%ld-%ld-*-*-iso8859-1", dpi_x, dpi_y);
    font = XLoadQueryFont(pdpy, fontname);
    XSetFont(pdpy, pgc, font->fid);
    XDrawString(pdpy, pwin, pgc, 100, 100, "hello world from X11 print system", 33);

#define DO_EMBED_TEST 1

#ifdef DO_EMBED_TEST
    if( psembeddata )
    {
      char *embedded_formats_supported;
      
      embedded_formats_supported = XpGetOneAttribute(pdpy, pcontext, XPPrinterAttr, "xp-embedded-formats-supported");

      Log(("psembed: xp-embedded-formats-supported='%s'\n", NULLSTR(embedded_formats_supported)));

      /* MAX(XExtendedMaxRequestSize(pdpy), XMaxRequestSize(pdpy)) defines the
       * maximum length of emebdded PostScript data which can be send in one
       * step using XpPutDocumentData() */
      Log(("psembed: XExtendedMaxRequestSize=%ld\n", (long)XExtendedMaxRequestSize(pdpy)));
      Log(("psembed: XMaxRequestSize=%ld\n", (long)XMaxRequestSize(pdpy)));
      
      /* Should we embed the demo ? */
      if( !strcmp(psembeddata, "xppsembeddemo1") )
      {
        Log(("psembed: Using PS embedding demo 1\n"));
        psembeddata = "newpath\n270 360 moveto\n 0 72 rlineto\n"
                      "72 0 rlineto\n 0 -72 rlineto\n closepath\n fill\n";
      }
      else
      {
        Log(("psembed: Using user PS embedding data = '%s'\n", psembeddata));
      }

      /* Check whether "PostScript Level 2" is supported as embedding format
       * (The content of the "xp-embedded-formats-supported" attribute needs
       * to be searched in a case-insensitive way since the model-configs
       * may use the same word with multiple variants of case
       * (e.g. "PostScript" vs. "Postscript" or "PCL" vs. "Pcl" etc.")
       * To avoid problems we simply use |str_case_str()| (case-insensitive
       * strstr()) instead of |strstr()| here...)
       */
      if( embedded_formats_supported &&
          (str_case_str(embedded_formats_supported, "PostScript 2") != NULL) )
      {
        /* Note that the emebdded PostScript code uses the same resolution and
         * coordinate space as currently be used by the DDX (if you don not
         * want that simply reset it yourself :) */
        char *test     = (char *)psembeddata;
        int   test_len = strlen(test);
        char *type     = "PostScript 2"; /* Format of embedded data 
                                          * (older PS DDX may be picky, fixed via
                                          * http://xprint.mozdev.org/bugs/show_bug.cgi?id=4023)
                                          */
        char *option   = "";             /* PostScript DDX does not support any options yet
                                          * (in general |BadValue| will be returned for not
                                          * supported options/option values) */
        XpPutDocumentData(pdpy, pwin, test, test_len, type, option);
      }
      else
      {
        Log(("psembed: error: cannot embed data, 'PostScript 2' not supported as embedded data format for this printer\n"));
      }
    }
#endif /* DO_EMBED_TEST */
    
    XpEndPage(pdpy);
    XpuWaitForPrintNotify(pdpy, xp_event_base, XPEndPageNotify);
    Log(("end page.\n"));
    
#ifdef DO_SOME_MORE_RENDERING
    XpStartPage(pdpy);
    XpuWaitForPrintNotify(pdpy, xp_event_base, XPStartPageNotify);

    /* some more rendering.....   */

    XpEndPage(pdpy);
    XpuWaitForPrintNotify(pdpy, xp_event_base, XPEndPageNotify);
#endif /* DO_SOME_MORE_RENDERING */

#ifdef MULTIPLE_DOCUMENTS_IN_ONE_JOB
    /* End document. Do _not_ use it if you did not explicitly used 
     * XpStartDoc() above (e.g. if XpStartDoc() was triggered by first 
     * XpStartPage() - see comment about XpStartDoc() above... 
     */
    XpEndDoc(pdpy);
    XpuWaitForPrintNotify(pdpy, xp_event_base, XPEndDocNotify);
#endif /* MULTIPLE_DOCUMENTS_IN_ONE_JOB */

    /* End the print job - the final results are sent by the X print
     * server to the spooler sub system.
     */
    XpEndJob(pdpy);
    XpuWaitForPrintNotify(pdpy, xp_event_base, XPEndJobNotify);    
    Log(("end job.\n"));

    if( printerfile )
    {
      if( XpuWaitForPrintFileChild(printtofile_handle) != XPGetDocFinished )
      {
        fprintf(stderr, "%s: Error while printing to file.\n", ProgramName);
        XpuClosePrinterDisplay(pdpy, pcontext);
        return(EXIT_FAILURE);
      }
    }

    /* End of spooled job - get spooler command results and print them */
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
    
    XpuClosePrinterDisplay(pdpy, pcontext);
    return(EXIT_SUCCESS);
}

int main (int argc, char *argv[])
{
    const char    *printername     = NULL;  /* printer to query */
    const char    *toFile          = NULL;  /* output file (instead of printer) */
    const char    *embedpsl2data   = NULL;  /* PS Level 2 code fragment for embedding in output */
    XPPrinterList  plist;                   /* list of printers */
    int            plist_count;             /* number of entries in |plist|-array */
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
      else if (!strncmp("-printfile", arg, len))
      {
        if (++i >= argc)
          usage();
        toFile = argv[i];
      }
      else if (!strncmp("-embedpsl2data", arg, len))
      {
        if (++i >= argc)
          usage();
        embedpsl2data = argv[i];
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
    
    plist = XpuGetPrinterList(printername, &plist_count);

    if (!plist) {
      fprintf(stderr, "%s:  no printers found for printer spec \"%s\".\n",
         ProgramName, NULLSTR(printername));
      exit(EXIT_FAILURE);
    }
    
    Log(("Using printer '%s'\n", plist[0].name));
    
    retval = do_hello_world(plist[0].name, toFile, embedpsl2data);
    
    XpuFreePrinterList(plist);
    
    return(retval);
}


