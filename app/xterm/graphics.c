/* $XTermId: graphics.c,v 1.135 2024/05/11 09:49:08 tom Exp $ */

/*
 * Copyright 2013-2023,2024 by Thomas E. Dickey
 * Copyright 2013-2022,2023 by Ross Combs
 *
 *                         All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

#include <xterm.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <data.h>
#include <ptyx.h>

#include <assert.h>
#include <graphics.h>

#define OPT_INHERIT_COLORS 0

#if OPT_REGIS_GRAPHICS
#include <graphics_regis.h>
#endif

#undef DUMP_BITMAP
#undef DUMP_COLORS
#undef DEBUG_PALETTE
#undef DEBUG_PIXEL
#undef DEBUG_REFRESH

/*
 * graphics TODO list
 *
 * ReGIS:
 * - ship a default alphabet zero font instead of scaling Xft fonts
 * - input cursors
 * - output cursors
 * - mouse/tablet/arrow-key input
 * - fix graphic pages for ReGIS -- they should also apply to text and sixel graphics
 * - fix interpolated curves to more closely match implementation (identical despite direction and starting point)
 * - non-ASCII alphabets
 * - enter/leave anywhere in a command
 * - locator key definitions (DECLKD)
 * - command display mode
 * - re-rasterization on window resize
 * - macros
 * - improved fills for narrow angles (track actual lines not just pixels)
 * - hardcopy/screen-capture support (need dialog of some sort for safety)
 * - error reporting
 *
 * sixel:
 * - fix problem where new_row < 0 during sixel parsing (see FIXME)
 * - screen-capture support (need dialog of some sort for safety)
 *
 * VT55/VT105 waveform graphics
 * - everything
 *
 * Tektronix:
 * - color (VT340 4014 emulation, 41xx, IRAF GTERM, and also MS-DOS Kermit color support)
 * - polygon fill (41xx)
 * - clear area extension
 * - area fill extension
 * - pixel operations (RU/RS/RP)
 * - research other 41xx and 42xx extensions
 *
 * common graphics features:
 * - handle light/dark screen modes (CSI?5[hl])
 * - update text fg/bg color which overlaps images
 * - handle graphic updates in scroll regions (verify effect on graphics)
 * - handle rectangular area copies (verify they work with graphics)
 * - invalidate graphics under graphic if same origin, at least as big, and bg not transparent
 * - invalidate graphic if completely scrolled past end of scrollback
 * - invalidate graphic if all pixels are transparent/erased
 * - invalidate graphic if completely scrolled out of alt buffer
 * - posturize requested colors to match hardware palettes (e.g. only four possible shades on VT240)
 * - color register report/restore
 * - ability to select/copy graphics for pasting in other programs
 * - ability to show non-scroll-mode sixel graphics in a separate window
 * - ability to show ReGIS graphics in a separate window
 * - ability to show Tektronix graphics in VT100 window
 * - truncate graphics at bottom edge of terminal?
 * - locator events (DECEFR DECSLE DECELR DECLRP)
 * - locator controller mode (CSI6i / CSI7i)
 *
 * new escape sequences:
 * - way to query text font size without "window ops" (or make "window ops" permissions more fine grained)
 * - way to query and set the number of graphics pages
 *
 * ReGIS extensions:
 * - non-integer text scaling
 * - free distortionless text rotation (vs. simulating the distortion and aligning to 45deg increments)
 * - font characteristics: bold/underline/italic
 * - remove/increase arbitrary limits (pattern size, pages, alphabets, stack size, font names, etc.)
 * - shade/fill with borders
 * - sprites (copy portion of page into/out of buffer with scaling and rotation)
 * - ellipses
 * - 2D patterns
 * - option to set actual graphic size (not just coordinate range)
 * - gradients (for lines and fills)
 * - line width (RLogin has this and it is mentioned in docs for the DEC ReGIS to Postscript converter)
 * - transparency
 * - background color as stackable write control
 * - true color (virtual color registers created upon lookup)
 * - anti-aliasing
 * - variable-width (proportional) text
 */

/* font sizes:
 * VT510:
 *   80 Columns 132 Columns Maximum Number of Lines
 *   10 x 16   6 x 16  26 lines + keyboard indicator line
 *   10 x 13   6 x 13  26 lines + keyboard indicator line
 *   10 x 10   6 x 10  42 lines + keyboard indicator line
 *   10 x 8    6 x 8   53 lines + keyboard indicator line
 */

typedef struct allocated_color_register {
    struct allocated_color_register *next;
    Pixel pix;
    short r, g, b;
} AllocatedColorRegister;

#define LOOKUP_WIDTH 16
static AllocatedColorRegister *allocated_colors[LOOKUP_WIDTH][LOOKUP_WIDTH][LOOKUP_WIDTH];

#define FOR_EACH_SLOT(ii) for (ii = 0U; ii < MAX_GRAPHICS; ii++)

static ColorRegister *shared_color_registers;
static Graphic *displayed_graphics[MAX_GRAPHICS];
static unsigned next_graphic_id = 0U;
static unsigned used_graphics;	/* 0 to MAX_GRAPHICS */

static int valid_graphics;
static GC graphics_gc;
static XGCValues xgcv;
static ColorRegister last_color;
static ColorRegister gc_color;

#define DiffColor(this,that) \
	(this.r != that.r || \
	 this.g != that.g || \
	 this.b != that.b)

static ColorRegister null_color =
{-1, -1, -1};

static ColorRegister *
allocRegisters(void)
{
    return TypeCallocN(ColorRegister, MAX_COLOR_REGISTERS);
}

static Graphic *
freeGraphic(Graphic *obj)
{
    if (obj) {
	free(obj->pixels);
	free(obj->private_color_registers);
	free(obj);
    }
    return NULL;
}

static Graphic *
allocGraphic(int max_w, int max_h)
{
    Graphic *result = TypeCalloc(Graphic);
    if (result) {
	result->max_width = max_w;
	result->max_height = max_h;
	if (!(result->pixels = TypeCallocN(RegisterNum,
					     (size_t) max_w * (size_t) max_h))) {
	    result = freeGraphic(result);
	} else if (!(result->private_color_registers = allocRegisters())) {
	    result = freeGraphic(result);
	}
    }
    return result;
}

#define getActiveSlot(n) \
	(((n) < MAX_GRAPHICS && \
	 displayed_graphics[n] && \
	 displayed_graphics[n]->valid) \
	 ? displayed_graphics[n] \
	 : NULL)

static Graphic *
getInactiveSlot(const TScreen *screen, unsigned n)
{
    if (n < MAX_GRAPHICS &&
	(!displayed_graphics[n] ||
	 !displayed_graphics[n]->valid)) {
	if (!displayed_graphics[n]) {
	    displayed_graphics[n] = allocGraphic(screen->graphics_max_wide,
						 screen->graphics_max_high);
	    used_graphics += (displayed_graphics[n] != NULL);
	}
	return displayed_graphics[n];
    }
    return NULL;
}

static ColorRegister *
getSharedRegisters(void)
{
    if (!shared_color_registers)
	shared_color_registers = allocRegisters();
    return shared_color_registers;
}

static void
deactivateSlot(unsigned n)
{
    if ((n < MAX_GRAPHICS) && displayed_graphics[n]) {
	displayed_graphics[n] = freeGraphic(displayed_graphics[n]);
	used_graphics--;
    }
}

extern RegisterNum
read_pixel(Graphic *graphic, int x, int y)
{
    return (((x) >= 0 &&
	     (x) < (graphic)->actual_width &&
	     (y) >= 0 &&
	     (y) < (graphic)->actual_height)
	    ? (graphic)->pixels[(y) * (graphic)->max_width + (x)]
	    : (RegisterNum) COLOR_HOLE);
}

