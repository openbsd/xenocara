/*
 * SiSUSB driver main code
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb.h"

#include "xf86RAC.h"
#include "dixstruct.h"
#include "shadowfb.h"
#include "fb.h"
#include "micmap.h"
#include "mibank.h"
#include "mipointer.h"
#include "mibstore.h"

#include "sisusb_regs.h"
#include "sisusb_dac.h"

#include "sisusb_driver.h"

#include "globals.h"

#define DPMS_SERVER
#include <X11/extensions/dpms.h>

/*
 * This is intentionally screen-independent.  It indicates the binding
 * choice made in the first PreInit.
 */
static int pix24bpp = 0;

/*
 * This contains the functions needed by the server after loading the driver
 * module.  It must be supplied, and gets passed back by the SetupProc
 * function in the dynamic case.  In the static case, a reference to this
 * is compiled in, and this requires that the name of this DriverRec be
 * an upper-case version of the driver name.
 */

#ifdef _X_EXPORT
_X_EXPORT
#endif
DriverRec SISUSB = {
    SISUSB_CURRENT_VERSION,
    SISUSB_DRIVER_NAME,
    SISUSBIdentify,
    SISUSBProbe,
    SISUSBAvailableOptions,
    NULL,
    0
#ifdef SISUSB_HAVE_DRIVER_FUNC
     ,
    SISUSBDriverFunc
#endif
};

static SymTabRec SISUSBChipsets[] = {
    { USB_CHIP_SIS315, "SIS315E/PRO USB" },
    { -1,              NULL }
};

static const char *fbSymbols[] = {
    "fbPictureInit",
    "fbScreenInit",
    NULL
};

static const char *shadowSymbols[] = {
    "ShadowFBInit",
    NULL
};

static const char *ramdacSymbols[] = {
    "xf86CreateCursorInfoRec",
    "xf86DestroyCursorInfoRec",
    "xf86InitCursor",
    NULL
};

#ifdef XFree86LOADER

static MODULESETUPPROTO(sisusbSetup);

static XF86ModuleVersionInfo sisVersRec =
{
    SISUSB_DRIVER_NAME,
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
#ifdef XORG_VERSION_CURRENT
    XORG_VERSION_CURRENT,
#else
    XF86_VERSION_CURRENT,
#endif
    SISUSB_MAJOR_VERSION, SISUSB_MINOR_VERSION, SISUSB_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,         /* This is a video driver */
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0,0,0,0}
};

#ifdef _X_EXPORT
_X_EXPORT
#endif
XF86ModuleData sisusbModuleData = { &sisVersRec, sisusbSetup, NULL };

pointer
sisusbSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    if(!setupDone) {
       setupDone = TRUE;
       xf86AddDriver(&SISUSB, module, SISUSB_HaveDriverFuncs);
       LoaderRefSymLists(fbSymbols, shadowSymbols, ramdacSymbols, NULL);
       return (pointer)TRUE;
    }

    if(errmaj) *errmaj = LDR_ONCEONLY;
    return NULL;
}

#endif /* XFree86LOADER */

/* Mandatory */
static void
SISUSBIdentify(int flags)
{
    xf86PrintChipsets(SISUSB_NAME, "driver for SiSUSB chipsets", SISUSBChipsets);
}

#ifdef SISUSB_HAVE_DRIVER_FUNC
static Bool
SISUSBDriverFunc(ScrnInfoPtr pScrn, xorgDriverFuncOp op, pointer ptr)
{
    xorgHWFlags *flag;

    switch(op) {
    case GET_REQUIRED_HW_INTERFACES:
	flag = (xorgHWFlags *)ptr;
	(*flag) = 0;
	return TRUE;
    default:
	return FALSE;
    }
}
#endif

static Bool
SISUSBGetRec(ScrnInfoPtr pScrn)
{
    /* Allocate an SISUSBRec, and hook it into pScrn->driverPrivate.
     * pScrn->driverPrivate is initialised to NULL, so we can check if
     * the allocation has already been done.
     */
    if(pScrn->driverPrivate != NULL) return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(SISUSBRec), 1);

    /* Initialise it to 0 */
    memset(pScrn->driverPrivate, 0, sizeof(SISUSBRec));

    return TRUE;
}

static void
SISUSBFreeRec(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    /* Just to make sure... */
    if(!pSiSUSB) return;

    if(pSiSUSB->pstate) xfree(pSiSUSB->pstate);
    pSiSUSB->pstate = NULL;
    if(pSiSUSB->fonts) xfree(pSiSUSB->fonts);
    pSiSUSB->fonts = NULL;

    if(pSiSUSB->SiS_Pr) xfree(pSiSUSB->SiS_Pr);
    pSiSUSB->SiS_Pr = NULL;

    if(pSiSUSB->sisusbdevopen) {
       close(pSiSUSB->sisusbdev);
       pSiSUSB->sisusbdevopen = FALSE;
    }

    if(pScrn->chipset) {
       xfree(pScrn->chipset);
       pScrn->chipset = NULL;
    }

    if(pScrn->driverPrivate) {
       xfree(pScrn->driverPrivate);
       pScrn->driverPrivate = NULL;
    }
}

static void
SISUSBErrorLog(ScrnInfoPtr pScrn, const char *format, ...)
{
    va_list ap;
    static const char *str = "**************************************************\n";

    va_start(ap, format);
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, str);
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
    	"                      ERROR:\n");
    xf86VDrvMsgVerb(pScrn->scrnIndex, X_ERROR, 1, format, ap);
    va_end(ap);
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
    	"                  END OF MESSAGE\n");
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, str);
}

static int
SiSUSBFindDuplicate(int myminor, int *minorArray, int numDevSections)
{
    int i;
    for(i = 0; i < numDevSections; i++) {
        if(minorArray[i] == -1) continue;
	if(minorArray[i] == myminor) return TRUE;
    }
    return FALSE;
}

static int
SiSUSBCheckForUSBDongle(char *filename, SISUSBPtr pSiSUSB, int *filehandle)
{
    int retval = -1, myfile, dontclose = 0;
    unsigned int sisusbversion;
    CARD32 sisusbinfosize;
    sisusb_info *mysisusbinfo;

    if((myfile = open(filename, O_RDWR, 0)) != -1) {
       if(!ioctl(myfile, SISUSB_GET_CONFIG_SIZE, &sisusbinfosize)) {
	  if((mysisusbinfo = xalloc(sisusbinfosize))) {
	     if(!ioctl(myfile, (SISUSB_GET_CONFIG | (sisusbinfosize << 16)), mysisusbinfo)) {
		if(mysisusbinfo->sisusb_id == SISUSB_ID) {
		   sisusbversion = (mysisusbinfo->sisusb_version  << 16) |
		   		   (mysisusbinfo->sisusb_revision <<  8) |
				    mysisusbinfo->sisusb_patchlevel;
		   if(pSiSUSB) {
		      pSiSUSB->sisusbmembase    = mysisusbinfo->sisusb_vrambase;
		      pSiSUSB->sisusbmmiobase   = mysisusbinfo->sisusb_mmiobase;
		      pSiSUSB->sisusbioportbase = mysisusbinfo->sisusb_iobase;
		      pSiSUSB->sisusbpcibase    = mysisusbinfo->sisusb_pcibase;
		      pSiSUSB->sisusbvramsize   = mysisusbinfo->sisusb_vramsize;
		      pSiSUSB->sisusbinit       = mysisusbinfo->sisusb_gfxinit;
		      pSiSUSB->sisusbversion    = mysisusbinfo->sisusb_version;
		      pSiSUSB->sisusbrevision   = mysisusbinfo->sisusb_revision;
		      pSiSUSB->sisusbpatchlevel = mysisusbinfo->sisusb_patchlevel;
		      pSiSUSB->sisusbfbactive   = 0;
		      pSiSUSB->sisusbconactive  = 0;
		      if(sisusbversion >= 0x000007) {
		         pSiSUSB->sisusbfbactive = mysisusbinfo->sisusb_fbdevactive;
		      }
		      if(sisusbversion >= 0x000008) {
		         pSiSUSB->sisusbconactive = mysisusbinfo->sisusb_conactive;
		      }
		   }
		   if(filehandle) {
		      (*filehandle) = myfile;
		      dontclose = 1;
		   }
		   retval = mysisusbinfo->sisusb_minor;
		}
	     }
	     xfree(mysisusbinfo);
	     mysisusbinfo = NULL;
	  }
       }
       if(!dontclose) close(myfile);
   }

   return retval;
}

static int
SiSUSBFindUSBDongle(GDevPtr dev, int *minorArray, int numDevSections, char **nameptr)
{
    int i, retval = -1, gotdev = 0;
    char *p;

    *nameptr = NULL;

    if((dev) && (dev->busID) && (*dev->busID)) {
       if(*dev->busID == 'U' || *dev->busID == 'u') {
          if((p = strchr(dev->busID, ':'))) p++;
	  else p = dev->busID;
       } else {
          p = dev->busID;
       }
       if((p) && (*p) && (*p == '/')) {
          gotdev = 1;
	  *nameptr = xalloc(strlen(p) + 1);
          strcpy(*nameptr, p);
	  retval = SiSUSBCheckForUSBDongle(*nameptr, NULL, NULL);
       } else if((p) && (*p) && (sscanf(p, "%d", &i) == 1)) {
          if(i >= 0 && i <= 31) {
             gotdev = 1;
	     *nameptr = xalloc(32);
             sprintf(*nameptr, "/dev/sisusbvga%d", i);
             retval = SiSUSBCheckForUSBDongle(*nameptr, NULL, NULL);
	     if(retval < 0) {
   	        sprintf(*nameptr, "/dev/usb/sisusbvga%d", i);
                retval = SiSUSBCheckForUSBDongle(*nameptr, NULL, NULL);
	     }
          }
       }
    }
    if(!gotdev) {
       *nameptr = xalloc(32);
       for(i = 0; i < 64; i++) {
          if(i < 32) sprintf(*nameptr, "/dev/sisusbvga%d", i);
	  else       sprintf(*nameptr, "/dev/usb/sisusbvga%d", i);
          if((retval = SiSUSBCheckForUSBDongle(*nameptr, NULL, NULL)) >= 0) {
	     if(!SiSUSBFindDuplicate(retval, minorArray, numDevSections)) {
	        break;
	     }
	  }
       }
    }
    if(retval >= 0) {
       xf86Msg(X_INFO, "Found SiSUSB dongle (node %s, minor %d)\n", *nameptr, retval);
    } else if((*nameptr)) {
       xfree(*nameptr);
       *nameptr = NULL;
    }

    return retval;
}

