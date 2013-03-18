/*
 * Copyright (c) 2011 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"

#if __x86_64__
#define USE_SSE2 1
#endif

#if USE_SSE2
#include <xmmintrin.h>

#if __x86_64__
#define have_sse2() 1
#else
enum {
	MMX = 0x1,
	MMX_EXTENSIONS = 0x2,
	SSE = 0x6,
	SSE2 = 0x8,
	CMOV = 0x10
};

#ifdef __GNUC__
static unsigned int
detect_cpu_features(void)
{
	unsigned int features;
	unsigned int result = 0;

	char vendor[13];
	vendor[0] = 0;
	vendor[12] = 0;

	asm (
	     "pushf\n"
	     "pop %%eax\n"
	     "mov %%eax, %%ecx\n"
	     "xor $0x00200000, %%eax\n"
	     "push %%eax\n"
	     "popf\n"
	     "pushf\n"
	     "pop %%eax\n"
	     "mov $0x0, %%edx\n"
	     "xor %%ecx, %%eax\n"
	     "jz 1f\n"

	     "mov $0x00000000, %%eax\n"
	     "push %%ebx\n"
	     "cpuid\n"
	     "mov %%ebx, %%eax\n"
	     "pop %%ebx\n"
	     "mov %%eax, %1\n"
	     "mov %%edx, %2\n"
	     "mov %%ecx, %3\n"
	     "mov $0x00000001, %%eax\n"
	     "push %%ebx\n"
	     "cpuid\n"
	     "pop %%ebx\n"
	     "1:\n"
	     "mov %%edx, %0\n"
	     : "=r" (result), "=m" (vendor[0]), "=m" (vendor[4]), "=m" (vendor[8])
	     :: "%eax", "%ecx", "%edx");

	features = 0;
	if (result) {
		/* result now contains the standard feature bits */
		if (result & (1 << 15))
			features |= CMOV;
		if (result & (1 << 23))
			features |= MMX;
		if (result & (1 << 25))
			features |= SSE;
		if (result & (1 << 26))
			features |= SSE2;
	}
	return features;
}
#else
static unsigned int detect_cpu_features(void) { return 0; }
#endif

static bool have_sse2(void)
{
	static int sse2_present = -1;

	if (sse2_present == -1)
		sse2_present = detect_cpu_features() & SSE2;

	return sse2_present;
}
#endif

static inline __m128i
xmm_create_mask_32(uint32_t mask)
{
	return _mm_set_epi32(mask, mask, mask, mask);
}

static inline __m128i
xmm_load_128u(const __m128i *src)
{
	return _mm_loadu_si128(src);
}

static inline void
xmm_save_128(__m128i *dst, __m128i data)
{
	_mm_store_si128(dst, data);
}
#endif

void
memcpy_blt(const void *src, void *dst, int bpp,
	   int32_t src_stride, int32_t dst_stride,
	   int16_t src_x, int16_t src_y,
	   int16_t dst_x, int16_t dst_y,
	   uint16_t width, uint16_t height)
{
	const uint8_t *src_bytes;
	uint8_t *dst_bytes;
	int byte_width;

	assert(src);
	assert(dst);
	assert(width && height);
	assert(bpp >= 8);
	assert(width*bpp <= 8*src_stride);
	assert(width*bpp <= 8*dst_stride);

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=%dx%d, pitch=%d/%d\n",
	     __FUNCTION__, src_x, src_y, dst_x, dst_y, width, height, src_stride, dst_stride));

	bpp /= 8;

	src_bytes = (const uint8_t *)src + src_stride * src_y + src_x * bpp;
	dst_bytes = (uint8_t *)dst + dst_stride * dst_y + dst_x * bpp;

	byte_width = width * bpp;
	if (byte_width == src_stride && byte_width == dst_stride) {
		byte_width *= height;
		height = 1;
	}

	switch (byte_width) {
	case 1:
		do {
			*dst_bytes = *src_bytes;
			src_bytes += src_stride;
			dst_bytes += dst_stride;
		} while (--height);
		break;

	case 2:
		do {
			*(uint16_t *)dst_bytes = *(const uint16_t *)src_bytes;
			src_bytes += src_stride;
			dst_bytes += dst_stride;
		} while (--height);
		break;

	case 4:
		do {
			*(uint32_t *)dst_bytes = *(const uint32_t *)src_bytes;
			src_bytes += src_stride;
			dst_bytes += dst_stride;
		} while (--height);
		break;

	case 8:
		do {
			*(uint64_t *)dst_bytes = *(const uint64_t *)src_bytes;
			src_bytes += src_stride;
			dst_bytes += dst_stride;
		} while (--height);
		break;

	default:
		do {
			memcpy(dst_bytes, src_bytes, byte_width);
			src_bytes += src_stride;
			dst_bytes += dst_stride;
		} while (--height);
		break;
	}
}

