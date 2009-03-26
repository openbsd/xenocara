#	$OpenBSD: Makefile,v 1.14 2009/03/26 07:31:44 matthieu Exp $

FREETYPESRC=	${.CURDIR}/src

FT_VERSION_INFO= 9.20.3 # Get it from builds/unix/configure.ac

.ifmake includes
SUBDIR=	include
.endif

DEBUG?=

LIB=	freetype

SRCS=   ftbase.c ftbbox.c ftbdf.c ftbitmap.c ftdebug.c ftcache.c \
	ftcid.c ftfstype.c ftgasp.c ftglyph.c \
	ftgxval.c ftlcdfil.c ftotval.c ftpatent.c \
        ftinit.c ftlzw.c ftmm.c ftpfr.c ftstroke.c ftsynth.c ftsystem.c \
        fttype1.c ftwinfnt.c ftxf86.c autofit.c bdf.c cff.c \
        type1cid.c ftgzip.c pcf.c pfr.c psaux.c pshinter.c psnames.c \
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
	@cmp -s ${DESTDIR}${X11BASE}/include/ft2build.h \
	  ${.CURDIR}/builds/unix/ft2unix.h || \
	  ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) \
	  ${.CURDIR}/builds/unix/ft2unix.h \
	  ${DESTDIR}${X11BASE}/include/ft2build.h

includes::
	@cmp -s ${DESTDIR}${X11BASE}/include/freetype2/freetype/config/ftconfig.h \
	  ${.CURDIR}/builds/unix/ftconfig.h || \
	  ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) \
	  ${.CURDIR}/builds/unix/ftconfig.h \
	  ${DESTDIR}${X11BASE}/include/freetype2/freetype/config/ftconfig.h

includes:: freetype2.pc
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) \
	freetype2.pc ${DESTDIR}${X11BASE}/lib/pkgconfig

includes:: freetype-config
	${INSTALL} ${INSTALL_COPY} -m 755 -o $(BINOWN) -g $(BINGRP) \
	freetype-config ${DESTDIR}${X11BASE}/bin

freetype2.pc: freetype2.in Makefile
	sed -e s%@prefix@%${X11BASE}% -e s%@exec_prefix@%${X11BASE}% \
		-e s%@ft_version@%${FT_VERSION_INFO}% \
		-e s%@includedir@%${INCSDIR}% -e s%@libdir@%${LIBDIR}% \
		-e s%@LIBZ@%-lz% -e s%@FT2_EXTRA_LIBS@%% \
		< ${.CURDIR}/builds/unix/freetype2.in > freetype2.pc

freetype-config: freetype-config.in
	sed -e s%@prefix@%${X11BASE}% -e s%@exec_prefix@%${X11BASE}% \
		-e s%@includedir@%${INCSDIR}% -e s%@libdir@%${LIBDIR}% \
		-e s%@LIBZ@%-lz% -e s%@FT2_EXTRA_LIBS@%% \
		-e s%@enable_shared@%yes% \
		-e 's%@hardcode_libdir_flag_spec@%%' \
		-e 's%@ft_version@%${FT_VERSION_INFO}%' -e 's%@wl@%%' \
		< ${.CURDIR}/builds/unix/freetype-config.in \
		> freetype-config

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

