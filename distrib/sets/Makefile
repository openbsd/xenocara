# $OpenBSD: Makefile,v 1.5 2016/10/02 09:16:22 tb Exp $

DB = /usr/X11R6/lib/locate/xorg.db
XETC = ${DESTDIR}/var/sysmerge/xetc.tgz

all: tarxetc makedb

makedb:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makelocatedb ${OSrev} >${DESTDIR}${DB}
	chown ${BINOWN}:${BINGRP} ${DESTDIR}${DB}

tarxetc:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makexetc
	chown root:wheel ${XETC}

.PHONY: makedb tarxetc

.include <bsd.own.mk>
