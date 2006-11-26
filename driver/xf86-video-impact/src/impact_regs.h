/* 
 * impact_regs.h 2005/07/12 23:24:15, Copyright (c) 2005 peter fuerst
 *
 * Register Layouts of the SGI Impact/ImpactSR (MardiGras) chips.
 *
 * Based on:
 * - linux/include/video/impact.h, 2005 pf.
 * - linux/include/video/impactsr.h, (c) 2004 by Stanislaw Skowronek.
 * - newport_regs.h
 *   # xc/programs/Xserver/hw/xfree86/drivers/newport/newport_regs.h,v
 *     1.2 2001/11/23 19:50:45 dawes Exp #
 *   # newport_regs.h,v 1.5 2000/11/18 23:23:14 agx Exp #
 */

#ifndef __IMPACT_REGS_H__
#define __IMPACT_REGS_H__

typedef volatile unsigned long long int mgireg64_t;
typedef volatile unsigned int mgireg32_t;
typedef volatile unsigned short int mgireg16_t;
typedef volatile unsigned char mgireg8_t;

typedef union mgicfifo {
	mgireg64_t x;
	mgireg32_t w;
} mgicfifo_t;

typedef 
struct Impact_xmapregs {
	mgireg64_t a___________;		/* 0x0000 */
	mgireg8_t  pp1select;			/* 0x0008 */
	mgireg8_t  b___________[0x7f];
	mgireg8_t  index;			/* 0x0088 */
	mgireg8_t  c___________[0x77];
	mgireg32_t config;			/* 0x0100 */
	mgireg32_t d___________;
	mgireg8_t  configb;			/* 0x0108 */
	mgireg8_t  f___________[0x77];
	mgireg32_t buf_select;			/* 0x0180 */
	mgireg8_t  g___________[0x7c];
	mgireg32_t main_mode;			/* 0x0200 */
	mgireg8_t  h___________[0x7c];
	mgireg32_t overlay_mode;		/* 0x0280 */
	mgireg8_t  i___________[0x7c];
	mgireg32_t dib;				/* 0x0300 */
	mgireg8_t  j___________[0x3c];
	mgireg32_t dib_dw;			/* 0x0340 */
	mgireg8_t  k___________[0x3c];
	mgireg32_t re_rac;			/* 0x0380 */
} Impact_xmapregs_t;

typedef 
struct Impact_vc3regs {
	mgireg64_t a________;			/* 0x0000 */
	mgireg8_t  index;			/* 0x0008 */
	mgireg8_t  b________[0x2f];
	mgireg32_t indexdata;			/* 0x038 */
	mgireg8_t  c________[0x74];
	mgireg16_t data;			/* 0x00b0 */
	mgireg16_t d________[0x6f];
	mgireg16_t ram;				/* 0x0190 */
} Impact_vc3regs_t;

typedef 
struct Impact_rexregs {
	mgireg8_t  unused[0x78];		/* 0x0000 */
	mgireg32_t status;			/* 0x0078 */
} Impact_rexregs_t;

typedef 
struct ImpactI2_regs {
	/* Indigo2 Impact */
	mgireg8_t  a__________[0x50020];		/* 0x00000 */
	mgireg32_t cfifo_hw;			/* 0x50020 */
	mgireg32_t cfifo_lw;			/* 0x50024 */
	mgireg32_t cfifo_delay;			/* 0x50028 */
	mgireg32_t dfifo_hw;			/* 0x5002c */
	mgireg32_t dfifo_lw;			/* 0x50030 */
	mgireg32_t dfifo_delay;			/* 0x50034 */
	mgireg8_t  b__________[0x48];
	mgicfifo_t cfifop;			/* 0x50080 */
	mgireg8_t  c__________[0x11b78];
	Impact_xmapregs_t xmap;			/* 0x61c00 */
	mgireg8_t  d__________[0x62000-sizeof(Impact_xmapregs_t)-0x61c00];
	Impact_vc3regs_t vc3;			/* 0x62000 */
	mgireg8_t  e__________[0x70000-sizeof(Impact_vc3regs_t)-0x62000];
	mgireg32_t status;			/* 0x70000 */
	mgireg32_t fifostatus;			/* 0x70004 */
	mgireg8_t  f__________[0x78];
	mgicfifo_t cfifo;			/* 0x70080 */
	mgireg8_t  g__________[0x78];
	mgireg32_t giostatus;			/* 0x70100 */
	mgireg32_t dmabusy;			/* 0x70104 */
	mgireg8_t  h__________[0xc3f8];
	Impact_rexregs_t rss;			/* 0x7c500 */
} ImpactI2Regs, *ImpactI2RegsPtr;

typedef 
struct ImpactSR_regs {
	/* Octane ImpactSR */
	mgireg8_t  a__________[0x20000];	/* 0x00000 */
	mgireg32_t status;			/* 0x20000 */
	mgireg8_t  b__________[4];
	mgireg32_t fifostatus;			/* 0x20008 */
	mgireg8_t  c__________[0xf4];
	mgireg32_t giostatus;			/* 0x20100 */
	mgireg8_t  d__________[0xfc];
	mgireg32_t dmabusy;			/* 0x20200 */
	mgireg8_t  e__________[0x1fc];
	mgicfifo_t cfifo;			/* 0x20400 */
	mgireg8_t  f__________[0xc0f8];
	Impact_rexregs_t rss;			/* 0x2c500 */
	mgireg8_t  g__________[0x40000-sizeof(Impact_rexregs_t)-0x2c500];
	mgireg32_t cfifo_hw;			/* 0x40000 */
	mgireg32_t h__________;
	mgireg32_t cfifo_lw;			/* 0x40008 */
	mgireg32_t i__________;
	mgireg32_t cfifo_delay;			/* 0x40010 */
	mgireg32_t j__________[3];
	mgireg32_t dfifo_hw;			/* 0x40020 */
	mgireg32_t k__________;
	mgireg32_t dfifo_lw;			/* 0x40028 */
	mgireg32_t l__________;
	mgireg32_t dfifo_delay;			/* 0x40030 */
	mgireg8_t  m__________[0x31bcc];
	Impact_xmapregs_t xmap;			/* 0x71c00 */
	mgireg8_t  n__________[0x72000-sizeof(Impact_xmapregs_t)-0x71c00];
	Impact_vc3regs_t vc3;			/* 0x72000 */
	mgireg8_t  o__________[0x130400-sizeof(Impact_vc3regs_t)-0x72000];
	mgicfifo_t cfifop;			/* 0x130400 */
} ImpactSRRegs, *ImpactSRRegsPtr;

typedef union {
	struct ImpactI2_regs i2;
	struct ImpactSR_regs sr;
} ImpactRegs, *ImpactRegsPtr;

static __inline__ void
ImpactFifoCmd64( mgicfifo_t *fifo, unsigned cmd, unsigned reg, unsigned val )
{
#if (_MIPS_SZLONG == 64) /* see Xmd.h */
	fifo->x = ((unsigned long)cmd | reg<<8) << 32 | val;
#else
	fifo->w = cmd | reg<<8;
	fifo->w = val;
#endif
}

static __inline__ void
ImpactFifoCmd32( mgicfifo_t *fifo, unsigned cmd, unsigned reg )
{
	(&fifo->w)[1] = cmd | reg<<8;
}

#define ImpactCmdWriteRss( fifo, reg, val )\
	ImpactFifoCmd64( fifo, 0x00180004, reg, val )
#define ImpactCmdExecRss( fifo, reg, val )\
	ImpactFifoCmd64( fifo, 0x001c0004, reg, val )

#define impact_cmd_gline_xstartf(f,v)	ImpactCmdWriteRss(f,0x000c,v)
#define impact_cmd_ir_alias(f,v)	ImpactCmdExecRss( f,0x0045,v)
#define impact_cmd_blockxystarti(f,x,y)	ImpactCmdWriteRss(f,0x0046,(x)<<16|(y))
#define impact_cmd_blockxyendi(f,x,y)	ImpactCmdWriteRss(f,0x0047,(x)<<16|(y))
#define impact_cmd_packedcolor(f,v)	ImpactCmdWriteRss(f,0x005b,v)
#define impact_cmd_red(f,v)		ImpactCmdWriteRss(f,0x005c,v)
#define impact_cmd_alpha(f,v)		ImpactCmdWriteRss(f,0x005f,v)
#define impact_cmd_char(f,v)		ImpactCmdExecRss( f,0x0070,v)
#define impact_cmd_char_h(f,v)		ImpactCmdWriteRss(f,0x0070,v)
#define impact_cmd_char_l(f,v)		ImpactCmdExecRss( f,0x0071,v)
#define impact_cmd_xfrcontrol(f,v)	ImpactCmdWriteRss(f,0x0102,v)
#define impact_cmd_fillmode(f,v)	ImpactCmdWriteRss(f,0x0110,v)
#define impact_cmd_config(f,v)		ImpactCmdWriteRss(f,0x0112,v)
#define impact_cmd_xywin(f,x,y)		ImpactCmdWriteRss(f,0x0115,(y)<<16|(x))
#define impact_cmd_xfrsize(f,x,y)	ImpactCmdWriteRss(f,0x0153,(y)<<16|(x))
#define impact_cmd_xfrmasklo(f,v)	ImpactCmdWriteRss(f,0x0156,v)
#define impact_cmd_xfrmaskhi(f,v)	ImpactCmdWriteRss(f,0x0157,v)
#define impact_cmd_xfrcounters(f,x,y)	ImpactCmdWriteRss(f,0x0158,(y)<<16|(x))
#define impact_cmd_xfrmode(f,v)		ImpactCmdWriteRss(f,0x0159,v)
#define impact_cmd_re_togglecntx(f,v)	ImpactCmdWriteRss(f,0x015f,v)
#define impact_cmd_pixcmd(f,v)		ImpactCmdWriteRss(f,0x0160,v)
#define impact_cmd_pp1fillmode(f,m,o)	ImpactCmdWriteRss(f,0x0161,(m)|(o)<<26)
#define impact_cmd_colormaskmsbs(f,v)	ImpactCmdWriteRss(f,0x0162,v)
#define impact_cmd_colormasklsbsa(f,v)	ImpactCmdWriteRss(f,0x0163,v)
#define impact_cmd_colormasklsbsb(f,v)	ImpactCmdWriteRss(f,0x0164,v)
#define impact_cmd_drbpointers(f,v)	ImpactCmdWriteRss(f,0x016d,v)

