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

#include "dev/intel_wa.h"
#include "dev/intel_device_info.h"
#include "util/bitset.h"

void intel_device_info_init_was(struct intel_device_info *devinfo)
{
   switch(devinfo->platform) {
      case INTEL_PLATFORM_EHL:
         switch(intel_device_info_wa_stepping(devinfo)) {
            case INTEL_STEPPING_A0:
               BITSET_SET(devinfo->workarounds, INTEL_WA_1806230709);
               break;
            default:
               break;
         }
         BITSET_SET(devinfo->workarounds, INTEL_WA_220579888);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1305770284);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1306055483);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1306463417);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1405543622);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406306137);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406614636);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406697149);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406950495);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407240128);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407685933);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408908852);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604061319);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1805811773);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1805992985);
         BITSET_SET(devinfo->workarounds, INTEL_WA_2201039848);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22010893396);
         break;
      case INTEL_PLATFORM_ICL:
         BITSET_SET(devinfo->workarounds, INTEL_WA_220579888);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1305770284);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1306055483);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1306463417);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1405586840);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406306137);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406614636);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406697149);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406756463);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406950495);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407240128);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407685933);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408556878);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408908852);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604061319);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604366864);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1805811773);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1805992985);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806068545);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806230709);
         BITSET_SET(devinfo->workarounds, INTEL_WA_2201039848);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010013414);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22010893396);
         break;
      case INTEL_PLATFORM_ADL:
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406337848);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406606421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407528679);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408224581);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408264532);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409392000);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409600907);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1508744258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604061319);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1605967699);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1606932921);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607854226);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607956946);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806527549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806565034);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808121037);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808850743);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010013414);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010017096);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010840176);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010915640);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013111325);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014617373);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018126777);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011107343);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011448509);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011478345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014912113);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013889147);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020603990);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011782311);
         break;
      case INTEL_PLATFORM_DG1:
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406337848);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406606421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407528679);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408224581);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408264532);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409392000);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409600907);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1508744258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604061319);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1605967699);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1606932921);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607446692);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607610283);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607854226);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607956946);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806527549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806565034);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808121037);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808850743);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010017096);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010840176);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010915640);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013111325);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014617373);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018126777);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16010655327);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011107343);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011448509);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011478345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014912113);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013889147);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020603990);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022502561);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011782311);
         break;
      case INTEL_PLATFORM_RKL:
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406337848);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406606421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406609750);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407528679);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408264532);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409600907);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1508744258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604061319);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1606932921);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607610283);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607854226);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607956946);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806527549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806565034);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808121037);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808850743);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010017096);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010840176);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010915640);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013111325);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014617373);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018126777);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011107343);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011448509);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011478345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014912113);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013889147);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020603990);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011782311);
         break;
      case INTEL_PLATFORM_RPL:
         switch(intel_device_info_wa_stepping(devinfo)) {
            case INTEL_STEPPING_A0:
               BITSET_SET(devinfo->workarounds, INTEL_WA_22011186057);
               break;
            default:
               break;
         }
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406337848);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406606421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407528679);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408224581);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408264532);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409392000);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409600907);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1508744258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604061319);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1605967699);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1606932921);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607854226);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607956946);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806527549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806565034);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808121037);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808850743);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010013414);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010017096);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010840176);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010915640);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013111325);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014617373);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018126777);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011107343);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011448509);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011478345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014912113);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013889147);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020603990);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011782311);
         break;
      case INTEL_PLATFORM_TGL:
         switch(intel_device_info_wa_stepping(devinfo)) {
            case INTEL_STEPPING_A0:
               BITSET_SET(devinfo->workarounds, INTEL_WA_1607794140);
               break;
            case INTEL_STEPPING_B0:
               BITSET_SET(devinfo->workarounds, INTEL_WA_1607794140);
               break;
            default:
               break;
         }
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406337848);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1406606421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1407528679);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408224581);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1408264532);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409392000);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1409600907);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1508744258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1604061319);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1605967699);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1606932921);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607446692);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607610283);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607854226);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1607956946);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806527549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1806565034);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808121037);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1808850743);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010017096);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010840176);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010915640);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013111325);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014617373);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018126777);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16010655327);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011107343);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011448509);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011478345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014912113);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013889147);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020603990);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022502561);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011782311);
         break;
      case INTEL_PLATFORM_DG2_G10:
         BITSET_SET(devinfo->workarounds, INTEL_WA_1509820217);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14012437816);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14012688258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014063774);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014176256);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014414195);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014595444);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014851047);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015055625);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015420481);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015782607);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015868140);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015907227);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016407139);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016712196);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016896531);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018912822);
         BITSET_SET(devinfo->workarounds, INTEL_WA_15011116421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011411144);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011627967);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011773973);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16012775297);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013063087);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014390852);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014538804);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16018063123);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012201914);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013179988);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18019110168);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18019816803);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020335297);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022330953);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18023881693);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011440098);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22012699309);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013045878);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013073587);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013689345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22014344549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22014412737);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22015614752);
         break;
      case INTEL_PLATFORM_DG2_G11:
         BITSET_SET(devinfo->workarounds, INTEL_WA_1509820217);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14012437816);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14012688258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014063774);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014176256);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014414195);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014595444);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014851047);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015055625);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015420481);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015782607);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015868140);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015907227);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016896531);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_15011116421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011411144);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011627967);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011773973);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16012775297);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013000631);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013063087);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014390852);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014538804);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012201914);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18019110168);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18019816803);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020335297);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022330953);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18023881693);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011440098);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22012725308);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013073587);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013689345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22014344549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22014412737);
         break;
      case INTEL_PLATFORM_DG2_G12:
         BITSET_SET(devinfo->workarounds, INTEL_WA_1509820217);
         BITSET_SET(devinfo->workarounds, INTEL_WA_1809626530);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14010755945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14012437816);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14012688258);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14013672992);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014063774);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014176256);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014414195);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014595444);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014851047);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015055625);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015420481);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015782607);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015907227);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016074189);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_15011116421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011411144);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011627967);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011773973);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16012775297);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013063087);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014390852);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16014538804);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012201914);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013179988);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18019110168);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18019816803);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18020335297);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022330953);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18023881693);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22011440098);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22012699309);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013045878);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013073587);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22013689345);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22014344549);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22014412737);
         BITSET_SET(devinfo->workarounds, INTEL_WA_22015614752);
         break;
      case INTEL_PLATFORM_MTL_M:
         switch(intel_device_info_wa_stepping(devinfo)) {
            case INTEL_STEPPING_A0:
               BITSET_SET(devinfo->workarounds, INTEL_WA_14012437816);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14014097488);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14014414195);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14014595444);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14014851047);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14015055625);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14015420481);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14015782607);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14017240301);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14017245111);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16012775297);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16013063087);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16014390852);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16014538804);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16016015852);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18019110168);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18019816803);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18020335297);
               BITSET_SET(devinfo->workarounds, INTEL_WA_22013689345);
               BITSET_SET(devinfo->workarounds, INTEL_WA_22014412737);
               break;
            default:
               break;
         }
         BITSET_SET(devinfo->workarounds, INTEL_WA_1509820217);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014063774);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014176256);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015868140);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016243945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016407139);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016712196);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018912822);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14019028097);
         BITSET_SET(devinfo->workarounds, INTEL_WA_15011116421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16011627967);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16018063123);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013179988);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022330953);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18023881693);
         break;
      case INTEL_PLATFORM_MTL_P:
         switch(intel_device_info_wa_stepping(devinfo)) {
            case INTEL_STEPPING_A0:
               BITSET_SET(devinfo->workarounds, INTEL_WA_14012437816);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14014595444);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14014851047);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14015055625);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14015420481);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14015782607);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14017240301);
               BITSET_SET(devinfo->workarounds, INTEL_WA_14017245111);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16012775297);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16013994831);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16014390852);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16014538804);
               BITSET_SET(devinfo->workarounds, INTEL_WA_16016015852);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18012660806);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18019110168);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18019816803);
               BITSET_SET(devinfo->workarounds, INTEL_WA_18020335297);
               BITSET_SET(devinfo->workarounds, INTEL_WA_22013689345);
               BITSET_SET(devinfo->workarounds, INTEL_WA_22014412737);
               break;
            default:
               break;
         }
         BITSET_SET(devinfo->workarounds, INTEL_WA_1509820217);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014063774);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14014176256);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015360517);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14015868140);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016243945);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016407139);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14016712196);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14017468336);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14018545335);
         BITSET_SET(devinfo->workarounds, INTEL_WA_14019028097);
         BITSET_SET(devinfo->workarounds, INTEL_WA_15011116421);
         BITSET_SET(devinfo->workarounds, INTEL_WA_16018063123);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18013179988);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022330953);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18022508906);
         BITSET_SET(devinfo->workarounds, INTEL_WA_18023881693);
         break;
      default:
         /* unsupported platform */
         break;
   };
}
