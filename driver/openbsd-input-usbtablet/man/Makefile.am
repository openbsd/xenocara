#
# Copyright (c) 2006 Matthieu Herrb
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
drivermandir= $(DRIVER_MAN_DIR)
driverman_PRE= @DRIVER_NAME@.man
driverman_DATA= $(driverman_PRE:man=@DRIVER_MAN_SUFFIX@)

EXTRA_DIST= @DRIVER_NAME@.man

CLEANFILES= $(driverman_DATA)

SED= sed

# Strings to replace in man pages
XORGRELSTRING = @PACKAGE_STRING@
  XORGMANNAME = X Version 11

MAN_SUBSTS = \
        -e 's|__vendorversion__|"$(XORGRELSTRING)" "$(XORGMANNAME)"|' \
        -e 's|__xorgversion__|"$(XORGRELSTRING)" "$(XORGMANNAME)"|' \
        -e 's|__xservername__|Xorg|g' \
        -e 's|__xconfigfile__|xorg.conf|g' \
        -e 's|__projectroot__|$(prefix)|g' \
        -e 's|__appmansuffix__|$(APP_MAN_SUFFIX)|g' \
        -e 's|__drivermansuffix__|$(DRIVER_MAN_SUFFIX)|g' \
        -e 's|__adminmansuffix__|$(ADMIN_MAN_SUFFIX)|g' \
        -e 's|__miscmansuffix__|$(MISC_MAN_SUFFIX)|g' \
        -e 's|__filemansuffix__|$(FILE_MAN_SUFFIX)|g'

SUFFIXES = .$(DRIVER_MAN_SUFFIX) .man

.man.$(DRIVER_MAN_SUFFIX):
	sed $(MAN_SUBSTS) < $< > $@
