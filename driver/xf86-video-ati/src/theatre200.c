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
 * $Log: theatre200.c,v $
 * Revision 1.4  2012/01/25 21:33:35  matthieu
 * Update to xf86-video-ati 6.14.3. Tested by many.
 *
 * Revision 1.3  2009/08/25 18:51:45  matthieu
 * update do xf86-video-ati 6.12.2
 *
 * This has been in snapshots for weeks. ok oga@, todd@.
 *
 * Revision 1.6  2006/03/22 22:30:14  krh
 * 2006-03-22  Kristian Høgsberg  <krh@redhat.com>
 *
 * 	* src/theatre200.c: Convert use of xf86fopen() and other xf86
 * 	wrapped libc symbols to use libc symbols directly.  The xf86*
 * 	versions aren't supposed to be used directly.
 *
 * 	* src/ *.c: Drop libc wrapper; don't include xf86_ansic.h and add
 * 	includes now missing.
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

#include <stdio.h>
#include <string.h>

#include "xf86.h"
#include "generic_bus.h"
#include "radeon_reg.h"
#include "radeon.h"
#include "theatre_reg.h"
#include "theatre200.h"
#include "radeon_macros.h"

#undef read
#undef write
#undef ioctl

void DumpRageTheatreRegsByName(TheatrePtr t);

static int DownloadMicrocode(TheatrePtr t);
static int microc_load (char* micro_path, char* micro_type, struct rt200_microc_data* microc_datap, int screen);
static void microc_clean(struct rt200_microc_data* microc_datap, int screen);
static int dsp_init(TheatrePtr t, struct rt200_microc_data* microc_datap);
static int dsp_load(TheatrePtr t, struct rt200_microc_data* microc_datap);

static uint32_t dsp_send_command(TheatrePtr t, uint32_t fb_scratch1, uint32_t fb_scratch0);
static uint32_t dsp_set_video_input_connector(TheatrePtr t, uint32_t connector);
//static uint32_t dsp_reset(TheatrePtr t);
static uint32_t dsp_set_lowpowerstate(TheatrePtr t, uint32_t pstate);
static uint32_t dsp_set_video_standard(TheatrePtr t, uint32_t standard);
static uint32_t dsp_set_videostreamformat(TheatrePtr t, uint32_t format);
static uint32_t dsp_video_standard_detection(TheatrePtr t);
//static uint32_t dsp_get_signallockstatus(TheatrePtr t);
//static uint32_t dsp_get_signallinenumber(TheatrePtr t);

static uint32_t dsp_set_brightness(TheatrePtr t, uint8_t brightness);
static uint32_t dsp_set_contrast(TheatrePtr t, uint8_t contrast);
//static uint32_t dsp_set_sharpness(TheatrePtr t, int sharpness);
static uint32_t dsp_set_tint(TheatrePtr t, uint8_t tint);
static uint32_t dsp_set_saturation(TheatrePtr t, uint8_t saturation);
static uint32_t dsp_set_video_scaler_horizontal(TheatrePtr t, uint16_t output_width, uint16_t horz_start, uint16_t horz_end);
static uint32_t dsp_set_video_scaler_vertical(TheatrePtr t, uint16_t output_height, uint16_t vert_start, uint16_t vert_end);
static uint32_t dsp_audio_mute(TheatrePtr t, uint8_t left, uint8_t right);
static uint32_t dsp_set_audio_volume(TheatrePtr t, uint8_t left, uint8_t right, uint8_t auto_mute);
//static uint32_t dsp_audio_detection(TheatrePtr t, uint8_t option);
static uint32_t dsp_configure_i2s_port(TheatrePtr t, uint8_t tx_mode, uint8_t rx_mode, uint8_t clk_mode);
static uint32_t dsp_configure_spdif_port(TheatrePtr t, uint8_t state);

static Bool theatre_read(TheatrePtr t,uint32_t reg, uint32_t *data)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->read(t->VIP, ((t->theatre_num & 0x3)<<14) | reg,4, (uint8_t *) data);
}

static Bool theatre_write(TheatrePtr t,uint32_t reg, uint32_t data)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->write(t->VIP,((t->theatre_num & 0x03)<<14) | reg,4, (uint8_t *) &data);
}

static Bool theatre_fifo_read(TheatrePtr t,uint32_t fifo, uint8_t *data)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->fifo_read(t->VIP, ((t->theatre_num & 0x3)<<14) | fifo,1, (uint8_t *) data);
}

static Bool theatre_fifo_write(TheatrePtr t,uint32_t fifo, uint32_t count, uint8_t* buffer)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->fifo_write(t->VIP,((t->theatre_num & 0x03)<<14) | fifo,count, (uint8_t *)buffer);
}

#define RT_regr(reg,data)				theatre_read(t,(reg),(data))
#define RT_regw(reg,data)				theatre_write(t,(reg),(data))
#define RT_fifor(fifo,data)			theatre_fifo_read(t,(fifo),(data))
#define RT_fifow(fifo,count,data)	theatre_fifo_write(t,(fifo),(count),(data))
#define VIP_TYPE      "ATI VIP BUS"

static int microc_load (char* micro_path, char* micro_type, struct rt200_microc_data* microc_datap, int screen)
{
	FILE* file;
	struct rt200_microc_head* microc_headp = &microc_datap->microc_head;
	struct rt200_microc_seg* seg_list = NULL;
	struct rt200_microc_seg* curr_seg = NULL;
	struct rt200_microc_seg* prev_seg = NULL;
	int i;

	if (micro_path == NULL)
		return -1;

	if (micro_type == NULL)
		return -1;

	file = fopen(micro_path, "r");
	if (file == NULL) {
		ERROR_0("Cannot open microcode file\n");
					 return -1;
	}

	if (!strcmp(micro_type, "BINARY"))
	{
		if (fread(microc_headp, sizeof(struct rt200_microc_head), 1, file) != 1)
		{
			ERROR("Cannot read header from file: %s\n", micro_path);
			goto fail_exit;
		}

		DEBUG("Microcode: num_seg: %x\n", microc_headp->num_seg);

		if (microc_headp->num_seg == 0)
			goto fail_exit;
		
		for (i = 0; i < microc_headp->num_seg; i++)
		{
			int ret;
			
			curr_seg = (struct rt200_microc_seg*)malloc(sizeof(struct rt200_microc_seg));
			if (curr_seg == NULL)
			{
				ERROR_0("Cannot allocate memory\n");
				goto fail_exit;
			}

			ret = fread(&curr_seg->num_bytes, 4, 1, file);
			ret += fread(&curr_seg->download_dst, 4, 1, file);
			ret += fread(&curr_seg->crc_val, 4, 1, file);
			if (ret != 3)
			{
				ERROR("Cannot read segment from microcode file: %s\n", micro_path);
				goto fail_exit;
			}

			curr_seg->data = (unsigned char*)malloc(curr_seg->num_bytes);
			if (curr_seg->data == NULL)
			{
				ERROR_0("cannot allocate memory\n");
				goto fail_exit;
			}

			DEBUG("Microcode: segment number: %x\n", i);
			DEBUG("Microcode: curr_seg->num_bytes: %x\n", curr_seg->num_bytes);
			DEBUG("Microcode: curr_seg->download_dst: %x\n", curr_seg->download_dst);
			DEBUG("Microcode: curr_seg->crc_val: %x\n", curr_seg->crc_val);

			if (seg_list)
			{
				prev_seg->next = curr_seg;
				curr_seg->next = NULL;
				prev_seg = curr_seg;
			}
			else
				seg_list = prev_seg = curr_seg;

		}
	
		curr_seg = seg_list;
		while (curr_seg)
		{
			if (fread(curr_seg->data, curr_seg->num_bytes, 1, file) != 1)
			{
				ERROR_0("Cannot read segment data\n");
				goto fail_exit;
			}

			curr_seg = curr_seg->next;
		}
	}
	else if (!strcmp(micro_type, "ASCII"))
	{
		char tmp1[12], tmp2[12], tmp3[12], tmp4[12];
		unsigned int ltmp;

		if ((fgets(tmp1, 12, file) != NULL) &&
			(fgets(tmp2, 12, file) != NULL) &&
			(fgets(tmp3, 12, file) != NULL) &&
			fgets(tmp4, 12, file) != NULL)
		{
			microc_headp->device_id = strtoul(tmp1, NULL, 16);
			microc_headp->vendor_id = strtoul(tmp2, NULL, 16);
			microc_headp->revision_id = strtoul(tmp3, NULL, 16);
			microc_headp->num_seg = strtoul(tmp4, NULL, 16);
		}
		else
		{
			ERROR("Cannot read header from file: %s\n", micro_path);
			goto fail_exit;
		}

		DEBUG("Microcode: num_seg: %x\n", microc_headp->num_seg);

		if (microc_headp->num_seg == 0)
			goto fail_exit;

		for (i = 0; i < microc_headp->num_seg; i++)
		{
			curr_seg = (struct rt200_microc_seg*)malloc(sizeof(struct rt200_microc_seg));
			if (curr_seg == NULL)
			{
				ERROR_0("Cannot allocate memory\n");
				goto fail_exit;
			}

			if (fgets(tmp1, 12, file) != NULL &&
				fgets(tmp2, 12, file) != NULL &&
				fgets(tmp3, 12, file) != NULL)
			{
				curr_seg->num_bytes = strtoul(tmp1, NULL, 16); 
				curr_seg->download_dst = strtoul(tmp2, NULL, 16); 
				curr_seg->crc_val = strtoul(tmp3, NULL, 16); 
			}
			else
			{
				ERROR("Cannot read segment from microcode file: %s\n", micro_path);
				goto fail_exit;
			}
								
			curr_seg->data = (unsigned char*)malloc(curr_seg->num_bytes);
			if (curr_seg->data == NULL)
			{
				ERROR_0("cannot allocate memory\n");
				goto fail_exit;
			}

			DEBUG("Microcode: segment number: %x\n", i);
			DEBUG("Microcode: curr_seg->num_bytes: %x\n", curr_seg->num_bytes);
			DEBUG("Microcode: curr_seg->download_dst: %x\n", curr_seg->download_dst);
			DEBUG("Microcode: curr_seg->crc_val: %x\n", curr_seg->crc_val);

			if (seg_list)
			{
				curr_seg->next = NULL;
				prev_seg->next = curr_seg;
				prev_seg = curr_seg;
			}
			else
				seg_list = prev_seg = curr_seg;
		}

		curr_seg = seg_list;
		while (curr_seg)
		{
			for ( i = 0; i < curr_seg->num_bytes; i+=4)
			{

				if (fgets(tmp1, 12, file) == NULL)
				{
					ERROR_0("Cannot read from file\n");
					goto fail_exit;
				}
				ltmp = strtoul(tmp1, NULL, 16);

				*(unsigned int*)(curr_seg->data + i) = ltmp;
			}
								  
			curr_seg = curr_seg->next;
		}

	}
	else
	{
		ERROR("File type %s unknown\n", micro_type);
	}

	microc_datap->microc_seg_list = seg_list;

	fclose(file);
	return 0;

fail_exit:
	curr_seg = seg_list;
	while(curr_seg)
	{
		free(curr_seg->data);
		prev_seg = curr_seg;
		curr_seg = curr_seg->next;
		free(prev_seg);
	}
	fclose(file);

	return -1;
}

static void microc_clean(struct rt200_microc_data* microc_datap, int screen)
{
	struct rt200_microc_seg* seg_list = microc_datap->microc_seg_list;
	struct rt200_microc_seg* prev_seg;

	while(seg_list)
	{
		free(seg_list->data);
		prev_seg = seg_list;
		seg_list = seg_list->next;
		free(prev_seg);
	}
}

static int dsp_init(TheatrePtr t, struct rt200_microc_data* microc_datap)
{
	uint32_t data;
	int i = 0;
	int screen = t->VIP->scrnIndex;

	/* Map FIFOD to DSP Port I/O port */
	RT_regr(VIP_HOSTINTF_PORT_CNTL, &data);
	RT_regw(VIP_HOSTINTF_PORT_CNTL, data & (~VIP_HOSTINTF_PORT_CNTL__FIFO_RW_MODE));

	/* The default endianess is LE. It matches the ost one for x86 */
	RT_regr(VIP_HOSTINTF_PORT_CNTL, &data);
	RT_regw(VIP_HOSTINTF_PORT_CNTL, data & (~VIP_HOSTINTF_PORT_CNTL__FIFOD_ENDIAN_SWAP));

	/* Wait until Shuttle bus channel 14 is available */
	RT_regr(VIP_TC_STATUS, &data);
	while(((data & VIP_TC_STATUS__TC_CHAN_BUSY) & 0x00004000) && (i++ < 10000))
		RT_regr(VIP_TC_STATUS, &data);
		  
	DEBUG_0("Microcode: dsp_init: channel 14 available\n");
		  
	return 0;
}

