# $OpenBSD: Makefile,v 1.18 2011/03/22 10:47:59 okan Exp $

.include <bsd.xconf.mk>

PROG=		cwm

SRCS=		calmwm.c screen.c xmalloc.c client.c menu.c \
		search.c util.c xutil.c conf.c xevents.c group.c \
		kbfunc.c mousefunc.c font.c parse.y

CPPFLAGS+=	-I${X11BASE}/include -I${X11BASE}/include/freetype2 -I${.CURDIR}

CFLAGS+=	-Wall

LDADD+=		-L${X11BASE}/lib -lXft -lXrender -lX11 -lxcb -lXau -lXdmcp \
		-lfontconfig -lexpat -lfreetype -lz -lXinerama -lXrandr -lXext

MANDIR=		${X11BASE}/man/cat
MAN=		cwm.1 cwmrc.5

CLEANFILES=	cwm.cat1 cwmrc.cat5

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
