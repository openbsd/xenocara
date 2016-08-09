#	$OpenBSD: Makefile,v 1.8 2016/08/09 18:57:45 matthieu Exp $

.include <bsd.xconf.mk>

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

LIB=	fontconfig

SRCS = \
	fcatomic.c \
	fcblanks.c \
	fccache.c \
	fccfg.c \
	fccharset.c \
	fccompat.c \
	fcdbg.c \
	fcdefault.c \
	fcdir.c \
	fcformat.c \
	fcfreetype.c \
	fcfs.c \
	fcinit.c \
	fclang.c \
	fclist.c \
	fcmatch.c \
	fcmatrix.c \
	fcname.c \
	fcobjs.c \
	fcpat.c \
	fcrange.c \
	fcserialize.c \
	fcstat.c \
	fcstr.c \
	fcweight.c \
	fcxml.c \
	ftglue.c

CFLAGS +=	-DFC_CACHEDIR='"$(FC_CACHEDIR)"' \
		-DFONTCONFIG_PATH='"$(BASECONFIGDIR)"' \
		-I.

LDADD =	-L${X11BASE}/lib -lfreetype -lexpat -lz -lpthread-stubs

PUBLIC_FILES = \
	$(FONTCONFIG)/fontconfig/fontconfig.h \
	$(FONTCONFIG)/src/fcdeprecate.h \
	$(FONTCONFIG)/fontconfig/fcprivate.h

PUBLIC_FT_FILES = \
	$(FONTCONFIG)/fontconfig/fcfreetype.h

fcaliastail.h: fcalias.h

fcalias.h: $(FONTCONFIG)/src/makealias $(PUBLIC_FILES)
	sh $(FONTCONFIG)/src/makealias "$(FONTCONFIG)/src" \
		fcalias.h fcaliastail.h $(PUBLIC_FILES)

fcftaliastail.h: fcftalias.h

fcftalias.h: $(FONTCONFIG)/src/makealias $(PUBLIC_FT_FILES)
	sh $(FONTCONFIG)/src/makealias "$(FONTCONFIG)/src" \
		fcftalias.h fcftaliastail.h $(PUBLIC_FT_FILES)

.if defined(XENOCARA_RERUN_GPERF) && ${XENOCARA_RERUN_GPERF:L=yes}
fcobjshash.h: fcobjshash.gperf
	gperf -m 100 $< > $@.tmp && \
	mv -f $@.tmp $@ || ( $(RM) $@.tmp && false )
.endif

beforedepend: fcalias.h fcftalias.h fcobjshash.h lib${LIB}.la

lib${LIB}.la: ${.CURDIR}/lib${LIB}.la.in
	sed -e 's#@libdir@#${LIBDIR}#g' \
	     -e 's#@sharedlibfontconfig@#libfontconfig.so.${SHLIB_MAJOR}.${SHLIB_MINOR}#g' \
	< $? > $@

afterinstall:
	${INSTALL_DATA} lib${LIB}.la ${DESTDIR}${LIBDIR}

obj:_xenocara_obj

CLEANFILES = fcalias.h fcaliastail.h fcftalias.h fcftaliastail.h lib${LIB}.la

.PATH: ${FONTCONFIG}/src

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
