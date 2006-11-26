#ifndef _TSENG_ACCEL_H
#define _TSENG_ACCEL_H
/*
 * Shortcuts to Tseng memory-mapped accelerator-control registers
 */

#define MMU_CONTROL(x)  MMIO_OUT8(pTseng->MMioBase, 0x13<<0, x)
#define ACL_SUSPEND_TERMINATE(x)  MMIO_OUT8(pTseng->MMioBase, 0x30<<0, x)
#define ACL_OPERATION_STATE(x)  MMIO_OUT8(pTseng->MMioBase, 0x31<<0, x)

#define ACL_SYNC_ENABLE(x)  MMIO_OUT8(pTseng->MMioBase, 0x32<<0, x)
    /* for ET6000, ACL_SYNC_ENABLE becomes ACL_6K_CONFIG */

#define ACL_INTERRUPT_STATUS(x) \
                        MMIO_OUT8(pTseng->MMioBase, 0x35<<0, x)
#define ACL_INTERRUPT_MASK(x) MMIO_OUT8(pTseng->MMioBase, 0x34<<0, x)
#define ACL_ACCELERATOR_STATUS (0x36 << 0)
#define ACL_ACCELERATOR_STATUS_SET(x) \
                        MMIO_OUT8(pTseng->MMioBase, ACL_ACCELERATOR_STATUS, x)
#define ACL_WRITE_INTERFACE_VALID (0x33 << 0)

    /* and this is only for the ET6000 */
#define ACL_POWER_CONTROL(x) MMIO_OUT8(pTseng->MMioBase, 0x37<<0, x)

    /* non-queued for w32p's and ET6000 */
#define ACL_NQ_X_POSITION(x)  MMIO_OUT16(pTseng->MMioBase, 0x38<<0, x)
#define ACL_NQ_Y_POSITION(x)  MMIO_OUT16(pTseng->MMioBase, 0x3A<<0, x)
    /* queued for w32 and w32i */
#define ACL_X_POSITION(x)  MMIO_OUT16(pTseng->MMioBase, 0x94<<0, x)
#define ACL_Y_POSITION(x)  MMIO_OUT16(pTseng->MMioBase, 0x96<<0, x)

#define ACL_PATTERN_ADDRESS(x)  MMIO_OUT32(pTseng->MMioBase, 0x80<<0, x)
#define ACL_SOURCE_ADDRESS(x)  MMIO_OUT32(pTseng->MMioBase, 0x84<<0, x)

#define ACL_PATTERN_Y_OFFSET(x)  MMIO_OUT16(pTseng->MMioBase, 0x88<<0, x)
#define ACL_PATTERN_Y_OFFSET32(x)  MMIO_OUT32(pTseng->MMioBase, 0x88<<0, x)
#define ACL_SOURCE_Y_OFFSET(x)  MMIO_OUT16(pTseng->MMioBase, 0x8A<<0, x)
#define ACL_DESTINATION_Y_OFFSET(x)  MMIO_OUT16(pTseng->MMioBase, 0x8C<<0, x)

    /* W32i */
#define ACL_VIRTUAL_BUS_SIZE(x) MMIO_OUT8(pTseng->MMioBase, 0x8E<<0, x)
    /* w32p */
#define ACL_PIXEL_DEPTH(x)  MMIO_OUT8(pTseng->MMioBase, 0x8E<<0, x)

    /* w32 and w32i */
#define ACL_XY_DIRECTION(x)  MMIO_OUT8(pTseng->MMioBase, 0x8F<<0, x)

#define ACL_PATTERN_WRAP(x)   MMIO_OUT8(pTseng->MMioBase, 0x90<<0, x)
#define ACL_PATTERN_WRAP32(x)   MMIO_OUT32(pTseng->MMioBase, 0x90<<0, x)
#define ACL_TRANSFER_DISABLE(x)  MMIO_OUT8(pTseng->MMioBase, 0x91<<0, x) /* ET6000 only */
#define ACL_SOURCE_WRAP(x) MMIO_OUT8(pTseng->MMioBase, 0x92<<0, x)

#define ACL_X_COUNT(x) MMIO_OUT16(pTseng->MMioBase, 0x98<<0, x)
#define ACL_Y_COUNT(x) MMIO_OUT16(pTseng->MMioBase, 0x9A<<0, x)
/* shortcut. not a real register */
#define ACL_XY_COUNT(x) MMIO_OUT32(pTseng->MMioBase, 0x98<<0, x)

#define ACL_ROUTING_CONTROL(x) MMIO_OUT8(pTseng->MMioBase, 0x9C<<0, x)
    /* for ET6000, ACL_ROUTING_CONTROL becomes ACL_MIX_CONTROL */
