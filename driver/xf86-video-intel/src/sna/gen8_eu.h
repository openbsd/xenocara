#ifndef GEN8_EU_H
#define GEN8_EU_H

#include "brw/brw_eu.h"

bool gen8_wm_kernel__affine(struct brw_compile *p, int dispatch_width);
bool gen8_wm_kernel__affine_mask(struct brw_compile *p, int dispatch_width);
bool gen8_wm_kernel__affine_mask_ca(struct brw_compile *p, int dispatch_width);
bool gen8_wm_kernel__affine_mask_sa(struct brw_compile *p, int dispatch_width);

bool gen8_wm_kernel__projective(struct brw_compile *p, int dispatch_width);
bool gen8_wm_kernel__projective_mask(struct brw_compile *p, int dispatch_width);
bool gen8_wm_kernel__projective_mask_ca(struct brw_compile *p, int dispatch_width);
bool gen8_wm_kernel__projective_mask_sa(struct brw_compile *p, int dispatch_width);

bool gen8_wm_kernel__affine_opacity(struct brw_compile *p, int dispatch_width);
bool gen8_wm_kernel__projective_opacity(struct brw_compile *p, int dispatch_width);

#endif /* GEN8_EU_H */
