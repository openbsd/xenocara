/*
 * Copyright © 2009 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Zhenyu Wang <zhenyu.z.wang@intel.com>
 *    Wu Fengguang <fengguang.wu@intel.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <pciaccess.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#include "reg_dumper.h"
#include "../i810_reg.h"

#define dump_reg(reg, desc)					\
    do {							\
	    dword = INREG(reg);	  				\
	    printf("%-18s 0x%08x  %s\n", # reg, dword, desc);	\
    } while (0)

#define BITSTO(n)		(n >= sizeof(long) * 8 ? ~0 : (1UL << (n)) - 1)
#define BITMASK(high, low)	(BITSTO(high+1) & ~BITSTO(low))
#define BITS(reg, high, low)	(((reg) & (BITMASK(high, low))) >> (low))
#define BIT(reg, n)		BITS(reg, n, n)

#define AUD_CONFIG		0x62000
#define AUD_DEBUG		0x62010
#define AUD_VID_DID		0x62020
#define AUD_RID			0x62024
#define AUD_SUBN_CNT		0x62028
#define AUD_FUNC_GRP		0x62040
#define AUD_SUBN_CNT2		0x62044
#define AUD_GRP_CAP		0x62048
#define AUD_PWRST		0x6204c
#define AUD_SUPPWR		0x62050
#define AUD_SID			0x62054
#define AUD_OUT_CWCAP		0x62070
#define AUD_OUT_PCMSIZE		0x62074
#define AUD_OUT_STR		0x62078
#define AUD_OUT_DIG_CNVT	0x6207c
#define AUD_OUT_CH_STR		0x62080
#define AUD_OUT_STR_DESC	0x62084
#define AUD_PINW_CAP		0x620a0
#define AUD_PIN_CAP		0x620a4
#define AUD_PINW_CONNLNG	0x620a8
#define AUD_PINW_CONNLST	0x620ac
#define AUD_PINW_CNTR		0x620b0
#define AUD_PINW_UNSOLRESP	0x620b8
#define AUD_CNTL_ST		0x620b4
#define AUD_PINW_CONFIG		0x620bc
#define AUD_HDMIW_STATUS	0x620d4
#define AUD_HDMIW_HDMIEDID	0x6210c
#define AUD_HDMIW_INFOFR	0x62118
#define AUD_CONV_CHCNT 		0x62120
#define AUD_CTS_ENABLE		0x62128

#define VIDEO_DIP_CTL		0x61170
#define VIDEO_DIP_ENABLE	(1<<31)
#define VIDEO_DIP_ENABLE_AVI	(1<<21)
#define VIDEO_DIP_ENABLE_VENDOR	(1<<22)
#define VIDEO_DIP_ENABLE_SPD	(1<<24)
#define VIDEO_DIP_BUF_AVI	(0<<19)
#define VIDEO_DIP_BUF_VENDOR	(1<<19)
#define VIDEO_DIP_BUF_SPD	(3<<19)
#define VIDEO_DIP_TRANS_ONCE	(0<<16)
#define VIDEO_DIP_TRANS_1	(1<<16)
#define VIDEO_DIP_TRANS_2	(2<<16)

#define AUDIO_HOTPLUG_EN	(1<<24)

#define min_t(type, x, y) ({                    \
        type __min1 = (x);                      \
        type __min2 = (y);                      \
        __min1 < __min2 ? __min1: __min2; })

#define OPNAME(names, index)   \
               names[min_t(unsigned int, index, ARRAY_SIZE(names) - 1)]

static char *pixel_clock[] = {
	[0] = "25.2 / 1.001 MHz",
	[1] = "25.2 MHz",
	[2] = "27 MHz",
	[3] = "27 * 1.001 MHz",
	[4] = "54 MHz",
	[5] = "54 * 1.001 MHz",
	[6] = "74.25 / 1.001 MHz",
	[7] = "74.25 MHz",
	[8] = "148.5 / 1.001 MHz",
	[9] = "148.5 MHz",
	[10] = "Reserved",
};

static char *power_state[] = {
	[0] = "D0",
	[1] = "D1",
	[2] = "D2",
	[3] = "D3",
};

static char *stream_type[] = {
	[0] = "default samples",
	[1] = "one bit stream",
	[2] = "DST stream",
	[3] = "MLP stream",
	[4] = "Reserved",
};

static char *dip_port[] = {
	[0] = "Reserved",
	[1] = "HDMI B",
	[2] = "HDMI C",
	[3] = "Reserved",
};

static char *dip_index[] = {
	[0] = "Audio DIP",
	[1] = "ACP DIP",
	[2] = "ISRC1 DIP",
	[3] = "ISRC2 DIP",
	[4] = "Reserved",
};

static char *dip_trans[] = {
	[0] = "disabled",
	[1] = "reserved",
	[2] = "send once",
	[3] = "best effort",
};

static char *video_dip_index[] = {
	[0] = "AVI DIP",
	[1] = "Vendor-specific DIP",
	[2] = "Reserved",
	[3] = "Source Product Description DIP",
};

static char *video_dip_trans[] = {
	[0] = "send once",
	[1] = "send every vsync",
	[2] = "send at least every other vsync",
	[3] = "reserved",
};

static void do_self_tests(void)
{
	if (BIT(1, 0) != 1)
		exit(1);
	if (BIT(0x80000000, 31) != 1)
		exit(2);
	if (BITS(0xc0000000, 31, 30) != 3)
		exit(3);
}

int main(int argc, char **argv)
{
	I830Rec i830;
	I830Ptr pI830 = &i830;
	uint32_t dword;
	int i;

	do_self_tests();
	intel_i830rec_init(pI830);

	/* printf("%-18s   %8s  %s\n\n", "register name", "raw value", "description"); */

