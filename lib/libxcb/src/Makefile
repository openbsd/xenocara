#	$OpenBSD: Makefile,v 1.13 2022/07/17 08:31:10 matthieu Exp $

# This Makefile is used to generate the sources files with xcbgen
# The x11/py-xcbgen package needs to be installed

DATADIR=	${X11BASE}/share
PYTHON=		python${PYTHON_VERSION}

MANOPTIONS=	-c 'libxcb 1.15' -l 'X Version 11' -s 3

SRCS= \
	bigreq.c \
	composite.c \
	damage.c \
	dbe.c \
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
	xf86dri.c \
	xfixes.c \
	xinerama.c \
	xinput.c \
	xkb.c \
	xproto.c \
	xselinux.c \
	xtest.c \
	xv.c \
	xvmc.c

HDRS= ${SRCS:.c=.h}


all: ${SRCS} ${HDRS}

clean:

force-clean:
	rm -f ${SRCS} ${HDRS}

.SUFFIXES: .xml .c .h

.xml.c:
	${PYTHON} ${LIBXCB}/src/c_client.py ${MANOPTIONS}  $<

.xml.h:
	${PYTHON} ${LIBXCB}/src/c_client.py ${MANOPTIONS} $<

.include <bsd.xorg.mk>

${SRCS} ${HDRS}: ${LIBXCB}/src/c_client.py
