/* **********************************************************
 * Copyright (C) 1998-2001 VMware, Inc.
 * All Rights Reserved
 * **********************************************************/
#ifdef VMX86_DEVEL
char rcsId_vmware[] =
    "Id: vmware.c,v 1.11 2001/02/23 02:10:39 yoel Exp $";
#endif
/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/vmware/vmware.c,v 1.18 2003/09/24 02:43:31 dawes Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * TODO: support the vmware linux kernel fb driver (Option "UseFBDev").
 */

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Resources.h"

#include "compiler.h"	/* inb/outb */

#include "xf86PciInfo.h"	/* pci vendor id */
#include "xf86Pci.h"		/* pci */

#include "mipointer.h"		/* sw cursor */
#include "mibstore.h"		/* backing store */
#include "micmap.h"		/* mi color map */
#include "vgaHW.h"		/* VGA hardware */
#include "fb.h"
#include "shadowfb.h"           /* ShadowFB wrappers */

#include "xf86cmap.h"		/* xf86HandleColormaps */

#include "vmware.h"
#include "guest_os.h"
#include "vm_device_version.h"
#include "svga_modes.h"

#ifdef HaveDriverFuncs
#define VMWARE_DRIVER_FUNC HaveDriverFuncs
#else
#define VMWARE_DRIVER_FUNC 0
#endif

/*
 * So that the file compiles unmodified when dropped in to a < 6.9 source tree.
 */
#ifndef _X_EXPORT
#define _X_EXPORT
#endif
/*
 * So that the file compiles unmodified when dropped into an xfree source tree.
 */
#ifndef XORG_VERSION_CURRENT
#define XORG_VERSION_CURRENT XF86_VERSION_CURRENT
#endif

/*
 * Sanity check that xf86PciInfo.h has the correct values (which come from
 * the VMware source tree in vm_device_version.h.
 */
#if PCI_CHIP_VMWARE0405 != PCI_DEVICE_ID_VMWARE_SVGA2
#error "PCI_CHIP_VMWARE0405 is wrong, update it from vm_device_version.h"
#endif
#if PCI_CHIP_VMWARE0710 != PCI_DEVICE_ID_VMWARE_SVGA
#error "PCI_CHIP_VMWARE0710 is wrong, update it from vm_device_version.h"
#endif
#if PCI_VENDOR_VMWARE != PCI_VENDOR_ID_VMWARE
#error "PCI_VENDOR_VMWARE is wrong, update it from vm_device_version.h"
#endif

/*
 * This is the only way I know to turn a #define of an integer constant into
 * a constant string.
 */
#define VMW_INNERSTRINGIFY(s) #s
#define VMW_STRING(str) VMW_INNERSTRINGIFY(str)

#define VMWARE_NAME "VMWARE"
#define VMWARE_DRIVER_NAME "vmware"
#define VMWARE_MAJOR_VERSION	10
#define VMWARE_MINOR_VERSION	16
#define VMWARE_PATCHLEVEL	5
#define VMWARE_DRIVER_VERSION \
   (VMWARE_MAJOR_VERSION * 65536 + VMWARE_MINOR_VERSION * 256 + VMWARE_PATCHLEVEL)
#define VMWARE_DRIVER_VERSION_STRING \
    VMW_STRING(VMWARE_MAJOR_VERSION) "." VMW_STRING(VMWARE_MINOR_VERSION) \
    "." VMW_STRING(VMWARE_PATCHLEVEL)

static const char VMWAREBuildStr[] = "VMware Guest X Server "
    VMWARE_DRIVER_VERSION_STRING " - build=$Name:  $\n";

/*
 * Standard four digit version string expected by VMware Tools installer.
 * As the driver's version is only  {major, minor, patchlevel}, simply append an
 * extra zero for the fourth digit.
 */
#ifdef __GNUC__
const char vm_svga_version[] __attribute__((section(".modinfo"),unused)) =
    "version=" VMWARE_DRIVER_VERSION_STRING ".0";
#endif

static SymTabRec VMWAREChipsets[] = {
    { PCI_CHIP_VMWARE0405, "vmware0405" },
    { PCI_CHIP_VMWARE0710, "vmware0710" },
    { -1,                  NULL }
};

static resRange vmwareLegacyRes[] = {
    { ResExcIoBlock, SVGA_LEGACY_BASE_PORT,
      SVGA_LEGACY_BASE_PORT + SVGA_NUM_PORTS*sizeof(uint32)},
    _VGA_EXCLUSIVE, _END
};

#if XSERVER_LIBPCIACCESS

#define VMWARE_DEVICE_MATCH(d, i) \
    {PCI_VENDOR_VMWARE, (d), PCI_MATCH_ANY, PCI_MATCH_ANY, 0, 0, (i) }

static const struct pci_id_match VMwareDeviceMatch[] = {
    VMWARE_DEVICE_MATCH (PCI_CHIP_VMWARE0405, 0 ),
    VMWARE_DEVICE_MATCH (PCI_CHIP_VMWARE0710, 0 ),
    { 0, 0, 0 },
};
#endif

/*
 * Currently, even the PCI obedient 0405 chip still only obeys IOSE and
 * MEMSE for the SVGA resources.  Thus, RES_EXCLUSIVE_VGA is required.
 *
 * The 0710 chip also uses hardcoded IO ports that aren't disablable.
 */

static PciChipsets VMWAREPciChipsets[] = {
    { PCI_CHIP_VMWARE0405, PCI_CHIP_VMWARE0405, RES_EXCLUSIVE_VGA },
    { PCI_CHIP_VMWARE0710, PCI_CHIP_VMWARE0710, vmwareLegacyRes },
    { -1,		       -1,		    RES_UNDEFINED }
};

static const char *vgahwSymbols[] = {
    "vgaHWGetHWRec",
    "vgaHWGetIOBase",
    "vgaHWGetIndex",
    "vgaHWInit",
    "vgaHWProtect",
    "vgaHWRestore",
    "vgaHWSave",
    "vgaHWSaveScreen",
    "vgaHWUnlock",
    NULL
};

static const char *fbSymbols[] = {
    "fbCreateDefColormap",
    "fbPictureInit",
    "fbScreenInit",
    NULL
};

static const char *ramdacSymbols[] = {
    "xf86CreateCursorInfoRec",
    "xf86DestroyCursorInfoRec",
    "xf86InitCursor",
    NULL
};

static const char *shadowfbSymbols[] = {
    "ShadowFBInit2",
    NULL
};

#ifdef XFree86LOADER
static XF86ModuleVersionInfo vmwareVersRec = {
    "vmware",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    VMWARE_MAJOR_VERSION, VMWARE_MINOR_VERSION, VMWARE_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    { 0, 0, 0, 0}
};
#endif	/* XFree86LOADER */

typedef enum {
    OPTION_HW_CURSOR,
    OPTION_XINERAMA,
    OPTION_STATIC_XINERAMA
} VMWAREOpts;

static const OptionInfoRec VMWAREOptions[] = {
    { OPTION_HW_CURSOR, "HWcursor",     OPTV_BOOLEAN,   {0},    FALSE },
    { OPTION_XINERAMA,  "Xinerama",     OPTV_BOOLEAN,   {0},    FALSE },
    { OPTION_STATIC_XINERAMA, "StaticXinerama", OPTV_STRING, {0}, FALSE },
    { -1,               NULL,           OPTV_NONE,      {0},    FALSE }
};

/* Table of default modes to always add to the mode list. */

typedef struct {
   int width;
   int height;
} VMWAREDefaultMode;

#define SVGA_DEFAULT_MODE(width, height) { width, height, },

static const VMWAREDefaultMode VMWAREDefaultModes[] = {
   SVGA_DEFAULT_MODES
};

#undef SVGA_DEFAULT_MODE

static void VMWAREStopFIFO(ScrnInfoPtr pScrn);
static void VMWARESave(ScrnInfoPtr pScrn);

static Bool
VMWAREGetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate != NULL) {
        return TRUE;
    }
    pScrn->driverPrivate = xnfcalloc(sizeof(VMWARERec), 1);
    /* FIXME: Initialize driverPrivate... */
    return TRUE;
}

static void
VMWAREFreeRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate) {
        xfree(pScrn->driverPrivate);
        pScrn->driverPrivate = NULL;
    }
}

