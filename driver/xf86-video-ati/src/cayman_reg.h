/*
 * Cayman Register documentation
 *
 * Copyright (C) 2011  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _CAYMAN_REG_H_
#define _CAYMAN_REG_H_

/*
 * Register definitions
 */

#include "cayman_reg_auto.h"

enum {
    SHADER_TYPE_PS,
    SHADER_TYPE_VS,
    SHADER_TYPE_GS,
    SHADER_TYPE_HS,
    SHADER_TYPE_LS,
    SHADER_TYPE_CS,
    SHADER_TYPE_FS,
};


/* SET_*_REG offsets + ends */
#define SET_CONFIG_REG_offset  0x00008000
#define SET_CONFIG_REG_end     0x0000ac00
#define SET_CONTEXT_REG_offset 0x00028000
#define SET_CONTEXT_REG_end    0x00029000
#define SET_RESOURCE_offset    0x00030000
#define SET_RESOURCE_end       0x00038000
#define SET_SAMPLER_offset     0x0003c000
#define SET_SAMPLER_end        0x0003c600
#define SET_CTL_CONST_offset   0x0003cff0
#define SET_CTL_CONST_end      0x0003ff0c
#define SET_LOOP_CONST_offset  0x0003a200
#define SET_LOOP_CONST_end     0x0003a500
#define SET_BOOL_CONST_offset  0x0003a500
#define SET_BOOL_CONST_end     0x0003a518


/* Packet3 commands */
enum {
    IT_NOP                      = 0x10,
    IT_INDIRECT_BUFFER_END      = 0x17,
    IT_SET_PREDICATION          = 0x20,
    IT_COND_EXEC                = 0x22,
    IT_PRED_EXEC                = 0x23,
    IT_DRAW_INDEX_2             = 0x27,
    IT_CONTEXT_CONTROL          = 0x28,
    IT_DRAW_INDEX_OFFSET        = 0x29,
    IT_INDEX_TYPE               = 0x2A,
    IT_DRAW_INDEX               = 0x2B,
    IT_DRAW_INDEX_AUTO          = 0x2D,
    IT_DRAW_INDEX_IMMD          = 0x2E,
    IT_NUM_INSTANCES            = 0x2F,
    IT_INDIRECT_BUFFER          = 0x32,
    IT_STRMOUT_BUFFER_UPDATE    = 0x34,
    IT_MEM_SEMAPHORE            = 0x39,
    IT_MPEG_INDEX               = 0x3A,
    IT_WAIT_REG_MEM             = 0x3C,
    IT_MEM_WRITE                = 0x3D,
    IT_SURFACE_SYNC             = 0x43,
    IT_ME_INITIALIZE            = 0x44,
    IT_COND_WRITE               = 0x45,
    IT_EVENT_WRITE              = 0x46,
    IT_EVENT_WRITE_EOP          = 0x47,
    IT_EVENT_WRITE_EOS          = 0x48,
    IT_SET_CONFIG_REG           = 0x68,
    IT_SET_CONTEXT_REG          = 0x69,
    IT_SET_ALU_CONST            = 0x6A,
    IT_SET_BOOL_CONST           = 0x6B,
    IT_SET_LOOP_CONST           = 0x6C,
    IT_SET_RESOURCE             = 0x6D,
    IT_SET_SAMPLER              = 0x6E,
    IT_SET_CTL_CONST            = 0x6F,
};

/* IT_WAIT_REG_MEM operation encoding */

#define IT_WAIT_ALWAYS          (0 << 0)
#define IT_WAIT_LT              (1 << 0)
#define IT_WAIT_LE              (2 << 0)
#define IT_WAIT_EQ              (3 << 0)
#define IT_WAIT_NE              (4 << 0)
#define IT_WAIT_GE              (5 << 0)
#define IT_WAIT_GT              (6 << 0)
#define IT_WAIT_REG             (0 << 4)
#define IT_WAIT_MEM             (1 << 4)

#define IT_WAIT_ADDR(x)         ((x) >> 2)

enum {

    SQ_LDS_ALLOC_PS                                       = 0x288ec,
    SQ_DYN_GPR_CNTL_PS_FLUSH_REQ                          = 0x8d8c,

