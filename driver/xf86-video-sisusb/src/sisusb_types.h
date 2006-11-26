/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_types.h,v 1.7 2005/08/15 22:57:51 twini Exp $ */
/*
 * General type definitions for universal mode switching modules
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License as published by
 * * the Free Software Foundation; either version 2 of the named License,
 * * or any later version.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) The name of the author may not be used to endorse or promote products
 * *    derived from this software without specific prior written permission.
 * *
 * * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: 	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifndef _VGATYPES_
#define _VGATYPES_

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef CHAR
typedef char CHAR;
#endif

#ifndef SHORT
typedef short SHORT;
#endif

#ifndef LONG
typedef long  LONG;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#ifndef ULONG
typedef unsigned long ULONG;
#endif

#ifndef BOOLEAN
typedef unsigned char BOOLEAN;
#endif

#define SISIOMEMTYPE

#if XF86_VERSION_CURRENT < XF86_VERSION_NUMERIC(4,2,0,0,0)
typedef unsigned long IOADDRESS;
typedef unsigned long SISIOADDRESS;
#else
typedef IOADDRESS SISIOADDRESS;
#endif

typedef enum _SIS_CHIP_TYPE {
    SIS_VGALegacy = 0,
    SIS_530,
    SIS_OLD,
    SIS_300,
    SIS_630,
    SIS_730,
    SIS_540,
    SIS_315H,   /* SiS 310 */
    SIS_315,
    SIS_315PRO, /* SiS 325 */
    SIS_550,
    SIS_650,
    SIS_740,
    SIS_330,
    SIS_661,
    SIS_741,
    SIS_670,
    SIS_660 = 35,
    SIS_760,
    SIS_761,
    SIS_762,
    SIS_770,
    SIS_340 = 55,
    SIS_341,
    SIS_342,
    XGI_20  = 75,
    XGI_40,
    MAX_SIS_CHIP
} SIS_CHIP_TYPE;

/* Addtional IOCTLs for communication sisfb <> X driver        */
/* If changing this, sisfb.h must also be changed (for sisfb) */

/* ioctl for identifying and giving some info (esp. memory heap start) */
#define SISUSBFB_GET_INFO_SIZE	0x8004f33c
#define SISUSBFB_GET_INFO	0x8000f33b  /* Must be patched with result from ..._SIZE at D[29:16] */

/* lock sisfb from register access */
#define SISUSBFB_SET_LOCK	0x4004f33a

/* Structure argument for SISUSBFB_GET_INFO ioctl  */
typedef struct _SISUSBFB_INFO sisusbfb_info, *psisusbfb_info;

struct _SISUSBFB_INFO {
	CARD32 	sisusbfb_id;         	/* for identifying sisusbfb */
#define SISUSBFB_ID	  0x53495546    /* Identify myself with 'SIUF' */
	CARD32	chip_id;		/* PCI ID of detected chip */
	CARD32	memory;			/* video memory in KB which sisusbfb manages */
	CARD32	heapstart;		/* heap start in KB */
	CARD8	fbvidmode;		/* current sisfb mode */

	CARD8	sisusbfb_version;
	CARD8	sisusbfb_revision;
	CARD8	sisusbfb_patchlevel;

	CARD8	sisusbfb_caps;		/* sisfb's capabilities */

	CARD32	sisusbfb_tqlen;		/* turbo/cmd queue length (in KB) */

	CARD32	sisusbfb_minor;    	/* minor device number of USB device */

	CARD32	reserved[32]; 		/* for future use */
};

/* Structure argument for SISUSB_GET_INFO ioctl  */
typedef struct _SISUSB_INFO sisusb_info, *psisusb_info;

struct _SISUSB_INFO {
	CARD32   sisusb_id;         	/* for identifying sisusb */
#define SISUSB_ID  0x53495355    	/* Identify myself with 'SISU' */
	CARD8   sisusb_version;
	CARD8   sisusb_revision;
	CARD8 	sisusb_patchlevel;
	CARD8	sisusb_gfxinit;		/* graphics core initialized? */

	CARD32	sisusb_vrambase;
	CARD32 	sisusb_mmiobase;
	CARD32	sisusb_iobase;
	CARD32	sisusb_pcibase;

	CARD32	sisusb_vramsize;

	CARD32  sisusb_minor;

	CARD32  sisusb_fbdevactive;	/* != 0 if framebuffer device active */

	CARD32  sisusb_conactive;	/* != 0 if console driver active */

	CARD8 	sisusb_reserved[28]; 	/* for future use */
};

typedef struct _SISUSB_COMMAND sisusb_command, *psisusb_command;

struct _SISUSB_COMMAND {
	CARD8  operation;	/* see below */
	CARD8  data0;		/* operation dependent */
	CARD8  data1;		/* operation dependent */
	CARD8  data2;		/* operation dependent */
	CARD32 data3;		/* operation dependent */
	CARD32 data4;		/* for future use */
};

/* quick routines to handle index registers */
#define SUCMD_GET      0x01	/* for all: data0 = index, data3 = port */
#define SUCMD_SET      0x02	/* data1 = value */
#define SUCMD_SETOR    0x03	/* data1 = or */
#define SUCMD_SETAND   0x04	/* data1 = and */
#define SUCMD_SETANDOR 0x05	/* data1 = and, data2 = or */
#define SUCMD_SETMASK  0x06	/* data1 = data, data2 = mask */

/* Clear video RAM */
#define SUCMD_CLRSCR   0x07	/* data0:1:2 = length, data3 = address */

/* Restore text mode & fonts (console driver); destroy text mode */
#define SUCMD_HANDLETEXTMODE 0x08

/* ioctl numbers */
#define SISUSB_GET_CONFIG_SIZE 	0x8004f33e     	/* _IOR(0xF3,0x3E,__u32) */
#define SISUSB_GET_CONFIG  	0x8000f33f	/* _IOR(0xF3,0x3F,__u32) */
		/* Must be patched with result from ..._SIZE at D[29:16] */
#define SISUSB_COMMAND 		0xc00cf33d	/* _IORW(0xF3,0x3D,struct _SISUSB_COMMAND) */

#endif