/* Mandatory */
static Bool
SISUSBProbe(DriverPtr drv, int flags)
{
    GDevPtr *devSections;
    int     i, numDevSections, numUsed, myminor;
    Bool    foundScreen = FALSE;
    int     *minorArray;
    char    **devnameArray;
    char    *nameptr;

    /*
     * The aim here is to find all cards that this driver can handle,
     * and for the ones not already claimed by another driver, claim the
     * slot, and allocate a ScrnInfoRec.
     *
     * This should be a minimal probe, and it should under no circumstances
     * change the state of the hardware.  Because a device is found, don't
     * assume that it will be used.  Don't do any initialisations other than
     * the required ScrnInfoRec initialisations.  Don't allocate any new
     * data structures.
     *
     */

    /*
     * Next we check, if there has been a chipset override in the config file.
     * For this we must find out if there is an active device section which
     * is relevant, i.e., which has no driver specified or has THIS driver
     * specified.
     */

    if((numDevSections = xf86MatchDevice(SISUSB_DRIVER_NAME, &devSections)) <= 0) {
       /*
        * There's no matching device section in the config file, so quit
        * now.
        */
       return FALSE;
    }

    /* This is to avoid referencing a null-ptr; xf86MatchDevice, strangely, returns
     * 1 sometimes (probe, doconfigure cases) with an empty device list
     */
    if(devSections == NULL) return FALSE;

    /*
     * We need to probe the hardware first.  We then need to see how this
     * fits in with what is given in the config file, and allow the config
     * file info to override any contradictions.
     */

    /*
     * Probing the USB2VGA dongle amounts to checking whether the required
     * /dev file is available and can be opened for r/w.
     */

    /* Allocate and initialize an array of ints for storing the minors */
    if(!(minorArray = (int *)xalloc(numDevSections * sizeof(int)))) {
       return FALSE;
    }
    for(i = 0; i < numDevSections; i++) minorArray[i] = -1;

    /* Allocate an array of char ptrs for storing the device node names */
    if(!(devnameArray = (char **)xalloc(numDevSections * sizeof(char *)))) {
       xfree(minorArray);
       return FALSE;
    }

    /* Go through all device sections and look for dongles. Filter out
     * duplicates, backup the device node name for further reference.
     */
    numUsed = 0;
    for(i = 0; i < numDevSections; i++) {
       if((myminor = SiSUSBFindUSBDongle(devSections[i], minorArray, numDevSections, &nameptr)) >= 0) {
          if(!SiSUSBFindDuplicate(myminor, minorArray, numDevSections)) {
	     minorArray[numUsed] = myminor;
	     devnameArray[numUsed] = xalloc(strlen(nameptr) + 1);
	     strcpy(devnameArray[numUsed], nameptr);
             numUsed++;
	  }
	  xfree(nameptr);
       }
    }

    /* Free the minor array, we don't need it anymore */
    xfree(minorArray);

    if(numUsed <= 0) {
       xfree(devSections);
       xfree(devnameArray);
       return FALSE;
    }

    if(flags & PROBE_DETECT) {

        foundScreen = TRUE;

    } else for(i = 0; i < numUsed; i++) {

        /* Allocate a ScrnInfoRec and claim the slot */
        ScrnInfoPtr pScrn = NULL;
	int entityIndex = xf86ClaimNoSlot(drv, 0, devSections[i], TRUE);

	if((pScrn = xf86AllocateScreen(drv, 0))) {
	    /* Add entity */
	    xf86AddEntityToScreen(pScrn, entityIndex);
            /* Fill in what we can of the ScrnInfoRec */
	    pScrn->chipset          = devnameArray[i];
            pScrn->driverVersion    = SISUSB_CURRENT_VERSION;
            pScrn->driverName       = SISUSB_DRIVER_NAME;
            pScrn->name             = SISUSB_NAME;
            pScrn->Probe            = SISUSBProbe;
            pScrn->PreInit          = SISUSBPreInit;
            pScrn->ScreenInit       = SISUSBScreenInit;
            pScrn->SwitchMode       = SISUSBSwitchMode;
            pScrn->AdjustFrame      = SISUSBAdjustFrame;
            pScrn->EnterVT          = SISUSBEnterVT;
            pScrn->LeaveVT          = SISUSBLeaveVT;
            pScrn->FreeScreen       = SISUSBFreeScreen;
            pScrn->ValidMode        = SISUSBValidMode;

            foundScreen = TRUE;
        }

    }

    xfree(devSections);
    xfree(devnameArray);
    return foundScreen;
}

static void
SiSUSB_SiSFB_Lock(ScrnInfoPtr pScrn, Bool lock)
{
    SISUSBPtr  pSiSUSB = SISUSBPTR(pScrn);
    int     fd;
    CARD32  parm;

    if(!pSiSUSB->sisfbfound) return;
    if(!pSiSUSB->sisfb_havelock) return;

    if((fd = open(pSiSUSB->sisfbdevname, O_RDONLY)) != -1) {
       parm = lock ? 1 : 0;
       ioctl(fd, SISUSBFB_SET_LOCK, &parm);
       close(fd);
    }
}

static void
SISUSBDisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    UChar  sr1=0, cr63=0, pmreg=0, sr7=0;
    UChar  oldpmreg=0;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 4,
          "SISUSBDisplayPowerManagementSet(%d)\n",PowerManagementMode);

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);
#endif

   switch (PowerManagementMode) {

       case DPMSModeOn:      /* HSync: On, VSync: On */
            pSiSUSB->Blank = FALSE;
            sr1   = 0x00;
	    pmreg = 0x00;
	    cr63  = 0x00;
	    sr7   = 0x10;
            break;

       case DPMSModeSuspend: /* HSync: On, VSync: Off */
            pSiSUSB->Blank = TRUE;
            sr1   = 0x20;
	    pmreg = 0x80;
	    cr63  = 0x40;
	    sr7   = 0x00;
            break;

       case DPMSModeStandby: /* HSync: Off, VSync: On */
            pSiSUSB->Blank = TRUE;
            sr1   = 0x20;
	    pmreg = 0x40;
	    cr63  = 0x40;
	    sr7   = 0x00;
            break;

       case DPMSModeOff:     /* HSync: Off, VSync: Off */
            pSiSUSB->Blank = TRUE;
            sr1   = 0x20;
	    pmreg = 0xc0;
	    cr63  = 0x40;
	    sr7   = 0x00;
	    break;

       default:
	    return;
    }

    if(!pSiSUSB->CRT1off)  {
        setSISIDXREG(pSiSUSB, SISCR, pSiSUSB->myCR63, 0xbf, cr63);
        setSISIDXREG(pSiSUSB, SISSR, 0x07, 0xef, sr7);
    }
    setSISIDXREG(pSiSUSB, SISSR, 0x01, ~0x20, sr1);    /* Set/Clear "Display On" bit */

    inSISIDXREG(pSiSUSB,SISSR, 0x1f, oldpmreg);
    if(!pSiSUSB->CRT1off) {
       setSISIDXREG(pSiSUSB, SISSR, 0x1f, 0x3f, pmreg);
    }

    oldpmreg &= 0xc0;

    if(pmreg != oldpmreg) {
       outSISIDXREG(pSiSUSB,SISSR, 0x00, 0x01);    /* Synchronous Reset */
       usleep(10000);
       outSISIDXREG(pSiSUSB,SISSR, 0x00, 0x03);    /* End Reset */
    }

}

#ifdef SISGAMMARAMP
static unsigned short
calcgammaval(int j, int nramp, float invgamma, float bri, float c)
{
    float k = (float)j;
    float nrm1 = (float)(nramp - 1);
    float con = c * nrm1 / 3.0;
    float l, v;

    if(con != 0.0) {
       l = nrm1 / 2.0;
       if(con <= 0.0) {
          k -= l;
          k *= (l + con) / l;
       } else {
          l -= 1.0;
          k -= l;
          k *= l / (l - con);
       }
       k += l;
       if(k < 0.0) k = 0.0;
    }

    if(invgamma == 1.0) {
       v = k / nrm1 * 65535.0;
    } else {
       v = pow(k / nrm1, invgamma) * 65535.0 + 0.5;
    }

    v += (bri * (65535.0 / 3.0)) ;

    if(v < 0.0) v = 0.0;
    else if(v > 65535.0) v = 65535.0;

    return (unsigned short)v;
}

static void
SISUSBCalculateGammaRamp(ScreenPtr pScreen, ScrnInfoPtr pScrn)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   int    i, j, nramp;
   UShort *ramp[3];
   float  gamma_max[3], framp;
   Bool   newmethod = FALSE;

   if(!(pSiSUSB->SiS_SD3_Flags & SiS_SD3_OLDGAMMAINUSE)) {
      newmethod = TRUE;
   } else {
      gamma_max[0] = (float)pSiSUSB->GammaBriR / 1000;
      gamma_max[1] = (float)pSiSUSB->GammaBriG / 1000;
      gamma_max[2] = (float)pSiSUSB->GammaBriB / 1000;
   }

   if(!(nramp = xf86GetGammaRampSize(pScreen))) return;

   for(i=0; i<3; i++) {
      ramp[i] = (UShort *)xalloc(nramp * sizeof(UShort));
      if(!ramp[i]) {
         if(ramp[0]) { xfree(ramp[0]); ramp[0] = NULL; }
	 if(ramp[1]) { xfree(ramp[1]); ramp[1] = NULL; }
         return;
      }
   }

   if(newmethod) {

      for(i = 0; i < 3; i++) {

         float invgamma = 0.0, bri = 0.0, con = 0.0;

         switch(i) {
         case 0: invgamma = 1. / pScrn->gamma.red;
		 bri = pSiSUSB->NewGammaBriR;
		 con = pSiSUSB->NewGammaConR;
		 break;
         case 1: invgamma = 1. / pScrn->gamma.green;
		 bri = pSiSUSB->NewGammaBriG;
		 con = pSiSUSB->NewGammaConG;
		 break;
         case 2: invgamma = 1. / pScrn->gamma.blue;
		 bri = pSiSUSB->NewGammaBriB;
                 con = pSiSUSB->NewGammaConB;
		 break;
         }

	 for(j = 0; j < nramp; j++) {
	    ramp[i][j] = calcgammaval(j, nramp, invgamma, bri, con);
	 }

      }

   } else {

      for(i = 0; i < 3; i++) {
         int fullscale = 65535 * gamma_max[i];
         float dramp = 1. / (nramp - 1);
         float invgamma = 0.0, v;

         switch(i) {
         case 0: invgamma = 1. / pScrn->gamma.red; break;
         case 1: invgamma = 1. / pScrn->gamma.green; break;
         case 2: invgamma = 1. / pScrn->gamma.blue; break;
         }

         for(j = 0; j < nramp; j++) {
            framp = pow(j * dramp, invgamma);

            v = (fullscale < 0) ? (65535 + fullscale * framp) :
	 		       fullscale * framp;
	    if(v < 0) v = 0;
	    else if(v > 65535) v = 65535;
	    ramp[i][j] = (UShort)v;
         }
      }

   }

   xf86ChangeGammaRamp(pScreen, nramp, ramp[0], ramp[1], ramp[2]);

   xfree(ramp[0]);
   xfree(ramp[1]);
   xfree(ramp[2]);
   ramp[0] = ramp[1] = ramp[2] = NULL;
}
#endif

static Bool
SiSUSBMakeOwnModeList(ScrnInfoPtr pScrn, Bool acceptcustommodes, Bool includelcdmodes,
                   Bool isfordvi, Bool *havecustommodes, Bool fakecrt2modes)
{
    DisplayModePtr tempmode, delmode, mymodes;

    if((mymodes = SiSUSBBuildBuiltInModeList(pScrn, includelcdmodes, isfordvi, fakecrt2modes))) {
       if(!acceptcustommodes) {
	  while(pScrn->monitor->Modes)
             xf86DeleteMode(&pScrn->monitor->Modes, pScrn->monitor->Modes);
	  pScrn->monitor->Modes = mymodes;
       } else {
	  delmode = pScrn->monitor->Modes;
	  while(delmode) {
	     if(delmode->type & M_T_DEFAULT) {
	        tempmode = delmode->next;
	        xf86DeleteMode(&pScrn->monitor->Modes, delmode);
	        delmode = tempmode;
	     } else {
	        delmode = delmode->next;
	     }
	  }
	  /* Link default modes AFTER user ones */
	  if((tempmode = pScrn->monitor->Modes)) {
	     *havecustommodes = TRUE;
	     while(tempmode) {
	        if(!tempmode->next) break;
	        else tempmode = tempmode->next;
	     }
	     tempmode->next = mymodes;
	     mymodes->prev = tempmode;
	  } else {
	     pScrn->monitor->Modes = mymodes;
	  }
       }
       return TRUE;
    } else
       return FALSE;
}

void SISUSBSaveDetectedDevices(ScrnInfoPtr pScrn)
{
    SISUSBPtr  pSiSUSB = SISUSBPTR(pScrn);
    /* Backup detected CRT2 devices */
    pSiSUSB->detectedCRT2Devices = pSiSUSB->VBFlags & (CRT2_LCD|CRT2_TV|CRT2_VGA|TV_AVIDEO|TV_SVIDEO|
                                                 TV_SCART|TV_HIVISION|TV_YPBPR);
}

/* Mandatory */
static Bool
SISUSBPreInit(ScrnInfoPtr pScrn, int flags)
{
    SISUSBPtr pSiSUSB;
    MessageType from;
    UChar srlockReg, crlockReg;
    unsigned int i;
    int pix24flags;
    ClockRangePtr clockRanges;

    if(flags & PROBE_DETECT) {
       return TRUE;
    }

    /*
     * Note: This function is only called once at server startup, and
     * not at the start of each server generation.  This means that
     * only things that are persistent across server generations can
     * be initialised here.  xf86Screens[] is the array of all screens,
     * (pScrn is a pointer to one of these).  Privates allocated using
     * xf86AllocateScrnInfoPrivateIndex() are too, and should be used
     * for data that must persist across server generations.
     *
     * Per-generation data should be allocated with
     * AllocateScreenPrivateIndex() from the ScreenInit() function.
     */

    /* Check the number of entities, and fail if it isn't one. */
    if(pScrn->numEntities != 1) {
       SISUSBErrorLog(pScrn, "Number of entities is not 1\n");
       return FALSE;
    }

    /* Due to the liberal license terms this is needed for
     * keeping the copyright notice readable and intact in
     * binary distributions. Removing this is a copyright
     * infringement. Please read the license terms above.
     */

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
        "SiSUSB driver (%d/%02d/%02d-%d, compiled for " SISUSBMYSERVERNAME " %d.%d.%d.%d)\n",
	SISUSBDRIVERVERSIONYEAR + 2000, SISUSBDRIVERVERSIONMONTH,
	SISUSBDRIVERVERSIONDAY, SISUSBDRIVERREVISION,
#ifdef XORG_VERSION_CURRENT
 	XORG_VERSION_MAJOR, XORG_VERSION_MINOR,
	XORG_VERSION_PATCH, XORG_VERSION_SNAP
#else
	XF86_VERSION_MAJOR, XF86_VERSION_MINOR,
	XF86_VERSION_PATCH, XF86_VERSION_SNAP
#endif
	);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	"Copyright (C) 2001-2005 Thomas Winischhofer <thomas@winischhofer.net>\n");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
        "*** See http://www.winischhofer.at/linuxsisusbvga.shtml\n");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	"*** for documentation and updates.\n");

