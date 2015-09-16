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

#include <sys/param.h>
#ifndef __NetBSD__
#include <sys/sysctl.h>
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__)
#include <machine/sysarch.h>
#endif

#include "shared/xf86Axp.h"

#include "xf86_OSlib.h"
#include "xf86OSpriv.h"

#if defined(__NetBSD__) && !defined(MAP_FILE)
#define MAP_FLAGS MAP_SHARED
#else
#define MAP_FLAGS (MAP_FILE | MAP_SHARED)
#endif

#ifndef __NetBSD__
extern unsigned long dense_base(void);
#else                           /* __NetBSD__ */
static struct alpha_bus_window *abw;
static int abw_count = -1;

static void
init_abw(void)
{
    if (abw_count < 0) {
        abw_count = alpha_bus_getwindows(ALPHA_BUS_TYPE_PCI_MEM, &abw);
        if (abw_count <= 0)
            FatalError("init_abw: alpha_bus_getwindows failed\n");
    }
}

static unsigned long
dense_base(void)
{
    if (abw_count < 0)
        init_abw();

    /* XXX check abst_flags for ABST_DENSE just to be safe? */
    xf86Msg(X_INFO, "dense base = %#lx\n", abw[0].abw_abst.abst_sys_start);     /* XXXX */
    return abw[0].abw_abst.abst_sys_start;
}

static unsigned long
memory_base(void)
{
    if (abw_count < 0)
        init_abw();

    if (abw_count > 0) {
        xf86Msg(X_INFO, "memory base = %#lx\n",
                abw[1].abw_abst.abst_sys_start); /* XXXX */
        return abw[1].abw_abst.abst_sys_start;
    }
    else {
        xf86Msg(X_INFO, "no memory base\n"); /* XXXX */
        return 0;
    }
}
#endif                          /* __NetBSD__ */

#define BUS_BASE	dense_base()
#define BUS_BASE_BWX	memory_base()

/***************************************************************************/
/* Video Memory Mapping section                                            */
/***************************************************************************/

#ifdef __OpenBSD__
#define SYSCTL_MSG "\tCheck that you have set 'machdep.allowaperture=1'\n"\
                  "\tin /etc/sysctl.conf and reboot your machine\n" \
                  "\trefer to xf86(4) for details"
#endif

static int devMemFd = -1;

#ifdef HAS_APERTURE_DRV
#define DEV_APERTURE "/dev/xf86"
#endif

/*
 * Check if /dev/mem can be mmap'd.  If it can't print a warning when
 * "warn" is TRUE.
 */
static void
checkDevMem(Bool warn)
{
    static Bool devMemChecked = FALSE;
    int fd;
    void *base;

    if (devMemChecked)
        return;
    devMemChecked = TRUE;

#ifdef HAS_APERTURE_DRV
    /* Try the aperture driver first */
    if ((fd = open(DEV_APERTURE, O_RDWR)) >= 0) {
        /* Try to map a page at the VGA address */
        base = mmap((caddr_t) 0, 4096, PROT_READ | PROT_WRITE,
                    MAP_FLAGS, fd, (off_t) 0xA0000 + BUS_BASE);

        if (base != MAP_FAILED) {
            munmap((caddr_t) base, 4096);
            devMemFd = fd;
            xf86Msg(X_INFO, "checkDevMem: using aperture driver %s\n",
                    DEV_APERTURE);
            return;
        }
        else {
            if (warn) {
                xf86Msg(X_WARNING, "checkDevMem: failed to mmap %s (%s)\n",
                        DEV_APERTURE, strerror(errno));
            }
        }
    }
#endif
    if ((fd = open(DEV_MEM, O_RDWR)) >= 0) {
        /* Try to map a page at the VGA address */
        base = mmap((caddr_t) 0, 4096, PROT_READ | PROT_WRITE,
                    MAP_FLAGS, fd, (off_t) 0xA0000 + BUS_BASE);

        if (base != MAP_FAILED) {
            munmap((caddr_t) base, 4096);
            devMemFd = fd;
            return;
        }
        else {
            if (warn) {
                xf86Msg(X_WARNING, "checkDevMem: failed to mmap %s (%s)\n",
                        DEV_MEM, strerror(errno));
            }
        }
    }
    if (warn) {
#ifndef HAS_APERTURE_DRV
        xf86Msg(X_WARNING, "checkDevMem: failed to open/mmap %s (%s)\n",
                DEV_MEM, strerror(errno));
#else
#ifndef __OpenBSD__
        xf86Msg(X_WARNING, "checkDevMem: failed to open %s and %s\n"
                "\t(%s)\n", DEV_APERTURE, DEV_MEM, strerror(errno));
#else                           /* __OpenBSD__ */
        xf86Msg(X_WARNING, "checkDevMem: failed to open %s and %s\n"
                "\t(%s)\n%s", DEV_APERTURE, DEV_MEM, strerror(errno),
                SYSCTL_MSG);
#endif                          /* __OpenBSD__ */
#endif
        xf86ErrorF("\tlinear framebuffer access unavailable\n");
    }
    return;
}

void
xf86OSInitVidMem(VidMemInfoPtr pVidMem)
{
    checkDevMem(TRUE);
    pVidMem->linearSupported = useDevMem;

    if (has_bwx()) {
        xf86Msg(X_PROBED, "Machine type has 8/16 bit access\n");
        pVidMem->mapMem = mapVidMem;
        pVidMem->unmapMem = unmapVidMem;
    }
    else {
        xf86Msg(X_PROBED, "Machine needs sparse mapping\n");
        pVidMem->mapMem = mapVidMemSparse;
        pVidMem->unmapMem = unmapVidMemSparse;
#ifndef __NetBSD__
        if (axpSystem == -1)
            axpSystem = bsdGetAXP();
        hae_thresh = xf86AXPParams[axpSystem].hae_thresh;
        hae_mask = xf86AXPParams[axpSystem].hae_mask;
#endif                          /* __NetBSD__ */
    }
    pVidMem->initialised = TRUE;
}