#define _draw_pixel(G, X, Y, C) \
    do { \
        unsigned _cell = (unsigned)((Y) * (G)->max_width + (X)); \
        SetSpixel(G, _cell, (RegisterNum) (C)); \
    } while (0)

void
draw_solid_pixel(Graphic *graphic, int x, int y, unsigned color)
{
    assert(color <= MAX_COLOR_REGISTERS);

#ifdef DEBUG_PIXEL
    TRACE(("drawing pixel at %d,%d color=%hu (hole=%hu, [%d,%d,%d])\n",
	   x,
	   y,
	   color,
	   COLOR_HOLE,
	   ((color != COLOR_HOLE)
	    ? (unsigned) graphic->color_registers[color].r : 0U),
	   ((color != COLOR_HOLE)
	    ? (unsigned) graphic->color_registers[color].g : 0U),
	   ((color != COLOR_HOLE)
	    ? (unsigned) graphic->color_registers[color].b : 0U)));
#endif
    if (x >= 0 && x < graphic->actual_width &&
	y >= 0 && y < graphic->actual_height) {
	_draw_pixel(graphic, x, y, color);
	if (color < MAX_COLOR_REGISTERS)
	    graphic->color_registers_used[color] = True;
    }
}

void
draw_solid_rectangle(Graphic *graphic, int x1, int y1, int x2, int y2, unsigned color)
{
    int x, y;
    int tmp;

    assert(color <= MAX_COLOR_REGISTERS);

    if (x1 > x2) {
	EXCHANGE(x1, x2, tmp);
    }
    if (y1 > y2) {
	EXCHANGE(y1, y2, tmp);
    }

    if (x2 < 0 || x1 >= graphic->actual_width ||
	y2 < 0 || y1 >= graphic->actual_height)
	return;

    if (x1 < 0)
	x1 = 0;
    if (x2 >= graphic->actual_width)
	x2 = graphic->actual_width - 1;
    if (y1 < 0)
	y1 = 0;
    if (y2 >= graphic->actual_height)
	y2 = graphic->actual_height - 1;

    if (color < MAX_COLOR_REGISTERS)
	graphic->color_registers_used[color] = True;
    for (y = y1; y <= y2; y++)
	for (x = x1; x <= x2; x++)
	    _draw_pixel(graphic, x, y, color);
}

void
copy_overlapping_area(Graphic *graphic, int src_ul_x, int src_ul_y,
		      int dst_ul_x, int dst_ul_y, unsigned w, unsigned h,
		      unsigned default_color)
{
    int sx, ex, dx;
    int sy, ey, dy;
    int xx, yy;
    RegisterNum color;

    if (dst_ul_x <= src_ul_x) {
	sx = 0;
	ex = (int) w - 1;
	dx = +1;
    } else {
	sx = (int) w - 1;
	ex = 0;
	dx = -1;
    }

    if (dst_ul_y <= src_ul_y) {
	sy = 0;
	ey = (int) h - 1;
	dy = +1;
    } else {
	sy = (int) h - 1;
	ey = 0;
	dy = -1;
    }

    for (yy = sy; yy != ey + dy; yy += dy) {
	int dst_y = dst_ul_y + yy;
	int src_y = src_ul_y + yy;
	if (dst_y < 0 || dst_y >= (int) graphic->actual_height)
	    continue;

	for (xx = sx; xx != ex + dx; xx += dx) {
	    int dst_x = dst_ul_x + xx;
	    int src_x = src_ul_x + xx;
	    int cell;
	    if (dst_x < 0 || dst_x >= (int) graphic->actual_width)
		continue;

	    if (src_x < 0 || src_x >= (int) graphic->actual_width ||
		src_y < 0 || src_y >= (int) graphic->actual_height)
		color = (RegisterNum) default_color;
	    else
		color = graphic->pixels[(unsigned) (src_y *
						    graphic->max_width) +
					(unsigned) src_x];

	    cell = (int) ((unsigned) (dst_y * graphic->max_width) +
			  (unsigned) dst_x);
	    SetSpixel(graphic, cell, color);
	}
    }
}

#define set_color_register(color_registers, color, pr, pg, pb) \
do { \
    assert(color <= MAX_COLOR_REGISTERS); \
    { \
    ColorRegister *reg = &color_registers[color]; \
    reg->r = (short) pr; \
    reg->g = (short) pg; \
    reg->b = (short) pb; \
    } \
} while (0)

/* Graphics which don't use private colors will act as if they are using a
 * device-wide color palette.
 */
static void
set_shared_color_register(unsigned color, int r, int g, int b)
{
    unsigned ii;

    assert(color < MAX_COLOR_REGISTERS);

    set_color_register(getSharedRegisters(), color, r, g, b);

    if (!used_graphics)
	return;

    FOR_EACH_SLOT(ii) {
	Graphic *graphic;

	if (!(graphic = getActiveSlot(ii)))
	    continue;
	if (graphic->private_colors)
	    continue;

	if (graphic->color_registers_used[ii]) {
	    graphic->dirty = True;
	}
    }
}

void
fetch_color_register(Graphic *graphic,
		     unsigned color,
		     ColorRegister *reg)
{
    assert(color < MAX_COLOR_REGISTERS);

    reg->r = -1;
    reg->g = -1;
    reg->b = -1;

    if (graphic->color_registers_used[color]) {
	if (graphic->private_colors) {
	    *reg = graphic->private_color_registers[color];
	} else {
	    *reg = getSharedRegisters()[color];
	}
    }
}

void
update_color_register(Graphic *graphic,
		      unsigned color,
		      int r,
		      int g,
		      int b)
{
    assert(color < MAX_COLOR_REGISTERS);

    if (graphic->private_colors) {
	set_color_register(graphic->private_color_registers,
			   color, r, g, b);
	if (graphic->color_registers_used[color]) {
	    graphic->dirty = True;
	}
	graphic->color_registers_used[color] = True;
    } else {
	set_shared_color_register(color, r, g, b);
    }
}

#define SQUARE(X) ( (X) * (X) )

RegisterNum
find_color_register(ColorRegister const *color_registers, int r, int g, int b)
{
    unsigned i;
    unsigned closest_index;
    unsigned closest_distance;

    /* I have no idea what algorithm DEC used for this.
     * The documentation warns that it is unpredictable, especially with values
     * far away from any allocated color so it is probably a very simple
     * heuristic rather than something fancy like finding the minimum distance
     * in a linear perceptive color space.
     */
    closest_index = MAX_COLOR_REGISTERS;
    closest_distance = 0U;
    for (i = 0U; i < MAX_COLOR_REGISTERS; i++) {
	unsigned d = (unsigned) (SQUARE(2 * (color_registers[i].r - r)) +
				 SQUARE(3 * (color_registers[i].g - g)) +
				 SQUARE(1 * (color_registers[i].b - b)));
	if (closest_index == MAX_COLOR_REGISTERS || d < closest_distance) {
	    closest_index = i;
	    closest_distance = d;
	}
    }

    TRACE(("found closest color register to %d,%d,%d: %u (distance %u value %d,%d,%d)\n",
	   r, g, b,
	   closest_index,
	   closest_distance,
	   color_registers[closest_index].r,
	   color_registers[closest_index].g,
	   color_registers[closest_index].b));
    return (RegisterNum) closest_index;
}

