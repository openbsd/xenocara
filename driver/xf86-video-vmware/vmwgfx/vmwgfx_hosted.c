/*
 * Copyright 2013 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vmwgfx_hosted.h"
#include "vmwgfx_hosted_priv.h"

/*
 * Hook up hosted environments here.
 */

/**
 * vmwgfx_hosted_detect - Check whether we are hosted
 *
 * Check whether we are hosted by a compositor and
 * in that case return a pointer to a valid struct vmwgfx_hosted_driver.
 * If not hosted, return NULL.
 */
const struct vmwgfx_hosted_driver *
vmwgfx_hosted_detect(void)
{
    const struct vmwgfx_hosted_driver *tmp = vmwgfx_xmir_detect();

    if (!tmp)
	tmp = vmwgfx_xwl_detect();

    return tmp;
}

/**
 * vmwgfx_hosted_modify_flags - Modify driver flags if hosted.
 *
 * @flag: Pointer to the flag argument given to the vmware driver's
 * DriverFunc function, when operation is GET_REQUIRED_HW_INTERFACES.
 *
 * Checks whether we are running hosted, and in that case modifies
 * the flag according to the hosted environment's requirements.
 */
void
vmwgfx_hosted_modify_flags(uint32_t *flags)
{
    vmwgfx_xmir_modify_flags(flags);
    vmwgfx_xwl_modify_flags(flags);
}