CARD32
vmwareReadReg(VMWAREPtr pVMWARE, int index)
{
    /*
     * Block SIGIO for the duration, so we don't get interrupted after the
     * outl but before the inl by a mouse move (which write to our registers).
     */
    int oldsigio, ret;
    oldsigio = xf86BlockSIGIO();
    outl(pVMWARE->indexReg, index);
    ret = inl(pVMWARE->valueReg);
    xf86UnblockSIGIO(oldsigio);
    return ret;
}

void
vmwareWriteReg(VMWAREPtr pVMWARE, int index, CARD32 value)
{
    /*
     * Block SIGIO for the duration, so we don't get interrupted in between
     * the outls by a mouse move (which write to our registers).
     */
    int oldsigio;
    oldsigio = xf86BlockSIGIO();
    outl(pVMWARE->indexReg, index);
    outl(pVMWARE->valueReg, value);
    xf86UnblockSIGIO(oldsigio);
}

void
vmwareWriteWordToFIFO(VMWAREPtr pVMWARE, CARD32 value)
{
    CARD32* vmwareFIFO = pVMWARE->vmwareFIFO;

    /* Need to sync? */
    if ((vmwareFIFO[SVGA_FIFO_NEXT_CMD] + sizeof(CARD32) == vmwareFIFO[SVGA_FIFO_STOP])
     || (vmwareFIFO[SVGA_FIFO_NEXT_CMD] == vmwareFIFO[SVGA_FIFO_MAX] - sizeof(CARD32) &&
	 vmwareFIFO[SVGA_FIFO_STOP] == vmwareFIFO[SVGA_FIFO_MIN])) {
        VmwareLog(("Syncing because of full fifo\n"));
        vmwareWaitForFB(pVMWARE);
    }

    vmwareFIFO[vmwareFIFO[SVGA_FIFO_NEXT_CMD] / sizeof(CARD32)] = value;
    if(vmwareFIFO[SVGA_FIFO_NEXT_CMD] == vmwareFIFO[SVGA_FIFO_MAX] -
       sizeof(CARD32)) {
        vmwareFIFO[SVGA_FIFO_NEXT_CMD] = vmwareFIFO[SVGA_FIFO_MIN];
    } else {
        vmwareFIFO[SVGA_FIFO_NEXT_CMD] += sizeof(CARD32);
    }
}

void
vmwareWaitForFB(VMWAREPtr pVMWARE)
{
    vmwareWriteReg(pVMWARE, SVGA_REG_SYNC, 1);
    while (vmwareReadReg(pVMWARE, SVGA_REG_BUSY));
}

void
vmwareSendSVGACmdUpdate(VMWAREPtr pVMWARE, BoxPtr pBB)
{
    vmwareWriteWordToFIFO(pVMWARE, SVGA_CMD_UPDATE);
    vmwareWriteWordToFIFO(pVMWARE, pBB->x1);
    vmwareWriteWordToFIFO(pVMWARE, pBB->y1);
    vmwareWriteWordToFIFO(pVMWARE, pBB->x2 - pBB->x1);
    vmwareWriteWordToFIFO(pVMWARE, pBB->y2 - pBB->y1);
}

static void
vmwareSendSVGACmdUpdateFullScreen(VMWAREPtr pVMWARE)
{
    BoxRec BB;

    BB.x1 = 0;
    BB.y1 = 0;
    BB.x2 = pVMWARE->ModeReg.svga_reg_width;
    BB.y2 = pVMWARE->ModeReg.svga_reg_height;
    vmwareSendSVGACmdUpdate(pVMWARE, &BB);
}

static CARD32
vmwareCalculateWeight(CARD32 mask)
{
    CARD32 weight;

    for (weight = 0; mask; mask >>= 1) {
        if (mask & 1) {
            weight++;
        }
    }
    return weight;
}

/*
 *-----------------------------------------------------------------------------
 *
 * VMXGetVMwareSvgaId --
 *
 *    Retrieve the SVGA_ID of the VMware SVGA adapter.
 *    This function should hide any backward compatibility mess.
 *
 * Results:
 *    The SVGA_ID_* of the present VMware adapter.
 *
 * Side effects:
 *    ins/outs
 *
 *-----------------------------------------------------------------------------
 */

static uint32
VMXGetVMwareSvgaId(VMWAREPtr pVMWARE)
{
    uint32 vmware_svga_id;

    /* Any version with any SVGA_ID_* support will initialize SVGA_REG_ID
     * to SVGA_ID_0 to support versions of this driver with SVGA_ID_0.
     *
     * Versions of SVGA_ID_0 ignore writes to the SVGA_REG_ID register.
     *
     * Versions of SVGA_ID_1 will allow us to overwrite the content
     * of the SVGA_REG_ID register only with the values SVGA_ID_0 or SVGA_ID_1.
     *
     * Versions of SVGA_ID_2 will allow us to overwrite the content
     * of the SVGA_REG_ID register only with the values SVGA_ID_0 or SVGA_ID_1
     * or SVGA_ID_2.
     */

    vmwareWriteReg(pVMWARE, SVGA_REG_ID, SVGA_ID_2);
    vmware_svga_id = vmwareReadReg(pVMWARE, SVGA_REG_ID);
    if (vmware_svga_id == SVGA_ID_2) {
        return SVGA_ID_2;
    }

    vmwareWriteReg(pVMWARE, SVGA_REG_ID, SVGA_ID_1);
    vmware_svga_id = vmwareReadReg(pVMWARE, SVGA_REG_ID);
    if (vmware_svga_id == SVGA_ID_1) {
        return SVGA_ID_1;
    }

    if (vmware_svga_id == SVGA_ID_0) {
        return SVGA_ID_0;
    }

    /* No supported VMware SVGA devices found */
    return SVGA_ID_INVALID;
}

#ifndef XSERVER_LIBPCIACCESS
/*
 *----------------------------------------------------------------------
 *
 *  RewriteTagString --
 *
 *      Rewrites the given string, removing the $Name:  $, and
 *      replacing it with the contents.  The output string must
 *      have enough room, or else.
 *
 * Results:
 *
 *      Output string updated.
 *
 * Side effects:
 *      None.
 *
 *----------------------------------------------------------------------
 */

static void
RewriteTagString(const char *istr, char *ostr, int osize)
{
    int chr;
    Bool inTag = FALSE;
    char *op = ostr;

    do {
	chr = *istr++;
	if (chr == '$') {
	    if (inTag) {
		inTag = FALSE;
		for (; op > ostr && op[-1] == ' '; op--) {
		}
		continue;
	    }
	    if (strncmp(istr, "Name:", 5) == 0) {
		istr += 5;
		istr += strspn(istr, " ");
		inTag = TRUE;
		continue;
	    }
	}
	*op++ = chr;
    } while (chr);
}
#endif

static void
VMWAREIdentify(int flags)
{
    xf86PrintChipsets(VMWARE_NAME, "driver for VMware SVGA", VMWAREChipsets);
}

static const OptionInfoRec *
VMWAREAvailableOptions(int chipid, int busid)
{
    return VMWAREOptions;
}

static int
VMWAREParseTopologyElement(ScrnInfoPtr pScrn,
                           unsigned int output,
                           const char *elementName,
                           const char *element,
                           const char *expectedTerminators,
                           Bool needTerminator,
                           unsigned int *outValue)
{
   char buf[10] = {0, };
   size_t i = 0;
   int retVal = -1;
   const char *str = element;

   for (i = 0; str[i] >= '0' && str[i] <= '9'; i++);
   if (i == 0) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Output %u: unable to parse %s.\n",
                 output, elementName);
      goto exit;
   }

   strncpy(buf, str, i);
   *outValue = atoi(buf);

   if (*outValue > (unsigned short)-1) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Output %u: %s must be less than %hu.\n",
                 output, elementName, (unsigned short)-1);
      goto exit;
   }

   str += i;

   if (needTerminator || str[0] != '\0') {
      Bool unexpected = TRUE;

      for (i = 0; i < strlen(expectedTerminators); i++) {
         if (str[0] == expectedTerminators[i]) {
            unexpected = FALSE;
         }
      }

      if (unexpected) {
         xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Output %u: unexpected character '%c' after %s.\n",
                    output, str[0], elementName);
         goto exit;
      } else {
         str++;
      }
   }

   retVal = str - element;

 exit:
   return retVal;
}

