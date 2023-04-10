/*
 * Copyright (c) 1993-2003 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "nv_local.h"
#include "compiler.h"
#include "nv_include.h"


void NVLockUnlock (
    NVPtr pNv,
    Bool  Lock
)
{
    CARD8 cr11;

    VGA_WR08(pNv->PCIO, 0x3D4, 0x1F);
    VGA_WR08(pNv->PCIO, 0x3D5, Lock ? 0x99 : 0x57);

    VGA_WR08(pNv->PCIO, 0x3D4, 0x11);
    cr11 = VGA_RD08(pNv->PCIO, 0x3D5);
    if(Lock) cr11 |= 0x80;
    else cr11 &= ~0x80;
    VGA_WR08(pNv->PCIO, 0x3D5, cr11);
}

int NVShowHideCursor (
    NVPtr pNv,
    int   ShowHide
)
{
    int current = pNv->CurrentState->cursor1;

    pNv->CurrentState->cursor1 = (pNv->CurrentState->cursor1 & 0xFE) |
                                 (ShowHide & 0x01);
    VGA_WR08(pNv->PCIO, 0x3D4, 0x31);
    VGA_WR08(pNv->PCIO, 0x3D5, pNv->CurrentState->cursor1);

    if(pNv->Architecture == NV_ARCH_40) {  /* HW bug */
       volatile CARD32 curpos = pNv->PRAMDAC[0x0300/4];
       pNv->PRAMDAC[0x0300/4] = curpos;
    }

    return (current & 0x01);
}

/****************************************************************************\
*                                                                            *
* The video arbitration routines calculate some "magic" numbers.  Fixes      *
* the snow seen when accessing the framebuffer without it.                   *
* It just works (I hope).                                                    *
*                                                                            *
\****************************************************************************/

typedef struct {
  int graphics_lwm;
  int video_lwm;
  int graphics_burst_size;
  int video_burst_size;
  int valid;
} nv4_fifo_info;

typedef struct {
  int pclk_khz;
  int mclk_khz;
  int nvclk_khz;
  char mem_page_miss;
  char mem_latency;
  int memory_width;
  char enable_video;
  char gr_during_vid;
  char pix_bpp;
  char mem_aligned;
  char enable_mp;
} nv4_sim_state;

typedef struct {
  int graphics_lwm;
  int video_lwm;
  int graphics_burst_size;
  int video_burst_size;
  int valid;
} nv10_fifo_info;

typedef struct {
  int pclk_khz;
  int mclk_khz;
  int nvclk_khz;
  char mem_page_miss;
  char mem_latency;
  int memory_type;
  int memory_width;
  char enable_video;
  char gr_during_vid;
  char pix_bpp;
  char mem_aligned;
  char enable_mp;
} nv10_sim_state;


static void nvGetClocks(NVPtr pNv, unsigned int *MClk, unsigned int *NVClk)
{
    unsigned int pll, N, M, MB, NB, P;

    if(pNv->Architecture >= NV_ARCH_40) {
       pll = pNv->PMC[0x4020/4];
       P = (pll >> 16) & 0x07;
       pll = pNv->PMC[0x4024/4];
       M = pll & 0xFF;
       N = (pll >> 8) & 0xFF;
       if(((pNv->Chipset & 0xfff0) == 0x0290) ||
          ((pNv->Chipset & 0xfff0) == 0x0390))
       {
          MB = 1;
          NB = 1;
       } else {
          MB = (pll >> 16) & 0xFF;
          NB = (pll >> 24) & 0xFF;
       }
       *MClk = ((N * NB * pNv->CrystalFreqKHz) / (M * MB)) >> P;

       pll = pNv->PMC[0x4000/4];
       P = (pll >> 16) & 0x07;  
       pll = pNv->PMC[0x4004/4];
       M = pll & 0xFF;
       N = (pll >> 8) & 0xFF;
       MB = (pll >> 16) & 0xFF;
       NB = (pll >> 24) & 0xFF;

       *NVClk = ((N * NB * pNv->CrystalFreqKHz) / (M * MB)) >> P;
    } else
    if(pNv->twoStagePLL) {
       pll = pNv->PRAMDAC0[0x0504/4];
       M = pll & 0xFF; 
       N = (pll >> 8) & 0xFF; 
       P = (pll >> 16) & 0x0F;
       pll = pNv->PRAMDAC0[0x0574/4];
       if(pll & 0x80000000) {
           MB = pll & 0xFF; 
           NB = (pll >> 8) & 0xFF;
       } else {
           MB = 1;
           NB = 1;
       }
       *MClk = ((N * NB * pNv->CrystalFreqKHz) / (M * MB)) >> P;

       pll = pNv->PRAMDAC0[0x0500/4];
       M = pll & 0xFF; 
       N = (pll >> 8) & 0xFF; 
       P = (pll >> 16) & 0x0F;
       pll = pNv->PRAMDAC0[0x0570/4];
       if(pll & 0x80000000) {
           MB = pll & 0xFF;
           NB = (pll >> 8) & 0xFF;
       } else {
           MB = 1;
           NB = 1;
       }
       *NVClk = ((N * NB * pNv->CrystalFreqKHz) / (M * MB)) >> P;
    } else 
    if(((pNv->Chipset & 0x0ff0) == 0x0300) ||
       ((pNv->Chipset & 0x0ff0) == 0x0330))
    {
       pll = pNv->PRAMDAC0[0x0504/4];
       M = pll & 0x0F; 
       N = (pll >> 8) & 0xFF;
       P = (pll >> 16) & 0x07;
       if(pll & 0x00000080) {
           MB = (pll >> 4) & 0x07;     
           NB = (pll >> 19) & 0x1f;
       } else {
           MB = 1;
           NB = 1;
       }
       *MClk = ((N * NB * pNv->CrystalFreqKHz) / (M * MB)) >> P;

       pll = pNv->PRAMDAC0[0x0500/4];
       M = pll & 0x0F;
       N = (pll >> 8) & 0xFF;
       P = (pll >> 16) & 0x07;
       if(pll & 0x00000080) {
           MB = (pll >> 4) & 0x07;
           NB = (pll >> 19) & 0x1f;
       } else {
           MB = 1;
           NB = 1;
       }
       *NVClk = ((N * NB * pNv->CrystalFreqKHz) / (M * MB)) >> P;
    } else {
       pll = pNv->PRAMDAC0[0x0504/4];
       M = pll & 0xFF; 
       N = (pll >> 8) & 0xFF; 
       P = (pll >> 16) & 0x0F;
       *MClk = (N * pNv->CrystalFreqKHz / M) >> P;

       pll = pNv->PRAMDAC0[0x0500/4];
       M = pll & 0xFF; 
       N = (pll >> 8) & 0xFF; 
       P = (pll >> 16) & 0x0F;
       *NVClk = (N * pNv->CrystalFreqKHz / M) >> P;
    }

#if 0
    ErrorF("NVClock = %i MHz, MEMClock = %i MHz\n", *NVClk/1000, *MClk/1000);
#endif
}


