#	$OpenBSD: Makefile.inc,v 1.1 2013/03/09 14:34:40 matthieu Exp $

X11BASE ?=	/usr/X11R6

FC_CACHEDIR =	/var/cache/fontconfig
BASECONFIGDIR =	/etc/fonts
MANDIR =	${X11BASE}/man/man

CFLAGS +=	-Wall -Wpointer-arith -Wstrict-prototypes \
		-Wmissing-prototypes -Wmissing-declarations \
		-Wnested-externs -fno-strict-aliasing
CFLAGS +=	-DHAVE_CONFIG_H
CFLAGS +=	-I${FONTCONFIG} \
		-I${X11BASE}/include -I${X11BASE}/include/freetype2 \
		-I${.CURDIR}/../src -I${FONTCONFIG}/src \
		-I${.CURDIR}/../common \
		-I${.CURDIR}

NOPROFILE =