#ifdef XORG_VERSION_CURRENT
#if 0  /* no prototype yet */
    if(xorgGetVersion() != XORG_VERSION_CURRENT) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
         "This driver binary is not compiled for this version of " SISUSBMYSERVERNAME "\n");
    }
#endif
#else
#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,2,99,0)
    if(xf86GetVersion() != XF86_VERSION_CURRENT) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
         "This driver binary is not compiled for this version of " SISUSBMYSERVERNAME "\n");
    }
#endif
#endif

    /* Allocate the SISUSBRec driverPrivate */
    if(!SISUSBGetRec(pScrn)) {
       SISUSBErrorLog(pScrn, "Could not allocate memory for pSiSUSB private\n");
       return FALSE;
    }
    pSiSUSB = SISUSBPTR(pScrn);
    pSiSUSB->pScrn = pScrn;

    /* Get the entity */
    pSiSUSB->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

    if(!(pScrn->chipset)) {
       SISUSBErrorLog(pScrn, "Internal error: dev node name not found!\n");
       SISUSBFreeRec(pScrn);
       return FALSE;
    }

    pSiSUSB->sisusbfatalerror = 0;
    pSiSUSB->timeout = 0;

    if(SiSUSBCheckForUSBDongle(pScrn->chipset, pSiSUSB, &pSiSUSB->sisusbdev) < 0) {
       SISUSBErrorLog(pScrn, "Failed to open %d for read/write\n", pScrn->chipset);
       SISUSBFreeRec(pScrn);
       return FALSE;
    }
    pSiSUSB->sisusbdevopen = TRUE;
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
       		"Found USB dongle (device %s, kernel driver %d.%d.%d)\n",
		pScrn->chipset, pSiSUSB->sisusbversion, pSiSUSB->sisusbrevision,
		pSiSUSB->sisusbpatchlevel);

    /* Operations for which memory access is required */
    /* USB2VGA: We never need memory or i/o access */
    pScrn->racMemFlags = 0;
    pScrn->racIoFlags = 0;

    /* Load ramdac module */
    if(!xf86LoadSubModule(pScrn, "ramdac")) {
       SISUSBErrorLog(pScrn, "Could not load ramdac module\n");
       SISUSBFreeRec(pScrn);
       return FALSE;
    }

    xf86LoaderReqSymLists(ramdacSymbols, NULL);

    /* Set pScrn->monitor */
    pScrn->monitor = pScrn->confScreen->monitor;

    /* Set Chipset type and revision */
    pSiSUSB->Chipset = USB_CHIP_SIS315;
    pSiSUSB->ChipRev = 0;
    pSiSUSB->ChipType = SIS_315PRO;

    /* Always do a ValidMode() inside Switchmode() */
    pSiSUSB->skipswitchcheck = FALSE;

    /* Determine chipset and VGA engine type */
    pSiSUSB->ChipFlags = 0;
    pSiSUSB->SiS_SD_Flags = pSiSUSB->SiS_SD2_Flags = 0;
    pSiSUSB->SiS_SD3_Flags = pSiSUSB->SiS_SD4_Flags = 0;
    pSiSUSB->HWCursorMBufNum = pSiSUSB->HWCursorCBufNum = 0;
    pSiSUSB->NeedFlush = FALSE;

    pSiSUSB->VGAEngine = SIS_315_VGA;
    pSiSUSB->ChipFlags |= (SiSCF_Is315USB | SiSCF_315Core | SiSCF_MMIOPalette);
    pSiSUSB->SiS_SD_Flags |= SiS_SD_IS315SERIES;
    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_SUPPORTXVHUESAT;
    pSiSUSB->myCR63 = 0x63;
    pSiSUSB->mmioSize = 128;

    /* Now check if sisusbfb is active */

    pSiSUSB->OldMode = 0;
    pSiSUSB->sisfbfound = FALSE;
    pSiSUSB->sisfbdevname[0] = 0;
    pSiSUSB->sisfb_havelock = FALSE;

    {

       sisusbfb_info *mysisfbinfo = NULL;
       CARD32     sisfbinfosize = 0, sisfbversion;
       int        fd, i;
       char       name[16];

       i=0;
       do {

	  if(i <= 7) {
             sprintf(name, "/dev/fb%1d", i);
	  } else {
	     sprintf(name, "/dev/fb/%1d", i-8);
	  }

          if((fd = open(name, O_RDONLY)) != -1) {

	     Bool gotit = FALSE;

	     if(!ioctl(fd, SISUSBFB_GET_INFO_SIZE, &sisfbinfosize)) {
		if((mysisfbinfo = xalloc(sisfbinfosize))) {
		   if(!ioctl(fd, (SISUSBFB_GET_INFO | (sisfbinfosize << 16)), mysisfbinfo)) {
		      gotit = TRUE;
		   } else {
		      xfree(mysisfbinfo);
		      mysisfbinfo = NULL;
		   }
		}
	     }

	     if(gotit) {

	        if(mysisfbinfo->sisusbfb_id == SISUSBFB_ID) {

		   sisfbversion = (mysisfbinfo->sisusbfb_version << 16) |
		                  (mysisfbinfo->sisusbfb_revision << 8) |
			          (mysisfbinfo->sisusbfb_patchlevel);


		   if(mysisfbinfo->sisusbfb_minor  == pSiSUSB->sisusb_minor) {

		      pSiSUSB->sisfbfound = TRUE;

		      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				"%s: SiSUSB kernel fb driver (sisusbfb) %d.%d.%d detected\n",
				&name[5],
				mysisfbinfo->sisusbfb_version,
				mysisfbinfo->sisusbfb_revision,
				mysisfbinfo->sisusbfb_patchlevel);

		      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				"sisusbfb: using video mode 0x%02x\n", mysisfbinfo->fbvidmode);
		      pSiSUSB->OldMode = mysisfbinfo->fbvidmode;

		      pSiSUSB->sisfb_havelock = TRUE;

		   }
	        }
		xfree(mysisfbinfo);
		mysisfbinfo = NULL;
	     }
	     close (fd);
          }
	  i++;
       } while((i <= 15) && (!pSiSUSB->sisfbfound));

       if(pSiSUSB->sisfbfound) {
          strncpy(pSiSUSB->sisfbdevname, name, 15);
       } else {
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "sisusbfb not found\n");
       }
    }

    /*
     * The first thing we should figure out is the depth, bpp, etc.
     * Additionally, determine the size of the HWCursor memory area.
     */
    pSiSUSB->CursorSize = 16384;
    pix24flags = Support32bppFb;

    /* Save the name of our Device section for SiSCtrl usage */
    {
       int ttt = 0;
       GDevPtr device = xf86GetDevFromEntity(pScrn->entityList[0],
						pScrn->entityInstanceList[0]);
       if(device && device->identifier) {
          if((ttt = strlen(device->identifier)) > 31) ttt = 31;
	  strncpy(&pSiSUSB->devsectname[0], device->identifier, 31);
       }
       pSiSUSB->devsectname[ttt] = 0;
    }

    pSiSUSB->ForceCursorOff = FALSE;

    /* Allocate SiS_Private (for mode switching code) and initialize it */

    if(!(pSiSUSB->SiS_Pr = xnfcalloc(sizeof(SiS_Private), 1))) {
       SISUSBErrorLog(pScrn, "Could not allocate memory for SiS_Pr structure\n");
       SISUSBFreeRec(pScrn);
       return FALSE;
    }
    memset(pSiSUSB->SiS_Pr, 0, sizeof(SiS_Private));

    pSiSUSB->SiS_Pr->ChipType = pSiSUSB->ChipType;
    pSiSUSB->SiS_Pr->ChipRevision = pSiSUSB->ChipRev;
    pSiSUSB->SiS_Pr->CRT1UsesCustomMode = FALSE;
    pSiSUSB->SiS_Pr->SiS_MyCR63 = pSiSUSB->myCR63;
    pSiSUSB->SiS_Pr->pSiSUSB = (void *)pSiSUSB;

    /* Get our relocated IO registers */
    pSiSUSB->RelIO = (SISIOADDRESS)pSiSUSB->sisusbioportbase;
    pSiSUSB->SiS_Pr->IOAddress = (SISIOADDRESS)(pSiSUSB->RelIO + 0x30);

    /* Initialize SiS Port Reg definitions for externally used
     * sisusb_init.c routines.
     */
    SiSUSBRegInit(pSiSUSB->SiS_Pr, pSiSUSB->RelIO + 0x30);

    if(!xf86SetDepthBpp(pScrn, 0, 0, 0, pix24flags)) {
       SISUSBErrorLog(pScrn, "xf86SetDepthBpp() error\n");
       SISUSBFreeRec(pScrn);
       return FALSE;
    }

    /* Check that the returned depth is one we support */
    switch(pScrn->depth) {
       case 8:
       case 16:
       case 24:
          break;
       default:
	  SISUSBErrorLog(pScrn,
		"Given color depth (%d) is not supported by this driver/chipset\n",
		pScrn->depth);
	  SISUSBFreeRec(pScrn);
	  return FALSE;
    }

    xf86PrintDepthBpp(pScrn);

    if(pScrn->bitsPerPixel == 24) {
       SISUSBErrorLog(pScrn,
            "Framebuffer bpp %d not supported for this chipset\n", pScrn->bitsPerPixel);
       SISUSBFreeRec(pScrn);
       return FALSE;
    }

    /* Get the depth24 pixmap format */
    if(pScrn->depth == 24 && pix24bpp == 0) {
       pix24bpp = xf86GetBppFromDepth(pScrn, 24);
    }

    /*
     * This must happen after pScrn->display has been set because
     * xf86SetWeight references it.
     */
    if(pScrn->depth > 8) {
        /* The defaults are OK for us */
        rgb zeros = {0, 0, 0};

        if(!xf86SetWeight(pScrn, zeros, zeros)) {
	    SISUSBErrorLog(pScrn, "xf86SetWeight() error\n");
	    SISUSBFreeRec(pScrn);
            return FALSE;
        } else {
           Bool ret = FALSE;
           switch(pScrn->depth) {
	   case 16:
	      if((pScrn->weight.red != 5) ||
	         (pScrn->weight.green != 6) ||
		 (pScrn->weight.blue != 5)) ret = TRUE;
	      break;
	   case 24:
	      if((pScrn->weight.red != 8) ||
	         (pScrn->weight.green != 8) ||
		 (pScrn->weight.blue != 8)) ret = TRUE;
	      break;
           }
	   if(ret) {
	      SISUSBErrorLog(pScrn,
	      	   "RGB weight %d%d%d at depth %d not supported by hardware\n",
		   (int)pScrn->weight.red, (int)pScrn->weight.green,
		   (int)pScrn->weight.blue, pScrn->depth);
	      SISUSBFreeRec(pScrn);
              return FALSE;
	   }
        }
    }

    /* Set the current layout parameters */
    pSiSUSB->CurrentLayout.bitsPerPixel = pScrn->bitsPerPixel;
    pSiSUSB->CurrentLayout.depth        = pScrn->depth;
    /* (Inside this function, we can use pScrn's contents anyway) */

    if(!xf86SetDefaultVisual(pScrn, -1)) {
       SISUSBErrorLog(pScrn, "xf86SetDefaultVisual() error\n");
       SISUSBFreeRec(pScrn);
       return FALSE;
    } else {
       /* We don't support DirectColor at > 8bpp */
       if(pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
          SISUSBErrorLog(pScrn,
	       "Given default visual (%s) is not supported at depth %d\n",
               xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	  SISUSBFreeRec(pScrn);
          return FALSE;
       }
    }

    /* Unlock registers */
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, &srlockReg, &crlockReg);

    /* Evaluate options */
    SiSUSBOptions(pScrn);

    /* We use a programamble clock */
    pScrn->progClock = TRUE;

    /* Set the bits per RGB for 8bpp mode */
    if(pScrn->depth == 8) pScrn->rgbBits = 8;

    /* Do some HW configuration detection (memory amount & type, clock, etc) */
    SiSUSBSetup(pScrn);

    /* Get framebuffer address */
    pSiSUSB->FbAddress = pSiSUSB->sisusbmembase;
    pSiSUSB->realFbAddress = pSiSUSB->FbAddress;

    /* Get MMIO address */
    pSiSUSB->IOAddress = pSiSUSB->sisusbmmiobase;

    pSiSUSB->RealVideoRam = pScrn->videoRam;

    pSiSUSB->FbMapSize = pSiSUSB->availMem = pScrn->videoRam * 1024;

    /* Calculate real availMem according to Accel/TurboQueue and
     * HWCursur setting. Also, initialize some variables used
     * in other modules.
     */
    pSiSUSB->cursorOffset = 0;
    pSiSUSB->CurARGBDest = NULL;
    pSiSUSB->CurMonoSrc = NULL;
    pSiSUSB->CurFGCol = pSiSUSB->CurBGCol = 0;

