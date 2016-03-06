/* 
 * Copyright © 2001 Keith Packard, member of The XFree86 Project, Inc.
 * Copyright © 2002 Hewlett Packard Company, Inc.
 * Copyright © 2006 Intel Corporation
 * Copyright © 2013 NVIDIA Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 * Thanks to Jim Gettys who wrote most of the client side code,
 * and part of the server code for randr.
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>	/* we share subpixel information */
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>
#include <math.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

static char	*program_name;
static Display	*dpy;
static Window	root;
static int	screen = -1;
static Bool	verbose = False;
static Bool	automatic = False;
static Bool	properties = False;
static Bool	grab_server = True;
static Bool	no_primary = False;

static const char *direction[5] = {
    "normal", 
    "left", 
    "inverted", 
    "right",
    "\n"};

static const char *reflections[5] = {
    "normal", 
    "x", 
    "y", 
    "xy",
    "\n"};

/* subpixel order */
static const char *order[6] = {
    "unknown",
    "horizontal rgb",
    "horizontal bgr",
    "vertical rgb",
    "vertical bgr",
    "no subpixels"};

static const struct {
    const char	    *string;
    unsigned long   flag;
} mode_flags[] = {
    { "+HSync", RR_HSyncPositive },
    { "-HSync", RR_HSyncNegative },
    { "+VSync", RR_VSyncPositive },
    { "-VSync", RR_VSyncNegative },
    { "Interlace", RR_Interlace },
    { "DoubleScan", RR_DoubleScan },
    { "CSync",	    RR_CSync },
    { "+CSync",	    RR_CSyncPositive },
    { "-CSync",	    RR_CSyncNegative },
    { NULL,	    0 }
};

static void
usage(void)
{
    printf("usage: %s [options]\n%s", program_name,
           "  where options are:\n"
           "  --display <display> or -d <display>\n"
           "  --help\n"
           "  -o <normal,inverted,left,right,0,1,2,3>\n"
           "            or --orientation <normal,inverted,left,right,0,1,2,3>\n"
           "  -q        or --query\n"
           "  -s <size>/<width>x<height> or --size <size>/<width>x<height>\n"
           "  -r <rate> or --rate <rate> or --refresh <rate>\n"
           "  -v        or --version\n"
           "  -x        (reflect in x)\n"
           "  -y        (reflect in y)\n"
           "  --screen <screen>\n"
           "  --verbose\n"
           "  --current\n"
           "  --dryrun\n"
           "  --nograb\n"
           "  --prop or --properties\n"
           "  --fb <width>x<height>\n"
           "  --fbmm <width>x<height>\n"
           "  --dpi <dpi>/<output>\n"
           "  --output <output>\n"
           "      --auto\n"
           "      --mode <mode>\n"
           "      --preferred\n"
           "      --pos <x>x<y>\n"
           "      --rate <rate> or --refresh <rate>\n"
           "      --reflect normal,x,y,xy\n"
           "      --rotate normal,inverted,left,right\n"
           "      --left-of <output>\n"
           "      --right-of <output>\n"
           "      --above <output>\n"
           "      --below <output>\n"
           "      --same-as <output>\n"
           "      --set <property> <value>\n"
           "      --scale <x>x<y>\n"
           "      --scale-from <w>x<h>\n"
           "      --transform <a>,<b>,<c>,<d>,<e>,<f>,<g>,<h>,<i>\n"
           "      --off\n"
           "      --crtc <crtc>\n"
           "      --panning <w>x<h>[+<x>+<y>[/<track:w>x<h>+<x>+<y>[/<border:l>/<t>/<r>/<b>]]]\n"
           "      --gamma <r>:<g>:<b>\n"
           "      --brightness <value>\n"
           "      --primary\n"
           "  --noprimary\n"
           "  --newmode <name> <clock MHz>\n"
           "            <hdisp> <hsync-start> <hsync-end> <htotal>\n"
           "            <vdisp> <vsync-start> <vsync-end> <vtotal>\n"
           "            [flags...]\n"
           "            Valid flags: +HSync -HSync +VSync -VSync\n"
           "                         +CSync -CSync CSync Interlace DoubleScan\n"
           "  --rmmode <name>\n"
           "  --addmode <output> <name>\n"
           "  --delmode <output> <name>\n"
           "  --listproviders\n"
           "  --setprovideroutputsource <prov-xid> <source-xid>\n"
           "  --setprovideroffloadsink <prov-xid> <sink-xid>\n"
	   "  --listmonitors\n"
	   "  --listactivemonitors\n"
	   "  --setmonitor <name> {auto|<w>/<mmw>x<h>/<mmh>+<x>+<y>} {none|<output>,<output>,...}\n"
	   "  --delmonitor <name>\n");
}

static void _X_NORETURN _X_ATTRIBUTE_PRINTF(1,2)
fatal (const char *format, ...)
{
    va_list ap;
    
    va_start (ap, format);
    fprintf (stderr, "%s: ", program_name);
    vfprintf (stderr, format, ap);
    va_end (ap);
    exit (1);
    /*NOTREACHED*/
}

static void _X_ATTRIBUTE_PRINTF(1,2)
warning (const char *format, ...)
{
    va_list ap;
    
    va_start (ap, format);
    fprintf (stderr, "%s: ", program_name);
    vfprintf (stderr, format, ap);
    va_end (ap);
}

static void _X_NORETURN _X_ATTRIBUTE_PRINTF(1,2)
argerr (const char *format, ...)
{
    va_list ap;

    va_start (ap, format);
    fprintf (stderr, "%s: ", program_name);
    vfprintf (stderr, format, ap);
    fprintf (stderr, "Try '%s --help' for more information.\n", program_name);
    va_end (ap);
    exit (1);
    /*NOTREACHED*/
}

/* Because fmin requires C99 suppport */
static inline double dmin (double x, double y)
{
    return x < y ? x : y;
}

static const char *
rotation_name (Rotation rotation)
{
    int	i;

    if ((rotation & 0xf) == 0)
	return "normal";
    for (i = 0; i < 4; i++)
	if (rotation & (1 << i))
	    return direction[i];
    return "invalid rotation";
}

static const char *
reflection_name (Rotation rotation)
{
    rotation &= (RR_Reflect_X|RR_Reflect_Y);
    switch (rotation) {
    case 0:
	return "none";
    case RR_Reflect_X:
	return "X axis";
    case RR_Reflect_Y:
	return "Y axis";
    case RR_Reflect_X|RR_Reflect_Y:
	return "X and Y axis";
    }
    return "invalid reflection";
}

static const char *
capability_name (int cap_bit)
{
    switch (cap_bit) {
    case RR_Capability_SourceOutput:
	return "Source Output";
    case RR_Capability_SinkOutput:
	return "Sink Output";
    case RR_Capability_SourceOffload:
	return "Source Offload";
    case RR_Capability_SinkOffload:
	return "Sink Offload";
    }
    return "invalid capability";
}

typedef enum _relation {
    relation_left_of,
    relation_right_of,
    relation_above,
    relation_below,
    relation_same_as,
} relation_t;

typedef struct {
    int	    x, y, width, height;
} rectangle_t;

typedef struct {
    int	    x1, y1, x2, y2;
} box_t;

typedef struct {
    int	    x, y;
} point_t;

typedef enum _changes {
    changes_none = 0,
    changes_crtc = (1 << 0),
    changes_mode = (1 << 1),
    changes_relation = (1 << 2),
    changes_position = (1 << 3),
    changes_rotation = (1 << 4),
    changes_reflection = (1 << 5),
    changes_automatic = (1 << 6),
    changes_refresh = (1 << 7),
    changes_property = (1 << 8),
    changes_transform = (1 << 9),
    changes_panning = (1 << 10),
    changes_gamma = (1 << 11),
    changes_primary = (1 << 12),
} changes_t;

typedef enum _name_kind {
    name_none = 0,
    name_string = (1 << 0),
    name_xid = (1 << 1),
    name_index = (1 << 2),
    name_preferred = (1 << 3),
} name_kind_t;

typedef struct {
    name_kind_t	    kind;
    char    	    *string;
    XID	    	    xid;
    int		    index;
} name_t;

typedef struct _crtc crtc_t;
typedef struct _output	output_t;
typedef struct _transform transform_t;
typedef struct _umode	umode_t;
typedef struct _output_prop output_prop_t;
typedef struct _provider provider_t;
typedef struct _monitors monitors_t;
typedef struct _umonitor umonitor_t;

struct _transform {
    XTransform	    transform;
    const char	    *filter;
    int		    nparams;
    XFixed	    *params;
};

struct _crtc {
    name_t	    crtc;
    Bool	    changing;
    XRRCrtcInfo	    *crtc_info;

    XRRModeInfo	    *mode_info;
    XRRPanning      *panning_info;
    int		    x;
    int		    y;
    Rotation	    rotation;
    output_t	    **outputs;
    int		    noutput;
    transform_t	    current_transform, pending_transform;
};

struct _output_prop {
    struct _output_prop	*next;
    char		*name;
    char		*value;
};

struct _output {
    struct _output   *next;
    
    changes_t	    changes;
    
    output_prop_t   *props;

    name_t	    output;
    XRROutputInfo   *output_info;
    
    name_t	    crtc;
    crtc_t	    *crtc_info;
    crtc_t	    *current_crtc_info;
    
    name_t	    mode;
    double	    refresh;
    XRRModeInfo	    *mode_info;
    
    name_t	    addmode;

    relation_t	    relation;
    char	    *relative_to;

    int		    x, y;
    Rotation	    rotation;

    XRRPanning      panning;

    Bool    	    automatic;
    int     	    scale_from_w, scale_from_h;
    transform_t	    transform;

    struct {
	float red;
	float green;
	float blue;
    } gamma;

    float	    brightness;

    Bool	    primary;

    Bool	    found;
};

typedef enum _umode_action {
    umode_create, umode_destroy, umode_add, umode_delete
} umode_action_t;


struct _umode {
    struct _umode   *next;
    
    umode_action_t  action;
    XRRModeInfo	    mode;
    name_t	    output;
    name_t	    name;
};

struct _provider {
    name_t		provider;
    XRRProviderInfo	*info;
};

struct _monitors {
    int			n;
    XRRMonitorInfo	*monitors;
};

struct _umonitor {
    struct _umonitor	*next;
    char		*name;
    Bool		set;
    Bool		primary;
    int			x, y, width, height;
    int			mmwidth, mmheight;
    int			noutput;
    name_t		*outputs;
};

static const char *connection[3] = {
    "connected",
    "disconnected",
    "unknown connection"};

#define OUTPUT_NAME 1

#define CRTC_OFF    2
#define CRTC_UNSET  3
#define CRTC_INDEX  0x40000000

#define MODE_NAME   1
#define MODE_OFF    2
#define MODE_UNSET  3
#define MODE_PREF   4

#define POS_UNSET   -1

static output_t	*all_outputs = NULL;
static output_t	**all_outputs_tail = &all_outputs;
static crtc_t	*crtcs;
static provider_t	*providers;
static umode_t	*umodes;
static int	num_crtcs, num_providers;
static XRRScreenResources  *res;
static int	fb_width = 0, fb_height = 0;
static int	fb_width_mm = 0, fb_height_mm = 0;
static double	dpi = 0;
static char	*dpi_output_name = NULL;
static Bool	dryrun = False;
static int	minWidth, maxWidth, minHeight, maxHeight;
static Bool    	has_1_2 = False;
static Bool    	has_1_3 = False;
static Bool    	has_1_4 = False;
static Bool	has_1_5 = False;
static name_t   provider_name, output_source_provider_name, offload_sink_provider_name;
static monitors_t	*monitors;
static umonitor_t	*umonitors;

static int
mode_height (XRRModeInfo *mode_info, Rotation rotation)
{
    switch (rotation & 0xf) {
    case RR_Rotate_0:
    case RR_Rotate_180:
	return mode_info->height;
    case RR_Rotate_90:
    case RR_Rotate_270:
	return mode_info->width;
    default:
	return 0;
    }
}

static int
mode_width (XRRModeInfo *mode_info, Rotation rotation)
{
    switch (rotation & 0xf) {
    case RR_Rotate_0:
    case RR_Rotate_180:
	return mode_info->width;
    case RR_Rotate_90:
    case RR_Rotate_270:
	return mode_info->height;
    default:
	return 0;
    }
}

static Bool
transform_point (XTransform *transform, double *xp, double *yp)
{
    double  vector[3];
    double  result[3];
    int	    i, j;
    double  v;

    vector[0] = *xp;
    vector[1] = *yp;
    vector[2] = 1;
    for (j = 0; j < 3; j++)
    {
	v = 0;
	for (i = 0; i < 3; i++)
	    v += (XFixedToDouble (transform->matrix[j][i]) * vector[i]);
	result[j] = v;
    }
    if (!result[2])
	return False;
    for (j = 0; j < 2; j++) {
	vector[j] = result[j] / result[2];
	if (vector[j] > 32767 || vector[j] < -32767)
	    return False;
    }
    *xp = vector[0];
    *yp = vector[1];
    return True;
}

static void
path_bounds (XTransform *transform, point_t *points, int npoints, box_t *box)
{
    int	    i;
    box_t   point;

    for (i = 0; i < npoints; i++) {
	double	x, y;
	x = points[i].x;
	y = points[i].y;
	transform_point (transform, &x, &y);
	point.x1 = floor (x);
	point.y1 = floor (y);
	point.x2 = ceil (x);
	point.y2 = ceil (y);
	if (i == 0)
	    *box = point;
	else {
	    if (point.x1 < box->x1) box->x1 = point.x1;
	    if (point.y1 < box->y1) box->y1 = point.y1;
	    if (point.x2 > box->x2) box->x2 = point.x2;
	    if (point.y2 > box->y2) box->y2 = point.y2;
	}
    }
}

static void
mode_geometry (XRRModeInfo *mode_info, Rotation rotation,
	       XTransform *transform,
	       box_t *bounds)
{
    point_t rect[4];
    int	width = mode_width (mode_info, rotation);
    int height = mode_height (mode_info, rotation);

    rect[0].x = 0;
    rect[0].y = 0;
    rect[1].x = width;
    rect[1].y = 0;
    rect[2].x = width;
    rect[2].y = height;
    rect[3].x = 0;
    rect[3].y = height;
    path_bounds (transform, rect, 4, bounds);
}

/* v refresh frequency in Hz */
static double
mode_refresh (const XRRModeInfo *mode_info)
{
    double rate;
    double vTotal = mode_info->vTotal;

    if (mode_info->modeFlags & RR_DoubleScan) {
	/* doublescan doubles the number of lines */
	vTotal *= 2;
    }

    if (mode_info->modeFlags & RR_Interlace) {
	/* interlace splits the frame into two fields */
	/* the field rate is what is typically reported by monitors */
	vTotal /= 2;
    }
    
    if (mode_info->hTotal && vTotal)
	rate = ((double) mode_info->dotClock /
		((double) mode_info->hTotal * (double) vTotal));
    else
    	rate = 0;
    return rate;
}

