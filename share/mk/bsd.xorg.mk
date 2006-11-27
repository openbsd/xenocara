# $OpenBSD: bsd.xorg.mk,v 1.5 2006/11/27 12:03:10 matthieu Exp $ -*- makefile  -*-
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

# Where to buid - not used?
# XORG_BUILDDIR=	${XENOCARA_TOP}

# Where to install
X11BASE?=		/usr/X11R6

BINDIR=			${X11BASE}/bin
LIBDIR=			${X11BASE}/lib
INCSDIR=		${X11BASE}/include
MANDIR=			${X11BASE}/man/cat

PKG_CONFIG_PATH=	${X11BASE}/lib/pkgconfig

_cache= --cache-file=${XENOCARA_TOP}/xorg-config.cache.${MACHINE}

MAKE_ENV+=	AUTOMAKE_VERSION="$(AUTOMAKE_VERSION)" \
		AUTOCONF_VERSION="$(AUTOCONF_VERSION)" \
		ACLOCAL="aclocal -I ${X11BASE}/share/aclocal" \
		PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"

.if !target(.MAIN)
.MAIN: all
.endif

.if !target(includes)
includes:
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
config.status:
	cd ${.CURDIR}; ${MAKE_ENV} autoreconf -v --install --force
	PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" \
		${CONFIGURE_ENV} ${.CURDIR}/configure \
		--enable-maintainer-mode --prefix=${X11BASE} \
		--sysconfdir=/etc \
		--mandir=${X11BASE}/man \
		${_cache} \
		${CONFIGURE_ARGS}
.else
config.status:
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
install::
	${MAKE_ENV} ${MAKE} ${_lt_libs} install
.endif

.if target(extra-install)
install::
	cd ${.CURDIR} && ${MAKE_ENV} ${MAKE} -f Makefile.bsd-wrapper extra-install
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

.include <bsd.obj.mk>
