# $OpenBSD: Makefile,v 1.2 2014/08/28 17:08:33 matthieu Exp $

DB = /usr/X11R6/lib/locate/xorg.db
XETC = ${DESTDIR}/usr/share/sysmerge/xetc.tgz

all: ${XETC} makedb

makedb:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makelocatedb ${OSrev} >${DESTDIR}${DB}

${XETC}:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makexetc

.PHONY: makedb
