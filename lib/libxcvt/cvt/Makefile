# $OpenBSD: Makefile,v 1.1 2021/10/27 16:13:42 matthieu Exp $

PROG= cvt

CFLAGS+= -I${.CURDIR}/../include

LDADD= -L../lib -lxcvt -lm

obj: _xenocara_obj

cvt.1: ${.CURDIR}/../man/cvt.man
	sed -e 's,appmansuffix,1,g' \
		-e 's,filemansuffix,5,g' \
		-e 's,vendorversion,"libxcvt 0.1.1" "X Version 11",g' \
		< ${.CURDIR}/../man/cvt.man >$@

CLEANFILES+= cvt.1

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