static xXineramaScreenInfo *
VMWAREParseTopologyString(ScrnInfoPtr pScrn,
                          const char *topology,
                          unsigned int *retNumOutputs)
{
   xXineramaScreenInfo *extents = NULL;
   unsigned int numOutputs = 0;
   const char *str = topology;

   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Parsing static Xinerama topology: Starting...\n");

   do {
      unsigned int x, y, width, height;
      int i;

      i = VMWAREParseTopologyElement(pScrn, numOutputs, "width", str, "xX", TRUE, &width);
      if (i == -1) {
         goto error;
      }
      str += i;

      i = VMWAREParseTopologyElement(pScrn, numOutputs, "height", str, "+", TRUE, &height);
      if (i == -1) {
         goto error;
      }
      str += i;

      i= VMWAREParseTopologyElement(pScrn, numOutputs, "X offset", str, "+", TRUE, &x);
      if (i == -1) {
         goto error;
      }
      str += i;

      i = VMWAREParseTopologyElement(pScrn, numOutputs, "Y offset", str, ";", FALSE, &y);
      if (i == -1) {
         goto error;
      }
      str += i;

      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Output %u: %ux%u+%u+%u\n",
                 numOutputs, width, height, x, y);

      numOutputs++;
      extents = xrealloc(extents, numOutputs * sizeof (xXineramaScreenInfo));
      extents[numOutputs - 1].x_org = x;
      extents[numOutputs - 1].y_org = y;
      extents[numOutputs - 1].width = width;
      extents[numOutputs - 1].height = height;
   } while (*str != 0);

   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Parsing static Xinerama topology: Succeeded.\n");
   goto exit;

 error:
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Parsing static Xinerama topology: Failed.\n");

   xfree(extents);
   extents = NULL;
   numOutputs = 0;

 exit:
   *retNumOutputs = numOutputs;
   return extents;
}


static Bool
VMWAREPreInit(ScrnInfoPtr pScrn, int flags)
{
    MessageType from;
    VMWAREPtr pVMWARE;
    OptionInfoPtr options;
    int bpp24flags;
    uint32 id;
    int i;
    ClockRange* clockRanges;
    IOADDRESS domainIOBase = 0;

#ifndef BUILD_FOR_420
    domainIOBase = pScrn->domainIOBase;
#endif

    if (flags & PROBE_DETECT) {
        return FALSE;
    }

    if (pScrn->numEntities != 1) {
        return FALSE;
    }

    if (!VMWAREGetRec(pScrn)) {
        return FALSE;
    }
    pVMWARE = VMWAREPTR(pScrn);

    pVMWARE->pvtSema = &pScrn->vtSema;

    pVMWARE->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
    if (pVMWARE->pEnt->location.type != BUS_PCI) {
        return FALSE;
    }
    pVMWARE->PciInfo = xf86GetPciInfoForEntity(pVMWARE->pEnt->index);
    if (pVMWARE->PciInfo == NULL) {
        return FALSE;
    }

    if (DEVICE_ID(pVMWARE->PciInfo) == PCI_CHIP_VMWARE0710) {
        pVMWARE->indexReg = domainIOBase +
           SVGA_LEGACY_BASE_PORT + SVGA_INDEX_PORT*sizeof(uint32);
        pVMWARE->valueReg = domainIOBase +
           SVGA_LEGACY_BASE_PORT + SVGA_VALUE_PORT*sizeof(uint32);
    } else {
        /* Note:  This setting of valueReg causes unaligned I/O */
#if XSERVER_LIBPCIACCESS
        pVMWARE->portIOBase = pVMWARE->PciInfo->regions[0].base_addr;
#else
        pVMWARE->portIOBase = pVMWARE->PciInfo->ioBase[0];
#endif
        pVMWARE->indexReg = domainIOBase +
           pVMWARE->portIOBase + SVGA_INDEX_PORT;
        pVMWARE->valueReg = domainIOBase +
           pVMWARE->portIOBase + SVGA_VALUE_PORT;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
               "VMware SVGA regs at (0x%04lx, 0x%04lx)\n",
               pVMWARE->indexReg, pVMWARE->valueReg);

    if (!xf86LoadSubModule(pScrn, "vgahw")) {
        return FALSE;
    }

    xf86LoaderReqSymLists(vgahwSymbols, NULL);

    if (!vgaHWGetHWRec(pScrn)) {
        return FALSE;
    }

    /*
     * Save the current video state.  Do it here before VMXGetVMwareSvgaId
     * writes to any registers.
     */
    VMWARESave(pScrn);

    id = VMXGetVMwareSvgaId(pVMWARE);
    if (id == SVGA_ID_0 || id == SVGA_ID_INVALID) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "No supported VMware SVGA found (read ID 0x%08x).\n", id);
        return FALSE;
    }

#if !XSERVER_LIBPCIACCESS
    pVMWARE->PciTag = pciTag(pVMWARE->PciInfo->bus, pVMWARE->PciInfo->device,
                             pVMWARE->PciInfo->func);
#endif
    pVMWARE->Primary = xf86IsPrimaryPci(pVMWARE->PciInfo);

    pScrn->monitor = pScrn->confScreen->monitor;

#ifdef ACCELERATE_OPS
    pVMWARE->vmwareCapability = vmwareReadReg(pVMWARE, SVGA_REG_CAPABILITIES);
#else
    pVMWARE->vmwareCapability = 0;
#endif

    pVMWARE->bitsPerPixel = vmwareReadReg(pVMWARE,
                                          SVGA_REG_HOST_BITS_PER_PIXEL);
    if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
       vmwareWriteReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL, pVMWARE->bitsPerPixel);
    }

    pVMWARE->depth = vmwareReadReg(pVMWARE, SVGA_REG_DEPTH);
    pVMWARE->videoRam = vmwareReadReg(pVMWARE, SVGA_REG_VRAM_SIZE);
    pVMWARE->memPhysBase = vmwareReadReg(pVMWARE, SVGA_REG_FB_START);
    pVMWARE->maxWidth = vmwareReadReg(pVMWARE, SVGA_REG_MAX_WIDTH);
    pVMWARE->maxHeight = vmwareReadReg(pVMWARE, SVGA_REG_MAX_HEIGHT);
    pVMWARE->cursorDefined = FALSE;
    pVMWARE->cursorShouldBeHidden = FALSE;

    if (pVMWARE->vmwareCapability & SVGA_CAP_CURSOR_BYPASS_2) {
        pVMWARE->cursorRemoveFromFB = SVGA_CURSOR_ON_REMOVE_FROM_FB;
        pVMWARE->cursorRestoreToFB = SVGA_CURSOR_ON_RESTORE_TO_FB;
    } else {
        pVMWARE->cursorRemoveFromFB = SVGA_CURSOR_ON_HIDE;
        pVMWARE->cursorRestoreToFB = SVGA_CURSOR_ON_SHOW;
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "caps:  0x%08X\n", pVMWARE->vmwareCapability);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "depth: %d\n", pVMWARE->depth);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "bpp:   %d\n", pVMWARE->bitsPerPixel);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "vram:  %d\n", pVMWARE->videoRam);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "pbase: 0x%08lx\n", pVMWARE->memPhysBase);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "mwidt: %d\n", pVMWARE->maxWidth);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "mheig: %d\n", pVMWARE->maxHeight);

    if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
        bpp24flags = Support24bppFb | Support32bppFb;
    } else {
        switch (pVMWARE->depth) {
        case 16:
            /*
             * In certain cases, the Windows host appears to
             * report 16 bpp and 16 depth but 555 weight.  Just
             * silently convert it to depth of 15.
             */
            if (pVMWARE->bitsPerPixel == 16 &&
                pVMWARE->weight.green == 5)
                pVMWARE->depth = 15;
        case 8:
        case 15:
            bpp24flags = NoDepth24Support;
         break;
        case 32:
            /*
             * There is no 32 bit depth, apparently it can get
             * reported this way sometimes on the Windows host.
             */
            if (pVMWARE->bitsPerPixel == 32)
                pVMWARE->depth = 24;
        case 24:
            if (pVMWARE->bitsPerPixel == 24)
                bpp24flags = Support24bppFb;
            else
                bpp24flags = Support32bppFb;
            break;
       default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Adapter is using an unsupported depth (%d).\n",
                       pVMWARE->depth);
            return FALSE;
       }
    }

    if (!xf86SetDepthBpp(pScrn, pVMWARE->depth, pVMWARE->bitsPerPixel,
                         pVMWARE->bitsPerPixel, bpp24flags)) {
        return FALSE;
    }

    /* Check that the returned depth is one we support */
    switch (pScrn->depth) {
    case 8:
    case 15:
    case 16:
    case 24:
        /* OK */
        break;
    default:
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Given depth (%d) is not supported by this driver\n",
                   pScrn->depth);
        return FALSE;
    }

    if (pScrn->bitsPerPixel != pVMWARE->bitsPerPixel) {
        if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
            vmwareWriteReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL,
                           pScrn->bitsPerPixel);
            pVMWARE->bitsPerPixel =
               vmwareReadReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL);
            pVMWARE->depth = vmwareReadReg(pVMWARE, SVGA_REG_DEPTH);
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Currently unavailable depth/bpp of %d/%d requested.\n"
                       "\tThe guest X server must run at the same depth and bpp as the host\n"
                       "\t(which are currently %d/%d).  This is automatically detected.  Please\n"
                       "\tdo not specify a depth on the command line or via the config file.\n",
                       pScrn->depth, pScrn->bitsPerPixel,
                       pVMWARE->depth, pVMWARE->bitsPerPixel);
            return FALSE;
        }
    }

    /*
     * Defer reading the colour registers until here in case we changed
     * bpp above.
     */

    pVMWARE->weight.red =
       vmwareCalculateWeight(vmwareReadReg(pVMWARE, SVGA_REG_RED_MASK));
    pVMWARE->weight.green =
       vmwareCalculateWeight(vmwareReadReg(pVMWARE, SVGA_REG_GREEN_MASK));
    pVMWARE->weight.blue =
       vmwareCalculateWeight(vmwareReadReg(pVMWARE, SVGA_REG_BLUE_MASK));
    pVMWARE->offset.blue = 0;
    pVMWARE->offset.green = pVMWARE->weight.blue;
    pVMWARE->offset.red = pVMWARE->weight.green + pVMWARE->offset.green;
    pVMWARE->defaultVisual = vmwareReadReg(pVMWARE, SVGA_REG_PSEUDOCOLOR) ?
       PseudoColor : TrueColor;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "depth: %d\n", pVMWARE->depth);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "bpp:   %d\n", pVMWARE->bitsPerPixel);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "w.red: %d\n", (int)pVMWARE->weight.red);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "w.grn: %d\n", (int)pVMWARE->weight.green);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "w.blu: %d\n", (int)pVMWARE->weight.blue);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "vis:   %d\n", pVMWARE->defaultVisual);

    if (pScrn->depth != pVMWARE->depth) {
        if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Currently unavailable depth of %d requested.\n"
                       "\tIf the guest X server's BPP matches the host's "
                       "BPP, then\n\tthe guest X server's depth must also "
                       "match the\n\thost's depth (currently %d).\n",
                       pScrn->depth, pVMWARE->depth);
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Currently unavailable depth of %d requested.\n"
                       "\tThe guest X server must run at the same depth as "
                       "the host (which\n\tis currently %d).  This is "
                       "automatically detected.  Please do not\n\tspecify "
                       "a depth on the command line or via the config file.\n",
                       pScrn->depth, pVMWARE->depth);
        }
           return FALSE;
    }
    xf86PrintDepthBpp(pScrn);

