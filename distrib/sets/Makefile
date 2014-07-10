# $OpenBSD: Makefile,v 1.1 2014/07/10 07:13:13 espie Exp $

DB = /usr/X11R6/lib/locate/xorg.db

makedb:
	MACHINE=${MACHINE} /bin/sh ${.CURDIR}/makelocatedb ${OSrev} >${DESTDIR}${DB}

.PHONY: makedb
