/*
 * Copyright 2008  Christian König <deathsimple@vodafone.de>
 * Copyright 2007  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_audio.h"
#include "rhd_hdmi.h"
#include "rhd_regs.h"

#define AUDIO_TIMER_INTERVALL 100 /* 1/10 sekund should be enough */

/*
 * current number of channels
 */
static int
AudioChannels(struct rhdAudio* Audio)
{
    return (RHDRegRead(Audio, AUDIO_RATE_BPS_CHANNEL) & 0x7) + 1;
}

/*
 * current bits per sample
 */
static int
AudioBitsPerSample(struct rhdAudio* Audio)
{
    CARD32 value = (RHDRegRead(Audio, AUDIO_RATE_BPS_CHANNEL) & 0xF0) >> 4;
    switch(value)
    {
	case 0x0: return  8;
	case 0x1: return 16;
	case 0x2: return 20;
	case 0x3: return 24;
	case 0x4: return 32;
    }

    xf86DrvMsg(Audio->scrnIndex, X_WARNING, "%s: unknown bits per sample 0x%x "
               "using 16 instead.\n", __func__, (int) value);

    return 16;
}

/*
 * current sampling rate in HZ
 */
static int
AudioRate(struct rhdAudio* Audio)
{
    CARD32 value = RHDRegRead(Audio, AUDIO_RATE_BPS_CHANNEL);
    CARD32 result;

    if(value & 0x4000)
	result = 44100;
    else
	result = 48000;

    result *= ((value >> 11) & 0x7) + 1;
    result /= ((value >> 8) & 0x7) + 1;

    return result;
}

#if 0
/*
 * something playing ? (not used anymore)
 */
static Bool
AudioPlaying(struct rhdAudio* Audio)
{
    return (RHDRegRead(Audio, AUDIO_PLAYING) >> 4) & 1;
}
#endif

/*
 * iec 60958 status bits
 */
static CARD8
AudioStatusBits(struct rhdAudio* Audio)
{
    return RHDRegRead(Audio, AUDIO_STATUS_BITS) & 0xff;
}

/*
 * iec 60958 category code
 */
static CARD8
AudioCategoryCode(struct rhdAudio* Audio)
{
    return (RHDRegRead(Audio, AUDIO_STATUS_BITS) >> 8) & 0xff;
}

/*
 * update all registered hdmi interfaces with current audio parameters
 */
static CARD32
AudioUpdateHdmi(OsTimerPtr timer, CARD32 time, pointer ptr)
{
    struct rhdAudio *Audio = (struct rhdAudio*)ptr;
    int channels = AudioChannels(Audio);
    int rate = AudioRate(Audio);
    int bps = AudioBitsPerSample(Audio);
    CARD8 status_bits = AudioStatusBits(Audio);
    CARD8 category_code = AudioCategoryCode(Audio);

    Bool changes = FALSE;

    struct rhdHdmi* hdmi;

    changes |= channels != Audio->SavedChannels;
    changes |= rate != Audio->SavedRate;
    changes |= bps != Audio->SavedBitsPerSample;
    changes |= status_bits != Audio->SavedStatusBits;
    changes |= category_code != Audio->SavedCategoryCode;

    if(changes) {
	Audio->SavedChannels = channels;
	Audio->SavedRate = rate;
	Audio->SavedBitsPerSample = bps;
	Audio->SavedStatusBits = status_bits;
	Audio->SavedCategoryCode = category_code;
    }

    for(hdmi=Audio->Registered; hdmi != NULL; hdmi=hdmi->Next)
	if(changes || RHDHdmiBufferStatusChanged(hdmi))
	    RHDHdmiUpdateAudioSettings(
		hdmi, channels,
		rate, bps, status_bits,
		category_code);

    return AUDIO_TIMER_INTERVALL;
}

/*
 * allocate and init the audio structure
 */
void
RHDAudioInit(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

    if (rhdPtr->ChipSet >= RHD_RS600) {
	struct rhdAudio *Audio = (struct rhdAudio *) xnfcalloc(sizeof(struct rhdAudio), 1);

	Audio->scrnIndex = rhdPtr->scrnIndex;
	Audio->Registered = NULL;
	Audio->Stored = FALSE;

	rhdPtr->Audio = Audio;
    } else
	rhdPtr->Audio = NULL;
}

/*
 * enable or disable the complete audio engine
 */
