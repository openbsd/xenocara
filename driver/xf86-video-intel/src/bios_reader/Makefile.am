AM_CFLAGS = @WARN_CFLAGS@ @XORG_CFLAGS@ @XMODES_CFLAGS@ @PCIACCESS_CFLAGS@

noinst_PROGRAMS = bios_reader  $(BIOS_DUMPER)

if LIBPCIACCESS
BIOS_DUMPER = bios_dumper

bios_dumper_SOURCES = bios_dumper.c

bios_dumper_LDADD = $(PCIACCESS_LIBS)

endif