#ifdef SISVRAMQ
    pSiSUSB->cmdQueueSizeMask = pSiSUSB->cmdQueueSize - 1;	/* VRAM Command Queue is variable (in therory) */
    pSiSUSB->cmdQueueOffset = (pScrn->videoRam * 1024) - pSiSUSB->cmdQueueSize;
    pSiSUSB->cmdQueueLen = 0;
    pSiSUSB->cmdQueueLenMin = 0x200;
    pSiSUSB->cmdQueueLenMax = pSiSUSB->cmdQueueSize - pSiSUSB->cmdQueueLenMin;
    pSiSUSB->cmdQueueSize_div2 = pSiSUSB->cmdQueueSize / 2;
    pSiSUSB->cmdQueueSize_div4 = pSiSUSB->cmdQueueSize / 4;
    pSiSUSB->cmdQueueSize_4_3 = (pSiSUSB->cmdQueueSize / 4) * 3;
    pSiSUSB->availMem -= pSiSUSB->cmdQueueSize;
    pSiSUSB->cursorOffset = (pSiSUSB->cmdQueueSize / 1024);
#else
    pSiSUSB->availMem -= (512*1024);  			/* MMIO Command Queue is 512k (variable in theory) */
    pSiSUSB->cursorOffset = 512;
#endif

    if(pSiSUSB->HWCursor) {
       if(!(pSiSUSB->USBCursorBuf = xcalloc(pSiSUSB->CursorSize * 4, 1))) pSiSUSB->HWCursor = FALSE;

       pSiSUSB->availMem -= (pSiSUSB->CursorSize * 2);
       if(pSiSUSB->OptUseColorCursor) pSiSUSB->availMem -= (pSiSUSB->CursorSize * 2);

    }
    pSiSUSB->cursorBufferNum = 0;

    pSiSUSB->maxxfbmem = pSiSUSB->availMem;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using %ldK of framebuffer memory\n",
    				pSiSUSB->maxxfbmem / 1024);

    /* Find out about sub-classes of some chipsets and check
     * if the chipset supports two video overlays
     */
    pSiSUSB->hasTwoOverlays = FALSE;
    pSiSUSB->ChipFlags |= SiSCF_LARGEOVERLAY;

    /* Backup VB connection and CRT1 on/off register */

    inSISIDXREG(pSiSUSB,SISSR, 0x1f, pSiSUSB->oldSR1F);
    inSISIDXREG(pSiSUSB,SISCR, 0x17, pSiSUSB->oldCR17);
    inSISIDXREG(pSiSUSB,SISCR, 0x32, pSiSUSB->oldCR32);
    inSISIDXREG(pSiSUSB,SISCR, 0x36, pSiSUSB->oldCR36);
    inSISIDXREG(pSiSUSB,SISCR, 0x37, pSiSUSB->oldCR37);
    inSISIDXREG(pSiSUSB,SISCR, pSiSUSB->myCR63, pSiSUSB->oldCR63);
    pSiSUSB->postVBCR32 = pSiSUSB->oldCR32;

    pSiSUSB->CRT1off = 0;

    /* Detect video bridge and sense TV/VGA2 */
    SISUSBVGAPreInit(pScrn);

    /* Setup SD flags */
    pSiSUSB->SiS_SD_Flags |= SiS_SD_ADDLSUPFLAG;
    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_USEVBFLAGS2;
    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_VBINVB2ONLY;
    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_HAVESD34;
    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_NEWGAMMABRICON;

    /* Backup detected CRT2 devices */
    SISUSBSaveDetectedDevices(pScrn);

    /* Setup gamma (the cmap layer needs this to be initialised) */
    /* (Do this after evaluating options) */
    {
       Gamma zeros = {0.0, 0.0, 0.0};
       xf86SetGamma(pScrn, zeros);
    }

       xf86DrvMsg(pScrn->scrnIndex, pSiSUSB->CRT1gammaGiven ? X_CONFIG : X_INFO,
	     "Gamma correction is %s\n",
	     pSiSUSB->CRT1gamma ? "enabled" : "disabled");

#ifdef SIS_GLOBAL_ENABLEXV
       if(!(pSiSUSB->NoXvideo)) {
          xf86DrvMsg(pScrn->scrnIndex, pSiSUSB->XvGammaGiven ? X_CONFIG : X_INFO,
		"Separate Xv gamma correction is %s\n",
		pSiSUSB->XvGamma ? "enabled" : "disabled");
	  if(pSiSUSB->XvGamma) {
	     xf86DrvMsg(pScrn->scrnIndex, pSiSUSB->XvGammaGiven ? X_CONFIG : X_INFO,
	        "Xv gamma correction: %.3f %.3f %.3f\n",
		(float)((float)pSiSUSB->XvGammaRed / 1000),
		(float)((float)pSiSUSB->XvGammaGreen / 1000),
		(float)((float)pSiSUSB->XvGammaBlue / 1000));
	     if(!pSiSUSB->CRT1gamma) {
	        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Xv gamma correction requires gamma correction enabled\n");
	     }
	  }
       }
#else
       pSiSUSB->XvGamma = FALSE;
#endif

    pSiSUSB->CRT1changed = FALSE;
    pSiSUSB->newCR32 = pSiSUSB->postVBCR32;

    pSiSUSB->CRT1off = 0;
    pSiSUSB->XvOnCRT2 = FALSE;

    pSiSUSB->VBFlags |= (VB_DISPMODE_SINGLE | DISPTYPE_CRT1);

    /* Init ptrs for Save/Restore functions and calc MaxClock */
    SISUSBDACPreInit(pScrn);

    /* ********** end of VBFlags setup ********** */

    /* VBFlags are initialized now. Back them up for SlaveMode modes. */
    pSiSUSB->VBFlags_backup = pSiSUSB->VBFlags;

    /* Backup CR32,36,37 (in order to write them back after a VT switch) */
    inSISIDXREG(pSiSUSB,SISCR,0x32,pSiSUSB->myCR32);
    inSISIDXREG(pSiSUSB,SISCR,0x36,pSiSUSB->myCR36);
    inSISIDXREG(pSiSUSB,SISCR,0x37,pSiSUSB->myCR37);

    /* Note: Do not use availMem for anything from now. Use
     * maxxfbmem instead. (availMem does not take dual head
     * mode into account.)
     */

    pSiSUSB->DRIheapstart = pSiSUSB->DRIheapend = 0;

    /* From here, we mainly deal with clocks and modes */

    /* Set the min pixel clock */
    pSiSUSB->MinClock = 10000;

    xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT, "Min pixel clock is %d MHz\n",
                pSiSUSB->MinClock / 1000);

    /* If the user has specified ramdac speed in the config
     * file, we respect that setting.
     */
    from = X_PROBED;
    if(pSiSUSB->pEnt->device->dacSpeeds[0]) {
       int speed = 0;
       switch(pScrn->bitsPerPixel) {
       case 8:  speed = pSiSUSB->pEnt->device->dacSpeeds[DAC_BPP8];
                break;
       case 16: speed = pSiSUSB->pEnt->device->dacSpeeds[DAC_BPP16];
                break;
       case 24: speed = pSiSUSB->pEnt->device->dacSpeeds[DAC_BPP24];
                break;
       case 32: speed = pSiSUSB->pEnt->device->dacSpeeds[DAC_BPP32];
                break;
       }
       if(speed == 0) pSiSUSB->MaxClock = pSiSUSB->pEnt->device->dacSpeeds[0];
       else           pSiSUSB->MaxClock = speed;
       from = X_CONFIG;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Max pixel clock is %d MHz\n",
                pSiSUSB->MaxClock / 1000);

    /*
     * Setup the ClockRanges, which describe what clock ranges are available,
     * and what sort of modes they can be used for.
     */
    clockRanges = xnfcalloc(sizeof(ClockRange), 1);
    clockRanges->next = NULL;
    clockRanges->minClock = pSiSUSB->MinClock;
    clockRanges->maxClock = pSiSUSB->MaxClock;
    clockRanges->clockIndex = -1;               /* programmable */
    clockRanges->interlaceAllowed = TRUE;
    clockRanges->doubleScanAllowed = TRUE;

    /*
     * Since we have lots of built-in modes for 300/315/330/340 series
     * with vb support, we replace the given default mode list with our
     * own. In case the video bridge is to be used, we only allow other
     * modes if
     *   -) vbtype is 301, 301B, 301C or 302B, and
     *   -) crt2 device is not TV, and
     *   -) crt1 is not LCDA, unless bridge is TMDS/LCDA capable (301C)
     */
    {
       if(!(pSiSUSB->noInternalModes)) {
          Bool acceptcustommodes = TRUE;  /* Accept user modelines */
	  Bool includelcdmodes   = FALSE; /* Include modes reported by DDC */
	  Bool isfordvi          = FALSE; /* Is for digital DVI output */

	  pSiSUSB->HaveCustomModes = FALSE;
          if(SiSUSBMakeOwnModeList(pScrn, acceptcustommodes, includelcdmodes,
	  		isfordvi, &pSiSUSB->HaveCustomModes, FALSE)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	         "Replaced %s mode list with built-in modes\n",
	     pSiSUSB->HaveCustomModes ? "default" : "entire");
          } else {
	     xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	   	"Building list of built-in modes failed, using server defaults\n");
	  }
       } else {
          pSiSUSB->HaveCustomModes = TRUE;
       }
    }


    /*
     * xf86ValidateModes will check that the mode HTotal and VTotal values
     * don't exceed the chipset's limit if pScrn->maxHValue and
     * pScrn->maxVValue are set.  Since our SISValidMode() already takes
     * care of this, we don't worry about setting them here.
     */

    /* Select valid modes from those available */
    /*
     * Assuming min pitch 256, min height 128
     */
    {
       int minpitch, maxpitch, minheight, maxheight;

       minpitch = 256;
       minheight = 128;
       maxpitch = 4088;
       maxheight = 4096;

       i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
                      pScrn->display->modes, clockRanges, NULL,
                      minpitch, maxpitch,
                      pScrn->bitsPerPixel * 8,
		      minheight, maxheight,
                      pScrn->display->virtualX,
                      pScrn->display->virtualY,
                      pSiSUSB->maxxfbmem,
                      LOOKUP_BEST_REFRESH);
    }

    if(i == -1) {
       SISUSBErrorLog(pScrn, "xf86ValidateModes() error\n");
       sisusbRestoreExtRegisterLock(pSiSUSB,srlockReg,crlockReg);
       SISUSBFreeRec(pScrn);
       return FALSE;
    }

    /* Check the virtual screen against the available memory */
    {
       ULong memreq = (pScrn->virtualX * ((pScrn->bitsPerPixel + 7) / 8)) * pScrn->virtualY;

       if(memreq > pSiSUSB->maxxfbmem) {
          SISUSBErrorLog(pScrn,
       	     "Virtual screen too big for memory; %ldK needed, %ldK available\n",
	     memreq/1024, pSiSUSB->maxxfbmem/1024);
          sisusbRestoreExtRegisterLock(pSiSUSB,srlockReg,crlockReg);
          SISUSBFreeRec(pScrn);
          return FALSE;
       }
    }

    /* Prune the modes marked as invalid */
    xf86PruneDriverModes(pScrn);

    if(i == 0 || pScrn->modes == NULL) {
       SISUSBErrorLog(pScrn, "No valid modes found - check VertRefresh/HorizSync\n");
       sisusbRestoreExtRegisterLock(pSiSUSB,srlockReg,crlockReg);
       SISUSBFreeRec(pScrn);
       return FALSE;
    }

    xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);

    /* Set the current mode to the first in the list */
    pScrn->currentMode = pScrn->modes;

    /* Copy to CurrentLayout */
    pSiSUSB->CurrentLayout.mode = pScrn->currentMode;
    pSiSUSB->CurrentLayout.displayWidth = pScrn->displayWidth;
    pSiSUSB->CurrentLayout.displayHeight = pScrn->virtualY;

    /* Print the list of modes being used */
    xf86PrintModes(pScrn);

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    /* Load fb module */
    switch(pScrn->bitsPerPixel) {
      case 8:
      case 16:
      case 24:
      case 32:
	if(!xf86LoadSubModule(pScrn, "fb")) {
           SISUSBErrorLog(pScrn, "Failed to load fb module");
	   sisusbRestoreExtRegisterLock(pSiSUSB,srlockReg,crlockReg);
           SISUSBFreeRec(pScrn);
           return FALSE;
        }
	break;
      default:
        SISUSBErrorLog(pScrn, "Unsupported framebuffer bpp (%d)\n", pScrn->bitsPerPixel);
	sisusbRestoreExtRegisterLock(pSiSUSB,srlockReg,crlockReg);
        SISUSBFreeRec(pScrn);
        return FALSE;
    }
    xf86LoaderReqSymLists(fbSymbols, NULL);

    /* Load shadowfb (if needed) */
    if(pSiSUSB->ShadowFB) {
       if(!xf86LoadSubModule(pScrn, "shadowfb")) {
	  SISUSBErrorLog(pScrn, "Could not load shadowfb module\n");
	  sisusbRestoreExtRegisterLock(pSiSUSB,srlockReg,crlockReg);
	  SISUSBFreeRec(pScrn);
          return FALSE;
       }
       xf86LoaderReqSymLists(shadowSymbols, NULL);
    }

    pSiSUSB->UseVESA = 0;

    sisusbRestoreExtRegisterLock(pSiSUSB,srlockReg,crlockReg);

    pSiSUSB->SiS_SD_Flags |= SiS_SD_SUPPORTXVGAMMA1;

    if(pSiSUSB->enablesisctrl) pSiSUSB->SiS_SD_Flags |= SiS_SD_ENABLED;

    pSiSUSB->currentModeLast = pScrn->currentMode;
    pSiSUSB->VBFlagsInit = pSiSUSB->VBFlags;

    return TRUE;
}


