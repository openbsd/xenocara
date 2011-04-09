/*
 * Copyright © 1997 C. Scott Ananian
 * Copyright © 1998-2000 Bruce Kalk
 * Copyright © 2001 Stefan Gmeiner
 * Copyright © 2002 Linuxcare Inc. David Kennedy
 * Copyright © 2003 Fred Hucht <fred@thp.Uni-Duisburg.de>
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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include "ps2comm.h"
#include "synproto.h"
#include "synaptics.h"
#include "synapticsstr.h"
#include <xf86.h>

#define MAX_UNSYNC_PACKETS 10				/* i.e. 10 to 60 bytes */
/*
 * The x/y limits are taken from the Synaptics TouchPad interfacing Guide,
 * section 2.3.2, which says that they should be valid regardless of the
 * actual size of the sensor.
 */
#define XMIN_NOMINAL 1472
#define XMAX_NOMINAL 5472
#define YMIN_NOMINAL 1408
#define YMAX_NOMINAL 4448

#define XMAX_VALID 6143

/* synaptics queries */
#define SYN_QUE_IDENTIFY		0x00
#define SYN_QUE_MODES			0x01
#define SYN_QUE_CAPABILITIES		0x02
#define SYN_QUE_MODEL			0x03
#define SYN_QUE_SERIAL_NUMBER_PREFIX	0x06
#define SYN_QUE_SERIAL_NUMBER_SUFFIX	0x07
#define SYN_QUE_RESOLUTION		0x08
#define SYN_QUE_EXT_CAPAB		0x09

/* status request response bits (PS2_CMD_STATUS_REQUEST) */
#define PS2_RES_REMOTE(r)	((r) & (1 << 22))
#define PS2_RES_ENABLE(r)	((r) & (1 << 21))
#define PS2_RES_SCALING(r)	((r) & (1 << 20))
#define PS2_RES_LEFT(r)		((r) & (1 << 18))
#define PS2_RES_MIDDLE(r)	((r) & (1 << 17))
#define PS2_RES_RIGHT(r)	((r) & (1 << 16))
#define PS2_RES_RESOLUTION(r)	(((r) >> 8) & 0x03)
#define PS2_RES_SAMPLE_RATE(r)	((r) & 0xff)

#ifdef DEBUG
#define PS2DBG(x) (x)
#else
#define PS2DBG(x)
#endif

struct SynapticsHwInfo {
    unsigned int model_id;		    /* Model-ID */
    unsigned int capabilities;		    /* Capabilities */
    unsigned int ext_cap;		    /* Extended Capabilities */
    unsigned int identity;		    /* Identification */
};

/*****************************************************************************
 *	PS/2 Utility functions.
 *     Many parts adapted from tpconfig.c by C. Scott Ananian
 ****************************************************************************/

/*
 * Read a byte from the ps/2 port
 */
static Bool
ps2_getbyte(int fd, byte *b)
{
    if (xf86WaitForInput(fd, 50000) > 0) {
	if (xf86ReadSerial(fd, b, 1) != 1) {
	    PS2DBG(ErrorF("ps2_getbyte: No byte read\n"));
	    return FALSE;
	}
	PS2DBG(ErrorF("ps2_getbyte: byte %02X read\n", *b));
	return TRUE;
    }
    PS2DBG(ErrorF("ps2_getbyte: timeout xf86WaitForInput\n"));
    return FALSE;
}

/*
 * Write a byte to the ps/2 port, wait for ACK
 */
Bool
ps2_putbyte(int fd, byte b)
{
    byte ack;

    if (xf86WriteSerial(fd, &b, 1) != 1) {
	PS2DBG(ErrorF("ps2_putbyte: error xf86WriteSerial\n"));
	return FALSE;
    }
    PS2DBG(ErrorF("ps2_putbyte: byte %02X send\n", b));
    /* wait for an ACK */
    if (!ps2_getbyte(fd, &ack)) {
	return FALSE;
    }
    if (ack != PS2_ACK) {
	PS2DBG(ErrorF("ps2_putbyte: wrong acknowledge 0x%02x\n", ack));
	return FALSE;
    }
    return TRUE;
}

