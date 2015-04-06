noinst_PROGRAMS = test_xcb_image test_formats test_bitmap

if HAVE_SHM
noinst_PROGRAMS += test_xcb_image_shm
endif

check_PROGRAMS = test_swap

TESTS=test_swap

test_swap_SOURCES = test_swap.c
test_swap_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS) -I$(top_srcdir)/image
test_swap_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(top_builddir)/image/libxcb-image.la

test_xcb_image_SOURCES = test_xcb_image.c
test_xcb_image_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS) -I$(top_srcdir)/image
test_xcb_image_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(top_builddir)/image/libxcb-image.la

test_xcb_image_shm_SOURCES = test_xcb_image_shm.c
test_xcb_image_shm_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS) -I$(top_srcdir)/image
test_xcb_image_shm_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(XCB_SHM_LIBS) $(top_builddir)/image/libxcb-image.la

test_formats_SOURCES = test_formats.c
test_formats_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS) $(XPROTO_CFLAGS) -I$(top_srcdir)/image
test_formats_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(top_builddir)/image/libxcb-image.la

test_bitmap_SOURCES = test_bitmap.c test.xbm
test_bitmap_CPPFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) $(XCB_UTIL_CFLAGS)  -I$(top_srcdir)/image
test_bitmap_LDADD = $(XCB_LIBS) $(XCB_UTIL_LIBS) $(top_builddir)/image/libxcb-image.la