void
memcpy_to_tiled_x(const void *src, void *dst, int bpp, int swizzling,
		  int32_t src_stride, int32_t dst_stride,
		  int16_t src_x, int16_t src_y,
		  int16_t dst_x, int16_t dst_y,
		  uint16_t width, uint16_t height)
{
	const unsigned tile_width = 512;
	const unsigned tile_height = 8;
	const unsigned tile_size = 4096;

	const unsigned cpp = bpp / 8;
	const unsigned stride_tiles = dst_stride / tile_width;
	const unsigned swizzle_pixels = (swizzling ? 64 : tile_width) / cpp;
	const unsigned tile_pixels = ffs(tile_width / cpp) - 1;
	const unsigned tile_mask = (1 << tile_pixels) - 1;

	unsigned x, y;

	DBG(("%s(bpp=%d, swizzling=%d): src=(%d, %d), dst=(%d, %d), size=%dx%d, pitch=%d/%d\n",
	     __FUNCTION__, bpp, swizzling, src_x, src_y, dst_x, dst_y, width, height, src_stride, dst_stride));

	src = (const uint8_t *)src + src_y * src_stride + src_x * cpp;

	for (y = 0; y < height; ++y) {
		const uint32_t dy = y + dst_y;
		const uint32_t tile_row =
			(dy / tile_height * stride_tiles * tile_size +
			 (dy & (tile_height-1)) * tile_width);
		const uint8_t *src_row = (const uint8_t *)src + src_stride * y;
		uint32_t dx = dst_x, offset;

		x = width * cpp;
		if (dx & (swizzle_pixels - 1)) {
			const uint32_t swizzle_bound_pixels = ALIGN(dx + 1, swizzle_pixels);
			const uint32_t length = min(dst_x + width, swizzle_bound_pixels) - dx;
			offset = tile_row +
				(dx >> tile_pixels) * tile_size +
				(dx & tile_mask) * cpp;
			switch (swizzling) {
			case I915_BIT_6_SWIZZLE_NONE:
				break;
			case I915_BIT_6_SWIZZLE_9:
				offset ^= (offset >> 3) & 64;
				break;
			case I915_BIT_6_SWIZZLE_9_10:
				offset ^= ((offset ^ (offset >> 1)) >> 3) & 64;
				break;
			case I915_BIT_6_SWIZZLE_9_11:
				offset ^= ((offset ^ (offset >> 2)) >> 3) & 64;
				break;
			}

			memcpy((char *)dst + offset, src_row, length * cpp);

			src_row += length * cpp;
			x -= length * cpp;
			dx += length;
		}
		if (swizzling) {
			while (x >= 64) {
				offset = tile_row +
					(dx >> tile_pixels) * tile_size +
					(dx & tile_mask) * cpp;
				switch (swizzling) {
				case I915_BIT_6_SWIZZLE_9:
					offset ^= (offset >> 3) & 64;
					break;
				case I915_BIT_6_SWIZZLE_9_10:
					offset ^= ((offset ^ (offset >> 1)) >> 3) & 64;
					break;
				case I915_BIT_6_SWIZZLE_9_11:
					offset ^= ((offset ^ (offset >> 2)) >> 3) & 64;
					break;
				}

				memcpy((char *)dst + offset, src_row, 64);

				src_row += 64;
				x -= 64;
				dx += swizzle_pixels;
			}
		} else {
			while (x >= 512) {
				assert((dx & tile_mask) == 0);
				offset = tile_row + (dx >> tile_pixels) * tile_size;

				memcpy((char *)dst + offset, src_row, 512);

				src_row += 512;
				x -= 512;
				dx += swizzle_pixels;
			}
		}
		if (x) {
			offset = tile_row +
				(dx >> tile_pixels) * tile_size +
				(dx & tile_mask) * cpp;
			switch (swizzling) {
			case I915_BIT_6_SWIZZLE_NONE:
				break;
			case I915_BIT_6_SWIZZLE_9:
				offset ^= (offset >> 3) & 64;
				break;
			case I915_BIT_6_SWIZZLE_9_10:
				offset ^= ((offset ^ (offset >> 1)) >> 3) & 64;
				break;
			case I915_BIT_6_SWIZZLE_9_11:
				offset ^= ((offset ^ (offset >> 2)) >> 3) & 64;
				break;
			}

			memcpy((char *)dst + offset, src_row, x);
		}
	}
}