#if OPT_INHERIT_COLORS
static void
copy_color_registers(Graphic *target, Graphic *source)
{
    memcpy(target->color_registers_used,
	   source->color_registers_used,
	   sizeof(Boolean) * MAX_COLOR_REGISTERS);

    memcpy(target->private_color_registers,
	   source->color_registers,
	   sizeof(ColorRegister) * MAX_COLOR_REGISTERS);
}
#endif

static void
init_color_registers(TScreen const *screen, ColorRegister *color_registers)
{
    const int graphics_termid = GraphicsTermId(screen);

    TRACE(("setting initial colors for terminal %d\n", graphics_termid));
    memset(color_registers,
	   0,
	   sizeof(ColorRegister) * MAX_COLOR_REGISTERS);

    /*
     * default color registers:
     *     (mono) (color)
     * VK100/GIGI (fixed)
     * VT125:
     *   0: 0%      0%
     *   1: 33%     blue
     *   2: 66%     red
     *   3: 100%    green
     * VT240:
     *   0: 0%      0%
     *   1: 33%     blue
     *   2: 66%     red
     *   3: 100%    green
     * VT241:
     *   0: 0%      0%
     *   1: 33%     blue
     *   2: 66%     red
     *   3: 100%    green
     * VT330:
     *   0: 0%      0%              (bg for light on dark mode)
     *   1: 33%     blue (red?)
     *   2: 66%     red (green?)
     *   3: 100%    green (yellow?) (fg for light on dark mode)
     * VT340:
     *   0: 0%      0%              (bg for light on dark mode)
     *   1: 14%     blue
     *   2: 29%     red
     *   3: 43%     green
     *   4: 57%     magenta
     *   5: 71%     cyan
     *   6: 86%     yellow
     *   7: 100%    50%             (fg for light on dark mode)
     *   8: 0%      25%
     *   9: 14%     gray-blue
     *  10: 29%     gray-red
     *  11: 43%     gray-green
     *  12: 57%     gray-magenta
     *  13: 71%     gray-cyan
     *  14: 86%     gray-yellow
     *  15: 100%    75%             ("white")
     * VT382:
     *   ? (FIXME: B&W only?)
     * dxterm:
     *  ?
     */
    switch (graphics_termid) {
    case 125:
    case 241:
	set_color_register(color_registers, 0, 0, 0, 0);
	set_color_register(color_registers, 1, 0, 0, 100);
	set_color_register(color_registers, 2, 0, 100, 0);
	set_color_register(color_registers, 3, 100, 0, 0);
	break;
    case 240:
    case 330:
	set_color_register(color_registers, 0, 0, 0, 0);
	set_color_register(color_registers, 1, 33, 33, 33);
	set_color_register(color_registers, 2, 66, 66, 66);
	set_color_register(color_registers, 3, 100, 100, 100);
	break;
    case 340:
    default:
	set_color_register(color_registers, 0, 0, 0, 0);
	set_color_register(color_registers, 1, 20, 20, 80);
	set_color_register(color_registers, 2, 80, 13, 13);
	set_color_register(color_registers, 3, 20, 80, 20);
	set_color_register(color_registers, 4, 80, 20, 80);
	set_color_register(color_registers, 5, 20, 80, 80);
	set_color_register(color_registers, 6, 80, 80, 20);
	set_color_register(color_registers, 7, 53, 53, 53);
	set_color_register(color_registers, 8, 26, 26, 26);
	set_color_register(color_registers, 9, 33, 33, 60);
	set_color_register(color_registers, 10, 60, 26, 26);
	set_color_register(color_registers, 11, 33, 60, 33);
	set_color_register(color_registers, 12, 60, 33, 60);
	set_color_register(color_registers, 13, 33, 60, 60);
	set_color_register(color_registers, 14, 60, 60, 33);
	set_color_register(color_registers, 15, 80, 80, 80);
	break;
    case 382:			/* FIXME: verify */
	set_color_register(color_registers, 0, 0, 0, 0);
	set_color_register(color_registers, 1, 100, 100, 100);
	break;
    }

#ifdef DEBUG_PALETTE
    {
	unsigned i;

	for (i = 0U; i < MAX_COLOR_REGISTERS; i++) {
	    TRACE(("initial value for register %03u: %d,%d,%d\n",
		   i,
		   color_registers[i].r,
		   color_registers[i].g,
		   color_registers[i].b));
	}
    }
#endif
}

unsigned
get_color_register_count(TScreen const *screen)
{
    unsigned num_color_registers;

    if (screen->numcolorregisters >= 0) {
	num_color_registers = (unsigned) screen->numcolorregisters;
    } else {
	num_color_registers = 0U;
    }

    if (num_color_registers > 1U) {
	if (num_color_registers > MAX_COLOR_REGISTERS)
	    return MAX_COLOR_REGISTERS;
	return num_color_registers;
    }

    /*
     * color capabilities:
     * VK100/GIGI  1 plane (12x1 pixel attribute blocks) colorspace is 8 fixed colors (black, white, red, green, blue, cyan, yellow, magenta)
     * VT125       2 planes (4 registers) colorspace is (64?) (color), ? (grayscale)
     * VT240       2 planes (4 registers) colorspace is 4 shades (grayscale)
     * VT241       2 planes (4 registers) colorspace is ? (color), ? shades (grayscale)
     * VT330       2 planes (4 registers) colorspace is 4 shades (grayscale)
     * VT340       4 planes (16 registers) colorspace is r16g16b16 (color), 16 shades (grayscale)
     * VT382       1 plane (two fixed colors: black and white)  FIXME: verify
     * dxterm      ?
     */
    switch (screen->graphics_termid) {
    case 125:
	return 4U;
    case 240:
	return 4U;
    case 241:
	return 4U;
    case 330:
	return 4U;
    case 340:
	return 16U;
    case 382:
	return 2U;
    default:
	/* unknown graphics model -- might as well be generous */
	return MAX_COLOR_REGISTERS;
    }
}

static void
init_graphic(TScreen *screen,
	     Graphic *graphic,
	     unsigned type,
	     int charrow,
	     int charcol,
	     unsigned num_color_registers)
{
    int private_colors = screen->privatecolorregisters;
    const unsigned max_pixels = (unsigned) (graphic->max_width *
					    graphic->max_height);

    TRACE(("init_graphic %u pixels at %d,%d\n", max_pixels, charrow, charcol));

    graphic->hidden = False;
    graphic->dirty = True;
    memset(graphic->pixels, COLOR_HOLE & 0xff, max_pixels * sizeof(RegisterNum));
    memset(graphic->color_registers_used, False, sizeof(graphic->color_registers_used));

    /*
     * text and graphics interactions:
     * VK100/GIGI                text writes on top of graphics buffer, color attribute shared with text
     * VT240,VT241,VT330,VT340   text writes on top of graphics buffer
     * VT382                     text writes on top of graphics buffer FIXME: verify
     * VT125                     graphics buffer overlaid on top of text in B&W display, text not present in color display
     */

    /*
     * dimensions (ReGIS logical, physical):
     * VK100/GIGI  768x4??  768x240(status?)
     * VT125       768x460  768x230(+10status) (1:2 aspect ratio, ReGIS halves vertical addresses through "odd y emulation")
     * VT240       800x460  800x230(+10status) (1:2 aspect ratio, ReGIS halves vertical addresses through "odd y emulation")
     * VT241       800x460  800x230(+10status) (1:2 aspect ratio, ReGIS halves vertical addresses through "odd y emulation")
     * VT330       800x480  800x480(+?status)
     * VT340       800x480  800x480(+?status)
     * VT382       960x750  sixel only
     * dxterm      ?x? ?x?  variable?
     */

    graphic->actual_width = 0;
    graphic->actual_height = 0;

    graphic->pixw = 1;
    graphic->pixh = 1;

    graphic->valid_registers = num_color_registers;
    TRACE(("%d color registers\n", graphic->valid_registers));

    graphic->private_colors = private_colors;
    if (graphic->private_colors) {
#if OPT_INHERIT_COLORS
	unsigned ii;
	int max_charrow = -1;
	Graphic *newest = NULL;
#endif

	TRACE(("using private color registers\n"));

#if OPT_INHERIT_COLORS
	FOR_EACH_SLOT(ii) {
	    Graphic *check;
	    if (!(check = getActiveSlot(ii)))
		continue;
	    if (!newest || check->charrow >= max_charrow) {
		max_charrow = check->charrow;
		newest = check;
	    }
	}

	if (newest != NULL && newest != graphic) {
	    copy_color_registers(graphic, newest);
	} else {
	    init_color_registers(screen, graphic->private_color_registers);
	}
#else
	init_color_registers(screen, graphic->private_color_registers);
#endif
	graphic->color_registers = graphic->private_color_registers;
    } else {
	TRACE(("using shared color registers\n"));
	graphic->color_registers = getSharedRegisters();
    }

    graphic->charrow = charrow;
    graphic->charcol = charcol;
    graphic->type = type;
    graphic->valid = False;
}

