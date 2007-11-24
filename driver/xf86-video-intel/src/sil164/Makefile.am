# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.
AM_CFLAGS = @WARN_CFLAGS@ @XMODES_CFLAGS@ @XORG_CFLAGS@ @DRI_CFLAGS@

sil164_la_LTLIBRARIES = sil164.la
sil164_la_LDFLAGS = -module -avoid-version
sil164_ladir = @moduledir@/drivers

sil164_la_SOURCES = \
	sil164.c \
	sil164_module.c \
	sil164.h \
	sil164_reg.h
