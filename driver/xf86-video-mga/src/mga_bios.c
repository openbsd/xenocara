/*
 * (C) Copyright IBM Corporation 2005
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * IBM AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * \file mga_bios.c
 * Routines for processing the PInS data stored in the MGA BIOS.
 *
 * The structure of this code was inspired by similar routines in the Linux
 * kernel matroxfb code.  Specifically, the routines in matroxfb_misc.c.  In
 * addition, that code was used in place of documentation about the structure
 * of the PInS data for non-G450 cards.
 *
 * \author Ian Romanick <idr@us.ibm.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* All drivers should typically include these */
#include "xf86.h"
#include "xf86_OSproc.h"

/* All drivers need this */

#include "compiler.h"

/* Drivers that need to access the PCI config space directly need this */
#include "xf86Pci.h"

#include <X11/Xmd.h>

#include "mga.h"

#if defined(DEBUG)
#define BIOS_DEBUG
#endif

/**
 * Read a little-endian, unaligned data value and return as 16-bit.
 */
static __inline__ CARD16 get_u16( const CARD8 * data )
{
    CARD16 temp;
    
    temp = data[1];
    temp <<= 8;
    temp += data[0];
    return temp;
}


/**
 * Read a little-endian, unaligned data value and return as 32-bit.
 */
static __inline__ CARD32 get_u32( const CARD8 * data )
{
    CARD32 temp;


    temp = data[3];
    temp <<= 8;
    temp += data[2];
    temp <<= 8;
    temp += data[1];
    temp <<= 8;
    temp += data[0];

    return temp;
}


/**
 * Parse version 0x01XX of the BIOS PInS structure.
 * 
 * Version 0x01XX of the BIOS PInS structure is only found in Millenium cards.
 *
 * \todo
 * There used to be an "OverclockMem" option that would scale the memory clock
 * by 12 instead 10.  Add support for this back in.
 */
static void mga_parse_bios_ver_1( struct mga_bios_values * bios, 
				  const CARD8 * bios_data )
{
    unsigned maxdac;

    if ( get_u16( & bios_data[24] ) ) {
	maxdac = get_u16( & bios_data[24] ) * 10;
    }
    else {
	/* There is some disagreement here between the Linux kernel matroxfb
	 * driver and the old X.org mga driver.  matroxfb has case-statements
	 * for 0 and 1 (with the values below), and the mga driver has
	 * case-statements for 1 and 2 (with the values below).  The default
	 * value for the mga driver is 17500, but the default value for the
	 * matroxfb driver is 240000.
	 */

	switch( bios_data[22] ) {
	case 0:  maxdac = 175000; break;
	case 1:  maxdac = 220000; break;
	case 2:  maxdac = 250000; break;
	default: maxdac = 240000; break;
	}
    }
    
    if ( get_u16( & bios_data[28] ) ) {
	bios->mem_clock = get_u16( & bios_data[28] ) * 10;
    }

    if ( (bios_data[48] & 0x01) == 0 ) {
	bios->fast_bitblt = TRUE;
    }

    bios->pixel.max_freq = maxdac;
}


/**
 * Parse version 0x02XX of the BIOS PInS structure.
 * 
 * Version 0x02XX of the BIOS PInS structure is only found in Millenium II
 * and Mystique cards.
 */
static void mga_parse_bios_ver_2( struct mga_bios_values * bios, 
				  const CARD8 * bios_data )
{
    if ( bios_data[41] != 0xff ) {
	const unsigned maxdac = (bios_data[41] + 100) * 1000;

	bios->pixel.max_freq = maxdac;
	bios->system.max_freq = maxdac;
    }

    if ( bios_data[43] != 0xff ) {
	const unsigned system_pll = (bios_data[43] + 100) * 1000;
	bios->mem_clock = system_pll;
    }
}


/**
 * Parse version 0x03XX of the BIOS PInS structure.
 * 
 * Version 0x03XX of the BIOS PInS structure is only found in G100 and G200
 * cards.
 */
static void mga_parse_bios_ver_3( struct mga_bios_values * bios, 
				  const CARD8 * bios_data )
{
    if ( bios_data[36] != 0xff ) {
	const unsigned maxdac = (bios_data[36] + 100) * 1000;

	bios->pixel.max_freq = maxdac;
	bios->system.max_freq = maxdac;
    }

    if ( (bios_data[52] & 0x20) != 0 ) {
	bios->pll_ref_freq = 14318;
    }
}


