/*
 * Copyright 2004-2005 The Unichrome Project  [unichrome.sf.net]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * A big structure with card-ID information, plus some checking functions.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"
#include "via.h"
#include "via_id.h"

/*
 * There's no reason for this to be known outside of via_id.o;
 * only a pointer to a single entry will ever be used outside.
 */
static struct ViaCardIdStruct ViaCardId[] = {
    /*** CLE266 ***/
    {"ECS CLE266 (1.0)",                      VIA_CLE266,  0x1019, 0x1B43, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"LT21 VA28",                             VIA_CLE266,  0x1019, 0x1B44, VIA_DEVICE_CRT},
    {"ECS G320",                              VIA_CLE266,  0x1019, 0xB320, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Asustek Terminator C3V",                VIA_CLE266,  0x1043, 0x8155, VIA_DEVICE_CRT},
    {"VIA VT3122 (CLE266)-EPIA M/MII/...",    VIA_CLE266,  0x1106, 0x3122, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"MSI MS-6723",                           VIA_CLE266,  0x1462, 0X7238, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Clevo T200V",                           VIA_CLE266,  0x1558, 0x200A, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Biostar ARKplus III",                   VIA_CLE266,  0x1565, 0x3204, VIA_DEVICE_CRT | VIA_DEVICE_TV}, /* FS454 TV encoder */

    /*** KM400, KM400A, KN400, P4M800 ***/
    {"ECS KM400-M2",                          VIA_KM400,   0x1019, 0x1842, VIA_DEVICE_CRT},
    {"Acer Aspire 135x",                      VIA_KM400,   0x1025, 0x0033, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Asustek A7V8X-MX",                      VIA_KM400,   0x1043, 0x80ED, VIA_DEVICE_CRT},
    {"Asustek A7V8X-LA",                      VIA_KM400,   0x1043, 0x80F9, VIA_DEVICE_CRT},
    {"Asustek A7V8X-MX SE / A7V400-MX",       VIA_KM400,   0x1043, 0x8118, VIA_DEVICE_CRT},
    {"Asustek Terminator A7VT",               VIA_KM400,   0x1043, 0x813E, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Mitac 8375X",                           VIA_KM400,   0x1071, 0x8375, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV}, /* aka "UMAX 585T" */
    {"Soltek SL-75MIV2",                      VIA_KM400,   0x1106, 0x0000, VIA_DEVICE_CRT}, /* VIA/0x0000 */
    {"VIA VT3205 (KM400)",                    VIA_KM400,   0x1106, 0x3205, VIA_DEVICE_CRT | VIA_DEVICE_TV}, /* borrowed by Soltek SL-B7C-FGR */ 
    {"VIA VT7205 (KM400A)",                   VIA_KM400,   0x1106, 0x7205, VIA_DEVICE_CRT}, /* borrowed by Biostar iDEQ 200V/Chaintech 7VIF4 */
    {"Shuttle FX43",                          VIA_KM400,   0x1297, 0xF643, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Giga-byte 7VM400(A)M",                  VIA_KM400,   0x1458, 0xD000, VIA_DEVICE_CRT},
    {"MSI KM4(A)M-V",                         VIA_KM400,   0x1462, 0x7061, VIA_DEVICE_CRT}, /* aka "DFI KM400-MLV" */
    {"MSI PM8M2-V",                           VIA_KM400,   0x1462, 0x7071, VIA_DEVICE_CRT},
    {"MSI KM4(A)M-L",                         VIA_KM400,   0x1462, 0x7348, VIA_DEVICE_CRT},
    {"Abit VA-10 (1)",                        VIA_KM400,   0x147B, 0x140B, VIA_DEVICE_CRT},
    {"Abit VA-10 (2)",                        VIA_KM400,   0x147B, 0x140C, VIA_DEVICE_CRT},
    {"Abit VA-20",                            VIA_KM400,   0x147B, 0x1411, VIA_DEVICE_CRT},
    {"Averatec 322x",                         VIA_KM400,   0x14FF, 0x030D, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"FIC K7M-400A",                          VIA_KM400,   0x1509, 0x9233, VIA_DEVICE_CRT},
    {"Biostar M7VIZ",                         VIA_KM400,   0x1565, 0x1200, VIA_DEVICE_CRT},
    {"Biostar P4M800-M7",                     VIA_KM400,   0x1565, 0x1202, VIA_DEVICE_CRT},
    {"Uniwill 755CI",                         VIA_KM400,   0x1584, 0x800A, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV}, /* aka "Gericom Hummer Advance", "Maxdata M-Book 1200X" */
    {"Packard Bell Quasar2 (MSI MS6786)",     VIA_KM400,   0x1631, 0xD002, VIA_DEVICE_CRT},
    {"Epox EP-8KMM3I",                        VIA_KM400,   0x1695, 0x9023, VIA_DEVICE_CRT},
    {"ASRock Inc. K7VM2/3/4",                 VIA_KM400,   0x1849, 0x7205, VIA_DEVICE_CRT},
    {"ACorp KM400QP",                         VIA_KM400,   0x1915, 0x1100, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Mercury P4VM800M7 (1.0)",               VIA_KM400,   0x3344, 0x1122, VIA_DEVICE_CRT},
    {"Soyo K7VME",                            VIA_KM400,   0xA723, 0x10FD, VIA_DEVICE_CRT},

    /*** K8M800, K8N800, K8N800A ***/
    {"ZX-5360",                               VIA_K8M800,  0x1019, 0x0F60, VIA_DEVICE_CRT | VIA_DEVICE_LCD },
    {"ECS K8M800-M2 (1.0)",                   VIA_K8M800,  0x1019, 0x1828, VIA_DEVICE_CRT},
    {"ECS K8M800-M2 (2.0)",                   VIA_K8M800,  0x1019, 0x1B45, VIA_DEVICE_CRT},
    {"Acer Aspire 136x",                      VIA_K8M800,  0x1025, 0x006E, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Asustek K8V-MX",                        VIA_K8M800,  0x1043, 0x8129, VIA_DEVICE_CRT},
    {"Mitac 8399",                            VIA_K8M800,  0x1071, 0x8399, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV}, /* aka "Pogolinux Konabook 3100" */
    {"Mitac 8889",                            VIA_K8M800,  0x1071, 0x8889, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"MSI K8M Neo-V (broken pci id)",         VIA_K8M800,  0x1106, 0x0204, VIA_DEVICE_CRT},
    {"VIA VT3108 (K8M800)",                   VIA_K8M800,  0x1106, 0x3108, VIA_DEVICE_CRT}, /* borrowed by Asustek A8V-MX */ 
    {"Shuttle FX21",                          VIA_K8M800,  0x1297, 0x3052, VIA_DEVICE_CRT},
    {"Shuttle FX83",                          VIA_K8M800,  0x1297, 0xF683, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Sharp Actius AL27",                     VIA_K8M800,  0x13BD, 0x1044, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Sharp PC-AE30J",                        VIA_K8M800,  0x13BD, 0x104B, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Giga-byte GA-K8VM800M",                 VIA_K8M800,  0x1458, 0xD000, VIA_DEVICE_CRT},
    {"MSI K8M Neo-V",                         VIA_K8M800,  0x1462, 0x0320, VIA_DEVICE_CRT},
    {"MSI K8MM-V",                            VIA_K8M800,  0x1462, 0x7142, VIA_DEVICE_CRT},
    {"MSI K8MM3-V",                           VIA_K8M800,  0x1462, 0x7181, VIA_DEVICE_CRT},
    {"MSI K9MM-V",                            VIA_K8M800,  0x1462, 0x7312, VIA_DEVICE_CRT},
    {"MSI K8MM-ILSR",                         VIA_K8M800,  0x1462, 0x7410, VIA_DEVICE_CRT},
    {"Abit KV-80",                            VIA_K8M800,  0x147B, 0x1419, VIA_DEVICE_CRT},
    {"Abit KV-81",                            VIA_K8M800,  0x147B, 0x141A, VIA_DEVICE_CRT},
    {"Averatec 327x",                         VIA_K8M800,  0x14FF, 0x0315, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Twinhead N14RA",                        VIA_K8M800,  0x14FF, 0x0321, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Averatec 3715",                         VIA_K8M800,  0x14FF, 0x0322, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Averatec 54xx",                         VIA_K8M800,  0x1509, 0x3930, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"FIC K8M-800M",                          VIA_K8M800,  0x1509, 0x6001, VIA_DEVICE_CRT},
    {"Clevo L570W",                           VIA_K8M800,  0x1558, 0x0570, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Mesh Pegasus",                          VIA_K8M800,  0x1558, 0x4702, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Biostar K8VGA-M",                       VIA_K8M800,  0x1565, 0x1203, VIA_DEVICE_CRT},
    {"DFI K8M800-MLVF",                       VIA_K8M800,  0x15BD, 0x1002, VIA_DEVICE_CRT},
    {"Packard Bell Easynote E6116/E63xx",     VIA_K8M800,  0x1631, 0xC008, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Packard Bell Easynote B3 800/B3340",    VIA_K8M800,  0x1631, 0xC009, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Packard Bell Imedia 2097",              VIA_K8M800,  0x1631, 0xD007, VIA_DEVICE_CRT},
    {"Fujitsu-Siemens Amilo K7610",           VIA_K8M800,  0x1734, 0x10B3, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"ASRock K8Upgrade-VM800",                VIA_K8M800,  0x1849, 0x3108, VIA_DEVICE_CRT},
    {"Axper XP-M8VM800",                      VIA_K8M800,  0x1940, 0xD000, VIA_DEVICE_CRT},

    /*** PM800, PM880, PN800, CN400 ***/
    {"VIA VT3118 (PM800)",                    VIA_PM800,   0x1106, 0x3118, VIA_DEVICE_CRT}, /* borrowed by ECS PM800-M2 */
    {"Hasee F700C",                           VIA_PM800,   0x1071, 0x8650, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Mitac 8666",                            VIA_PM800,   0x1071, 0x8666, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Medion MIM2080",                        VIA_PM800,   0x1071, 0x8965, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"TwinHead E12BL",                        VIA_PM800,   0x14FF, 0x0314, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"MaxSelect Optima C4",                   VIA_PM800,   0x1558, 0x5402, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Biostar P4VMA-M",                       VIA_PM800,   0x1565, 0x1202, VIA_DEVICE_CRT},
    {"Sotec WA2330S5",                        VIA_PM800,   0x161F, 0x2037, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Packard Bell Easynote R1100",           VIA_PM800,   0x1631, 0xC015, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Fujitsu/Siemens Amilo Pro V2010",       VIA_PM800,   0x1734, 0x1078, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Fujitsu/Siemens Amilo L7310",           VIA_PM800,   0x1734, 0x10AB, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"ASRock P4VM8",                          VIA_PM800,   0x1849, 0x3118, VIA_DEVICE_CRT},
    {"Chaintech MPM800-3",                    VIA_PM800,   0x270F, 0x7671, VIA_DEVICE_CRT},
    {"KamLAB KINO-LUKE-533-R20",              VIA_PM800,   0x3344, 0x1122, VIA_DEVICE_CRT},

    /*** P4M800Pro, VN800, CN700 ***/
    {"Clevo/RoverBook Partner E419L",         VIA_VM800,   0x1019, 0x0F75, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Haier A60-440256080BD",                 VIA_VM800,   0x1019, 0x0F79, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"PCChips P23G",                          VIA_VM800,   0x1019, 0x1623, VIA_DEVICE_CRT},
    {"ECS P4M800PRO-M",                       VIA_VM800,   0x1019, 0x2122, VIA_DEVICE_CRT},
    {"ECS C7VCM",                             VIA_VM800,   0x1019, 0xAA2D, VIA_DEVICE_CRT},
    {"PCChips V21G",                          VIA_VM800,   0x1019, 0xAA51, VIA_DEVICE_CRT},
    {"Asustek P5VDC-MX",                      VIA_VM800,   0x1043, 0x3344, VIA_DEVICE_CRT},
    {"Asustek P5VDC-TVM",                     VIA_VM800,   0x1043, 0x81CE, VIA_DEVICE_CRT},
    {"Foxconn P4M800P7MB-RS2H",               VIA_VM800,   0x105B, 0x0CF0, VIA_DEVICE_CRT},
    {"Gateway MX3210",                        VIA_VM800,   0x107B, 0x0216, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"VIA VT3344 (VM800) - EPIA EN",          VIA_VM800,   0x1106, 0x3344, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Gigabyte GA-8VM800M-775",               VIA_VM800,   0x1458, 0xD000, VIA_DEVICE_CRT},
    {"MSI PM8M-V",                            VIA_VM800,   0x1462, 0x7104, VIA_DEVICE_CRT},
    {"MSI Fuzzy CN700/CN700T/CN700G",         VIA_VM800,   0x1462, 0x7199, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"MSI PM8M3-V",                           VIA_VM800,   0x1462, 0x7211, VIA_DEVICE_CRT},
    {"MSI PM8PM",                             VIA_VM800,   0x1462, 0x7222, VIA_DEVICE_CRT},
    {"Twinhead M6",                           VIA_VM800,   0x14FF, 0xA007, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"RoverBook Partner W500",                VIA_VM800,   0x1509, 0x4330, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Clevo/RoverBook Voyager V511L",         VIA_VM800,   0x1558, 0x0662, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Clevo M5xxS",                           VIA_VM800,   0x1558, 0x5406, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Biostar P4M80-M4 / P4VMA-M",            VIA_VM800,   0x1565, 0x1202, VIA_DEVICE_CRT},
    {"Biostar P4M800 Pro-M7",                 VIA_VM800,   0x1565, 0x1206, VIA_DEVICE_CRT},
    {"Packard Bell Lima (ASUS MBP5VDZ-NVM)",  VIA_VM800,   0x1631, 0xE018, VIA_DEVICE_CRT},
    {"Fujitsu/Siemens Amilo Pro V2030",       VIA_VM800,   0x1734, 0x109B, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Fujitsu/Siemens Amilo Pro V2035",       VIA_VM800,   0x1734, 0x10AE, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Fujitsu/Siemens Amilo Pro V2055",       VIA_VM800,   0x1734, 0x10CA, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Fujitsu/Siemens Amilo L7320",           VIA_VM800,   0x1734, 0x10CD, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"ASRock P4VM800",                        VIA_VM800,   0x1849, 0x3344, VIA_DEVICE_CRT},
    {"Asustek P5V800-MX",                     VIA_VM800,   0x3344, 0x1122, VIA_DEVICE_CRT},

    /*** K8M890 ***/
    {"IBM AnyPlace Kiosk 3xx",                VIA_K8M890,  0x1106, 0x3230, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Asustek A8V-VM",                        VIA_K8M890,  0x1043, 0x81B5, VIA_DEVICE_CRT},
    {"Asustek M2V-MX SE",                     VIA_K8M890,  0x1043, 0x8297, VIA_DEVICE_CRT},
    {"Foxconn K8M890M2MA-RS2H",               VIA_K8M890,  0x105B, 0x0C84, VIA_DEVICE_CRT},
    {"Shuttle FX22V1",                        VIA_K8M890,  0x1297, 0x3080, VIA_DEVICE_CRT},
    {"MSI K9VGM-V",                           VIA_K8M890,  0x1462, 0x7253, VIA_DEVICE_CRT},
    {"Averatec 226x",                         VIA_K8M890,  0x14FF, 0xA002, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Fujitsu/Siemens Amilo La 1703",         VIA_K8M890,  0x1734, 0x10D9, VIA_DEVICE_CRT | VIA_DEVICE_LCD},

    /*** P4M900, VN896, CN896 ***/
    {"VIA Epia SN",                           VIA_P4M900,  0x0908, 0x1975, VIA_DEVICE_CRT},
    {"Hewlett Packard 2133 Mini-Note",        VIA_P4M900,  0x103C, 0x3030, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Asustek P5VD2-VM",                      VIA_P4M900,  0x1043, 0x81CE, VIA_DEVICE_CRT},
    {"Asustek P5VD2-VM SE",                   VIA_P4M900,  0x1043, 0x8252, VIA_DEVICE_CRT},
    {"Foxconn P4M9007MB-8RS2H",               VIA_P4M900,  0x105B, 0x0C87, VIA_DEVICE_CRT},
    {"Mitac 8515",                            VIA_P4M900,  0x1071, 0x8515, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Medion Notebook MD96483",               VIA_P4M900,  0x1071, 0x8615, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Mitac 8624",                            VIA_P4M900,  0x1071, 0x8624, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"VIA VT3364 (P4M900)",                   VIA_P4M900,  0x1106, 0x3371, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Gigabyte GA-VM900M",                    VIA_P4M900,  0x1458, 0xD000, VIA_DEVICE_CRT},
    {"MSI VR321",                             VIA_P4M900,  0x1462, 0x3355, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"MSI P4M900M / P4M900M2-F/L",            VIA_P4M900,  0x1462, 0x7255, VIA_DEVICE_CRT},
    {"MSI P4M900M3-L",                        VIA_P4M900,  0x1462, 0x7387, VIA_DEVICE_CRT},
    {"Twinhead H12V",                         VIA_P4M900,  0x14FF, 0xA00F, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Everex NC1501/NC1503",                  VIA_P4M900,  0x1509, 0x1E30, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Clevo M660SE",                          VIA_P4M900,  0x1558, 0x0664, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Clevo M660SR",                          VIA_P4M900,  0x1558, 0x0669, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Neo Endura 540SLe",                     VIA_P4M900,  0x1558, 0x5408, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Clevo M54xSR",                          VIA_P4M900,  0x1558, 0x5409, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Biostar P4M900M-M7 SE",                 VIA_P4M900,  0x1565, 0x1207, VIA_DEVICE_CRT},
    {"Fujitsu/Siemens Amilo Pro V3515",       VIA_P4M900,  0x1734, 0x10CB, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Fujitsu/Siemens Amilo Li1705",          VIA_P4M900,  0x1734, 0x10F7, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"ASRock P4VM900-SATA2",                  VIA_P4M900,  0x1849, 0x3371, VIA_DEVICE_CRT},

    /*** CX700 ***/
    {"VIA VT8454B",                           VIA_CX700,   0x0908, 0x1975, VIA_DEVICE_CRT}, /* Evaluation board, reference possibly wrong */
    {"VIA VT3324 (CX700)",                    VIA_CX700,   0x1106, 0x3157, VIA_DEVICE_CRT},
    {"MSI Fuzzy CX700/CX700D",                VIA_CX700,   0x1462, 0x8020, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Samsung Q1B",                           VIA_CX700,   0x144D, 0xC02C, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"FIC CE260",                             VIA_CX700,   0x1509, 0x2D30, VIA_DEVICE_LCD},
    {"FIC CE261",                             VIA_CX700,   0x1509, 0x2F07, VIA_DEVICE_LCD},
    {"Gigabyte M704 / RoverPC A700GQ",        VIA_CX700,   0x161F, 0x2060, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Packard Bell EasyNote XS",              VIA_CX700,   0x1631, 0xC201, VIA_DEVICE_LCD}, /* aka Everex Cloudbook CE1200V */

    /*** P4M890, VN890 ***/
    {"PCChips P29G",                          VIA_P4M890,  0x1019, 0x1629, VIA_DEVICE_CRT},
    {"ECS P4M890T-M v2.0",                    VIA_P4M890,  0x1019, 0x2171, VIA_DEVICE_CRT},
    {"PCChips ????",                          VIA_P4M890,  0x1019, 0x2174, VIA_DEVICE_CRT},
    {"Asustek P5V-VM ULTRA",                  VIA_P4M890,  0x1043, 0x81B5, VIA_DEVICE_CRT},
    {"Asustek P5V-VM DH",                     VIA_P4M890,  0x1043, 0x81CE, VIA_DEVICE_CRT},
    {"Mitac 8615",                            VIA_P4M890,  0x1071, 0x8615, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Mitac 8624 (Joybook R41E)",             VIA_P4M890,  0x1071, 0x8624, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"VIA VT3343 (P4M890)",                   VIA_P4M890,  0x1106, 0x3343, VIA_DEVICE_CRT},
    {"MSI P4M890M-L/IL (MS-7255)",            VIA_P4M890,  0x1462, 0x7255, VIA_DEVICE_CRT},
    {"Biostar P4M890-M7 TE",                  VIA_P4M890,  0x1565, 0x1207, VIA_DEVICE_CRT},
    {"ASRock P4VM890",                        VIA_P4M890,  0x1849, 0x3343, VIA_DEVICE_CRT},

    /*** VX800 ***/
    {"VIA Epia M700",                         VIA_VX800,   0x1106, 0x1122, VIA_DEVICE_CRT},
    {"VIA OpenBook",                          VIA_VX800,   0x1170, 0x0311, VIA_DEVICE_CRT | VIA_DEVICE_LCD}, /* VIA OpenBook eNote VBE8910 */
    {"Samsung NC20",                          VIA_VX800,   0x144d, 0xc04e, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Quanta DreamBook Light IL1",            VIA_VX800,   0x152d, 0x0771, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Lenovo S12",                            VIA_VX800,   0x17aa, 0x388c, VIA_DEVICE_CRT | VIA_DEVICE_LCD},

    /*** VX855 ***/
    {"VIA VT8562C",                           VIA_VX855,   0x1106, 0x5122, VIA_DEVICE_CRT},

    /* keep this */
    {NULL,                                    VIA_UNKNOWN, 0x0000, 0x0000, VIA_DEVICE_NONE}
};


void
ViaDoubleCheckCLE266Revision(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    /* Crtc 0x4F is only defined in CLE266Cx */
    CARD8 tmp = hwp->readCrtc(hwp, 0x4F);
    
    hwp->writeCrtc(hwp, 0x4F, 0x55);
    
    if (hwp->readCrtc(hwp, 0x4F) == 0x55) {
	if (CLE266_REV_IS_AX(pVia->ChipRev))
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "CLE266 Revision seems"
		       " to be Cx, yet %d was detected previously.\n", pVia->ChipRev);
    } else {
	if (CLE266_REV_IS_CX(pVia->ChipRev))
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "CLE266 Revision seems"
		       " to be Ax, yet %d was detected previously.\n", pVia->ChipRev);
    }
    hwp->writeCrtc(hwp, 0x4F, tmp);
}

void
ViaCheckCardId(ScrnInfoPtr pScrn)
{
    struct ViaCardIdStruct *Id;
    VIAPtr pVia = VIAPTR(pScrn);
    
    if ((SUBVENDOR_ID(pVia->PciInfo) == VENDOR_ID(pVia->PciInfo)) &&
       (SUBSYS_ID(pVia->PciInfo) == DEVICE_ID(pVia->PciInfo)))
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Manufacturer plainly copied main PCI IDs to subsystem/card IDs.\n");

    for (Id = ViaCardId; Id->String; Id++) {
	if ((Id->Chip == pVia->Chipset) && 
           (Id->Vendor == SUBVENDOR_ID(pVia->PciInfo)) &&
           (Id->Device == SUBSYS_ID(pVia->PciInfo))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Detected %s. Card-Ids (%4X|%4X)\n", Id->String, SUBVENDOR_ID(pVia->PciInfo), SUBSYS_ID(pVia->PciInfo));
	    pVia->Id = Id;
	    return;
	}
    }
    
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
	       "Unknown Card-Ids (%4X|%4X|%4X), Chipset: %s; please report to openchrome-users@openchrome.org\n", 
               DEVICE_ID(pVia->PciInfo), SUBVENDOR_ID(pVia->PciInfo), SUBSYS_ID(pVia->PciInfo), pScrn->chipset);
    pVia->Id = NULL;
}

