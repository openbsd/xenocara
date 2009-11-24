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

#ifndef _RHD_PM_H
# define _RHD_PM_H

/* Note about settings: RHD_PM_OFF may always change the memory clock, while
 * for the others it remains to be seen whether we can change it without video
 * disturbance. */
/* TODO: forced settings overwrite default settings (Q: in which cases? RHD_PM_OFF?) */
enum rhdPowerState_e {
    /* DPMS off (Q: what about screensavers, what about 3D offscreen rendering apps) */
    RHD_PM_OFF,
    /* DPMS on, no activity for some time */
    RHD_PM_IDLE,
    /* Simple 2D activity */
    RHD_PM_SLOW_2D,
    /* Advanced 2D activity, e.g. video playback */
    RHD_PM_FAST_2D,
    /* Simple 3D activity, e.g. compiz (Q: how to select? indirect rendering only? */
    RHD_PM_SLOW_3D,
    /* Fast 3D activity, e.g. games. Usually using default AtomBIOS setting. */
    RHD_PM_FAST_3D,
    /* Use theoretical chip maximum, maybe beyond default - not selected automatically */
    RHD_PM_MAX_3D,
    /* User supplied */
    RHD_PM_USER,
    RHD_PM_NUM_STATES
};

struct rhdPm {
    int               scrnIndex;

    /* R/O */
    struct rhdPowerState Default;
    struct rhdPowerState Minimum;
    struct rhdPowerState Maximum;
    /* Known good settings (in addition to Default). May be NULL */
    int    NumKnown;
    struct rhdPowerState *Known;

    struct rhdPowerState States[RHD_PM_NUM_STATES];
    struct rhdPowerState Current;
    struct rhdPowerState Stored;

    Bool (*DefineState) (RHDPtr rhdPtr, enum rhdPowerState_e num, struct rhdPowerState *state);
    Bool (*SelectState) (RHDPtr rhdPtr, enum rhdPowerState_e num);
#if 0	/* TODO: expose? */
    Bool (*SetRawState) (RHDPtr rhdPtr, struct rhdPowerState *state);
    void (*GetRawState) (RHDPtr rhdPtr, struct rhdPowerState *state);
#endif
};

void RHDPmInit(RHDPtr rhdPtr);
void RHDPmSave(RHDPtr rhdPtr);
void RHDPmRestore(RHDPtr rhdPtr);

#endif /* _RHD_PM_H */
