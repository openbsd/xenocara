# $OpenBSD: Makefile.inc,v 1.2 2016/10/02 20:55:09 matthieu Exp $

realinstall:
	exec ${MAKE} install FCCACHE=: MKFONTDIR=: MKFONTSCALE=:

