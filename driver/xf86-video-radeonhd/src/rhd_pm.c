/*
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Yang Zhao <yang@yangman.ca>
 *   Matthias Hopf <mhopf@suse.de>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_pm.h"

#include "rhd_atombios.h"

#ifdef ATOM_BIOS

#define COMPARE_MIN_ENGINE_CLOCK	100000
#define SAVE_MIN_ENGINE_CLOCK		200000
#define COMPARE_MAX_ENGINE_CLOCK	3000000
#define COMPARE_MIN_MEMORY_CLOCK	100000
#define SAVE_MIN_MEMORY_CLOCK		200000
#define COMPARE_MAX_MEMORY_CLOCK	3000000
#define COMPARE_MIN_VOLTAGE		500
#define COMPARE_MAX_VOLTAGE		2000

static char *PmLevels[] = {
    "Off", "Idle", "Slow2D", "Fast2D", "Slow3D", "Fast3D", "Max3D", "User"
} ;


static void
rhdPmPrint (struct rhdPm *Pm, char *name, struct rhdPowerState *state)
{
    xf86DrvMsg(Pm->scrnIndex, X_INFO, "  %-8s %8d kHz / %8d kHz / %6.3f V\n",
	       name, (int) state->EngineClock, (int) state->MemoryClock,
	       state->VDDCVoltage / 1000.0);
}

/* Certain clocks require certain voltage settings */
/* TODO: So far we only know few safe points. Interpolate? */
static void rhdPmValidateSetting (struct rhdPm *Pm, struct rhdPowerState *setting, int forceVoltage)
{
    /* CARD32 compare = setting->VDDCVoltage ? setting->VDDCVoltage : Pm->Current.VDDCVoltage; */
    if (! setting->EngineClock)
	setting->EngineClock = Pm->Current.EngineClock;
    if (setting->EngineClock < Pm->Minimum.EngineClock)
	setting->EngineClock = Pm->Minimum.EngineClock;
    if (setting->EngineClock < COMPARE_MIN_ENGINE_CLOCK)
	setting->EngineClock = SAVE_MIN_ENGINE_CLOCK;
    if (setting->EngineClock > Pm->Maximum.EngineClock && Pm->Maximum.EngineClock)
	setting->EngineClock = Pm->Maximum.EngineClock;
    if (setting->EngineClock > COMPARE_MAX_ENGINE_CLOCK)
	setting->EngineClock = Pm->Default.EngineClock;
    if (setting->EngineClock > COMPARE_MAX_ENGINE_CLOCK)
	setting->EngineClock = 0;
    if (! setting->MemoryClock)
	setting->MemoryClock = Pm->Current.MemoryClock;
    if (setting->MemoryClock < Pm->Minimum.MemoryClock)
	setting->MemoryClock = Pm->Minimum.MemoryClock;
    if (setting->MemoryClock < COMPARE_MIN_MEMORY_CLOCK)
	setting->MemoryClock = SAVE_MIN_MEMORY_CLOCK;
    if (setting->MemoryClock > Pm->Maximum.MemoryClock && Pm->Maximum.MemoryClock)
	setting->MemoryClock = Pm->Maximum.MemoryClock;
    if (setting->MemoryClock > COMPARE_MAX_MEMORY_CLOCK)
	setting->MemoryClock = Pm->Default.MemoryClock;
    if (setting->MemoryClock > COMPARE_MAX_MEMORY_CLOCK)
	setting->MemoryClock = 0;
    if (! setting->VDDCVoltage)
	setting->VDDCVoltage = Pm->Current.VDDCVoltage;
    if (setting->VDDCVoltage < Pm->Minimum.VDDCVoltage)
	setting->VDDCVoltage = Pm->Minimum.VDDCVoltage;
    if (setting->VDDCVoltage < COMPARE_MIN_VOLTAGE)
	setting->VDDCVoltage = Pm->Current.VDDCVoltage;
    if (setting->VDDCVoltage < COMPARE_MIN_VOLTAGE)
	setting->VDDCVoltage = 0;
    if (setting->VDDCVoltage > Pm->Maximum.VDDCVoltage && Pm->Maximum.VDDCVoltage)
	setting->VDDCVoltage = Pm->Maximum.VDDCVoltage;
    if (setting->VDDCVoltage > COMPARE_MAX_VOLTAGE)
	setting->VDDCVoltage = Pm->Default.VDDCVoltage;
    if (setting->VDDCVoltage > COMPARE_MAX_VOLTAGE)
	setting->VDDCVoltage = 0;
    /* TODO: voltage adaption logic missing */
    /* Only set to lower Voltages than compare if 0 */
}

static void rhdPmValidateMinMax (struct rhdPm *Pm)
{
    if (Pm->Maximum.EngineClock < Pm->Default.EngineClock)
	Pm->Maximum.EngineClock = Pm->Default.EngineClock;
    if (Pm->Maximum.MemoryClock < Pm->Default.MemoryClock)
	Pm->Maximum.MemoryClock = Pm->Default.MemoryClock;
    if (Pm->Maximum.VDDCVoltage < Pm->Default.VDDCVoltage)
	Pm->Maximum.VDDCVoltage = Pm->Default.VDDCVoltage;
    if (Pm->Maximum.EngineClock < Pm->Current.EngineClock)
	Pm->Maximum.EngineClock = Pm->Current.EngineClock;
    if (Pm->Maximum.MemoryClock < Pm->Current.MemoryClock)
	Pm->Maximum.MemoryClock = Pm->Current.MemoryClock;
    if (Pm->Maximum.VDDCVoltage < Pm->Current.VDDCVoltage)
	Pm->Maximum.VDDCVoltage = Pm->Current.VDDCVoltage;
    if((Pm->Minimum.EngineClock > Pm->Default.EngineClock && Pm->Default.EngineClock) || ! Pm->Minimum.EngineClock)
	Pm->Minimum.EngineClock = Pm->Default.EngineClock;
    if((Pm->Minimum.MemoryClock > Pm->Default.MemoryClock && Pm->Default.MemoryClock) || ! Pm->Minimum.MemoryClock)
	Pm->Minimum.MemoryClock = Pm->Default.MemoryClock;
    if((Pm->Minimum.VDDCVoltage > Pm->Default.VDDCVoltage && Pm->Default.VDDCVoltage) || ! Pm->Minimum.VDDCVoltage)
	Pm->Minimum.VDDCVoltage = Pm->Default.VDDCVoltage;
    if((Pm->Minimum.EngineClock > Pm->Current.EngineClock && Pm->Current.EngineClock) || ! Pm->Minimum.EngineClock)
	Pm->Minimum.EngineClock = Pm->Current.EngineClock;
    if((Pm->Minimum.MemoryClock > Pm->Current.MemoryClock && Pm->Current.MemoryClock) || ! Pm->Minimum.MemoryClock)
	Pm->Minimum.MemoryClock = Pm->Current.MemoryClock;
    if((Pm->Minimum.VDDCVoltage > Pm->Current.VDDCVoltage && Pm->Current.VDDCVoltage) || ! Pm->Minimum.VDDCVoltage)
	Pm->Minimum.VDDCVoltage = Pm->Current.VDDCVoltage;
    rhdPmValidateSetting (Pm, &Pm->Maximum, 1);
    rhdPmValidateSetting (Pm, &Pm->Minimum, 1);
    rhdPmValidateSetting (Pm, &Pm->Default, 1);

    if (Pm->NumKnown) {
	int i;
	for (i = 0; i < Pm->NumKnown; i++) {
	    if (Pm->Maximum.EngineClock < Pm->Known[i].EngineClock)
		Pm->Maximum.EngineClock = Pm->Known[i].EngineClock;
	    if (Pm->Maximum.MemoryClock < Pm->Known[i].MemoryClock)
		Pm->Maximum.MemoryClock = Pm->Known[i].MemoryClock;
	    if (Pm->Maximum.VDDCVoltage < Pm->Known[i].VDDCVoltage)
		Pm->Maximum.VDDCVoltage = Pm->Known[i].VDDCVoltage;
	    if (Pm->Minimum.EngineClock > Pm->Known[i].EngineClock && Pm->Known[i].EngineClock)
		Pm->Minimum.EngineClock = Pm->Known[i].EngineClock;
	    if (Pm->Minimum.MemoryClock > Pm->Known[i].MemoryClock && Pm->Known[i].MemoryClock)
		Pm->Minimum.MemoryClock = Pm->Known[i].MemoryClock;
	    if (Pm->Minimum.VDDCVoltage > Pm->Known[i].VDDCVoltage && Pm->Known[i].VDDCVoltage)
		Pm->Minimum.VDDCVoltage = Pm->Known[i].VDDCVoltage;
	}
    }

    if (Pm->Minimum.VDDCVoltage == Pm->Maximum.VDDCVoltage)
	Pm->Minimum.VDDCVoltage = Pm->Maximum.VDDCVoltage = Pm->Default.VDDCVoltage = 0;
}

