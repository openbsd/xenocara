/*
 * Copyright 1999-2006 by VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/*
 * vmmouse_proto.h --
 *
 *      The communication protocol between the guest and the vmmouse
 *      virtual device.
 */


#ifndef _VMMOUSE_PROTO_H_
#define _VMMOUSE_PROTO_H_

#include <stdint.h>

#ifdef HAVE_XORG_SERVER_1_1_0
#include <unistd.h>
#else
#include "xf86_libc.h"
#endif

/* Map Solaris/Sun compiler #defines to gcc equivalents */
#if !defined __i386__ && defined __i386
# define __i386__
#endif

#if !defined __x86_64__ && defined __amd64
# define __x86_64__
#endif

#if !defined __i386__ && !defined __x86_64__
#error The vmmouse protocol is only supported on x86 architectures.
#endif

#define VMMOUSE_PROTO_MAGIC 0x564D5868
#define VMMOUSE_PROTO_PORT 0x5658

#define VMMOUSE_PROTO_CMD_GETVERSION		10
#define VMMOUSE_PROTO_CMD_ABSPOINTER_DATA	39
#define VMMOUSE_PROTO_CMD_ABSPOINTER_STATUS	40
#define VMMOUSE_PROTO_CMD_ABSPOINTER_COMMAND	41
#define VMMOUSE_PROTO_CMD_ABSPOINTER_RESTRICT   86

#define DECLARE_REG32_STRUCT(_r) \
   union { \
      struct { \
         uint16_t low; \
         uint16_t high; \
      } vE##_r##_; \
      uint32_t vE##_r; \
   }

#ifdef __x86_64__

#define DECLARE_REG64_STRUCT(_r) \
   union { \
      DECLARE_REG32_STRUCT(_r); \
      struct { \
         uint32_t low; \
         uint32_t high; \
      } vR##_r##_; \
      uint64_t vR##_r; \
   }

#define DECLARE_REG_STRUCT(x) DECLARE_REG64_STRUCT(x)

#else

#define DECLARE_REG_STRUCT(x) DECLARE_REG32_STRUCT(x)

#endif

typedef union {
   struct {
      union {
         uint32_t magic;
         DECLARE_REG_STRUCT(ax);
      };
      union {
         size_t size;
         DECLARE_REG_STRUCT(bx);
      };
      union {
         uint16_t command;
         DECLARE_REG_STRUCT(cx);
      };
      union {
         uint16_t port;
         DECLARE_REG_STRUCT(dx);
      };
      DECLARE_REG_STRUCT(si);
      DECLARE_REG_STRUCT(di);
   } in;
   struct {
      DECLARE_REG_STRUCT(ax);
      DECLARE_REG_STRUCT(bx);
      DECLARE_REG_STRUCT(cx);
      DECLARE_REG_STRUCT(dx);
      DECLARE_REG_STRUCT(si);
      DECLARE_REG_STRUCT(di);
   } out;
} VMMouseProtoCmd;


void
VMMouseProto_SendCmd(VMMouseProtoCmd *cmd); // IN/OUT


#undef DECLARE_REG_STRUCT

#endif /* _VMMOUSE_PROTO_H_ */
