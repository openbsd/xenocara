
#ifndef __RENDITION_H__
#define __RENDITION_H__

#include <string.h>
#include <math.h>

/* All drivers should typically include these */
#include "xf86.h"
#include "xf86_OSproc.h"

/* All drivers need this */

/* Everything using inb/outb, etc needs "compiler.h" */
#include "compiler.h"

/* Drivers that need to access the PCI config space directly need this */
#include "xf86Pci.h"

/* All drivers using the vgahw module need this  */
/* All V1000 _need_ VGA register access,         */
/* so multihead operation is out of the question */
#include "vgaHW.h"

/* All drivers initialising the SW cursor need this */
#include "mipointer.h"

/* All drivers using the mi colormap manipulation need this */
#include "micmap.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
/* Needed by Resources Access Control (RAC) */
#include "xf86RAC.h"

/* Several predefined resource lists */
#include "xf86Resources.h"
#endif

/* Needed by the Shadow Framebuffer */
#include "shadowfb.h"

/* Needed for replacement LoadPalette function for Gamma Correction */
#include "xf86cmap.h"

/* Drivers using fb need this */

#include "fb.h"

/* Drivers using the XAA interface ... */
#ifdef HAVE_XAA_H
#include "xaa.h"
#include "xaalocal.h"
#endif
#include "xf86fbman.h"

/* HW-cursor definitions */
#include "xf86Cursor.h"

/* DDC support */
#include "xf86DDC.h"

#include "commonregs.h"

#include "compat-api.h"
/* end of __RENDITION_H__ */

#define PCI_CHIP_V1000 0x0001
#define PCI_CHIP_V2x00 0x2000

#endif
