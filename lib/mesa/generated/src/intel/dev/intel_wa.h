/*
 * Copyright © 2023 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#ifndef INTEL_WA_H
#define INTEL_WA_H

#include "util/macros.h"

#ifdef __cplusplus
extern "C" {
#endif

struct intel_device_info;
void intel_device_info_init_was(struct intel_device_info *devinfo);

enum intel_wa_steppings {
   INTEL_STEPPING_A0,
   INTEL_STEPPING_B0,
   INTEL_STEPPING_RELEASE
};

enum intel_workaround_id {
   INTEL_WA_220579888,
   INTEL_WA_1207137018,
   INTEL_WA_1406306137,
   INTEL_WA_1406479881,
   INTEL_WA_1406614636,
   INTEL_WA_1406631448,
   INTEL_WA_1406697149,
   INTEL_WA_1406756463,
   INTEL_WA_1406950495,
   INTEL_WA_1407240128,
   INTEL_WA_1407385565,
   INTEL_WA_1407391552,
   INTEL_WA_1407520876,
   INTEL_WA_1407528679,
   INTEL_WA_1407685933,
   INTEL_WA_1408224581,
   INTEL_WA_1408264532,
   INTEL_WA_1408615042,
   INTEL_WA_1408937953,
   INTEL_WA_1409392000,
   INTEL_WA_1409433168,
   INTEL_WA_1409600907,
   INTEL_WA_1505013527,
   INTEL_WA_1507384622,
   INTEL_WA_1508701464,
   INTEL_WA_1508744258,
   INTEL_WA_1509820217,
   INTEL_WA_1604061319,
   INTEL_WA_1604366864,
   INTEL_WA_1604608133,
   INTEL_WA_1605967699,
   INTEL_WA_1606376872,
   INTEL_WA_1606932921,
   INTEL_WA_1607225878,
   INTEL_WA_1607446692,
   INTEL_WA_1607610283,
   INTEL_WA_1607854226,
   INTEL_WA_1607956946,
   INTEL_WA_1608127078,
   INTEL_WA_1805811773,
   INTEL_WA_1806527549,
   INTEL_WA_1806565034,
   INTEL_WA_2201039848,
   INTEL_WA_14010013414,
   INTEL_WA_14010017096,
   INTEL_WA_14010239330,
   INTEL_WA_14010357979,
   INTEL_WA_14010595310,
   INTEL_WA_14010755945,
   INTEL_WA_14010899839,
   INTEL_WA_14010945292,
   INTEL_WA_14012437816,
   INTEL_WA_14012688258,
   INTEL_WA_14012865646,
   INTEL_WA_14013111325,
   INTEL_WA_14013745556,
   INTEL_WA_14014063774,
   INTEL_WA_14014148106,
   INTEL_WA_14014176256,
   INTEL_WA_14014414195,
   INTEL_WA_14014427904,
   INTEL_WA_14014595444,
   INTEL_WA_14014890652,
   INTEL_WA_14015297576,
   INTEL_WA_14015360373,
   INTEL_WA_14015360517,
   INTEL_WA_14015420481,
   INTEL_WA_14015465469,
   INTEL_WA_14015528146,
   INTEL_WA_14015590813,
   INTEL_WA_14015808183,
   INTEL_WA_14015842950,
   INTEL_WA_14015907227,
   INTEL_WA_14015965466,
   INTEL_WA_14016118574,
   INTEL_WA_14016243945,
   INTEL_WA_14016755692,
   INTEL_WA_14016880151,
   INTEL_WA_14016939504,
   INTEL_WA_14017076903,
   INTEL_WA_14017153641,
   INTEL_WA_14017171315,
   INTEL_WA_14017333800,
   INTEL_WA_14017341140,
   INTEL_WA_14017517122,
   INTEL_WA_14017880152,
   INTEL_WA_15010599737,
   INTEL_WA_16010655327,
   INTEL_WA_16011107343,
   INTEL_WA_16011411144,
   INTEL_WA_16011448509,
   INTEL_WA_16011627967,
   INTEL_WA_16011773973,
   INTEL_WA_16011969663,
   INTEL_WA_16012292205,
   INTEL_WA_16012351604,
   INTEL_WA_16012775297,
   INTEL_WA_16013000631,
   INTEL_WA_16013063087,
   INTEL_WA_16014390852,
   INTEL_WA_16014538804,
   INTEL_WA_16016080423,
   INTEL_WA_16016462106,
   INTEL_WA_16016616742,
   INTEL_WA_16016618273,
   INTEL_WA_16016772977,
   INTEL_WA_18012201914,
   INTEL_WA_18013179988,
   INTEL_WA_18018999230,
   INTEL_WA_22011186057,
   INTEL_WA_22011440098,
   INTEL_WA_22012244936,
   INTEL_WA_22012575642,
   INTEL_WA_22012725308,
   INTEL_WA_22012766191,
   INTEL_WA_22012785325,
   INTEL_WA_22012962271,
   INTEL_WA_22013073587,
   INTEL_WA_22013689345,
   INTEL_WA_22014272202,
   INTEL_WA_22014344549,
   INTEL_WA_22014559856,
   INTEL_WA_22015224714,
   INTEL_WA_22015761040,
   INTEL_WA_NUM
};

/* These defines are used to identify when a workaround potentially applies
 * in genxml code.  They should not be used directly. intel_needs_workaround()
 * checks these definitions to eliminate bitset tests at compile time.
 */
