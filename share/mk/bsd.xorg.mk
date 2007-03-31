# $OpenBSD: bsd.xorg.mk,v 1.19 2007/03/31 20:25:53 matthieu Exp $ -*- makefile  -*-
#
# Copyright © 2006 Matthieu Herrb
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

.if exists(${.CURDIR}/../Makefile.inc)
.include "${.CURDIR}/../Makefile.inc"
.endif

AUTOMAKE_VERSION=	1.9
AUTOCONF_VERSION=	2.59

# Where source lives
XSRCDIR?=		/usr/src/xenocara

# Where to build
XOBJDIR?=	/usr/xobj

# Where to install
BINDIR?=		${X11BASE}/bin
LIBDIR=			${X11BASE}/lib
INCSDIR=		${X11BASE}/include
MANDIR=			${X11BASE}/man/cat

PKG_CONFIG_LIBDIR=	${X11BASE}/lib/pkgconfig

# A few aliases for *-install targets
INSTALL_DATA = ${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 644

# Autoconf cache
_cache= --cache-file=${XOBJDIR}/xorg-config.cache.${MACHINE}

CFLAGS+=	$(COPTS)

CONFIGURE_ENV=	PKG_CONFIG_LIBDIR="$(PKG_CONFIG_LIBDIR)" \
		CFLAGS="$(CFLAGS:C/ *$//)"

AUTOTOOLS_ENV=  AUTOMAKE_VERSION="$(AUTOMAKE_VERSION)" \
		AUTOCONF_VERSION="$(AUTOCONF_VERSION)" \
		ACLOCAL="aclocal -I ${X11BASE}/share/aclocal" \
		$(CONFIGURE_ENV)

.if !target(.MAIN)
.MAIN: all
.endif

.if !target(includes)
includes: _SUBDIRUSE
.endif

.if defined(SHARED_LIBS)
_lt_libs=
.for _n _v in ${SHARED_LIBS}
_lt_libs+=lib${_n:S/+/_/g:S/-/_/g:S/./_/g}_ltversion=${_v}
.endfor
.endif

.if !target(all)
all:	config.status
	exec ${MAKE} ${_lt_libs}
.endif


REORDER_DEPENDENCIES += ${X11BASE}/share/mk/automake.dep
ECHO_REORDER ?= :

.if !target(config.status)
config.status:
.if defined(XENOCARA_RERUN_AUTOCONF) && ${XENOCARA_RERUN_AUTOCONF:L} == "yes"
	cd ${.CURDIR}; ${AUTOTOOLS_ENV} exec autoreconf -v --install --force
.else
	@sed -e '/^#/d' ${REORDER_DEPENDENCIES} | \
	  tsort -r|while read f; do \
	    cd ${.CURDIR}; \
		case $$f in \
		/*) \
			find . -name $${f#/} -print| while read i; \
				do ${ECHO_REORDER} "Touching $$i"; touch $$i; done \
			;; \
		*) \
			if test -e $$f ; then \
				${ECHO_REORDER} "Touching $$f"; touch $$f; \
			fi \
			;; \
		esac; done
.endif
	${CONFIGURE_ENV} exec sh ${.CURDIR}/configure --prefix=${X11BASE} \
		--sysconfdir=/etc \
		--mandir=${X11BASE}/man \
		${_cache} \
		${CONFIGURE_ARGS}
.endif

.if !target(depend)
depend: 
	@echo "no dependencies here yet"
.endif

.if !target(install)
.  if !target(beforeinstall)
beforeinstall:
.  endif
.  if !target(afterinstall)
afterinstall:
.  endif
.  if !target(realinstall)
realinstall:
	exec ${MAKE} ${_lt_libs} install
.endif
install: maninstall
maninstall: afterinstall
afterinstall: realinstall
realinstall: beforeinstall
.endif

.if !target(build)
.if exists(Makefile.bsd-wrapper)
_wrapper = -f Makefile.bsd-wrapper
.endif
build:
	cd ${.CURDIR} && exec ${MAKE} ${_wrapper} cleandir
	cd ${.CURDIR} && exec ${MAKE} ${_wrapper} depend
	cd ${.CURDIR} && exec ${MAKE} ${_wrapper} all
	cd ${.CURDIR} && exec ${SUDO} ${MAKE} ${_wrapper} install
.endif

.if !target(clean)
clean:
	-@if [ -e Makefile ]; then exec ${MAKE} clean; fi
.endif

.if !target(cleandir)
cleandir: clean
	-@if [ -e Makefile ]; then exec ${MAKE} distclean; fi
.endif

#
# Obj dir rules for xenocara
#
.if !target(_xenocara_obj)
. if defined(NOOBJ)
_xenocara_obj:
. else

. if defined(OBJMACHINE)
__objdir=	obj.$(MACHINE)
. else
__objdir=	obj
. endif

. if defined(XOBJMACHINE)
__xobjdir=	$(XOBJDIR).$(MACHINE)
__xobjdirpf=
. else
__xobjdir=	$(XOBJDIR)
.  if defined(OBJMACHINE)
__xobjdirpf=	.$(MACHINE)
.  else
__xobjdirpf=
.  endif
. endif

_SUBDIRUSE:

_xenocara_obj! _SUBDIRUSE
	@cd $(.CURDIR); \
	here=`/bin/pwd`; xsrcdir=`cd $(XSRCDIR); /bin/pwd`; \
	subdir=$${here#$${xsrcdir}/}; \
	if test $$here != $$subdir ; then \
	    dest=${__xobjdir}/$$subdir${__xobjdirpf}; \
	    echo "$$here/${__objdir} -> $$dest"; \
	    if test ! -L ${__objdir} -o \
		X`readlink ${__objdir}` != X$$dest; then \
		    if  test -e ${__objdir}; then rm -rf ${__objdir}; fi; \
		    ln -sf $$dest ${__objdir}; \
	    fi; \
	    if test -d ${__xobjdir} -a ! -d $$dest; then \
		mkdir -p $$dest; \
	    else \
		true; \
	    fi; \
	else \
	    dest=$$here/${__objdir}; \
	    if test ! -d ${__objdir}; then \
		echo "making $$dest"; \
		mkdir $$dest; \
	    fi ; \
	fi
. endif
.endif

.if !target(obj)
obj:	_xenocara_obj
.endif

.include <bsd.subdir.mk>
