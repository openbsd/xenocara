noinst_PROGRAMS = intel_reg_dumper

intel_reg_dumper_SOURCES = \
	main.c \
	reg_dumper.h \
	xprintf.c \
	../i830_debug.c

intel_reg_dumper_LDADD = $(PCIACCESS_LIBS)

intel_reg_dumper_CFLAGS = $(PCIACCESS_CFLAGS) $(WARN_CFLAGS) \
	-I$(srcdir)/.. -DREG_DUMPER
