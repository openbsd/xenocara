/*
 * Copyright 2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Unknown at vmware
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "compiler.h"
#include "xf86Pci.h"		/* pci */
#include "vm_device_version.h"
#include "vmware_bootstrap.h"
#include <stdint.h>

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#endif

#ifndef XSERVER_LIBPCIACCESS
#include "vm_basic_types.h"
#include "svga_reg.h"
#endif

#ifndef HAVE_XORG_SERVER_1_5_0
#include <xf86_ansic.h>
#include <xf86_libc.h>
#endif

#ifdef XSERVER_PLATFORM_BUS
#include "xf86platformBus.h"
#endif

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
 * This is the only way I know to turn a #define of an integer constant into
 * a constant string.
 */
#define VMW_INNERSTRINGIFY(s) #s
#define VMW_STRING(str) VMW_INNERSTRINGIFY(str)

#define VMWARE_NAME "vmware"
#define VMWARE_DRIVER_NAME "vmware"
#define VMWARE_DRIVER_VERSION \
   (PACKAGE_VERSION_MAJOR * 65536 + PACKAGE_VERSION_MINOR * 256 + PACKAGE_VERSION_PATCHLEVEL)
#define VMWARE_DRIVER_VERSION_STRING \
    VMW_STRING(PACKAGE_VERSION_MAJOR) "." VMW_STRING(PACKAGE_VERSION_MINOR) \
    "." VMW_STRING(PACKAGE_VERSION_PATCHLEVEL)

static const char VMWAREBuildStr[] = "VMware Guest X Server "
    VMWARE_DRIVER_VERSION_STRING " - build=$Name:  $\n";

/*
 * Standard four digit version string expected by VMware Tools installer.
 * As the driver's version is only  {major, minor, patchlevel},
 * The fourth digit may describe the commit number relative to the
 * last version tag as output from `git describe`
 */

#ifdef __GNUC__
#ifdef VMW_SUBPATCH
const char vmware_drv_modinfo[]
__attribute__((section(".modinfo"),unused)) =
  "version=" VMWARE_DRIVER_VERSION_STRING "." VMW_STRING(VMW_SUBPATCH);
#else
const char vmware_drv_modinfo[]
__attribute__((section(".modinfo"),unused)) =
  "version=" VMWARE_DRIVER_VERSION_STRING ".0";
#endif /*VMW_SUBPATCH*/
#endif

#ifndef XSERVER_LIBPCIACCESS
static resRange vmwareLegacyRes[] = {
    { ResExcIoBlock, SVGA_LEGACY_BASE_PORT,
      SVGA_LEGACY_BASE_PORT + SVGA_NUM_PORTS*sizeof(uint32)},
    _VGA_EXCLUSIVE, _END
};
#else
#define vmwareLegacyRes NULL
#endif

#if XSERVER_LIBPCIACCESS
#define VENDOR_ID(p)      (p)->vendor_id
#define DEVICE_ID(p)      (p)->device_id
#define SUBVENDOR_ID(p)   (p)->subvendor_id
#define SUBSYS_ID(p)      (p)->subdevice_id
#define CHIP_REVISION(p)  (p)->revision
#else
#define VENDOR_ID(p)      (p)->vendor
#define DEVICE_ID(p)      (p)->chipType
#define SUBVENDOR_ID(p)   (p)->subsysVendor
#define SUBSYS_ID(p)      (p)->subsysCard
#define CHIP_REVISION(p)  (p)->chipRev
#endif

#if XSERVER_LIBPCIACCESS

#define VMWARE_DEVICE_MATCH(d, i) \
    {PCI_VENDOR_ID_VMWARE, (d), PCI_MATCH_ANY, PCI_MATCH_ANY, 0, 0, (i) }

static const struct pci_id_match VMwareDeviceMatch[] = {
    VMWARE_DEVICE_MATCH (PCI_DEVICE_ID_VMWARE_SVGA2, 0 ),
    VMWARE_DEVICE_MATCH (PCI_DEVICE_ID_VMWARE_SVGA, 0 ),
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
    { PCI_DEVICE_ID_VMWARE_SVGA2, PCI_DEVICE_ID_VMWARE_SVGA2, RES_EXCLUSIVE_VGA },
    { PCI_DEVICE_ID_VMWARE_SVGA, PCI_DEVICE_ID_VMWARE_SVGA, vmwareLegacyRes },
    { -1,		       -1,		    RES_UNDEFINED }
};

