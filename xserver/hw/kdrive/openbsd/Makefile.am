# $OpenBSD: Makefile.am,v 1.1 2007/05/25 15:33:32 matthieu Exp $

INCLUDES = \
	@KDRIVE_INCS@ \
	@KDRIVE_CFLAGS@

AM_CFLAGS = -DHAVE_DIX_CONFIG_H

noinst_LIBRARIES =libopenbsd.a

if KDRIVE_HW
KDRIVE_HW_SOURCES = \
	agp.c 	\
	agp.h	\
	openbsd.c
endif

libopenbsd_a_SOURCES = \
	kopenbsd.h \
	$(KDRIVE_HW_SOURCES)

libopenbsd_a_DEPENDENCIES = \
	agp.c \
	openbsd.c
