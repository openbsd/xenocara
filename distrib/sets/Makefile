# $OpenBSD: Makefile,v 1.6 2016/11/19 08:36:48 tb Exp $

DB = /usr/X11R6/lib/locate/xorg.db
XETC = ${DESTDIR}/var/sysmerge/xetc.tgz

all: tarxetc makedb

makedb:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makelocatedb ${OSrev} >${DESTDIR}${DB}
	chown ${BINOWN}:${BINGRP} ${DESTDIR}${DB}
	chmod 644 ${DESTDIR}${DB}

tarxetc:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makexetc
	chown root:wheel ${XETC}
	chmod 644 ${XETC}

.PHONY: makedb tarxetc

.include <bsd.own.mk>
