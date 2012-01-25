/*************************************************************************************
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
 * $Log: theatre200.h,v $
 * Revision 1.1  2012/01/25 21:27:07  matthieu
 * Add radeon driver 6.12.2 renamed as 'radeonold'.
 *
 * This is the last known working version in 'zaphod' mode (one X
 * screen per head in multi-head configs). This makes it possible
 * to update the "real" radeon driver in xf86-video-ati, without
 * breaking existing zaphod setups.
 *
 * Revision 1.3  2009/08/25 18:51:45  matthieu
 * update do xf86-video-ati 6.12.2
 *
 * This has been in snapshots for weeks. ok oga@, todd@.
 *
 * Revision 1.5  2005/12/08 17:54:40  kem
 * 	Allow hard-coded paths to be configurable.
 *
 * Revision 1.4  2005/11/07 19:28:40  bogdand
 * Replaced the variadic macros(gcc) by macros according to C99 standard
 *
 * Revision 1.3  2005/08/28 18:00:23  bogdand
 * Modified the licens type from GPL to a X/MIT one
 *
 * Revision 1.2  2005/07/01 22:43:11  daniels
 * Change all misc.h and os.h references to <X11/foo.h>.
 *
 *
 ************************************************************************************/

#ifndef __THEATRE200_H__
#define __THEATRE200_H__

#include "theatre.h"

#ifdef MICROC_DIR
#define DEFAULT_MICROC_PATH MICROC_DIR"/rt2_pmem.bin"
#else
#define DEFAULT_MICROC_PATH "/usr/X11R6/lib/modules/multimedia/rt2_pmem.bin"
#endif
#define DEFAULT_MICROC_TYPE "BINARY"

/* #define ENABLE_DEBUG 1 */

#ifdef ENABLE_DEBUG
#define ERROR_0(str) xf86DrvMsg(screen, X_ERROR, str)
#define DEBUG_0(str) xf86DrvMsg(screen, X_INFO, str) 
#define ERROR(str,param1) xf86DrvMsg(screen, X_ERROR, str, param1)
#define DEBUG(str,param1) xf86DrvMsg(screen, X_INFO, str, param1) 
#define ERROR_2(str,param1,param2) xf86DrvMsg(screen, X_ERROR, str, param1, param2)
#define DEBUG_2(str,param1,param2) xf86DrvMsg(screen, X_INFO, str, param1, param2) 
#define ERROR_3(str,param1,param2,param3) xf86DrvMsg(screen, X_ERROR, str, param1, param2, param3)
#define DEBUG_3(str,param1,param2,param3) xf86DrvMsg(screen, X_INFO, str, param1, param2, param3) 
#else
#define ERROR_0(str) (void)screen
#define DEBUG_0(str) (void)screen
#define ERROR(str,param1) (void)screen
#define DEBUG(str,param1) (void)screen
#define ERROR_2(str,param1,param2) (void)screen
#define DEBUG_2(str,param1,param2) (void)screen
#define ERROR_3(str,param1,param2,param3) (void)screen
#define DEBUG_3(str,param1,param2,param3) (void)screen
#endif


#define DSP_OK						0x21
#define DSP_INVALID_PARAMETER		0x22
#define DSP_MISSING_PARAMETER		0x23
#define DSP_UNKNOWN_COMMAND			0x24
#define DSP_UNSUCCESS				0x25
#define DSP_BUSY					0x26
#define DSP_RESET_REQUIRED			0x27
#define DSP_UNKNOWN_RESULT			0x28
#define DSP_CRC_ERROR				0x29
#define DSP_AUDIO_GAIN_ADJ_FAIL		0x2a
#define DSP_AUDIO_GAIN_CHK_ERROR	0x2b
#define DSP_WARNING					0x2c
#define DSP_POWERDOWN_MODE			0x2d

#define RT200_NTSC_M				0x01
#define RT200_NTSC_433				0x03
#define RT200_NTSC_J				0x04
#define RT200_PAL_B					0x05
#define RT200_PAL_D					0x06
#define RT200_PAL_G					0x07
#define RT200_PAL_H					0x08
#define RT200_PAL_I					0x09
#define RT200_PAL_N					0x0a
#define RT200_PAL_Ncomb				0x0b
#define RT200_PAL_M					0x0c
#define RT200_PAL_60				0x0d
#define RT200_SECAM					0x0e
#define RT200_SECAM_B				0x0f
#define RT200_SECAM_D				0x10
#define RT200_SECAM_G				0x11
#define RT200_SECAM_H				0x12
#define RT200_SECAM_K				0x13
#define RT200_SECAM_K1				0x14
#define RT200_SECAM_L				0x15
#define RT200_SECAM_L1				0x16
#define RT200_480i					0x17
#define RT200_480p					0x18
#define RT200_576i					0x19
#define RT200_720p					0x1a
#define RT200_1080i					0x1b

struct rt200_microc_head
{
	unsigned int device_id;
	unsigned int vendor_id;
	unsigned int revision_id;
	unsigned int num_seg;
};

struct rt200_microc_seg
{
	unsigned int num_bytes;
	unsigned int download_dst;
	unsigned int crc_val;

	unsigned char* data;
	struct rt200_microc_seg* next;
};


struct rt200_microc_data
{
	struct rt200_microc_head		microc_head;
	struct rt200_microc_seg*		microc_seg_list;
};

#endif