static int dsp_load(TheatrePtr t, struct rt200_microc_data* microc_datap)
{
	struct rt200_microc_seg* seg_list = microc_datap->microc_seg_list;
	uint8_t	data8;
	uint32_t data, fb_scratch0, fb_scratch1;
	uint32_t i;
	uint32_t tries = 0;
	uint32_t result = 0;
	uint32_t seg_id = 0;
	int screen = t->VIP->scrnIndex;
		  
	DEBUG("Microcode: before everything: %x\n", data8);

	if (RT_fifor(0x000, &data8))
		DEBUG("Microcode: FIFO status0: %x\n", data8);
	else
	{
		ERROR_0("Microcode: error reading FIFO status0\n");
		return -1;
	}


	if (RT_fifor(0x100, &data8))
		DEBUG("Microcode: FIFO status1: %x\n", data8);
	else
	{
		ERROR_0("Microcode: error reading FIFO status1\n");
		return -1;
	}

	/*
	 * Download the Boot Code and CRC Checking Code (first segment)
	 */
	seg_id = 1;
	while(result != DSP_OK && tries < 10)
	{
		/* Put DSP in reset before download (0x02) */
		RT_regr(VIP_TC_DOWNLOAD, &data);
		RT_regw(VIP_TC_DOWNLOAD, (data & ~VIP_TC_DOWNLOAD__TC_RESET_MODE) | (0x02 << 17));
					 
		/* 
		 * Configure shuttle bus for tranfer between DSP I/O "Program Interface"
		 * and Program Memory at address 0 
		 */

		RT_regw(VIP_TC_SOURCE, 0x90000000);
		RT_regw(VIP_TC_DESTINATION, 0x00000000);
		RT_regw(VIP_TC_COMMAND, 0xe0000044 | ((seg_list->num_bytes - 1) << 7));

		/* Load first segment */
		DEBUG_0("Microcode: Loading first segment\n");

		if (!RT_fifow(0x700, seg_list->num_bytes, seg_list->data))
		{
			ERROR_0("Microcode: write to FIFOD failed\n");
			return -1;
		}

		/* Wait until Shuttle bus channel 14 is available */
		i = data = 0;
		RT_regr(VIP_TC_STATUS, &data);
		while(((data & VIP_TC_STATUS__TC_CHAN_BUSY) & 0x00004000) && (i++ < 10000))
			RT_regr(VIP_TC_STATUS, &data);

		if (i >= 10000)
		{
			ERROR_0("Microcode: channel 14 timeout\n");
			return -1;
		}

		DEBUG_0("Microcode: dsp_load: checkpoint 1\n");
		DEBUG("Microcode: TC_STATUS: %x\n", data);

		/* transfer the code from program memory to data memory */
		RT_regw(VIP_TC_SOURCE, 0x00000000);
		RT_regw(VIP_TC_DESTINATION, 0x10000000);
		RT_regw(VIP_TC_COMMAND, 0xe0000006 | ((seg_list->num_bytes - 1) << 7));

		/* Wait until Shuttle bus channel 14 is available */
		i = data = 0;
		RT_regr(VIP_TC_STATUS, &data);
		while(((data & VIP_TC_STATUS__TC_CHAN_BUSY) & 0x00004000) && (i++ < 10000))
			RT_regr(VIP_TC_STATUS, &data);
					 
		if (i >= 10000)
		{
			ERROR_0("Microcode: channel 14 timeout\n");
			return -1;
		}
		DEBUG_0("Microcode: dsp_load: checkpoint 2\n");
		DEBUG("Microcode: TC_STATUS: %x\n", data);

		/* Take DSP out from reset (0x0) */
		data = 0;
		RT_regr(VIP_TC_DOWNLOAD, &data);
		RT_regw(VIP_TC_DOWNLOAD, data & ~VIP_TC_DOWNLOAD__TC_RESET_MODE);

		RT_regr(VIP_TC_STATUS, &data);
		DEBUG_0("Microcode: dsp_load: checkpoint 3\n");
		DEBUG("Microcode: TC_STATUS: %x\n", data);

		/* send dsp_download_check_CRC */
		fb_scratch0 = ((seg_list->num_bytes << 16) & 0xffff0000) | ((seg_id << 8) & 0xff00) | (0xff & 193);
		fb_scratch1 = (unsigned int)seg_list->crc_val;
					 
		result = dsp_send_command(t, fb_scratch1, fb_scratch0);

		DEBUG_0("Microcode: dsp_load: checkpoint 4\n");
	}

	if (tries >= 10)
	{
		ERROR_0("Microcode: Download of boot degment failed\n");
		return -1;
	}

	DEBUG_0("Microcode: Download of boot code succeeded\n");

	while((seg_list = seg_list->next) != NULL)
	{
		seg_id++;
		result = tries = 0;
		while(result != DSP_OK && tries < 10)
		{
			/* 
			 * Configure shuttle bus for tranfer between DSP I/O "Program Interface"
			 * and Data Memory at address 0 
			 */

			RT_regw(VIP_TC_SOURCE, 0x90000000);
			RT_regw(VIP_TC_DESTINATION, 0x10000000);
			RT_regw(VIP_TC_COMMAND, 0xe0000044 | ((seg_list->num_bytes - 1) << 7));

			if (!RT_fifow(0x700, seg_list->num_bytes, seg_list->data))
			{
				ERROR_0("Microcode: write to FIFOD failed\n");
				return -1;
			}
										
			i = data = 0;
			RT_regr(VIP_TC_STATUS, &data);
			while(((data & VIP_TC_STATUS__TC_CHAN_BUSY) & 0x00004000) && (i++ < 10000))
				RT_regr(VIP_TC_STATUS, &data);
										
			/* send dsp_download_check_CRC */
			fb_scratch0 = ((seg_list->num_bytes << 16) & 0xffff0000) | ((seg_id << 8) & 0xff00) | (0xff & 193);
			fb_scratch1 = (unsigned int)seg_list->crc_val;
										
			result = dsp_send_command(t, fb_scratch1, fb_scratch0);
		}

		if (i >=10)
		{
			ERROR("Microcode: DSP failed to move seg: %x from data to code memory\n", seg_id);
			return -1;
		}

		DEBUG("Microcode: segment: %x loaded\n", seg_id);

		/*
		 * The segment is downloaded correctly to data memory. Now move it to code memory
		 * by using dsp_download_code_transfer command.
		 */

		fb_scratch0 = ((seg_list->num_bytes << 16) & 0xffff0000) | ((seg_id << 8) & 0xff00) | (0xff & 194);
		fb_scratch1 = (unsigned int)seg_list->download_dst;
								
		result = dsp_send_command(t, fb_scratch1, fb_scratch0);

		if (result != DSP_OK)
		{
			ERROR("Microcode: DSP failed to move seg: %x from data to code memory\n", seg_id);
			return -1;
		}
	}

	DEBUG_0("Microcode: download complete\n");

	/*
	 * The last step is sending dsp_download_check_CRC with "download complete"
	 */

	fb_scratch0 = ((165 << 8) & 0xff00) | (0xff & 193);
	fb_scratch1 = (unsigned int)0x11111;
								
	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	if (result == DSP_OK)
		DEBUG_0("Microcode: DSP microcode successfully loaded\n");
	else
	{
		ERROR_0("Microcode: DSP microcode UNsuccessfully loaded\n");
		return -1;
	}

	return 0;
}

static uint32_t dsp_send_command(TheatrePtr t, uint32_t fb_scratch1, uint32_t fb_scratch0)
{
	uint32_t data;
	int i;

	/*
	 * Clear the FB_INT0 bit in INT_CNTL
	 */
	RT_regr(VIP_INT_CNTL, &data);
	RT_regw(VIP_INT_CNTL, data | VIP_INT_CNTL__FB_INT0_CLR);

	/*
	 * Write FB_SCRATCHx registers. If FB_SCRATCH1==0 then we have a DWORD command.
	 */
	RT_regw(VIP_FB_SCRATCH0, fb_scratch0);
	if (fb_scratch1 != 0)
		RT_regw(VIP_FB_SCRATCH1, fb_scratch1);	

	/*
	 * Attention DSP. We are talking to you.
	 */
	RT_regr(VIP_FB_INT, &data);
	RT_regw(VIP_FB_INT, data | VIP_FB_INT__INT_7);

	/*
	 * Wait (by polling) for the DSP to process the command.
	 */
	i = 0;
	RT_regr(VIP_INT_CNTL, &data);
	while((!(data & VIP_INT_CNTL__FB_INT0)) /*&& (i++ < 10000)*/)
		RT_regr(VIP_INT_CNTL, &data);
	
	/*
	 * The return code is in FB_SCRATCH0
	 */
	RT_regr(VIP_FB_SCRATCH0, &fb_scratch0);

	/*
	 * If we are here it means we got an answer. Clear the FB_INT0 bit.
	 */
	RT_regr(VIP_INT_CNTL, &data);
	RT_regw(VIP_INT_CNTL, data | VIP_INT_CNTL__FB_INT0_CLR);

	
	return fb_scratch0;
}

static uint32_t dsp_set_video_input_connector(TheatrePtr t, uint32_t connector)
{
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((connector << 8) & 0xff00) | (55 & 0xff);

	result = dsp_send_command(t, 0, fb_scratch0);

	DEBUG_2("dsp_set_video_input_connector: %x, result: %x\n", connector, result);
		  
	 return result;
}

#if 0
static uint32_t dsp_reset(TheatrePtr t)
{
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((2 << 8) & 0xff00) | (8 & 0xff);

	result = dsp_send_command(t, 0, fb_scratch0);

	DEBUG("dsp_reset: %x\n", result);
		  
	return result;
}
#endif

static uint32_t dsp_set_lowpowerstate(TheatrePtr t, uint32_t pstate)
{
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((pstate << 8) & 0xff00) | (82 & 0xff);

	result = dsp_send_command(t, 0, fb_scratch0);

	DEBUG("dsp_set_lowpowerstate: %x\n", result);
		  
	return result;
}
static uint32_t dsp_set_video_standard(TheatrePtr t, uint32_t standard)
{
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((standard << 8) & 0xff00) | (52 & 0xff);

	result = dsp_send_command(t, 0, fb_scratch0);

	DEBUG("dsp_set_video_standard: %x\n", result);
		  
	return result;
}

static uint32_t dsp_set_videostreamformat(TheatrePtr t, uint32_t format)
{
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((format << 8) & 0xff00) | (65 & 0xff);

	result = dsp_send_command(t, 0, fb_scratch0);

	DEBUG("dsp_set_videostreamformat: %x\n", result);
		  
	return result;
}

static uint32_t dsp_video_standard_detection(TheatrePtr t)
{
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = 0 | (54 & 0xff);

	result = dsp_send_command(t, 0, fb_scratch0);

	DEBUG("dsp_video_standard_detection: %x\n", result);

	return result;
}

#if 0
static uint32_t dsp_get_signallockstatus(TheatrePtr t)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = 0 | (77 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG_3("dsp_get_signallockstatus: %x, h_pll: %x, v_pll: %x\n", \
		result, (result >> 8) & 0xff, (result >> 16) & 0xff);
		  
	return result;
}

static uint32_t dsp_get_signallinenumber(TheatrePtr t)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = 0 | (78 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG_2("dsp_get_signallinenumber: %x, linenum: %x\n", \
		result, (result >> 8) & 0xffff);
		  
	return result;
}
#endif

static uint32_t dsp_set_brightness(TheatrePtr t, uint8_t brightness)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((brightness << 8) & 0xff00) | (67 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);
	
	DEBUG("dsp_set_brightness: %x\n", result);
		  
	return result;
}

static uint32_t dsp_set_contrast(TheatrePtr t, uint8_t contrast)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((contrast << 8) & 0xff00) | (71 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_set_contrast: %x\n", result);
  
	return result;
}

#if 0
static uint32_t dsp_set_sharpness(TheatrePtr t, int sharpness)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = 0 | (73 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_set_sharpness: %x\n", result);
		  
	return result;
}
#endif

static uint32_t dsp_set_tint(TheatrePtr t, uint8_t tint)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((tint << 8) & 0xff00) | (75 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_set_tint: %x\n", result);
		  
	return result;
}

static uint32_t dsp_set_saturation(TheatrePtr t, uint8_t saturation)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((saturation << 8) & 0xff00) | (69 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_set_saturation: %x\n", result);
		  
	return result;
}

static uint32_t dsp_set_video_scaler_horizontal(TheatrePtr t, uint16_t output_width, uint16_t horz_start, uint16_t horz_end)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((output_width << 8) & 0x00ffff00) | (195 & 0xff);
	fb_scratch1 = ((horz_end << 16) & 0xffff0000) | (horz_start & 0xffff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_set_video_scaler_horizontal: %x\n", result);
		  
	return result;
}

static uint32_t dsp_set_video_scaler_vertical(TheatrePtr t, uint16_t output_height, uint16_t vert_start, uint16_t vert_end)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((output_height << 8) & 0x00ffff00) | (196 & 0xff);
	fb_scratch1 = ((vert_end << 16) & 0xffff0000) | (vert_start & 0xffff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_set_video_scaler_vertical: %x\n", result);
		  
	return result;
}

static uint32_t dsp_audio_mute(TheatrePtr t, uint8_t left, uint8_t right)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((right << 16) & 0xff0000) | ((left << 8) & 0xff00) | (21 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_audio_mute: %x\n", result);
		  
	return result;
}

static uint32_t dsp_set_audio_volume(TheatrePtr t, uint8_t left, uint8_t right, uint8_t auto_mute)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;
  
	fb_scratch0 = ((auto_mute << 24) & 0xff000000) | ((right << 16) & 0xff0000) | ((left << 8) & 0xff00) | (22 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_set_audio_volume: %x\n", result);
		  
	return result;
}

#if 0
static uint32_t dsp_audio_detection(TheatrePtr t, uint8_t option)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((option << 8) & 0xff00) | (16 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_audio_detection: %x\n", result);
		  
	return result;
}
#endif

static uint32_t dsp_configure_i2s_port(TheatrePtr t, uint8_t tx_mode, uint8_t rx_mode, uint8_t clk_mode)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((clk_mode << 24) & 0xff000000) | ((rx_mode << 16) & 0xff0000) | ((tx_mode << 8) & 0xff00) | (40 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_configure_i2s_port: %x\n", result);
		  
	return result;
}