#if 0
    if (pScrn->depth == 24 && pix24bpp == 0) {
        pix24bpp = xf86GetBppFromDepth(pScrn, 24);
    }
#endif

    if (pScrn->depth > 8) {
        rgb zeros = { 0, 0, 0 };

        if (!xf86SetWeight(pScrn, pVMWARE->weight, zeros)) {
            return FALSE;
        }
        /* FIXME check returned weight */
    }
    if (!xf86SetDefaultVisual(pScrn, pVMWARE->defaultVisual)) {
        return FALSE;
    }
    if (pScrn->defaultVisual != pVMWARE->defaultVisual) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Given visual (%d) is not supported by this driver (%d is required)\n",
                   pScrn->defaultVisual, pVMWARE->defaultVisual);
        return FALSE;
    }
#if 0
    bytesPerPixel = pScrn->bitsPerPixel / 8;
#endif
    pScrn->progClock = TRUE;

#if 0 /* MGA does not do this */
    if (pScrn->visual != 0) {	/* FIXME */
        /* print error message */
        return FALSE;
    }
#endif

    xf86CollectOptions(pScrn, NULL);
    if (!(options = xalloc(sizeof(VMWAREOptions))))
        return FALSE;
    memcpy(options, VMWAREOptions, sizeof(VMWAREOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, options);

    if (pScrn->depth <= 8) {
        pScrn->rgbBits = 8;
    }

    from = X_PROBED;
    pScrn->chipset = (char*)xf86TokenToString(VMWAREChipsets, DEVICE_ID(pVMWARE->PciInfo));

    if (!pScrn->chipset) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "ChipID 0x%04x is not recognised\n", DEVICE_ID(pVMWARE->PciInfo));
        return FALSE;
    }

    from = X_DEFAULT;
    pVMWARE->hwCursor = TRUE;
    if (xf86GetOptValBool(options, OPTION_HW_CURSOR, &pVMWARE->hwCursor)) {
        from = X_CONFIG;
    }
    if (pVMWARE->hwCursor && !(pVMWARE->vmwareCapability & SVGA_CAP_CURSOR)) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "HW cursor is not supported in this configuration\n");
        from = X_PROBED;
        pVMWARE->hwCursor = FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
               pVMWARE->hwCursor ? "HW" : "SW");
    pScrn->videoRam = pVMWARE->videoRam / 1024;
    pScrn->memPhysBase = pVMWARE->memPhysBase;

    xfree(options);

    {
        Gamma zeros = { 0.0, 0.0, 0.0 };
        if (!xf86SetGamma(pScrn, zeros)) {
            return FALSE;
        }
    }
#if 0
    if ((i = xf86GetPciInfoForScreen(pScrn->scrnIndex, &pciList, NULL)) != 1) {
        /* print error message */
        VMWAREFreeRec(pScrn);
        if (i > 0) {
            xfree(pciList);
        }
        return FALSE;
    }
#endif
    clockRanges = xnfcalloc(sizeof(ClockRange), 1);
    clockRanges->next = NULL;
    clockRanges->minClock = 1;
    clockRanges->maxClock = 400000000;
    clockRanges->clockIndex = -1;
    clockRanges->interlaceAllowed = FALSE;
    clockRanges->doubleScanAllowed = FALSE;
    clockRanges->ClockMulFactor = 1;
    clockRanges->ClockDivFactor = 1;

    i = xf86ValidateModes(pScrn, pScrn->monitor->Modes, pScrn->display->modes,
                          clockRanges, NULL, 256, pVMWARE->maxWidth, 32 * 32,
                          128, pVMWARE->maxHeight,
                          pScrn->display->virtualX, pScrn->display->virtualY,
                          pVMWARE->videoRam,
                          LOOKUP_BEST_REFRESH);
    if (i == -1) {
        VMWAREFreeRec(pScrn);
        return FALSE;
    }
    xf86PruneDriverModes(pScrn);
    if (i == 0 || pScrn->modes == NULL) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
        VMWAREFreeRec(pScrn);
        return FALSE;
    }
    xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);
    pScrn->currentMode = pScrn->modes;
    xf86PrintModes(pScrn);
    xf86SetDpi(pScrn, 0, 0);
    if (!xf86LoadSubModule(pScrn, "fb") ||
        !xf86LoadSubModule(pScrn, "shadowfb")) {
        VMWAREFreeRec(pScrn);
        return FALSE;
    }
    xf86LoaderReqSymLists(fbSymbols, shadowfbSymbols, NULL);

    /* Need ramdac for hwcursor */
    if (pVMWARE->hwCursor) {
        if (!xf86LoadSubModule(pScrn, "ramdac")) {
            VMWAREFreeRec(pScrn);
            return FALSE;
        }
        xf86LoaderReqSymLists(ramdacSymbols, NULL);
    }

    return TRUE;
}

static Bool
VMWAREMapMem(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
#if XSERVER_LIBPCIACCESS
    int err;
    struct pci_device *const device = pVMWARE->PciInfo;
#endif

#if XSERVER_LIBPCIACCESS
   err = pci_device_map_range(device,
                              pVMWARE->memPhysBase,
                              pVMWARE->videoRam,
                              PCI_DEV_MAP_FLAG_WRITABLE | 
                              PCI_DEV_MAP_FLAG_WRITE_COMBINE,
                              (void **) &pVMWARE->FbBase);
   if (err) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                  "Unable to map frame buffer BAR. %s (%d)\n",
                  strerror (err), err);
       return FALSE;
   }

#else
    pVMWARE->FbBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
                                    pVMWARE->PciTag,
                                    pVMWARE->memPhysBase,
                                    pVMWARE->videoRam);