/*
 * Use the Synaptics extended ps/2 syntax to write a special command byte. Needed by
 * ps2_send_cmd and ps2_set_mode.
 * special command: 0xE8 rr 0xE8 ss 0xE8 tt 0xE8 uu where (rr*64)+(ss*16)+(tt*4)+uu
 *                  is the command. A 0xF3 or 0xE9 must follow (see ps2_send_cmd, ps2_set_mode)
 */
static Bool
ps2_special_cmd(int fd, byte cmd)
{
    int i;

    /* initialize with 'inert' command */
    if (!ps2_putbyte(fd, PS2_CMD_SET_SCALING_1_1))
	return FALSE;

    /* send 4x 2-bits with set resolution command */
    for (i = 0; i < 4; i++) {
	if (!ps2_putbyte(fd, PS2_CMD_SET_RESOLUTION) ||
	    !ps2_putbyte(fd, (cmd >> 6) & 0x3))
	    return FALSE;
	cmd <<= 2;
    }
    return TRUE;
}

/*
 * Send a command to the synpatics touchpad by special commands
 */
static Bool
ps2_send_cmd(int fd, byte c)
{
    PS2DBG(ErrorF("send command: 0x%02X\n", c));
    return (ps2_special_cmd(fd, c) &&
	    ps2_putbyte(fd, PS2_CMD_STATUS_REQUEST));
}

/*****************************************************************************
 *	Synaptics communications functions
 ****************************************************************************/

/*
 * Set the synaptics touchpad mode byte by special commands
 */
static Bool
ps2_synaptics_set_mode(int fd, byte mode)
{
    PS2DBG(ErrorF("set mode byte to: 0x%02X\n", mode));
    return (ps2_special_cmd(fd, mode) &&
	    ps2_putbyte(fd, PS2_CMD_SET_SAMPLE_RATE) &&
	    ps2_putbyte(fd, 0x14));
}

/*
 * reset the touchpad
 */
static Bool
ps2_synaptics_reset(int fd)
{
    byte r[2];

    xf86FlushInput(fd);
    PS2DBG(ErrorF("Reset the Touchpad...\n"));
    if (!ps2_putbyte(fd, PS2_CMD_RESET)) {
	PS2DBG(ErrorF("...failed\n"));
	return FALSE;
    }
    xf86WaitForInput(fd, 4000000);
    if (ps2_getbyte(fd, &r[0]) && ps2_getbyte(fd, &r[1])) {
	if (r[0] == 0xAA && r[1] == 0x00) {
	    PS2DBG(ErrorF("...done\n"));
	    return TRUE;
	} else {
	    PS2DBG(ErrorF("...failed. Wrong reset ack 0x%02x, 0x%02x\n", r[0], r[1]));
	    return FALSE;
	}
    }
    PS2DBG(ErrorF("...failed\n"));
    return FALSE;
}

/*
 * Read the model-id bytes from the touchpad
 * see also SYN_MODEL_* macros
 */
static Bool
ps2_synaptics_model_id(int fd, struct SynapticsHwInfo *synhw)
{
    byte mi[3];

    PS2DBG(ErrorF("Read mode id...\n"));

    synhw->model_id = 0;
    if (ps2_send_cmd(fd, SYN_QUE_MODEL) &&
	ps2_getbyte(fd, &mi[0]) &&
	ps2_getbyte(fd, &mi[1]) &&
	ps2_getbyte(fd, &mi[2])) {
	synhw->model_id = (mi[0] << 16) | (mi[1] << 8) | mi[2];
	PS2DBG(ErrorF("model-id %06X\n", synhw->model_id));
	PS2DBG(ErrorF("...done.\n"));
	return TRUE;
    }
    PS2DBG(ErrorF("...failed.\n"));
    return FALSE;
}

/*
 * Read the capability-bits from the touchpad
 * see also the SYN_CAP_* macros
 */
