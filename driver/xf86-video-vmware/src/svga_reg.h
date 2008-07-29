/* **********************************************************
 * Copyright (C) 1998-2001 VMware, Inc.
 * All Rights Reserved
 * $Id: svga_reg.h,v 1.2 2008/07/29 20:07:54 matthieu Exp $
 * **********************************************************/

/*
 * svga_reg.h --
 *
 * SVGA hardware definitions
 */

#ifndef _SVGA_REG_H_
#define _SVGA_REG_H_

#define INCLUDE_ALLOW_USERLEVEL
#define INCLUDE_ALLOW_VMMEXT
#define INCLUDE_ALLOW_VMCORE
#include "includeCheck.h"

/*
 * Memory and port addresses and fundamental constants
 */

/*
 * Note-- MAX_WIDTH and MAX_HEIGHT are largely ignored by the code.  This
 * isn't such a bad thing for forward compatibility. --Jeremy.
 */
#define SVGA_MAX_WIDTH			2360
#define SVGA_MAX_HEIGHT			1770
#define SVGA_MAX_BITS_PER_PIXEL		32
#define SVGA_MAX_DEPTH                  24
#define SVGA_MAX_DISPLAYS               10

/*
 * The maximum size of the onscreen framebuffer.  The size of the
 * changeMap in the monitor is proportional to this number since it's
 * the amount of memory we need to trace in VESA mode.  Therefore, we'd
 * like to keep it as small as possible to reduce monitor overhead (using
 * SVGA_VRAM_MAX_SIZE for this increases the size of the shared area
 * by over 4k!).
 */
 
#define SVGA_FB_MAX_SIZE \
   ((((SVGA_MAX_WIDTH * SVGA_MAX_HEIGHT *                                    \
       SVGA_MAX_BITS_PER_PIXEL / 8) >> PAGE_SHIFT) + 1) << PAGE_SHIFT)

#define SVGA_MAX_PSEUDOCOLOR_DEPTH	8
#define SVGA_MAX_PSEUDOCOLORS		(1 << SVGA_MAX_PSEUDOCOLOR_DEPTH)
#define SVGA_NUM_PALETTE_REGS           (3 * SVGA_MAX_PSEUDOCOLORS)

#define SVGA_MAGIC         0x900000UL
#define SVGA_MAKE_ID(ver)  (SVGA_MAGIC << 8 | (ver))

/* Version 2 let the address of the frame buffer be unsigned on Win32 */
#define SVGA_VERSION_2     2
#define SVGA_ID_2          SVGA_MAKE_ID(SVGA_VERSION_2)

/* Version 1 has new registers starting with SVGA_REG_CAPABILITIES so
   PALETTE_BASE has moved */
#define SVGA_VERSION_1     1
#define SVGA_ID_1          SVGA_MAKE_ID(SVGA_VERSION_1)

/* Version 0 is the initial version */
#define SVGA_VERSION_0     0
#define SVGA_ID_0          SVGA_MAKE_ID(SVGA_VERSION_0)

/* Invalid SVGA_ID_ */
#define SVGA_ID_INVALID    0xFFFFFFFF

/* More backwards compatibility, old location of color map: */
#define SVGA_OLD_PALETTE_BASE	 17

/* Base and Offset gets us headed the right way for PCI Base Addr Registers */
#define SVGA_LEGACY_BASE_PORT	0x4560
#define SVGA_INDEX_PORT		0x0
#define SVGA_VALUE_PORT		0x1
#define SVGA_BIOS_PORT		0x2
#define SVGA_NUM_PORTS		0x3
#define SVGA_IRQSTATUS_PORT	0x8

/*
 * Interrupt source flags for IRQSTATUS_PORT and IRQMASK.
 *
 * Interrupts are only supported when the
 * SVGA_CAP_IRQMASK capability is present.
 */
#define SVGA_IRQFLAG_ANY_FENCE            0x1    /* Any fence was passed */
#define SVGA_IRQFLAG_FIFO_PROGRESS        0x2    /* Made forward progress in the FIFO */
#define SVGA_IRQFLAG_FENCE_GOAL           0x4    /* SVGA_FIFO_FENCE_GOAL reached */

/* This port is deprecated, but retained because of old drivers. */
#define SVGA_LEGACY_ACCEL_PORT	0x3

/* Legal values for the SVGA_REG_CURSOR_ON register in cursor bypass mode */
#define SVGA_CURSOR_ON_HIDE               0x0    /* Must be 0 to maintain backward compatibility */
#define SVGA_CURSOR_ON_SHOW               0x1    /* Must be 1 to maintain backward compatibility */
#define SVGA_CURSOR_ON_REMOVE_FROM_FB     0x2    /* Remove the cursor from the framebuffer because we need to see what's under it */
#define SVGA_CURSOR_ON_RESTORE_TO_FB      0x3    /* Put the cursor back in the framebuffer so the user can see it */

/*
 * Registers
 */