Graphic *
get_new_graphic(XtermWidget xw, int charrow, int charcol, unsigned type)
{
    TScreen *screen = TScreenOf(xw);
    const int bufferid = screen->whichBuf;
    Graphic *graphic = NULL;
    unsigned ii;

    TRACE(("get_new_graphic %d,%d type %d\n", charrow, charcol, type));

    FOR_EACH_SLOT(ii) {
	if ((graphic = getInactiveSlot(screen, ii))) {
	    TRACE(("using fresh graphic index %u as id %u\n",
		   ii, next_graphic_id));
	    break;
	}
    }

    /* if none are free, recycle the graphic scrolled back the farthest */
    if (!graphic) {
	int min_charrow = 0;
	Graphic *min_graphic = NULL;
	if_TRACE(unsigned best_ii = (1 + MAX_GRAPHICS));

	FOR_EACH_SLOT(ii) {
	    if (!(graphic = getActiveSlot(ii)))
		continue;
	    if (!min_graphic || graphic->charrow < min_charrow) {
		if_TRACE(best_ii = ii);
		min_charrow = graphic->charrow;
		min_graphic = graphic;
	    }
	}
	TRACE(("recycling old graphic index %u as id %u\n",
	       best_ii, next_graphic_id));
	graphic = min_graphic;
    }

    if (graphic) {
	unsigned num_color_registers;
	num_color_registers = get_color_register_count(screen);
	graphic->xw = xw;
	graphic->bufferid = bufferid;
	graphic->id = next_graphic_id++;
	init_graphic(screen,
		     graphic,
		     type,
		     charrow,
		     charcol,
		     num_color_registers);
    }
    return graphic;
}

Graphic *
get_new_or_matching_graphic(XtermWidget xw,
			    int charrow,
			    int charcol,
			    int actual_width,
			    int actual_height,
			    unsigned type)
{
    TScreen const *screen = TScreenOf(xw);
    const int bufferid = screen->whichBuf;
    Graphic *graphic;
    unsigned ii;

    FOR_EACH_SLOT(ii) {
	TRACE(("checking slot=%u for graphic at %d,%d %dx%d bufferid=%d type=%u\n", ii,
	       charrow, charcol,
	       actual_width, actual_height,
	       bufferid, type));
	if ((graphic = getActiveSlot(ii))) {
	    if (graphic->type == type &&
		graphic->bufferid == bufferid &&
		graphic->charrow == charrow &&
		graphic->charcol == charcol &&
		graphic->actual_width == actual_width &&
		graphic->actual_height == actual_height) {
		TRACE(("found existing graphic slot=%u id=%u\n", ii, graphic->id));
		return graphic;
	    }
	    TRACE(("not a match: graphic at %d,%d %dx%d bufferid=%d type=%u\n",
		   graphic->charrow, graphic->charcol,
		   graphic->actual_width, graphic->actual_height,
		   graphic->bufferid, graphic->type));
	}
    }

    /* if no match get a new graphic */
    if ((graphic = get_new_graphic(xw, charrow, charcol, type))) {
	graphic->actual_width = actual_width;
	graphic->actual_height = actual_height;
	TRACE(("no match; created graphic at %d,%d %dx%d bufferid=%d type=%u\n",
	       graphic->charrow, graphic->charcol,
	       graphic->actual_width, graphic->actual_height,
	       graphic->bufferid, graphic->type));
    }
    return graphic;
}

#define ScaleForXColor(s) (unsigned short) ((unsigned long)(s) * MAX_U_COLOR / CHANNEL_MAX)

static int
save_allocated_color(const ColorRegister *reg, XtermWidget xw, Pixel *pix)
{
    unsigned const rr = ((unsigned) reg->r * (LOOKUP_WIDTH - 1)) / CHANNEL_MAX;
    unsigned const gg = ((unsigned) reg->g * (LOOKUP_WIDTH - 1)) / CHANNEL_MAX;
    unsigned const bb = ((unsigned) reg->b * (LOOKUP_WIDTH - 1)) / CHANNEL_MAX;
    XColor xcolor;
    AllocatedColorRegister *new_color;

    /* *INDENT-EQLS* */
    xcolor.pixel = 0UL;
    xcolor.red   = ScaleForXColor(reg->r);
    xcolor.green = ScaleForXColor(reg->g);
    xcolor.blue  = ScaleForXColor(reg->b);
    xcolor.flags = DoRed | DoGreen | DoBlue;

    if (!allocateBestRGB(xw, &xcolor)) {
	TRACE(("unable to allocate xcolor\n"));
	*pix = 0UL;
	return 0;
    } else {
	*pix = xcolor.pixel;

	if (!(new_color = TypeMalloc(AllocatedColorRegister))) {
	    TRACE(("unable to save pixel %lu\n", (unsigned long) *pix));
	    return 0;
	} else {
	    new_color->r = reg->r;
	    new_color->g = reg->g;
	    new_color->b = reg->b;
	    new_color->pix = *pix;
	    new_color->next = allocated_colors[rr][gg][bb];

	    allocated_colors[rr][gg][bb] = new_color;

	    return 1;
	}
    }
}

/* FIXME: with so many possible colors we need to determine
 * when to free them to be nice to PseudoColor displays
 */
static Pixel
color_register_to_xpixel(const ColorRegister *reg, XtermWidget xw)
{
    Pixel result;
    unsigned const rr = ((unsigned) reg->r * (LOOKUP_WIDTH - 1)) / CHANNEL_MAX;
    unsigned const gg = ((unsigned) reg->g * (LOOKUP_WIDTH - 1)) / CHANNEL_MAX;
    unsigned const bb = ((unsigned) reg->b * (LOOKUP_WIDTH - 1)) / CHANNEL_MAX;
    const AllocatedColorRegister *search;

    for (search = allocated_colors[rr][gg][bb]; search; search = search->next) {
	if (search->r == reg->r &&
	    search->g == reg->g &&
	    search->b == reg->b) {
	    return search->pix;
	}
    }

    save_allocated_color(reg, xw, &result);
    return result;
}

