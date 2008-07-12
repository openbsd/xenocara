/*
 * Copyright 1997 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef ___ATICHIP_H___
#define ___ATICHIP_H___ 1

#include "atipriv.h"
#include "atiregs.h"

#include <X11/Xmd.h>

/*
 * Chip-related definitions.
 */
typedef enum
{
    ATI_CHIP_NONE = 0,
    ATI_CHIP_88800GXC,          /* Mach64 */
    ATI_CHIP_88800GXD,          /* Mach64 */
    ATI_CHIP_88800GXE,          /* Mach64 */
    ATI_CHIP_88800GXF,          /* Mach64 */
    ATI_CHIP_88800GX,           /* Mach64 */
    ATI_CHIP_88800CX,           /* Mach64 */
    ATI_CHIP_264CT,             /* Mach64 */
    ATI_CHIP_264ET,             /* Mach64 */
    ATI_CHIP_264VT,             /* Mach64 */
    ATI_CHIP_264GT,             /* Mach64 */
    ATI_CHIP_264VTB,            /* Mach64 */
    ATI_CHIP_264GTB,            /* Mach64 */
    ATI_CHIP_264VT3,            /* Mach64 */
    ATI_CHIP_264GTDVD,          /* Mach64 */
    ATI_CHIP_264LT,             /* Mach64 */
    ATI_CHIP_264VT4,            /* Mach64 */
    ATI_CHIP_264GT2C,           /* Mach64 */
    ATI_CHIP_264GTPRO,          /* Mach64 */
    ATI_CHIP_264LTPRO,          /* Mach64 */
    ATI_CHIP_264XL,             /* Mach64 */
    ATI_CHIP_MOBILITY,          /* Mach64 */
    ATI_CHIP_Mach64,            /* Last among Mach64's */
} ATIChipType;

/*
 * Foundry codes for 264xT's.
 */
typedef enum
{
    ATI_FOUNDRY_SGS,    /* SGS-Thompson */
    ATI_FOUNDRY_NEC,    /* NEC */
    ATI_FOUNDRY_KSC,    /* KSC (?) */
    ATI_FOUNDRY_UMC,    /* United Microelectronics Corporation */
    ATI_FOUNDRY_TSMC,   /* Taiwan Semiconductor Manufacturing Company */
    ATI_FOUNDRY_5,
    ATI_FOUNDRY_6,
    ATI_FOUNDRY_UMCA    /* UMC alternate */
} ATIFoundryType;

extern const char *ATIFoundryNames[];

extern void        ATIMach64ChipID(ATIPtr, const CARD16);

#define OldChipID(_1, _0) \
    (SetBits(_0 - 'A', CHIP_CODE_0) | SetBits(_1 - 'A', CHIP_CODE_1))

#define NewChipID(_1, _0) \
    (SetBits(_0, CFG_CHIP_TYPE0) | SetBits(_1, CFG_CHIP_TYPE1))

#define OldToNewChipID(_ChipID) \
    (SetBits(GetBits(_ChipID, CHIP_CODE_0) + 'A', CFG_CHIP_TYPE0) | \
     SetBits(GetBits(_ChipID, CHIP_CODE_1) + 'A', CFG_CHIP_TYPE1))

#define NewToOldChipID(_ChipID) \
    (SetBits(GetBits(_ChipID, CFG_CHIP_TYPE0) - 'A', CHIP_CODE_0) | \
    (SetBits(GetBits(_ChipID, CFG_CHIP_TYPE1) - 'A', CHIP_CODE_1))

#endif /* ___ATICHIP_H___ */