enum {
   SVGA_REG_ID = 0,
   SVGA_REG_ENABLE = 1,
   SVGA_REG_WIDTH = 2,
   SVGA_REG_HEIGHT = 3,
   SVGA_REG_MAX_WIDTH = 4,
   SVGA_REG_MAX_HEIGHT = 5,
   SVGA_REG_DEPTH = 6,
   SVGA_REG_BITS_PER_PIXEL = 7,     /* Current bpp in the guest */
   SVGA_REG_PSEUDOCOLOR = 8,
   SVGA_REG_RED_MASK = 9,
   SVGA_REG_GREEN_MASK = 10,
   SVGA_REG_BLUE_MASK = 11,
   SVGA_REG_BYTES_PER_LINE = 12,
   SVGA_REG_FB_START = 13,
   SVGA_REG_FB_OFFSET = 14,
   SVGA_REG_VRAM_SIZE = 15,
   SVGA_REG_FB_SIZE = 16,

   /* ID 0 implementation only had the above registers, then the palette */

   SVGA_REG_CAPABILITIES = 17,
   SVGA_REG_MEM_START = 18,	   /* Memory for command FIFO and bitmaps */
   SVGA_REG_MEM_SIZE = 19,
   SVGA_REG_CONFIG_DONE = 20,      /* Set when memory area configured */
   SVGA_REG_SYNC = 21,             /* See "FIFO Synchronization Registers" */
   SVGA_REG_BUSY = 22,             /* See "FIFO Synchronization Registers" */
   SVGA_REG_GUEST_ID = 23,	   /* Set guest OS identifier */
   SVGA_REG_CURSOR_ID = 24,	   /* ID of cursor */
   SVGA_REG_CURSOR_X = 25,	   /* Set cursor X position */
   SVGA_REG_CURSOR_Y = 26,	   /* Set cursor Y position */
   SVGA_REG_CURSOR_ON = 27,	   /* Turn cursor on/off */
   SVGA_REG_HOST_BITS_PER_PIXEL = 28, /* Current bpp in the host */
   SVGA_REG_SCRATCH_SIZE = 29,     /* Number of scratch registers */
   SVGA_REG_MEM_REGS = 30,         /* Number of FIFO registers */
   SVGA_REG_NUM_DISPLAYS = 31,     /* Number of guest displays */
   SVGA_REG_PITCHLOCK = 32,        /* Fixed pitch for all modes */
   SVGA_REG_IRQMASK = 33,          /* Interrupt mask */
   SVGA_REG_NUM_GUEST_DISPLAYS = 34,/* Number of guest displays in X/Y direction */
   SVGA_REG_DISPLAY_ID = 35,        /* The display ID for the following display attributes */
   SVGA_REG_DISPLAY_IS_PRIMARY = 36,/* Whether this is a primary display */
   SVGA_REG_DISPLAY_POSITION_X = 37,/* The display position x */
   SVGA_REG_DISPLAY_POSITION_Y = 38,/* The display position y */
   SVGA_REG_DISPLAY_WIDTH = 39,     /* The display's width */
   SVGA_REG_DISPLAY_HEIGHT = 40,    /* The display's height */
   SVGA_REG_TOP = 41,               /* Must be 1 more than the last register */

   SVGA_PALETTE_BASE = 1024,	    /* Base of SVGA color map */
   /* Next 768 (== 256*3) registers exist for colormap */
   SVGA_SCRATCH_BASE = SVGA_PALETTE_BASE + SVGA_NUM_PALETTE_REGS
                                    /* Base of scratch registers */
   /* Next reg[SVGA_REG_SCRATCH_SIZE] registers exist for scratch usage:
      First 4 are reserved for VESA BIOS Extension; any remaining are for
      the use of the current SVGA driver. */
};


/*
 *  Capabilities
 */

#define SVGA_CAP_NONE               0x00000000
#define SVGA_CAP_RECT_FILL	    0x00000001
#define SVGA_CAP_RECT_COPY	    0x00000002
#define SVGA_CAP_RECT_PAT_FILL      0x00000004
#define SVGA_CAP_LEGACY_OFFSCREEN   0x00000008
#define SVGA_CAP_RASTER_OP	    0x00000010
#define SVGA_CAP_CURSOR		    0x00000020
#define SVGA_CAP_CURSOR_BYPASS	    0x00000040
#define SVGA_CAP_CURSOR_BYPASS_2    0x00000080
#define SVGA_CAP_8BIT_EMULATION     0x00000100
#define SVGA_CAP_ALPHA_CURSOR       0x00000200
#define SVGA_CAP_GLYPH              0x00000400
#define SVGA_CAP_GLYPH_CLIPPING     0x00000800
#define SVGA_CAP_OFFSCREEN_1        0x00001000
#define SVGA_CAP_ALPHA_BLEND        0x00002000
#define SVGA_CAP_3D                 0x00004000
#define SVGA_CAP_EXTENDED_FIFO      0x00008000
#define SVGA_CAP_MULTIMON           0x00010000
#define SVGA_CAP_PITCHLOCK          0x00020000
#define SVGA_CAP_IRQMASK            0x00040000
#define SVGA_CAP_DISPLAY_TOPOLOGY   0x00080000