/* h sync frequency in Hz */
static double
mode_hsync (const XRRModeInfo *mode_info)
{
    double rate;
    
    if (mode_info->hTotal)
	rate = (double) mode_info->dotClock / (double) mode_info->hTotal;
    else
    	rate = 0;
    return rate;
}

static void
print_verbose_mode (const XRRModeInfo *mode, Bool current, Bool preferred)
{
    int f;

    printf ("  %s (0x%x) %6.3fMHz",
	    mode->name, (int)mode->id,
	    (double)mode->dotClock / 1000000.0);
    for (f = 0; mode_flags[f].flag; f++)
	if (mode->modeFlags & mode_flags[f].flag)
	    printf (" %s", mode_flags[f].string);
    if (current)
	printf (" *current");
    if (preferred)
	printf (" +preferred");
    printf ("\n");
    printf ("        h: width  %4d start %4d end %4d total %4d skew %4d clock %6.2fKHz\n",
	    mode->width, mode->hSyncStart, mode->hSyncEnd,
	    mode->hTotal, mode->hSkew, mode_hsync (mode) / 1000);
    printf ("        v: height %4d start %4d end %4d total %4d           clock %6.2fHz\n",
	    mode->height, mode->vSyncStart, mode->vSyncEnd, mode->vTotal,
	    mode_refresh (mode));
}

static void
init_name (name_t *name)
{
    name->kind = name_none;
}

static void
set_name_string (name_t *name, char *string)
{
    name->kind |= name_string;
    name->string = string;
}

static void
set_name_xid (name_t *name, XID xid)
{
    name->kind |= name_xid;
    name->xid = xid;
}

static void
set_name_index (name_t *name, int idx)
{
    name->kind |= name_index;
    name->index = idx;
}

static void
set_name_preferred (name_t *name)
{
    name->kind |= name_preferred;
}

static void
set_name_all (name_t *name, name_t *old)
{
    if (old->kind & name_xid)
	name->xid = old->xid;
    if (old->kind & name_string)
	name->string = old->string;
    if (old->kind & name_index)
	name->index = old->index;
    name->kind |= old->kind;
}

static void
set_name (name_t *name, char *string, name_kind_t valid)
{
    unsigned int xid; /* don't make it XID (which is unsigned long):
			 scanf() takes unsigned int */
    int idx;

    if ((valid & name_xid) && sscanf (string, "0x%x", &xid) == 1)
	set_name_xid (name, xid);
    else if ((valid & name_index) && sscanf (string, "%d", &idx) == 1)
	set_name_index (name, idx);
    else if (valid & name_string)
	set_name_string (name, string);
    else
	argerr ("invalid name '%s'\n", string);
}

static int
print_name (const name_t *name)
{
    name_kind_t kind = name->kind;

    if ((kind & name_xid))         return printf("XID 0x%x", (unsigned int)name->xid);
    else if ((kind & name_string)) return printf("name %s", name->string);
    else if ((kind & name_index))  return printf("index %d", name->index);
    else                           return printf("unknown name");
}

static void
init_transform (transform_t *transform)
{
    int x;
    memset (&transform->transform, '\0', sizeof (transform->transform));
    for (x = 0; x < 3; x++)
	transform->transform.matrix[x][x] = XDoubleToFixed (1.0);
    transform->filter = "";
    transform->nparams = 0;
    transform->params = NULL;
}

static void
set_transform (transform_t  *dest,
	       XTransform   *transform,
	       const char   *filter,
	       XFixed	    *params,
	       int	    nparams)
{
    dest->transform = *transform;
    /* note: this string is leaked */
    dest->filter = strdup (filter);
    dest->nparams = nparams;
    dest->params = malloc (nparams * sizeof (XFixed));
    memcpy (dest->params, params, nparams * sizeof (XFixed));
}

static void
copy_transform (transform_t *dest, transform_t *src)
{
    set_transform (dest, &src->transform,
		   src->filter, src->params, src->nparams);
}

static Bool
equal_transform (transform_t *a, transform_t *b)
{
    if (memcmp (&a->transform, &b->transform, sizeof (XTransform)) != 0)
	return False;
    if (strcmp (a->filter, b->filter) != 0)
	return False;
    if (a->nparams != b->nparams)
	return False;
    if (memcmp (a->params, b->params, a->nparams * sizeof (XFixed)) != 0)
	return False;
    return True;
}

static output_t *
add_output (void)
{
    output_t *output = calloc (1, sizeof (output_t));

    if (!output)
	fatal ("out of memory\n");
    output->next = NULL;
    output->found = False;
    output->brightness = 1.0;
    *all_outputs_tail = output;
    all_outputs_tail = &output->next;
    return output;
}

static output_t *
find_output (name_t *name)
{
    output_t *output;

    for (output = all_outputs; output; output = output->next)
    {
	name_kind_t common = name->kind & output->output.kind;
	
	if ((common & name_xid) && name->xid == output->output.xid)
	    break;
	if ((common & name_string) && !strcmp (name->string, output->output.string))
	    break;
	if ((common & name_index) && name->index == output->output.index)
	    break;
    }
    return output;
}

static output_t *
find_output_by_xid (RROutput output)
{
    name_t  output_name;

    init_name (&output_name);
    set_name_xid (&output_name, output);
    return find_output (&output_name);
}

static output_t *
find_output_by_name (char *name)
{
    name_t  output_name;

    init_name (&output_name);
    set_name_string (&output_name, name);
    return find_output (&output_name);
}

static crtc_t *
find_crtc (name_t *name)
{
    int	    c;
    crtc_t  *crtc = NULL;

    for (c = 0; c < num_crtcs; c++)
    {
	name_kind_t common;
	
	crtc = &crtcs[c];
	common = name->kind & crtc->crtc.kind;
	
	if ((common & name_xid) && name->xid == crtc->crtc.xid)
	    break;
	if ((common & name_string) && !strcmp (name->string, crtc->crtc.string))
	    break;
	if ((common & name_index) && name->index == crtc->crtc.index)
	    break;
	crtc = NULL;
    }
    return crtc;
}

static crtc_t *
find_crtc_by_xid (RRCrtc crtc)
{
    name_t  crtc_name;

    init_name (&crtc_name);
    set_name_xid (&crtc_name, crtc);
    return find_crtc (&crtc_name);
}

static XRRModeInfo *
find_mode (name_t *name, double refresh)
{
    int		m;
    XRRModeInfo	*best = NULL;
    double	bestDist = 0;

    for (m = 0; m < res->nmode; m++)
    {
	XRRModeInfo *mode = &res->modes[m];
	if ((name->kind & name_xid) && name->xid == mode->id)
	{
	    best = mode;
	    break;
	}
	if ((name->kind & name_string) && !strcmp (name->string, mode->name))
	{
	    double   dist;
	    
	    if (refresh)
		dist = fabs (mode_refresh (mode) - refresh);
	    else
		dist = 0;
	    if (!best || dist < bestDist)
	    {
		bestDist = dist;
		best = mode;
	    }
	}
    }
    return best;
}

static XRRModeInfo *
find_mode_by_xid (RRMode mode)
{
    name_t  mode_name;

    init_name (&mode_name);
    set_name_xid (&mode_name, mode);
    return find_mode (&mode_name, 0);
}

#if 0
static XRRModeInfo *
find_mode_by_name (char *name)
{
    name_t  mode_name;
    init_name (&mode_name);
    set_name_string (&mode_name, name);
    return find_mode (&mode_name, 0);
}
#endif

static
XRRModeInfo *
find_mode_for_output (output_t *output, name_t *name)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    m;
    XRRModeInfo	    *best = NULL;
    double	    bestDist = 0;

    for (m = 0; m < output_info->nmode; m++)
    {
	XRRModeInfo	    *mode;

	mode = find_mode_by_xid (output_info->modes[m]);
	if (!mode) continue;
	if ((name->kind & name_xid) && name->xid == mode->id)
	{
	    best = mode;
	    break;
	}
	if ((name->kind & name_string) && !strcmp (name->string, mode->name))
	{
	    double   dist;

	    /* Stay away from doublescan modes unless refresh rate is specified. */
	    if (!output->refresh && (mode->modeFlags & RR_DoubleScan))
		continue;

	    if (output->refresh)
		dist = fabs (mode_refresh (mode) - output->refresh);
	    else
		dist = 0;
	    if (!best || dist < bestDist)
	    {
		bestDist = dist;
		best = mode;
	    }
	}
    }
    return best;
}

static XRRModeInfo *
preferred_mode (output_t *output)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    m;
    XRRModeInfo	    *best;
    int		    bestDist;
    
    best = NULL;
    bestDist = 0;
    for (m = 0; m < output_info->nmode; m++)
    {
	XRRModeInfo *mode_info = find_mode_by_xid (output_info->modes[m]);
	int	    dist;
	
	if (m < output_info->npreferred)
	    dist = 0;
	else if (output_info->mm_height)
	    dist = (1000 * DisplayHeight(dpy, screen) / DisplayHeightMM(dpy, screen) -
		    1000 * mode_info->height / output_info->mm_height);
	else
	    dist = DisplayHeight(dpy, screen) - mode_info->height;

        if (dist < 0) dist = -dist;
	if (!best || dist < bestDist)
	{
	    best = mode_info;
	    bestDist = dist;
	}
    }
    return best;
}

static Bool
output_can_use_crtc (output_t *output, crtc_t *crtc)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    c;

    for (c = 0; c < output_info->ncrtc; c++)
	if (output_info->crtcs[c] == crtc->crtc.xid)
	    return True;
    return False;
}

static Bool
output_can_use_mode (output_t *output, XRRModeInfo *mode)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    m;

    for (m = 0; m < output_info->nmode; m++)
	if (output_info->modes[m] == mode->id)
	    return True;
    return False;
}

static Bool
crtc_can_use_rotation (crtc_t *crtc, Rotation rotation)
{
    Rotation	rotations = crtc->crtc_info->rotations;
    Rotation	dir = rotation & (RR_Rotate_0|RR_Rotate_90|RR_Rotate_180|RR_Rotate_270);
    Rotation	reflect = rotation & (RR_Reflect_X|RR_Reflect_Y);
    if (((rotations & dir) != 0) && ((rotations & reflect) == reflect))
	return True;
    return False;
}

#if 0
static Bool
crtc_can_use_transform (crtc_t *crtc, XTransform *transform)
{
    int	major, minor;

    XRRQueryVersion (dpy, &major, &minor);
    if (major > 1 || (major == 1 && minor >= 3))
	return True;
    return False;
}
#endif

/*
 * Report only rotations that are supported by all crtcs
 */
static Rotation
output_rotations (output_t *output)
{
    Bool	    found = False;
    Rotation	    rotation = RR_Rotate_0;
    XRROutputInfo   *output_info = output->output_info;
    int		    c;
    
    for (c = 0; c < output_info->ncrtc; c++)
    {
	crtc_t	*crtc = find_crtc_by_xid (output_info->crtcs[c]);
	if (crtc)
	{
	    if (!found) {
		rotation = crtc->crtc_info->rotations;
		found = True;
	    } else
		rotation &= crtc->crtc_info->rotations;
	}
    }
    return rotation;
}

static Bool
output_can_use_rotation (output_t *output, Rotation rotation)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    c;

    /* make sure all of the crtcs can use this rotation.
     * yes, this is not strictly necessary, but it is 
     * simpler,and we expect most drivers to either
     * support rotation everywhere or nowhere
     */
    for (c = 0; c < output_info->ncrtc; c++)
    {
	crtc_t	*crtc = find_crtc_by_xid (output_info->crtcs[c]);
	if (crtc && !crtc_can_use_rotation (crtc, rotation))
	    return False;
    }
    return True;
}

static Bool
output_is_primary(output_t *output)
{
    if (has_1_3)
	    return XRRGetOutputPrimary(dpy, root) == output->output.xid;
    return False;
}

/* Returns the index of the last value in an array < 0xffff */
static int
find_last_non_clamped(CARD16 array[], int size) {
    int i;
    for (i = size - 1; i > 0; i--) {
        if (array[i] < 0xffff)
	    return i;
    }
    return 0;
}

static void
set_gamma_info(output_t *output)
{
    XRRCrtcGamma *crtc_gamma;
    double i1, v1, i2, v2;
    int size, middle, last_best, last_red, last_green, last_blue;
    CARD16 *best_array;

    if (!output->crtc_info)
	return;

    size = XRRGetCrtcGammaSize(dpy, output->crtc_info->crtc.xid);
    if (!size) {
	warning("Failed to get size of gamma for output %s\n", output->output.string);
	return;
    }

    crtc_gamma = XRRGetCrtcGamma(dpy, output->crtc_info->crtc.xid);
    if (!crtc_gamma) {
	warning("Failed to get gamma for output %s\n", output->output.string);
	return;
    }

    /*
     * Here is a bit tricky because gamma is a whole curve for each
     * color.  So, typically, we need to represent 3 * 256 values as 3 + 1
     * values.  Therefore, we approximate the gamma curve (v) by supposing
     * it always follows the way we set it: a power function (i^g)
     * multiplied by a brightness (b).
     * v = i^g * b
     * so g = (ln(v) - ln(b))/ln(i)
     * and b can be found using two points (v1,i1) and (v2, i2):
     * b = e^((ln(v2)*ln(i1) - ln(v1)*ln(i2))/ln(i1/i2))
     * For the best resolution, we select i2 at the highest place not
     * clamped and i1 at i2/2. Note that if i2 = 1 (as in most normal
     * cases), then b = v2.
     */
    last_red = find_last_non_clamped(crtc_gamma->red, size);
    last_green = find_last_non_clamped(crtc_gamma->green, size);
    last_blue = find_last_non_clamped(crtc_gamma->blue, size);
    best_array = crtc_gamma->red;
    last_best = last_red;
    if (last_green > last_best) {
	last_best = last_green;
	best_array = crtc_gamma->green;
    }
    if (last_blue > last_best) {
	last_best = last_blue;
	best_array = crtc_gamma->blue;
    }
    if (last_best == 0)
	last_best = 1;

    middle = last_best / 2;
    i1 = (double)(middle + 1) / size;
    v1 = (double)(best_array[middle]) / 65535;
    i2 = (double)(last_best + 1) / size;
    v2 = (double)(best_array[last_best]) / 65535;
    if (v2 < 0.0001) { /* The screen is black */
	output->brightness = 0;
	output->gamma.red = 1;
	output->gamma.green = 1;
	output->gamma.blue = 1;
    } else {
	if ((last_best + 1) == size)
	    output->brightness = v2;
	else
	    output->brightness = exp((log(v2)*log(i1) - log(v1)*log(i2))/log(i1/i2));
	output->gamma.red = log((double)(crtc_gamma->red[last_red / 2]) / output->brightness
				/ 65535) / log((double)((last_red / 2) + 1) / size);
	output->gamma.green = log((double)(crtc_gamma->green[last_green / 2]) / output->brightness
				  / 65535) / log((double)((last_green / 2) + 1) / size);
	output->gamma.blue = log((double)(crtc_gamma->blue[last_blue / 2]) / output->brightness
				 / 65535) / log((double)((last_blue / 2) + 1) / size);
    }

    XRRFreeGamma(crtc_gamma);
}

