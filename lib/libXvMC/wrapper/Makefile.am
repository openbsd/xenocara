lib_LTLIBRARIES = libXvMCW.la

AM_CPPFLAGS = 					\
	-I$(top_srcdir)/include			\
	$(MALLOC_ZERO_CFLAGS)			\
	-DXVMC_CONFIGDIR=$(sysconfdir)/X11	\
	-DXVMC_SOVERSION=\".1.0\"		\
	-DXV_SOVERSION=\".1.0\"

AM_CFLAGS = $(CWARNFLAGS) $(XVMC_CFLAGS)

libXvMCW_la_SOURCES = XvMCWrapper.c
libXvMCW_la_LIBADD = $(XVMC_LIBS) $(DLOPEN_LIBS)
libXvMCW_la_LDFLAGS = -version-number 1:0:0 -no-undefined

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) \
                $(AM_CPPFLAGS) $(CPPFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(libXvMCW_la_SOURCES) $(AM_CFLAGS) $(XVMC_LIBS)
endif LINT
