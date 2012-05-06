
#ifndef SAVAGE_REGS_H
#define SAVAGE_REGS_H

/* Copied and renamed from radeon_reg.h for AGP/PCI detection. */
#define SAVAGE_STATUS_PCI_CONFIG            0x06
#       define SAVAGE_CAP_LIST              0x100000
#define SAVAGE_CAPABILITIES_PTR_PCI_CONFIG  0x34 /* offset in PCI config*/
#       define SAVAGE_CAP_PTR_MASK          0xfc /* mask off reserved bits of CAP_PTR */
#       define SAVAGE_CAP_ID_NULL           0x00 /* End of capability list */
#       define SAVAGE_CAP_ID_AGP            0x02 /* AGP capability ID */

#define S3_SAVAGE3D_SERIES(chip)  ((chip>=S3_SAVAGE3D) && (chip<=S3_SAVAGE_MX))

#define S3_SAVAGE4_SERIES(chip)  ((chip==S3_SAVAGE4)            \
                                  || (chip==S3_PROSAVAGE)       \
                                  || (chip==S3_TWISTER)         \
                                  || (chip==S3_PROSAVAGEDDR))

#define	S3_SAVAGE_MOBILE_SERIES(chip)	((chip==S3_SAVAGE_MX) || (chip==S3_SUPERSAVAGE))

#define S3_SAVAGE_SERIES(chip)    ((chip>=S3_SAVAGE3D) && (chip<=S3_SAVAGE2000))

#define S3_MOBILE_TWISTER_SERIES(chip)   ((chip==S3_TWISTER)    \
                                          ||(chip==S3_PROSAVAGEDDR))

/* Chip tags.  These are used to group the adapters into 
 * related families.
 */

enum S3CHIPTAGS {
    S3_UNKNOWN = 0,
    S3_SAVAGE3D,
    S3_SAVAGE_MX,
    S3_SAVAGE4,
    S3_PROSAVAGE,
    S3_TWISTER,
    S3_PROSAVAGEDDR,
    S3_SUPERSAVAGE,
    S3_SAVAGE2000,
    S3_LAST
};

#define BIOS_BSIZE			1024
#define BIOS_BASE			0xc0000

#define SAVAGE_NEWMMIO_REGBASE_S3	0x1000000  /* 16MB */
#define SAVAGE_NEWMMIO_REGBASE_S4	0x0000000 
#define SAVAGE_NEWMMIO_REGSIZE		0x0080000	/* 512kb */
#define SAVAGE_NEWMMIO_VGABASE		0x8000

#define BASE_FREQ			14.31818	

#define FIFO_CONTROL_REG		0x8200
#define MIU_CONTROL_REG			0x8204
#define STREAMS_TIMEOUT_REG		0x8208
#define MISC_TIMEOUT_REG		0x820c

#define ADVANCED_FUNC_CTRL		0x850C	

/*
 * CR/SR registers MMIO offset
 * MISC Output Register(W:0x3c2,R:0x3cc) controls CR is 0X83Cx or 0X83Bx
 * but do we need to set MISC Output Register ???
 * (Note that CRT_ADDRESS_REG and CRT_DATA_REG are assumed to be COLOR)???
 */
#define MMIO_BASE_OF_VGA3C0             0X83C0
#define MMIO_BASE_OF_VGA3D0             0X83D0

#define ATTR_ADDRESS_REG        \
    (MMIO_BASE_OF_VGA3C0 + (0x03C0 - 0x03C0))
#define ATTR_DATA_WRITE_REG     \
    (MMIO_BASE_OF_VGA3C0 + (0x03C0 - 0x03C0))
#define ATTR_DATA_READ_REG      \
    (MMIO_BASE_OF_VGA3C0 + (0x03C1 - 0x03C0))
#define VIDEO_SUBSYSTEM_ENABLE  \
    (MMIO_BASE_OF_VGA3C0 + (0x03C3 - 0x03C0))
#define SEQ_ADDRESS_REG         \
    (MMIO_BASE_OF_VGA3C0 + (0x03C4 - 0x03C0))
#define SEQ_DATA_REG            \
    (MMIO_BASE_OF_VGA3C0 + (0x03C5 - 0x03C0))
#define DAC_PIXEL_MASK_REG      \
    (MMIO_BASE_OF_VGA3C0 + (0x03C6 - 0x03C0))
#define DAC_PEL_MASK            \
    (MMIO_BASE_OF_VGA3C0 + (0x03C6 - 0x03C0))
#define DAC_STATUS_REG          \
    (MMIO_BASE_OF_VGA3C0 + (0x03C7 - 0x03C0))
#define DAC_ADDRESS_READ_REG    \
    (MMIO_BASE_OF_VGA3C0 + (0x03C7 - 0x03C0))
#define DAC_ADDRESS_WRITE_REG   \
    (MMIO_BASE_OF_VGA3C0 + (0x03C8 - 0x03C0))
#define DAC_DATA_REG            \
    (MMIO_BASE_OF_VGA3C0 + (0x03C9 - 0x03C0))
#define DAC_DATA_REG_PORT       \
    (MMIO_BASE_OF_VGA3C0 + (0x03C9 - 0x03C0))
#define MISC_OUTPUT_REG_WRITE   \
    (MMIO_BASE_OF_VGA3C0 + (0x03C2 - 0x03C0))
#define MISC_OUTPUT_REG_READ    \
    (MMIO_BASE_OF_VGA3C0 + (0x03CC - 0x03C0))
#define GR_ADDRESS_REG          \
    (MMIO_BASE_OF_VGA3C0 + (0x03CE - 0x03C0))
#define GR_DATA_REG             \
    (MMIO_BASE_OF_VGA3C0 + (0x03CF - 0x03C0))
#define WAKEUP_REG              \
    (MMIO_BASE_OF_VGA3C0 + (0x0510 - 0x03C0))

#define CRT_ADDRESS_REG         \
    (MMIO_BASE_OF_VGA3D0 + (0x03D4 - 0x03D0))
#define CRT_DATA_REG            \
    (MMIO_BASE_OF_VGA3D0 + (0x03D5 - 0x03D0))
#define SYSTEM_CONTROL_REG      \
    (MMIO_BASE_OF_VGA3D0 + (0x03DA - 0x03D0))

/* GX-3 Configuration/Status Registers */
#define S3_SHADOW_STATUS              0x48C0C
#define S3_BUFFER_THRESHOLD           0x48C10
#define S3_OVERFLOW_BUFFER            0x48C14
#define S3_OVERFLOW_BUFFER_PTR        0x48C18

#define ENABLE_BCI                        0x08   /* MM48C18_3 */
#define ENABLE_COMMAND_OVERFLOW_BUF       0x04   /* MM48C18_2 */
#define ENABLE_COMMAND_BUF_STATUS_UPDATE  0x02   /* MM48C18_1 */
#define ENABLE_SHADOW_STATUS_UPDATE       0x01   /* MM48C0C_0 */


#define MEMORY_CTRL0_REG            0xCA
#define MEMORY_CTRL1_REG            0xCB
#define MEMORY_CTRL2_REG            0xCC

#define MEMORY_CONFIG_REG           0x31

/* bitmap descriptor register */
#define S3_GLB_BD_LOW                      0X8168
#define S3_GLB_BD_HIGH                     0X816C
#define S3_PRI_BD_LOW                      0X8170
#define S3_PRI_BD_HIGH                     0X8174
#define S3_SEC_BD_LOW                      0X8178
#define S3_SEC_BD_HIGH                     0X817c

/* duoview */

#define SELECT_IGA1                 0x4026
#define SELECT_IGA2_READS_WRITES    0x4f26

#define SelectIGA1()                 \
do {                                    \
    OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA1); \
} while (0)

#define SelectIGA2()                 \
do {                                    \
    OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA2_READS_WRITES); \
} while (0)

#define MEM_PS1                     0x10    /*CRCA_4 :Primary stream 1*/
#define MEM_PS2                     0x20    /*CRCA_5 :Primary stream 2*/
#define MEM_SS1                     0x40    /*CRCA_6 :Secondary stream 1*/
#define MEM_SS2                     0x80    /*CRCA_7 :Secondary stream 2*/

#define SUBSYS_STAT_REG			0x8504

#define SRC_BASE			0xa4d4
#define DEST_BASE			0xa4d8
#define CLIP_L_R			0xa4dc
#define CLIP_T_B			0xa4e0
#define DEST_SRC_STR			0xa4e4
#define MONO_PAT_0			0xa4e8
#define MONO_PAT_1			0xa4ec

#define TILED_SURFACE_REGISTER_0        0x48c40
#define TILED_SURFACE_REGISTER_1        0x48c44
#define TILED_SURFACE_REGISTER_2        0x48c48
#define TILED_SURFACE_REGISTER_3        0x48c4c
#define TILED_SURFACE_REGISTER_4        0x48c50
                                                
#define TILED_SURF_BPP4    0x00000000  /* bits 31-30=00 for  4 bits/pixel */
#define TILED_SURF_BPP8    0x40000000  /* bits 31-30=01 for  8 bits/pixel */
#define TILED_SURF_BPP16   0x80000000	/* bits 31-30=10 for 16 bits/pixel */
#define TILED_SURF_BPP32   0xC0000000  /* bits 31-30=11 for 32 bits/pixel */

#define TILED_SURF_BPP16_2000   0x00000000   /* bit 31=0 for 16 bits/pixel */
#define TILED_SURF_BPP32_2000   0x80000000  /* bit 31=1 for 32 bits/pixel */

/*
 * CR88_4 =1 : disable block write
 * the "2D" is partly to set this apart from "BLOCK_WRITE_DISABLE"
 * constant used for bitmap  descriptor
 */
#define DISABLE_BLOCK_WRITE_2D      0x10 
#define BLOCK_WRITE_DISABLE         0x0

/* CR31[0] set = Enable 8MB display memory through 64K window at A0000H. */
#define ENABLE_CPUA_BASE_A0000      0x01  

/* Constants for CR69. */

#define CRT_ACTIVE	0x01
#define LCD_ACTIVE	0x02
#define TV_ACTIVE	0x04
#define CRT_ATTACHED	0x10
#define LCD_ATTACHED	0x20
#define TV_ATTACHED	0x40


/*
 * reads from SUBSYS_STAT
 */
#define STATUS_WORD0            (INREG(0x48C00))
#define ALT_STATUS_WORD0        (INREG(0x48C60))
#define MAXLOOP			0xffffff
#define IN_SUBSYS_STAT()	(INREG(SUBSYS_STAT_REG))

#define MAXFIFO		0x7f00

#define inStatus1() (VGAHWPTR(pScrn))->readST01( VGAHWPTR(pScrn) )


/*
 * unprotect CRTC[0-7]              
 * CR11_7 = 0: Writing to all CRT Controller registers enabled      
 *        = 1: Writing to all bits of CR0~CR7 except CR7_4 disabled 
 */                                                                 
#define UnProtectCRTC()                 \
do {                                    \
    unsigned char byte;                 \
    OUTREG8(CRT_ADDRESS_REG,0x11);      \
    byte = INREG8(CRT_DATA_REG) & 0X7F; \
    OUTREG16(CRT_ADDRESS_REG,byte << 8 | 0x11); \
} while (0)

/*                                  
 * unlock extended regs                     
 * CR38:unlock CR20~CR3F            
 * CR39:unlock CR40~CRFF            
 * SR08:unlock SR09~SRFF            
 */                                 
#define UnLockExtRegs()                 \
do {                                    \
    OUTREG16(CRT_ADDRESS_REG,0X4838);   \
    OUTREG16(CRT_ADDRESS_REG,0XA039);   \
    OUTREG16(SEQ_ADDRESS_REG,0X0608);   \
} while (0)

#define VerticalRetraceWait()           \
do {                                    \
	INREG8(CRT_ADDRESS_REG);            \
	OUTREG8(CRT_ADDRESS_REG, 0x17);     \
	if (INREG8(CRT_DATA_REG) & 0x80) {  \
		int i = 0x10000;                \
		while ((INREG8(SYSTEM_CONTROL_REG) & 0x08) == 0x08 && i--) ; \
		i = 0x10000;                                                  \
		while ((INREG8(SYSTEM_CONTROL_REG) & 0x08) == 0x00 && i--) ; \
	} \
} while (0)

/*
 * Jiayo Hsu, Mar 21, 2002             
 * modify this to scalable schema,because different chips have differnt regs,          
 * besides add in patch code for  Paramount(SuperSavage) from 2K
 */
#define InI2CREG(a,reg)                                 \
do {                                                    \
	OUTREG8(CRT_ADDRESS_REG, reg);                      \
	if (psav->Chipset == S3_SUPERSAVAGE)                \
	   OUTREG8(CRT_DATA_REG, INREG8(CRT_DATA_REG)); 	\
	a = INREG8(CRT_DATA_REG);                           \
} while (0)

#define OutI2CREG(a,reg)                        \
do {                                            \
	OUTREG8(CRT_ADDRESS_REG, reg);				\
	if (psav->Chipset == S3_SUPERSAVAGE)		\
	   OUTREG8(CRT_DATA_REG, a); 				\
	OUTREG8(CRT_DATA_REG, a);                   \
} while (0)
 
#define HZEXP_COMP_1		0x54
#define HZEXP_BORDER		0x58
#define HZEXP_FACTOR_IGA1	0x59

#define VTEXP_COMP_1		0x56
#define VTEXP_BORDER		0x5a
#define VTEXP_FACTOR_IGA1	0x5b

#define EC1_CENTER_ON	0x10
#define EC1_EXPAND_ON	0x0c

#endif /* SAVAGE_REGS_H */