static void nv4CalcArbitration (
    nv4_fifo_info *fifo,
    nv4_sim_state *arb
)
{
    int data, pagemiss, cas, width, video_enable, bpp;
    int nvclks, mclks, pclks, vpagemiss, crtpagemiss, vbs;
    int found, mclk_extra, mclk_loop, cbs, m1, p1;
    int mclk_freq, pclk_freq, nvclk_freq, mp_enable;
    int us_m, us_n, us_p, video_drain_rate, crtc_drain_rate;
    int vpm_us, us_video, vlwm, video_fill_us, cpm_us, us_crt, clwm;

    fifo->valid = 1;
    pclk_freq = arb->pclk_khz;
    mclk_freq = arb->mclk_khz;
    nvclk_freq = arb->nvclk_khz;
    pagemiss = arb->mem_page_miss;
    cas = arb->mem_latency;
    width = arb->memory_width >> 6;
    video_enable = arb->enable_video;
    bpp = arb->pix_bpp;
    mp_enable = arb->enable_mp;
    clwm = 0;
    vlwm = 0;
    cbs = 128;
    pclks = 2;
    nvclks = 2;
    nvclks += 2;
    nvclks += 1;
    mclks = 5;
    mclks += 3;
    mclks += 1;
    mclks += cas;
    mclks += 1;
    mclks += 1;
    mclks += 1;
    mclks += 1;
    mclk_extra = 3;
    nvclks += 2;
    nvclks += 1;
    nvclks += 1;
    nvclks += 1;
    if (mp_enable)
        mclks+=4;
    nvclks += 0;
    pclks += 0;
    found = 0;
    vbs = 0;
    while (found != 1)
    {
        fifo->valid = 1;
        found = 1;
        mclk_loop = mclks+mclk_extra;
        us_m = mclk_loop *1000*1000 / mclk_freq;
        us_n = nvclks*1000*1000 / nvclk_freq;
        us_p = nvclks*1000*1000 / pclk_freq;
        if (video_enable)
        {
            video_drain_rate = pclk_freq * 2;
            crtc_drain_rate = pclk_freq * bpp/8;
            vpagemiss = 2;
            vpagemiss += 1;
            crtpagemiss = 2;
            vpm_us = (vpagemiss * pagemiss)*1000*1000/mclk_freq;
            if (nvclk_freq * 2 > mclk_freq * width)
                video_fill_us = cbs*1000*1000 / 16 / nvclk_freq ;
            else
                video_fill_us = cbs*1000*1000 / (8 * width) / mclk_freq;
            us_video = vpm_us + us_m + us_n + us_p + video_fill_us;
            vlwm = us_video * video_drain_rate/(1000*1000);
            vlwm++;
            vbs = 128;
            if (vlwm > 128) vbs = 64;
            if (vlwm > (256-64)) vbs = 32;
            if (nvclk_freq * 2 > mclk_freq * width)
                video_fill_us = vbs *1000*1000/ 16 / nvclk_freq ;
            else
                video_fill_us = vbs*1000*1000 / (8 * width) / mclk_freq;
            cpm_us = crtpagemiss  * pagemiss *1000*1000/ mclk_freq;
            us_crt =
            us_video
            +video_fill_us
            +cpm_us
            +us_m + us_n +us_p
            ;
            clwm = us_crt * crtc_drain_rate/(1000*1000);
            clwm++;
        }
        else
        {
            crtc_drain_rate = pclk_freq * bpp/8;
            crtpagemiss = 2;
            crtpagemiss += 1;
            cpm_us = crtpagemiss  * pagemiss *1000*1000/ mclk_freq;
            us_crt =  cpm_us + us_m + us_n + us_p ;
            clwm = us_crt * crtc_drain_rate/(1000*1000);
            clwm++;
        }
        m1 = clwm + cbs - 512;
        p1 = m1 * pclk_freq / mclk_freq;
        p1 = p1 * bpp / 8;
        if ((p1 < m1) && (m1 > 0))
        {
            fifo->valid = 0;
            found = 0;
            if (mclk_extra ==0)   found = 1;
            mclk_extra--;
        }
        else if (video_enable)
        {
            if ((clwm > 511) || (vlwm > 255))
            {
                fifo->valid = 0;
                found = 0;
                if (mclk_extra ==0)   found = 1;
                mclk_extra--;
            }
        }
        else
        {
            if (clwm > 519)
            {
                fifo->valid = 0;
                found = 0;
                if (mclk_extra ==0)   found = 1;
                mclk_extra--;
            }
        }
        if (clwm < 384) clwm = 384;
        if (vlwm < 128) vlwm = 128;
        data = (int)(clwm);
        fifo->graphics_lwm = data;
        fifo->graphics_burst_size = 128;
        data = (int)((vlwm+15));
        fifo->video_lwm = data;
        fifo->video_burst_size = vbs;
    }
}

static void nv4UpdateArbitrationSettings (
    unsigned      VClk, 
    unsigned      pixelDepth, 
    unsigned     *burst,
    unsigned     *lwm,
    NVPtr        pNv
)
{
    nv4_fifo_info fifo_data;
    nv4_sim_state sim_data;
    unsigned int MClk, NVClk, cfg1;

    nvGetClocks(pNv, &MClk, &NVClk);

    cfg1 = pNv->PFB[0x00000204/4];
    sim_data.pix_bpp        = (char)pixelDepth;
    sim_data.enable_video   = 0;
    sim_data.enable_mp      = 0;
    sim_data.memory_width   = (pNv->PEXTDEV[0x0000/4] & 0x10) ? 128 : 64;
    sim_data.mem_latency    = (char)cfg1 & 0x0F;
    sim_data.mem_aligned    = 1;
    sim_data.mem_page_miss  = (char)(((cfg1 >> 4) &0x0F) + ((cfg1 >> 31) & 0x01));
    sim_data.gr_during_vid  = 0;
    sim_data.pclk_khz       = VClk;
    sim_data.mclk_khz       = MClk;
    sim_data.nvclk_khz      = NVClk;
    nv4CalcArbitration(&fifo_data, &sim_data);
    if (fifo_data.valid)
    {
        int  b = fifo_data.graphics_burst_size >> 4;
        *burst = 0;
        while (b >>= 1) (*burst)++;
        *lwm   = fifo_data.graphics_lwm >> 3;
    }
}

static void nv10CalcArbitration (
    nv10_fifo_info *fifo,
    nv10_sim_state *arb
)
{
    int data, pagemiss, width, video_enable, bpp;
    int nvclks, mclks, pclks, vpagemiss, crtpagemiss;
    int nvclk_fill;
    int found, mclk_extra, mclk_loop, cbs, m1;
    int mclk_freq, pclk_freq, nvclk_freq, mp_enable;
    int us_m, us_m_min, us_n, us_p, crtc_drain_rate;
    int vus_m;
    int vpm_us, us_video, cpm_us, us_crt,clwm;
    int clwm_rnd_down;
    int m2us, us_pipe_min, p1clk, p2;
    int min_mclk_extra;
    int us_min_mclk_extra;

    fifo->valid = 1;
    pclk_freq = arb->pclk_khz; /* freq in KHz */
    mclk_freq = arb->mclk_khz;
    nvclk_freq = arb->nvclk_khz;
    pagemiss = arb->mem_page_miss;
    width = arb->memory_width/64;
    video_enable = arb->enable_video;
    bpp = arb->pix_bpp;
    mp_enable = arb->enable_mp;
    clwm = 0;

    cbs = 512;

    pclks = 4; /* lwm detect. */

    nvclks = 3; /* lwm -> sync. */
    nvclks += 2; /* fbi bus cycles (1 req + 1 busy) */

    mclks  = 1;   /* 2 edge sync.  may be very close to edge so just put one. */

    mclks += 1;   /* arb_hp_req */
    mclks += 5;   /* ap_hp_req   tiling pipeline */

    mclks += 2;    /* tc_req     latency fifo */
    mclks += 2;    /* fb_cas_n_  memory request to fbio block */
    mclks += 7;    /* sm_d_rdv   data returned from fbio block */

    /* fb.rd.d.Put_gc   need to accumulate 256 bits for read */
    if (arb->memory_type == 0)
      if (arb->memory_width == 64) /* 64 bit bus */
        mclks += 4;
      else
        mclks += 2;
    else
      if (arb->memory_width == 64) /* 64 bit bus */
        mclks += 2;
      else
        mclks += 1;

    if ((!video_enable) && (arb->memory_width == 128))
    {  
      mclk_extra = (bpp == 32) ? 31 : 42; /* Margin of error */
      min_mclk_extra = 17;
    }
    else
    {
      mclk_extra = (bpp == 32) ? 8 : 4; /* Margin of error */
      /* mclk_extra = 4; */ /* Margin of error */
      min_mclk_extra = 18;
    }

    nvclks += 1; /* 2 edge sync.  may be very close to edge so just put one. */
    nvclks += 1; /* fbi_d_rdv_n */
    nvclks += 1; /* Fbi_d_rdata */
    nvclks += 1; /* crtfifo load */

    if(mp_enable)
      mclks+=4; /* Mp can get in with a burst of 8. */
    /* Extra clocks determined by heuristics */

    nvclks += 0;
    pclks += 0;
    found = 0;
    while(found != 1) {
      fifo->valid = 1;
      found = 1;
      mclk_loop = mclks+mclk_extra;
      us_m = mclk_loop *1000*1000 / mclk_freq; /* Mclk latency in us */
      us_m_min = mclks * 1000*1000 / mclk_freq; /* Minimum Mclk latency in us */
      us_min_mclk_extra = min_mclk_extra *1000*1000 / mclk_freq;
      us_n = nvclks*1000*1000 / nvclk_freq;/* nvclk latency in us */
      us_p = pclks*1000*1000 / pclk_freq;/* nvclk latency in us */
      us_pipe_min = us_m_min + us_n + us_p;

      vus_m = mclk_loop *1000*1000 / mclk_freq; /* Mclk latency in us */

      if(video_enable) {
        crtc_drain_rate = pclk_freq * bpp/8; /* MB/s */

        vpagemiss = 1; /* self generating page miss */
        vpagemiss += 1; /* One higher priority before */

        crtpagemiss = 2; /* self generating page miss */
        if(mp_enable)
            crtpagemiss += 1; /* if MA0 conflict */

        vpm_us = (vpagemiss * pagemiss)*1000*1000/mclk_freq;

        us_video = vpm_us + vus_m; /* Video has separate read return path */

        cpm_us = crtpagemiss  * pagemiss *1000*1000/ mclk_freq;
        us_crt =
          us_video  /* Wait for video */
          +cpm_us /* CRT Page miss */
          +us_m + us_n +us_p /* other latency */
          ;

        clwm = us_crt * crtc_drain_rate/(1000*1000);
        clwm++; /* fixed point <= float_point - 1.  Fixes that */
      } else {
        crtc_drain_rate = pclk_freq * bpp/8; /* bpp * pclk/8 */

        crtpagemiss = 1; /* self generating page miss */
        crtpagemiss += 1; /* MA0 page miss */
        if(mp_enable)
            crtpagemiss += 1; /* if MA0 conflict */
        cpm_us = crtpagemiss  * pagemiss *1000*1000/ mclk_freq;
        us_crt =  cpm_us + us_m + us_n + us_p ;
        clwm = us_crt * crtc_drain_rate/(1000*1000);
        clwm++; /* fixed point <= float_point - 1.  Fixes that */

          /* Finally, a heuristic check when width == 64 bits */
          if(width == 1){
              nvclk_fill = nvclk_freq * 8;
              if(crtc_drain_rate * 100 >= nvclk_fill * 102)
                      clwm = 0xfff; /*Large number to fail */

              else if(crtc_drain_rate * 100  >= nvclk_fill * 98) {
                  clwm = 1024;
                  cbs = 512;
              }
          }
      }


      /*
        Overfill check:

        */

      clwm_rnd_down = ((int)clwm/8)*8;
      if (clwm_rnd_down < clwm)
          clwm += 8;

      m1 = clwm + cbs -  1024; /* Amount of overfill */
      m2us = us_pipe_min + us_min_mclk_extra;

      /* pclk cycles to drain */
      p1clk = m2us * pclk_freq/(1000*1000); 
      p2 = p1clk * bpp / 8; /* bytes drained. */

      if((p2 < m1) && (m1 > 0)) {
          fifo->valid = 0;
          found = 0;
          if(min_mclk_extra == 0)   {
            if(cbs <= 32) {
              found = 1; /* Can't adjust anymore! */
            } else {
              cbs = cbs/2;  /* reduce the burst size */
            }
          } else {
            min_mclk_extra--;
          }
      } else {
        if (clwm > 1023){ /* Have some margin */
          fifo->valid = 0;
          found = 0;
          if(min_mclk_extra == 0)   
              found = 1; /* Can't adjust anymore! */
          else 
              min_mclk_extra--;
        }
      }

      if(clwm < (1024-cbs+8)) clwm = 1024-cbs+8;
      data = (int)(clwm);
      /*  printf("CRT LWM: %f bytes, prog: 0x%x, bs: 256\n", clwm, data ); */
      fifo->graphics_lwm = data;   fifo->graphics_burst_size = cbs;

      fifo->video_lwm = 1024;  fifo->video_burst_size = 512;
    }
}

