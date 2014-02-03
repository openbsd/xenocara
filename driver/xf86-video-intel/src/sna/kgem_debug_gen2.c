/*
 * Copyright © 2007-2011 Intel Corporation
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
 *    Eric Anholt <eric@anholt.net>
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/mman.h>
#include <assert.h>

#include "sna.h"

#include "gen2_render.h"

#include "kgem_debug.h"

static struct state {
	int vertex_format;
} state;

static inline float int_as_float(uint32_t dw)
{
	union {
		float f;
		uint32_t dw;
	} u;
	u.dw = dw;
	return u.f;
}

static int
decode_3d_primitive(struct kgem *kgem, uint32_t offset)
{
    uint32_t *data = kgem->batch + offset;
    char immediate = (data[0] & (1 << 23)) == 0;
    unsigned int len;
    const char *primtype;

    switch ((data[0] >> 18) & 0xf) {
    case 0x0: primtype = "TRILIST"; break;
    case 0x1: primtype = "TRISTRIP"; break;
    case 0x2: primtype = "TRISTRIP_REVERSE"; break;
    case 0x3: primtype = "TRIFAN"; break;
    case 0x4: primtype = "POLYGON"; break;
    case 0x5: primtype = "LINELIST"; break;
    case 0x6: primtype = "LINESTRIP"; break;
    case 0x7: primtype = "RECTLIST"; break;
    case 0x8: primtype = "POINTLIST"; break;
    case 0x9: primtype = "DIB"; break;
    case 0xa: primtype = "CLEAR_RECT"; break;
    default: primtype = "unknown"; break;
    }

    /* XXX: 3DPRIM_DIB not supported */
    if (immediate) {
	len = (data[0] & 0x0003ffff) + 2;
	kgem_debug_print(data, offset, 0, "3DPRIMITIVE inline %s\n", primtype);
#if 0
	if (!saved_s2_set || !saved_s4_set) {
	    fprintf(out, "unknown vertex format\n");
	    for (i = 1; i < len; i++) {
		kgem_debug_print(data, offset, i,
			  "           vertex data (%f float)\n",
			  int_as_float(data[i]));
	    }
	} else {
	    unsigned int vertex = 0;
	    for (i = 1; i < len;) {
		unsigned int tc;

#define VERTEX_OUT(fmt, ...) do {					\
    if (i < len)							\
	kgem_debug_print(data, offset, i, " V%d."fmt"\n", vertex, __VA_ARGS__); \
    else								\
	fprintf(out, " missing data in V%d\n", vertex);			\
    i++;								\
} while (0)

		VERTEX_OUT("X = %f", int_as_float(data[i]));
		VERTEX_OUT("Y = %f", int_as_float(data[i]));
	        switch (saved_s4 >> 6 & 0x7) {
		case 0x1:
		    VERTEX_OUT("Z = %f", int_as_float(data[i]));
		    break;
		case 0x2:
		    VERTEX_OUT("Z = %f", int_as_float(data[i]));
		    VERTEX_OUT("W = %f", int_as_float(data[i]));
		    break;
		case 0x3:
		    break;
		case 0x4:
		    VERTEX_OUT("W = %f", int_as_float(data[i]));
		    break;
		default:
		    fprintf(out, "bad S4 position mask\n");
		}

		if (saved_s4 & (1 << 10)) {
		    VERTEX_OUT("color = (A=0x%02x, R=0x%02x, G=0x%02x, "
			       "B=0x%02x)",
			       data[i] >> 24,
			       (data[i] >> 16) & 0xff,
			       (data[i] >> 8) & 0xff,
			       data[i] & 0xff);
		}
		if (saved_s4 & (1 << 11)) {
		    VERTEX_OUT("spec = (A=0x%02x, R=0x%02x, G=0x%02x, "
			       "B=0x%02x)",
			       data[i] >> 24,
			       (data[i] >> 16) & 0xff,
			       (data[i] >> 8) & 0xff,
			       data[i] & 0xff);
		}
		if (saved_s4 & (1 << 12))
		    VERTEX_OUT("width = 0x%08x)", data[i]);

		for (tc = 0; tc <= 7; tc++) {
		    switch ((saved_s2 >> (tc * 4)) & 0xf) {
		    case 0x0:
			VERTEX_OUT("T%d.X = %f", tc, int_as_float(data[i]));
			VERTEX_OUT("T%d.Y = %f", tc, int_as_float(data[i]));
			break;
		    case 0x1:
			VERTEX_OUT("T%d.X = %f", tc, int_as_float(data[i]));
			VERTEX_OUT("T%d.Y = %f", tc, int_as_float(data[i]));
			VERTEX_OUT("T%d.Z = %f", tc, int_as_float(data[i]));
			break;
		    case 0x2:
			VERTEX_OUT("T%d.X = %f", tc, int_as_float(data[i]));
			VERTEX_OUT("T%d.Y = %f", tc, int_as_float(data[i]));
			VERTEX_OUT("T%d.Z = %f", tc, int_as_float(data[i]));
			VERTEX_OUT("T%d.W = %f", tc, int_as_float(data[i]));
			break;
		    case 0x3:
			VERTEX_OUT("T%d.X = %f", tc, int_as_float(data[i]));
			break;
		    case 0x4:
			VERTEX_OUT("T%d.XY = 0x%08x half-float", tc, data[i]);
			break;
		    case 0x5:
			VERTEX_OUT("T%d.XY = 0x%08x half-float", tc, data[i]);
			VERTEX_OUT("T%d.ZW = 0x%08x half-float", tc, data[i]);
			break;
		    case 0xf:
			break;
		    default:
			fprintf(out, "bad S2.T%d format\n", tc);
		    }
		}
		vertex++;
	    }
	}
