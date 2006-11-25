# $XdotOrg: app/xdm/greeter/Makefile.am,v $

if DYNAMIC_GREETER
xdmlibdir = $(XDMLIBDIR)

xdmlib_LTLIBRARIES = libXdmGreet.la

libXdmGreet_la_SOURCES = \
		  Login.c \
		  greet.c \
		  verify.c

libXdmGreet_la_LIBADD = $(XDMGREET_LIBS)

AM_CFLAGS = $(XDMGREET_CFLAGS) -I$(top_srcdir)

libXdmGreet_la_LDFLAGS = -module -avoid-version
endif
