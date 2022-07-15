/*
 * Copyright © 1997 C. Scott Ananian
 * Copyright © 1998-2000 Bruce Kalk
 * Copyright © 2001 Stefan Gmeiner
 * Copyright © 2002 Linuxcare Inc. David Kennedy
 * Copyright © 2003 Fred Hucht
 * Copyright © 2004 Arne Schwabe
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of Red Hat
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  Red
 * Hat makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:
 *      Stefan Gmeiner (riddlebox@freesurf.ch)
 *      C. Scott Ananian (cananian@alumni.priceton.edu)
 *      Bruce Kalk (kall@compass.com)
 *      Linuxcare Inc. David Kennedy (dkennedy@linuxcare.com)
 *      Fred Hucht (fred@thp.Uni-Duisburg.de)
 *      Arne Schwabe <schwabe@uni-paderborn.de>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mouse.h>
#include <errno.h>
#include <string.h>
#include "synproto.h"
#include "synapticsstr.h"
#include "ps2comm.h"            /* ps2_print_ident() */
#include <xf86.h>

#define SYSCALL(call) while (((call) == -1) && (errno == EINTR))

/*
 * Identify Touchpad
 * See also the SYN_ID_* macros
 */
static Bool
psm_synaptics_identify(int fd, synapticshw_t * ident)
{
    int ret;

    SYSCALL(ret = ioctl(fd, MOUSE_SYN_GETHWINFO, ident));
    if (ret == 0)
        return TRUE;
    else
        return FALSE;
}

/* This define is used in a ioctl but not in mouse.h :/ */
#define PSM_LEVEL_NATIVE	2

static Bool
PSMQueryIsSynaptics(InputInfoPtr pInfo)
{
    int ret;
    int level = PSM_LEVEL_NATIVE;
    mousehw_t mhw;

    /* Put the device in native protocol mode to be sure
     * Otherwise HWINFO will not return the right id
     * And we will need native mode anyway ...
     */
    SYSCALL(ret = ioctl(pInfo->fd, MOUSE_SETLEVEL, &level));
    if (ret != 0) {
        xf86IDrvMsg(pInfo, X_ERROR, "%s Can't set native mode\n", pInfo->name);
        return FALSE;
    }
    SYSCALL(ret = ioctl(pInfo->fd, MOUSE_GETHWINFO, &mhw));
    if (ret != 0) {
        xf86IDrvMsg(pInfo, X_ERROR, "%s Can't get hardware info\n",
                    pInfo->name);
        return FALSE;
    }

    if (mhw.model == MOUSE_MODEL_SYNAPTICS) {
        return TRUE;
    }
    else {
        xf86IDrvMsg(pInfo, X_ERROR,
                    "%s Found no Synaptics, found Mouse model %d instead\n",
                    pInfo->name, mhw.model);
        return FALSE;
    }
}

static void
convert_hw_info(const synapticshw_t * psm_ident,
                struct PS2SynapticsHwInfo *synhw)
{
    memset(synhw, 0, sizeof(*synhw));
    synhw->model_id = ((psm_ident->infoRot180 << 23) |
                       (psm_ident->infoPortrait << 22) |
                       (psm_ident->infoSensor << 16) |
                       (psm_ident->infoHardware << 9) |
                       (psm_ident->infoNewAbs << 7) |
                       (psm_ident->capPen << 6) |
                       (psm_ident->infoSimplC << 5) |
                       (psm_ident->infoGeometry));
    synhw->capabilities = ((psm_ident->capExtended << 23) |
                           (psm_ident->capPassthrough << 7) |
                           (psm_ident->capSleep << 4) |
                           (psm_ident->capFourButtons << 3) |
                           (psm_ident->capMultiFinger << 1) |
                           (psm_ident->capPalmDetect));
    synhw->ext_cap = 0;
    synhw->identity = ((psm_ident->infoMajor) |
                       (0x47 << 8) | (psm_ident->infoMinor << 16));
}

static Bool
PSMQueryHardware(InputInfoPtr pInfo)
{
    synapticshw_t psm_ident;
    struct PS2SynapticsHwInfo *synhw;
    SynapticsPrivate *priv;

    priv = (SynapticsPrivate *) pInfo->private;

    if (!priv->proto_data)
        priv->proto_data = calloc(1, sizeof(struct PS2SynapticsHwInfo));
    synhw = (struct PS2SynapticsHwInfo *) priv->proto_data;

    /* is the synaptics touchpad active? */
    if (!PSMQueryIsSynaptics(pInfo))
        return FALSE;

    xf86IDrvMsg(pInfo, X_PROBED, "synaptics touchpad found\n");

    if (!psm_synaptics_identify(pInfo->fd, &psm_ident))
        return FALSE;

    convert_hw_info(&psm_ident, synhw);

    ps2_print_ident(pInfo, synhw);

    return TRUE;
}

static Bool
PSMReadHwState(InputInfoPtr pInfo,
               struct CommData *comm, struct SynapticsHwState *hwRet)
{
    return PS2ReadHwStateProto(pInfo, &psm_proto_operations, comm, hwRet);
}

struct SynapticsProtocolOperations psm_proto_operations = {
    NULL,
    NULL,
    PSMQueryHardware,
    PSMReadHwState,
    NULL,
    NULL
};