/*
 *  Raster op codes (same encoding as X) used by FIFO drivers.
 */

#define SVGA_ROP_CLEAR          0x00     /* 0 */
#define SVGA_ROP_AND            0x01     /* src AND dst */
#define SVGA_ROP_AND_REVERSE    0x02     /* src AND NOT dst */
#define SVGA_ROP_COPY           0x03     /* src */
#define SVGA_ROP_AND_INVERTED   0x04     /* NOT src AND dst */
#define SVGA_ROP_NOOP           0x05     /* dst */
#define SVGA_ROP_XOR            0x06     /* src XOR dst */
#define SVGA_ROP_OR             0x07     /* src OR dst */
#define SVGA_ROP_NOR            0x08     /* NOT src AND NOT dst */
#define SVGA_ROP_EQUIV          0x09     /* NOT src XOR dst */
#define SVGA_ROP_INVERT         0x0a     /* NOT dst */
#define SVGA_ROP_OR_REVERSE     0x0b     /* src OR NOT dst */
#define SVGA_ROP_COPY_INVERTED  0x0c     /* NOT src */
#define SVGA_ROP_OR_INVERTED    0x0d     /* NOT src OR dst */
#define SVGA_ROP_NAND           0x0e     /* NOT src OR NOT dst */
#define SVGA_ROP_SET            0x0f     /* 1 */
#define SVGA_ROP_UNSUPPORTED    0x10

#define SVGA_NUM_SUPPORTED_ROPS   16
#define SVGA_ROP_ALL            (MASK(SVGA_NUM_SUPPORTED_ROPS))
#define SVGA_IS_VALID_ROP(rop)  (rop < SVGA_NUM_SUPPORTED_ROPS)

#define SVGA_INVALID_DISPLAY_ID ((uint32)-1)

/*
 *  Ops
 *  For each pixel, the four channels of the image are computed with: 
 *
 *	C = Ca * Fa + Cb * Fb
 *
 *  where C, Ca, Cb are the values of the respective channels and Fa 
 *  and Fb come from the following table: 
 *
 *	BlendOp		Fa			Fb
 *	------------------------------------------
 *	Clear		0			0
 *	Src		1			0
 *	Dst		0			1
 *	Over		1			1-Aa
 *	OverReverse	1-Ab			1
 *	In		Ab			0
 *	InReverse	0			Aa
 *	Out		1-Ab			0
 *	OutReverse	0			1-Aa
 *	Atop		Ab			1-Aa
 *	AtopReverse	1-Ab			Aa
 *	Xor		1-Ab			1-Aa
 *	Add		1			1
 *	Saturate	min(1,(1-Ab)/Aa)	1
 *
 *  Flags
 *  You can use the following flags to achieve additional affects:
 * 
 *      Flag                    Effect
 *	------------------------------------------
 *      ConstantSourceAlpha     Ca = Ca * Param0
 *      ConstantDestAlpha       Cb = Cb * Param1
 *
 *  Flag effects resolve before the op.  For example
 *  BlendOp == Add && Flags == ConstantSourceAlpha |
 *  ConstantDestAlpha results in:
 *
 *       C = (Ca * Param0) + (Cb * Param1)
 */

#define SVGA_BLENDOP_CLEAR                      0
#define SVGA_BLENDOP_SRC                        1
#define SVGA_BLENDOP_DST                        2
#define SVGA_BLENDOP_OVER                       3
#define SVGA_BLENDOP_OVER_REVERSE               4
#define SVGA_BLENDOP_IN                         5
#define SVGA_BLENDOP_IN_REVERSE                 6
#define SVGA_BLENDOP_OUT                        7
#define SVGA_BLENDOP_OUT_REVERSE                8
#define SVGA_BLENDOP_ATOP                       9 
#define SVGA_BLENDOP_ATOP_REVERSE               10
#define SVGA_BLENDOP_XOR                        11
#define SVGA_BLENDOP_ADD                        12
#define SVGA_BLENDOP_SATURATE                   13

#define SVGA_NUM_BLENDOPS                       14
#define SVGA_IS_VALID_BLENDOP(op)               (op >= 0 && op < SVGA_NUM_BLENDOPS)

#define SVGA_BLENDFLAG_CONSTANT_SOURCE_ALPHA    0x01
#define SVGA_BLENDFLAG_CONSTANT_DEST_ALPHA      0x02
#define SVGA_NUM_BLENDFLAGS                     2
#define SVGA_BLENDFLAG_ALL                      (MASK(SVGA_NUM_BLENDFLAGS))
#define SVGA_IS_VALID_BLENDFLAG(flag)           ((flag & ~SVGA_BLENDFLAG_ALL) == 0)