static uint32_t dsp_configure_spdif_port(TheatrePtr t, uint8_t state)
{
	uint32_t fb_scratch1 = 0;
	uint32_t fb_scratch0 = 0;
	uint32_t result;
	int screen = t->VIP->scrnIndex;

	fb_scratch0 = ((state << 8) & 0xff00) | (41 & 0xff);

	result = dsp_send_command(t, fb_scratch1, fb_scratch0);

	DEBUG("dsp_configure_spdif_port: %x\n", result);
		  
	return result;
}

enum
{
fld_tmpReg1=0,
fld_tmpReg2,
fld_tmpReg3,
fld_LP_CONTRAST,
fld_LP_BRIGHTNESS,
fld_CP_HUE_CNTL,
fld_LUMA_FILTER,
fld_H_SCALE_RATIO,
fld_H_SHARPNESS,
fld_V_SCALE_RATIO,
fld_V_DEINTERLACE_ON,
fld_V_BYPSS,
fld_V_DITHER_ON,
fld_EVENF_OFFSET,
fld_ODDF_OFFSET,
fld_INTERLACE_DETECTED,
fld_VS_LINE_COUNT,
fld_VS_DETECTED_LINES,
fld_VS_ITU656_VB,
fld_VBI_CC_DATA,
fld_VBI_CC_WT,
fld_VBI_CC_WT_ACK,
fld_VBI_CC_HOLD,
fld_VBI_DECODE_EN,
fld_VBI_CC_DTO_P,
fld_VBI_20BIT_DTO_P,
fld_VBI_CC_LEVEL,
fld_VBI_20BIT_LEVEL,
fld_VBI_CLK_RUNIN_GAIN,
fld_H_VBI_WIND_START,
fld_H_VBI_WIND_END,
fld_V_VBI_WIND_START,
fld_V_VBI_WIND_END,
fld_VBI_20BIT_DATA0,
fld_VBI_20BIT_DATA1,
fld_VBI_20BIT_WT,
fld_VBI_20BIT_WT_ACK,
fld_VBI_20BIT_HOLD,
fld_VBI_CAPTURE_ENABLE,
fld_VBI_EDS_DATA,
fld_VBI_EDS_WT,
fld_VBI_EDS_WT_ACK,
fld_VBI_EDS_HOLD,
fld_VBI_SCALING_RATIO,
fld_VBI_ALIGNER_ENABLE,
fld_H_ACTIVE_START,
fld_H_ACTIVE_END,
fld_V_ACTIVE_START,
fld_V_ACTIVE_END,
fld_CH_HEIGHT,
fld_CH_KILL_LEVEL,
fld_CH_AGC_ERROR_LIM,
fld_CH_AGC_FILTER_EN,
fld_CH_AGC_LOOP_SPEED,
fld_HUE_ADJ,
fld_STANDARD_SEL,
fld_STANDARD_YC,
fld_ADC_PDWN,
fld_INPUT_SELECT,
fld_ADC_PREFLO,
fld_H_SYNC_PULSE_WIDTH,
fld_HS_GENLOCKED,
fld_HS_SYNC_IN_WIN,
fld_VIN_ASYNC_RST,
fld_DVS_ASYNC_RST,
fld_VIP_VENDOR_ID,
fld_VIP_DEVICE_ID,
fld_VIP_REVISION_ID,
fld_BLACK_INT_START,
fld_BLACK_INT_LENGTH,
fld_UV_INT_START,
fld_U_INT_LENGTH,
fld_V_INT_LENGTH,
fld_CRDR_ACTIVE_GAIN,
fld_CBDB_ACTIVE_GAIN,
fld_DVS_DIRECTION,
fld_DVS_VBI_UINT8_SWAP,
fld_DVS_CLK_SELECT,
fld_CONTINUOUS_STREAM,
fld_DVSOUT_CLK_DRV,
fld_DVSOUT_DATA_DRV,
fld_COMB_CNTL0,
fld_COMB_CNTL1,
fld_COMB_CNTL2,
fld_COMB_LENGTH,
fld_SYNCTIP_REF0,
fld_SYNCTIP_REF1,
fld_CLAMP_REF,
fld_AGC_PEAKWHITE,
fld_VBI_PEAKWHITE,
fld_WPA_THRESHOLD,
fld_WPA_TRIGGER_LO,
fld_WPA_TRIGGER_HIGH,
fld_LOCKOUT_START,
fld_LOCKOUT_END,
fld_CH_DTO_INC,
fld_PLL_SGAIN,
fld_PLL_FGAIN,
fld_CR_BURST_GAIN,
fld_CB_BURST_GAIN,
fld_VERT_LOCKOUT_START,
fld_VERT_LOCKOUT_END,
fld_H_IN_WIND_START,
fld_V_IN_WIND_START,
fld_H_OUT_WIND_WIDTH,
fld_V_OUT_WIND_WIDTH,
fld_HS_LINE_TOTAL,
fld_MIN_PULSE_WIDTH,
fld_MAX_PULSE_WIDTH,
fld_WIN_CLOSE_LIMIT,
fld_WIN_OPEN_LIMIT,
fld_VSYNC_INT_TRIGGER,
fld_VSYNC_INT_HOLD,
fld_VIN_M0,
fld_VIN_N0,
fld_MNFLIP_EN,
fld_VIN_P,
fld_REG_CLK_SEL,
fld_VIN_M1,
fld_VIN_N1,
fld_VIN_DRIVER_SEL,
fld_VIN_MNFLIP_REQ,
fld_VIN_MNFLIP_DONE,
fld_TV_LOCK_TO_VIN,
fld_TV_P_FOR_WINCLK,
fld_VINRST,
fld_VIN_CLK_SEL,
fld_VS_FIELD_BLANK_START,
fld_VS_FIELD_BLANK_END,
fld_VS_FIELD_IDLOCATION,
fld_VS_FRAME_TOTAL,
fld_SYNC_TIP_START,
fld_SYNC_TIP_LENGTH,
fld_GAIN_FORCE_DATA,
fld_GAIN_FORCE_EN,
fld_I_CLAMP_SEL,
fld_I_AGC_SEL,
fld_EXT_CLAMP_CAP,
fld_EXT_AGC_CAP,
fld_DECI_DITHER_EN,
fld_ADC_PREFHI,
fld_ADC_CH_GAIN_SEL,
fld_HS_PLL_SGAIN,
fld_NREn,
fld_NRGainCntl,
fld_NRBWTresh,
fld_NRGCTresh,
fld_NRCoefDespeclMode,
fld_GPIO_5_OE,
fld_GPIO_6_OE,
fld_GPIO_5_OUT,
fld_GPIO_6_OUT,

regRT_MAX_REGS
} a;


typedef struct {
	uint8_t size;
	uint32_t fld_id;
	uint32_t dwRegAddrLSBs;
	uint32_t dwFldOffsetLSBs;
	uint32_t dwMaskLSBs;
	uint32_t addr2;
	uint32_t offs2;
	uint32_t mask2;
	uint32_t dwCurrValue;
	uint32_t rw;
	} RTREGMAP;

#define READONLY 1
#define WRITEONLY 2
#define READWRITE 3

