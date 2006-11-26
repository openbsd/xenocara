/*
 * Copyright (c) 2005 ASPEED Technology Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/* VRAM Size Definition */
#define VIDEOMEM_SIZE_08M	0x00800000
#define VIDEOMEM_SIZE_16M	0x01000000
#define VIDEOMEM_SIZE_32M	0x02000000
#define VIDEOMEM_SIZE_64M	0x04000000

#define AR_PORT_WRITE		(pAST->RelocateIO + 0x40)
#define MISC_PORT_WRITE		(pAST->RelocateIO + 0x42)
#define SEQ_PORT 		(pAST->RelocateIO + 0x44)
#define DAC_INDEX_WRITE		(pAST->RelocateIO + 0x48)
#define DAC_DATA		(pAST->RelocateIO + 0x49)
#define GR_PORT			(pAST->RelocateIO + 0x4E)
#define CRTC_PORT 		(pAST->RelocateIO + 0x54)
#define INPUT_STATUS1_READ	(pAST->RelocateIO + 0x5A)

#define GetReg(base)				inb(base)
#define SetReg(base,val)			outb(base,val)
#define GetIndexReg(base,index,val)			do {			\
                      				outb(base,index);	\
                      				val = inb(base+1);		\
                    				} while (0)
#define SetIndexReg(base,index, val)		do { \
                      				outb(base,index);	\
                      				outb(base+1,val);   	\
                    				} while (0)
#define GetIndexRegMask(base,index, and, val)	do {			\
                      				outb(base,index);	\
                      				val = (inb(base+1) & and);		\
                    				} while (0)                    			
#define SetIndexRegMask(base,index, and, val)  	do { \
                      				UCHAR __Temp; 	\
                      				outb(base,index);   	\
                      				__Temp = (inb((base)+1)&(and))|(val); 	\
                      				SetIndexReg(base,index,__Temp); 	\
                    				} while (0)

#define VGA_LOAD_PALETTE_INDEX(index, red, green, blue) \
{ \
   UCHAR __junk;				\
   SetReg(DAC_INDEX_WRITE,(UCHAR)(index));	\
   __junk = GetReg(SEQ_PORT);			\
   SetReg(DAC_DATA,(UCHAR)(red));		\
   __junk = GetReg(SEQ_PORT);			\
   SetReg(DAC_DATA,(UCHAR)(green));		\
   __junk = GetReg(SEQ_PORT);			\
   SetReg(DAC_DATA,(UCHAR)(blue));		\
   __junk = GetReg(SEQ_PORT);      		\
}
