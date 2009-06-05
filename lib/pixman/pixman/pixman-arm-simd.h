/*
 * Copyright © 2008 Mozilla Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Mozilla Corporation not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Mozilla Corporation makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 * Author:  Jeff Muizelaar (jeff@infidigm.net)
 *
 */

#include "pixman-private.h"

#ifdef USE_ARM_SIMD

pixman_bool_t pixman_have_arm_simd(void);

#else
#define pixman_have_arm_simd() FALSE
#endif

#ifdef USE_ARM_SIMD

extern const FastPathInfo *const arm_simd_fast_paths;

void
fbCompositeSrcAdd_8000x8000arm (
                            pixman_implementation_t * impl,
                            pixman_op_t op,
				pixman_image_t * pSrc,
				pixman_image_t * pMask,
				pixman_image_t * pDst,
				int32_t      xSrc,
				int32_t      ySrc,
				int32_t      xMask,
				int32_t      yMask,
				int32_t      xDst,
				int32_t      yDst,
				int32_t      width,
				int32_t      height);

void
fbCompositeSrc_8888x8888arm (
                            pixman_implementation_t * impl,
                            pixman_op_t op,
			 pixman_image_t * pSrc,
			 pixman_image_t * pMask,
			 pixman_image_t * pDst,
				int32_t      xSrc,
				int32_t      ySrc,
				int32_t      xMask,
				int32_t      yMask,
				int32_t      xDst,
				int32_t      yDst,
				int32_t      width,
				int32_t      height);

void
fbCompositeSrc_8888x8x8888arm (
                            pixman_implementation_t * impl,
                            pixman_op_t op,
			 pixman_image_t * pSrc,
			 pixman_image_t * pMask,
			 pixman_image_t * pDst,
				int32_t      xSrc,
				int32_t      ySrc,
				int32_t      xMask,
				int32_t      yMask,
				int32_t      xDst,
				int32_t      yDst,
				int32_t      width,
				int32_t      height);

void
fbCompositeSolidMask_nx8x8888arm (
                            pixman_implementation_t * impl,
                            pixman_op_t op,
			 pixman_image_t * pSrc,
			 pixman_image_t * pMask,
			 pixman_image_t * pDst,
				int32_t      xSrc,
				int32_t      ySrc,
				int32_t      xMask,
				int32_t      yMask,
				int32_t      xDst,
				int32_t      yDst,
				int32_t      width,
				int32_t      height);


#endif /* USE_ARM */
