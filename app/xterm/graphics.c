/* $XTermId: graphics.c,v 1.42 2014/05/03 14:26:57 tom Exp $ */

/*
 * Copyright 2013,2014 by Ross Combs
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

#undef DUMP_BITMAP
#undef DUMP_COLORS
#undef DEBUG_PALETTE
#undef DEBUG_PIXEL
#undef DEBUG_REFRESH

/* TODO:
 * ReGIS:
 * - shading with text
 * - polygon filling
 * - plane write control
 * - fix interpolated curves to more closely match implementation (identical despite direction and starting point)
 * - text
 * - input and output cursors
 * - mouse input
 * - stacks
 * - investigate second graphic page for ReGIS -- does it also apply to text and sixel graphics? are the contents preserved?
 * - font upload, italics, and other text attributes
 * - enter/leave during a command
 * - command display mode
 * - scrolling
 * - custom coordinate systems
 * - scaling/re-rasterization to fit screen
 * - macros
 * sixel:
 * - fix problem where new_row < 0 during sixel parsing (see FIXME)
 * VT55/VT105 waveform graphics
 * - everything
 * common:
 * - handle light/dark screen modes (CSI?5[hl])
 * - update text fg/bg color which overlaps images
 * - erase graphic when erasing screen
 * - handle graphic updates in scroll regions
 * - handle rectangular area copies (verify they work with graphics)
 * - maintain ordered list/array instead of qsort()
 * - erase text under graphic if bg not transparent to avoid flickering (or not: bad if the font changes or window resizes)
 * - erase graphics under graphic if same origin and bg not transparent to avoid flickering
 * - erase scrolled portions of all graphics on alt buffer
 * - delete graphic if scrolled past end of scrollback
 * - delete graphic if all pixels are transparent/erased
 * - dynamic memory allocation of graphics buffers, add configurable limits
 * - auto convert color graphics in VT330 mode
 * - posturize requested colors to match hardware palettes (e.g. four possible shades on VT240)
 * - color register report/restore
 * escape sequences:
 * - way to query font size without "window ops" (or make "window ops" permissions more fine grained)
 * - way to query and/or set the maximum number of color registers
 * - way to query and set the number of graphics pages
 * ReGIS extensions:
 * - gradients
 * - line width (RLogin has this and it is mentioned in docs for the DEC ReGIS to Postscript converter)
 * - F option for screen command (mentioned in docs for the DEC ReGIS to Postscript converter)
 * - transparency
 * - background color as stackable write control
 * - RGB triplets
 * - true color (virtual color registers created upon lookup)
 * - anti-aliasing
 */

/* font sizes:
 * VT510:
 *   80 Columns 132 Columns Maximum Number of Lines
 *   10 x 16   6 x 16  26 lines + keyboard indicator line
 *   10 x 13   6 x 13  26 lines + keyboard indicator line
 *   10 x 10   6 x 10  42 lines + keyboard indicator line
 *   10 x 8    6 x 8   53 lines + keyboard indicator line
*/

#define FOR_EACH_SLOT(ii) for (ii = 0U; ii < MAX_GRAPHICS; ii++)

static ColorRegister *shared_color_registers;
static Graphic *displayed_graphics[MAX_GRAPHICS];
static unsigned next_graphic_id = 0U;

static ColorRegister *
allocRegisters(void)
{
    return TypeCallocN(ColorRegister, MAX_COLOR_REGISTERS);
}

static Graphic *
freeGraphic(Graphic *obj)
{
    if (obj) {
	if (obj->pixels)
	    free(obj->pixels);
	if (obj->private_color_registers)
	    free(obj->private_color_registers);
	free(obj);
    }
    return NULL;
}

static Graphic *
allocGraphic(void)
{
    Graphic *result = TypeCalloc(Graphic);
    if (result) {
	if (!(result->pixels = TypeCallocN(RegisterNum, MAX_PIXELS))) {
	    result = freeGraphic(result);
	} else if (!(result->private_color_registers = allocRegisters())) {
	    result = freeGraphic(result);
	}
    }
    return result;
}