void
RHDAudioSetEnable(RHDPtr rhdPtr, Bool Enable)
{
    struct rhdAudio *Audio = rhdPtr->Audio;

    if (!Audio)	return;
    RHDFUNC(Audio);

    RHDRegMask(Audio, AUDIO_ENABLE, Enable ? 0x81000000 : 0x0, 0x81000000);
    if(Enable) {
	/* the hardware generates an interrupt if audio starts/stops playing,
	 * but since drm doesn't support this interrupt, we check
	 * every AUDIO_TIMER_INTERVALL ms if something has changed
	 */
	Audio->SavedChannels = -1;
	Audio->SavedRate = -1;
	Audio->SavedBitsPerSample = -1;
        Audio->SavedStatusBits = 0;
        Audio->SavedCategoryCode = 0;
	Audio->Timer = TimerSet(NULL, 0, AUDIO_TIMER_INTERVALL, AudioUpdateHdmi, Audio);

	/* 48kHz and 16/20 bits per sample are always supported */
	RHDAudioSetSupported(rhdPtr, TRUE,
		AUDIO_RATE_48000_HZ|
		AUDIO_BPS_16|AUDIO_BPS_20,
		AUDIO_CODEC_PCM
	);
    } else {
	TimerFree(Audio->Timer);
	Audio->Timer = NULL;
    }
}

/*
 * programm the audio clock and timing registers
 */
void
RHDAudioSetClock(RHDPtr rhdPtr, struct rhdOutput* Output, CARD32 Clock)
{
    struct rhdAudio *Audio = rhdPtr->Audio;
    int Rate = 48000;

    if (!Audio)	return;
    RHDFUNC(Audio);

    xf86DrvMsg(Audio->scrnIndex, X_INFO, "%s: using %s as clock source with %d khz\n",
	__func__, Output->Name, (int)Clock);

    switch(Output->Id) {
	case RHD_OUTPUT_TMDSA:
	case RHD_OUTPUT_LVTMA:
	    RHDRegMask(Audio, AUDIO_TIMING, 0, 0x301);
	    break;

	case RHD_OUTPUT_UNIPHYA:
	case RHD_OUTPUT_UNIPHYB:
	case RHD_OUTPUT_KLDSKP_LVTMA:
	    RHDRegMask(Audio, AUDIO_TIMING, 0x100, 0x301);
	    break;

	default:
	    xf86DrvMsg(Audio->scrnIndex, X_ERROR, "%s: unsupported output type\n", __func__);
	    break;
    }

    switch(RHDOutputTmdsIndex(Output)) {
	case 0:
	    RHDRegWrite(Audio, AUDIO_PLL1_MUL, Rate*50);
	    RHDRegWrite(Audio, AUDIO_PLL1_DIV, Clock*100);
	    RHDRegWrite(Audio, AUDIO_CLK_SRCSEL, 0);
	    break;

	case 1:
	    RHDRegWrite(Audio, AUDIO_PLL2_MUL, Rate*50);
	    RHDRegWrite(Audio, AUDIO_PLL2_DIV, Clock*100);
	    RHDRegWrite(Audio, AUDIO_CLK_SRCSEL, 1);
	    break;
    }
}

/*
 * set the supported audio rates, bits per sample and codecs
 */
void
RHDAudioSetSupported(RHDPtr rhdPtr, Bool clear, CARD32 config, CARD32 codec)
{
    struct rhdAudio *Audio = rhdPtr->Audio;
    if (!Audio)	return;

    RHDFUNC(Audio);
    xf86DrvMsg(Audio->scrnIndex, X_INFO, "%s: config 0x%x codec 0x%x\n",
	__func__, (int) config, (int) codec);

    if(config & 0xFFE0F000)
	xf86DrvMsg(Audio->scrnIndex, X_WARNING, "%s: reserved config bits set 0x%x\n",
		   __func__, (int) config);

    if(codec & 0xFFFFFFF8)
	xf86DrvMsg(Audio->scrnIndex, X_WARNING, "%s: reserved codec bits set 0x%x\n",
		   __func__, (int) codec);

    if(clear) {
	RHDRegWrite(Audio, AUDIO_SUPPORTED_SIZE_RATE, config);
	RHDRegWrite(Audio, AUDIO_SUPPORTED_CODEC, codec);
    } else {
	RHDRegMask(Audio, AUDIO_SUPPORTED_SIZE_RATE, config, config);
	RHDRegMask(Audio, AUDIO_SUPPORTED_CODEC, codec, codec);
    }
}