/*
 * FIFO register indices.
 *
 * The FIFO is a chunk of device memory mapped into guest physmem.  It
 * is always treated as 32-bit words.
 *
 * The guest driver gets to decide how to partition it between
 * - FIFO registers (there are always at least 4, specifying where the
 *   following data area is and how much data it contains; there may be
 *   more registers following these, depending on the FIFO protocol
 *   version in use)
 * - FIFO data, written by the guest and slurped out by the VMX.
 * These indices are 32-bit word offsets into the FIFO.
 */

enum {
   /*
    * Block 1 (basic registers): The originally defined FIFO registers.
    * These exist and are valid for all versions of the FIFO protocol.
    */

   SVGA_FIFO_MIN = 0,
   SVGA_FIFO_MAX,       /* The distance from MIN to MAX must be at least 10K */
   SVGA_FIFO_NEXT_CMD,
   SVGA_FIFO_STOP,

   /*
    * Block 2 (extended registers): Mandatory registers for the extended
    * FIFO.  These exist if the SVGA caps register includes
    * SVGA_CAP_EXTENDED_FIFO; some of them are valid only if their
    * associated capability bit is enabled.
    *
    * Note that when originally defined, SVGA_CAP_EXTENDED_FIFO implied
    * support only for (FIFO registers) CAPABILITIES, FLAGS, and FENCE.
    * This means that the guest has to test individually (in most cases
    * using FIFO caps) for the presence of registers after this; the VMX
    * can define "extended FIFO" to mean whatever it wants, and currently
    * won't enable it unless there's room for that set and much more.
    */

   SVGA_FIFO_CAPABILITIES = 4,
   SVGA_FIFO_FLAGS,
   /* Valid with SVGA_FIFO_CAP_FENCE: */
   SVGA_FIFO_FENCE,

   /*
    * Block 3a (optional extended registers): Additional registers for the
    * extended FIFO, whose presence isn't actually implied by
    * SVGA_CAP_EXTENDED_FIFO; these exist if SVGA_FIFO_MIN is high enough to
    * leave room for them.
    *
    * These in block 3a, the VMX currently considers mandatory for the
    * extended FIFO.
    */
   
   /* Valid if exists (i.e. if extended FIFO enabled): */
   SVGA_FIFO_3D_HWVERSION,       /* See SVGA3dHardwareVersion in svga3d_reg.h */
   /* Valid with SVGA_FIFO_CAP_PITCHLOCK: */
   SVGA_FIFO_PITCHLOCK,
   /* Valid with SVGA_FIFO_CAP_CURSOR_BYPASS_3: */
   SVGA_FIFO_CURSOR_ON,          /* Cursor bypass 3 show/hide register */
   SVGA_FIFO_CURSOR_X,           /* Cursor bypass 3 x register */
   SVGA_FIFO_CURSOR_Y,           /* Cursor bypass 3 y register */
   SVGA_FIFO_CURSOR_COUNT,       /* Incremented when any of the other 3 change */
   SVGA_FIFO_CURSOR_LAST_UPDATED,/* Last time the host updated the cursor */
   /* Valid with SVGA_FIFO_CAP_RESERVE: */
   SVGA_FIFO_RESERVED,           /* Bytes past NEXT_CMD with real contents */
   /*
    * XXX: The gap here, up until SVGA_FIFO_3D_CAPS, can be used for new
    * registers, but this must be done carefully and with judicious use of
    * capability bits, since comparisons based on SVGA_FIFO_MIN aren't
    * enough to tell you whether the register exists: we've shipped drivers
    * and products that used SVGA_FIFO_3D_CAPS but didn't know about some of
    * the earlier ones.  The actual order of introduction was:
    * - PITCHLOCK
    * - 3D_CAPS
    * - CURSOR_* (cursor bypass 3)
    * - RESERVED
    * So, code that wants to know whether it can use any of the
    * aforementioned registers, or anything else added after PITCHLOCK and
    * before 3D_CAPS, needs to reason about something other than
    * SVGA_FIFO_MIN.
    */
   /*
    * 3D caps block space; valid with 3D hardware version >=
    * SVGA3D_HWVERSION_WS6_B1.
    */
   SVGA_FIFO_3D_CAPS      = 32,
   SVGA_FIFO_3D_CAPS_LAST = 32 + 255,

   /*
    * End of VMX's current definition of "extended-FIFO registers".
    * Registers before here are always enabled/disabled as a block; either
    * the extended FIFO is enabled and includes all preceding registers, or
    * it's disabled entirely.
    *
    * Block 3b (truly optional extended registers): Additional registers for
    * the extended FIFO, which the VMX already knows how to enable and
    * disable with correct granularity.
    *
    * Registers after here exist if and only if the guest SVGA driver
    * sets SVGA_FIFO_MIN high enough to leave room for them.
    */