#endif
    } else {
	/* indirect vertices */
	len = data[0] & 0x0000ffff; /* index count */
#if 0
	if (data[0] & (1 << 17)) {
	    /* random vertex access */
	    kgem_debug_print(data, offset, 0,
		      "3DPRIMITIVE random indirect %s (%d)\n", primtype, len);
	    if (len == 0) {
		/* vertex indices continue until 0xffff is found */
		for (i = 1; i < count; i++) {
		    if ((data[i] & 0xffff) == 0xffff) {
			kgem_debug_print(data, offset, i,
				  "    indices: (terminator)\n");
			ret = i;
			goto out;
		    } else if ((data[i] >> 16) == 0xffff) {
			kgem_debug_print(data, offset, i,
				  "    indices: 0x%04x, (terminator)\n",
				  data[i] & 0xffff);
			ret = i;
			goto out;
		    } else {
			kgem_debug_print(data, offset, i,
				  "    indices: 0x%04x, 0x%04x\n",
				  data[i] & 0xffff, data[i] >> 16);
		    }
		}
		fprintf(out,
			"3DPRIMITIVE: no terminator found in index buffer\n");
		ret = count;
		goto out;
	    } else {
		/* fixed size vertex index buffer */
		for (j = 1, i = 0; i < len; i += 2, j++) {
		    if (i * 2 == len - 1) {
			kgem_debug_print(data, offset, j,
				  "    indices: 0x%04x\n",
				  data[j] & 0xffff);
		    } else {
			kgem_debug_print(data, offset, j,
				  "    indices: 0x%04x, 0x%04x\n",
				  data[j] & 0xffff, data[j] >> 16);
		    }
		}
	    }
	    ret = (len + 1) / 2 + 1;
	    goto out;
	} else {
	    /* sequential vertex access */
	    kgem_debug_print(data, offset, 0,
		      "3DPRIMITIVE sequential indirect %s, %d starting from "
		      "%d\n", primtype, len, data[1] & 0xffff);
	    kgem_debug_print(data, offset, 1, "           start\n");
	    ret = 2;
	    goto out;
	}
#endif
    }

    return len;
}