#define ACL_RELOAD_CONTROL(x) MMIO_OUT8(pTseng->MMioBase, 0x9D<<0, x)
    /* for ET6000, ACL_RELOAD_CONTROL becomes ACL_STEPPING_INHIBIT */

#define ACL_BACKGROUND_RASTER_OPERATION(x)  MMIO_OUT8(pTseng->MMioBase, 0x9E<<0, x)
#define ACL_FOREGROUND_RASTER_OPERATION(x)  MMIO_OUT8(pTseng->MMioBase, 0x9F<<0, x)

#define ACL_DESTINATION_ADDRESS(x) MMIO_OUT32(pTseng->MMioBase, 0xA0<<0, x)

    /* the following is for the w32p's only */
#define ACL_MIX_ADDRESS(x) MMIO_OUT32(pTseng->MMioBase, 0xA4<<0, x)

#define ACL_MIX_Y_OFFSET(x) MMIO_OUT16(pTseng->MMioBase, 0xA8<<0, x)
#define ACL_ERROR_TERM(x) MMIO_OUT16(pTseng->MMioBase, 0xAA<<0, x)
#define ACL_DELTA_MINOR(x) MMIO_OUT16(pTseng->MMioBase, 0xAC<<0, x)
#define ACL_DELTA_MINOR32(x) MMIO_OUT32(pTseng->MMioBase, 0xAC<<0, x)
#define ACL_DELTA_MAJOR(x) MMIO_OUT16(pTseng->MMioBase, 0xAE<<0, x)

    /* ET6000 only (trapezoids) */
#define ACL_SECONDARY_EDGE(x) MMIO_OUT8(pTseng->MMioBase, 0x93<<0, x)
#define ACL_SECONDARY_ERROR_TERM(x) MMIO_OUT16(pTseng->MMioBase, 0xB2<<0, x)
#define ACL_SECONDARY_DELTA_MINOR(x) MMIO_OUT16(pTseng->MMioBase, 0xB4<<0, x)
#define ACL_SECONDARY_DELTA_MINOR32(x) MMIO_OUT32(pTseng->MMioBase, 0xB4<<0, x)
#define ACL_SECONDARY_DELTA_MAJOR(x) MMIO_OUT16(pTseng->MMioBase, 0xB6<<0, x)

/* for ET6000: */
#define ACL_6K_CONFIG ACL_SYNC_ENABLE

/* for ET6000: */
#define ACL_MIX_CONTROL ACL_ROUTING_CONTROL
#define ACL_STEPPING_INHIBIT ACL_RELOAD_CONTROL

/*
 * Some shortcuts. 
 */

#define MAX_WAIT_CNT 500000	       /* how long we wait before we time out */
#undef WAIT_VERBOSE		       /* if defined: print out how long we waited */

static void tseng_recover_timeout(TsengPtr pTseng);

static __inline__ void 
tseng_wait(TsengPtr pTseng, int reg, char *name, unsigned char mask)
{
    int cnt = MAX_WAIT_CNT;

    while ((MMIO_IN32(pTseng->MMioBase,reg)) & mask)
	if (--cnt < 0) {
	    ErrorF("WAIT_%s: timeout.\n", name);
	    tseng_recover_timeout(pTseng);
	    break;
	}
#ifdef WAIT_VERBOSE
    ErrorF("%s%d ", name, MAX_WAIT_CNT - cnt);
#endif
}

#define WAIT_QUEUE tseng_wait(pTseng, ACL_ACCELERATOR_STATUS, "QUEUE", 0x1)

/* This is only for W32p rev b...d */
#define WAIT_INTERFACE tseng_wait(pTseng, ACL_WRITE_INTERFACE_VALID, "INTERFACE", 0xf)

#define WAIT_ACL tseng_wait(pTseng, ACL_ACCELERATOR_STATUS, "ACL", 0x2)

#define WAIT_XY tseng_wait(pTseng, ACL_ACCELERATOR_STATUS, "XY", 0x4)

#define SET_FUNCTION_BLT \
    if (pTseng->ChipType == ET6000) \
        ACL_MIX_CONTROL(0x33); \
    else \
        ACL_ROUTING_CONTROL(0x00);

#define SET_FUNCTION_BLT_TR \
        ACL_MIX_CONTROL(0x13);

#define FBADDR(pTseng, x,y) ( (y) * pTseng->line_width + MULBPP(pTseng, x) )

#define SET_FG_ROP(rop) \
    ACL_FOREGROUND_RASTER_OPERATION(W32OpTable[rop]);

