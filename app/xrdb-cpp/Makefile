# $OpenBSD: Makefile,v 1.2 2014/07/12 14:27:42 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xrdb-cpp
MAN=	xrdb-cpp.1

CPPFLAGS += -DUCPP_CONFIG -DSTAND_ALONE

SRCS= assert.c cpp.c eval.c lexer.c macro.c mem.c nhash.c

MANDIR=	${X11BASE}/man/man

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