/* Rage Theatre's Register Mappings, including the default values: */
RTREGMAP RT_RegMap[regRT_MAX_REGS]={
/*
{size, fidname, AddrOfst, Ofst, Mask, Addr, Ofst, Mask, Cur, R/W
*/
{32 , fld_tmpReg1       ,0x151                          , 0, 0x0, 0, 0,0, 0,READWRITE },
{1  , fld_tmpReg2       ,VIP_VIP_SUB_VENDOR_DEVICE_ID   , 3, 0xFFFFFFFF, 0, 0,0, 0,READWRITE },
{1  , fld_tmpReg3       ,VIP_VIP_COMMAND_STATUS         , 3, 0xFFFFFFFF, 0, 0,0, 0,READWRITE },
{8  , fld_LP_CONTRAST   ,VIP_LP_CONTRAST            ,  0, 0xFFFFFF00, 0, 0,0, fld_LP_CONTRAST_def       ,READWRITE  },
{14 , fld_LP_BRIGHTNESS ,VIP_LP_BRIGHTNESS          ,  0, 0xFFFFC000, 0, 0,0, fld_LP_BRIGHTNESS_def     ,READWRITE  },
{8  , fld_CP_HUE_CNTL   ,VIP_CP_HUE_CNTL            ,  0, 0xFFFFFF00, 0, 0,0, fld_CP_HUE_CNTL_def       ,READWRITE  },
{1  , fld_LUMA_FILTER   ,VIP_LP_BRIGHTNESS          , 15, 0xFFFF7FFF, 0, 0,0, fld_LUMA_FILTER_def       ,READWRITE  },
{21 , fld_H_SCALE_RATIO ,VIP_H_SCALER_CONTROL       ,  0, 0xFFE00000, 0, 0,0, fld_H_SCALE_RATIO_def     ,READWRITE  },
{4  , fld_H_SHARPNESS   ,VIP_H_SCALER_CONTROL       , 25, 0xE1FFFFFF, 0, 0,0, fld_H_SHARPNESS_def       ,READWRITE  },
{12 , fld_V_SCALE_RATIO ,VIP_V_SCALER_CONTROL       ,  0, 0xFFFFF000, 0, 0,0, fld_V_SCALE_RATIO_def     ,READWRITE  },
{1  , fld_V_DEINTERLACE_ON,VIP_V_SCALER_CONTROL     , 12, 0xFFFFEFFF, 0, 0,0, fld_V_DEINTERLACE_ON_def  ,READWRITE  },
{1  , fld_V_BYPSS       ,VIP_V_SCALER_CONTROL       , 14, 0xFFFFBFFF, 0, 0,0, fld_V_BYPSS_def           ,READWRITE  },
{1  , fld_V_DITHER_ON   ,VIP_V_SCALER_CONTROL       , 15, 0xFFFF7FFF, 0, 0,0, fld_V_DITHER_ON_def       ,READWRITE  },
{11 , fld_EVENF_OFFSET  ,VIP_V_DEINTERLACE_CONTROL  ,  0, 0xFFFFF800, 0, 0,0, fld_EVENF_OFFSET_def      ,READWRITE  },
{11 , fld_ODDF_OFFSET   ,VIP_V_DEINTERLACE_CONTROL  , 11, 0xFFC007FF, 0, 0,0, fld_ODDF_OFFSET_def       ,READWRITE  },
{1  , fld_INTERLACE_DETECTED    ,VIP_VS_LINE_COUNT  , 15, 0xFFFF7FFF, 0, 0,0, fld_INTERLACE_DETECTED_def,READONLY   },
{10 , fld_VS_LINE_COUNT     ,VIP_VS_LINE_COUNT      ,  0, 0xFFFFFC00, 0, 0,0, fld_VS_LINE_COUNT_def     ,READONLY   },
{10 , fld_VS_DETECTED_LINES ,VIP_VS_LINE_COUNT      , 16, 0xFC00FFFF, 0, 0,0, fld_VS_DETECTED_LINES_def ,READONLY   },
{1  , fld_VS_ITU656_VB  ,VIP_VS_LINE_COUNT          , 13, 0xFFFFDFFF, 0, 0,0, fld_VS_ITU656_VB_def  ,READONLY   },
{16 , fld_VBI_CC_DATA   ,VIP_VBI_CC_CNTL            ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_CC_DATA_def       ,READWRITE  },
{1  , fld_VBI_CC_WT     ,VIP_VBI_CC_CNTL            , 24, 0xFEFFFFFF, 0, 0,0, fld_VBI_CC_WT_def         ,READWRITE  },
{1  , fld_VBI_CC_WT_ACK ,VIP_VBI_CC_CNTL            , 25, 0xFDFFFFFF, 0, 0,0, fld_VBI_CC_WT_ACK_def     ,READONLY   },
{1  , fld_VBI_CC_HOLD   ,VIP_VBI_CC_CNTL            , 26, 0xFBFFFFFF, 0, 0,0, fld_VBI_CC_HOLD_def       ,READWRITE  },
{1  , fld_VBI_DECODE_EN ,VIP_VBI_CC_CNTL            , 31, 0x7FFFFFFF, 0, 0,0, fld_VBI_DECODE_EN_def     ,READWRITE  },
{16 , fld_VBI_CC_DTO_P  ,VIP_VBI_DTO_CNTL           ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_CC_DTO_P_def      ,READWRITE  },
{16 ,fld_VBI_20BIT_DTO_P,VIP_VBI_DTO_CNTL           , 16, 0x0000FFFF, 0, 0,0, fld_VBI_20BIT_DTO_P_def   ,READWRITE  },
{7  ,fld_VBI_CC_LEVEL   ,VIP_VBI_LEVEL_CNTL         ,  0, 0xFFFFFF80, 0, 0,0, fld_VBI_CC_LEVEL_def      ,READWRITE  },
{7  ,fld_VBI_20BIT_LEVEL,VIP_VBI_LEVEL_CNTL         ,  8, 0xFFFF80FF, 0, 0,0, fld_VBI_20BIT_LEVEL_def   ,READWRITE  },
{9  ,fld_VBI_CLK_RUNIN_GAIN,VIP_VBI_LEVEL_CNTL      , 16, 0xFE00FFFF, 0, 0,0, fld_VBI_CLK_RUNIN_GAIN_def,READWRITE  },
{11 ,fld_H_VBI_WIND_START,VIP_H_VBI_WINDOW          ,  0, 0xFFFFF800, 0, 0,0, fld_H_VBI_WIND_START_def  ,READWRITE  },
{11 ,fld_H_VBI_WIND_END,VIP_H_VBI_WINDOW            , 16, 0xF800FFFF, 0, 0,0, fld_H_VBI_WIND_END_def    ,READWRITE  },
{10 ,fld_V_VBI_WIND_START,VIP_V_VBI_WINDOW          ,  0, 0xFFFFFC00, 0, 0,0, fld_V_VBI_WIND_START_def  ,READWRITE  },
{10 ,fld_V_VBI_WIND_END,VIP_V_VBI_WINDOW            , 16, 0xFC00FFFF, 0, 0,0, fld_V_VBI_WIND_END_def    ,READWRITE  }, /* CHK */
{16 ,fld_VBI_20BIT_DATA0,VIP_VBI_20BIT_CNTL         ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_20BIT_DATA0_def   ,READWRITE  },
{4  ,fld_VBI_20BIT_DATA1,VIP_VBI_20BIT_CNTL         , 16, 0xFFF0FFFF, 0, 0,0, fld_VBI_20BIT_DATA1_def   ,READWRITE  },
{1  ,fld_VBI_20BIT_WT   ,VIP_VBI_20BIT_CNTL         , 24, 0xFEFFFFFF, 0, 0,0, fld_VBI_20BIT_WT_def      ,READWRITE  },
{1  ,fld_VBI_20BIT_WT_ACK   ,VIP_VBI_20BIT_CNTL     , 25, 0xFDFFFFFF, 0, 0,0, fld_VBI_20BIT_WT_ACK_def  ,READONLY   },
{1  ,fld_VBI_20BIT_HOLD ,VIP_VBI_20BIT_CNTL         , 26, 0xFBFFFFFF, 0, 0,0, fld_VBI_20BIT_HOLD_def    ,READWRITE  },
{2  ,fld_VBI_CAPTURE_ENABLE ,VIP_VBI_CONTROL        ,  0, 0xFFFFFFFC, 0, 0,0, fld_VBI_CAPTURE_ENABLE_def,READWRITE  },
{16 ,fld_VBI_EDS_DATA   ,VIP_VBI_EDS_CNTL           ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_EDS_DATA_def      ,READWRITE  },
{1  ,fld_VBI_EDS_WT     ,VIP_VBI_EDS_CNTL           , 24, 0xFEFFFFFF, 0, 0,0, fld_VBI_EDS_WT_def        ,READWRITE  },
{1  ,fld_VBI_EDS_WT_ACK ,VIP_VBI_EDS_CNTL           , 25, 0xFDFFFFFF, 0, 0,0, fld_VBI_EDS_WT_ACK_def    ,READONLY   },
{1  ,fld_VBI_EDS_HOLD   ,VIP_VBI_EDS_CNTL           , 26, 0xFBFFFFFF, 0, 0,0, fld_VBI_EDS_HOLD_def      ,READWRITE  },
{17 ,fld_VBI_SCALING_RATIO  ,VIP_VBI_SCALER_CONTROL ,  0, 0xFFFE0000, 0, 0,0, fld_VBI_SCALING_RATIO_def ,READWRITE  },
{1  ,fld_VBI_ALIGNER_ENABLE ,VIP_VBI_SCALER_CONTROL , 17, 0xFFFDFFFF, 0, 0,0, fld_VBI_ALIGNER_ENABLE_def,READWRITE  },
{11 ,fld_H_ACTIVE_START ,VIP_H_ACTIVE_WINDOW        ,  0, 0xFFFFF800, 0, 0,0, fld_H_ACTIVE_START_def    ,READWRITE  },
{11 ,fld_H_ACTIVE_END   ,VIP_H_ACTIVE_WINDOW        , 16, 0xF800FFFF, 0, 0,0, fld_H_ACTIVE_END_def      ,READWRITE  },
{10 ,fld_V_ACTIVE_START ,VIP_V_ACTIVE_WINDOW        ,  0, 0xFFFFFC00, 0, 0,0, fld_V_ACTIVE_START_def    ,READWRITE  },
{10 ,fld_V_ACTIVE_END   ,VIP_V_ACTIVE_WINDOW        , 16, 0xFC00FFFF, 0, 0,0, fld_V_ACTIVE_END_def      ,READWRITE  },
{8  ,fld_CH_HEIGHT          ,VIP_CP_AGC_CNTL        ,  0, 0xFFFFFF00, 0, 0,0, fld_CH_HEIGHT_def         ,READWRITE  },
{8  ,fld_CH_KILL_LEVEL      ,VIP_CP_AGC_CNTL        ,  8, 0xFFFF00FF, 0, 0,0, fld_CH_KILL_LEVEL_def     ,READWRITE  },
{2  ,fld_CH_AGC_ERROR_LIM   ,VIP_CP_AGC_CNTL        , 16, 0xFFFCFFFF, 0, 0,0, fld_CH_AGC_ERROR_LIM_def  ,READWRITE  },
{1  ,fld_CH_AGC_FILTER_EN   ,VIP_CP_AGC_CNTL        , 18, 0xFFFBFFFF, 0, 0,0, fld_CH_AGC_FILTER_EN_def  ,READWRITE  },
{1  ,fld_CH_AGC_LOOP_SPEED  ,VIP_CP_AGC_CNTL        , 19, 0xFFF7FFFF, 0, 0,0, fld_CH_AGC_LOOP_SPEED_def ,READWRITE  },
{8  ,fld_HUE_ADJ            ,VIP_CP_HUE_CNTL        ,  0, 0xFFFFFF00, 0, 0,0, fld_HUE_ADJ_def           ,READWRITE  },
{2  ,fld_STANDARD_SEL       ,VIP_STANDARD_SELECT    ,  0, 0xFFFFFFFC, 0, 0,0, fld_STANDARD_SEL_def      ,READWRITE  },
{1  ,fld_STANDARD_YC        ,VIP_STANDARD_SELECT    ,  2, 0xFFFFFFFB, 0, 0,0, fld_STANDARD_YC_def       ,READWRITE  },
{1  ,fld_ADC_PDWN           ,VIP_ADC_CNTL           ,  7, 0xFFFFFF7F, 0, 0,0, fld_ADC_PDWN_def          ,READWRITE  },
{3  ,fld_INPUT_SELECT       ,VIP_ADC_CNTL           ,  0, 0xFFFFFFF8, 0, 0,0, fld_INPUT_SELECT_def      ,READWRITE  },
{2  ,fld_ADC_PREFLO         ,VIP_ADC_CNTL           , 24, 0xFCFFFFFF, 0, 0,0, fld_ADC_PREFLO_def        ,READWRITE  },
{8  ,fld_H_SYNC_PULSE_WIDTH ,VIP_HS_PULSE_WIDTH     ,  0, 0xFFFFFF00, 0, 0,0, fld_H_SYNC_PULSE_WIDTH_def,READONLY   },
{1  ,fld_HS_GENLOCKED       ,VIP_HS_PULSE_WIDTH     ,  8, 0xFFFFFEFF, 0, 0,0, fld_HS_GENLOCKED_def      ,READONLY   },
{1  ,fld_HS_SYNC_IN_WIN     ,VIP_HS_PULSE_WIDTH     ,  9, 0xFFFFFDFF, 0, 0,0, fld_HS_SYNC_IN_WIN_def    ,READONLY   },
{1  ,fld_VIN_ASYNC_RST      ,VIP_MASTER_CNTL        ,  5, 0xFFFFFFDF, 0, 0,0, fld_VIN_ASYNC_RST_def     ,READWRITE  },
{1  ,fld_DVS_ASYNC_RST      ,VIP_MASTER_CNTL        ,  7, 0xFFFFFF7F, 0, 0,0, fld_DVS_ASYNC_RST_def     ,READWRITE  },
{16 ,fld_VIP_VENDOR_ID      ,VIP_VIP_VENDOR_DEVICE_ID, 0, 0xFFFF0000, 0, 0,0, fld_VIP_VENDOR_ID_def     ,READONLY   },
{16 ,fld_VIP_DEVICE_ID      ,VIP_VIP_VENDOR_DEVICE_ID,16, 0x0000FFFF, 0, 0,0, fld_VIP_DEVICE_ID_def     ,READONLY   },
{16 ,fld_VIP_REVISION_ID    ,VIP_VIP_REVISION_ID    ,  0, 0xFFFF0000, 0, 0,0, fld_VIP_REVISION_ID_def   ,READONLY   },
{8  ,fld_BLACK_INT_START    ,VIP_SG_BLACK_GATE      ,  0, 0xFFFFFF00, 0, 0,0, fld_BLACK_INT_START_def   ,READWRITE  },
{4  ,fld_BLACK_INT_LENGTH   ,VIP_SG_BLACK_GATE      ,  8, 0xFFFFF0FF, 0, 0,0, fld_BLACK_INT_LENGTH_def  ,READWRITE  },
{8  ,fld_UV_INT_START       ,VIP_SG_UVGATE_GATE     ,  0, 0xFFFFFF00, 0, 0,0, fld_UV_INT_START_def      ,READWRITE  },
{4  ,fld_U_INT_LENGTH       ,VIP_SG_UVGATE_GATE     ,  8, 0xFFFFF0FF, 0, 0,0, fld_U_INT_LENGTH_def      ,READWRITE  },
{4  ,fld_V_INT_LENGTH       ,VIP_SG_UVGATE_GATE     , 12, 0xFFFF0FFF, 0, 0,0, fld_V_INT_LENGTH_def      ,READWRITE  },
{10 ,fld_CRDR_ACTIVE_GAIN   ,VIP_CP_ACTIVE_GAIN     ,  0, 0xFFFFFC00, 0, 0,0, fld_CRDR_ACTIVE_GAIN_def  ,READWRITE  },
{10 ,fld_CBDB_ACTIVE_GAIN   ,VIP_CP_ACTIVE_GAIN     , 16, 0xFC00FFFF, 0, 0,0, fld_CBDB_ACTIVE_GAIN_def  ,READWRITE  },
{1  ,fld_DVS_DIRECTION      ,VIP_DVS_PORT_CTRL      ,  0, 0xFFFFFFFE, 0, 0,0, fld_DVS_DIRECTION_def     ,READWRITE  },
{1  ,fld_DVS_VBI_UINT8_SWAP  ,VIP_DVS_PORT_CTRL      ,  1, 0xFFFFFFFD, 0, 0,0, fld_DVS_VBI_UINT8_SWAP_def ,READWRITE  },
{1  ,fld_DVS_CLK_SELECT     ,VIP_DVS_PORT_CTRL      ,  2, 0xFFFFFFFB, 0, 0,0, fld_DVS_CLK_SELECT_def    ,READWRITE  },
{1  ,fld_CONTINUOUS_STREAM  ,VIP_DVS_PORT_CTRL      ,  3, 0xFFFFFFF7, 0, 0,0, fld_CONTINUOUS_STREAM_def ,READWRITE  },
{1  ,fld_DVSOUT_CLK_DRV     ,VIP_DVS_PORT_CTRL      ,  4, 0xFFFFFFEF, 0, 0,0, fld_DVSOUT_CLK_DRV_def    ,READWRITE  },
{1  ,fld_DVSOUT_DATA_DRV    ,VIP_DVS_PORT_CTRL      ,  5, 0xFFFFFFDF, 0, 0,0, fld_DVSOUT_DATA_DRV_def   ,READWRITE  },
{32 ,fld_COMB_CNTL0         ,VIP_COMB_CNTL0         ,  0, 0x00000000, 0, 0,0, fld_COMB_CNTL0_def        ,READWRITE  },
{32 ,fld_COMB_CNTL1         ,VIP_COMB_CNTL1         ,  0, 0x00000000, 0, 0,0, fld_COMB_CNTL1_def        ,READWRITE  },
{32 ,fld_COMB_CNTL2         ,VIP_COMB_CNTL2         ,  0, 0x00000000, 0, 0,0, fld_COMB_CNTL2_def        ,READWRITE  },
{32 ,fld_COMB_LENGTH        ,VIP_COMB_LINE_LENGTH   ,  0, 0x00000000, 0, 0,0, fld_COMB_LENGTH_def       ,READWRITE  },
{8  ,fld_SYNCTIP_REF0       ,VIP_LP_AGC_CLAMP_CNTL0 ,  0, 0xFFFFFF00, 0, 0,0, fld_SYNCTIP_REF0_def      ,READWRITE  },
{8  ,fld_SYNCTIP_REF1       ,VIP_LP_AGC_CLAMP_CNTL0 ,  8, 0xFFFF00FF, 0, 0,0, fld_SYNCTIP_REF1_def      ,READWRITE  },
{8  ,fld_CLAMP_REF          ,VIP_LP_AGC_CLAMP_CNTL0 , 16, 0xFF00FFFF, 0, 0,0, fld_CLAMP_REF_def          ,READWRITE  },
{8  ,fld_AGC_PEAKWHITE      ,VIP_LP_AGC_CLAMP_CNTL0 , 24, 0x00FFFFFF, 0, 0,0, fld_AGC_PEAKWHITE_def     ,READWRITE  },
{8  ,fld_VBI_PEAKWHITE      ,VIP_LP_AGC_CLAMP_CNTL1 ,  0, 0xFFFFFF00, 0, 0,0, fld_VBI_PEAKWHITE_def     ,READWRITE  },
{11 ,fld_WPA_THRESHOLD      ,VIP_LP_WPA_CNTL0       ,  0, 0xFFFFF800, 0, 0,0, fld_WPA_THRESHOLD_def     ,READWRITE  },
{10 ,fld_WPA_TRIGGER_LO     ,VIP_LP_WPA_CNTL1       ,  0, 0xFFFFFC00, 0, 0,0, fld_WPA_TRIGGER_LO_def    ,READWRITE  },
{10 ,fld_WPA_TRIGGER_HIGH   ,VIP_LP_WPA_CNTL1       , 16, 0xFC00FFFF, 0, 0,0, fld_WPA_TRIGGER_HIGH_def  ,READWRITE  },
{10 ,fld_LOCKOUT_START      ,VIP_LP_VERT_LOCKOUT    ,  0, 0xFFFFFC00, 0, 0,0, fld_LOCKOUT_START_def     ,READWRITE  },
{10 ,fld_LOCKOUT_END        ,VIP_LP_VERT_LOCKOUT    , 16, 0xFC00FFFF, 0, 0,0, fld_LOCKOUT_END_def       ,READWRITE  },
{24 ,fld_CH_DTO_INC         ,VIP_CP_PLL_CNTL0       ,  0, 0xFF000000, 0, 0,0, fld_CH_DTO_INC_def        ,READWRITE  },
{4  ,fld_PLL_SGAIN          ,VIP_CP_PLL_CNTL0       , 24, 0xF0FFFFFF, 0, 0,0, fld_PLL_SGAIN_def         ,READWRITE  },
{4  ,fld_PLL_FGAIN          ,VIP_CP_PLL_CNTL0       , 28, 0x0FFFFFFF, 0, 0,0, fld_PLL_FGAIN_def         ,READWRITE  },
{9  ,fld_CR_BURST_GAIN      ,VIP_CP_BURST_GAIN      ,  0, 0xFFFFFE00, 0, 0,0, fld_CR_BURST_GAIN_def     ,READWRITE  },
{9  ,fld_CB_BURST_GAIN      ,VIP_CP_BURST_GAIN      , 16, 0xFE00FFFF, 0, 0,0, fld_CB_BURST_GAIN_def     ,READWRITE  },
{10 ,fld_VERT_LOCKOUT_START ,VIP_CP_VERT_LOCKOUT    ,  0, 0xFFFFFC00, 0, 0,0, fld_VERT_LOCKOUT_START_def,READWRITE  },
{10 ,fld_VERT_LOCKOUT_END   ,VIP_CP_VERT_LOCKOUT    , 16, 0xFC00FFFF, 0, 0,0, fld_VERT_LOCKOUT_END_def  ,READWRITE  },
{11 ,fld_H_IN_WIND_START    ,VIP_SCALER_IN_WINDOW   ,  0, 0xFFFFF800, 0, 0,0, fld_H_IN_WIND_START_def   ,READWRITE  },
{10 ,fld_V_IN_WIND_START    ,VIP_SCALER_IN_WINDOW   , 16, 0xFC00FFFF, 0, 0,0, fld_V_IN_WIND_START_def   ,READWRITE  },
{10 ,fld_H_OUT_WIND_WIDTH   ,VIP_SCALER_OUT_WINDOW ,  0, 0xFFFFFC00, 0, 0,0, fld_H_OUT_WIND_WIDTH_def   ,READWRITE  },
{9  ,fld_V_OUT_WIND_WIDTH   ,VIP_SCALER_OUT_WINDOW , 16, 0xFE00FFFF, 0, 0,0, fld_V_OUT_WIND_WIDTH_def   ,READWRITE  },
{11 ,fld_HS_LINE_TOTAL      ,VIP_HS_PLINE          ,  0, 0xFFFFF800, 0, 0,0, fld_HS_LINE_TOTAL_def      ,READWRITE  },
{8  ,fld_MIN_PULSE_WIDTH    ,VIP_HS_MINMAXWIDTH    ,  0, 0xFFFFFF00, 0, 0,0, fld_MIN_PULSE_WIDTH_def    ,READWRITE  },
{8  ,fld_MAX_PULSE_WIDTH    ,VIP_HS_MINMAXWIDTH    ,  8, 0xFFFF00FF, 0, 0,0, fld_MAX_PULSE_WIDTH_def    ,READWRITE  },
{11 ,fld_WIN_CLOSE_LIMIT    ,VIP_HS_WINDOW_LIMIT   ,  0, 0xFFFFF800, 0, 0,0, fld_WIN_CLOSE_LIMIT_def    ,READWRITE  },
{11 ,fld_WIN_OPEN_LIMIT     ,VIP_HS_WINDOW_LIMIT   , 16, 0xF800FFFF, 0, 0,0, fld_WIN_OPEN_LIMIT_def     ,READWRITE  },
{11 ,fld_VSYNC_INT_TRIGGER  ,VIP_VS_DETECTOR_CNTL   ,  0, 0xFFFFF800, 0, 0,0, fld_VSYNC_INT_TRIGGER_def ,READWRITE  },
{11 ,fld_VSYNC_INT_HOLD     ,VIP_VS_DETECTOR_CNTL   , 16, 0xF800FFFF, 0, 0,0, fld_VSYNC_INT_HOLD_def        ,READWRITE  },
{11 ,fld_VIN_M0             ,VIP_VIN_PLL_CNTL      ,  0, 0xFFFFF800, 0, 0,0, fld_VIN_M0_def             ,READWRITE  },
{11 ,fld_VIN_N0             ,VIP_VIN_PLL_CNTL      , 11, 0xFFC007FF, 0, 0,0, fld_VIN_N0_def             ,READWRITE  },
{1  ,fld_MNFLIP_EN          ,VIP_VIN_PLL_CNTL      , 22, 0xFFBFFFFF, 0, 0,0, fld_MNFLIP_EN_def          ,READWRITE  },
{4  ,fld_VIN_P              ,VIP_VIN_PLL_CNTL      , 24, 0xF0FFFFFF, 0, 0,0, fld_VIN_P_def              ,READWRITE  },
{2  ,fld_REG_CLK_SEL        ,VIP_VIN_PLL_CNTL      , 30, 0x3FFFFFFF, 0, 0,0, fld_REG_CLK_SEL_def        ,READWRITE  },
{11 ,fld_VIN_M1             ,VIP_VIN_PLL_FINE_CNTL  ,  0, 0xFFFFF800, 0, 0,0, fld_VIN_M1_def            ,READWRITE  },
{11 ,fld_VIN_N1             ,VIP_VIN_PLL_FINE_CNTL  , 11, 0xFFC007FF, 0, 0,0, fld_VIN_N1_def            ,READWRITE  },
{1  ,fld_VIN_DRIVER_SEL     ,VIP_VIN_PLL_FINE_CNTL  , 22, 0xFFBFFFFF, 0, 0,0, fld_VIN_DRIVER_SEL_def    ,READWRITE  },
{1  ,fld_VIN_MNFLIP_REQ     ,VIP_VIN_PLL_FINE_CNTL  , 23, 0xFF7FFFFF, 0, 0,0, fld_VIN_MNFLIP_REQ_def    ,READWRITE  },
{1  ,fld_VIN_MNFLIP_DONE    ,VIP_VIN_PLL_FINE_CNTL  , 24, 0xFEFFFFFF, 0, 0,0, fld_VIN_MNFLIP_DONE_def   ,READONLY   },
{1  ,fld_TV_LOCK_TO_VIN     ,VIP_VIN_PLL_FINE_CNTL  , 27, 0xF7FFFFFF, 0, 0,0, fld_TV_LOCK_TO_VIN_def    ,READWRITE  },
{4  ,fld_TV_P_FOR_WINCLK    ,VIP_VIN_PLL_FINE_CNTL  , 28, 0x0FFFFFFF, 0, 0,0, fld_TV_P_FOR_WINCLK_def   ,READWRITE  },
{1  ,fld_VINRST             ,VIP_PLL_CNTL1          ,  1, 0xFFFFFFFD, 0, 0,0, fld_VINRST_def            ,READWRITE  },
{1  ,fld_VIN_CLK_SEL        ,VIP_CLOCK_SEL_CNTL     ,  7, 0xFFFFFF7F, 0, 0,0, fld_VIN_CLK_SEL_def       ,READWRITE  },
{10 ,fld_VS_FIELD_BLANK_START,VIP_VS_BLANKING_CNTL  ,  0, 0xFFFFFC00, 0, 0,0, fld_VS_FIELD_BLANK_START_def  ,READWRITE  },
{10 ,fld_VS_FIELD_BLANK_END,VIP_VS_BLANKING_CNTL    , 16, 0xFC00FFFF, 0, 0,0, fld_VS_FIELD_BLANK_END_def    ,READWRITE  },
{9  ,fld_VS_FIELD_IDLOCATION,VIP_VS_FIELD_ID_CNTL   ,  0, 0xFFFFFE00, 0, 0,0, fld_VS_FIELD_IDLOCATION_def   ,READWRITE  },
{10 ,fld_VS_FRAME_TOTAL     ,VIP_VS_FRAME_TOTAL     ,  0, 0xFFFFFC00, 0, 0,0, fld_VS_FRAME_TOTAL_def    ,READWRITE  },
{11 ,fld_SYNC_TIP_START     ,VIP_SG_SYNCTIP_GATE    ,  0, 0xFFFFF800, 0, 0,0, fld_SYNC_TIP_START_def    ,READWRITE  },
{4  ,fld_SYNC_TIP_LENGTH    ,VIP_SG_SYNCTIP_GATE    , 12, 0xFFFF0FFF, 0, 0,0, fld_SYNC_TIP_LENGTH_def   ,READWRITE  },
{12 ,fld_GAIN_FORCE_DATA    ,VIP_CP_DEBUG_FORCE     ,  0, 0xFFFFF000, 0, 0,0, fld_GAIN_FORCE_DATA_def   ,READWRITE  },
{1  ,fld_GAIN_FORCE_EN      ,VIP_CP_DEBUG_FORCE     , 12, 0xFFFFEFFF, 0, 0,0, fld_GAIN_FORCE_EN_def ,READWRITE  },
{2  ,fld_I_CLAMP_SEL        ,VIP_ADC_CNTL           ,  3, 0xFFFFFFE7, 0, 0,0, fld_I_CLAMP_SEL_def   ,READWRITE  },
{2  ,fld_I_AGC_SEL          ,VIP_ADC_CNTL           ,  5, 0xFFFFFF9F, 0, 0,0, fld_I_AGC_SEL_def     ,READWRITE  },
{1  ,fld_EXT_CLAMP_CAP      ,VIP_ADC_CNTL           ,  8, 0xFFFFFEFF, 0, 0,0, fld_EXT_CLAMP_CAP_def ,READWRITE  },
{1  ,fld_EXT_AGC_CAP        ,VIP_ADC_CNTL           ,  9, 0xFFFFFDFF, 0, 0,0, fld_EXT_AGC_CAP_def       ,READWRITE  },
{1  ,fld_DECI_DITHER_EN     ,VIP_ADC_CNTL           , 12, 0xFFFFEFFF, 0, 0,0, fld_DECI_DITHER_EN_def ,READWRITE },
{2  ,fld_ADC_PREFHI         ,VIP_ADC_CNTL           , 22, 0xFF3FFFFF, 0, 0,0, fld_ADC_PREFHI_def        ,READWRITE  },
{2  ,fld_ADC_CH_GAIN_SEL    ,VIP_ADC_CNTL           , 16, 0xFFFCFFFF, 0, 0,0, fld_ADC_CH_GAIN_SEL_def   ,READWRITE  },
{4  ,fld_HS_PLL_SGAIN       ,VIP_HS_PLLGAIN         ,  0, 0xFFFFFFF0, 0, 0,0, fld_HS_PLL_SGAIN_def      ,READWRITE  },
{1  ,fld_NREn               ,VIP_NOISE_CNTL0        ,  0, 0xFFFFFFFE, 0, 0,0, fld_NREn_def      ,READWRITE  },
{3  ,fld_NRGainCntl         ,VIP_NOISE_CNTL0        ,  1, 0xFFFFFFF1, 0, 0,0, fld_NRGainCntl_def        ,READWRITE  },
{6  ,fld_NRBWTresh          ,VIP_NOISE_CNTL0        ,  4, 0xFFFFFC0F, 0, 0,0, fld_NRBWTresh_def     ,READWRITE  },
{5  ,fld_NRGCTresh          ,VIP_NOISE_CNTL0       ,  10, 0xFFFF83FF, 0, 0,0, fld_NRGCTresh_def     ,READWRITE  },
{1  ,fld_NRCoefDespeclMode  ,VIP_NOISE_CNTL0       ,  15, 0xFFFF7FFF, 0, 0,0, fld_NRCoefDespeclMode_def     ,READWRITE  },
{1  ,fld_GPIO_5_OE      ,VIP_GPIO_CNTL      ,  5, 0xFFFFFFDF, 0, 0,0, fld_GPIO_5_OE_def     ,READWRITE  },
{1  ,fld_GPIO_6_OE      ,VIP_GPIO_CNTL      ,  6, 0xFFFFFFBF, 0, 0,0, fld_GPIO_6_OE_def     ,READWRITE  },
{1  ,fld_GPIO_5_OUT     ,VIP_GPIO_INOUT    ,   5, 0xFFFFFFDF, 0, 0,0, fld_GPIO_5_OUT_def        ,READWRITE  },
{1  ,fld_GPIO_6_OUT     ,VIP_GPIO_INOUT    ,   6, 0xFFFFFFBF, 0, 0,0, fld_GPIO_6_OUT_def        ,READWRITE  },
};