#endif
    if (!pVMWARE->FbBase)
        return FALSE;

    VmwareLog(("FB Mapped: %p/%u -> %p/%u\n",
               pVMWARE->memPhysBase, pVMWARE->videoRam,
               pVMWARE->FbBase, pVMWARE->videoRam));
    return TRUE;
}

static Bool
VMWAREUnmapMem(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE;

    pVMWARE = VMWAREPTR(pScrn);

    VmwareLog(("Unmapped: %p/%u\n", pVMWARE->FbBase, pVMWARE->videoRam));

#if XSERVER_LIBPCIACCESS
    pci_device_unmap_range(pVMWARE->PciInfo, pVMWARE->FbBase, pVMWARE->videoRam);
#else
    xf86UnMapVidMem(pScrn->scrnIndex, pVMWARE->FbBase, pVMWARE->videoRam);
#endif
    pVMWARE->FbBase = NULL;
    return TRUE;
}

static void
VMWARESave(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->SavedReg;
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWARERegPtr vmwareReg = &pVMWARE->SavedReg;

    vgaHWSave(pScrn, vgaReg, VGA_SR_ALL);

    vmwareReg->svga_reg_enable = vmwareReadReg(pVMWARE, SVGA_REG_ENABLE);
    vmwareReg->svga_reg_width = vmwareReadReg(pVMWARE, SVGA_REG_WIDTH);
    vmwareReg->svga_reg_height = vmwareReadReg(pVMWARE, SVGA_REG_HEIGHT);
    vmwareReg->svga_reg_bits_per_pixel =
       vmwareReadReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL);
    vmwareReg->svga_reg_id = vmwareReadReg(pVMWARE, SVGA_REG_ID);

    /* XXX this should be based on the cap bit, not hwCursor... */
    if (pVMWARE->hwCursor) {
       vmwareReg->svga_reg_cursor_on =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_ON);
       vmwareReg->svga_reg_cursor_x =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_X);
       vmwareReg->svga_reg_cursor_y =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_Y);
       vmwareReg->svga_reg_cursor_id =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_ID);
    }

    vmwareReg->svga_fifo_enabled = vmwareReadReg(pVMWARE, SVGA_REG_CONFIG_DONE);
}

static void
VMWARERestoreRegs(ScrnInfoPtr pScrn, VMWARERegPtr vmwareReg)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VmwareLog(("VMWARERestoreRegs: W: %d, H: %d, BPP: %d, Enable: %d\n",
	       vmwareReg->svga_reg_width, vmwareReg->svga_reg_height,
	       vmwareReg->svga_reg_bits_per_pixel, vmwareReg->svga_reg_enable));
    if (vmwareReg->svga_reg_enable) {
        vmwareWriteReg(pVMWARE, SVGA_REG_ID, vmwareReg->svga_reg_id);
        vmwareWriteReg(pVMWARE, SVGA_REG_WIDTH, vmwareReg->svga_reg_width);
        vmwareWriteReg(pVMWARE, SVGA_REG_HEIGHT, vmwareReg->svga_reg_height);
        vmwareWriteReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL,
                       vmwareReg->svga_reg_bits_per_pixel);
        vmwareWriteReg(pVMWARE, SVGA_REG_ENABLE, vmwareReg->svga_reg_enable);
        vmwareWriteReg(pVMWARE, SVGA_REG_GUEST_ID, GUEST_OS_LINUX);
        if (pVMWARE->hwCursor) {
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_ID,
                           vmwareReg->svga_reg_cursor_id);
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_X,
                           vmwareReg->svga_reg_cursor_x);
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_Y,
                           vmwareReg->svga_reg_cursor_y);
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_ON,
                           vmwareReg->svga_reg_cursor_on);
        }
    } else {
        vmwareWriteReg(pVMWARE, SVGA_REG_ENABLE, vmwareReg->svga_reg_enable);
    }
}

static void
VMWARERestore(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->SavedReg;
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWARERegPtr vmwareReg = &pVMWARE->SavedReg;

    vmwareWaitForFB(pVMWARE);
    if (!vmwareReg->svga_fifo_enabled) {
        VMWAREStopFIFO(pScrn);
    }

    vgaHWProtect(pScrn, TRUE);
    VMWARERestoreRegs(pScrn, vmwareReg);
    vgaHWRestore(pScrn, vgaReg, VGA_SR_ALL);
    vgaHWProtect(pScrn, FALSE);
}

static Bool
VMWAREModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode, Bool rebuildPixmap)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->ModeReg;
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWARERegPtr vmwareReg = &pVMWARE->ModeReg;

    vgaHWUnlock(hwp);
    if (!vgaHWInit(pScrn, mode))
        return FALSE;
    pScrn->vtSema = TRUE;

    vmwareReg->svga_reg_enable = 1;
    vmwareReg->svga_reg_width = max(mode->HDisplay, pScrn->virtualX);
    vmwareReg->svga_reg_height = max(mode->VDisplay, pScrn->virtualY);
    vmwareReg->svga_reg_bits_per_pixel = pVMWARE->bitsPerPixel;

    vgaHWProtect(pScrn, TRUE);

    vgaHWRestore(pScrn, vgaReg, VGA_SR_ALL);
    VMWARERestoreRegs(pScrn, vmwareReg);

    if (pVMWARE->hwCursor) {
        vmwareCursorModeInit(pScrn, mode);
    }

    VmwareLog(("Required mode: %ux%u\n", mode->HDisplay, mode->VDisplay));
    VmwareLog(("Virtual:       %ux%u\n", pScrn->virtualX, pScrn->virtualY));
    VmwareLog(("dispWidth:     %u\n", pScrn->displayWidth));
    pVMWARE->fbOffset = vmwareReadReg(pVMWARE, SVGA_REG_FB_OFFSET);
    pVMWARE->fbPitch = vmwareReadReg(pVMWARE, SVGA_REG_BYTES_PER_LINE);
    pVMWARE->FbSize = vmwareReadReg(pVMWARE, SVGA_REG_FB_SIZE);

    pScrn->displayWidth = (pVMWARE->fbPitch * 8) / ((pScrn->bitsPerPixel + 7) & ~7);
    VmwareLog(("fbOffset:      %u\n", pVMWARE->fbOffset));
    VmwareLog(("fbPitch:       %u\n", pVMWARE->fbPitch));
    VmwareLog(("fbSize:        %u\n", pVMWARE->FbSize));
    VmwareLog(("New dispWidth: %u\n", pScrn->displayWidth));

    vmwareCheckVideoSanity(pScrn);

    if (rebuildPixmap) {
        pScrn->pScreen->ModifyPixmapHeader((*pScrn->pScreen->GetScreenPixmap)(pScrn->pScreen),
                                           pScrn->pScreen->width,
                                           pScrn->pScreen->height,
                                           pScrn->pScreen->rootDepth,
                                           pScrn->bitsPerPixel,
                                           PixmapBytePad(pScrn->displayWidth,
                                                         pScrn->pScreen->rootDepth),
                                           (pointer)(pVMWARE->FbBase + pScrn->fbOffset));

        (*pScrn->EnableDisableFBAccess)(pScrn->pScreen->myNum, FALSE);
        (*pScrn->EnableDisableFBAccess)(pScrn->pScreen->myNum, TRUE);
    }

    vgaHWProtect(pScrn, FALSE);

    /*
     * XXX -- If we want to check that we got the mode we asked for, this
     * would be a good place.
     */

    /*
     * Update Xinerama info appropriately.
     */
    if (pVMWARE->xinerama && !pVMWARE->xineramaStatic) {
       if (pVMWARE->xineramaNextState) {
          xfree(pVMWARE->xineramaState);
          pVMWARE->xineramaState = pVMWARE->xineramaNextState;
          pVMWARE->xineramaNumOutputs = pVMWARE->xineramaNextNumOutputs;

          pVMWARE->xineramaNextState = NULL;
          pVMWARE->xineramaNextNumOutputs = 0;
       } else {
          VMWAREXineramaPtr basicState =
             (VMWAREXineramaPtr)xcalloc(1, sizeof (VMWAREXineramaRec));
          if (basicState) {
             basicState->x_org = 0;
             basicState->y_org = 0;
             basicState->width = vmwareReg->svga_reg_width;
             basicState->height = vmwareReg->svga_reg_height;

             xfree(pVMWARE->xineramaState);
             pVMWARE->xineramaState = basicState;
             pVMWARE->xineramaNumOutputs = 1;
          }
       }
    }

    /*
     * Update host's view of guest topology.
     */
    if (pVMWARE->vmwareCapability & SVGA_CAP_DISPLAY_TOPOLOGY) {
        if (pVMWARE->xinerama) {
            int i = 0;
            VMWAREXineramaPtr xineramaState = pVMWARE->xineramaState;
            vmwareWriteReg(pVMWARE, SVGA_REG_NUM_GUEST_DISPLAYS,
                           pVMWARE->xineramaNumOutputs);

            for (i = 0; i < pVMWARE->xineramaNumOutputs; i++) {
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_ID, i);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_IS_PRIMARY, TRUE);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_X,
                               xineramaState[i].x_org);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_Y,
                               xineramaState[i].y_org);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_WIDTH,
                               xineramaState[i].width);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_HEIGHT,
                               xineramaState[i].height);
            }
        } else {
            vmwareWriteReg(pVMWARE, SVGA_REG_NUM_GUEST_DISPLAYS, 1);

            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_ID, 0);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_IS_PRIMARY, TRUE);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_X, 0);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_Y, 0);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_WIDTH, mode->HDisplay);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_HEIGHT, mode->VDisplay);
        }

        vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_ID, SVGA_INVALID_DISPLAY_ID);
    }

    return TRUE;
}