static void *
mapVidMem(int ScreenNum, unsigned long Base, unsigned long Size, int flags)
{
    void *base;

    checkDevMem(FALSE);
    Base = Base & ((1L << 32) - 1);

    if (useDevMem) {
        if (devMemFd < 0) {
            FatalError("xf86MapVidMem: failed to open %s (%s)\n",
                       DEV_MEM, strerror(errno));
        }
        base = mmap((caddr_t) 0, Size,
                    (flags & VIDMEM_READONLY) ?
                    PROT_READ : (PROT_READ | PROT_WRITE),
                    MAP_FLAGS, devMemFd, (off_t) Base + BUS_BASE_BWX);
        if (base == MAP_FAILED) {
            FatalError("%s: could not mmap %s [s=%lx,a=%lx] (%s)\n",
                       "xf86MapVidMem", DEV_MEM, Size, Base, strerror(errno));
        }
        return base;
    }

    /* else, mmap /dev/vga */
    if ((unsigned long) Base < 0xA0000 || (unsigned long) Base >= 0xC0000) {
        FatalError("%s: Address 0x%lx outside allowable range\n",
                   "xf86MapVidMem", Base);
    }
    base = mmap(0, Size,
                (flags & VIDMEM_READONLY) ?
                PROT_READ : (PROT_READ | PROT_WRITE),
                MAP_FLAGS, xf86Info.consoleFd, (unsigned long) Base + BUS_BASE);
    if (base == MAP_FAILED) {
        FatalError("xf86MapVidMem: Could not mmap /dev/vga (%s)\n",
                   strerror(errno));
    }
    return base;
}

static void
unmapVidMem(int ScreenNum, void *Base, unsigned long Size)
{
    munmap((caddr_t) Base, Size);
}

/*
 * Read BIOS via mmap()ing DEV_MEM
 */

_X_EXPORT int
xf86ReadBIOS(unsigned long Base, unsigned long Offset, unsigned char *Buf,
             int Len)
{
    unsigned char *ptr;
    int psize;
    int mlen;

    checkDevMem(TRUE);
    if (devMemFd == -1) {
        return -1;
    }

    psize = getpagesize();
    Offset += Base & (psize - 1);
    Base &= ~(psize - 1);
    mlen = (Offset + Len + psize - 1) & ~(psize - 1);
    ptr = (unsigned char *) mmap((caddr_t) 0, mlen, PROT_READ,
                                 MAP_SHARED, devMemFd, (off_t) Base + BUS_BASE);
    if ((long) ptr == -1) {
        xf86Msg(X_WARNING,
                "xf86ReadBIOS: %s mmap[s=%x,a=%lx,o=%lx] failed (%s)\n",
                DEV_MEM, Len, Base, Offset, strerror(errno));
        return -1;
    }
#ifdef DEBUG
    xf86MsgVerb(X_INFO, 3,
                "xf86ReadBIOS: BIOS at 0x%08x has signature 0x%04x\n", Base,
                ptr[0] | (ptr[1] << 8));
#endif
    (void) memcpy(Buf, (void *) (ptr + Offset), Len);
    (void) munmap((caddr_t) ptr, mlen);
#ifdef DEBUG
    xf86MsgVerb(X_INFO, 3, "xf86ReadBIOS(%x, %x, Buf, %x)"
                "-> %02x %02x %02x %02x...\n",
                Base, Offset, Len, Buf[0], Buf[1], Buf[2], Buf[3]);
#endif
    return Len;
}

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__OpenBSD__)

extern int ioperm(unsigned long from, unsigned long num, int on);

_X_EXPORT Bool
xf86EnableIO()
{
    if (!ioperm(0, 65536, TRUE))
        return TRUE;
    return FALSE;
}

_X_EXPORT void
xf86DisableIO()
{
    return;
}

#endif                          /* __FreeBSD_kernel__ || __OpenBSD__ */

#ifdef USE_ALPHA_PIO

Bool
xf86EnableIO()
{
    alpha_pci_io_enable(1);
    return TRUE;
}

void
xf86DisableIO()
{
    alpha_pci_io_enable(0);
}

#endif                          /* USE_ALPHA_PIO */

extern int readDense8(void *Base, register unsigned long Offset);
extern int readDense16(void *Base, register unsigned long Offset);
extern int readDense32(void *Base, register unsigned long Offset);
extern void
 writeDense8(int Value, void *Base, register unsigned long Offset);
extern void
 writeDense16(int Value, void *Base, register unsigned long Offset);
extern void
 writeDense32(int Value, void *Base, register unsigned long Offset);

void (*xf86WriteMmio8) (int Value, void *Base, unsigned long Offset)
    = writeDense8;
void (*xf86WriteMmio16) (int Value, void *Base, unsigned long Offset)
    = writeDense16;
void (*xf86WriteMmio32) (int Value, void *Base, unsigned long Offset)
    = writeDense32;
int (*xf86ReadMmio8) (void *Base, unsigned long Offset)
    = readDense8;
int (*xf86ReadMmio16) (void *Base, unsigned long Offset)
    = readDense16;
int (*xf86ReadMmio32) (void *Base, unsigned long Offset)
    = readDense32;

/*
 * Do all things that need root privileges early 
 * and revoke those priviledges 
 */
_X_EXPORT void
xf86PrivilegedInit(void)
{
    xf86EnableIO();
    checkDevMem(TRUE);
    pci_system_init();
    xf86OpenConsole();
}