/* Rage Theatre's register fields default values: */
uint32_t RT_RegDef[regRT_MAX_REGS]=
{
fld_tmpReg1_def,
fld_tmpReg2_def,
fld_tmpReg3_def,
fld_LP_CONTRAST_def,
fld_LP_BRIGHTNESS_def,
fld_CP_HUE_CNTL_def,
fld_LUMA_FILTER_def,
fld_H_SCALE_RATIO_def,
fld_H_SHARPNESS_def,
fld_V_SCALE_RATIO_def,
fld_V_DEINTERLACE_ON_def,
fld_V_BYPSS_def,
fld_V_DITHER_ON_def,
fld_EVENF_OFFSET_def,
fld_ODDF_OFFSET_def,
fld_INTERLACE_DETECTED_def,
fld_VS_LINE_COUNT_def,
fld_VS_DETECTED_LINES_def,
fld_VS_ITU656_VB_def,
fld_VBI_CC_DATA_def,
fld_VBI_CC_WT_def,
fld_VBI_CC_WT_ACK_def,
fld_VBI_CC_HOLD_def,
fld_VBI_DECODE_EN_def,
fld_VBI_CC_DTO_P_def,
fld_VBI_20BIT_DTO_P_def,
fld_VBI_CC_LEVEL_def,
fld_VBI_20BIT_LEVEL_def,
fld_VBI_CLK_RUNIN_GAIN_def,
fld_H_VBI_WIND_START_def,
fld_H_VBI_WIND_END_def,
fld_V_VBI_WIND_START_def,
fld_V_VBI_WIND_END_def,
fld_VBI_20BIT_DATA0_def,
fld_VBI_20BIT_DATA1_def,
fld_VBI_20BIT_WT_def,
fld_VBI_20BIT_WT_ACK_def,
fld_VBI_20BIT_HOLD_def,
fld_VBI_CAPTURE_ENABLE_def,
fld_VBI_EDS_DATA_def,
fld_VBI_EDS_WT_def,
fld_VBI_EDS_WT_ACK_def,
fld_VBI_EDS_HOLD_def,
fld_VBI_SCALING_RATIO_def,
fld_VBI_ALIGNER_ENABLE_def,
fld_H_ACTIVE_START_def,
fld_H_ACTIVE_END_def,
fld_V_ACTIVE_START_def,
fld_V_ACTIVE_END_def,
fld_CH_HEIGHT_def,
fld_CH_KILL_LEVEL_def,
fld_CH_AGC_ERROR_LIM_def,
fld_CH_AGC_FILTER_EN_def,
fld_CH_AGC_LOOP_SPEED_def,
fld_HUE_ADJ_def,
fld_STANDARD_SEL_def,
fld_STANDARD_YC_def,
fld_ADC_PDWN_def,
fld_INPUT_SELECT_def,
fld_ADC_PREFLO_def,
fld_H_SYNC_PULSE_WIDTH_def,
fld_HS_GENLOCKED_def,
fld_HS_SYNC_IN_WIN_def,
fld_VIN_ASYNC_RST_def,
fld_DVS_ASYNC_RST_def,
fld_VIP_VENDOR_ID_def,
fld_VIP_DEVICE_ID_def,
fld_VIP_REVISION_ID_def,
fld_BLACK_INT_START_def,
fld_BLACK_INT_LENGTH_def,
fld_UV_INT_START_def,
fld_U_INT_LENGTH_def,
fld_V_INT_LENGTH_def,
fld_CRDR_ACTIVE_GAIN_def,
fld_CBDB_ACTIVE_GAIN_def,
fld_DVS_DIRECTION_def,
fld_DVS_VBI_UINT8_SWAP_def,
fld_DVS_CLK_SELECT_def,
fld_CONTINUOUS_STREAM_def,
fld_DVSOUT_CLK_DRV_def,
fld_DVSOUT_DATA_DRV_def,
fld_COMB_CNTL0_def,
fld_COMB_CNTL1_def,
fld_COMB_CNTL2_def,
fld_COMB_LENGTH_def,
fld_SYNCTIP_REF0_def,
fld_SYNCTIP_REF1_def,
fld_CLAMP_REF_def,
fld_AGC_PEAKWHITE_def,
fld_VBI_PEAKWHITE_def,
fld_WPA_THRESHOLD_def,
fld_WPA_TRIGGER_LO_def,
fld_WPA_TRIGGER_HIGH_def,
fld_LOCKOUT_START_def,
fld_LOCKOUT_END_def,
fld_CH_DTO_INC_def,
fld_PLL_SGAIN_def,
fld_PLL_FGAIN_def,
fld_CR_BURST_GAIN_def,
fld_CB_BURST_GAIN_def,
fld_VERT_LOCKOUT_START_def,
fld_VERT_LOCKOUT_END_def,
fld_H_IN_WIND_START_def,
fld_V_IN_WIND_START_def,
fld_H_OUT_WIND_WIDTH_def,
fld_V_OUT_WIND_WIDTH_def,
fld_HS_LINE_TOTAL_def,
fld_MIN_PULSE_WIDTH_def,
fld_MAX_PULSE_WIDTH_def,
fld_WIN_CLOSE_LIMIT_def,
fld_WIN_OPEN_LIMIT_def,
fld_VSYNC_INT_TRIGGER_def,
fld_VSYNC_INT_HOLD_def,
fld_VIN_M0_def,
fld_VIN_N0_def,
fld_MNFLIP_EN_def,
fld_VIN_P_def,
fld_REG_CLK_SEL_def,
fld_VIN_M1_def,
fld_VIN_N1_def,
fld_VIN_DRIVER_SEL_def,
fld_VIN_MNFLIP_REQ_def,
fld_VIN_MNFLIP_DONE_def,
fld_TV_LOCK_TO_VIN_def,
fld_TV_P_FOR_WINCLK_def,
fld_VINRST_def,
fld_VIN_CLK_SEL_def,
fld_VS_FIELD_BLANK_START_def,
fld_VS_FIELD_BLANK_END_def,
fld_VS_FIELD_IDLOCATION_def,
fld_VS_FRAME_TOTAL_def,
fld_SYNC_TIP_START_def,
fld_SYNC_TIP_LENGTH_def,
fld_GAIN_FORCE_DATA_def,
fld_GAIN_FORCE_EN_def,
fld_I_CLAMP_SEL_def,
fld_I_AGC_SEL_def,
fld_EXT_CLAMP_CAP_def,
fld_EXT_AGC_CAP_def,
fld_DECI_DITHER_EN_def,
fld_ADC_PREFHI_def,
fld_ADC_CH_GAIN_SEL_def,
fld_HS_PLL_SGAIN_def,
fld_NREn_def,
fld_NRGainCntl_def,
fld_NRBWTresh_def,
fld_NRGCTresh_def,
fld_NRCoefDespeclMode_def,
fld_GPIO_5_OE_def,
fld_GPIO_6_OE_def,
fld_GPIO_5_OUT_def,
fld_GPIO_6_OUT_def,
};