static SymTabRec VMWAREChipsets[] = {
    { PCI_DEVICE_ID_VMWARE_SVGA2, "vmware0405" },
    { PCI_DEVICE_ID_VMWARE_SVGA, "vmware0710" },
    { -1,                  NULL }
};

#ifdef XFree86LOADER
static XF86ModuleVersionInfo vmwareVersRec = {
    VMWARE_DRIVER_NAME,
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    { 0, 0, 0, 0}
};
#endif	/* XFree86LOADER */

static const OptionInfoRec VMWAREOptions[] = {
    { OPTION_HW_CURSOR, "HWcursor",     OPTV_BOOLEAN,   {0},    FALSE },
    { OPTION_XINERAMA,  "Xinerama",     OPTV_BOOLEAN,   {0},    FALSE },
    { OPTION_STATIC_XINERAMA, "StaticXinerama", OPTV_STRING, {0}, FALSE },
    { OPTION_GUI_LAYOUT, "GuiLayout", OPTV_STRING, {0}, FALSE },
    { OPTION_DEFAULT_MODE, "AddDefaultMode", OPTV_BOOLEAN,   {0},    FALSE },
    { OPTION_RENDER_ACCEL, "RenderAccel", OPTV_BOOLEAN, {0}, FALSE},
    { OPTION_DRI, "DRI", OPTV_BOOLEAN, {0}, FALSE},
    { OPTION_DIRECT_PRESENTS, "DirectPresents", OPTV_BOOLEAN, {0}, FALSE},
    { OPTION_HW_PRESENTS, "HWPresents", OPTV_BOOLEAN, {0}, FALSE},
    { OPTION_RENDERCHECK, "RenderCheck", OPTV_BOOLEAN, {0}, FALSE},
    { -1,               NULL,           OPTV_NONE,      {0},    FALSE }
};

OptionInfoPtr VMWARECopyOptions(void)
{
    OptionInfoPtr options;
    if (!(options = malloc(sizeof(VMWAREOptions))))
        return NULL;

    memcpy(options, VMWAREOptions, sizeof(VMWAREOptions));
    return options;
}

/*
 * Also in vmwgfx_hosted.h, which we don't include.
 */
void *
vmwgfx_hosted_detect(void);

static Bool
VMwarePreinitStub(ScrnInfoPtr pScrn, int flags)
{
#if XSERVER_LIBPCIACCESS
    struct pci_device *pciInfo;
#else
    pciVideoPtr pciInfo;
#endif /* XSERVER_LIBPCIACCESS */
    EntityInfoPtr pEnt;

    pScrn->PreInit = pScrn->driverPrivate;

#ifdef BUILD_VMWGFX
    pScrn->driverPrivate = NULL;

    /*
     * Try vmwgfx path.
     */
    if ((*pScrn->PreInit)(pScrn, flags))
	return TRUE;

    /*
     * Can't run legacy hosted
     */
    if (vmwgfx_hosted_detect())
	return FALSE;
#else
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Driver was compiled without KMS- and 3D support.\n");
#endif /* defined(BUILD_VMWGFX) */
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "Disabling 3D support.\n");
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "Disabling Render Acceleration.\n");
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "Disabling RandR12+ support.\n");

    pScrn->driverPrivate = NULL;
    vmwlegacy_hookup(pScrn);

    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
    pciInfo = xf86GetPciInfoForEntity(pEnt->index);
    if (pciInfo == NULL)
        return FALSE;

    pScrn->chipset = (char*)xf86TokenToString(VMWAREChipsets,
					      DEVICE_ID(pciInfo));

    return (*pScrn->PreInit)(pScrn, flags);
};

