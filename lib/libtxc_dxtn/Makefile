#	$OpenBSD: Makefile,v 1.3 2013/09/05 22:14:39 todd Exp $

LIB=		txc_dxtn

CPPFLAGS+=	-I${X11BASE}/include

HEADERS=	txc_dxtn.h
SRCS=		txc_compress_dxtn.c txc_fetch_dxtn.c

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
