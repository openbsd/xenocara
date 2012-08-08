/*************************************************************************************
 * 
 * Copyright (C) 2005 Bogdan D. bogdand@users.sourceforge.net
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the author shall not be used in advertising or 
 * otherwise to promote the sale, use or other dealings in this Software without prior written 
 * authorization from the author.
 *
 * $Log: theatre_detect.c,v $
 * Revision 1.7  2012/08/08 16:25:22  matthieu
 * Update to xf86-video-ati 6.14.6. Tested by mpi@, brad, eric@, ajacoutot@.
 *
 * Revision 1.6  2012/03/04 16:01:20  matthieu
 * Re-update xf86-video-ati to 6.14.3.
 *
 * Because of ati.c revision 1.5, it will not be selected by
 * autoconfiguration code, so people who want to run this version of the
 * driver on their radeon cards need an explicit "Device" section in
 * xorg.conf.
 *
 * Revision 1.4  2012/01/25 21:33:35  matthieu
 * Update to xf86-video-ati 6.14.3. Tested by many.
 *
 * Revision 1.3  2009/08/25 18:51:45  matthieu
 * update do xf86-video-ati 6.12.2
 *
 * This has been in snapshots for weeks. ok oga@, todd@.
 *
 * Revision 1.4  2005/08/28 18:00:23  bogdand
 * Modified the licens type from GPL to a X/MIT one
 *
 * Revision 1.3  2005/07/11 02:29:45  ajax
 * Prep for modular builds by adding guarded #include "config.h" everywhere.
 *
 * Revision 1.2  2005/07/01 22:43:11  daniels
 * Change all misc.h and os.h references to <X11/foo.h>.
 *
 *
 ************************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include "xf86.h"
#include "generic_bus.h"
#include "theatre.h"
#include "theatre_reg.h"
#include "theatre_detect.h"

static Bool theatre_read(TheatrePtr t,uint32_t reg, uint32_t *data)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->read(t->VIP, ((t->theatre_num & 0x3)<<14) | reg,4, (uint8_t *) data);
}

/* Unused code - reference */
#if 0
static Bool theatre_write(TheatrePtr t,uint32_t reg, uint32_t data)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->write(t->VIP,((t->theatre_num & 0x03)<<14) | reg,4, (uint8_t *) &data);
}
#define RT_regw(reg,data)	theatre_write(t,(reg),(data))
#endif

#define RT_regr(reg,data)	theatre_read(t,(reg),(data))
#define VIP_TYPE      "ATI VIP BUS"


_X_EXPORT TheatrePtr DetectTheatre(GENERIC_BUS_Ptr b)
{
   TheatrePtr t;  
   int i;
   uint32_t val;
   char s[20];
   
   b->ioctl(b,GB_IOCTL_GET_TYPE,20,s);
   if(strcmp(VIP_TYPE, s)){
   xf86DrvMsg(b->pScrn->scrnIndex, X_ERROR, "DetectTheatre must be called with bus of type \"%s\", not \"%s\"\n",
          VIP_TYPE, s);
   return NULL;
   }
   
   t = calloc(1,sizeof(TheatreRec));
   t->VIP = b;
   t->theatre_num = -1;
   t->mode=MODE_UNINITIALIZED;

   b->read(b, VIP_VIP_VENDOR_DEVICE_ID, 4, (uint8_t *)&val);
   for(i=0;i<4;i++)
   {
	if(b->read(b, ((i & 0x03)<<14) | VIP_VIP_VENDOR_DEVICE_ID, 4, (uint8_t *)&val))
        {
	  if(val)xf86DrvMsg(b->pScrn->scrnIndex, X_INFO,
			    "Device %d on VIP bus ids as 0x%08x\n", i,
			    (unsigned)val);
	  if(t->theatre_num>=0)continue; /* already found one instance */
	  switch(val){
	  	case RT100_ATI_ID:
	           t->theatre_num=i;
		   t->theatre_id=RT100_ATI_ID;
		   break;
		case RT200_ATI_ID:
	           t->theatre_num=i;
		   t->theatre_id=RT200_ATI_ID;
		   break;
                }
	} else {
	  xf86DrvMsg(b->pScrn->scrnIndex, X_INFO, "No response from device %d on VIP bus\n",i);	
	}
   }
   if(t->theatre_num>=0)xf86DrvMsg(b->pScrn->scrnIndex, X_INFO,
				   "Detected Rage Theatre as device %d on VIP bus with id 0x%08x\n",
				   t->theatre_num, (unsigned)t->theatre_id);

   if(t->theatre_num < 0)
   {
   free(t);
   return NULL;
   }

   RT_regr(VIP_VIP_REVISION_ID, &val);
   xf86DrvMsg(b->pScrn->scrnIndex, X_INFO, "Detected Rage Theatre revision %8.8X\n",
	      (unsigned)val);

#if 0
DumpRageTheatreRegsByName(t);
#endif
	
   return t;
}