#define SET_FG_ROP_PLANEMASK(rop) \
    ACL_FOREGROUND_RASTER_OPERATION(W32OpTable_planemask[rop]);

#define SET_BG_ROP(rop) \
    ACL_BACKGROUND_RASTER_OPERATION(W32PatternOpTable[rop]);

#define SET_BG_ROP_TR(rop, bg_color) \
  if ((bg_color) == -1)    /* transparent color expansion */ \
    ACL_BACKGROUND_RASTER_OPERATION(0xaa); \
  else \
    ACL_BACKGROUND_RASTER_OPERATION(W32PatternOpTable[rop]);

#define SET_DELTA(Min, Maj) \
    ACL_DELTA_MINOR32(((Maj) << 16) + (Min))

#define SET_SECONDARY_DELTA(Min, Maj) \
    ACL_SECONDARY_DELTA_MINOR(((Maj) << 16) + (Min))

#ifdef NO_OPTIMIZE
#define SET_XYDIR(dir) \
      ACL_XY_DIRECTION(dir);
#else
/*
 * only changing ACL_XY_DIRECTION when it needs to be changed avoids
 * unnecessary PCI bus writes, which are slow. This shows up very well
 * on consecutive small fills.
 */
#define SET_XYDIR(dir) \
    if ((dir) != pTseng->tseng_old_dir) \
      pTseng->tseng_old_dir = (dir); \
      ACL_XY_DIRECTION(pTseng->tseng_old_dir);
#endif

#define SET_SECONDARY_XYDIR(dir) \
      ACL_SECONDARY_EDGE(dir);

/* Must do 0x09 (in one operation) for the W32 */
#define START_ACL(pTseng, dst) \
    ACL_DESTINATION_ADDRESS(dst);

/* START_ACL for the ET6000 */
#define START_ACL_6(dst) \
    ACL_DESTINATION_ADDRESS(dst);

#define START_ACL_CPU(pTseng, dst) \
      ACL_DESTINATION_ADDRESS(dst);

/*    ACL_DESTINATION_ADDRESS(dst);    should be enough for START_ACL_CPU */

/*
 * Some commonly used inline functions and utility functions.
 */

static __inline__ int
COLOR_REPLICATE_DWORD(TsengPtr pTseng, int color)
{
    switch (pTseng->Bytesperpixel) {
    case 1:
	color &= 0xFF;
	color = (color << 8) | color;
	color = (color << 16) | color;
	break;
    case 2:
	color &= 0xFFFF;
	color = (color << 16) | color;
	break;
    }
    return color;
}

/*
 * Optimizing note: increasing the wrap size for fixed-color source/pattern
 * tiles from 4x1 (as below) to anything bigger doesn't seem to affect
 * performance (it might have been better for larger wraps, but it isn't).
 */

static __inline__ void
SET_FG_COLOR(TsengPtr pTseng, int color)
{
    ACL_SOURCE_ADDRESS(pTseng->AccelColorBufferOffset + pTseng->tsengFg);
    ACL_SOURCE_Y_OFFSET(3);
    color = COLOR_REPLICATE_DWORD(pTseng, color);
    MMIO_OUT32(pTseng->scratchMemBase, pTseng->tsengFg, color);
    
    ACL_SOURCE_WRAP(0x02);
}

static __inline__ void
SET_BG_COLOR(TsengPtr pTseng, int color)
{
    ACL_PATTERN_ADDRESS(pTseng->AccelColorBufferOffset + pTseng->tsengPat);
    ACL_PATTERN_Y_OFFSET(3);
    color = COLOR_REPLICATE_DWORD(pTseng, color);
    MMIO_OUT32(pTseng->scratchMemBase,pTseng->tsengPat, color);
    
    ACL_PATTERN_WRAP(0x02);
}

/*
 * this does the same as SET_FG_COLOR and SET_BG_COLOR together, but is
 * faster, because it allows the PCI chipset to chain the requests into a
 * burst sequence. The order of the commands is partly linear.
 * So far for the theory...
 */
static __inline__ void
SET_FG_BG_COLOR(TsengPtr pTseng, int fgcolor, int bgcolor)
{
    ACL_PATTERN_ADDRESS(pTseng->AccelColorBufferOffset + pTseng->tsengPat);
    ACL_SOURCE_ADDRESS(pTseng->AccelColorBufferOffset + pTseng->tsengFg);
    ACL_PATTERN_Y_OFFSET32(0x00030003);
    fgcolor = COLOR_REPLICATE_DWORD(pTseng, fgcolor);
    bgcolor = COLOR_REPLICATE_DWORD(pTseng, bgcolor);
    MMIO_OUT32(pTseng->scratchMemBase,pTseng->tsengFg, fgcolor);
    MMIO_OUT32(pTseng->scratchMemBase,pTseng->tsengPat, bgcolor);

    ACL_PATTERN_WRAP32(0x00020002);
}

