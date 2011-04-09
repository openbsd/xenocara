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

#include <unistd.h>
#include <sys/ioctl.h>
#include <xf86Xinput.h>
#include <xisb.h>

/*
 * A structure to describe the state of the touchpad hardware (buttons and pad)
 */
struct SynapticsHwState {
    int millis;			/* Timestamp in milliseconds */
    int x;			/* X position of finger */
    int y;			/* Y position of finger */
    int z;			/* Finger pressure */
    int numFingers;
    int fingerWidth;

    Bool left;
    Bool right;
    Bool up;
    Bool down;

    Bool multi[8];
    Bool middle;		/* Some ALPS touchpads have a middle button */
};

struct CommData {
    XISBuffer *buffer;
    unsigned char protoBuf[6];		/* Buffer for Packet */
    unsigned char lastByte;		/* Last read byte. Use for reset sequence detection. */
    int outOfSync;			/* How many consecutive incorrect packets we
					   have received */
    int protoBufTail;

    /* Used for keeping track of partial HwState updates. */
    struct SynapticsHwState hwState;
    Bool oneFinger;
    Bool twoFingers;
    Bool threeFingers;
};

enum SynapticsProtocol {
    SYN_PROTO_PSAUX,		/* Raw psaux device */
#ifdef BUILD_EVENTCOMM
    SYN_PROTO_EVENT,		/* Linux kernel event interface */
#endif /* BUILD_EVENTCOMM */
#ifdef BUILD_PSMCOMM
    SYN_PROTO_PSM,		/* FreeBSD psm driver */
#endif /* BUILD_PSMCOMM */
    SYN_PROTO_ALPS		/* ALPS touchpad protocol */
};

struct _SynapticsParameters;
struct SynapticsHwInfo;
struct CommData;

struct SynapticsProtocolOperations {
    void (*DeviceOnHook)(InputInfoPtr pInfo, struct _SynapticsParameters *para);
    void (*DeviceOffHook)(InputInfoPtr pInfo);
    Bool (*QueryHardware)(InputInfoPtr pInfo);
    Bool (*ReadHwState)(InputInfoPtr pInfo,
			struct SynapticsProtocolOperations *proto_ops,
			struct CommData *comm, struct SynapticsHwState *hwRet);
    Bool (*AutoDevProbe)(InputInfoPtr pInfo);
    void (*ReadDevDimensions)(InputInfoPtr pInfo);
};

extern struct SynapticsProtocolOperations psaux_proto_operations;
#ifdef BUILD_EVENTCOMM
extern struct SynapticsProtocolOperations event_proto_operations;
#endif /* BUILD_EVENTCOMM */
#ifdef BUILD_PSMCOMM
extern struct SynapticsProtocolOperations psm_proto_operations;
#endif /* BUILD_PSMCOMM */
extern struct SynapticsProtocolOperations alps_proto_operations;


#endif /* _SYNPROTO_H_ */