/****************************************************************************
 * WriteRT_fld (uint32_t dwReg, uint32_t dwData)                                  *
 *  Function: Writes a register field within Rage Theatre                   *
 *    Inputs: uint32_t dwReg = register field to be written                    *
 *            uint32_t dwData = data that will be written to the reg field     *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
static void WriteRT_fld1 (TheatrePtr t, uint32_t dwReg, uint32_t dwData)
{
	uint32_t dwResult=0;
	uint32_t dwValue=0;
	
	if (RT_regr (RT_RegMap[dwReg].dwRegAddrLSBs, &dwResult) == TRUE)
	{
		dwValue = (dwResult & RT_RegMap[dwReg].dwMaskLSBs) |
			(dwData << RT_RegMap[dwReg].dwFldOffsetLSBs);

		if (RT_regw (RT_RegMap[dwReg].dwRegAddrLSBs, dwValue) == TRUE)
		{
			/* update the memory mapped registers */
			RT_RegMap[dwReg].dwCurrValue = dwData;
		}
	}

	return;

} /* WriteRT_fld ()... */

#if 0
/****************************************************************************
 * ReadRT_fld (uint32_t dwReg)                                                 *
 *  Function: Reads a register field within Rage Theatre                    *
 *    Inputs: uint32_t dwReg = register field to be read                       *
 *   Outputs: uint32_t - value read from register field                        *
 ****************************************************************************/
static uint32_t ReadRT_fld1 (TheatrePtr t,uint32_t dwReg)
{
	uint32_t dwResult=0;

	if (RT_regr (RT_RegMap[dwReg].dwRegAddrLSBs, &dwResult) == TRUE)
	{
		RT_RegMap[dwReg].dwCurrValue = ((dwResult & ~RT_RegMap[dwReg].dwMaskLSBs) >>
                                                            RT_RegMap[dwReg].dwFldOffsetLSBs);
		return (RT_RegMap[dwReg].dwCurrValue);
	}
	else
	{
		return (0xFFFFFFFF);
	}

} /* ReadRT_fld ()... */

#define ReadRT_fld(a)	   ReadRT_fld1(t,(a))
#endif

#define WriteRT_fld(a,b)   WriteRT_fld1(t, (a), (b))


/****************************************************************************
 * RT_SetTint (int hue)                                                     *
 *  Function: sets the tint (hue) for the Rage Theatre video in             *
 *    Inputs: int hue - the hue value to be set.                            *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetTint (TheatrePtr t, int hue)
{
    /* Validate Hue level */
    if (hue < -1000)
    {
        hue = -1000;
    }
    else if (hue > 1000)
    {
        hue = 1000;
    }

    t->iHue=hue;
	
	dsp_set_tint(t, (uint8_t)((hue*255)/2000 + 128));

} /* RT_SetTint ()... */


/****************************************************************************
 * RT_SetSaturation (int Saturation)                                        *
 *  Function: sets the saturation level for the Rage Theatre video in       *
 *    Inputs: int Saturation - the saturation value to be set.              *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetSaturation (TheatrePtr t, int Saturation)
{
    /* VALIDATE SATURATION LEVEL */
    if (Saturation < -1000L)
    {
        Saturation = -1000;
    }
    else if (Saturation > 1000L)
    {
        Saturation = 1000;
    }

    t->iSaturation = Saturation;

	/* RT200 has saturation in range 0 to 255 with nominal value 128 */
	dsp_set_saturation(t, (uint8_t)((Saturation*255)/2000 + 128));

	return;
} /* RT_SetSaturation ()...*/

/****************************************************************************
 * RT_SetBrightness (int Brightness)                                        *
 *  Function: sets the brightness level for the Rage Theatre video in       *
 *    Inputs: int Brightness - the brightness value to be set.              *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetBrightness (TheatrePtr t, int Brightness)
{
    /* VALIDATE BRIGHTNESS LEVEL */
    if (Brightness < -1000)
    {
        Brightness = -1000;
    }
    else if (Brightness > 1000)
    {
        Brightness = 1000;
    }

    /* Save value */
    t->iBrightness = Brightness;
    t->dbBrightnessRatio =  (double) (Brightness+1000.0) / 10.0;

	 /* RT200 is having brightness level from 0 to 255  with 128 nominal value */
	 dsp_set_brightness(t, (uint8_t)((Brightness*255)/2000 + 128));

	 return;
} /* RT_SetBrightness ()... */


/****************************************************************************
 * RT_SetSharpness (uint16_t wSharpness)                                        *
 *  Function: sets the sharpness level for the Rage Theatre video in        *
 *    Inputs: uint16_t wSharpness - the sharpness value to be set.              *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetSharpness (TheatrePtr t, uint16_t wSharpness)
{
	switch (wSharpness)
	{
		case DEC_SMOOTH :
			WriteRT_fld (fld_H_SHARPNESS, RT_NORM_SHARPNESS);
			t->wSharpness = RT_NORM_SHARPNESS;
			break;
		case DEC_SHARP  :
			WriteRT_fld (fld_H_SHARPNESS, RT_HIGH_SHARPNESS);
			t->wSharpness = RT_HIGH_SHARPNESS;
			break;
		default:
			break;
	}
	return;

} /* RT_SetSharpness ()... */


/****************************************************************************
 * RT_SetContrast (int Contrast)                                            *
 *  Function: sets the contrast level for the Rage Theatre video in         *
 *    Inputs: int Contrast - the contrast value to be set.                  *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetContrast (TheatrePtr t, int Contrast)
{
	/* VALIDATE CONTRAST LEVEL */
	if (Contrast < -1000)
	{
		Contrast = -1000;
    }
    else if (Contrast > 1000)
    {
        Contrast = 1000;
    }

    /* Save contrast value */
    t->iContrast = Contrast;
    t->dbContrast = (double) (Contrast+1000.0) / 1000.0;
	 
	/* RT200 has contrast values between 0 to 255 with nominal value at 128 */
	dsp_set_contrast(t, (uint8_t)((Contrast*255)/2000 + 128));
	return;

} /* RT_SetContrast ()... */

/****************************************************************************
 * RT_SetInterlace (uint8_t bInterlace)                                        *
 *  Function: to set the interlacing pattern for the Rage Theatre video in  *
 *    Inputs: uint8_t bInterlace                                               *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetInterlace (TheatrePtr t, uint8_t bInterlace)
{
	switch(bInterlace)
	{
		case (TRUE):    /*DEC_INTERLACE */
			WriteRT_fld (fld_V_DEINTERLACE_ON, 0x1);
			t->wInterlaced = (uint16_t) RT_DECINTERLACED;
			break;
		case (FALSE):    /*DEC_NONINTERLACE */
			WriteRT_fld (fld_V_DEINTERLACE_ON, RT_DECNONINTERLACED);
			t->wInterlaced = (uint16_t) RT_DECNONINTERLACED;
			break;
	   default:
			break;
	}

	return;

} /* RT_SetInterlace ()... */


