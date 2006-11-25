/*
 * $Xorg: xlsprinters.c,v 1.1 2002/02/09 22:54:18 gisburn Exp $
 * 
 * xlsprinters - print information about Xprint printers and their attributes
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

/* Turn a NULL pointer string into an empty string */
#define NULLSTR(x) (((x)!=NULL)?(x):(""))

#define BOOL2STR(x) ((x)?("true"):("false"))

static const char *ProgramName;

static 
void usage(void)
{
    fprintf (stderr, "usage:  %s [options]\n", ProgramName);
    fprintf (stderr, "-printer printername\tprinter to use\n");
    fprintf (stderr, "-l\tlist detailed printer info\n");
    fprintf (stderr, "-dump\tdump all available printer attrbutes\n");
    fprintf (stderr, "\n");
    exit(EXIT_FAILURE);
}

static
void dumpAttributes( Display *pdpy, XPContext pcontext )
{
    char *s;
    printf("--> Job\n%s\n",         s=XpuGetJobAttributes(pdpy, pcontext));     XFree(s);
    printf("--> Doc\n%s\n",         s=XpuGetDocAttributes(pdpy, pcontext));     XFree(s);
    printf("--> Page\n%s\n",        s=XpuGetPageAttributes(pdpy, pcontext));    XFree(s);
    printf("--> Printer\n%s\n",     s=XpuGetPrinterAttributes(pdpy, pcontext)); XFree(s);
    printf("--> Server\n%s\n",      s=XpuGetServerAttributes(pdpy, pcontext));  XFree(s);
    printf("image resolution %d\n", (int)XpGetImageResolution(pdpy, pcontext));
}

static
void print_medium_sizes( Display *pdpy, XPContext pcontext )
{
    XpuMediumSourceSizeList list;
    int                     list_count;
    char                   *value;
    int                     i;

    value = XpGetOneAttribute(pdpy, pcontext, XPDocAttr, "default-medium");
    if( value )
    {
      printf("\tdefault-medium=%s\n", NULLSTR(value));
      XFree(value);
    }
    value = XpGetOneAttribute(pdpy, pcontext, XPDocAttr, "default-input-tray");
    if( value )
    {
      printf("\tdefault-input-tray=%s\n", NULLSTR(value));
      XFree(value); 
    }

    list = XpuGetMediumSourceSizeList(pdpy, pcontext, &list_count);
    if( !list )
    {
      fprintf(stderr, "XpuGetMediumSourceSizeList returned NULL\n");
      return;
    }

    for( i = 0 ; i < list_count ; i++ )
    {
      XpuMediumSourceSizeRec *curr = &list[i];
      if( curr->tray_name )
      {
        printf("\tmedium-source-sizes-supported=%s/%s %s %g %g %g %g\n", 
               curr->tray_name, curr->medium_name, BOOL2STR(curr->mbool),
               curr->ma1, curr->ma2, curr->ma3, curr->ma4);
      }
      else
      {
        printf("\tmedium-source-sizes-supported=%s %s %g %g %g %g\n", 
               curr->medium_name, BOOL2STR(curr->mbool), 
               curr->ma1, curr->ma2, curr->ma3, curr->ma4);
      }
    }
  
    XpuFreeMediumSourceSizeList(list);
}


static
void print_resolutions( Display *pdpy, XPContext pcontext )
{
    XpuResolutionList list;
    int               list_count;
    int               i;
    char              *defresname; /* name of default resolution */

    list = XpuGetResolutionList(pdpy, pcontext, &list_count);
    if( !list )
    {
      fprintf(stderr, "XpuGetResolutionList returned NULL\n");
      return;
    }

    defresname = XpGetOneAttribute(pdpy, pcontext, XPDocAttr, "default-printer-resolution");
    if( defresname )
    {
      XpuResolutionRec *res = XpuFindResolutionByName(list, list_count, defresname);
      if( res )
      {
        printf("\tdefault-printer-resolution=%s (%ldx%ld)\n", res->name, res->x_dpi, res->y_dpi);
      }
      else
      {
        fprintf(stderr, "XpuFindResolutionByName() returned no match for default resolution '%s'\n",
                defresname);
      }
      XFree(defresname);
    }

    for( i = 0 ; i < list_count ; i++ )
    {
      XpuResolutionRec *curr = &list[i];
      printf("\tresolution=%s (%ldx%ld)\n", curr->name, curr->x_dpi, curr->y_dpi);
    }
  
    XpuFreeResolutionList(list);
}

