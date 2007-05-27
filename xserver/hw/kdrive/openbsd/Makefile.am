#

INCLUDES = \
	@KDRIVE_INCS@ \
	@KDRIVE_CFLAGS@

AM_CFLAGS = -DHAVE_DIX_CONFIG_H

noinst_LIBRARIES =libopenbsd.a

if KDRIVE_HW
KDRIVE_HW_SOURCES = \
	openbsd.c
endif

libopenbsd_a_SOURCES = \
	kopenbsd.h \
	$(KDRIVE_HW_SOURCES)

libopenbsd_a_DEPENDENCIES = \
	openbsd.c
