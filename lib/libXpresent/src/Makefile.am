lib_LTLIBRARIES = libXpresent.la

libXpresent_la_SOURCES = Xpresent.c

libXpresent_la_LIBADD = @PRESENTEXT_LIBS@
AM_CFLAGS = $(CWARNFLAGS) @PRESENTEXT_CFLAGS@

AM_CPPFLAGS = -I$(top_srcdir)/include

libXpresent_la_LDFLAGS = -version-number 1:0:0 -no-undefined

libXpresentincludedir = $(includedir)/X11/extensions
libXpresentinclude_HEADERS = $(top_srcdir)/include/X11/extensions/Xpresent.h

