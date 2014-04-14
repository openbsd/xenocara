
lib_LTLIBRARIES = libxcb-icccm.la

xcbinclude_HEADERS = xcb_icccm.h

AM_CFLAGS = $(BASE_CFLAGS)
AM_CPPFLAGS = $(XCB_CFLAGS)

libxcb_icccm_la_SOURCES = icccm.c
libxcb_icccm_la_LIBADD = $(XCB_LIBS)
libxcb_icccm_la_LDFLAGS = -version-info 4:0:0 -no-undefined

pkgconfig_DATA = xcb-icccm.pc