static void
refresh_graphic(TScreen const *screen,
		Graphic const *graphic,
		ColorRegister *buffer,
		int refresh_x,
		int refresh_y,
		int refresh_w,
		int refresh_h,
		int draw_x,
		int draw_y,
		int draw_w,
		int draw_h)
{
    int const pw = graphic->pixw;
    int const ph = graphic->pixh;
    int const graph_x = graphic->charcol * FontWidth(screen);
    int const graph_y = graphic->charrow * FontHeight(screen);
    int const graph_w = graphic->actual_width;
    int const graph_h = graphic->actual_height;
    int const mw = graphic->max_width;

    int r, c;
    int pmy;
    RegisterNum regnum;

    if_TRACE(int holes = 0);
    if_TRACE(int total = 0);
    if_TRACE(int out_of_range = 0);

    TRACE(("refreshing graphic %u from %d,%d %dx%d (valid=%d, size=%dx%d, scale=%dx%d max=%dx%d)\n",
	   graphic->id,
	   graph_x, graph_y, draw_w, draw_h,
	   graphic->valid,
	   graphic->actual_width,
	   graphic->actual_height,
	   pw, ph,
	   graphic->max_width,
	   graphic->max_height));

    TRACE(("refresh pixmap starts at %d,%d\n", refresh_x, refresh_y));

    for (r = 0, pmy = graph_y; r < graph_h; r++, pmy += ph) {
	int pmx, buffer_y, pixel_y;

	if (pmy + ph - 1 < draw_y)
	    continue;
	if (pmy > draw_y + draw_h - 1)
	    break;

	if (pmy < draw_y ||
	    pmy < refresh_y ||
	    pmy > refresh_y + refresh_h - 1) {
	    if_TRACE(out_of_range++);
	    continue;
	}
	pixel_y = r * mw;
	buffer_y = (pmy - refresh_y) * refresh_w;

	for (c = 0, pmx = graph_x; c < graph_w; c++, pmx += pw) {

	    if (pmx + pw - 1 < draw_x)
		continue;
	    if (pmx > draw_x + draw_w - 1)
		break;

	    if (pmx < draw_x ||
		pmx < refresh_x ||
		pmx > refresh_x + refresh_w - 1) {
		if_TRACE(out_of_range++);
		continue;
	    }

	    if_TRACE(total++);
	    regnum = graphic->pixels[pixel_y + c];
	    if (regnum == COLOR_HOLE) {
		if_TRACE(holes++);
	    } else {
		buffer[buffer_y + (pmx - refresh_x)] =
		    graphic->color_registers[regnum];
	    }
	}
    }

    TRACE(("done refreshing graphic: %d of %d refreshed pixels were holes; %d were out of pixmap range\n",
	   holes, total, out_of_range));
}

#define MAX_PCT 100.		/* HLS uses this for L, S percentages */
#define MAX_RGB 100.		/* use this rather than 255 */

/*
 * In HLS, H is an angle, in degrees, and L, S are percentages.
 * Primary color hues:
 *  blue:    0 degrees
 *  red:   120 degrees
 *  green: 240 degrees
 */
void
hls2rgb(int h, int l, int s, short *r, short *g, short *b)
{
    int hs;
    const double lv = l / MAX_PCT;
    const double sv = s / MAX_PCT;
    double c, x, m, c2;
    double r1, g1, b1;

    h = h - 120;		/* Rotate so that blue is at 0 degrees  */
    while (h < 0)
	h += 360;		/* Normalize to 0 to 360, */
    while (h >= 360)
	h -= 360;
    hs = ((h + 59) / 60) % 6;

    if (s == 0) {
	*r = *g = *b = (short) l;
	return;
    }

    c2 = (2.0 * lv) - 1.0;
    if (c2 < 0.0)
	c2 = -c2;
    c = (1.0 - c2) * sv;
    x = (hs & 1) ? c : 0.0;
    m = lv - 0.5 * c;

    switch (hs) {
    case 0:
	r1 = c;
	g1 = x;
	b1 = 0.0;
	break;
    case 1:
	r1 = x;
	g1 = c;
	b1 = 0.0;
	break;
    case 2:
	r1 = 0.0;
	g1 = c;
	b1 = x;
	break;
    case 3:
	r1 = 0.0;
	g1 = x;
	b1 = c;
	break;
    case 4:
	r1 = x;
	g1 = 0.0;
	b1 = c;
	break;
    case 5:
	r1 = c;
	g1 = 0.0;
	b1 = x;
	break;
    default:
	TRACE(("Bad HLS input: [%d,%d,%d], returning white\n", h, l, s));
	*r = (short) 360;
	*g = (short) MAX_PCT;
	*b = (short) MAX_PCT;
	return;
    }

    *r = (short) ((r1 + m) * MAX_PCT + 0.5);
    *g = (short) ((g1 + m) * MAX_PCT + 0.5);
    *b = (short) ((b1 + m) * MAX_PCT + 0.5);

    if (*r < 0)
	*r = 0;
    else if (*r > 100)
	*r = 100;
    if (*g < 0)
	*g = 0;
    else if (*g > 100)
	*g = 100;
    if (*b < 0)
	*b = 0;
    else if (*b > 100)
	*b = 100;
}

void
rgb2hls(int r, int g, int b, short *h, short *l, short *s)
{
    const double scaled_r = (r / MAX_RGB);
    const double scaled_g = (g / MAX_RGB);
    const double scaled_b = (b / MAX_RGB);

    const double min_scale = Min(Min(scaled_r, scaled_g), scaled_b);
    const double max_scale = Max(Max(scaled_r, scaled_g), scaled_b);
    const double dif_scale = max_scale - min_scale;

    double h_work = 0.;
    double s_work = 0.;
    double l_work = ((max_scale + min_scale) / 2.);

    if (dif_scale != 0.) {
	if (l_work < 0.5f) {
	    s_work = (dif_scale / (max_scale + min_scale));
	} else {
	    s_work = (dif_scale / (2. - max_scale - min_scale));
	}

	if (scaled_r == max_scale) {
	    h_work = (scaled_g - scaled_b) / dif_scale;
	} else if (scaled_g == max_scale) {
	    h_work = 2. + (scaled_b - scaled_r) / dif_scale;
	} else if (scaled_b == max_scale) {
	    h_work = 4. + (scaled_r - scaled_g) / dif_scale;
	}
    }

    h_work *= 60.;
    if (h_work < 0)
	h_work += 360.;

    s_work *= MAX_RGB;
    l_work *= MAX_RGB;

    *h = (short) h_work;
    *s = (short) s_work;
    *l = (short) l_work;
}

void
dump_graphic(Graphic const *graphic)
{
#if defined(DUMP_COLORS) || defined(DUMP_BITMAP)
    RegisterNum color;
#endif
#ifdef DUMP_BITMAP
    int r, c;
    ColorRegister const *reg;
#endif

    (void) graphic;

    TRACE(("graphic stats: id=%u charrow=%d charcol=%d actual_width=%d actual_height=%d pixw=%d pixh=%d\n",
	   graphic->id,
	   graphic->charrow,
	   graphic->charcol,
	   graphic->actual_width,
	   graphic->actual_height,
	   graphic->pixw,
	   graphic->pixh));

#ifdef DUMP_COLORS
    TRACE(("graphic colors:\n"));
    for (color = 0; color < graphic->valid_registers; color++) {
	TRACE(("%03u: %d,%d,%d\n",
	       color,
	       graphic->color_registers[color].r,
	       graphic->color_registers[color].g,
	       graphic->color_registers[color].b));
    }
#endif

#ifdef DUMP_BITMAP
    TRACE(("graphic pixels:\n"));
    for (r = 0; r < graphic->actual_height; r++) {
	for (c = 0; c < graphic->actual_width; c++) {
	    color = graphic->pixels[r * graphic->max_width + c];
	    if (color == COLOR_HOLE) {
		TRACE(("?"));
	    } else {
		reg = &graphic->color_registers[color];
		if (reg->r + reg->g + reg->b > 200) {
		    TRACE(("#"));
		} else if (reg->r + reg->g + reg->b > 150) {
		    TRACE(("%%"));
		} else if (reg->r + reg->g + reg->b > 100) {
		    TRACE((":"));
		} else if (reg->r + reg->g + reg->b > 80) {
		    TRACE(("."));
		} else {
		    TRACE((" "));
		}
	    }
	}
	TRACE(("\n"));
    }

    TRACE(("\n"));
#endif
}

