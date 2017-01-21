/*
 * Copyright 2002-2006 by VMware, Inc.
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
 * vmmouse_client.c --
 *
 *      VMware Virtual Mouse Client
 *
 * This module provides functions to enable, operate and process
 * packets via the VMMouse module hosted in the VMX.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vmmouse_client.h"
#include "vmmouse_proto.h"

/*
 *----------------------------------------------------------------------------
 *
 * VMMouseClientVMCheck --
 *
 *      Checks if we're running in a VM by sending the GETVERSION command.
 *
 * Returns:
 *      0 if we're running natively/the version command failed,
 *      1 if we're in a VM.
 *
 *----------------------------------------------------------------------------
 */

static Bool
VMMouseClientVMCheck(void)
{
   VMMouseProtoCmd vmpc;

   vmpc.in.vEbx = ~VMMOUSE_PROTO_MAGIC;
   vmpc.in.command = VMMOUSE_PROTO_CMD_GETVERSION;
   VMMouseProto_SendCmd(&vmpc);

   /*
    * ebx should contain VMMOUSE_PROTO_MAGIC
    * eax should contain version
    */
   if (vmpc.out.vEbx != VMMOUSE_PROTO_MAGIC || vmpc.out.vEax == 0xffffffff) {
      return FALSE;
   }

   return TRUE;
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseClient_Disable --
 *
 *	Tries to disable VMMouse communication mode on the host.
 *	The caller is responsible for maintaining state (we don't check
 *	if we're enabled before attempting to disable the VMMouse).
 *
 * Results:
 *	TRUE if we successfully disable the VMMouse communication mode,
 *	FALSE if something went wrong.
 *
 * Side effects:
 *	Disables the absolute positioning mode.
 *
 *----------------------------------------------------------------------
 */

void
VMMouseClient_Disable(void)
{
   uint32_t status;
   VMMouseProtoCmd vmpc;

   VMwareLog(("VMMouseClient_Disable: writing disable command to port\n"));
   vmpc.in.vEbx = VMMOUSE_CMD_DISABLE;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_COMMAND;
   VMMouseProto_SendCmd(&vmpc);
   /*
    * We should get 0xffff in the flags now.
    */
   vmpc.in.vEbx = 0;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_STATUS;
   VMMouseProto_SendCmd(&vmpc);
   status = vmpc.out.vEax;
   if ((status & VMMOUSE_ERROR) != VMMOUSE_ERROR) {
      VMwareLog(("VMMouseClient_Disable: wrong status returned\n"));
   }
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseClient_Enable --
 *
 *	Public Enable entry point. The driver calls this once it feels
 *	ready to deal with VMMouse stuff. For now, we just try to enable
 *	and return the result, but conceivably we could do more.
 *
 * Results:
 *	TRUE if the enable succeeds, FALSE otherwise.
 *
 * Side effects:
 *	Causes host-side state change.
 *
 *----------------------------------------------------------------------
 */

Bool
VMMouseClient_Enable(void) {

   uint32_t status;
   uint32_t data;
   VMMouseProtoCmd vmpc;

   /*
    * First, make sure we're in a VM; i.e. in dualboot configurations we might
    * find ourselves running on real hardware.
    */

   if (!VMMouseClientVMCheck()) {
      return FALSE;
   }

   VMwareLog(("VMMouseClientVMCheck succeeded, checking VMMOUSE version\n"));
   VMwareLog(("VMMouseClient_Enable: READ_ID 0x%08x, VERSION_ID 0x%08x\n",
       VMMOUSE_CMD_READ_ID, VMMOUSE_VERSION_ID));

   /*
    * We probe for the VMMouse backend by sending the ENABLE
    * command to the mouse. We should get back the VERSION_ID on
    * the data port.
    */
   vmpc.in.vEbx = VMMOUSE_CMD_READ_ID;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_COMMAND;
   VMMouseProto_SendCmd(&vmpc);

   /*
    * Check whether the VMMOUSE_VERSION_ID is available to read
    */
   vmpc.in.vEbx = 0;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_STATUS;
   VMMouseProto_SendCmd(&vmpc);
   status = vmpc.out.vEax;
   if ((status & 0x0000ffff) == 0) {
      VMwareLog(("VMMouseClient_Enable: no data on port."));
      return FALSE;
   }

   /*
    * Get the VMMOUSE_VERSION_ID then
    */
   /* Get just one item */
   vmpc.in.vEbx = 1;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_DATA;
   VMMouseProto_SendCmd(&vmpc);
   data = vmpc.out.vEax;
   if (data!= VMMOUSE_VERSION_ID) {
      VMwareLog(("VMMouseClient_Enable: data was not VERSION_ID"));
      return FALSE;
   }

   /*
    * Restrict access to the VMMouse backdoor handler.
    */
   vmpc.in.vEbx = VMMOUSE_RESTRICT_IOPL;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_RESTRICT;
   VMMouseProto_SendCmd(&vmpc);

   /*
    * To quote Jeremy, "Go Go Go!"
    */

   VMwareLog(("VMMouseClient_Enable: go go go!\n"));
   return TRUE;
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseClient_GetInput --
 *
 *	Retrieves a 4-word input packet from the VMMouse data port and
 *	stores it in the specified input structure.
 *
 * Results:
 *	The number of packets in the queue, including the retrieved
 *	packet.
 *
 * Side effects:
 *	Could cause host state change.
 *
 *----------------------------------------------------------------------
 */

unsigned int
VMMouseClient_GetInput (PVMMOUSE_INPUT_DATA pvmmouseInput) {

   uint32_t status;
   uint16_t numWords;
   uint32_t packetInfo;
   VMMouseProtoCmd vmpc;

   /*
    * The status dword has two parts: the high 16 bits are
    * for flags, the low 16-bits are the number of DWORDs
    * waiting in the data queue. VMMOUSE_ERROR is a special
    * case that indicates there's something wrong on the
    * host end, e.g. the VMMouse was disabled on the host-side.
    */
   vmpc.in.vEbx = 0;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_STATUS;
   VMMouseProto_SendCmd(&vmpc);
   status = vmpc.out.vEax;
   if ((status & VMMOUSE_ERROR) == VMMOUSE_ERROR) {
      VMwareLog(("VMMouseClient_GetInput: VMMOUSE_ERROR status, abort!\n"));
      return VMMOUSE_ERROR;
   }

   /*
    * We don't use the status flags, just get the words
    */
   numWords = status & 0x0000ffff;

   if ((numWords % 4) != 0) {
      VMwareLog(("VMMouseClient_GetInput: invalid status numWords, abort!\n"));
      return (0);
   }

   if (numWords == 0) {
      return (0);
   }

   /*
    * The VMMouse uses a 4-dword packet protocol:
    *	DWORD 0: Button State and per-packet flags
    *	DWORD 1: X position (absolute or relative)
    *	DWORD 2: Y position (absolute or relative)
    *	DWORD 3: Z position (relative)
    */
   /* Get 4 items at once */
   vmpc.in.vEbx = 4;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_DATA;
   VMMouseProto_SendCmd(&vmpc);
   packetInfo = vmpc.out.vEax;
   pvmmouseInput->Flags = (packetInfo & 0xffff0000) >> 16;
   pvmmouseInput->Buttons = (packetInfo & 0x0000ffff);

   /* Note that Z is always signed, and X/Y are signed in relative mode. */
   pvmmouseInput->X = (int)vmpc.out.vEbx;
   pvmmouseInput->Y = (int)vmpc.out.vEcx;
   pvmmouseInput->Z = (int)vmpc.out.vEdx;

   /*
    * Return number of packets (including this one) in queue.
    */
   return (numWords >> 2);
}


/*
 *----------------------------------------------------------------------------
 *
 * VMMouseClient_RequestRelative --
 *
 *      Request that the host switch to posting relative packets. It's just
 *      advisory, so we make no guarantees about if/when the switch will
 *      happen.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Host may start posting relative packets in the near future.
 *
 *----------------------------------------------------------------------------
 */

void
VMMouseClient_RequestRelative(void)
{
   VMMouseProtoCmd vmpc;

   VMwareLog(("VMMouseClient: requesting relative mode\n"));
   vmpc.in.vEbx = VMMOUSE_CMD_REQUEST_RELATIVE;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_COMMAND;
   VMMouseProto_SendCmd(&vmpc);
}


/*
 *----------------------------------------------------------------------------
 *
 * VMMouseClient_RequestAbsolute --
 *
 *      Request that the host switch to posting absolute packets. It's just
 *      advisory, so we make no guarantees about if/when the switch will
 *      happen.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Host may start posting absolute packets in the near future.
 *
 *----------------------------------------------------------------------------
 */

void
VMMouseClient_RequestAbsolute(void)
{
   VMMouseProtoCmd vmpc;

   VMwareLog(("VMMouseClient: requesting absolute mode\n"));
   vmpc.in.vEbx = VMMOUSE_CMD_REQUEST_ABSOLUTE;
   vmpc.in.command = VMMOUSE_PROTO_CMD_ABSPOINTER_COMMAND;
   VMMouseProto_SendCmd(&vmpc);
}