/* Some AtomBIOSes provide broken current clocks (esp. memory) */
static void rhdPmValidateClearSetting (struct rhdPm *Pm, struct rhdPowerState *setting)
{
    if (setting->EngineClock < COMPARE_MIN_ENGINE_CLOCK)
	setting->EngineClock = 0;
    if (setting->EngineClock > COMPARE_MAX_ENGINE_CLOCK)
	setting->EngineClock = 0;
    if (setting->MemoryClock < COMPARE_MIN_MEMORY_CLOCK)
	setting->MemoryClock = 0;
    if (setting->MemoryClock > COMPARE_MAX_MEMORY_CLOCK)
	setting->MemoryClock = 0;
    if (setting->VDDCVoltage < COMPARE_MIN_VOLTAGE)
	setting->VDDCVoltage = 0;
    if (setting->VDDCVoltage > COMPARE_MAX_VOLTAGE)
	setting->VDDCVoltage = 0;
}

/* Have: a list of possible power settings, eventual minimum and maximum settings.
 * Want: all rhdPowerState_e settings */
static void rhdPmSelectSettings (RHDPtr rhdPtr)
{
    int i;
    struct rhdPm *Pm = rhdPtr->Pm;

    /* Initialize with default; STORED state is special */
    for (i = 0; i < RHD_PM_NUM_STATES; i++)
	memcpy (&Pm->States[i], &Pm->Default, sizeof(struct rhdPowerState));

    /* TODO: This still needs a lot of work */

    /* RHD_PM_OFF: minimum */
    memcpy (&Pm->States[RHD_PM_OFF], &Pm->Minimum, sizeof (struct rhdPowerState));

    if (rhdPtr->lowPowerMode.val.bool) {
	/* TODO: copy lowest config with default Voltage/Mem setting? */
        if (!rhdPtr->lowPowerModeEngineClock.val.integer) {
	    Pm->States[RHD_PM_IDLE].EngineClock = Pm->States[RHD_PM_OFF].EngineClock;
	    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
		       "ForceLowPowerMode: calculated engine clock at %dkHz\n",
		       (int) Pm->States[RHD_PM_IDLE].EngineClock);
        } else {
	    /* TODO: this should actually set the user mode */
            Pm->States[RHD_PM_IDLE].EngineClock = rhdPtr->lowPowerModeEngineClock.val.integer;
            xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
		       "ForceLowPowerMode: set idle engine clock to %dkHz\n",
		       (int) Pm->States[RHD_PM_IDLE].EngineClock);
        }

        if (!rhdPtr->lowPowerModeMemoryClock.val.integer) {
	    Pm->States[RHD_PM_IDLE].MemoryClock = Pm->States[RHD_PM_OFF].MemoryClock;
                xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
			   "ForceLowPowerMode: calculated memory clock at %dkHz\n",
			   (int) Pm->States[RHD_PM_IDLE].MemoryClock);
        } else {
            Pm->States[RHD_PM_IDLE].MemoryClock = rhdPtr->lowPowerModeMemoryClock.val.integer;
            xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
		       "ForceLowPowerMode: set idle memory clock to %dkHz\n",
		       (int) Pm->States[RHD_PM_IDLE].MemoryClock);
        }

	rhdPmValidateSetting (Pm, &Pm->States[RHD_PM_IDLE], 1);

        if (rhdPtr->lowPowerModeEngineClock.val.integer < 0) {
            Pm->States[RHD_PM_IDLE].EngineClock = - rhdPtr->lowPowerModeEngineClock.val.integer;
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING,
		       "ForceLowPowerMode: user requested to ignore validation for engine clock\n");
	}
        if (rhdPtr->lowPowerModeMemoryClock.val.integer < 0) {
            Pm->States[RHD_PM_IDLE].MemoryClock = - rhdPtr->lowPowerModeMemoryClock.val.integer;
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING,
		       "ForceLowPowerMode: user requested to ignore validation for memory clock\n");
	}
    }

    memcpy (&Pm->States[RHD_PM_MAX_3D], &Pm->Maximum, sizeof (struct rhdPowerState));

    xf86DrvMsg (rhdPtr->scrnIndex, X_INFO,
		"Power Management: Final Levels\n");
    ASSERT (sizeof(PmLevels) / sizeof(char *) == RHD_PM_NUM_STATES);
    for (i = 0; i < RHD_PM_NUM_STATES; i++)
	rhdPmPrint (Pm, PmLevels[i], &Pm->States[i]);
}

