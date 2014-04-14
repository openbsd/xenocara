lib_LTLIBRARIES = libxcb-util.la

AM_CPPFLAGS = $(XCB_CFLAGS)
AM_CFLAGS = $(CWARNFLAGS)

libxcb_util_la_LIBADD = $(XCB_LIBS)
libxcb_util_la_LDFLAGS = -version-info 1:0:0 -no-undefined
libxcb_util_la_SOURCES = \
	atoms.c		\
	event.c		\
	xcb_aux.c

xcbinclude_HEADERS =	\
	xcb_util.h	\
	xcb_atom.h	\
	xcb_aux.h	\
	xcb_event.h