static Bool
ps2_synaptics_capability(int fd, struct SynapticsHwInfo *synhw)
{
    byte cap[3];

    PS2DBG(ErrorF("Read capabilites...\n"));

    synhw->capabilities = 0;
    synhw->ext_cap = 0;
    if (ps2_send_cmd(fd, SYN_QUE_CAPABILITIES) &&
	ps2_getbyte(fd, &cap[0]) &&
	ps2_getbyte(fd, &cap[1]) &&
	ps2_getbyte(fd, &cap[2])) {
	synhw->capabilities = (cap[0] << 16) | (cap[1] << 8) | cap[2];
	PS2DBG(ErrorF("capabilities %06X\n", synhw->capabilities));
	if (SYN_CAP_VALID(synhw)) {
	    if (SYN_EXT_CAP_REQUESTS(synhw)) {
		if (ps2_send_cmd(fd, SYN_QUE_EXT_CAPAB) &&
		    ps2_getbyte(fd, &cap[0]) &&
		    ps2_getbyte(fd, &cap[1]) &&
		    ps2_getbyte(fd, &cap[2])) {
		    synhw->ext_cap = (cap[0] << 16) | (cap[1] << 8) | cap[2];
		    PS2DBG(ErrorF("ext-capability %06X\n", synhw->ext_cap));
		} else {
		    PS2DBG(ErrorF("synaptics says, that it has extended-capabilities, "
				  "but I cannot read them."));
		}
	    }
	    PS2DBG(ErrorF("...done.\n"));
	    return TRUE;
	}
    }
    PS2DBG(ErrorF("...failed.\n"));
    return FALSE;
}

/*
 * Identify Touchpad
 * See also the SYN_ID_* macros
 */
static Bool
ps2_synaptics_identify(int fd, struct SynapticsHwInfo *synhw)
{
    byte id[3];

    PS2DBG(ErrorF("Identify Touchpad...\n"));

    synhw->identity = 0;
    if (ps2_send_cmd(fd, SYN_QUE_IDENTIFY) &&
	ps2_getbyte(fd, &id[0]) &&
	ps2_getbyte(fd, &id[1]) &&
	ps2_getbyte(fd, &id[2])) {
	synhw->identity = (id[0] << 16) | (id[1] << 8) | id[2];
	PS2DBG(ErrorF("ident %06X\n", synhw->identity));
	if (SYN_ID_IS_SYNAPTICS(synhw)) {
	    PS2DBG(ErrorF("...done.\n"));
	    return TRUE;
	}
    }
    PS2DBG(ErrorF("...failed.\n"));
    return FALSE;
}

static Bool
ps2_synaptics_enable_device(int fd)
{
    return ps2_putbyte(fd, PS2_CMD_ENABLE);
}

static Bool
ps2_synaptics_disable_device(int fd)
{
    xf86FlushInput(fd);
    return ps2_putbyte(fd, PS2_CMD_DISABLE);
}

static Bool
ps2_query_is_synaptics(int fd, struct SynapticsHwInfo* synhw)
{
    int i;

    for (i = 0; i < 3; i++) {
	if (ps2_synaptics_disable_device(fd))
	    break;
    }

    xf86WaitForInput(fd, 20000);
    xf86FlushInput(fd);
    if (ps2_synaptics_identify(fd, synhw)) {
	return TRUE;
    } else {
	xf86Msg(X_ERROR, "Query no Synaptics: %06X\n", synhw->identity);
	return FALSE;
    }
}

