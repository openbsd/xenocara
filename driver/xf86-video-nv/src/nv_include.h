#ifndef __NV_INCLUDE_H__
#define __NV_INCLUDE_H__

#include <string.h>
#include <math.h>
#include <unistd.h>

/* All drivers should typically include these */
#include "xf86.h"
#include "xf86_OSproc.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#include "xf86RAC.h"
#endif
#include "compiler.h"

/* Drivers for PCI hardware need this */
#include "xf86PciInfo.h"

/* Drivers that need to access the PCI config space directly need this */
#include "xf86Pci.h"

/* All drivers initialising the SW cursor need this */
#include "mipointer.h"

#include "micmap.h"

#include "xf86DDC.h"
#include "xf86Modes.h"

#include "vbe.h"

#include "nv_const.h"

#include "dixstruct.h"
#include "scrnintstr.h"

#include "fb.h"

#ifdef HAVE_XAA_H
#include "xaa.h"
#endif
#include "xf86fbman.h"
#include "xf86cmap.h"
#include "shadowfb.h"
#include "fbdevhw.h"

#include "xf86xv.h"
#include <X11/extensions/Xv.h>

#include "vgaHW.h"

#include "xf86Cursor.h"

#include "region.h"

#ifdef RANDR
#include <X11/extensions/randr.h>
#endif

#include "compat-api.h"
#include "nv_local.h"
#include "nv_type.h"
#include "nv_proto.h"

#endif /* __NV_INCLUDE_H__ */