static void
set_output_info (output_t *output, RROutput xid, XRROutputInfo *output_info)
{
    /* sanity check output info */
    if (output_info->connection != RR_Disconnected && !output_info->nmode)
	warning ("Output %s is not disconnected but has no modes\n",
		 output_info->name);
    
    /* set output name and info */
    if (!(output->output.kind & name_xid))
	set_name_xid (&output->output, xid);
    if (!(output->output.kind & name_string))
	set_name_string (&output->output, output_info->name);
    output->output_info = output_info;
    
    /* set crtc name and info */
    if (!(output->changes & changes_crtc))
	set_name_xid (&output->crtc, output_info->crtc);
    
    if (output->crtc.kind == name_xid && output->crtc.xid == None)
	output->crtc_info = NULL;
    else
    {
	output->crtc_info = find_crtc (&output->crtc);
	if (!output->crtc_info)
	{
	    if (output->crtc.kind & name_xid)
		fatal ("cannot find crtc 0x%lx\n", output->crtc.xid);
	    if (output->crtc.kind & name_index)
		fatal ("cannot find crtc %d\n", output->crtc.index);
	}
	if (!output_can_use_crtc (output, output->crtc_info))
	    fatal ("output %s cannot use crtc 0x%lx\n", output->output.string,
		   output->crtc_info->crtc.xid);
    }

    /* set mode name and info */
    if (!(output->changes & changes_mode))
    {
	crtc_t	*crtc = NULL;
	
	if (output_info->crtc)
	    crtc = find_crtc_by_xid(output_info->crtc);
	if (crtc && crtc->crtc_info)
	    set_name_xid (&output->mode, crtc->crtc_info->mode);
	else if (output->crtc_info)
	    set_name_xid (&output->mode, output->crtc_info->crtc_info->mode);
	else
	    set_name_xid (&output->mode, None);
	if (output->mode.xid)
	{
	    output->mode_info = find_mode_by_xid (output->mode.xid);
	    if (!output->mode_info)
		fatal ("server did not report mode 0x%lx for output %s\n",
		       output->mode.xid, output->output.string);
	}
	else
	    output->mode_info = NULL;
    }
    else if (output->mode.kind == name_xid && output->mode.xid == None)
	output->mode_info = NULL;
    else
    {
	if (output->mode.kind == name_preferred)
	    output->mode_info = preferred_mode (output);
	else
	    output->mode_info = find_mode_for_output (output, &output->mode);
	if (!output->mode_info)
	{
	    if (output->mode.kind & name_preferred)
		fatal ("cannot find preferred mode\n");
	    if (output->mode.kind & name_string)
		fatal ("cannot find mode %s\n", output->mode.string);
	    if (output->mode.kind & name_xid)
		fatal ("cannot find mode 0x%lx\n", output->mode.xid);
	}
	if (!output_can_use_mode (output, output->mode_info))
	    fatal ("output %s cannot use mode %s\n", output->output.string,
		   output->mode_info->name);
    }

    /* set position */
    if (!(output->changes & changes_position))
    {
	if (output->crtc_info)
	{
	    output->x = output->crtc_info->crtc_info->x;
	    output->y = output->crtc_info->crtc_info->y;
	}
	else
	{
	    output->x = 0;
	    output->y = 0;
	}
    }

    /* set rotation */
    if (!(output->changes & changes_rotation))
    {
	output->rotation &= ~0xf;
	if (output->crtc_info)
	    output->rotation |= (output->crtc_info->crtc_info->rotation & 0xf);
	else
	    output->rotation = RR_Rotate_0;
    }
    if (!(output->changes & changes_reflection))
    {
	output->rotation &= ~(RR_Reflect_X|RR_Reflect_Y);
	if (output->crtc_info)
	    output->rotation |= (output->crtc_info->crtc_info->rotation &
				 (RR_Reflect_X|RR_Reflect_Y));
    }
    if (!output_can_use_rotation (output, output->rotation))
	fatal ("output %s cannot use rotation \"%s\" reflection \"%s\"\n",
	       output->output.string,
	       rotation_name (output->rotation),
	       reflection_name (output->rotation));

    /* set gamma */
    if (!(output->changes & changes_gamma))
	    set_gamma_info(output);

    /* set transformation */
    if (!(output->changes & changes_transform))
    {
	if (output->crtc_info)
	    copy_transform (&output->transform, &output->crtc_info->current_transform);
	else
	    init_transform (&output->transform);
    } else {
	/* transform was already set for --scale or --transform */

	/* for --scale-from, figure out the mode size and compute the transform
	 * for the target framebuffer area */
	if (output->scale_from_w > 0 && output->mode_info) {
	    double sx = (double)output->scale_from_w /
				output->mode_info->width;
	    double sy = (double)output->scale_from_h /
				output->mode_info->height;
	    if (verbose)
		printf("scaling %s by %lfx%lf\n", output->output.string, sx,
		       sy);
	    init_transform (&output->transform);
	    output->transform.transform.matrix[0][0] = XDoubleToFixed (sx);
	    output->transform.transform.matrix[1][1] = XDoubleToFixed (sy);
	    output->transform.transform.matrix[2][2] = XDoubleToFixed (1.0);
	    if (sx != 1 || sy != 1)
		output->transform.filter = "bilinear";
	    else
		output->transform.filter = "nearest";
	    output->transform.nparams = 0;
	    output->transform.params = NULL;
	}
    }

    /* set primary */
    if (!(output->changes & changes_primary))
	output->primary = output_is_primary(output);
}
    
static void
get_screen (Bool current)
{
    if (!has_1_2)
        fatal ("Server RandR version before 1.2\n");

    if (res)
	return;

    XRRGetScreenSizeRange (dpy, root, &minWidth, &minHeight,
			   &maxWidth, &maxHeight);
    
    if (current)
	res = XRRGetScreenResourcesCurrent (dpy, root);
    else
	res = XRRGetScreenResources (dpy, root);
    if (!res) fatal ("could not get screen resources");
}

static void
get_crtcs (void)
{
    int		c;

    num_crtcs = res->ncrtc;
    crtcs = calloc (num_crtcs, sizeof (crtc_t));
    if (!crtcs) fatal ("out of memory\n");
    
    for (c = 0; c < res->ncrtc; c++)
    {
	XRRCrtcInfo *crtc_info = XRRGetCrtcInfo (dpy, res, res->crtcs[c]);
	XRRCrtcTransformAttributes  *attr;
	XRRPanning  *panning_info = NULL;

	if (has_1_3) {
	    XRRPanning zero;
	    memset(&zero, 0, sizeof(zero));
	    panning_info = XRRGetPanning  (dpy, res, res->crtcs[c]);
	    zero.timestamp = panning_info->timestamp;
	    if (!memcmp(panning_info, &zero, sizeof(zero))) {
		Xfree(panning_info);
		panning_info = NULL;
	    }
	}

	set_name_xid (&crtcs[c].crtc, res->crtcs[c]);
	set_name_index (&crtcs[c].crtc, c);
	if (!crtc_info) fatal ("could not get crtc 0x%lx information\n", res->crtcs[c]);
	crtcs[c].crtc_info = crtc_info;
	crtcs[c].panning_info = panning_info;
	if (crtc_info->mode == None)
	{
	    crtcs[c].mode_info = NULL;
	    crtcs[c].x = 0;
	    crtcs[c].y = 0;
	    crtcs[c].rotation = RR_Rotate_0;
	}
	if (XRRGetCrtcTransform (dpy, res->crtcs[c], &attr) && attr) {
	    set_transform (&crtcs[c].current_transform,
			   &attr->currentTransform,
			   attr->currentFilter,
			   attr->currentParams,
			   attr->currentNparams);
	    XFree (attr);
	}
	else
	{
	    init_transform (&crtcs[c].current_transform);
	}
	copy_transform (&crtcs[c].pending_transform, &crtcs[c].current_transform);
   }
}

static void
crtc_add_output (crtc_t *crtc, output_t *output)
{
    if (crtc->outputs)
	crtc->outputs = realloc (crtc->outputs, (crtc->noutput + 1) * sizeof (output_t *));
    else
    {
	crtc->outputs = malloc (sizeof (output_t *));
	crtc->x = output->x;
	crtc->y = output->y;
	crtc->rotation = output->rotation;
	crtc->mode_info = output->mode_info;
	copy_transform (&crtc->pending_transform, &output->transform);
   }
    if (!crtc->outputs) fatal ("out of memory\n");
    crtc->outputs[crtc->noutput++] = output;
}

static void
set_crtcs (void)
{
    output_t	*output;

    for (output = all_outputs; output; output = output->next)
    {
	if (!output->mode_info) continue;
	crtc_add_output (output->crtc_info, output);
    }
}

static void
set_panning (void)
{
    output_t	*output;

    for (output = all_outputs; output; output = output->next)
    {
	if (! output->crtc_info)
	    continue;
	if (! (output->changes & changes_panning))
	    continue;
	if (! output->crtc_info->panning_info)
	    output->crtc_info->panning_info = malloc (sizeof(XRRPanning));
	memcpy (output->crtc_info->panning_info, &output->panning, sizeof(XRRPanning));
	output->crtc_info->changing = 1;
    }
}

static void
set_gamma(void)
{
    output_t	*output;

    for (output = all_outputs; output; output = output->next) {
	int i, size;
	crtc_t *crtc;
	XRRCrtcGamma *crtc_gamma;
	float gammaRed;
	float gammaGreen;
	float gammaBlue;

	if (!(output->changes & changes_gamma))
	    continue;

	if (!output->crtc_info) {
	    fatal("Need crtc to set gamma on.\n");
	    continue;
	}

	crtc = output->crtc_info;

	size = XRRGetCrtcGammaSize(dpy, crtc->crtc.xid);

	if (!size) {
	    fatal("Gamma size is 0.\n");
	    continue;
	}

	/*
	 * The gamma-correction lookup table managed through XRR[GS]etCrtcGamma
	 * is 2^n in size, where 'n' is the number of significant bits in
	 * the X Color.  Because an X Color is 16 bits, size cannot be larger
	 * than 2^16.
	 */
	if (size > 65536) {
	    fatal("Gamma correction table is impossibly large.\n");
	    continue;
	}

	crtc_gamma = XRRAllocGamma(size);
	if (!crtc_gamma) {
	    fatal("Gamma allocation failed.\n");
	    continue;
	}

	if (output->gamma.red == 0.0)
	    output->gamma.red = 1.0;
	if (output->gamma.green == 0.0)
	    output->gamma.green = 1.0;
	if (output->gamma.blue == 0.0)
	    output->gamma.blue = 1.0;

	gammaRed = 1.0 / output->gamma.red;
	gammaGreen = 1.0 / output->gamma.green;
	gammaBlue = 1.0 / output->gamma.blue;

	for (i = 0; i < size; i++) {
	    if (gammaRed == 1.0 && output->brightness == 1.0)
		crtc_gamma->red[i] = (double)i / (double)(size - 1) * 65535.0;
	    else
		crtc_gamma->red[i] = dmin(pow((double)i/(double)(size - 1),
					      gammaRed) * output->brightness,
					  1.0) * 65535.0;

	    if (gammaGreen == 1.0 && output->brightness == 1.0)
		crtc_gamma->green[i] = (double)i / (double)(size - 1) * 65535.0;
	    else
		crtc_gamma->green[i] = dmin(pow((double)i/(double)(size - 1),
						gammaGreen) * output->brightness,
					    1.0) * 65535.0;

	    if (gammaBlue == 1.0 && output->brightness == 1.0)
		crtc_gamma->blue[i] = (double)i / (double)(size - 1) * 65535.0;
	    else
		crtc_gamma->blue[i] = dmin(pow((double)i/(double)(size - 1),
					       gammaBlue) * output->brightness,
					   1.0) * 65535.0;
	}

	XRRSetCrtcGamma(dpy, crtc->crtc.xid, crtc_gamma);

	free(crtc_gamma);
    }
}

static void
set_primary(void)
{
    output_t *output;

    if (no_primary) {
	XRRSetOutputPrimary(dpy, root, None);
    } else {
	for (output = all_outputs; output; output = output->next) {
	    if (!(output->changes & changes_primary))
		continue;
	    if (output->primary)
		XRRSetOutputPrimary(dpy, root, output->output.xid);
	}
    }
}

static Status
crtc_disable (crtc_t *crtc)
{
    if (verbose)
    	printf ("crtc %d: disable\n", crtc->crtc.index);
	
    if (dryrun)
	return RRSetConfigSuccess;
    return XRRSetCrtcConfig (dpy, res, crtc->crtc.xid, CurrentTime,
			     0, 0, None, RR_Rotate_0, NULL, 0);
}

static void
crtc_set_transform (crtc_t *crtc, transform_t *transform)
{
    int	major, minor;

    XRRQueryVersion (dpy, &major, &minor);
    if (major > 1 || (major == 1 && minor >= 3))
	XRRSetCrtcTransform (dpy, crtc->crtc.xid,
			     &transform->transform,
			     transform->filter,
			     transform->params,
			     transform->nparams);
}

static Status
crtc_revert (crtc_t *crtc)
{
    XRRCrtcInfo	*crtc_info = crtc->crtc_info;
    
    if (verbose)
    	printf ("crtc %d: revert\n", crtc->crtc.index);
	
    if (dryrun)
	return RRSetConfigSuccess;

    if (!equal_transform (&crtc->current_transform, &crtc->pending_transform))
	crtc_set_transform (crtc, &crtc->current_transform);
    return XRRSetCrtcConfig (dpy, res, crtc->crtc.xid, CurrentTime,
			    crtc_info->x, crtc_info->y,
			    crtc_info->mode, crtc_info->rotation,
			    crtc_info->outputs, crtc_info->noutput);
}