/*
 * register and hdmi interface for getting updates when audio parameters change
 */
void
RHDAudioRegisterHdmi(RHDPtr rhdPtr, struct rhdHdmi* rhdHdmi)
{
    struct rhdAudio *Audio = rhdPtr->Audio;
    if (!Audio)	return;
    RHDFUNC(Audio);

    if(!rhdHdmi)
	return;

    /* make shure the HDMI interface is not registered */
    RHDAudioUnregisterHdmi(rhdPtr, rhdHdmi);

    rhdHdmi->Next = Audio->Registered;
    Audio->Registered = rhdHdmi;
}


/*
 * unregister the hdmi interface
 */
void RHDAudioUnregisterHdmi(RHDPtr rhdPtr, struct rhdHdmi* rhdHdmi)
{
    struct rhdAudio *Audio = rhdPtr->Audio;
    struct rhdHdmi** hdmiPtr;
    if (!Audio)	return;
    RHDFUNC(Audio);

    for(hdmiPtr=&Audio->Registered; *hdmiPtr!=NULL;hdmiPtr=&(*hdmiPtr)->Next)
	if(*hdmiPtr == rhdHdmi) {
	    *hdmiPtr = rhdHdmi->Next;
	    rhdHdmi->Next = NULL;
	    return;
	}
}

/*
 * save the current config of audio engine
 */
void
RHDAudioSave(RHDPtr rhdPtr)
{
    struct rhdAudio *Audio = rhdPtr->Audio;
    if (!Audio)	return;

    RHDFUNC(Audio);

    Audio->StoreEnabled = RHDRegRead(Audio, AUDIO_ENABLE);
    Audio->StoreTiming = RHDRegRead(Audio, AUDIO_TIMING);

    Audio->StoreSupportedSizeRate = RHDRegRead(Audio, AUDIO_SUPPORTED_SIZE_RATE);
    Audio->StoreSupportedCodec = RHDRegRead(Audio, AUDIO_SUPPORTED_CODEC);

    Audio->StorePll1Mul     = RHDRegRead(Audio, AUDIO_PLL1_MUL);
    Audio->StorePll1Div     = RHDRegRead(Audio, AUDIO_PLL1_DIV);
    Audio->StorePll2Mul     = RHDRegRead(Audio, AUDIO_PLL2_MUL);
    Audio->StorePll2Div     = RHDRegRead(Audio, AUDIO_PLL2_DIV);
    Audio->StoreClockSrcSel = RHDRegRead(Audio, AUDIO_CLK_SRCSEL);

    Audio->Stored = TRUE;
}

/*
 * restore the saved config of audio engine
 */
void
RHDAudioRestore(RHDPtr rhdPtr)
{
    struct rhdAudio *Audio = rhdPtr->Audio;
    if (!Audio)	return;

    RHDFUNC(Audio);

    if (!Audio->Stored) {
        xf86DrvMsg(Audio->scrnIndex, X_ERROR, "%s: trying to restore "
                   "uninitialized values.\n", __func__);
        return;
    }

    /* 
     * Shutdown the audio engine before doing anything else.
     */
    RHDAudioSetEnable(rhdPtr, FALSE);

    RHDRegWrite(Audio, AUDIO_TIMING, Audio->StoreTiming);
    RHDRegWrite(Audio, AUDIO_SUPPORTED_SIZE_RATE, Audio->StoreSupportedSizeRate);
    RHDRegWrite(Audio, AUDIO_SUPPORTED_CODEC, Audio->StoreSupportedCodec);

    RHDRegWrite(Audio, AUDIO_PLL1_MUL, Audio->StorePll1Mul);
    RHDRegWrite(Audio, AUDIO_PLL1_DIV, Audio->StorePll1Div);
    RHDRegWrite(Audio, AUDIO_PLL2_MUL, Audio->StorePll2Mul);
    RHDRegWrite(Audio, AUDIO_PLL2_DIV, Audio->StorePll2Div);
    RHDRegWrite(Audio, AUDIO_CLK_SRCSEL, Audio->StoreClockSrcSel);
    RHDRegWrite(Audio, AUDIO_ENABLE, Audio->StoreEnabled);
}

/*
 * release the allocated memory
 */
void
RHDAudioDestroy(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

    if (!rhdPtr->Audio)	return;

    if(rhdPtr->Audio->Timer)
	TimerFree(rhdPtr->Audio->Timer);

    xfree(rhdPtr->Audio);
}
