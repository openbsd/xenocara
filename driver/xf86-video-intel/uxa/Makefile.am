noinst_LTLIBRARIES = libuxa.la

# Override these since UXA doesn't need them and the needed files aren't
# built (in hw/xfree86/os-support/solaris) until after UXA is built
SOLARIS_ASM_CFLAGS=""

INCLUDES = \
	$(XORG_INCS)

AM_CFLAGS = $(WARN_CFLAGS) $(XORG_CFLAGS) $(DIX_CFLAGS)

libuxa_la_SOURCES = \
	uxa.c \
	uxa.h \
	uxa-accel.c \
	uxa-glyphs.c \
	uxa-render.c \
	uxa-priv.h \
	uxa-unaccel.c

