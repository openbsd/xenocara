# $OpenBSD: bsd.xorg.mk,v 1.53 2016/03/28 11:59:06 matthieu Exp $ -*- makefile  -*-
#
# Copyright © 2006,2012 Matthieu Herrb
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

AUTOMAKE_VERSION=	1.12
AUTOCONF_VERSION=	2.69
PYTHON_VERSION=		2.7

# Where source lives
XSRCDIR?=		/usr/xenocara

# Where to build
XOBJDIR?=	/usr/xobj

# Where to install
BINDIR?=		${X11BASE}/bin
LIBDIR=			${X11BASE}/lib
INCSDIR=		${X11BASE}/include
MANDIR=			${X11BASE}/man/man

PKG_CONFIG_LIBDIR=	/usr/lib/pkgconfig:${X11BASE}/lib/pkgconfig

# A few aliases for *-install targets
INSTALL_DATA = \
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m ${SHAREMODE}

# Autoconf cache
_cache= --cache-file=${XOBJDIR}/xorg-config.cache.${MACHINE}

CFLAGS+=	$(COPTS)

.if !defined(CONFIG_SITE)
CONFIG_SITE=	${XSRCDIR}/etc/config.site
.endif

_SRCDIR?=	${.CURDIR}

XENOCARA_PATH?=	/bin:/sbin:/usr/bin:/usr/sbin:/usr/X11R6/bin

CONFIGURE_ENV=	PKG_CONFIG_LIBDIR="$(PKG_CONFIG_LIBDIR)" \
		CONFIG_SITE=$(CONFIG_SITE) \
		CFLAGS="$(CFLAGS:C/ *$//)" \
		MAKE="${MAKE}"

CONFIGURE_ARGS+= --disable-silent-rules

AUTOTOOLS_ENV=  AUTOMAKE_VERSION="$(AUTOMAKE_VERSION)" \
		AUTOCONF_VERSION="$(AUTOCONF_VERSION)" \
		ACLOCAL="aclocal -I ${X11BASE}/share/aclocal" \
		$(CONFIGURE_ENV)

##
## Default rules
##

# pkgconfig
.if defined(PKGCONFIG)

PACKAGE_VERSION ?= `m4 ${DESTDIR}${X11BASE}/share/mk/package_version.m4 ${_SRCDIR}/configure.ac`

all: ${PKGCONFIG}

${PKGCONFIG}: ${PKGCONFIG}.in
	@sed -e 's#@prefix@#${X11BASE}#g' \
	    -e 's#@datarootdir@#$${prefix}/share#g' \
	    -e 's#@datadir@#$${datarootdir}#g' \
	    -e 's#@exec_prefix@#$${prefix}#g' \
	    -e 's#@libdir@#$${exec_prefix}/lib#g' \
	    -e 's#@includedir@#$${prefix}/include#g' \
	    -e 's#@PACKAGE_VERSION@#'${PACKAGE_VERSION}'#g' \
	    ${EXTRA_PKGCONFIG_SUBST} \
	< $? > $@

install-pc: ${PKGCONFIG}
	${INSTALL_DATA} ${PKGCONFIG} ${DESTDIR}${LIBDIR}/pkgconfig

clean-pc:
	rm -rf ${PKGCONFIG}

realinstall: install-pc

clean:	clean-pc


.endif

# headers
.if defined(HEADERS)
install-headers:
	@echo installing ${HEADERS} in ${INCSDIR}/${HEADERS_SUBDIR}
	@cd ${_SRCDIR}; for i in ${HEADERS}; do \
	    cmp -s $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i;\
	done

realinstall: install-headers
.endif
.if defined(HEADERS_SUBDIRS)
.for d in ${HEADERS_SUBDIRS}
install-headers-subdirs::
	@echo installing ${HEADERS_${d:S/\//_/}} in ${INCSDIR}/${d}
	@cd ${_SRCDIR}; for i in ${HEADERS_${d:S/\//_/}}; do \
	    cmp -s $$i ${DESTDIR}${INCSDIR}/$d/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/${d}; \
	done

realinstall: install-headers-subdirs
.endfor
.endif

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
	@exec ${MAKE} ${_lt_libs}
.endif


REORDER_DEPENDENCIES += ${X11BASE}/share/mk/automake.dep
ECHO_REORDER ?= :

.if !target(config.status)
config.status:
.if defined(XENOCARA_RERUN_AUTOCONF) && ${XENOCARA_RERUN_AUTOCONF:L} == "yes"
	cd ${_SRCDIR}; ${AUTOTOOLS_ENV} exec autoreconf -v --install --force
.else
.if !defined(NO_REORDER)
	@sed -e '/^#/d' ${REORDER_DEPENDENCIES} | \
	  tsort -r|while read f; do \
	    cd ${_SRCDIR}; \
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
.endif
	${CONFIGURE_ENV} PATH=$(XENOCARA_PATH) \
		exec sh ${_SRCDIR}/configure --prefix=${X11BASE} \
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
	exec ${MAKE} ${MAKE_FLAGS} ${_lt_libs} install
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
.if defined(DESTDIR)
build:
	@echo "cannot run make build with DESTDIR set"
	@exit 2
.else
build:
	cd ${.CURDIR} && exec ${MAKE} ${MAKE_FLAGS} ${_wrapper} cleandir
	cd ${.CURDIR} && exec ${MAKE} ${MAKE_FLAGS} ${_wrapper} depend
	cd ${.CURDIR} && exec ${MAKE} ${MAKE_FLAGS} ${_wrapper} all
	cd ${.CURDIR} && exec ${SUDO} ${MAKE} ${MAKE_FLAGS} ${_wrapper} install
.endif
.endif

.if !target(clean) && ${MAKEFILE:T} != "Makefile"
clean:
	-@if [ -f Makefile ]; then exec ${MAKE} clean; fi
.endif

.if !target(cleandir)
cleandir: clean
	-@if [ -f Makefile ]; then exec ${MAKE} distclean; fi
	-@if [ -f config.status ]; then \
		echo "rm config.status"; \
		rm -f config.status; \
	fi
.endif

#
# Obj dir rules for xenocara
#
.if !target(_xenocara_obj)
. if defined(NOOBJ)
_xenocara_obj:
. else

. if defined(MAKEOBJDIR)
__objdir=	${MAKEOBJDIR}
. else
__objdir=	obj
. endif

_SUBDIRUSE:

_xenocara_obj! _SUBDIRUSE
	@cd $(.CURDIR); \
	here=`/bin/pwd`; xsrcdir=`cd $(XSRCDIR); /bin/pwd`; \
	subdir=$${here#$${xsrcdir}/}; \
	if test $$here != $$subdir ; then \
	    dest=${XOBJDIR}/$$subdir; \
	    echo "$$here/${__objdir} -> $$dest"; \
	    if test ! -L ${__objdir} -o \
		X`readlink ${__objdir}` != X$$dest; then \
		    if  test -e ${__objdir}; then rm -rf ${__objdir}; fi; \
		    ln -sf $$dest ${__objdir}; \
	    fi; \
	    if test -d ${XOBJDIR}; then \
		    test -d $$dest || mkdir -p $$dest; \
	    else \
		    if test -e ${XOBJDIR}; then \
			    echo "${XOBJDIR} is not a directory"; \
		    else \
			    echo "${XOBJDIR} does not exist"; \
		    fi; \
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

.PHONY: _xenocara_obj

.include <bsd.subdir.mk>