#define impact_cmd_hq_pixelformat(f,v)	ImpactFifoCmd64(f,0x000c0004,0,v)
#define impact_cmd_hq_scanwidth(f,v)	ImpactFifoCmd64(f,0x000a0204,0,v)
#define impact_cmd_hq_dmatype(f,v)	ImpactFifoCmd64(f,0x000a0604,0,v)
#define impact_cmd_hq_pg_list0(f,v)	ImpactFifoCmd64(f,0x00080004,0,v)
#define impact_cmd_hq_pg_width(f,v)	ImpactFifoCmd64(f,0x00080404,0,v)
#define impact_cmd_hq_pg_offset(f,v)	ImpactFifoCmd64(f,0x00080504,0,v)
#define impact_cmd_hq_pg_startaddr(f,v)	ImpactFifoCmd64(f,0x00080604,0,v)
#define impact_cmd_hq_pg_linecnt(f,v)	ImpactFifoCmd64(f,0x00080704,0,v)
#define impact_cmd_hq_pg_widtha(f,v)	ImpactFifoCmd64(f,0x00080804,0,v)
#if 0
#define impact_cmd_hq_dmactrl_1(f)	ImpactFifoCmd32(f,0x00080b04,0)
#define impact_cmd_hq_dmactrl_2(f)	ImpactFifoCmd64(f,0x000000b7,0,0x000e0400)
#else
#define impact_cmd_hq_dmactrl_a(f,p)	ImpactFifoCmd64(f,0x00080b04,0,((p)<<1)|0x000000b1)
#define impact_cmd_hq_dmactrl_b(f)	ImpactFifoCmd32(f,0x000e0400,0)
#endif

/* Now the Indexed registers of the VC2. */
#define VC2_IREG_CONFIG    0x1f	/* this is wrong (0x20) in newport_regs.h */

#define VC3_IREG_CURSOR    0x1d
#define VC3_IREG_CONTROL   0x1e
#define VC3_IREG_CONFIG    VC2_IREG_CONFIG

/* VC2 Control register bits */
#define VC2_CTRL_EVIRQ     0x0001	/* VINTR enable */
#define VC2_CTRL_EDISP     0x0002	/* Blackout */
#define VC2_CTRL_EVIDEO    0x0004	/* Video Timing Enable */
#define VC2_CTRL_EDIDS     0x0008	/* DID Function Enable */

#define VC3_CTRL_EVIRQ     VC2_CTRL_EVIRQ
#define VC3_CTRL_EDISP     VC2_CTRL_EDISP
#define VC3_CTRL_EVIDEO    VC2_CTRL_EVIDEO
#define VC3_CTRL_EDIDS     VC2_CTRL_EDIDS
#define VC3_CTRL_ECURS     0x0001	/* Cursor Enable */

/* Logic operations for the PP1 (SI=source invert, DI=dest invert, RI=result invert) */
#define IMPACT_LO_CLEAR	0
#define IMPACT_LO_AND	1
#define IMPACT_LO_DIAND	2
#define IMPACT_LO_COPY	3
#define IMPACT_LO_SIAND	4
#define IMPACT_LO_NOP	5
#define IMPACT_LO_XOR	6
#define IMPACT_LO_OR	7
#define IMPACT_LO_RIOR	8
#define IMPACT_LO_RIXOR	9
#define IMPACT_LO_RINOP	10
#define IMPACT_LO_DIOR	11
#define IMPACT_LO_RICOPY	12
#define IMPACT_LO_SIOR	13
#define IMPACT_LO_RIAND	14
#define IMPACT_LO_SET	15

/*
 * Scanning physaddr range 1f000000 to 1f3fffff on Indigo2 shows this
 * periodic pattern (but couldn't test more than a single board yet).
 */
#define IMPACTI2_BASE_ADDR0  0x1f000000
#define IMPACTI2_BASE_OFFSET 0x00100000
#define IMPACTI2_MAX_BOARDS  3
/* Don't know, how it looks on Octane... */
#define IMPACTSR_BASE_ADDR0  0x1c000000
#define IMPACTSR_BASE_OFFSET 0
#define IMPACTSR_MAX_BOARDS  1

#endif /* __IMPACT_REGS_H__ */
