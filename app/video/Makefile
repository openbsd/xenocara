PROG=	video
SRCS=	video.c
MAN=	video.1

CFLAGS+= -I${X11BASE}/include
LDFLAGS+= -L${X11BASE}/lib
LDADD+= -lX11 -lXv
BINDIR=	${X11BASE}/bin
MANDIR=	${X11BASE}/man/cat

.include <bsd.xconf.mk>
.include <bsd.prog.mk>

