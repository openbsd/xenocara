/*
 * Copyright © 2006 Eric Anholt
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifndef I2C_VID_H
#define I2C_VID_H
#include <randrstr.h>

typedef struct _I830I2CVidOutputRec {
    /**
     * Initialize the device at startup time.
     * Returns NULL if the device does not exist.
     */
    void *(*init)(I2CBusPtr b, I2CSlaveAddr addr);

    /**
     * Called to allow the output a chance to create properties after the
     * RandR objects have been created.
     */
    void
    (*create_resources)(I2CDevPtr d);

    /**
     * Turns the output on/off, or sets intermediate power levels if available.
     *
     * Unsupported intermediate modes drop to the lower power setting.  If the
     * mode is DPMSModeOff, the output must be disabled, as the DPLL may be
     * disabled afterwards.
     */
    void (*dpms)(I2CDevPtr d, int mode);
    
    /**
     * Saves the output's state for restoration on VT switch.
     */
    void (*save)(I2CDevPtr d);

    /**
     * Restore's the output's state at VT switch.
     */
    void (*restore)(I2CDevPtr d);

    /**
     * Callback for testing a video mode for a given output.
     *
     * This function should only check for cases where a mode can't be supported
     * on the output specifically, and not represent generic CRTC limitations.
     *
     * \return MODE_OK if the mode is valid, or another MODE_* otherwise.
     */
    int (*mode_valid)(I2CDevPtr d, DisplayModePtr mode);

    /**
     * Callback to adjust the mode to be set in the CRTC.
     *
     * This allows an output to adjust the clock or even the entire set of
     * timings, which is used for panels with fixed timings or for
     * buses with clock limitations.
     */
    Bool (*mode_fixup)(I2CDevPtr d, DisplayModePtr mode, DisplayModePtr adjusted_mode);

    /**
     * Callback for preparing mode changes on an output
     */
    void (*prepare)(I2CDevPtr d);

    /**
     * Callback for committing mode changes on an output
     */
    void (*commit)(I2CDevPtr d);
    
    /**
     * Callback for setting up a video mode after fixups have been made.
     *
     * This is only called while the output is disabled.  The dpms callback
     * must be all that's necessary for the output, to turn the output on
     * after this function is called.
     */
    void (*mode_set)(I2CDevPtr d, DisplayModePtr mode, DisplayModePtr adjusted_mode);
    
    /**
     * Probe for a connected output, and return detect_status.
     */
    xf86OutputStatus (*detect)(I2CDevPtr d);

    /**
     * Query the device for the modes it provides.
     *
     * This function may also update MonInfo, mm_width, and mm_height.
     *
     * \return singly-linked list of modes or NULL if no modes found.
     */
    DisplayModePtr
    (*get_modes)(I2CDevPtr d);
    
#ifdef RANDR_12_INTERFACE
    /**
     * Callback when an output's property has changed.
     */
    Bool
    (*set_property)(I2CDevPtr d, Atom property, RRPropertyValuePtr value);
#endif

    /**
     * Clean up driver-specific bits of the output
     */
    void (*destroy) (I2CDevPtr d);

    /**
     * Debugging hook to dump device registers to log file
     */
    void (*dump_regs)(I2CDevPtr d);
} I830I2CVidOutputRec, *I830I2CVidOutputPtr;

#endif
