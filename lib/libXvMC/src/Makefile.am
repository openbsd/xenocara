lib_LTLIBRARIES = libXvMC.la

AM_CPPFLAGS = -I$(top_srcdir)/include $(MALLOC_ZERO_CFLAGS)
AM_CFLAGS = $(CWARNFLAGS) $(XVMC_CFLAGS)

libXvMC_la_SOURCES = XvMC.c XvMClibint.h
libXvMC_la_LIBADD = $(XVMC_LIBS)
libXvMC_la_LDFLAGS = -version-number 1:0:0 -no-undefined

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) \
                $(AM_CPPFLAGS) $(CPPFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(libXvMC_la_SOURCES) $(AM_CFLAGS) $(XVMC_LIBS)
endif LINT