#define INTEL_WA_220579888_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1207137018_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1406306137_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1406479881_GFX_VER ((GFX_VERx10 == 110) || (GFX_VERx10 == 120))
#define INTEL_WA_1406614636_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1406631448_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1406697149_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1406756463_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1406950495_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1407240128_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1407385565_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1407391552_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1407520876_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1407528679_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1407685933_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1408224581_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1408264532_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1408615042_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1408937953_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1409392000_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1409433168_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1409600907_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1505013527_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1507384622_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1508701464_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1508744258_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1509820217_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_1604061319_GFX_VER ((GFX_VERx10 == 110) || (GFX_VERx10 == 120))
#define INTEL_WA_1604366864_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1604608133_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1605967699_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1606376872_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1606932921_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1607225878_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1607446692_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1607610283_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1607854226_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1607956946_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1608127078_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_1805811773_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_1806527549_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_1806565034_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_2201039848_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_14010013414_GFX_VER ((GFX_VERx10 == 110) || (GFX_VERx10 == 120))
#define INTEL_WA_14010017096_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_14010239330_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14010357979_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_14010595310_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_14010755945_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14010899839_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14010945292_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_14012437816_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14012688258_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14012865646_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_14013111325_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_14013745556_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14014063774_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14014148106_GFX_VER ((GFX_VERx10 == 110) || (GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14014176256_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14014414195_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14014427904_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14014595444_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14014890652_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015297576_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015360373_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015360517_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14015420481_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015465469_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14015528146_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14015590813_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015808183_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015842950_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015907227_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14015965466_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14016118574_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14016243945_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14016755692_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14016880151_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14016939504_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14017076903_GFX_VER ((GFX_VERx10 == 110) || (GFX_VERx10 == 125))
#define INTEL_WA_14017153641_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14017171315_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14017333800_GFX_VER ((GFX_VERx10 == 110) || (GFX_VERx10 == 125))
#define INTEL_WA_14017341140_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_14017517122_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_14017880152_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_15010599737_GFX_VER ((GFX_VERx10 == 110) || (GFX_VERx10 == 125))
#define INTEL_WA_16010655327_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_16011107343_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_16011411144_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16011448509_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_16011627967_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16011773973_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16011969663_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_16012292205_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_16012351604_GFX_VER (GFX_VERx10 == 110)
#define INTEL_WA_16012775297_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16013000631_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16013063087_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16014390852_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16014538804_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16016080423_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16016462106_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16016616742_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16016618273_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_16016772977_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_18012201914_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_18013179988_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_18018999230_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_22011186057_GFX_VER (GFX_VERx10 == 120)
#define INTEL_WA_22011440098_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22012244936_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22012575642_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_22012725308_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22012766191_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22012785325_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22012962271_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_22013073587_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22013689345_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22014272202_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22014344549_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22014559856_GFX_VER (GFX_VERx10 == 125)
#define INTEL_WA_22015224714_GFX_VER ((GFX_VERx10 == 120) || (GFX_VERx10 == 125))
#define INTEL_WA_22015761040_GFX_VER (GFX_VERx10 == 125)

/* These defines are suitable for use to compile out genxml code using #if
 * guards.  Workarounds that apply to part of a generation must use a
 * combination of run time checks and INTEL_WA_{NUM}_GFX_VER macros.  Those
 * workarounds are 'poisoned' below.
 */
