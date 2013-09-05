#	$OpenBSD: Makefile,v 1.2 2013/09/05 20:23:05 todd Exp $

LIB=		txc_dxtn

CPPFLAGS+=	-I${X11BASE}/include

HEADERS=	txc_dxtn.h
SRCS=		txc_compress_dxtn.c txc_fetch_dxtn.c

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