    CP_COHER_CNTL                                         = 0x85f0,
	DEST_BASE_0_ENA_bit                               = 1 << 0,
	DEST_BASE_1_ENA_bit                               = 1 << 1,
	SO0_DEST_BASE_ENA_bit                             = 1 << 2,
	SO1_DEST_BASE_ENA_bit                             = 1 << 3,
	SO2_DEST_BASE_ENA_bit                             = 1 << 4,
	SO3_DEST_BASE_ENA_bit                             = 1 << 5,
	CB0_DEST_BASE_ENA_bit                             = 1 << 6,
	CB1_DEST_BASE_ENA_bit                             = 1 << 7,
	CB2_DEST_BASE_ENA_bit                             = 1 << 8,
	CB3_DEST_BASE_ENA_bit                             = 1 << 9,
	CB4_DEST_BASE_ENA_bit                             = 1 << 10,
	CB5_DEST_BASE_ENA_bit                             = 1 << 11,
	CB6_DEST_BASE_ENA_bit                             = 1 << 12,
	CB7_DEST_BASE_ENA_bit                             = 1 << 13,
	DB_DEST_BASE_ENA_bit                              = 1 << 14,
	CB8_DEST_BASE_ENA_bit                             = 1 << 15,
	CB9_DEST_BASE_ENA_bit                             = 1 << 16,
	CB10_DEST_BASE_ENA_bit                            = 1 << 17,
	CB11_DEST_BASE_ENA_bit                            = 1 << 18,
	FULL_CACHE_ENA_bit                                = 1 << 20,
	TC_ACTION_ENA_bit                                 = 1 << 23,
	CB_ACTION_ENA_bit                                 = 1 << 25,
	DB_ACTION_ENA_bit                                 = 1 << 26,
	SH_ACTION_ENA_bit                                 = 1 << 27,
	SX_ACTION_ENA_bit                                 = 1 << 28,
    CP_COHER_SIZE                                         = 0x85f4,
    CP_COHER_BASE                                         = 0x85f8,
    CP_COHER_STATUS                                       = 0x85fc,
	MATCHING_GFX_CNTX_mask                            = 0xff << 0,
	MATCHING_GFX_CNTX_shift                           = 0,
	STATUS_bit                                        = 1 << 31,

//  SQ_VTX_CONSTANT_WORD2_0                               = 0x00030008,
//    	SQ_VTX_CONSTANT_WORD2_0__DATA_FORMAT_mask         = 0x3f << 20,
	FMT_INVALID=0,      FMT_8,          FMT_4_4,            FMT_3_3_2,
	                    FMT_16=5,       FMT_16_FLOAT,       FMT_8_8,
	FMT_5_6_5,          FMT_6_5_5,      FMT_1_5_5_5,        FMT_4_4_4_4,
	FMT_5_5_5_1,        FMT_32,         FMT_32_FLOAT,       FMT_16_16,
	FMT_16_16_FLOAT=16, FMT_8_24,       FMT_8_24_FLOAT,     FMT_24_8,
	FMT_24_8_FLOAT,     FMT_10_11_11,   FMT_10_11_11_FLOAT, FMT_11_11_10,
	FMT_11_11_10_FLOAT, FMT_2_10_10_10, FMT_8_8_8_8,        FMT_10_10_10_2,
	FMT_X24_8_32_FLOAT, FMT_32_32,      FMT_32_32_FLOAT,    FMT_16_16_16_16,
	FMT_16_16_16_16_FLOAT=32,           FMT_32_32_32_32=34, FMT_32_32_32_32_FLOAT,
	                    FMT_1 = 37,                         FMT_GB_GR=39,
	FMT_BG_RG,          FMT_32_AS_8,    FMT_32_AS_8_8,      FMT_5_9_9_9_SHAREDEXP,
	FMT_8_8_8,          FMT_16_16_16,   FMT_16_16_16_FLOAT, FMT_32_32_32,
	FMT_32_32_32_FLOAT=48,

//  High level register file lengths
    SQ_FETCH_RESOURCE                                       = SQ_TEX_RESOURCE_WORD0_0,
    SQ_FETCH_RESOURCE_ps_num                                = 176,
    SQ_FETCH_RESOURCE_vs_num                                = 160,
    SQ_FETCH_RESOURCE_gs_num                                = 160,
    SQ_FETCH_RESOURCE_hs_num                                = 160,
    SQ_FETCH_RESOURCE_ls_num                                = 160,
    SQ_FETCH_RESOURCE_cs_num                                = 176,
    SQ_FETCH_RESOURCE_fs_num                                = 32,
    SQ_FETCH_RESOURCE_all_num                               = 1024,
    SQ_FETCH_RESOURCE_offset                                = 32,
    SQ_FETCH_RESOURCE_ps                                    = 0,                                               //   0...175
    SQ_FETCH_RESOURCE_vs                                    = SQ_FETCH_RESOURCE_ps + SQ_FETCH_RESOURCE_ps_num, // 176...335
    SQ_FETCH_RESOURCE_gs                                    = SQ_FETCH_RESOURCE_vs + SQ_FETCH_RESOURCE_vs_num, // 336...495
    SQ_FETCH_RESOURCE_hs                                    = SQ_FETCH_RESOURCE_gs + SQ_FETCH_RESOURCE_gs_num, // 496...655
    SQ_FETCH_RESOURCE_ls                                    = SQ_FETCH_RESOURCE_hs + SQ_FETCH_RESOURCE_hs_num, // 656...815
    SQ_FETCH_RESOURCE_cs                                    = SQ_FETCH_RESOURCE_ls + SQ_FETCH_RESOURCE_ls_num, // 816...991
    SQ_FETCH_RESOURCE_fs                                    = SQ_FETCH_RESOURCE_cs + SQ_FETCH_RESOURCE_cs_num, // 992...1023

