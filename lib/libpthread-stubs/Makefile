#	$OpenBSD: Makefile,v 1.2 2009/10/03 13:57:56 matthieu Exp $

LIB=		pthread-stubs

SRCS=		stubs.c

PKGCONFIG= 	pthread-stubs.pc

EXTRA_PKGCONFIG_SUBST=	'-e s,@PKG_CONFIG_LIBS@,-L$${libdir} -lpthread-stubs,g'

NOPROFILE=

obj:	_xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