/*
 * Map the framebuffer and MMIO memory
 */

static Bool
SISUSBMapMem(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    pSiSUSB->FbBase = (UChar *)pSiSUSB->FbAddress;
    pSiSUSB->IOBase = (UChar *)pSiSUSB->IOAddress;

    return TRUE;
}


/*
 * Unmap the framebuffer and MMIO memory.
 */

static Bool
SISUSBUnmapMem(ScrnInfoPtr pScrn)
{
    return TRUE;
}

/*
 * This function saves the video state.
 */
static void
SISUSBSave(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    SISUSBRegPtr sisReg = &pSiSUSB->SavedReg;
    int flags = SISVGA_SR_CMAP | SISVGA_SR_MODE;

    SiSUSBVGASave(pScrn, sisReg, flags);

    sisusbSaveUnlockExtRegisterLock(pSiSUSB,&sisReg->sisRegs3C4[0x05],&sisReg->sisRegs3D4[0x80]);

    (*pSiSUSB->SiSSave)(pScrn, sisReg);

    /* "Save" these again as they may have been changed prior to SISUSBSave() call */

    sisReg->sisRegs3C4[0x1f] = pSiSUSB->oldSR1F;
    sisReg->sisRegs3D4[0x17] = pSiSUSB->oldCR17;
    sisReg->sisRegs3D4[0x32] = pSiSUSB->oldCR32;
    sisReg->sisRegs3D4[0x36] = pSiSUSB->oldCR36;
    sisReg->sisRegs3D4[0x37] = pSiSUSB->oldCR37;
    sisReg->sisRegs3D4[pSiSUSB->myCR63] = pSiSUSB->oldCR63;
}

/*
 * Initialise a new mode.
 */
static Bool
SISUSBModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    /* Notify kernel driver that we are about
     * to destoy the text console
     */
    if(pSiSUSB->sisusbconactive) {
       sisrestoredestroyconsole(pSiSUSB, 1);
    }

    andSISIDXREG(pSiSUSB,SISCR,0x11,0x7f);   	/* Unlock CRTC registers */

    SISUSBModifyModeInfo(mode);		/* Quick check of the mode parameters */

    SiSUSBRegInit(pSiSUSB->SiS_Pr, pSiSUSB->RelIO+0x30);

    if(!(*pSiSUSB->ModeInit)(pScrn, mode)) {
       SISUSBErrorLog(pScrn, "ModeInit() failed\n");
       return FALSE;
    }

    pScrn->vtSema = TRUE;

    SiSUSBPreSetMode(pScrn, mode, SIS_MODE_SIMU);

    if(!SiSUSBBIOSSetMode(pSiSUSB->SiS_Pr, pScrn, mode, pSiSUSB->IsCustom)) {
       SISUSBErrorLog(pScrn, "SiSUSBBIOSSetMode() failed\n");
       return FALSE;
    }

    SiSUSBPostSetMode(pScrn, &pSiSUSB->ModeReg);

#ifdef TWDEBUG
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"REAL REGISTER CONTENTS AFTER SETMODE:\n");
    (*pSiSUSB->ModeInit)(pScrn, mode);
#endif

    /* Update Currentlayout */
    pSiSUSB->CurrentLayout.mode = pSiSUSB->currentModeLast = mode;

    return TRUE;
}

/*
 * Restore the initial mode. To be used internally only!
 */
static void
SISUSBRestore(ScrnInfoPtr pScrn)
{
    SISUSBPtr    pSiSUSB = SISUSBPTR(pScrn);
    SISUSBRegPtr sisReg = &pSiSUSB->SavedReg;
    int	         flags;

#if 0
    /* Wait for the accelerators */
    if(!pSiSUSB->NoAccel) {
       SiSUSBSync(pScrn);
    }
#endif

    /* Clear video RAM if sisusbfb not active */
    if(!pSiSUSB->sisfbfound && !pSiSUSB->sisusbfbactive) {
       sisclearvram(pSiSUSB, pSiSUSB->FbBase, pSiSUSB->maxxfbmem);
    }

    /* Set up restore flags */
    flags = SISVGA_SR_MODE | SISVGA_SR_CMAP;

    SiSUSBVGAProtect(pScrn, TRUE);

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL,NULL);
#endif

    /* First, restore CRT1 on/off and VB connection registers */
    outSISIDXREG(pSiSUSB,SISCR, 0x32, pSiSUSB->oldCR32);
    outSISIDXREG(pSiSUSB,SISCR, 0x17, pSiSUSB->oldCR17);
    outSISIDXREG(pSiSUSB,SISCR, pSiSUSB->myCR63, pSiSUSB->oldCR63);
    outSISIDXREG(pSiSUSB,SISSR, 0x1f, pSiSUSB->oldSR1F);

    if(pSiSUSB->sisusbconactive) {

	   sisrestoredestroyconsole(pSiSUSB, 0);

    } else if( (pSiSUSB->restorebyset) && (pSiSUSB->OldMode) ) {

	   int mymode = pSiSUSB->OldMode;

           xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 3,
	         "Restoring by setting old mode 0x%02x\n", pSiSUSB->OldMode);

	   pSiSUSB->SiS_Pr->UseCustomMode = FALSE;
	   pSiSUSB->SiS_Pr->CRT1UsesCustomMode = FALSE;
	   SiSUSBSetMode(pSiSUSB->SiS_Pr, pScrn, mymode, FALSE);

	   SiSUSB_GetSetModeID(pScrn, pSiSUSB->OldMode); /* NOT mymode! */

	   /* Restore CRT1 status */
	   outSISIDXREG(pSiSUSB,SISCR, pSiSUSB->myCR63, pSiSUSB->oldCR63);
	   outSISIDXREG(pSiSUSB,SISSR, 0x1f, pSiSUSB->oldSR1F);

#ifdef SISVRAMQ
	   /* Restore queue mode registers on 315/330/340 series */
	   /* (This became necessary due to the switch to VRAM queue) */
	   SiSUSBRestoreQueueMode(pSiSUSB, sisReg);
#endif

    } else {

	   (*pSiSUSB->SiSRestore)(pScrn, sisReg);

    }

    if(!pSiSUSB->sisusbconactive) {
       SiSUSBVGAProtect(pScrn, TRUE);
       SiSUSBVGARestore(pScrn, sisReg, flags);
       SiSUSBVGAProtect(pScrn, FALSE);
    }

    sisusbRestoreExtRegisterLock(pSiSUSB,sisReg->sisRegs3C4[0x05],sisReg->sisRegs3D4[0x80]);
}

/* Restore bridge config registers - to be called BEFORE Restore */
static void
SISUSBBridgeRestore(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    SiSUSBRestoreBridge(pScrn, &pSiSUSB->SavedReg);
}

/* Our generic BlockHandler for Xv */
static void
SISUSBBlockHandler(int i, pointer blockData, pointer pTimeout, pointer pReadmask)
{
    ScreenPtr pScreen = screenInfo.screens[i];
    ScrnInfoPtr pScrn = xf86Screens[i];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    if((pSiSUSB->sisusbfatalerror) && (pSiSUSB->timeout != -1)) {
       pSiSUSB->sisusberrorsleepcount++;
       if(!(pSiSUSB->sisusberrorsleepcount % 100)) {
	  if(SiSUSBCheckForUSBDongle(pScrn->chipset, pSiSUSB, &pSiSUSB->sisusbdev) >= 0) {
	     pSiSUSB->sisusberrorsleepcount = 0;
	     pSiSUSB->sisusbfatalerror = 0;
	     pSiSUSB->sisusbdevopen = TRUE;
	     (*pScrn->SwitchMode)(pScrn->scrnIndex, pScrn->currentMode, 0);
	     pSiSUSB->ShBoxcount = 1;
	     pSiSUSB->ShXmin = pSiSUSB->ShYmin = 0;
	     pSiSUSB->ShXmax = pScrn->virtualX;
	     pSiSUSB->ShYmax = pScrn->virtualY;
	  }
       } else if(pSiSUSB->timeout > 0) {
          if(currentTime.milliseconds >= pSiSUSB->errorTime + (pSiSUSB->timeout * 1000)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	     		"Device disconnection timeout exceeded... Aborting...\n");
	     GiveUp(0);
	  }
       }
    }

    SISUSBDoRefreshArea(pScrn);

    pScreen->BlockHandler = pSiSUSB->BlockHandler;
    (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);
    pScreen->BlockHandler = SISUSBBlockHandler;

    if(pSiSUSB->VideoTimerCallback) {
       (*pSiSUSB->VideoTimerCallback)(pScrn, currentTime.milliseconds);
    }

}

/* Mandatory
 * This gets called at the start of each server generation
 *
 * We use pScrn and not CurrentLayout here, because the
 * properties we use have not changed (displayWidth,
 * depth, bitsPerPixel)
 */