#if 0				/* enable HDMI audio bits */
	dump_reg(SDVOB, "Digital Display Port B Control Register");
	dword |= SDVO_ENABLE;
	dword |= SDVO_BORDER_ENABLE;
	dword |= SDVO_AUDIO_ENABLE;
	dword |= SDVO_NULL_PACKETS_DURING_VSYNC;
	OUTREG(SDVOB, dword);

	dump_reg(PORT_HOTPLUG_EN, "Hot Plug Detect Enable");
	OUTREG(PORT_HOTPLUG_EN, dword | AUDIO_HOTPLUG_EN);

	dump_reg(VIDEO_DIP_CTL, "Video DIP Control");
	dword &= ~(VIDEO_DIP_ENABLE_AVI |
		   VIDEO_DIP_ENABLE_VENDOR | VIDEO_DIP_ENABLE_SPD);
	OUTREG(VIDEO_DIP_CTL, dword);
	dword |= VIDEO_DIP_ENABLE;
	OUTREG(VIDEO_DIP_CTL, dword);
#endif

#if 0				/* disable HDMI audio bits */
	dump_reg(SDVOB, "Digital Display Port B Control Register");
	dword &= ~SDVO_AUDIO_ENABLE;
	dword &= ~SDVO_NULL_PACKETS_DURING_VSYNC;
	OUTREG(SDVOB, dword);