   /* Valid if register exists: */
   SVGA_FIFO_GUEST_3D_HWVERSION, /* Guest driver's 3D version */
   SVGA_FIFO_FENCE_GOAL,         /* Matching target for SVGA_IRQFLAG_FENCE_GOAL */
   SVGA_FIFO_BUSY,               /* See "FIFO Synchronization Registers" */

   /*
    * Always keep this last.  This defines the maximum number of
    * registers we know about.  At power-on, this value is placed in
    * the SVGA_REG_MEM_REGS register, and we expect the guest driver
    * to allocate this much space in FIFO memory for registers.
    */
    SVGA_FIFO_NUM_REGS
};


/*
 * Definition of registers included in extended FIFO support.
 *
 * The guest SVGA driver gets to allocate the FIFO between registers
 * and data.  It must always allocate at least 4 registers, but old
 * drivers stopped there.
 *
 * The VMX will enable extended FIFO support if and only if the guest
 * left enough room for all registers defined as part of the mandatory
 * set for the extended FIFO.
 *
 * Note that the guest drivers typically allocate the FIFO only at
 * initialization time, not at mode switches, so it's likely that the
 * number of FIFO registers won't change without a reboot.
 *
 * All registers less than this value are guaranteed to be present if
 * svgaUser->fifo.extended is set. Any later registers must be tested
 * individually for compatibility at each use (in the VMX).
 *
 * This value is used only by the VMX, so it can change without
 * affecting driver compatibility; keep it that way?
 */
#define SVGA_FIFO_EXTENDED_MANDATORY_REGS  (SVGA_FIFO_3D_CAPS_LAST + 1)


/*
 * FIFO Synchronization Registers
 *
 *  This explains the relationship between the various FIFO
 *  sync-related registers in IOSpace and in FIFO space.
 *
 *  SVGA_REG_SYNC --
 *
 *       The SYNC register can be used in two different ways by the guest:
 *
 *         1. If the guest wishes to fully sync (drain) the FIFO,
 *            it will write once to SYNC then poll on the BUSY
 *            register. The FIFO is sync'ed once BUSY is zero.
 *
 *         2. If the guest wants to asynchronously wake up the host,
 *            it will write once to SYNC without polling on BUSY.
 *            Ideally it will do this after some new commands have
 *            been placed in the FIFO, and after reading a zero
 *            from SVGA_FIFO_BUSY.
 *
 *       (1) is the original behaviour that SYNC was designed to
 *       support.  Originally, a write to SYNC would implicitly
 *       trigger a read from BUSY. This causes us to synchronously
 *       process the FIFO.
 *
 *       This behaviour has since been changed so that writing SYNC
 *       will *not* implicitly cause a read from BUSY. Instead, it
 *       makes a channel call which asynchronously wakes up the MKS
 *       thread.
 *
 *       New guests can use this new behaviour to implement (2)
 *       efficiently. This lets guests get the host's attention
 *       without waiting for the MKS to poll, which gives us much
 *       better CPU utilization on SMP hosts and on UP hosts while
 *       we're blocked on the host GPU.
 *
 *       Old guests shouldn't notice the behaviour change. SYNC was
 *       never guaranteed to process the entire FIFO, since it was
 *       bounded to a particular number of CPU cycles. Old guests will
 *       still loop on the BUSY register until the FIFO is empty.
 *
 *       Writing to SYNC currently has the following side-effects:
 *
 *         - Sets SVGA_REG_BUSY to TRUE (in the monitor)
 *         - Asynchronously wakes up the MKS thread for FIFO processing
 *         - The value written to SYNC is recorded as a "reason", for
 *           stats purposes.
 *
 *       If SVGA_FIFO_BUSY is available, drivers are advised to only
 *       write to SYNC if SVGA_FIFO_BUSY is FALSE. Drivers should set
 *       SVGA_FIFO_BUSY to TRUE after writing to SYNC. The MKS will
 *       eventually set SVGA_FIFO_BUSY on its own, but this approach
 *       lets the driver avoid sending multiple asynchronous wakeup
 *       messages to the MKS thread.
 *
 *  SVGA_REG_BUSY --
 *
 *       This register is set to TRUE when SVGA_REG_SYNC is written,
 *       and it reads as FALSE when the FIFO has been completely
 *       drained.
 *
 *       Every read from this register causes us to synchronously
 *       process FIFO commands. There is no guarantee as to how many
 *       commands each read will process.
 *
 *       CPU time spent processing FIFO commands will be billed to
 *       the guest.
 *
 *       New drivers should avoid using this register unless they
 *       need to guarantee that the FIFO is completely drained. It
 *       is overkill for performing a sync-to-fence. Older drivers
 *       will use this register for any type of synchronization.
 *
 *  SVGA_FIFO_BUSY --
 *
 *       This register is a fast way for the guest driver to check
 *       whether the FIFO is already being processed. It reads and
 *       writes at normal RAM speeds, with no monitor intervention.
 *
 *       If this register reads as TRUE, the host is guaranteeing that
 *       any new commands written into the FIFO will be noticed before
 *       the MKS goes back to sleep.
 *
 *       If this register reads as FALSE, no such guarantee can be
 *       made.
 *
 *       The guest should use this register to quickly determine
 *       whether or not it needs to wake up the host. If the guest
 *       just wrote a command or group of commands that it would like
 *       the host to begin processing, it should:
 *
 *         1. Read SVGA_FIFO_BUSY. If it reads as TRUE, no further
 *            action is necessary.
 *
 *         2. Write TRUE to SVGA_FIFO_BUSY. This informs future guest
 *            code that we've already sent a SYNC to the host and we
 *            don't need to send a duplicate.
 *
 *         3. Write a reason to SVGA_REG_SYNC. This will send an
 *            asynchronous wakeup to the MKS thread.
 */


