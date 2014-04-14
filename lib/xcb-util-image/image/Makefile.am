MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libxcb-image.la

xcbinclude_HEADERS = xcb_image.h xcb_pixel.h xcb_bitops.h

AM_CFLAGS = $(CWARNFLAGS)

XCB_IMAGE_LIBS = libxcb-image.la

libxcb_image_la_SOURCES = xcb_image.c
libxcb_image_la_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS)
libxcb_image_la_LIBADD = $(XCB_LIBS) $(XCB_SHM_LIBS) $(XCB_UTIL_LIBS)
libxcb_image_la_LDFLAGS = -no-undefined

pkgconfig_DATA = xcb-image.pc

EXTRA_DIST=xcb-image.pc.in

noinst_PROGRAMS = test_xcb_image test_formats test_bitmap

if HAVE_SHM
noinst_PROGRAMS += test_xcb_image_shm
endif

check_PROGRAMS = test_swap

TESTS=test_swap

test_swap_SOURCES = test_swap.c
test_swap_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS)
test_swap_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(XCB_IMAGE_LIBS)

test_xcb_image_SOURCES = test_xcb_image.c
test_xcb_image_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS)
test_xcb_image_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(XCB_IMAGE_LIBS)

test_xcb_image_shm_SOURCES = test_xcb_image_shm.c
test_xcb_image_shm_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS)
test_xcb_image_shm_LDADD = $(XCB_LIBS) $(XCB_SHM_LIBS) \
	$(XCB_UTIL_LIBS) $(XCB_IMAGE_LIBS)

test_formats_SOURCES = test_formats.c
test_formats_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS)
test_formats_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(XCB_IMAGE_LIBS)

test_bitmap_SOURCES = test_bitmap.c test.xbm
test_bitmap_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS)
test_bitmap_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(XCB_IMAGE_LIBS)
