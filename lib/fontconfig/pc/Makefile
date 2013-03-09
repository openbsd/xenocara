#	$OpenBSD: Makefile,v 1.1 2013/03/09 14:34:46 matthieu Exp $

FONTCONFIG =	${.CURDIR}/../../../dist/fontconfig

PKGCONFIG = fontconfig.pc

PACKAGE_VERSION = 2.10.91
PACKAGE = fontconfig
VERSION = $(PACKAGE_VERSION)

EXTRA_PKGCONFIG_SUBST = \
	-e 's,@PACKAGE@,'${PACKAGE}',g' \
	-e 's,@VERSION@,'${VERSION}',g' \
	-e 's,@LIBXML2_LIBS@,,g' \
	-e 's,@EXPAT_LIBS@,-lexpat,g' \
	-e 's,@FREETYPE_LIBS@,-lfreetype,g' \
	-e 's,@ICONV_LIBS@,,g' \
	-e 's,@sysconfdir@,/etc,g' \
	-e 's,@localstatedir@,/var,g' \
	-e 's,@baseconfigdir@,'${BASECONFIGDIR}',g' \
	-e 's,@fc_cachedir@,'${FC_CACHEDIR}',g'

obj: _xenocara_obj

.PATH: ${FONTCONFIG}

.include <bsd.xorg.mk>
