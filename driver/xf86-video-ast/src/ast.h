/*
 * Copyright (c) 2005 ASPEED Technology Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>
#include <stdlib.h>

#include "ast_pcirename.h"

/* Compiler Options */
#define	Accel_2D
/* #define MMIO_2D */
#define HWC
/* #define PATCH_ABI_VERSION */

/* Vendor & Device Info */
#ifndef PCI_VENDOR_AST
#define PCI_VENDOR_AST			0x1A03	
#endif	

#ifndef	PCI_CHIP_AST2000
#define PCI_CHIP_AST2000		0x2000		
#endif

typedef enum _CHIP_ID {
    VGALegacy,
    AST2000,
    AST2100	
} CHIP_ID;

/* AST REC Info */
#define AST_NAME 			"AST"
#define AST_DRIVER_NAME 		"ast"
#define AST_MAJOR_VERSION 		PACKAGE_VERSION_MAJOR
#define AST_MINOR_VERSION 		PACKAGE_VERSION_MINOR
#define AST_PATCH_VERSION		PACKAGE_VERSION_PATCHLEVEL
#define AST_VERSION	\
        ((AST_MAJOR_VERSION << 20) | (AST_MINOR_VERSION << 10) | AST_PATCH_VERSION)

/* Customized Info */
#define DEFAULT_VIDEOMEM_SIZE		0x00800000
#define DEFAULT_MMIO_SIZE		0x00020000
#define DEFAULT_CMDQ_SIZE		0x00100000
#define MIN_CMDQ_SIZE			0x00040000
#define CMD_QUEUE_GUARD_BAND    	0x00000020
#define DEFAULT_HWC_NUM			0x00000002

/* Patch Info */
#define ABI_VIDEODRV_VERSION_PATCH	SET_ABI_VERSION(0, 5)

/* Data Type Definition */
typedef INT32  		LONG;
typedef CARD8   	UCHAR;
typedef CARD16  	USHORT;
typedef CARD32  	ULONG;

/* Data Structure Definition */
typedef struct _ASTRegRec {
    UCHAR 	ExtCRTC[0x50];
    
} ASTRegRec, *ASTRegPtr;

typedef struct _VIDEOMODE {

    int			ScreenWidth;
    int			ScreenHeight;
    int			bitsPerPixel;
    int			ScreenPitch;
    	
} VIDEOMODE, *PVIDEOMODE;

typedef struct {

    ULONG		ulCMDQSize;
    ULONG		ulCMDQType;
    
    ULONG		ulCMDQOffsetAddr;
    UCHAR       	*pjCMDQVirtualAddr;
    
    UCHAR       	*pjCmdQBasePort;
    UCHAR       	*pjWritePort;    
    UCHAR       	*pjReadPort;     
    UCHAR       	*pjEngStatePort;
          
    ULONG		ulCMDQMask;
    ULONG		ulCurCMDQueueLen;
                
    ULONG		ulWritePointer;
    ULONG		ulReadPointer;
    
    ULONG		ulReadPointer_OK;		/* for Eng_DBGChk */
    
} CMDQINFO, *PCMDQINFO;

typedef struct {

    int			HWC_NUM;
    int			HWC_NUM_Next;

    ULONG		ulHWCOffsetAddr;
    UCHAR       	*pjHWCVirtualAddr;
    
    USHORT		cursortype;
    USHORT		width;
    USHORT   		height;   
    USHORT		offset_x;
    USHORT   		offset_y;
    ULONG		fg;
    ULONG		bg;
    
        
} HWCINFO, *PHWCINFO;

typedef struct _ASTRec {
	
    EntityInfoPtr 	pEnt;
#ifndef XSERVER_LIBPCIACCESS	
	pciVideoPtr		PciInfo;
	PCITAG			PciTag;
#else
	struct pci_device       *PciInfo;
#endif

    OptionInfoPtr 	Options;
    DisplayModePtr      ModePtr;		    
    FBLinearPtr 	pCMDQPtr;    
    XAAInfoRecPtr	AccelInfoPtr;
    xf86CursorInfoPtr   HWCInfoPtr;
    FBLinearPtr 	pHWCPtr;    

    CloseScreenProcPtr CloseScreen;
    ScreenBlockHandlerProcPtr BlockHandler;

    UCHAR		jChipType;
             
    Bool 		noAccel;
    Bool 		noHWC;
    Bool 		MMIO2D;
    int			ENGCaps;
    int			DBGSelect;
              	
    ULONG     		FBPhysAddr;		/* Frame buffer physical address     */
    ULONG     		MMIOPhysAddr;     	/* MMIO region physical address      */
    ULONG     		BIOSPhysAddr;     	/* BIOS physical address             */
    
    UCHAR     		*FBVirtualAddr;   	/* Map of frame buffer               */
    UCHAR     		*MMIOVirtualAddr; 	/* Map of MMIO region                */

    unsigned long	FbMapSize;
    unsigned long	MMIOMapSize;
       
    IOADDRESS		IODBase;        	/* Base of PIO memory area */
    IOADDRESS		PIOOffset;
    IOADDRESS		RelocateIO;
    
    VIDEOMODE 		VideoModeInfo;
    ASTRegRec           SavedReg;
    CMDQINFO		CMDQInfo;
    HWCINFO    		HWCInfo;
    ULONG		ulCMDReg;   
    Bool		EnableClip;
   		
} ASTRec, *ASTRecPtr;
	
#define ASTPTR(p) ((ASTRecPtr)((p)->driverPrivate))

/* Include Files */
#include "ast_mode.h"
#include "ast_vgatool.h"
#include "ast_2dtool.h"
#include "ast_cursor.h"