static int
decode_3d_1d(struct kgem *kgem, uint32_t offset)
{
    uint32_t *data = kgem->batch + offset;
    unsigned int len, i, idx, word, map;
    const char *format, *zformat, *type;
    uint32_t opcode;

    static const struct {
	uint32_t opcode;
	int min_len;
	int max_len;
	const char *name;
    } opcodes_3d_1d[] = {
	{ 0x86, 4, 4, "3DSTATE_CHROMA_KEY" },
	{ 0x88, 2, 2, "3DSTATE_CONSTANT_BLEND_COLOR" },
	{ 0x99, 2, 2, "3DSTATE_DEFAULT_DIFFUSE" },
	{ 0x9a, 2, 2, "3DSTATE_DEFAULT_SPECULAR" },
	{ 0x98, 2, 2, "3DSTATE_DEFAULT_Z" },
	{ 0x97, 2, 2, "3DSTATE_DEPTH_OFFSET_SCALE" },
	{ 0x9d, 65, 65, "3DSTATE_FILTER_COEFFICIENTS_4X4" },
	{ 0x9e, 4, 4, "3DSTATE_MONO_FILTER" },
	{ 0x89, 4, 4, "3DSTATE_FOG_MODE" },
	{ 0x8f, 2, 16, "3DSTATE_MAP_PALLETE_LOAD_32" },
	{ 0x83, 2, 2, "3DSTATE_SPAN_STIPPLE" },
	{ 0x8c, 2, 2, "3DSTATE_MAP_COORD_TRANSFORM" },
	{ 0x8b, 2, 2, "3DSTATE_MAP_VERTEX_TRANSFORM" },
	{ 0x8d, 3, 3, "3DSTATE_W_STATE" },
	{ 0x01, 2, 2, "3DSTATE_COLOR_FACTOR" },
	{ 0x02, 2, 2, "3DSTATE_MAP_COORD_SETBIND" },
    }, *opcode_3d_1d;

    opcode = (data[0] & 0x00ff0000) >> 16;

    switch (opcode) {
    case 0x07:
	/* This instruction is unusual.  A 0 length means just 1 DWORD instead of
	 * 2.  The 0 length is specified in one place to be unsupported, but
	 * stated to be required in another, and 0 length LOAD_INDIRECTs appear
	 * to cause no harm at least.
	 */
	kgem_debug_print(data, offset, 0, "3DSTATE_LOAD_INDIRECT\n");
	len = (data[0] & 0x000000ff) + 1;
	i = 1;
	if (data[0] & (0x01 << 8)) {
	    kgem_debug_print(data, offset, i++, "SIS.0\n");
	    kgem_debug_print(data, offset, i++, "SIS.1\n");
	}
	if (data[0] & (0x02 << 8)) {
	    kgem_debug_print(data, offset, i++, "DIS.0\n");
	}
	if (data[0] & (0x04 << 8)) {
	    kgem_debug_print(data, offset, i++, "SSB.0\n");
	    kgem_debug_print(data, offset, i++, "SSB.1\n");
	}
	if (data[0] & (0x08 << 8)) {
	    kgem_debug_print(data, offset, i++, "MSB.0\n");
	    kgem_debug_print(data, offset, i++, "MSB.1\n");
	}
	if (data[0] & (0x10 << 8)) {
	    kgem_debug_print(data, offset, i++, "PSP.0\n");
	    kgem_debug_print(data, offset, i++, "PSP.1\n");
	}
	if (data[0] & (0x20 << 8)) {
	    kgem_debug_print(data, offset, i++, "PSC.0\n");
	    kgem_debug_print(data, offset, i++, "PSC.1\n");
	}
	assert(len == i);
	return len;
    case 0x04:
	kgem_debug_print(data, offset, 0, "3DSTATE_LOAD_STATE_IMMEDIATE_1\n");
	len = (data[0] & 0x0000000f) + 2;
	i = 1;
	for (word = 0; word <= 8; word++) {
	    if (data[0] & (1 << (4 + word))) {
		kgem_debug_print(data, offset, i, "S%d: 0x%08x\n", i, data[i]);
		i++;
	    }
	}
	assert (len ==i);
	return len;
    case 0x03:
	kgem_debug_print(data, offset, 0, "3DSTATE_LOAD_STATE_IMMEDIATE_2\n");
	len = (data[0] & 0x0000000f) + 2;
	i = 1;
	for (word = 6; word <= 14; word++) {
	    if (data[0] & (1 << word)) {
		if (word == 6)
		    kgem_debug_print(data, offset, i++, "TBCF\n");
		else if (word >= 7 && word <= 10) {
		    kgem_debug_print(data, offset, i++, "TB%dC\n", word - 7);
		    kgem_debug_print(data, offset, i++, "TB%dA\n", word - 7);
		} else if (word >= 11 && word <= 14) {
		    kgem_debug_print(data, offset, i, "TM%dS0: offset=0x%08x, %s\n",
			      word - 11,
			      data[i]&0xfffffffe,
			      data[i]&1?"use fence":"");
		    i++;
		    kgem_debug_print(data, offset, i, "TM%dS1: height=%i, width=%i, %s\n",
			      word - 11,
			      data[i]>>21, (data[i]>>10)&0x3ff,
			      data[i]&2?(data[i]&1?"y-tiled":"x-tiled"):"");
		    i++;
		    kgem_debug_print(data, offset, i, "TM%dS2: pitch=%i, \n",
			      word - 11,
			      ((data[i]>>21) + 1)*4);
		    i++;
		    kgem_debug_print(data, offset, i++, "TM%dS3\n", word - 11);
		    kgem_debug_print(data, offset, i++, "TM%dS4: dflt color\n", word - 11);
		}
	    }
	}
	assert (len == i);
	return len;
    case 0x00:
	kgem_debug_print(data, offset, 0, "3DSTATE_MAP_STATE\n");
	len = (data[0] & 0x0000003f) + 2;
	kgem_debug_print(data, offset, 1, "mask\n");

	i = 2;
	for (map = 0; map <= 15; map++) {
	    if (data[1] & (1 << map)) {
		int width, height, pitch, dword;
		const char *tiling;

		dword = data[i];
		kgem_debug_print(data, offset, i++, "map %d MS2 %s%s%s\n", map,
			  dword&(1<<31)?"untrusted surface, ":"",
			  dword&(1<<1)?"vertical line stride enable, ":"",
			  dword&(1<<0)?"vertical ofs enable, ":"");

		dword = data[i];
		width = ((dword >> 10) & ((1 << 11) - 1))+1;
		height = ((dword >> 21) & ((1 << 11) - 1))+1;

		tiling = "none";
		if (dword & (1 << 2))
			tiling = "fenced";
		else if (dword & (1 << 1))
			tiling = dword & (1 << 0) ? "Y" : "X";
		type = " BAD";
		format = "BAD";
		switch ((dword>>7) & 0x7) {
		case 1:
		    type = "8b";
		    switch ((dword>>3) & 0xf) {
		    case 0: format = "I"; break;
		    case 1: format = "L"; break;
		    case 2: format = "A"; break;
		    case 3: format = " mono"; break; }
		    break;
		case 2:
		    type = "16b";
		    switch ((dword>>3) & 0xf) {
		    case 0: format = " rgb565"; break;
		    case 1: format = " argb1555"; break;
		    case 2: format = " argb4444"; break;
		    case 5: format = " ay88"; break;
		    case 6: format = " bump655"; break;
		    case 7: format = "I"; break;
		    case 8: format = "L"; break;
		    case 9: format = "A"; break; }
		    break;
		case 3:
		    type = "32b";
		    switch ((dword>>3) & 0xf) {
		    case 0: format = " argb8888"; break;
		    case 1: format = " abgr8888"; break;
		    case 2: format = " xrgb8888"; break;
		    case 3: format = " xbgr8888"; break;
		    case 4: format = " qwvu8888"; break;
		    case 5: format = " axvu8888"; break;
		    case 6: format = " lxvu8888"; break;
		    case 7: format = " xlvu8888"; break;
		    case 8: format = " argb2101010"; break;
		    case 9: format = " abgr2101010"; break;
		    case 10: format = " awvu2101010"; break;
		    case 11: format = " gr1616"; break;
		    case 12: format = " vu1616"; break;
		    case 13: format = " xI824"; break;
		    case 14: format = " xA824"; break;
		    case 15: format = " xL824"; break; }
		    break;
		case 5:
		    type = "422";
		    switch ((dword>>3) & 0xf) {
		    case 0: format = " yuv_swapy"; break;
		    case 1: format = " yuv"; break;
		    case 2: format = " yuv_swapuv"; break;
		    case 3: format = " yuv_swapuvy"; break; }
		    break;
		case 6:
		    type = "compressed";
		    switch ((dword>>3) & 0x7) {
		    case 0: format = " dxt1"; break;
		    case 1: format = " dxt2_3"; break;
		    case 2: format = " dxt4_5"; break;
		    case 3: format = " fxt1"; break;
		    case 4: format = " dxt1_rb"; break; }
		    break;
		case 7:
		    type = "4b indexed";
		    switch ((dword>>3) & 0xf) {
		    case 7: format = " argb8888"; break; }
		    break;
		}
		dword = data[i];
		kgem_debug_print(data, offset, i++, "map %d MS3 [width=%d, height=%d, format=%s%s, tiling=%s%s]\n",
			  map, width, height, type, format, tiling,
			  dword&(1<<9)?" palette select":"");

		dword = data[i];
		pitch = 4*(((dword >> 21) & ((1 << 11) - 1))+1);
		kgem_debug_print(data, offset, i++, "map %d MS4 [pitch=%d, max_lod=%i, vol_depth=%i, cube_face_ena=%x, %s]\n",
			  map, pitch,
			  (dword>>9)&0x3f, dword&0xff, (dword>>15)&0x3f,
			  dword&(1<<8)?"miplayout legacy":"miplayout right");
	    }
	}
	assert (len == i);
	return len;
    case 0x85:
	len = (data[0] & 0x0000000f) + 2;
	assert (len == 2);
	kgem_debug_print(data, offset, 0,
		  "3DSTATE_DEST_BUFFER_VARIABLES\n");

	switch ((data[1] >> 8) & 0xf) {
	case 0x0: format = "g8"; break;
	case 0x1: format = "x1r5g5b5"; break;
	case 0x2: format = "r5g6b5"; break;
	case 0x3: format = "a8r8g8b8"; break;
	case 0x4: format = "ycrcb_swapy"; break;
	case 0x5: format = "ycrcb_normal"; break;
	case 0x6: format = "ycrcb_swapuv"; break;
	case 0x7: format = "ycrcb_swapuvy"; break;
	case 0x8: format = "a4r4g4b4"; break;
	case 0x9: format = "a1r5g5b5"; break;
	case 0xa: format = "a2r10g10b10"; break;
	default: format = "BAD"; break;
	}
	switch ((data[1] >> 2) & 0x3) {
	case 0x0: zformat = "u16"; break;
	case 0x1: zformat = "f16"; break;
	case 0x2: zformat = "u24x8"; break;
	default: zformat = "BAD"; break;
	}
	kgem_debug_print(data, offset, 1, "%s format, %s depth format, early Z %sabled\n",
		  format, zformat,
		  (data[1] & (1 << 31)) ? "en" : "dis");
	return len;

    case 0x8e:
	{
	    const char *name, *tiling;

	    len = (data[0] & 0x0000000f) + 2;
	    assert (len == 3);

	    switch((data[1] >> 24) & 0x7) {
	    case 0x3: name = "color"; break;
	    case 0x7: name = "depth"; break;
	    default: name = "unknown"; break;
	    }

	    tiling = "none";
	    if (data[1] & (1 << 23))
		tiling = "fenced";
	    else if (data[1] & (1 << 22))
		tiling = data[1] & (1 << 21) ? "Y" : "X";

	    kgem_debug_print(data, offset, 0, "3DSTATE_BUFFER_INFO\n");
	    kgem_debug_print(data, offset, 1, "%s, tiling = %s, pitch=%d\n", name, tiling, data[1]&0xffff);

	    kgem_debug_print(data, offset, 2, "address\n");
	    return len;
	}

    case 0x81:
	len = (data[0] & 0x0000000f) + 2;
	assert (len == 3);

	kgem_debug_print(data, offset, 0,
		  "3DSTATE_SCISSOR_RECTANGLE\n");
	kgem_debug_print(data, offset, 1, "(%d,%d)\n",
		  data[1] & 0xffff, data[1] >> 16);
	kgem_debug_print(data, offset, 2, "(%d,%d)\n",
		  data[2] & 0xffff, data[2] >> 16);
	return len;

    case 0x80:
	len = (data[0] & 0x0000000f) + 2;
	assert (len == 5);

	kgem_debug_print(data, offset, 0,
		  "3DSTATE_DRAWING_RECTANGLE\n");
	kgem_debug_print(data, offset, 1, "%s\n",
		  data[1]&(1<<30)?"depth ofs disabled ":"");
	kgem_debug_print(data, offset, 2, "(%d,%d)\n",
		  data[2] & 0xffff, data[2] >> 16);
	kgem_debug_print(data, offset, 3, "(%d,%d)\n",
		  data[3] & 0xffff, data[3] >> 16);
	kgem_debug_print(data, offset, 4, "(%d,%d)\n",
		  data[4] & 0xffff, data[4] >> 16);
	return len;

    case 0x9c:
	len = (data[0] & 0x0000000f) + 2;
	assert (len == 7);

	kgem_debug_print(data, offset, 0,
		  "3DSTATE_CLEAR_PARAMETERS\n");
	kgem_debug_print(data, offset, 1, "prim_type=%s, clear=%s%s%s\n",
		  data[1]&(1<<16)?"CLEAR_RECT":"ZONE_INIT",
		  data[1]&(1<<2)?"color,":"",
		  data[1]&(1<<1)?"depth,":"",
		  data[1]&(1<<0)?"stencil,":"");
	kgem_debug_print(data, offset, 2, "clear color\n");
	kgem_debug_print(data, offset, 3, "clear depth/stencil\n");
	kgem_debug_print(data, offset, 4, "color value (rgba8888)\n");
	kgem_debug_print(data, offset, 5, "depth value %f\n",
		  int_as_float(data[5]));
	kgem_debug_print(data, offset, 6, "clear stencil\n");
	return len;
    }

    for (idx = 0; idx < ARRAY_SIZE(opcodes_3d_1d); idx++) {
	opcode_3d_1d = &opcodes_3d_1d[idx];
	if (((data[0] & 0x00ff0000) >> 16) == opcode_3d_1d->opcode) {
	    len = 1;

	    kgem_debug_print(data, offset, 0, "%s\n", opcode_3d_1d->name);
	    if (opcode_3d_1d->max_len > 1) {
		len = (data[0] & 0x0000ffff) + 2;
		assert (len >= opcode_3d_1d->min_len &&
			len <= opcode_3d_1d->max_len);
	    }

	    for (i = 1; i < len; i++)
		kgem_debug_print(data, offset, i, "dword %d\n", i);

	    return len;
	}
    }

    kgem_debug_print(data, offset, 0, "3D UNKNOWN: 3d_1d opcode = 0x%x\n", opcode);
    return 1;
}

