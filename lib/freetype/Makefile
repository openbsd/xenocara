#	$Xenocara: Makefile,v 1.8 2006/04/17 21:35:40 matthieu Exp $

FREETYPESRC=	${.CURDIR}/src

.ifmake includes
SUBDIR=	include
.endif

LIB=	freetype

SRCS=   ftapi.c ftbase.c ftbbox.c ftbdf.c ftbitmap.c ftdebug.c ftglyph.c \
        ftinit.c ftlzw.c ftmm.c ftpfr.c ftstroke.c ftsynth.c ftsystem.c \
        fttype1.c ftwinfnt.c ftxf86.c autofit.c bdf.c cff.c \
        type1cid.c ftgzip.c pcf.c pfr.c psaux.c pshinter.c psnames.c \
        raster.c sfnt.c smooth.c truetype.c type1.c type42.c winfnt.c

CPPFLAGS+= -I${.CURDIR}/include -I${.CURDIR}/src/lzw -DFT2_BUILD_LIBRARY

CLEANFILES+=	freetype-config freetype2.pc

build:
	cd ${.CURDIR} && ${MAKE} clean
	cd ${.CURDIR} && ${SUDO} ${MAKE} includes
	cd ${.CURDIR} && ${MAKE} all
	cd ${.CURDIR} && ${SUDO} ${MAKE} install

includes:: _SUBDIRUSE
	@cmp -s ${DESTDIR}${X11BASE}/include/ft2build.h \
	  ${.CURDIR}/builds/unix/ft2unix.h || \
	  ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) \
	  ${.CURDIR}/builds/unix/ft2unix.h \
	  ${DESTDIR}${X11BASE}/include/ft2build.h

includes:: freetype2.pc
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) \
	freetype2.pc ${DESTDIR}${X11BASE}/lib/pkgconfig

includes:: freetype-config
	${INSTALL} ${INSTALL_COPY} -m 755 -o $(BINOWN) -g $(BINGRP) \
	freetype-config ${DESTDIR}${X11BASE}/bin

freetype2.pc: freetype2.in
	sed -e s%@prefix@%${X11BASE}% -e s%@exec_prefix@%${XORG_PREFIX}% \
		-e s%@includedir@%${INCSDIR}% -e s%@libdir@%${LIBDIR}% \
		-e s%@LIBZ@%-lz% < ${.CURDIR}/builds/unix/freetype2.in \
		> freetype2.pc

freetype-config: freetype-config.in
	sed -e s%@prefix@%${X11BASE}% -e s%@exec_prefix@%${XORG_PREFIX}% \
		-e s%@includedir@%${INCSDIR}% -e s%@libdir@%${LIBDIR}% \
		-e s%@LIBZ@%-lz% -e s%@enable_shared@%yes% \
		-e 's%@hardcode_libdir_flag_spec@%%' \
		-e 's%@ft_version@%9.7.3%' -e 's%@wl@%%' \
		< ${.CURDIR}/builds/unix/freetype-config.in \
		> freetype-config

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${FREETYPESRC}/autofit
.PATH:  ${FREETYPESRC}/base
.PATH:  ${FREETYPESRC}/bdf
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
.PATH:		${.CURDIR}
.PATH:		${.CURDIR}/builds/unix


.include <bsd.subdir.mk>