/****************************************************************************
 * RT_SetStandard (uint16_t wStandard)                                          *
 *  Function: to set the input standard for the Rage Theatre video in       *
 *    Inputs: uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetStandard (TheatrePtr t, uint16_t wStandard)
{
	xf86DrvMsg(t->VIP->scrnIndex,X_INFO,"Rage Theatre setting standard 0x%04x\n",
		wStandard);
	
	t->wStandard = wStandard;

	/* Program the new standards: */
	switch (wStandard & 0x00FF)
	{
		case (DEC_NTSC): /*NTSC GROUP - 480 lines */
			switch (wStandard & 0xFF00)
			{
				case (extNONE):
				case (extNTSC):
					dsp_set_video_standard(t, 2);
					break;
				case (extNTSC_J):
					dsp_set_video_standard(t, RT200_NTSC_J);
					break;
				case (extNTSC_443):
					dsp_set_video_standard(t, RT200_NTSC_433);
					break;
				default:
					dsp_video_standard_detection(t);
					break;
			}
			break;
		case (DEC_PAL):  /*PAL GROUP  - 625 lines */
			switch (wStandard & 0xFF00)
			{
				case (extNONE):
				case (extPAL):
				case (extPAL_B):
				case (extPAL_BGHI):
					dsp_set_video_standard(t, RT200_PAL_B);
					break;
				case (extPAL_D):
					dsp_set_video_standard(t, RT200_PAL_D);
					break;
				case (extPAL_G):
					dsp_set_video_standard(t, RT200_PAL_G);
					break;
				case (extPAL_H):
					dsp_set_video_standard(t, RT200_PAL_H);
					break;
				case (extPAL_I):
					dsp_set_video_standard(t, RT200_PAL_D);
					break;
				case (extPAL_N):
					dsp_set_video_standard(t, RT200_PAL_N);
					break;
				case (extPAL_NCOMB):
					dsp_set_video_standard(t, RT200_PAL_Ncomb);
					break;
				case (extPAL_M):
					dsp_set_video_standard(t, RT200_PAL_M);
					break;
				case (extPAL_60):
					dsp_set_video_standard(t, RT200_PAL_60);
					break;
				default:
					dsp_video_standard_detection(t);
					break;
				}
				break;
		  case (DEC_SECAM):  /*SECAM GROUP*/
				switch (wStandard & 0xFF00)
				{
					case (extNONE):
					case (extSECAM):
						dsp_set_video_standard(t, RT200_SECAM);
						break;
					case (extSECAM_B):
						dsp_set_video_standard(t, RT200_SECAM_B);
						break;
					case (extSECAM_D):
						dsp_set_video_standard(t, RT200_SECAM_D);
						break;
					case (extSECAM_G):
						dsp_set_video_standard(t, RT200_SECAM_G);
						break;
					case (extSECAM_H):
						dsp_set_video_standard(t, RT200_SECAM_H);
						break;
					case (extSECAM_K):
						dsp_set_video_standard(t, RT200_SECAM_K);
						break;
					case (extSECAM_K1):
						dsp_set_video_standard(t, RT200_SECAM_K1);
						break;
					case (extSECAM_L):
						dsp_set_video_standard(t, RT200_SECAM_L);
						break;
					case (extSECAM_L1):
						dsp_set_video_standard(t, RT200_SECAM_L1);
						break;
					default:
						dsp_video_standard_detection(t);
						break;
				}
				break;
		  default:
				dsp_video_standard_detection(t);
	}
    	
} /* RT_SetStandard ()... */


/****************************************************************************
 * RT_SetOutputVideoSize (uint16_t wHorzSize, uint16_t wVertSize,                   *
 *                          uint8_t fCC_On, uint8_t fVBICap_On)                   *
 *  Function: sets the output video size for the Rage Theatre video in      *
 *    Inputs: uint16_t wHorzSize - width of output in pixels                    *
 *            uint16_t wVertSize - height of output in pixels (lines)           *
 *            uint8_t fCC_On - enable CC output                                *
 *            uint8_t fVBI_Cap_On - enable VBI capture                         *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetOutputVideoSize (TheatrePtr t, uint16_t wHorzSize, uint16_t wVertSize, uint8_t fCC_On, uint8_t fVBICap_On)
{
	/* VBI is ignored now */

	/* 
	 * If I pass the (wHorzSize, 0, 0) (wVertSize, 0, 0) the image does not synchronize
	 */
	dsp_set_video_scaler_horizontal(t, 0, 0, 0);
	dsp_set_video_scaler_vertical(t, 0, 0, 0);

} /* RT_SetOutputVideoSize ()...*/


/****************************************************************************
 * RT_SetConnector (uint16_t wStandard, int tunerFlag)                          *
 *  Function:
 *    Inputs: uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *            int tunerFlag
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetConnector (TheatrePtr t, uint16_t wConnector, int tunerFlag)
{
	uint32_t data;

	t->wConnector = wConnector;

	theatre_read(t, VIP_GPIO_CNTL, &data);
	xf86DrvMsg(t->VIP->scrnIndex,X_INFO,"VIP_GPIO_CNTL: %x\n",
		   (unsigned)data);

	theatre_read(t, VIP_GPIO_INOUT, &data);
	xf86DrvMsg(t->VIP->scrnIndex,X_INFO,"VIP_GPIO_INOUT: %x\n",
		   (unsigned)data);
	
	switch (wConnector)
	{
		case (DEC_TUNER):   /* Tuner*/
			/* RT200 does not have any input connector 0 */
			dsp_set_video_input_connector(t, t->wTunerConnector + 1);
				 
			/* this is to set the analog mux used for sond */
			theatre_read(t, VIP_GPIO_CNTL, &data);
			data &= ~0x10;
			theatre_write(t, VIP_GPIO_CNTL, data);
	 
			theatre_read(t, VIP_GPIO_INOUT, &data);
			data &= ~0x10;
			theatre_write(t, VIP_GPIO_INOUT, data);

			break;
		case (DEC_COMPOSITE):   /* Comp*/
			dsp_set_video_input_connector(t, t->wComp0Connector);
					 
			/* this is to set the analog mux used for sond */
			theatre_read(t, VIP_GPIO_CNTL, &data);
			data |= 0x10;
			theatre_write(t, VIP_GPIO_CNTL, data);
	 
			theatre_read(t, VIP_GPIO_INOUT, &data);
			data |= 0x10;
			theatre_write(t, VIP_GPIO_INOUT, data);

			break;
		  case (DEC_SVIDEO):  /* Svideo*/
			dsp_set_video_input_connector(t, t->wSVideo0Connector);
					 
			/* this is to set the analog mux used for sond */
			theatre_read(t, VIP_GPIO_CNTL, &data);
			data |= 0x10;
			theatre_write(t, VIP_GPIO_CNTL, data);
	 
			theatre_read(t, VIP_GPIO_INOUT, &data);
			data |= 0x10;
			theatre_write(t, VIP_GPIO_INOUT, data);

			break;
		  default:
			dsp_set_video_input_connector(t, t->wComp0Connector);
	}

	theatre_read(t, VIP_GPIO_CNTL, &data);
	xf86DrvMsg(t->VIP->scrnIndex,X_INFO,"VIP_GPIO_CNTL: %x\n",
		   (unsigned)data);

	theatre_read(t, VIP_GPIO_INOUT, &data);
	xf86DrvMsg(t->VIP->scrnIndex,X_INFO,"VIP_GPIO_INOUT: %x\n",
		   (unsigned)data);


	dsp_configure_i2s_port(t, 0, 0, 0);
	dsp_configure_spdif_port(t, 0);

	/*dsp_audio_detection(t, 0);*/
	dsp_audio_mute(t, 1, 1);
	dsp_set_audio_volume(t, 128, 128, 0);

} /* RT_SetConnector ()...*/


_X_EXPORT void InitTheatre(TheatrePtr t)
{
	uint32_t data;
	uint32_t M, N, P;

	/* this will give 108Mhz at 27Mhz reference */
	M = 28;
	N = 224;
	P = 1;

	ShutdownTheatre(t);
	usleep(100000);
	t->mode=MODE_INITIALIZATION_IN_PROGRESS;


	data = M | (N << 11) | (P <<24);
	RT_regw(VIP_DSP_PLL_CNTL, data);

	RT_regr(VIP_PLL_CNTL0, &data);
	data |= 0x2000;
	RT_regw(VIP_PLL_CNTL0, data);

	/* RT_regw(VIP_I2C_SLVCNTL, 0x249); */
	RT_regr(VIP_PLL_CNTL1, &data);
	data |= 0x00030003;
	RT_regw(VIP_PLL_CNTL1, data);

	RT_regr(VIP_PLL_CNTL0, &data);
	data &= 0xfffffffc;
	RT_regw(VIP_PLL_CNTL0, data);
	usleep(15000);

	RT_regr(VIP_CLOCK_SEL_CNTL, &data);
	data |= 0x1b;
	RT_regw(VIP_CLOCK_SEL_CNTL, data);

	RT_regr(VIP_MASTER_CNTL, &data);
	data &= 0xffffff07;
	RT_regw(VIP_MASTER_CNTL, data);
	data &= 0xffffff03;
	RT_regw(VIP_MASTER_CNTL, data);
	usleep(1000);
	 
	if (t->microc_path == NULL)
	{
		t->microc_path = DEFAULT_MICROC_PATH;
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: Use default microcode path: %s\n", DEFAULT_MICROC_PATH);
	}
	else
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: Use microcode path: %s\n", t->microc_path);


	if (t->microc_type == NULL)
	{
		t->microc_type = DEFAULT_MICROC_TYPE;
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: Use default microcode type: %s\n", DEFAULT_MICROC_TYPE);
	}
	else
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: Use microcode type: %s\n", t->microc_type);

	if (DownloadMicrocode(t) < 0)
	{
		ShutdownTheatre(t);
		return;
	}
	
	dsp_set_lowpowerstate(t, 1);
	dsp_set_videostreamformat(t, 1);

	t->mode=MODE_INITIALIZED_FOR_TV_IN;
}

static int DownloadMicrocode(TheatrePtr t)
{
	struct rt200_microc_data microc_data;
	microc_data.microc_seg_list = NULL;

	if (microc_load(t->microc_path, t->microc_type, &microc_data, t->VIP->scrnIndex) < 0)
	{
		xf86DrvMsg(t->VIP->scrnIndex, X_ERROR, "Microcode: cannot load microcode\n");
		goto err_exit;
	}
	else
	{
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: device_id: %x\n", microc_data.microc_head.device_id);
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: vendor_id: %x\n", microc_data.microc_head.vendor_id);
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: rev_id: %x\n", microc_data.microc_head.revision_id);
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: num_seg: %x\n", microc_data.microc_head.num_seg);
	}
	
	if (dsp_init(t, &microc_data) < 0)
	{
		xf86DrvMsg(t->VIP->scrnIndex, X_ERROR, "Microcode: dsp_init failed\n");
		goto err_exit;
	}
	else
	{
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: dsp_init OK\n");
	}

	if (dsp_load(t, &microc_data) < 0)
	{
		xf86DrvMsg(t->VIP->scrnIndex, X_ERROR, "Microcode: dsp_download failed\n");
		goto err_exit;
	}
	else
	{
		xf86DrvMsg(t->VIP->scrnIndex, X_INFO, "Microcode: dsp_download OK\n");
	}

	microc_clean(&microc_data, t->VIP->scrnIndex);
	return 0;

err_exit:

	microc_clean(&microc_data, t->VIP->scrnIndex);
	return -1;
				
}


_X_EXPORT void ShutdownTheatre(TheatrePtr t)
{
#if 0
    WriteRT_fld (fld_VIN_ASYNC_RST, RT_ASYNC_DISABLE);
    WriteRT_fld (fld_VINRST       , RT_VINRST_RESET);
    WriteRT_fld (fld_ADC_PDWN     , RT_ADC_DISABLE);
    WriteRT_fld (fld_DVS_DIRECTION, RT_DVSDIR_IN);
#endif
    t->mode=MODE_UNINITIALIZED;
}

_X_EXPORT void DumpRageTheatreRegs(TheatrePtr t)
{
    int i;
    uint32_t data;
    
    for(i=0;i<0x900;i+=4)
    {
       RT_regr(i, &data);
       xf86DrvMsg(t->VIP->scrnIndex, X_INFO,
		  "register 0x%04x is equal to 0x%08x\n", i, (unsigned)data);
    }   

}

