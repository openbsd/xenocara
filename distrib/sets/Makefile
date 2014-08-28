# $OpenBSD: Makefile,v 1.3 2014/08/28 17:11:19 matthieu Exp $

DB = /usr/X11R6/lib/locate/xorg.db
XETC = ${DESTDIR}/usr/share/sysmerge/xetc.tgz

all: tarxetc makedb

makedb:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makelocatedb ${OSrev} >${DESTDIR}${DB}

tarxetc:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makexetc

.PHONY: makedb tarxetc
