
lib_LTLIBRARIES = libxcb-ewmh.la

nodist_xcbinclude_HEADERS = xcb_ewmh.h

AM_CFLAGS = $(BASE_CFLAGS)
AM_CPPFLAGS = $(XCB_CFLAGS)

libxcb_ewmh_la_SOURCES = ewmh.c.m4 atomlist.m4 xcb_ewmh.h.m4
nodist_libxcb_ewmh_la_SOURCES = ewmh.c
libxcb_ewmh_la_LIBADD = $(XCB_LIBS)
libxcb_ewmh_la_LDFLAGS = -version-info 2:0:0 -no-undefined

pkgconfig_DATA = xcb-ewmh.pc

BUILT_SOURCES = ewmh.c xcb_ewmh.h
CLEANFILES = $(BUILT_SOURCES)

ewmh.c: $(srcdir)/ewmh.c.m4 $(srcdir)/atomlist.m4
	$(AM_V_GEN)$(M4) -I$(srcdir) $(srcdir)/ewmh.c.m4 >$@

xcb_ewmh.h: $(srcdir)/xcb_ewmh.h.m4 $(srcdir)/atomlist.m4
	$(AM_V_GEN)$(M4) -I$(srcdir) $(srcdir)/xcb_ewmh.h.m4 >$@
