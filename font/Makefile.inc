# $OpenBSD: Makefile.inc,v 1.3 2021/09/03 07:28:26 matthieu Exp $

CONFIGURE_ARGS += --disable-all-encodings --enable-iso8859-1

realinstall:
	exec ${MAKE} install FCCACHE=: MKFONTDIR=: MKFONTSCALE=:

