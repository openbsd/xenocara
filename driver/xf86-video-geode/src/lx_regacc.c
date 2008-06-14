/*
 * Copyright (c) 2006 Advanced Micro Devices, Inc.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Neither the name of the Advanced Micro Devices, Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 */

/*
 * This is the main file used to add Cimarron graphics support to a software
 * project.  The main reason to have a single file include the other files
 * is that it centralizes the location of the compiler options.  This file
 * should be tuned for a specific implementation, and then modified as needed
 * for new Cimarron releases.  The releases.txt file indicates any updates to
 * this main file, such as a new definition for a new hardware platform.
 *
 * In other words, this file should be copied from the Cimarron source files
 * once when a software project starts, and then maintained as necessary.
 * It should not be recopied with new versions of Cimarron unless the
 * developer is willing to tune the file again for the specific project.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cim_defs.h"

extern unsigned char *cim_vid_ptr;

void cim_write_vid32(unsigned long offset, unsigned long value);
unsigned long cim_read_vid32(unsigned long offset);

void
cim_write_vid32(unsigned long offset, unsigned long value)
{
    WRITE_VID32(offset, value);
}

unsigned long
cim_read_vid32(unsigned long offset)
{
    unsigned long value;

    value = READ_VID32(offset);
    return value;
}