static void nv10UpdateArbitrationSettings (
    unsigned      VClk, 
    unsigned      pixelDepth, 
    unsigned     *burst,
    unsigned     *lwm,
    NVPtr        pNv
)
{
    nv10_fifo_info fifo_data;
    nv10_sim_state sim_data;
    unsigned int MClk, NVClk, cfg1;

    nvGetClocks(pNv, &MClk, &NVClk);

    cfg1 = pNv->PFB[0x0204/4];
    sim_data.pix_bpp        = (char)pixelDepth;
    sim_data.enable_video   = 1;
    sim_data.enable_mp      = 0;
    sim_data.memory_type    = (pNv->PFB[0x0200/4] & 0x01) ? 1 : 0;
    sim_data.memory_width   = (pNv->PEXTDEV[0x0000/4] & 0x10) ? 128 : 64;
    sim_data.mem_latency    = (char)cfg1 & 0x0F;
    sim_data.mem_aligned    = 1;
    sim_data.mem_page_miss  = (char)(((cfg1>>4) &0x0F) + ((cfg1>>31) & 0x01));
    sim_data.gr_during_vid  = 0;
    sim_data.pclk_khz       = VClk;
    sim_data.mclk_khz       = MClk;
    sim_data.nvclk_khz      = NVClk;
    nv10CalcArbitration(&fifo_data, &sim_data);
    if (fifo_data.valid) {
        int  b = fifo_data.graphics_burst_size >> 4;
        *burst = 0;
        while (b >>= 1) (*burst)++;
        *lwm   = fifo_data.graphics_lwm >> 3;
    }
}


static void nv30UpdateArbitrationSettings (
    NVPtr        pNv,
    unsigned     *burst,
    unsigned     *lwm
)   
{
    unsigned int MClk, NVClk;
    unsigned int fifo_size, burst_size, graphics_lwm;

    fifo_size = 2048;
    burst_size = 512;
    graphics_lwm = fifo_size - burst_size;

    nvGetClocks(pNv, &MClk, &NVClk);
    
    *burst = 0;
    burst_size >>= 5;
    while(burst_size >>= 1) (*burst)++;
    *lwm = graphics_lwm >> 3;
}

#if XSERVER_LIBPCIACCESS
static inline uint32_t
pciaccessReadLong(struct pci_device *const dev, pciaddr_t offset) {
    uint32_t tmp;
    pci_device_cfg_read_u32(dev, &tmp, offset);
    return tmp;
}
#endif

static void nForceUpdateArbitrationSettings (
    unsigned      VClk,
    unsigned      pixelDepth,
    unsigned     *burst,
    unsigned     *lwm,
    NVPtr        pNv
)
{
#if XSERVER_LIBPCIACCESS
    struct pci_device *const dev1 = pci_device_find_by_slot(0, 0, 0, 1);
    struct pci_device *const dev2 = pci_device_find_by_slot(0, 0, 0, 2);
    struct pci_device *const dev3 = pci_device_find_by_slot(0, 0, 0, 3);
    struct pci_device *const dev5 = pci_device_find_by_slot(0, 0, 0, 5);
#   define READ_LONG(num, offset) pciaccessReadLong(dev##num, (offset))
#else
    #define READ_LONG(num, offset) pciReadLong(pciTag(0, 0, num), (offset))
#endif
    nv10_fifo_info fifo_data;
    nv10_sim_state sim_data;
    unsigned int M, N, P, pll, MClk, NVClk, memctrl;

    if((pNv->Chipset & 0x0FF0) == 0x01A0) {
       unsigned int uMClkPostDiv;

       uMClkPostDiv = (READ_LONG(3, 0x6C) >> 8) & 0xf;
       if(!uMClkPostDiv) uMClkPostDiv = 4; 
       MClk = 400000 / uMClkPostDiv;
    } else {
       MClk = READ_LONG(5, 0x4C) / 1000;
    }

    pll = pNv->PRAMDAC0[0x0500/4];
    M = (pll >> 0)  & 0xFF; N = (pll >> 8)  & 0xFF; P = (pll >> 16) & 0x0F;
    NVClk  = (N * pNv->CrystalFreqKHz / M) >> P;
    sim_data.pix_bpp        = (char)pixelDepth;
    sim_data.enable_video   = 0;
    sim_data.enable_mp      = 0;
    sim_data.memory_type    = (READ_LONG(1, 0x7C) >> 12) & 1;
    sim_data.memory_width   = 64;

    memctrl = READ_LONG(3, 0x00) >> 16;

    if((memctrl == 0x1A9) || (memctrl == 0x1AB) || (memctrl == 0x1ED)) {
        int dimm[3];

        dimm[0] = (READ_LONG(2, 0x40) >> 8) & 0x4F;
        dimm[1] = (READ_LONG(2, 0x44) >> 8) & 0x4F;
        dimm[2] = (READ_LONG(2, 0x48) >> 8) & 0x4F;

        if((dimm[0] + dimm[1]) != dimm[2]) {
             ErrorF("WARNING: "
              "your nForce DIMMs are not arranged in optimal banks!\n");
        } 
    }

    sim_data.mem_latency    = 3;
    sim_data.mem_aligned    = 1;
    sim_data.mem_page_miss  = 10;
    sim_data.gr_during_vid  = 0;
    sim_data.pclk_khz       = VClk;
    sim_data.mclk_khz       = MClk;
    sim_data.nvclk_khz      = NVClk;
    nv10CalcArbitration(&fifo_data, &sim_data);
    if (fifo_data.valid)
    {
        int  b = fifo_data.graphics_burst_size >> 4;
        *burst = 0;
        while (b >>= 1) (*burst)++;
        *lwm   = fifo_data.graphics_lwm >> 3;
    }

#undef READ_LONG
}


