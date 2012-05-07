/*
 * Copyright 2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Unknown at vmware
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xf86.h>
#include "vmware_common.h"

#ifndef HAVE_XORG_SERVER_1_5_0
#include <xf86_ansic.h>
#include <xf86_libc.h>
#endif

static int
VMWAREParseTopologyElement(ScrnInfoPtr pScrn,
                           unsigned int output,
                           const char *elementName,
                           const char *element,
                           const char *expectedTerminators,
                           Bool needTerminator,
                           unsigned int *outValue)
{
   char buf[10] = {0, };
   size_t i = 0;
   int retVal = -1;
   const char *str = element;

   for (i = 0; str[i] >= '0' && str[i] <= '9'; i++);
   if (i == 0) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Output %u: unable to parse %s.\n",
                 output, elementName);
      goto exit;
   }

   strncpy(buf, str, i);
   *outValue = atoi(buf);

   if (*outValue > (unsigned short)-1) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Output %u: %s must be less than %hu.\n",
                 output, elementName, (unsigned short)-1);
      goto exit;
   }

   str += i;

   if (needTerminator || str[0] != '\0') {
      Bool unexpected = TRUE;

      for (i = 0; i < strlen(expectedTerminators); i++) {
         if (str[0] == expectedTerminators[i]) {
            unexpected = FALSE;
         }
      }

      if (unexpected) {
         xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Output %u: unexpected character '%c' after %s.\n",
                    output, str[0], elementName);
         goto exit;
      } else {
         str++;
      }
   }

   retVal = str - element;

 exit:
   return retVal;
}

xXineramaScreenInfo *
VMWAREParseTopologyString(ScrnInfoPtr pScrn,
                          const char *topology,
                          unsigned int *retNumOutputs,
			  const char info[])
{
   xXineramaScreenInfo *extents = NULL;
   unsigned int numOutputs = 0;
   const char *str = topology;

   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Parsing %s topology: Starting...\n",
	      info);

   do {
      unsigned int x, y, width, height;
      int i;

      i = VMWAREParseTopologyElement(pScrn, numOutputs, "width", str, "xX", TRUE, &width);
      if (i == -1) {
         goto error;
      }
      str += i;

      i = VMWAREParseTopologyElement(pScrn, numOutputs, "height", str, "+", TRUE, &height);
      if (i == -1) {
         goto error;
      }
      str += i;

      i= VMWAREParseTopologyElement(pScrn, numOutputs, "X offset", str, "+", TRUE, &x);
      if (i == -1) {
         goto error;
      }
      str += i;

      i = VMWAREParseTopologyElement(pScrn, numOutputs, "Y offset", str, ";", FALSE, &y);
      if (i == -1) {
         goto error;
      }
      str += i;

      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Output %u: %ux%u+%u+%u\n",
                 numOutputs, width, height, x, y);

      numOutputs++;
      extents = realloc(extents, numOutputs * sizeof (xXineramaScreenInfo));
      extents[numOutputs - 1].x_org = x;
      extents[numOutputs - 1].y_org = y;
      extents[numOutputs - 1].width = width;
      extents[numOutputs - 1].height = height;
   } while (*str != 0);

   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Parsing %s topology: Succeeded.\n",
	      info);
   goto exit;

 error:
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Parsing %s topology: Failed.\n",
	      info);

   free(extents);
   extents = NULL;
   numOutputs = 0;

 exit:
   *retNumOutputs = numOutputs;
   return extents;
}