/* Erase the portion of any displayed graphic overlapping with a rectangle
 * of the given size and location in pixels relative to the start of the
 * graphic.  This is used to allow text to "erase" graphics underneath it.
 */
static void
erase_graphic(Graphic *graphic, int x, int y, int w, int h)
{
    const int pw = graphic->pixw;
    const int ph = graphic->pixh;
    const int r_min = y - ph + 1;
    const int r_max = y + h - 1;
    const int c_min = x - pw + 1;
    const int c_max = x + w - 1;

    int r;
    int rbase = 0;

    TRACE(("erasing graphic %d,%d %dx%d\n", x, y, w, h));

    for (r = 0; r < graphic->actual_height; r++) {
	if (rbase >= r_min
	    && rbase <= r_max) {
	    int c;
	    int cbase = 0;
	    for (c = 0; c < graphic->actual_width; c++) {
		if (cbase >= c_min
		    && cbase <= c_max) {
		    const int cell = r * graphic->max_width + c;
		    ClrSpixel(graphic, cell);
		}
		cbase += pw;
	    }
	}
	rbase += ph;
    }
}

static int
compare_graphic_ids(const void *left, const void *right)
{
    const Graphic *l = *(const Graphic *const *) left;
    const Graphic *r = *(const Graphic *const *) right;

    if (!l->valid || !r->valid)
	return 0;

    if (l->bufferid < r->bufferid)
	return -1;
    else if (l->bufferid > r->bufferid)
	return 1;

    if (l->id < r->id)
	return -1;
    else
	return 1;
}

static void
clip_area(int *orig_x, int *orig_y, int *orig_w, int *orig_h,
	  int clip_x, int clip_y, int clip_w, int clip_h)
{
    if (*orig_x < clip_x) {
	const int diff = clip_x - *orig_x;
	*orig_x += diff;
	*orig_w -= diff;
    }
    if (*orig_w > 0 && *orig_x + *orig_w > clip_x + clip_w) {
	*orig_w -= (*orig_x + *orig_w) - (clip_x + clip_w);
    }

    if (*orig_y < clip_y) {
	const int diff = clip_y - *orig_y;
	*orig_y += diff;
	*orig_h -= diff;
    }
    if (*orig_h > 0 && *orig_y + *orig_h > clip_y + clip_h) {
	*orig_h -= (*orig_y + *orig_h) - (clip_y + clip_h);
    }
}

static Bool
GetGraphicsOrder(TScreen *screen,
		 int skip_clean,
		 Graphic *ordered_graphics[MAX_GRAPHICS],
		 unsigned *resultp)
{
    unsigned ii;
    unsigned active_count;

    *resultp = active_count = 0;
    FOR_EACH_SLOT(ii) {
	Graphic *graphic;
	if (!(graphic = getActiveSlot(ii)))
	    continue;
	TRACE(("refreshing graphic %d on buffer %d, current buffer %d\n",
	       graphic->id, graphic->bufferid, screen->whichBuf));
	if (screen->whichBuf == 0) {
	    if (graphic->bufferid != 0) {
		TRACE(("skipping graphic %d from alt buffer (%d) when drawing screen=%d\n",
		       graphic->id, graphic->bufferid, screen->whichBuf));
		continue;
	    }
	} else {
	    if (graphic->bufferid == 0 && graphic->charrow >= 0) {
		TRACE(("skipping graphic %d from normal buffer (%d) when drawing screen=%d because it is not in scrollback area\n",
		       graphic->id, graphic->bufferid, screen->whichBuf));
		continue;
	    }
	    if (graphic->bufferid == 1 &&
		graphic->charrow + (graphic->actual_height +
				    FontHeight(screen) - 1) /
		FontHeight(screen) < 0) {
		TRACE(("skipping graphic %d from alt buffer (%d) when drawing screen=%d because it is completely in scrollback area\n",
		       graphic->id, graphic->bufferid, screen->whichBuf));
		continue;
	    }
	}
	if (graphic->hidden)
	    continue;
	ordered_graphics[active_count++] = graphic;
    }

    if (active_count == 0)
	return False;
    if (active_count > 1) {
	qsort(ordered_graphics,
	      (size_t) active_count,
	      sizeof(ordered_graphics[0]),
	      compare_graphic_ids);
    }

    if (skip_clean) {
	unsigned jj;
	unsigned skip_count;

	for (jj = 0; jj < active_count; ++jj) {
	    if (ordered_graphics[jj]->dirty)
		break;
	}
	skip_count = jj;
	if (skip_count == active_count)
	    return False;

	active_count -= skip_count;
	for (jj = 0; jj < active_count; ++jj) {
	    ordered_graphics[jj] = ordered_graphics[jj + skip_count];
	}
    }
    *resultp = active_count;
    return True;
}

static ColorRegister *
AllocGraphicsBuffer(TScreen *screen,
		    int ncols,
		    int nrows)
{
    int xx, yy;
    int const refresh_w = ncols * FontWidth(screen);
    int const refresh_h = nrows * FontHeight(screen);
    ColorRegister *buffer;

    if (!(buffer = TypeMallocN(ColorRegister,
			         (unsigned) refresh_w * (unsigned) refresh_h))) {
	TRACE(("unable to allocate %dx%d buffer for graphics refresh\n",
	       refresh_w, refresh_h));
    } else {
	/* assuming two's complement, the memset will be much faster than loop */
	if ((unsigned short) null_color.r == 0xffff) {
	    memset(buffer, 0xff,
		   sizeof(ColorRegister) * (size_t) (refresh_h * refresh_w));
	} else {
	    for (yy = 0; yy < refresh_h; yy++) {
		for (xx = 0; xx < refresh_w; xx++) {
		    buffer[yy * refresh_w + xx] = null_color;
		}
	    }
	}
    }
    return buffer;
}

typedef struct {
    int x_min;
    int x_max;
    int y_min;
    int y_max;
} ClipLimits;

