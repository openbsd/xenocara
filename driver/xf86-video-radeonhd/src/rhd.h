/*
 * Copyright 2007 - 2009  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2007 - 2009  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007 - 2009  Egbert Eich   <eich@novell.com>
 * Copyright 2007, 2008  Advanced Micro Devices, Inc.
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

#include <compiler.h>

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
    RHD_RS600,
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
    /* R600 second batch - RV670 came into existence after RV6x0 and M7x */
    RHD_RV670,
    RHD_M88,
    RHD_R680,
    RHD_RV620,
    RHD_M82,
    RHD_RV635,
    RHD_M86,
    RHD_RS780,
    RHD_RS880,
    RHD_RV770,
    /* R700 */
    RHD_R700,
    RHD_M98,
    RHD_RV730,
    RHD_M96,
    RHD_RV710,
    RHD_M92,
    RHD_M93,
    RHD_M97,
    RHD_RV790,
    RHD_RV740,
    RHD_CHIP_END
};

enum RHD_HPD_USAGE {
    RHD_HPD_USAGE_AUTO = 0,
    RHD_HPD_USAGE_OFF,
    RHD_HPD_USAGE_NORMAL,
    RHD_HPD_USAGE_SWAP,
    RHD_HPD_USAGE_AUTO_SWAP,
    RHD_HPD_USAGE_AUTO_OFF
};

enum RHD_TV_MODE {
    RHD_TV_NONE = 0,
    RHD_TV_NTSC = 1,
    RHD_TV_NTSCJ = 1 << 2,
    RHD_TV_PAL = 1 << 3,
    RHD_TV_PALM = 1 << 4,
    RHD_TV_PALCN = 1 << 5,
    RHD_TV_PALN = 1 << 6,
    RHD_TV_PAL60 = 1 << 7,
    RHD_TV_SECAM = 1 << 8,
    RHD_TV_CV = 1 << 9
};

enum rhdPropertyAction {
    rhdPropertyCheck,
    rhdPropertyGet,
    rhdPropertySet,
    rhdPropertyCommit
};

union rhdPropertyData
{
    CARD32 integer;
    char *string;
    Bool Bool;
};

#define RHD_BACKLIGHT_PROPERTY_MAX 255

#define RHD_CONNECTORS_MAX 6

/* Just define where which PCI BAR lives for now. Will deal with different
 * locations as soon as cards with a different BAR layout arrives.
 */
#define RHD_FB_BAR   0
#define RHD_MMIO_BAR 2

/* More realistic powermanagement */
#define RHD_POWER_ON       0
#define RHD_POWER_RESET    1   /* off temporarily */
#define RHD_POWER_SHUTDOWN 2   /* long term shutdown */
#define RHD_POWER_UNKNOWN  3   /* initial state */

#define RHD_VBIOS_SIZE 0x10000

#ifndef XSERVER_LIBPCIACCESS
# define PCI_BUS(x)	((x)->bus)
# define PCI_DEV(x)	((x)->device)
# define PCI_FUNC(x)	((x)->func)
#else
# define PCI_BUS(x)	(((x)->domain << 8) | (x)->bus)
# define PCI_DEV(x)	((x)->dev)
# define PCI_FUNC(x)	((x)->func)
typedef struct pci_device *pciVideoPtr;
#endif

#ifndef NO_ASSERT
enum debugFlags {
    VGA_SETUP,
    MC_SETUP
};
#endif

enum rhdCardType {
    RHD_CARD_NONE,
    RHD_CARD_AGP,
    RHD_CARD_PCIE
};

enum {
    RHD_PCI_CAPID_AGP    = 0x02,
    RHD_PCI_CAPID_PCIE   = 0x10
};

typedef struct BIOSScratchOutputPrivate rhdOutputDriverPrivate;
typedef struct _rhdI2CRec *rhdI2CPtr;
typedef struct _atomBiosHandle *atomBiosHandlePtr;
typedef struct _rhdShadowRec *rhdShadowPtr;

