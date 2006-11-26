/* 
 * impact.h 2005/07/15 01:40:13, Copyright (c) 2005 peter fuerst
 *
 * Based on:
 * # newport_cmap.c,v 1.1 2000/11/29 20:58:10 agx Exp #
 * # xc/programs/Xserver/hw/xfree86/drivers/newport/newport_cmap.c,v
 *   1.2 2001/11/23 19:50:45 dawes Exp #
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "impact.h"

/*
unsigned ImpactGetPalReg(ImpactPtr pImpact, int i)
{
	return pImpact->pseudo_palette[i & 0xff];
}
*/

/* Load a colormap into the "hardware" */
void ImpactLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
			LOCO* colors, VisualPtr pVisual)
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);

	TRACEV("ImpactLoadPalette(%d)  %c", numColors, numColors!=1?'\n':0);
	if (numColors > 256) numColors = 256;

	for ( ; numColors > 0; numColors--, indices++, colors++) {
		unsigned rgb = colors->blue & 0xff00;
		rgb = (rgb<<8) | (colors->green & 0xff00) | (colors->red>>8);
		pImpact->pseudo_palette[*indices] = rgb;
		TRACEV("%d:%04x  %04x,%04x,%04x\n", *indices, rgb,
			(int)colors->red, (int)colors->green, (int)colors->blue);
	}
	TRACEV("\n");
}

void ImpactBackupPalette(ScrnInfoPtr pScrn)
{
	(void)IMPACTPTR(pScrn)->pseudo_palette;	/* Nothing to do? */
}

/* restore the default colormap */
void ImpactRestorePalette(ScrnInfoPtr pScrn)
{
	(void)IMPACTPTR(pScrn)->pseudo_palette;	/* Nothing to do? */
}