static void
VMWAREAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    /* FIXME */
}

static void
VMWAREInitFIFO(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
#if XSERVER_LIBPCIACCESS
    struct pci_device *const device = pVMWARE->PciInfo;
    int err;
#endif
    CARD32* vmwareFIFO;
    Bool extendedFifo;
    int min;

    TRACEPOINT

    pVMWARE->mmioPhysBase = vmwareReadReg(pVMWARE, SVGA_REG_MEM_START);
    pVMWARE->mmioSize = vmwareReadReg(pVMWARE, SVGA_REG_MEM_SIZE) & ~3;
#if XSERVER_LIBPCIACCESS
    err = pci_device_map_range(device, pVMWARE->mmioPhysBase,
                               pVMWARE->mmioSize,
                               PCI_DEV_MAP_FLAG_WRITABLE,
                               (void **) &pVMWARE->mmioVirtBase);
    if (err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to map mmio BAR. %s (%d)\n",
                   strerror (err), err);
        return;
    }
#else
    pVMWARE->mmioVirtBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO,
                                          pVMWARE->PciTag,
                                          pVMWARE->mmioPhysBase,
                                          pVMWARE->mmioSize);
#endif
    vmwareFIFO = pVMWARE->vmwareFIFO = (CARD32*)pVMWARE->mmioVirtBase;

    extendedFifo = pVMWARE->vmwareCapability & SVGA_CAP_EXTENDED_FIFO;
    min = extendedFifo ? vmwareReadReg(pVMWARE, SVGA_REG_MEM_REGS) : 4;

    vmwareFIFO[SVGA_FIFO_MIN] = min * sizeof(CARD32);
    vmwareFIFO[SVGA_FIFO_MAX] = pVMWARE->mmioSize;
    vmwareFIFO[SVGA_FIFO_NEXT_CMD] = min * sizeof(CARD32);
    vmwareFIFO[SVGA_FIFO_STOP] = min * sizeof(CARD32);
    vmwareWriteReg(pVMWARE, SVGA_REG_CONFIG_DONE, 1);
}

static void
VMWAREStopFIFO(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

    TRACEPOINT

    vmwareWriteReg(pVMWARE, SVGA_REG_CONFIG_DONE, 0);
#if XSERVER_LIBPCIACCESS
    pci_device_unmap_range(pVMWARE->PciInfo, pVMWARE->mmioVirtBase, pVMWARE->mmioSize);
#else
    xf86UnMapVidMem(pScrn->scrnIndex, pVMWARE->mmioVirtBase, pVMWARE->mmioSize);
#endif
}

static Bool
VMWARECloseScreen(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    ScreenPtr save = &pVMWARE->ScrnFuncs;

    VmwareLog(("cursorSema: %d\n", pVMWARE->cursorSema));

    if (*pVMWARE->pvtSema) {
        if (pVMWARE->videoStreams) {
            vmwareVideoEnd(pScreen);
        }

        if (pVMWARE->CursorInfoRec) {
            vmwareCursorCloseScreen(pScreen);
        }

        VMWARERestore(pScrn);
        VMWAREUnmapMem(pScrn);

        pScrn->vtSema = FALSE;
    }

    pScreen->CloseScreen = save->CloseScreen;
    pScreen->SaveScreen = save->SaveScreen;

#if VMWARE_DRIVER_FUNC
    pScrn->DriverFunc = NULL;
#endif

    return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}

static Bool
VMWARESaveScreen(ScreenPtr pScreen, int mode)
{
    VmwareLog(("VMWareSaveScreen() mode = %d\n", mode));

    /*
     * This thoroughly fails to do anything useful to svga mode.  I doubt
     * we care; who wants to idle-blank their VM's screen anyway?
     */
    return vgaHWSaveScreen(pScreen, mode);
}

/* disabled by default to reduce spew in DEBUG_LOGGING mode. */
/*#define DEBUG_LOG_UPDATES*/

static void
VMWAREPreDirtyBBUpdate(ScrnInfoPtr pScrn, int nboxes, BoxPtr boxPtr)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

#ifdef DEBUG_LOG_UPDATES
    {
        int i;
        for (i = 0; i < nboxes; i++) {
            VmwareLog(("PreUpdate #%d (%d, %d, w = %d, h = %d)\n", nboxes - i,
                       boxPtr[i].x1, boxPtr[i].y1,
                       boxPtr[i].x2 - boxPtr[i].x1,
                       boxPtr[i].y2 - boxPtr[i].y1));
        }
    }
#endif

    /*
     * We only register this callback if we have a HW cursor.
     */
    while (nboxes--) {
        if (BOX_INTERSECT(*boxPtr, pVMWARE->hwcur.box)) {
            if (!pVMWARE->cursorExcludedForUpdate) {
                PRE_OP_HIDE_CURSOR();
                pVMWARE->cursorExcludedForUpdate = TRUE;
            }
	    break;
        }
        boxPtr++;
    }
}

static void
VMWAREPostDirtyBBUpdate(ScrnInfoPtr pScrn, int nboxes, BoxPtr boxPtr)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    while (nboxes--) {
#ifdef DEBUG_LOG_UPDATES
        VmwareLog(("PostUpdate #%d (%d, %d, w = %d, h = %d)\n", nboxes,
                   boxPtr->x1, boxPtr->y1,
                   boxPtr->x2 - boxPtr->x1, boxPtr->y2 - boxPtr->y1));
#endif

        /* Clip off (y only) for offscreen memory */
        if (boxPtr->y2 >= pVMWARE->ModeReg.svga_reg_height)
            boxPtr->y2 = pVMWARE->ModeReg.svga_reg_height;
        if (boxPtr->y1 >= pVMWARE->ModeReg.svga_reg_height)
            boxPtr->y1 = pVMWARE->ModeReg.svga_reg_height;
        if (boxPtr->y1 == boxPtr->y2) {
            boxPtr++;
            continue;
        }

        vmwareSendSVGACmdUpdate(pVMWARE, boxPtr++);
    }

    if (pVMWARE->hwCursor && pVMWARE->cursorExcludedForUpdate) {
        POST_OP_SHOW_CURSOR();
        pVMWARE->cursorExcludedForUpdate = FALSE;
    }
}

static void
VMWARELoadPalette(ScrnInfoPtr pScrn, int numColors, int* indices,
                  LOCO* colors, VisualPtr pVisual)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    int i;

    for (i = 0; i < numColors; i++) {
        vmwareWriteReg(pVMWARE, SVGA_PALETTE_BASE + *indices * 3 + 0, colors[*indices].red);
        vmwareWriteReg(pVMWARE, SVGA_PALETTE_BASE + *indices * 3 + 1, colors[*indices].green);
        vmwareWriteReg(pVMWARE, SVGA_PALETTE_BASE + *indices * 3 + 2, colors[*indices].blue);
        indices++;
    }
    VmwareLog(("Palette loading done\n"));
}


