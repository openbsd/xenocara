# $OpenBSD: Makefile,v 1.22 2016/09/20 18:21:32 okan Exp $

.include <bsd.xconf.mk>

PROG=		cwm

SRCS=		calmwm.c screen.c xmalloc.c client.c menu.c \
		search.c util.c xutil.c conf.c xevents.c group.c \
		kbfunc.c mousefunc.c parse.y

CPPFLAGS+=	-I${X11BASE}/include -I${X11BASE}/include/freetype2 -I${.CURDIR}

CFLAGS+=	-Wall -g

LDADD+=		-L${X11BASE}/lib -lXft -lXrender -lX11 -lxcb -lXau -lXdmcp \
		-lfontconfig -lexpat -lfreetype -lz -lXrandr -lXext

MANDIR=		${X11BASE}/man/man
MAN=		cwm.1 cwmrc.5

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
