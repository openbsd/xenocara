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

/* Chip tags, used to group the adapters into families. */
enum VIACHIPTAGS {
    VIA_UNKNOWN = 0,
    VIA_CLE266,
    VIA_KM400,
    VIA_K8M800,
    VIA_PM800,
    VIA_VM800,
    VIA_K8M890,
    VIA_P4M900,
    VIA_CX700,
    VIA_P4M890,
    VIA_LAST
};


#define PCI_VIA_VENDOR_ID       0x1106

#define PCI_CHIP_CLE3122        0x3122 /* CLE266 */
#define PCI_CHIP_VT3205         0x7205 /* KM400 */
#define PCI_CHIP_VT3204         0x3108 /* K8M800 */
#define PCI_CHIP_VT3259         0x3118 /* PM800 */
#define PCI_CHIP_VT3314         0x3344 /* VM800 */
#define PCI_CHIP_VT3336         0x3230 /* K8M890 */
#define PCI_CHIP_VT3364         0x3371 /* P4M900 */
#define PCI_CHIP_VT3324         0x3157 /* CX700 */
#define PCI_CHIP_VT3327         0x3343 /* P4M890 */

/* There is some conflicting information about the two major revisions of
 * the CLE266, often labelled Ax and Cx.  The dividing line seems to be
 * either 0x0f or 0x10. */
#define CLE266_REV_IS_AX(x) ((x) < 0x10)
#define CLE266_REV_IS_CX(x) ((x) >= 0x10)

struct ViaCardIdStruct {
    char*  String;  /* full identification string */
    CARD8  Chip;    /* which family of unichrome */
    CARD16 Vendor;  /* PCI subsystem Vendor ID */
    CARD16 Device;  /* PCI subsystem Device ID */
    CARD8  Outputs; /* whether it supports CRT, LCD, and TV */
};

void ViaDoubleCheckCLE266Revision(ScrnInfoPtr pScrn);
void ViaCheckCardId(ScrnInfoPtr pScrn);

#endif /* _VIA_ID_H_ */
