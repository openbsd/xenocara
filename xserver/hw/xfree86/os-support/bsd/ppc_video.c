/*
 * Copyright 1992 by Rich Murphey <Rich@Rice.edu>
 * Copyright 1993 by David Wexelblat <dwex@goblin.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Rich Murphey and David Wexelblat
 * not be used in advertising or publicity pertaining to distribution of
 * the software without specific, written prior permission.  Rich Murphey and
 * David Wexelblat make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 * RICH MURPHEY AND DAVID WEXELBLAT DISCLAIM ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL RICH MURPHEY OR DAVID WEXELBLAT BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <X11/X.h>
#include "xf86.h"
#include "xf86Priv.h"

#include "xf86_OSlib.h"
#include "xf86OSpriv.h"

#include "bus/Pci.h"

/***************************************************************************/
/* Video Memory Mapping section                                            */
/***************************************************************************/

#ifdef __OpenBSD__
#define DEV_MEM "/dev/xf86"
#endif

#ifdef __NetBSD__
Bool xf86EnableIO(void);
void xf86DisableIO(void);
#endif

void
xf86OSInitVidMem(VidMemInfoPtr pVidMem)
{
    xf86OpenConsole();
    
    pVidMem->initialised = TRUE;
    
    pci_system_init_dev_mem(xf86Info.consoleFd);
    xf86EnableIO();
}


volatile unsigned char *ioBase = MAP_FAILED;


static int kmem = -1;

_X_EXPORT int
xf86ReadBIOS(unsigned long Base, unsigned long Offset, unsigned char *Buf,
             int Len)
{

    return -1;
}

#ifdef X_PRIVSEP
/*
 * Do all initialisation that need root privileges
 */
_X_EXPORT void
xf86PrivilegedInit(void)
{
    kmem = open(DEV_MEM, 2);
    if (kmem == -1) {
        xf86Msg(X_PROBED,
                "no aperture driver access: only wsfb driver useable\n");
    }
    pci_system_init();
    xf86OpenConsole();
}
#endif

#ifdef __NetBSD__
Bool xf86EnableIO()
{
    int fd = xf86Info.consoleFd;

    xf86MsgVerb(X_WARNING, 3, "xf86EnableIO %d\n", fd);
    if (ioBase == MAP_FAILED) {
        ioBase = mmap(NULL, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                      0xf2000000);
        xf86MsgVerb(X_INFO, 3, "xf86EnableIO: %08x\n", ioBase);
        if (ioBase == MAP_FAILED) {
            xf86MsgVerb(X_WARNING, 3, "Can't map IO space!\n");
            return FALSE;
        }
    }
    return TRUE;
}

void
xf86DisableIO()
{

    if (ioBase != MAP_FAILED) {
        munmap(__UNVOLATILE(ioBase), 0x10000);
        ioBase = MAP_FAILED;
    }
}

#endif /* __NetBSD__ */
