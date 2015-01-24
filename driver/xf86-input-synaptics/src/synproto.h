/*
 * Copyright © 2004 Peter Osterlund
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
 *      Peter Osterlund (petero2@telia.com)
 */

#ifndef _SYNPROTO_H_
#define _SYNPROTO_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <xf86.h>
#include <xf86Xinput.h>
#include <xisb.h>

#ifndef XI86_SERVER_FD
#define XI86_SERVER_FD 0x20
#endif

struct _SynapticsPrivateRec;
typedef struct _SynapticsPrivateRec SynapticsPrivate;

enum SynapticsSlotState {
    SLOTSTATE_EMPTY = 0,        /* no slot in this cycle */
    SLOTSTATE_OPEN,             /* tracking ID received */
    SLOTSTATE_CLOSE,            /* tracking ID -1 received */
    SLOTSTATE_OPEN_EMPTY,       /* previously had tracking id, no events in this read cycle */
    SLOTSTATE_UPDATE,           /* had tracking id, other events in this cycle */
};

/* used to mark emulated hw button state */
#define BTN_EMULATED_FLAG 0x80

/*
 * A structure to describe the state of the touchpad hardware (buttons and pad)
 */
struct SynapticsHwState {
    CARD32 millis;              /* Timestamp in milliseconds */
    int x;                      /* X position of finger */
    int y;                      /* Y position of finger */
    int z;                      /* Finger pressure */
    int cumulative_dx;          /* Cumulative delta X for clickpad dragging */
    int cumulative_dy;          /* Cumulative delta Y for clickpad dragging */
    int numFingers;
    int fingerWidth;

    Bool left;
    Bool right;
    Bool up;
    Bool down;

    Bool multi[8];
    Bool middle;                /* Some ALPS touchpads have a middle button */

    int num_mt_mask;
    ValuatorMask **mt_mask;
    enum SynapticsSlotState *slot_state;
};

struct CommData {
    XISBuffer *buffer;
    unsigned char protoBuf[6];  /* Buffer for Packet */
    unsigned char lastByte;     /* Last read byte. Use for reset sequence detection. */
    int outOfSync;              /* How many consecutive incorrect packets we
                                   have received */
    int protoBufTail;

    /* Used for keeping track of partial HwState updates. */
    struct SynapticsHwState *hwState;
    Bool oneFinger;
    Bool twoFingers;
    Bool threeFingers;
};

struct _SynapticsParameters;

struct SynapticsProtocolOperations {
    Bool (*DeviceOnHook) (InputInfoPtr pInfo,
                          struct _SynapticsParameters * para);
    Bool (*DeviceOffHook) (InputInfoPtr pInfo);
    Bool (*QueryHardware) (InputInfoPtr pInfo);
    Bool (*ReadHwState) (InputInfoPtr pInfo,
                         struct CommData * comm,
                         struct SynapticsHwState * hwRet);
    Bool (*AutoDevProbe) (InputInfoPtr pInfo, const char *device);
    void (*ReadDevDimensions) (InputInfoPtr pInfo);
};

#ifdef BUILD_PS2COMM
extern struct SynapticsProtocolOperations psaux_proto_operations;
extern struct SynapticsProtocolOperations alps_proto_operations;
#endif                          /* BUILD_PS2COMM */
#ifdef BUILD_EVENTCOMM
extern struct SynapticsProtocolOperations event_proto_operations;
#endif                          /* BUILD_EVENTCOMM */
#ifdef BUILD_PSMCOMM
extern struct SynapticsProtocolOperations psm_proto_operations;
#endif                          /* BUILD_PSMCOMM */
#ifdef BUILD_WSCONSCOMM
extern struct SynapticsProtocolOperations wscons_proto_operations;
#endif                          /* BUILD_WSCONSCOMM */

extern struct SynapticsHwState *SynapticsHwStateAlloc(SynapticsPrivate * priv);
extern void SynapticsHwStateFree(struct SynapticsHwState **hw);
extern void SynapticsCopyHwState(struct SynapticsHwState *dst,
                                 const struct SynapticsHwState *src);
extern void SynapticsResetHwState(struct SynapticsHwState *hw);
extern void SynapticsResetTouchHwState(struct SynapticsHwState *hw,
                                       Bool set_slot_empty);

extern Bool SynapticsIsSoftButtonAreasValid(int *values);

#endif                          /* _SYNPROTO_H_ */