#define INTEL_NEEDS_WA_220579888 INTEL_WA_220579888_GFX_VER
#define INTEL_NEEDS_WA_1207137018 INTEL_WA_1207137018_GFX_VER
#define INTEL_NEEDS_WA_1406306137 INTEL_WA_1406306137_GFX_VER
#define INTEL_NEEDS_WA_1406479881 INTEL_WA_1406479881_GFX_VER
#define INTEL_NEEDS_WA_1406614636 INTEL_WA_1406614636_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1406631448)
#define INTEL_NEEDS_WA_1406697149 INTEL_WA_1406697149_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1406756463)
#define INTEL_NEEDS_WA_1406950495 INTEL_WA_1406950495_GFX_VER
#define INTEL_NEEDS_WA_1407240128 INTEL_WA_1407240128_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1407385565)
#define INTEL_NEEDS_WA_1407391552 INTEL_WA_1407391552_GFX_VER
#define INTEL_NEEDS_WA_1407520876 INTEL_WA_1407520876_GFX_VER
#define INTEL_NEEDS_WA_1407528679 INTEL_WA_1407528679_GFX_VER
#define INTEL_NEEDS_WA_1407685933 INTEL_WA_1407685933_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1408224581)
#define INTEL_NEEDS_WA_1408264532 INTEL_WA_1408264532_GFX_VER
#define INTEL_NEEDS_WA_1408615042 INTEL_WA_1408615042_GFX_VER
#define INTEL_NEEDS_WA_1408937953 INTEL_WA_1408937953_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1409392000)
#define INTEL_NEEDS_WA_1409433168 INTEL_WA_1409433168_GFX_VER
#define INTEL_NEEDS_WA_1409600907 INTEL_WA_1409600907_GFX_VER
#define INTEL_NEEDS_WA_1505013527 INTEL_WA_1505013527_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1507384622)
#define INTEL_NEEDS_WA_1508701464 INTEL_WA_1508701464_GFX_VER
#define INTEL_NEEDS_WA_1508744258 INTEL_WA_1508744258_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1509820217)
#define INTEL_NEEDS_WA_1604061319 INTEL_WA_1604061319_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1604366864)
PRAGMA_POISON(INTEL_NEEDS_WA_1604608133)
PRAGMA_POISON(INTEL_NEEDS_WA_1605967699)
#define INTEL_NEEDS_WA_1606376872 INTEL_WA_1606376872_GFX_VER
#define INTEL_NEEDS_WA_1606932921 INTEL_WA_1606932921_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_1607225878)
PRAGMA_POISON(INTEL_NEEDS_WA_1607446692)
PRAGMA_POISON(INTEL_NEEDS_WA_1607610283)
#define INTEL_NEEDS_WA_1607854226 INTEL_WA_1607854226_GFX_VER
#define INTEL_NEEDS_WA_1607956946 INTEL_WA_1607956946_GFX_VER
#define INTEL_NEEDS_WA_1608127078 INTEL_WA_1608127078_GFX_VER
#define INTEL_NEEDS_WA_1805811773 INTEL_WA_1805811773_GFX_VER
#define INTEL_NEEDS_WA_1806527549 INTEL_WA_1806527549_GFX_VER
#define INTEL_NEEDS_WA_1806565034 INTEL_WA_1806565034_GFX_VER
#define INTEL_NEEDS_WA_2201039848 INTEL_WA_2201039848_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14010013414)
#define INTEL_NEEDS_WA_14010017096 INTEL_WA_14010017096_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14010239330)
#define INTEL_NEEDS_WA_14010357979 INTEL_WA_14010357979_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14010595310)
PRAGMA_POISON(INTEL_NEEDS_WA_14010755945)
PRAGMA_POISON(INTEL_NEEDS_WA_14010899839)
#define INTEL_NEEDS_WA_14010945292 INTEL_WA_14010945292_GFX_VER
#define INTEL_NEEDS_WA_14012437816 INTEL_WA_14012437816_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14012688258)
#define INTEL_NEEDS_WA_14012865646 INTEL_WA_14012865646_GFX_VER
#define INTEL_NEEDS_WA_14013111325 INTEL_WA_14013111325_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14013745556)
#define INTEL_NEEDS_WA_14014063774 INTEL_WA_14014063774_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14014148106)
#define INTEL_NEEDS_WA_14014176256 INTEL_WA_14014176256_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14014414195)
PRAGMA_POISON(INTEL_NEEDS_WA_14014427904)
#define INTEL_NEEDS_WA_14014595444 INTEL_WA_14014595444_GFX_VER
#define INTEL_NEEDS_WA_14014890652 INTEL_WA_14014890652_GFX_VER
#define INTEL_NEEDS_WA_14015297576 INTEL_WA_14015297576_GFX_VER
#define INTEL_NEEDS_WA_14015360373 INTEL_WA_14015360373_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14015360517)
#define INTEL_NEEDS_WA_14015420481 INTEL_WA_14015420481_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14015465469)
#define INTEL_NEEDS_WA_14015528146 INTEL_WA_14015528146_GFX_VER
#define INTEL_NEEDS_WA_14015590813 INTEL_WA_14015590813_GFX_VER
#define INTEL_NEEDS_WA_14015808183 INTEL_WA_14015808183_GFX_VER
#define INTEL_NEEDS_WA_14015842950 INTEL_WA_14015842950_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14015907227)
#define INTEL_NEEDS_WA_14015965466 INTEL_WA_14015965466_GFX_VER
#define INTEL_NEEDS_WA_14016118574 INTEL_WA_14016118574_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14016243945)
#define INTEL_NEEDS_WA_14016755692 INTEL_WA_14016755692_GFX_VER
#define INTEL_NEEDS_WA_14016880151 INTEL_WA_14016880151_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14016939504)
#define INTEL_NEEDS_WA_14017076903 INTEL_WA_14017076903_GFX_VER
#define INTEL_NEEDS_WA_14017153641 INTEL_WA_14017153641_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_14017171315)
PRAGMA_POISON(INTEL_NEEDS_WA_14017333800)
PRAGMA_POISON(INTEL_NEEDS_WA_14017341140)
PRAGMA_POISON(INTEL_NEEDS_WA_14017517122)
PRAGMA_POISON(INTEL_NEEDS_WA_14017880152)
PRAGMA_POISON(INTEL_NEEDS_WA_15010599737)
PRAGMA_POISON(INTEL_NEEDS_WA_16010655327)
#define INTEL_NEEDS_WA_16011107343 INTEL_WA_16011107343_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_16011411144)
#define INTEL_NEEDS_WA_16011448509 INTEL_WA_16011448509_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_16011627967)
PRAGMA_POISON(INTEL_NEEDS_WA_16011773973)
#define INTEL_NEEDS_WA_16011969663 INTEL_WA_16011969663_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_16012292205)
#define INTEL_NEEDS_WA_16012351604 INTEL_WA_16012351604_GFX_VER
#define INTEL_NEEDS_WA_16012775297 INTEL_WA_16012775297_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_16013000631)
PRAGMA_POISON(INTEL_NEEDS_WA_16013063087)
#define INTEL_NEEDS_WA_16014390852 INTEL_WA_16014390852_GFX_VER
#define INTEL_NEEDS_WA_16014538804 INTEL_WA_16014538804_GFX_VER
#define INTEL_NEEDS_WA_16016080423 INTEL_WA_16016080423_GFX_VER
#define INTEL_NEEDS_WA_16016462106 INTEL_WA_16016462106_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_16016616742)
PRAGMA_POISON(INTEL_NEEDS_WA_16016618273)
PRAGMA_POISON(INTEL_NEEDS_WA_16016772977)
PRAGMA_POISON(INTEL_NEEDS_WA_18012201914)
PRAGMA_POISON(INTEL_NEEDS_WA_18013179988)
PRAGMA_POISON(INTEL_NEEDS_WA_18018999230)
PRAGMA_POISON(INTEL_NEEDS_WA_22011186057)
PRAGMA_POISON(INTEL_NEEDS_WA_22011440098)
PRAGMA_POISON(INTEL_NEEDS_WA_22012244936)
#define INTEL_NEEDS_WA_22012575642 INTEL_WA_22012575642_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_22012725308)
#define INTEL_NEEDS_WA_22012766191 INTEL_WA_22012766191_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_22012785325)
PRAGMA_POISON(INTEL_NEEDS_WA_22012962271)
PRAGMA_POISON(INTEL_NEEDS_WA_22013073587)
#define INTEL_NEEDS_WA_22013689345 INTEL_WA_22013689345_GFX_VER
#define INTEL_NEEDS_WA_22014272202 INTEL_WA_22014272202_GFX_VER
PRAGMA_POISON(INTEL_NEEDS_WA_22014344549)
#define INTEL_NEEDS_WA_22014559856 INTEL_WA_22014559856_GFX_VER
#define INTEL_NEEDS_WA_22015224714 INTEL_WA_22015224714_GFX_VER
#define INTEL_NEEDS_WA_22015761040 INTEL_WA_22015761040_GFX_VER

