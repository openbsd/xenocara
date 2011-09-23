if ENABLE_UNIT_TESTS
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include
AM_CFLAGS = $(XORG_CFLAGS) $(CWARNFLAGS)
fake_syms = fake-symbols.c fake-symbols.h

if BUILD_EVENTCOMM
noinst_PROGRAMS = eventcomm-test

eventcomm_test_SOURCES = eventcomm-test.c\
			 $(top_srcdir)/src/eventcomm.c \
			 $(fake_syms)
endif

TESTS = $(noinst_PROGRAMS)
endif
