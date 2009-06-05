/*
 * Copyright © 2009 ARM Ltd
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of ARM Ltd not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  ARM Ltd makes no
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
 * Author:  Ian Rickards (ian.rickards@arm.com) 
 *
 */

#include "pixman-private.h"

#ifdef USE_ARM_NEON

pixman_bool_t pixman_have_arm_neon(void);

#else
#define pixman_have_arm_neon() FALSE
#endif

#ifdef USE_ARM_NEON

extern const FastPathInfo *const arm_neon_fast_paths;

void
fbCompositeSrcAdd_8000x8000neon (
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
fbCompositeSrc_8888x8888neon (
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
fbCompositeSrc_8888x8x8888neon (
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
fbCompositeSolidMask_nx8x0565neon (
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
fbCompositeSolidMask_nx8x8888neon (
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
fbCompositeSrcAdd_8888x8x8neon (
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
fbCompositeSrc_16x16neon (
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
fbCompositeSrc_24x16neon (
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

pixman_bool_t
pixman_fill_neon (uint32_t *bits,
		  int stride,
		  int bpp,
		  int x,
		  int y,
		  int width,
		  int height,
		  uint32_t _xor);

#endif /* USE_ARM_NEON */