void
ps2_print_ident(const struct SynapticsHwInfo *synhw)
{
    xf86Msg(X_PROBED, " Synaptics Touchpad, model: %d\n", SYN_ID_MODEL(synhw));
    xf86Msg(X_PROBED, " Firmware: %d.%d\n", SYN_ID_MAJOR(synhw),
	    SYN_ID_MINOR(synhw));

    if (SYN_MODEL_ROT180(synhw))
	xf86Msg(X_PROBED, " 180 degree mounted touchpad\n");
    if (SYN_MODEL_PORTRAIT(synhw))
	xf86Msg(X_PROBED, " portrait touchpad\n");
    xf86Msg(X_PROBED, " Sensor: %d\n", SYN_MODEL_SENSOR(synhw));
    if (SYN_MODEL_NEWABS(synhw))
	xf86Msg(X_PROBED, " new absolute packet format\n");
    if (SYN_MODEL_PEN(synhw))
	xf86Msg(X_PROBED, " pen detection\n");

    if (SYN_CAP_EXTENDED(synhw)) {
	xf86Msg(X_PROBED, " Touchpad has extended capability bits\n");
	if (SYN_CAP_MULTI_BUTTON_NO(synhw))
	    xf86Msg(X_PROBED, " -> %d multi buttons, i.e. besides standard buttons\n",
		    (int)(SYN_CAP_MULTI_BUTTON_NO(synhw)));
	if (SYN_CAP_MIDDLE_BUTTON(synhw))
	    xf86Msg(X_PROBED, " -> middle button\n");
	if (SYN_CAP_FOUR_BUTTON(synhw))
	    xf86Msg(X_PROBED, " -> four buttons\n");
	if (SYN_CAP_MULTIFINGER(synhw))
	    xf86Msg(X_PROBED, " -> multifinger detection\n");
	if (SYN_CAP_PALMDETECT(synhw))
	    xf86Msg(X_PROBED, " -> palm detection\n");
	if (SYN_CAP_PASSTHROUGH(synhw))
	    xf86Msg(X_PROBED, " -> pass-through port\n");
    }
}

static void
PS2DeviceOffHook(InputInfoPtr pInfo)
{
    ps2_synaptics_reset(pInfo->fd);
    ps2_synaptics_enable_device(pInfo->fd);
}

static Bool
PS2QueryHardware(InputInfoPtr pInfo)
{
    int mode;
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct SynapticsHwInfo *synhw;

    if (!priv->proto_data)
        priv->proto_data = calloc(1, sizeof(struct SynapticsHwInfo));
    synhw = (struct SynapticsHwInfo*)priv->proto_data;

    /* is the synaptics touchpad active? */
    if (!ps2_query_is_synaptics(pInfo->fd, synhw))
	return FALSE;

    xf86Msg(X_PROBED, "%s synaptics touchpad found\n", pInfo->name);

    if (!ps2_synaptics_reset(pInfo->fd))
	xf86Msg(X_ERROR, "%s reset failed\n", pInfo->name);

    if (!ps2_synaptics_identify(pInfo->fd, synhw))
	return FALSE;

    if (!ps2_synaptics_model_id(pInfo->fd, synhw))
	return FALSE;

    if (!ps2_synaptics_capability(pInfo->fd, synhw))
	return FALSE;

    mode = SYN_BIT_ABSOLUTE_MODE | SYN_BIT_HIGH_RATE;
    if (SYN_ID_MAJOR(synhw) >= 4)
	mode |= SYN_BIT_DISABLE_GESTURE;
    if (SYN_CAP_EXTENDED(synhw))
	mode |= SYN_BIT_W_MODE;
    if (!ps2_synaptics_set_mode(pInfo->fd, mode))
	return FALSE;

    ps2_synaptics_enable_device(pInfo->fd);

    ps2_print_ident(synhw);

    return TRUE;
}

/*
 * Decide if the current packet stored in priv->protoBuf is valid.
 */
static Bool
ps2_packet_ok(struct SynapticsHwInfo *synhw, struct CommData *comm)
{
    unsigned char *buf = comm->protoBuf;
    int newabs = SYN_MODEL_NEWABS(synhw);

    if (newabs ? ((buf[0] & 0xC0) != 0x80) : ((buf[0] & 0xC0) != 0xC0)) {
	DBG(4, "Synaptics driver lost sync at 1st byte\n");
	return FALSE;
    }

    if (!newabs && ((buf[1] & 0x60) != 0x00)) {
	DBG(4, "Synaptics driver lost sync at 2nd byte\n");
	return FALSE;
    }

    if ((newabs ? ((buf[3] & 0xC0) != 0xC0) : ((buf[3] & 0xC0) != 0x80))) {
	DBG(4, "Synaptics driver lost sync at 4th byte\n");
	return FALSE;
    }

    if (!newabs && ((buf[4] & 0x60) != 0x00)) {
	DBG(4, "Synaptics driver lost sync at 5th byte\n");
	return FALSE;
    }

    return TRUE;
}