/*
 * Real 32-bit multiplications are horribly slow compared to 16-bit (on i386).
 */
#ifdef NO_OPTIMIZE
static __inline__ int
MULBPP(TsengPtr pTseng, int x)
{
    return (x * pTseng->Bytesperpixel);
}
#else
static __inline__ int
MULBPP(TsengPtr pTseng, int x)
{
    int result = x << pTseng->powerPerPixel;

    if (pTseng->Bytesperpixel != 3)
	return result;
    else
	return result + x;
}
#endif

static __inline__ int
CALC_XY(TsengPtr pTseng, int x, int y)
{
    int new_x, xy;

    if ((pTseng->old_y == y) && (pTseng->old_x == x))
	return -1;

    if (pTseng->ChipType == ET4000)
	new_x = MULBPP(pTseng, x - 1);
    else
	new_x = MULBPP(pTseng, x) - 1;
    xy = ((y - 1) << 16) + new_x;
    pTseng->old_x = x;
    pTseng->old_y = y;
    return xy;
}

/* generic SET_XY */
static __inline__ void
SET_XY(TsengPtr pTseng, int x, int y)
{
    int new_x;

    if (pTseng->ChipType == ET4000)
	new_x = MULBPP(pTseng, x - 1);
    else
	new_x = MULBPP(pTseng, x) - 1;
    ACL_XY_COUNT(((y - 1) << 16) + new_x);
    pTseng->old_x = x;
    pTseng->old_y = y;
}

static __inline__ void
SET_X_YRAW(TsengPtr pTseng, int x, int y)
{
    int new_x;

    if (pTseng->ChipType == ET4000)
	new_x = MULBPP(pTseng, x - 1);
    else
	new_x = MULBPP(pTseng, x) - 1;
    ACL_XY_COUNT((y << 16) + new_x);
    pTseng->old_x = x;
    pTseng->old_y = y - 1;	      /* old_y is invalid (raw transfer) */
}

/*
 * This is plain and simple "benchmark rigging".
 * (no real application does lots of subsequent same-size blits)
 *
 * The effect of this is amazingly good on e.g large blits: 400x400
 * rectangle fill in 24 and 32 bpp on ET6000 jumps from 276 MB/sec to up to
 * 490 MB/sec... But not always. There must be a good reason why this gives
 * such a boost, but I don't know it.
 */

static __inline__ void
SET_XY_4(TsengPtr pTseng, int x, int y)
{
    int new_xy;

    if ((pTseng->old_y != y) || (pTseng->old_x != x)) {
	new_xy = ((y - 1) << 16) + MULBPP(pTseng, x - 1);
	ACL_XY_COUNT(new_xy);
	pTseng->old_x = x;
	pTseng->old_y = y;
    }
}

static __inline__ void
SET_XY_6(TsengPtr pTseng, int x, int y)
{
    int new_xy;			       /* using this intermediate variable is faster */

    if ((pTseng->old_y != y) || (pTseng->old_x != x)) {
	new_xy = ((y - 1) << 16) + MULBPP(pTseng, x) - 1;
	ACL_XY_COUNT(new_xy);
	pTseng->old_x = x;
	pTseng->old_y = y;
    }
}

/* generic SET_XY_RAW */
static __inline__ void
SET_XY_RAW(TsengPtr pTseng,int x, int y)
{
    ACL_XY_COUNT((y << 16) + x);
    pTseng->old_x = pTseng->old_y = -1;   /* invalidate old_x/old_y (raw transfers) */
}

static __inline__ void
PINGPONG(TsengPtr pTseng)
{
    if (pTseng->tsengFg == 0) {
	pTseng->tsengFg = 8;
	pTseng->tsengBg = 24;
	pTseng->tsengPat = 40;
    } else {
	pTseng->tsengFg = 0;
	pTseng->tsengBg = 16;
	pTseng->tsengPat = 32;
    }
}

/*
 * This is called in each ACL function just before the first ACL register is
 * written to. It waits for the accelerator to finish on cards that don't
 * support hardware-wait-state locking, and waits for a free queue entry on
 * others, if hardware-wait-states are not enabled.
 */
static __inline__ void
wait_acl_queue(TsengPtr pTseng)
{
    if (pTseng->UsePCIRetry)
	WAIT_QUEUE;
    if (pTseng->need_wait_acl)
	WAIT_ACL;
}
#endif /* _TSENG_ACCEL_H */
