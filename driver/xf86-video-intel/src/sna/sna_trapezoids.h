#include <stdbool.h>

#ifndef SNA_TRAPEZOIDS_H
#define SNA_TRAPEZOIDS_H

#define NO_ACCEL 0
#define FORCE_FALLBACK 0
#define NO_ALIGNED_BOXES 0
#define NO_UNALIGNED_BOXES 0
#define NO_SCAN_CONVERTER 0
#define NO_GPU_THREADS 0

#define NO_IMPRECISE 0
#define NO_PRECISE 0

bool
composite_aligned_boxes(struct sna *sna,
			CARD8 op,
			PicturePtr src,
			PicturePtr dst,
			PictFormatPtr maskFormat,
			INT16 src_x, INT16 src_y,
			int ntrap, const xTrapezoid *traps,
			bool force_fallback);

bool
composite_unaligned_boxes(struct sna *sna,
			  CARD8 op,
			  PicturePtr src,
			  PicturePtr dst,
			  PictFormatPtr maskFormat,
			  INT16 src_x, INT16 src_y,
			  int ntrap, const xTrapezoid *traps,
			  bool force_fallback);

bool
mono_trapezoids_span_converter(struct sna *sna,
			       CARD8 op, PicturePtr src, PicturePtr dst,
			       INT16 src_x, INT16 src_y,
			       int ntrap, xTrapezoid *traps);

bool
mono_trapezoid_span_inplace(struct sna *sna,
			    CARD8 op,
			    PicturePtr src,
			    PicturePtr dst,
			    INT16 src_x, INT16 src_y,
			    int ntrap, xTrapezoid *traps);

bool
mono_trap_span_converter(struct sna *sna,
			 PicturePtr dst,
			 INT16 x, INT16 y,
			 int ntrap, xTrap *traps);

bool
mono_triangles_span_converter(struct sna *sna,
			      CARD8 op, PicturePtr src, PicturePtr dst,
			      INT16 src_x, INT16 src_y,
			      int count, xTriangle *tri);

bool
imprecise_trapezoid_span_inplace(struct sna *sna,
				 CARD8 op, PicturePtr src, PicturePtr dst,
				 PictFormatPtr maskFormat, unsigned flags,
				 INT16 src_x, INT16 src_y,
				 int ntrap, xTrapezoid *traps,
				 bool fallback);

bool
imprecise_trapezoid_span_converter(struct sna *sna,
				   CARD8 op, PicturePtr src, PicturePtr dst,
				   PictFormatPtr maskFormat, unsigned int flags,
				   INT16 src_x, INT16 src_y,
				   int ntrap, xTrapezoid *traps);

bool
imprecise_trapezoid_mask_converter(CARD8 op, PicturePtr src, PicturePtr dst,
				   PictFormatPtr maskFormat, unsigned flags,
				   INT16 src_x, INT16 src_y,
				   int ntrap, xTrapezoid *traps);

bool
imprecise_trapezoid_span_fallback(CARD8 op, PicturePtr src, PicturePtr dst,
				  PictFormatPtr maskFormat, unsigned flags,
				  INT16 src_x, INT16 src_y,
				  int ntrap, xTrapezoid *traps);

bool
precise_trapezoid_span_inplace(struct sna *sna,
				 CARD8 op, PicturePtr src, PicturePtr dst,
				 PictFormatPtr maskFormat, unsigned flags,
				 INT16 src_x, INT16 src_y,
				 int ntrap, xTrapezoid *traps,
				 bool fallback);

bool
precise_trapezoid_span_converter(struct sna *sna,
				   CARD8 op, PicturePtr src, PicturePtr dst,
				   PictFormatPtr maskFormat, unsigned int flags,
				   INT16 src_x, INT16 src_y,
				   int ntrap, xTrapezoid *traps);

bool
precise_trapezoid_mask_converter(CARD8 op, PicturePtr src, PicturePtr dst,
				   PictFormatPtr maskFormat, unsigned flags,
				   INT16 src_x, INT16 src_y,
				   int ntrap, xTrapezoid *traps);

