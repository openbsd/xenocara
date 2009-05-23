#	$OpenBSD: Makefile,v 1.1 2009/05/23 15:04:34 matthieu Exp $

LIB=		pthread-stubs

SRCS=		stubs.c

PKGCONFIG= 	pthread-stubs.pc

EXTRA_PKGCONFIG_SUBST=	'-e s,@PKG_CONFIG_LIBS@,-L$${libdir} -lpthread-stubs,g'

obj:	_xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
