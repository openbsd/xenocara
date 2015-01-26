#	$OpenBSD: Makefile,v 1.6 2015/01/26 21:32:11 matthieu Exp $

DATADIR=	${X11BASE}/share
PYTHON=		python${PYTHON_VERSION}
PYTHONDIR=	${X11BASE}/lib/python${PYTHON_VERSION}/site-packages

MANOPTIONS=	-c 'libxcb 1.11' -l 'X Version 11' -s 3

SRCS= \
	bigreq.c \
	composite.c \
	damage.c \
	dpms.c \
	dri2.c \
	dri3.c \
	glx.c \
	present.c \
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
	xkb.c \
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

clean:

.SUFFIXES: .xml .c .h

.xml.c:
	${PYTHON} ${LIBXCB}/src/c_client.py ${MANOPTIONS} -p ${PYTHONDIR} $<

.xml.h:
	${PYTHON} ${LIBXCB}/src/c_client.py ${MANOPTIONS} -p ${PYTHONDIR} $<


.include <bsd.xorg.mk>

${SRCS} ${HDRS}: ${LIBXCB}/src/c_client.py