bool
precise_trapezoid_span_fallback(CARD8 op, PicturePtr src, PicturePtr dst,
				PictFormatPtr maskFormat, unsigned flags,
				INT16 src_x, INT16 src_y,
				int ntrap, xTrapezoid *traps);

static inline bool is_mono(PicturePtr dst, PictFormatPtr mask)
{
	return mask ? mask->depth < 8 : dst->polyEdge==PolyEdgeSharp;
}

static inline bool is_precise(PicturePtr dst, PictFormatPtr mask)
{
	return dst->polyMode == PolyModePrecise && !is_mono(dst, mask);
}

static inline bool
trapezoid_span_inplace(struct sna *sna,
		       CARD8 op, PicturePtr src, PicturePtr dst,
		       PictFormatPtr maskFormat, unsigned flags,
		       INT16 src_x, INT16 src_y,
		       int ntrap, xTrapezoid *traps,
		       bool fallback)
{
	if (NO_SCAN_CONVERTER)
		return false;

	if (dst->alphaMap) {
		DBG(("%s: fallback -- dst alphamap\n",
		     __FUNCTION__));
		return false;
	}

	if (!fallback && is_gpu(sna, dst->pDrawable, PREFER_GPU_SPANS)) {
		DBG(("%s: fallback -- can not perform operation in place, destination busy\n",
		     __FUNCTION__));

		return false;
	}

	if (is_mono(dst, maskFormat))
		return mono_trapezoid_span_inplace(sna, op, src, dst, src_x, src_y, ntrap, traps);
	else if (is_precise(dst, maskFormat))
		return precise_trapezoid_span_inplace(sna, op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps, fallback);
	else
		return imprecise_trapezoid_span_inplace(sna, op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps, fallback);
}

static inline bool
trapezoid_span_converter(struct sna *sna,
			 CARD8 op, PicturePtr src, PicturePtr dst,
			 PictFormatPtr maskFormat, unsigned int flags,
			 INT16 src_x, INT16 src_y,
			 int ntrap, xTrapezoid *traps)
{
	if (NO_SCAN_CONVERTER)
		return false;

	if (is_mono(dst, maskFormat))
		return mono_trapezoids_span_converter(sna, op, src, dst, src_x, src_y, ntrap, traps);
	else if (is_precise(dst, maskFormat))
		return precise_trapezoid_span_converter(sna, op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps);
	else
		return imprecise_trapezoid_span_converter(sna, op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps);
}

static inline bool
trapezoid_mask_converter(CARD8 op, PicturePtr src, PicturePtr dst,
			 PictFormatPtr maskFormat, unsigned flags,
			 INT16 src_x, INT16 src_y,
			 int ntrap, xTrapezoid *traps)
{
	if (NO_SCAN_CONVERTER)
		return false;

	if (is_precise(dst, maskFormat))
		return precise_trapezoid_mask_converter(op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps);
	else
		return imprecise_trapezoid_mask_converter(op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps);
}

static inline bool
trapezoid_span_fallback(CARD8 op, PicturePtr src, PicturePtr dst,
			PictFormatPtr maskFormat, unsigned flags,
			INT16 src_x, INT16 src_y,
			int ntrap, xTrapezoid *traps)
{
	if (NO_SCAN_CONVERTER)
		return false;

	if (is_precise(dst, maskFormat))
		return precise_trapezoid_span_fallback(op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps);
	else
		return imprecise_trapezoid_span_fallback(op, src, dst, maskFormat, flags, src_x, src_y, ntrap, traps);
}

bool
trap_span_converter(struct sna *sna,
		    PicturePtr dst,
		    INT16 src_x, INT16 src_y,
		    int ntrap, xTrap *trap);

bool
trap_mask_converter(struct sna *sna,
		    PicturePtr picture,
		    INT16 x, INT16 y,
		    int ntrap, xTrap *trap);

bool
triangles_span_converter(struct sna *sna,
			 CARD8 op, PicturePtr src, PicturePtr dst,
			 PictFormatPtr maskFormat, INT16 src_x, INT16 src_y,
			 int count, xTriangle *tri);

bool
triangles_mask_converter(CARD8 op, PicturePtr src, PicturePtr dst,
			 PictFormatPtr maskFormat, INT16 src_x, INT16 src_y,
			 int count, xTriangle *tri);