static Bool
SISUSBScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int ret;
    VisualPtr visual;
    ULong OnScreenSize;
    int height, width, displayWidth;
    UChar *FBStart;

    /* Map the SiS memory and MMIO areas */
    if(!SISUSBMapMem(pScrn)) {
       SISUSBErrorLog(pScrn, "SiSUSBMapMem() failed\n");
       return FALSE;
    }

    SiSUSB_SiSFB_Lock(pScrn, TRUE);

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);
#endif

    /* Save the current state */
    SISUSBSave(pScrn);

    {

       if(!pSiSUSB->OldMode) {

          /* Try to find out current (=old) mode number
	   * (Do this only if not sisfb has told us its mode yet)
	   */

	  /* Read 0:449 which the BIOS sets to the current mode number
	   * Unfortunately, this not reliable since the int10 emulation
	   * does not change this. So if we call the VBE later, this
	   * byte won't be touched (which is why we set this manually
	   * then).
	   */
          UChar myoldmode = SiSUSB_GetSetModeID(pScrn,0xFF);
	  UChar cr30, cr31;

          /* Read CR34 which the BIOS sets to the current mode number for CRT2
	   * This is - of course - not reliable if the machine has no video
	   * bridge...
	   */
          inSISIDXREG(pSiSUSB,SISCR, 0x34, pSiSUSB->OldMode);
	  inSISIDXREG(pSiSUSB,SISCR, 0x30, cr30);
	  inSISIDXREG(pSiSUSB,SISCR, 0x31, cr31);

	  /* What if CR34 is different from the BIOS scratch byte? */
	  if(pSiSUSB->OldMode != myoldmode) {
	     /* If no bridge output is active, trust the BIOS scratch byte */
	     if( (!(pSiSUSB->VBFlags2 & VB2_VIDEOBRIDGE)) ||
	         (pSiSUSB->OldMode == 0)                  ||
	         (!cr31 && !cr30)                         ||
		 (cr31 & 0x20) ) {
		pSiSUSB->OldMode = myoldmode;
 	     }
	     /* ..else trust CR34 */
	  }

	  /* Newer 650 BIOSes set CR34 to 0xff if the mode has been
	   * "patched", for instance for 80x50 text mode. (That mode
	   * has no number of its own, it's 0x03 like 80x25). In this
	   * case, we trust the BIOS scratch byte (provided that any
	   * of these two is valid).
	   */
	  if(pSiSUSB->OldMode > 0x7f) {
	     pSiSUSB->OldMode = myoldmode;
	  }
       }

    }

    /* RandR resets screen mode and size in CloseScreen(), hence
     * we need to adapt our VBFlags to the initial state if the
     * current mode has changed since closescreen() (or Screeninit()
     * for the first instance)
     */
    if(pScrn->currentMode != pSiSUSB->currentModeLast) {
       pSiSUSB->VBFlags = pSiSUSB->VBFlags_backup = pSiSUSB->VBFlagsInit;
    }

    /* Initialise the first mode */
    if(!SISUSBModeInit(pScrn, pScrn->currentMode)) {
       SISUSBErrorLog(pScrn, "SiSUSBModeInit() failed\n");
       return FALSE;
    }

    /* Darken the screen for aesthetic reasons */
    /* Not using Dual Head variant on purpose; we darken
     * the screen for both displays, and un-darken
     * it when the second head is finished
     */
    SISUSBSaveScreen(pScreen, SCREEN_SAVER_ON);

    /* Set the viewport */
    SISUSBAdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    /* Reset visual list. */
    miClearVisualTypes();

    /* Setup the visuals we support. */

    /*
     * For bpp > 8, the default visuals are not acceptable because we only
     * support TrueColor and not DirectColor.
     */
    if(!miSetVisualTypes(pScrn->depth,
			 (pScrn->bitsPerPixel > 8) ?
			 	TrueColorMask : miGetDefaultVisualMask(pScrn->depth),
			 pScrn->rgbBits, pScrn->defaultVisual)) {
       SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
       SISUSBErrorLog(pScrn, "miSetVisualTypes() failed (bpp %d)\n",
	  		pScrn->bitsPerPixel);
       return FALSE;
    }

    width = pScrn->virtualX;
    height = pScrn->virtualY;
    displayWidth = pScrn->displayWidth;

    if(pSiSUSB->ShadowFB) {
       pSiSUSB->ShadowPitch = BitmapBytePad(pScrn->bitsPerPixel * displayWidth);
       pSiSUSB->ShadowPtr = xalloc(pSiSUSB->ShadowPitch * height);
       if(!(FBStart = pSiSUSB->ShadowPtr)) {
          SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
          SISUSBErrorLog(pScrn, "Failed to allocate shadow framebuffer\n");
          return FALSE;
       }
#if X_BYTE_ORDER == X_BIG_ENDIAN
       if(pScrn->bitsPerPixel == 16) {
          /* For 16bpp, we need to swap the bytes in the framebuffer */
          if(!(pSiSUSB->ShadowPtrSwap = xalloc(pSiSUSB->ShadowPitch * height))) {
	     SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
             SISUSBErrorLog(pScrn, "Failed to allocate swap buffer for shadow framebuffer\n");
             return FALSE;
	  }
	  FBStart = pSiSUSB->ShadowPtrSwap;
       }
#endif
    } else {
       pSiSUSB->ShadowPtr = NULL;
       FBStart = pSiSUSB->FbBase;
    }

    if(!miSetPixmapDepths()) {
       SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
       SISUSBErrorLog(pScrn, "miSetPixmapDepths() failed\n");
       return FALSE;
    }

    /* Point cmdQueuePtr to pSiSEnt for shared usage
     * (same technique is then eventually used in DRIScreeninit)
     * For 315/330 series, this is done in EnableTurboQueue
     * which has already been called during ModeInit().
     */

    pSiSUSB->cmdQueueLenPtr = &(pSiSUSB->cmdQueueLen);

    pSiSUSB->cmdQueueLen = 0; /* Force an EngineIdle() at start */

    /*
     * Call the framebuffer layer's ScreenInit function and fill in other
     * pScreen fields.
     */
    switch(pScrn->bitsPerPixel) {
      case 8:
      case 16:
      case 32:
        ret = fbScreenInit(pScreen, FBStart, width,
                        height, pScrn->xDpi, pScrn->yDpi,
                        displayWidth, pScrn->bitsPerPixel);
        break;
      default:
        ret = FALSE;
        break;
    }
    if(!ret) {
       SISUSBErrorLog(pScrn, "Unsupported bpp (%d) or fbScreenInit() failed\n",
               pScrn->bitsPerPixel);
       SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
       return FALSE;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* Fixup RGB ordering on BE machines */
    /* For 24bpp, we just swap the rgb masks and offsets */
    if(pScrn->bitsPerPixel == 32) {
       pScrn->offset.red   =  8;
       pScrn->offset.green = 16;
       pScrn->offset.blue  = 24;
       pScrn->mask.red   = 0x0000ff00;
       pScrn->mask.green = 0x00ff0000;
       pScrn->mask.blue  = 0xff000000;
    }
#endif

    if(pScrn->bitsPerPixel > 8) {
       /* Fixup RGB ordering */
       visual = pScreen->visuals + pScreen->numVisuals;
       while (--visual >= pScreen->visuals) {
          if((visual->class | DynamicClass) == DirectColor) {
             visual->offsetRed = pScrn->offset.red;
             visual->offsetGreen = pScrn->offset.green;
             visual->offsetBlue = pScrn->offset.blue;
             visual->redMask = pScrn->mask.red;
             visual->greenMask = pScrn->mask.green;
             visual->blueMask = pScrn->mask.blue;
          }
       }
    }

    /* Initialize RENDER ext; must be after RGB ordering fixed */
    fbPictureInit(pScreen, 0, 0);

    xf86SetBlackWhitePixels(pScreen);

    /* Initialize the accelerators */
    SiSUSBAccelInit(pScreen);

    miInitializeBackingStore(pScreen);
    xf86SetBackingStore(pScreen);

    /* NO SilkenMouse. Never. Ever. */

    /* Initialise cursor functions */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    if(pSiSUSB->HWCursor) {
       SiSUSBHWCursorInit(pScreen);
    }

    /* Initialise default colormap */
    if(!miCreateDefColormap(pScreen)) {
       SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
       SISUSBErrorLog(pScrn, "miCreateDefColormap() failed\n");
       return FALSE;
    }

    if(!xf86HandleColormaps(pScreen, 256, (pScrn->depth == 8) ? 8 : pScrn->rgbBits,
                    SISUSBLoadPalette, NULL,
                    CMAP_PALETTED_TRUECOLOR | CMAP_RELOAD_ON_MODE_SWITCH)) {
       SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
       SISUSBErrorLog(pScrn, "xf86HandleColormaps() failed\n");
       return FALSE;
    }

#ifdef SISGAMMARAMP
    if((pSiSUSB->GammaBriR != 1000) ||
       (pSiSUSB->GammaBriB != 1000) ||
       (pSiSUSB->GammaBriG != 1000)) {
       SISUSBCalculateGammaRamp(pScreen, pScrn);
    }
#endif

    pSiSUSB->ShBoxcount = 0;
    pSiSUSB->delaycount = 0;
    if(pSiSUSB->ShadowFB) {
       ShadowFBInit(pScreen, SISUSBRefreshArea);
#if 0
       if(!pSiSUSB->NoAccel) {
          if(!SiSUSBFBInit(pScreen)) {
	     pSiSUSB->NoAccel = TRUE;
	  }
       }
#endif
    }

#if 0
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "2D acceleration %sabled\n", pSiSUSB->NoAccel ? "dis":"en");
#endif

    xf86DPMSInit(pScreen, (DPMSSetProcPtr)SISUSBDisplayPowerManagementSet, 0);

    /* Init memPhysBase and fbOffset in pScrn */
    pScrn->memPhysBase = pSiSUSB->FbAddress;
    pScrn->fbOffset = 0;

    pSiSUSB->ResetXv = pSiSUSB->ResetXvGamma = NULL;
    pSiSUSB->xv_sisdirectunlocked = 0;

#ifdef SIS_GLOBAL_ENABLEXV
#if (XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,3,99,0)) || (defined(XvExtension))
    if(!pSiSUSB->NoXvideo) {
       SISUSBInitVideo(pScreen);
    }
#endif
#endif

    /* Wrap some funcs and setup remaining SD flags */

    pSiSUSB->SiS_SD_Flags &= ~(SiS_SD_PSEUDOXINERAMA);

    pSiSUSB->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = SISUSBCloseScreen;

    pScreen->SaveScreen = SISUSBSaveScreen;

    /* Install BlockHandler */
    pSiSUSB->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = SISUSBBlockHandler;

    /* Report any unused options (only for the first generation) */
    if(serverGeneration == 1) {
       xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }

    /* Clear frame buffer */
    /* For CRT2, we don't do that at this point in dual head
     * mode since the mode isn't switched at this time (it will
     * be reset when setting the CRT1 mode). Hence, we just
     * save the necessary data and clear the screen when
     * going through this for CRT1.
     */

    OnScreenSize = pScrn->displayWidth * pScrn->currentMode->VDisplay
                               * (pScrn->bitsPerPixel >> 3);

    /* Turn on the screen now */

    SISUSBSaveScreen(pScreen, SCREEN_SAVER_OFF);
    sisclearvram(pSiSUSB, pSiSUSB->FbBase, OnScreenSize);

    pSiSUSB->SiS_SD_Flags &= ~SiS_SD_SUPPORTSGRCRT2;

    pSiSUSB->SiS_SD_Flags &= ~SiS_SD_ISDEPTH8;
    if(pSiSUSB->CurrentLayout.bitsPerPixel == 8) {
    	pSiSUSB->SiS_SD_Flags |= SiS_SD_ISDEPTH8;
	pSiSUSB->SiS_SD_Flags &= ~SiS_SD_SUPPORTXVGAMMA1;
	pSiSUSB->SiS_SD_Flags &= ~SiS_SD_SUPPORTSGRCRT2;
    }

#ifdef SISGAMMARAMP
    pSiSUSB->SiS_SD_Flags |= SiS_SD_CANSETGAMMA;
#else
    pSiSUSB->SiS_SD_Flags &= ~SiS_SD_CANSETGAMMA;
#endif

#ifdef SIS_ENABLEXV
    pSiSUSB->SiS_SD2_Flags &= ~SiS_SD2_NOOVERLAY;
#else
    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_NOOVERLAY;
#endif

    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_NODDCSUPPORT;

    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_SUPPLTFLAG;
    pSiSUSB->SiS_SD2_Flags |= SiS_SD2_HAVESD34;

    SiSUSBCtrlExtInit(pScrn);

    return TRUE;
}

/* Usually mandatory */
Bool
SISUSBSwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    if(!pSiSUSB->skipswitchcheck) {
       if(SISUSBValidMode(scrnIndex, mode, TRUE, flags) != MODE_OK) {
          return FALSE;
       }
    }

#if 0
    if(!pSiSUSB->NoAccel) {
       SiSUSBSync(pScrn);
    }
#endif

    if(!(SISUSBModeInit(xf86Screens[scrnIndex], mode))) return FALSE;

    return TRUE;
}

static void
SISUSBSetStartAddressCRT1(SISUSBPtr pSiSUSB, ULong base)
{
    UChar cr11backup;

    inSISIDXREG(pSiSUSB,SISCR,  0x11, cr11backup);  /* Unlock CRTC registers */
    andSISIDXREG(pSiSUSB, SISCR, 0x11, 0x7F);
    outSISIDXREG(pSiSUSB,SISCR, 0x0D, base & 0xFF);
    outSISIDXREG(pSiSUSB,SISCR, 0x0C, (base >> 8) & 0xFF);
    outSISIDXREG(pSiSUSB,SISSR, 0x0D, (base >> 16) & 0xFF);
    setSISIDXREG(pSiSUSB, SISSR, 0x37, 0xFE, (base >> 24) & 0x01);

    /* Eventually lock CRTC registers */
    setSISIDXREG(pSiSUSB, SISCR, 0x11, 0x7F,(cr11backup & 0x80));
}

