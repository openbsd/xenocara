#!/usr/bin/env python3
"""
Generate a PCF font that exercises the bitmap scaler path
(ZDI-CAN-30558).

The original bug: BitmapScaleBitmaps uses an 'unsigned int' (32-bit)
bytestoalloc accumulator. With many glyphs scaled to a large size, the
sum wraps around, causing an undersized calloc followed by heap overflow.

This test font has 64 small glyphs (1x2 pixels). When loaded at a large
pixel size via the scaler, each glyph expands significantly. With the
fix (size_t + overflow check), the library either:
- Succeeds with a properly-sized allocation (64-bit)
- Rejects the font cleanly (overflow check on 32-bit)

Usage:
    python3 gen_evil_pcf_bitmapscale.py <output_dir>

Creates <output_dir>/evil-scale.pcf and <output_dir>/fonts.dir
The font is stored at pixel=1; opening at pixel=1000 triggers scaling.
"""

import struct
import sys
import os

PCF_DEFAULT_FORMAT = 0x00000000
PCF_PROPERTIES = 1
PCF_ACCELERATORS = 2
PCF_METRICS = 4
PCF_BITMAPS = 8
PCF_BDF_ENCODINGS = 32


def u32(x):
    return struct.pack("<I", x & 0xFFFFFFFF)


def i32(x):
    return struct.pack("<i", x)


def u16(x):
    return struct.pack("<H", x & 0xFFFF)


def i16(x):
    return struct.pack("<h", x)


def u8(x):
    return struct.pack("<B", x & 0xFF)


def xCharInfo(lsb, rsb, cw, asc, desc, attr):
    return i16(lsb) + i16(rsb) + i16(cw) + i16(asc) + i16(desc) + u16(attr)


# 1105 glyphs with 2D encoding (5 rows x 221 cols).
# At scale factor ~16384, each glyph's BYTES_FOR_GLYPH is ~62 MiB.
# 1105 * 62 MiB ≈ 68 GB which wraps an unsigned int to ~1.5 MiB.
# With the fix (size_t + size limit), the true value is preserved and
# rejected by the size limit check. Without the fix, calloc allocates
# 1.5 MiB and ScaleBitmap writes 62 MiB past it.
NUM_GLYPHS = 1105

# 2D encoding: 5 rows x 221 cols = 1105 cells
ENC_FIRST_COL = 0
ENC_LAST_COL = 220
ENC_FIRST_ROW = 0
ENC_LAST_ROW = 4

# Per-glyph source metric: 1x2 pixels
SRC_LSB, SRC_RSB = 0, 1
SRC_ASCENT, SRC_DESCENT = 1, 1

# --- PROPERTIES ---
properties = b""
properties += u32(PCF_DEFAULT_FORMAT)
properties += i32(1)
properties += i32(0)
properties += u8(0)
properties += i32(0)
properties += b"\x00" * 3
properties += i32(4)
properties += b"FOO\x00"

# --- ACCELERATORS ---
accel = b""
accel += u32(PCF_DEFAULT_FORMAT)
accel += u8(0) * 8
accel += i32(SRC_ASCENT)
accel += i32(SRC_DESCENT)
accel += i32(0)
accel += xCharInfo(0, 0, 0, 0, 0, 0)
accel += xCharInfo(SRC_LSB, SRC_RSB, SRC_RSB, SRC_ASCENT, SRC_DESCENT, 0)

# --- METRICS ---
metrics = b""
metrics += u32(PCF_DEFAULT_FORMAT)
metrics += i32(NUM_GLYPHS)
for _ in range(NUM_GLYPHS):
    metrics += xCharInfo(SRC_LSB, SRC_RSB, SRC_RSB, SRC_ASCENT, SRC_DESCENT, 0)

# --- BITMAPS ---
# srcPad=1: each glyph = 1 byte/row * 2 rows = 2 bytes
bytes_per_glyph_src = 2
bytes_per_glyph_dst = 8  # pad=4: 4 bytes/row * 2 rows
sizebitmaps_src = NUM_GLYPHS * bytes_per_glyph_src
sizebitmaps_dst = NUM_GLYPHS * bytes_per_glyph_dst
BITMAP_SIZES = [sizebitmaps_src, NUM_GLYPHS * 3, sizebitmaps_dst, NUM_GLYPHS * 16]

bitmaps = b""
bitmaps += u32(PCF_DEFAULT_FORMAT)
bitmaps += i32(NUM_GLYPHS)
for i in range(NUM_GLYPHS):
    bitmaps += u32(i * bytes_per_glyph_src)
for sz in BITMAP_SIZES:
    bitmaps += u32(sz)
bitmaps += b"\xff\xff" * NUM_GLYPHS

# --- BDF_ENCODINGS (2D: 5 rows x 221 cols = 1105 cells) ---
encodings = b""
encodings += u32(PCF_DEFAULT_FORMAT)
encodings += u16(ENC_FIRST_COL)
encodings += u16(ENC_LAST_COL)
encodings += u16(ENC_FIRST_ROW)
encodings += u16(ENC_LAST_ROW)
encodings += u16(0)  # defaultCh
for i in range(NUM_GLYPHS):
    encodings += u16(i)

# --- Assemble ---
TABLE_COUNT = 5
BODY_START = 8 + TABLE_COUNT * 16

properties_off = BODY_START
accel_off = properties_off + len(properties)
metrics_off = accel_off + len(accel)
bitmaps_off = metrics_off + len(metrics)
encodings_off = bitmaps_off + len(bitmaps)


def toc(type_, format_, size, offset):
    return u32(type_) + u32(format_) + u32(size) + u32(offset)


out = b"\x01fcp" + u32(TABLE_COUNT)
out += toc(PCF_PROPERTIES, 0, len(properties), properties_off)
out += toc(PCF_ACCELERATORS, 0, len(accel), accel_off)
out += toc(PCF_METRICS, 0, len(metrics), metrics_off)
out += toc(PCF_BITMAPS, 0, len(bitmaps), bitmaps_off)
out += toc(PCF_BDF_ENCODINGS, 0, len(encodings), encodings_off)
out += properties + accel + metrics + bitmaps + encodings

output_dir = sys.argv[1] if len(sys.argv) > 1 else "."
os.makedirs(output_dir, exist_ok=True)

pcf_name = "evil-scale.pcf"
# Stored at pixel=1; test opens at different pixel to trigger scaler
xlfd = "-evil-scale-medium-r-normal--1-10-75-75-c-10-iso10646-1"

with open(os.path.join(output_dir, pcf_name), "wb") as f:
    f.write(out)

with open(os.path.join(output_dir, "fonts.dir"), "w") as f:
    f.write("1\n")
    f.write(f"{pcf_name} {xlfd}\n")

# fonts.scale declares the font as scalable (pixel, point, avgwidth = 0)
# so the scaler path is used when a different size is requested.
scalable_xlfd = "-evil-scale-medium-r-normal--0-0-75-75-c-0-iso10646-1"
with open(os.path.join(output_dir, "fonts.scale"), "w") as f:
    f.write("1\n")
    f.write(f"{pcf_name} {scalable_xlfd}\n")