bool
tristrip_span_converter(struct sna *sna,
			CARD8 op, PicturePtr src, PicturePtr dst,
			PictFormatPtr maskFormat, INT16 src_x, INT16 src_y,
			int count, xPointFixed *points);

inline static void trapezoid_origin(const xLineFixed *l, int16_t *x, int16_t *y)
{
	if (l->p1.y < l->p2.y) {
		*x = pixman_fixed_to_int(l->p1.x);
		*y = pixman_fixed_to_int(l->p1.y);
	} else {
		*x = pixman_fixed_to_int(l->p2.x);
		*y = pixman_fixed_to_int(l->p2.y);
	}
}

#define ONE_HALF 0x7f
#define RB_MASK 0x00ff00ff
#define RB_ONE_HALF 0x007f007f
#define RB_MASK_PLUS_ONE 0x01000100
#define G_SHIFT 8

static force_inline uint32_t
mul8x2_8 (uint32_t a, uint8_t b)
{
	uint32_t t = (a & RB_MASK) * b + RB_ONE_HALF;
	return ((t + ((t >> G_SHIFT) & RB_MASK)) >> G_SHIFT) & RB_MASK;
}

static force_inline uint32_t
add8x2_8x2(uint32_t a, uint32_t b)
{
	uint32_t t = a + b;
	t |= RB_MASK_PLUS_ONE - ((t >> G_SHIFT) & RB_MASK);
	return t & RB_MASK;
}

static force_inline uint32_t
lerp8x4(uint32_t src, uint8_t a, uint32_t dst)
{
	return (add8x2_8x2(mul8x2_8(src, a),
			   mul8x2_8(dst, ~a)) |
		add8x2_8x2(mul8x2_8(src >> G_SHIFT, a),
			   mul8x2_8(dst >> G_SHIFT, ~a)) << G_SHIFT);
}

static force_inline uint8_t
mul_8_8(uint8_t a, uint8_t b)
{
    uint16_t t = a * (uint16_t)b + 0x7f;
    return ((t >> 8) + t) >> 8;
}

static inline uint32_t multa(uint32_t s, uint8_t a, int shift)
{
	return mul_8_8((s >> shift) & 0xff, a) << shift;
}

static inline uint32_t mul_4x8_8(uint32_t color, uint8_t alpha)
{
	uint32_t v;

	v = 0;
	v |= multa(color, alpha, 24);
	v |= multa(color, alpha, 16);
	v |= multa(color, alpha, 8);
	v |= multa(color, alpha, 0);

	return v;
}

static inline bool
xTriangleValid(const xTriangle *t)
{
	xPointFixed v1, v2;

	v1.x = t->p2.x - t->p1.x;
	v1.y = t->p2.y - t->p1.y;

	v2.x = t->p3.x - t->p1.x;
	v2.y = t->p3.y - t->p1.y;

	/* if the length of any edge is zero, the area must be zero */
	if (v1.x == 0 && v1.y == 0)
		return false;
	if (v2.x == 0 && v2.y == 0)
		return false;

	/* if the cross-product is zero, so it the size */
	return v2.y * v1.x != v1.y * v2.x;
}

#define SAMPLES_X 17
#define SAMPLES_Y 15

#define FAST_SAMPLES_shift 2
#define FAST_SAMPLES_X (1<<FAST_SAMPLES_shift)
#define FAST_SAMPLES_Y (1<<FAST_SAMPLES_shift)
#define FAST_SAMPLES_mask ((1<<FAST_SAMPLES_shift)-1)

#define pixman_fixed_integer_floor(V) pixman_fixed_to_int(V)
#define pixman_fixed_integer_ceil(V) pixman_fixed_to_int(pixman_fixed_ceil(V))

static inline int pixman_fixed_to_fast(pixman_fixed_t v)
{
	return (v + ((1<<(16-FAST_SAMPLES_shift-1))-1)) >> (16 - FAST_SAMPLES_shift);
}

bool trapezoids_bounds(int n, const xTrapezoid *t, BoxPtr box);

#define TOR_INPLACE_SIZE 128

#endif /* SNA_TRAPEZOIDS_H */
