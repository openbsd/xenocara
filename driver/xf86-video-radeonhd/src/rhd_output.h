/*
 * Copyright 2004-2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007       Matthias Hopf <mhopf@novell.com>
 * Copyright 2007       Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _RHD_OUTPUT_H
#define _RHD_OUTPUT_H

/* Also needed for connector -> output mapping */
typedef enum _rhdOutputType {
    RHD_OUTPUT_NONE  = 0,
    RHD_OUTPUT_DAC_EXTERNAL = RHD_OUTPUT_NONE,
    RHD_OUTPUT_DACA,
    RHD_OUTPUT_DACB,
    RHD_OUTPUT_TMDSA,
    RHD_OUTPUT_LVTMA,
    RHD_OUTPUT_TMDSB = RHD_OUTPUT_NONE,
    RHD_OUTPUT_LVDS = RHD_OUTPUT_NONE,
    RHD_OUTPUT_LVTMB = RHD_OUTPUT_NONE
} rhdOutputType;

/*
 *
 * This structure should deal with everything output related.
 *
 */
struct rhdOutput {
    struct rhdOutput *Next;

    int scrnIndex;

    char *Name;
    enum _rhdOutputType Id;

    Bool Active;

    struct rhdCrtc *Crtc;
    struct rhdConnector *Connector;

    Bool (*Sense) (struct rhdOutput *Output, enum rhdConnectorType Type);
    ModeStatus (*ModeValid) (struct rhdOutput *Output, DisplayModePtr Mode);
    void (*Mode) (struct rhdOutput *Output);
    void (*Power) (struct rhdOutput *Output, int Power);
    void (*Save) (struct rhdOutput *Output);
    void (*Restore) (struct rhdOutput *Output);
    void (*Destroy) (struct rhdOutput *Output);

    /* Output Private data */
    void *Private;
};

void RHDOutputAdd(RHDPtr rhdPtr, struct rhdOutput *Output);
void RHDOutputsMode(RHDPtr rhdPtr, struct rhdCrtc *Crtc);
void RHDOutputsPower(RHDPtr rhdPtr, int Power);
void RHDOutputsShutdownInactive(RHDPtr rhdPtr);
void RHDOutputsSave(RHDPtr rhdPtr);
void RHDOutputsRestore(RHDPtr rhdPtr);
void RHDOutputsDestroy(RHDPtr rhdPtr);

/* output local functions. */
struct rhdOutput *RHDDACAInit(RHDPtr rhdPtr);
struct rhdOutput *RHDDACBInit(RHDPtr rhdPtr);
struct rhdOutput *RHDTMDSAInit(RHDPtr rhdPtr);
struct rhdOutput *RHDLVTMAInit(RHDPtr rhdPtr, CARD8 Type);

#endif /* _RHD_OUTPUT_H */
