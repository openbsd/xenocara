/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
 * Copyright (C) 2009  VMware, Inc.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


/**
 * \file compiler.h
 * Compiler-related stuff.
 */


#ifndef COMPILER_H
#define COMPILER_H


#include <assert.h>

#include "util/macros.h"

#include "c99_compat.h" /* inline, __func__, etc. */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Either define MESA_BIG_ENDIAN or MESA_LITTLE_ENDIAN, and CPU_TO_LE32.
 * Do not use these unless absolutely necessary!
 * Try to use a runtime test instead.
 * For now, only used by some DRI hardware drivers for color/texel packing.
 */
#ifdef __OpenBSD__
#include <endian.h>
#define CPU_TO_LE32( x )	htole32( x )
#define LE32_TO_CPU( x )	letoh32( x )
#if BYTE_ORDER == BIG_ENDIAN
#define MESA_BIG_ENDIAN 1
#else
#define MESA_LITTLE_ENDIAN 1
#endif
#endif /* __OpenBSD__ */

#define IEEE_ONE 0x3f800000


#ifdef __cplusplus
}
#endif


#endif /* COMPILER_H */