static Boolean
RefreshClipped(TScreen *screen,
	       int leftcol,
	       int toprow,
	       int ncols,
	       int nrows,
	       Graphic *ordered_graphics[MAX_GRAPHICS],
	       unsigned active_count,
	       ColorRegister *buffer,
	       ClipLimits * result)
{
    int const scroll_y = screen->topline * FontHeight(screen);
    int const refresh_x = leftcol * FontWidth(screen);
    int const refresh_y = toprow * FontHeight(screen) + scroll_y;
    int const refresh_w = ncols * FontWidth(screen);
    int const refresh_h = nrows * FontHeight(screen);
    ClipLimits my_limits;
    unsigned jj;

    int const altarea_x = 0;
    int const altarea_y = 0;
    int const altarea_w = Width(screen) * FontWidth(screen);
    int const altarea_h = Height(screen) * FontHeight(screen);

    int const scrollarea_x = 0;
    int const scrollarea_y = scroll_y;
    int const scrollarea_w = Width(screen) * FontWidth(screen);
    int const scrollarea_h = -scroll_y;

    int const mainarea_x = 0;
    int const mainarea_y = scroll_y;
    int const mainarea_w = Width(screen) * FontWidth(screen);
    int const mainarea_h = -scroll_y + Height(screen) * FontHeight(screen);

    my_limits.x_min = refresh_x + refresh_w;
    my_limits.x_max = refresh_x - 1;
    my_limits.y_min = refresh_y + refresh_h;
    my_limits.y_max = refresh_y - 1;
    for (jj = 0; jj < active_count; ++jj) {
	Graphic *graphic = ordered_graphics[jj];
	int draw_x = graphic->charcol * FontWidth(screen);
	int draw_y = graphic->charrow * FontHeight(screen);
	int draw_w = graphic->actual_width;
	int draw_h = graphic->actual_height;

	if (screen->whichBuf != 0) {
	    if (graphic->bufferid != 0) {
		/* clip to alt buffer */
		clip_area(&draw_x, &draw_y, &draw_w, &draw_h,
			  altarea_x, altarea_y, altarea_w, altarea_h);
	    } else {
		/* clip to scrollback area */
		clip_area(&draw_x, &draw_y, &draw_w, &draw_h,
			  scrollarea_x, scrollarea_y,
			  scrollarea_w, scrollarea_h);
	    }
	} else {
	    /* clip to scrollback + normal area */
	    clip_area(&draw_x, &draw_y, &draw_w, &draw_h,
		      mainarea_x, mainarea_y,
		      mainarea_w, mainarea_h);
	}

	clip_area(&draw_x, &draw_y, &draw_w, &draw_h,
		  refresh_x, refresh_y, refresh_w, refresh_h);

	TRACE(("refresh: graph=%u\n", jj));
	TRACE(("         refresh_x=%d refresh_y=%d refresh_w=%d refresh_h=%d\n",
	       refresh_x, refresh_y, refresh_w, refresh_h));
	TRACE(("         draw_x=%d draw_y=%d draw_w=%d draw_h=%d\n",
	       draw_x, draw_y, draw_w, draw_h));

	if (draw_w > 0 && draw_h > 0) {
	    refresh_graphic(screen, graphic, buffer,
			    refresh_x, refresh_y,
			    refresh_w, refresh_h,
			    draw_x, draw_y,
			    draw_w, draw_h);
	    if (draw_x < my_limits.x_min)
		my_limits.x_min = draw_x;
	    if (draw_x + draw_w - 1 > my_limits.x_max)
		my_limits.x_max = draw_x + draw_w - 1;
	    if (draw_y < my_limits.y_min)
		my_limits.y_min = draw_y;
	    if (draw_y + draw_h - 1 > my_limits.y_max)
		my_limits.y_max = draw_y + draw_h - 1;
	}
	graphic->dirty = False;
    }

    if (my_limits.x_max < refresh_x ||
	my_limits.x_min > refresh_x + refresh_w - 1 ||
	my_limits.y_max < refresh_y ||
	my_limits.y_min > refresh_y + refresh_h - 1) {
	return False;
    }
    *result = my_limits;
    return True;
}

static Boolean
FindGraphicHoles(int refresh_x,
		 int refresh_y,
		 int refresh_w,
		 ColorRegister *buffer,
		 ClipLimits * limits,
		 unsigned *result)
{
    const int y_min = limits->y_min - refresh_y;
    const int y_max = limits->y_max - refresh_y;
    const int x_min = limits->x_min - refresh_x;
    const int x_max = limits->x_max - refresh_x;
    const ColorRegister *base = buffer + (y_min * refresh_w);
    int xx, yy;

    unsigned holes = 0U;
    unsigned non_holes = 0U;

    for (yy = y_min; yy <= y_max; yy++) {
	const ColorRegister *scan = base + x_min;
	for (xx = x_min; xx <= x_max; xx++) {
	    if (scan->r < 0 || scan->g < 0 || scan->b < 0) {
		holes++;
	    } else {
		non_holes++;
	    }
	    if (non_holes && holes)
		goto finish;
	    ++scan;
	}
	base += refresh_w;
    }

  finish:
    *result = holes;
    return (non_holes != 0);
}