static int
decode_3d_1c(struct kgem *kgem, uint32_t offset)
{
    uint32_t *data = kgem->batch + offset;
    uint32_t opcode;

    opcode = (data[0] & 0x00f80000) >> 19;

    switch (opcode) {
    case 0x11:
	kgem_debug_print(data, offset, 0, "3DSTATE_DEPTH_SUBRECTANGLE_DISABLE\n");
	return 1;
    case 0x10:
	kgem_debug_print(data, offset, 0, "3DSTATE_SCISSOR_ENABLE %s\n",
		data[0]&1?"enabled":"disabled");
	return 1;
    case 0x01:
	kgem_debug_print(data, offset, 0, "3DSTATE_MAP_COORD_SET_I830\n");
	return 1;
    case 0x0a:
	kgem_debug_print(data, offset, 0, "3DSTATE_MAP_CUBE_I830\n");
	return 1;
    case 0x05:
	kgem_debug_print(data, offset, 0, "3DSTATE_MAP_TEX_STREAM_I830\n");
	return 1;
    }

    kgem_debug_print(data, offset, 0, "3D UNKNOWN: 3d_1c opcode = 0x%x\n",
	      opcode);
    return 1;
}

int kgem_gen2_decode_3d(struct kgem *kgem, uint32_t offset)
{
    const static struct {
	uint32_t opcode;
	int min_len;
	int max_len;
	const char *name;
    } opcodes[] = {
	{ 0x02, 1, 1, "3DSTATE_MODES_3" },
	{ 0x03, 1, 1, "3DSTATE_ENABLES_1"},
	{ 0x04, 1, 1, "3DSTATE_ENABLES_2"},
	{ 0x05, 1, 1, "3DSTATE_VFT0"},
	{ 0x06, 1, 1, "3DSTATE_AA"},
	{ 0x07, 1, 1, "3DSTATE_RASTERIZATION_RULES" },
	{ 0x08, 1, 1, "3DSTATE_MODES_1" },
	{ 0x09, 1, 1, "3DSTATE_STENCIL_TEST" },
	{ 0x0a, 1, 1, "3DSTATE_VFT1"},
	{ 0x0b, 1, 1, "3DSTATE_INDPT_ALPHA_BLEND" },
	{ 0x0c, 1, 1, "3DSTATE_MODES_5" },
	{ 0x0d, 1, 1, "3DSTATE_MAP_BLEND_OP" },
	{ 0x0e, 1, 1, "3DSTATE_MAP_BLEND_ARG" },
	{ 0x0f, 1, 1, "3DSTATE_MODES_2" },
	{ 0x15, 1, 1, "3DSTATE_FOG_COLOR" },
	{ 0x16, 1, 1, "3DSTATE_MODES_4" },
    };
    uint32_t *data = kgem->batch + offset;
    uint32_t opcode = (data[0] & 0x1f000000) >> 24;
    uint32_t idx;

    switch (opcode) {
    case 0x1f:
	return decode_3d_primitive(kgem, offset);
    case 0x1d:
	return decode_3d_1d(kgem, offset);
    case 0x1c:
	return decode_3d_1c(kgem, offset);
    }

    /* Catch the known instructions */
    for (idx = 0; idx < ARRAY_SIZE(opcodes); idx++) {
	if (opcode == opcodes[idx].opcode) {
	    unsigned int len = 1, i;

	    kgem_debug_print(data, offset, 0, "%s\n", opcodes[idx].name);
	    if (opcodes[idx].max_len > 1) {
		len = (data[0] & 0xf) + 2;
		assert(len >= opcodes[idx].min_len &&
		       len <= opcodes[idx].max_len);
	    }

	    for (i = 1; i < len; i++)
		kgem_debug_print(data, offset, i, "dword %d\n", i);
	    return len;
	}
    }

    kgem_debug_print(data, offset, 0, "3D UNKNOWN: 3d opcode = 0x%x\n", opcode);
    return 1;
}

void kgem_gen2_finish_state(struct kgem *kgem)
{
	memset(&state, 0, sizeof(state));
}