static void
rhdPmGetRawState (RHDPtr rhdPtr, struct rhdPowerState *state)
{
    union AtomBiosArg data;

    if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			 ATOM_GET_ENGINE_CLOCK, &data) == ATOM_SUCCESS)
        state->EngineClock = data.clockValue;
    if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			 ATOM_GET_MEMORY_CLOCK, &data) == ATOM_SUCCESS)
        state->MemoryClock = data.clockValue;
    if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			 ATOM_GET_VOLTAGE, &data) == ATOM_SUCCESS)
        state->VDDCVoltage = data.val;
}

static Bool
rhdPmSetRawState (RHDPtr rhdPtr, struct rhdPowerState *state)
{
    union AtomBiosArg data;
    Bool ret = TRUE;
    struct rhdPowerState dummy;

    /* TODO: Idle first; find which idles are needed and expose them */
    /* FIXME: Voltage */
    /* FIXME: If Voltage is to be rised, then do that first, then change frequencies.
     *        If Voltage is to be lowered, do it the other way round. */
    if (state->EngineClock && state->EngineClock != rhdPtr->Pm->Current.EngineClock) {
	data.clockValue = state->EngineClock;
	if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			     ATOM_SET_ENGINE_CLOCK, &data) == ATOM_SUCCESS)
	    rhdPtr->Pm->Current.EngineClock = state->EngineClock;
	else
	    ret = FALSE;
    }
#if 0	/* don't do for the moment */
    if (state->MemoryClock && state->MemoryClock != rhdPtr->Pm->Current.MemoryClock) {
	data.clockValue = state->MemoryClock;
	if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			     ATOM_SET_MEMORY_CLOCK, &data) != ATOM_SUCCESS)
	    rhdPtr->Pm->Current.MemoryClock = state->MemoryClock;
	else
	    ret = FALSE;
    }
#endif
#if 0	/* don't do for the moment */
    if (state->VDDCVoltage && state->VDDCVoltage != rhdPtr->Pm->Current.VDDCVoltage) {
	data.val = state->VDDCVoltage;
	if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			     ATOM_SET_VOLTAGE, &data) != ATOM_SUCCESS)
	    rhdPtr->Pm->Current.VDDCVoltage = state->VDDCVoltage;
	else
	    ret = FALSE;
    }
#endif

    /* AtomBIOS might change values, so that later comparisons would fail, even
     * if re-setting wouldn't change the actual values. So don't save real
     * state in Current, but update only to current values. */
    rhdPmGetRawState (rhdPtr, &dummy);
    return ret;
}


/*
 * API
 */

static Bool
rhdPmSelectState (RHDPtr rhdPtr, enum rhdPowerState_e num)
{
    return rhdPmSetRawState (rhdPtr, &rhdPtr->Pm->States[num]);
}

static Bool
rhdPmDefineState (RHDPtr rhdPtr, enum rhdPowerState_e num, struct rhdPowerState *state)
{
    ASSERT(0);
}

