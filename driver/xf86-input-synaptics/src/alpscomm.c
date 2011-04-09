/*
 * Copyright © 2001 Stefan Gmeiner
 * Copyright © 2003 Neil Brown
 * Copyright © 2003-2005,2007 Peter Osterlund
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
 *      Neil Brown (neilb@cse.unsw.edu.au)
 *      Peter Osterlund (petero2@telia.com)
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include "alpscomm.h"
#include "synproto.h"
#include "synaptics.h"
#include "synapticsstr.h"
#include <xf86.h>


/* Wait for the channel to go silent, which means we're in sync */
static void
ALPS_sync(int fd)
{
    byte buffer[64];
    while (xf86WaitForInput(fd, 250000) > 0) {
	xf86ReadSerial(fd, &buffer, 64);
    }
}

/*
 * send the ALPS init sequence, ie 4 consecutive "disable"s before the "enable"
 * This "magic knock" is performed both for the trackpad and for the pointing
 * stick. Not all models have a pointing stick, but trying to initialize it
 * anyway doesn't seem to hurt.
 */
static void
ALPS_initialize(int fd)
{
    xf86FlushInput(fd);
    ps2_putbyte(fd, PS2_CMD_SET_DEFAULT);
    ps2_putbyte(fd, PS2_CMD_SET_SCALING_2_1);
    ps2_putbyte(fd, PS2_CMD_SET_SCALING_2_1);
    ps2_putbyte(fd, PS2_CMD_SET_SCALING_2_1);
    ps2_putbyte(fd, PS2_CMD_DISABLE);

    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_ENABLE);

    ps2_putbyte(fd, PS2_CMD_SET_SCALING_1_1);
    ps2_putbyte(fd, PS2_CMD_SET_SCALING_1_1);
    ps2_putbyte(fd, PS2_CMD_SET_SCALING_1_1);
    ps2_putbyte(fd, PS2_CMD_DISABLE);

    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_DISABLE);
    ps2_putbyte(fd, PS2_CMD_ENABLE);

    ALPS_sync(fd);
}

static Bool
ALPSQueryHardware(InputInfoPtr pInfo)
{
    ALPS_initialize(pInfo->fd);
    return TRUE;
}

static Bool
ALPS_packet_ok(struct CommData *comm)
{
    /* ALPS absolute mode packets start with 0b11111mrl */
    if ((comm->protoBuf[0] & 0xf8) == 0xf8)
	return TRUE;
    return FALSE;
}

static Bool
ALPS_get_packet(struct CommData *comm, InputInfoPtr pInfo)
{
    int c;

    while ((c = XisbRead(comm->buffer)) >= 0) {
	unsigned char u = (unsigned char)c;

	comm->protoBuf[comm->protoBufTail++] = u;

	if (comm->protoBufTail == 3) { /* PS/2 packet received? */
	    if ((comm->protoBuf[0] & 0xc8) == 0x08) {
		comm->protoBufTail = 0;
		return TRUE;
	    }
	}

	if (comm->protoBufTail >= 6) { /* Full packet received */
	    comm->protoBufTail = 0;
	    if (ALPS_packet_ok(comm))
		return TRUE;
	    while ((c = XisbRead(comm->buffer)) >= 0)
		;		   /* If packet is invalid, re-sync */
	}
    }

    return FALSE;
}

/*
 * ALPS abolute Mode
 * byte 0: 1 1 1 1 1 mid0 rig0 lef0
 * byte 1: 0 x6 x5 x4 x3 x2 x1 x0
 * byte 2: 0 x10 x9 x8 x7 up1 fin ges
 * byte 3: 0 y9 y8 y7 1 mid1 rig1 lef1
 * byte 4: 0 y6 y5 y4 y3 y2 y1 y0
 * byte 5: 0 z6 z5 z4 z3 z2 z1 z0
 *
 * On a dualpoint, {mid,rig,lef}0 are the stick, 1 are the pad.
 * We just 'or' them together for now.
 *
 * The touchpad on an 'Acer Aspire' has 4 buttons:
 *   left,right,up,down.
 * This device always sets {mid,rig,lef}0 to 1 and
 * reflects left,right,down,up in lef1,rig1,mid1,up1.
 */
static void
ALPS_process_packet(unsigned char *packet, struct SynapticsHwState *hw)
{
    int x = 0, y = 0, z = 0;
    int left = 0, right = 0, middle = 0;
    int i;

    x = (packet[1] & 0x7f) | ((packet[2] & 0x78) << (7-3));
    y = (packet[4] & 0x7f) | ((packet[3] & 0x70) << (7-4));
    z = packet[5];

    if (z == 127) {    /* DualPoint stick is relative, not absolute */
	hw->left  = packet[3] & 1;
	hw->right = (packet[3] >> 1) & 1;
	return;
    }

    /* Handle normal packets */
    hw->x = hw->y = hw->z = hw->numFingers = hw->fingerWidth = 0;
    hw->left = hw->right = hw->up = hw->down = hw->middle = FALSE;
    for (i = 0; i < 8; i++)
	hw->multi[i] = FALSE;

    if (z > 0) {
	hw->x = x;
	hw->y = y;
    }
    hw->z = z;
    hw->numFingers = (z > 0) ? 1 : 0;
    hw->fingerWidth = 5;

    left  |= (packet[2]     ) & 1;
    left  |= (packet[3]     ) & 1;
    right |= (packet[3] >> 1) & 1;
    if (packet[0] == 0xff) {
	int back    = (packet[3] >> 2) & 1;
	int forward = (packet[2] >> 2) & 1;
	if (back && forward) {
	    middle = 1;
	    back = 0;
	    forward = 0;
	}
	hw->down = back;
	hw->up = forward;
    } else {
	left   |= (packet[0]     ) & 1;
	right  |= (packet[0] >> 1) & 1;
	middle |= (packet[0] >> 2) & 1;
	middle |= (packet[3] >> 2) & 1;
    }

    hw->left = left;
    hw->right = right;
    hw->middle = middle;
}

static Bool
ALPSReadHwState(InputInfoPtr pInfo,
		struct SynapticsProtocolOperations *proto_ops,
		struct CommData *comm, struct SynapticsHwState *hwRet)
{
    unsigned char *buf = comm->protoBuf;
    struct SynapticsHwState *hw = &(comm->hwState);

    if (!ALPS_get_packet(comm, pInfo))
	return FALSE;

    ALPS_process_packet(buf, hw);

    *hwRet = *hw;
    return TRUE;
}

static Bool
ALPSAutoDevProbe(InputInfoPtr pInfo)
{
    return FALSE;
}

struct SynapticsProtocolOperations alps_proto_operations = {
    NULL,
    NULL,
    ALPSQueryHardware,
    ALPSReadHwState,
    ALPSAutoDevProbe,
    SynapticsDefaultDimensions
};
