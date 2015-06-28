#	$OpenBSD: Makefile,v 1.39 2015/06/28 08:27:44 dcoppa Exp $

FREETYPESRC=	${.CURDIR}/src

# Get it from builds/unix/configure.ac
FT_VERSION_INFO= 18.0.12
INSTALL_PROGRAM = ${INSTALL} ${INSTALL_COPY} -m 755 -o $(BINOWN) -g $(BINGRP)

.ifmake includes
SUBDIR=	include
.endif

DEBUG?=

LIB=	freetype

SRCS=   ftbase.c ftbbox.c ftbdf.c ftbitmap.c ftdebug.c ftcache.c \
	ftcid.c ftfstype.c ftgasp.c ftglyph.c \
	ftgxval.c ftlcdfil.c ftotval.c ftpatent.c \
        ftinit.c ftlzw.c ftmm.c ftpfr.c ftstroke.c ftsynth.c ftsystem.c \
        fttype1.c ftwinfnt.c ftfntfmt.c autofit.c bdf.c cff.c \
        ftgzip.c pcf.c pfr.c psaux.c pshinter.c psnames.c \
        raster.c sfnt.c smooth.c truetype.c type1.c type1cid.c type42.c \
	winfnt.c

CPPFLAGS+= -I${.CURDIR}/include -I${.CURDIR}/builds/unix -I${.CURDIR}/src/lzw -DFT2_BUILD_LIBRARY

CLEANFILES+=	freetype-config freetype2.pc

obj:	_xenocara_obj

build:
	cd ${.CURDIR} && ${MAKE} clean
	cd ${.CURDIR} && ${SUDO} ${MAKE} includes
	cd ${.CURDIR} && ${MAKE} depend
	cd ${.CURDIR} && ${MAKE} all
	cd ${.CURDIR} && ${SUDO} ${MAKE} install

includes:: _SUBDIRUSE
	@cmp -s ${DESTDIR}${X11BASE}/include/freetype2/ft2build.h \
	  ${.CURDIR}/include/ft2build.h || \
	${INSTALL_DATA} ${.CURDIR}/include/ft2build.h \
	  ${DESTDIR}${X11BASE}/include/freetype2/ft2build.h

depend:	freetype-config freetype2.pc

includes::
	@cmp -s ${DESTDIR}${X11BASE}/include/freetype2/freetype/config/ftconfig.h \
	  ${.CURDIR}/builds/unix/ftconfig.h || \
	${INSTALL_DATA} ${.CURDIR}/builds/unix/ftconfig.h \
	  ${DESTDIR}${X11BASE}/include/freetype2/config/ftconfig.h

install-pc: freetype2.pc
	${INSTALL_DATA} freetype2.pc ${DESTDIR}${LIBDIR}/pkgconfig

install-config: freetype-config
	${INSTALL_PROGRAM} freetype-config ${DESTDIR}${X11BASE}/bin

freetype2.pc: freetype2.in Makefile
	sed -e s,%prefix%,${X11BASE}, -e s,%exec_prefix%,${X11BASE}, \
		-e s,%ft_version%,${FT_VERSION_INFO}, \
		-e s,%includedir%,${INCSDIR}, -e s,%libdir%,${LIBDIR}, \
		-e s,%LIBS_PRIVATE%,-lz, -e s,%REQUIRES_PRIVATE%,, \
		< ${.CURDIR}/builds/unix/freetype2.in > freetype2.pc

freetype-config: freetype-config.in
	sed -e s,%prefix%,${X11BASE}, -e s,%exec_prefix%,${X11BASE}, \
		-e s,%includedir%,${INCSDIR}, -e s,%libdir%,${LIBDIR}, \
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
.PATH:  ${FREETYPESRC}/sfnt
.PATH:  ${FREETYPESRC}/smooth
.PATH:  ${FREETYPESRC}/truetype
.PATH:  ${FREETYPESRC}/type1
.PATH:  ${FREETYPESRC}/type42
.PATH:  ${FREETYPESRC}/winfonts

.include <bsd.subdir.mk>