static Status
crtc_apply (crtc_t *crtc)
{
    RROutput	*rr_outputs;
    int		o;
    Status	s;
    RRMode	mode = None;

    if (!crtc->changing || !crtc->mode_info)
	return RRSetConfigSuccess;

    rr_outputs = calloc (crtc->noutput, sizeof (RROutput));
    if (!rr_outputs)
	return BadAlloc;
    for (o = 0; o < crtc->noutput; o++)
	rr_outputs[o] = crtc->outputs[o]->output.xid;
    mode = crtc->mode_info->id;
    if (verbose) {
	printf ("crtc %d: %12s %6.2f +%d+%d", crtc->crtc.index,
		crtc->mode_info->name, mode_refresh (crtc->mode_info),
		crtc->x, crtc->y);
	for (o = 0; o < crtc->noutput; o++)
	    printf (" \"%s\"", crtc->outputs[o]->output.string);
	printf ("\n");
    }
    
    if (dryrun)
	s = RRSetConfigSuccess;
    else
    {
	if (!equal_transform (&crtc->current_transform, &crtc->pending_transform))
	    crtc_set_transform (crtc, &crtc->pending_transform);
	s = XRRSetCrtcConfig (dpy, res, crtc->crtc.xid, CurrentTime,
			      crtc->x, crtc->y, mode, crtc->rotation,
			      rr_outputs, crtc->noutput);
	if (s == RRSetConfigSuccess && crtc->panning_info) {
	    if (has_1_3)
		s = XRRSetPanning (dpy, res, crtc->crtc.xid, crtc->panning_info);
	    else
		fatal ("panning needs RandR 1.3\n");
	}
    }
    free (rr_outputs);
    return s;
}

static void
screen_revert (void)
{
    if (verbose)
	printf ("screen %d: revert\n", screen);

    if (dryrun)
	return;
    XRRSetScreenSize (dpy, root,
		      DisplayWidth (dpy, screen),
		      DisplayHeight (dpy, screen),
		      DisplayWidthMM (dpy, screen),
		      DisplayHeightMM (dpy, screen));
}

static void
screen_apply (void)
{
    if (fb_width == DisplayWidth (dpy, screen) &&
	fb_height == DisplayHeight (dpy, screen) &&
	fb_width_mm == DisplayWidthMM (dpy, screen) &&
	fb_height_mm == DisplayHeightMM (dpy, screen))
    {
	return;
    }
    if (verbose)
	printf ("screen %d: %dx%d %dx%d mm %6.2fdpi\n", screen,
		fb_width, fb_height, fb_width_mm, fb_height_mm, dpi);
    if (dryrun)
	return;
    XRRSetScreenSize (dpy, root, fb_width, fb_height,
		      fb_width_mm, fb_height_mm);
}

static void
revert (void)
{
    int	c;

    /* first disable all crtcs */
    for (c = 0; c < res->ncrtc; c++)
	crtc_disable (&crtcs[c]);
    /* next reset screen size */
    screen_revert ();
    /* now restore all crtcs */
    for (c = 0; c < res->ncrtc; c++)
	crtc_revert (&crtcs[c]);
}

/*
 * uh-oh, something bad happened in the middle of changing
 * the configuration. Revert to the previous configuration
 * and bail
 */
static void _X_NORETURN
panic (Status s, crtc_t *crtc)
{
    int	    c = crtc->crtc.index;
    const char *message;
    
    switch (s) {
    case RRSetConfigSuccess:		message = "succeeded";		    break;
    case BadAlloc:			message = "out of memory";	    break;
    case RRSetConfigFailed:		message = "failed";		    break;
    case RRSetConfigInvalidConfigTime:	message = "invalid config time";    break;
    case RRSetConfigInvalidTime:	message = "invalid time";	    break;
    default:				message = "unknown failure";	    break;
    }
    
    fprintf (stderr, "%s: Configure crtc %d %s\n", program_name, c, message);
    revert ();
    exit (1);
}

static void
apply (void)
{
    Status  s;
    int	    c;
    
    /*
     * Hold the server grabbed while messing with
     * the screen so that apps which notice the resize
     * event and ask for xinerama information from the server
     * receive up-to-date information
     */
    if (grab_server)
	XGrabServer (dpy);
    
    /*
     * Turn off any crtcs which are to be disabled or which are
     * larger than the target size
     */
    for (c = 0; c < res->ncrtc; c++)
    {
	crtc_t	    *crtc = &crtcs[c];
	XRRCrtcInfo *crtc_info = crtc->crtc_info;

	/* if this crtc is already disabled, skip it */
	if (crtc_info->mode == None) 
	    continue;
	
	/* 
	 * If this crtc is to be left enabled, make
	 * sure the old size fits then new screen
	 */
	if (crtc->mode_info) 
	{
	    XRRModeInfo	*old_mode = find_mode_by_xid (crtc_info->mode);
	    int x, y, w, h;
	    box_t bounds;

	    if (!old_mode) 
		panic (RRSetConfigFailed, crtc);
	    
	    /* old position and size information */
	    mode_geometry (old_mode, crtc_info->rotation,
			   &crtc->current_transform.transform,
			   &bounds);

	    x = crtc_info->x + bounds.x1;
	    y = crtc_info->y + bounds.y1;
	    w = bounds.x2 - bounds.x1;
	    h = bounds.y2 - bounds.y1;

	    /* if it fits, skip it */
	    if (x + w <= fb_width && y + h <= fb_height) 
		continue;
	    crtc->changing = True;
	}
	s = crtc_disable (crtc);
	if (s != RRSetConfigSuccess)
	    panic (s, crtc);
    }

    /*
     * Set the screen size
     */
    screen_apply ();
    
    /*
     * Set crtcs
     */

    for (c = 0; c < res->ncrtc; c++)
    {
	crtc_t	*crtc = &crtcs[c];
	
	s = crtc_apply (crtc);
	if (s != RRSetConfigSuccess)
	    panic (s, crtc);
    }

    set_primary ();

    /*
     * Release the server grab and let all clients
     * respond to the updated state
     */
    if (grab_server)
	XUngrabServer (dpy);
}

/*
 * Use current output state to complete the output list
 */
static void
get_outputs (void)
{
    int		o;
    output_t    *q;
    
    for (o = 0; o < res->noutput; o++)
    {
	XRROutputInfo	*output_info = XRRGetOutputInfo (dpy, res, res->outputs[o]);
	output_t	*output;
	name_t		output_name;
	if (!output_info) fatal ("could not get output 0x%lx information\n", res->outputs[o]);
	set_name_xid (&output_name, res->outputs[o]);
	set_name_index (&output_name, o);
	set_name_string (&output_name, output_info->name);
	output = find_output (&output_name);
	if (!output)
	{
	    output = add_output ();
	    set_name_all (&output->output, &output_name);
	    /*
	     * When global --automatic mode is set, turn on connected but off
	     * outputs, turn off disconnected but on outputs
	     */
	    if (automatic)
	    {
		switch (output_info->connection) {
		case RR_Connected:
		    if (!output_info->crtc) {
			output->changes |= changes_automatic;
			output->automatic = True;
		    }
		    break;
		case RR_Disconnected:
		    if (output_info->crtc)
		    {
			output->changes |= changes_automatic;
			output->automatic = True;
		    }
		    break;
		}
	    }
	}
	output->found = True;

	/*
	 * Automatic mode -- track connection state and enable/disable outputs
	 * as necessary
	 */
	if (output->automatic)
	{
	    switch (output_info->connection) {
	    case RR_Connected:
	    case RR_UnknownConnection:
		if ((!(output->changes & changes_mode)))
		{
		    set_name_preferred (&output->mode);
		    output->changes |= changes_mode;
		}
		break;
	    case RR_Disconnected:
		if ((!(output->changes & changes_mode)))
		{
		    set_name_xid (&output->mode, None);
		    set_name_xid (&output->crtc, None);
		    output->changes |= changes_mode;
		    output->changes |= changes_crtc;
		}
		break;
	    }
	}

	set_output_info (output, res->outputs[o], output_info);
    }
    for (q = all_outputs; q; q = q->next)
    {
	if (!q->found)
	{
	    fprintf(stderr, "warning: output %s not found; ignoring\n",
		    q->output.string);
	}
    }
}

static void
mark_changing_crtcs (void)
{
    int	c;

    for (c = 0; c < num_crtcs; c++)
    {
	crtc_t	    *crtc = &crtcs[c];
	int	    o;
	output_t    *output;

	/* walk old output list (to catch disables) */
	for (o = 0; o < crtc->crtc_info->noutput; o++)
	{
	    output = find_output_by_xid (crtc->crtc_info->outputs[o]);
	    if (!output) fatal ("cannot find output 0x%lx\n",
				crtc->crtc_info->outputs[o]);
	    if (output->changes)
		crtc->changing = True;
	}
	/* walk new output list */
	for (o = 0; o < crtc->noutput; o++)
	{
	    output = crtc->outputs[o];
	    if (output->changes)
		crtc->changing = True;
	}
    }
}

/*
 * Test whether 'crtc' can be used for 'output'
 */
static Bool
check_crtc_for_output (crtc_t *crtc, output_t *output)
{
    int		c;
    int		l;
    output_t    *other;
    
    for (c = 0; c < output->output_info->ncrtc; c++)
	if (output->output_info->crtcs[c] == crtc->crtc.xid)
	    break;
    if (c == output->output_info->ncrtc)
	return False;
    for (other = all_outputs; other; other = other->next)
    {
	if (other == output)
	    continue;

	if (other->mode_info == NULL)
	    continue;

	if (other->crtc_info != crtc)
	    continue;

	/* see if the output connected to the crtc can clone to this output */
	for (l = 0; l < output->output_info->nclone; l++)
	    if (output->output_info->clones[l] == other->output.xid)
		break;
	/* not on the list, can't clone */
	if (l == output->output_info->nclone) 
	    return False;
    }

    if (crtc->noutput)
    {
	/* make sure the state matches */
	if (crtc->mode_info != output->mode_info)
	    return False;
	if (crtc->x != output->x)
	    return False;
	if (crtc->y != output->y)
	    return False;
	if (crtc->rotation != output->rotation)
	    return False;
	if (!equal_transform (&crtc->current_transform, &output->transform))
	    return False;
    }
    else if (crtc->crtc_info->noutput)
    {
	/* make sure the state matches the already used state */
	XRRModeInfo *mode = find_mode_by_xid (crtc->crtc_info->mode);

	if (mode != output->mode_info)
	    return False;
	if (crtc->crtc_info->x != output->x)
	    return False;
	if (crtc->crtc_info->y != output->y)
	    return False;
	if (crtc->crtc_info->rotation != output->rotation)
	    return False;
    }
    return True;
}

static crtc_t *
find_crtc_for_output (output_t *output)
{
    int	    c;

    for (c = 0; c < output->output_info->ncrtc; c++)
    {
	crtc_t	    *crtc;

	crtc = find_crtc_by_xid (output->output_info->crtcs[c]);
	if (!crtc) fatal ("cannot find crtc 0x%lx\n", output->output_info->crtcs[c]);

	if (check_crtc_for_output (crtc, output))
	    return crtc;
    }
    return NULL;
}

static void
set_positions (void)
{
    output_t	*output;
    Bool	keep_going;
    Bool	any_set;
    int		min_x, min_y;

    for (;;)
    {
	any_set = False;
	keep_going = False;
	for (output = all_outputs; output; output = output->next)
	{
	    output_t    *relation;
	    name_t	relation_name;

	    if (!(output->changes & changes_relation)) continue;
	    
	    if (output->mode_info == NULL) continue;

	    init_name (&relation_name);
	    set_name_string (&relation_name, output->relative_to);
	    relation = find_output (&relation_name);
	    if (!relation) fatal ("cannot find output \"%s\"\n", output->relative_to);
	    
	    if (relation->mode_info == NULL) 
	    {
		output->x = 0;
		output->y = 0;
		output->changes |= changes_position;
		any_set = True;
		continue;
	    }
	    /*
	     * Make sure the dependent object has been set in place
	     */
	    if ((relation->changes & changes_relation) && 
		!(relation->changes & changes_position))
	    {
		keep_going = True;
		continue;
	    }
	    
	    switch (output->relation) {
	    case relation_left_of:
		output->y = relation->y;
		output->x = relation->x - mode_width (output->mode_info, output->rotation);
		break;
	    case relation_right_of:
		output->y = relation->y;
		output->x = relation->x + mode_width (relation->mode_info, relation->rotation);
		break;
	    case relation_above:
		output->x = relation->x;
		output->y = relation->y - mode_height (output->mode_info, output->rotation);
		break;
	    case relation_below:
		output->x = relation->x;
		output->y = relation->y + mode_height (relation->mode_info, relation->rotation);
		break;
	    case relation_same_as:
		output->x = relation->x;
		output->y = relation->y;
	    }
	    output->changes |= changes_position;
	    any_set = True;
	}
	if (!keep_going)
	    break;
	if (!any_set)
	    fatal ("loop in relative position specifications\n");
    }

    /*
     * Now normalize positions so the upper left corner of all outputs is at 0,0
     */
    min_x = 32768;
    min_y = 32768;
    for (output = all_outputs; output; output = output->next)
    {
	if (output->mode_info == NULL) continue;
	
	if (output->x < min_x) min_x = output->x;
	if (output->y < min_y) min_y = output->y;
    }
    if (min_x || min_y)
    {
	/* move all outputs */
	for (output = all_outputs; output; output = output->next)
	{
	    if (output->mode_info == NULL) continue;

	    output->x -= min_x;
	    output->y -= min_y;
	    output->changes |= changes_position;
	}
    }
}

static void
set_screen_size (void)
{
    output_t	*output;
    Bool	fb_specified = fb_width != 0 && fb_height != 0;
    
    for (output = all_outputs; output; output = output->next)
    {
	XRRModeInfo *mode_info = output->mode_info;
	int	    x, y, w, h;
	box_t	    bounds;
	
	if (!mode_info) continue;
	
	mode_geometry (mode_info, output->rotation,
		       &output->transform.transform,
		       &bounds);
	x = output->x + bounds.x1;
	y = output->y + bounds.y1;
	w = bounds.x2 - bounds.x1;
	h = bounds.y2 - bounds.y1;
	/* make sure output fits in specified size */
	if (fb_specified)
	{
	    if (x + w > fb_width || y + h > fb_height)
		warning ("specified screen %dx%d not large enough for output %s (%dx%d+%d+%d)\n",
			 fb_width, fb_height, output->output.string, w, h, x, y);
	}
	/* fit fb to output */
	else
	{
	    XRRPanning *pan;
	    if (x + w > fb_width)
		fb_width = x + w;
	    if (y + h > fb_height)
		fb_height = y + h;
	    if (output->changes & changes_panning)
		pan = &output->panning;
	    else
		pan = output->crtc_info ? output->crtc_info->panning_info : NULL;
	    if (pan && pan->left + pan->width > fb_width)
		fb_width = pan->left + pan->width;
	    if (pan && pan->top + pan->height > fb_height)
		fb_height = pan->top + pan->height;
	}
    }	

    if (fb_width > maxWidth || fb_height > maxHeight)
        fatal ("screen cannot be larger than %dx%d (desired size %dx%d)\n",
	       maxWidth, maxHeight, fb_width, fb_height);
    if (fb_specified)
    {
	if (fb_width < minWidth || fb_height < minHeight)
	    fatal ("screen must be at least %dx%d\n", minWidth, minHeight);
    }
    else
    {
	if (fb_width < minWidth) fb_width = minWidth;
	if (fb_height < minHeight) fb_height = minHeight;
    }
}
    

