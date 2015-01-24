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

#ifndef _EVENTCOMM_H_
#define _EVENTCOMM_H_

#include <xorg-server.h>

#include <linux/input.h>
#include <linux/version.h>
#include <xf86Xinput.h>
#include "synproto.h"

/* for auto-dev: */
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

struct eventcomm_proto_data;

extern struct eventcomm_proto_data *EventProtoDataAlloc(int fd);

extern Bool
EventReadHwState(InputInfoPtr pInfo,
                 struct CommData *comm, struct SynapticsHwState *hwRet);

#endif                          /* _EVENTCOMM_H_ */
