/*
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _RHD_H
# define _RHD_H

# if defined HAVE_CONFIG_H && !defined _CONFIG_H
#  error "config.h missing!"
# endif

#define RHD_MAJOR_VERSION (PACKAGE_VERSION_MAJOR)
#define RHD_MINOR_VERSION (PACKAGE_VERSION_MINOR)
#define RHD_PATCHLEVEL    (PACKAGE_VERSION_PATCHLEVEL)

/* It does not seem to matter how exactly we construct this int for
    + * DriverRec.driverVersion. So... choose 20/10/0 bitshift. */
#define RHD_VERSION            \
    ((RHD_MAJOR_VERSION<<20) | \
     (RHD_MINOR_VERSION<<10) | \
           (RHD_PATCHLEVEL))

#define RHD_NAME "RADEONHD"
#define RHD_DRIVER_NAME "radeonhd"


enum RHD_CHIPSETS {
    RHD_UNKNOWN = 0,
    /* R500 */
    RHD_RV505,
    RHD_RV515,
    RHD_RV516,
    RHD_R520,
    RHD_RV530,
    RHD_RV535,
    RHD_RV550,
    RHD_RV560,
    RHD_RV570,
    RHD_R580,
    /* R500 Mobility */
    RHD_M52,
    RHD_M54,
    RHD_M56,
    RHD_M58,
    RHD_M62,
    RHD_M64,
    RHD_M66,
    RHD_M68,
    RHD_M71,
    /* R500 integrated */
    RHD_RS690,
    RHD_RS740,
    /* R600 */
    RHD_R600,
    RHD_RV610,
    RHD_RV630,
    /* R600 Mobility */
    RHD_M72,
    RHD_M74,
    RHD_M76,
    /* R600 integrated */
    RHD_CHIP_END
};

#define RHD_CONNECTORS_MAX 4

/* Just define where which PCI BAR lives for now. Will deal with different
 * locations as soon as cards with a different BAR layout arrives.
 */
#define RHD_FB_BAR   0
#define RHD_MMIO_BAR 2

/* More realistic powermanagement */
#define RHD_POWER_ON       0
#define RHD_POWER_RESET    1   /* off temporarily */
#define RHD_POWER_SHUTDOWN 2   /* long term shutdown */

#define RHD_VBIOS_SIZE 0x10000

typedef struct _rhdI2CRec *rhdI2CPtr;
typedef struct _atomBiosHandle *atomBiosHandlePtr;
typedef struct _rhdShadowRec *rhdShadowPtr;

typedef struct _RHDopt {
    Bool set;
    union  {
        Bool bool;
        int integer;
        unsigned long uslong;
        double real;
        double freq;
        char *string;
    } val;
} RHDOpt, *RHDOptPtr;

typedef struct RHDRec {
    int                 scrnIndex;

    int                 ChipSet;
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device   *PciInfo;
#else
    pciVideoRec         *PciInfo;
#endif
    PCITAG              PciTag;
    unsigned int	PciDeviceID;
    int			entityIndex;
    struct rhdCard      *Card;
    OptionInfoPtr       Options;
    RHDOpt              noAccel;
    RHDOpt              swCursor;
    RHDOpt		shadowFB;
    RHDOpt		forceReduced;
    RHDOpt		ignoreHpd;
    RHDOpt		noRandr;
    RHDOpt		rrUseXF86Edid;
    RHDOpt		rrOutputOrder;
    unsigned int        FbMapSize;
    pointer             FbBase;   /* map base of fb   */
    unsigned int        FbIntAddress; /* card internal address of FB */

    /* Some simplistic memory handling */
    /* Use this macro to always chew up 4096byte aligned pieces. */
#define RHD_FB_CHUNK(x)     (((x) + 0xFFF) & ~0xFFF) /* align */
    unsigned int        FbFreeStart;
    unsigned int        FbFreeSize;

    unsigned int        MMIOMapSize;
    pointer             MMIOBase; /* map base if mmio */

    struct _xf86CursorInfoRec  *CursorInfo;
    struct rhd_Cursor_Bits     *CursorBits; /* ARGB if NULL */
    CARD32              CursorColor0, CursorColor1;
    CARD32             *CursorImage;

    CloseScreenProcPtr  CloseScreen;

    struct _I2CBusRec	**I2C;  /* I2C bus list */
    atomBiosHandlePtr   atomBIOS; /* handle for AtomBIOS */
    /*
     * BIOS copy - kludge that should go away
     * once we know how to read PCI BIOS on
     * POSTed hardware
     */
    unsigned char*	BIOSCopy;

    struct rhdMC       *MC;  /* Memory Controller */
    struct rhdVGA      *VGA; /* VGA compatibility HW */
    struct rhdCrtc     *Crtc[2];
    struct rhdPLL      *PLLs[2]; /* Pixelclock PLLs */

    struct rhdLUTStore  *LUTStore;
    struct rhdLUT       *LUT[2];

    /* List of output devices:
     * we can go up to 5: DACA, DACB, TMDS, shared LVDS/TMDS, DVO.
     * Will also include displayport when this happens. */
    struct rhdOutput   *Outputs;

    struct rhdConnector *Connector[RHD_CONNECTORS_MAX];
    struct rhdHPD      *HPD; /* Hot plug detect subsystem */

    /* don't ignore the Monitor section of the conf file */
    struct rhdMonitor  *ConfigMonitor;

    rhdShadowPtr	shadowPtr;

    /* RandR compatibility layer */
    struct rhdRandr    *randr;
} RHDRec, *RHDPtr;

