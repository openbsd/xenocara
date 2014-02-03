#  Copyright 2005 Adam Jackson.
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  on the rights to use, copy, modify, merge, publish, distribute, sub
#  license, and/or sell copies of the Software, and to permit persons to whom
#  the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice (including the next
#  paragraph) shall be included in all copies or substantial portions of the
#  Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
#  ADAM JACKSON BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

AM_CFLAGS = \
	@CWARNFLAGS@ \
	$(TOOL_CFLAGS) \
	@NOWARNFLAGS@ \
	$(NULL)

drivermandir = $(DRIVER_MAN_DIR)

bin_PROGRAMS = intel-virtual-output
driverman_DATA = intel-virtual-output.$(DRIVER_MAN_SUFFIX)

intel_virtual_output_SOURCES = \
	virtual.c \
	$(NULL)

intel_virtual_output_LDADD = $(TOOL_LIBS)

EXTRA_DIST = intel-virtual-output.man
CLEANFILES = $(driverman_DATA)

# String replacements in MAN_SUBSTS now come from xorg-macros.m4 via configure
SUFFIXES = .$(DRIVER_MAN_SUFFIX) .man
.man.$(DRIVER_MAN_SUFFIX):
	$(AM_V_GEN)$(SED) $(MAN_SUBSTS) < $< > $@