/****************************************************************************\
*                                                                            *
*                          RIVA Mode State Routines                          *
*                                                                            *
\****************************************************************************/

/*
 * Calculate the Video Clock parameters for the PLL.
 */
static void CalcVClock (
    int           clockIn,
    int          *clockOut,
    U032         *pllOut,
    NVPtr        pNv
)
{
    unsigned lowM, highM;
    unsigned DeltaNew, DeltaOld;
    unsigned VClk, Freq;
    unsigned M, N, P;
    
    DeltaOld = 0xFFFFFFFF;

    VClk = (unsigned)clockIn;
    
    if (pNv->CrystalFreqKHz == 13500) {
        lowM  = 7;
        highM = 13;
    } else {
        lowM  = 8;
        highM = 14;
    }

    for (P = 0; P <= 4; P++) {
        Freq = VClk << P;
        if ((Freq >= 128000) && (Freq <= 350000)) {
            for (M = lowM; M <= highM; M++) {
                N = ((VClk << P) * M) / pNv->CrystalFreqKHz;
                if(N <= 255) {
                    Freq = ((pNv->CrystalFreqKHz * N) / M) >> P;
                    if (Freq > VClk)
                        DeltaNew = Freq - VClk;
                    else
                        DeltaNew = VClk - Freq;
                    if (DeltaNew < DeltaOld) {
                        *pllOut   = (P << 16) | (N << 8) | M;
                        *clockOut = Freq;
                        DeltaOld  = DeltaNew;
                    }
                }
            }
        }
    }
}

static void CalcVClock2Stage (
    int           clockIn,
    int          *clockOut,
    U032         *pllOut,
    U032         *pllBOut,
    NVPtr        pNv
)
{
    unsigned DeltaNew, DeltaOld;
    unsigned VClk, Freq;
    unsigned M, N, P;

    DeltaOld = 0xFFFFFFFF;

    *pllBOut = 0x80000401;  /* fixed at x4 for now */

    VClk = (unsigned)clockIn;

    for (P = 0; P <= 6; P++) {
        Freq = VClk << P;
        if ((Freq >= 400000) && (Freq <= 1000000)) {
            for (M = 1; M <= 13; M++) {
                N = ((VClk << P) * M) / (pNv->CrystalFreqKHz << 2);
                if((N >= 5) && (N <= 255)) {
                    Freq = (((pNv->CrystalFreqKHz << 2) * N) / M) >> P;
                    if (Freq > VClk)
                        DeltaNew = Freq - VClk;
                    else
                        DeltaNew = VClk - Freq;
                    if (DeltaNew < DeltaOld) {
                        *pllOut   = (P << 16) | (N << 8) | M;
                        *clockOut = Freq;
                        DeltaOld  = DeltaNew;
                    }
                }
            }
        }
    }
}

/*
 * Calculate extended mode parameters (SVGA) and save in a 
 * mode state structure.
 */
void NVCalcStateExt (
    NVPtr pNv,
    RIVA_HW_STATE *state,
    int            bpp,
    int            width,
    int            hDisplaySize,
    int            height,
    int            dotClock,
    int		   flags 
)
{
    int pixelDepth, VClk = 0;
    /*
     * Save mode parameters.
     */
    state->bpp    = bpp;    /* this is not bitsPerPixel, it's 8,15,16,32 */
    state->width  = width;
    state->height = height;
    /*
     * Extended RIVA registers.
     */
    pixelDepth = (bpp + 1)/8;
    if(pNv->twoStagePLL)
        CalcVClock2Stage(dotClock, &VClk, &state->pll, &state->pllB, pNv);
    else
        CalcVClock(dotClock, &VClk, &state->pll, pNv);

    switch (pNv->Architecture)
    {
        case NV_ARCH_04:
            nv4UpdateArbitrationSettings(VClk, 
                                         pixelDepth * 8, 
                                        &(state->arbitration0),
                                        &(state->arbitration1),
                                         pNv);
            state->cursor0  = 0x00;
            state->cursor1  = 0xbC;
	    if (flags & V_DBLSCAN)
		state->cursor1 |= 2;
            state->cursor2  = 0x00000000;
            state->pllsel   = 0x10000700;
            state->config   = 0x00001114;
            state->general  = bpp == 16 ? 0x00101100 : 0x00100100;
            state->repaint1 = hDisplaySize < 1280 ? 0x04 : 0x00;
            break;
        case NV_ARCH_40:
            if(!pNv->FlatPanel)
                state->control = pNv->PRAMDAC0[0x0580/4] & 0xeffffeff;
            /* fallthrough */
        case NV_ARCH_10:
        case NV_ARCH_20:
        case NV_ARCH_30:
        default:
            if(((pNv->Chipset & 0xfff0) == 0x0240) ||
               ((pNv->Chipset & 0xfff0) == 0x03D0) ||
               ((pNv->Chipset & 0xfff0) == 0x0530))
            {
                state->arbitration0 = 128; 
                state->arbitration1 = 0x0480; 
            } else
            if(((pNv->Chipset & 0xffff) == 0x01A0) ||
               ((pNv->Chipset & 0xffff) == 0x01f0))
            {
                nForceUpdateArbitrationSettings(VClk,
                                          pixelDepth * 8,
                                         &(state->arbitration0),
                                         &(state->arbitration1),
                                          pNv);
            } else if(pNv->Architecture < NV_ARCH_30) {
                nv10UpdateArbitrationSettings(VClk, 
                                          pixelDepth * 8, 
                                         &(state->arbitration0),
                                         &(state->arbitration1),
                                          pNv);
            } else {
                nv30UpdateArbitrationSettings(pNv,
                                         &(state->arbitration0),
                                         &(state->arbitration1));
            }
            state->cursor0  = 0x80 | (pNv->CursorStart >> 17);
            state->cursor1  = (pNv->CursorStart >> 11) << 2;
	    state->cursor2  = pNv->CursorStart >> 24;
	    if (flags & V_DBLSCAN) 
		state->cursor1 |= 2;
            state->pllsel   = 0x10000700;
            state->config   = pNv->PFB[0x00000200/4];
            state->general  = bpp == 16 ? 0x00101100 : 0x00100100;
            state->repaint1 = hDisplaySize < 1280 ? 0x04 : 0x00;
            break;
    }

    if(bpp != 8) /* DirectColor */
	state->general |= 0x00000030;

    state->repaint0 = (((width / 8) * pixelDepth) & 0x700) >> 3;
    state->pixel    = (pixelDepth > 2) ? 3 : pixelDepth;
}