void RHDPmInit(RHDPtr rhdPtr)
{
    struct rhdPm *Pm = (struct rhdPm *) xnfcalloc(sizeof(struct rhdPm), 1);
    union AtomBiosArg data;
    RHDFUNC(rhdPtr);

    rhdPtr->Pm = Pm;

    Pm->scrnIndex   = rhdPtr->scrnIndex;
    Pm->SelectState = rhdPmSelectState;
    Pm->DefineState = rhdPmDefineState;

    if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			 ATOM_GET_CHIP_LIMITS, &data) != ATOM_SUCCESS) {
	/* Not getting the information is fatal */
	xfree (Pm);
	rhdPtr->Pm = NULL;
	return;
    }
    memcpy (&Pm->Minimum, &data.chipLimits.Minimum, sizeof (struct rhdPowerState));
    memcpy (&Pm->Maximum, &data.chipLimits.Maximum, sizeof (struct rhdPowerState));
    memcpy (&Pm->Default, &data.chipLimits.Default, sizeof (struct rhdPowerState));

    memcpy (&Pm->Current, &Pm->Default, sizeof (Pm->Default));
    rhdPmGetRawState (rhdPtr, &Pm->Current);
    rhdPmValidateClearSetting (Pm, &Pm->Current);

    xf86DrvMsg (rhdPtr->scrnIndex, X_INFO,
		"Power Management: used engine clock / memory clock / core (VDDC) voltage   (0: ignore)\n");
    xf86DrvMsg (rhdPtr->scrnIndex, X_INFO, "Power Management: Raw Ranges\n");
    rhdPmPrint (Pm, "Minimum", &Pm->Minimum);
    rhdPmPrint (Pm, "Maximum", &Pm->Maximum);
    rhdPmPrint (Pm, "Default", &Pm->Default);

    if (RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			 ATOM_GET_CHIP_CONFIGS, &data) == ATOM_SUCCESS) {
	Pm->NumKnown = data.chipConfigs.num;
	Pm->Known    = data.chipConfigs.Settings;
    } else
	xf86DrvMsg (rhdPtr->scrnIndex, X_ERROR,
		    "Power Management: Cannot get known good chip configurations\n");

    /* Validate */
    if (! Pm->Default.EngineClock || ! Pm->Default.MemoryClock)
	memcpy (&Pm->Default, &Pm->Current, sizeof (Pm->Current));
    rhdPmValidateMinMax  (Pm);
    rhdPmValidateSetting (Pm, &Pm->Current, 0);

    xf86DrvMsg (rhdPtr->scrnIndex, X_INFO, "Power Management: Validated Ranges\n");
    rhdPmPrint (Pm, "Minimum", &Pm->Minimum);
    rhdPmPrint (Pm, "Maximum", &Pm->Maximum);
    rhdPmPrint (Pm, "Default", &Pm->Default);

    if (Pm->NumKnown) {
	int i;
	xf86DrvMsg (rhdPtr->scrnIndex, X_INFO,
		    "Power Management: Known Good Configurations\n");
	for (i = 0; i < Pm->NumKnown; i++) {
	    char buf[4];		/* number of known entries is 8bit */
	    snprintf (buf, 4, "%d", i+1);
	    rhdPmPrint (Pm, buf, &Pm->Known[i]);
	}
    }

    rhdPmSelectSettings (rhdPtr);
    /* TODO: cleanup function: xfree(): Pm->Known[], Pm */
}

#else		/* ATOM_BIOS */

void
RHDPmInit (RHDPtr rhdPtr)
{
    rhdPtr->Pm = NULL;
}

#endif		/* ATOM_BIOS */


/*
 * save current engine clock
 */
void
RHDPmSave (RHDPtr rhdPtr)
{
    struct rhdPm *Pm = rhdPtr->Pm;
    RHDFUNC(rhdPtr);

#ifdef ATOM_BIOS
    /* ATM unconditionally enable power management features
     * if low power mode requested */
    if (rhdPtr->atomBIOS) {
	union AtomBiosArg data;

	data.val = 1;
	RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			 ATOM_PM_SETUP, &data);
	if (rhdPtr->ChipSet < RHD_R600) {
	    data.val = 1;
	    RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			     ATOM_PM_CLOCKGATING_SETUP, &data);
	}
    }
#endif

    if (!Pm) return;

    memcpy (&Pm->Stored, &Pm->Default, sizeof (Pm->Default));
    rhdPmGetRawState (rhdPtr, &Pm->Stored);
    rhdPmValidateClearSetting (Pm, &Pm->Stored);
}

/*
 * restore saved engine clock
 */
void
RHDPmRestore (RHDPtr rhdPtr)
{
    struct rhdPm *Pm = rhdPtr->Pm;

    RHDFUNC(rhdPtr);

#ifdef ATOM_BIOS
    /* Don't know how to save state yet - unconditionally disable */
    if (rhdPtr->atomBIOS) {
	union AtomBiosArg data;

	data.val = 0;
	RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			 ATOM_PM_SETUP, &data);
	if (rhdPtr->ChipSet < RHD_R600) {
	    data.val = 0;
	    RHDAtomBiosFunc (rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			     ATOM_PM_CLOCKGATING_SETUP, &data);
	}
    }
#endif

    if (!Pm)
	return;

    if (! Pm->Stored.EngineClock && ! Pm->Stored.MemoryClock) {
        xf86DrvMsg (Pm->scrnIndex, X_ERROR, "%s: trying to restore "
		    "uninitialized values.\n", __func__);
        return;
    }
    rhdPmSetRawState (rhdPtr, &Pm->Stored);
}