void DumpRageTheatreRegsByName(TheatrePtr t)
{
    int i;
    uint32_t data;
    struct { char *name; long addr; } rt_reg_list[]={
    { "ADC_CNTL                ", 0x0400 },
    { "ADC_DEBUG               ", 0x0404 },
    { "AUD_CLK_DIVIDERS        ", 0x00e8 },
    { "AUD_DTO_INCREMENTS      ", 0x00ec },
    { "AUD_PLL_CNTL            ", 0x00e0 },
    { "AUD_PLL_FINE_CNTL       ", 0x00e4 },
    { "CLKOUT_CNTL             ", 0x004c },
    { "CLKOUT_GPIO_CNTL        ", 0x0038 },
    { "CLOCK_SEL_CNTL          ", 0x00d0 },
    { "COMB_CNTL0              ", 0x0440 },
    { "COMB_CNTL1              ", 0x0444 },
    { "COMB_CNTL2              ", 0x0448 },
    { "COMB_LINE_LENGTH        ", 0x044c },
    { "CP_ACTIVE_GAIN          ", 0x0594 },
    { "CP_AGC_CNTL             ", 0x0590 },
    { "CP_BURST_GAIN           ", 0x058c },
    { "CP_DEBUG_FORCE          ", 0x05b8 },
    { "CP_HUE_CNTL             ", 0x0588 },
    { "CP_PLL_CNTL0            ", 0x0580 },
    { "CP_PLL_CNTL1            ", 0x0584 },
    { "CP_PLL_STATUS0          ", 0x0598 },
    { "CP_PLL_STATUS1          ", 0x059c },
    { "CP_PLL_STATUS2          ", 0x05a0 },
    { "CP_PLL_STATUS3          ", 0x05a4 },
    { "CP_PLL_STATUS4          ", 0x05a8 },
    { "CP_PLL_STATUS5          ", 0x05ac },
    { "CP_PLL_STATUS6          ", 0x05b0 },
    { "CP_PLL_STATUS7          ", 0x05b4 },
    { "CP_VERT_LOCKOUT         ", 0x05bc },
    { "CRC_CNTL                ", 0x02c0 },
    { "CRT_DTO_INCREMENTS      ", 0x0394 },
    { "CRT_PLL_CNTL            ", 0x00c4 },
    { "CRT_PLL_FINE_CNTL       ", 0x00bc },
    { "DECODER_DEBUG_CNTL      ", 0x05d4 },
    { "DELAY_ONE_MAP_A         ", 0x0114 },
    { "DELAY_ONE_MAP_B         ", 0x0118 },
    { "DELAY_ZERO_MAP_A        ", 0x011c },
    { "DELAY_ZERO_MAP_B        ", 0x0120 },
    { "DFCOUNT                 ", 0x00a4 },
    { "DFRESTART               ", 0x00a8 },
    { "DHRESTART               ", 0x00ac },
    { "DVRESTART               ", 0x00b0 },
    { "DVS_PORT_CTRL           ", 0x0610 },
    { "DVS_PORT_READBACK       ", 0x0614 },
    { "FIFOA_CONFIG            ", 0x0800 },
    { "FIFOB_CONFIG            ", 0x0804 },
    { "FIFOC_CONFIG            ", 0x0808 },
    { "FRAME_LOCK_CNTL         ", 0x0100 },
    { "GAIN_LIMIT_SETTINGS     ", 0x01e4 },
    { "GPIO_CNTL               ", 0x0034 },
    { "GPIO_INOUT              ", 0x0030 },
    { "HCOUNT                  ", 0x0090 },
    { "HDISP                   ", 0x0084 },
    { "HOST_RD_WT_CNTL         ", 0x0188 },
    { "HOST_READ_DATA          ", 0x0180 },
    { "HOST_WRITE_DATA         ", 0x0184 },
    { "HSIZE                   ", 0x0088 },
    { "HSTART                  ", 0x008c },
    { "HS_DTOINC               ", 0x0484 },
    { "HS_GENLOCKDELAY         ", 0x0490 },
    { "HS_MINMAXWIDTH          ", 0x048c },
    { "HS_PLINE                ", 0x0480 },
    { "HS_PLLGAIN              ", 0x0488 },
    { "HS_PLL_ERROR            ", 0x04a0 },
    { "HS_PLL_FS_PATH          ", 0x04a4 },
    { "HS_PULSE_WIDTH          ", 0x049c },
    { "HS_WINDOW_LIMIT         ", 0x0494 },
    { "HS_WINDOW_OC_SPEED      ", 0x0498 },
    { "HTOTAL                  ", 0x0080 },
    { "HW_DEBUG                ", 0x0010 },
    { "H_ACTIVE_WINDOW         ", 0x05c0 },
    { "H_SCALER_CONTROL        ", 0x0600 },
    { "H_VBI_WINDOW            ", 0x05c8 },
    { "I2C_CNTL                ", 0x0054 },
    { "I2C_CNTL_0              ", 0x0020 },
    { "I2C_CNTL_1              ", 0x0024 },
    { "I2C_DATA                ", 0x0028 },
    { "I2S_RECEIVE_CNTL        ", 0x081c },
    { "I2S_TRANSMIT_CNTL       ", 0x0818 },
    { "IIS_TX_CNT_REG          ", 0x0824 },
    { "INT_CNTL                ", 0x002c },
    { "L54_DTO_INCREMENTS      ", 0x00f8 },
    { "L54_PLL_CNTL            ", 0x00f0 },
    { "L54_PLL_FINE_CNTL       ", 0x00f4 },
    { "LINEAR_GAIN_SETTINGS    ", 0x01e8 },
    { "LP_AGC_CLAMP_CNTL0      ", 0x0500 },
    { "LP_AGC_CLAMP_CNTL1      ", 0x0504 },
    { "LP_BLACK_LEVEL          ", 0x051c },
    { "LP_BRIGHTNESS           ", 0x0508 },
    { "LP_CONTRAST             ", 0x050c },
    { "LP_SLICE_LEVEL          ", 0x0520 },
    { "LP_SLICE_LIMIT          ", 0x0510 },
    { "LP_SYNCTIP_LEVEL        ", 0x0524 },
    { "LP_VERT_LOCKOUT         ", 0x0528 },
    { "LP_WPA_CNTL0            ", 0x0514 },
    { "LP_WPA_CNTL1            ", 0x0518 },
    { "MASTER_CNTL             ", 0x0040 },
    { "MODULATOR_CNTL1         ", 0x0200 },
    { "MODULATOR_CNTL2         ", 0x0204 },
    { "MV_LEVEL_CNTL1          ", 0x0210 },
    { "MV_LEVEL_CNTL2          ", 0x0214 },
    { "MV_MODE_CNTL            ", 0x0208 },
    { "MV_STATUS               ", 0x0330 },
    { "MV_STRIPE_CNTL          ", 0x020c },
    { "NOISE_CNTL0             ", 0x0450 },
    { "PLL_CNTL0               ", 0x00c8 },
    { "PLL_CNTL1               ", 0x00fc },
    { "PLL_TEST_CNTL           ", 0x00cc },
    { "PRE_DAC_MUX_CNTL        ", 0x0240 },
    { "RGB_CNTL                ", 0x0048 },
    { "RIPINTF_PORT_CNTL       ", 0x003c },
    { "SCALER_IN_WINDOW        ", 0x0618 },
    { "SCALER_OUT_WINDOW       ", 0x061c },
    { "SG_BLACK_GATE           ", 0x04c0 },
    { "SG_SYNCTIP_GATE         ", 0x04c4 },
    { "SG_UVGATE_GATE          ", 0x04c8 },
    { "SINGLE_STEP_DATA        ", 0x05d8 },
    { "SPDIF_AC3_PREAMBLE      ", 0x0814 },
    { "SPDIF_CHANNEL_STAT      ", 0x0810 },
    { "SPDIF_PORT_CNTL         ", 0x080c },
    { "SPDIF_TX_CNT_REG        ", 0x0820 },
    { "STANDARD_SELECT         ", 0x0408 },
    { "SW_SCRATCH              ", 0x0014 },
    { "SYNC_CNTL               ", 0x0050 },
    { "SYNC_LOCK_CNTL          ", 0x0104 },
    { "SYNC_SIZE               ", 0x00b4 },
    { "THERMO2BIN_STATUS       ", 0x040c },
    { "TIMING_CNTL             ", 0x01c4 },
    { "TVO_DATA_DELAY_A        ", 0x0140 },
    { "TVO_DATA_DELAY_B        ", 0x0144 },
    { "TVO_SYNC_PAT_ACCUM      ", 0x0108 },
    { "TVO_SYNC_PAT_EXPECT     ", 0x0110 },
    { "TVO_SYNC_THRESHOLD      ", 0x010c },
    { "TV_DAC_CNTL             ", 0x0280 },
    { "TV_DTO_INCREMENTS       ", 0x0390 },
    { "TV_PLL_CNTL             ", 0x00c0 },
    { "TV_PLL_FINE_CNTL        ", 0x00b8 },
    { "UPSAMP_AND_GAIN_CNTL    ", 0x01e0 },
    { "UPSAMP_COEFF0_0         ", 0x0340 },
    { "UPSAMP_COEFF0_1         ", 0x0344 },
    { "UPSAMP_COEFF0_2         ", 0x0348 },
    { "UPSAMP_COEFF1_0         ", 0x034c },
    { "UPSAMP_COEFF1_1         ", 0x0350 },
    { "UPSAMP_COEFF1_2         ", 0x0354 },
    { "UPSAMP_COEFF2_0         ", 0x0358 },
    { "UPSAMP_COEFF2_1         ", 0x035c },
    { "UPSAMP_COEFF2_2         ", 0x0360 },
    { "UPSAMP_COEFF3_0         ", 0x0364 },
    { "UPSAMP_COEFF3_1         ", 0x0368 },
    { "UPSAMP_COEFF3_2         ", 0x036c },
    { "UPSAMP_COEFF4_0         ", 0x0370 },
    { "UPSAMP_COEFF4_1         ", 0x0374 },
    { "UPSAMP_COEFF4_2         ", 0x0378 },
    { "UV_ADR                  ", 0x0300 },
    { "VBI_20BIT_CNTL          ", 0x02d0 },
    { "VBI_CC_CNTL             ", 0x02c8 },
    { "VBI_CONTROL             ", 0x05d0 },
    { "VBI_DTO_CNTL            ", 0x02d4 },
    { "VBI_EDS_CNTL            ", 0x02cc },
    { "VBI_LEVEL_CNTL          ", 0x02d8 },
    { "VBI_SCALER_CONTROL      ", 0x060c },
    { "VCOUNT                  ", 0x009c },
    { "VDISP                   ", 0x0098 },
    { "VFTOTAL                 ", 0x00a0 },
    { "VIDEO_PORT_SIG          ", 0x02c4 },
    { "VIN_PLL_CNTL            ", 0x00d4 },
    { "VIN_PLL_FINE_CNTL       ", 0x00d8 },
    { "VIP_COMMAND_STATUS      ", 0x0008 },
    { "VIP_REVISION_ID         ", 0x000c },
    { "VIP_SUB_VENDOR_DEVICE_ID", 0x0004 },
    { "VIP_VENDOR_DEVICE_ID    ", 0x0000 },
    { "VSCALER_CNTL1           ", 0x01c0 },
    { "VSCALER_CNTL2           ", 0x01c8 },
    { "VSYNC_DIFF_CNTL         ", 0x03a0 },
    { "VSYNC_DIFF_LIMITS       ", 0x03a4 },
    { "VSYNC_DIFF_RD_DATA      ", 0x03a8 },
    { "VS_BLANKING_CNTL        ", 0x0544 },
    { "VS_COUNTER_CNTL         ", 0x054c },
    { "VS_DETECTOR_CNTL        ", 0x0540 },
    { "VS_FIELD_ID_CNTL        ", 0x0548 },
    { "VS_FRAME_TOTAL          ", 0x0550 },
    { "VS_LINE_COUNT           ", 0x0554 },
    { "VTOTAL                  ", 0x0094 },
    { "V_ACTIVE_WINDOW         ", 0x05c4 },
    { "V_DEINTERLACE_CONTROL   ", 0x0608 },
    { "V_SCALER_CONTROL        ", 0x0604 },
    { "V_VBI_WINDOW            ", 0x05cc },
    { "Y_FALL_CNTL             ", 0x01cc },
    { "Y_RISE_CNTL             ", 0x01d0 },
    { "Y_SAW_TOOTH_CNTL        ", 0x01d4 },
    {NULL, 0}
    };

    for(i=0; rt_reg_list[i].name!=NULL;i++){
        RT_regr(rt_reg_list[i].addr, &data);
        xf86DrvMsg(t->VIP->scrnIndex, X_INFO,
		   "register (0x%04lx) %s is equal to 0x%08x\n",
		   rt_reg_list[i].addr, rt_reg_list[i].name, (unsigned)data);
    	}

}

_X_EXPORT void ResetTheatreRegsForNoTVout(TheatrePtr t)
{
     RT_regw(VIP_CLKOUT_CNTL, 0x0); 
     RT_regw(VIP_HCOUNT, 0x0); 
     RT_regw(VIP_VCOUNT, 0x0); 
     RT_regw(VIP_DFCOUNT, 0x0); 
     #if 0
     RT_regw(VIP_CLOCK_SEL_CNTL, 0x2b7);  /* versus 0x237 <-> 0x2b7 */
     RT_regw(VIP_VIN_PLL_CNTL, 0x60a6039);
     #endif
     RT_regw(VIP_FRAME_LOCK_CNTL, 0x0);
}


_X_EXPORT void ResetTheatreRegsForTVout(TheatrePtr t)
{
/*    RT_regw(VIP_HW_DEBUG, 0x200);   */
/*     RT_regw(VIP_INT_CNTL, 0x0); 
     RT_regw(VIP_GPIO_INOUT, 0x10090000); 
     RT_regw(VIP_GPIO_INOUT, 0x340b0000);  */
/*     RT_regw(VIP_MASTER_CNTL, 0x6e8);  */
     RT_regw(VIP_CLKOUT_CNTL, 0x29); 
#if 1
     RT_regw(VIP_HCOUNT, 0x1d1); 
     RT_regw(VIP_VCOUNT, 0x1e3); 
#else
     RT_regw(VIP_HCOUNT, 0x322); 
     RT_regw(VIP_VCOUNT, 0x151);
#endif
     RT_regw(VIP_DFCOUNT, 0x01); 
/*     RT_regw(VIP_CLOCK_SEL_CNTL, 0xb7);  versus 0x237 <-> 0x2b7 */
     RT_regw(VIP_CLOCK_SEL_CNTL, 0x2b7);  /* versus 0x237 <-> 0x2b7 */
     RT_regw(VIP_VIN_PLL_CNTL, 0x60a6039);
/*     RT_regw(VIP_PLL_CNTL1, 0xacacac74); */
     RT_regw(VIP_FRAME_LOCK_CNTL, 0x0f);
/*     RT_regw(VIP_ADC_CNTL, 0x02a420a8); 
     RT_regw(VIP_COMB_CNTL_0, 0x0d438083); 
     RT_regw(VIP_COMB_CNTL_2, 0x06080102); 
     RT_regw(VIP_HS_MINMAXWIDTH, 0x462f); 
     ...
     */
/*
     RT_regw(VIP_HS_PULSE_WIDTH, 0x359);
     RT_regw(VIP_HS_PLL_ERROR, 0xab6);
     RT_regw(VIP_HS_PLL_FS_PATH, 0x7fff08f8);
     RT_regw(VIP_VS_LINE_COUNT, 0x49b5e005);
	*/
}

