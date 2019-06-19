lib_LTLIBRARIES = libXdamage.la

libXdamage_la_SOURCES = \
	xdamageint.h	\
	Xdamage.c

libXdamage_la_LIBADD = $(XDAMAGE_LIBS)
libXdamage_la_LDFLAGS = -version-number 1:1:0 -no-undefined

AM_CFLAGS = $(CWARNFLAGS) $(XDAMAGE_CFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)/include/X11/extensions

libXdamageincludedir = $(includedir)/X11/extensions
libXdamageinclude_HEADERS = $(top_srcdir)/include/X11/extensions/Xdamage.h