void
memmove_box(const void *src, void *dst,
	    int bpp, int32_t stride,
	    const BoxRec *box,
	    int dx, int dy)
{
	union {
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
	} tmp;
	const uint8_t *src_bytes;
	uint8_t *dst_bytes;
	int width, height;

	assert(src);
	assert(dst);
	assert(bpp >= 8);
	assert(box->x2 > box->x1);
	assert(box->y2 > box->y1);

	DBG(("%s: box=(%d, %d), (%d, %d), pitch=%d, bpp=%d, dx=%d, dy=%d\n",
	     __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2,
	     stride, bpp, dx, dy));

	bpp /= 8;
	width = box->y1 * stride + box->x1 * bpp;
	src_bytes = (const uint8_t *)src + width;
	dst_bytes = (uint8_t *)dst + width;

	width = (box->x2 - box->x1) * bpp;
	height = (box->y2 - box->y1);
	assert(width <= 8*stride);
	if (width == stride) {
		width *= height;
		height = 1;
	}

	if (dy >= 0) {
		switch (width) {
		case 1:
			do {
				*dst_bytes = tmp.u8 = *src_bytes;
				src_bytes += stride;
				dst_bytes += stride;
			} while (--height);
			break;

		case 2:
			do {
				*(uint16_t *)dst_bytes = tmp.u16 = *(const uint16_t *)src_bytes;
				src_bytes += stride;
				dst_bytes += stride;
			} while (--height);
			break;

		case 4:
			do {
				*(uint32_t *)dst_bytes = tmp.u32 = *(const uint32_t *)src_bytes;
				src_bytes += stride;
				dst_bytes += stride;
			} while (--height);
			break;

		case 8:
			do {
				*(uint64_t *)dst_bytes = tmp.u64 = *(const uint64_t *)src_bytes;
				src_bytes += stride;
				dst_bytes += stride;
			} while (--height);
			break;

		default:
			if (dst_bytes < src_bytes + width &&
			    src_bytes < dst_bytes + width) {
				do {
					memmove(dst_bytes, src_bytes, width);
					src_bytes += stride;
					dst_bytes += stride;
				} while (--height);
			} else {
				do {
					memcpy(dst_bytes, src_bytes, width);
					src_bytes += stride;
					dst_bytes += stride;
				} while (--height);
			}
			break;
		}
	} else {
		src_bytes += (height-1) * stride;
		dst_bytes += (height-1) * stride;

		switch (width) {
		case 1:
			do {
				*dst_bytes = tmp.u8 = *src_bytes;
				src_bytes -= stride;
				dst_bytes -= stride;
			} while (--height);
			break;

		case 2:
			do {
				*(uint16_t *)dst_bytes = tmp.u16 = *(const uint16_t *)src_bytes;
				src_bytes -= stride;
				dst_bytes -= stride;
			} while (--height);
			break;

		case 4:
			do {
				*(uint32_t *)dst_bytes = tmp.u32 = *(const uint32_t *)src_bytes;
				src_bytes -= stride;
				dst_bytes -= stride;
			} while (--height);
			break;

		case 8:
			do {
				*(uint64_t *)dst_bytes = tmp.u64 = *(const uint64_t *)src_bytes;
				src_bytes -= stride;
				dst_bytes -= stride;
			} while (--height);
			break;

		default:
			if (dst_bytes < src_bytes + width &&
			    src_bytes < dst_bytes + width) {
				do {
					memmove(dst_bytes, src_bytes, width);
					src_bytes -= stride;
					dst_bytes -= stride;
				} while (--height);
			} else {
				do {
					memcpy(dst_bytes, src_bytes, width);
					src_bytes -= stride;
					dst_bytes -= stride;
				} while (--height);
			}
			break;
		}
	}
}

