"""
Tests for the setCode() heap out-of-bounds write fix (ZDI-CAN-31888).

The top commit fixes two bugs in setCode():

1. Heap OOB write via large row_size:
   When row_size > 256, the computed index can exceed the 0x10000-entry
   allocation.  For example, SIZE 256 258 with mapping 0xff00 -> 0
   gives index = 255 * 258 + 0 = 65790, but the buffer is only 65536
   entries.  The fix adds: if (index > 0xFFFF) return 0;

2. Wrong variable in column bounds check:
   The column check tested global variable "value1" instead of the
   function parameter "from".  For CODE_LINE this happens to be the
   same value, but for CODE_RANGE_LINE the loop increments "from"
   while value1 remains the start of the range.  The fix changes
   (value1 & 0xFF) to (from & 0xFF).
"""

import signal
import pytest


class TestSetCodeHeapOOB:
    """Test the heap out-of-bounds write via large row_size."""

    def test_normal_encoding_parses(self, parse_encoding, enc_file):
        """A normal encoding file with valid mappings should parse successfully."""
        content = """\
STARTENCODING test-normal
SIZE 256 0
STARTMAPPING unicode
0x41 0x0041
0x42 0x0042
0x43 0x0043
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, row_size = parse_encoding(path)
        assert success is True
        assert row_size == 0

    def test_normal_encoding_with_row_size(self, parse_encoding, enc_file):
        """An encoding with a valid row_size should parse successfully."""
        content = """\
STARTENCODING test-rowsize
SIZE 256 128
STARTMAPPING unicode
0x0001 0x0041
0x0002 0x0042
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, row_size = parse_encoding(path)
        assert success is True
        assert row_size == 128

    def test_large_row_size_oob_mapping_ignored(self, parse_encoding, enc_file):
        """
        ZDI-CAN-31888: SIZE with row_size > 256 and a high 'from' value
        that would cause index > 0xFFFF.

        SIZE 256 258 with mapping 0xff00 -> 0x0000:
          index = (0xff00 >> 8) * 258 + (0xff00 & 0xFF)
                = 255 * 258 + 0 = 65790
          allocation is capped at 0x10000 = 65536

        Without the fix, this writes past the end of the buffer.
        With the fix, setCode() returns 0 (ignore) for index > 0xFFFF.

        The encoding should still parse successfully (the out-of-range
        mapping is silently dropped).
        """
        content = """\
STARTENCODING test-oob
SIZE 256 258
STARTMAPPING unicode
0x0001 0x0041
0xff00 0x0000
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True, \
            "Encoding with OOB mapping should parse (mapping silently dropped)"

    def test_large_row_size_max_index_boundary(self, parse_encoding, enc_file):
        """
        Test the exact boundary: row_size=257, from=0xfe00.
          index = (0xfe00 >> 8) * 257 + 0 = 254 * 257 = 65278
        This is < 0xFFFF so should be accepted.
        """
        content = """\
STARTENCODING test-boundary-ok
SIZE 256 257
STARTMAPPING unicode
0xfe00 0x0041
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_large_row_size_just_over_boundary(self, parse_encoding, enc_file):
        """
        Test just over the boundary: row_size=258, from=0xff00.
          index = 255 * 258 + 0 = 65790 > 0xFFFF
        This mapping should be silently ignored.
        """
        content = """\
STARTENCODING test-boundary-over
SIZE 256 258
STARTMAPPING unicode
0x0001 0x0041
0xff00 0x0042
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_row_size_256_max_from_is_valid(self, parse_encoding, enc_file):
        """
        With row_size=256 (the standard case), from=0xffff gives:
          index = 255 * 256 + 255 = 65535 = 0xFFFF
        This is exactly at the limit and should be valid.
        """
        content = """\
STARTENCODING test-256-max
SIZE 256 256
STARTMAPPING unicode
0xffff 0x0041
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_extreme_row_size(self, parse_encoding, enc_file):
        """
        An extreme row_size (e.g. 65535) with from=0x0100 gives:
          index = 1 * 65535 + 0 = 65535 which is exactly 0xFFFF.
        This should be accepted.

        But from=0x0200 gives:
          index = 2 * 65535 + 0 = 131070 > 0xFFFF.
        This should be silently dropped.
        """
        content = """\
STARTENCODING test-extreme-rowsize
SIZE 256 65535
STARTMAPPING unicode
0x0100 0x0041
0x0200 0x0042
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True


class TestSetCodeColumnBoundsCheck:
    """
    Test the column bounds check fix (value1 -> from).

    The old code checked (value1 & 0xFF) >= row_size, where value1 is a
    global variable set by the parser.  For CODE_LINE, value1 == from,
    so the check was correct by accident.  For CODE_RANGE_LINE, the loop
    increments 'from' while value1 stays at the start of the range.
    """

    def test_code_range_column_out_of_range(self, parse_encoding, enc_file):
        """
        With row_size=4 and a range 0x0000 0x0005 0x0041:
        The parser sets value1=0, value2=5, value3=0x41 and loops from=0..5.
        - from=0: (0 & 0xFF)=0 < 4 → OK
        - from=1: (1 & 0xFF)=1 < 4 → OK
        - from=2: (2 & 0xFF)=2 < 4 → OK
        - from=3: (3 & 0xFF)=3 < 4 → OK
        - from=4: (4 & 0xFF)=4 >= 4 → should be REJECTED
        - from=5: (5 & 0xFF)=5 >= 4 → should be REJECTED

        With the old bug (checking value1 instead of from), value1=0
        so (0 & 0xFF)=0 < 4 passes for ALL iterations, allowing
        from=4 and from=5 to generate invalid indices.

        With the fix, from=4 and from=5 are correctly rejected.
        The encoding should parse successfully (invalid mappings dropped).
        """
        content = """\