static void
disable_outputs (output_t *outputs)
{
    while (outputs)
    {
	outputs->crtc_info = NULL;
	outputs = outputs->next;
    }
}

/*
 * find the best mapping from output to crtc available
 */
static int
pick_crtcs_score (output_t *outputs)
{
    output_t	*output;
    int		best_score;
    int		my_score;
    int		score;
    crtc_t	*best_crtc;
    int		c;
    
    if (!outputs)
	return 0;
    
    output = outputs;
    outputs = outputs->next;
    /*
     * Score with this output disabled
     */
    output->crtc_info = NULL;
    best_score = pick_crtcs_score (outputs);
    if (output->mode_info == NULL)
	return best_score;

    best_crtc = NULL;
    /* 
     * Now score with this output any valid crtc
     */
    for (c = 0; c < output->output_info->ncrtc; c++)
    {
	crtc_t	    *crtc;

	crtc = find_crtc_by_xid (output->output_info->crtcs[c]);
	if (!crtc)
	    fatal ("cannot find crtc 0x%lx\n", output->output_info->crtcs[c]);
	
	/* reset crtc allocation for following outputs */
	disable_outputs (outputs);
	if (!check_crtc_for_output (crtc, output))
	    continue;
	
	my_score = 1000;
	/* slight preference for existing connections */
	if (crtc == output->current_crtc_info)
	    my_score++;

	output->crtc_info = crtc;
	score = my_score + pick_crtcs_score (outputs);
	if (score > best_score)
	{
	    best_crtc = crtc;
	    best_score = score;
	}
    }
    if (output->crtc_info != best_crtc)
	output->crtc_info = best_crtc;
    /*
     * Reset other outputs based on this one using the best crtc
     */
    (void) pick_crtcs_score (outputs);

    return best_score;
}

/*
 * Pick crtcs for any changing outputs that don't have one
 */
static void
pick_crtcs (void)
{
    output_t	*output;
    int saved_crtc_noutput[num_crtcs];
    int n;

    /*
     * First try to match up newly enabled outputs with spare crtcs
     */
    for (output = all_outputs; output; output = output->next)
    {
	if (output->changes && output->mode_info)
	{
	    if (output->crtc_info) {
		if (output->crtc_info->crtc_info->noutput > 0 &&
		    (output->crtc_info->crtc_info->noutput > 1 ||
		     output != find_output_by_xid (output->crtc_info->crtc_info->outputs[0])))
		    break;
	    } else {
		output->crtc_info = find_crtc_for_output (output);
		if (!output->crtc_info)
		    break;
	    }
	}
    }
    /*
     * Everyone is happy
     */
    if (!output)
	return;
    /*
     * When the simple way fails, see if there is a way
     * to swap crtcs around and make things work
     */
    for (output = all_outputs; output; output = output->next)
	output->current_crtc_info = output->crtc_info;

    /* Mark all CRTC as currently unused */
    for (n = 0; n < num_crtcs; n++) {
	    saved_crtc_noutput[n] = crtcs[n].crtc_info->noutput;
	    crtcs[n].crtc_info->noutput = 0;
    }

    pick_crtcs_score (all_outputs);

    for (n = 0; n < num_crtcs; n++)
	    crtcs[n].crtc_info->noutput = saved_crtc_noutput[n];

    for (output = all_outputs; output; output = output->next)
    {
	if (output->mode_info && !output->crtc_info)
	    fatal ("cannot find crtc for output %s\n", output->output.string);
	if (!output->changes && output->crtc_info != output->current_crtc_info)
	    output->changes |= changes_crtc;
    }
}

static int
check_strtol(char *s)
{
    char *endptr;
    int result = strtol(s, &endptr, 10);
    if (s == endptr)
	argerr ("failed to parse '%s' as a number\n", s);
    return result;
}

static double
check_strtod(char *s)
{
    char *endptr;
    double result = strtod(s, &endptr);
    if (s == endptr)
	argerr ("failed to parse '%s' as a number\n", s);
    return result;
}


static void *
property_values_from_string(const char *str, const Atom type, const int format,
                            int *returned_nitems)
{
    char *token, *tmp;
    void *returned_bytes = NULL;
    int nitems = 0, bytes_per_item;

    if (type != XA_INTEGER && type != XA_CARDINAL)
	return NULL;

    /* compute memory needed for Xlib datatype (sigh) */
    switch (format) {
    case 8:
       bytes_per_item = sizeof(char);
       break;
    case 16:
       bytes_per_item = sizeof(short);
       break;
    case 32:
       bytes_per_item = sizeof(long);
       break;
    default:
       return NULL;
    }

    tmp = strdup (str);

    for (token = strtok (tmp, ","); token; token = strtok (NULL, ","))
    {
	char *endptr;
	long int val = strtol (token, &endptr, 0);

	if (token == endptr || *endptr != '\0')
	{
	    argerr ("failed to parse '%s' as a number\n", token);
	}

	returned_bytes = realloc (returned_bytes, (nitems + 1) * bytes_per_item);

	if (type == XA_INTEGER && format == 8)
	{
	    signed char *ptr = returned_bytes;
	    ptr[nitems] = (char) val;
	}
	else if (type == XA_INTEGER && format == 16)
	{
	    short *ptr = returned_bytes;
	    ptr[nitems] = (short) val;
	}
	else if (type == XA_INTEGER && format == 32)
	{
	    long *ptr = returned_bytes;
	    ptr[nitems] = (long) val;
	}
	else if (type == XA_CARDINAL && format == 8)
	{
	    unsigned char *ptr = returned_bytes;
	    ptr[nitems] = (unsigned char) val;
	}
	else if (type == XA_CARDINAL && format == 16)
	{
	    unsigned short *ptr = returned_bytes;
	    ptr[nitems] = (unsigned short) val;
	}
	else if (type == XA_CARDINAL && format == 32)
	{
	    unsigned long *ptr = returned_bytes;
	    ptr[nitems] = (unsigned long) val;
	}
	else
	{
	    free (tmp);
	    free (returned_bytes);
	    return NULL;
	}

	nitems++;
    }

    free (tmp);

    *returned_nitems = nitems;
    return returned_bytes;
}


static void
print_output_property_value(int value_format, /* 8, 16, 32 */
                            Atom value_type,  /* XA_{ATOM,INTEGER,CARDINAL} */
                            const void *value_bytes)
{
    if (value_type == XA_ATOM && value_format == 32)
    {
	const Atom *val = value_bytes;
	char *str = XGetAtomName (dpy, *val);
	if (str != NULL)
	{
	    printf ("%s", str);
	    XFree (str);
	    return;
	}
    }

    if (value_type == XA_INTEGER)
    {
	if (value_format == 8)
	{
	    const signed char *val = value_bytes;
	    printf ("%d", *val);
	    return;
	}
	if (value_format == 16)
	{
	    const short *val = value_bytes;
	    printf ("%d", *val);
	    return;
	}
	if (value_format == 32)
	{
	    const long *val = value_bytes;
	    printf ("%ld", *val);
	    return;
	}
    }

    if (value_type == XA_CARDINAL)
    {
	if (value_format == 8)
	{
	    const unsigned char *val = value_bytes;
	    printf ("%u", *val);
	    return;
	}
	if (value_format == 16)
	{
	    const unsigned short *val = value_bytes;
	    printf ("%u", *val);
	    return;
	}
	if (value_format == 32)
	{
	    const unsigned long *val = value_bytes;
	    printf ("%lu", *val);
	    return;
	}
    }

    printf ("?");
}

static void
print_edid(int nitems, const unsigned char *prop)
{
    int k;

    printf ("\n\t\t");

    for (k = 0; k < nitems; k++)
    {
	if (k != 0 && (k % 16) == 0)
	{
	    printf ("\n\t\t");
	}

	printf("%02" PRIx8, prop[k]);
    }

    printf("\n");
}

static void
print_guid(const unsigned char *prop)
{
    int k;

    printf("{");

    for (k = 0; k < 16; k++)
    {
	printf("%02" PRIX8, prop[k]);
	if (k == 3 || k == 5 || k == 7 || k == 9)
	{
	    printf("-");
	}
    }

    printf("}\n");
}

static void
print_output_property(const char *atom_name,
                      int value_format,
                      Atom value_type,
                      int nitems,
                      const unsigned char *prop)
{
    int bytes_per_item;
    int k;

    switch (value_format) {
    case 8:
       bytes_per_item = sizeof(char);
       break;
    case 16:
       bytes_per_item = sizeof(short);
       break;
    case 32:
       bytes_per_item = sizeof(long);
       break;
    default:
       return;
    }
    /*
     * Check for properties that need special formatting.
     */
    if (strcmp (atom_name, "EDID") == 0 && value_format == 8 &&
	value_type == XA_INTEGER)
    {
	print_edid (nitems, prop);
	return;
    }
    else if (strcmp (atom_name, "GUID") == 0 && value_format == 8 &&
	     value_type == XA_INTEGER && nitems == 16)
    {
	print_guid (prop);
	return;
    }

    for (k = 0; k < nitems; k++)
    {
	if (k != 0)
	{
	    if ((k % 16) == 0)
	    {
		printf ("\n\t\t");
	    }
	}
	print_output_property_value (value_format, value_type,
				     prop + (k * bytes_per_item));
	printf (" ");
    }

    printf ("\n");
}

static void
get_providers (void)
{
    XRRProviderResources *pr;
    int i;

    if (!has_1_4 || providers)
	return;

    pr = XRRGetProviderResources(dpy, root);
    num_providers = pr->nproviders;
    providers = calloc (num_providers, sizeof (provider_t));
    if (!providers)
	fatal ("out of memory\n");

    for (i = 0; i < num_providers; i++) {
	provider_t *provider = &providers[i];
	name_t *name = &provider->provider;
	XRRProviderInfo *info = XRRGetProviderInfo(dpy, res, pr->providers[i]);

	provider->info = info;
	set_name_xid (name, pr->providers[i]);
	set_name_index (name, i);
	set_name_string (name, info->name);
   }

   XRRFreeProviderResources(pr);
}

static provider_t *
find_provider (name_t *name)
{
    int i;

    if ((name->kind & name_xid) && name->xid == 0)
	return NULL;
    for (i = 0; i < num_providers; i++) {
	provider_t *p = &providers[i];
	name_kind_t common = name->kind & p->provider.kind;

	if ((common & name_xid) && name->xid == p->provider.xid)
	    return p;
	if ((common & name_string) && !strcmp (name->string, p->provider.string))
	    return p;
	if ((common & name_index) && name->index == p->provider.index)
	    return p;
    }

    printf ("Could not find provider with ");
    print_name (name);
    printf ("\n");
    exit (1);
}

static void
get_monitors(Bool get_active)
{
    XRRMonitorInfo	*m;
    int			n;

    if (!has_1_5 || monitors)
	return;

    m = XRRGetMonitors(dpy, root, get_active, &n);
    if (n == -1)
	fatal("get monitors failed\n");
    monitors = calloc(1, sizeof (monitors_t));
    monitors->n = n;
    monitors->monitors = m;
}