static Bool
ps2_synaptics_get_packet(InputInfoPtr pInfo, struct SynapticsHwInfo *synhw,
			 struct SynapticsProtocolOperations *proto_ops,
			 struct CommData *comm)
{
    int count = 0;
    int c;
    unsigned char u;

    while ((c = XisbRead(comm->buffer)) >= 0) {
	u = (unsigned char)c;

	/* test if there is a reset sequence received */
	if ((c == 0x00) && (comm->lastByte == 0xAA)) {
	    if (xf86WaitForInput(pInfo->fd, 50000) == 0) {
		DBG(7, "Reset received\n");
		proto_ops->QueryHardware(pInfo);
	    } else
		DBG(3, "faked reset received\n");
	}
	comm->lastByte = u;

	/* to avoid endless loops */
	if (count++ > 30) {
	    xf86Msg(X_ERROR, "Synaptics driver lost sync... got gigantic packet!\n");
	    return FALSE;
	}

	comm->protoBuf[comm->protoBufTail++] = u;

	/* Check that we have a valid packet. If not, we are out of sync,
	   so we throw away the first byte in the packet.*/
	if (comm->protoBufTail >= 6) {
	    if (!ps2_packet_ok(synhw, comm)) {
		int i;
		for (i = 0; i < comm->protoBufTail - 1; i++)
		    comm->protoBuf[i] = comm->protoBuf[i + 1];
		comm->protoBufTail--;
		comm->outOfSync++;
		if (comm->outOfSync > MAX_UNSYNC_PACKETS) {
		    comm->outOfSync = 0;
		    DBG(3, "Synaptics synchronization lost too long -> reset touchpad.\n");
		    proto_ops->QueryHardware(pInfo); /* including a reset */
		    continue;
		}
	    }
	}

	if (comm->protoBufTail >= 6) { /* Full packet received */
	    if (comm->outOfSync > 0) {
		comm->outOfSync = 0;
		DBG(4, "Synaptics driver resynced.\n");
	    }
	    comm->protoBufTail = 0;
	    return TRUE;
	}
    }

    return FALSE;
}

static Bool
PS2ReadHwState(InputInfoPtr pInfo,
	       struct SynapticsProtocolOperations *proto_ops,
	       struct CommData *comm, struct SynapticsHwState *hwRet)
{
    unsigned char *buf = comm->protoBuf;
    struct SynapticsHwState *hw = &(comm->hwState);
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    SynapticsParameters *para = &priv->synpara;
    struct SynapticsHwInfo *synhw;
    int newabs;
    int w, i;

    synhw = (struct SynapticsHwInfo*)priv->proto_data;
    if (!synhw)
    {
        xf86Msg(X_ERROR,
                "%s: PS2ReadHwState, synhw is NULL. This is a bug.\n",
                pInfo->name);
        return FALSE;
    }

    newabs = SYN_MODEL_NEWABS(synhw);

    if (!ps2_synaptics_get_packet(pInfo, synhw, proto_ops, comm))
	return FALSE;

    /* Handle normal packets */
    hw->x = hw->y = hw->z = hw->numFingers = hw->fingerWidth = 0;
    hw->left = hw->right = hw->up = hw->down = hw->middle = FALSE;
    for (i = 0; i < 8; i++)
	hw->multi[i] = FALSE;

