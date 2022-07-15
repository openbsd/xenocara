/*
 * Copyright © 2012 Canonical, Ltd.
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "synproto.h"
#include "synapticsstr.h"

static int
HwStateAllocTouch(struct SynapticsHwState *hw, SynapticsPrivate * priv)
{
    int num_vals;
    int i = 0;

    hw->num_mt_mask = priv->num_slots;
    hw->mt_mask = malloc(hw->num_mt_mask * sizeof(ValuatorMask *));
    if (!hw->mt_mask)
        goto fail;

    num_vals = 2;               /* x and y */
    num_vals += 2;              /* scroll axes */
    num_vals += priv->num_mt_axes;

    for (; i < hw->num_mt_mask; i++) {
        hw->mt_mask[i] = valuator_mask_new(num_vals);
        if (!hw->mt_mask[i])
            goto fail;
    }

    hw->slot_state = calloc(hw->num_mt_mask, sizeof(enum SynapticsSlotState));
    if (!hw->slot_state)
        goto fail;

    return Success;

 fail:
    for (i--; i >= 0; i--)
        valuator_mask_free(&hw->mt_mask[i]);
    free(hw->mt_mask);
    hw->mt_mask = NULL;
    return BadAlloc;
}

struct SynapticsHwState *
SynapticsHwStateAlloc(SynapticsPrivate * priv)
{
    struct SynapticsHwState *hw;

    hw = calloc(1, sizeof(struct SynapticsHwState));
    if (!hw)
        return NULL;

    if (HwStateAllocTouch(hw, priv) != Success) {
        free(hw);
        return NULL;
    }

    return hw;
}

void
SynapticsHwStateFree(struct SynapticsHwState **hw)
{
    int i;

    if (!*hw)
        return;

    free((*hw)->slot_state);
    for (i = 0; i < (*hw)->num_mt_mask; i++)
        valuator_mask_free(&(*hw)->mt_mask[i]);
    free((*hw)->mt_mask);

    free(*hw);
    *hw = NULL;
}

void
SynapticsCopyHwState(struct SynapticsHwState *dst,
                     const struct SynapticsHwState *src)
{
    int i;

    dst->millis = src->millis;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    dst->cumulative_dx = src->cumulative_dx;
    dst->cumulative_dy = src->cumulative_dy;
    dst->numFingers = src->numFingers;
    dst->fingerWidth = src->fingerWidth;
    dst->left = src->left & BTN_EMULATED_FLAG ? 0 : src->left;
    dst->right = src->right & BTN_EMULATED_FLAG ? 0 : src->right;
    dst->up = src->up;
    dst->down = src->down;
    memcpy(dst->multi, src->multi, sizeof(dst->multi));
    dst->middle = src->middle & BTN_EMULATED_FLAG ? 0 : src->middle;
    for (i = 0; i < dst->num_mt_mask && i < src->num_mt_mask; i++)
        valuator_mask_copy(dst->mt_mask[i], src->mt_mask[i]);
    memcpy(dst->slot_state, src->slot_state,
           dst->num_mt_mask * sizeof(enum SynapticsSlotState));
}

void
SynapticsResetHwState(struct SynapticsHwState *hw)
{
    hw->millis = 0;
    hw->x = INT_MIN;
    hw->y = INT_MIN;
    hw->z = 0;
    hw->cumulative_dx = 0;
    hw->cumulative_dy = 0;
    hw->numFingers = 0;
    hw->fingerWidth = 0;

    hw->left = 0;
    hw->right = 0;
    hw->up = 0;
    hw->down = 0;

    hw->middle = 0;
    memset(hw->multi, 0, sizeof(hw->multi));

    SynapticsResetTouchHwState(hw, TRUE);
}

void
SynapticsResetTouchHwState(struct SynapticsHwState *hw, Bool set_slot_empty)
{
    int i;

    for (i = 0; i < hw->num_mt_mask; i++) {
        int j;

        /* Leave x and y valuators in case we need to restart touch */
        for (j = 2; j < valuator_mask_num_valuators(hw->mt_mask[i]); j++)
            valuator_mask_unset(hw->mt_mask[i], j);

        switch (hw->slot_state[i]) {
        case SLOTSTATE_OPEN:
        case SLOTSTATE_OPEN_EMPTY:
        case SLOTSTATE_UPDATE:
            hw->slot_state[i] =
                set_slot_empty ? SLOTSTATE_EMPTY : SLOTSTATE_OPEN_EMPTY;
            break;

        default:
            hw->slot_state[i] = SLOTSTATE_EMPTY;
            break;
        }
    }
}
