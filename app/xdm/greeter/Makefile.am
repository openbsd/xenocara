xdmlibdir = $(XDMLIBDIR)

if DYNAMIC_GREETER
xdmlib_LTLIBRARIES = libXdmGreet.la
else
noinst_LTLIBRARIES = libXdmGreet.la
endif

libXdmGreet_la_SOURCES = \
		  Login.c \
		  Login.h \
		  LoginP.h \
		  greet.c \
		  verify.c

libXdmGreet_la_LIBADD = $(XDMGREET_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = $(CWARNFLAGS) $(XDMGREET_CFLAGS) -DGREET_LIB

if DYNAMIC_GREETER
libXdmGreet_la_LDFLAGS = -module -avoid-version
else
libXdmGreet_la_LDFLAGS = -static
endif

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(libXdmGreet_la_SOURCES)

endif LINT
