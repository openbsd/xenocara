#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

appmandir = $(APP_MAN_DIR)
appman_PRE = xenodm.man
appman_DATA = $(appman_PRE:man=$(APP_MAN_SUFFIX))

EXTRA_DIST = $(appman_PRE) $(xenodmlman_PRE)
CLEANFILES = $(appman_DATA) $(xenodmlman_DATA) $(noinst_DATA)
SUFFIXES = .$(APP_MAN_SUFFIX) .man

# String replacements in MAN_SUBSTS now come from xorg-macros.m4 via configure
MAN_SUBSTS +=	-e 's|XENODMLOGDIR|$(XENODMLOGDIR)|g' \
		-e 's|XENODMDIR|$(XENODMCONFIGDIR)|g' \
		-e 's|BINDIR|$(bindir)|g' \
		-e 's|XENODMXAUTHDIR|$(XENODMXAUTHDIR)|g' \
		-e 's|DEF_USER_PATH|$(DEF_USER_PATH)|g' \
		-e 's|DEF_SYSTEM_PATH|$(DEF_SYSTEM_PATH)|g'

.man.$(APP_MAN_SUFFIX):
	$(AM_V_GEN)$(SED) $(MAN_SUBSTS) < $< > $@
