/*
 * Copyright © 2011 Alexandr Shadchin
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include "synproto.h"
#include "synaptics.h"
#include "synapticsstr.h"
#include <xf86.h>

#include <dev/wscons/wsconsio.h>

#ifdef X_PRIVSEP
extern int priv_open_device(const char *);
#endif

#define DEFAULT_WSMOUSE0_DEV		"/dev/wsmouse0"
#define NEVENTS				64

static const char *synaptics_devs[] = {
    DEFAULT_WSMOUSE0_DEV,
    NULL
};

static Bool
WSConsIsSynaptics(InputInfoPtr pInfo, const char *device)
{
    int wsmouse_type, fd = -1;
    Bool res = FALSE;

#ifndef X_PRIVSEP
    fd = open(synaptics_devs[i], O_RDONLY);
#else
    fd = priv_open_device(device);
#endif

    if (fd < 0)
        return FALSE;

    if (ioctl(fd, WSMOUSEIO_GTYPE, &wsmouse_type) == -1) {
        xf86Msg(X_ERROR, "%s: cannot get mouse type\n", pInfo->name);
        goto out;
    }

    if (wsmouse_type == WSMOUSE_TYPE_SYNAPTICS)
        res = TRUE;

out:
    close(fd);
    return res;
}

static void
WSConsDeviceOnHook(InputInfoPtr pInfo, SynapticsParameters *para)
{
    int wsmouse_mode = WSMOUSE_NATIVE;

    if (ioctl(pInfo->fd, WSMOUSEIO_SETMODE, &wsmouse_mode) == -1)
        xf86Msg(X_ERROR, "%s: cannot set absolute mode\n", pInfo->name);
}

static void
WSConsDeviceOffHook(InputInfoPtr pInfo)
{
    int wsmouse_mode = WSMOUSE_COMPAT;

    if (ioctl(pInfo->fd, WSMOUSEIO_SETMODE, &wsmouse_mode) == -1)
        xf86Msg(X_ERROR, "%s: cannot set relative mode\n", pInfo->name);
}

static Bool
WSConsQueryHardware(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct CommData *comm = &priv->comm;
    int wsmouse_type;

    if (ioctl(pInfo->fd, WSMOUSEIO_GTYPE, &wsmouse_type) == -1) {
        xf86Msg(X_ERROR, "%s: cannot get mouse type\n", pInfo->name);
        return FALSE;
    }

    if (wsmouse_type != WSMOUSE_TYPE_SYNAPTICS)
        return FALSE;

    if (comm->buffer)
        XisbFree(comm->buffer);
    comm->buffer = XisbNew(pInfo->fd, sizeof(struct wscons_event) * NEVENTS);
    if (comm->buffer == NULL)
        return FALSE;

    return TRUE;
}

static Bool
WSConsReadHwState(InputInfoPtr pInfo,
    struct CommData *comm, struct SynapticsHwState *hwRet)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct SynapticsHwState *hw = &(comm->hwState);
    struct wscons_event event;
    unsigned char *pBuf = (unsigned char *)&event;
    int c, n = 0;
    Bool v;

    XisbBlockDuration(comm->buffer, -1);
    while (n < sizeof(struct wscons_event) && (c = XisbRead(comm->buffer)) >= 0)
        pBuf[n++] = (unsigned char)c;

    if (n != sizeof(struct wscons_event))
        return FALSE;

    switch (event.type) {
    case WSCONS_EVENT_MOUSE_UP:
    case WSCONS_EVENT_MOUSE_DOWN:
        v = (event.type == WSCONS_EVENT_MOUSE_DOWN) ? TRUE : FALSE;
        switch (event.value) {
        case 0:
            hw->left = v;
            break;
        case 1:
            hw->middle = v;
            break;
        case 2:
            hw->right = v;
            break;
        case 3:
            hw->up = v;
            break;
        case 4:
            hw->down = v;
            break;
        case 5:
            hw->multi[0] = v;
            break;
        case 6:
            hw->multi[1] = v;
            break;
        case 7:
            hw->multi[2] = v;
            break;
        case 8:
            hw->multi[3] = v;
            break;
        case 9:
            hw->multi[4] = v;
            break;
        case 10:
            hw->multi[5] = v;
            break;
        case 11:
            hw->multi[6] = v;
            break;
        case 12:
            hw->multi[7] = v;
            break;
        }
        break;
    case WSCONS_EVENT_MOUSE_ABSOLUTE_X:
        hw->x = event.value;
        break;
    case WSCONS_EVENT_MOUSE_ABSOLUTE_Y:
        hw->y = priv->maxy - event.value + priv->miny;
        break;
    case WSCONS_EVENT_MOUSE_ABSOLUTE_Z:
        hw->z = event.value;
        break;
    case WSCONS_EVENT_MOUSE_ABSOLUTE_W:
        switch (event.value) {
        case 0:
            hw->fingerWidth = 5;
            hw->numFingers = 2;
            break;
        case 1:
            hw->fingerWidth = 5;
            hw->numFingers = 3;
            break;
        case 4 ... 5:
            hw->fingerWidth = event.value;
            hw->numFingers = 1;
            break;
        }
        break;
    default:
        return FALSE;
    }

    *hwRet = *hw;
    return TRUE;
}

static Bool
WSConsAutoDevProbe(InputInfoPtr pInfo, const char *device)
{
    int i;

    if (device && WSConsIsSynaptics(pInfo, device))
        return TRUE;

    for (i = 0; synaptics_devs[i]; i++)
        if (WSConsIsSynaptics(pInfo, synaptics_devs[i])) {
            xf86Msg(X_PROBED, "%s auto-dev sets device to %s\n",
                pInfo->name, synaptics_devs[i]);
            xf86ReplaceStrOption(pInfo->options, "Device", synaptics_devs[i]);
            return TRUE;
        }

    return FALSE;
}

static void
WSConsReadDevDimensions(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct wsmouse_calibcoords wsmc;

    if (ioctl(pInfo->fd, WSMOUSEIO_GCALIBCOORDS, &wsmc) != 0) {
        xf86Msg(X_ERROR, "%s: failed to query axis range (%s)\n",
            pInfo->name, strerror(errno));
        return;
    }

    priv->minx = wsmc.minx;
    priv->maxx = wsmc.maxx;
    priv->resx = wsmc.resx;
    xf86Msg(X_PROBED, "%s: x-axis range %d - %d resolution %d\n",
        pInfo->name, priv->minx, priv->maxx, priv->resx);

    priv->miny = wsmc.miny;
    priv->maxy = wsmc.maxy;
    priv->resy = wsmc.resy;
    xf86Msg(X_PROBED, "%s: y-axis range %d - %d resolution %d\n",
        pInfo->name, priv->miny, priv->maxy, priv->resy);

    priv->has_pressure = TRUE;
    priv->has_width = TRUE;
    priv->has_left = TRUE;
    priv->has_right = TRUE;
    priv->has_middle = TRUE;
    priv->has_double = TRUE;
    priv->has_triple = TRUE;
    priv->has_scrollbuttons = TRUE;

    priv->model = MODEL_SYNAPTICS;
}

struct SynapticsProtocolOperations wscons_proto_operations = {
    WSConsDeviceOnHook,
    WSConsDeviceOffHook,
    WSConsQueryHardware,
    WSConsReadHwState,
    WSConsAutoDevProbe,
    WSConsReadDevDimensions
};