    SQ_TEX_SAMPLER_WORD                                   = SQ_TEX_SAMPLER_WORD0_0,
    SQ_TEX_SAMPLER_WORD_ps_num                            = 18,
    SQ_TEX_SAMPLER_WORD_vs_num                            = 18,
    SQ_TEX_SAMPLER_WORD_gs_num                            = 18,
    SQ_TEX_SAMPLER_WORD_hs_num                            = 18,
    SQ_TEX_SAMPLER_WORD_ls_num                            = 18,
    SQ_TEX_SAMPLER_WORD_cs_num                            = 18,
    SQ_TEX_SAMPLER_WORD_all_num                           = 108,
    SQ_TEX_SAMPLER_WORD_offset                            = 12,
    SQ_TEX_SAMPLER_WORD_ps                                = 0,                                                   //  0...17
    SQ_TEX_SAMPLER_WORD_vs                                = SQ_TEX_SAMPLER_WORD_ps + SQ_TEX_SAMPLER_WORD_ps_num, // 18...35
    SQ_TEX_SAMPLER_WORD_gs                                = SQ_TEX_SAMPLER_WORD_vs + SQ_TEX_SAMPLER_WORD_vs_num, // 36...53
    SQ_TEX_SAMPLER_WORD_hs                                = SQ_TEX_SAMPLER_WORD_gs + SQ_TEX_SAMPLER_WORD_gs_num, // 54...71
    SQ_TEX_SAMPLER_WORD_ls                                = SQ_TEX_SAMPLER_WORD_hs + SQ_TEX_SAMPLER_WORD_hs_num, // 72...89
    SQ_TEX_SAMPLER_WORD_cs                                = SQ_TEX_SAMPLER_WORD_ls + SQ_TEX_SAMPLER_WORD_ls_num, // 90...107

    SQ_LOOP_CONST                                         = SQ_LOOP_CONST_0,
    SQ_LOOP_CONST_ps_num                                  = 32,
    SQ_LOOP_CONST_vs_num                                  = 32,
    SQ_LOOP_CONST_gs_num                                  = 32,
    SQ_LOOP_CONST_hs_num                                  = 32,
    SQ_LOOP_CONST_ls_num                                  = 32,
    SQ_LOOP_CONST_cs_num                                  = 32,
    SQ_LOOP_CONST_all_num                                 = 192,
    SQ_LOOP_CONST_offset                                  = 4,
    SQ_LOOP_CONST_ps                                      = 0,                                       //   0...31
    SQ_LOOP_CONST_vs                                      = SQ_LOOP_CONST_ps + SQ_LOOP_CONST_ps_num, //  32...63
    SQ_LOOP_CONST_gs                                      = SQ_LOOP_CONST_vs + SQ_LOOP_CONST_vs_num, //  64...95
    SQ_LOOP_CONST_hs                                      = SQ_LOOP_CONST_gs + SQ_LOOP_CONST_gs_num, //  96...127
    SQ_LOOP_CONST_ls                                      = SQ_LOOP_CONST_hs + SQ_LOOP_CONST_hs_num, // 128...159
    SQ_LOOP_CONST_cs                                      = SQ_LOOP_CONST_ls + SQ_LOOP_CONST_ls_num, // 160...191

    SQ_BOOL_CONST                                         = SQ_BOOL_CONST_0, /* 32 bits each */
    SQ_BOOL_CONST_ps_num                                  = 1,
    SQ_BOOL_CONST_vs_num                                  = 1,
    SQ_BOOL_CONST_gs_num                                  = 1,
    SQ_BOOL_CONST_hs_num                                  = 1,
    SQ_BOOL_CONST_ls_num                                  = 1,
    SQ_BOOL_CONST_cs_num                                  = 1,
    SQ_BOOL_CONST_all_num                                 = 6,
    SQ_BOOL_CONST_offset                                  = 4,
    SQ_BOOL_CONST_ps                                      = 0,
    SQ_BOOL_CONST_vs                                      = SQ_BOOL_CONST_ps + SQ_BOOL_CONST_ps_num,
    SQ_BOOL_CONST_gs                                      = SQ_BOOL_CONST_vs + SQ_BOOL_CONST_vs_num,
    SQ_BOOL_CONST_hs                                      = SQ_BOOL_CONST_gs + SQ_BOOL_CONST_gs_num,
    SQ_BOOL_CONST_ls                                      = SQ_BOOL_CONST_hs + SQ_BOOL_CONST_hs_num,
    SQ_BOOL_CONST_cs                                      = SQ_BOOL_CONST_ls + SQ_BOOL_CONST_ls_num,

};

#endif