/*
 * FIFO Capabilities
 *
 *      Fence -- Fence register and command are supported
 *      Accel Front -- Front buffer only commands are supported
 *      Pitch Lock -- Pitch lock register is supported
 *      Video -- SVGA Video overlay units are supported
 *      Escape -- Escape command is supported
 */

#define SVGA_FIFO_CAP_NONE                  0
#define SVGA_FIFO_CAP_FENCE             (1<<0)
#define SVGA_FIFO_CAP_ACCELFRONT        (1<<1)
#define SVGA_FIFO_CAP_PITCHLOCK         (1<<2)
#define SVGA_FIFO_CAP_VIDEO             (1<<3)
#define SVGA_FIFO_CAP_CURSOR_BYPASS_3   (1<<4)
#define SVGA_FIFO_CAP_ESCAPE            (1<<5)
#define SVGA_FIFO_CAP_RESERVE           (1<<6)


/*
 * FIFO Flags
 *
 *      Accel Front -- Driver should use front buffer only commands
 */

#define SVGA_FIFO_FLAG_NONE                 0
#define SVGA_FIFO_FLAG_ACCELFRONT       (1<<0)
#define SVGA_FIFO_FLAG_RESERVED        (1<<31) /* Internal use only */

/*
 * FIFO reservation sentinel value
 */

#define SVGA_FIFO_RESERVED_UNKNOWN      0xffffffff


/*
 * Video overlay support
 */

#define SVGA_NUM_OVERLAY_UNITS 32


/*
 * Video capabilities that the guest is currently using
 */

#define SVGA_VIDEO_FLAG_COLORKEY        0x0001


/*
 * Offsets for the video overlay registers
 */

enum {
   SVGA_VIDEO_ENABLED = 0,
   SVGA_VIDEO_FLAGS,
   SVGA_VIDEO_DATA_OFFSET,
   SVGA_VIDEO_FORMAT,
   SVGA_VIDEO_COLORKEY,
   SVGA_VIDEO_SIZE,
   SVGA_VIDEO_WIDTH,
   SVGA_VIDEO_HEIGHT,
   SVGA_VIDEO_SRC_X,
   SVGA_VIDEO_SRC_Y,
   SVGA_VIDEO_SRC_WIDTH,
   SVGA_VIDEO_SRC_HEIGHT,
   SVGA_VIDEO_DST_X,
   SVGA_VIDEO_DST_Y,
   SVGA_VIDEO_DST_WIDTH,
   SVGA_VIDEO_DST_HEIGHT,
   SVGA_VIDEO_PITCH_1,
   SVGA_VIDEO_PITCH_2,
   SVGA_VIDEO_PITCH_3,
   SVGA_VIDEO_NUM_REGS
};


/*
 * SVGA Overlay Units
 *
 *      width and height relate to the entire source video frame.
 *      srcX, srcY, srcWidth and srcHeight represent subset of the source
 *      video frame to be displayed.
 */

typedef struct SVGAOverlayUnit {
   uint32 enabled;
   uint32 flags;
   uint32 dataOffset;
   uint32 format;
   uint32 colorKey;
   uint32 size;
   uint32 width;
   uint32 height;
   uint32 srcX;
   uint32 srcY;
   uint32 srcWidth;
   uint32 srcHeight;
   uint32 dstX;
   uint32 dstY;
   uint32 dstWidth;
   uint32 dstHeight;
   uint32 pitches[3];
} SVGAOverlayUnit;


/*
 *  Drawing object ID's, in the range 0 to SVGA_MAX_ID
 */

#define	 SVGA_MAX_ID	      499

/*
 *  Macros to compute variable length items (sizes in 32-bit words, except
 *  for SVGA_GLYPH_SCANLINE_SIZE, which is in bytes).
 */

