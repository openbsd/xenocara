lib_LTLIBRARIES = libxcb-image.la

xcbinclude_HEADERS = xcb_image.h xcb_pixel.h xcb_bitops.h

AM_CFLAGS = $(CWARNFLAGS)
AM_CPPFLAGS = 			\
	$(XCB_CFLAGS)		\
	$(XCB_SHM_CFLAGS)	\
	$(XCB_UTIL_CFLAGS)	\
	$(XPROTO_CFLAGS)

XCB_IMAGE_LIBS = libxcb-image.la

libxcb_image_la_SOURCES = xcb_image.c
libxcb_image_la_LIBADD = $(XCB_LIBS) $(XCB_SHM_LIBS) $(XCB_UTIL_LIBS)
libxcb_image_la_LDFLAGS = -no-undefined

pkgconfig_DATA = xcb-image.pc