#define RHDPTR(p) 	((RHDPtr)((p)->driverPrivate))
#define RHDPTRI(p) 	(RHDPTR(xf86Screens[(p)->scrnIndex]))

#if defined(__GNUC__)
#  define NORETURN __attribute__((noreturn))
#  define CONST    __attribute__((pure))
#else
#  define NORETURN
#  define CONST
#endif


/* rhd_driver.c */
/* Some handy functions that makes life so much more readable */
unsigned int RHDReadPCIBios(RHDPtr rhdPtr, unsigned char **prt);
CARD32 _RHDRegRead(int scrnIndex, CARD16 offset);
#define RHDRegRead(ptr, offset) _RHDRegRead((ptr)->scrnIndex, (offset))
void _RHDRegWrite(int scrnIndex, CARD16 offset, CARD32 value);
#define RHDRegWrite(ptr, offset, value) _RHDRegWrite((ptr)->scrnIndex, (offset), (value))
void _RHDRegMask(int scrnIndex, CARD16 offset, CARD32 value, CARD32 mask);
#define RHDRegMask(ptr, offset, value, mask) _RHDRegMask((ptr)->scrnIndex, (offset), (value), (mask))
CARD32 _RHDReadMC(int scrnIndex, CARD32 addr);
#define RHDReadMC(ptr,addr) _RHDReadMC((ptr)->scrnIndex,(addr))
void _RHDWriteMC(int scrnIndex, CARD32 addr, CARD32 data);
#define RHDWriteMC(ptr,addr,value) _RHDWriteMC((ptr)->scrnIndex,(addr),(value))

/* rhd_helper.c */
void RhdGetOptValBool(const OptionInfoRec *table, int token,
                      RHDOptPtr optp, Bool def);
void RhdGetOptValInteger(const OptionInfoRec *table, int token,
                         RHDOptPtr optp, int def);
void RhdGetOptValULong(const OptionInfoRec *table, int token,
                       RHDOptPtr optp, unsigned long def);
void RhdGetOptValReal(const OptionInfoRec *table, int token,
                      RHDOptPtr optp, double def);
void RhdGetOptValFreq(const OptionInfoRec *table, int token,
                      OptFreqUnits expectedUnits, RHDOptPtr optp, double def);
void RhdGetOptValString(const OptionInfoRec *table, int token,
                        RHDOptPtr optp, char *def);
char *RhdAppendString(char *s1, const char *s2);
void RhdAssertFailed(const char *str,
		     const char *file, int line, const char *func) NORETURN;

/* Extra debugging verbosity: decimates gdb usage */

/* __func__ is really nice, but not universal */
#if !defined(__GNUC__) && !defined(C99)
#define __func__ "unknown"
#endif

#ifndef NO_ASSERT
#  define ASSERT(x) do { if (!(x)) RhdAssertFailed \
			 (#x, __FILE__, __LINE__, __func__); } while(0)
#else
#  define ASSERT(x) ((void)0)
#endif

#define LOG_DEBUG 7
void RHDDebug(int scrnIndex, const char *format, ...);
void RHDDebugCont(const char *format, ...);
void RHDDebugVerb(int scrnIndex, int verb, const char *format, ...);
void RHDDebugContVerb(int verb, const char *format, ...);
#define RHDFUNC(ptr) RHDDebug((ptr)->scrnIndex, "FUNCTION: %s\n", __func__)
#define RHDFUNCI(scrnIndex) RHDDebug(scrnIndex, "FUNCTION: %s\n", __func__)
void RhdDebugDump(int scrnIndex, unsigned char *start, int size);
#ifdef RHD_DEBUG
# define DEBUGP(x) {x;}
#else
# define DEBUGP(x)
#endif

#endif /* _RHD_H */