    if (newabs) {			    /* newer protos...*/
	DBG(7, "using new protocols\n");
	hw->x = (((buf[3] & 0x10) << 8) |
		 ((buf[1] & 0x0f) << 8) |
		 buf[4]);
	hw->y = (((buf[3] & 0x20) << 7) |
		 ((buf[1] & 0xf0) << 4) |
		 buf[5]);

	hw->z = buf[2];
	w = (((buf[0] & 0x30) >> 2) |
	     ((buf[0] & 0x04) >> 1) |
	     ((buf[3] & 0x04) >> 2));

	hw->left  = (buf[0] & 0x01) ? 1 : 0;
	hw->right = (buf[0] & 0x02) ? 1 : 0;

	if (SYN_CAP_EXTENDED(synhw)) {
	    if (SYN_CAP_MIDDLE_BUTTON(synhw)) {
		hw->middle = ((buf[0] ^ buf[3]) & 0x01) ? 1 : 0;
	    }
	    if (SYN_CAP_FOUR_BUTTON(synhw)) {
		hw->up = ((buf[3] & 0x01)) ? 1 : 0;
		if (hw->left)
		    hw->up = !hw->up;
		hw->down = ((buf[3] & 0x02)) ? 1 : 0;
		if (hw->right)
		    hw->down = !hw->down;
	    }
	    if (SYN_CAP_MULTI_BUTTON_NO(synhw)) {
		if ((buf[3] & 2) ? !hw->right : hw->right) {
		    switch (SYN_CAP_MULTI_BUTTON_NO(synhw) & ~0x01) {
		    default:
			break;
		    case 8:
			hw->multi[7] = ((buf[5] & 0x08)) ? 1 : 0;
			hw->multi[6] = ((buf[4] & 0x08)) ? 1 : 0;
		    case 6:
			hw->multi[5] = ((buf[5] & 0x04)) ? 1 : 0;
			hw->multi[4] = ((buf[4] & 0x04)) ? 1 : 0;
		    case 4:
			hw->multi[3] = ((buf[5] & 0x02)) ? 1 : 0;
			hw->multi[2] = ((buf[4] & 0x02)) ? 1 : 0;
		    case 2:
			hw->multi[1] = ((buf[5] & 0x01)) ? 1 : 0;
			hw->multi[0] = ((buf[4] & 0x01)) ? 1 : 0;
		    }
		}
	    }
	}
    } else {			    /* old proto...*/
	DBG(7, "using old protocol\n");
	hw->x = (((buf[1] & 0x1F) << 8) |
		 buf[2]);
	hw->y = (((buf[4] & 0x1F) << 8) |
		 buf[5]);

	hw->z = (((buf[0] & 0x30) << 2) |
		 (buf[3] & 0x3F));
	w = (((buf[1] & 0x80) >> 4) |
	     ((buf[0] & 0x04) >> 1));

	hw->left  = (buf[0] & 0x01) ? 1 : 0;
	hw->right = (buf[0] & 0x02) ? 1 : 0;
    }

    hw->y = YMAX_NOMINAL + YMIN_NOMINAL - hw->y;

    if (hw->z >= para->finger_high) {
	int w_ok = 0;
	/*
	 * Use capability bits to decide if the w value is valid.
	 * If not, set it to 5, which corresponds to a finger of
	 * normal width.
	 */
	if (SYN_CAP_EXTENDED(synhw)) {
	    if ((w >= 0) && (w <= 1)) {
		w_ok = SYN_CAP_MULTIFINGER(synhw);
	    } else if (w == 2) {
		w_ok = SYN_MODEL_PEN(synhw);
	    } else if ((w >= 4) && (w <= 15)) {
		w_ok = SYN_CAP_PALMDETECT(synhw);
	    }
	}
	if (!w_ok)
	    w = 5;

	switch (w) {
	case 0:
	    hw->numFingers = 2;
	    hw->fingerWidth = 5;
	    break;
	case 1:
	    hw->numFingers = 3;
	    hw->fingerWidth = 5;
	    break;
	default:
	    hw->numFingers = 1;
	    hw->fingerWidth = w;
	    break;
	}
    }

    *hwRet = *hw;
    return TRUE;
}

static Bool
PS2AutoDevProbe(InputInfoPtr pInfo)
{
    return FALSE;
}

struct SynapticsProtocolOperations psaux_proto_operations = {
    NULL,
    PS2DeviceOffHook,
    PS2QueryHardware,
    PS2ReadHwState,
    PS2AutoDevProbe,
    SynapticsDefaultDimensions
};
