noinst_LTLIBRARIES = libuxa.la

# Override these since UXA doesn't need them and the needed files aren't
# built (in hw/xfree86/os-support/solaris) until after UXA is built
SOLARIS_ASM_CFLAGS=""

AM_CFLAGS = $(CWARNFLAGS) $(XORG_CFLAGS)

if GLAMOR
AM_CFLAGS += @LIBGLAMOR_CFLAGS@
libuxa_la_LIBADD = @LIBGLAMOR_LIBS@
endif

libuxa_la_SOURCES = \
	uxa.c \
	uxa.h \
	uxa-accel.c \
	uxa-glamor.h \
	uxa-glyphs.c \
	uxa-render.c \
	uxa-priv.h \
	uxa-unaccel.c