/*
 * This function is used to initialize the Start Address - the first
 * displayed location in the video memory.
 *
 * Usually mandatory
 */
void
SISUSBAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr   pScrn = xf86Screens[scrnIndex];
    SISUSBPtr        pSiSUSB = SISUSBPTR(pScrn);
    ULong base;

    if(pScrn->bitsPerPixel < 8) {
       base = (y * pSiSUSB->CurrentLayout.displayWidth + x + 3) >> 3;
    } else {
       base  = y * pSiSUSB->CurrentLayout.displayWidth + x;

       /* calculate base bpp dep. */
       switch(pSiSUSB->CurrentLayout.bitsPerPixel) {
          case 16:
     	     base >>= 1;
             break;
          case 24:
             base = ((base * 3)) >> 2;
             base -= base % 6;
             break;
          case 32:
             break;
          default:      /* 8bpp */
             base >>= 2;
             break;
       }
    }

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);
#endif

    SISUSBSetStartAddressCRT1(pSiSUSB, base);
}

/*
 * This is called when VT switching back to the X server.  Its job is
 * to reinitialise the video mode.
 * Mandatory!
 */
static Bool
SISUSBEnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    SiSUSB_SiSFB_Lock(pScrn, TRUE);

    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);

    outSISIDXREG(pSiSUSB,SISCR,0x32,pSiSUSB->myCR32);
    outSISIDXREG(pSiSUSB,SISCR,0x36,pSiSUSB->myCR36);
    outSISIDXREG(pSiSUSB,SISCR,0x37,pSiSUSB->myCR37);

    if(!SISUSBModeInit(pScrn, pScrn->currentMode)) {
       SISUSBErrorLog(pScrn, "SiSUSBEnterVT: SISUSBModeInit() failed\n");
       return FALSE;
    }

    SISUSBAdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    if(pSiSUSB->ResetXv) {
       (pSiSUSB->ResetXv)(pScrn);
    }

    return TRUE;
}

/*
 * This is called when VT switching away from the X server.  Its job is
 * to restore the previous (text) mode.
 * Mandatory!
 */
static void
SISUSBLeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    if(pSiSUSB->CursorInfoPtr) {
          pSiSUSB->CursorInfoPtr->HideCursor(pScrn);
          SISUSBWaitVBRetrace(pScrn);
    }

    SISUSBBridgeRestore(pScrn);

    SISUSBRestore(pScrn);

    /* We use (otherwise unused) bit 7 to indicate that we are running
     * to keep sisfb to change the displaymode (this would result in
     * lethal display corruption upon quitting X or changing to a VT
     * until a reboot)
     */

    orSISIDXREG(pSiSUSB, SISCR,0x34,0x80);

    SISUSBVGALock(pSiSUSB);

    SiSUSB_SiSFB_Lock(pScrn, FALSE);
}


/*
 * This is called at the end of each server generation.  It restores the
 * original (text) mode.  It should really also unmap the video memory too.
 * Mandatory!
 */
static Bool
SISUSBCloseScreen(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    if(pSiSUSB->SiSCtrlExtEntry) {
       SiSUSBCtrlExtUnregister(pSiSUSB, pScrn->scrnIndex);
    }

    if(pScrn->vtSema) {

        if(pSiSUSB->CursorInfoPtr) {
             pSiSUSB->CursorInfoPtr->HideCursor(pScrn);
             SISUSBWaitVBRetrace(pScrn);
	}

        SISUSBBridgeRestore(pScrn);

	SISUSBRestore(pScrn);

        SISUSBVGALock(pSiSUSB);

    }

    SiSUSB_SiSFB_Lock(pScrn, FALSE);

    /* We should restore the mode number in case vtsema = false as well,
     * but since we haven't register access then we can't do it. I think
     * I need to rework the save/restore stuff, like saving the video
     * status when returning to the X server and by that save me the
     * trouble if sisfb was started from a textmode VT while X was on.
     */

    SISUSBUnmapMem(pScrn);

    if(pSiSUSB->CursorInfoPtr) {
       xf86DestroyCursorInfoRec(pSiSUSB->CursorInfoPtr);
       pSiSUSB->CursorInfoPtr = NULL;
    }

    if(pSiSUSB->USBCursorBuf) {
       xfree(pSiSUSB->USBCursorBuf);
       pSiSUSB->USBCursorBuf = NULL;
    }

    if(pSiSUSB->ShadowPtr) {
       xfree(pSiSUSB->ShadowPtr);
       pSiSUSB->ShadowPtr = NULL;
    }

#if 0
    if(pSiSUSB->PreAllocMem) {
       xfree(pSiSUSB->PreAllocMem);
       pSiSUSB->PreAllocMem = NULL;
    }
#endif

    if(pSiSUSB->adaptor) {
       xfree(pSiSUSB->adaptor);
       pSiSUSB->adaptor = NULL;
       pSiSUSB->ResetXv = pSiSUSB->ResetXvGamma = NULL;
    }

    pScrn->vtSema = FALSE;

    /* Restore Blockhandler */
    pScreen->BlockHandler = pSiSUSB->BlockHandler;

#if 0
    if(pSiSUSB->AWCreateGC) {
       pScreen->CreateGC = pSiSUSB->AWCreateGC;
    }
#endif

    pScreen->CloseScreen = pSiSUSB->CloseScreen;

    return(*pScreen->CloseScreen)(scrnIndex, pScreen);
}


/* Free up any per-generation data structures */

/* Optional */
static void
SISUSBFreeScreen(int scrnIndex, int flags)
{
    SISUSBFreeRec(xf86Screens[scrnIndex]);
}


/* Checks if a mode is suitable for the selected chipset. */

static ModeStatus
SISUSBValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    if(SiSUSB_CheckModeCRT1(pScrn, mode, pSiSUSB->VBFlags, pSiSUSB->HaveCustomModes) < 0x14)
       return MODE_BAD;

    return MODE_OK;
}

/* Do screen blanking
 *
 * Mandatory
 */
static Bool
SISUSBSaveScreen(ScreenPtr pScreen, int mode)
{
    return SiSUSBVGASaveScreen(pScreen, mode);
}


#ifdef DEBUG
static void
SiSUSBDumpModeInfo(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Clock : %x\n", mode->Clock);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Display : %x\n", mode->CrtcHDisplay);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Blank Start : %x\n", mode->CrtcHBlankStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Sync Start : %x\n", mode->CrtcHSyncStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Sync End : %x\n", mode->CrtcHSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Blank End : %x\n", mode->CrtcHBlankEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Total : %x\n", mode->CrtcHTotal);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Skew : %x\n", mode->CrtcHSkew);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz HAdjusted : %x\n", mode->CrtcHAdjusted);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Display : %x\n", mode->CrtcVDisplay);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Blank Start : %x\n", mode->CrtcVBlankStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Sync Start : %x\n", mode->CrtcVSyncStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Sync End : %x\n", mode->CrtcVSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Blank End : %x\n", mode->CrtcVBlankEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Total : %x\n", mode->CrtcVTotal);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt VAdjusted : %x\n", mode->CrtcVAdjusted);
}
#endif

static void
SISUSBModifyModeInfo(DisplayModePtr mode)
{
    if(mode->CrtcHBlankStart == mode->CrtcHDisplay)
        mode->CrtcHBlankStart++;
    if(mode->CrtcHBlankEnd == mode->CrtcHTotal)
        mode->CrtcHBlankEnd--;
    if(mode->CrtcVBlankStart == mode->CrtcVDisplay)
        mode->CrtcVBlankStart++;
    if(mode->CrtcVBlankEnd == mode->CrtcVTotal)
        mode->CrtcVBlankEnd--;
}

/* Enable the Turboqueue/Commandqueue (For 300 and 315/330/340 series only) */
static void
SiSUSBEnableTurboQueue(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    UShort SR26;
    ULong  temp;

    if(!pSiSUSB->NoAccel) {
	      /* On 315/330/340 series, there are three queue modes available
	       * which are chosen by setting bits 7:5 in SR26:
	       * 1. MMIO queue mode (bit 5, 0x20). The hardware will keep
	       *    track of the queue, the FIFO, command parsing and so
	       *    on. This is the one comparable to the 300 series.
	       * 2. VRAM queue mode (bit 6, 0x40). In this case, one will
	       *    have to do queue management himself.
	       * 3. AGP queue mode (bit 7, 0x80). Works as 2., but keeps the
	       *    queue in AGP memory space.
	       * We go VRAM or MMIO here.
	       * SR26 bit 4 is called "Bypass H/W queue".
	       * SR26 bit 1 is called "Enable Command Queue Auto Correction"
	       * SR26 bit 0 resets the queue
	       * Size of queue memory is encoded in bits 3:2 like this:
	       *    00  (0x00)  512K
	       *    01  (0x04)  1M
	       *    10  (0x08)  2M
	       *    11  (0x0C)  4M
	       * The queue location is to be written to 0x85C0.
	       */
#ifdef SISVRAMQ
	      /* We use VRAM Cmd Queue, not MMIO or AGP */
	      UChar tempCR55 = 0;

	      pSiSUSB->cmdQ_SharedWritePort = &(pSiSUSB->cmdQ_SharedWritePort_2D);

	      /* Set Command Queue Threshold to max value 11111b (?) */
	      outSISIDXREG(pSiSUSB, SISSR, 0x27, 0x1F);
	      /* Disable queue flipping */
	      inSISIDXREG(pSiSUSB, SISCR, 0x55, tempCR55) ;
    	      andSISIDXREG(pSiSUSB, SISCR, 0x55, 0x33) ;
	      /* Syncronous reset for Command Queue */
	      outSISIDXREG(pSiSUSB, SISSR, 0x26, 0x01);
	      SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85c4, 0);
	      /* Enable VRAM Command Queue mode */
	      switch(pSiSUSB->cmdQueueSize) {
    		case 1*1024*1024: SR26 = (0x40 | 0x04 | 0x01); break;
    		case 2*1024*1024: SR26 = (0x40 | 0x08 | 0x01); break;
    		case 4*1024*1024: SR26 = (0x40 | 0x0C | 0x01); break;
		default:
		                  pSiSUSB->cmdQueueSize = 512 * 1024;
		case    512*1024: SR26 = (0x40 | 0x00 | 0x01);
	      }
    	      outSISIDXREG(pSiSUSB,SISSR, 0x26, SR26);
	      SR26 &= 0xfe;
	      outSISIDXREG(pSiSUSB,SISSR, 0x26, SR26);
	      pSiSUSB->cmdQ_SharedWritePort_2D = (ULong)(SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x85c8));
              *(pSiSUSB->cmdQ_SharedWritePort) = pSiSUSB->cmdQ_SharedWritePort_2D;
              SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85c4, pSiSUSB->cmdQ_SharedWritePort_2D);
	      SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85C0, pSiSUSB->cmdQueueOffset);
	      temp = (ULong)pSiSUSB->FbBase;
              temp += pSiSUSB->cmdQueueOffset;
              pSiSUSB->cmdQueueBase = (ULong *)temp;
    	      outSISIDXREG(pSiSUSB,SISCR, 0x55, tempCR55);
#else
	      /* For MMIO */
	      /* Set Command Queue Threshold to max value 11111b */
	      outSISIDXREG(pSiSUSB,SISSR, 0x27, 0x1F);
	      /* Syncronous reset for Command Queue */
	      outSISIDXREG(pSiSUSB,SISSR, 0x26, 0x01);
	      /* Do some magic (cp readport to writeport) */
	      temp = SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x85C8);
	      SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85C4, temp);
	      /* Enable MMIO Command Queue mode (0x20),
	       * Enable_command_queue_auto_correction (0x02)
	       *        (no idea, but sounds good, so use it)
	       * 512k (0x00) (does this apply to MMIO mode?) */
    	      outSISIDXREG(pSiSUSB,SISSR, 0x26, 0x22);
	      /* Calc Command Queue position (Q is always 512k)*/
	      temp = (pScrn->videoRam - 512) * 1024;
	      /* Set Q position */
	      SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85C0, temp);
#endif
    }

}

