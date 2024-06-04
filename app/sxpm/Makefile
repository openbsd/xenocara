# $OpenBSD: Makefile,v 1.1 2024/06/04 05:42:22 matthieu Exp $

.include <bsd.xconf.mk>

PROG=sxpm
NOMAN=

CPPFLAGS+= -I${X11BASE}/include

LDADD+= -L${X11BASE}/lib -lXt -lX11 -lXext -lXpm

MANDIR= ${X11BASE}/man/man

obj: _xenocara_obj

.PATH.c: ${.CURDIR}/../../lib/libXpm/sxpm

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