/* the coordinates are relative to the screen */
static void
refresh_graphics(XtermWidget xw,
		 int leftcol,
		 int toprow,
		 int ncols,
		 int nrows,
		 int skip_clean)
{
    TScreen *const screen = TScreenOf(xw);
    Display *const display = screen->display;
    Window const drawable = VDrawable(screen);
    int const scroll_y = screen->topline * FontHeight(screen);
    int const refresh_x = leftcol * FontWidth(screen);
    int const refresh_y = toprow * FontHeight(screen) + scroll_y;
    int const refresh_w = ncols * FontWidth(screen);

    Graphic *ordered_graphics[MAX_GRAPHICS];
    unsigned active_count;
    unsigned holes;
    int xx, yy;

    ColorRegister *buffer;
    ClipLimits clip_limits;

    if_TRACE(int const refresh_h = nrows * FontHeight(screen));

    if (!GetGraphicsOrder(screen, skip_clean, ordered_graphics, &active_count))
	return;

    if (!valid_graphics) {
	memset(&xgcv, 0, sizeof(xgcv));
	xgcv.graphics_exposures = False;
	graphics_gc = XCreateGC(display, drawable, GCGraphicsExposures, &xgcv);
	last_color = null_color;
	gc_color = null_color;
	if (graphics_gc == None) {
	    TRACE(("unable to allocate GC for graphics refresh\n"));
	    valid_graphics = -1;
	} else {
	    valid_graphics = 1;
	}
    }
    if (valid_graphics < 0)
	return;

    if ((buffer = AllocGraphicsBuffer(screen, ncols, nrows)) == NULL)
	return;

    TRACE(("refresh: screen->topline=%d leftcol=%d toprow=%d nrows=%d ncols=%d (%d,%d %dx%d)\n",
	   screen->topline,
	   leftcol, toprow,
	   nrows, ncols,
	   refresh_x, refresh_y,
	   refresh_w, refresh_h));

    if (!RefreshClipped(screen, leftcol, toprow, ncols, nrows,
			ordered_graphics,
			active_count,
			buffer,
			&clip_limits)) {
	free(buffer);
	return;
    }

    if (!FindGraphicHoles(refresh_x,
			  refresh_y,
			  refresh_w,
			  buffer,
			  &clip_limits,
			  &holes)) {
	TRACE(("refresh: visible graphics areas are erased; nothing to do\n"));
	free(buffer);
	return;
    }

    /*
     * If we have any holes we can't just copy an image rectangle, and masking
     * with bitmaps is very expensive.  This fallback is surprisingly faster
     * than the XPutImage version in some cases, but I don't know why.
     * (This is even though there's no X11 primitive for drawing a horizontal
     * line of height one and no attempt is made to handle multiple lines at
     * once.)
     */
    if (holes > 0U) {
	int run;

	run = 0;
	for (yy = clip_limits.y_min - refresh_y;
	     yy <= clip_limits.y_max - refresh_y;
	     yy++) {
	    for (xx = clip_limits.x_min - refresh_x;
		 xx <= clip_limits.x_max - refresh_x;
		 xx++) {
		const ColorRegister color = buffer[yy * refresh_w + xx];

		if (color.r < 0 || color.g < 0 || color.b < 0) {
		    last_color = color;
		    if (run > 0) {
			XDrawLine(display, drawable, graphics_gc,
				  OriginX(screen) + refresh_x + xx - run,
				  (OriginY(screen) - scroll_y) + refresh_y + yy,
				  OriginX(screen) + refresh_x + xx - 1,
				  (OriginY(screen) - scroll_y) + refresh_y + yy);
			run = 0;
		    }
		    continue;
		}

		if (DiffColor(color, last_color)) {
		    last_color = color;
		    if (run > 0) {
			XDrawLine(display, drawable, graphics_gc,
				  OriginX(screen) + refresh_x + xx - run,
				  (OriginY(screen) - scroll_y) + refresh_y + yy,
				  OriginX(screen) + refresh_x + xx - 1,
				  (OriginY(screen) - scroll_y) + refresh_y + yy);
			run = 0;
		    }

		    if (DiffColor(color, gc_color)) {
			xgcv.foreground =
			    color_register_to_xpixel(&color, xw);
			XChangeGC(display, graphics_gc, GCForeground, &xgcv);
			gc_color = color;
		    }
		}
		run++;
	    }
	    if (run > 0) {
		last_color = null_color;
		XDrawLine(display, drawable, graphics_gc,
			  OriginX(screen) + refresh_x + xx - run,
			  (OriginY(screen) - scroll_y) + refresh_y + yy,
			  OriginX(screen) + refresh_x + xx - 1,
			  (OriginY(screen) - scroll_y) + refresh_y + yy);
		run = 0;
	    }
	}
    } else {
	ColorRegister old_colors[2];
	Pixel fg, old_result[2];
	XImage *image;
	char *imgdata;
	const unsigned image_w = ((unsigned) clip_limits.x_max + 1U -
				  (unsigned) clip_limits.x_min);
	const unsigned image_h = ((unsigned) clip_limits.y_max + 1U -
				  (unsigned) clip_limits.y_min);
	int nn;

	image = XCreateImage(display, xw->visInfo->visual,
			     (unsigned) xw->visInfo->depth,
			     ZPixmap, 0, NULL,
			     image_w, image_h,
			     (int) (sizeof(int) * 8U), 0);
	if (!image) {
	    TRACE(("unable to allocate XImage for graphics refresh\n"));
	    free(buffer);
	    return;
	}
	imgdata = TypeMallocN(char, (size_t)(image_h * (unsigned)image->bytes_per_line));
	if (!imgdata) {
	    TRACE(("unable to allocate XImage for graphics refresh\n"));
	    XDestroyImage(image);
	    free(buffer);
	    return;
	}
	image->data = imgdata;

	fg = 0U;
	nn = 0;

	/* two-level cache cuts down on lookup-calls */
	old_result[0] = 0U;
	old_result[1] = 0U;
	old_colors[0] = null_color;
	old_colors[1] = null_color;

	for (yy = clip_limits.y_min - refresh_y;
	     yy <= clip_limits.y_max - refresh_y;
	     yy++) {
	    for (xx = clip_limits.x_min - refresh_x;
		 xx <= clip_limits.x_max - refresh_x;
		 xx++) {
		const ColorRegister color = buffer[yy * refresh_w + xx];

		if (DiffColor(color, old_colors[nn])) {
		    if (DiffColor(color, old_colors[!nn])) {
			nn = !nn;
			fg = color_register_to_xpixel(&color, xw);
			old_result[nn] = fg;
			old_colors[nn] = color;
		    } else {
			nn = !nn;
			fg = old_result[nn];
		    }
		}

		XPutPixel(image,
			  xx + refresh_x - clip_limits.x_min,
			  yy + refresh_y - clip_limits.y_min, fg);
	    }
	}

	XPutImage(display, drawable, graphics_gc, image,
		  0, 0,
		  OriginX(screen) + clip_limits.x_min,
		  (OriginY(screen) - scroll_y) + clip_limits.y_min,
		  image_w, image_h);
	free(imgdata);
	image->data = NULL;
	XDestroyImage(image);
    }

    free(buffer);
    XFlush(display);
}

void
refresh_displayed_graphics(XtermWidget xw,
			   int leftcol,
			   int toprow,
			   int ncols,
			   int nrows)
{
    refresh_graphics(xw, leftcol, toprow, ncols, nrows, 0);
}

void
refresh_modified_displayed_graphics(XtermWidget xw)
{
    TScreen const *screen = TScreenOf(xw);
    refresh_graphics(xw, 0, 0, MaxCols(screen), MaxRows(screen), 1);
}

void
scroll_displayed_graphics(XtermWidget xw, int rows)
{
    if (used_graphics) {
	TScreen const *screen = TScreenOf(xw);
	unsigned ii;

	TRACE(("graphics scroll: moving all up %d rows\n", rows));
	/* FIXME: VT125 ReGIS graphics are fixed at the upper left of the display; need to verify */

	FOR_EACH_SLOT(ii) {
	    Graphic *graphic;

	    if (!(graphic = getActiveSlot(ii)))
		continue;
	    if (graphic->bufferid != screen->whichBuf)
		continue;
	    if (graphic->hidden)
		continue;

	    graphic->charrow -= rows;
	}
    }
}

void
pixelarea_clear_displayed_graphics(TScreen const *screen,
				   int winx,
				   int winy,
				   int w,
				   int h)
{
    unsigned ii;

    if (!used_graphics)
	return;

    FOR_EACH_SLOT(ii) {
	Graphic *graphic;
	/* FIXME: are these coordinates (scrolled) screen-relative? */
	int const scroll_y = (screen->whichBuf == 0
			      ? screen->topline * FontHeight(screen)
			      : 0);
	int graph_x;
	int graph_y;
	int x, y;

	if (!(graphic = getActiveSlot(ii)))
	    continue;
	if (graphic->bufferid != screen->whichBuf)
	    continue;
	if (graphic->hidden)
	    continue;

	graph_x = graphic->charcol * FontWidth(screen);
	graph_y = graphic->charrow * FontHeight(screen);
	x = winx - graph_x;
	y = (winy - scroll_y) - graph_y;

	TRACE(("pixelarea clear graphics: screen->topline=%d winx=%d winy=%d w=%d h=%d x=%d y=%d\n",
	       screen->topline,
	       winx, winy,
	       w, h,
	       x, y));
	erase_graphic(graphic, x, y, w, h);
    }
}

void
chararea_clear_displayed_graphics(TScreen const *screen,
				  int leftcol,
				  int toprow,
				  int ncols,
				  int nrows)
{
    if (used_graphics) {
	int const x = leftcol * FontWidth(screen);
	int const y = toprow * FontHeight(screen);
	int const w = ncols * FontWidth(screen);
	int const h = nrows * FontHeight(screen);

	TRACE(("chararea clear graphics: screen->topline=%d leftcol=%d toprow=%d nrows=%d ncols=%d x=%d y=%d w=%d h=%d\n",
	       screen->topline,
	       leftcol, toprow,
	       nrows, ncols,
	       x, y, w, h));
	pixelarea_clear_displayed_graphics(screen, x, y, w, h);
    }
}

void
reset_displayed_graphics(TScreen const *screen)
{
    init_color_registers(screen, getSharedRegisters());

    if (used_graphics) {
	unsigned ii;

	TRACE(("resetting all graphics\n"));
	FOR_EACH_SLOT(ii) {
	    deactivateSlot(ii);
	}
#if OPT_REGIS_GRAPHICS
	reset_regis();
#endif
    }
}

#ifdef NO_LEAKS
void
noleaks_graphics(Display *dpy)
{
    unsigned ii;

    FOR_EACH_SLOT(ii) {
	deactivateSlot(ii);
    }
    if (valid_graphics > 0)
	XFreeGC(dpy, graphics_gc);
}
#endif
