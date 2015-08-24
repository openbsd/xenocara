# $OpenBSD: Makefile,v 1.4 2015/08/24 10:41:21 ajacoutot Exp $

DB = /usr/X11R6/lib/locate/xorg.db
XETC = ${DESTDIR}/var/sysmerge/xetc.tgz

all: tarxetc makedb

makedb:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makelocatedb ${OSrev} >${DESTDIR}${DB}

tarxetc:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makexetc

.PHONY: makedb tarxetc