typedef struct RHDOpt {
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

/* Some more intelligent handling of chosing which acceleration to use */
enum AccelMethod {
    RHD_ACCEL_NONE = 0, /* ultra slow, but might be desired for debugging. */
    RHD_ACCEL_FORCE_SHADOWFB = 1, /* shadowfb even with dri enabled. Known to have damage issues. */
    RHD_ACCEL_SHADOWFB = 2, /* cache in main ram. */
    RHD_ACCEL_XAA = 3, /* "old" X acceleration architecture. */
    RHD_ACCEL_EXA = 4, /* not done yet. */
    RHD_ACCEL_DEFAULT = 5 /* keep as highest. */
};

typedef struct RHDRec {
    int                 scrnIndex;

    enum RHD_CHIPSETS   ChipSet;
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device   *PciInfo;
    struct pci_device   *NBPciInfo;
#else
    pciVideoRec         *PciInfo;
    PCITAG              PciTag;
    PCITAG		NBPciTag;
#endif
    unsigned int	PciDeviceID;
    enum rhdCardType	cardType;
    int			entityIndex;
    EntityInfoPtr       pEnt;
    struct rhdCard      *Card;
    OptionInfoPtr       Options;
    enum AccelMethod    AccelMethod;
    RHDOpt              swCursor;
    RHDOpt		shadowFB;
    RHDOpt		forceReduced;
    RHDOpt              forceDPI;
    RHDOpt		noRandr;
    RHDOpt		rrUseXF86Edid;
    RHDOpt		rrOutputOrder;
    RHDOpt		useDRI;
    RHDOpt		tvModeName;
    RHDOpt		scaleTypeOpt;
    RHDOpt		unverifiedFeatures;
    RHDOpt		audio;
    RHDOpt		audioWorkaround;
    RHDOpt		hdmi;
    RHDOpt		coherent;
    RHDOpt              lowPowerMode;
    RHDOpt              lowPowerModeEngineClock;
    RHDOpt              lowPowerModeMemoryClock;
    enum RHD_HPD_USAGE	hpdUsage;
    unsigned int        FbMapSize;
    pointer             FbBase;   /* map base of fb   */
    unsigned int        FbPhysAddress; /* card PCI BAR address of FB */
    unsigned int        FbIntAddress; /* card internal address of FB */
    CARD32              FbIntSize; /* card internal FB aperture size */
    unsigned int        FbPCIAddress; /* physical address of FB */

    Bool		directRenderingEnabled;

    /* Some simplistic memory handling */
#define ALIGN(x,align)	(((x)+(align)-1)&~((align)-1))
#define RHD_FB_ALIGNMENT 0x1000
    /* Use this macro to always chew up 4096byte aligned pieces. */
#define RHD_FB_CHUNK(x)     ALIGN((x), RHD_FB_ALIGNMENT)
    unsigned int        FbFreeStart;
    unsigned int        FbFreeSize;

    /* visible part of the framebuffer */
    unsigned int        FbScanoutStart;
    unsigned int        FbScanoutSize;

    /* for 2d acceleration: pixmapcache and such */
    RHDOpt              OffscreenOption;
    unsigned int        FbOffscreenStart;
    unsigned int        FbOffscreenSize;

    unsigned int        MMIOMapSize;
    pointer             MMIOBase; /* map base of mmio */
    unsigned int        MMIOPCIAddress; /* physical address of mmio */

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
    struct rhdAudio    *Audio;

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
    enum RHD_TV_MODE   tvMode;
    rhdShadowPtr       shadowPtr;

    struct RhdCS       *CS;

    struct _XAAInfoRec *XAAInfo;
#ifdef USE_EXA
    struct _ExaDriver  *EXAInfo;
#endif
    void               *TwoDPrivate;

    /* For EXA Render and Textured Video */
    void               *ThreeDPrivate;

    /* RandR compatibility layer */
    struct rhdRandr    *randr;
    /* log verbosity - store this for convenience */
    int			verbosity;

    /* DRI */
    struct rhdDri      *dri;

    /* BIOS Scratch registers */
    struct rhdBiosScratchRegisters *BIOSScratch;

    /* AtomBIOS usage */
    RHDOpt		UseAtomBIOS;
    CARD32		UseAtomFlags;

    struct rhdPm       *Pm;

    struct rhdOutput *DigEncoderOutput[2];
# define RHD_CHECKDEBUGFLAG(rhdPtr, FLAG) (rhdPtr->DebugFlags & (1 << FLAG))
#ifndef NO_ASSERT
# define RHD_SETDEBUGFLAG(rhdPtr, FLAG) (rhdPtr->DebugFlags |= (1 << FLAG))
# define RHD_UNSETDEBUGFLAG(rhdPtr, FLAG) (rhdPtr->DebugFlags &= ~((CARD32)1 << FLAG))
    CARD32 DebugFlags;
#endif
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


enum atomSubSystem {
    atomUsageCrtc,
    atomUsagePLL,
    atomUsageOutput,
    atomUsageAny
};

enum rhdOptStatus {
    RHD_OPTION_NOT_SET,
    RHD_OPTION_DEFAULT,
    RHD_OPTION_OFF,
    RHD_OPTION_ON
};

struct rhdPowerState {
    /* All entries: 0 means unknown / unspecified / dontchange */
    /* Clocks in kHz, Voltage in mV */
    CARD32 EngineClock;
    CARD32 MemoryClock;
    CARD32 VDDCVoltage;
};

/* rhd_driver.c */
/* Some handy functions that makes life so much more readable */
extern unsigned int RHDReadPCIBios(RHDPtr rhdPtr, unsigned char **prt);
extern Bool RHDScalePolicy(struct rhdMonitor *Monitor, struct rhdConnector *Connector);
extern void RHDPrepareMode(RHDPtr rhdPtr);
extern Bool RHDUseAtom(RHDPtr rhdPtr, enum RHD_CHIPSETS *BlackList, enum atomSubSystem subsys);

#define RHDRegRead(ptr, offset) MMIO_IN32(RHDPTRI(ptr)->MMIOBase, offset)
#define RHDRegWrite(ptr, offset, value) MMIO_OUT32(RHDPTRI(ptr)->MMIOBase, offset, value)
#define RHDRegMask(ptr, offset, value, mask)	\
do {						\
    CARD32 tmp;					\
    tmp = RHDRegRead((ptr), (offset));		\
    tmp &= ~(mask);				\
    tmp |= ((value) & (mask));			\
    RHDRegWrite((ptr), (offset), tmp);		\
} while(0)

extern CARD32 _RHDReadMC(int scrnIndex, CARD32 addr);
#define RHDReadMC(ptr,addr) _RHDReadMC((ptr)->scrnIndex,(addr))
extern void _RHDWriteMC(int scrnIndex, CARD32 addr, CARD32 data);
#define RHDWriteMC(ptr,addr,value) _RHDWriteMC((ptr)->scrnIndex,(addr),(value))
extern CARD32 _RHDReadPLL(int scrnIndex, CARD16 offset);
#define RHDReadPLL(ptr, off) _RHDReadPLL((ptr)->scrnIndex,(off))
extern void _RHDWritePLL(int scrnIndex, CARD16 offset, CARD32 data);
#define RHDWritePLL(ptr, off, value) _RHDWritePLL((ptr)->scrnIndex,(off),(value))
extern unsigned int RHDAllocFb(RHDPtr rhdPtr, unsigned int size, const char *name);

/* rhd_id.c */
Bool RHDIsIGP(enum RHD_CHIPSETS chipset);

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
enum rhdOptStatus RhdParseBooleanOption(struct RHDOpt *Option, char *Name);
char *RhdAppendString(char *s1, const char *s2);
void RhdAssertFailed(const char *str,
		     const char *file, int line, const char *func) NORETURN;
void RhdAssertFailedFormat(const char *str,  const char *file, int line,
			   const char *func, const char *format, ...) NORETURN;

/* Extra debugging verbosity: decimates gdb usage */

/* __func__ is really nice, but not universal */
#if !defined(__GNUC__) && !defined(C99)
#define __func__ "unknown"
#endif

#ifndef NO_ASSERT
#  define ASSERT(x) do { if (!(x)) RhdAssertFailed \
			 (#x, __FILE__, __LINE__, __func__); } while(0)
#  define ASSERTF(x,f...) do { if (!(x)) RhdAssertFailedFormat \
			    (#x, __FILE__, __LINE__, __func__, ##f); } while(0)
#else
#  define ASSERT(x) ((void)0)
#  define ASSERTF(x,...) ((void)0)
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
CARD32 _RHDRegReadD(int scrnIndex, CARD16 offset);
# define RHDRegReadD(ptr, offset) _RHDRegReadD((ptr)->scrnIndex, (offset))
void _RHDRegWriteD(int scrnIndex, CARD16 offset, CARD32 value);
# define RHDRegWriteD(ptr, offset, value) _RHDRegWriteD((ptr)->scrnIndex, (offset), (value))
void _RHDRegMaskD(int scrnIndex, CARD16 offset, CARD32 value, CARD32 mask);
# define RHDRegMaskD(ptr, offset, value, mask) _RHDRegMaskD((ptr)->scrnIndex, (offset), (value), (mask))
# define DEBUGP(x) {x;}
#else
# define RHDRegReadD(ptr, offset) RHDRegRead(ptr, offset)
# define RHDRegWriteD(ptr, offset, value) RHDRegWrite(ptr, offset, value)
# define RHDRegMaskD(ptr, offset, value, mask) RHDRegMask(ptr, offset, value, mask)
# define DEBUGP(x)
#endif

#endif /* _RHD_H */