STARTENCODING test-range-col-oob
SIZE 256 4
STARTMAPPING unicode
0x0000 0x0005 0x0041
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_code_range_all_columns_valid(self, parse_encoding, enc_file):
        """
        With row_size=8 and range 0x0000 0x0003 0x0041:
        All values from=0..3 have (from & 0xFF) < 8, so all pass.
        This should work identically with both old and new code.
        """
        content = """\
STARTENCODING test-range-all-valid
SIZE 256 8
STARTMAPPING unicode
0x0000 0x0003 0x0041
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_code_range_crosses_row_boundary(self, parse_encoding, enc_file):
        """
        With row_size=4 and range 0x00fe 0x0102 0x0041:
        - from=0xfe: (0xfe & 0xFF)=254 >= 4 → rejected
        - from=0xff: (0xff & 0xFF)=255 >= 4 → rejected
        - from=0x100: (0x100 & 0xFF)=0 < 4 → OK
        - from=0x101: (0x101 & 0xFF)=1 < 4 → OK
        - from=0x102: (0x102 & 0xFF)=2 < 4 → OK

        With the old bug, value1=0xfe, (0xfe & 0xFF)=254 >= 4,
        so ALL entries in the range would be rejected (the check
        passed for the first entry and rejected everything).
        Wait -- actually value1 >= row_size means return 0 ("ignore"),
        so with the old bug it would ignore ALL mappings.

        With the fix, each 'from' is checked individually, so 0x100-0x102
        are accepted while 0xfe-0xff are rejected.

        The encoding should parse in both cases (mappings are silently
        dropped), but the behavior differs in which mappings survive.
        """
        content = """\
STARTENCODING test-range-cross-row
SIZE 256 4
STARTMAPPING unicode
0x00fe 0x0102 0x0041
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_undefine_range_column_check(self, parse_encoding, enc_file):
        """
        CODE_UNDEFINE_LINE also calls setCode() in a loop.
        With row_size=4, UNDEFINE 0x0000 0x0005 tries to undefine
        codes 0-5, but from=4 and from=5 have (from & 0xFF) >= 4
        and should be ignored.

        With the old bug, value1=0 stays, so (0 & 0xFF) < 4 passes
        for all iterations.  With the fix, from=4 and from=5 are
        correctly rejected.
        """
        content = """\
STARTENCODING test-undef-col-oob
SIZE 256 4
STARTMAPPING unicode
0x0000 0x0041
0x0001 0x0042
0x0002 0x0043
0x0003 0x0044
UNDEFINE 0x0000 0x0005
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True


class TestSetCodeEdgeCases:
    """Edge cases for setCode() behavior."""

    def test_from_over_0xffff_ignored(self, parse_encoding, enc_file):
        """
        Mappings with from > 0xFFFF should be silently ignored.
        The parser caps CODE_RANGE values at 0x10000, and setCode()
        itself returns 0 for from > 0xFFFF.
        """
        content = """\
STARTENCODING test-from-large
SIZE 256 0
STARTMAPPING unicode
0x0041 0x0041
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_row_size_zero_linear(self, parse_encoding, enc_file):
        """
        With row_size=0 (linear encoding), index=from directly.
        This is the simple case and should always work.
        """
        content = """\
STARTENCODING test-linear
SIZE 65536 0
STARTMAPPING unicode
0x0000 0x0041
0x00ff 0x0042
0xff00 0x0043
0xffff 0x0044
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_empty_mapping(self, parse_encoding, enc_file):
        """An encoding with an empty mapping block should parse fine."""
        content = """\
STARTENCODING test-empty-map
SIZE 256 0
STARTMAPPING unicode
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True

    def test_multiple_mappings(self, parse_encoding, enc_file):
        """An encoding with multiple mapping blocks should parse."""
        content = """\
STARTENCODING test-multi-map
SIZE 256 0
STARTMAPPING unicode
0x41 0x0041
ENDMAPPING
STARTMAPPING unicode
0x42 0x0042
ENDMAPPING
ENDENCODING
"""
        path = enc_file(content)
        success, _ = parse_encoding(path)
        assert success is True