#endif

	dump_reg(VIDEO_DIP_CTL, "Video DIP Control");
	dump_reg(SDVOB, "Digital Display Port B Control Register");
	dump_reg(SDVOC, "Digital Display Port C Control Register");
	dump_reg(PORT_HOTPLUG_EN, "Hot Plug Detect Enable");

	dump_reg(AUD_CONFIG, "Audio Configuration");
	dump_reg(AUD_DEBUG, "Audio Debug");
	dump_reg(AUD_VID_DID, "Audio Vendor ID / Device ID");
	dump_reg(AUD_RID, "Audio Revision ID");
	dump_reg(AUD_SUBN_CNT, "Audio Subordinate Node Count");
	dump_reg(AUD_FUNC_GRP, "Audio Function Group Type");
	dump_reg(AUD_SUBN_CNT2, "Audio Subordinate Node Count");
	dump_reg(AUD_GRP_CAP, "Audio Function Group Capabilities");
	dump_reg(AUD_PWRST, "Audio Power State");
	dump_reg(AUD_SUPPWR, "Audio Supported Power States");
	dump_reg(AUD_SID, "Audio Root Node Subsystem ID");
	dump_reg(AUD_OUT_CWCAP, "Audio Output Converter Widget Capabilities");
	dump_reg(AUD_OUT_PCMSIZE, "Audio PCM Size and Rates");
	dump_reg(AUD_OUT_STR, "Audio Stream Formats");
	dump_reg(AUD_OUT_DIG_CNVT, "Audio Digital Converter");
	dump_reg(AUD_OUT_CH_STR, "Audio Channel ID and Stream ID");
	dump_reg(AUD_OUT_STR_DESC, "Audio Stream Descriptor Format");
	dump_reg(AUD_PINW_CAP, "Audio Pin Complex Widget Capabilities");
	dump_reg(AUD_PIN_CAP, "Audio Pin Capabilities");
	dump_reg(AUD_PINW_CONNLNG, "Audio Connection List Length");
	dump_reg(AUD_PINW_CONNLST, "Audio Connection List Entry");
	dump_reg(AUD_PINW_CNTR, "Audio Pin Widget Control");
	dump_reg(AUD_PINW_UNSOLRESP, "Audio Unsolicited Response Enable");
	dump_reg(AUD_CNTL_ST, "Audio Control State Register");
	dump_reg(AUD_PINW_CONFIG, "Audio Configuration Default");
	dump_reg(AUD_HDMIW_STATUS, "Audio HDMI Status");
	dump_reg(AUD_HDMIW_HDMIEDID, "Audio HDMI Data EDID Block");
	dump_reg(AUD_HDMIW_INFOFR, "Audio HDMI Widget Data Island Packet");
	dump_reg(AUD_CONV_CHCNT, "Audio Converter Channel Count");
	dump_reg(AUD_CTS_ENABLE, "Audio CTS Programming Enable");

	printf("\nDetails:\n\n");

	dword = INREG(AUD_VID_DID);
	printf("AUD_VID_DID vendor id\t\t\t0x%x\n", dword >> 16);
	printf("AUD_VID_DID device id\t\t\t0x%x\n", dword & 0xffff);

	dword = INREG(AUD_RID);
	printf("AUD_RID major revision\t\t\t0x%lx\n", BITS(dword, 23, 20));
	printf("AUD_RID minor revision\t\t\t0x%lx\n", BITS(dword, 19, 16));
	printf("AUD_RID revision id\t\t\t0x%lx\n", BITS(dword, 15, 8));
	printf("AUD_RID stepping id\t\t\t0x%lx\n", BITS(dword, 7, 0));

	dword = INREG(SDVOB);
	printf("SDVOB enable\t\t\t\t%u\n", !!(dword & SDVO_ENABLE));
	printf("SDVOB HDMI encoding\t\t\t%u\n", !!(dword & SDVO_ENCODING_HDMI));
	printf("SDVOB SDVO encoding\t\t\t%u\n", !!(dword & SDVO_ENCODING_SDVO));
	printf("SDVOB null packets\t\t\t%u\n",
	       !!(dword & SDVO_NULL_PACKETS_DURING_VSYNC));
	printf("SDVOB audio enabled\t\t\t%u\n", !!(dword & SDVO_AUDIO_ENABLE));

	dword = INREG(SDVOC);
	printf("SDVOC enable\t\t\t\t%u\n", !!(dword & SDVO_ENABLE));
	printf("SDVOC HDMI encoding\t\t\t%u\n", !!(dword & SDVO_ENCODING_HDMI));
	printf("SDVOC SDVO encoding\t\t\t%u\n", !!(dword & SDVO_ENCODING_SDVO));
	printf("SDVOC null packets\t\t\t%u\n",
	       !!(dword & SDVO_NULL_PACKETS_DURING_VSYNC));
	printf("SDVOC audio enabled\t\t\t%u\n", !!(dword & SDVO_AUDIO_ENABLE));

	dword = INREG(PORT_HOTPLUG_EN);
	printf("PORT_HOTPLUG_EN DisplayPort/HDMI port B\t%ld\n",
	       BIT(dword, 29)),
	    printf("PORT_HOTPLUG_EN DisplayPort/HDMI port C\t%ld\n",
		   BIT(dword, 28)),
	    printf("PORT_HOTPLUG_EN DisplayPort port D\t%ld\n", BIT(dword, 27)),
	    printf("PORT_HOTPLUG_EN SDVOB\t\t\t%ld\n", BIT(dword, 26)),
	    printf("PORT_HOTPLUG_EN SDVOC\t\t\t%ld\n", BIT(dword, 25)),
	    printf("PORT_HOTPLUG_EN audio\t\t\t%ld\n", BIT(dword, 24)),
	    printf("PORT_HOTPLUG_EN TV\t\t\t%ld\n", BIT(dword, 23)),
	    printf("PORT_HOTPLUG_EN CRT\t\t\t%ld\n", BIT(dword, 9)), dword =
	    INREG(VIDEO_DIP_CTL);
	printf("VIDEO_DIP_CTL enable graphics DIP\t%ld\n", BIT(dword, 31)),
	    printf("VIDEO_DIP_CTL port select\t\t[0x%lx] %s\n",
		   BITS(dword, 30, 29), dip_port[BITS(dword, 30, 29)]);
	printf("VIDEO_DIP_CTL DIP buffer trans active\t%lu\n", BIT(dword, 28));
	printf("VIDEO_DIP_CTL AVI DIP enabled\t\t%lu\n", BIT(dword, 21));
	printf("VIDEO_DIP_CTL vendor DIP enabled\t%lu\n", BIT(dword, 22));
	printf("VIDEO_DIP_CTL SPD DIP enabled\t\t%lu\n", BIT(dword, 24));
	printf("VIDEO_DIP_CTL DIP buffer index\t\t[0x%lx] %s\n",
	       BITS(dword, 20, 19), video_dip_index[BITS(dword, 20, 19)]);
	printf("VIDEO_DIP_CTL DIP trans freq\t\t[0x%lx] %s\n",
	       BITS(dword, 17, 16), video_dip_trans[BITS(dword, 17, 16)]);
	printf("VIDEO_DIP_CTL DIP buffer size\t\t%lu\n", BITS(dword, 11, 8));
	printf("VIDEO_DIP_CTL DIP address\t\t%lu\n", BITS(dword, 3, 0));

	dword = INREG(AUD_CONFIG);
	printf("AUD_CONFIG pixel clock\t\t\t[0x%lx] %s\n", BITS(dword, 19, 16),
	       OPNAME(pixel_clock, BITS(dword, 19, 16)));
	printf("AUD_CONFIG fabrication enabled\t\t%lu\n", BITS(dword, 2, 2));
	printf("AUD_CONFIG professional use allowed\t%lu\n", BIT(dword, 1));
	printf("AUD_CONFIG fuse enabled\t\t\t%lu\n", BIT(dword, 0));

	dword = INREG(AUD_DEBUG);
	printf("AUD_DEBUG function reset\t\t%lu\n", BIT(dword, 0));

	dword = INREG(AUD_SUBN_CNT);
	printf("AUD_SUBN_CNT starting node number\t0x%lx\n",
	       BITS(dword, 23, 16));
	printf("AUD_SUBN_CNT total number of nodes\t0x%lx\n",
	       BITS(dword, 7, 0));

	dword = INREG(AUD_SUBN_CNT2);
	printf("AUD_SUBN_CNT2 starting node number\t0x%lx\n",
	       BITS(dword, 24, 16));
	printf("AUD_SUBN_CNT2 total number of nodes\t0x%lx\n",
	       BITS(dword, 7, 0));

	dword = INREG(AUD_FUNC_GRP);
	printf("AUD_FUNC_GRP unsol capable\t\t%lu\n", BIT(dword, 8));
	printf("AUD_FUNC_GRP node type\t\t\t0x%lx\n", BITS(dword, 7, 0));

	dword = INREG(AUD_GRP_CAP);
	printf("AUD_GRP_CAP beep 0\t\t\t%lu\n", BIT(dword, 16));
	printf("AUD_GRP_CAP input delay\t\t\t%lu\n", BITS(dword, 11, 8));
	printf("AUD_GRP_CAP output delay\t\t%lu\n", BITS(dword, 3, 0));

	dword = INREG(AUD_PWRST);
	printf("AUD_PWRST device power state\t\t%s\n",
	       power_state[BITS(dword, 5, 4)]);
	printf("AUD_PWRST device power state setting\t%s\n",
	       power_state[BITS(dword, 1, 0)]);

	dword = INREG(AUD_SUPPWR);
	printf("AUD_SUPPWR support D0\t\t\t%lu\n", BIT(dword, 0));
	printf("AUD_SUPPWR support D1\t\t\t%lu\n", BIT(dword, 1));
	printf("AUD_SUPPWR support D2\t\t\t%lu\n", BIT(dword, 2));
	printf("AUD_SUPPWR support D3\t\t\t%lu\n", BIT(dword, 3));

	dword = INREG(AUD_OUT_CWCAP);
	printf("AUD_OUT_CWCAP widget type\t\t0x%lx\n", BITS(dword, 23, 20));
	printf("AUD_OUT_CWCAP sample delay\t\t0x%lx\n", BITS(dword, 19, 16));
	printf("AUD_OUT_CWCAP channel count\t\t%lu\n",
	       BITS(dword, 15, 13) * 2 + BIT(dword, 0) + 1);
	printf("AUD_OUT_CWCAP L-R swap\t\t\t%lu\n", BIT(dword, 11));
	printf("AUD_OUT_CWCAP power control\t\t%lu\n", BIT(dword, 10));
	printf("AUD_OUT_CWCAP digital\t\t\t%lu\n", BIT(dword, 9));
	printf("AUD_OUT_CWCAP conn list\t\t\t%lu\n", BIT(dword, 8));
	printf("AUD_OUT_CWCAP unsol\t\t\t%lu\n", BIT(dword, 7));
	printf("AUD_OUT_CWCAP mute\t\t\t%lu\n", BIT(dword, 5));
	printf("AUD_OUT_CWCAP format override\t\t%lu\n", BIT(dword, 4));
	printf("AUD_OUT_CWCAP amp param override\t%lu\n", BIT(dword, 3));
	printf("AUD_OUT_CWCAP out amp present\t\t%lu\n", BIT(dword, 2));
	printf("AUD_OUT_CWCAP in amp present\t\t%lu\n", BIT(dword, 1));

	dword = INREG(AUD_OUT_DIG_CNVT);
	printf("AUD_OUT_DIG_CNVT SPDIF category\t\t0x%lx\n",
	       BITS(dword, 14, 8));
	printf("AUD_OUT_DIG_CNVT SPDIF level\t\t%lu\n", BIT(dword, 7));
	printf("AUD_OUT_DIG_CNVT professional\t\t%lu\n", BIT(dword, 6));
	printf("AUD_OUT_DIG_CNVT non PCM\t\t%lu\n", BIT(dword, 5));
	printf("AUD_OUT_DIG_CNVT copyright asserted\t%lu\n", BIT(dword, 4));
	printf("AUD_OUT_DIG_CNVT filter preemphasis\t%lu\n", BIT(dword, 3));
	printf("AUD_OUT_DIG_CNVT validity config\t%lu\n", BIT(dword, 2));
	printf("AUD_OUT_DIG_CNVT validity flag\t\t%lu\n", BIT(dword, 1));
	printf("AUD_OUT_DIG_CNVT digital enable\t\t%lu\n", BIT(dword, 0));

	dword = INREG(AUD_OUT_CH_STR);
	printf("AUD_OUT_CH_STR stream id\t\t0x%lx\n", BITS(dword, 7, 4));
	printf("AUD_OUT_CH_STR lowest channel\t\t0x%lx\n", BITS(dword, 3, 0));

	dword = INREG(AUD_OUT_STR_DESC);
	printf("AUD_OUT_STR_DESC stream channels\t0x%lx\n", BITS(dword, 3, 0));

	dword = INREG(AUD_PINW_CAP);
	printf("AUD_PINW_CAP widget type\t\t0x%lx\n", BITS(dword, 23, 20));
	printf("AUD_PINW_CAP sample delay\t\t0x%lx\n", BITS(dword, 19, 16));
	printf("AUD_PINW_CAP channel count\t\t0x%lx\n",
	       BITS(dword, 15, 13) * 2 + BIT(dword, 0));
	printf("AUD_PINW_CAP HDCP\t\t\t%lu\n", BIT(dword, 12));
	printf("AUD_PINW_CAP L-R swap\t\t\t%lu\n", BIT(dword, 11));
	printf("AUD_PINW_CAP power control\t\t%lu\n", BIT(dword, 10));
	printf("AUD_PINW_CAP digital\t\t\t%lu\n", BIT(dword, 9));
	printf("AUD_PINW_CAP conn list\t\t\t%lu\n", BIT(dword, 8));
	printf("AUD_PINW_CAP unsol\t\t\t%lu\n", BIT(dword, 7));
	printf("AUD_PINW_CAP mute\t\t\t%lu\n", BIT(dword, 5));
	printf("AUD_PINW_CAP format override\t\t%lu\n", BIT(dword, 4));
	printf("AUD_PINW_CAP amp param override\t\t%lu\n", BIT(dword, 3));
	printf("AUD_PINW_CAP out amp present\t\t%lu\n", BIT(dword, 2));
	printf("AUD_PINW_CAP in amp present\t\t%lu\n", BIT(dword, 1));

	dword = INREG(AUD_PIN_CAP);
	printf("AUD_PIN_CAP EAPD\t\t\t%lu\n", BIT(dword, 16));
	printf("AUD_PIN_CAP HDMI\t\t\t%lu\n", BIT(dword, 7));
	printf("AUD_PIN_CAP output\t\t\t%lu\n", BIT(dword, 4));
	printf("AUD_PIN_CAP presence detect\t\t%lu\n", BIT(dword, 2));

	dword = INREG(AUD_PINW_CNTR);
	printf("AUD_PINW_CNTR mute status\t\t%lu\n", BIT(dword, 8));
	printf("AUD_PINW_CNTR out enable\t\t%lu\n", BIT(dword, 6));
	printf("AUD_PINW_CNTR amp mute status\t\t%lu\n", BIT(dword, 8));
	printf("AUD_PINW_CNTR amp mute status\t\t%lu\n", BIT(dword, 8));
	printf("AUD_PINW_CNTR stream type\t\t[0x%lx] %s\n",
	       BITS(dword, 2, 0), OPNAME(stream_type, BITS(dword, 2, 0)));

	dword = INREG(AUD_PINW_UNSOLRESP);
	printf("AUD_PINW_UNSOLRESP enable unsol resp\t%lu\n", BIT(dword, 31));

	dword = INREG(AUD_CNTL_ST);
	printf("AUD_CNTL_ST DIP audio enabled\t\t%lu\n", BIT(dword, 21));
	printf("AUD_CNTL_ST DIP ACP enabled\t\t%lu\n", BIT(dword, 22));
	printf("AUD_CNTL_ST DIP ISRCx enabled\t\t%lu\n", BIT(dword, 23));
	printf("AUD_CNTL_ST DIP port select\t\t[0x%lx] %s\n",
	       BITS(dword, 30, 29), dip_port[BITS(dword, 30, 29)]);
	printf("AUD_CNTL_ST DIP buffer index\t\t[0x%lx] %s\n",
	       BITS(dword, 20, 18), OPNAME(dip_index, BITS(dword, 20, 18)));
	printf("AUD_CNTL_ST DIP trans freq\t\t[0x%lx] %s\n",
	       BITS(dword, 17, 16), dip_trans[BITS(dword, 17, 16)]);
	printf("AUD_CNTL_ST DIP address\t\t\t%lu\n", BITS(dword, 3, 0));
	printf("AUD_CNTL_ST CP ready\t\t\t%lu\n", BIT(dword, 15));
	printf("AUD_CNTL_ST ELD valid\t\t\t%lu\n", BIT(dword, 14));
	printf("AUD_CNTL_ST ELD ack\t\t\t%lu\n", BIT(dword, 4));
	printf("AUD_CNTL_ST ELD bufsize\t\t\t%lu\n", BITS(dword, 13, 9));
	printf("AUD_CNTL_ST ELD address\t\t\t%lu\n", BITS(dword, 8, 5));

	dword = INREG(AUD_HDMIW_STATUS);
	printf("AUD_HDMIW_STATUS CDCLK/DOTCLK underrun\t%lu\n", BIT(dword, 31));
	printf("AUD_HDMIW_STATUS CDCLK/DOTCLK overrun\t%lu\n", BIT(dword, 30));
	printf("AUD_HDMIW_STATUS BCLK/CDCLK underrun\t%lu\n", BIT(dword, 29));
	printf("AUD_HDMIW_STATUS BCLK/CDCLK overrun\t%lu\n", BIT(dword, 28));

	dword = INREG(AUD_CONV_CHCNT);
	printf("AUD_CONV_CHCNT HDMI HBR enabled\t\t%lu\n", BITS(dword, 15, 14));
	printf("AUD_CONV_CHCNT HDMI channel count\t%lu\n",
	       BITS(dword, 11, 8) + 1);

	printf("AUD_CONV_CHCNT HDMI channel mapping:\n");
	for (i = 0; i < 8; i++) {
		OUTREG(AUD_CONV_CHCNT, i);
		dword = INREG(AUD_CONV_CHCNT);
		printf("\t\t\t\t\t[0x%x] %u => %lu \n", dword, i,
		       BITS(dword, 7, 4));
	}

	return 0;
}