void NVLoadStateExt (
    NVPtr pNv,
    RIVA_HW_STATE *state
)
{
    int i, j;

    pNv->PMC[0x0140/4] = 0x00000000;
    pNv->PMC[0x0200/4] = 0xFFFF00FF;
    pNv->PMC[0x0200/4] = 0xFFFFFFFF;

    pNv->PTIMER[0x0200] = 0x00000008;
    pNv->PTIMER[0x0210] = 0x00000003;
    pNv->PTIMER[0x0140] = 0x00000000;
    pNv->PTIMER[0x0100] = 0xFFFFFFFF;

    if(pNv->Architecture == NV_ARCH_04) {
        if (state)
            pNv->PFB[0x0200/4] = state->config;
    } else 
    if((pNv->Architecture < NV_ARCH_40) ||
       ((pNv->Chipset & 0xfff0) == 0x0040))
    {
        for(i = 0; i < 8; i++) {
           pNv->PFB[(0x0240 + (i * 0x10))/4] = 0;
           pNv->PFB[(0x0244 + (i * 0x10))/4] = pNv->FbMapSize - 1;
        }
    } else {
        int regions = 12;

        if(((pNv->Chipset & 0xfff0) == 0x0090) ||
           ((pNv->Chipset & 0xfff0) == 0x01D0) ||
           ((pNv->Chipset & 0xfff0) == 0x0290) ||
           ((pNv->Chipset & 0xfff0) == 0x0390) ||
           ((pNv->Chipset & 0xfff0) == 0x03D0))
        {
           regions = 15;
        }
 
       for(i = 0; i < regions; i++) {
          pNv->PFB[(0x0600 + (i * 0x10))/4] = 0;
          pNv->PFB[(0x0604 + (i * 0x10))/4] = pNv->FbMapSize - 1;
       }
    }

    if(pNv->Architecture >= NV_ARCH_40) {
       pNv->PRAMIN[0x0000] = 0x80000010;
       pNv->PRAMIN[0x0001] = 0x00101202;
       pNv->PRAMIN[0x0002] = 0x80000011;
       pNv->PRAMIN[0x0003] = 0x00101204;
       pNv->PRAMIN[0x0004] = 0x80000012;
       pNv->PRAMIN[0x0005] = 0x00101206;
       pNv->PRAMIN[0x0006] = 0x80000013;
       pNv->PRAMIN[0x0007] = 0x00101208;
       pNv->PRAMIN[0x0008] = 0x80000014;
       pNv->PRAMIN[0x0009] = 0x0010120A;
       pNv->PRAMIN[0x000A] = 0x80000015;
       pNv->PRAMIN[0x000B] = 0x0010120C;
       pNv->PRAMIN[0x000C] = 0x80000016;
       pNv->PRAMIN[0x000D] = 0x0010120E;
       pNv->PRAMIN[0x000E] = 0x80000017;
       pNv->PRAMIN[0x000F] = 0x00101210;
       pNv->PRAMIN[0x0800] = 0x00003000;
       pNv->PRAMIN[0x0801] = pNv->FbMapSize - 1;
       pNv->PRAMIN[0x0802] = 0x00000002;
       pNv->PRAMIN[0x0808] = 0x02080062;
       pNv->PRAMIN[0x0809] = 0x00000000;
       pNv->PRAMIN[0x080A] = 0x00001200;
       pNv->PRAMIN[0x080B] = 0x00001200;
       pNv->PRAMIN[0x080C] = 0x00000000;
       pNv->PRAMIN[0x080D] = 0x00000000;
       pNv->PRAMIN[0x0810] = 0x02080043;
       pNv->PRAMIN[0x0811] = 0x00000000;
       pNv->PRAMIN[0x0812] = 0x00000000;
       pNv->PRAMIN[0x0813] = 0x00000000;
       pNv->PRAMIN[0x0814] = 0x00000000;
       pNv->PRAMIN[0x0815] = 0x00000000;
       pNv->PRAMIN[0x0818] = 0x02080044;
       pNv->PRAMIN[0x0819] = 0x02000000;
       pNv->PRAMIN[0x081A] = 0x00000000;
       pNv->PRAMIN[0x081B] = 0x00000000;
       pNv->PRAMIN[0x081C] = 0x00000000;
       pNv->PRAMIN[0x081D] = 0x00000000;
       pNv->PRAMIN[0x0820] = 0x02080019;
       pNv->PRAMIN[0x0821] = 0x00000000;
       pNv->PRAMIN[0x0822] = 0x00000000;
       pNv->PRAMIN[0x0823] = 0x00000000;
       pNv->PRAMIN[0x0824] = 0x00000000;
       pNv->PRAMIN[0x0825] = 0x00000000;
       pNv->PRAMIN[0x0828] = 0x020A005C;
       pNv->PRAMIN[0x0829] = 0x00000000;
       pNv->PRAMIN[0x082A] = 0x00000000;
       pNv->PRAMIN[0x082B] = 0x00000000;
       pNv->PRAMIN[0x082C] = 0x00000000;
       pNv->PRAMIN[0x082D] = 0x00000000;
       pNv->PRAMIN[0x0830] = 0x0208009F;
       pNv->PRAMIN[0x0831] = 0x00000000;
       pNv->PRAMIN[0x0832] = 0x00001200;
       pNv->PRAMIN[0x0833] = 0x00001200;
       pNv->PRAMIN[0x0834] = 0x00000000;
       pNv->PRAMIN[0x0835] = 0x00000000;
       pNv->PRAMIN[0x0838] = 0x0208004A;
       pNv->PRAMIN[0x0839] = 0x02000000;
       pNv->PRAMIN[0x083A] = 0x00000000;
       pNv->PRAMIN[0x083B] = 0x00000000;
       pNv->PRAMIN[0x083C] = 0x00000000;
       pNv->PRAMIN[0x083D] = 0x00000000;
       pNv->PRAMIN[0x0840] = 0x02080077;
       pNv->PRAMIN[0x0841] = 0x00000000;
       pNv->PRAMIN[0x0842] = 0x00001200;
       pNv->PRAMIN[0x0843] = 0x00001200;
       pNv->PRAMIN[0x0844] = 0x00000000;
       pNv->PRAMIN[0x0845] = 0x00000000;
       pNv->PRAMIN[0x084C] = 0x00003002;
       pNv->PRAMIN[0x084D] = 0x00007FFF;
       pNv->PRAMIN[0x084E] = pNv->FbUsableSize | 0x00000002;

#if X_BYTE_ORDER == X_BIG_ENDIAN
       pNv->PRAMIN[0x080A] |= 0x01000000;
       pNv->PRAMIN[0x0812] |= 0x01000000;
       pNv->PRAMIN[0x081A] |= 0x01000000;
       pNv->PRAMIN[0x0822] |= 0x01000000;
       pNv->PRAMIN[0x082A] |= 0x01000000;
       pNv->PRAMIN[0x0832] |= 0x01000000;
       pNv->PRAMIN[0x083A] |= 0x01000000;
       pNv->PRAMIN[0x0842] |= 0x01000000;  
       pNv->PRAMIN[0x0819] = 0x01000000;
       pNv->PRAMIN[0x0839] = 0x01000000;
#endif
    } else {
       pNv->PRAMIN[0x0000] = 0x80000010;
       pNv->PRAMIN[0x0001] = 0x80011201;  
       pNv->PRAMIN[0x0002] = 0x80000011;
       pNv->PRAMIN[0x0003] = 0x80011202; 
       pNv->PRAMIN[0x0004] = 0x80000012;
       pNv->PRAMIN[0x0005] = 0x80011203;
       pNv->PRAMIN[0x0006] = 0x80000013;
       pNv->PRAMIN[0x0007] = 0x80011204;
       pNv->PRAMIN[0x0008] = 0x80000014;
       pNv->PRAMIN[0x0009] = 0x80011205;
       pNv->PRAMIN[0x000A] = 0x80000015;
       pNv->PRAMIN[0x000B] = 0x80011206;
       pNv->PRAMIN[0x000C] = 0x80000016;
       pNv->PRAMIN[0x000D] = 0x80011207;
       pNv->PRAMIN[0x000E] = 0x80000017;
       pNv->PRAMIN[0x000F] = 0x80011208;
       pNv->PRAMIN[0x0800] = 0x00003000;
       pNv->PRAMIN[0x0801] = pNv->FbMapSize - 1;
       pNv->PRAMIN[0x0802] = 0x00000002;
       pNv->PRAMIN[0x0803] = 0x00000002;
       if(pNv->Architecture >= NV_ARCH_10)
          pNv->PRAMIN[0x0804] = 0x01008062;
       else
          pNv->PRAMIN[0x0804] = 0x01008042;
       pNv->PRAMIN[0x0805] = 0x00000000;
       pNv->PRAMIN[0x0806] = 0x12001200;
       pNv->PRAMIN[0x0807] = 0x00000000;
       pNv->PRAMIN[0x0808] = 0x01008043;
       pNv->PRAMIN[0x0809] = 0x00000000;
       pNv->PRAMIN[0x080A] = 0x00000000;
       pNv->PRAMIN[0x080B] = 0x00000000;
       pNv->PRAMIN[0x080C] = 0x01008044;
       pNv->PRAMIN[0x080D] = 0x00000002;
       pNv->PRAMIN[0x080E] = 0x00000000;
       pNv->PRAMIN[0x080F] = 0x00000000;
       pNv->PRAMIN[0x0810] = 0x01008019;
       pNv->PRAMIN[0x0811] = 0x00000000;
       pNv->PRAMIN[0x0812] = 0x00000000;
       pNv->PRAMIN[0x0813] = 0x00000000;
       pNv->PRAMIN[0x0814] = 0x0100A05C;
       pNv->PRAMIN[0x0815] = 0x00000000;
       pNv->PRAMIN[0x0816] = 0x00000000;
       pNv->PRAMIN[0x0817] = 0x00000000;
       if(pNv->WaitVSyncPossible)
          pNv->PRAMIN[0x0818] = 0x0100809F;
       else
          pNv->PRAMIN[0x0818] = 0x0100805F;
       pNv->PRAMIN[0x0819] = 0x00000000;
       pNv->PRAMIN[0x081A] = 0x12001200;
       pNv->PRAMIN[0x081B] = 0x00000000;
       pNv->PRAMIN[0x081C] = 0x0100804A;
       pNv->PRAMIN[0x081D] = 0x00000002;
       pNv->PRAMIN[0x081E] = 0x00000000;
       pNv->PRAMIN[0x081F] = 0x00000000;
       pNv->PRAMIN[0x0820] = 0x01018077;
       pNv->PRAMIN[0x0821] = 0x00000000;
       pNv->PRAMIN[0x0822] = 0x12001200;
       pNv->PRAMIN[0x0823] = 0x00000000;
       pNv->PRAMIN[0x0824] = 0x00003002;
       pNv->PRAMIN[0x0825] = 0x00007FFF;
       pNv->PRAMIN[0x0826] = pNv->FbUsableSize | 0x00000002;
       pNv->PRAMIN[0x0827] = 0x00000002;

#if X_BYTE_ORDER == X_BIG_ENDIAN
       pNv->PRAMIN[0x0804] |= 0x00080000;
       pNv->PRAMIN[0x0808] |= 0x00080000;
       pNv->PRAMIN[0x080C] |= 0x00080000;
       pNv->PRAMIN[0x0810] |= 0x00080000;
       pNv->PRAMIN[0x0814] |= 0x00080000;
       pNv->PRAMIN[0x0818] |= 0x00080000;
       pNv->PRAMIN[0x081C] |= 0x00080000;
       pNv->PRAMIN[0x0820] |= 0x00080000;
       pNv->PRAMIN[0x080D] = 0x00000001;
       pNv->PRAMIN[0x081D] = 0x00000001;
#endif
    }

    if(pNv->Architecture < NV_ARCH_10) {
       if((pNv->Chipset & 0x0fff) == 0x0020) {
           pNv->PRAMIN[0x0824] |= 0x00020000;
           pNv->PRAMIN[0x0826] += pNv->FbAddress;
       }
       pNv->PGRAPH[0x0080/4] = 0x000001FF;
       pNv->PGRAPH[0x0080/4] = 0x1230C000;
       pNv->PGRAPH[0x0084/4] = 0x72111101;
       pNv->PGRAPH[0x0088/4] = 0x11D5F071;
       pNv->PGRAPH[0x008C/4] = 0x0004FF31;
       pNv->PGRAPH[0x008C/4] = 0x4004FF31;

       pNv->PGRAPH[0x0140/4] = 0x00000000;
       pNv->PGRAPH[0x0100/4] = 0xFFFFFFFF;
       pNv->PGRAPH[0x0170/4] = 0x10010100;
       pNv->PGRAPH[0x0710/4] = 0xFFFFFFFF;
       pNv->PGRAPH[0x0720/4] = 0x00000001;

       pNv->PGRAPH[0x0810/4] = 0x00000000;
       pNv->PGRAPH[0x0608/4] = 0xFFFFFFFF; 
    } else {
       pNv->PGRAPH[0x0080/4] = 0xFFFFFFFF;
       pNv->PGRAPH[0x0080/4] = 0x00000000;

       pNv->PGRAPH[0x0140/4] = 0x00000000;
       pNv->PGRAPH[0x0100/4] = 0xFFFFFFFF;
       pNv->PGRAPH[0x0144/4] = 0x10010100;
       pNv->PGRAPH[0x0714/4] = 0xFFFFFFFF;
       pNv->PGRAPH[0x0720/4] = 0x00000001;
       pNv->PGRAPH[0x0710/4] &= 0x0007ff00;
       pNv->PGRAPH[0x0710/4] |= 0x00020100;

       if(pNv->Architecture == NV_ARCH_10) {
           pNv->PGRAPH[0x0084/4] = 0x00118700;
           pNv->PGRAPH[0x0088/4] = 0x24E00810;
           pNv->PGRAPH[0x008C/4] = 0x55DE0030;

           for(i = 0; i < 32; i++)
             pNv->PGRAPH[(0x0B00/4) + i] = pNv->PFB[(0x0240/4) + i];

           pNv->PGRAPH[0x640/4] = 0;
           pNv->PGRAPH[0x644/4] = 0;
           pNv->PGRAPH[0x684/4] = pNv->FbMapSize - 1;
           pNv->PGRAPH[0x688/4] = pNv->FbMapSize - 1;

           pNv->PGRAPH[0x0810/4] = 0x00000000;
           pNv->PGRAPH[0x0608/4] = 0xFFFFFFFF;
       } else {
           if(pNv->Architecture >= NV_ARCH_40) {
              pNv->PGRAPH[0x0084/4] = 0x401287c0;
              pNv->PGRAPH[0x008C/4] = 0x60de8051;
              pNv->PGRAPH[0x0090/4] = 0x00008000;
              pNv->PGRAPH[0x0610/4] = 0x00be3c5f;
              pNv->PGRAPH[0x0bc4/4] |= 0x00008000;

              j = pNv->REGS[0x1540/4] & 0xff;
              if(j) {
                  for(i = 0; !(j & 1); j >>= 1, i++);
                  pNv->PGRAPH[0x5000/4] = i;
              }

              if((pNv->Chipset & 0xfff0) == 0x0040) {
                 pNv->PGRAPH[0x09b0/4] = 0x83280fff;
                 pNv->PGRAPH[0x09b4/4] = 0x000000a0;
              } else {
                 pNv->PGRAPH[0x0820/4] = 0x83280eff;
                 pNv->PGRAPH[0x0824/4] = 0x000000a0;
              }

              switch(pNv->Chipset & 0xfff0) {
              case 0x0040:
              case 0x0210:
                 pNv->PGRAPH[0x09b8/4] = 0x0078e366;
                 pNv->PGRAPH[0x09bc/4] = 0x0000014c;
                 pNv->PFB[0x033C/4] &= 0xffff7fff;
                 break;
              case 0x00C0:
              case 0x0120:
                 pNv->PGRAPH[0x0828/4] = 0x007596ff;
                 pNv->PGRAPH[0x082C/4] = 0x00000108;
                 break;
              case 0x0160:
              case 0x01D0:
              case 0x0240:
              case 0x03D0:
              case 0x0530:
                 pNv->PMC[0x1700/4] = pNv->PFB[0x020C/4];
                 pNv->PMC[0x1704/4] = 0;
                 pNv->PMC[0x1708/4] = 0;
                 pNv->PMC[0x170C/4] = pNv->PFB[0x020C/4];
                 pNv->PGRAPH[0x0860/4] = 0;
                 pNv->PGRAPH[0x0864/4] = 0;
                 pNv->PRAMDAC[0x0608/4] |= 0x00100000;
                 break;
              case 0x0140:
                 pNv->PGRAPH[0x0828/4] = 0x0072cb77;
                 pNv->PGRAPH[0x082C/4] = 0x00000108;
                 break;
              case 0x0220:
                 pNv->PGRAPH[0x0860/4] = 0;
                 pNv->PGRAPH[0x0864/4] = 0;
                 pNv->PRAMDAC[0x0608/4] |= 0x00100000;
                 break;
              case 0x0090:
              case 0x0290:
              case 0x0390:
                 pNv->PRAMDAC[0x0608/4] |= 0x00100000;
                 pNv->PGRAPH[0x0828/4] = 0x07830610;
                 pNv->PGRAPH[0x082C/4] = 0x0000016A;
                 break;
              default:
                 break;
              };

              pNv->PGRAPH[0x0b38/4] = 0x2ffff800;
              pNv->PGRAPH[0x0b3c/4] = 0x00006000;
              pNv->PGRAPH[0x032C/4] = 0x01000000; 
              pNv->PGRAPH[0x0220/4] = 0x00001200;
           } else
           if(pNv->Architecture == NV_ARCH_30) {
              pNv->PGRAPH[0x0084/4] = 0x40108700;
              pNv->PGRAPH[0x0890/4] = 0x00140000;
              pNv->PGRAPH[0x008C/4] = 0xf00e0431;
              pNv->PGRAPH[0x0090/4] = 0x00008000;
              pNv->PGRAPH[0x0610/4] = 0xf04b1f36;
              pNv->PGRAPH[0x0B80/4] = 0x1002d888;
              pNv->PGRAPH[0x0B88/4] = 0x62ff007f;
           } else {
              pNv->PGRAPH[0x0084/4] = 0x00118700;
              pNv->PGRAPH[0x008C/4] = 0xF20E0431;
              pNv->PGRAPH[0x0090/4] = 0x00000000;
              pNv->PGRAPH[0x009C/4] = 0x00000040;

              if((pNv->Chipset & 0x0ff0) >= 0x0250) {
                 pNv->PGRAPH[0x0890/4] = 0x00080000;
                 pNv->PGRAPH[0x0610/4] = 0x304B1FB6; 
                 pNv->PGRAPH[0x0B80/4] = 0x18B82880; 
                 pNv->PGRAPH[0x0B84/4] = 0x44000000; 
                 pNv->PGRAPH[0x0098/4] = 0x40000080; 
                 pNv->PGRAPH[0x0B88/4] = 0x000000ff; 
              } else {
                 pNv->PGRAPH[0x0880/4] = 0x00080000;
                 pNv->PGRAPH[0x0094/4] = 0x00000005;
                 pNv->PGRAPH[0x0B80/4] = 0x45CAA208; 
                 pNv->PGRAPH[0x0B84/4] = 0x24000000;
                 pNv->PGRAPH[0x0098/4] = 0x00000040;
                 pNv->PGRAPH[0x0750/4] = 0x00E00038;
                 pNv->PGRAPH[0x0754/4] = 0x00000030;
                 pNv->PGRAPH[0x0750/4] = 0x00E10038;
                 pNv->PGRAPH[0x0754/4] = 0x00000030;
              }
           }

           if((pNv->Architecture < NV_ARCH_40) ||
              ((pNv->Chipset & 0xfff0) == 0x0040)) 
           {
              for(i = 0; i < 32; i++) {
                pNv->PGRAPH[(0x0900/4) + i] = pNv->PFB[(0x0240/4) + i];
                pNv->PGRAPH[(0x6900/4) + i] = pNv->PFB[(0x0240/4) + i];
              }
           } else {
              if(((pNv->Chipset & 0xfff0) == 0x0090) ||
                 ((pNv->Chipset & 0xfff0) == 0x01D0) ||
                 ((pNv->Chipset & 0xfff0) == 0x0290) ||
                 ((pNv->Chipset & 0xfff0) == 0x0390) ||
                 ((pNv->Chipset & 0xfff0) == 0x03D0))
              {
                 for(i = 0; i < 60; i++) {
                   pNv->PGRAPH[(0x0D00/4) + i] = pNv->PFB[(0x0600/4) + i];
                   pNv->PGRAPH[(0x6900/4) + i] = pNv->PFB[(0x0600/4) + i];
                 }
              } else {
                 for(i = 0; i < 48; i++) {
                   pNv->PGRAPH[(0x0900/4) + i] = pNv->PFB[(0x0600/4) + i];
                   if(((pNv->Chipset & 0xfff0) != 0x0160) &&
                      ((pNv->Chipset & 0xfff0) != 0x0220) &&
                      ((pNv->Chipset & 0xfff0) != 0x0240) &&
                      ((pNv->Chipset & 0xfff0) != 0x0530))
                   {
                      pNv->PGRAPH[(0x6900/4) + i] = pNv->PFB[(0x0600/4) + i];
                   }
                 }
              }
           }

           if(pNv->Architecture >= NV_ARCH_40) {
              if((pNv->Chipset & 0xfff0) == 0x0040) {
                 pNv->PGRAPH[0x09A4/4] = pNv->PFB[0x0200/4];
                 pNv->PGRAPH[0x09A8/4] = pNv->PFB[0x0204/4];
                 pNv->PGRAPH[0x69A4/4] = pNv->PFB[0x0200/4];
                 pNv->PGRAPH[0x69A8/4] = pNv->PFB[0x0204/4];

                 pNv->PGRAPH[0x0820/4] = 0;
                 pNv->PGRAPH[0x0824/4] = 0;
                 pNv->PGRAPH[0x0864/4] = pNv->FbMapSize - 1;
                 pNv->PGRAPH[0x0868/4] = pNv->FbMapSize - 1;
              } else {
                 if(((pNv->Chipset & 0xfff0) == 0x0090) ||
                    ((pNv->Chipset & 0xfff0) == 0x01D0) ||
                    ((pNv->Chipset & 0xfff0) == 0x0290) ||
                    ((pNv->Chipset & 0xfff0) == 0x0390)) 
                 {
                    pNv->PGRAPH[0x0DF0/4] = pNv->PFB[0x0200/4];
                    pNv->PGRAPH[0x0DF4/4] = pNv->PFB[0x0204/4];
                 } else {
                    pNv->PGRAPH[0x09F0/4] = pNv->PFB[0x0200/4];
                    pNv->PGRAPH[0x09F4/4] = pNv->PFB[0x0204/4];
                 }
                 pNv->PGRAPH[0x69F0/4] = pNv->PFB[0x0200/4];
                 pNv->PGRAPH[0x69F4/4] = pNv->PFB[0x0204/4];

                 pNv->PGRAPH[0x0840/4] = 0;
                 pNv->PGRAPH[0x0844/4] = 0;
                 pNv->PGRAPH[0x08a0/4] = pNv->FbMapSize - 1;
                 pNv->PGRAPH[0x08a4/4] = pNv->FbMapSize - 1;
              }
           } else {
              pNv->PGRAPH[0x09A4/4] = pNv->PFB[0x0200/4];
              pNv->PGRAPH[0x09A8/4] = pNv->PFB[0x0204/4];
              pNv->PGRAPH[0x0750/4] = 0x00EA0000;
              pNv->PGRAPH[0x0754/4] = pNv->PFB[0x0200/4];
              pNv->PGRAPH[0x0750/4] = 0x00EA0004;
              pNv->PGRAPH[0x0754/4] = pNv->PFB[0x0204/4];

              pNv->PGRAPH[0x0820/4] = 0;
              pNv->PGRAPH[0x0824/4] = 0;
              pNv->PGRAPH[0x0864/4] = pNv->FbMapSize - 1;
              pNv->PGRAPH[0x0868/4] = pNv->FbMapSize - 1;
           }

           pNv->PGRAPH[0x0B20/4] = 0x00000000;
           pNv->PGRAPH[0x0B04/4] = 0xFFFFFFFF;
       }
    }
    pNv->PGRAPH[0x053C/4] = 0;
    pNv->PGRAPH[0x0540/4] = 0;
    pNv->PGRAPH[0x0544/4] = 0x00007FFF;
    pNv->PGRAPH[0x0548/4] = 0x00007FFF;

    pNv->PFIFO[0x0140] = 0x00000000;
    pNv->PFIFO[0x0141] = 0x00000001;
    pNv->PFIFO[0x0480] = 0x00000000;
    pNv->PFIFO[0x0494] = 0x00000000;
    if(pNv->Architecture >= NV_ARCH_40)
       pNv->PFIFO[0x0481] = 0x00010000;
    else
       pNv->PFIFO[0x0481] = 0x00000100;
    pNv->PFIFO[0x0490] = 0x00000000;
    pNv->PFIFO[0x0491] = 0x00000000;
    if(pNv->Architecture >= NV_ARCH_40)
       pNv->PFIFO[0x048B] = 0x00001213;
    else
       pNv->PFIFO[0x048B] = 0x00001209;
    pNv->PFIFO[0x0400] = 0x00000000;
    pNv->PFIFO[0x0414] = 0x00000000;
    pNv->PFIFO[0x0084] = 0x03000100;
    pNv->PFIFO[0x0085] = 0x00000110;
    pNv->PFIFO[0x0086] = 0x00000112;
    pNv->PFIFO[0x0143] = 0x0000FFFF;
    pNv->PFIFO[0x0496] = 0x0000FFFF;
    pNv->PFIFO[0x0050] = 0x00000000;
    pNv->PFIFO[0x0040] = 0xFFFFFFFF;
    pNv->PFIFO[0x0415] = 0x00000001;
    pNv->PFIFO[0x048C] = 0x00000000;
    pNv->PFIFO[0x04A0] = 0x00000000;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    pNv->PFIFO[0x0489] = 0x800F0078;
#else
    pNv->PFIFO[0x0489] = 0x000F0078;
#endif
    pNv->PFIFO[0x0488] = 0x00000001;
    pNv->PFIFO[0x0480] = 0x00000001;
    pNv->PFIFO[0x0494] = 0x00000001;
    pNv->PFIFO[0x0495] = 0x00000001;
    pNv->PFIFO[0x0140] = 0x00000001;

    if(!state) {
        pNv->CurrentState = NULL;
        return;
    }

    if(pNv->Architecture >= NV_ARCH_10) {
        if(pNv->twoHeads) {
           pNv->PCRTC0[0x0860/4] = state->head;
           pNv->PCRTC0[0x2860/4] = state->head2;
        }
        pNv->PRAMDAC[0x0404/4] |= (1 << 25);
    
        pNv->PMC[0x8704/4] = 1;
        pNv->PMC[0x8140/4] = 0;
        pNv->PMC[0x8920/4] = 0;
        pNv->PMC[0x8924/4] = 0;
        pNv->PMC[0x8908/4] = pNv->FbMapSize - 1;
        pNv->PMC[0x890C/4] = pNv->FbMapSize - 1;
        pNv->PMC[0x1588/4] = 0;

        pNv->PCRTC[0x0810/4] = state->cursorConfig;
        pNv->PCRTC[0x0830/4] = state->displayV - 3;
        pNv->PCRTC[0x0834/4] = state->displayV - 1;
    
        if(pNv->FlatPanel) {
           if((pNv->Chipset & 0x0ff0) == 0x0110) {
               pNv->PRAMDAC[0x0528/4] = state->dither;
           } else 
           if(pNv->twoHeads) {
               pNv->PRAMDAC[0x083C/4] = state->dither;
           }
    
           VGA_WR08(pNv->PCIO, 0x03D4, 0x53);
           VGA_WR08(pNv->PCIO, 0x03D5, state->timingH);
           VGA_WR08(pNv->PCIO, 0x03D4, 0x54);
           VGA_WR08(pNv->PCIO, 0x03D5, state->timingV);
           VGA_WR08(pNv->PCIO, 0x03D4, 0x21);
           VGA_WR08(pNv->PCIO, 0x03D5, 0xfa);
        }

        VGA_WR08(pNv->PCIO, 0x03D4, 0x41);
        VGA_WR08(pNv->PCIO, 0x03D5, state->extra);
    }

    VGA_WR08(pNv->PCIO, 0x03D4, 0x19);
    VGA_WR08(pNv->PCIO, 0x03D5, state->repaint0);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x1A);
    VGA_WR08(pNv->PCIO, 0x03D5, state->repaint1);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x25);
    VGA_WR08(pNv->PCIO, 0x03D5, state->screen);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x28);
    VGA_WR08(pNv->PCIO, 0x03D5, state->pixel);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x2D);
    VGA_WR08(pNv->PCIO, 0x03D5, state->horiz);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x1C);
    VGA_WR08(pNv->PCIO, 0x03D5, state->fifo);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x1B);
    VGA_WR08(pNv->PCIO, 0x03D5, state->arbitration0);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x20);
    VGA_WR08(pNv->PCIO, 0x03D5, state->arbitration1);
    if(pNv->Architecture >= NV_ARCH_30) {
      VGA_WR08(pNv->PCIO, 0x03D4, 0x47);
      VGA_WR08(pNv->PCIO, 0x03D5, state->arbitration1 >> 8);
    }
    VGA_WR08(pNv->PCIO, 0x03D4, 0x30);
    VGA_WR08(pNv->PCIO, 0x03D5, state->cursor0);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x31);
    VGA_WR08(pNv->PCIO, 0x03D5, state->cursor1);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x2F);
    VGA_WR08(pNv->PCIO, 0x03D5, state->cursor2);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x39);
    VGA_WR08(pNv->PCIO, 0x03D5, state->interlace);

    if(!pNv->FlatPanel) {
       if(pNv->Architecture >= NV_ARCH_40) {
           pNv->PRAMDAC0[0x0580/4] = state->control;
       }

       pNv->PRAMDAC0[0x050C/4] = state->pllsel;
       pNv->PRAMDAC0[0x0508/4] = state->vpll;
       if(pNv->twoHeads)
          pNv->PRAMDAC0[0x0520/4] = state->vpll2;
       if(pNv->twoStagePLL) {
          pNv->PRAMDAC0[0x0578/4] = state->vpllB;
          pNv->PRAMDAC0[0x057C/4] = state->vpll2B;
       }
    } else {
       pNv->PRAMDAC[0x0848/4] = state->scale;
       pNv->PRAMDAC[0x0828/4] = state->crtcSync;
       pNv->PRAMDAC[0x0808/4] = state->crtcVSync;
    }
    pNv->PRAMDAC[0x0600/4] = state->general;

    pNv->PCRTC[0x0140/4] = 0;
    pNv->PCRTC[0x0100/4] = 1;

    pNv->CurrentState = state;
}