#define SVGA_BITMAP_SIZE(w,h) ((((w)+31) >> 5) * (h))
#define SVGA_BITMAP_SCANLINE_SIZE(w) (( (w)+31 ) >> 5)
#define SVGA_PIXMAP_SIZE(w,h,bpp) ((( ((w)*(bpp))+31 ) >> 5) * (h))
#define SVGA_PIXMAP_SCANLINE_SIZE(w,bpp) (( ((w)*(bpp))+31 ) >> 5)
#define SVGA_GLYPH_SIZE(w,h) ((((((w) + 7) >> 3) * (h)) + 3) >> 2)
#define SVGA_GLYPH_SCANLINE_SIZE(w) (((w) + 7) >> 3)
#define SVGA_ESCAPE_SIZE(s) (((s) + 3) >> 2)

/*
 *  Increment from one scanline to the next of a bitmap or pixmap
 */
#define SVGA_BITMAP_INCREMENT(w) ((( (w)+31 ) >> 5) * sizeof (uint32))
#define SVGA_PIXMAP_INCREMENT(w,bpp) ((( ((w)*(bpp))+31 ) >> 5) * sizeof (uint32))

/*
 *  Transparent color for DRAW_GLYPH_CLIPPED
 */
#define SVGA_COLOR_TRANSPARENT (~0)

/*
 *  Commands in the command FIFO
 */

#define	 SVGA_CMD_INVALID_CMD		   0
	 /* FIFO layout:
            <nothing> (well, undefined) */

#define	 SVGA_CMD_UPDATE		   1
	 /* FIFO layout:
	    X, Y, Width, Height */

#define	 SVGA_CMD_RECT_FILL		   2
	 /* FIFO layout:
	    Color, X, Y, Width, Height */

#define	 SVGA_CMD_RECT_COPY		   3
	 /* FIFO layout:
	    Source X, Source Y, Dest X, Dest Y, Width, Height */

#define	 SVGA_CMD_DEFINE_BITMAP		   4
	 /* FIFO layout:
	    Pixmap ID, Width, Height, <scanlines> */

#define	 SVGA_CMD_DEFINE_BITMAP_SCANLINE   5
	 /* FIFO layout:
	    Pixmap ID, Width, Height, Line #, scanline */

#define	 SVGA_CMD_DEFINE_PIXMAP		   6
	 /* FIFO layout:
	    Pixmap ID, Width, Height, Depth, <scanlines> */

#define	 SVGA_CMD_DEFINE_PIXMAP_SCANLINE   7
	 /* FIFO layout:
	    Pixmap ID, Width, Height, Depth, Line #, scanline */

#define	 SVGA_CMD_RECT_BITMAP_FILL	   8
	 /* FIFO layout:
	    Bitmap ID, X, Y, Width, Height, Foreground, Background */

#define	 SVGA_CMD_RECT_PIXMAP_FILL	   9
	 /* FIFO layout:
	    Pixmap ID, X, Y, Width, Height */

#define	 SVGA_CMD_RECT_BITMAP_COPY	  10
	 /* FIFO layout:
	    Bitmap ID, Source X, Source Y, Dest X, Dest Y,
	    Width, Height, Foreground, Background */

#define	 SVGA_CMD_RECT_PIXMAP_COPY	  11
	 /* FIFO layout:
	    Pixmap ID, Source X, Source Y, Dest X, Dest Y, Width, Height */

#define	 SVGA_CMD_FREE_OBJECT		  12
	 /* FIFO layout:
	    Object (pixmap, bitmap, ...) ID */

#define	 SVGA_CMD_RECT_ROP_FILL           13
         /* FIFO layout:
            Color, X, Y, Width, Height, ROP */

#define	 SVGA_CMD_RECT_ROP_COPY           14
         /* FIFO layout:
            Source X, Source Y, Dest X, Dest Y, Width, Height, ROP */

#define	 SVGA_CMD_RECT_ROP_BITMAP_FILL    15
         /* FIFO layout:
            ID, X, Y, Width, Height, Foreground, Background, ROP */

#define	 SVGA_CMD_RECT_ROP_PIXMAP_FILL    16
         /* FIFO layout:
            ID, X, Y, Width, Height, ROP */

#define	 SVGA_CMD_RECT_ROP_BITMAP_COPY    17
         /* FIFO layout:
            ID, Source X, Source Y,
            Dest X, Dest Y, Width, Height, Foreground, Background, ROP */

#define	 SVGA_CMD_RECT_ROP_PIXMAP_COPY    18
         /* FIFO layout:
            ID, Source X, Source Y, Dest X, Dest Y, Width, Height, ROP */

#define	SVGA_CMD_DEFINE_CURSOR		  19
	/* FIFO layout:
	   ID, Hotspot X, Hotspot Y, Width, Height,
	   Depth for AND mask, Depth for XOR mask,
	   <scanlines for AND mask>, <scanlines for XOR mask> */

#define	SVGA_CMD_DISPLAY_CURSOR		  20
	/* FIFO layout:
	   ID, On/Off (1 or 0) */

#define	SVGA_CMD_MOVE_CURSOR		  21
	/* FIFO layout:
	   X, Y */

#define SVGA_CMD_DEFINE_ALPHA_CURSOR      22
	/* FIFO layout:
	   ID, Hotspot X, Hotspot Y, Width, Height,
	   <scanlines> */