#define INTEL_ALL_WA \
  INTEL_WA(220579888), \
  INTEL_WA(1207137018), \
  INTEL_WA(1406306137), \
  INTEL_WA(1406479881), \
  INTEL_WA(1406614636), \
  INTEL_WA(1406631448), \
  INTEL_WA(1406697149), \
  INTEL_WA(1406756463), \
  INTEL_WA(1406950495), \
  INTEL_WA(1407240128), \
  INTEL_WA(1407385565), \
  INTEL_WA(1407391552), \
  INTEL_WA(1407520876), \
  INTEL_WA(1407528679), \
  INTEL_WA(1407685933), \
  INTEL_WA(1408224581), \
  INTEL_WA(1408264532), \
  INTEL_WA(1408615042), \
  INTEL_WA(1408937953), \
  INTEL_WA(1409392000), \
  INTEL_WA(1409433168), \
  INTEL_WA(1409600907), \
  INTEL_WA(1505013527), \
  INTEL_WA(1507384622), \
  INTEL_WA(1508701464), \
  INTEL_WA(1508744258), \
  INTEL_WA(1509820217), \
  INTEL_WA(1604061319), \
  INTEL_WA(1604366864), \
  INTEL_WA(1604608133), \
  INTEL_WA(1605967699), \
  INTEL_WA(1606376872), \
  INTEL_WA(1606932921), \
  INTEL_WA(1607225878), \
  INTEL_WA(1607446692), \
  INTEL_WA(1607610283), \
  INTEL_WA(1607854226), \
  INTEL_WA(1607956946), \
  INTEL_WA(1608127078), \
  INTEL_WA(1805811773), \
  INTEL_WA(1806527549), \
  INTEL_WA(1806565034), \
  INTEL_WA(2201039848), \
  INTEL_WA(14010013414), \
  INTEL_WA(14010017096), \
  INTEL_WA(14010239330), \
  INTEL_WA(14010357979), \
  INTEL_WA(14010595310), \
  INTEL_WA(14010755945), \
  INTEL_WA(14010899839), \
  INTEL_WA(14010945292), \
  INTEL_WA(14012437816), \
  INTEL_WA(14012688258), \
  INTEL_WA(14012865646), \
  INTEL_WA(14013111325), \
  INTEL_WA(14013745556), \
  INTEL_WA(14014063774), \
  INTEL_WA(14014148106), \
  INTEL_WA(14014176256), \
  INTEL_WA(14014414195), \
  INTEL_WA(14014427904), \
  INTEL_WA(14014595444), \
  INTEL_WA(14014890652), \
  INTEL_WA(14015297576), \
  INTEL_WA(14015360373), \
  INTEL_WA(14015360517), \
  INTEL_WA(14015420481), \
  INTEL_WA(14015465469), \
  INTEL_WA(14015528146), \
  INTEL_WA(14015590813), \
  INTEL_WA(14015808183), \
  INTEL_WA(14015842950), \
  INTEL_WA(14015907227), \
  INTEL_WA(14015965466), \
  INTEL_WA(14016118574), \
  INTEL_WA(14016243945), \
  INTEL_WA(14016755692), \
  INTEL_WA(14016880151), \
  INTEL_WA(14016939504), \
  INTEL_WA(14017076903), \
  INTEL_WA(14017153641), \
  INTEL_WA(14017171315), \
  INTEL_WA(14017333800), \
  INTEL_WA(14017341140), \
  INTEL_WA(14017517122), \
  INTEL_WA(14017880152), \
  INTEL_WA(15010599737), \
  INTEL_WA(16010655327), \
  INTEL_WA(16011107343), \
  INTEL_WA(16011411144), \
  INTEL_WA(16011448509), \
  INTEL_WA(16011627967), \
  INTEL_WA(16011773973), \
  INTEL_WA(16011969663), \
  INTEL_WA(16012292205), \
  INTEL_WA(16012351604), \
  INTEL_WA(16012775297), \
  INTEL_WA(16013000631), \
  INTEL_WA(16013063087), \
  INTEL_WA(16014390852), \
  INTEL_WA(16014538804), \
  INTEL_WA(16016080423), \
  INTEL_WA(16016462106), \
  INTEL_WA(16016616742), \
  INTEL_WA(16016618273), \
  INTEL_WA(16016772977), \
  INTEL_WA(18012201914), \
  INTEL_WA(18013179988), \
  INTEL_WA(18018999230), \
  INTEL_WA(22011186057), \
  INTEL_WA(22011440098), \
  INTEL_WA(22012244936), \
  INTEL_WA(22012575642), \
  INTEL_WA(22012725308), \
  INTEL_WA(22012766191), \
  INTEL_WA(22012785325), \
  INTEL_WA(22012962271), \
  INTEL_WA(22013073587), \
  INTEL_WA(22013689345), \
  INTEL_WA(22014272202), \
  INTEL_WA(22014344549), \
  INTEL_WA(22014559856), \
  INTEL_WA(22015224714), \
  INTEL_WA(22015761040), \

#ifdef __cplusplus
}
#endif

#endif /* INTEL_WA_H */
