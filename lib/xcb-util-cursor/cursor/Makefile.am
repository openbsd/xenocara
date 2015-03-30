
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libxcb-cursor.la

xcbinclude_HEADERS = xcb_cursor.h

AM_CFLAGS = $(CWARNFLAGS)

libxcb_cursor_la_SOURCES = cursor.c shape_to_id.c load_cursor.c parse_cursor_file.c
libxcb_cursor_la_CPPFLAGS = $(XCB_CFLAGS) $(XPROTO_CFLAGS) $(XCB_RENDER_CFLAGS) $(XCB_RENDERUTIL_CFLAGS) $(XCB_IMAGE_CFLAGS)
libxcb_cursor_la_LIBADD = $(XCB_LIBS) $(XPROTO_LIBS) $(XCB_RENDER_LIBS) $(XCB_RENDERUTIL_LIBS) $(XCB_IMAGE_LIBS)
libxcb_cursor_la_LDFLAGS = -version-info 0:0:0 -no-undefined -export-symbols-regex '^xcb_cursor_'

pkgconfig_DATA = xcb-cursor.pc

EXTRA_DIST = xcb-cursor.pc.in cursor.h shape_to_id.gperf

# TODO: We cannot use --pic because then the gperf-generated code does not compile :-/
GPERFFLAGS = --includes --struct-type --language=ANSI-C --switch=1
CLEANFILES = shape_to_id.c

# shape_to_id.c: shape_to_id.gperf
#	$(AM_V_GEN)if $(GPERF) $(GPERFFLAGS) shape_to_id.gperf >$@t; then \
	  mv $@t $@; \
	elif $(GPERF) --version >/dev/null 2>&1; then \
	  rm $@t; \
	  exit 1; \
	elif test -f $@; then \
	  rm $@t; \
	  touch $@; \
	else \
	  rm $@t; \
	  exit 1; \
	fi