#define SVGA_CMD_DRAW_GLYPH               23
	/* FIFO layout:
	   X, Y, W, H, FGCOLOR, <stencil buffer> */

#define SVGA_CMD_DRAW_GLYPH_CLIPPED       24
	/* FIFO layout:
	   X, Y, W, H, FGCOLOR, BGCOLOR, <cliprect>, <stencil buffer>
           Transparent color expands are done by setting BGCOLOR to ~0 */

#define	SVGA_CMD_UPDATE_VERBOSE	          25
        /* FIFO layout:
	   X, Y, Width, Height, Reason */

#define SVGA_CMD_SURFACE_FILL             26
        /* FIFO layout:
	   color, dstSurfaceOffset, x, y, w, h, rop */

#define SVGA_CMD_SURFACE_COPY             27
        /* FIFO layout:
	   srcSurfaceOffset, dstSurfaceOffset, srcX, srcY,
           destX, destY, w, h, rop */

#define SVGA_CMD_SURFACE_ALPHA_BLEND      28
        /* FIFO layout:
	   srcSurfaceOffset, dstSurfaceOffset, srcX, srcY,
           destX, destY, w, h, op (SVGA_BLENDOP*), flags (SVGA_BLENDFLAGS*), 
           param1, param2 */

#define	 SVGA_CMD_FRONT_ROP_FILL          29
         /* FIFO layout:
            Color, X, Y, Width, Height, ROP */

#define	 SVGA_CMD_FENCE                   30
         /* FIFO layout:
            Fence value */

#define SVGA_CMD_VIDEO_PLAY_OBSOLETE      31
         /* Obsolete; do not use. */
 
#define SVGA_CMD_VIDEO_END_OBSOLETE       32
         /* Obsolete; do not use. */

#define SVGA_CMD_ESCAPE                   33
         /* FIFO layout:
            Namespace ID, size(bytes), data */

#define	SVGA_CMD_MAX                      34

#define SVGA_CMD_MAX_ARGS                 64

/*
 * Location and size of SVGA frame buffer and the FIFO.
 */
#define SVGA_VRAM_MIN_SIZE   (4 * 640 * 480)  /* bytes */
#define SVGA_VRAM_MAX_SIZE   (128 * 1024 * 1024)

#define SVGA_VRAM_SIZE_WS       (16 * 1024 * 1024) /* 16 MB */
#define SVGA_MEM_SIZE_WS        (2  * 1024 * 1024) /* 2  MB */
#define SVGA_VRAM_SIZE_SERVER   (4  * 1024 * 1024) /* 4  MB */
#define SVGA_MEM_SIZE_SERVER    (256 * 1024)       /* 256 KB */

#if /* defined(VMX86_WGS) || */ defined(VMX86_SERVER)
#define SVGA_VRAM_SIZE         SVGA_VRAM_SIZE_SERVER
#define SVGA_MEM_SIZE          SVGA_MEM_SIZE_SERVER
#else
#define SVGA_VRAM_SIZE         SVGA_VRAM_SIZE_WS
#define SVGA_MEM_SIZE          SVGA_MEM_SIZE_WS
#endif

/*
 * SVGA_FB_START is the default starting address of the SVGA frame
 * buffer in the guest's physical address space.
 * SVGA_FB_START_BIGMEM is the starting address of the SVGA frame
 * buffer for VMs that have a large amount of physical memory.
 *
 * The address of SVGA_FB_START is set to 2GB - (SVGA_FB_MAX_SIZE + SVGA_MEM_SIZE), 
 * thus the SVGA frame buffer sits at [SVGA_FB_START .. 2GB-1] in the
 * physical address space.  Our older SVGA drivers for NT treat the
 * address of the frame buffer as a signed integer.  For backwards
 * compatibility, we keep the default location of the frame buffer
 * at under 2GB in the address space.  This restricts VMs to have "only"
 * up to ~2031MB (i.e., up to SVGA_FB_START) of physical memory.
 *
 * For VMs that want more memory than the ~2031MB, we place the SVGA
 * frame buffer at SVGA_FB_START_BIGMEM.  This allows VMs to have up
 * to 3584MB, at least as far as the SVGA frame buffer is concerned
 * (note that there may be other issues that limit the VM memory
 * size).  PCI devices use high memory addresses, so we have to put
 * SVGA_FB_START_BIGMEM low enough so that it doesn't overlap with any
 * of these devices.  Placing SVGA_FB_START_BIGMEM at 0xE0000000
 * should leave plenty of room for the PCI devices.
 *
 * NOTE: All of that is only true for the 0710 chipset.  As of the 0405
 * chipset, the framebuffer start is determined solely based on the value
 * the guest BIOS or OS programs into the PCI base address registers.
 */
#define SVGA_FB_LEGACY_START		0x7EFC0000
#define SVGA_FB_LEGACY_START_BIGMEM	0xE0000000

#endif