DisplayModeRec *
VMWAREAddDisplayMode(ScrnInfoPtr pScrn,
                     const char *name,
                     int width,
                     int height)
{
   DisplayModeRec *mode;

   mode = xalloc(sizeof(DisplayModeRec));

   mode->name = xalloc(strlen(name) + 1);
   strcpy(mode->name, name);
   mode->status = MODE_OK;
   mode->type = M_T_DEFAULT;
   mode->HDisplay = width;
   mode->VDisplay = height;

   mode->next = pScrn->modes;
   mode->prev = pScrn->modes->prev;
   pScrn->modes->prev->next = mode;
   pScrn->modes->prev = mode;

   return mode;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareIsRegionEqual --
 *
 *    This function implements REGION_EQUAL because older versions of
 *    regionstr.h don't define it.
 *    It is a slightly modified version of miRegionEqual from $Xorg: miregion.c
 *
 * Results:
 *    TRUE if regions are equal; FALSE otherwise
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

Bool
vmwareIsRegionEqual(const RegionPtr reg1,
                    const RegionPtr reg2)
{
    int i, num;
    BoxPtr rects1, rects2;

    if ((reg1->extents.x1 != reg2->extents.x1) ||
        (reg1->extents.x2 != reg2->extents.x2) ||
        (reg1->extents.y1 != reg2->extents.y1) ||
        (reg1->extents.y2 != reg2->extents.y2)) {
        return FALSE;
    }

    num = REGION_NUM_RECTS(reg1);
    if (num != REGION_NUM_RECTS(reg2)) {
        return FALSE;
    }

    rects1 = REGION_RECTS(reg1);
    rects2 = REGION_RECTS(reg2);

    for (i = 0; i < num; i++) {
        if ((rects1[i].x1 != rects2[i].x1) ||
            (rects1[i].x2 != rects2[i].x2) ||
            (rects1[i].y1 != rects2[i].y1) ||
            (rects1[i].y2 != rects2[i].y2)) {
            return FALSE;
        }
    }

    return TRUE;
}


#if VMWARE_DRIVER_FUNC
static Bool
VMWareDriverFunc(ScrnInfoPtr pScrn,
                 xorgDriverFuncOp op,
                 pointer data)
{
   CARD32 *flag;
   xorgRRModeMM *modemm;

   switch (op) {
   case GET_REQUIRED_HW_INTERFACES:
      flag = (CARD32 *)data;

      if (flag) {
         *flag = HW_IO | HW_MMIO;
      }
      return TRUE;
   case RR_GET_MODE_MM:
      modemm = (xorgRRModeMM *)data;

      /*
       * Because changing the resolution of the guest is usually changing the size
       * of a window on the host desktop, the real physical DPI will not change. To
       * keep the guest in sync, we scale the 'physical' screen dimensions to
       * keep the DPI constant.
       */
      if (modemm && modemm->mode) {
         modemm->mmWidth *= modemm->mode->HDisplay / (double)(modemm->virtX);
         modemm->mmHeight *= modemm->mode->VDisplay / (double)(modemm->virtY);
      }
      return TRUE;
   default:
      return FALSE;
   }
}
#endif


static Bool
VMWAREScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
    ScrnInfoPtr pScrn;
    vgaHWPtr hwp;
    VMWAREPtr pVMWARE;
    OptionInfoPtr options;
    Bool useXinerama = TRUE;

    /* Get the ScrnInfoRec */
    pScrn = xf86Screens[pScreen->myNum];
    pVMWARE = VMWAREPTR(pScrn);


    xf86CollectOptions(pScrn, NULL);
    if (!(options = xalloc(sizeof(VMWAREOptions))))
        return FALSE;
    memcpy(options, VMWAREOptions, sizeof(VMWAREOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, options);

    /*
     * Init xinerama preferences.
     */
    useXinerama = xf86ReturnOptValBool(options, OPTION_XINERAMA,
                                       pVMWARE->vmwareCapability & SVGA_CAP_MULTIMON);
    if (useXinerama && !(pVMWARE->vmwareCapability & SVGA_CAP_MULTIMON)) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                  "Xinerama is not safely supported by the current virtual hardware. "
                  "Do not request resolutions that require > 16MB of framebuffer.\n");
    }


    if (useXinerama && xf86IsOptionSet(options, OPTION_STATIC_XINERAMA)) {
       char *topology = xf86GetOptValString(options, OPTION_STATIC_XINERAMA);
       if (topology) {
          pVMWARE->xineramaState =
             VMWAREParseTopologyString(pScrn, topology, &pVMWARE->xineramaNumOutputs);

         pVMWARE->xineramaStatic = pVMWARE->xineramaState != NULL;

         xfree(topology);
       }
    }

    xfree(options);

    /* Initialise VMWARE_CTRL extension. */
    VMwareCtrl_ExtInit(pScrn);

    /* Initialise Xinerama extension. */
    if (useXinerama) {
       VMwareXinerama_ExtInit(pScrn);
    }

    if (pVMWARE->xinerama && pVMWARE->xineramaStatic) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, pVMWARE->xineramaState ?
                                            "Using static Xinerama.\n" :
                                            "Failed to configure static Xinerama.\n");
    }

    /*
     * If using the vgahw module, its data structures and related
     * things are typically initialised/mapped here.
     */
    hwp = VGAHWPTR(pScrn);
    vgaHWGetIOBase(hwp);

    VMWAREInitFIFO(pScrn);

    /* Initialise the first mode */
    VMWAREModeInit(pScrn, pScrn->currentMode, FALSE);

    /* Set the viewport if supported */
    VMWAREAdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    /*
     * Setup the screen's visuals, and initialise the framebuffer
     * code.
     */
    VMWAREMapMem(pScrn);

    /*
     * Clear the framebuffer (and any black-border mode areas).
     */
    memset(pVMWARE->FbBase, 0, pVMWARE->FbSize);
    vmwareSendSVGACmdUpdateFullScreen(pVMWARE);

    /* Reset the visual list */
    miClearVisualTypes();

    /*
     * Setup the visuals supported.  This driver only supports
     * TrueColor for bpp > 8, so the default set of visuals isn't
     * acceptable.  To deal with this, call miSetVisualTypes with
     * the appropriate visual mask.
     */
    if (pScrn->bitsPerPixel > 8) {
        if (!miSetVisualTypes(pScrn->depth, TrueColorMask,
                              pScrn->rgbBits, pScrn->defaultVisual)) {
            return FALSE;
        }
    } else {
        if (!miSetVisualTypes(pScrn->depth,
                              miGetDefaultVisualMask(pScrn->depth),
                              pScrn->rgbBits, pScrn->defaultVisual)) {
            return FALSE;
        }
    }

    miSetPixmapDepths();

    /*
     * Initialise the framebuffer.
     */
    if (!fbScreenInit(pScreen, pVMWARE->FbBase + pVMWARE->fbOffset,
                      pScrn->virtualX, pScrn->virtualY,
                      pScrn->xDpi, pScrn->yDpi,
                      pScrn->displayWidth,
                      pScrn->bitsPerPixel)) {
        return FALSE;
    }

    /* Override the default mask/offset settings */
    if (pScrn->bitsPerPixel > 8) {
        int i;
        VisualPtr visual;

        for (i = 0, visual = pScreen->visuals;
             i < pScreen->numVisuals; i++, visual++) {
            if ((visual->class | DynamicClass) == DirectColor) {
                visual->offsetRed = pScrn->offset.red;
                visual->offsetGreen = pScrn->offset.green;
                visual->offsetBlue = pScrn->offset.blue;
                visual->redMask = pScrn->mask.red;
                visual->greenMask = pScrn->mask.green;
                visual->blueMask = pScrn->mask.blue;
            }
        }
    }

    /* must be after RGB ordering fixed */
    fbPictureInit (pScreen, 0, 0);

    /*
     * Save the old screen vector, then wrap CloseScreen and
     * set SaveScreen.
     */
    pVMWARE->ScrnFuncs = *pScreen;
    pScreen->CloseScreen = VMWARECloseScreen;
    pScreen->SaveScreen = VMWARESaveScreen;

    /*
     * Set initial black & white colourmap indices.
     */
    xf86SetBlackWhitePixels(pScreen);

    /*
     * Initialize shadowfb to notify us of dirty rectangles.  We only
     * need preFB access callbacks if we're using the hw cursor.
     */
    if (!ShadowFBInit2(pScreen, 
                       pVMWARE->hwCursor ? VMWAREPreDirtyBBUpdate : NULL,
                       VMWAREPostDirtyBBUpdate)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "ShadowFB initialization failed\n");
        return FALSE;
    }

    /*
     * If we have a hw cursor, we need to hook functions that might
     * read from the framebuffer.
     */
    if (pVMWARE->hwCursor) {
        vmwareCursorHookWrappers(pScreen);
    }

    /*
     * If backing store is to be supported (as is usually the case),
     * initialise it.
     */
    miInitializeBackingStore(pScreen);
    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /*
     * Initialize software cursor.
     */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /*
     * Initialize hardware cursor.
     */
    if (pVMWARE->hwCursor) {
        if (!vmwareCursorInit(pScreen)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Hardware cursor initialization failed\n");
            pVMWARE->hwCursor = FALSE;
        }
    }

    /*
     * Install colourmap functions.  If using the vgahw module,
     * vgaHandleColormaps would usually be called here.
     */
    if (!fbCreateDefColormap(pScreen))
        return FALSE;

    if (!xf86HandleColormaps(pScreen, 256, 8,
                             VMWARELoadPalette, NULL,
                             CMAP_PALETTED_TRUECOLOR |
                             CMAP_RELOAD_ON_MODE_SWITCH)) {
        return FALSE;
    }

    /*
     * We explictly add a set of default modes because the X server will
     * not include modes larger than the initial one.
     */
   {
      unsigned int i;
      unsigned int numModes = sizeof (VMWAREDefaultModes) / sizeof *(VMWAREDefaultModes);
      char name[10];
      for (i = 0; i < numModes; i++) {
         const VMWAREDefaultMode *mode = &VMWAREDefaultModes[i];

         /* Only modes that fit the hardware maximums should be added. */
         if (mode->width <= pVMWARE->maxWidth && mode->height <= pVMWARE->maxHeight) {
            snprintf(name, 10, "%dx%d", mode->width, mode->height);
            VMWAREAddDisplayMode(pScrn, name, mode->width, mode->height);
         }
      }

      /* Add the hardware maximums as a mode. */
      snprintf(name, 10, "%dx%d", pVMWARE->maxWidth, pVMWARE->maxHeight);
      VMWAREAddDisplayMode(pScrn, name, pVMWARE->maxWidth, pVMWARE->maxHeight);
   }

    /*
     * We will lazily add the dynamic modes as the are needed when new
     * modes are requested through the control extension.
     */
    pVMWARE->dynMode1 = NULL;
    pVMWARE->dynMode2 = NULL;