#if XSERVER_LIBPCIACCESS
static Bool
VMwarePciProbe (DriverPtr           drv,
                int                 entity_num,
                struct pci_device   *device,
                intptr_t            match_data)
{
    ScrnInfoPtr     scrn = NULL;

    scrn = xf86ConfigPciEntity(scrn, 0, entity_num, VMWAREPciChipsets,
                               NULL, NULL, NULL, NULL, NULL);
    if (scrn != NULL) {
        scrn->driverVersion = VMWARE_DRIVER_VERSION;
        scrn->driverName = VMWARE_DRIVER_NAME;
        scrn->name = VMWARE_NAME;
        scrn->Probe = NULL;
    }

    switch (DEVICE_ID(device)) {
    case PCI_DEVICE_ID_VMWARE_SVGA2:
    case PCI_DEVICE_ID_VMWARE_SVGA:
        xf86MsgVerb(X_INFO, 4, "VMwarePciProbe: Valid device\n");

#ifdef BUILD_VMWGFX
	vmwgfx_hookup(scrn);
#else
	vmwlegacy_hookup(scrn);
#endif /* defined(BUILD_VMWGFX) */

	scrn->driverPrivate = scrn->PreInit;
	scrn->PreInit = VMwarePreinitStub;
        break;
    default:
        xf86MsgVerb(X_INFO, 4, "VMwarePciProbe: Unknown device\n");
    }
    return scrn != NULL;
}
#else

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
        numUsed = xf86MatchPciInstances(VMWARE_NAME, PCI_VENDOR_ID_VMWARE,
                                        VMWAREChipsets, VMWAREPciChipsets, devSections,
                                        numDevSections, drv, &usedChips);
        free(devSections);
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

#ifdef BUILD_VMWGFX
		    vmwgfx_hookup(pScrn);
#else
		    vmwlegacy_hookup(pScrn);
#endif /* defined(BUILD_VMWGFX) */

		    pScrn->driverPrivate = pScrn->PreInit;
		    pScrn->PreInit = VMwarePreinitStub;
                    foundScreen = TRUE;
                }
            }
        free(usedChips);
    }
    return foundScreen;
}
#endif

#ifdef XSERVER_PLATFORM_BUS
static Bool
VMwarePlatformProbe(DriverPtr drv, int entity, int flags,
                    struct xf86_platform_device *dev, intptr_t match_data)
{
    ScrnInfoPtr pScrn;
    int scrnFlag = 0;

    if (!dev->pdev)
        return FALSE;

    if (flags & PLATFORM_PROBE_GPU_SCREEN)
        scrnFlag = XF86_ALLOCATE_GPU_SCREEN;

    pScrn = xf86AllocateScreen(drv, scrnFlag);
    if (!pScrn)
        return FALSE;

    if (xf86IsEntitySharable(entity))
        xf86SetEntityShared(entity);

    xf86AddEntityToScreen(pScrn, entity);

    pScrn->driverVersion = VMWARE_DRIVER_VERSION;
    pScrn->driverName = VMWARE_DRIVER_NAME;
    pScrn->name = VMWARE_NAME;
    pScrn->Probe = NULL;
#ifdef BUILD_VMWGFX
    vmwgfx_hookup(pScrn);
#else
    vmwlegacy_hookup(pScrn);
#endif
    pScrn->driverPrivate = pScrn->PreInit;
    pScrn->PreInit = VMwarePreinitStub;

    return TRUE;
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

#if VMWARE_DRIVER_FUNC
static Bool
VMWareDriverFunc(ScrnInfoPtr pScrn,
                 xorgDriverFuncOp op,
                 pointer data)
{
   uint32_t *flag;
   xorgRRModeMM *modemm;

   switch (op) {
   case GET_REQUIRED_HW_INTERFACES:
      flag = (uint32_t *)data;

      if (flag) {
#ifdef BUILD_VMWGFX
	  vmwgfx_modify_flags(flag);
#else
         *flag = HW_IO | HW_MMIO;
#endif
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
	  modemm->mmWidth = (modemm->mode->HDisplay * VMWARE_INCHTOMM +
			     pScrn->xDpi / 2)  / pScrn->xDpi;
	  modemm->mmHeight = (modemm->mode->VDisplay * VMWARE_INCHTOMM +
			      pScrn->yDpi / 2) / pScrn->yDpi;
      }
      return TRUE;
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 18
   case SUPPORTS_SERVER_FDS:
      return TRUE;
#endif
   default:
      return FALSE;
   }
}
#endif


_X_EXPORT DriverRec vmware = {
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
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 4
#if XSERVER_LIBPCIACCESS
    VMwareDeviceMatch,
    VMwarePciProbe,
#else
    NULL,
    NULL,
#endif
#endif
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 13
#ifdef XSERVER_PLATFORM_BUS
    VMwarePlatformProbe,
#else
    NULL,
#endif
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

        xf86AddDriver(&vmware, module, VMWARE_DRIVER_FUNC);

        VMWARERefSymLists();

        return (pointer)1;
    }
    if (errmaj) {
        *errmaj = LDR_ONCEONLY;
    }
    return NULL;
}
#endif	/* XFree86LOADER */