static
void print_orientations( Display *pdpy, XPContext pcontext )
{
    char               *default_orientation;
    XpuOrientationList  list;
    int                 list_count;
    int                 i;

    default_orientation = XpGetOneAttribute(pdpy, pcontext, XPDocAttr, "content-orientation"); 
    if( default_orientation )
    {
      printf("\tdefault_orientation=%s\n", default_orientation);
      XFree(default_orientation);
    }

    list = XpuGetOrientationList(pdpy, pcontext, &list_count);
    if( !list || list_count == 0 )
    {
      fprintf(stderr, "XpuGetOrientationList returned NULL\n");
      return;
    }

    for( i = 0 ; i < list_count ; i++ )
    {
      XpuOrientationRec *curr = &list[i];
      printf("\torientation=%s\n", curr->orientation);
    }
  
    XpuFreeOrientationList(list);
}

static
void print_plexes( Display *pdpy, XPContext pcontext )
{
    char        *default_plex;
    XpuPlexList  list;
    int          list_count;
    int          i;

    default_plex = XpGetOneAttribute(pdpy, pcontext, XPDocAttr, "plex");
    if( default_plex )
    {
      printf("\tdefault_plex=%s\n", default_plex);
      XFree(default_plex);
    }

    list = XpuGetPlexList(pdpy, pcontext, &list_count);
    if( !list || list_count == 0 )
    {
      fprintf(stderr, "XpGetOneAttribute returned NULL\n");
      return;
    }

    for( i = 0 ; i < list_count ; i++ )
    {
      XpuPlexRec *curr = &list[i];
      printf("\tplex=%s\n", curr->plex);
    }
  
    XpuFreePlexList(list);
}

static
void print_detailed_printer_info(XPPrinterRec *xp_rec, int detailLevel)
{   
    Display    *pdpy;     /* X connection */
    XPContext   pcontext; /* Xprint context */

    if( detailLevel < 2 )
      return;

    if( XpuGetPrinter(xp_rec->name, &pdpy, &pcontext) != 1 )
    {
      fprintf(stderr, "Cannot open printer '%s'\n", xp_rec->name);
      return;
    }
    
    printf("printer: %s\n", xp_rec->name);
    printf("\tdescription=%s\n", NULLSTR(xp_rec->desc));
    printf("\tmodel-identifier=%s\n", NULLSTR(XpGetOneAttribute(pdpy, pcontext, XPPrinterAttr, "xp-model-identifier")));
  
    print_medium_sizes(pdpy, pcontext);
    print_resolutions(pdpy, pcontext);
    print_orientations(pdpy, pcontext);
    print_plexes(pdpy, pcontext);
    
    if (detailLevel > 100)
      dumpAttributes(pdpy, pcontext);
     
    XpuClosePrinterDisplay(pdpy, pcontext);
}

static
void print_printer_info(XPPrinterRec *xp_rec, int detailLevel)
{   
    printf("printer: %s\n", xp_rec->name);
    
    if( detailLevel < 1 )
      return;
      
    printf("\tdescription=%s\n", NULLSTR(xp_rec->desc));
}

int main (int argc, char *argv[])
{
    char *printername = NULL;  /* printer to query */
    int   details     = 0;
    Bool  use_threadsafe_api = False; /* Use threadsafe API (for debugging) */
    int i;                     /* temp variable:  iterator */
    XPPrinterList plist;       /* list of printers */
    int           plist_count; /* number of entries in |plist|-array */
    

    ProgramName = argv[0];

    for( i = 1 ; i < argc ; i++ )
    {
      char *arg = argv[i];
      int   len = strlen(arg);
    
      if( !strncmp("-printer", arg, len) )
      {
        if (++i >= argc)
          usage ();
        printername = argv[i];
      } 
      else if( !strncmp("-d", arg, len) )
      {
        details = 1;
      }
      else if( !strncmp("-l", arg, len) )
      {
        details = 2;
      }
      else if( !strncmp("-dump", arg, len) )
      {
        details = 255;
      }
      else if( !strncmp("-debug_use_threadsafe_api", arg, len) )
      {
        use_threadsafe_api = True;
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

    for (i = 0; i < plist_count; i++)
    {
      if( details < 2)
        print_printer_info(&plist[i], details);
      else
        print_detailed_printer_info(&plist[i], details);
    }
    
    XpuFreePrinterList(plist);

    return(EXIT_SUCCESS);
}


