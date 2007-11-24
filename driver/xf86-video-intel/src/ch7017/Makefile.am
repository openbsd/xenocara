# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.
AM_CFLAGS = @WARN_CFLAGS@ @XMODES_CFLAGS@ @XORG_CFLAGS@ @DRI_CFLAGS@

ch7017_la_LTLIBRARIES = ch7017.la
ch7017_la_LDFLAGS = -module -avoid-version
ch7017_ladir = @moduledir@/drivers

ch7017_la_SOURCES = \
	ch7017.c \
	ch7017_module.c \
	ch7017_reg.h