#ifdef SISVRAMQ
static void
SiSUSBRestoreQueueMode(SISUSBPtr pSiSUSB, SISUSBRegPtr sisReg)
{
    UChar tempCR55=0;

    inSISIDXREG(pSiSUSB,SISCR,0x55,tempCR55);
    andSISIDXREG(pSiSUSB, SISCR,0x55,0x33);
    outSISIDXREG(pSiSUSB,SISSR,0x26,0x01);
    SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85c4, 0);
    outSISIDXREG(pSiSUSB,SISSR,0x27,sisReg->sisRegs3C4[0x27]);
    outSISIDXREG(pSiSUSB,SISSR,0x26,sisReg->sisRegs3C4[0x26]);
    SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85C0, sisReg->sisMMIO85C0);
    outSISIDXREG(pSiSUSB,SISCR,0x55,tempCR55);
}
#endif

/* Things to do before a ModeSwitch.
 */
void SiSUSBPreSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode, int viewmode)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    UChar  CR30, CR31, CR33;
    int    crt1rateindex = 0;
    unsigned int vbflag = pSiSUSB->VBFlags;
    Bool   hcm = pSiSUSB->HaveCustomModes;
    DisplayModePtr mymode = mode;

    pSiSUSB->IsCustom = FALSE;

    if(SiSUSB_CheckModeCRT1(pScrn, mymode, vbflag, hcm) == 0xfe) {
       pSiSUSB->IsCustom = TRUE;
    }

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);    /* Unlock Registers */
#endif

    inSISIDXREG(pSiSUSB,SISCR, 0x33, CR33);

    CR30 = 0x00;
    CR31 = 0x00;

    if(!pSiSUSB->IsCustom) {
       crt1rateindex = SISUSBSearchCRT1Rate(pScrn, mymode);
    }

    CR33 &= 0xf0;
    CR33 |= (crt1rateindex & 0x0f);

    if(pSiSUSB->CRT1off) CR33 &= 0xf0;

    outSISIDXREG(pSiSUSB,SISCR, 0x30, CR30);
    outSISIDXREG(pSiSUSB,SISCR, 0x31, CR31);
    outSISIDXREG(pSiSUSB,SISCR, 0x33, CR33);
}


/* Calc dotclock from registers */
static int
SiSUSBGetClockFromRegs(UChar sr2b, UChar sr2c)
{
   float num, denum, postscalar, divider;
   int   myclock;

   divider = (sr2b & 0x80) ? 2.0 : 1.0;
   postscalar = (sr2c & 0x80) ?
              ( (((sr2c >> 5) & 0x03) == 0x02) ? 6.0 : 8.0 ) :
	      ( ((sr2c >> 5) & 0x03) + 1.0 );
   num = (sr2b & 0x7f) + 1.0;
   denum = (sr2c & 0x1f) + 1.0;
   myclock = (int)((14318 * (divider / postscalar) * (num / denum)) / 1000);
   return myclock;
}

/* PostSetMode:
 * -) Disable CRT1 for saving bandwidth. This doesn't work with VESA;
 *    VESA uses the bridge in SlaveMode and switching CRT1 off while
 *    the bridge is in SlaveMode not that clever...
 * -) Check if overlay can be used (depending on dotclock)
 * -) Check if Panel Scaler is active on LVDS for overlay re-scaling
 * -) Save TV registers for further processing
 * -) Apply TV settings
 */
static void
SiSUSBPostSetMode(ScrnInfoPtr pScrn, SISUSBRegPtr sisReg)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    UChar sr2b, sr2c;
    int   myclock1, myclock2;

    pSiSUSB->CRT1isoff = pSiSUSB->CRT1off;

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);
#endif

    andSISIDXREG(pSiSUSB, SISCR,pSiSUSB->myCR63,0xBF);
    andSISIDXREG(pSiSUSB, SISSR,0x1f,0x3f);

    /* Determine if the video overlay can be used */
    if(!pSiSUSB->NoXvideo) {
       int clklimit1=0, clklimit2=0, clklimitg=0;
       inSISIDXREG(pSiSUSB,SISSR,0x2b,sr2b);
       inSISIDXREG(pSiSUSB,SISSR,0x2c,sr2c);
       myclock1 = myclock2 = SiSUSBGetClockFromRegs(sr2b, sr2c);

       pSiSUSB->MiscFlags &= ~(MISC_CRT1OVERLAY | MISC_CRT2OVERLAY | MISC_CRT1OVERLAYGAMMA);

       clklimit1 = clklimit2 = 180;  /* ? */
       clklimitg = 166;		     /* ? */

       if(myclock1 <= clklimit1) pSiSUSB->MiscFlags |= MISC_CRT1OVERLAY;
       if(myclock2 <= clklimit2) pSiSUSB->MiscFlags |= MISC_CRT2OVERLAY;
       if(myclock1 <= clklimitg) pSiSUSB->MiscFlags |= MISC_CRT1OVERLAYGAMMA;
       if(!(pSiSUSB->MiscFlags & MISC_CRT1OVERLAY)) {
             xf86DrvMsgVerb(pScrn->scrnIndex, X_WARNING, 3,
	     	"Current dotclock (%dMhz) too high for video overlay on CRT1\n",
		myclock1);
       }
    }

    /* Determine if the Panel Link scaler is active */
    pSiSUSB->MiscFlags &= ~MISC_PANELLINKSCALER;

    /* Determine if our very special TV mode is active */
    pSiSUSB->MiscFlags &= ~MISC_TVNTSC1024;

    {
       int i;
#ifdef SISVRAMQ
       /* Re-Enable command queue */
       SiSUSBEnableTurboQueue(pScrn);
#endif
       /* Get HWCursor register contents for backup */
       for(i = 0; i < 16; i++) {
          pSiSUSB->HWCursorBackup[i] = SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x8500 + (i << 2));
       }
    }

    /* Reset XV gamma correction */
    if(pSiSUSB->ResetXvGamma) {
       (pSiSUSB->ResetXvGamma)(pScrn);
    }
}


/* Check if video bridge is in slave mode */
Bool
SiSUSBBridgeIsInSlaveMode(ScrnInfoPtr pScrn)
{
    return FALSE;
}

UShort
SiSUSB_GetModeNumber(ScrnInfoPtr pScrn, DisplayModePtr mode, unsigned int VBFlags)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   UShort i = (pSiSUSB->CurrentLayout.bitsPerPixel+7)/8 - 1;

   return(SiSUSB_GetModeID(pSiSUSB->VGAEngine, VBFlags, mode->HDisplay, mode->VDisplay,
   			i, pSiSUSB->FSTN, pSiSUSB->LCDwidth, pSiSUSB->LCDheight));
}

static UShort
SiSUSB_CheckModeCRT1(ScrnInfoPtr pScrn, DisplayModePtr mode, unsigned int VBFlags, Bool havecustommodes)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   UShort i = (pSiSUSB->CurrentLayout.bitsPerPixel+7)/8 - 1;

   if((havecustommodes) && (!(mode->type & M_T_DEFAULT))) {
      return 0xfe;
   }

   return (SiSUSB_GetModeID(pSiSUSB->VGAEngine, VBFlags, mode->HDisplay, mode->VDisplay,
   			i, pSiSUSB->FSTN, pSiSUSB->LCDwidth, pSiSUSB->LCDheight));
}

/* Calculate the vertical refresh rate from a mode */
int
SiSUSBCalcVRate(DisplayModePtr mode)
{
   float hsync, refresh = 0;

   if(mode->HSync > 0.0)
       	hsync = mode->HSync;
   else if(mode->HTotal > 0)
       	hsync = (float)mode->Clock / (float)mode->HTotal;
   else
       	hsync = 0.0;

   if(mode->VTotal > 0)
       	refresh = hsync * 1000.0 / mode->VTotal;

   if(mode->Flags & V_INTERLACE)
       	refresh *= 2.0;

   if(mode->Flags & V_DBLSCAN)
       	refresh /= 2.0;

   if(mode->VScan > 1)
        refresh /= mode->VScan;

   if(mode->VRefresh > 0.0)
    	refresh = mode->VRefresh;

   if(hsync == 0 || refresh == 0) return(0);

   return((int)(refresh));
}

/* Calculate CR33 (rate index) for CRT1.
 * Calculation is done using currentmode, therefore it is
 * recommended to set VertRefresh and HorizSync to correct
 * values in config file.
 */
UChar
SISUSBSearchCRT1Rate(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
   int     i = 0, irefresh;
   UShort  xres = mode->HDisplay;
   UShort  yres = mode->VDisplay;
   UChar   index, defindex;

   defindex = (xres == 800 || xres == 1024 || xres == 1280) ? 0x02 : 0x01;

   irefresh = SiSUSBCalcVRate(mode);
   if(!irefresh) return defindex;

   /* We need the REAL refresh rate here */
   if(mode->Flags & V_INTERLACE) irefresh /= 2;

   /* Do not multiply by 2 when DBLSCAN! */

   index = 0;
   while((sisx_vrate[i].idx != 0) && (sisx_vrate[i].xres <= xres)) {
      if((sisx_vrate[i].xres == xres) && (sisx_vrate[i].yres == yres)) {
	 if(sisx_vrate[i].refresh == irefresh) {
	    index = sisx_vrate[i].idx;
	    break;
	 } else if(sisx_vrate[i].refresh > irefresh) {
	    if((sisx_vrate[i].refresh - irefresh) <= 3) {
		index = sisx_vrate[i].idx;
	    } else if( ((irefresh - sisx_vrate[i - 1].refresh) <=  2) &&
			(sisx_vrate[i].idx != 1) ) {
		index = sisx_vrate[i - 1].idx;
	    }
	    break;
	 } else if((irefresh - sisx_vrate[i].refresh) <= 2) {
	    index = sisx_vrate[i].idx;
	    break;
	 }
      }
      i++;
   }

   if(index > 0) return index;
   else          return defindex;
}

#if 0
static int
SISUSBGetScanline(SISUSBPtr pSiSUSB)
{
   SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase,0x8514,0x00000001);
   return(((SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase,0x8514)) >> 16) & 0x7ff);
#if 0
   inSISIDXREG(pSiSUSB,SISCR,0x20,reg1);
   inSISIDXREG(pSiSUSB,SISCR,0x1c,reg2);
   inSISIDXREG(pSiSUSB,SISCR,0x1d,reg3);
   return (reg2 | ((reg3 & 0x07) << 8));
#endif
}
#endif

void
SISUSBWaitRetraceCRT1(ScrnInfoPtr pScrn)
{
   usleep(10000);
#if 0
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   int line1, line2;
   UChar  temp;

   inSISIDXREG(pSiSUSB,SISSR,0x1f,temp);
   if(temp & 0xc0) return;
#endif
}

static void
SISUSBWaitVBRetrace(ScrnInfoPtr pScrn)
{
   SISUSBWaitRetraceCRT1(pScrn);
}

#define MODEID_OFF 0x449

UChar
SiSUSB_GetSetModeID(ScrnInfoPtr pScrn, UChar id)
{
    return(SiSUSB_GetSetBIOSScratch(pScrn, MODEID_OFF, id));
}

UChar
SiSUSB_GetSetBIOSScratch(ScrnInfoPtr pScrn, UShort offset, UChar value)
{
    return 0;
}

void
sisusbSaveUnlockExtRegisterLock(SISUSBPtr pSiSUSB, UChar *reg1, UChar *reg2)
{
    register UChar val;
    ULong mylockcalls;

    pSiSUSB->lockcalls++;
    mylockcalls = pSiSUSB->lockcalls;

    if(pSiSUSB->sisusbfatalerror) return;

    /* check if already unlocked */
    inSISIDXREG(pSiSUSB, SISSR, 0x05, val);
    if(val != 0xa1) {
       /* save State */
       if(reg1) *reg1 = val;
       /* unlock */
       outSISIDXREG(pSiSUSB, SISSR, 0x05, 0x86);
       inSISIDXREG(pSiSUSB, SISSR, 0x05, val);
       if(val != 0xA1) {
          SISUSBErrorLog(pSiSUSB->pScrn,
               "Failed to unlock sr registers (%p, %lx, 0x%02x; %ld)\n",
	       (void *)pSiSUSB, (ULong)pSiSUSB->RelIO, val, mylockcalls);
       }
    }

}

void
sisusbRestoreExtRegisterLock(SISUSBPtr pSiSUSB, UChar reg1, UChar reg2)
{
    /* restore lock */
#ifndef UNLOCK_ALWAYS
    outSISIDXREG(pSiSUSB, SISSR, 0x05, reg1 == 0xA1 ? 0x86 : 0x00);
#endif
}