/**
 * Parse version 0x04XX of the BIOS PInS structure.
 * 
 * Version 0x04XX of the BIOS PInS structure is only found in G400 cards.
 */
static void mga_parse_bios_ver_4( struct mga_bios_values * bios, 
				  const CARD8 * bios_data )
{
    if ( bios_data[39] != 0xff ) {
	const unsigned maxdac = bios_data[39] * 4 * 1000;

	bios->pixel.max_freq = maxdac;
	bios->system.max_freq = maxdac;
    }

    if ( bios_data[38] != 0xff ) {
	const unsigned maxdac = bios_data[38] * 4 * 1000;

	bios->system.max_freq = maxdac;
    }

    if ( (bios_data[92] & 0x01) != 0 ) {
	bios->pll_ref_freq = 14318;
    }

    bios->host_interface = (bios_data[95] >> 3) & 0x07;

    if ( bios_data[65] != 0xff ) {
	const unsigned system_pll = bios_data[65] * 4 * 1000;
	bios->mem_clock = system_pll;
    }
}


/**
 * Parse version 0x05XX of the BIOS PInS structure.
 * 
 * Version 0x05XX of the BIOS PInS structure is only found in G450 and G550
 * cards.
 */
static void mga_parse_bios_ver_5( struct mga_bios_values * bios, 
				  const CARD8 * bios_data )
{
    const unsigned scale = (bios_data[4] != 0) ? 8000 : 6000;


    if ( bios_data[38] != 0xff ) {
	const unsigned maxdac = bios_data[38] * scale;

	bios->pixel.max_freq = maxdac;
	bios->system.max_freq = maxdac;
	bios->video.max_freq = maxdac;
    }

    if ( bios_data[36] != 0xff ) {
	const unsigned maxdac = bios_data[36] * scale;

	bios->system.max_freq = maxdac;
	bios->video.max_freq = maxdac;
    }

    if ( bios_data[37] != 0xff ) {
	const unsigned maxdac = bios_data[37] * scale;

	bios->video.max_freq = maxdac;
    }


    if ( bios_data[123] != 0xff ) {
	const unsigned mindac = bios_data[123] * scale;

	bios->pixel.min_freq = mindac;
	bios->system.min_freq = mindac;
	bios->video.min_freq = mindac;
    }

    if ( bios_data[121] != 0xff ) {
	const unsigned mindac = bios_data[121] * scale;

	bios->system.min_freq = mindac;
	bios->video.min_freq = mindac;
    }

    if ( bios_data[122] != 0xff ) {
	const unsigned mindac = bios_data[122] * scale;

	bios->video.min_freq = mindac;
    }


    if ( bios_data[92] != 0xff ) {
	const unsigned system_pll = bios_data[92] * 4 * 1000;
	bios->mem_clock = system_pll;
    }

    if ( (bios_data[110] & 0x01) != 0 ) {
	bios->pll_ref_freq = 14318;
    }

    bios->host_interface = (bios_data[113] >> 3) & 0x07;
}


/**
 * Read the BIOS data from the card and initialize internal values.
 */