void
memcpy_xor(const void *src, void *dst, int bpp,
	   int32_t src_stride, int32_t dst_stride,
	   int16_t src_x, int16_t src_y,
	   int16_t dst_x, int16_t dst_y,
	   uint16_t width, uint16_t height,
	   uint32_t and, uint32_t or)
{
	const uint8_t *src_bytes;
	uint8_t *dst_bytes;
	int i;

	assert(width && height);
	assert(bpp >= 8);
	assert(width*bpp <= 8*src_stride);
	assert(width*bpp <= 8*dst_stride);

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=%dx%d, pitch=%d/%d, bpp=%d, and=%x, xor=%x\n",
	     __FUNCTION__,
	     src_x, src_y, dst_x, dst_y,
	     width, height,
	     src_stride, dst_stride,
	     bpp, and, or));

	bpp /= 8;
	src_bytes = (const uint8_t *)src + src_stride * src_y + src_x * bpp;
	dst_bytes = (uint8_t *)dst + dst_stride * dst_y + dst_x * bpp;

	if (and == 0xffffffff) {
		switch (bpp) {
		case 1:
			if (width & 1) {
				do {
					for (i = 0; i < width; i++)
						dst_bytes[i] = src_bytes[i] | or;

					src_bytes += src_stride;
					dst_bytes += dst_stride;
				} while (--height);
				break;
			} else {
				width /= 2;
				or |= or << 8;
			}
		case 2:
			if (width & 1) {
				do {
					uint16_t *d = (uint16_t *)dst_bytes;
					const uint16_t *s = (const uint16_t *)src_bytes;

					for (i = 0; i < width; i++)
						d[i] = s[i] | or;

					src_bytes += src_stride;
					dst_bytes += dst_stride;
				} while (--height);
				break;
			} else {
				width /= 2;
				or |= or << 16;
			}
		case 4:
#if USE_SSE2
			if (width * 4 == dst_stride && dst_stride == src_stride) {
				width *= height;
				height = 1;
			}

			if (have_sse2()) {
				do {
					uint32_t *d = (uint32_t *)dst_bytes;
					const uint32_t *s = (const uint32_t *)src_bytes;
					__m128i mask = xmm_create_mask_32(or);

					i = width;
					while (i && (uintptr_t)d & 15) {
						*d++ = *s++ | or;
						i--;
					}

					while (i >= 16) {
						__m128i xmm1, xmm2, xmm3, xmm4;

						xmm1 = xmm_load_128u((__m128i*)s + 0);
						xmm2 = xmm_load_128u((__m128i*)s + 1);
						xmm3 = xmm_load_128u((__m128i*)s + 2);
						xmm4 = xmm_load_128u((__m128i*)s + 3);

						xmm_save_128((__m128i*)d + 0,
							     _mm_or_si128(xmm1, mask));
						xmm_save_128((__m128i*)d + 1,
							     _mm_or_si128(xmm2, mask));
						xmm_save_128((__m128i*)d + 2,
							     _mm_or_si128(xmm3, mask));
						xmm_save_128((__m128i*)d + 3,
							     _mm_or_si128(xmm4, mask));

						d += 16;
						s += 16;
						i -= 16;
					}

					if (i & 8) {
						__m128i xmm1, xmm2;

						xmm1 = xmm_load_128u((__m128i*)s + 0);
						xmm2 = xmm_load_128u((__m128i*)s + 1);

						xmm_save_128((__m128i*)d + 0,
							     _mm_or_si128(xmm1, mask));
						xmm_save_128((__m128i*)d + 1,
							     _mm_or_si128(xmm2, mask));
						d += 8;
						s += 8;
						i -= 8;
					}

					if (i & 4) {
						xmm_save_128((__m128i*)d,
							     _mm_or_si128(xmm_load_128u((__m128i*)s),
									  mask));

						d += 4;
						s += 4;
						i -= 4;
					}

					while (i) {
						*d++ = *s++ | or;
						i--;
					}

					src_bytes += src_stride;
					dst_bytes += dst_stride;
				} while (--height);
			} else
#else
				do {
					uint32_t *d = (uint32_t *)dst_bytes;
					uint32_t *s = (uint32_t *)src_bytes;

					for (i = 0; i < width; i++)
						d[i] = s[i] | or;

					src_bytes += src_stride;
					dst_bytes += dst_stride;
				} while (--height);
#endif
			break;
		}
	} else {
		switch (bpp) {
		case 1:
			do {
				for (i = 0; i < width; i++)
					dst_bytes[i] = (src_bytes[i] & and) | or;

				src_bytes += src_stride;
				dst_bytes += dst_stride;
			} while (--height);
			break;

		case 2:
			do {
				uint16_t *d = (uint16_t *)dst_bytes;
				uint16_t *s = (uint16_t *)src_bytes;

				for (i = 0; i < width; i++)
					d[i] = (s[i] & and) | or;

				src_bytes += src_stride;
				dst_bytes += dst_stride;
			} while (--height);
			break;

		case 4:
			do {
				uint32_t *d = (uint32_t *)dst_bytes;
				uint32_t *s = (uint32_t *)src_bytes;

				for (i = 0; i < width; i++)
					d[i] = (s[i] & and) | or;

				src_bytes += src_stride;
				dst_bytes += dst_stride;
			} while (--height);
			break;
		}
	}
}
