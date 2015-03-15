#
#  Copyright 2005  Red Hat, Inc.
#
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Red Hat not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Red Hat makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
#
#  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.

bin_PROGRAMS = xdm

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = $(CWARNFLAGS) $(XDM_CFLAGS) $(SYSTEMD_DAEMON_CFLAGS)

AM_LDFLAGS = $(XDM_LIBS) $(SYSTEMD_DAEMON_LIBS)

# The xdm binary needs to export symbols so that they can be used from
# libXdmGreet.so loaded through a dlopen call from session.c
if DYNAMIC_GREETER
AM_LDFLAGS = $(AM_LDFLAGS) -export-dynamic
else
LDADD = ../greeter/libXdmGreet.la
endif

xdm_SOURCES =		\
        access.c	\
        auth.c		\
        choose.c	\
        daemon.c	\
        dm.c		\
        dpylist.c	\
        error.c		\
        file.c		\
        genauth.c	\
        mitauth.c	\
        netaddr.c	\
        policy.c	\
        protodpy.c	\
        reset.c		\
        resource.c	\
        server.c	\
        session.c	\
        socket.c	\
        streams.c	\
        util.c		\
        xdmcp.c

if HAS_XDM_AUTH
AM_CFLAGS +=  -DHASXDMAUTH
xdm_SOURCES += xdmauth.c
endif

if !HAVE_ARC4RANDOM
xdm_SOURCES += prngc.c
endif

if HAS_SECURE_RPC
AM_CFLAGS += -DSECURE_RPC
xdm_SOURCES += rpcauth.c
endif

if HAS_KERBEROS_FIVE
xdm_SOURCES += krb5auth.c
endif

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(xdm_SOURCES) $(XDM_LIBS)

endif LINT
