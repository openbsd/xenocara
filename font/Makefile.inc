# $OpenBSD: Makefile.inc,v 1.1 2007/06/01 01:05:25 matthieu Exp $

realinstall:
	exec ${MAKE} install FCCACHE=:

