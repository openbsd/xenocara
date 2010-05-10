# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.
AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ \
	@PCIACCESS_CFLAGS@

tfp410_la_LTLIBRARIES = tfp410.la
tfp410_la_LDFLAGS = -module -avoid-version
tfp410_ladir = @moduledir@/drivers

tfp410_la_SOURCES = \
	tfp410.c \
	tfp410_module.c \
	tfp410.h \
	tfp410_reg.h