static Graphic *
getActiveSlot(unsigned n)
{
    if (n < MAX_GRAPHICS &&
	displayed_graphics[n] &&
	displayed_graphics[n]->valid) {
	return displayed_graphics[n];
    }
    return NULL;
}

static Graphic *
getInactiveSlot(unsigned n)
{
    if (n < MAX_GRAPHICS &&
	(!displayed_graphics[n] ||
	 !displayed_graphics[n]->valid)) {
	if (!displayed_graphics[n]) {
	    displayed_graphics[n] = allocGraphic();
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
    if (n < MAX_GRAPHICS) {
	displayed_graphics[n] = freeGraphic(displayed_graphics[n]);
    }
}

extern RegisterNum
read_pixel(Graphic *graphic, int x, int y)
{
    if (x < 0 && x >= graphic->actual_width &&
	y < 0 && y >= graphic->actual_height) {
	return COLOR_HOLE;
    }

    return graphic->pixels[y * graphic->max_width + x];
}

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
	graphic->pixels[y * graphic->max_width + x] = (RegisterNum) color;
	if (color < MAX_COLOR_REGISTERS)
	    graphic->color_registers_used[color] = 1;
    } else {
	TRACE(("pixel %d,%d out of bounds\n", x, y));
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

    for (y = y1; y <= y2; y++)
	for (x = x1; x < x2; x++)
	    draw_solid_pixel(graphic, x, y, color);
}

void
draw_solid_line(Graphic *graphic, int x1, int y1, int x2, int y2, unsigned color)
{
    int x, y;
    int dx, dy;
    int dir, diff;

    assert(color <= MAX_COLOR_REGISTERS);

    dx = abs(x1 - x2);
    dy = abs(y1 - y2);

    if (dx > dy) {
	if (x1 > x2) {
	    int tmp;
	    EXCHANGE(x1, x2, tmp);
	    EXCHANGE(y1, y2, tmp);
	}
	if (y1 < y2)
	    dir = 1;
	else if (y1 > y2)
	    dir = -1;
	else
	    dir = 0;

	diff = 0;
	y = y1;
	for (x = x1; x <= x2; x++) {
	    if (diff >= dx) {
		diff -= dx;
		y += dir;
	    }
	    diff += dy;
	    draw_solid_pixel(graphic, x, y, color);
	}
    } else {
	if (y1 > y2) {
	    int tmp;
	    EXCHANGE(x1, x2, tmp);
	    EXCHANGE(y1, y2, tmp);
	}
	if (x1 < x2)
	    dir = 1;
	else if (x1 > x2)
	    dir = -1;
	else
	    dir = 0;

	diff = 0;
	x = x1;
	for (y = y1; y <= y2; y++) {
	    if (diff >= dy) {
		diff -= dy;
		x += dir;
	    }
	    diff += dx;
	    draw_solid_pixel(graphic, x, y, color);
	}
    }
}

static void
set_color_register(ColorRegister *color_registers,
		   unsigned color,
		   int r,
		   int g,
		   int b)
{
    ColorRegister *reg = &color_registers[color];
    reg->r = (short) r;
    reg->g = (short) g;
    reg->b = (short) b;
    reg->allocated = 0;
}

/* Graphics which don't use private colors will act as if they are using a
 * device-wide color palette.
 */
static void
set_shared_color_register(unsigned color, int r, int g, int b)
{
    Graphic *graphic;
    unsigned ii;

    assert(color < MAX_COLOR_REGISTERS);

    set_color_register(getSharedRegisters(), color, r, g, b);

    FOR_EACH_SLOT(ii) {
	if (!(graphic = getActiveSlot(ii)))
	    continue;
	if (graphic->private_colors)
	    continue;

	if (graphic->color_registers_used[ii]) {
	    graphic->dirty = 1;
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
	    graphic->dirty = 1;
	}
	graphic->color_registers_used[color] = 1;
    } else {
	set_shared_color_register(color, r, g, b);
    }
}

#define SQUARE(X) ( (X) * (X) )

RegisterNum
find_color_register(ColorRegister const *color_registers, int r, int g, int b)
{
    unsigned i;
    unsigned d;
    unsigned closest_index;
    unsigned closest_distance;

    /* I have no idea what algorithm DEC used for this.
     * The documentation warns that it is unpredictable, especially with values
     * far away from any allocated color so it is probably a very simple
     * hueristic rather than something fancy like finding the minimum distance
     * in a linear perceptive color space.
     */
    closest_index = MAX_COLOR_REGISTERS;
    closest_distance = 0U;
    for (i = 0U; i < MAX_COLOR_REGISTERS; i++) {
	d = (unsigned) (SQUARE(2 * (color_registers[i].r - r)) +
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

static void
init_color_registers(ColorRegister *color_registers, int terminal_id)
{
    TRACE(("setting inital colors for terminal %d\n", terminal_id));
    {
	unsigned i;

	for (i = 0U; i < MAX_COLOR_REGISTERS; i++) {
	    set_color_register(color_registers, (RegisterNum) i, 0, 0, 0);
	}
    }

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
    switch (terminal_id) {
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
	    printf("initial value for register %03u: %d,%d,%d\n",
		   i,
		   color_registers[i].r,
		   color_registers[i].g,
		   color_registers[i].b);
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
    switch (screen->terminal_id) {
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
init_graphic(Graphic *graphic,
	     unsigned type,
	     int terminal_id,
	     int charrow,
	     int charcol,
	     unsigned num_color_registers,
	     int private_colors)
{
    unsigned i;

    TRACE(("initializing graphic object\n"));

    graphic->dirty = 1;
    for (i = 0U; i < MAX_PIXELS; i++)
	graphic->pixels[i] = COLOR_HOLE;
    memset(graphic->color_registers_used, 0, sizeof(graphic->color_registers_used));

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
    graphic->max_width = BUFFER_WIDTH;
    graphic->max_height = BUFFER_HEIGHT;

    graphic->actual_width = 0;
    graphic->actual_height = 0;

    graphic->pixw = 1;
    graphic->pixh = 1;

    graphic->valid_registers = num_color_registers;
    TRACE(("%d color registers\n", graphic->valid_registers));

    graphic->private_colors = private_colors;
    if (graphic->private_colors) {
	TRACE(("using private color registers\n"));
	init_color_registers(graphic->private_color_registers, terminal_id);
	graphic->color_registers = graphic->private_color_registers;
    } else {
	TRACE(("using shared color registers\n"));
	graphic->color_registers = getSharedRegisters();
    }

    graphic->charrow = charrow;
    graphic->charcol = charcol;
    graphic->type = type;
    graphic->valid = 0;
}

Graphic *
get_new_graphic(XtermWidget xw, int charrow, int charcol, unsigned type)
{
    TScreen const *screen = TScreenOf(xw);
    int bufferid = screen->whichBuf;
    int terminal_id = screen->terminal_id;
    Graphic *graphic;
    unsigned ii;

    FOR_EACH_SLOT(ii) {
	if ((graphic = getInactiveSlot(ii))) {
	    TRACE(("using fresh graphic index=%u id=%u\n", ii, next_graphic_id));
	    break;
	}
    }

    /* if none are free, recycle the graphic scrolled back the farthest */
    if (!graphic) {
	int min_charrow = 0;
	Graphic *min_graphic = NULL;

	FOR_EACH_SLOT(ii) {
	    if (!(graphic = getActiveSlot(ii)))
		continue;
	    if (!min_graphic || graphic->charrow < min_charrow) {
		min_charrow = graphic->charrow;
		min_graphic = graphic;
	    }
	}
	TRACE(("recycling old graphic index=%u id=%u\n", ii, next_graphic_id));
	graphic = min_graphic;
    }

    if (graphic) {
	unsigned num_color_registers;
	num_color_registers = get_color_register_count(screen);
	graphic->xw = xw;
	graphic->bufferid = bufferid;
	graphic->id = next_graphic_id++;
	init_graphic(graphic,
		     type,
		     terminal_id,
		     charrow,
		     charcol,
		     num_color_registers,
		     screen->privatecolorregisters);
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
    int bufferid = screen->whichBuf;
    Graphic *graphic;
    unsigned ii;

    FOR_EACH_SLOT(ii) {
	if ((graphic = getActiveSlot(ii)) &&
	    graphic->type == type &&
	    graphic->bufferid == bufferid &&
	    graphic->charrow == charrow &&
	    graphic->charcol == charcol &&
	    graphic->actual_width == actual_width &&
	    graphic->actual_height == actual_height) {
	    TRACE(("found existing graphic index=%u id=%u\n", ii, graphic->id));
	    return graphic;
	}
    }

    /* if no match get a new graphic */
    if ((graphic = get_new_graphic(xw, charrow, charcol, type))) {
	graphic->actual_width = actual_width;
	graphic->actual_height = actual_height;
    }
    return graphic;
}

#define ScaleForXColor(s) (unsigned short) ((long)(s) * 65535 / 100)

static Pixel
color_register_to_xpixel(ColorRegister *reg, XtermWidget xw)
{
    if (!reg->allocated) {
	XColor def;

	def.red = ScaleForXColor(reg->r);
	def.green = ScaleForXColor(reg->g);
	def.blue = ScaleForXColor(reg->b);
	def.flags = DoRed | DoGreen | DoBlue;
	if (!allocateBestRGB(xw, &def)) {
	    TRACE(("unable to allocate xcolor for color register\n"));
	    return 0UL;
	}
	reg->pix = def.pixel;
	reg->allocated = 1;
    }

    /* FIXME: with so many possible colors we need to determine
     * when to free them to be nice to PseudoColor displays
     */
    return reg->pix;
}

static void
refresh_graphic(TScreen const *screen,
		Graphic const *graphic,
		int xbase,
		int ybase,
		int x,
		int y,
		int w,
		int h)
{
    Display *display = screen->display;
    Window vwindow = WhichVWin(screen)->window;
    GC graphics_gc;
    int r, c;
    int pw, ph;
    int rbase, cbase;
    RegisterNum color;
    RegisterNum old_fg;
    XGCValues xgcv;
    XtGCMask mask;
    int holes, total;

    TRACE(("refreshing graphic from %d,%d %dx%d (valid=%d, size=%dx%d, scale=%dx%d max=%dx%d) at base=%d,%d\n",
	   x, y, w, h,
	   graphic->valid,
	   graphic->actual_width,
	   graphic->actual_height,
	   graphic->pixw,
	   graphic->pixh,
	   graphic->max_width,
	   graphic->max_height,
	   xbase, ybase));

    memset(&xgcv, 0, sizeof(xgcv));
    xgcv.foreground = 0UL;
    xgcv.graphics_exposures = False;
    mask = GCForeground | GCGraphicsExposures;
    graphics_gc = XCreateGC(display, vwindow, mask, &xgcv);

    pw = graphic->pixw;
    ph = graphic->pixh;

    TRACE(("refreshed graphic covers 0,0 to %d,%d\n",
	   (graphic->actual_width - 1) * pw + pw - 1,
	   (graphic->actual_height - 1) * ph + ph - 1));
    TRACE(("refreshed area covers %d,%d to %d,%d\n",
	   x, y,
	   x + w - 1,
	   y + h - 1));

    old_fg = COLOR_HOLE;
    holes = total = 0;
    rbase = 0;
    for (r = 0; r < graphic->actual_height; r++) {
	int rtest = rbase;

	rbase += ph;
	if (rtest + ph - 1 < y)
	    continue;
	if (rtest > y + h - 1)
	    continue;

	cbase = 0;
	for (c = 0; c < graphic->actual_width; c++) {
	    int ctest = cbase;

	    cbase += pw;
	    if (ctest + pw - 1 < x)
		continue;
	    if (ctest > x + w - 1)
		continue;

	    total++;
	    color = graphic->pixels[r * graphic->max_width + c];
	    if (color == COLOR_HOLE) {
		holes++;
		continue;
	    }

	    if (color != old_fg) {
		xgcv.foreground =
		    color_register_to_xpixel(&graphic->color_registers[color],
					     graphic->xw);
		XChangeGC(display, graphics_gc, mask, &xgcv);
		old_fg = color;
	    }

	    XFillRectangle(display, vwindow, graphics_gc,
			   xbase + ctest,
			   ybase + rtest,
			   (unsigned) pw,
			   (unsigned) ph);
	}
    }

#ifdef DEBUG_REFRESH
    {
	XColor def;

	def.red = (short) (1.0 * 65535.0);
	def.green = (short) (0.1 * 65535.0);
	def.blue = (short) (1.0 * 65535.0);
	def.flags = DoRed | DoGreen | DoBlue;
	if (allocateBestRGB(graphic->xw, &def)) {
	    xgcv.foreground = def.pixel;
	    XChangeGC(display, graphics_gc, mask, &xgcv);
	}
	XFillRectangle(display, vwindow, graphics_gc,
		       xbase + 0,
		       ybase + 0,
		       (unsigned) pw, (unsigned) ph);
	XFillRectangle(display, vwindow, graphics_gc,
		       xbase + (graphic->actual_width - 1) * pw,
		       ybase + (graphic->actual_height - 1) * ph,
		       (unsigned) pw, (unsigned) ph);

	def.red = (unsigned short) ((1.0 - 0.1 * (rand() / (double)
						  RAND_MAX) * 65535.0));
	def.green = (unsigned short) ((0.7 + 0.2 * (rand() / (double)
						    RAND_MAX)) * 65535.0);
	def.blue = (unsigned short) ((0.1 + 0.1 * (rand() / (double)
						   RAND_MAX)) * 65535.0);
	def.flags = DoRed | DoGreen | DoBlue;
	if (allocateBestRGB(graphic->xw, &def)) {
	    xgcv.foreground = def.pixel;
	    XChangeGC(display, graphics_gc, mask, &xgcv);
	}
	XDrawLine(display, vwindow, graphics_gc,
		  xbase + x + 0, ybase + y + 0,
		  xbase + x + w - 1, ybase + y + 0);
	XDrawLine(display, vwindow, graphics_gc,
		  xbase + x + w - 1, ybase + y + 0,
		  xbase + x + 0, ybase + y + h - 1);
	XDrawLine(display, vwindow, graphics_gc,
		  xbase + x + 0, ybase + y + h - 1,
		  xbase + x + w - 1, ybase + y + h - 1);
	XDrawLine(display, vwindow, graphics_gc,
		  xbase + x + w - 1, ybase + y + h - 1,
		  xbase + x + 0, ybase + y + 0);
    }
#endif
    XFlush(display);
    TRACE(("done refreshing graphic: %d of %d refreshed pixels were holes\n",
	   holes, total));

    XFreeGC(display, graphics_gc);
}

/*
 * Primary color hues:
 *  blue:    0 degrees
 *  red:   120 degrees
 *  green: 240 degrees
 */
void
hls2rgb(int h, int l, int s, short *r, short *g, short *b)
{
    double hs = (h + 240) % 360;
    double hv = hs / 360.0;
    double lv = l / 100.0;
    double sv = s / 100.0;
    double c, x, m, c2;
    double r1, g1, b1;
    int hpi;

    if (s == 0) {
	*r = *g = *b = (short) l;
	return;
    }

    if ((c2 = ((2.0 * lv) - 1.0)) < 0.0)
	c2 = -c2;
    c = (1.0 - c2) * sv;
    hpi = (int) (hv * 6.0);
    x = (hpi & 1) ? c : 0.0;
    m = lv - 0.5 * c;

    switch (hpi) {
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
	*r = (short) 100;
	*g = (short) 100;
	*b = (short) 100;
	return;
    }

    *r = (short) ((r1 + m) * 100.0 + 0.5);
    *g = (short) ((g1 + m) * 100.0 + 0.5);
    *b = (short) ((b1 + m) * 100.0 + 0.5);

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
 * of the given size and location in pixels.
 * This is used to allow text to "erase" graphics underneath it.
 */
static void
erase_graphic(Graphic *graphic, int x, int y, int w, int h)
{
    RegisterNum hole = COLOR_HOLE;
    int pw, ph;
    int r, c;
    int rbase, cbase;

    pw = graphic->pixw;
    ph = graphic->pixh;

    TRACE(("erasing graphic %d,%d %dx%d\n", x, y, w, h));

    rbase = 0;
    for (r = 0; r < graphic->actual_height; r++) {
	if (rbase + ph - 1 >= y
	    && rbase <= y + h - 1) {
	    cbase = 0;
	    for (c = 0; c < graphic->actual_width; c++) {
		if (cbase + pw - 1 >= x
		    && cbase <= x + w - 1) {
		    graphic->pixels[r * graphic->max_width + c] = hole;
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
    if (l->id < r->id)
	return -1;
    else
	return 1;
}

void
refresh_displayed_graphics(TScreen const *screen,
			   int leftcol,
			   int toprow,
			   int ncols,
			   int nrows)
{
    Graphic *ordered_graphics[MAX_GRAPHICS];
    Graphic *graphic;
    unsigned ii;
    unsigned jj = 0;
    int x, y, w, h;
    int xbase, ybase;

    FOR_EACH_SLOT(ii) {
	if ((graphic = getActiveSlot(ii))) {
	    ordered_graphics[jj++] = graphic;
	}
    }
    if (jj > 1) {
	qsort(ordered_graphics,
	      (size_t) jj,
	      sizeof(ordered_graphics[0]),
	      compare_graphic_ids);
    }

    for (ii = 0; ii < jj; ++ii) {
	graphic = ordered_graphics[ii];
	if (graphic->bufferid != screen->whichBuf)
	    continue;

	x = (leftcol - graphic->charcol) * FontWidth(screen);
	y = (toprow - graphic->charrow) * FontHeight(screen);
	w = ncols * FontWidth(screen);
	h = nrows * FontHeight(screen);

	xbase = (OriginX(screen)
		 + graphic->charcol * FontWidth(screen));
	ybase = (OriginY(screen)
		 + (graphic->charrow - screen->topline) * FontHeight(screen));

	if (xbase + x + w + OriginX(screen) > FullWidth(screen))
	    w = FullWidth(screen) - (xbase + x + OriginX(screen));
	if (ybase + y + h + OriginY(screen) > FullHeight(screen))
	    h = FullHeight(screen) - (ybase + y + OriginY(screen));
	else if (ybase + y < OriginY(screen)) {
	    int diff = OriginY(screen) - (ybase + y);
	    y += diff;
	    h -= diff;
	}

	TRACE(("graphics refresh: screen->topline=%d leftcol=%d toprow=%d nrows=%d ncols=%d x=%d y=%d w=%d h=%d xbase=%d ybase=%d\n",
	       screen->topline,
	       leftcol, toprow,
	       nrows, ncols,
	       x, y, w, h,
	       xbase, ybase));
	refresh_graphic(screen, graphic, xbase, ybase, x, y, w, h);
    }
}

void
refresh_modified_displayed_graphics(TScreen const *screen)
{
    Graphic *graphic;
    unsigned ii;
    int leftcol, toprow;
    int nrows, ncols;
    int x, y, w, h;
    int xbase, ybase;

    FOR_EACH_SLOT(ii) {
	if (!(graphic = getActiveSlot(ii)))
	    continue;
	if (graphic->bufferid != screen->whichBuf)
	    continue;
	if (!graphic->dirty)
	    continue;

	leftcol = graphic->charcol;
	toprow = graphic->charrow;
	nrows = (((graphic->actual_height * graphic->pixh)
		  + FontHeight(screen) - 1)
		 / FontHeight(screen));
	ncols = (((graphic->actual_width * graphic->pixw)
		  + FontWidth(screen) - 1)
		 / FontWidth(screen));

	x = (leftcol - graphic->charcol) * FontWidth(screen);
	y = (toprow - graphic->charrow) * FontHeight(screen);
	w = ncols * FontWidth(screen);
	h = nrows * FontHeight(screen);

	xbase = (OriginX(screen)
		 + graphic->charcol * FontWidth(screen));
	ybase = (OriginY(screen)
		 + (graphic->charrow - screen->topline) * FontHeight(screen));

	if (xbase + x + w + OriginX(screen) > FullWidth(screen))
	    w = FullWidth(screen) - (xbase + x + OriginX(screen));
	if (ybase + y + h + OriginY(screen) > FullHeight(screen))
	    h = FullHeight(screen) - (ybase + y + OriginY(screen));
	else if (ybase + y < OriginY(screen)) {
	    int diff = OriginY(screen) - (ybase + y);
	    y += diff;
	    h -= diff;
	}

	TRACE(("full graphics refresh: screen->topline=%d leftcol=%d toprow=%d nrows=%d ncols=%d x=%d y=%d w=%d h=%d xbase=%d ybase=%d\n",
	       screen->topline,
	       leftcol, toprow,
	       nrows, ncols,
	       x, y, w, h,
	       xbase, ybase));
	refresh_graphic(screen, graphic, xbase, ybase, x, y, w, h);
	graphic->dirty = 0;
    }
}

void
scroll_displayed_graphics(int rows)
{
    Graphic *graphic;
    unsigned ii;

    TRACE(("graphics scroll: moving all up %d rows\n", rows));
    /* FIXME: VT125 ReGIS graphics are fixed at the upper left of the display; need to verify */

    FOR_EACH_SLOT(ii) {
	if (!(graphic = getActiveSlot(ii)))
	    continue;

	graphic->charrow -= rows;
    }
}

void
pixelarea_clear_displayed_graphics(TScreen const *screen,
				   int winx,
				   int winy,
				   int w,
				   int h)
{
    Graphic *graphic;
    unsigned ii;
    int x, y;

    FOR_EACH_SLOT(ii) {
	if (!(graphic = getActiveSlot(ii)))
	    continue;

	x = winx - graphic->charcol * FontWidth(screen);
	y = winy - graphic->charrow * FontHeight(screen);

	TRACE(("pixelarea graphics erase: screen->topline=%d winx=%d winy=%d w=%d h=%d x=%d y=%d\n",
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
    int x, y, w, h;

    x = leftcol * FontWidth(screen);
    y = toprow * FontHeight(screen);
    w = ncols * FontWidth(screen);
    h = nrows * FontHeight(screen);

    TRACE(("chararea clear graphics: screen->topline=%d leftcol=%d toprow=%d nrows=%d ncols=%d x=%d y=%d w=%d h=%d\n",
	   screen->topline,
	   leftcol, toprow,
	   nrows, ncols,
	   x, y, w, h));
    pixelarea_clear_displayed_graphics(screen, x, y, w, h);
}

void
reset_displayed_graphics(TScreen const *screen)
{
    unsigned ii;

    init_color_registers(getSharedRegisters(), screen->terminal_id);

    TRACE(("resetting all graphics\n"));
    FOR_EACH_SLOT(ii) {
	deactivateSlot(ii);
    }
}

#ifdef NO_LEAKS
void
noleaks_graphics(void)
{
    unsigned ii;

    FOR_EACH_SLOT(ii) {
	deactivateSlot(ii);
    }
}
#endif
