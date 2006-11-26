/*
 * Copyright 2004-2005 The Unichrome Project  [unichrome.sf.net]
 *
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
#ifndef _VIA_ID_H_
#define _VIA_ID_H_ 1

/*
 * K8M800 and PM800/PM880/CN400 are currently untested
 * and support is disabled.
 */

#define HAVE_K8M800
#define HAVE_PM800


/* Chip tags.  These are used to group the adapters into
 * related families.
 */
enum VIACHIPTAGS {
    VIA_UNKNOWN = 0,
    VIA_CLE266,
    VIA_KM400,
    VIA_K8M800,
    VIA_PM800,
    VIA_VM800,
    VIA_LAST
};


#define PCI_VIA_VENDOR_ID       0x1106

/*
 * I have disabled CLE3022. This way anyone using this device 
 * will have to read this comment or at least complain 
 * someplace. There is also the possibility that it just does 
 * not exist in the wild.
 *
 */
/* #define PCI_CHIP_CLE3022        0x3022 */ /* CLE266??? */
#define PCI_CHIP_VT3204         0x3108 /* K8M800 */
#define PCI_CHIP_VT3259         0x3118 /* PM800/PM880/CN400 */
#define PCI_CHIP_CLE3122        0x3122 /* CLE266 */
#define PCI_CHIP_VT3205         0x7205 /* KM400 */
#define PCI_CHIP_VT3314         0x3344 /* VM800 */

/*
 * There is also quite some conflicting information on the
 * 2 major revisions of the CLE266, oft labelled as Ax and Cx
 * It seems to center around 
 *        ChipRev > 15 == Cx
 *   and
 *        ChipRev < 15 == Ax
 * There is only one case in original xfree86 code where 15 is
 * handled; in via_bandwidth.c:
 *   if (pBIOSInfo->ChipRev > 14) {  // For 3123Cx
 * While setting the primary fifo, the secondary is > 15 again.
 *
 * So does this rule out the existence of CLE266B?
 *
 * It seems to be 0x10, anything from that and up is Cx, anything
 * below is Ax
 */
#define CLE266_REV_IS_CX(x) ((x) >= 0x10)
#define CLE266_REV_IS_AX(x) ((x) < 0x10)

struct ViaCardIdStruct {
    char*  String;  /* Full identification string. */
    CARD8  Chip;    /* Which unichrome device? */
    CARD16 Vendor;  /* PCI Card/Subsystem vendor id */
    CARD16 Device;  /* PCI Card/Subsystem device id */
    CARD8  Outputs; /* ORed list of VIA_DEVICE_CRT, VIA_DEVICE_LCD, VIA_DEVICE_TV */
};

void ViaDoubleCheckCLE266Revision(ScrnInfoPtr pScrn);
void ViaCheckCardId(ScrnInfoPtr pScrn);

#endif /* _VIA_ID_H_ */