void NVUnloadStateExt
(
    NVPtr pNv,
    RIVA_HW_STATE *state
)
{
    VGA_WR08(pNv->PCIO, 0x03D4, 0x19);
    state->repaint0     = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x1A);
    state->repaint1     = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x25);
    state->screen       = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x28);
    state->pixel        = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x2D);
    state->horiz        = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x1C);
    state->fifo         = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x1B);
    state->arbitration0 = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x20);
    state->arbitration1 = VGA_RD08(pNv->PCIO, 0x03D5);
    if(pNv->Architecture >= NV_ARCH_30) {
       VGA_WR08(pNv->PCIO, 0x03D4, 0x47);
       state->arbitration1 |= (VGA_RD08(pNv->PCIO, 0x03D5) & 1) << 8;
    }
    VGA_WR08(pNv->PCIO, 0x03D4, 0x30);
    state->cursor0      = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x31);
    state->cursor1      = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x2F);
    state->cursor2      = VGA_RD08(pNv->PCIO, 0x03D5);
    VGA_WR08(pNv->PCIO, 0x03D4, 0x39);
    state->interlace    = VGA_RD08(pNv->PCIO, 0x03D5);
    state->vpll         = pNv->PRAMDAC0[0x0508/4];
    if(pNv->twoHeads)
       state->vpll2     = pNv->PRAMDAC0[0x0520/4];
    if(pNv->twoStagePLL) {
        state->vpllB    = pNv->PRAMDAC0[0x0578/4];
        state->vpll2B   = pNv->PRAMDAC0[0x057C/4];
    }
    state->pllsel       = pNv->PRAMDAC0[0x050C/4];
    state->general      = pNv->PRAMDAC[0x0600/4];
    state->scale        = pNv->PRAMDAC[0x0848/4];
    state->config       = pNv->PFB[0x0200/4];

    if(pNv->Architecture >= NV_ARCH_40 && !pNv->FlatPanel) {
        state->control  = pNv->PRAMDAC0[0x0580/4];
    }

    if(pNv->Architecture >= NV_ARCH_10) {
        if(pNv->twoHeads) {
           state->head     = pNv->PCRTC0[0x0860/4];
           state->head2    = pNv->PCRTC0[0x2860/4];
           VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
           state->crtcOwner = VGA_RD08(pNv->PCIO, 0x03D5);
        }
        VGA_WR08(pNv->PCIO, 0x03D4, 0x41);
        state->extra = VGA_RD08(pNv->PCIO, 0x03D5);
        state->cursorConfig = pNv->PCRTC[0x0810/4];

        if((pNv->Chipset & 0x0ff0) == 0x0110) {
           state->dither = pNv->PRAMDAC[0x0528/4];
        } else 
        if(pNv->twoHeads) {
            state->dither = pNv->PRAMDAC[0x083C/4];
        }

        if(pNv->FlatPanel) {
           VGA_WR08(pNv->PCIO, 0x03D4, 0x53);
           state->timingH = VGA_RD08(pNv->PCIO, 0x03D5);
           VGA_WR08(pNv->PCIO, 0x03D4, 0x54);
           state->timingV = VGA_RD08(pNv->PCIO, 0x03D5);
        }
    }

    if(pNv->FlatPanel) {
       state->crtcSync = pNv->PRAMDAC[0x0828/4];
       state->crtcVSync = pNv->PRAMDAC[0x0808/4];
    }
}

void NVSetStartAddress (
    NVPtr   pNv,
    CARD32 start
)
{
    if (pNv->VBEDualhead) {
        pNv->PCRTC0[0x800/4] = start;
        pNv->PCRTC0[0x2800/4] = start + pNv->vbeCRTC1Offset;
    } else {
        pNv->PCRTC[0x800/4] = start;
    }
}
