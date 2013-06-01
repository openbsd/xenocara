/*
 * Attribute setting for the Creator and Creator3D framebuffer.
 *
 * Copyright (C) 1999 David S. Miller (davem@redhat.com)
 * Copyright (C) 1999 Jakub Jelinek (jakub@redhat.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * JAKUB JELINEK OR DAVID MILLER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ffb.h"
#include "ffb_fifo.h"
#include "ffb_rcache.h"

/* If we're going to write any attributes, write them all. */
void __FFB_Attr_Raw(FFBPtr pFfb, unsigned int ppc, unsigned int ppc_mask,
		    unsigned int pmask, unsigned int rop,
		    int drawop, int fg, unsigned int fbc, unsigned int wid)
{
	ffb_fbcPtr ffb = pFfb->regs;

	FFBLOG(("WRATTRS_RAW: PPC[%08x:%08x] PMSK[%08x] ROP[%08x] DOP[%08x] FG[%08x] FBC[%08x] WID[%02x]\n",
		ppc, ppc_mask, pmask, rop, drawop, fg, fbc, wid));
	pFfb->ppc_cache &= ~ppc_mask;
	pFfb->ppc_cache |= ppc;
	pFfb->fg_cache = fg;
	pFfb->fbc_cache = fbc;
	pFfb->wid_cache = wid;
	pFfb->rop_cache = rop;
	pFfb->pmask_cache = pmask;
	pFfb->drawop_cache = drawop;
	pFfb->rp_active = 1;
	FFBFifo(pFfb, 7);
	ffb->ppc = ppc;
	ffb->fg = fg;
	ffb->fbc = fbc;
	ffb->wid = wid;
	ffb->rop = rop;
	ffb->pmask = pmask;
	ffb->drawop = drawop;
}

void __FFB_Attr_SFB_VAR(FFBPtr pFfb, unsigned int ppc, unsigned int ppc_mask, unsigned int fbc,
			unsigned int wid, unsigned int rop, unsigned int pmask)
{
	ffb_fbcPtr ffb = pFfb->regs;

	FFBLOG(("WRATTRS_SFBVAR: PPC[%08x:%08x] PMSK[%08x] ROP[%08x] FBC[%08x] WID[%02x]\n",
		ppc, ppc_mask, pmask, rop, fbc, wid));
	pFfb->ppc_cache &= ~ppc_mask;
	pFfb->ppc_cache |= ppc;
	pFfb->fbc_cache = fbc;
	pFfb->wid_cache = wid;
	pFfb->rop_cache = rop;
	pFfb->pmask_cache = pmask;
	pFfb->rp_active = 1;
	FFBFifo(pFfb, 5);
	ffb->ppc = ppc;
	ffb->fbc = fbc;
	ffb->wid = wid;
	ffb->rop = rop;
	ffb->pmask = pmask;
}
