BUILT_SOURCES =
CLEANFILES =
include $(top_srcdir)/RadeonHD.am

EXTRA_DIST = README Imakefile

EXTRA_PROGRAMS = rhd_conntest
if HAVE_PCI_PCI_H
if HAVE_ZLIB
noinst_PROGRAMS = rhd_conntest
endif
endif

all-local:
	@if test "x$(noinst_PROGRAMS)" = "x"; then \
		echo "WARNING: Cannot build the rhd_conntest program due to missing dependencies."; \
		echo "         *IF* you want to build rhd_conntest, install development packages of"; \
		echo "         both pciutils and zlib/libz and re-run the configure (or autogen) script."; \
	fi

# Including config.h requires xorg-config.h, so we need the XORG_CFLAGS here
AM_CFLAGS   = @XORG_CFLAGS@ @WARN_CFLAGS@
AM_CPPFLAGS = -I$(top_srcdir)/src/AtomBios/includes

rhd_conntest_SOURCES = rhd_conntest.c
nodist_rhd_conntest_SOURCES = git_version.h
rhd_conntest_LDADD   = -lz -lpci
