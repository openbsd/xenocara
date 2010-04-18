#	$OpenBSD: Makefile,v 1.2 2010/04/18 19:58:53 matthieu Exp $

DATADIR=	${X11BASE}/share
PYTHON=		python${PYTHON_VERSION}
PYTHONDIR=	${X11BASE}/lib/python${PYTHON_VERSION}/site-packages

SRCS= \
	bigreq.c \
	composite.c \
	damage.c \
	dpms.c \
	dri2.c \
	glx.c \
	randr.c \
	record.c \
	render.c \
	res.c \
	screensaver.c \
	shape.c \
	shm.c \
	sync.c \
	xc_misc.c \
	xevie.c \
	xf86dri.c \
	xfixes.c \
	xinerama.c \
	xinput.c \
	xprint.c \
	xproto.c \
	xselinux.c \
	xtest.c \
	xv.c \
	xvmc.c

HDRS= ${SRCS:.c=.h}


all: ${SRCS} ${HDRS}

test:
	@echo ${SRCS}

.SUFFIXES: .xml .c .h

.xml.c:
	${PYTHON} ${LIBXCB}/src/c_client.py -p ${PYTHONDIR} $<

.xml.h:
	${PYTHON} ${LIBXCB}/src/c_client.py -p ${PYTHONDIR} $<

.include <bsd.xorg.mk>
