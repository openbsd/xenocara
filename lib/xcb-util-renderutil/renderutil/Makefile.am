
lib_LTLIBRARIES = libxcb-render-util.la

xcbinclude_HEADERS = xcb_renderutil.h

AM_CFLAGS = $(BASE_CFLAGS)
AM_CPPFLAGS = $(XCB_CFLAGS) $(XCB_RENDER_CFLAGS)
libxcb_render_util_la_SOURCES = cache.c util.c glyph.c
libxcb_render_util_la_LIBADD = $(XCB_LIBS) $(XCB_RENDER_LIBS)
libxcb_render_util_la_LDFLAGS = -no-undefined

pkgconfig_DATA = xcb-renderutil.pc