#if VMWARE_DRIVER_FUNC
    pScrn->DriverFunc = VMWareDriverFunc;
#endif

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
        xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }

    /* Initialize Xv extension */
    pVMWARE->videoStreams = NULL;
    if (vmwareVideoEnabled(pVMWARE)) {
        if (!vmwareVideoInit(pScreen)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Xv initialization failed\n");
        }
    }


    /* Done */
    return TRUE;
}

static Bool
VMWARESwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
    return VMWAREModeInit(xf86Screens[scrnIndex], mode, TRUE);
}

static Bool
VMWAREEnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

    if (!pVMWARE->SavedReg.svga_fifo_enabled) {
        VMWAREInitFIFO(pScrn);
    }

    return VMWAREModeInit(pScrn, pScrn->currentMode, TRUE);
}

static void
VMWARELeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    VMWARERestore(pScrn);
}

static void
VMWAREFreeScreen(int scrnIndex, int flags)
{
    /*
     * If the vgahw module is used vgaHWFreeHWRec() would be called
     * here.
     */
   VMWAREFreeRec(xf86Screens[scrnIndex]);
}

static ModeStatus
VMWAREValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags)
{
    return MODE_OK;
}

#if XSERVER_LIBPCIACCESS
static Bool
VMwarePciProbe (DriverPtr           drv,
                int                 entity_num,
                struct pci_device   *device,
                intptr_t            match_data)
{
    ScrnInfoPtr     scrn = NULL;
    EntityInfoPtr   entity;

    scrn = xf86ConfigPciEntity(scrn, 0, entity_num, VMWAREPciChipsets,
                               NULL, NULL, NULL, NULL, NULL);
    if (scrn != NULL) {
        scrn->driverVersion = VMWARE_DRIVER_VERSION;
        scrn->driverName = VMWARE_DRIVER_NAME;
        scrn->name = VMWARE_NAME;
        scrn->Probe = NULL;
    }

    entity = xf86GetEntityInfo(entity_num);
    switch (DEVICE_ID(device)) {
    case PCI_CHIP_VMWARE0405:
    case PCI_CHIP_VMWARE0710:
        xf86MsgVerb(X_INFO, 4, "VMwarePciProbe: Valid device\n");
        scrn->PreInit = VMWAREPreInit;
        scrn->ScreenInit = VMWAREScreenInit;
        scrn->SwitchMode = VMWARESwitchMode;
        scrn->EnterVT = VMWAREEnterVT;
        scrn->LeaveVT = VMWARELeaveVT;
        scrn->FreeScreen = VMWAREFreeScreen;
        scrn->ValidMode = VMWAREValidMode;
        break;
    default:
        xf86MsgVerb(X_INFO, 4, "VMwarePciProbe: Unknown device\n");
    }
    return scrn != NULL;
}
#else 

static Bool
VMWAREProbe(DriverPtr drv, int flags)
{
    int numDevSections, numUsed;
    GDevPtr *devSections;
    int *usedChips;
    int i;
    Bool foundScreen = FALSE;
    char buildString[sizeof(VMWAREBuildStr)];

    RewriteTagString(VMWAREBuildStr, buildString, sizeof(VMWAREBuildStr));
    xf86MsgVerb(X_PROBED, 4, "%s", buildString);

    numDevSections = xf86MatchDevice(VMWARE_DRIVER_NAME, &devSections);
    if (numDevSections <= 0) {
#ifdef DEBUG
        xf86MsgVerb(X_ERROR, 0, "No vmware driver section\n");
#endif
        return FALSE;
    }
    if (xf86GetPciVideoInfo()) {
        VmwareLog(("Some PCI Video Info Exists\n"));
        numUsed = xf86MatchPciInstances(VMWARE_NAME, PCI_VENDOR_VMWARE,
                                        VMWAREChipsets, VMWAREPciChipsets, devSections,
                                        numDevSections, drv, &usedChips);
        xfree(devSections);
        if (numUsed <= 0)
            return FALSE;
        if (flags & PROBE_DETECT)
            foundScreen = TRUE;
        else
            for (i = 0; i < numUsed; i++) {
                ScrnInfoPtr pScrn = NULL;

                VmwareLog(("Even some VMware SVGA PCI instances exists\n"));
                pScrn = xf86ConfigPciEntity(pScrn, flags, usedChips[i],
                                            VMWAREPciChipsets, NULL, NULL, NULL,
                                            NULL, NULL);
                if (pScrn) {
                    VmwareLog(("And even configuration suceeded\n"));
                    pScrn->driverVersion = VMWARE_DRIVER_VERSION;
                    pScrn->driverName = VMWARE_DRIVER_NAME;
                    pScrn->name = VMWARE_NAME;
                    pScrn->Probe = VMWAREProbe;
                    pScrn->PreInit = VMWAREPreInit;
                    pScrn->ScreenInit = VMWAREScreenInit;
                    pScrn->SwitchMode = VMWARESwitchMode;
                    pScrn->AdjustFrame = VMWAREAdjustFrame;
                    pScrn->EnterVT = VMWAREEnterVT;
                    pScrn->LeaveVT = VMWARELeaveVT;
                    pScrn->FreeScreen = VMWAREFreeScreen;
                    pScrn->ValidMode = VMWAREValidMode;
                    foundScreen = TRUE;
                }
            }
        xfree(usedChips);
    }
    return foundScreen;
}
#endif


_X_EXPORT DriverRec VMWARE = {
    VMWARE_DRIVER_VERSION,
    VMWARE_DRIVER_NAME,
    VMWAREIdentify,
#if XSERVER_LIBPCIACCESS
    NULL,
#else
    VMWAREProbe,
#endif
    VMWAREAvailableOptions,
    NULL,
    0,
#if VMWARE_DRIVER_FUNC
    VMWareDriverFunc,
#endif
#if XSERVER_LIBPCIACCESS
    VMwareDeviceMatch,
    VMwarePciProbe,
#endif
};

#ifdef XFree86LOADER
static MODULESETUPPROTO(vmwareSetup);

_X_EXPORT XF86ModuleData vmwareModuleData = {
    &vmwareVersRec,
    vmwareSetup,
    NULL
};

static pointer
vmwareSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    if (!setupDone) {
        setupDone = TRUE;
        xf86AddDriver(&VMWARE, module, VMWARE_DRIVER_FUNC);

        LoaderRefSymLists(vgahwSymbols, fbSymbols, ramdacSymbols,
                          shadowfbSymbols, NULL);

        return (pointer)1;
    }
    if (errmaj) {
        *errmaj = LDR_ONCEONLY;
    }
    return NULL;
}
#endif	/* XFree86LOADER */
