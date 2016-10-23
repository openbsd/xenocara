xenodmlibdir = $(XENODMLIBDIR)

noinst_LTLIBRARIES = libXdmGreet.la

libXdmGreet_la_SOURCES = \
		  Login.c \
		  Login.h \
		  LoginP.h \
		  greet.c \
		  verify.c

libXdmGreet_la_LIBADD = $(XDMGREET_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = $(CWARNFLAGS) $(XDMGREET_CFLAGS)

libXdmGreet_la_LDFLAGS = -static

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(libXdmGreet_la_SOURCES)

endif LINT
