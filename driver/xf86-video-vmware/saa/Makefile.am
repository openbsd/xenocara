
if BUILD_VMWGFX
noinst_LTLIBRARIES = libsaa.la

libsaa_la_CFLAGS = $(CWARNFLAGS) $(XORG_CFLAGS) -I$(top_srcdir)/src
libsaa_la_LDFLAGS = -static
libsaa_la_SOURCES = \
	saa.c \
	saa_pixmap.c \
	saa_unaccel.c \
	saa_priv.h \
	saa_render.c \
	saa_accel.c \
	saa.h
endif
