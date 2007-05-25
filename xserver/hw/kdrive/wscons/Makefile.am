# $OpenBSD: Makefile.am,v 1.1 2007/05/25 15:33:32 matthieu Exp $

INCLUDES = \
	@KDRIVE_INCS@ \
	@KDRIVE_CFLAGS@

noinst_LIBRARIES = libwscons.a

bin_PROGRAMS = Xwscons

libwscons_a_SOURCES = \
	keyboard.c \
	mouse.c \
	wsfb.c \
	wsfb.h

Xwscons_SOURCES = \
	wsinit.c

Xwscons_LDADD = \
	libwscons.a \
	@KDRIVE_LIBS@ \
	@XSERVER_LIBS@

Xwscons_DEPENDENCIES = \
	libwscons.a
