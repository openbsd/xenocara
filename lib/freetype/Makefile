#	$OpenBSD: Makefile,v 1.58 2023/05/21 09:25:29 matthieu Exp $

.include <bsd.own.mk>

FREETYPESRC=	${.CURDIR}/src

# Get it from builds/unix/configure.ac
FT_VERSION_INFO= 25.0.19

INSTALL_PROGRAM = ${INSTALL} ${INSTALL_COPY} -m 755 -o $(BINOWN) -g $(BINGRP)

.ifmake includes
SUBDIR=	include
.endif

DEBUG?=

LIB=	freetype
LDADD=	-lz

SRCS=   ftbase.c ftbbox.c ftbdf.c ftbitmap.c ftdebug.c ftcache.c \
	ftcid.c ftfstype.c ftgasp.c ftglyph.c ftgxval.c ftotval.c \
	ftpatent.c ftinit.c ftlzw.c ftmm.c ftpfr.c ftstroke.c \
	ftsynth.c ftsystem.c fttype1.c ftwinfnt.c autofit.c bdf.c \
	cff.c ftgzip.c pcf.c pfr.c psaux.c pshinter.c psnames.c \
	raster.c sdf.c sfnt.c smooth.c svg.c truetype.c type1.c type1cid.c \
	type42.c winfnt.c

CPPFLAGS+= -I${.CURDIR}/include -I${.CURDIR}/builds/unix -I${.CURDIR}/src/lzw -DFT2_BUILD_LIBRARY -DFT_CONFIG_CONFIG_H='<ftconfig.h>'

.if ${COMPILER_VERSION} != "gcc3"
CFLAGS+= -fvisibility=hidden -std=gnu99 -Wall  -Wdeclaration-after-statement  -Werror=implicit-function-declaration  -Werror=undef  -Wextra  -Winit-self  -Winline  -Winvalid-pch  -Wmissing-declarations  -Wmissing-format-attribute  -Wmissing-prototypes  -Wnested-externs  -Wno-attributes  -Wno-long-long  -Wno-missing-field-initializers  -Wno-unused-parameter  -Wold-style-definition  -Wpacked  -Wpointer-arith  -Wshadow  -Wsign-compare  -Wstrict-aliasing=2  -Wstrict-prototypes  -Wwrite-strings
PICFLAG= -fPIC
VERSION_SCRIPT= ${.CURDIR}/Symbols.map
.endif

CLEANFILES+=	freetype-config freetype2.pc

obj:	_xenocara_obj

beforedepend:	freetype-config freetype2.pc
BUILDFIRST =	freetype-config freetype2.pc

beforeinstall:
	cd ${.CURDIR} ; exec ${MAKE} ${MAKE_FLAGS} includes

includes: _SUBDIRUSE
	@cmp -s ${DESTDIR}${X11BASE}/include/freetype2/ft2build.h \
	  ${.CURDIR}/include/ft2build.h || \
	${INSTALL_DATA} ${.CURDIR}/include/ft2build.h \
	  ${DESTDIR}${X11BASE}/include/freetype2/ft2build.h
	@cmp -s ${DESTDIR}${X11BASE}/include/freetype2/freetype/config/ftconfig.h \
	  ${.CURDIR}/builds/unix/ftconfig.h || \
	${INSTALL_DATA} ${.CURDIR}/builds/unix/ftconfig.h \
	  ${DESTDIR}${X11BASE}/include/freetype2/freetype/config/ftconfig.h

install-pc: freetype2.pc
	${INSTALL_DATA} freetype2.pc ${DESTDIR}${LIBDIR}/pkgconfig

install-config: freetype-config
	${INSTALL_PROGRAM} freetype-config ${DESTDIR}${X11BASE}/bin

freetype2.pc: freetype2.in Makefile
	sed -e s,%prefix%,${X11BASE}, -e s,%exec_prefix%,${X11BASE}, \
		-e s,%ft_version%,${FT_VERSION_INFO}, \
		-e s,%includedir%,${INCSDIR}, -e s,%libdir%,${LIBDIR}, \
		-e s,%PKGCONFIG_REQUIRES_PRIVATE%,, \
		-e s,%PKGCONFIG_LIBS_PRIVATE%,-lz, \
		-e s,%PKGCONFIG_REQUIRES%,, \
		-e "s,%PKGCONFIG_LIBS%,-L${LIBDIR} -lfreetype," \
		< ${.CURDIR}/builds/unix/freetype2.in > freetype2.pc

freetype-config: freetype-config.in
	sed -e s,%prefix%,${X11BASE}, -e s,%exec_prefix%,${X11BASE}, \
		-e s,%includedir%,${INCSDIR}, -e s,%libdir%,${LIBDIR}, \
		-e 's,%PKG_CONFIG%,/usr/bin/pkg-config,' \
		-e 's,%LIBSSTATIC_CONFIG%,-lfreetype -lz,' \
		-e 's,%hardcode_libdir_flag_spec%,,' \
		-e 's,%ft_version%,${FT_VERSION_INFO},' -e 's,%wl%,,' \
		-e 's,%build_libtool_libs%,yes,' \
		< ${.CURDIR}/builds/unix/freetype-config.in \
		> freetype-config

realinstall:	install-pc install-config

NOPROFILE=

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${.CURDIR}/builds/unix
.PATH:	${FREETYPESRC}/autofit
.PATH:  ${FREETYPESRC}/base
.PATH:  ${FREETYPESRC}/bdf
.PATH:	${FREETYPESRC}/cache
.PATH:  ${FREETYPESRC}/cff
.PATH:  ${FREETYPESRC}/cid
.PATH:  ${FREETYPESRC}/gzip
.PATH:	${FREETYPESRC}/lzw
.PATH:  ${FREETYPESRC}/pcf
.PATH:  ${FREETYPESRC}/pfr
.PATH:  ${FREETYPESRC}/psaux
.PATH:  ${FREETYPESRC}/pshinter
.PATH:  ${FREETYPESRC}/psnames
.PATH:  ${FREETYPESRC}/raster
.PATH:  ${FREETYPESRC}/sdf
.PATH:  ${FREETYPESRC}/sfnt
.PATH:  ${FREETYPESRC}/smooth
.PATH:	${FREETYPESRC}/svg
.PATH:  ${FREETYPESRC}/truetype
.PATH:  ${FREETYPESRC}/type1
.PATH:  ${FREETYPESRC}/type42
.PATH:  ${FREETYPESRC}/winfonts

.include <bsd.subdir.mk>
