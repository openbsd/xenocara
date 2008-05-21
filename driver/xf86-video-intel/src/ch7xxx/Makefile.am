# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.
AM_CFLAGS = @WARN_CFLAGS@ @XMODES_CFLAGS@ @XORG_CFLAGS@ @DRI_CFLAGS@ \
	@PCIACCESS_CFLAGS@

ch7xxx_la_LTLIBRARIES = ch7xxx.la
ch7xxx_la_LDFLAGS = -module -avoid-version
ch7xxx_ladir = @moduledir@/drivers

ch7xxx_la_SOURCES = \
	ch7xxx.c \
	ch7xxx_module.c \
	ch7xxx.h \
	ch7xxx_reg.h
