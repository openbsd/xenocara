#  Copyright 2005 Red Hat, Inc.
#
#  Permission to use, copy, modify, distribute, and sell this software
#  and its documentation for any purpose is hereby granted without
#  fee, provided that the above copyright notice appear in all copies
#  and that both that copyright notice and this permission notice
#  appear in supporting documentation, and that the name of Red Hat
#  not be used in advertising or publicity pertaining to distribution
#  of the software without specific, written prior permission.  Red
#  Hat makes no representations about the suitability of this software
#  for any purpose.  It is provided "as is" without express or implied
#  warranty.
#
#  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
#  NO EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
#  OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
#  NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
#  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

FONT_FILES = arabic24

BDF_FILES = $(FONT_FILES:%=%.bdf)
PCF_FILES = $(FONT_FILES:%=%.pcf$(COMPRESS_SUFFIX))

SUFFIXES = .bdf .pcf$(COMPRESS_SUFFIX)

.bdf.pcf$(COMPRESS_SUFFIX):
	@rm -f $@
	$(AM_V_GEN)$(BDFTOPCF) -t $< | $(COMPRESS) $(COMPRESS_FLAGS) > $@

fontdir = @FONTDIR@
font_DATA = $(PCF_FILES)
CLEANFILES = $(font_DATA)

EXTRA_DIST = $(BDF_FILES) uniarab.txt README.md

MAINTAINERCLEANFILES = ChangeLog INSTALL

install-data-hook:
	@rm -f $(DESTDIR)$(fontdir)/fonts.dir
	$(MKFONTDIR) $(DESTDIR)$(fontdir)
	@RUN_FCCACHE@

distuninstallcheck:
	@:


.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
