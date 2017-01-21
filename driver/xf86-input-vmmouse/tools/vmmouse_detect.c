/*
 * Copyright 2007 by VMware, Inc.
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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <signal.h>
#include "vmmouse_client.h"

extern int vmmouse_uses_kernel_driver(void);

void
segvCB(int sig)
{
#if defined HAVE_XORG_SERVER_1_1_0
   exit(1);
#endif
}


int
main(void)
{
   if (vmmouse_uses_kernel_driver())
      return 1;

   /*
    * If the vmmouse test is not run in a VMware virtual machine, it
    * will segfault instead of successfully accessing the port.
    */
   signal(SIGSEGV, segvCB);

#if defined __i386__ || defined __x86_64__ 
   (void) xf86EnableIO();
   if (VMMouseClient_Enable()) {
      VMMouseClient_Disable();
      return 0;
   } else {
      /*
       * We get here if we are running in a VM and the vmmouse
       * device is disabled.
       */
      return 1;
   }
#endif
   return 1;
}
