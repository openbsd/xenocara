# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.
AM_CFLAGS = @WARN_CFLAGS@ @XMODES_CFLAGS@ @XORG_CFLAGS@ @DRI_CFLAGS@

ivch_la_LTLIBRARIES = ivch.la
ivch_la_LDFLAGS = -module -avoid-version
ivch_ladir = @moduledir@/drivers

ivch_la_SOURCES = \
	ivch.c \
	ivch_module.c \
	ivch_reg.h
