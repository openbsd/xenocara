# $OpenBSD: bsd.xorg.mk,v 1.9 2006/12/17 20:41:36 matthieu Exp $ -*- makefile  -*-
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

.include <bsd.own.mk>

.if exists(${.CURDIR}/../Makefile.inc)
.include "${.CURDIR}/../Makefile.inc"
.endif

AUTOMAKE_VERSION=	1.9
AUTOCONF_VERSION=	2.59

# Where source lives
XENOCARA_TOP?=		/usr/xenocara

# Where to build
XENOCARA_OBJDIR?=	/usr/obj/xenocara

# Where to install
X11BASE?=		/usr/X11R6
X11ETC?=		/etc/X11

BINDIR?=		${X11BASE}/bin
LIBDIR=			${X11BASE}/lib
INCSDIR=		${X11BASE}/include
MANDIR=			${X11BASE}/man/cat

PKG_CONFIG_PATH=	${X11BASE}/lib/pkgconfig

_cache= --cache-file=${XENOCARA_OBJDIR}/xorg-config.cache.${MACHINE}

MAKE_ENV+=	AUTOMAKE_VERSION="$(AUTOMAKE_VERSION)" \
		AUTOCONF_VERSION="$(AUTOCONF_VERSION)" \
		ACLOCAL="aclocal -I ${X11BASE}/share/aclocal" \
		PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"

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
	${MAKE_ENV} ${MAKE} ${_lt_libs}
.endif

.if !target(config.status)
.if defined(XENOCARA_RERUN_AUTOCONF) && ${XENOCARA_RERUN_AUTOCONF:L} == "yes"
config.status: _xenocara_obj
	cd ${.CURDIR}; ${MAKE_ENV} autoreconf -v --install --force
	PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" \
		${CONFIGURE_ENV} ${.CURDIR}/configure \
		--enable-maintainer-mode --prefix=${X11BASE} \
		--sysconfdir=/etc \
		--mandir=${X11BASE}/man \
		${_cache} \
		${CONFIGURE_ARGS}
.else
config.status: _xenocara_obj
	PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" \
		${CONFIGURE_ENV} ${.CURDIR}/configure --prefix=${X11BASE} \
		--sysconfdir=/etc \
		--mandir=${X11BASE}/man \
		${_cache} \
		${CONFIGURE_ARGS}
.endif
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
	${MAKE_ENV} ${MAKE} ${_lt_libs} install
.endif
install: maninstall
maninstall: afterinstall
afterinstall: realinstall
realinstall: beforeinstall
.endif

.if !target(dist)
dist:
	${MAKE_ENV} ${MAKE} ${_lt_libs} dist
.endif

.if !target(build)
.if exists(Makefile.bsd-wrapper)
_wrapper = -f Makefile.bsd-wrapper
.endif
build:
	cd ${.CURDIR} && ${MAKE_ENV} ${MAKE} ${_wrapper} cleandir
	cd ${.CURDIR} && ${MAKE_ENV} ${MAKE} ${_wrapper} depend
	cd ${.CURDIR} && ${MAKE_ENV} ${MAKE} ${_wrapper} all
	cd ${.CURDIR} && ${MAKE_ENV} ${SUDO} ${MAKE} ${_wrapper} install
.endif

.if !target(clean)
clean:
	-@if [ -e Makefile ]; then ${MAKE_ENV} ${MAKE} clean; fi
.endif

.if !target(cleandir)
cleandir: clean
	-@if [ -e Makefile ]; then ${MAKE_ENV} ${MAKE} distclean; fi
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

. if defined(XENOCARA_OBJMACHINE)
__xobjdir=	$(XENOCARA_OBJDIR).$(MACHINE)
__xobjdirpf=
. else
__xobjdir=	$(XENOCARA_OBJDIR)
.  if defined(OBJMACHINE)
__xobjdirpf=	.$(MACHINE)
.  else
__xobjdirpf=
.  endif
. endif

_SUBDIRUSE:

_xenocara_obj! _SUBDIRUSE
	@cd $(.CURDIR); \
	here=`/bin/pwd`; xenocara_top=`cd $(XENOCARA_TOP); /bin/pwd`; \
	subdir=$${here#$${xenocara_top}/}; \
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