int
main (int argc, char **argv)
{
    XRRScreenSize *sizes;
    XRRScreenConfiguration *sc;
    int		nsize;
    int		nrate;
    short		*rates;
    Status	status = RRSetConfigFailed;
    int		rot = -1;
    int		query = False;
    int		action_requested = False;
    Rotation	current_rotation;
    XEvent	event;
    XRRScreenChangeNotifyEvent *sce;    
    char          *display_name = NULL;
    int 		i;
    SizeID	current_size;
    short	current_rate;
    double    	rate = -1;
    int		size = -1;
    int		dirind = 0;
    Bool	setit = False;
    Bool    	version = False;
    int		event_base, error_base;
    int		reflection = 0;
    int		width = 0, height = 0;
    Bool    	have_pixel_size = False;
    int		ret = 0;
    output_t	*config_output = NULL;
    Bool    	setit_1_2 = False;
    Bool    	query_1_2 = False;
    Bool	modeit = False;
    Bool	propit = False;
    Bool	query_1 = False;
    Bool	list_providers = False;
    Bool        provsetoutsource = False;
    Bool        provsetoffsink = False;
    Bool	monitorit = False;
    Bool	list_monitors = False;
    Bool	list_active_monitors = False;
    int		major, minor;
    Bool	current = False;
    Bool	toggle_x = False;
    Bool	toggle_y = False;

    program_name = argv[0];
    for (i = 1; i < argc; i++) {
	if (!strcmp ("-display", argv[i]) || !strcmp ("--display", argv[i]) ||
	    !strcmp ("-d", argv[i])) {
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    display_name = argv[i];
	    continue;
	}
	if (!strcmp("-help", argv[i]) || !strcmp("--help", argv[i])) {
	    usage();
	    exit(0);
	}
	if (!strcmp ("--verbose", argv[i])) {
	    verbose = True;
	    continue;
	}
	if (!strcmp ("--dryrun", argv[i])) {
	    dryrun = True;
	    verbose = True;
	    continue;
	}
	if (!strcmp ("--nograb", argv[i])) {
	    grab_server = False;
	    continue;
	}
	if (!strcmp("--current", argv[i])) {
	    current = True;
	    continue;
	}

	if (!strcmp ("-s", argv[i]) || !strcmp ("--size", argv[i])) {
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf (argv[i], "%dx%d", &width, &height) == 2) {
		have_pixel_size = True;
	    } else {
		size = check_strtol(argv[i]);
                if (size < 0) argerr ("--size argument must be nonnegative\n");
            }
	    setit = True;
	    action_requested = True;
	    continue;
	}

	if (!strcmp ("-r", argv[i]) ||
	    !strcmp ("--rate", argv[i]) ||
	    !strcmp ("--refresh", argv[i]))
	{
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    rate = check_strtod(argv[i]);
	    setit = True;
	    if (config_output)
	    {
		config_output->refresh = rate;
		config_output->changes |= changes_refresh;
		setit_1_2 = True;
	    }
	    action_requested = True;
	    continue;
	}

	if (!strcmp ("-v", argv[i]) || !strcmp ("--version", argv[i])) {
	    version = True;
	    action_requested = True;
	    continue;
	}

	if (!strcmp ("-x", argv[i])) {
	    toggle_x = True;
	    setit = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("-y", argv[i])) {
	    toggle_y = True;
	    setit = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--screen", argv[i])) {
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    screen = check_strtol(argv[i]);
	    if (screen < 0) argerr ("--screen argument must be nonnegative\n");
	    continue;
	}
	if (!strcmp ("-q", argv[i]) || !strcmp ("--query", argv[i])) {
	    query = True;
	    continue;
	}
	if (!strcmp ("-o", argv[i]) || !strcmp ("--orientation", argv[i])) {
	    char *endptr;
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    dirind = strtol(argv[i], &endptr, 10);
	    if (argv[i] == endptr) {
		for (dirind = 0; dirind < 4; dirind++) {
		    if (strcmp (direction[dirind], argv[i]) == 0) break;
		}
	    }
	    if ((dirind < 0) || (dirind > 3))
		argerr ("%s: invalid argument '%s'\n", argv[i-1], argv[i]);
	    rot = dirind;
	    setit = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--prop", argv[i]) ||
	    !strcmp ("--props", argv[i]) ||
	    !strcmp ("--madprops", argv[i]) ||
	    !strcmp ("--properties", argv[i]))
	{
	    query_1_2 = True;
	    properties = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--output", argv[i])) {
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);

	    config_output = find_output_by_name (argv[i]);
	    if (!config_output) {
		config_output = add_output ();
		set_name (&config_output->output, argv[i], name_string|name_xid);
	    }
	    
	    setit_1_2 = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--crtc", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    set_name (&config_output->crtc, argv[i], name_xid|name_index);
	    config_output->changes |= changes_crtc;
	    continue;
	}
	if (!strcmp ("--mode", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    set_name (&config_output->mode, argv[i], name_string|name_xid);
	    config_output->changes |= changes_mode;
	    continue;
	}
	if (!strcmp ("--preferred", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    set_name_preferred (&config_output->mode);
	    config_output->changes |= changes_mode;
	    continue;
	}
	if (!strcmp ("--pos", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf (argv[i], "%dx%d",
			&config_output->x, &config_output->y) != 2)
		argerr ("failed to parse '%s' as a position\n", argv[i]);
	    config_output->changes |= changes_position;
	    continue;
	}
	if (!strcmp ("--rotation", argv[i]) || !strcmp ("--rotate", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    for (dirind = 0; dirind < 4; dirind++) {
		if (strcmp (direction[dirind], argv[i]) == 0) break;
	    }
	    if (dirind == 4)
		argerr ("%s: invalid argument '%s'\n", argv[i-1], argv[i]);
	    config_output->rotation &= ~0xf;
	    config_output->rotation |= 1 << dirind;
	    config_output->changes |= changes_rotation;
	    continue;
	}
	if (!strcmp ("--reflect", argv[i]) || !strcmp ("--reflection", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    for (dirind = 0; dirind < 4; dirind++) {
		if (strcmp (reflections[dirind], argv[i]) == 0) break;
	    }
	    if (dirind == 4)
		argerr ("%s: invalid argument '%s'\n", argv[i-1], argv[i]);
	    config_output->rotation &= ~(RR_Reflect_X|RR_Reflect_Y);
	    config_output->rotation |= dirind * RR_Reflect_X;
	    config_output->changes |= changes_reflection;
	    continue;
	}
	if (!strcmp ("--left-of", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    config_output->relation = relation_left_of;
	    config_output->relative_to = argv[i];
	    config_output->changes |= changes_relation;
	    continue;
	}
	if (!strcmp ("--right-of", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    config_output->relation = relation_right_of;
	    config_output->relative_to = argv[i];
	    config_output->changes |= changes_relation;
	    continue;
	}
	if (!strcmp ("--above", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    config_output->relation = relation_above;
	    config_output->relative_to = argv[i];
	    config_output->changes |= changes_relation;
	    continue;
	}
	if (!strcmp ("--below", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    config_output->relation = relation_below;
	    config_output->relative_to = argv[i];
	    config_output->changes |= changes_relation;
	    continue;
	}
	if (!strcmp ("--same-as", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    config_output->relation = relation_same_as;
	    config_output->relative_to = argv[i];
	    config_output->changes |= changes_relation;
	    continue;
	}
	if (!strcmp ("--panning", argv[i])) {
	    XRRPanning *pan;
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    pan = &config_output->panning;
	    switch (sscanf (argv[i], "%dx%d+%d+%d/%dx%d+%d+%d/%d/%d/%d/%d",
			    &pan->width, &pan->height, &pan->left, &pan->top,
			    &pan->track_width, &pan->track_height,
			    &pan->track_left, &pan->track_top,
			    &pan->border_left, &pan->border_top,
			    &pan->border_right, &pan->border_bottom)) {
	    case 2:
		pan->left = pan->top = 0;
		/* fall through */
	    case 4:
		pan->track_left = pan->track_top =
		    pan->track_width = pan->track_height = 0;
		/* fall through */
	    case 8:
		pan->border_left = pan->border_top =
		    pan->border_right = pan->border_bottom = 0;
		/* fall through */
	    case 12:
		break;
	    default:
		argerr ("%s: invalid argument '%s'\n", argv[i-1], argv[i]);
	    }
	    config_output->changes |= changes_panning;
	    continue;
	}
	if (!strcmp ("--gamma", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf(argv[i], "%f:%f:%f", &config_output->gamma.red,
		    &config_output->gamma.green, &config_output->gamma.blue) != 3)
		argerr ("%s: invalid argument '%s'\n", argv[i-1], argv[i]);
	    config_output->changes |= changes_gamma;
	    setit_1_2 = True;
	    continue;
	}
	if (!strcmp ("--brightness", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf(argv[i], "%f", &config_output->brightness) != 1)
		argerr ("%s: invalid argument '%s'\n", argv[i-1], argv[i]);
	    config_output->changes |= changes_gamma;
	    setit_1_2 = True;
	    continue;
	}
	if (!strcmp ("--primary", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    config_output->changes |= changes_primary;
	    config_output->primary = True;
	    setit_1_2 = True;
	    continue;
	}
	if (!strcmp ("--noprimary", argv[i])) {
	    no_primary = True;
	    setit_1_2 = True;
	    continue;
	}
	if (!strcmp ("--set", argv[i])) {
	    output_prop_t   *prop;
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (i+2 >= argc) argerr ("%s requires two arguments\n", argv[i]);
	    prop = malloc (sizeof (output_prop_t));
	    prop->next = config_output->props;
	    config_output->props = prop;
	    prop->name = argv[++i];
	    prop->value = argv[++i];
	    propit = True;
	    config_output->changes |= changes_property;
	    setit_1_2 = True;
	    continue;
	}
	if (!strcmp ("--scale", argv[i]))
	{
	    double  sx, sy;
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf (argv[i], "%lfx%lf", &sx, &sy) != 2)
		argerr ("failed to parse '%s' as a scaling factor\n", argv[i]);
	    init_transform (&config_output->transform);
	    config_output->transform.transform.matrix[0][0] = XDoubleToFixed (sx);
	    config_output->transform.transform.matrix[1][1] = XDoubleToFixed (sy);
	    config_output->transform.transform.matrix[2][2] = XDoubleToFixed (1.0);
	    if (sx != 1 || sy != 1)
		config_output->transform.filter = "bilinear";
	    else
		config_output->transform.filter = "nearest";
	    config_output->transform.nparams = 0;
	    config_output->transform.params = NULL;
	    config_output->changes |= changes_transform;
	    continue;
	}
	if (!strcmp ("--scale-from", argv[i]))
	{
	    int w, h;
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf (argv[i], "%dx%d", &w, &h) != 2)
		argerr ("failed to parse '%s' as a scale-from size\n", argv[i]);
	    if (w <=0 || h <= 0)
		argerr ("--scale-from dimensions must be nonnegative\n");
	    config_output->scale_from_w = w;
	    config_output->scale_from_h = h;
	    config_output->changes |= changes_transform;
	    continue;
	}
	if (!strcmp ("--transform", argv[i])) {
	    double  transform[3][3];
	    int	    k, l;
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    init_transform (&config_output->transform);
	    if (strcmp (argv[i], "none") != 0)
	    {
		if (sscanf(argv[i], "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
			   &transform[0][0],&transform[0][1],&transform[0][2],
			   &transform[1][0],&transform[1][1],&transform[1][2],
			   &transform[2][0],&transform[2][1],&transform[2][2])
		    != 9)
		    argerr ("failed to parse '%s' as a transformation\n", argv[i]);
		init_transform (&config_output->transform);
		for (k = 0; k < 3; k++)
		    for (l = 0; l < 3; l++) {
			config_output->transform.transform.matrix[k][l] = XDoubleToFixed (transform[k][l]);
		    }
		config_output->transform.filter = "bilinear";
		config_output->transform.nparams = 0;
		config_output->transform.params = NULL;
	    }
	    config_output->changes |= changes_transform;
	    continue;
	}
	if (!strcmp ("--off", argv[i])) {
	    if (!config_output) argerr ("%s must be used after --output\n", argv[i]);
	    set_name_xid (&config_output->mode, None);
	    set_name_xid (&config_output->crtc, None);
	    config_output->changes |= changes_mode | changes_crtc;
	    continue;
	}
	if (!strcmp ("--fb", argv[i])) {
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf (argv[i], "%dx%d",
			&fb_width, &fb_height) != 2)
		argerr ("failed to parse '%s' as a framebuffer size\n", argv[i]);
	    setit_1_2 = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--fbmm", argv[i])) {
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    if (sscanf (argv[i], "%dx%d",
			&fb_width_mm, &fb_height_mm) != 2)
		argerr ("failed to parse '%s' as a physical size\n", argv[i]);
	    setit_1_2 = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--dpi", argv[i])) {
	    char *strtod_error;
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    dpi = strtod(argv[i], &strtod_error);
	    if (argv[i] == strtod_error)
	    {
		dpi = 0.0;
		dpi_output_name = argv[i];
	    }
	    setit_1_2 = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--auto", argv[i])) {
	    if (config_output)
	    {
		config_output->automatic = True;
		config_output->changes |= changes_automatic;
	    }
	    else
		automatic = True;
	    setit_1_2 = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--q12", argv[i]))
	{
	    query_1_2 = True;
	    continue;
	}
	if (!strcmp ("--q1", argv[i]))
	{
	    query_1 = True;
	    continue;
	}
	if (!strcmp ("--newmode", argv[i]))
	{
	    umode_t  *m = calloc (1, sizeof (umode_t));
	    double    clock;
	    
	    ++i;
	    if (i + 9 >= argc)
		argerr ("failed to parse '%s' as a mode specification\n", argv[i]);
	    m->mode.name = argv[i];
	    m->mode.nameLength = strlen (argv[i]);
	    i++;
	    clock = check_strtod(argv[i++]);
	    m->mode.dotClock = clock * 1e6;

	    m->mode.width = check_strtol(argv[i++]);
	    m->mode.hSyncStart = check_strtol(argv[i++]);
	    m->mode.hSyncEnd = check_strtol(argv[i++]);
	    m->mode.hTotal = check_strtol(argv[i++]);
	    m->mode.height = check_strtol(argv[i++]);
	    m->mode.vSyncStart = check_strtol(argv[i++]);
	    m->mode.vSyncEnd = check_strtol(argv[i++]);
	    m->mode.vTotal = check_strtol(argv[i++]);
	    m->mode.modeFlags = 0;
	    while (i < argc) {
		int f;
		
		for (f = 0; mode_flags[f].string; f++)
		    if (!strcasecmp (mode_flags[f].string, argv[i]))
			break;
		
		if (!mode_flags[f].string)
		    break;
    		m->mode.modeFlags |= mode_flags[f].flag;
    		i++;
	    }
	    m->next = umodes;
	    m->action = umode_create;
	    umodes = m;
	    modeit = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--rmmode", argv[i]))
	{
	    umode_t  *m = calloc (1, sizeof (umode_t));

	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    set_name (&m->name, argv[i], name_string|name_xid);
	    m->action = umode_destroy;
	    m->next = umodes;
	    umodes = m;
	    modeit = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--addmode", argv[i]))
	{
	    umode_t  *m = calloc (1, sizeof (umode_t));

	    if (i+2 >= argc) argerr ("%s requires two arguments\n", argv[i]);
	    set_name (&m->output, argv[++i], name_string|name_xid);
	    set_name (&m->name, argv[++i], name_string|name_xid);
	    m->action = umode_add;
	    m->next = umodes;
	    umodes = m;
	    modeit = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--delmode", argv[i]))
	{
	    umode_t  *m = calloc (1, sizeof (umode_t));

	    if (i+2 >= argc) argerr ("%s requires two arguments\n", argv[i]);
	    set_name (&m->output, argv[++i], name_string|name_xid);
	    set_name (&m->name, argv[++i], name_string|name_xid);
	    m->action = umode_delete;
	    m->next = umodes;
	    umodes = m;
	    modeit = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp ("--listproviders", argv[i]))
	{
	    list_providers = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp("--setprovideroutputsource", argv[i]))
	{ 
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    set_name (&provider_name, argv[i], name_string|name_xid|name_index);
	    if (++i>=argc) 
		set_name_xid (&output_source_provider_name, 0);
	    else
		set_name (&output_source_provider_name, argv[i], name_string|name_xid|name_index);
	    action_requested = True;
	    provsetoutsource = True;
	    continue;
	}
	if (!strcmp("--setprovideroffloadsink", argv[i]))
	{ 
	    if (++i >= argc) argerr ("%s requires an argument\n", argv[i-1]);
	    set_name (&provider_name, argv[i], name_string|name_xid|name_index);
	    if (++i>=argc) 
		set_name_xid (&offload_sink_provider_name, 0);
	    else
		set_name (&offload_sink_provider_name, argv[i], name_string|name_xid|name_index);
	    action_requested = True;
	    provsetoffsink = True;
	    continue;
	}
	if (!strcmp("--listmonitors", argv[i]))
	{
	    list_monitors = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp("--listactivemonitors", argv[i]))
	{
	    list_active_monitors = True;
	    action_requested = True;
	    continue;
	}
	if (!strcmp("--setmonitor", argv[i]))
	{
	    umonitor_t	*m = calloc(1, sizeof (umonitor_t)), **l;
	    char	*t;
	    char	*o;
	    char	*n;
	    char	*geom;

	    if (i+3 >= argc) argerr("%s requires three argument\n", argv[i]);
	    n = argv[++i];
	    if (*n == '*') {
		m->primary = True;
		n++;
	    }
	    m->name = n;
	    m->set = True;
	    geom = argv[++i];

	    if (strncmp (geom, "auto", 4) != 0) {
		if (sscanf(geom, "%d/%dx%d/%d+%d+%d",
			   &m->width, &m->mmwidth, &m->height, &m->mmheight, &m->x, &m->y) != 6)
		    argerr ("failed to parse '%s' as monitor geometry\n", argv[i]);
	    }

	    o = argv[++i];
	    if (strcmp(o, "none") != 0) {
		printf ("output list %s\n", o);
		for (; (t = strtok(o, ",")) != NULL; o = NULL) {
		    m->outputs = realloc(m->outputs, (m->noutput + 1) * sizeof (name_t));
		    printf ("add monitor %s\n", t);
		    set_name(&m->outputs[m->noutput++], t, name_string|name_xid|name_index);
		    printf ("output name %s\n", m->outputs[m->noutput-1].string);
		}
	    }
	    for (l = &umonitors; *l; l = &((*l)->next));
	    *l = m;
	    action_requested = True;
	    monitorit = True;
	    continue;
	}
	if (!strcmp("--delmonitor", argv[i]))
	{
	    umonitor_t	*m = calloc(1, sizeof (umonitor_t)), **l;

	    if (++i >= argc) argerr("%s requires an argument\n", argv[i-1]);

	    m->name = argv[i];
	    m->set = False;
	    for (l = &umonitors; *l; l = &((*l)->next));
	    *l = m;
	    action_requested = True;
	    monitorit = True;
	    continue;
	}

	argerr ("unrecognized option '%s'\n", argv[i]);
    }
    if (!action_requested)
	    query = True;
    if (verbose) 
    {
	query = True;
	if (setit && !setit_1_2)
	    query_1 = True;
    }
    if (version)
	printf("xrandr program version       " VERSION "\n");

    dpy = XOpenDisplay (display_name);

    if (dpy == NULL) {
	fprintf (stderr, "Can't open display %s\n", XDisplayName(display_name));
	exit (1);
    }
    if (screen < 0)
	screen = DefaultScreen (dpy);
    if (screen >= ScreenCount (dpy)) {
	fprintf (stderr, "Invalid screen number %d (display has %d)\n",
		 screen, ScreenCount (dpy));
	exit (1);
    }

    root = RootWindow (dpy, screen);

    if (!XRRQueryExtension (dpy, &event_base, &error_base) ||
        !XRRQueryVersion (dpy, &major, &minor))
    {
	fprintf (stderr, "RandR extension missing\n");
	exit (1);
    }
    if (major > 1 || (major == 1 && minor >= 2))
	has_1_2 = True;
    if (major > 1 || (major == 1 && minor >= 3))
	has_1_3 = True;
    if (major > 1 || (major == 1 && minor >= 4))
	has_1_4 = True;
    if (major > 1 || (major == 1 && minor >= 5))
	has_1_5 = True;
    if (has_1_2 && modeit)
    {
	umode_t	*m;

        get_screen (True);
	get_crtcs();
	get_outputs();
	
	for (m = umodes; m; m = m->next)
	{
	    XRRModeInfo *e;
	    output_t	*o;
	    
	    switch (m->action) {
	    case umode_create:
		XRRCreateMode (dpy, root, &m->mode);
		break;
	    case umode_destroy:
		e = find_mode (&m->name, 0);
		if (!e)
		    fatal ("cannot find mode \"%s\"\n", m->name.string);
		XRRDestroyMode (dpy, e->id);
		break;
	    case umode_add:
		o = find_output (&m->output);
		if (!o)
		    fatal ("cannot find output \"%s\"\n", m->output.string);
		e = find_mode (&m->name, 0);
		if (!e)
		    fatal ("cannot find mode \"%s\"\n", m->name.string);
		XRRAddOutputMode (dpy, o->output.xid, e->id);
		break;
	    case umode_delete:
		o = find_output (&m->output);
		if (!o)
		    fatal ("cannot find output \"%s\"\n", m->output.string);
		e = find_mode (&m->name, 0);
		if (!e)
		    fatal ("cannot find mode \"%s\"\n", m->name.string);
		XRRDeleteOutputMode (dpy, o->output.xid, e->id);
		break;
	    }
	}
	if (!propit && !setit_1_2 && !monitorit)
	{
	    XSync (dpy, False);
	    exit (0);
	}
    }
    if (has_1_2 && propit)
    {
	output_t *output;

        get_screen (True);
	get_crtcs();
	get_outputs();
	
	for (output = all_outputs; output; output = output->next)
	{
	    output_prop_t   *prop;

	    for (prop = output->props; prop; prop = prop->next)
	    {
		Atom		name = XInternAtom (dpy, prop->name, False);
		Atom		type;
		int		format = 0;
		unsigned char	*data, *malloced_data = NULL;
		int		nelements;
		int		int_value;
		unsigned long	ulong_value;
		unsigned char	*prop_data;
		int		actual_format;
		unsigned long	nitems, bytes_after;
		Atom		actual_type;
		XRRPropertyInfo *propinfo;

		type = AnyPropertyType;
		
		if (XRRGetOutputProperty (dpy, output->output.xid, name,
					  0, 100, False, False,
					  AnyPropertyType,
					  &actual_type, &actual_format,
					  &nitems, &bytes_after, &prop_data) == Success &&

		    (propinfo = XRRQueryOutputProperty(dpy, output->output.xid,
						      name)))
		{
		    type = actual_type;
		    format = actual_format;
		}

		malloced_data = property_values_from_string
		    (prop->value, type, actual_format, &nelements);

		if (malloced_data)
		{
		    data = malloced_data;
		    type = actual_type;
		    format = actual_format;
		}
		else if (type == AnyPropertyType &&
		    (sscanf (prop->value, "%d", &int_value) == 1 ||
		     sscanf (prop->value, "0x%x", &int_value) == 1))
		{
		    type = XA_INTEGER;
		    ulong_value = int_value;
		    data = (unsigned char *) &ulong_value;
		    nelements = 1;
		    format = 32;
		}
		else if (type == XA_ATOM)
		{
		    ulong_value = XInternAtom (dpy, prop->value, False);
		    data = (unsigned char *) &ulong_value;
		    nelements = 1;
		}
		else if (type == XA_STRING || type == AnyPropertyType)
		{
		    type = XA_STRING;
		    data = (unsigned char *) prop->value;
		    nelements = strlen (prop->value);
		    format = 8;
		}
		else
		    continue;
		XRRChangeOutputProperty (dpy, output->output.xid,
					 name, type, format, PropModeReplace,
					 data, nelements);
		free (malloced_data);
	    }
	}
	if (!setit_1_2)
	{
	    XSync (dpy, False);
	    exit (0);
	}
    }
    if (provsetoutsource)
    {
	provider_t *provider, *source;

	if (!has_1_4)
	    fatal ("--setprovideroutputsource requires RandR 1.4\n");

	get_screen (True);
	get_providers ();

	provider = find_provider (&provider_name);
	source = find_provider(&output_source_provider_name);

	XRRSetProviderOutputSource(dpy, provider->provider.xid, source ? source->provider.xid : 0);
    }
    if (provsetoffsink)
    {
	provider_t *provider, *sink;

	if (!has_1_4)
	    fatal ("--setprovideroffloadsink requires RandR 1.4\n");

	get_screen (True);
	get_providers ();

	provider = find_provider (&provider_name);
	sink = find_provider(&offload_sink_provider_name);

	XRRSetProviderOffloadSink(dpy, provider->provider.xid, sink ? sink->provider.xid : 0);
    }
    if (setit_1_2)
    {
	get_screen (True);
	get_crtcs ();
	get_outputs ();
	set_positions ();
	set_screen_size ();

	pick_crtcs ();

	/*
	 * Assign outputs to crtcs
	 */
	set_crtcs ();
	
	/*
	 * Mark changing crtcs
	 */
	mark_changing_crtcs ();

	/*
	 * If an output was specified to track dpi, use it
	 */
	if (dpi_output_name)
	{
	    output_t	*dpi_output = find_output_by_name (dpi_output_name);
	    XRROutputInfo	*output_info;
	    XRRModeInfo	*mode_info;
	    if (!dpi_output)
		fatal ("Cannot find output %s\n", dpi_output_name);
	    output_info = dpi_output->output_info;
	    mode_info = dpi_output->mode_info;
	    if (output_info && mode_info && output_info->mm_height)
	    {
		/*
		 * When this output covers the whole screen, just use
		 * the known physical size
		 */
		if (fb_width == mode_info->width &&
		    fb_height == mode_info->height)
		{
		    fb_width_mm = output_info->mm_width;
		    fb_height_mm = output_info->mm_height;
		}
		else
		{
		    dpi = (25.4 * mode_info->height) / output_info->mm_height;
		}
	    }
	}

	/*
	 * Compute physical screen size
	 */
	if (fb_width_mm == 0 || fb_height_mm == 0)
	{
	    if (fb_width != DisplayWidth (dpy, screen) ||
		fb_height != DisplayHeight (dpy, screen) || dpi != 0.0)
	    {
		if (dpi <= 0)
		    dpi = (25.4 * DisplayHeight (dpy, screen)) / DisplayHeightMM(dpy, screen);

		fb_width_mm = (25.4 * fb_width) / dpi;
		fb_height_mm = (25.4 * fb_height) / dpi;
	    }
	    else
	    {
		fb_width_mm = DisplayWidthMM (dpy, screen);
		fb_height_mm = DisplayHeightMM (dpy, screen);
	    }
	}
	
	/*
	 * Set panning
	 */
	set_panning ();

	/* 
	 * Set gamma on crtc's that belong to the outputs.
	 */
	set_gamma ();

	/*
	 * Now apply all of the changes
	 */
	apply ();
	
	if (!monitorit) {
	    XSync (dpy, False);
	    exit (0);
	}
    }
    if (monitorit) {
	umonitor_t	*u;
	Atom		name;

	if (!has_1_5) {
	    printf("RandR 1.5 not supported\n");
	    exit(0);
	}

	get_screen(True);
	get_monitors(True);
	get_crtcs();
	get_outputs();

	for (u = umonitors; u; u = u->next) {
	    if (u->set) {
		XRRMonitorInfo	*m;
		int		o;

		name = XInternAtom(dpy, u->name, False);
		m = XRRAllocateMonitor(dpy, u->noutput);

		m->name = name;
		m->primary = u->primary;
		m->x = u->x;
		m->y = u->y;
		m->width = u->width;
		m->height = u->height;
		m->mwidth = u->mmwidth;
		m->mheight = u->mmheight;
		for (o = 0; o < u->noutput; o++) {
		    output_t	*output = find_output(&u->outputs[o]);
		    if (!output)
			fatal("cannot find output\n");
		    m->outputs[o] = output->output.xid;
		}

		XRRSetMonitor(dpy, root, m);

		XRRFreeMonitors(m);
	    } else {
		int	m;

		name = XInternAtom(dpy, u->name, True);
		if (!name) {
		    printf("No monitor named '%s'\n", u->name);
		} else {
		    if (!monitors)
			printf ("No monitors\n");
		    else {
			for (m = 0; m < monitors->n; m++) {
			    if (monitors->monitors[m].name == name)
				break;
			}
			if (m == monitors->n)
			    printf("No monitor named '%s'\n", u->name);
			else
			    XRRDeleteMonitor(dpy, root, name);
		    }
		}
	    }
	}
	XSync (dpy, False);
	exit (0);
    }
    if (query_1_2 || (query && has_1_2 && !query_1))
    {
	output_t    *output;
	int	    m;
	
#define ModeShown   0x80000000
	
	get_screen (current);
	get_crtcs ();
	get_outputs ();

        printf ("Screen %d: minimum %d x %d, current %d x %d, maximum %d x %d\n",
		screen, minWidth, minHeight,
		DisplayWidth (dpy, screen), DisplayHeight(dpy, screen),
		maxWidth, maxHeight);

	for (output = all_outputs; output; output = output->next)
	{
	    XRROutputInfo   *output_info = output->output_info;
	    crtc_t	    *cur_crtc = output->crtc_info;
	    XRRCrtcInfo	    *crtc_info = cur_crtc ? cur_crtc->crtc_info : NULL;
	    XRRModeInfo	    *cur_mode = output->mode_info;
	    Atom	    *props;
	    int		    j, nprop;
	    Bool	    *mode_shown;
	    Rotation	    rotations = output_rotations (output);

	    printf ("%s %s", output_info->name, connection[output_info->connection]);
	    if (output->primary) {
		printf(" primary");
	    }
	    if (cur_mode)
	    {
		if (crtc_info) {
		    printf (" %dx%d+%d+%d",
			    crtc_info->width, crtc_info->height,
			    crtc_info->x, crtc_info->y);
		} else {
		    printf (" %dx%d+%d+%d",
			    cur_mode->width, cur_mode->height, output->x,
			    output->y);
		}
		if (verbose)
		    printf (" (0x%x)", (int)cur_mode->id);
		if (output->rotation != RR_Rotate_0 || verbose)
		{
		    printf (" %s", 
			    rotation_name (output->rotation));
		    if (output->rotation & (RR_Reflect_X|RR_Reflect_Y))
			printf (" %s", reflection_name (output->rotation));
		}
	    }
	    if (rotations != RR_Rotate_0 || verbose)
	    {
		Bool    first = True;
		printf (" (");
		for (i = 0; i < 4; i ++) {
		    if ((rotations >> i) & 1) {
			if (!first) printf (" "); first = False;
			printf("%s", direction[i]);
		    }
		}
		if (rotations & RR_Reflect_X)
		{
		    if (!first) printf (" "); first = False;
		    printf ("x axis");
		}
		if (rotations & RR_Reflect_Y)
		{
		    if (!first) printf (" ");
		    printf ("y axis");
		}
		printf (")");
	    }

	    if (cur_mode)
	    {
		printf (" %dmm x %dmm",
			(int)output_info->mm_width, (int)output_info->mm_height);
	    }

	    if (cur_crtc && cur_crtc->panning_info &&
		cur_crtc->panning_info->width > 0)
	    {
		XRRPanning *pan = cur_crtc->panning_info;
		printf (" panning %dx%d+%d+%d",
			pan->width, pan->height, pan->left, pan->top);
		if ((pan->track_width    != 0 &&
		     (pan->track_left    != pan->left		||
		      pan->track_width   != pan->width		||
		      pan->border_left   != 0			||
		      pan->border_right  != 0))			||
		    (pan->track_height   != 0 &&
		     (pan->track_top     != pan->top		||
		      pan->track_height  != pan->height		||
		      pan->border_top    != 0			||
		      pan->border_bottom != 0)))
		    printf (" tracking %dx%d+%d+%d border %d/%d/%d/%d",
			    pan->track_width,  pan->track_height,
			    pan->track_left,   pan->track_top,
			    pan->border_left,  pan->border_top,
			    pan->border_right, pan->border_bottom);
	    }
	    printf ("\n");

	    if (verbose)
	    {
		printf ("\tIdentifier: 0x%x\n", (int)output->output.xid);
		printf ("\tTimestamp:  %d\n", (int)output_info->timestamp);
		printf ("\tSubpixel:   %s\n", order[output_info->subpixel_order]);
	        if (output->gamma.red != 0.0 && output->gamma.green != 0.0 && output->gamma.blue != 0.0) {
		    printf ("\tGamma:      %#.2g:%#.2g:%#.2g\n",
			    output->gamma.red, output->gamma.green, output->gamma.blue);
		    printf ("\tBrightness: %#.2g\n", output->brightness);
		}
		printf ("\tClones:    ");
		for (j = 0; j < output_info->nclone; j++)
		{
		    output_t	*clone = find_output_by_xid (output_info->clones[j]);

		    if (clone) printf (" %s", clone->output.string);
		}
		printf ("\n");
		if (output->crtc_info)
		    printf ("\tCRTC:       %d\n", output->crtc_info->crtc.index);
		printf ("\tCRTCs:     ");
		for (j = 0; j < output_info->ncrtc; j++)
		{
		    crtc_t	*crtc = find_crtc_by_xid (output_info->crtcs[j]);
		    if (crtc)
			printf (" %d", crtc->crtc.index);
		}
		printf ("\n");
		if (output->crtc_info && output->crtc_info->panning_info) {
		    XRRPanning *pan = output->crtc_info->panning_info;
		    printf ("\tPanning:    %dx%d+%d+%d\n",
			    pan->width, pan->height, pan->left, pan->top);
		    printf ("\tTracking:   %dx%d+%d+%d\n",
			    pan->track_width,  pan->track_height,
			    pan->track_left,   pan->track_top);
		    printf ("\tBorder:     %d/%d/%d/%d\n",
			    pan->border_left,  pan->border_top,
			    pan->border_right, pan->border_bottom);
		}
	    }
	    if (verbose)
	    {
		int x, y;

		printf ("\tTransform: ");
		for (y = 0; y < 3; y++)
		{
		    for (x = 0; x < 3; x++)
			printf (" %f", XFixedToDouble (output->transform.transform.matrix[y][x]));
		    if (y < 2)
			printf ("\n\t           ");
		}
		if (output->transform.filter)
		    printf ("\n\t           filter: %s", output->transform.filter);
		printf ("\n");
	    }
	    if (verbose || properties)
	    {
		props = XRRListOutputProperties (dpy, output->output.xid,
						 &nprop);
		for (j = 0; j < nprop; j++) {
		    unsigned char *prop;
		    int actual_format;
		    unsigned long nitems, bytes_after;
		    Atom actual_type;
		    XRRPropertyInfo *propinfo;
		    char *atom_name = XGetAtomName (dpy, props[j]);
		    int k;

		    XRRGetOutputProperty (dpy, output->output.xid, props[j],
					  0, 100, False, False,
					  AnyPropertyType,
					  &actual_type, &actual_format,
					  &nitems, &bytes_after, &prop);

		    propinfo = XRRQueryOutputProperty(dpy, output->output.xid,
						      props[j]);

		    printf ("\t%s: ", atom_name);

		    print_output_property(atom_name, actual_format,
					  actual_type, nitems, prop);

		    if (propinfo->range && propinfo->num_values > 0)
		    {
			printf ("\t\trange%s: ",
			        (propinfo->num_values == 2) ? "" : "s");
			for (k = 0; k < propinfo->num_values / 2; k++)
			{
			    printf ("(");
			    print_output_property_value (32, actual_type,
							 (unsigned char *) &(propinfo->values[k * 2]));
			    printf (", ");
			    print_output_property_value (32, actual_type,
							 (unsigned char *) &(propinfo->values[k * 2 + 1]));
			    printf (")");
			    if (k < propinfo->num_values / 2 - 1)
				printf (", ");
			}
			printf ("\n");
		    }
		    if (!propinfo->range && propinfo->num_values > 0)
		    {
			printf ("\t\tsupported: ");
			for (k = 0; k < propinfo->num_values; k++)
			{
			    print_output_property_value (32, actual_type,
							 (unsigned char *) &(propinfo->values[k]));
			    if (k < propinfo->num_values - 1)
				printf (", ");
			}
			printf ("\n");
		    }

		    free(propinfo);
		}
	    }

	    if (verbose)
	    {
		for (j = 0; j < output_info->nmode; j++)
		{
		    XRRModeInfo	*mode = find_mode_by_xid (output_info->modes[j]);

		    print_verbose_mode (mode, mode == output->mode_info,
					j < output_info->npreferred);
		    mode->modeFlags |= ModeShown;
		}
	    }
	    else
	    {
		mode_shown = calloc (output_info->nmode, sizeof (Bool));
		if (!mode_shown) fatal ("out of memory\n");
		for (j = 0; j < output_info->nmode; j++)
		{
		    XRRModeInfo *jmode, *kmode;
		    int k;
		    
		    if (mode_shown[j]) continue;
    
		    jmode = find_mode_by_xid (output_info->modes[j]);
		    printf (" ");
		    printf ("  %-12s", jmode->name);
		    for (k = j; k < output_info->nmode; k++)
		    {
			if (mode_shown[k]) continue;
			kmode = find_mode_by_xid (output_info->modes[k]);
			if (strcmp (jmode->name, kmode->name) != 0) continue;
			mode_shown[k] = True;
			kmode->modeFlags |= ModeShown;
			printf (" %6.2f", mode_refresh (kmode));
			if (kmode == output->mode_info)
			    printf ("*");
			else
			    printf (" ");
			if (k < output_info->npreferred)
			    printf ("+");
			else
			    printf (" ");
		    }
		    printf ("\n");
		}
		free (mode_shown);
	    }
	}
	for (m = 0; m < res->nmode; m++)
	{
	    XRRModeInfo	*mode = &res->modes[m];

	    if (!(mode->modeFlags & ModeShown))
		print_verbose_mode(mode, False, False);
	}
	exit (0);
    }
    if (list_providers) {
	int k;

	if (!has_1_4) {
	    printf ("RandR 1.4 not supported\n");
	    exit (0);
	}

	get_screen (current);
	get_providers ();

	if (providers) {
	    int j;

	    printf("Providers: number : %d\n", num_providers);

	    for (j = 0; j < num_providers; j++) {
		provider_t *provider = &providers[j];
		XRRProviderInfo *info = provider->info;

		printf("Provider %d: id: 0x%x cap: 0x%x", j, (int)provider->provider.xid, info->capabilities);
		for (k = 0; k < 4; k++)
			if (info->capabilities & (1 << k))
				printf(", %s", capability_name(1<<k));

		printf(" crtcs: %d outputs: %d associated providers: %d name:%s\n", info->ncrtcs, info->noutputs, info->nassociatedproviders, info->name);
	    }
	}
    }
    if (list_monitors || list_active_monitors) {

	if (!has_1_5) {
	    printf("RandR 1.5 not supported\n");
	    exit(0);
	}

	get_screen(current);
	get_monitors(list_active_monitors ? True : False);
	get_crtcs();
	get_outputs();

	if (monitors) {
	    int m, o;

	    printf("Monitors: %d\n", monitors->n);

	    for (m = 0; m < monitors->n; m++) {
		printf (" %d: %s%s%s %d/%dx%d/%d+%d+%d ",
			m,
			monitors->monitors[m].automatic ? "+" : "",
			monitors->monitors[m].primary ? "*" : "",
			XGetAtomName(dpy, monitors->monitors[m].name),
			monitors->monitors[m].width,
			monitors->monitors[m].mwidth,
			monitors->monitors[m].height,
			monitors->monitors[m].mheight,
			monitors->monitors[m].x,
			monitors->monitors[m].y);
		for (o = 0; o < monitors->monitors[m].noutput; o++) {
		    output_t	*output = find_output_by_xid(monitors->monitors[m].outputs[o]);
		    if (output)
			printf (" %s", output->output.string);
		    else
			printf (" unknown output 0x%x\n", (CARD32) monitors->monitors[m].outputs[o]);
		}
		printf ("\n");
	    }
	}
    }

    sc = XRRGetScreenInfo (dpy, root);

    if (sc == NULL) 
	exit (1);

    current_size = XRRConfigCurrentConfiguration (sc, &current_rotation);

    sizes = XRRConfigSizes(sc, &nsize);

    if (have_pixel_size) {
	for (size = 0; size < nsize; size++)
	{
	    if (sizes[size].width == width && sizes[size].height == height)
		break;
	}
	if (size >= nsize) {
	    fprintf (stderr,
		     "Size %dx%d not found in available modes\n", width, height);
	    exit (1);
	}
    }
    else if (size < 0)
	size = current_size;
    else if (size >= nsize) {
	fprintf (stderr,
		 "Size index %d is too large, there are only %d sizes\n",
		 size, nsize);
	exit (1);
    }

    if (rot < 0)
    {
	for (rot = 0; rot < 4; rot++)
	    if (1 << rot == (current_rotation & 0xf))
		break;
    }

    current_rate = XRRConfigCurrentRate (sc);

    if (rate < 0)
    {
	if (size == current_size)
	    rate = current_rate;
	else
	    rate = 0;
    }
    else
    {
	rates = XRRConfigRates (sc, size, &nrate);
	for (i = 0; i < nrate; i++)
	    if (rate == rates[i])
		break;
	if (i == nrate) {
	    fprintf (stderr, "Rate %.2f Hz not available for this size\n", rate);
	    exit (1);
	}
    }

    if (version) {
	int major_version, minor_version;
	XRRQueryVersion (dpy, &major_version, &minor_version);
	printf("Server reports RandR version %d.%d\n", 
	       major_version, minor_version);
    }

    if (query || query_1) {
	printf(" SZ:    Pixels          Physical       Refresh\n");
	for (i = 0; i < nsize; i++) {
	    int j;

	    printf ("%c%-2d %5d x %-5d  (%4dmm x%4dmm )",
		    i == current_size ? '*' : ' ',
		    i, sizes[i].width, sizes[i].height,
		    sizes[i].mwidth, sizes[i].mheight);
	    rates = XRRConfigRates (sc, i, &nrate);
	    if (nrate) printf ("  ");
	    for (j = 0; j < nrate; j++)
		printf ("%c%-4d",
			i == current_size && rates[j] == current_rate ? '*' : ' ',
			rates[j]);
	    printf ("\n");
	}
    }

    {
	Rotation rotations = XRRConfigRotations(sc, &current_rotation);

	if (toggle_x && !(current_rotation & RR_Reflect_X)) reflection |= RR_Reflect_X;
	if (toggle_y && !(current_rotation & RR_Reflect_Y)) reflection |= RR_Reflect_Y;

	if (query) {
	    printf("Current rotation - %s\n",
		   rotation_name (current_rotation));

	    printf("Current reflection - %s\n",
		   reflection_name (current_rotation));

	    printf ("Rotations possible - ");
	    for (i = 0; i < 4; i ++) {
		if ((rotations >> i) & 1)  printf("%s ", direction[i]);
	    }
	    printf ("\n");

	    printf ("Reflections possible - ");
	    if (rotations & (RR_Reflect_X|RR_Reflect_Y))
	    {
		if (rotations & RR_Reflect_X) printf ("X Axis ");
		if (rotations & RR_Reflect_Y) printf ("Y Axis");
	    }
	    else
		printf ("none");
	    printf ("\n");
	}
    }

    if (verbose) { 
	printf("Setting size to %d, rotation to %s\n",  size, direction[rot]);

	printf ("Setting reflection on ");
	if (reflection)
	{
	    if (reflection & RR_Reflect_X) printf ("X Axis ");
	    if (reflection & RR_Reflect_Y) printf ("Y Axis");
	}
	else
	    printf ("neither axis");
	printf ("\n");
    }

    /* we should test configureNotify on the root window */
    XSelectInput (dpy, root, StructureNotifyMask);

    if (setit && !dryrun) XRRSelectInput (dpy, root,
			       RRScreenChangeNotifyMask);
    if (setit && !dryrun) {
	Rotation rotation = 1 << rot;
	status = XRRSetScreenConfigAndRate (dpy, sc, root, (SizeID) size,
					    (Rotation) (rotation | reflection),
					    rate, CurrentTime);
    }

    if (setit && !dryrun && status == RRSetConfigFailed) {
	printf ("Failed to change the screen configuration!\n");
	ret = 1;
    }

    if (verbose && setit && !dryrun && size != current_size) {
	if (status == RRSetConfigSuccess)
	{
	    Bool    seen_screen = False;
	    while (!seen_screen) {
		int spo;
		XNextEvent(dpy, (XEvent *) &event);

		printf ("Event received, type = %d\n", event.type);
		/* update Xlib's knowledge of the event */
		XRRUpdateConfiguration (&event);
		if (event.type == ConfigureNotify)
		    printf("Received ConfigureNotify Event!\n");

		switch (event.type - event_base) {
		case RRScreenChangeNotify:
		    sce = (XRRScreenChangeNotifyEvent *) &event;

		    printf("Got a screen change notify event!\n");
		    printf(" window = %d\n root = %d\n size_index = %d\n rotation %d\n", 
			   (int) sce->window, (int) sce->root, 
			   sce->size_index,  sce->rotation);
		    printf(" timestamp = %ld, config_timestamp = %ld\n",
			   sce->timestamp, sce->config_timestamp);
		    printf(" Rotation = %x\n", sce->rotation);
		    printf(" %d X %d pixels, %d X %d mm\n",
			   sce->width, sce->height, sce->mwidth, sce->mheight);
		    printf("Display width   %d, height   %d\n",
			   DisplayWidth(dpy, screen), DisplayHeight(dpy, screen));
		    printf("Display widthmm %d, heightmm %d\n", 
			   DisplayWidthMM(dpy, screen), DisplayHeightMM(dpy, screen));
		    spo = sce->subpixel_order;
		    if ((spo < 0) || (spo > 5))
			printf ("Unknown subpixel order, value = %d\n", spo);
		    else printf ("new Subpixel rendering model is %s\n", order[spo]);
		    seen_screen = True;
		    break;
		default:
		    if (event.type != ConfigureNotify) 
			printf("unknown event received, type = %d!\n", event.type);
		}
	    }
	}
    }
    XRRFreeScreenConfigInfo(sc);
    return(ret);
}