Bool mga_read_and_process_bios( ScrnInfoPtr pScrn )
{
    CARD8  bios_data[0x20000];
    unsigned offset;
    MGAPtr pMga = MGAPTR(pScrn);
#ifndef XSERVER_LIBPCIACCESS
    Bool pciBIOS = TRUE;
#endif
    int rlen;
    static const unsigned expected_length[] = { 0, 64, 64, 64, 128, 128 };
    unsigned version;
    unsigned pins_len;
    const CARD8 * pins_data;
    int err;
#ifdef BIOS_DEBUG
    static const char * const host_interface_strings[8] = {
	"Reserved",
	"Reserved",
	"Reserved",
	"Hybrid (AGP 4x on data transfers only)",
	"PCI",
	"AGP 1x",
	"AGP 2x",
	"AGP 4x"
    };
#endif


    /* Initialize the stored BIOS data to some reasonable values for the
     * card at hand.  This is done now so that even if the PInS data block
     * isn't found or can't be read we'll still have some reasonable values
     * to use.
     */
    if (pMga->chip_attribs)
	(void) memcpy(& pMga->bios, & pMga->chip_attribs->default_bios_values,
                      sizeof(struct mga_bios_values));


    /* If the BIOS address was probed, it was found from the PCI config space
     * If it was given in the config file, try to guess when it looks like it
     * might be controlled by the PCI config space.
     */

#ifdef XSERVER_LIBPCIACCESS
    err = pci_device_read_rom(pMga->PciInfo, bios_data);
#else
    if (pMga->BiosFrom == X_DEFAULT) {
	pciBIOS = FALSE;
    }
    else if (pMga->BiosFrom == X_CONFIG && pMga->BiosAddress < 0x100000) {
	pciBIOS = TRUE;
    }

    if (pciBIOS) {
	rlen = xf86ReadPciBIOS(0, pMga->PciTag, pMga->framebuffer_bar,
			       bios_data, sizeof(bios_data));
    }
    else {
	rlen = xf86ReadDomainMemory(pMga->PciTag, pMga->BiosAddress,
				    sizeof(bios_data), bios_data);
    }

    err = rlen < (bios_data[2] << 9);
#endif

    if (err) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Could not retrieve video BIOS!\n");
	return FALSE;
    }
        
    /* Get the output mode set by the BIOS */
    pMga->BiosOutputMode = bios_data[0x7ff1];

    /* Get the video BIOS info block */
    if (strncmp((char *)(&bios_data[45]), "MATROX", 6)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Video BIOS info block not detected!\n");
	return FALSE;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* The offset information that is included in the BIOS on PC Matrox cards
     * is not there on PowerPC cards.  Instead, we have to search the BIOS
     * image for the magic signature.  This is the 16-bit value 0x412d.  This
     * value is followed by the length of the PInS block.  We know that this
     * must (currently) be either 0x80 or 0x40.
     * 
     * Happy hunting.
     */
    for (offset = 0 ; offset < 0x7ffc ; offset++) {
	if ((bios_data[offset] == 0x2e) && (bios_data[offset+1] == 0x41)
	    && ((bios_data[offset+2] == 0x80) || (bios_data[offset+2] == 0x40))) {
	    break;
	}
    }

    if (offset == 0x7ffc) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Video BIOS PInS data not found!\n");
	return FALSE;
    }
#else
    /* Get the info block offset */
    offset = (unsigned)((bios_data[0x7ffd] << 8) | bios_data[0x7ffc]);
#endif

    /* Let the world know what we are up to */
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	       "Video BIOS info block at offset 0x%05lX\n",
	       (long)(offset));

    
    /* Determine the version of the PInS block.  This will determine how the
     * data is processed.  Only the first version of the PInS data structure
     * did *NOT* have the initial 0x412e (in little-endian order!) signature.
     */
    
    pins_data = & bios_data[ offset ];
    if ( (pins_data[0] == 0x2e) && (pins_data[1] == 0x41) ) {
	version = pins_data[5];
	pins_len = pins_data[2];
    }
    else {
	version = 1;
	pins_len = get_u16( pins_data );
    }
    
    
    if ( (version < 1) || (version > 5) ) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "PInS data version (%u) not supported.\n", version);
	return FALSE;
    }

    if ( pins_len != expected_length[ version ] ) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "PInS data length (%u) does not match expected length (%u)"
		   " for version %u.X.\n",
		   pins_len, expected_length[ version ], version);
	return FALSE;
    }
    
    switch( version ) {
    case 1:  mga_parse_bios_ver_1( & pMga->bios, pins_data ); break;
    case 2:  mga_parse_bios_ver_2( & pMga->bios, pins_data ); break;
    case 3:  mga_parse_bios_ver_3( & pMga->bios, pins_data ); break;
    case 4:  mga_parse_bios_ver_4( & pMga->bios, pins_data ); break;
    case 5:  mga_parse_bios_ver_5( & pMga->bios, pins_data ); break;
    }
    
#ifdef BIOS_DEBUG
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "system VCO = [%u, %u]\n",
	       pMga->bios.system.min_freq, pMga->bios.system.max_freq);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "pixel VCO  = [%u, %u]\n",
	       pMga->bios.pixel.min_freq,  pMga->bios.pixel.max_freq);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "video VCO  = [%u, %u]\n",
	       pMga->bios.video.min_freq,  pMga->bios.video.max_freq);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "memory clock = %ukHz\n", pMga->bios.mem_clock);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "PLL reference frequency = %ukHz\n",
	       pMga->bios.pll_ref_freq);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "%s fast bitblt\n",
	       (pMga->bios.fast_bitblt) ? "Has" : "Does not have");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Host interface: %s (%u)\n",
	       host_interface_strings[ pMga->bios.host_interface ],
	       pMga->bios.host_interface);
#endif

    return TRUE;
}
